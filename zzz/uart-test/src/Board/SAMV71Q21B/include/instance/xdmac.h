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

#ifndef _SAMV71_XDMAC_INSTANCE_
#define _SAMV71_XDMAC_INSTANCE_

/* ========== Register definition for XDMAC peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_XDMAC_GTYPE                      (0x40078000U) /**< \brief (XDMAC) Global Type Register */
  #define REG_XDMAC_GCFG                       (0x40078004U) /**< \brief (XDMAC) Global Configuration Register */
  #define REG_XDMAC_GWAC                       (0x40078008U) /**< \brief (XDMAC) Global Weighted Arbiter Configuration Register */
  #define REG_XDMAC_GIE                        (0x4007800CU) /**< \brief (XDMAC) Global Interrupt Enable Register */
  #define REG_XDMAC_GID                        (0x40078010U) /**< \brief (XDMAC) Global Interrupt Disable Register */
  #define REG_XDMAC_GIM                        (0x40078014U) /**< \brief (XDMAC) Global Interrupt Mask Register */
  #define REG_XDMAC_GIS                        (0x40078018U) /**< \brief (XDMAC) Global Interrupt Status Register */
  #define REG_XDMAC_GE                         (0x4007801CU) /**< \brief (XDMAC) Global Channel Enable Register */
  #define REG_XDMAC_GD                         (0x40078020U) /**< \brief (XDMAC) Global Channel Disable Register */
  #define REG_XDMAC_GS                         (0x40078024U) /**< \brief (XDMAC) Global Channel Status Register */
  #define REG_XDMAC_GRS                        (0x40078028U) /**< \brief (XDMAC) Global Channel Read Suspend Register */
  #define REG_XDMAC_GWS                        (0x4007802CU) /**< \brief (XDMAC) Global Channel Write Suspend Register */
  #define REG_XDMAC_GRWS                       (0x40078030U) /**< \brief (XDMAC) Global Channel Read Write Suspend Register */
  #define REG_XDMAC_GRWR                       (0x40078034U) /**< \brief (XDMAC) Global Channel Read Write Resume Register */
  #define REG_XDMAC_GSWR                       (0x40078038U) /**< \brief (XDMAC) Global Channel Software Request Register */
  #define REG_XDMAC_GSWS                       (0x4007803CU) /**< \brief (XDMAC) Global Channel Software Request Status Register */
  #define REG_XDMAC_GSWF                       (0x40078040U) /**< \brief (XDMAC) Global Channel Software Flush Request Register */
  #define REG_XDMAC_CIE0                       (0x40078050U) /**< \brief (XDMAC) Channel Interrupt Enable Register (chid = 0) */
  #define REG_XDMAC_CID0                       (0x40078054U) /**< \brief (XDMAC) Channel Interrupt Disable Register (chid = 0) */
  #define REG_XDMAC_CIM0                       (0x40078058U) /**< \brief (XDMAC) Channel Interrupt Mask Register (chid = 0) */
  #define REG_XDMAC_CIS0                       (0x4007805CU) /**< \brief (XDMAC) Channel Interrupt Status Register (chid = 0) */
  #define REG_XDMAC_CSA0                       (0x40078060U) /**< \brief (XDMAC) Channel Source Address Register (chid = 0) */
  #define REG_XDMAC_CDA0                       (0x40078064U) /**< \brief (XDMAC) Channel Destination Address Register (chid = 0) */
  #define REG_XDMAC_CNDA0                      (0x40078068U) /**< \brief (XDMAC) Channel Next Descriptor Address Register (chid = 0) */
  #define REG_XDMAC_CNDC0                      (0x4007806CU) /**< \brief (XDMAC) Channel Next Descriptor Control Register (chid = 0) */
  #define REG_XDMAC_CUBC0                      (0x40078070U) /**< \brief (XDMAC) Channel Microblock Control Register (chid = 0) */
  #define REG_XDMAC_CBC0                       (0x40078074U) /**< \brief (XDMAC) Channel Block Control Register (chid = 0) */
  #define REG_XDMAC_CC0                        (0x40078078U) /**< \brief (XDMAC) Channel Configuration Register (chid = 0) */
  #define REG_XDMAC_CDS_MSP0                   (0x4007807CU) /**< \brief (XDMAC) Channel Data Stride Memory Set Pattern (chid = 0) */
  #define REG_XDMAC_CSUS0                      (0x40078080U) /**< \brief (XDMAC) Channel Source Microblock Stride (chid = 0) */
  #define REG_XDMAC_CDUS0                      (0x40078084U) /**< \brief (XDMAC) Channel Destination Microblock Stride (chid = 0) */
  #define REG_XDMAC_CIE1                       (0x40078090U) /**< \brief (XDMAC) Channel Interrupt Enable Register (chid = 1) */
  #define REG_XDMAC_CID1                       (0x40078094U) /**< \brief (XDMAC) Channel Interrupt Disable Register (chid = 1) */
  #define REG_XDMAC_CIM1                       (0x40078098U) /**< \brief (XDMAC) Channel Interrupt Mask Register (chid = 1) */
  #define REG_XDMAC_CIS1                       (0x4007809CU) /**< \brief (XDMAC) Channel Interrupt Status Register (chid = 1) */
  #define REG_XDMAC_CSA1                       (0x400780A0U) /**< \brief (XDMAC) Channel Source Address Register (chid = 1) */
  #define REG_XDMAC_CDA1                       (0x400780A4U) /**< \brief (XDMAC) Channel Destination Address Register (chid = 1) */
  #define REG_XDMAC_CNDA1                      (0x400780A8U) /**< \brief (XDMAC) Channel Next Descriptor Address Register (chid = 1) */
  #define REG_XDMAC_CNDC1                      (0x400780ACU) /**< \brief (XDMAC) Channel Next Descriptor Control Register (chid = 1) */
  #define REG_XDMAC_CUBC1                      (0x400780B0U) /**< \brief (XDMAC) Channel Microblock Control Register (chid = 1) */
  #define REG_XDMAC_CBC1                       (0x400780B4U) /**< \brief (XDMAC) Channel Block Control Register (chid = 1) */
  #define REG_XDMAC_CC1                        (0x400780B8U) /**< \brief (XDMAC) Channel Configuration Register (chid = 1) */
  #define REG_XDMAC_CDS_MSP1                   (0x400780BCU) /**< \brief (XDMAC) Channel Data Stride Memory Set Pattern (chid = 1) */
  #define REG_XDMAC_CSUS1                      (0x400780C0U) /**< \brief (XDMAC) Channel Source Microblock Stride (chid = 1) */
  #define REG_XDMAC_CDUS1                      (0x400780C4U) /**< \brief (XDMAC) Channel Destination Microblock Stride (chid = 1) */
  #define REG_XDMAC_CIE2                       (0x400780D0U) /**< \brief (XDMAC) Channel Interrupt Enable Register (chid = 2) */
  #define REG_XDMAC_CID2                       (0x400780D4U) /**< \brief (XDMAC) Channel Interrupt Disable Register (chid = 2) */
  #define REG_XDMAC_CIM2                       (0x400780D8U) /**< \brief (XDMAC) Channel Interrupt Mask Register (chid = 2) */
  #define REG_XDMAC_CIS2                       (0x400780DCU) /**< \brief (XDMAC) Channel Interrupt Status Register (chid = 2) */
  #define REG_XDMAC_CSA2                       (0x400780E0U) /**< \brief (XDMAC) Channel Source Address Register (chid = 2) */
  #define REG_XDMAC_CDA2                       (0x400780E4U) /**< \brief (XDMAC) Channel Destination Address Register (chid = 2) */
  #define REG_XDMAC_CNDA2                      (0x400780E8U) /**< \brief (XDMAC) Channel Next Descriptor Address Register (chid = 2) */
  #define REG_XDMAC_CNDC2                      (0x400780ECU) /**< \brief (XDMAC) Channel Next Descriptor Control Register (chid = 2) */
  #define REG_XDMAC_CUBC2                      (0x400780F0U) /**< \brief (XDMAC) Channel Microblock Control Register (chid = 2) */
  #define REG_XDMAC_CBC2                       (0x400780F4U) /**< \brief (XDMAC) Channel Block Control Register (chid = 2) */
  #define REG_XDMAC_CC2                        (0x400780F8U) /**< \brief (XDMAC) Channel Configuration Register (chid = 2) */
  #define REG_XDMAC_CDS_MSP2                   (0x400780FCU) /**< \brief (XDMAC) Channel Data Stride Memory Set Pattern (chid = 2) */
  #define REG_XDMAC_CSUS2                      (0x40078100U) /**< \brief (XDMAC) Channel Source Microblock Stride (chid = 2) */
  #define REG_XDMAC_CDUS2                      (0x40078104U) /**< \brief (XDMAC) Channel Destination Microblock Stride (chid = 2) */
  #define REG_XDMAC_CIE3                       (0x40078110U) /**< \brief (XDMAC) Channel Interrupt Enable Register (chid = 3) */
  #define REG_XDMAC_CID3                       (0x40078114U) /**< \brief (XDMAC) Channel Interrupt Disable Register (chid = 3) */
  #define REG_XDMAC_CIM3                       (0x40078118U) /**< \brief (XDMAC) Channel Interrupt Mask Register (chid = 3) */
  #define REG_XDMAC_CIS3                       (0x4007811CU) /**< \brief (XDMAC) Channel Interrupt Status Register (chid = 3) */
  #define REG_XDMAC_CSA3                       (0x40078120U) /**< \brief (XDMAC) Channel Source Address Register (chid = 3) */
  #define REG_XDMAC_CDA3                       (0x40078124U) /**< \brief (XDMAC) Channel Destination Address Register (chid = 3) */
  #define REG_XDMAC_CNDA3                      (0x40078128U) /**< \brief (XDMAC) Channel Next Descriptor Address Register (chid = 3) */
  #define REG_XDMAC_CNDC3                      (0x4007812CU) /**< \brief (XDMAC) Channel Next Descriptor Control Register (chid = 3) */
  #define REG_XDMAC_CUBC3                      (0x40078130U) /**< \brief (XDMAC) Channel Microblock Control Register (chid = 3) */
  #define REG_XDMAC_CBC3                       (0x40078134U) /**< \brief (XDMAC) Channel Block Control Register (chid = 3) */
  #define REG_XDMAC_CC3                        (0x40078138U) /**< \brief (XDMAC) Channel Configuration Register (chid = 3) */
  #define REG_XDMAC_CDS_MSP3                   (0x4007813CU) /**< \brief (XDMAC) Channel Data Stride Memory Set Pattern (chid = 3) */
  #define REG_XDMAC_CSUS3                      (0x40078140U) /**< \brief (XDMAC) Channel Source Microblock Stride (chid = 3) */
  #define REG_XDMAC_CDUS3                      (0x40078144U) /**< \brief (XDMAC) Channel Destination Microblock Stride (chid = 3) */
  #define REG_XDMAC_CIE4                       (0x40078150U) /**< \brief (XDMAC) Channel Interrupt Enable Register (chid = 4) */
  #define REG_XDMAC_CID4                       (0x40078154U) /**< \brief (XDMAC) Channel Interrupt Disable Register (chid = 4) */
  #define REG_XDMAC_CIM4                       (0x40078158U) /**< \brief (XDMAC) Channel Interrupt Mask Register (chid = 4) */
  #define REG_XDMAC_CIS4                       (0x4007815CU) /**< \brief (XDMAC) Channel Interrupt Status Register (chid = 4) */
  #define REG_XDMAC_CSA4                       (0x40078160U) /**< \brief (XDMAC) Channel Source Address Register (chid = 4) */
  #define REG_XDMAC_CDA4                       (0x40078164U) /**< \brief (XDMAC) Channel Destination Address Register (chid = 4) */
  #define REG_XDMAC_CNDA4                      (0x40078168U) /**< \brief (XDMAC) Channel Next Descriptor Address Register (chid = 4) */
  #define REG_XDMAC_CNDC4                      (0x4007816CU) /**< \brief (XDMAC) Channel Next Descriptor Control Register (chid = 4) */
  #define REG_XDMAC_CUBC4                      (0x40078170U) /**< \brief (XDMAC) Channel Microblock Control Register (chid = 4) */
  #define REG_XDMAC_CBC4                       (0x40078174U) /**< \brief (XDMAC) Channel Block Control Register (chid = 4) */
  #define REG_XDMAC_CC4                        (0x40078178U) /**< \brief (XDMAC) Channel Configuration Register (chid = 4) */
  #define REG_XDMAC_CDS_MSP4                   (0x4007817CU) /**< \brief (XDMAC) Channel Data Stride Memory Set Pattern (chid = 4) */
  #define REG_XDMAC_CSUS4                      (0x40078180U) /**< \brief (XDMAC) Channel Source Microblock Stride (chid = 4) */
  #define REG_XDMAC_CDUS4                      (0x40078184U) /**< \brief (XDMAC) Channel Destination Microblock Stride (chid = 4) */
  #define REG_XDMAC_CIE5                       (0x40078190U) /**< \brief (XDMAC) Channel Interrupt Enable Register (chid = 5) */
  #define REG_XDMAC_CID5                       (0x40078194U) /**< \brief (XDMAC) Channel Interrupt Disable Register (chid = 5) */
  #define REG_XDMAC_CIM5                       (0x40078198U) /**< \brief (XDMAC) Channel Interrupt Mask Register (chid = 5) */
  #define REG_XDMAC_CIS5                       (0x4007819CU) /**< \brief (XDMAC) Channel Interrupt Status Register (chid = 5) */
  #define REG_XDMAC_CSA5                       (0x400781A0U) /**< \brief (XDMAC) Channel Source Address Register (chid = 5) */
  #define REG_XDMAC_CDA5                       (0x400781A4U) /**< \brief (XDMAC) Channel Destination Address Register (chid = 5) */
  #define REG_XDMAC_CNDA5                      (0x400781A8U) /**< \brief (XDMAC) Channel Next Descriptor Address Register (chid = 5) */
  #define REG_XDMAC_CNDC5                      (0x400781ACU) /**< \brief (XDMAC) Channel Next Descriptor Control Register (chid = 5) */
  #define REG_XDMAC_CUBC5                      (0x400781B0U) /**< \brief (XDMAC) Channel Microblock Control Register (chid = 5) */
  #define REG_XDMAC_CBC5                       (0x400781B4U) /**< \brief (XDMAC) Channel Block Control Register (chid = 5) */
  #define REG_XDMAC_CC5                        (0x400781B8U) /**< \brief (XDMAC) Channel Configuration Register (chid = 5) */
  #define REG_XDMAC_CDS_MSP5                   (0x400781BCU) /**< \brief (XDMAC) Channel Data Stride Memory Set Pattern (chid = 5) */
  #define REG_XDMAC_CSUS5                      (0x400781C0U) /**< \brief (XDMAC) Channel Source Microblock Stride (chid = 5) */
  #define REG_XDMAC_CDUS5                      (0x400781C4U) /**< \brief (XDMAC) Channel Destination Microblock Stride (chid = 5) */
  #define REG_XDMAC_CIE6                       (0x400781D0U) /**< \brief (XDMAC) Channel Interrupt Enable Register (chid = 6) */
  #define REG_XDMAC_CID6                       (0x400781D4U) /**< \brief (XDMAC) Channel Interrupt Disable Register (chid = 6) */
  #define REG_XDMAC_CIM6                       (0x400781D8U) /**< \brief (XDMAC) Channel Interrupt Mask Register (chid = 6) */
  #define REG_XDMAC_CIS6                       (0x400781DCU) /**< \brief (XDMAC) Channel Interrupt Status Register (chid = 6) */
  #define REG_XDMAC_CSA6                       (0x400781E0U) /**< \brief (XDMAC) Channel Source Address Register (chid = 6) */
  #define REG_XDMAC_CDA6                       (0x400781E4U) /**< \brief (XDMAC) Channel Destination Address Register (chid = 6) */
  #define REG_XDMAC_CNDA6                      (0x400781E8U) /**< \brief (XDMAC) Channel Next Descriptor Address Register (chid = 6) */
  #define REG_XDMAC_CNDC6                      (0x400781ECU) /**< \brief (XDMAC) Channel Next Descriptor Control Register (chid = 6) */
  #define REG_XDMAC_CUBC6                      (0x400781F0U) /**< \brief (XDMAC) Channel Microblock Control Register (chid = 6) */
  #define REG_XDMAC_CBC6                       (0x400781F4U) /**< \brief (XDMAC) Channel Block Control Register (chid = 6) */
  #define REG_XDMAC_CC6                        (0x400781F8U) /**< \brief (XDMAC) Channel Configuration Register (chid = 6) */
  #define REG_XDMAC_CDS_MSP6                   (0x400781FCU) /**< \brief (XDMAC) Channel Data Stride Memory Set Pattern (chid = 6) */
  #define REG_XDMAC_CSUS6                      (0x40078200U) /**< \brief (XDMAC) Channel Source Microblock Stride (chid = 6) */
  #define REG_XDMAC_CDUS6                      (0x40078204U) /**< \brief (XDMAC) Channel Destination Microblock Stride (chid = 6) */
  #define REG_XDMAC_CIE7                       (0x40078210U) /**< \brief (XDMAC) Channel Interrupt Enable Register (chid = 7) */
  #define REG_XDMAC_CID7                       (0x40078214U) /**< \brief (XDMAC) Channel Interrupt Disable Register (chid = 7) */
  #define REG_XDMAC_CIM7                       (0x40078218U) /**< \brief (XDMAC) Channel Interrupt Mask Register (chid = 7) */
  #define REG_XDMAC_CIS7                       (0x4007821CU) /**< \brief (XDMAC) Channel Interrupt Status Register (chid = 7) */
  #define REG_XDMAC_CSA7                       (0x40078220U) /**< \brief (XDMAC) Channel Source Address Register (chid = 7) */
  #define REG_XDMAC_CDA7                       (0x40078224U) /**< \brief (XDMAC) Channel Destination Address Register (chid = 7) */
  #define REG_XDMAC_CNDA7                      (0x40078228U) /**< \brief (XDMAC) Channel Next Descriptor Address Register (chid = 7) */
  #define REG_XDMAC_CNDC7                      (0x4007822CU) /**< \brief (XDMAC) Channel Next Descriptor Control Register (chid = 7) */
  #define REG_XDMAC_CUBC7                      (0x40078230U) /**< \brief (XDMAC) Channel Microblock Control Register (chid = 7) */
  #define REG_XDMAC_CBC7                       (0x40078234U) /**< \brief (XDMAC) Channel Block Control Register (chid = 7) */
  #define REG_XDMAC_CC7                        (0x40078238U) /**< \brief (XDMAC) Channel Configuration Register (chid = 7) */
  #define REG_XDMAC_CDS_MSP7                   (0x4007823CU) /**< \brief (XDMAC) Channel Data Stride Memory Set Pattern (chid = 7) */
  #define REG_XDMAC_CSUS7                      (0x40078240U) /**< \brief (XDMAC) Channel Source Microblock Stride (chid = 7) */
  #define REG_XDMAC_CDUS7                      (0x40078244U) /**< \brief (XDMAC) Channel Destination Microblock Stride (chid = 7) */
  #define REG_XDMAC_CIE8                       (0x40078250U) /**< \brief (XDMAC) Channel Interrupt Enable Register (chid = 8) */
  #define REG_XDMAC_CID8                       (0x40078254U) /**< \brief (XDMAC) Channel Interrupt Disable Register (chid = 8) */
  #define REG_XDMAC_CIM8                       (0x40078258U) /**< \brief (XDMAC) Channel Interrupt Mask Register (chid = 8) */
  #define REG_XDMAC_CIS8                       (0x4007825CU) /**< \brief (XDMAC) Channel Interrupt Status Register (chid = 8) */
  #define REG_XDMAC_CSA8                       (0x40078260U) /**< \brief (XDMAC) Channel Source Address Register (chid = 8) */
  #define REG_XDMAC_CDA8                       (0x40078264U) /**< \brief (XDMAC) Channel Destination Address Register (chid = 8) */
  #define REG_XDMAC_CNDA8                      (0x40078268U) /**< \brief (XDMAC) Channel Next Descriptor Address Register (chid = 8) */
  #define REG_XDMAC_CNDC8                      (0x4007826CU) /**< \brief (XDMAC) Channel Next Descriptor Control Register (chid = 8) */
  #define REG_XDMAC_CUBC8                      (0x40078270U) /**< \brief (XDMAC) Channel Microblock Control Register (chid = 8) */
  #define REG_XDMAC_CBC8                       (0x40078274U) /**< \brief (XDMAC) Channel Block Control Register (chid = 8) */
  #define REG_XDMAC_CC8                        (0x40078278U) /**< \brief (XDMAC) Channel Configuration Register (chid = 8) */
  #define REG_XDMAC_CDS_MSP8                   (0x4007827CU) /**< \brief (XDMAC) Channel Data Stride Memory Set Pattern (chid = 8) */
  #define REG_XDMAC_CSUS8                      (0x40078280U) /**< \brief (XDMAC) Channel Source Microblock Stride (chid = 8) */
  #define REG_XDMAC_CDUS8                      (0x40078284U) /**< \brief (XDMAC) Channel Destination Microblock Stride (chid = 8) */
  #define REG_XDMAC_CIE9                       (0x40078290U) /**< \brief (XDMAC) Channel Interrupt Enable Register (chid = 9) */
  #define REG_XDMAC_CID9                       (0x40078294U) /**< \brief (XDMAC) Channel Interrupt Disable Register (chid = 9) */
  #define REG_XDMAC_CIM9                       (0x40078298U) /**< \brief (XDMAC) Channel Interrupt Mask Register (chid = 9) */
  #define REG_XDMAC_CIS9                       (0x4007829CU) /**< \brief (XDMAC) Channel Interrupt Status Register (chid = 9) */
  #define REG_XDMAC_CSA9                       (0x400782A0U) /**< \brief (XDMAC) Channel Source Address Register (chid = 9) */
  #define REG_XDMAC_CDA9                       (0x400782A4U) /**< \brief (XDMAC) Channel Destination Address Register (chid = 9) */
  #define REG_XDMAC_CNDA9                      (0x400782A8U) /**< \brief (XDMAC) Channel Next Descriptor Address Register (chid = 9) */
  #define REG_XDMAC_CNDC9                      (0x400782ACU) /**< \brief (XDMAC) Channel Next Descriptor Control Register (chid = 9) */
  #define REG_XDMAC_CUBC9                      (0x400782B0U) /**< \brief (XDMAC) Channel Microblock Control Register (chid = 9) */
  #define REG_XDMAC_CBC9                       (0x400782B4U) /**< \brief (XDMAC) Channel Block Control Register (chid = 9) */
  #define REG_XDMAC_CC9                        (0x400782B8U) /**< \brief (XDMAC) Channel Configuration Register (chid = 9) */
  #define REG_XDMAC_CDS_MSP9                   (0x400782BCU) /**< \brief (XDMAC) Channel Data Stride Memory Set Pattern (chid = 9) */
  #define REG_XDMAC_CSUS9                      (0x400782C0U) /**< \brief (XDMAC) Channel Source Microblock Stride (chid = 9) */
  #define REG_XDMAC_CDUS9                      (0x400782C4U) /**< \brief (XDMAC) Channel Destination Microblock Stride (chid = 9) */
  #define REG_XDMAC_CIE10                      (0x400782D0U) /**< \brief (XDMAC) Channel Interrupt Enable Register (chid = 10) */
  #define REG_XDMAC_CID10                      (0x400782D4U) /**< \brief (XDMAC) Channel Interrupt Disable Register (chid = 10) */
  #define REG_XDMAC_CIM10                      (0x400782D8U) /**< \brief (XDMAC) Channel Interrupt Mask Register (chid = 10) */
  #define REG_XDMAC_CIS10                      (0x400782DCU) /**< \brief (XDMAC) Channel Interrupt Status Register (chid = 10) */
  #define REG_XDMAC_CSA10                      (0x400782E0U) /**< \brief (XDMAC) Channel Source Address Register (chid = 10) */
  #define REG_XDMAC_CDA10                      (0x400782E4U) /**< \brief (XDMAC) Channel Destination Address Register (chid = 10) */
  #define REG_XDMAC_CNDA10                     (0x400782E8U) /**< \brief (XDMAC) Channel Next Descriptor Address Register (chid = 10) */
  #define REG_XDMAC_CNDC10                     (0x400782ECU) /**< \brief (XDMAC) Channel Next Descriptor Control Register (chid = 10) */
  #define REG_XDMAC_CUBC10                     (0x400782F0U) /**< \brief (XDMAC) Channel Microblock Control Register (chid = 10) */
  #define REG_XDMAC_CBC10                      (0x400782F4U) /**< \brief (XDMAC) Channel Block Control Register (chid = 10) */
  #define REG_XDMAC_CC10                       (0x400782F8U) /**< \brief (XDMAC) Channel Configuration Register (chid = 10) */
  #define REG_XDMAC_CDS_MSP10                  (0x400782FCU) /**< \brief (XDMAC) Channel Data Stride Memory Set Pattern (chid = 10) */
  #define REG_XDMAC_CSUS10                     (0x40078300U) /**< \brief (XDMAC) Channel Source Microblock Stride (chid = 10) */
  #define REG_XDMAC_CDUS10                     (0x40078304U) /**< \brief (XDMAC) Channel Destination Microblock Stride (chid = 10) */
  #define REG_XDMAC_CIE11                      (0x40078310U) /**< \brief (XDMAC) Channel Interrupt Enable Register (chid = 11) */
  #define REG_XDMAC_CID11                      (0x40078314U) /**< \brief (XDMAC) Channel Interrupt Disable Register (chid = 11) */
  #define REG_XDMAC_CIM11                      (0x40078318U) /**< \brief (XDMAC) Channel Interrupt Mask Register (chid = 11) */
  #define REG_XDMAC_CIS11                      (0x4007831CU) /**< \brief (XDMAC) Channel Interrupt Status Register (chid = 11) */
  #define REG_XDMAC_CSA11                      (0x40078320U) /**< \brief (XDMAC) Channel Source Address Register (chid = 11) */
  #define REG_XDMAC_CDA11                      (0x40078324U) /**< \brief (XDMAC) Channel Destination Address Register (chid = 11) */
  #define REG_XDMAC_CNDA11                     (0x40078328U) /**< \brief (XDMAC) Channel Next Descriptor Address Register (chid = 11) */
  #define REG_XDMAC_CNDC11                     (0x4007832CU) /**< \brief (XDMAC) Channel Next Descriptor Control Register (chid = 11) */
  #define REG_XDMAC_CUBC11                     (0x40078330U) /**< \brief (XDMAC) Channel Microblock Control Register (chid = 11) */
  #define REG_XDMAC_CBC11                      (0x40078334U) /**< \brief (XDMAC) Channel Block Control Register (chid = 11) */
  #define REG_XDMAC_CC11                       (0x40078338U) /**< \brief (XDMAC) Channel Configuration Register (chid = 11) */
  #define REG_XDMAC_CDS_MSP11                  (0x4007833CU) /**< \brief (XDMAC) Channel Data Stride Memory Set Pattern (chid = 11) */
  #define REG_XDMAC_CSUS11                     (0x40078340U) /**< \brief (XDMAC) Channel Source Microblock Stride (chid = 11) */
  #define REG_XDMAC_CDUS11                     (0x40078344U) /**< \brief (XDMAC) Channel Destination Microblock Stride (chid = 11) */
  #define REG_XDMAC_CIE12                      (0x40078350U) /**< \brief (XDMAC) Channel Interrupt Enable Register (chid = 12) */
  #define REG_XDMAC_CID12                      (0x40078354U) /**< \brief (XDMAC) Channel Interrupt Disable Register (chid = 12) */
  #define REG_XDMAC_CIM12                      (0x40078358U) /**< \brief (XDMAC) Channel Interrupt Mask Register (chid = 12) */
  #define REG_XDMAC_CIS12                      (0x4007835CU) /**< \brief (XDMAC) Channel Interrupt Status Register (chid = 12) */
  #define REG_XDMAC_CSA12                      (0x40078360U) /**< \brief (XDMAC) Channel Source Address Register (chid = 12) */
  #define REG_XDMAC_CDA12                      (0x40078364U) /**< \brief (XDMAC) Channel Destination Address Register (chid = 12) */
  #define REG_XDMAC_CNDA12                     (0x40078368U) /**< \brief (XDMAC) Channel Next Descriptor Address Register (chid = 12) */
  #define REG_XDMAC_CNDC12                     (0x4007836CU) /**< \brief (XDMAC) Channel Next Descriptor Control Register (chid = 12) */
  #define REG_XDMAC_CUBC12                     (0x40078370U) /**< \brief (XDMAC) Channel Microblock Control Register (chid = 12) */
  #define REG_XDMAC_CBC12                      (0x40078374U) /**< \brief (XDMAC) Channel Block Control Register (chid = 12) */
  #define REG_XDMAC_CC12                       (0x40078378U) /**< \brief (XDMAC) Channel Configuration Register (chid = 12) */
  #define REG_XDMAC_CDS_MSP12                  (0x4007837CU) /**< \brief (XDMAC) Channel Data Stride Memory Set Pattern (chid = 12) */
  #define REG_XDMAC_CSUS12                     (0x40078380U) /**< \brief (XDMAC) Channel Source Microblock Stride (chid = 12) */
  #define REG_XDMAC_CDUS12                     (0x40078384U) /**< \brief (XDMAC) Channel Destination Microblock Stride (chid = 12) */
  #define REG_XDMAC_CIE13                      (0x40078390U) /**< \brief (XDMAC) Channel Interrupt Enable Register (chid = 13) */
  #define REG_XDMAC_CID13                      (0x40078394U) /**< \brief (XDMAC) Channel Interrupt Disable Register (chid = 13) */
  #define REG_XDMAC_CIM13                      (0x40078398U) /**< \brief (XDMAC) Channel Interrupt Mask Register (chid = 13) */
  #define REG_XDMAC_CIS13                      (0x4007839CU) /**< \brief (XDMAC) Channel Interrupt Status Register (chid = 13) */
  #define REG_XDMAC_CSA13                      (0x400783A0U) /**< \brief (XDMAC) Channel Source Address Register (chid = 13) */
  #define REG_XDMAC_CDA13                      (0x400783A4U) /**< \brief (XDMAC) Channel Destination Address Register (chid = 13) */
  #define REG_XDMAC_CNDA13                     (0x400783A8U) /**< \brief (XDMAC) Channel Next Descriptor Address Register (chid = 13) */
  #define REG_XDMAC_CNDC13                     (0x400783ACU) /**< \brief (XDMAC) Channel Next Descriptor Control Register (chid = 13) */
  #define REG_XDMAC_CUBC13                     (0x400783B0U) /**< \brief (XDMAC) Channel Microblock Control Register (chid = 13) */
  #define REG_XDMAC_CBC13                      (0x400783B4U) /**< \brief (XDMAC) Channel Block Control Register (chid = 13) */
  #define REG_XDMAC_CC13                       (0x400783B8U) /**< \brief (XDMAC) Channel Configuration Register (chid = 13) */
  #define REG_XDMAC_CDS_MSP13                  (0x400783BCU) /**< \brief (XDMAC) Channel Data Stride Memory Set Pattern (chid = 13) */
  #define REG_XDMAC_CSUS13                     (0x400783C0U) /**< \brief (XDMAC) Channel Source Microblock Stride (chid = 13) */
  #define REG_XDMAC_CDUS13                     (0x400783C4U) /**< \brief (XDMAC) Channel Destination Microblock Stride (chid = 13) */
  #define REG_XDMAC_CIE14                      (0x400783D0U) /**< \brief (XDMAC) Channel Interrupt Enable Register (chid = 14) */
  #define REG_XDMAC_CID14                      (0x400783D4U) /**< \brief (XDMAC) Channel Interrupt Disable Register (chid = 14) */
  #define REG_XDMAC_CIM14                      (0x400783D8U) /**< \brief (XDMAC) Channel Interrupt Mask Register (chid = 14) */
  #define REG_XDMAC_CIS14                      (0x400783DCU) /**< \brief (XDMAC) Channel Interrupt Status Register (chid = 14) */
  #define REG_XDMAC_CSA14                      (0x400783E0U) /**< \brief (XDMAC) Channel Source Address Register (chid = 14) */
  #define REG_XDMAC_CDA14                      (0x400783E4U) /**< \brief (XDMAC) Channel Destination Address Register (chid = 14) */
  #define REG_XDMAC_CNDA14                     (0x400783E8U) /**< \brief (XDMAC) Channel Next Descriptor Address Register (chid = 14) */
  #define REG_XDMAC_CNDC14                     (0x400783ECU) /**< \brief (XDMAC) Channel Next Descriptor Control Register (chid = 14) */
  #define REG_XDMAC_CUBC14                     (0x400783F0U) /**< \brief (XDMAC) Channel Microblock Control Register (chid = 14) */
  #define REG_XDMAC_CBC14                      (0x400783F4U) /**< \brief (XDMAC) Channel Block Control Register (chid = 14) */
  #define REG_XDMAC_CC14                       (0x400783F8U) /**< \brief (XDMAC) Channel Configuration Register (chid = 14) */
  #define REG_XDMAC_CDS_MSP14                  (0x400783FCU) /**< \brief (XDMAC) Channel Data Stride Memory Set Pattern (chid = 14) */
  #define REG_XDMAC_CSUS14                     (0x40078400U) /**< \brief (XDMAC) Channel Source Microblock Stride (chid = 14) */
  #define REG_XDMAC_CDUS14                     (0x40078404U) /**< \brief (XDMAC) Channel Destination Microblock Stride (chid = 14) */
  #define REG_XDMAC_CIE15                      (0x40078410U) /**< \brief (XDMAC) Channel Interrupt Enable Register (chid = 15) */
  #define REG_XDMAC_CID15                      (0x40078414U) /**< \brief (XDMAC) Channel Interrupt Disable Register (chid = 15) */
  #define REG_XDMAC_CIM15                      (0x40078418U) /**< \brief (XDMAC) Channel Interrupt Mask Register (chid = 15) */
  #define REG_XDMAC_CIS15                      (0x4007841CU) /**< \brief (XDMAC) Channel Interrupt Status Register (chid = 15) */
  #define REG_XDMAC_CSA15                      (0x40078420U) /**< \brief (XDMAC) Channel Source Address Register (chid = 15) */
  #define REG_XDMAC_CDA15                      (0x40078424U) /**< \brief (XDMAC) Channel Destination Address Register (chid = 15) */
  #define REG_XDMAC_CNDA15                     (0x40078428U) /**< \brief (XDMAC) Channel Next Descriptor Address Register (chid = 15) */
  #define REG_XDMAC_CNDC15                     (0x4007842CU) /**< \brief (XDMAC) Channel Next Descriptor Control Register (chid = 15) */
  #define REG_XDMAC_CUBC15                     (0x40078430U) /**< \brief (XDMAC) Channel Microblock Control Register (chid = 15) */
  #define REG_XDMAC_CBC15                      (0x40078434U) /**< \brief (XDMAC) Channel Block Control Register (chid = 15) */
  #define REG_XDMAC_CC15                       (0x40078438U) /**< \brief (XDMAC) Channel Configuration Register (chid = 15) */
  #define REG_XDMAC_CDS_MSP15                  (0x4007843CU) /**< \brief (XDMAC) Channel Data Stride Memory Set Pattern (chid = 15) */
  #define REG_XDMAC_CSUS15                     (0x40078440U) /**< \brief (XDMAC) Channel Source Microblock Stride (chid = 15) */
  #define REG_XDMAC_CDUS15                     (0x40078444U) /**< \brief (XDMAC) Channel Destination Microblock Stride (chid = 15) */
  #define REG_XDMAC_CIE16                      (0x40078450U) /**< \brief (XDMAC) Channel Interrupt Enable Register (chid = 16) */
  #define REG_XDMAC_CID16                      (0x40078454U) /**< \brief (XDMAC) Channel Interrupt Disable Register (chid = 16) */
  #define REG_XDMAC_CIM16                      (0x40078458U) /**< \brief (XDMAC) Channel Interrupt Mask Register (chid = 16) */
  #define REG_XDMAC_CIS16                      (0x4007845CU) /**< \brief (XDMAC) Channel Interrupt Status Register (chid = 16) */
  #define REG_XDMAC_CSA16                      (0x40078460U) /**< \brief (XDMAC) Channel Source Address Register (chid = 16) */
  #define REG_XDMAC_CDA16                      (0x40078464U) /**< \brief (XDMAC) Channel Destination Address Register (chid = 16) */
  #define REG_XDMAC_CNDA16                     (0x40078468U) /**< \brief (XDMAC) Channel Next Descriptor Address Register (chid = 16) */
  #define REG_XDMAC_CNDC16                     (0x4007846CU) /**< \brief (XDMAC) Channel Next Descriptor Control Register (chid = 16) */
  #define REG_XDMAC_CUBC16                     (0x40078470U) /**< \brief (XDMAC) Channel Microblock Control Register (chid = 16) */
  #define REG_XDMAC_CBC16                      (0x40078474U) /**< \brief (XDMAC) Channel Block Control Register (chid = 16) */
  #define REG_XDMAC_CC16                       (0x40078478U) /**< \brief (XDMAC) Channel Configuration Register (chid = 16) */
  #define REG_XDMAC_CDS_MSP16                  (0x4007847CU) /**< \brief (XDMAC) Channel Data Stride Memory Set Pattern (chid = 16) */
  #define REG_XDMAC_CSUS16                     (0x40078480U) /**< \brief (XDMAC) Channel Source Microblock Stride (chid = 16) */
  #define REG_XDMAC_CDUS16                     (0x40078484U) /**< \brief (XDMAC) Channel Destination Microblock Stride (chid = 16) */
  #define REG_XDMAC_CIE17                      (0x40078490U) /**< \brief (XDMAC) Channel Interrupt Enable Register (chid = 17) */
  #define REG_XDMAC_CID17                      (0x40078494U) /**< \brief (XDMAC) Channel Interrupt Disable Register (chid = 17) */
  #define REG_XDMAC_CIM17                      (0x40078498U) /**< \brief (XDMAC) Channel Interrupt Mask Register (chid = 17) */
  #define REG_XDMAC_CIS17                      (0x4007849CU) /**< \brief (XDMAC) Channel Interrupt Status Register (chid = 17) */
  #define REG_XDMAC_CSA17                      (0x400784A0U) /**< \brief (XDMAC) Channel Source Address Register (chid = 17) */
  #define REG_XDMAC_CDA17                      (0x400784A4U) /**< \brief (XDMAC) Channel Destination Address Register (chid = 17) */
  #define REG_XDMAC_CNDA17                     (0x400784A8U) /**< \brief (XDMAC) Channel Next Descriptor Address Register (chid = 17) */
  #define REG_XDMAC_CNDC17                     (0x400784ACU) /**< \brief (XDMAC) Channel Next Descriptor Control Register (chid = 17) */
  #define REG_XDMAC_CUBC17                     (0x400784B0U) /**< \brief (XDMAC) Channel Microblock Control Register (chid = 17) */
  #define REG_XDMAC_CBC17                      (0x400784B4U) /**< \brief (XDMAC) Channel Block Control Register (chid = 17) */
  #define REG_XDMAC_CC17                       (0x400784B8U) /**< \brief (XDMAC) Channel Configuration Register (chid = 17) */
  #define REG_XDMAC_CDS_MSP17                  (0x400784BCU) /**< \brief (XDMAC) Channel Data Stride Memory Set Pattern (chid = 17) */
  #define REG_XDMAC_CSUS17                     (0x400784C0U) /**< \brief (XDMAC) Channel Source Microblock Stride (chid = 17) */
  #define REG_XDMAC_CDUS17                     (0x400784C4U) /**< \brief (XDMAC) Channel Destination Microblock Stride (chid = 17) */
  #define REG_XDMAC_CIE18                      (0x400784D0U) /**< \brief (XDMAC) Channel Interrupt Enable Register (chid = 18) */
  #define REG_XDMAC_CID18                      (0x400784D4U) /**< \brief (XDMAC) Channel Interrupt Disable Register (chid = 18) */
  #define REG_XDMAC_CIM18                      (0x400784D8U) /**< \brief (XDMAC) Channel Interrupt Mask Register (chid = 18) */
  #define REG_XDMAC_CIS18                      (0x400784DCU) /**< \brief (XDMAC) Channel Interrupt Status Register (chid = 18) */
  #define REG_XDMAC_CSA18                      (0x400784E0U) /**< \brief (XDMAC) Channel Source Address Register (chid = 18) */
  #define REG_XDMAC_CDA18                      (0x400784E4U) /**< \brief (XDMAC) Channel Destination Address Register (chid = 18) */
  #define REG_XDMAC_CNDA18                     (0x400784E8U) /**< \brief (XDMAC) Channel Next Descriptor Address Register (chid = 18) */
  #define REG_XDMAC_CNDC18                     (0x400784ECU) /**< \brief (XDMAC) Channel Next Descriptor Control Register (chid = 18) */
  #define REG_XDMAC_CUBC18                     (0x400784F0U) /**< \brief (XDMAC) Channel Microblock Control Register (chid = 18) */
  #define REG_XDMAC_CBC18                      (0x400784F4U) /**< \brief (XDMAC) Channel Block Control Register (chid = 18) */
  #define REG_XDMAC_CC18                       (0x400784F8U) /**< \brief (XDMAC) Channel Configuration Register (chid = 18) */
  #define REG_XDMAC_CDS_MSP18                  (0x400784FCU) /**< \brief (XDMAC) Channel Data Stride Memory Set Pattern (chid = 18) */
  #define REG_XDMAC_CSUS18                     (0x40078500U) /**< \brief (XDMAC) Channel Source Microblock Stride (chid = 18) */
  #define REG_XDMAC_CDUS18                     (0x40078504U) /**< \brief (XDMAC) Channel Destination Microblock Stride (chid = 18) */
  #define REG_XDMAC_CIE19                      (0x40078510U) /**< \brief (XDMAC) Channel Interrupt Enable Register (chid = 19) */
  #define REG_XDMAC_CID19                      (0x40078514U) /**< \brief (XDMAC) Channel Interrupt Disable Register (chid = 19) */
  #define REG_XDMAC_CIM19                      (0x40078518U) /**< \brief (XDMAC) Channel Interrupt Mask Register (chid = 19) */
  #define REG_XDMAC_CIS19                      (0x4007851CU) /**< \brief (XDMAC) Channel Interrupt Status Register (chid = 19) */
  #define REG_XDMAC_CSA19                      (0x40078520U) /**< \brief (XDMAC) Channel Source Address Register (chid = 19) */
  #define REG_XDMAC_CDA19                      (0x40078524U) /**< \brief (XDMAC) Channel Destination Address Register (chid = 19) */
  #define REG_XDMAC_CNDA19                     (0x40078528U) /**< \brief (XDMAC) Channel Next Descriptor Address Register (chid = 19) */
  #define REG_XDMAC_CNDC19                     (0x4007852CU) /**< \brief (XDMAC) Channel Next Descriptor Control Register (chid = 19) */
  #define REG_XDMAC_CUBC19                     (0x40078530U) /**< \brief (XDMAC) Channel Microblock Control Register (chid = 19) */
  #define REG_XDMAC_CBC19                      (0x40078534U) /**< \brief (XDMAC) Channel Block Control Register (chid = 19) */
  #define REG_XDMAC_CC19                       (0x40078538U) /**< \brief (XDMAC) Channel Configuration Register (chid = 19) */
  #define REG_XDMAC_CDS_MSP19                  (0x4007853CU) /**< \brief (XDMAC) Channel Data Stride Memory Set Pattern (chid = 19) */
  #define REG_XDMAC_CSUS19                     (0x40078540U) /**< \brief (XDMAC) Channel Source Microblock Stride (chid = 19) */
  #define REG_XDMAC_CDUS19                     (0x40078544U) /**< \brief (XDMAC) Channel Destination Microblock Stride (chid = 19) */
  #define REG_XDMAC_CIE20                      (0x40078550U) /**< \brief (XDMAC) Channel Interrupt Enable Register (chid = 20) */
  #define REG_XDMAC_CID20                      (0x40078554U) /**< \brief (XDMAC) Channel Interrupt Disable Register (chid = 20) */
  #define REG_XDMAC_CIM20                      (0x40078558U) /**< \brief (XDMAC) Channel Interrupt Mask Register (chid = 20) */
  #define REG_XDMAC_CIS20                      (0x4007855CU) /**< \brief (XDMAC) Channel Interrupt Status Register (chid = 20) */
  #define REG_XDMAC_CSA20                      (0x40078560U) /**< \brief (XDMAC) Channel Source Address Register (chid = 20) */
  #define REG_XDMAC_CDA20                      (0x40078564U) /**< \brief (XDMAC) Channel Destination Address Register (chid = 20) */
  #define REG_XDMAC_CNDA20                     (0x40078568U) /**< \brief (XDMAC) Channel Next Descriptor Address Register (chid = 20) */
  #define REG_XDMAC_CNDC20                     (0x4007856CU) /**< \brief (XDMAC) Channel Next Descriptor Control Register (chid = 20) */
  #define REG_XDMAC_CUBC20                     (0x40078570U) /**< \brief (XDMAC) Channel Microblock Control Register (chid = 20) */
  #define REG_XDMAC_CBC20                      (0x40078574U) /**< \brief (XDMAC) Channel Block Control Register (chid = 20) */
  #define REG_XDMAC_CC20                       (0x40078578U) /**< \brief (XDMAC) Channel Configuration Register (chid = 20) */
  #define REG_XDMAC_CDS_MSP20                  (0x4007857CU) /**< \brief (XDMAC) Channel Data Stride Memory Set Pattern (chid = 20) */
  #define REG_XDMAC_CSUS20                     (0x40078580U) /**< \brief (XDMAC) Channel Source Microblock Stride (chid = 20) */
  #define REG_XDMAC_CDUS20                     (0x40078584U) /**< \brief (XDMAC) Channel Destination Microblock Stride (chid = 20) */
  #define REG_XDMAC_CIE21                      (0x40078590U) /**< \brief (XDMAC) Channel Interrupt Enable Register (chid = 21) */
  #define REG_XDMAC_CID21                      (0x40078594U) /**< \brief (XDMAC) Channel Interrupt Disable Register (chid = 21) */
  #define REG_XDMAC_CIM21                      (0x40078598U) /**< \brief (XDMAC) Channel Interrupt Mask Register (chid = 21) */
  #define REG_XDMAC_CIS21                      (0x4007859CU) /**< \brief (XDMAC) Channel Interrupt Status Register (chid = 21) */
  #define REG_XDMAC_CSA21                      (0x400785A0U) /**< \brief (XDMAC) Channel Source Address Register (chid = 21) */
  #define REG_XDMAC_CDA21                      (0x400785A4U) /**< \brief (XDMAC) Channel Destination Address Register (chid = 21) */
  #define REG_XDMAC_CNDA21                     (0x400785A8U) /**< \brief (XDMAC) Channel Next Descriptor Address Register (chid = 21) */
  #define REG_XDMAC_CNDC21                     (0x400785ACU) /**< \brief (XDMAC) Channel Next Descriptor Control Register (chid = 21) */
  #define REG_XDMAC_CUBC21                     (0x400785B0U) /**< \brief (XDMAC) Channel Microblock Control Register (chid = 21) */
  #define REG_XDMAC_CBC21                      (0x400785B4U) /**< \brief (XDMAC) Channel Block Control Register (chid = 21) */
  #define REG_XDMAC_CC21                       (0x400785B8U) /**< \brief (XDMAC) Channel Configuration Register (chid = 21) */
  #define REG_XDMAC_CDS_MSP21                  (0x400785BCU) /**< \brief (XDMAC) Channel Data Stride Memory Set Pattern (chid = 21) */
  #define REG_XDMAC_CSUS21                     (0x400785C0U) /**< \brief (XDMAC) Channel Source Microblock Stride (chid = 21) */
  #define REG_XDMAC_CDUS21                     (0x400785C4U) /**< \brief (XDMAC) Channel Destination Microblock Stride (chid = 21) */
  #define REG_XDMAC_CIE22                      (0x400785D0U) /**< \brief (XDMAC) Channel Interrupt Enable Register (chid = 22) */
  #define REG_XDMAC_CID22                      (0x400785D4U) /**< \brief (XDMAC) Channel Interrupt Disable Register (chid = 22) */
  #define REG_XDMAC_CIM22                      (0x400785D8U) /**< \brief (XDMAC) Channel Interrupt Mask Register (chid = 22) */
  #define REG_XDMAC_CIS22                      (0x400785DCU) /**< \brief (XDMAC) Channel Interrupt Status Register (chid = 22) */
  #define REG_XDMAC_CSA22                      (0x400785E0U) /**< \brief (XDMAC) Channel Source Address Register (chid = 22) */
  #define REG_XDMAC_CDA22                      (0x400785E4U) /**< \brief (XDMAC) Channel Destination Address Register (chid = 22) */
  #define REG_XDMAC_CNDA22                     (0x400785E8U) /**< \brief (XDMAC) Channel Next Descriptor Address Register (chid = 22) */
  #define REG_XDMAC_CNDC22                     (0x400785ECU) /**< \brief (XDMAC) Channel Next Descriptor Control Register (chid = 22) */
  #define REG_XDMAC_CUBC22                     (0x400785F0U) /**< \brief (XDMAC) Channel Microblock Control Register (chid = 22) */
  #define REG_XDMAC_CBC22                      (0x400785F4U) /**< \brief (XDMAC) Channel Block Control Register (chid = 22) */
  #define REG_XDMAC_CC22                       (0x400785F8U) /**< \brief (XDMAC) Channel Configuration Register (chid = 22) */
  #define REG_XDMAC_CDS_MSP22                  (0x400785FCU) /**< \brief (XDMAC) Channel Data Stride Memory Set Pattern (chid = 22) */
  #define REG_XDMAC_CSUS22                     (0x40078600U) /**< \brief (XDMAC) Channel Source Microblock Stride (chid = 22) */
  #define REG_XDMAC_CDUS22                     (0x40078604U) /**< \brief (XDMAC) Channel Destination Microblock Stride (chid = 22) */
  #define REG_XDMAC_CIE23                      (0x40078610U) /**< \brief (XDMAC) Channel Interrupt Enable Register (chid = 23) */
  #define REG_XDMAC_CID23                      (0x40078614U) /**< \brief (XDMAC) Channel Interrupt Disable Register (chid = 23) */
  #define REG_XDMAC_CIM23                      (0x40078618U) /**< \brief (XDMAC) Channel Interrupt Mask Register (chid = 23) */
  #define REG_XDMAC_CIS23                      (0x4007861CU) /**< \brief (XDMAC) Channel Interrupt Status Register (chid = 23) */
  #define REG_XDMAC_CSA23                      (0x40078620U) /**< \brief (XDMAC) Channel Source Address Register (chid = 23) */
  #define REG_XDMAC_CDA23                      (0x40078624U) /**< \brief (XDMAC) Channel Destination Address Register (chid = 23) */
  #define REG_XDMAC_CNDA23                     (0x40078628U) /**< \brief (XDMAC) Channel Next Descriptor Address Register (chid = 23) */
  #define REG_XDMAC_CNDC23                     (0x4007862CU) /**< \brief (XDMAC) Channel Next Descriptor Control Register (chid = 23) */
  #define REG_XDMAC_CUBC23                     (0x40078630U) /**< \brief (XDMAC) Channel Microblock Control Register (chid = 23) */
  #define REG_XDMAC_CBC23                      (0x40078634U) /**< \brief (XDMAC) Channel Block Control Register (chid = 23) */
  #define REG_XDMAC_CC23                       (0x40078638U) /**< \brief (XDMAC) Channel Configuration Register (chid = 23) */
  #define REG_XDMAC_CDS_MSP23                  (0x4007863CU) /**< \brief (XDMAC) Channel Data Stride Memory Set Pattern (chid = 23) */
  #define REG_XDMAC_CSUS23                     (0x40078640U) /**< \brief (XDMAC) Channel Source Microblock Stride (chid = 23) */
  #define REG_XDMAC_CDUS23                     (0x40078644U) /**< \brief (XDMAC) Channel Destination Microblock Stride (chid = 23) */
  #define REG_XDMAC_VERSION                    (0x40078FFCU) /**< \brief (XDMAC) XDMAC Version Register */
