#include <CAN_OSI.h>
#include <string.h>
#include "CRC/CRC.h"
#include "CAN_Flag.h"
#include <stdlib.h>
#include "CAN_OSI.h"
#include "main.h"
//#include "can.h"
//#include "dma.h"
extern DMA_HandleTypeDef hdma_memtomem_dma1_channel1;
extern CAN_HandleTypeDef hcan;

void CAN_Receive_Error_Handle(FlagRecNotification *FlagNoti,
    FlagFrameHandle *FlagHandle) {
  if (*FlagNoti == REC_FRAMEDATA_ERROR) {
    for (uint8_t FrameType = 0; FrameType < FlagHandle->NumberOfFrame;
        FrameType++) {
      if (FlagHandle->FlagID[FlagHandle->ID].FrameError[FrameType] == 1) {
        CAN_Send_Response(FlagHandle->ID, FRAME_ERROR, FrameType);
      }
    }
  }
}
uint16_t CAN_Send_Response(uint8_t ID, uint8_t Opcode, uint8_t FrameType) {
  CAN_TxHeaderTypeDef TxHeader;
  uint32_t Txmailbox;
  uint8_t OpcodeData[8] = { Opcode, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55 };
  uint8_t StID = 0x00;
  StID |= ID;
  StID = (StID << 3) | FrameType;
  CAN_TXHeaderConfig(&TxHeader, StID);
  if (HAL_CAN_AddTxMessage(&hcan, &TxHeader, OpcodeData, &Txmailbox)
      != HAL_OK) {
    Error_Handler();
  }
  while (HAL_CAN_IsTxMessagePending(&hcan, Txmailbox))
    ;
  return HAL_OK;
}

void CAN_Receive_Response(CAN_RxHeaderTypeDef *RxHeader, uint8_t *Data) {
  if (HAL_CAN_GetRxFifoFillLevel(&hcan, CAN_RX_FIFO0) > 0) {
    if (HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO0, RxHeader, Data) != HAL_OK) {
      Error_Handler(); //get message from RAM;
    }
  }
}
void CAN_Send_Error_Handle(CANBufferHandleStruct *Buffer,
    CANConfigIDTxtypedef *pIDType, CAN_RxHeaderTypeDef *RxHeader, uint8_t *Data) {
  CAN_TxHeaderTypeDef TxHeader;
  uint32_t Txmailbox;
  CAN_Receive_Response(RxHeader, Data);
  uint8_t StId = 0;
  StId = (StId << 3) | Buffer->FrameType_Index;
  StId = (Buffer->SenderID << 3) | Data[1];
  if (Data[0] == FRAME_ERROR) {
    if (HAL_CAN_AddTxMessage(&hcan, &TxHeader, Buffer->Buffer[Data[1]],
        &Txmailbox) != HAL_OK) {
      Error_Handler();
    }
    while (HAL_CAN_IsTxMessagePending(&hcan, Txmailbox))
      ;
  }
}
void CAN_ProcessRxBuffer(FlagFrameHandle *FlagHandle, uint8_t ID,
    CANBufferHandleStruct *RxBuffer, uint8_t *DataPhysical,
    FlagRecNotification *FlagRecHandle) {
  uint8_t FrameType = 0;
  RxBuffer->NodeHandle[ID].NodeIndex++;
  for (; FrameType < RxBuffer->NodeHandle[ID].NumberOfFrame; FrameType++) {
    CAN_ProcessFrame(FlagHandle, ID, RxBuffer, FrameType, DataPhysical);
  }
  if (RxBuffer->NodeHandle[ID].NodeIndex
      == RxBuffer->NodeHandle[ID].NumberOfFrame) {
    if (FlagHandle->FlagID[ID].SumOfFlag
        == RxBuffer->NodeHandle[ID].NumberOfFrame) {
      *FlagRecHandle = REC_FRAMEDATA_SUCCESS;
      RxBuffer->NodeHandle[ID].NodeIndex = 0;
      RxBuffer->NodeHandle[ID].DuplicateFrame = 0;
      FlagHandle->FlagID[ID].SumOfFlag = 0;
      for (FrameType = 0; FrameType < RxBuffer->NodeHandle[ID].NumberOfFrame;
          FrameType++) {
        FlagHandle->FlagID[ID].FlagFrameFull[FrameType] = 0;
      }
    } else {
      *FlagRecHandle = REC_FRAMEDATA_ERROR;
      RxBuffer->NodeHandle[ID].NodeIndex = 0;
      RxBuffer->NodeHandle[ID].DuplicateFrame = 0;
      FlagHandle->FlagID[ID].SumOfFlag = 0;
      CAN_Receive_Error_Handle(FlagRecHandle, FlagHandle);
      for (FrameType = 0; FrameType <= RxBuffer->NodeHandle[ID].NumberOfFrame;
          FrameType++) {
        if (FlagHandle->FlagID[ID].FlagFrameFull[FrameType] == 0) {
          FlagHandle->FlagID[ID].FrameError[FrameType] = 1;
          FlagHandle->ID = ID;
          FlagHandle->NumberOfFrame = RxBuffer->NodeHandle[ID].NumberOfFrame;
        }
      }
    }
  }
}

