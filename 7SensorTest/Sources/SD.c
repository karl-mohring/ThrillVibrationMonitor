/******************************************************************************
 *                                                  
 *  (c) copyright Freescale Semiconductor 2008
 *  ALL RIGHTS RESERVED
 *
 *  File Name:   SD.c
 *                                                                          
 *  Description: SD Card using SPI Driver 
 *                                                                                     
 *  Assembler:   Codewarrior for HC(S)08 V6.1
 *                                            
 *  Version:     1.0                                                         
 *                                                                                                                                                         
 *  Author:      Jose Ruiz (SSE Americas)
 *                                                                                       
 *  Location:    Guadalajara,Mexico                                              
 *                                                                                                                  
 *                                                  
 * UPDATED HISTORY:
 *
 * REV   YYYY.MM.DD  AUTHOR        DESCRIPTION OF CHANGE
 * ---   ----------  ------        --------------------- 
 * 1.0   2008.02.18  Jose Ruiz     Initial version
 * 
 ******************************************************************************/
/* Freescale  is  not  obligated  to  provide  any  support, upgrades or new */
/* releases  of  the Software. Freescale may make changes to the Software at */
/* any time, without any obligation to notify or provide updated versions of */
/* the  Software  to you. Freescale expressly disclaims any warranty for the */
/* Software.  The  Software is provided as is, without warranty of any kind, */
/* either  express  or  implied,  including, without limitation, the implied */
/* warranties  of  merchantability,  fitness  for  a  particular purpose, or */
/* non-infringement.  You  assume  the entire risk arising out of the use or */
/* performance of the Software, or any systems you design using the software */
/* (if  any).  Nothing  may  be construed as a warranty or representation by */
/* Freescale  that  the  Software  or  any derivative work developed with or */
/* incorporating  the  Software  will  be  free  from  infringement  of  the */
/* intellectual property rights of third parties. In no event will Freescale */
/* be  liable,  whether in contract, tort, or otherwise, for any incidental, */
/* special,  indirect, consequential or punitive damages, including, but not */
/* limited  to,  damages  for  any loss of use, loss of time, inconvenience, */
/* commercial loss, or lost profits, savings, or revenues to the full extent */
/* such  may be disclaimed by law. The Software is not fault tolerant and is */
/* not  designed,  manufactured  or  intended by Freescale for incorporation */
/* into  products intended for use or resale in on-line control equipment in */
/* hazardous, dangerous to life or potentially life-threatening environments */
/* requiring  fail-safe  performance,  such  as  in the operation of nuclear */
/* facilities,  aircraft  navigation  or  communication systems, air traffic */
/* control,  direct  life  support machines or weapons systems, in which the */
/* failure  of  products  could  lead  directly to death, personal injury or */
/* severe  physical  or  environmental  damage  (High  Risk Activities). You */
/* specifically  represent and warrant that you will not use the Software or */
/* any  derivative  work of the Software for High Risk Activities.           */
/* Freescale  and the Freescale logos are registered trademarks of Freescale */
/* Semiconductor Inc.                                                        */
/*****************************************************************************/

/* Includes */
#include "SD.h"
#include "FslTypes.h"
#include "Vib_Cont_01.h"

T32_8 gu8SD_Argument;
static UINT8 gu8SD_Response[5];
static UINT8 gu8SD_CSD[16];
extern UINT32 u32DataSector;

/***********************************************************************
 *
 *    SD_Init
 *
 *  Description: Configures the SPI2 module and the File system to handle
 *                 an SD Card.
 *
 *  Returns:     OK if SPI configured, SD card is present, CMD0 true, 
 *                  SD card initialised, block length set to 512, 
 *                  SPI clock set to 4MHz.
 *               NO_SD_CARD if no SD card is present.
 *               INIT_FAILS if CMD0 (idle) fails, block length set fails.
 *               
 *               .
 *
 *************************************************************************/
