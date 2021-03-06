
#pragma once


enum{
	GPIO_DIR_IN 	= 0,
	GPIO_DIR_OUT	= 1
};

// do not use enum,  because we use this in preprocessor diretive,  #if
#define AS_GPIO		0
#define AS_MSPI		1
#define AS_SWIRE	2
#define AS_UART		3
#define AS_PWM		4
#define AS_I2C		5
#define AS_SPI		6
#define AS_ETH_MAC	7
#define AS_I2S		8
#define AS_SDM		9
#define AS_DMIC		10
#define AS_USB		11
#define AS_SWS		12
#define AS_SWM		13
#define AS_TEST		14

#include "../common/static_assert.h"

#if(MCU_CORE_TYPE == MCU_CORE_5320)
#include "../mcu_spec/gpio_default_5320.h"
#include "../mcu_spec/gpio_5320.h"
#elif(MCU_CORE_TYPE == MCU_CORE_5328)
#include "../mcu_spec/gpio_default_5328.h"
#include "../mcu_spec/gpio_5328.h"
#elif(MCU_CORE_TYPE == MCU_CORE_5330)
#include "../mcu_spec/gpio_default_5330.h"
#include "../mcu_spec/gpio_5330.h"
#elif(MCU_CORE_TYPE == MCU_CORE_5332)
#include "../mcu_spec/gpio_default_5332.h"
#include "../mcu_spec/gpio_5332.h"
#elif(MCU_CORE_TYPE == MCU_CORE_3520)
#include "../mcu_spec/gpio_default_3520.h"
#include "../mcu_spec/gpio_3520.h"
#endif

