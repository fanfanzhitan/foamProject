
#pragma once

#if(MCU_CORE_TYPE == MCU_CORE_8266)
#include "pm_8266.h"
#elif(MCU_CORE_TYPE == MCU_CORE_8366)
#include "pm_8366.h"
#elif(MCU_CORE_TYPE == MCU_CORE_8368)
#include "pm_8368.h"
#endif

