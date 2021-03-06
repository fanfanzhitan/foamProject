
#pragma once

#define	CHIP_PACKAGE_QFN64  1
#define	CHIP_PACKAGE_QFN48  2
#define	CHIP_PACKAGE_QFN40  3
#define	CHIP_PACKAGE_QFN32  4

#ifndef CHIP_PACKAGE
#define	CHIP_PACKAGE 		CHIP_PACKAGE_QFN64
#endif

#define	CHIP_TYPE_8870  	1   //5320
#define	CHIP_TYPE_8869 		2   //5320
#define	CHIP_TYPE_8868 		3   //5320
#define	CHIP_TYPE_8669 		4   //5328
#define	CHIP_TYPE_8666 		5   //5328
#define	CHIP_TYPE_8566 		6   //5330
#define	CHIP_TYPE_8568 		7   //5332
#define	CHIP_TYPE_3520 		20

#ifndef CHIP_TYPE
#define	CHIP_TYPE 			CHIP_TYPE_8566
//#define	CHIP_TYPE 			CHIP_TYPE_8870
#endif

#define	MCU_CORE_5320 		1
#define	MCU_CORE_5328 		2
#define	MCU_CORE_5330 		3
#define	MCU_CORE_5332 		4
#define	MCU_CORE_5322 		5
#define	MCU_CORE_3520 		20

#if(CHIP_TYPE == CHIP_TYPE_8870 || CHIP_TYPE == CHIP_TYPE_8869 || CHIP_TYPE == CHIP_TYPE_8868)
	#define MCU_CORE_TYPE	MCU_CORE_5320
#elif(CHIP_TYPE == CHIP_TYPE_8566)
	#define MCU_CORE_TYPE	MCU_CORE_5330
#elif(CHIP_TYPE == CHIP_TYPE_8568)
	#define MCU_CORE_TYPE	MCU_CORE_5332
#elif(CHIP_TYPE == CHIP_TYPE_3520)
	#define MCU_CORE_TYPE	MCU_CORE_3520
#else
	#define MCU_CORE_TYPE	MCU_CORE_5328
#endif


