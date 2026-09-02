/**
 * \file
 *
 * Copyright (c) 2015-2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#ifndef _SAMV71_MCAN1_INSTANCE_
#define _SAMV71_MCAN1_INSTANCE_

/* ========== Register definition for MCAN1 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_MCAN1_CREL                    (0x40034000U) /**< \brief (MCAN1) Core Release Register */
  #define REG_MCAN1_ENDN                    (0x40034004U) /**< \brief (MCAN1) Endian Register */
  #define REG_MCAN1_CUST                    (0x40034008U) /**< \brief (MCAN1) Customer Register */
  #define REG_MCAN1_FBTP                    (0x4003400CU) /**< \brief (MCAN1) Fast Bit Timing and Prescaler Register */
  #define REG_MCAN1_DBTP                    (0x4003400CU) /**< \brief (MCAN1) Data Bit Timing and Prescaler Register */
  #define REG_MCAN1_TEST                    (0x40034010U) /**< \brief (MCAN1) Test Register */
  #define REG_MCAN1_RWD                     (0x40034014U) /**< \brief (MCAN1) RAM Watchdog Register */
  #define REG_MCAN1_CCCR                    (0x40034018U) /**< \brief (MCAN1) CC Control Register */
  #define REG_MCAN1_BTP                     (0x4003401CU) /**< \brief (MCAN1) Bit Timing and Prescaler Register */
  #define REG_MCAN1_NBTP                    (0x4003401CU) /**< \brief (MCAN1) Nominal Bit Timing and Prescaler Register */
  #define REG_MCAN1_TSCC                    (0x40034020U) /**< \brief (MCAN1) Timestamp Counter Configuration Register */
  #define REG_MCAN1_TSCV                    (0x40034024U) /**< \brief (MCAN1) Timestamp Counter Value Register */
  #define REG_MCAN1_TOCC                    (0x40034028U) /**< \brief (MCAN1) Timeout Counter Configuration Register */
  #define REG_MCAN1_TOCV                    (0x4003402CU) /**< \brief (MCAN1) Timeout Counter Value Register */
  #define REG_MCAN1_ECR                     (0x40034040U) /**< \brief (MCAN1) Error Counter Register */
  #define REG_MCAN1_PSR                     (0x40034044U) /**< \brief (MCAN1) Protocol Status Register */
  #define REG_MCAN1_TDCR                    (0x40034048U) /**< \brief (MCAN1) Transmit Delay Compensation Register */
  #define REG_MCAN1_IR                      (0x40034050U) /**< \brief (MCAN1) Interrupt Register */
  #define REG_MCAN1_IE                      (0x40034054U) /**< \brief (MCAN1) Interrupt Enable Register */
  #define REG_MCAN1_ILS                     (0x40034058U) /**< \brief (MCAN1) Interrupt Line Select Register */
  #define REG_MCAN1_ILE                     (0x4003405CU) /**< \brief (MCAN1) Interrupt Line Enable Register */
  #define REG_MCAN1_GFC                     (0x40034080U) /**< \brief (MCAN1) Global Filter Configuration Register */
  #define REG_MCAN1_SIDFC                   (0x40034084U) /**< \brief (MCAN1) Standard ID Filter Configuration Register */
  #define REG_MCAN1_XIDFC                   (0x40034088U) /**< \brief (MCAN1) Extended ID Filter Configuration Register */
  #define REG_MCAN1_XIDAM                   (0x40034090U) /**< \brief (MCAN1) Extended ID AND Mask Register */
  #define REG_MCAN1_HPMS                    (0x40034094U) /**< \brief (MCAN1) High Priority Message Status Register */
  #define REG_MCAN1_NDAT1                   (0x40034098U) /**< \brief (MCAN1) New Data 1 Register */
  #define REG_MCAN1_NDAT2                   (0x4003409CU) /**< \brief (MCAN1) New Data 2 Register */
  #define REG_MCAN1_RXF0C                   (0x400340A0U) /**< \brief (MCAN1) Receive FIFO 0 Configuration Register */
  #define REG_MCAN1_RXF0S                   (0x400340A4U) /**< \brief (MCAN1) Receive FIFO 0 Status Register */
  #define REG_MCAN1_RXF0A                   (0x400340A8U) /**< \brief (MCAN1) Receive FIFO 0 Acknowledge Register */
  #define REG_MCAN1_RXBC                    (0x400340ACU) /**< \brief (MCAN1) Receive Rx Buffer Configuration Register */
  #define REG_MCAN1_RXF1C                   (0x400340B0U) /**< \brief (MCAN1) Receive FIFO 1 Configuration Register */
  #define REG_MCAN1_RXF1S                   (0x400340B4U) /**< \brief (MCAN1) Receive FIFO 1 Status Register */
  #define REG_MCAN1_RXF1A                   (0x400340B8U) /**< \brief (MCAN1) Receive FIFO 1 Acknowledge Register */
  #define REG_MCAN1_RXESC                   (0x400340BCU) /**< \brief (MCAN1) Receive Buffer / FIFO Element Size Configuration Register */
  #define REG_MCAN1_TXBC                    (0x400340C0U) /**< \brief (MCAN1) Transmit Buffer Configuration Register */
  #define REG_MCAN1_TXFQS                   (0x400340C4U) /**< \brief (MCAN1) Transmit FIFO/Queue Status Register */
  #define REG_MCAN1_TXESC                   (0x400340C8U) /**< \brief (MCAN1) Transmit Buffer Element Size Configuration Register */
  #define REG_MCAN1_TXBRP                   (0x400340CCU) /**< \brief (MCAN1) Transmit Buffer Request Pending Register */
  #define REG_MCAN1_TXBAR                   (0x400340D0U) /**< \brief (MCAN1) Transmit Buffer Add Request Register */
  #define REG_MCAN1_TXBCR                   (0x400340D4U) /**< \brief (MCAN1) Transmit Buffer Cancellation Request Register */
  #define REG_MCAN1_TXBTO                   (0x400340D8U) /**< \brief (MCAN1) Transmit Buffer Transmission Occurred Register */
  #define REG_MCAN1_TXBCF                   (0x400340DCU) /**< \brief (MCAN1) Transmit Buffer Cancellation Finished Register */
  #define REG_MCAN1_TXBTIE                  (0x400340E0U) /**< \brief (MCAN1) Transmit Buffer Transmission Interrupt Enable Register */
  #define REG_MCAN1_TXBCIE                  (0x400340E4U) /**< \brief (MCAN1) Transmit Buffer Cancellation Finished Interrupt Enable Register */
  #define REG_MCAN1_TXEFC                   (0x400340F0U) /**< \brief (MCAN1) Transmit Event FIFO Configuration Register */
  #define REG_MCAN1_TXEFS                   (0x400340F4U) /**< \brief (MCAN1) Transmit Event FIFO Status Register */
  #define REG_MCAN1_TXEFA                   (0x400340F8U) /**< \brief (MCAN1) Transmit Event FIFO Acknowledge Register */
