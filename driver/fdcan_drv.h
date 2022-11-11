#ifndef __FDCAN_DRV_H
#define __FDCAN_DRV_H

#include <stdbool.h>
#include "fdcan.h"



#define FDCAN1_ENABLED 
#define FDCAN1_PORT                 &hfdcan1
#define FDCAN2_PORT                 &hfdcan2
#define FDCAN_MAX_DLC               8  
#define FDCAN_STANDARDID_MAX_SIZE   0x7FF


typedef enum{
    	ExtendedId 	  = 0,
	    StandardId	  = 1,
}FDCAN_IDType;

typedef enum{
	FIFO0_FDCAN = 0,
	FIFO1_FDCAN = 1,
}FIFOs_FDCAN;

typedef enum{
	// FDCAN1 FilterBanks 
	FBANK0	    = 0,
	FBANK1	    = 1,
	FBANK2	    = 2,
	FBANK3	    = 3,
	FBANK4	    = 4,
	FBANK5	    = 5,
	FBANK6	    = 6,
	FBANK7	    = 7,
	FBANK8	    = 8,
	FBANK9	    = 9,
	FBANK10	    = 10,
	FBANK11	    = 11,
	FBANK12	    = 12,
	FBANK13	    = 13,

    // FDCAN2 FilterBanks 
	FBANK14		= 14,
	FBANK15		= 15,
	FBANK16		= 16,
	FBANK17		= 17,
	FBANK18		= 18,
	FBANK19		= 19,
	FBANK20		= 20,
	FBANK21		= 21,
	FBANK22		= 22,
	FBANK23		= 23,
	FBANK24		= 24,
	FBANK25		= 25,
	FBANK26		= 26,
	FBANK27		= 27,
}FDCAN_FilterBanks;

typedef enum {
     DLC1       = 1,
	 DLC2,       
	 DLC3,
	 DLC4,
	 DLC5,
	 DLC6, 
     DLC7, 
	 DLC8, 
}FDCAN_DataLength;


typedef enum {
       FDCAN_OK,
       FDCAN_ERROR,
       FDCAN_TIMEOUT,
       FDCAN_AVAILABLE,
}FDCAN_STATUS;

typedef struct fdcan_drv_t {
	    uint8_t   messageToBeTransmittedBuffer[FDCAN_MAX_DLC];
		uint8_t   messageReceivedBuffer[FDCAN_MAX_DLC];
        FDCAN_STATUS    (*initialize) (FDCAN_HandleTypeDef *hfdcan, uint32_t* CANIDs);
        void (*transfer)(FDCAN_HandleTypeDef *hfdcan, uint32_t transmitCANId, FDCAN_IDType idType, uint8_t* messages, uint8_t DLC);
}fdcan_drv_t;

extern fdcan_drv_t __fdcan;

#endif