void CAN_ProcessFrame(FlagFrameHandle *FlagHandle, uint8_t ID,
    CANBufferHandleStruct *RxBuffer, uint8_t FrameType, uint8_t *Data) {
  if (RxBuffer->NodeHandle[ID].FrameType == FrameType
      && FlagHandle->FlagID[ID].FlagFrameFull[FrameType] == 0) {
    HAL_DMA_Start(&hdma_memtomem_dma1_channel1, (uint32_t) Data,
        (uint32_t) RxBuffer->NodeHandle[ID].NodeBuffer[RxBuffer->NodeHandle[ID].FrameType],
        CAN_MAX_DATA);
    while (HAL_DMA_PollForTransfer(&hdma_memtomem_dma1_channel1,
        HAL_DMA_FULL_TRANSFER, 1) != HAL_OK) {
        __NOP();
    }
    FlagHandle->FlagID[ID].FlagFrameFull[FrameType] = 1;
    FlagHandle->FlagID[ID].SumOfFlag +=
        FlagHandle->FlagID[ID].FlagFrameFull[FrameType];
  }
}

uint8_t CAN_Send_Application(CANBufferHandleStruct *AppBuffer,
    CANConfigIDTxtypedef *pStID, uint8_t *Data, uint8_t DataLength) {
  return CAN_Send_Network_Packet(AppBuffer, Data, DataLength, pStID);
}

