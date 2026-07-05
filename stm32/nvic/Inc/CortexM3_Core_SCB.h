/*
 * CortexM3_Core_SCB.h
 *
 *  Created on: May 18, 2026
 *      Author: Adham
 */

#ifndef CORTEXM3_CORE_SCB_H_
#define CORTEXM3_CORE_SCB_H_

/* ---------------------------------------- Includes Start ---------------------------------------- */

#include "Std_Types.h"

/* ---------------------------------------- Includes End ------------------------------------------ */


/* ---------------------------------------- Macro Declaration Start ------------------------------- */

/* Memory Mapping of the Core Hardware */
#define SCB_BASE	(0xE000ED00UL)
#define SCB			((SCB_Type*)SCB_BASE)			// Pointer to the base address of the SCB registers memory block

/** @defgroup CORTEX_PREEMPTION_Priority_Group
 *
 */
#define SCB_PRIORITY_GROUP_0			0x00000007U		// 0 bits for preemption priority & 4 bits for sub priority
#define SCB_PRIORITY_GROUP_1			0x00000006U		// 1 bits for preemption priority & 3 bits for sub priority
#define SCB_PRIORITY_GROUP_2			0x00000005U		// 2 bits for preemption priority & 2 bits for sub priority
#define SCB_PRIORITY_GROUP_3			0x00000004U		// 3 bits for preemption priority & 1 bits for sub priority
#define SCB_PRIORITY_GROUP_4			0x00000003U		// 4 bits for preemption priority & 0 bits for sub priority

#define SCB_AIRCR_PRIGROUP_POS			8U				// PRIGROUP Position
#define SCB_AIRCR_PRIGROUP_MASK			(7UL << SCB_AIRCR_PRIGROUP_POS)

#define SCB_AIRCR_VECTKEYSTAT_POS		16U				// VECTKEYSTAT Position
#define SCB_AIRCR_VECTKEYSTAT_MASK		(0xFFFFUL << SCB_AIRCR_VECTKEYSTAT_POS)

/* ---------------------------------------- Macro Declaration End --------------------------------- */


/* ---------------------------------------- Macro Function Declarations Start --------------------- */

/* ---------------------------------------- Macro Function Declarations End ----------------------- */


/* ---------------------------------------- Data Type Declaration Start --------------------------- */

typedef struct
{
    volatile uint32_t CPUID;                      // Offset: 0x000 (R/ ) CPU ID Base Register
    volatile uint32_t ICSR;                       // Offset: 0x004 (R/W) Interrupt Control and State Register
    volatile uint32_t VTOR;                       // Offset: 0x008 (R/W) Vector Table Offset Register
    volatile uint32_t AIRCR;                      // Offset: 0x00C (R/W) Application Interrupt and Reset Control Register
    volatile uint32_t SCR;                        // Offset: 0x010 (R/W) System Control Register
    volatile uint32_t CCR;                        // Offset: 0x014 (R/W) Configuration and Control Register
    volatile uint8_t  SHPR1;					  // Offset: 0x018 (R/W) System Handler Priority Register 1
    volatile uint8_t  SHPR2;					  // Offset: 0x01C (R/W) System Handler Priority Register 2
    volatile uint8_t  SHPR3;					  // Offset: 0x020 (R/W) System Handler Priority Register 2
    volatile uint32_t SHCSR;                      // Offset: 0x024 (R/W) System Handler Control and State Register
    volatile uint32_t CFSR;                       // Offset: 0x028 (R/W) Configurable Fault Status Register
    volatile uint32_t HFSR;                       // Offset: 0x02C (R/W) HardFault Status Register
    volatile uint32_t DFSR;                       // Offset: 0x030 (R/W) Debug Fault Status Register
    volatile uint32_t MMFAR;                      // Offset: 0x034 (R/W) MemManage Fault Address Register
    volatile uint32_t BFAR;                       // Offset: 0x038 (R/W) BusFault Address Register
    volatile uint32_t AFSR;                       // Offset: 0x03C (R/W) Auxiliary Fault Status Register
    volatile uint32_t PFR[2U];                    // Offset: 0x040 (R/ ) Processor Feature Register
    volatile uint32_t DFR;                        // Offset: 0x048 (R/ ) Debug Feature Register
    volatile uint32_t ADR;                        // Offset: 0x04C (R/ ) Auxiliary Feature Register
    volatile uint32_t MMFR[4U];                   // Offset: 0x050 (R/ ) Memory Model Feature Register
    volatile uint32_t ISAR[5U];                   // Offset: 0x060 (R/ ) Instruction Set Attributes Register
    uint32_t RESERVED0[5U];
    volatile uint32_t CPACR;                      // Offset: 0x088 (R/W) Coprocessor Access Control Register
} SCB_Type;

/* ---------------------------------------- Data Type Declaration End ----------------------------- */


/* ---------------------------------------- Software Interfaces Declaration Start ----------------- */

/**
 * brief	Set Priority Grouping
 * details	Sets the priority grouping field using the required unlock sequence.
 * 			The parameter PriorityGroup is assigned to the field SCB->AIRCR [10:8] (only values from 0:7 are used)
 * param	Priority grouping field
 */
void SCB_SetPriorityGrouping(uint32_t PriorityGrouping);

/**
 * brief	Get Priority Grouping
 * details	Reads the priority grouping field from the SCB register.
 * return	Priority Grouping Filed (SCB->AIRCR [10:8])
 */
uint32_t SCB_GetPriorityGrouping(void);

/* ---------------------------------------- Software Interfaces Declaration End ------------------- */

#endif /* CORTEXM3_CORE_SCB_H_ */
