#include "fdcan_app.h"

FDCAN_STATUS FDCAN_addRxCANIdAndInitilize(void);

fdcan_app_t fdcan = {
            .RxCANIds = {
                          FDCAN_RxID_500,
                          FDCAN_RxID_705,
                          FDCAN_RxID_1200,
                        },
            .addRxCANIdAndInitilize = FDCAN_addRxCANIdAndInitilize,
};


FDCAN_STATUS FDCAN_addRxCANIdAndInitilize(void) {
    FDCAN_STATUS initStatus = FDCAN_OK;
    if(__fdcan.initialize(FDCAN1_PORT, fdcan.RxCANIds) != FDCAN_OK) initStatus = FDCAN_ERROR;
    return initStatus;
}