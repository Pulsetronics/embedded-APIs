#include "fdcan_drv.h"

FDCAN_STATUS FDCAN_initialize(FDCAN_HandleTypeDef*, uint32_t*);
void FDCAN_dataTransfer(FDCAN_HandleTypeDef*, uint32_t, FDCAN_IDType, uint8_t*, uint8_t);

fdcan_drv_t __fdcan = {
        .initialize = FDCAN_initialize,
	 .transfer   = FDCAN_dataTransfer,
};

/*Custom Function Prototypes Start*/

static void __fdcan_configIdType(FDCAN_TxHeaderTypeDef* txMsg, FDCAN_IDType idType) {
       if(ExtendedId == idType)
	   txMsg->IdType = FDCAN_EXTENDED_ID;
	else if(StandardId == idType) 
	   txMsg->IdType = FDCAN_STANDARD_ID;
}

static void __fdcan_filterIdTypeIsExtended (FDCAN_FilterTypeDef* filterConfig, uint32_t receiveCANId, FDCAN_IDType idType) {
	filterConfig->IdType    = FDCAN_EXTENDED_ID;
	filterConfig->FilterID1 = (receiveCANId  & 0x1FFFE000)>>13;
	filterConfig->FilterID2 = ((receiveCANId & 0x00001FFF)<<3)|0x04;
}

static void __fdcan_filterIdTypeIsStandard (FDCAN_FilterTypeDef* filterConfig, uint32_t receiveCANId, FDCAN_IDType idType) {
	filterConfig->IdType    = FDCAN_STANDARD_ID;
	filterConfig->FilterID1 = receiveCANId;
	filterConfig->FilterID2 = 0x7FF;
}

static void __fdcan_filterConfigIdType (FDCAN_FilterTypeDef* filterConfig, uint32_t receiveCANId, FDCAN_IDType idType) {
       if(idType == ExtendedId) 
          __fdcan_filterIdTypeIsExtended(filterConfig, receiveCANId, idType);
	else if(idType == StandardId)
	   __fdcan_filterIdTypeIsStandard(filterConfig, receiveCANId, idType);

}

static void __fdcan_setFilterConfigFIFO (FDCAN_FilterTypeDef* filterConfig, FIFOs_FDCAN fifo) {
       if(fifo == FIFO0_FDCAN) 
          filterConfig->FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
	else if(fifo == FIFO1_FDCAN) 
	   filterConfig->FilterConfig = FDCAN_FILTER_TO_RXFIFO1;
}

static FDCAN_STATUS __fdcan_setupAsStandardId(FDCAN_HandleTypeDef *hfdcan, uint32_t CANId, uint16_t filterBank) {
       FDCAN_STATUS setupStatus = FDCAN_OK;
       if(__fdcan_filterConfig(hfdcan, CANId, StandardId, FBANK0+filterBank, FIFO0_FDCAN) != FDCAN_OK) 
          setupStatus = FDCAN_ERROR;
       return setupStatus;
}

static FDCAN_STATUS __fdcan_setupAsExtendedId(FDCAN_HandleTypeDef *hfdcan, uint32_t CANId, uint16_t filterBank) {
       FDCAN_STATUS setupStatus = FDCAN_OK;
       if(__fdcan_filterConfig(hfdcan, CANId, ExtendedId, FBANK14+filterBank, FIFO0_FDCAN) != FDCAN_OK) 
          setupStatus = FDCAN_ERROR;
       return setupStatus;
}

