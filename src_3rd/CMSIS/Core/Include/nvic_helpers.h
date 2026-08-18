//
// Created by anolsen on 10.09.2019.
//

#ifndef NORM_FW_NVIC_HELPERS_H
#define NORM_FW_NVIC_HELPERS_H

/* ##########################   NVIC functions  #################################### */
/** \ingroup  CMSIS_Core_FunctionInterface
    \defgroup CMSIS_Core_NVICFunctions NVIC Functions
    \brief      Functions that manage interrupts and exceptions via the NVIC.
    @{
 */


/* SCB Application Interrupt and Reset Control Register Definitions */
#define SCB_AIRCR_VECTKEY_Pos              16                                             /*!< SCB AIRCR: VECTKEY Position */
#define SCB_AIRCR_VECTKEY_Msk              (0xFFFFUL << SCB_AIRCR_VECTKEY_Pos)            /*!< SCB AIRCR: VECTKEY Mask */

#define SCB_AIRCR_SYSRESETREQ_Pos           2                                             /*!< SCB AIRCR: SYSRESETREQ Position */
#define SCB_AIRCR_SYSRESETREQ_Msk          (1UL << SCB_AIRCR_SYSRESETREQ_Pos)             /*!< SCB AIRCR: SYSRESETREQ Mask */

#define SCB_AIRCR_PRIGROUP_Pos              8                                             /*!< SCB AIRCR: PRIGROUP Position */
#define SCB_AIRCR_PRIGROUP_Msk             (7UL << SCB_AIRCR_PRIGROUP_Pos)                /*!< SCB AIRCR: PRIGROUP Mask */

/** \brief  Set Priority Grouping

  The function sets the priority grouping field using the required unlock sequence.
  The parameter PriorityGroup is assigned to the field SCB->AIRCR [10:8] PRIGROUP field.
  Only values from 0..7 are used.
  In case of a conflict between priority grouping and available
  priority bits (__NVIC_PRIO_BITS), the smallest possible priority group is set.

    \param [in]      PriorityGroup  Priority grouping field.
 */
static inline void NVIC_SetPriorityGrouping(uint32_t PriorityGroup) {
    uint32_t reg_value;
    uint32_t PriorityGroupTmp = (PriorityGroup &
                                 (uint32_t) 0x07);               /* only values 0..7 are used          */

    reg_value = SCB->AIRCR;                                                   /* read old register configuration    */
    reg_value &= ~(SCB_AIRCR_VECTKEY_Msk | SCB_AIRCR_PRIGROUP_Msk);             /* clear bits to change               */
    reg_value = (reg_value |
                 ((uint32_t) 0x5FA << SCB_AIRCR_VECTKEY_Pos) |
                 (PriorityGroupTmp << 8));                                     /* Insert write key and priorty group */
    SCB->AIRCR = reg_value;
}


/** \brief  Get Priority Grouping

  The function reads the priority grouping field from the NVIC Interrupt Controller.

    \return                Priority grouping field (SCB->AIRCR [10:8] PRIGROUP field).
 */
static inline uint32_t NVIC_GetPriorityGrouping(void) {
    return ((SCB->AIRCR & SCB_AIRCR_PRIGROUP_Msk) >> SCB_AIRCR_PRIGROUP_Pos);   /* read priority grouping field */
}


/** \brief  Enable External Interrupt

    The function enables a device-specific interrupt in the NVIC interrupt controller.

    \param [in]      IRQn  External interrupt number. Value cannot be negative.
 */
static inline void NVIC_EnableIRQ(uint32_t IRQn) {
/*  NVIC->ISER[((uint32_t)(IRQn) >> 5)] = (1 << ((uint32_t)(IRQn) & 0x1F));  enable interrupt */
    NVIC->ISER[(uint32_t) ((int32_t) IRQn) >> 5] = (uint32_t) (1
            << ((uint32_t) ((int32_t) IRQn) & (uint32_t) 0x1F)); /* enable interrupt */
}


