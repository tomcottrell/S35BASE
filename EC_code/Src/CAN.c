////////////////////////////////////////////////////////////////////////////////////////////////////
// fileName: can.c
//
// Description: Low level CAN functions
//
// Author: Enovation controls
//
//         http://www.enovationcontrols.com
//
// copyright Copyright 2024 by Enovation Controls.
//            This file and/or parts of it are subject to
//            software license terms.
//
//            Without written approval of Enovation Controls this
//            software may not be copied, redistributed or used
//            in any other way.
//
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////////////////////////
#include <stdbool.h>
#include <stdint.h>
#include "CAN.h"
#include "TickTimers.h"
#include "MiscFunctions.h"
#include "ConfigMemory.h"
#include "nonVolStorage.h"
#include <stdlib.h>


////////////////////////////////////////////////////////////////////////////////////////////////////
//Defines
////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////
// Typedefs
////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////
// Private variables
////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////
// Private interface
////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////
// Public interface
////////////////////////////////////////////////////////////////////////////////////////////////////
//CAN RX FIFO
uint8_t can_fifo_in_pointer = 0;
uint8_t can_fifo_out_pointer = 0;
uint32_t can_fifo_arbid[CAN_FIFO_DEPTH];
uint8_t can_fifo_dlen[CAN_FIFO_DEPTH];
uint8_t can_fifo_data[CAN_FIFO_DEPTH][8];

CAN_TxHeaderTypeDef CAN_tx_header; 		//header for message transmissions
uint32_t CAN_tx_mailbox;

uint8_t can_tx_data[8];
uint8_t can_rx_data[8];

uint32_t can_rx_count = 0;
uint32_t can_tx_count = 0;

//J1939 variables
uint16_t j1939EngineSpeed;
uint16_t j1939EngineOilPressure;
int16_t j1939EngineCoolantTemp;
uint16_t j1939EngineLoad;
uint16_t j1939Pitch;
uint16_t j1939Roll;

//MC2
uint16_t j1939PTOStatus;
uint16_t j1939EStopStatus;

uint16_t j1939Truck_Latched;
uint16_t j1939Truck_Lowered;
uint16_t j1939Truck_Moving;
uint16_t j1939Truck_Raised;
uint16_t j1939PTO_Disabled;
uint16_t j1939EStop_Active;
uint16_t j1939Angle_Exceeded;
uint16_t j1939Truck_Press;
uint16_t j1939TruckProximity;
uint16_t j1939Trailer_Latched;
uint16_t j1939Trailer_Lowered;
uint16_t j1939Trailer_Moving;
uint16_t j1939Trailer_Raised;
uint16_t j1939Trailer_Press;
uint16_t j1939TrailerProximity;
uint16_t j1939Overpressure;

uint8_t Raisetest; //added
uint8_t Lowertest;
uint8_t Latchtest;
uint8_t Traystate;

uint8_t TrailerRaisetest = 0;
uint8_t TrailerLowertest = 0;
uint8_t TrailerLatchtest = 0;
uint8_t TrailerTraystate = 0;

// In your CAN variables section, add a flag
volatile uint8_t can_data_updated = 0;

////////////////////////////////////////////////////////////////////////
//
// clear_CAN_tx_data()
//
// Sets all CAN TX data to 0xFF
//
//    Parameters:
//       none
//
////////////////////////////////////////////////////////////////////////
void clear_CAN_tx_data()
{
	uint8_t i;

	for(i=0;i<8;++i)
		can_tx_data[i] = 0xFF;
}