UINT8 SD_Init(void) {

	UINT8 Count;
	UINT8 res;
	UINT8 CRC7_BYTE;
	UINT8 j;
	UINT8 i = 0;

	/* Configure the SD Card inserted detection pin */_SD_PRESENT = _IN; // Configure the data direction as IN.
	SD_PRESENT_PU = 1; // Enable Internal Pull-UP Resistor

	/* Initialise SPI Module */InitSPI(); // SS high, 375kHz clock.

	/* Delay SD card requires a delay of at least 1mSec after power up */
	for (j = 0; j < 200; j++) {
		for (Count = 0; Count < 255; Count++) {
			i += 1;
		}
	}

	/* Check for SD */
	if (SD_PRESENT) // Pin is low when SD Card present
//        return(NO_SD_CARD);
		return (0x11);

	/*** Start SD card Init ***/
	/* Minimum of 74 dummy clock cycles with MOSI high & SS enabled (low) */
//    SPI_SS = ENABLE;           // Enable SS
//    SD_CLKDelay(10);            // Send 80 clocks
//    SPI_SS_Disable_Delay(); 
//    SPI_SS = DISABLE;
	SD_CLKDelay(10); // Send 80 clocks with SS disabled
	SPI_SS_Disable_Delay();

	/* CMD0 - GO_IDLE_STATE
	 Reset the sd card & place it in SPI mode.
	 CRC must be valid.
	 Valid reset command is 0x40, 0x00, 0x00, 0x00, 0x00, 0x95.
	 Correct response is R1 (1 byte) = SD_IDLE (0x01).  
	 Note that the command frame starts with 
	 bits 47:46=0:1 (start & transmission bits) 
	 so the CMD byte has b6 set. */

	SPI_SS = ENABLE;
	gu8SD_Argument.lword = 0; // CMD0 argument is 0x00, 0x00, 0x00, 0x00.
	if (SD_SendCommand(SD_CMD0 | 0x40, SD_IDLE)) // returns OK (0x00) if OK
			{
		SPI_SS = DISABLE;
//        return(INIT_FAILS);
		return (0x12);
	}
	/* Card is in Idle State */
	SPI_SS_Disable_Delay();
	SPI_SS = DISABLE;

	SD_CLKDelay(1); // 8 dummy clocks

	/* Check if card is Ver2 compliant.
	 Send a SEND_IF_COND (CMD8) with supply voltage (VHS) set to 2.7-3.6volts.
	 Argument will be 0x000001AA and CRC7 + end bit 0x87.
	 CMD8 response is R7 (5 bytes - R1 + argument). */

	SPI_SS = ENABLE;
	gu8SD_Argument.lword = 0x000001AA;
	CRC7_BYTE = 0x87;
	if (SD_SendCommandR7(SD_CMD8, CRC7_BYTE)) {
		SPI_SS = DISABLE;
//        return(INIT_FAILS);
		return (0x13);
	}
	/* Card is version 2 compliant */
	SPI_SS_Disable_Delay();
	SPI_SS = DISABLE;

	SD_CLKDelay(1); // 8 dummy clocks

	/* Read the Operation Conditions Register OCR & check for correct voltage range.
	 Valid voltage range of 2.7-3.6 volts is indicated by 
	 gu8SD_Response[2:3] = 0xFF:0x80. Ref Table 5-1 P92.
	 Send READ_OCR CMD58.
	 CMD58 response R3 (5bytes - R1 + OCR). */SPI_SS = ENABLE;
	gu8SD_Argument.lword = 0x00000000;
	CRC7_BYTE = 0xFD;
	if (SD_SendCommandR3(SD_CMD58, SD_IDLE, CRC7_BYTE)) // Correct R1 is SD_IDLE (0x01)
			{
		SPI_SS = DISABLE;
//        return(INIT_FAILS);
		return (0x14);
	}
	/* Valid response received */
	SPI_SS_Disable_Delay();
	SPI_SS = DISABLE;
	SD_CLKDelay(1); // 8 dummy clocks
	/* Check for correct voltage range */
	if ((gu8SD_Response[2] != 0xFF) && (gu8SD_Response[3] != 0x80)) {
//      return(INIT_FAILS);
		return (0x15);
	}

	/* Initialise Card */
	/* Send continuous SD_SEND_OP_COND (ACMD41) until R1 b0 clears. */

	j = 0; // Clear loop counter
	do {
		j++;
		/* The next command is an application-specific command */
		do { // Wait for a successful CMD55
			/* The next command is an application-specific command */
			SPI_SS = ENABLE; // Enable SS
			gu8SD_Argument.lword = 0x00000000;
//        res = SD_SendCommand(SD_CMD55|0x40,0x80);  // returns OK (0x00) if  
			res = SD_SendCommand(SD_CMD55 | 0x40, SD_IDLE); // returns OK (0x00) if
															//  any valid response received
			SPI_SS_Disable_Delay();
			SPI_SS = DISABLE;
			SD_CLKDelay(1); // 8 dummy clocks
		} while (res);

		/* Send SD_SEND_OP_COND (ACMD41) */
		do {

			SPI_SS = ENABLE; // Enable SS
			/* High Capacity Support (HCS - argument b30) is set if host supports
			 SDHC or SDXC, wgich it does not in this case. */
//        gu8SD_Argument.lword = 0x40000000;            // HCS set
			gu8SD_Argument.lword = 0x00000000; // HCS clear
			res = SD_SendCommand(SD_CMD41 | 0x40, 0x80); // returns OK (0x00) if
//        res = SD_SendCommand(SD_CMD41|0x40,SD_OK);     // returns OK (0x00) if
					//  any valid response received
			SPI_SS_Disable_Delay();
			SPI_SS = DISABLE;
			SD_CLKDelay(1); // 8 dummy clocks
		} while (res);
	} while ((gu8SD_Response[0] != SD_OK) && (j < SD_IDLE_WAIT_MAX));

	/* Assume we're OK if we didn't hit the timeout */
	if (j >= SD_IDLE_WAIT_MAX)
//       return(INIT_FAILS);
		return (0x16);

	/* Read the Operation Conditions Register OCR & 
	 check the Card Power Up Status bit b31 and Card Capacity Status (CCS) b30. 
	 b31 & b30 of the OCR which is gu8SD_Response[1] b6. 
	 Power Up Status Bit will be 1 when the card is finished powering up.
	 CCS is 0 for a SDSC. Ref Table 5-1 P92.
	 Send READ_OCR CMD58.
	 CMD58 response R3 (5bytes - R1 + OCR). */SPI_SS = ENABLE;
	gu8SD_Argument.lword = 0x00000000;
	CRC7_BYTE = 0xFD;
	if (SD_SendCommandR3(SD_CMD58, SD_OK, CRC7_BYTE)) // Correct R1 response is SD_OK (0x00)
			{
		SPI_SS = DISABLE;
//        return(INIT_FAILS);
		return (0x17);
	}
	/* Valid response received */
	SPI_SS_Disable_Delay();
	SPI_SS = DISABLE;
	SD_CLKDelay(1); // 8 dummy clocks

	/* Check Power Up Status */
	if ((gu8SD_Response[1] & 0x80) == 0x00)
		return (0x19); // return Power Up not finished.

	/* Check CCS  */
	if (gu8SD_Response[1] & 0x40)
		return (0x1A); // return Card is not SDSC.

	/*  Set the Block Length to 512 bytes */SPI_SS = ENABLE;

	gu8SD_Argument.lword = SD_BLOCK_SIZE; // 512 bytes
	if (SD_SendCommand(SD_CMD16 | 0x40, SD_OK)) {
		SPI_SS = DISABLE;
//        return(INIT_FAILS);
		return (0x18);
	}

	SPI_SS_Disable_Delay();
	SPI_SS = DISABLE;
	SD_CLKDelay(1); // 8 dummy clocks

//    WriteSPIByte(0x00);
//    WriteSPIByte(0x00);

	/* Read the Card-Specific Data register (CSD) to gu8SD_CSD[16] */
	if (SD_Read_CSD()) {
		return (0x1B); // error 11. CSD read fail
	}

	HighSpeedSPI();

	return (OK);
}