/** \brief  Disable External Interrupt

    The function disables a device-specific interrupt in the NVIC interrupt controller.

    \param [in]      IRQn  External interrupt number. Value cannot be negative.
 */
static inline void  NVIC_DisableIRQ(uint32_t IRQn) {
    NVIC->ICER[(IRQn >> 5)] = (1 << (IRQn & 0x1F)); /* disable interrupt */
}


/** \brief  Get Pending Interrupt

    The function reads the pending register in the NVIC and returns the pending bit
    for the specified interrupt.

    \param [in]      IRQn  Interrupt number.

    \return             0  Interrupt status is not pending.
    \return             1  Interrupt status is pending.
 */
static inline uint32_t NVIC_GetPendingIRQ(uint32_t IRQn) {
    return ((uint32_t) ((NVIC->ISPR[(IRQn >> 5)] & (1 << (IRQn & 0x1F))) ? 1 : 0)); /* Return 1 if pending else 0 */
}


/** \brief  Set Pending Interrupt

    The function sets the pending bit of an external interrupt.

    \param [in]      IRQn  Interrupt number. Value cannot be negative.
 */
static inline void NVIC_SetPendingIRQ(uint32_t IRQn) {
    NVIC->ISPR[(IRQn >> 5)] = (1 << (IRQn & 0x1F)); /* set interrupt pending */
}


/** \brief  Clear Pending Interrupt

    The function clears the pending bit of an external interrupt.

    \param [in]      IRQn  External interrupt number. Value cannot be negative.
 */
static inline void NVIC_ClearPendingIRQ(uint32_t IRQn) {
    NVIC->ICPR[((uint32_t) (IRQn) >> 5)] = (1 << ((uint32_t) (IRQn) & 0x1F)); /* Clear pending interrupt */
}


/** \brief  Get Active Interrupt

    The function reads the active register in NVIC and returns the active bit.

    \param [in]      IRQn  Interrupt number.

    \return             0  Interrupt status is not active.
    \return             1  Interrupt status is active.
 */
static inline uint32_t NVIC_GetActive(IRQn_Type IRQn) {
    return ((uint32_t) ((NVIC->IABR[(uint32_t) (IRQn) >> 5] & (1 << ((uint32_t) (IRQn) & 0x1F))) ? 1
                                                                                                 : 0)); /* Return 1 if active else 0 */
}


/** \brief  Set Interrupt Priority

    The function sets the priority of an interrupt.

    \note The priority cannot be set for every core interrupt.

    \param [in]      IRQn  Interrupt number.
    \param [in]  priority  Priority to set.
 */
static inline void NVIC_SetPriority(IRQn_Type IRQn, uint32_t priority) {
    if (IRQn < 0) {
        SCB->SHPR[((uint32_t) (IRQn) & 0xF) - 4] = ((priority << (8 - __NVIC_PRIO_BITS)) & 0xff);
    } /* set Priority for Cortex-M  System Interrupts */
    else {
        NVIC->IP[(uint32_t) (IRQn)] = ((priority << (8 - __NVIC_PRIO_BITS)) & 0xff);
    } /* set Priority for device specific Interrupts  */
}


/** \brief  Get Interrupt Priority

    The function reads the priority of an interrupt. The interrupt
    number can be positive to specify an external (device specific)
    interrupt, or negative to specify an internal (core) interrupt.


    \param [in]   IRQn  Interrupt number.
    \return             Interrupt Priority. Value is aligned automatically to the implemented
                        priority bits of the microcontroller.
 */
static inline uint32_t NVIC_GetPriority(IRQn_Type IRQn) {

    if (IRQn < 0) {
        return ((uint32_t) (SCB->SHPR[((uint32_t) (IRQn) & 0xF) - 4] >> (8 - __NVIC_PRIO_BITS)));
    } /* get priority for Cortex-M  system interrupts */
    else {
        return ((uint32_t) (NVIC->IP[(uint32_t) (IRQn)] >> (8 - __NVIC_PRIO_BITS)));
    } /* get priority for device specific interrupts  */
}