////////////////////////////////////////////////////////////////////////
//
// CAN_transmit_coms()
//
// Main function for all CAN transmits
//
//    Parameters:
//       none
//
////////////////////////////////////////////////////////////////////////
void CAN_transmit_coms()
{
	 //Transmit 29 bit message at 50mS rate
     if(can_tx_timer == 0 && HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) != 0)
	 {
		can_tx_timer = 50;

		CAN_tx_header.IDE=CAN_ID_EXT; 							//set identifier to extended
		CAN_tx_header.DLC=8; 									//set message data length (0-8)
		CAN_tx_header.RTR=CAN_RTR_DATA; 						//set data type to DATA
		CAN_tx_header.ExtId=0x18FEF100 + CAN_SOURCE_ADDRESS;	//set message identifier (29 bits)

		clear_CAN_tx_data();

		can_tx_data[0] = 0x00;
		can_tx_data[1] = 0x11;
		can_tx_data[2] = 0x22;

		if (HAL_CAN_AddTxMessage(&hcan1, &CAN_tx_header, can_tx_data, &CAN_tx_mailbox) != HAL_OK)
		{
			//Error_Handler ();
		}
		else
		{
			//Successful TX
			++can_tx_count;
		}
	 }

     //Transmit PGN 66521 message at 100mS rate
     if(can_tx_timer2 == 0 && HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) != 0)
     	{
     		can_tx_timer2 = 100;

     		CAN_tx_header.IDE=CAN_ID_EXT; 							//set identifier to extended
     		CAN_tx_header.DLC=8; 									//set message data length (0-8)
     		CAN_tx_header.RTR=CAN_RTR_DATA; 						//set data type to DATA
     		// PGN 66521 (0x10408) with source address
     		CAN_tx_header.ExtId=0x18FFF100 + CAN_SOURCE_ADDRESS;	//set message identifier (29 bits)

     		clear_CAN_tx_data();

     		//can_tx_data[0] = 0x01;	// Byte 1: 0x01 as requested
     		can_tx_data[0] = Raisetest;	// Byte 1: 0x01 as requested
     		can_tx_data[1] = Lowertest;	// Byte 2: 0x01 as requested
     		can_tx_data[2] = Latchtest;
     		can_tx_data[3] = Traystate;
     		can_tx_data[4] = TrailerRaisetest;
     		can_tx_data[5] = TrailerLowertest;
     		can_tx_data[6] = TrailerLatchtest;
     		can_tx_data[7] = TrailerTraystate;

     		if (HAL_CAN_AddTxMessage(&hcan1, &CAN_tx_header, can_tx_data, &CAN_tx_mailbox) != HAL_OK)
     		{
     			//Error_Handler ();
     		}
     		else
     		{
     			//Successful TX
     			++can_tx_count;
     		}
     	 }
         // NEW: TouchGFX Settings transmission - PGN 65522 (0xFFF2) at 1000ms rate
         if(can_tx_timer3 == 0 && HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) != 0)
         {
             can_tx_timer3 = 1000;

             CAN_tx_header.IDE=CAN_ID_EXT;
             CAN_tx_header.DLC=8;
             CAN_tx_header.RTR=CAN_RTR_DATA;
             // PGN 65522 (0xFFF2) with source address
             CAN_tx_header.ExtId=0x18FFF200 + CAN_SOURCE_ADDRESS;

             clear_CAN_tx_data();

             // Byte 0: Vehicle Type (single byte, max value is 1)
             can_tx_data[0] = (uint8_t)S35_config[TOUCHGFX_VEHICLETYPE];

             // Byte 1-2: Truck Max (2 bytes, default 400)
             can_tx_data[1] = (uint8_t)(S35_config[TOUCHGFX_TRUCKMAX] & 0xFF);        // Low byte
             can_tx_data[2] = (uint8_t)((S35_config[TOUCHGFX_TRUCKMAX] >> 8) & 0xFF); // High byte

             // Byte 3: Truck Min (single byte, max value 100)
             can_tx_data[3] = (uint8_t)S35_config[TOUCHGFX_TRUCKMIN];

             // Byte 4-5: Trailer Max (2 bytes, default 400)
             can_tx_data[4] = (uint8_t)(S35_config[TOUCHGFX_TRAILERMAX] & 0xFF);        // Low byte
             can_tx_data[5] = (uint8_t)((S35_config[TOUCHGFX_TRAILERMAX] >> 8) & 0xFF); // High byte

             // Byte 6: Trailer Min (single byte, max value 100)
             can_tx_data[6] = (uint8_t)S35_config[TOUCHGFX_TRAILERMIN];

             // Byte 7: Reserved for future use
             can_tx_data[7] = 0xFF;

             if (HAL_CAN_AddTxMessage(&hcan1, &CAN_tx_header, can_tx_data, &CAN_tx_mailbox) != HAL_OK)
             {
                 //Error_Handler ();
             }
             else
             {
                 ++can_tx_count;
             }
         }

         // NEW: Pressure Settings transmission - PGN 65523 (0xFFF3) at 1000ms rate
         if(can_tx_timer4 == 0 && HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) != 0)
         {
             can_tx_timer4 = 1000;

             CAN_tx_header.IDE=CAN_ID_EXT;
             CAN_tx_header.DLC=8;
             CAN_tx_header.RTR=CAN_RTR_DATA;
             // PGN 65523 (0xFFF3) with source address
             CAN_tx_header.ExtId=0x18FFF300 + CAN_SOURCE_ADDRESS;

             clear_CAN_tx_data();

             // Byte 0-1: Lowered Setpoint (2 bytes, default 50)
             can_tx_data[0] = (uint8_t)(S35_config[TOUCHGFX_TRUCK_LOWERED] & 0xFF);        // Low byte
             can_tx_data[1] = (uint8_t)((S35_config[TOUCHGFX_TRUCK_LOWERED] >> 8) & 0xFF); // High byte

             // Byte 2-3: Raised Setpoint (2 bytes, default 280)
             can_tx_data[2] = (uint8_t)(S35_config[TOUCHGFX_TRUCK_RAISED] & 0xFF);        // Low byte
             can_tx_data[3] = (uint8_t)((S35_config[TOUCHGFX_TRUCK_RAISED] >> 8) & 0xFF); // High byte

             // Byte 4-5: Overload Setpoint (2 bytes, default 380)
             can_tx_data[4] = (uint8_t)(S35_config[TOUCHGFX_TRUCK_OVERLOAD] & 0xFF);        // Low byte
             can_tx_data[5] = (uint8_t)((S35_config[TOUCHGFX_TRUCK_OVERLOAD] >> 8) & 0xFF); // High byte

             // Byte 6-7: Emergency Setpoint (2 bytes, default 395)
             can_tx_data[6] = (uint8_t)(S35_config[TOUCHGFX_TRUCK_EMERGENCY] & 0xFF);        // Low byte
             can_tx_data[7] = (uint8_t)((S35_config[TOUCHGFX_TRUCK_EMERGENCY] >> 8) & 0xFF); // High byte

             if (HAL_CAN_AddTxMessage(&hcan1, &CAN_tx_header, can_tx_data, &CAN_tx_mailbox) != HAL_OK)
             {
                 //Error_Handler ();
             }
             else
             {
                 ++can_tx_count;
             }
         }
         // NEW: Trailer Pressure Settings transmission - PGN 65524 (0xFFF4) at 1000ms rate
         if(can_tx_timer5 == 0 && HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) != 0)
         {
             can_tx_timer5 = 1000;

             CAN_tx_header.IDE=CAN_ID_EXT;
             CAN_tx_header.DLC=8;
             CAN_tx_header.RTR=CAN_RTR_DATA;
             // PGN 65524 (0xFFF4) with source address
             CAN_tx_header.ExtId=0x18FFF400 + CAN_SOURCE_ADDRESS;

             clear_CAN_tx_data();

             // Byte 0-1: Trailer Lowered Setpoint (2 bytes, default 50)
             can_tx_data[0] = (uint8_t)(S35_config[TOUCHGFX_TRAILER_LOWERED] & 0xFF);        // Low byte
             can_tx_data[1] = (uint8_t)((S35_config[TOUCHGFX_TRAILER_LOWERED] >> 8) & 0xFF); // High byte

             // Byte 2-3: Trailer Raised Setpoint (2 bytes, default 280)
             can_tx_data[2] = (uint8_t)(S35_config[TOUCHGFX_TRAILER_RAISED] & 0xFF);        // Low byte
             can_tx_data[3] = (uint8_t)((S35_config[TOUCHGFX_TRAILER_RAISED] >> 8) & 0xFF); // High byte

             // Byte 4-5: Trailer Overload Setpoint (2 bytes, default 380)
             can_tx_data[4] = (uint8_t)(S35_config[TOUCHGFX_TRAILER_OVERLOAD] & 0xFF);        // Low byte
             can_tx_data[5] = (uint8_t)((S35_config[TOUCHGFX_TRAILER_OVERLOAD] >> 8) & 0xFF); // High byte

             // Byte 6-7: Trailer Emergency Setpoint (2 bytes, default 395)
             can_tx_data[6] = (uint8_t)(S35_config[TOUCHGFX_TRAILER_EMERGENCY] & 0xFF);        // Low byte
             can_tx_data[7] = (uint8_t)((S35_config[TOUCHGFX_TRAILER_EMERGENCY] >> 8) & 0xFF); // High byte

             if (HAL_CAN_AddTxMessage(&hcan1, &CAN_tx_header, can_tx_data, &CAN_tx_mailbox) != HAL_OK)
             {
                 //Error_Handler ();
             }
             else
             {
                 ++can_tx_count;
             }
         }
         if(can_tx_timer6 == 0 && HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) != 0)
         {
             can_tx_timer6 = 100;

             CAN_tx_header.IDE=CAN_ID_EXT;
             CAN_tx_header.DLC=8;
             CAN_tx_header.RTR=CAN_RTR_DATA;
             // PGN 66522 (0x1040A) with source address
             CAN_tx_header.ExtId=0x18FFF500 + CAN_SOURCE_ADDRESS;

             clear_CAN_tx_data();

             can_tx_data[0] = S35_config[TOUCHGFX_ANGLE];   // Byte 1: Trailer raise command
             can_tx_data[1] =  S35_config[TOUCHGFX_INCLO_BUBBLE];   // Byte 2: Trailer lower command
             can_tx_data[2] = S35_config[TOUCHGFX_PRESSURE_BUBBLE];   // Byte 3: Trailer latch command
             //can_tx_data[3] = TrailerTraystate;   // Byte 4: Trailer state

             if (HAL_CAN_AddTxMessage(&hcan1, &CAN_tx_header, can_tx_data, &CAN_tx_mailbox) != HAL_OK)
             {
                 //Error_Handler ();
             }
             else
             {
                 ++can_tx_count;
             }
         }

}