/*****************************************************************************
 *
 *   SPI_SS_Disable_Delay
 *
 *  Description:  Provides a short delay to allow the completion of a character 
 *                transmission before disabling the slave select.
 *
 ******************************************************************************/
void SPI_SS_Disable_Delay(void) {
	UINT8 Count, i = 0;
	for (Count = 0; Count < 50; Count++) {
		i += 1;
	}

}

/***************************************************************************************
 *
 *     *** SD_CBufferDataSectorWrite ***
 *
 *     Description: Write a 512 bytes from the circular buffer as a data sector.
 *                  Assume:
 *                         - SD Card has been previously erased.
 *                         - Write a full sector at a time.
 *
 *     Entry: void. 
 *
 ****************************************************************************************/
void SD_CBufferDataSectorWrite(void) {
	UINT16 u16Counter;

	SPI_SS = ENABLE;

	gu8SD_Argument.lword = u32DataSector++; // Set WRITE_BLOCK command argument
											//     to the current data segment
	gu8SD_Argument.lword = gu8SD_Argument.lword << SD_BLOCK_SHIFT; // Convert sector number to
																   // byte address

	if (SD_SendCommand(SD_CMD24 | 0x40, SD_OK)) // WRITE_BLOCK command
			{
		SPI_SS = DISABLE;
		return;
	}

	WriteSPIByte(0xFE); // Start Block Token

	for (u16Counter = 0; u16Counter < BLOCK_SIZE; u16Counter++) // Write the block
		WriteSPIByte(FromCBuffer());

	WriteSPIByte(0xFF); // checksum Bytes not needed
	WriteSPIByte(0xFF);

//    for(u16Counter=0;u16Counter<BLOCK_SIZE;u16Counter++);

//    if((ReadSPIByte() & 0x0F) != 0x05)                // Read the status token.
	// Correct response is 0x05
	// Foust p5.
	while ((SPI_Receive_byte() & 0x0F) != 0x05)
		;
//    {
//        SPI_SS=DISABLE;
//        return;      
//    }

	while (SPI_Receive_byte() == 0x00)
		; // Wait while the card is busy

	SPI_SS = DISABLE;

	SD_CLKDelay(1); // 8 dummy clocks

	return;

}