#else
  #define REG_MCAN1_CREL   (*(volatile  uint32_t*)0x40034000U) /**< \brief (MCAN1) Core Release Register */
  #define REG_MCAN1_ENDN   (*(volatile  uint32_t*)0x40034004U) /**< \brief (MCAN1) Endian Register */
  #define REG_MCAN1_CUST   (*(volatile uint32_t*)0x40034008U) /**< \brief (MCAN1) Customer Register */
  #define REG_MCAN1_FBTP   (*(volatile uint32_t*)0x4003400CU) /**< \brief (MCAN1) Fast Bit Timing and Prescaler Register */
  #define REG_MCAN1_DBTP   (*(volatile uint32_t*)0x4003400CU) /**< \brief (MCAN1) Data Bit Timing and Prescaler Register */
  #define REG_MCAN1_TEST   (*(volatile uint32_t*)0x40034010U) /**< \brief (MCAN1) Test Register */
  #define REG_MCAN1_RWD    (*(volatile uint32_t*)0x40034014U) /**< \brief (MCAN1) RAM Watchdog Register */
  #define REG_MCAN1_CCCR   (*(volatile uint32_t*)0x40034018U) /**< \brief (MCAN1) CC Control Register */
  #define REG_MCAN1_BTP    (*(volatile uint32_t*)0x4003401CU) /**< \brief (MCAN1) Bit Timing and Prescaler Register */
  #define REG_MCAN1_NBTP   (*(volatile uint32_t*)0x4003401CU) /**< \brief (MCAN1) Nominal Bit Timing and Prescaler Register */
  #define REG_MCAN1_TSCC   (*(volatile uint32_t*)0x40034020U) /**< \brief (MCAN1) Timestamp Counter Configuration Register */
  #define REG_MCAN1_TSCV   (*(volatile uint32_t*)0x40034024U) /**< \brief (MCAN1) Timestamp Counter Value Register */
  #define REG_MCAN1_TOCC   (*(volatile uint32_t*)0x40034028U) /**< \brief (MCAN1) Timeout Counter Configuration Register */
  #define REG_MCAN1_TOCV   (*(volatile uint32_t*)0x4003402CU) /**< \brief (MCAN1) Timeout Counter Value Register */
  #define REG_MCAN1_ECR    (*(volatile  uint32_t*)0x40034040U) /**< \brief (MCAN1) Error Counter Register */
  #define REG_MCAN1_PSR    (*(volatile  uint32_t*)0x40034044U) /**< \brief (MCAN1) Protocol Status Register */
  #define REG_MCAN1_TDCR   (*(volatile uint32_t*)0x40034048U) /**< \brief (MCAN1) Transmit Delay Compensation Register */
  #define REG_MCAN1_IR     (*(volatile uint32_t*)0x40034050U) /**< \brief (MCAN1) Interrupt Register */
  #define REG_MCAN1_IE     (*(volatile uint32_t*)0x40034054U) /**< \brief (MCAN1) Interrupt Enable Register */
  #define REG_MCAN1_ILS    (*(volatile uint32_t*)0x40034058U) /**< \brief (MCAN1) Interrupt Line Select Register */
  #define REG_MCAN1_ILE    (*(volatile uint32_t*)0x4003405CU) /**< \brief (MCAN1) Interrupt Line Enable Register */
  #define REG_MCAN1_GFC    (*(volatile uint32_t*)0x40034080U) /**< \brief (MCAN1) Global Filter Configuration Register */
  #define REG_MCAN1_SIDFC  (*(volatile uint32_t*)0x40034084U) /**< \brief (MCAN1) Standard ID Filter Configuration Register */
  #define REG_MCAN1_XIDFC  (*(volatile uint32_t*)0x40034088U) /**< \brief (MCAN1) Extended ID Filter Configuration Register */
  #define REG_MCAN1_XIDAM  (*(volatile uint32_t*)0x40034090U) /**< \brief (MCAN1) Extended ID AND Mask Register */
  #define REG_MCAN1_HPMS   (*(volatile  uint32_t*)0x40034094U) /**< \brief (MCAN1) High Priority Message Status Register */
  #define REG_MCAN1_NDAT1  (*(volatile uint32_t*)0x40034098U) /**< \brief (MCAN1) New Data 1 Register */
  #define REG_MCAN1_NDAT2  (*(volatile uint32_t*)0x4003409CU) /**< \brief (MCAN1) New Data 2 Register */
  #define REG_MCAN1_RXF0C  (*(volatile uint32_t*)0x400340A0U) /**< \brief (MCAN1) Receive FIFO 0 Configuration Register */
  #define REG_MCAN1_RXF0S  (*(volatile  uint32_t*)0x400340A4U) /**< \brief (MCAN1) Receive FIFO 0 Status Register */
  #define REG_MCAN1_RXF0A  (*(volatile uint32_t*)0x400340A8U) /**< \brief (MCAN1) Receive FIFO 0 Acknowledge Register */
  #define REG_MCAN1_RXBC   (*(volatile uint32_t*)0x400340ACU) /**< \brief (MCAN1) Receive Rx Buffer Configuration Register */
  #define REG_MCAN1_RXF1C  (*(volatile uint32_t*)0x400340B0U) /**< \brief (MCAN1) Receive FIFO 1 Configuration Register */
  #define REG_MCAN1_RXF1S  (*(volatile  uint32_t*)0x400340B4U) /**< \brief (MCAN1) Receive FIFO 1 Status Register */
  #define REG_MCAN1_RXF1A  (*(volatile uint32_t*)0x400340B8U) /**< \brief (MCAN1) Receive FIFO 1 Acknowledge Register */
  #define REG_MCAN1_RXESC  (*(volatile uint32_t*)0x400340BCU) /**< \brief (MCAN1) Receive Buffer / FIFO Element Size Configuration Register */
  #define REG_MCAN1_TXBC   (*(volatile uint32_t*)0x400340C0U) /**< \brief (MCAN1) Transmit Buffer Configuration Register */
  #define REG_MCAN1_TXFQS  (*(volatile  uint32_t*)0x400340C4U) /**< \brief (MCAN1) Transmit FIFO/Queue Status Register */
  #define REG_MCAN1_TXESC  (*(volatile uint32_t*)0x400340C8U) /**< \brief (MCAN1) Transmit Buffer Element Size Configuration Register */
  #define REG_MCAN1_TXBRP  (*(volatile  uint32_t*)0x400340CCU) /**< \brief (MCAN1) Transmit Buffer Request Pending Register */
  #define REG_MCAN1_TXBAR  (*(volatile uint32_t*)0x400340D0U) /**< \brief (MCAN1) Transmit Buffer Add Request Register */
  #define REG_MCAN1_TXBCR  (*(volatile uint32_t*)0x400340D4U) /**< \brief (MCAN1) Transmit Buffer Cancellation Request Register */
  #define REG_MCAN1_TXBTO  (*(volatile  uint32_t*)0x400340D8U) /**< \brief (MCAN1) Transmit Buffer Transmission Occurred Register */
  #define REG_MCAN1_TXBCF  (*(volatile  uint32_t*)0x400340DCU) /**< \brief (MCAN1) Transmit Buffer Cancellation Finished Register */
  #define REG_MCAN1_TXBTIE (*(volatile uint32_t*)0x400340E0U) /**< \brief (MCAN1) Transmit Buffer Transmission Interrupt Enable Register */
  #define REG_MCAN1_TXBCIE (*(volatile uint32_t*)0x400340E4U) /**< \brief (MCAN1) Transmit Buffer Cancellation Finished Interrupt Enable Register */
  #define REG_MCAN1_TXEFC  (*(volatile uint32_t*)0x400340F0U) /**< \brief (MCAN1) Transmit Event FIFO Configuration Register */
  #define REG_MCAN1_TXEFS  (*(volatile  uint32_t*)0x400340F4U) /**< \brief (MCAN1) Transmit Event FIFO Status Register */
  #define REG_MCAN1_TXEFA  (*(volatile uint32_t*)0x400340F8U) /**< \brief (MCAN1) Transmit Event FIFO Acknowledge Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAMV71_MCAN1_INSTANCE_ */
