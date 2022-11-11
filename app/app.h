#ifndef __APP_H
#define __APP_H

#include "main.h"
#include "fdcan_app.h"


typedef struct app_t{
        void      (*error)(void);
        void      (*delay)(uint32_t Delay);
        uint32_t  (*getCurrentTimeinMs)(void);
}app_t;

extern app_t app;

#endif