/*****************************************************************************
 *
 **************SD_Write_Block************
 *
 *      Description: Write a 512 byte sector (block) to the SD card.
 *
 *      Inputs:      u16SD_Block - the sector (block) number where the block is
 *                                 to be written.
 *                   pu8DataPointer - the 512 byte buffer that is to be written
 *                                    to the SD card.
 *
 *****************************************************************************/
UINT8 SD_Write_Block(UINT32 u16SD_Block, UINT8 *pu8DataPointer) {
	UINT16 u16Counter;

	SPI_SS = ENABLE;

	gu8SD_Argument.lword = u16SD_Block; // Set WRITE_BLOCK command argument
	gu8SD_Argument.lword = gu8SD_Argument.lword << SD_BLOCK_SHIFT; // Convert sector number to
																   // byte address

	if (SD_SendCommand(SD_CMD24 | 0x40, SD_OK)) // WRITE_BLOCK command
			{
		SPI_SS = DISABLE;
		return (WRITE_COMMAND_FAILS);
	}

	WriteSPIByte(0xFE); // Start Block Token

	for (u16Counter = 0; u16Counter < BLOCK_SIZE; u16Counter++) // Write the block
		WriteSPIByte(*pu8DataPointer++);

	WriteSPIByte(0xFF); // checksum Bytes not needed
	WriteSPIByte(0xFF);

	for (u16Counter = 0; u16Counter < BLOCK_SIZE; u16Counter++)
		;

	if ((ReadSPIByte() & 0x0F) != 0x05) // Read the status token.
										// Correct response is 0x05
										// Foust p5.
			{
		SPI_SS = DISABLE;
		return (WRITE_DATA_FAILS);
	}

	while (SPI_Receive_byte() == 0x00)
		; // Wait while the card is busy

	SPI_SS = DISABLE;

	SD_CLKDelay(1); // 8 dummy clocks

	return (OK);
}