/** \brief  Encode Priority

    The function encodes the priority for an interrupt with the given priority group,
    preemptive priority value, and subpriority value.
    In case of a conflict between priority grouping and available
    priority bits (__NVIC_PRIO_BITS), the smallest possible priority group is set.

    \param [in]     PriorityGroup  Used priority group.
    \param [in]   PreemptPriority  Preemptive priority value (starting from 0).
    \param [in]       SubPriority  Subpriority value (starting from 0).
    \return                        Encoded priority. Value can be used in the function \ref NVIC_SetPriority().
 */
static inline uint32_t NVIC_EncodePriority(uint32_t PriorityGroup, uint32_t PreemptPriority, uint32_t SubPriority) {
    uint32_t PriorityGroupTmp = (PriorityGroup & 0x07);          /* only values 0..7 are used          */
    uint32_t PreemptPriorityBits;
    uint32_t SubPriorityBits;

    PreemptPriorityBits = ((7 - PriorityGroupTmp) > __NVIC_PRIO_BITS) ? __NVIC_PRIO_BITS : 7 - PriorityGroupTmp;
    SubPriorityBits = ((PriorityGroupTmp + __NVIC_PRIO_BITS) < 7) ? 0 : PriorityGroupTmp - 7 + __NVIC_PRIO_BITS;

    return (
            ((PreemptPriority & ((1 << (PreemptPriorityBits)) - 1)) << SubPriorityBits) |
            ((SubPriority & ((1 << (SubPriorityBits)) - 1)))
    );
}


/** \brief  Decode Priority

    The function decodes an interrupt priority value with a given priority group to
    preemptive priority value and subpriority value.
    In case of a conflict between priority grouping and available
    priority bits (__NVIC_PRIO_BITS) the smallest possible priority group is set.

    \param [in]         Priority   Priority value, which can be retrieved with the function \ref NVIC_GetPriority().
    \param [in]     PriorityGroup  Used priority group.
    \param [out] pPreemptPriority  Preemptive priority value (starting from 0).
    \param [out]     pSubPriority  Subpriority value (starting from 0).
 */
static inline void
NVIC_DecodePriority(uint32_t Priority, uint32_t PriorityGroup, uint32_t *pPreemptPriority, uint32_t *pSubPriority) {
    uint32_t PriorityGroupTmp = (PriorityGroup & 0x07);          /* only values 0..7 are used          */
    uint32_t PreemptPriorityBits;
    uint32_t SubPriorityBits;

    PreemptPriorityBits = ((7 - PriorityGroupTmp) > __NVIC_PRIO_BITS) ? __NVIC_PRIO_BITS : 7 - PriorityGroupTmp;
    SubPriorityBits = ((PriorityGroupTmp + __NVIC_PRIO_BITS) < 7) ? 0 : PriorityGroupTmp - 7 + __NVIC_PRIO_BITS;

    *pPreemptPriority = (Priority >> SubPriorityBits) & ((1 << (PreemptPriorityBits)) - 1);
    *pSubPriority = (Priority) & ((1 << (SubPriorityBits)) - 1);
}


/** \brief  System Reset

    The function initiates a system reset request to reset the MCU.
 */
static inline void NVIC_SystemReset(void) {
    __asm volatile ("dsb");
    SCB->AIRCR = ((0x5FA << SCB_AIRCR_VECTKEY_Pos) |
                  (SCB->AIRCR & SCB_AIRCR_PRIGROUP_Msk) |
                  SCB_AIRCR_SYSRESETREQ_Msk);
    __asm volatile ("dsb");
    while (1);                                                    /* wait until reset */
}

#endif //NORM_FW_NVIC_HELPERS_H
