#ifndef __FDCAN_APP_H
#define __FDCAN_APP_H

#include <stdio.h>
#include <stdlib.h>
#include "fdcan_drv.h"


#define FDCAN_RxID_500        0x500
#define FDCAN_RxID_705        0x705
#define FDCAN_RxID_1200       0x1200
#define FDCAN_MAX_RxID_SIZE   3

typedef struct fdcan_app_t{
        uint32_t RxCANIds[FDCAN_MAX_RxID_SIZE];
        FDCAN_STATUS (*addRxCANIdAndInitialize)(void);
        FDCAN_STATUS (*writeMessages)(FDCAN_HandleTypeDef* port, uint32_t id, uint8_t* messages, uint8_t size);
}fdcan_app_t;

extern fdcan_app_t fdcan;

#endif