uint8_t CAN_Send_Network_Packet(CANBufferHandleStruct *TxBuffer, uint8_t *Data,
    uint8_t DataLength, CANConfigIDTxtypedef *pStID) {
  TxBuffer->PacketDataLength = DataLength + 2;
    TxBuffer->CRCValue = crc_8(Data, DataLength);
    TxBuffer->Buffer_Index = DataLength;
    if (TxBuffer->PacketDataLength % 8 == 0) {
      TxBuffer->NumberOfFrame = (TxBuffer->PacketDataLength / 8);
    } else {
      TxBuffer->NumberOfFrame = (TxBuffer->PacketDataLength / 8) + 1;
    }
    HAL_DMA_Start(&hdma_memtomem_dma1_channel1, (uint32_t) Data,
        (uint32_t) TxBuffer->NetworkBuffer, DataLength);
    while (HAL_DMA_PollForTransfer(&hdma_memtomem_dma1_channel1,
        HAL_DMA_FULL_TRANSFER, 1) != HAL_OK) {
      __NOP();
    }
    TxBuffer->NetworkBuffer[TxBuffer->Buffer_Index] = TxBuffer->PacketDataLength;
    TxBuffer->NetworkBuffer[TxBuffer->Buffer_Index + 1] = TxBuffer->CRCValue;
    TxBuffer->Buffer_Index = 0;
  return CAN_Send_DataLink_Separate(TxBuffer, Data, pStID);
}
uint8_t CAN_Send_DataLink_Separate(CANBufferHandleStruct *TxBuffer,
    uint8_t *Data, CANConfigIDTxtypedef *pStID) {
  uint8_t PacketLength = TxBuffer->PacketDataLength;
  uint8_t NumberOfFrame = TxBuffer->NumberOfFrame;
  TxBuffer->Buffer[NumberOfFrame - 1][6] = PacketLength;
  TxBuffer->Buffer[NumberOfFrame - 1][7] = TxBuffer->CRCValue;
  for (int i = 0; i < NumberOfFrame; i++) {
    for (TxBuffer->Buffer_Index = 0; TxBuffer->Buffer_Index < 8;
        TxBuffer->Buffer_Index++) {
      TxBuffer->Buffer[i][TxBuffer->Buffer_Index] = TxBuffer->NetworkBuffer[i
          * 8 + TxBuffer->Buffer_Index];
      PacketLength--;
      if (PacketLength == 2) {
        break;
      }
    }
    if (PacketLength == 2) {
      break;
    }
  }
  TxBuffer->Buffer_Index = 0;
  return CAN_Send_Physical_Send(TxBuffer, Data, pStID);
}
uint8_t CAN_Send_Physical_Send(CANBufferHandleStruct *TxBuffer, uint8_t *Data,
    CANConfigIDTxtypedef *pIDtype) {
  uint32_t Txmailbox;
  CAN_TxHeaderTypeDef Txheader;
  uint8_t Message_ID = pIDtype->MessageType;
  uint8_t Sender_ID = pIDtype->SenderID;
  uint8_t FrameType = TxBuffer->FrameType_Index;
  uint8_t NumberOfFrame = TxBuffer->NumberOfFrame;
  uint16_t StdId = 0x00;

  StdId |= Message_ID;
  StdId = (StdId << 4) | Sender_ID;
  TxBuffer->SenderID = StdId;
  StdId = (StdId << 3) | TxBuffer->FrameType_Index;
  Txheader.DLC = 8;
  Txheader.RTR = CAN_RTR_DATA;
  Txheader.IDE = CAN_ID_STD;

  for (int8_t i = NumberOfFrame - 1; i >= 0; i--) {
    Txheader.StdId = StdId;
    if (HAL_CAN_AddTxMessage(&hcan, &Txheader, TxBuffer->Buffer[i], &Txmailbox)
        != HAL_OK) {
      Error_Handler();
    }
    while (HAL_CAN_IsTxMessagePending(&hcan, Txmailbox))
      ;

    StdId = StdId >> 3;
    FrameType++;
    StdId = (StdId << 3) | FrameType;

  }
  return HAL_OK;
}

//uint8_t CAN_Recieve_Physical_FIFO0(CAN_RxHeaderTypeDef *RxHeader, uint8_t *Data1, uint8_t *Data2) {
//
//  if (HAL_CAN_GetRxFifoFillLevel(&hcan, CAN_RX_FIFO0)>0)
//  {
//  if (HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO0, RxHeader, Data1) != HAL_OK) {
//    HAL_CAN_ResetError(&hcan);
//  }
//  }
//  if(HAL_CAN_GetRxFifoFillLevel(&hcan, CAN_RX_FIFO1)>0)
//    {
//    if (HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO1, RxHeader, Data2) != HAL_OK) {
//        HAL_CAN_ResetError(&hcan);
//      }
//    }
//  return HAL_OK;
//}

void CAN_Recieve_Physical_FIFO0(CAN_RxHeaderTypeDef *RxHeader, uint8_t *Data) {
	while (HAL_CAN_GetRxFifoFillLevel(&hcan, CAN_RX_FIFO0) == 0)
		;
	if (HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO0, RxHeader, Data) != HAL_OK) {
	  HAL_CAN_ResetError(&hcan);
	}
}

