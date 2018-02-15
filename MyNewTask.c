/*
 * MyNewTask.c
 *
 *  Created on: Feb 12, 2018
 *      Author: FelipedeJesús
 */
#include "MyNewTask.h"
#include "MApp.h"
#include "Sound.h"
#include "NVM_Interface.h"
#include "string.h"
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

extern uint8_t flagNetwork = FALSE;

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

/* This is either the short address assigned by the PAN coordinator
   during association, or our own extended MAC address. */
extern uint8_t maMyAddress[8];
/* The devices address mode. If 2, then maMyAddress contains the short
   address assigned by the PAN coordinator. If 3, then maMyAddress is
   equal to the extended address. */
extern uint8_t mAddrMode;

/* Global Variable to store our TimerID */
tmrTimerID_t myTimerID;
/* Local variable to store the current state of the LEDs */
static uint8_t ledsState = 0;

uint8_t counter = 0;

tsTaskID_t myNewTaskID;
/* Timer Callback prototype */
void myTaskTimerCallback(tmrTimerID_t timer);


/* MyNewTask main function that will handle the events*/
void MyNewTask(event_t events)
{
	/* Depending on the received event */
	switch(events){
	case gMyNewTaskEvent1_c:
		TMR_StartTimer(myTimerID, /* Timer ID allocated in "MyNewTaskInit()",
called in MApp_init.c during initialization*/
				gTmrIntervalTimer_c, /* Type of timer: INTERVAL */
				3000, /* Timer's Timeout */
				myTaskTimerCallback /* pointer to myTaskTimerCallback function */
		);
		TurnOffLeds(); /* Ensure all LEDs are turned off */
		break;
	case gMyNewTaskEvent2_c: /* Event called from myTaskTimerCallback */
		if(flagNetwork){
		if(4 == counter)
			counter = 1;
		else
			counter++;
		mpPacket = MSG_Alloc(gMaxRxTxDataLength_c);
		char message[9];
		strcpy(message, "Counter: ");
	

		/* get data from serial terminal interface */        
		mpPacket->msgData.dataReq.pMsdu = (uint8_t*)(&(mpPacket->msgData.dataReq.pMsdu)) + sizeof(uint8_t*);
		FLib_MemCpy(mpPacket->msgData.dataReq.pMsdu, (uint8_t*) message, sizeof(message));
		/* Data was available in the serial terminal interface receive buffer. Now create an
	         MCPS-Data Request message containing the serial terminal interface data. */
		mpPacket->msgType = gMcpsDataReq_c;
		/* Create the header using coordinator information gained during 
	         the scan procedure. Also use the short address we were assigned
	         by the coordinator during association. */
		FLib_MemCpy(mpPacket->msgData.dataReq.dstAddr, mCoordInfo.coordAddress, 8);
		FLib_MemCpy(mpPacket->msgData.dataReq.srcAddr, maMyAddress, 8);
		FLib_MemCpy(mpPacket->msgData.dataReq.dstPanId, mCoordInfo.coordPanId, 2);
		FLib_MemCpy(mpPacket->msgData.dataReq.srcPanId, mCoordInfo.coordPanId, 2);
		mpPacket->msgData.dataReq.dstAddrMode = mCoordInfo.coordAddrMode;
		mpPacket->msgData.dataReq.srcAddrMode = mAddrMode;
		mpPacket->msgData.dataReq.msduLength = sizeof(message);
		/* Request MAC level acknowledgement of the data packet */
		mpPacket->msgData.dataReq.txOptions = gTxOptsAck_c;
		/* Give the data packet a handle. The handle is
	         returned in the MCPS-Data Confirm message. */
		mpPacket->msgData.dataReq.msduHandle = mMsduHandle++;
#ifdef gMAC2006_d
		mpPacket->msgData.dataReq.securityLevel = 0;
#endif //gMAC2006_d      

		/* Send the Data Request to the MCPS */
		(void)MSG_Send(NWK_MCPS, mpPacket);
		/* Prepare for another data buffer */
		mpPacket = NULL;
		}
		break;
	default:
		break;
	}
}

/* This function is called in MApp_init.c during initialization ( main() )
 * It will INITIALIZE all required components for the task to work and then
 * call an event for the new Task.*/
void MyNewTaskInit(void)
{
	/* Allocate in memory the timer*/
	myTimerID = TMR_AllocateTimer();
	ledsState = 0;
	/* Create the New Task */
	myNewTaskID = TS_CreateTask(gMyNewTaskPriority_c, MyNewTask);
	/* Send an Event to myNewTask with the first event */
	TS_SendEvent(myNewTaskID, gMyNewTaskEvent1_c);
}

/* This is the function called by the Timer each time it expires */
static void myTaskTimerCallback(tmrTimerID_t timer)
{
	(void)timer; // this line is just to clear a warning
	TS_SendEvent(myNewTaskID, gMyNewTaskEvent2_c);
}

/* Public function to send an event to stop the timer */
void MyTaskTimer_Stop(void)
{
	TS_SendEvent(myNewTaskID, gMyNewTaskEvent3_c);
}