#else
  #define REG_XDMAC_GTYPE     (*(volatile  uint32_t*)0x40078000U) /**< \brief (XDMAC) Global Type Register */
  #define REG_XDMAC_GCFG      (*(volatile uint32_t*)0x40078004U) /**< \brief (XDMAC) Global Configuration Register */
  #define REG_XDMAC_GWAC      (*(volatile uint32_t*)0x40078008U) /**< \brief (XDMAC) Global Weighted Arbiter Configuration Register */
  #define REG_XDMAC_GIE       (*(volatile  uint32_t*)0x4007800CU) /**< \brief (XDMAC) Global Interrupt Enable Register */
  #define REG_XDMAC_GID       (*(volatile  uint32_t*)0x40078010U) /**< \brief (XDMAC) Global Interrupt Disable Register */
  #define REG_XDMAC_GIM       (*(volatile  uint32_t*)0x40078014U) /**< \brief (XDMAC) Global Interrupt Mask Register */
  #define REG_XDMAC_GIS       (*(volatile  uint32_t*)0x40078018U) /**< \brief (XDMAC) Global Interrupt Status Register */
  #define REG_XDMAC_GE        (*(volatile  uint32_t*)0x4007801CU) /**< \brief (XDMAC) Global Channel Enable Register */
  #define REG_XDMAC_GD        (*(volatile  uint32_t*)0x40078020U) /**< \brief (XDMAC) Global Channel Disable Register */
  #define REG_XDMAC_GS        (*(volatile  uint32_t*)0x40078024U) /**< \brief (XDMAC) Global Channel Status Register */
  #define REG_XDMAC_GRS       (*(volatile uint32_t*)0x40078028U) /**< \brief (XDMAC) Global Channel Read Suspend Register */
  #define REG_XDMAC_GWS       (*(volatile uint32_t*)0x4007802CU) /**< \brief (XDMAC) Global Channel Write Suspend Register */
  #define REG_XDMAC_GRWS      (*(volatile  uint32_t*)0x40078030U) /**< \brief (XDMAC) Global Channel Read Write Suspend Register */
  #define REG_XDMAC_GRWR      (*(volatile  uint32_t*)0x40078034U) /**< \brief (XDMAC) Global Channel Read Write Resume Register */
  #define REG_XDMAC_GSWR      (*(volatile  uint32_t*)0x40078038U) /**< \brief (XDMAC) Global Channel Software Request Register */
  #define REG_XDMAC_GSWS      (*(volatile  uint32_t*)0x4007803CU) /**< \brief (XDMAC) Global Channel Software Request Status Register */
  #define REG_XDMAC_GSWF      (*(volatile  uint32_t*)0x40078040U) /**< \brief (XDMAC) Global Channel Software Flush Request Register */
  #define REG_XDMAC_CIE0      (*(volatile  uint32_t*)0x40078050U) /**< \brief (XDMAC) Channel Interrupt Enable Register (chid = 0) */
  #define REG_XDMAC_CID0      (*(volatile  uint32_t*)0x40078054U) /**< \brief (XDMAC) Channel Interrupt Disable Register (chid = 0) */
  #define REG_XDMAC_CIM0      (*(volatile  uint32_t*)0x40078058U) /**< \brief (XDMAC) Channel Interrupt Mask Register (chid = 0) */
  #define REG_XDMAC_CIS0      (*(volatile  uint32_t*)0x4007805CU) /**< \brief (XDMAC) Channel Interrupt Status Register (chid = 0) */
  #define REG_XDMAC_CSA0      (*(volatile uint32_t*)0x40078060U) /**< \brief (XDMAC) Channel Source Address Register (chid = 0) */
  #define REG_XDMAC_CDA0      (*(volatile uint32_t*)0x40078064U) /**< \brief (XDMAC) Channel Destination Address Register (chid = 0) */
  #define REG_XDMAC_CNDA0     (*(volatile uint32_t*)0x40078068U) /**< \brief (XDMAC) Channel Next Descriptor Address Register (chid = 0) */
  #define REG_XDMAC_CNDC0     (*(volatile uint32_t*)0x4007806CU) /**< \brief (XDMAC) Channel Next Descriptor Control Register (chid = 0) */
  #define REG_XDMAC_CUBC0     (*(volatile uint32_t*)0x40078070U) /**< \brief (XDMAC) Channel Microblock Control Register (chid = 0) */
  #define REG_XDMAC_CBC0      (*(volatile uint32_t*)0x40078074U) /**< \brief (XDMAC) Channel Block Control Register (chid = 0) */
  #define REG_XDMAC_CC0       (*(volatile uint32_t*)0x40078078U) /**< \brief (XDMAC) Channel Configuration Register (chid = 0) */
  #define REG_XDMAC_CDS_MSP0  (*(volatile uint32_t*)0x4007807CU) /**< \brief (XDMAC) Channel Data Stride Memory Set Pattern (chid = 0) */
  #define REG_XDMAC_CSUS0     (*(volatile uint32_t*)0x40078080U) /**< \brief (XDMAC) Channel Source Microblock Stride (chid = 0) */
  #define REG_XDMAC_CDUS0     (*(volatile uint32_t*)0x40078084U) /**< \brief (XDMAC) Channel Destination Microblock Stride (chid = 0) */
  #define REG_XDMAC_CIE1      (*(volatile  uint32_t*)0x40078090U) /**< \brief (XDMAC) Channel Interrupt Enable Register (chid = 1) */
  #define REG_XDMAC_CID1      (*(volatile  uint32_t*)0x40078094U) /**< \brief (XDMAC) Channel Interrupt Disable Register (chid = 1) */
  #define REG_XDMAC_CIM1      (*(volatile  uint32_t*)0x40078098U) /**< \brief (XDMAC) Channel Interrupt Mask Register (chid = 1) */
  #define REG_XDMAC_CIS1      (*(volatile  uint32_t*)0x4007809CU) /**< \brief (XDMAC) Channel Interrupt Status Register (chid = 1) */
  #define REG_XDMAC_CSA1      (*(volatile uint32_t*)0x400780A0U) /**< \brief (XDMAC) Channel Source Address Register (chid = 1) */
  #define REG_XDMAC_CDA1      (*(volatile uint32_t*)0x400780A4U) /**< \brief (XDMAC) Channel Destination Address Register (chid = 1) */
  #define REG_XDMAC_CNDA1     (*(volatile uint32_t*)0x400780A8U) /**< \brief (XDMAC) Channel Next Descriptor Address Register (chid = 1) */
  #define REG_XDMAC_CNDC1     (*(volatile uint32_t*)0x400780ACU) /**< \brief (XDMAC) Channel Next Descriptor Control Register (chid = 1) */
  #define REG_XDMAC_CUBC1     (*(volatile uint32_t*)0x400780B0U) /**< \brief (XDMAC) Channel Microblock Control Register (chid = 1) */
  #define REG_XDMAC_CBC1      (*(volatile uint32_t*)0x400780B4U) /**< \brief (XDMAC) Channel Block Control Register (chid = 1) */
  #define REG_XDMAC_CC1       (*(volatile uint32_t*)0x400780B8U) /**< \brief (XDMAC) Channel Configuration Register (chid = 1) */
  #define REG_XDMAC_CDS_MSP1  (*(volatile uint32_t*)0x400780BCU) /**< \brief (XDMAC) Channel Data Stride Memory Set Pattern (chid = 1) */
  #define REG_XDMAC_CSUS1     (*(volatile uint32_t*)0x400780C0U) /**< \brief (XDMAC) Channel Source Microblock Stride (chid = 1) */
  #define REG_XDMAC_CDUS1     (*(volatile uint32_t*)0x400780C4U) /**< \brief (XDMAC) Channel Destination Microblock Stride (chid = 1) */
  #define REG_XDMAC_CIE2      (*(volatile  uint32_t*)0x400780D0U) /**< \brief (XDMAC) Channel Interrupt Enable Register (chid = 2) */
  #define REG_XDMAC_CID2      (*(volatile  uint32_t*)0x400780D4U) /**< \brief (XDMAC) Channel Interrupt Disable Register (chid = 2) */
  #define REG_XDMAC_CIM2      (*(volatile  uint32_t*)0x400780D8U) /**< \brief (XDMAC) Channel Interrupt Mask Register (chid = 2) */
  #define REG_XDMAC_CIS2      (*(volatile  uint32_t*)0x400780DCU) /**< \brief (XDMAC) Channel Interrupt Status Register (chid = 2) */
  #define REG_XDMAC_CSA2      (*(volatile uint32_t*)0x400780E0U) /**< \brief (XDMAC) Channel Source Address Register (chid = 2) */
  #define REG_XDMAC_CDA2      (*(volatile uint32_t*)0x400780E4U) /**< \brief (XDMAC) Channel Destination Address Register (chid = 2) */
  #define REG_XDMAC_CNDA2     (*(volatile uint32_t*)0x400780E8U) /**< \brief (XDMAC) Channel Next Descriptor Address Register (chid = 2) */
  #define REG_XDMAC_CNDC2     (*(volatile uint32_t*)0x400780ECU) /**< \brief (XDMAC) Channel Next Descriptor Control Register (chid = 2) */
  #define REG_XDMAC_CUBC2     (*(volatile uint32_t*)0x400780F0U) /**< \brief (XDMAC) Channel Microblock Control Register (chid = 2) */
  #define REG_XDMAC_CBC2      (*(volatile uint32_t*)0x400780F4U) /**< \brief (XDMAC) Channel Block Control Register (chid = 2) */
  #define REG_XDMAC_CC2       (*(volatile uint32_t*)0x400780F8U) /**< \brief (XDMAC) Channel Configuration Register (chid = 2) */
  #define REG_XDMAC_CDS_MSP2  (*(volatile uint32_t*)0x400780FCU) /**< \brief (XDMAC) Channel Data Stride Memory Set Pattern (chid = 2) */
  #define REG_XDMAC_CSUS2     (*(volatile uint32_t*)0x40078100U) /**< \brief (XDMAC) Channel Source Microblock Stride (chid = 2) */
  #define REG_XDMAC_CDUS2     (*(volatile uint32_t*)0x40078104U) /**< \brief (XDMAC) Channel Destination Microblock Stride (chid = 2) */
  #define REG_XDMAC_CIE3      (*(volatile  uint32_t*)0x40078110U) /**< \brief (XDMAC) Channel Interrupt Enable Register (chid = 3) */
  #define REG_XDMAC_CID3      (*(volatile  uint32_t*)0x40078114U) /**< \brief (XDMAC) Channel Interrupt Disable Register (chid = 3) */
  #define REG_XDMAC_CIM3      (*(volatile  uint32_t*)0x40078118U) /**< \brief (XDMAC) Channel Interrupt Mask Register (chid = 3) */
  #define REG_XDMAC_CIS3      (*(volatile  uint32_t*)0x4007811CU) /**< \brief (XDMAC) Channel Interrupt Status Register (chid = 3) */
  #define REG_XDMAC_CSA3      (*(volatile uint32_t*)0x40078120U) /**< \brief (XDMAC) Channel Source Address Register (chid = 3) */
  #define REG_XDMAC_CDA3      (*(volatile uint32_t*)0x40078124U) /**< \brief (XDMAC) Channel Destination Address Register (chid = 3) */
  #define REG_XDMAC_CNDA3     (*(volatile uint32_t*)0x40078128U) /**< \brief (XDMAC) Channel Next Descriptor Address Register (chid = 3) */
  #define REG_XDMAC_CNDC3     (*(volatile uint32_t*)0x4007812CU) /**< \brief (XDMAC) Channel Next Descriptor Control Register (chid = 3) */
  #define REG_XDMAC_CUBC3     (*(volatile uint32_t*)0x40078130U) /**< \brief (XDMAC) Channel Microblock Control Register (chid = 3) */
  #define REG_XDMAC_CBC3      (*(volatile uint32_t*)0x40078134U) /**< \brief (XDMAC) Channel Block Control Register (chid = 3) */
  #define REG_XDMAC_CC3       (*(volatile uint32_t*)0x40078138U) /**< \brief (XDMAC) Channel Configuration Register (chid = 3) */
  #define REG_XDMAC_CDS_MSP3  (*(volatile uint32_t*)0x4007813CU) /**< \brief (XDMAC) Channel Data Stride Memory Set Pattern (chid = 3) */
  #define REG_XDMAC_CSUS3     (*(volatile uint32_t*)0x40078140U) /**< \brief (XDMAC) Channel Source Microblock Stride (chid = 3) */
  #define REG_XDMAC_CDUS3     (*(volatile uint32_t*)0x40078144U) /**< \brief (XDMAC) Channel Destination Microblock Stride (chid = 3) */
  #define REG_XDMAC_CIE4      (*(volatile  uint32_t*)0x40078150U) /**< \brief (XDMAC) Channel Interrupt Enable Register (chid = 4) */
  #define REG_XDMAC_CID4      (*(volatile  uint32_t*)0x40078154U) /**< \brief (XDMAC) Channel Interrupt Disable Register (chid = 4) */
  #define REG_XDMAC_CIM4      (*(volatile  uint32_t*)0x40078158U) /**< \brief (XDMAC) Channel Interrupt Mask Register (chid = 4) */
  #define REG_XDMAC_CIS4      (*(volatile  uint32_t*)0x4007815CU) /**< \brief (XDMAC) Channel Interrupt Status Register (chid = 4) */
  #define REG_XDMAC_CSA4      (*(volatile uint32_t*)0x40078160U) /**< \brief (XDMAC) Channel Source Address Register (chid = 4) */
  #define REG_XDMAC_CDA4      (*(volatile uint32_t*)0x40078164U) /**< \brief (XDMAC) Channel Destination Address Register (chid = 4) */
  #define REG_XDMAC_CNDA4     (*(volatile uint32_t*)0x40078168U) /**< \brief (XDMAC) Channel Next Descriptor Address Register (chid = 4) */
  #define REG_XDMAC_CNDC4     (*(volatile uint32_t*)0x4007816CU) /**< \brief (XDMAC) Channel Next Descriptor Control Register (chid = 4) */
  #define REG_XDMAC_CUBC4     (*(volatile uint32_t*)0x40078170U) /**< \brief (XDMAC) Channel Microblock Control Register (chid = 4) */
  #define REG_XDMAC_CBC4      (*(volatile uint32_t*)0x40078174U) /**< \brief (XDMAC) Channel Block Control Register (chid = 4) */
  #define REG_XDMAC_CC4       (*(volatile uint32_t*)0x40078178U) /**< \brief (XDMAC) Channel Configuration Register (chid = 4) */
  #define REG_XDMAC_CDS_MSP4  (*(volatile uint32_t*)0x4007817CU) /**< \brief (XDMAC) Channel Data Stride Memory Set Pattern (chid = 4) */
  #define REG_XDMAC_CSUS4     (*(volatile uint32_t*)0x40078180U) /**< \brief (XDMAC) Channel Source Microblock Stride (chid = 4) */
  #define REG_XDMAC_CDUS4     (*(volatile uint32_t*)0x40078184U) /**< \brief (XDMAC) Channel Destination Microblock Stride (chid = 4) */
  #define REG_XDMAC_CIE5      (*(volatile  uint32_t*)0x40078190U) /**< \brief (XDMAC) Channel Interrupt Enable Register (chid = 5) */
  #define REG_XDMAC_CID5      (*(volatile  uint32_t*)0x40078194U) /**< \brief (XDMAC) Channel Interrupt Disable Register (chid = 5) */
  #define REG_XDMAC_CIM5      (*(volatile  uint32_t*)0x40078198U) /**< \brief (XDMAC) Channel Interrupt Mask Register (chid = 5) */
  #define REG_XDMAC_CIS5      (*(volatile  uint32_t*)0x4007819CU) /**< \brief (XDMAC) Channel Interrupt Status Register (chid = 5) */
  #define REG_XDMAC_CSA5      (*(volatile uint32_t*)0x400781A0U) /**< \brief (XDMAC) Channel Source Address Register (chid = 5) */
  #define REG_XDMAC_CDA5      (*(volatile uint32_t*)0x400781A4U) /**< \brief (XDMAC) Channel Destination Address Register (chid = 5) */
  #define REG_XDMAC_CNDA5     (*(volatile uint32_t*)0x400781A8U) /**< \brief (XDMAC) Channel Next Descriptor Address Register (chid = 5) */
  #define REG_XDMAC_CNDC5     (*(volatile uint32_t*)0x400781ACU) /**< \brief (XDMAC) Channel Next Descriptor Control Register (chid = 5) */
  #define REG_XDMAC_CUBC5     (*(volatile uint32_t*)0x400781B0U) /**< \brief (XDMAC) Channel Microblock Control Register (chid = 5) */
  #define REG_XDMAC_CBC5      (*(volatile uint32_t*)0x400781B4U) /**< \brief (XDMAC) Channel Block Control Register (chid = 5) */
  #define REG_XDMAC_CC5       (*(volatile uint32_t*)0x400781B8U) /**< \brief (XDMAC) Channel Configuration Register (chid = 5) */
  #define REG_XDMAC_CDS_MSP5  (*(volatile uint32_t*)0x400781BCU) /**< \brief (XDMAC) Channel Data Stride Memory Set Pattern (chid = 5) */
  #define REG_XDMAC_CSUS5     (*(volatile uint32_t*)0x400781C0U) /**< \brief (XDMAC) Channel Source Microblock Stride (chid = 5) */
  #define REG_XDMAC_CDUS5     (*(volatile uint32_t*)0x400781C4U) /**< \brief (XDMAC) Channel Destination Microblock Stride (chid = 5) */
  #define REG_XDMAC_CIE6      (*(volatile  uint32_t*)0x400781D0U) /**< \brief (XDMAC) Channel Interrupt Enable Register (chid = 6) */
  #define REG_XDMAC_CID6      (*(volatile  uint32_t*)0x400781D4U) /**< \brief (XDMAC) Channel Interrupt Disable Register (chid = 6) */
  #define REG_XDMAC_CIM6      (*(volatile  uint32_t*)0x400781D8U) /**< \brief (XDMAC) Channel Interrupt Mask Register (chid = 6) */
  #define REG_XDMAC_CIS6      (*(volatile  uint32_t*)0x400781DCU) /**< \brief (XDMAC) Channel Interrupt Status Register (chid = 6) */
  #define REG_XDMAC_CSA6      (*(volatile uint32_t*)0x400781E0U) /**< \brief (XDMAC) Channel Source Address Register (chid = 6) */
  #define REG_XDMAC_CDA6      (*(volatile uint32_t*)0x400781E4U) /**< \brief (XDMAC) Channel Destination Address Register (chid = 6) */
  #define REG_XDMAC_CNDA6     (*(volatile uint32_t*)0x400781E8U) /**< \brief (XDMAC) Channel Next Descriptor Address Register (chid = 6) */
  #define REG_XDMAC_CNDC6     (*(volatile uint32_t*)0x400781ECU) /**< \brief (XDMAC) Channel Next Descriptor Control Register (chid = 6) */
  #define REG_XDMAC_CUBC6     (*(volatile uint32_t*)0x400781F0U) /**< \brief (XDMAC) Channel Microblock Control Register (chid = 6) */
  #define REG_XDMAC_CBC6      (*(volatile uint32_t*)0x400781F4U) /**< \brief (XDMAC) Channel Block Control Register (chid = 6) */
  #define REG_XDMAC_CC6       (*(volatile uint32_t*)0x400781F8U) /**< \brief (XDMAC) Channel Configuration Register (chid = 6) */
  #define REG_XDMAC_CDS_MSP6  (*(volatile uint32_t*)0x400781FCU) /**< \brief (XDMAC) Channel Data Stride Memory Set Pattern (chid = 6) */
  #define REG_XDMAC_CSUS6     (*(volatile uint32_t*)0x40078200U) /**< \brief (XDMAC) Channel Source Microblock Stride (chid = 6) */
  #define REG_XDMAC_CDUS6     (*(volatile uint32_t*)0x40078204U) /**< \brief (XDMAC) Channel Destination Microblock Stride (chid = 6) */
  #define REG_XDMAC_CIE7      (*(volatile  uint32_t*)0x40078210U) /**< \brief (XDMAC) Channel Interrupt Enable Register (chid = 7) */
  #define REG_XDMAC_CID7      (*(volatile  uint32_t*)0x40078214U) /**< \brief (XDMAC) Channel Interrupt Disable Register (chid = 7) */
  #define REG_XDMAC_CIM7      (*(volatile  uint32_t*)0x40078218U) /**< \brief (XDMAC) Channel Interrupt Mask Register (chid = 7) */
  #define REG_XDMAC_CIS7      (*(volatile  uint32_t*)0x4007821CU) /**< \brief (XDMAC) Channel Interrupt Status Register (chid = 7) */
  #define REG_XDMAC_CSA7      (*(volatile uint32_t*)0x40078220U) /**< \brief (XDMAC) Channel Source Address Register (chid = 7) */
  #define REG_XDMAC_CDA7      (*(volatile uint32_t*)0x40078224U) /**< \brief (XDMAC) Channel Destination Address Register (chid = 7) */
  #define REG_XDMAC_CNDA7     (*(volatile uint32_t*)0x40078228U) /**< \brief (XDMAC) Channel Next Descriptor Address Register (chid = 7) */
  #define REG_XDMAC_CNDC7     (*(volatile uint32_t*)0x4007822CU) /**< \brief (XDMAC) Channel Next Descriptor Control Register (chid = 7) */
  #define REG_XDMAC_CUBC7     (*(volatile uint32_t*)0x40078230U) /**< \brief (XDMAC) Channel Microblock Control Register (chid = 7) */
  #define REG_XDMAC_CBC7      (*(volatile uint32_t*)0x40078234U) /**< \brief (XDMAC) Channel Block Control Register (chid = 7) */
  #define REG_XDMAC_CC7       (*(volatile uint32_t*)0x40078238U) /**< \brief (XDMAC) Channel Configuration Register (chid = 7) */
  #define REG_XDMAC_CDS_MSP7  (*(volatile uint32_t*)0x4007823CU) /**< \brief (XDMAC) Channel Data Stride Memory Set Pattern (chid = 7) */
  #define REG_XDMAC_CSUS7     (*(volatile uint32_t*)0x40078240U) /**< \brief (XDMAC) Channel Source Microblock Stride (chid = 7) */
  #define REG_XDMAC_CDUS7     (*(volatile uint32_t*)0x40078244U) /**< \brief (XDMAC) Channel Destination Microblock Stride (chid = 7) */
  #define REG_XDMAC_CIE8      (*(volatile  uint32_t*)0x40078250U) /**< \brief (XDMAC) Channel Interrupt Enable Register (chid = 8) */
  #define REG_XDMAC_CID8      (*(volatile  uint32_t*)0x40078254U) /**< \brief (XDMAC) Channel Interrupt Disable Register (chid = 8) */
  #define REG_XDMAC_CIM8      (*(volatile  uint32_t*)0x40078258U) /**< \brief (XDMAC) Channel Interrupt Mask Register (chid = 8) */
  #define REG_XDMAC_CIS8      (*(volatile  uint32_t*)0x4007825CU) /**< \brief (XDMAC) Channel Interrupt Status Register (chid = 8) */
  #define REG_XDMAC_CSA8      (*(volatile uint32_t*)0x40078260U) /**< \brief (XDMAC) Channel Source Address Register (chid = 8) */
  #define REG_XDMAC_CDA8      (*(volatile uint32_t*)0x40078264U) /**< \brief (XDMAC) Channel Destination Address Register (chid = 8) */
  #define REG_XDMAC_CNDA8     (*(volatile uint32_t*)0x40078268U) /**< \brief (XDMAC) Channel Next Descriptor Address Register (chid = 8) */
  #define REG_XDMAC_CNDC8     (*(volatile uint32_t*)0x4007826CU) /**< \brief (XDMAC) Channel Next Descriptor Control Register (chid = 8) */
  #define REG_XDMAC_CUBC8     (*(volatile uint32_t*)0x40078270U) /**< \brief (XDMAC) Channel Microblock Control Register (chid = 8) */
  #define REG_XDMAC_CBC8      (*(volatile uint32_t*)0x40078274U) /**< \brief (XDMAC) Channel Block Control Register (chid = 8) */
  #define REG_XDMAC_CC8       (*(volatile uint32_t*)0x40078278U) /**< \brief (XDMAC) Channel Configuration Register (chid = 8) */
  #define REG_XDMAC_CDS_MSP8  (*(volatile uint32_t*)0x4007827CU) /**< \brief (XDMAC) Channel Data Stride Memory Set Pattern (chid = 8) */
  #define REG_XDMAC_CSUS8     (*(volatile uint32_t*)0x40078280U) /**< \brief (XDMAC) Channel Source Microblock Stride (chid = 8) */
  #define REG_XDMAC_CDUS8     (*(volatile uint32_t*)0x40078284U) /**< \brief (XDMAC) Channel Destination Microblock Stride (chid = 8) */
  #define REG_XDMAC_CIE9      (*(volatile  uint32_t*)0x40078290U) /**< \brief (XDMAC) Channel Interrupt Enable Register (chid = 9) */
  #define REG_XDMAC_CID9      (*(volatile  uint32_t*)0x40078294U) /**< \brief (XDMAC) Channel Interrupt Disable Register (chid = 9) */
  #define REG_XDMAC_CIM9      (*(volatile  uint32_t*)0x40078298U) /**< \brief (XDMAC) Channel Interrupt Mask Register (chid = 9) */
  #define REG_XDMAC_CIS9      (*(volatile  uint32_t*)0x4007829CU) /**< \brief (XDMAC) Channel Interrupt Status Register (chid = 9) */
  #define REG_XDMAC_CSA9      (*(volatile uint32_t*)0x400782A0U) /**< \brief (XDMAC) Channel Source Address Register (chid = 9) */
  #define REG_XDMAC_CDA9      (*(volatile uint32_t*)0x400782A4U) /**< \brief (XDMAC) Channel Destination Address Register (chid = 9) */
  #define REG_XDMAC_CNDA9     (*(volatile uint32_t*)0x400782A8U) /**< \brief (XDMAC) Channel Next Descriptor Address Register (chid = 9) */
  #define REG_XDMAC_CNDC9     (*(volatile uint32_t*)0x400782ACU) /**< \brief (XDMAC) Channel Next Descriptor Control Register (chid = 9) */
  #define REG_XDMAC_CUBC9     (*(volatile uint32_t*)0x400782B0U) /**< \brief (XDMAC) Channel Microblock Control Register (chid = 9) */
  #define REG_XDMAC_CBC9      (*(volatile uint32_t*)0x400782B4U) /**< \brief (XDMAC) Channel Block Control Register (chid = 9) */
  #define REG_XDMAC_CC9       (*(volatile uint32_t*)0x400782B8U) /**< \brief (XDMAC) Channel Configuration Register (chid = 9) */
  #define REG_XDMAC_CDS_MSP9  (*(volatile uint32_t*)0x400782BCU) /**< \brief (XDMAC) Channel Data Stride Memory Set Pattern (chid = 9) */
  #define REG_XDMAC_CSUS9     (*(volatile uint32_t*)0x400782C0U) /**< \brief (XDMAC) Channel Source Microblock Stride (chid = 9) */
  #define REG_XDMAC_CDUS9     (*(volatile uint32_t*)0x400782C4U) /**< \brief (XDMAC) Channel Destination Microblock Stride (chid = 9) */
  #define REG_XDMAC_CIE10     (*(volatile  uint32_t*)0x400782D0U) /**< \brief (XDMAC) Channel Interrupt Enable Register (chid = 10) */
  #define REG_XDMAC_CID10     (*(volatile  uint32_t*)0x400782D4U) /**< \brief (XDMAC) Channel Interrupt Disable Register (chid = 10) */
  #define REG_XDMAC_CIM10     (*(volatile  uint32_t*)0x400782D8U) /**< \brief (XDMAC) Channel Interrupt Mask Register (chid = 10) */
  #define REG_XDMAC_CIS10     (*(volatile  uint32_t*)0x400782DCU) /**< \brief (XDMAC) Channel Interrupt Status Register (chid = 10) */
  #define REG_XDMAC_CSA10     (*(volatile uint32_t*)0x400782E0U) /**< \brief (XDMAC) Channel Source Address Register (chid = 10) */
  #define REG_XDMAC_CDA10     (*(volatile uint32_t*)0x400782E4U) /**< \brief (XDMAC) Channel Destination Address Register (chid = 10) */
  #define REG_XDMAC_CNDA10    (*(volatile uint32_t*)0x400782E8U) /**< \brief (XDMAC) Channel Next Descriptor Address Register (chid = 10) */
  #define REG_XDMAC_CNDC10    (*(volatile uint32_t*)0x400782ECU) /**< \brief (XDMAC) Channel Next Descriptor Control Register (chid = 10) */
  #define REG_XDMAC_CUBC10    (*(volatile uint32_t*)0x400782F0U) /**< \brief (XDMAC) Channel Microblock Control Register (chid = 10) */
  #define REG_XDMAC_CBC10     (*(volatile uint32_t*)0x400782F4U) /**< \brief (XDMAC) Channel Block Control Register (chid = 10) */
  #define REG_XDMAC_CC10      (*(volatile uint32_t*)0x400782F8U) /**< \brief (XDMAC) Channel Configuration Register (chid = 10) */
  #define REG_XDMAC_CDS_MSP10 (*(volatile uint32_t*)0x400782FCU) /**< \brief (XDMAC) Channel Data Stride Memory Set Pattern (chid = 10) */
  #define REG_XDMAC_CSUS10    (*(volatile uint32_t*)0x40078300U) /**< \brief (XDMAC) Channel Source Microblock Stride (chid = 10) */
  #define REG_XDMAC_CDUS10    (*(volatile uint32_t*)0x40078304U) /**< \brief (XDMAC) Channel Destination Microblock Stride (chid = 10) */
  #define REG_XDMAC_CIE11     (*(volatile  uint32_t*)0x40078310U) /**< \brief (XDMAC) Channel Interrupt Enable Register (chid = 11) */
  #define REG_XDMAC_CID11     (*(volatile  uint32_t*)0x40078314U) /**< \brief (XDMAC) Channel Interrupt Disable Register (chid = 11) */
  #define REG_XDMAC_CIM11     (*(volatile  uint32_t*)0x40078318U) /**< \brief (XDMAC) Channel Interrupt Mask Register (chid = 11) */
  #define REG_XDMAC_CIS11     (*(volatile  uint32_t*)0x4007831CU) /**< \brief (XDMAC) Channel Interrupt Status Register (chid = 11) */
  #define REG_XDMAC_CSA11     (*(volatile uint32_t*)0x40078320U) /**< \brief (XDMAC) Channel Source Address Register (chid = 11) */
  #define REG_XDMAC_CDA11     (*(volatile uint32_t*)0x40078324U) /**< \brief (XDMAC) Channel Destination Address Register (chid = 11) */
  #define REG_XDMAC_CNDA11    (*(volatile uint32_t*)0x40078328U) /**< \brief (XDMAC) Channel Next Descriptor Address Register (chid = 11) */
  #define REG_XDMAC_CNDC11    (*(volatile uint32_t*)0x4007832CU) /**< \brief (XDMAC) Channel Next Descriptor Control Register (chid = 11) */
  #define REG_XDMAC_CUBC11    (*(volatile uint32_t*)0x40078330U) /**< \brief (XDMAC) Channel Microblock Control Register (chid = 11) */
  #define REG_XDMAC_CBC11     (*(volatile uint32_t*)0x40078334U) /**< \brief (XDMAC) Channel Block Control Register (chid = 11) */
  #define REG_XDMAC_CC11      (*(volatile uint32_t*)0x40078338U) /**< \brief (XDMAC) Channel Configuration Register (chid = 11) */
  #define REG_XDMAC_CDS_MSP11 (*(volatile uint32_t*)0x4007833CU) /**< \brief (XDMAC) Channel Data Stride Memory Set Pattern (chid = 11) */
  #define REG_XDMAC_CSUS11    (*(volatile uint32_t*)0x40078340U) /**< \brief (XDMAC) Channel Source Microblock Stride (chid = 11) */
  #define REG_XDMAC_CDUS11    (*(volatile uint32_t*)0x40078344U) /**< \brief (XDMAC) Channel Destination Microblock Stride (chid = 11) */
  #define REG_XDMAC_CIE12     (*(volatile  uint32_t*)0x40078350U) /**< \brief (XDMAC) Channel Interrupt Enable Register (chid = 12) */
  #define REG_XDMAC_CID12     (*(volatile  uint32_t*)0x40078354U) /**< \brief (XDMAC) Channel Interrupt Disable Register (chid = 12) */
  #define REG_XDMAC_CIM12     (*(volatile  uint32_t*)0x40078358U) /**< \brief (XDMAC) Channel Interrupt Mask Register (chid = 12) */
  #define REG_XDMAC_CIS12     (*(volatile  uint32_t*)0x4007835CU) /**< \brief (XDMAC) Channel Interrupt Status Register (chid = 12) */
  #define REG_XDMAC_CSA12     (*(volatile uint32_t*)0x40078360U) /**< \brief (XDMAC) Channel Source Address Register (chid = 12) */
  #define REG_XDMAC_CDA12     (*(volatile uint32_t*)0x40078364U) /**< \brief (XDMAC) Channel Destination Address Register (chid = 12) */
  #define REG_XDMAC_CNDA12    (*(volatile uint32_t*)0x40078368U) /**< \brief (XDMAC) Channel Next Descriptor Address Register (chid = 12) */
  #define REG_XDMAC_CNDC12    (*(volatile uint32_t*)0x4007836CU) /**< \brief (XDMAC) Channel Next Descriptor Control Register (chid = 12) */
  #define REG_XDMAC_CUBC12    (*(volatile uint32_t*)0x40078370U) /**< \brief (XDMAC) Channel Microblock Control Register (chid = 12) */
  #define REG_XDMAC_CBC12     (*(volatile uint32_t*)0x40078374U) /**< \brief (XDMAC) Channel Block Control Register (chid = 12) */
  #define REG_XDMAC_CC12      (*(volatile uint32_t*)0x40078378U) /**< \brief (XDMAC) Channel Configuration Register (chid = 12) */
  #define REG_XDMAC_CDS_MSP12 (*(volatile uint32_t*)0x4007837CU) /**< \brief (XDMAC) Channel Data Stride Memory Set Pattern (chid = 12) */
  #define REG_XDMAC_CSUS12    (*(volatile uint32_t*)0x40078380U) /**< \brief (XDMAC) Channel Source Microblock Stride (chid = 12) */
  #define REG_XDMAC_CDUS12    (*(volatile uint32_t*)0x40078384U) /**< \brief (XDMAC) Channel Destination Microblock Stride (chid = 12) */
  #define REG_XDMAC_CIE13     (*(volatile  uint32_t*)0x40078390U) /**< \brief (XDMAC) Channel Interrupt Enable Register (chid = 13) */
  #define REG_XDMAC_CID13     (*(volatile  uint32_t*)0x40078394U) /**< \brief (XDMAC) Channel Interrupt Disable Register (chid = 13) */
  #define REG_XDMAC_CIM13     (*(volatile  uint32_t*)0x40078398U) /**< \brief (XDMAC) Channel Interrupt Mask Register (chid = 13) */
  #define REG_XDMAC_CIS13     (*(volatile  uint32_t*)0x4007839CU) /**< \brief (XDMAC) Channel Interrupt Status Register (chid = 13) */
  #define REG_XDMAC_CSA13     (*(volatile uint32_t*)0x400783A0U) /**< \brief (XDMAC) Channel Source Address Register (chid = 13) */
  #define REG_XDMAC_CDA13     (*(volatile uint32_t*)0x400783A4U) /**< \brief (XDMAC) Channel Destination Address Register (chid = 13) */
  #define REG_XDMAC_CNDA13    (*(volatile uint32_t*)0x400783A8U) /**< \brief (XDMAC) Channel Next Descriptor Address Register (chid = 13) */
  #define REG_XDMAC_CNDC13    (*(volatile uint32_t*)0x400783ACU) /**< \brief (XDMAC) Channel Next Descriptor Control Register (chid = 13) */
  #define REG_XDMAC_CUBC13    (*(volatile uint32_t*)0x400783B0U) /**< \brief (XDMAC) Channel Microblock Control Register (chid = 13) */
  #define REG_XDMAC_CBC13     (*(volatile uint32_t*)0x400783B4U) /**< \brief (XDMAC) Channel Block Control Register (chid = 13) */
  #define REG_XDMAC_CC13      (*(volatile uint32_t*)0x400783B8U) /**< \brief (XDMAC) Channel Configuration Register (chid = 13) */
  #define REG_XDMAC_CDS_MSP13 (*(volatile uint32_t*)0x400783BCU) /**< \brief (XDMAC) Channel Data Stride Memory Set Pattern (chid = 13) */
  #define REG_XDMAC_CSUS13    (*(volatile uint32_t*)0x400783C0U) /**< \brief (XDMAC) Channel Source Microblock Stride (chid = 13) */
  #define REG_XDMAC_CDUS13    (*(volatile uint32_t*)0x400783C4U) /**< \brief (XDMAC) Channel Destination Microblock Stride (chid = 13) */
  #define REG_XDMAC_CIE14     (*(volatile  uint32_t*)0x400783D0U) /**< \brief (XDMAC) Channel Interrupt Enable Register (chid = 14) */
  #define REG_XDMAC_CID14     (*(volatile  uint32_t*)0x400783D4U) /**< \brief (XDMAC) Channel Interrupt Disable Register (chid = 14) */
  #define REG_XDMAC_CIM14     (*(volatile  uint32_t*)0x400783D8U) /**< \brief (XDMAC) Channel Interrupt Mask Register (chid = 14) */
  #define REG_XDMAC_CIS14     (*(volatile  uint32_t*)0x400783DCU) /**< \brief (XDMAC) Channel Interrupt Status Register (chid = 14) */
  #define REG_XDMAC_CSA14     (*(volatile uint32_t*)0x400783E0U) /**< \brief (XDMAC) Channel Source Address Register (chid = 14) */
  #define REG_XDMAC_CDA14     (*(volatile uint32_t*)0x400783E4U) /**< \brief (XDMAC) Channel Destination Address Register (chid = 14) */
  #define REG_XDMAC_CNDA14    (*(volatile uint32_t*)0x400783E8U) /**< \brief (XDMAC) Channel Next Descriptor Address Register (chid = 14) */
  #define REG_XDMAC_CNDC14    (*(volatile uint32_t*)0x400783ECU) /**< \brief (XDMAC) Channel Next Descriptor Control Register (chid = 14) */
  #define REG_XDMAC_CUBC14    (*(volatile uint32_t*)0x400783F0U) /**< \brief (XDMAC) Channel Microblock Control Register (chid = 14) */
  #define REG_XDMAC_CBC14     (*(volatile uint32_t*)0x400783F4U) /**< \brief (XDMAC) Channel Block Control Register (chid = 14) */
  #define REG_XDMAC_CC14      (*(volatile uint32_t*)0x400783F8U) /**< \brief (XDMAC) Channel Configuration Register (chid = 14) */
  #define REG_XDMAC_CDS_MSP14 (*(volatile uint32_t*)0x400783FCU) /**< \brief (XDMAC) Channel Data Stride Memory Set Pattern (chid = 14) */
  #define REG_XDMAC_CSUS14    (*(volatile uint32_t*)0x40078400U) /**< \brief (XDMAC) Channel Source Microblock Stride (chid = 14) */
  #define REG_XDMAC_CDUS14    (*(volatile uint32_t*)0x40078404U) /**< \brief (XDMAC) Channel Destination Microblock Stride (chid = 14) */
  #define REG_XDMAC_CIE15     (*(volatile  uint32_t*)0x40078410U) /**< \brief (XDMAC) Channel Interrupt Enable Register (chid = 15) */
  #define REG_XDMAC_CID15     (*(volatile  uint32_t*)0x40078414U) /**< \brief (XDMAC) Channel Interrupt Disable Register (chid = 15) */
  #define REG_XDMAC_CIM15     (*(volatile  uint32_t*)0x40078418U) /**< \brief (XDMAC) Channel Interrupt Mask Register (chid = 15) */
  #define REG_XDMAC_CIS15     (*(volatile  uint32_t*)0x4007841CU) /**< \brief (XDMAC) Channel Interrupt Status Register (chid = 15) */
  #define REG_XDMAC_CSA15     (*(volatile uint32_t*)0x40078420U) /**< \brief (XDMAC) Channel Source Address Register (chid = 15) */
  #define REG_XDMAC_CDA15     (*(volatile uint32_t*)0x40078424U) /**< \brief (XDMAC) Channel Destination Address Register (chid = 15) */
  #define REG_XDMAC_CNDA15    (*(volatile uint32_t*)0x40078428U) /**< \brief (XDMAC) Channel Next Descriptor Address Register (chid = 15) */
  #define REG_XDMAC_CNDC15    (*(volatile uint32_t*)0x4007842CU) /**< \brief (XDMAC) Channel Next Descriptor Control Register (chid = 15) */
  #define REG_XDMAC_CUBC15    (*(volatile uint32_t*)0x40078430U) /**< \brief (XDMAC) Channel Microblock Control Register (chid = 15) */
  #define REG_XDMAC_CBC15     (*(volatile uint32_t*)0x40078434U) /**< \brief (XDMAC) Channel Block Control Register (chid = 15) */
  #define REG_XDMAC_CC15      (*(volatile uint32_t*)0x40078438U) /**< \brief (XDMAC) Channel Configuration Register (chid = 15) */
  #define REG_XDMAC_CDS_MSP15 (*(volatile uint32_t*)0x4007843CU) /**< \brief (XDMAC) Channel Data Stride Memory Set Pattern (chid = 15) */
  #define REG_XDMAC_CSUS15    (*(volatile uint32_t*)0x40078440U) /**< \brief (XDMAC) Channel Source Microblock Stride (chid = 15) */
  #define REG_XDMAC_CDUS15    (*(volatile uint32_t*)0x40078444U) /**< \brief (XDMAC) Channel Destination Microblock Stride (chid = 15) */
  #define REG_XDMAC_CIE16     (*(volatile  uint32_t*)0x40078450U) /**< \brief (XDMAC) Channel Interrupt Enable Register (chid = 16) */
  #define REG_XDMAC_CID16     (*(volatile  uint32_t*)0x40078454U) /**< \brief (XDMAC) Channel Interrupt Disable Register (chid = 16) */
  #define REG_XDMAC_CIM16     (*(volatile  uint32_t*)0x40078458U) /**< \brief (XDMAC) Channel Interrupt Mask Register (chid = 16) */
  #define REG_XDMAC_CIS16     (*(volatile  uint32_t*)0x4007845CU) /**< \brief (XDMAC) Channel Interrupt Status Register (chid = 16) */
  #define REG_XDMAC_CSA16     (*(volatile uint32_t*)0x40078460U) /**< \brief (XDMAC) Channel Source Address Register (chid = 16) */
  #define REG_XDMAC_CDA16     (*(volatile uint32_t*)0x40078464U) /**< \brief (XDMAC) Channel Destination Address Register (chid = 16) */
  #define REG_XDMAC_CNDA16    (*(volatile uint32_t*)0x40078468U) /**< \brief (XDMAC) Channel Next Descriptor Address Register (chid = 16) */
  #define REG_XDMAC_CNDC16    (*(volatile uint32_t*)0x4007846CU) /**< \brief (XDMAC) Channel Next Descriptor Control Register (chid = 16) */
  #define REG_XDMAC_CUBC16    (*(volatile uint32_t*)0x40078470U) /**< \brief (XDMAC) Channel Microblock Control Register (chid = 16) */
  #define REG_XDMAC_CBC16     (*(volatile uint32_t*)0x40078474U) /**< \brief (XDMAC) Channel Block Control Register (chid = 16) */
  #define REG_XDMAC_CC16      (*(volatile uint32_t*)0x40078478U) /**< \brief (XDMAC) Channel Configuration Register (chid = 16) */
  #define REG_XDMAC_CDS_MSP16 (*(volatile uint32_t*)0x4007847CU) /**< \brief (XDMAC) Channel Data Stride Memory Set Pattern (chid = 16) */
  #define REG_XDMAC_CSUS16    (*(volatile uint32_t*)0x40078480U) /**< \brief (XDMAC) Channel Source Microblock Stride (chid = 16) */
  #define REG_XDMAC_CDUS16    (*(volatile uint32_t*)0x40078484U) /**< \brief (XDMAC) Channel Destination Microblock Stride (chid = 16) */
  #define REG_XDMAC_CIE17     (*(volatile  uint32_t*)0x40078490U) /**< \brief (XDMAC) Channel Interrupt Enable Register (chid = 17) */
  #define REG_XDMAC_CID17     (*(volatile  uint32_t*)0x40078494U) /**< \brief (XDMAC) Channel Interrupt Disable Register (chid = 17) */
  #define REG_XDMAC_CIM17     (*(volatile  uint32_t*)0x40078498U) /**< \brief (XDMAC) Channel Interrupt Mask Register (chid = 17) */
  #define REG_XDMAC_CIS17     (*(volatile  uint32_t*)0x4007849CU) /**< \brief (XDMAC) Channel Interrupt Status Register (chid = 17) */
  #define REG_XDMAC_CSA17     (*(volatile uint32_t*)0x400784A0U) /**< \brief (XDMAC) Channel Source Address Register (chid = 17) */
  #define REG_XDMAC_CDA17     (*(volatile uint32_t*)0x400784A4U) /**< \brief (XDMAC) Channel Destination Address Register (chid = 17) */
  #define REG_XDMAC_CNDA17    (*(volatile uint32_t*)0x400784A8U) /**< \brief (XDMAC) Channel Next Descriptor Address Register (chid = 17) */
  #define REG_XDMAC_CNDC17    (*(volatile uint32_t*)0x400784ACU) /**< \brief (XDMAC) Channel Next Descriptor Control Register (chid = 17) */
  #define REG_XDMAC_CUBC17    (*(volatile uint32_t*)0x400784B0U) /**< \brief (XDMAC) Channel Microblock Control Register (chid = 17) */
  #define REG_XDMAC_CBC17     (*(volatile uint32_t*)0x400784B4U) /**< \brief (XDMAC) Channel Block Control Register (chid = 17) */
  #define REG_XDMAC_CC17      (*(volatile uint32_t*)0x400784B8U) /**< \brief (XDMAC) Channel Configuration Register (chid = 17) */
  #define REG_XDMAC_CDS_MSP17 (*(volatile uint32_t*)0x400784BCU) /**< \brief (XDMAC) Channel Data Stride Memory Set Pattern (chid = 17) */
  #define REG_XDMAC_CSUS17    (*(volatile uint32_t*)0x400784C0U) /**< \brief (XDMAC) Channel Source Microblock Stride (chid = 17) */
  #define REG_XDMAC_CDUS17    (*(volatile uint32_t*)0x400784C4U) /**< \brief (XDMAC) Channel Destination Microblock Stride (chid = 17) */
  #define REG_XDMAC_CIE18     (*(volatile  uint32_t*)0x400784D0U) /**< \brief (XDMAC) Channel Interrupt Enable Register (chid = 18) */
  #define REG_XDMAC_CID18     (*(volatile  uint32_t*)0x400784D4U) /**< \brief (XDMAC) Channel Interrupt Disable Register (chid = 18) */
  #define REG_XDMAC_CIM18     (*(volatile  uint32_t*)0x400784D8U) /**< \brief (XDMAC) Channel Interrupt Mask Register (chid = 18) */
  #define REG_XDMAC_CIS18     (*(volatile  uint32_t*)0x400784DCU) /**< \brief (XDMAC) Channel Interrupt Status Register (chid = 18) */
  #define REG_XDMAC_CSA18     (*(volatile uint32_t*)0x400784E0U) /**< \brief (XDMAC) Channel Source Address Register (chid = 18) */
  #define REG_XDMAC_CDA18     (*(volatile uint32_t*)0x400784E4U) /**< \brief (XDMAC) Channel Destination Address Register (chid = 18) */
  #define REG_XDMAC_CNDA18    (*(volatile uint32_t*)0x400784E8U) /**< \brief (XDMAC) Channel Next Descriptor Address Register (chid = 18) */
  #define REG_XDMAC_CNDC18    (*(volatile uint32_t*)0x400784ECU) /**< \brief (XDMAC) Channel Next Descriptor Control Register (chid = 18) */
  #define REG_XDMAC_CUBC18    (*(volatile uint32_t*)0x400784F0U) /**< \brief (XDMAC) Channel Microblock Control Register (chid = 18) */
  #define REG_XDMAC_CBC18     (*(volatile uint32_t*)0x400784F4U) /**< \brief (XDMAC) Channel Block Control Register (chid = 18) */
  #define REG_XDMAC_CC18      (*(volatile uint32_t*)0x400784F8U) /**< \brief (XDMAC) Channel Configuration Register (chid = 18) */
  #define REG_XDMAC_CDS_MSP18 (*(volatile uint32_t*)0x400784FCU) /**< \brief (XDMAC) Channel Data Stride Memory Set Pattern (chid = 18) */
  #define REG_XDMAC_CSUS18    (*(volatile uint32_t*)0x40078500U) /**< \brief (XDMAC) Channel Source Microblock Stride (chid = 18) */
  #define REG_XDMAC_CDUS18    (*(volatile uint32_t*)0x40078504U) /**< \brief (XDMAC) Channel Destination Microblock Stride (chid = 18) */
  #define REG_XDMAC_CIE19     (*(volatile  uint32_t*)0x40078510U) /**< \brief (XDMAC) Channel Interrupt Enable Register (chid = 19) */
  #define REG_XDMAC_CID19     (*(volatile  uint32_t*)0x40078514U) /**< \brief (XDMAC) Channel Interrupt Disable Register (chid = 19) */
  #define REG_XDMAC_CIM19     (*(volatile  uint32_t*)0x40078518U) /**< \brief (XDMAC) Channel Interrupt Mask Register (chid = 19) */
  #define REG_XDMAC_CIS19     (*(volatile  uint32_t*)0x4007851CU) /**< \brief (XDMAC) Channel Interrupt Status Register (chid = 19) */
  #define REG_XDMAC_CSA19     (*(volatile uint32_t*)0x40078520U) /**< \brief (XDMAC) Channel Source Address Register (chid = 19) */
  #define REG_XDMAC_CDA19     (*(volatile uint32_t*)0x40078524U) /**< \brief (XDMAC) Channel Destination Address Register (chid = 19) */
  #define REG_XDMAC_CNDA19    (*(volatile uint32_t*)0x40078528U) /**< \brief (XDMAC) Channel Next Descriptor Address Register (chid = 19) */
  #define REG_XDMAC_CNDC19    (*(volatile uint32_t*)0x4007852CU) /**< \brief (XDMAC) Channel Next Descriptor Control Register (chid = 19) */
  #define REG_XDMAC_CUBC19    (*(volatile uint32_t*)0x40078530U) /**< \brief (XDMAC) Channel Microblock Control Register (chid = 19) */
  #define REG_XDMAC_CBC19     (*(volatile uint32_t*)0x40078534U) /**< \brief (XDMAC) Channel Block Control Register (chid = 19) */
  #define REG_XDMAC_CC19      (*(volatile uint32_t*)0x40078538U) /**< \brief (XDMAC) Channel Configuration Register (chid = 19) */
  #define REG_XDMAC_CDS_MSP19 (*(volatile uint32_t*)0x4007853CU) /**< \brief (XDMAC) Channel Data Stride Memory Set Pattern (chid = 19) */
  #define REG_XDMAC_CSUS19    (*(volatile uint32_t*)0x40078540U) /**< \brief (XDMAC) Channel Source Microblock Stride (chid = 19) */
  #define REG_XDMAC_CDUS19    (*(volatile uint32_t*)0x40078544U) /**< \brief (XDMAC) Channel Destination Microblock Stride (chid = 19) */
  #define REG_XDMAC_CIE20     (*(volatile  uint32_t*)0x40078550U) /**< \brief (XDMAC) Channel Interrupt Enable Register (chid = 20) */
  #define REG_XDMAC_CID20     (*(volatile  uint32_t*)0x40078554U) /**< \brief (XDMAC) Channel Interrupt Disable Register (chid = 20) */
  #define REG_XDMAC_CIM20     (*(volatile  uint32_t*)0x40078558U) /**< \brief (XDMAC) Channel Interrupt Mask Register (chid = 20) */
  #define REG_XDMAC_CIS20     (*(volatile  uint32_t*)0x4007855CU) /**< \brief (XDMAC) Channel Interrupt Status Register (chid = 20) */
  #define REG_XDMAC_CSA20     (*(volatile uint32_t*)0x40078560U) /**< \brief (XDMAC) Channel Source Address Register (chid = 20) */
  #define REG_XDMAC_CDA20     (*(volatile uint32_t*)0x40078564U) /**< \brief (XDMAC) Channel Destination Address Register (chid = 20) */
  #define REG_XDMAC_CNDA20    (*(volatile uint32_t*)0x40078568U) /**< \brief (XDMAC) Channel Next Descriptor Address Register (chid = 20) */
  #define REG_XDMAC_CNDC20    (*(volatile uint32_t*)0x4007856CU) /**< \brief (XDMAC) Channel Next Descriptor Control Register (chid = 20) */
  #define REG_XDMAC_CUBC20    (*(volatile uint32_t*)0x40078570U) /**< \brief (XDMAC) Channel Microblock Control Register (chid = 20) */
  #define REG_XDMAC_CBC20     (*(volatile uint32_t*)0x40078574U) /**< \brief (XDMAC) Channel Block Control Register (chid = 20) */
  #define REG_XDMAC_CC20      (*(volatile uint32_t*)0x40078578U) /**< \brief (XDMAC) Channel Configuration Register (chid = 20) */
  #define REG_XDMAC_CDS_MSP20 (*(volatile uint32_t*)0x4007857CU) /**< \brief (XDMAC) Channel Data Stride Memory Set Pattern (chid = 20) */
  #define REG_XDMAC_CSUS20    (*(volatile uint32_t*)0x40078580U) /**< \brief (XDMAC) Channel Source Microblock Stride (chid = 20) */
  #define REG_XDMAC_CDUS20    (*(volatile uint32_t*)0x40078584U) /**< \brief (XDMAC) Channel Destination Microblock Stride (chid = 20) */
  #define REG_XDMAC_CIE21     (*(volatile  uint32_t*)0x40078590U) /**< \brief (XDMAC) Channel Interrupt Enable Register (chid = 21) */
  #define REG_XDMAC_CID21     (*(volatile  uint32_t*)0x40078594U) /**< \brief (XDMAC) Channel Interrupt Disable Register (chid = 21) */
  #define REG_XDMAC_CIM21     (*(volatile  uint32_t*)0x40078598U) /**< \brief (XDMAC) Channel Interrupt Mask Register (chid = 21) */
  #define REG_XDMAC_CIS21     (*(volatile  uint32_t*)0x4007859CU) /**< \brief (XDMAC) Channel Interrupt Status Register (chid = 21) */
  #define REG_XDMAC_CSA21     (*(volatile uint32_t*)0x400785A0U) /**< \brief (XDMAC) Channel Source Address Register (chid = 21) */
  #define REG_XDMAC_CDA21     (*(volatile uint32_t*)0x400785A4U) /**< \brief (XDMAC) Channel Destination Address Register (chid = 21) */
  #define REG_XDMAC_CNDA21    (*(volatile uint32_t*)0x400785A8U) /**< \brief (XDMAC) Channel Next Descriptor Address Register (chid = 21) */
  #define REG_XDMAC_CNDC21    (*(volatile uint32_t*)0x400785ACU) /**< \brief (XDMAC) Channel Next Descriptor Control Register (chid = 21) */
  #define REG_XDMAC_CUBC21    (*(volatile uint32_t*)0x400785B0U) /**< \brief (XDMAC) Channel Microblock Control Register (chid = 21) */
  #define REG_XDMAC_CBC21     (*(volatile uint32_t*)0x400785B4U) /**< \brief (XDMAC) Channel Block Control Register (chid = 21) */
  #define REG_XDMAC_CC21      (*(volatile uint32_t*)0x400785B8U) /**< \brief (XDMAC) Channel Configuration Register (chid = 21) */
  #define REG_XDMAC_CDS_MSP21 (*(volatile uint32_t*)0x400785BCU) /**< \brief (XDMAC) Channel Data Stride Memory Set Pattern (chid = 21) */
  #define REG_XDMAC_CSUS21    (*(volatile uint32_t*)0x400785C0U) /**< \brief (XDMAC) Channel Source Microblock Stride (chid = 21) */
  #define REG_XDMAC_CDUS21    (*(volatile uint32_t*)0x400785C4U) /**< \brief (XDMAC) Channel Destination Microblock Stride (chid = 21) */
  #define REG_XDMAC_CIE22     (*(volatile  uint32_t*)0x400785D0U) /**< \brief (XDMAC) Channel Interrupt Enable Register (chid = 22) */
  #define REG_XDMAC_CID22     (*(volatile  uint32_t*)0x400785D4U) /**< \brief (XDMAC) Channel Interrupt Disable Register (chid = 22) */
  #define REG_XDMAC_CIM22     (*(volatile  uint32_t*)0x400785D8U) /**< \brief (XDMAC) Channel Interrupt Mask Register (chid = 22) */
  #define REG_XDMAC_CIS22     (*(volatile  uint32_t*)0x400785DCU) /**< \brief (XDMAC) Channel Interrupt Status Register (chid = 22) */
  #define REG_XDMAC_CSA22     (*(volatile uint32_t*)0x400785E0U) /**< \brief (XDMAC) Channel Source Address Register (chid = 22) */
  #define REG_XDMAC_CDA22     (*(volatile uint32_t*)0x400785E4U) /**< \brief (XDMAC) Channel Destination Address Register (chid = 22) */
  #define REG_XDMAC_CNDA22    (*(volatile uint32_t*)0x400785E8U) /**< \brief (XDMAC) Channel Next Descriptor Address Register (chid = 22) */
  #define REG_XDMAC_CNDC22    (*(volatile uint32_t*)0x400785ECU) /**< \brief (XDMAC) Channel Next Descriptor Control Register (chid = 22) */
  #define REG_XDMAC_CUBC22    (*(volatile uint32_t*)0x400785F0U) /**< \brief (XDMAC) Channel Microblock Control Register (chid = 22) */
  #define REG_XDMAC_CBC22     (*(volatile uint32_t*)0x400785F4U) /**< \brief (XDMAC) Channel Block Control Register (chid = 22) */
  #define REG_XDMAC_CC22      (*(volatile uint32_t*)0x400785F8U) /**< \brief (XDMAC) Channel Configuration Register (chid = 22) */
  #define REG_XDMAC_CDS_MSP22 (*(volatile uint32_t*)0x400785FCU) /**< \brief (XDMAC) Channel Data Stride Memory Set Pattern (chid = 22) */
  #define REG_XDMAC_CSUS22    (*(volatile uint32_t*)0x40078600U) /**< \brief (XDMAC) Channel Source Microblock Stride (chid = 22) */
  #define REG_XDMAC_CDUS22    (*(volatile uint32_t*)0x40078604U) /**< \brief (XDMAC) Channel Destination Microblock Stride (chid = 22) */
  #define REG_XDMAC_CIE23     (*(volatile  uint32_t*)0x40078610U) /**< \brief (XDMAC) Channel Interrupt Enable Register (chid = 23) */
  #define REG_XDMAC_CID23     (*(volatile  uint32_t*)0x40078614U) /**< \brief (XDMAC) Channel Interrupt Disable Register (chid = 23) */
  #define REG_XDMAC_CIM23     (*(volatile  uint32_t*)0x40078618U) /**< \brief (XDMAC) Channel Interrupt Mask Register (chid = 23) */
  #define REG_XDMAC_CIS23     (*(volatile  uint32_t*)0x4007861CU) /**< \brief (XDMAC) Channel Interrupt Status Register (chid = 23) */
  #define REG_XDMAC_CSA23     (*(volatile uint32_t*)0x40078620U) /**< \brief (XDMAC) Channel Source Address Register (chid = 23) */
  #define REG_XDMAC_CDA23     (*(volatile uint32_t*)0x40078624U) /**< \brief (XDMAC) Channel Destination Address Register (chid = 23) */
  #define REG_XDMAC_CNDA23    (*(volatile uint32_t*)0x40078628U) /**< \brief (XDMAC) Channel Next Descriptor Address Register (chid = 23) */
  #define REG_XDMAC_CNDC23    (*(volatile uint32_t*)0x4007862CU) /**< \brief (XDMAC) Channel Next Descriptor Control Register (chid = 23) */
  #define REG_XDMAC_CUBC23    (*(volatile uint32_t*)0x40078630U) /**< \brief (XDMAC) Channel Microblock Control Register (chid = 23) */
  #define REG_XDMAC_CBC23     (*(volatile uint32_t*)0x40078634U) /**< \brief (XDMAC) Channel Block Control Register (chid = 23) */
  #define REG_XDMAC_CC23      (*(volatile uint32_t*)0x40078638U) /**< \brief (XDMAC) Channel Configuration Register (chid = 23) */
  #define REG_XDMAC_CDS_MSP23 (*(volatile uint32_t*)0x4007863CU) /**< \brief (XDMAC) Channel Data Stride Memory Set Pattern (chid = 23) */
  #define REG_XDMAC_CSUS23    (*(volatile uint32_t*)0x40078640U) /**< \brief (XDMAC) Channel Source Microblock Stride (chid = 23) */
  #define REG_XDMAC_CDUS23    (*(volatile uint32_t*)0x40078644U) /**< \brief (XDMAC) Channel Destination Microblock Stride (chid = 23) */
  #define REG_XDMAC_VERSION   (*(volatile uint32_t*)0x40078FFCU) /**< \brief (XDMAC) XDMAC Version Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAMV71_XDMAC_INSTANCE_ */