/*****************************************************************************
 *
 ************* SD_Erase_Blocks ***********
 *
 *      Description: Erase multiple sectors (blocks) no the SD card.
 *
 *      Inputs:      u32SD_Start_Block - the sector (block) number of the first
 *                                 block to be erased.
 *                   u32SD_End_Block - the sector (block) number of the last
 *                                    block to be erased.
 *
 *****************************************************************************/
UINT8 SD_Erase_Blocks(UINT32 u32SD_Start_Block, UINT32 u32SD_End_Block) {
	UINT8 res;
	UINT16 u16Counter;

	SPI_SS = ENABLE;

	/* Set the address of the first write block to be erased */
	gu8SD_Argument.lword = u32SD_Start_Block; // Set ERASE_WR_BLK_START_ADDR 
											  //   command argument
	gu8SD_Argument.lword = gu8SD_Argument.lword << SD_BLOCK_SHIFT; // Convert sector number to
																   // byte address

	if (SD_SendCommand(SD_CMD32 | 0x40, SD_OK)) // ERASE_WR_BLK_START_ADDR command
			{
		SPI_SS = DISABLE;
		return (WRITE_COMMAND_FAILS);
	}

	SD_CLKDelay(1); // 8 dummy clocks

	/* Set the address of the last write block to be erased */
	gu8SD_Argument.lword = u32SD_End_Block; // Set ERASE_WR_BLK_END_ADDR 
											//   command argument
	gu8SD_Argument.lword = gu8SD_Argument.lword << SD_BLOCK_SHIFT; // Convert sector number to
																   // byte address

	if (SD_SendCommand(SD_CMD33 | 0x40, SD_OK)) // ERASE_WR_BLK_END_ADDR command
			{
		SPI_SS = DISABLE;
		return (WRITE_COMMAND_FAILS);
	}

	SD_CLKDelay(1); // 8 dummy clocks

	/* Send the erase command */
	gu8SD_Argument.lword = 0x00000000; // stuff bits 

	if (SD_SendCommand(SD_CMD38 | 0x40, SD_OK)) // ERASE command
			{
		SPI_SS = DISABLE;
		return (WRITE_COMMAND_FAILS);
	}

	/* Wait while card is busy. Indicated by reading 0x00 from SD Card */
	u16Counter = 0; // Counter to see if it is doing something
	do {
		res = SPI_Receive_byte();
		u16Counter++;
	} while (res == 0x00); // Wait while the card is busy

	SPI_SS = DISABLE;

	SD_CLKDelay(1); // 8 dummy clocks

	return (OK);
}

/*****************************************************************************
 *
 *     *** SD_Read_CSD ***
 *
 *     Read the SD Card Card-Specific Data Register (CSD).
 *
 *     The CSD is a 16 byte SD Card register that is read with the 
 *       CMD9 SEND_CSD command.
 *
 *     Output: If read successful return OK with CSD register in gu8SD_CSD[0-16]
 *                with MSB in gu8SD_CSD[0].
 *             Else return READ_COMMAND_FAILS.
 *
 *
 ******************************************************************************/