static FDCAN_STATUS __fdcan_setupStandardAndExtendedIds (FDCAN_HandleTypeDef *hfdcan, uint32_t* CANIDs) {
       uint16_t addressPointer  = 0, FBBank_ExtdIncr = 0, FBBank_StdIncr = 0;
       FDCAN_STATUS setupStatus = FDCAN_OK;
       while(CANIDs[addressPointer]>0) {
          if(CANIDs[addressPointer] <= FDCAN_STANDARDID_MAX_SIZE)
               if(__fdcan_setupAsStandardId(hfdcan, CANIDs[addressPointer++], FBBank_StdIncr++)  != FDCAN_OK) setupStatus = FDCAN_ERROR;
          else if(__fdcan_setupAsExtendedId(hfdcan, CANIDs[addressPointer++], FBBank_ExtdIncr++) != FDCAN_OK) setupStatus = FDCAN_ERROR;      
       }
       return setupStatus;
}

static FDCAN_STATUS __fdcan_filterConfig(FDCAN_HandleTypeDef* hfdcan, uint32_t receiveCANId, FDCAN_IDType idType, 
                                 FDCAN_FilterBanks filterbank, FIFOs_FDCAN fifo) {
	FDCAN_FilterTypeDef filterConfig;
       FDCAN_STATUS configStatus = FDCAN_OK;

	filterConfig.FilterType   = FDCAN_FILTER_MASK;
       filterConfig.FilterIndex  = filterbank;
       __fdcan_filterConfigIdType(&filterConfig, receiveCANId, idType);
	__fdcan_filterConfigFIFO(&filterConfig, fifo);

	if(HAL_FDCAN_ConfigFilter(hfdcan, &filterConfig) != HAL_OK) configStatus = FDCAN_ERROR;
       if(HAL_FDCAN_ConfigGlobalFilter(hfdcan,   FDCAN_REJECT, FDCAN_REJECT, FDCAN_REJECT_REMOTE, 
                                                 FDCAN_REJECT_REMOTE)!= HAL_OK) configStatus = FDCAN_ERROR;
       if(HAL_FDCAN_ActivateNotification(hfdcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0)!= HAL_OK) 
          configStatus = FDCAN_ERROR;
       return configStatus;
}

void FDCAN_dataTransfer(FDCAN_HandleTypeDef *hfdcan, uint32_t transmitCANId, FDCAN_IDType idType, 
                        uint8_t* messages, uint8_t DLC) {
     FDCAN_TxHeaderTypeDef txMsg;
     const uint32_t fdcan_dlcBytesPosition = 24;

     __fdcan_configIdType(&txMsg, idType);
     txMsg.Identifier           = transmitCANId;
     txMsg.TxFrameType          = FDCAN_DATA_FRAME;
     txMsg.DataLength           = DLC<<fdcan_dlcBytesPosition;
     txMsg.ErrorStateIndicator  = FDCAN_ESI_ACTIVE;
     txMsg.BitRateSwitch        = FDCAN_BRS_ON;
     txMsg.FDFormat             = FDCAN_FD_CAN;
     txMsg.TxEventFifoControl   = FDCAN_NO_TX_EVENTS;
     HAL_CAN_AddTxMessage(hfdcan, &txMsg, messages);
}

FDCAN_STATUS FDCAN_initialize(FDCAN_HandleTypeDef *hfdcan, uint32_t* CANIDs) {
     FDCAN_STATUS initStatus = FDCAN_OK;

#ifdef FDCAN1_ENABLED
	   MX_FDCAN1_Init();
#endif

#ifdef FDCAN2_ENABLED
       MX_FDCAN2_Init();
#endif

     __fdcan_setupStandardAndExtendedIds(hfdcan, CANIDs);  
     if(HAL_FDCAN_Start(hfdcan) != HAL_OK) initStatus = FDCAN_ERROR;
     return initStatus;
}

void HAL_CAN_RxFifo0MsgPendingCallback(FDCAN_HandleTypeDef *hfdcan) {
     FDCAN_RxHeaderTypeDef rxMessageBuffer;  
     if (hfdcan == FDCAN1_PORT)
         HAL_CAN_GetRxMessage(FDCAN1_PORT, FDCAN_RX_FIFO0, &rxMessageBuffer, (uint8_t*)__fdcan.messageReceivedBuffer);
}