uint8_t CAN_Receive_DataLink(FlagFrameHandle *FlagHandle,
    CANBufferHandleStruct *RxBuffer, FlagRecNotification *FlagNotiHandle) {
  CAN_RxHeaderTypeDef RxHeader;
  *FlagNotiHandle = REC_DATA;
  uint8_t DataPhysical[CAN_MAX_DATA] = { 0 }; // init local DataPhysical for get data from receive
  uint16_t StdID = 0;
  uint8_t ID = 0;
  CAN_Recieve_Physical_FIFO0(&RxHeader, DataPhysical);
  StdID = RxHeader.StdId;
  ID = (StdID >> 3) & 15;
  RxBuffer->RecvID = ID;
  RxBuffer->NodeHandle[ID].FrameType = StdID & 7; // get frame type store into Rxbuffer struct with Node ID manage frame type
  if (RxBuffer->NodeHandle[ID].FrameType == SET_UP_FRAME
      && RxBuffer->NodeHandle[ID].DuplicateFrame != 1) { // check if frame type = SET_UP_FRAME
    RxBuffer->NodeHandle[ID].DuplicateFrame = 1; // check send multiple SET_UP_frame
    RxBuffer->NodeHandle[ID].PacketLength = DataPhysical[6];
    RxBuffer->NodeHandle[ID].CRCValue = DataPhysical[7];
      RxBuffer->NodeHandle[ID].NumberOfFrame =
          (RxBuffer->NodeHandle[ID].PacketLength +7)/8;
  } else {
    if (RxBuffer->NodeHandle[ID].FrameType == SET_UP_FRAME) {
      *FlagNotiHandle = REC_FRAMEDATA_ERROR;
      CAN_Receive_Error_Handle(FlagNotiHandle, FlagHandle);
      FlagHandle->FlagID[ID].FrameError[RxBuffer->NodeHandle[ID].FrameType] = 1;
    }
  }
  CAN_ProcessRxBuffer(FlagHandle, ID, RxBuffer, DataPhysical, FlagNotiHandle);
  return HAL_OK;
}
uint8_t CAN_Receive_Network(CANBufferHandleStruct *NetBuffer,
    FlagFrameHandle *NetworkFlag, FlagRecNotification *FlagNotiHandle) {
  CAN_Receive_DataLink(NetworkFlag, NetBuffer, FlagNotiHandle);
  uint8_t FrameLength = 0;
  uint8_t FrameType = 0;
  uint8_t NetBufferIndex = 0;
  uint8_t DataLength = 0;
  uint8_t CRCValue = 0;
  uint8_t *NetData;
  FrameLength = NetBuffer->NodeHandle[NetBuffer->RecvID].NumberOfFrame;
  uint8_t NumberofFrame = FrameLength;
  FrameType = NetBuffer->NodeHandle[NetBuffer->RecvID].FrameType;
  if (*FlagNotiHandle == REC_FRAMEDATA_SUCCESS) {
    for (; FrameLength > 0; FrameLength--) {
      HAL_DMA_Start(&hdma_memtomem_dma1_channel1,
          (uint32_t) NetBuffer->NodeHandle[NetBuffer->RecvID].NodeBuffer[FrameType],
          (uint32_t) NetBuffer->Buffer[NetBufferIndex], CAN_MAX_DATA);
      while (HAL_DMA_PollForTransfer(&hdma_memtomem_dma1_channel1,
          HAL_DMA_FULL_TRANSFER, 1) != HAL_OK) {
          __NOP();
      }
      NetBufferIndex++;
      FrameType--;
    }
    DataLength = NetBuffer->NodeHandle[NetBuffer->RecvID].PacketLength - 2;
    NetData = (uint8_t*) pvPortMalloc(DataLength * sizeof(uint8_t));
    for (NetBufferIndex = 0; NetBufferIndex <= NumberofFrame;
        NetBufferIndex++) {
      for (int j = 0; j < 8; j++) {
        NetData[NetBufferIndex * 8 + j] = NetBuffer->Buffer[NetBufferIndex][j];
      }
    }
    CRCValue = crc_8(NetData, DataLength);
    if (CRCValue == NetBuffer->NodeHandle[NetBuffer->RecvID].CRCValue) {
      *FlagNotiHandle = REC_PACKET_SUCCESS;
      HAL_DMA_Start(&hdma_memtomem_dma1_channel1, (uint32_t) NetData,
          (uint32_t) NetBuffer->NetworkBuffer, DataLength);
      while (HAL_DMA_PollForTransfer(&hdma_memtomem_dma1_channel1,
          HAL_DMA_FULL_TRANSFER, 1) != HAL_OK) {
          __NOP();
      }
    } else {
      *FlagNotiHandle = REC_PACKET_ERROR;
    }
    free(NetData);
  }
  return HAL_OK;
}

