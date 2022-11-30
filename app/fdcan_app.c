#include "fdcan_app.h"

FDCAN_STATUS FDCAN_addRxCANIdAndInitialize(void);
FDCAN_STATUS FDCAN_writeMessages(FDCAN_HandleTypeDef*, uint32_t, uint8_t* , uint8_t );


fdcan_app_t fdcan = {
            .RxCANIds = {
                          FDCAN_RxID_500,
                          FDCAN_RxID_705,
                          FDCAN_RxID_1200,
                        },
            .addRxCANIdAndInitialize = FDCAN_addRxCANIdAndInitialize,
            .writeMessages        = FDCAN_writeMessages,
};


static FDCAN_IDType __fdcan_getIdType(uint32_t id) {
       FDCAN_IDType idType;
       if(FDCAN_STANDARDID_MAX_SIZE >= id) idType = StandardId;
       else idType = ExtendedId;
       return idType;
}


FDCAN_STATUS FDCAN_addRxCANIdAndInitialize(void) {
     FDCAN_STATUS initStatus = FDCAN_OK;
     if(__fdcan.initialize(FDCAN1_PORT, fdcan.RxCANIds) != FDCAN_OK) initStatus = FDCAN_ERROR;
     return initStatus;
}


FDCAN_STATUS FDCAN_writeMessages(FDCAN_HandleTypeDef* port, uint32_t id, uint8_t* messages, uint8_t size) {
     FDCAN_STATUS writeStatus = FDCAN_OK;
     if(__fdcan.transmitMessages(port, id, __fdcan_getIdType(id), messages, size) != FDCAN_OK) writeStatus = FDCAN_ERROR;
     return writeStatus;
}