UINT8 SD_Read_CSD(void) {
	UINT8 u8Temp = 0, u8Counter;

	SPI_SS = ENABLE;

	gu8SD_Argument.lword = 0x00000000;

	if (SD_SendCommand(SD_CMD9 | 0x40, SD_OK)) {
		SPI_SS = DISABLE;
		return (READ_COMMAND_FAILS);
	}

	while (u8Temp != 0xFE)
		u8Temp = SPI_Receive_byte();

	for (u8Counter = 0; u8Counter < 16; u8Counter++)
		gu8SD_CSD[u8Counter] = SPI_Receive_byte();

	(void) ReadSPIByte(); // Dummy SPI cycle for CRC
	(void) ReadSPIByte();

	SPI_SS = DISABLE;

	SD_CLKDelay(1); // 8 dummy clocks

	return (OK);
}

UINT8 SD_Read_Block(UINT32 u16SD_Block, UINT8 *pu8DataPointer) {
	UINT8 u8Temp = 0;
	UINT16 u16Counter;

	SPI_SS = ENABLE;

	gu8SD_Argument.lword = u16SD_Block;
	gu8SD_Argument.lword = gu8SD_Argument.lword << SD_BLOCK_SHIFT;

	if (SD_SendCommand(SD_CMD17 | 0x40, SD_OK)) {
		SPI_SS = DISABLE;
		return (READ_COMMAND_FAILS);
	}

	while (u8Temp != 0xFE)
		u8Temp = SPI_Receive_byte();

	for (u16Counter = 0; u16Counter < BLOCK_SIZE; u16Counter++)
		*pu8DataPointer++ = SPI_Receive_byte();

	(void) ReadSPIByte(); // Dummy SPI cycle, read CRC
	(void) ReadSPIByte();

	SPI_SS = DISABLE;

	SD_CLKDelay(1); // 8 dummy clocks

	return (OK);
}

/*****************************************************************************
 *
 *   SD_SendCommand
 *
 *  Description:  Sends a command to the SD card and receives the response.
 *                All data is sent MSB first, and MSb first.
 *                NB: Does not handle SS signal.
 *
 *  Input:        u8SDCommand - The command to send with start bit (b7) clear
 *                              and transmission bit (b6) set.
 *                u8SDResponse - The correct R1 response byte.
 *                               If 0x80 don't care what the response is so
 long as its a byte with b7 = 0.
 *                
 *  Response:     Return OK (0x00) if a response is received, 
 *                       COMMAND_FAILS (0x01) if not.
 *                Response is stored in response[] with the number of response
 *                     bytes controlled by response_type.
 *
 *****************************************************************************/
UINT8 SD_SendCommand(UINT8 u8SDCommand, UINT8 u8SDResponse) {
	UINT8 u8Counter, CRC7;
	volatile UINT8 u8Temp = 0;

	loopUntilReady();

	/* Send command byte */WriteSPIByte(u8SDCommand);

	/* Send Argument, MSB first. */
	for (u8Counter = 0; u8Counter < 4; u8Counter++)
		WriteSPIByte(gu8SD_Argument.bytes[u8Counter]);

	/* Send CRC */
	switch (u8SDCommand) {
	case (SD_CMD55 | 0x40): // CMD55 with zero argument
		CRC7 = 0x65;
		break;
	case (SD_CMD41 | 0x40): // ACMD41 with argument 0x40000000
		CRC7 = 0x77;
		break;
	default:
		CRC7 = 0x95;
	}
	WriteSPIByte(CRC7);

	/* Response Handler */
	u8Counter = SD_WAIT_CYCLES; // 10 byte reads

	if (u8SDResponse == 0x80) {

		/* Get any valid response - a byte with b7 clear */
		do {
			u8Temp = ReadSPIByte(); // Transmit 0xFF
			u8Counter--;
		} while ((u8Temp & 0x80) && u8Counter > 0);

	} else {

		/* Get a response that matches u8SDResponse */
		do {
			u8Temp = ReadSPIByte(); // Transmit 0xFF
			u8Counter--;
		} while ((u8Temp != u8SDResponse) && u8Counter > 0);

	}

	/* Save the response */
	gu8SD_Response[0] = u8Temp;
	gu8SD_Response[1] = u8Counter;

	/* Set up return conditions */
	if (u8Counter)
		return (OK); // 0x00
	else
		return (COMMAND_FAILS); // 0x01

}