////////////////////////////////////////////////////////////////////////
//
// CAN_receive_coms()
//
// Main function for all CAN reception
//
//    Parameters:
//       none
//
////////////////////////////////////////////////////////////////////////
void CAN_receive_coms()
{
	// Add this static variable at the top of CAN_receive_coms() function

	uint8_t i;
	uint8_t can_rx_Dlen;
	uint16_t j1939PGN;


	while(can_fifo_in_pointer != can_fifo_out_pointer)
	{
		//CAN RX data available from RX FIFO: reset receive timeout
		can_rx_timer = CAN_RX_TIMEOUT;

		++can_rx_count;

		//Extract all data for next received message
		can_rx_Dlen = can_fifo_dlen[can_fifo_out_pointer];			//Message data length
		j1939PGN = ((can_fifo_arbid[can_fifo_out_pointer] & 0x00FFFF00) / 0x100);

		for(i=0;i<can_rx_Dlen;++i)
			can_rx_data[i] = can_fifo_data[can_fifo_out_pointer][i];	//Message data

		//Process received data here
		//Extract SPN data from PGN
		switch(j1939PGN)
		{
			case 0xF004:
				//SPN190: Engine speed
				j1939EngineSpeed = make16(can_rx_data[4], can_rx_data[3]);
				j1939EngineSpeed /= 8;				//J1939 scaling
				break;
			case 0xFEEF:
				//SPN 100: Engine pressures
				j1939EngineOilPressure = can_rx_data[3];
				j1939EngineOilPressure *= 4;		//j1939 scaling
				break;
			case 0xFEEE:
				//SPN 110: Engine temperatures
				j1939EngineCoolantTemp = can_rx_data[0];
				j1939EngineCoolantTemp -= 40;		//j1939 offset
				break;
			case 0xF003:
				//SPN 92: Engine load
				j1939EngineLoad = can_rx_data[2];
				break;
			case 0xF013:
				//SPN 3318: Pitch Angle
	            uint16_t newRoll = make16(can_rx_data[3], can_rx_data[2]);
	            newRoll *= 0.002;

	            // Only update if significant change (reduce noise)
	            if (abs((int16_t)newRoll - (int16_t)j1939Roll) > 1)
	            {
	                j1939Roll = newRoll;
	                can_data_updated = 1;
	            }
				break;
			case 0xFF16:
				//PTO Status
				//Byte 1 Parameters
				j1939PTOStatus = (can_rx_data[0] & 0x01) ? 1 : 0;
				j1939EStopStatus = (can_rx_data[0] & 0x02) ? 1 : 0;
				j1939TruckProximity = (can_rx_data[0] & 0x04) ? 1 : 0;
				j1939TrailerProximity = (can_rx_data[0] & 0x08) ? 1 : 0;
				//j1939EStopStatus = (can_rx_data[0] & 0x10) ? 1 : 0;
				//j1939EStopStatus = (can_rx_data[0] & 0x20) ? 1 : 0;
				//j1939EStopStatus = (can_rx_data[0] & 0x40) ? 1 : 0;
				//j1939EStopStatus = (can_rx_data[0] & 0x80) ? 1 : 0;
				j1939Truck_Press = make16(can_rx_data[2], can_rx_data[1]);
				j1939Trailer_Press = make16(can_rx_data[4], can_rx_data[3]);

				break;
				//Byte 3 bit 6 example
			case 0xFF17: //Error Messages from MC2
				//Byte 1 Parameters
				j1939Truck_Latched = (can_rx_data[0] & 0x01) ? 1 : 0;
				j1939Truck_Lowered = (can_rx_data[0] & 0x02) ? 1 : 0;
				j1939Truck_Moving = (can_rx_data[0] & 0x04) ? 1 : 0;
				j1939Truck_Raised = (can_rx_data[0] & 0x08) ? 1 : 0;
				//j1939PTO_Disabled = (can_rx_data[0] & 0x10) ? 1 : 0;
				//j1939EStop_Active = (can_rx_data[0] & 0x20) ? 1 : 0;
				//j1939Angle_Exceeded = (can_rx_data[0] & 0x40) ? 1 : 0;
				//j1939EStopStatus = (can_rx_data[0] & 0x80) ? 1 : 0;
				break;
			case 0xFF18: //Error Messages from MC2
				j1939PTO_Disabled = (can_rx_data[0] & 0x01) ? 1 : 0;
				j1939EStop_Active = (can_rx_data[0] & 0x02) ? 1 : 0;
				j1939Angle_Exceeded = (can_rx_data[0] & 0x04) ? 1 : 0;
				j1939Overpressure = (can_rx_data[0] & 0x08) ? 1 : 0;
				break;
			case 0xFF19: // Trailer Status Messages (similar to truck 0xFF17)
			    // Byte 1 Parameters - Only position states, no error states
			    j1939Trailer_Latched = (can_rx_data[0] & 0x01) ? 1 : 0;
			    j1939Trailer_Lowered = (can_rx_data[0] & 0x02) ? 1 : 0;
			    j1939Trailer_Moving = (can_rx_data[0] & 0x04) ? 1 : 0;
			    j1939Trailer_Raised = (can_rx_data[0] & 0x08) ? 1 : 0;
			    // Bytes 2-3: Trailer pressure (if needed)
			    //j1939Trailer_Press = make16(can_rx_data[2], can_rx_data[1]);
		}

		//Update FIFO out pointer
		if(++can_fifo_out_pointer >= CAN_FIFO_DEPTH)
			can_fifo_out_pointer = 0;
	}

	//////////////////////////////
	// Check for CAN RX timeout //
	//////////////////////////////
	if(can_rx_timer == 0)
	{
		//RX timeout:
		j1939EngineSpeed = 0xFFFF;
		j1939EngineOilPressure = 0xFFFF;
		j1939EngineCoolantTemp = 0xFFFF;
		j1939EngineLoad = 0xFFFF;
		j1939Pitch = 0xFFFF;
		j1939Roll = 0xFFFF;
		j1939PTOStatus = 0xFFFF;
		j1939EStopStatus = 0xFFFF;
		//j1939Truck_Latched = 0xFFFF;
		//j1939Truck_Lowered = 0xFFFF;
		//j1939Truck_Moving = 0xFFFF;
		//j1939Truck_Raised = 0xFFFF;
		j1939PTO_Disabled = 0xFFFF;
		j1939EStop_Active = 0xFFFF;
		j1939Angle_Exceeded = 0xFFFF;
		j1939Truck_Press = 0xFFFF;
		j1939TruckProximity = 0xFFFF;
		j1939Trailer_Press = 0xFFFF;
		j1939TrailerProximity = 0xFFFF;
	}
}



