/************************************************************************************
* This header file is for MyWirelessApp Demo Non Beacon End Device application.
*
* (c) Copyright 2008, Freescale, Inc.  All rights reserved.
*
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale Semiconductor.
*
************************************************************************************/
#ifndef _MAPP_H_
#define _MAPP_H_

#include "MApp_init.h"
#include "ApplicationConf.h"
#include "802_15_4.h"      /* Include everything related to the 802.15.4 interface*/
#include "CommUtil.h"      /* Defines the interface of the demo serial terminal interface. */
#include "SPI_Interface.h" /* Defines the interface of the SPI driver. */
#include "Display.h"       /* Defines the interface of the LCD driver. */
#include "TMR_Interface.h" /* Defines the interface of the Timer driver. */
#include "TS_Interface.h"  /* Defines the interface of the TaskScheduler. */
#include "PWR_Interface.h" /* Defines the interface of the Low Power Module. */
#include "Led.h"           /* Defines the interface of the Led Module. */
#include "Keyboard.h"      /* Defines the interface of the Keyboard Module. */
#include "NV_Data.h"       /* Defines the interface of the Keyboard Module. */
#include "MyNewTask.c"



/************************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
************************************************************************************/

/* Maximum number of outstanding packets */
#define mDefaultValueOfMaxPendingDataPackets_c 2

/* The slow polling interval is used if the coordinator
   had no data last time we polled. */
#define mDefaultValueOfPollIntervalSlow_c 3000 /* ~(0.001*mDefaultValueOfPollIntervalSlow_c) secs */

/* The fast polling interval is used if the coordinator had data last time we
   polled, thus we increase the band-width while data is available. */
#define mDefaultValueOfPollIntervalFast_c 82 /* ~(0.001*mDefaultValueOfPollIntervalFast_c) secs */

/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/

/* All states in the applications state machine */
enum {
  stateInit,
  stateScanActiveStart,
  stateScanActiveWaitConfirm,
  stateAssociate,
  stateAssociateWaitConfirm,
  stateListen
};

/* Events that are passed to the application task. 
   Are defined as byte masks to make possible 
   send multiple events to the task */

#define gAppEvtDummyEvent_c            (1 << 0)
#define gAppEvtRxFromComm_c            (1 << 1)
#define gAppEvtMessageFromMLME_c       (1 << 2)
#define gAppEvtMessageFromMCPS_c       (1 << 3)


/* Error codes */
enum {
  errorNoError,
  errorWrongConfirm,
  errorNotSuccessful,
  errorNoMessage,
  errorAllocFailed,
  errorInvalidParameter,
  errorNoScanResults
};

/******************************************************************************
*******************************************************************************
* Public Prototypes
* 
*******************************************************************************
******************************************************************************/
extern void MApp_init(void);
extern tsTaskID_t gAppTaskID_c;
/* Information about the PAN we are part of */

/**********************************************************************************/
extern panDescriptor_t mCoordInfo;

/* This is either the short address assigned by the PAN coordinator
   during association, or our own extended MAC address. */
extern uint8_t maMyAddress[8];
/* The devices address mode. If 2, then maMyAddress contains the short
   address assigned by the PAN coordinator. If 3, then maMyAddress is
   equal to the extended address. */
extern uint8_t mAddrMode;
/* Data request packet for sending serial terminal interface input to the coordinator */
static nwkToMcpsMessage_t *mpPacket;
#endif /* _MAPP_H_ */