/*****************************************************************************
 *
 *   SD_SendCommandR7
 *
 *  Description:  Sends a command to the SD card that requires an R7 (5 byte) response. 
 *                Used by SEND_IF_COND (CMD8) to check if card is Ver2 compliant.
 *
 *  Input:        u8SDCommand - the command.
 *                u8CRC7 - the CRC7 + end bit for the command (with Tx bit set)
 *                             and argument.
 *                The 4 byte argument "gu8SD_Argument" must be set before 
 *                     calling this function.
 *
 *  Response:     OK (0x00) if valid R7 response received else COMMAND_FAILS (0x01).
 *
 *****************************************************************************/
UINT8 SD_SendCommandR7(UINT8 u8SDCommand, UINT8 u8CRC7) {
	UINT8 u8Counter, mlCounter, i;
	volatile UINT8 u8Temp = 0;

	loopUntilReady();

	/* Debug checks */
	for (i = 0; i <= 4; i++) { // Initialise gu8SD_Response[] so you can be sure its changed
		gu8SD_Response[i] = 0xAA;
	}

	/* Main loop - Send the command and wait for a valid response */
	mlCounter = 5; // loop max of 5 times
	do {
		mlCounter--;
		/* Send command byte with set transmission bit (b6) set */WriteSPIByte(
				u8SDCommand | 0x40);

		/* Send Argument */
		for (u8Counter = 0; u8Counter < 4; u8Counter++) {
			i = gu8SD_Argument.bytes[u8Counter];
			WriteSPIByte(gu8SD_Argument.bytes[u8Counter]);
		}

		/* Send CRC */WriteSPIByte(u8CRC7);

		/* Response Handler. 
		 Do up to SD_WAIT_CYCLES card reads looking for a valid response.
		 Valid response is 5 bytes which are stored in gu8SD_Response[].
		 Valid response is received MSB first.
		 The five bytes are: 
		 - gu8SD_Response[0]; MSB is a normal R1 response which is SD_IDLE (0x01).
		 - gu8SD_Response[1-4]; is the argument that was transmitted.
		 The card responds with 0xFF until it transmits the valid response. */
		u8Counter = SD_WAIT_CYCLES; // Do up to SD_WAIT_CYCLES card reads

		/* Response loop - Do up to SD_WAIT_CYCLES card reads looking for a valid response */
		do // Look for a R1 response byte
		{
			u8Temp = ReadSPIByte(); // Transmit 0xFF. MOSI line must be kept high
			u8Counter--;
		} while ((u8Temp & 0xC0) && u8Counter > 0); // Response loop test
													// - exit on valid response or timeout

	} while ((u8Temp & 0xC0) && mlCounter > 0); // Main loop test
												// - exit on valid response or timeout

	/* Place the 5 byte response in gu8SD_Response[] */
	gu8SD_Response[0] = u8Temp; // R1
	for (i = 1; i <= 4; i++) { // Argument
		gu8SD_Response[i] = ReadSPIByte();
	}

	/* Set up return condition */
	if ((gu8SD_Response[0] == SD_IDLE)
			&& (gu8SD_Response[1] == gu8SD_Argument.bytes[0])
			&& (gu8SD_Response[2] == gu8SD_Argument.bytes[1])
			&& (gu8SD_Response[3] == gu8SD_Argument.bytes[2])
			&& (gu8SD_Response[4] == gu8SD_Argument.bytes[3]))
		return (OK); // 0x00

	else
		return (COMMAND_FAILS); // 0x01
}

