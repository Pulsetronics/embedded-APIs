#include "app.h"


app_t app = {
      .error              = Error_Handler,
      .delay              = HAL_Delay,
      .getCurrentTimeinMs = HAL_GetTick,
};