uint8_t CAN_Receive_Application(CANBufferHandleStruct *AppBuffer, uint8_t *Data,
    FlagFrameHandle *FlagFrame, FlagRecNotification *FlagNotification) {
  uint8_t AppDataLength = AppBuffer->NodeHandle[AppBuffer->RecvID].PacketLength
      - 2;
  CAN_Receive_Network(AppBuffer, FlagFrame, FlagNotification);
  if (*FlagNotification == REC_PACKET_SUCCESS) {
    HAL_DMA_Start(&hdma_memtomem_dma1_channel1,
        (uint32_t) AppBuffer->NetworkBuffer, (uint32_t) Data, AppDataLength);
    while (HAL_DMA_PollForTransfer(&hdma_memtomem_dma1_channel1,
        HAL_DMA_FULL_TRANSFER, 1) != HAL_OK) {
        __NOP();
    }

    *FlagNotification = REC_SUCCESS;
  }
  return HAL_OK;
}

//void CAN_Config_filtering(uint8_t FillterBank , uint16_t NodeID, uint8_t FiFo)
//{
//  CAN_FilterTypeDef Can_filter_init;
//  Can_filter_init.FilterActivation=ENABLE;
//  Can_filter_init.FilterBank=FillterBank;
//  Can_filter_init.FilterFIFOAssignment=FiFo;
//  Can_filter_init.FilterIdHigh=NodeID<<8;
//  Can_filter_init.FilterIdLow= 0x0000;
//  Can_filter_init.FilterMaskIdHigh= 0x0F00;
//  Can_filter_init.FilterMaskIdLow= 0x0000;
//  Can_filter_init.FilterMode=CAN_FILTERMODE_IDMASK;
//  Can_filter_init.FilterScale=CAN_FILTERSCALE_32BIT;
//  if(HAL_CAN_ConfigFilter(&hcan,&Can_filter_init)!=HAL_OK)
//  {
//    Error_Handler();
//  }
//}
uint32_t CAN_Config_filtering(uint8_t FIFO) {
  CAN_FilterTypeDef Can_filter_init;
  Can_filter_init.FilterActivation = ENABLE;
  Can_filter_init.FilterBank = 0;
  Can_filter_init.FilterFIFOAssignment = FIFO;
  Can_filter_init.FilterIdHigh = 0x0000;
  Can_filter_init.FilterIdLow = 0x0000;
  Can_filter_init.FilterMaskIdHigh = 0x0000;
  Can_filter_init.FilterMaskIdLow = 0x0000;
  Can_filter_init.FilterMode = CAN_FILTERMODE_IDMASK;
  Can_filter_init.FilterScale = CAN_FILTERSCALE_32BIT;
  if (HAL_CAN_ConfigFilter(&hcan, &Can_filter_init) != HAL_OK) {
    Error_Handler();
  }
  return HAL_OK;
}