void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	CAN_RxHeaderTypeDef CAN_rx_header;
	uint8_t CAN_rx_data[8];
	uint8_t i;

	do
	{
		HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &CAN_rx_header, CAN_rx_data);

		//Store latest 29 bit data in software FIFO
		if(CAN_rx_header.IDE == CAN_ID_EXT)
		{

			can_fifo_arbid[can_fifo_in_pointer] = CAN_rx_header.ExtId;
			can_fifo_dlen[can_fifo_in_pointer] = CAN_rx_header.DLC;
			for(i=0;i<8;++i)
				can_fifo_data[can_fifo_in_pointer][i] = CAN_rx_data[i];

			if(++can_fifo_in_pointer >= CAN_FIFO_DEPTH)
				can_fifo_in_pointer = 0;

		}
	}while(HAL_CAN_GetRxFifoFillLevel(&hcan1, CAN_RX_FIFO0) != 0);

	can_data_updated = 1;  // Signal update
}

void CAN_InitTask(void)
{
    CAN_FilterTypeDef canfilterconfig;

    canfilterconfig.FilterActivation = CAN_FILTER_ENABLE;
    canfilterconfig.FilterBank = 0;  // single can interfaces only have 14, 0-13
    canfilterconfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    canfilterconfig.FilterIdHigh = 0;
    canfilterconfig.FilterIdLow = 0;
    canfilterconfig.FilterMaskIdHigh = 0x0;
    canfilterconfig.FilterMaskIdLow = 0x0000;
    canfilterconfig.FilterMode = CAN_FILTERMODE_IDMASK;
    canfilterconfig.FilterScale = CAN_FILTERSCALE_32BIT;
    canfilterconfig.SlaveStartFilterBank = 20;  // how many filters to assign to the CAN1 (master can)

    HAL_CAN_ConfigFilter(&hcan1, &canfilterconfig);

    //
    HAL_CAN_Start(&hcan1);

    // activate rx notifications
    HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
}