/*****************************************************************************
 *
 *   SD_SendCommandR3
 *
 *  Description:  Sends a command to the SD card that requires an R3 (5 byte) response. 
 *                Used by SEND_IF_COND (CMD58) to read the OCR.
 *
 *  Input:        u8SDCommand - the command.
 *                u8SDResponse - The correct R1 response for this command.
 *                u8CRC7 - the CRC7 + end bit for the command (with Tx bit set)
 *                             and argument.
 *                The 4 byte argument "gu8SD_Argument" must be set before 
 *                     calling this function.
 *
 *  Response:     OK (0x00) if valid R3 response received else COMMAND_FAILS (0x01).
 *                The R1 response is available in  gu8SD_Response[0].
 *                The 32 bit Operation Conditions Register (OCR) is available in
 *                    gu8SD_Response[1-4] with MSB in gu8SD_Response[1].
 *
 *****************************************************************************/
UINT8 SD_SendCommandR3(UINT8 u8SDCommand, UINT8 u8SDResponse, UINT8 u8CRC7) {
	UINT8 u8Counter, mlCounter, i;
	volatile UINT8 u8Temp = 0;

	loopUntilReady();

	/* Debug checks */
	for (i = 0; i <= 4; i++) { // Initialise gu8SD_Response[] so you can be sure its changed
		gu8SD_Response[i] = 0xAA;
	}

	/* Main loop - Send the command and wait for a valid response */
	mlCounter = 5; // loop max of 5 times
	do {
		mlCounter--;
		/* Send command byte with set transmission bit (b6) set */WriteSPIByte(
				u8SDCommand | 0x40);

		/* Send Argument */
		for (u8Counter = 0; u8Counter < 4; u8Counter++) {
			i = gu8SD_Argument.bytes[u8Counter];
			WriteSPIByte(gu8SD_Argument.bytes[u8Counter]);
		}

		/* Send CRC */WriteSPIByte(u8CRC7);

		/* Response Handler. 
		 Do up to SD_WAIT_CYCLES card reads looking for a valid response.
		 Valid response is 5 bytes which are stored in gu8SD_Response[].
		 Valid response is received MSB first.
		 The five bytes are: 
		 - gu8SD_Response[0]; MSB is a normal R1 response which is SD_IDLE (0x01).
		 - gu8SD_Response[1-4]; is the argument that was transmitted.
		 The card responds with 0xFF until it transmits the valid response. */
		u8Counter = SD_WAIT_CYCLES; // Do up to SD_WAIT_CYCLES card reads

		/* Response loop - Do up to SD_WAIT_CYCLES card reads looking for a valid response */
		do // Look for a R1 response byte
		{
			u8Temp = ReadSPIByte(); // Transmit 0xFF. MOSI line must be kept high
			u8Counter--;
		} while ((u8Temp != u8SDResponse) && u8Counter > 0); // Response loop test
//    } while((u8Temp & 0xC0) && u8Counter > 0);   // Response loop test
		// - exit on valid response or timeout

	} while ((u8Temp != u8SDResponse) && mlCounter > 0); // Main loop test
//  } while((u8Temp & 0xC0) && mlCounter > 0);     // Main loop test
	// - exit on valid response or timeout

	/* Place the 5 byte response in gu8SD_Response[] */
	gu8SD_Response[0] = u8Temp; // R1
	for (i = 1; i <= 4; i++) { // OCR
		gu8SD_Response[i] = ReadSPIByte();
	}

	/* Set up return condition */
	if (gu8SD_Response[0] == u8SDResponse)
		return (OK); // 0x00

	else
		return (COMMAND_FAILS); // 0x01
}

void SD_CLKDelay(UINT8 u8Frames) {
	while (u8Frames--)
		WriteSPIByte(0xFF);
}

void loopUntilReady() {
	SD_CLKDelay(5);
	//SPI_Send_byte(0xFF);
	//while (SPI_Receive_byte() != 0xFF) {
	//	SPI1_Send_byte(0xFF);
	//}
}
