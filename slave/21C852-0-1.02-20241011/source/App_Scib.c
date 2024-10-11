/*--------------------------------------------------------------------*/
#include "F28335_Device.h"     /* F2812 ÍâÉèÍ·ÎÄ¼þ*/
#include "App_UserDef.h"      /* F2812 Ó¦ÓÃÏîÄ¿Í·ÎÄ¼þ*/
/*--------------------------------------------------------------------*/


#pragma CODE_SECTION(Sci_DataPackage_Tx, "ramfuncs");
#pragma CODE_SECTION(Sci_DataCombine_Rx, "ramfuncs");
#pragma CODE_SECTION(ScibTxIsr, "ramfuncs");
#pragma CODE_SECTION(ScibRxIsr, "ramfuncs");
#pragma CODE_SECTION(ScibRxCheck, "ramfuncs");
#pragma CODE_SECTION(ScibTxCheck, "ramfuncs");
#pragma CODE_SECTION(WriteEEProm, "ramfuncs");

void ScibTxCheck(void)
{
 	volatile Uint16	i;
 	if(SCIB.FLAG.bit.TXBO_FLAG == 0)
 	{

		if(SCIB.m_Tx_Length > 0)
		{
			// ÌîÐ´ FIFOÊý¾Ý£¬ Ò»´ÎÌîÐ´×î¶àÌîÐ´12¸ö
			//×î¶à·¢ËÍ12¸ö×Ö·û,16¸öÈÝÒ×¶ª×Ö·û
			for (i = ScicRegs.SCIFFTX.bit.TXFFST; i < 12;i++ )
			{
				ScicRegs.SCITXBUF = *SCIB.p_Tx_Buffer;
				SCIB.p_Tx_Buffer++;
				SCIB.m_Tx_Length--;
				if(SCIB.m_Tx_Length == 0)	
					break;
			}	
			// ÇëÇóFIFOÔÙ´Î·¢ËÍÊý¾Ý£¬²¢¸ø³öÖÐ¶Ï				
			//ScicRegs.SCIFFTX.bit.TXFIFOXRESET = 1;
		}
 		else
 		{
 			SCIB.FLAG.bit.TXBO_FLAG = 1;
 			SCIB.p_Tx_Buffer = SCI_TX_BUFFER;
			//ScicRegs.SCIFFTX.bit.TXFIFOXRESET = 0;
 		}
 	}
 	
	ScicRegs.SCIFFTX.bit.TXFFINTCLR=1;  // Clear Interrupt flag
	PieCtrlRegs.PIEACK.bit.ACK8=1; 	
}
/*
void ScibRxCheck(void)
{
	volatile Uint16	i,Fi_Number;
	volatile Uint16	Store_Temp = 1;
	volatile Uint16	Sum_Temp = 0;

	if(SCIB.FLAG.bit.TXBO_FLAG == 0)
	{	
		//Store_Temp = ScicRegs.SCIRXBUF.bit.RXDT;
	}
	else
	{	
		//·¢Éú¼ä¶Ï´íÎóµÄÖÐ¶ÏÊ±£¬Í¨¹ýÈí¼þ¸´Î»Î»SWRESETÇåÁãÊµÏÖ±êÖ¾Î»ÇåÁã£¬È»ºóÖÃ1ÖØÐÂÊ¹ÄÜSCI
		if(ScicRegs.SCIRXST.bit.BRKDT == 1)
		{
			ScicRegs.SCICTL1.bit.SWRESET = 0;
			ScicRegs.SCICTL1.bit.SWRESET = 1;

    		ScicRegs.SCIFFTX.bit.TXFIFOXRESET=1;
    		ScicRegs.SCIFFRX.bit.RXFIFORESET=1;
			SCIB.FLAG.bit.TXBO_FLAG = 1;
			SCIB.p_Rx_Buffer = SCI_RX_BUFFER;
			SCIB.m_Rx_Length = 0;
		}
		if(SCIB.FLAG.bit.RX_FLAG == 0)
		{
			if(ScicRegs.SCIFFRX.bit.RXFFST > 0)
			{
				Fi_Number = ScicRegs.SCIFFRX.bit.RXFFST;
				for(i=0;i<Fi_Number;i++)
				{
					if(SCIB.p_Rx_Buffer >= (Uint16 *)(SCI_RX_BUFFER + RX_BUFFER_LENGTH))
					{
						SCIB.p_Rx_Buffer = SCI_RX_BUFFER;
						SCIB.m_Rx_Length = 0;
					}
					Store_Temp = ScicRegs.SCIRXBUF.bit.RXDT;		// ´®¿Ú½ÓÊÕ8Î»Êý¾Ý
					*SCIB.p_Rx_Buffer = (Store_Temp & 0x00FF);
					if(SCIB.p_Rx_Buffer == (Uint16 *)(SCI_RX_BUFFER))
					{
						if(SCI_RX_BUFFER[0]==0x00EB)
						{
							SCIB.p_Rx_Buffer++;
						}
						else
						{
							SCIB.p_Rx_Buffer = SCI_RX_BUFFER;
						}
					}
					else if(SCIB.p_Rx_Buffer == (Uint16 *)(SCI_RX_BUFFER + 0x0001))
					{
						if(SCI_RX_BUFFER[1]==0x0090)
						{
							SCIB.p_Rx_Buffer++;
						}
						else
						{
							SCIB.p_Rx_Buffer = SCI_RX_BUFFER;
						}
					}
					else if(SCIB.p_Rx_Buffer == (Uint16 *)(SCI_RX_BUFFER + 0x0004))
					{
						if(SCI_RX_BUFFER[4] <= (Uint16)(RX_BUFFER_LENGTH - 0x0007))
						{
							SCIB.m_Rx_Length = SCI_RX_BUFFER[4];
							if( (SCI_RX_BUFFER[2] == 0x00A0 || SCI_RX_BUFFER[2] == 0x00A2) 
								&& (SCIB.m_Rx_Length != 1) )
							{
								SCIB.p_Rx_Buffer = SCI_RX_BUFFER;
							}
							else if(SCI_RX_BUFFER[2] == 0x00A1 && SCIB.m_Rx_Length != 5)
							{	
								SCIB.p_Rx_Buffer = SCI_RX_BUFFER;
							}		
							else
								SCIB.p_Rx_Buffer++;
						}
						else
						{
							SCIB.p_Rx_Buffer = SCI_RX_BUFFER;
						}
					}			
					else
					{
						SCIB.p_Rx_Buffer++;
					}
				}
			}
			if(SCIB.p_Rx_Buffer >= (Uint16 *)(SCI_RX_BUFFER + SCIB.m_Rx_Length + 7))
			{
				Sum_Temp = CalCRC16_Byte(SCI_RX_BUFFER, SCIB.m_Rx_Length + 0x0005);
					
				if(Sum_Temp == SCI_RX_BUFFER[SCIB.m_Rx_Length + 5] + (SCI_RX_BUFFER[SCIB.m_Rx_Length + 6]<<8))
				{
					SCIB.FLAG.bit.RX_FLAG = 1;
					SCIB.p_Rx_Buffer = SCI_RX_BUFFER;
					SCIB.m_Rx_Length = 0;
				}
				else
				{
					SCIB.p_Rx_Buffer = SCI_RX_BUFFER;
					SCIB.m_Rx_Length = 0;
				}

			}				
	 	}
	}
 	

	ScicRegs.SCIFFRX.bit.RXFFOVRCLR=1;  // Clear Overflow flag
	ScicRegs.SCIFFRX.bit.RXFFINTCLR=1; 	// Clear Interrupt flag
	PieCtrlRegs.PIEACK.bit.ACK8=1;
}
*/
void ScibRxCheck(void)
{
	volatile Uint16	i,Fi_Number;
	volatile Uint16	Store_Temp = 1;
	volatile Uint16	Sum_Temp = 0;

	if(ScicRegs.SCIRXST.bit.BRKDT == 1)
	{
		ScicRegs.SCICTL1.bit.SWRESET = 0;
		ScicRegs.SCICTL1.bit.SWRESET = 1;

		ScicRegs.SCIFFTX.bit.TXFIFOXRESET=1;
		ScicRegs.SCIFFRX.bit.RXFIFORESET=1;
		SCIB.FLAG.bit.TXBO_FLAG = 1;
		SCIB.FLAG.bit.RX_FLAG = 0;
		SCIB.p_Rx_Buffer = SCI_RX_BUFFER;
		SCIB.p_Rx_AllBuffer = SCI_RX_ALLBUFFER;
		SCIB.m_Rx_Length = 0;
	}

	/*if(ScicRegs.SCIFFRX.bit.RXFFST > 0)
	{
		Fi_Number = ScicRegs.SCIFFRX.bit.RXFFST;
		for(i=0;i<Fi_Number;i++)
		{
			if(SCIB.p_Rx_AllBuffer >= (Uint16 *)(SCI_RX_ALLBUFFER + RX_ALLBUFFER_LENGTH))
			{
				SCIB.p_Rx_AllBuffer = SCI_RX_ALLBUFFER;
			}
			Store_Temp = ScicRegs.SCIRXBUF.bit.RXDT;
			*SCIB.p_Rx_AllBuffer = (Store_Temp & 0x00FF);
			SCIB.p_Rx_AllBuffer++;
		}
		ScicRegs.SCIFFRX.bit.RXFFOVRCLR=1;  // Clear Overflow flag
		ScicRegs.SCIFFRX.bit.RXFFINTCLR=1; 	// Clear Interrupt flag
		PieCtrlRegs.PIEACK.bit.ACK8=1;
	}*/

	if(SCIB.FLAG.bit.RX_FLAG == 0 && SCIB.FLAG.bit.TXBO_FLAG == 1 
		&& SCIB.p_Rx_AllBuffer > (Uint16 *)(SCI_RX_ALLBUFFER + 4))
	{
		SCIB.FLAG.bit.RXTIME_FLAG = 1;
			if(SCI_RX_ALLBUFFER[2] == 0x0000)
			{
				SCIB.FLAG.bit.PC_FLAG = 1;
			}
			else if(SCI_RX_ALLBUFFER[2] == 0x00A0 
				|| SCI_RX_ALLBUFFER[2] == 0x00A1
				|| SCI_RX_ALLBUFFER[2] == 0x00A2)
			{
				SCIB.FLAG.bit.PC_FLAG = 0;
			}
			if( SCI_RX_ALLBUFFER[0] != 0x00EB )
			{
				SCIB.p_Rx_AllBuffer = SCIB.p_Rx_AllBuffer - 1;
				for(i=0;i<(SCIB.p_Rx_AllBuffer - SCI_RX_ALLBUFFER);i++)
				{
					SCI_RX_ALLBUFFER[i] = SCI_RX_ALLBUFFER[i + 1];
				}
			}
			else if( SCI_RX_ALLBUFFER[1] != 0x0090 )
			{
				SCIB.p_Rx_AllBuffer = SCIB.p_Rx_AllBuffer - 1;
				for(i=0;i<(SCIB.p_Rx_AllBuffer - SCI_RX_ALLBUFFER);i++)
				{
					SCI_RX_ALLBUFFER[i] = SCI_RX_ALLBUFFER[i + 1];
				}
			}
			else if(SCIB.FLAG.bit.PC_FLAG == 0 
					&& SCI_RX_ALLBUFFER[2] != 0x00A0 
					&& SCI_RX_ALLBUFFER[2] != 0x00A1 
					&& SCI_RX_ALLBUFFER[2] != 0x00A2)
			{
				SCIB.p_Rx_AllBuffer = SCIB.p_Rx_AllBuffer - 1;
				for(i=0;i<(SCIB.p_Rx_AllBuffer - SCI_RX_ALLBUFFER);i++)
				{
					SCI_RX_ALLBUFFER[i] = SCI_RX_ALLBUFFER[i + 1];
				}
			}
			else
			{
				SCIB.m_Rx_Length = SCI_RX_ALLBUFFER[4];
				if( (SCI_RX_ALLBUFFER[2] == 0x00A0 || SCI_RX_ALLBUFFER[2] == 0x00A2) 
					&& (SCIB.m_Rx_Length != 1) )
				{
					SCIB.p_Rx_AllBuffer = SCIB.p_Rx_AllBuffer - 1;
					for(i=0;i<(SCIB.p_Rx_AllBuffer - SCI_RX_ALLBUFFER);i++)
					{
						SCI_RX_ALLBUFFER[i] = SCI_RX_ALLBUFFER[i + 1];
					}
				}
				else if(SCI_RX_ALLBUFFER[2] == 0x00A1 && SCIB.m_Rx_Length != 5)
				{
					SCIB.p_Rx_AllBuffer = SCIB.p_Rx_AllBuffer - 1;
					for(i=0;i<(SCIB.p_Rx_AllBuffer - SCI_RX_ALLBUFFER);i++)
					{
						SCI_RX_ALLBUFFER[i] = SCI_RX_ALLBUFFER[i + 1];
					}
				}
				else if(SCIB.m_Rx_Length > (Uint16)(RX_BUFFER_LENGTH - 0x0007))
				{
					SCIB.p_Rx_AllBuffer = SCIB.p_Rx_AllBuffer - 1;
					for(i=0;i<(SCIB.p_Rx_AllBuffer - SCI_RX_ALLBUFFER);i++)
					{
						SCI_RX_ALLBUFFER[i] = SCI_RX_ALLBUFFER[i + 1];
					}
				}
				else if(SCIB.p_Rx_AllBuffer >= (Uint16 *)(SCI_RX_ALLBUFFER + SCIB.m_Rx_Length + 7))
				{
					for(i=0;i<SCIB.m_Rx_Length + 7;i++)
					{
						SCI_RX_BUFFER[i] = SCI_RX_ALLBUFFER[i];
					}
					Sum_Temp = CalCRC16_Byte(SCI_RX_BUFFER, SCIB.m_Rx_Length + 0x0005);
						
					if(Sum_Temp == SCI_RX_BUFFER[SCIB.m_Rx_Length + 5] + (SCI_RX_BUFFER[SCIB.m_Rx_Length + 6]<<8))
					{
						SCIB.FLAG.bit.RX_FLAG = 1;
						SCIB.p_Rx_AllBuffer = (SCIB.p_Rx_AllBuffer - SCIB.m_Rx_Length - 7);
						for(i=0;i<(SCIB.p_Rx_AllBuffer - SCI_RX_ALLBUFFER);i++)
						{
							SCI_RX_ALLBUFFER[i] = SCI_RX_ALLBUFFER[i + SCIB.m_Rx_Length + 7];
						}
					}
					else
					{
						SCIB.p_Rx_AllBuffer = (SCIB.p_Rx_AllBuffer - 1);
						for(i=0;i<(SCIB.p_Rx_AllBuffer - SCI_RX_ALLBUFFER);i++)
						{
							SCI_RX_ALLBUFFER[i] = SCI_RX_ALLBUFFER[i + 1];
						}
					}
				}
			}
			SCIB.p_Rx_Buffer = SCI_RX_BUFFER;
			SCIB.m_Rx_Length = 0;
		SCIB.FLAG.bit.RXTIME_FLAG = 0;		
 	}
 	

	//ScicRegs.SCIFFRX.bit.RXFFOVRCLR=1;  // Clear Overflow flag
	//ScicRegs.SCIFFRX.bit.RXFFINTCLR=1; 	// Clear Interrupt flag
	//PieCtrlRegs.PIEACK.bit.ACK8=1;
}


/*----------------------------------------------------------------------------------------
 ·¢ËÍÊý¾Ý´ò°ü³ÌÐò
 ½«ÐèÒª·¢ËÍµÄÊý¾Ý°´ÕÕÍ¨Ñ¶¹æÔ¼´ò°ü³ÉÊý¾Ýèå
 Èë¿Ú²ÎÊý:ÐèÒª·¢ËÍµÄÊý¾ÝµØÖ·¡¢·¢ËÍµÄÃüÁî×Ö¡¢·¢ËÍµÄÊý¾Ý³¤¶È(×Ö)
 ³ö¿Ú²ÎÊý:ÎÞ
----------------------------------------------------------------------------------------*/
void Sci_DataPackage_Tx(_iq	*p_tx_data_L, Uint16 m_tx_command_L, Uint16 m_tx_length_L)
{
	volatile Uint16	i;
	volatile Uint16	Data_Temp;
	
	*SCIB.p_Tx_Buffer++ = 0x00EB;
	*SCIB.p_Tx_Buffer++ = 0x0090;
	*SCIB.p_Tx_Buffer++ = m_tx_command_L & 0x00FF;

	// ·¢ËÍµÄÊý¾Ý×Ö½ÚÊý¼ÓÉÏÐ§Ñé×Ö½Ú
	*SCIB.p_Tx_Buffer++ = ((m_tx_length_L << 2)) >> 8;
	*SCIB.p_Tx_Buffer++ = ((m_tx_length_L << 2)) & 0x00FF;

		
	for(i = 0; i < m_tx_length_L; i++)
	{
		Data_Temp = ((Uint32)(*p_tx_data_L) >> 24) & 0x00FF;
		*SCIB.p_Tx_Buffer++ = Data_Temp;
		Data_Temp = ((Uint32)(*p_tx_data_L) >> 16) & 0x00FF;
		*SCIB.p_Tx_Buffer++ = Data_Temp;
		Data_Temp = ((Uint32)(*p_tx_data_L) >> 8) & 0x00FF;
		*SCIB.p_Tx_Buffer++ = Data_Temp;
		Data_Temp = (*p_tx_data_L) & 0x00FF;
		p_tx_data_L++;
		*SCIB.p_Tx_Buffer = Data_Temp;
		SCIB.p_Tx_Buffer++;
	}
	
	Data_Temp = CalCRC16_Byte(SCI_TX_BUFFER, (m_tx_length_L << 2) + 5);
	*SCIB.p_Tx_Buffer++ = (Data_Temp & 0x00FF); 					/* ÀÛ¼ÓÇóÐ§ÑéºÍ*/
	*SCIB.p_Tx_Buffer = (Data_Temp>>8); 					/* ÀÛ¼ÓÇóÐ§ÑéºÍ*/
	SCIB.p_Tx_Buffer = SCI_TX_BUFFER;	
	SCIB.m_Tx_Length = (m_tx_length_L<<2) + 2 + 5;

}

/*--------------------------------------------------------------------------------
 ´®¿Ú·¢ËÍÖÐ¶Ï·þÎñ×Ó³ÌÐò
-------------------------------------------------------------------------------*/
interrupt void ScibTxIsr(void)
{ 	
	ScicRegs.SCIFFTX.bit.TXFFINTCLR=1;  // Clear Interrupt flag
	PieCtrlRegs.PIEACK.bit.ACK8=1; 	
}




/*--------------------------------------------------------------------------------
 ´®¿Ú½ÓÊÕÖÐ¶Ï·þÎñ×Ó³ÌÐò
-------------------------------------------------------------------------------*/
interrupt void ScibRxIsr(void)
{ 	

	ScicRegs.SCIFFRX.bit.RXFFOVRCLR=1;  // Clear Overflow flag
	ScicRegs.SCIFFRX.bit.RXFFINTCLR=1; 	// Clear Interrupt flag
	PieCtrlRegs.PIEACK.bit.ACK8=1;
}

void Sci_DataCombine_Rx(Uint16 *p_Rx)
{
	volatile Uint16	i=0,j=0;
	for(i=1; i<p_Rx[4]; i+=4)
	{	
		SCI_ReceData[j] = p_Rx[i+4];
		SCI_ReceData[j] = (Uint32)SCI_ReceData[j] << 8;
		SCI_ReceData[j] += p_Rx[i+5];
		SCI_ReceData[j] = (Uint32)SCI_ReceData[j] << 8;
		SCI_ReceData[j] += p_Rx[i+6];
		SCI_ReceData[j] = (Uint32)SCI_ReceData[j] << 8;
		SCI_ReceData[j] += p_Rx[i+7];
		j = j + 1;
	}
	
	SCIB.p_Rx_Buffer = SCI_RX_BUFFER;
}

//----------------------------------------------------------------------------------------
// EPROMµÄ´ò°ü³ÌÐò
// ½«ÐèÒªÐ´ÈëEEPROMµÄÊý¾Ý°´ÕÕÍ¨Ñ¶¹æÔ¼´ò°ü³ÉÊý¾Ýèå
// Èë¿Ú²ÎÊý:ÐèÒªÐ´ÈëµÄÊý¾ÝµØÖ·¡¢·¢ËÍµÄÃüÁî×Ö¡¢·¢ËÍµÄÊý¾Ý³¤¶È(×Ö)
// ³ö¿Ú²ÎÊý:ÎÞ
//----------------------------------------------------------------------------------------

void EEPROM_DataPackage(_iq	*p_tx_data_L, Uint16 m_tx_length_L)
{
	volatile Uint16	i;
	volatile Uint16	Data_Temp;
	p_EEProm_Buffer = (Uint16 *)EEPROM_BUFFER;
	p_EEProm_Buffer++;
	p_EEProm_Buffer++;
	p_EEProm_Buffer++;
	*p_EEProm_Buffer++ = ((m_tx_length_L << 2)) >> 8;
	*p_EEProm_Buffer++ = ((m_tx_length_L << 2)) & 0x00FF;

	for(i = 0; i < m_tx_length_L; i++)
	{
		Data_Temp = ((*p_tx_data_L) >> 24) & 0x00FF;
		*p_EEProm_Buffer++ = Data_Temp;
		Data_Temp = ((*p_tx_data_L) >> 16) & 0x00FF;
		*p_EEProm_Buffer++ = Data_Temp;
		Data_Temp = ((*p_tx_data_L) >> 8) & 0x00FF;
		*p_EEProm_Buffer++ = Data_Temp;
		Data_Temp = (*p_tx_data_L++) & 0x00FF;
		*p_EEProm_Buffer++ = Data_Temp;
	}

	p_EEProm_Buffer = (Uint16 *)EEPROM_BUFFER;
}


void Cmd_Deal(void)
{
	volatile	Uint16	command,i=0;
	command = SCI_RX_BUFFER[2];

	switch(command)
   	{
   	/*³õÊ¼»¯¼ì²â¿ØÖÆÆ÷Ö¸Áî*/
   	case 0x0000:
		SCI_SendData[0] = Board_Num;
		SCI_SendData[1] = BORAD_NUM;
		if(SCIB.FLAG.bit.TXBO_FLAG ==1)
   		{
	   	  	Sci_DataPackage_Tx(SCI_SendData,0x0070,2);
	   		SCIB.FLAG.bit.TXBO_FLAG = 0;
	   		//ScicRegs.SCITXBUF = *SCIB.p_Tx_Buffer++;
			//ScicRegs.SCIFFTX.bit.TXFFIENA = 1;//Ê¹ÄÜ·¢ËÍÖÐ¶Ï
    
   		}
   		break;
	case 0x0001:		//Çå³ý¹ÊÕÏ±êÖ¾Ö¸Áî
		Sys_Flag.bit.STOP_PWM_Flag_Id = 0;		// "1"±íÃ÷µç»ú¹ýÁ÷±£»¤
		Sys_Flag.bit.STOP_PWM_Flag_Velo = 0;	// "1"±íÃ÷³¬ËÙ±£»¤
		Sys_Flag.bit.UDC_FLAG = 0;				// "1"±íÃ÷Ö±Á÷µçÑ¹¹ÊÕÏ
		Sys_Flag.bit.STOP_PWM_Flag_Driv = 0;

		if(SCIB.FLAG.bit.TXBO_FLAG ==1)
		{
			Sci_DataPackage_Tx(SCI_SendData,0x0071,0);
			SCIB.FLAG.bit.TXBO_FLAG = 0;
	   		//ScicRegs.SCITXBUF = *SCIB.p_Tx_Buffer++;
			//ScicRegs.SCIFFTX.bit.TXFFIENA = 1;//Ê¹ÄÜ·¢ËÍÖÐ¶Ï
		}
		break;
   	/*--------------------------Lock--------------------------*/
   	case 0x000D:		//LOCK
   		Sci_DataCombine_Rx(SCI_RX_BUFFER);
   		Usa_Lock = SCI_ReceData[0];
   		Usb_Lock = SCI_ReceData[1];
   		Usc_Lock = SCI_ReceData[2];
   		Udc_Lock = SCI_ReceData[3];
		Choose_Mche= SCI_ReceData[4];
   		Ctrl_Flag.bit.LOCK_FLAG = 1;
	   	Ctrl_Flag.bit.OPEN_LOOP_FLAG = 0;
   		if(SCIB.FLAG.bit.TXBO_FLAG ==1)
   		{
   			Sci_DataPackage_Tx(SCI_SendData,0x007D,0);
	   		SCIB.FLAG.bit.TXBO_FLAG = 0;
	   		//ScicRegs.SCITXBUF = *SCIB.p_Tx_Buffer++;
			//ScicRegs.SCIFFTX.bit.TXFFIENA = 1;//Ê¹ÄÜ·¢ËÍÖÐ¶Ï
	   	}
   		break;
   	/*--------------------------Read Position--------------------------*/
	case 0x0002:		//Read Position
		SCI_SendData[0] = Isa;
   		SCI_SendData[1] = Isb;
   		SCI_SendData[2] = Isc;
   		SCI_SendData[3] = rdc1.DATA_BUFFER;
		SCI_SendData[4] = Idc;
   		//SCI_SendData[5] = IsbB;
   		//SCI_SendData[6] = IscB;
   		SCI_SendData[7] = rdc2.DATA_BUFFER;

		if(SCIB.FLAG.bit.TXBO_FLAG ==1)
		{
			Sci_DataPackage_Tx(SCI_SendData,0x0072,8);
			SCIB.FLAG.bit.TXBO_FLAG = 0;
	   		//ScicRegs.SCITXBUF = *SCIB.p_Tx_Buffer++;
			//ScicRegs.SCIFFTX.bit.TXFFIENA = 1;//Ê¹ÄÜ·¢ËÍÖÐ¶Ï
		}
		break;
   	/*--------------------------Openloop--------------------------*/
	case 0x0003:		//Openloop

		Sci_DataCombine_Rx(SCI_RX_BUFFER);

	   	Um_OpenLoop = SCI_ReceData[0];
	   	Freq_OpenLoop = SCI_ReceData[1];
		Udc_OpenLoop = SCI_ReceData[2];
		Choose_Mche= SCI_ReceData[3];
	   	Ctrl_Flag.bit.OPEN_LOOP_FLAG = 1;
   	   	Ctrl_Flag.bit.LOCK_FLAG = 0;

		if(SCIB.FLAG.bit.TXBO_FLAG ==1)
		{
			Sci_DataPackage_Tx(SCI_SendData,0x0073,0);
			SCIB.FLAG.bit.TXBO_FLAG = 0;
	   		//ScicRegs.SCITXBUF = *SCIB.p_Tx_Buffer++;
			//ScicRegs.SCIFFTX.bit.TXFFIENA = 1;//Ê¹ÄÜ·¢ËÍÖÐ¶Ï
		}
		break;
		/*--------------------------Release LOCK--------------------------*/
	case 0x000E:		//Release LOCK
		Choose_Mche= SCI_ReceData[0];
   	   	Ctrl_Flag.bit.LOCK_FLAG = 0;
	   	 	Ctrl_Flag.bit.OPEN_LOOP_FLAG = 0;
		if(SCIB.FLAG.bit.TXBO_FLAG ==1)
		{
			Sci_DataPackage_Tx(SCI_SendData,0x007E,0);
			SCIB.FLAG.bit.TXBO_FLAG = 0;
	   		//ScicRegs.SCITXBUF = *SCIB.p_Tx_Buffer++;
			//ScicRegs.SCIFFTX.bit.TXFFIENA = 1;//Ê¹ÄÜ·¢ËÍÖÐ¶Ï
		}
		break;
   	/*--------------------------Ð´µç»ú²ÎÊýºÍ»ùÖµÏµÍ³Ö¸Áî--------------------------*/

	case 0x0004:		//Ð´µç»ú²ÎÊýºÍ»ùÖµÏµÍ³Ö¸Áî			lxy

		Sci_DataCombine_Rx(SCI_RX_BUFFER);
   	   		Rs = SCI_ReceData[0];
   	   		Ld = SCI_ReceData[1];
   	   		Lq = SCI_ReceData[2];
   	   		PHI= SCI_ReceData[3];
   	   		Mp = SCI_ReceData[4];
   	   		Rp = SCI_ReceData[5];
   			p = _IQdiv(SCI_ReceData[4],SCI_ReceData[5]);
   	   		Udc_Setg= SCI_ReceData[6];
   	   	    Udc_Mche_realvalue = _IQdiv(_IQ(1),Udc_Setg);

   	   		U_Base = SCI_ReceData[7];
   	   		I_Base = SCI_ReceData[8];
   	   		V_Base = SCI_ReceData[9];		// µçËÙ¶È»ùÖµ(rad/s)
   	   		Angle_Init_Digital = SCI_ReceData[10];       //?????
   			Velo_Max = SCI_ReceData[11];
   			id_Max = SCI_ReceData[12];
   			Is_Add = SCI_ReceData[13];
   			Control_Period = SCI_ReceData[14];
   			id_Filter = SCI_ReceData[15];
   			Udc_Filter = SCI_ReceData[16];
   			Velo_Filter = SCI_ReceData[17];
   			Deadtime = SCI_ReceData[18];
   			Velo_Add = SCI_ReceData[19];
   			Is_DownAdd = SCI_ReceData[20];
   			Temp_Filter = SCI_ReceData[21];
   			Angle_Init_Fa = SCI_ReceData[22];
   			Com_angle_U = SCI_ReceData[23];
   			Com_angle_I = SCI_ReceData[24];
   			idSet_Max = SCI_ReceData[25];
   			//Freq_cut = SCI_ReceData[26];
   			IsLimit_Set = SCI_ReceData[27];
   			MotorTM = SCI_ReceData[28];
   			IgbtTM = SCI_ReceData[29];

   			Velo_PerAdd = _IQmpy(Velo_Add,Per_Ctrl);
			Velo_Duty = _IQdiv(3750000,Control_Period);

   			K_Velo_Cal = _IQdiv(_IQ(62.83185307179586476925286766559), V_Base);//?????
   	   		K_Velo_Cal = _IQmpy(K_Velo_Cal,Velo_Duty0);   //¶¨Ê±Æ÷ÖÜÆÚÎª100us
   			K_Velo_Cal = _IQdiv(K_Velo_Cal,Velo_Calc_Num<<20);
   	   		K_Velo_Cal = _IQmpy(K_Velo_Cal,p);


   			Is_PerAdd = _IQmpy(Is_Add,Per_Ctrl);
   			Is_DownPerAdd = _IQmpy(Is_DownAdd,Per_Ctrl);

   			pi_Id_Kc.Kp_reg 	= _IQmpy(Idq_Kc,Ld);
			pi_Id_Kc.Ki_reg		= _IQmpy(V_Base,Per_Ctrl);
			pi_Id_Kc.Ki_reg		= _IQmpy(pi_Id_Kc.Ki_reg,(pi_Id_Kc.i_period<<20));
			pi_Id_Kc.Ki_reg		= _IQmpy(Idq_Kc,pi_Id_Kc.Ki_reg);
			pi_Iq_Kc.Kp_reg 	= _IQmpy(Idq_Kc,Lq);
			pi_Iq_Kc.Ki_reg		= pi_Id_Kc.Ki_reg;

		if(SCIB.FLAG.bit.TXBO_FLAG ==1)
		{
			Sci_DataPackage_Tx(SCI_SendData,0x0074,0);
			SCIB.FLAG.bit.TXBO_FLAG = 0;
	   		//ScicRegs.SCITXBUF = *SCIB.p_Tx_Buffer++;
			//ScicRegs.SCIFFTX.bit.TXFFIENA = 1;//Ê¹ÄÜ·¢ËÍÖÐ¶Ï
		}

		break;
   	/*--------------------------¶Áµç»ú²ÎýºÍ»ùÖµÏµÍ³Ö¸Áî--------------------------*/
   	case 0x0005:
   	    		SCI_SendData[0] = Rs;
   	    		SCI_SendData[1] = Ld;
   	 	 	    SCI_SendData[2] = Lq;
   	    		SCI_SendData[3] = PHI;
   	    		SCI_SendData[4] = Mp;
   	    		SCI_SendData[5] = Rp;
   	    		SCI_SendData[6]	= Udc_Setg;
   	    		SCI_SendData[7] = U_Base;
   	    		SCI_SendData[8] = I_Base;
   	    		SCI_SendData[9] = V_Base;			// µçÐµËÙ¶È»ùÖµ(rad/s)
   	    		SCI_SendData[10] = Angle_Init_Digital;
   	 		    SCI_SendData[11] = Velo_Max;
   	 		    SCI_SendData[12] = id_Max;
   	 		    SCI_SendData[13] = Is_Add;
   	 		    SCI_SendData[14] = Control_Period;
   	 		    SCI_SendData[15] = id_Filter;
   	 		    SCI_SendData[16] = Udc_Filter;
   	 	    	SCI_SendData[17] = Velo_Filter;
   	 	    	SCI_SendData[18] = Deadtime;
   	 		    SCI_SendData[19] = Velo_Add;
   	 		    SCI_SendData[20] = Is_DownAdd;
   	 	  	    SCI_SendData[21] = Temp_Filter;
   	 		    SCI_SendData[22] = Angle_Init_Fa;
   	 		    SCI_SendData[23] = Com_angle_U;
   	 		    SCI_SendData[24] = Com_angle_I;
   	 		    SCI_SendData[25] = idSet_Max;
   	 		    //SCI_SendData[26] = Freq_cut;
   	 		    SCI_SendData[27] = IsLimit_Set;
   	 		    SCI_SendData[28] = MotorTM;
   	 		    SCI_SendData[29] = IgbtTM;
   		if(SCIB.FLAG.bit.TXBO_FLAG ==1)
   		{
	   	   	Sci_DataPackage_Tx(SCI_SendData,0x0075,30);
	   		SCIB.FLAG.bit.TXBO_FLAG = 0;
	   		//ScicRegs.SCITXBUF = *SCIB.p_Tx_Buffer++;
			//ScicRegs.SCIFFTX.bit.TXFFIENA = 1;//Ê¹ÄÜ·¢ËÍÖÐ¶Ï
	   	}
   		break;
   	/*--------------------------Î»ÖÃ¿ØÖÆÖ¸Áî--------------------------*/
	case 0x0006:		//Î»ÖÃ¿ØÖÆÖ¸Áî

		Sci_DataCombine_Rx(SCI_RX_BUFFER);

		if( Sys_Flag.bit.STOP_PWM_Flag_Driv == 1)
 		{
    		SCI_SendData[0] = 1;
 		}
  		else if( (Ctrl_Flag.bit.FIRST_PWM_Flag == 1 && Velo_Elec_abs > Velo_Start) )
   	 	{
   	    	SCI_SendData[0] = 2;
   	 	}
  		else
  		{
    	  	Isd_Set = SCI_ReceData[0];
 		    Pos_Set = SCI_ReceData[1];
			if(Pos_Set > POSMAX)
				Pos_Set = POSMAX;
			if(Pos_Set < POSMIN)
				Pos_Set = POSMIN;
			TorqueAngleA = SCI_ReceData[2];
 	  	    Ctrl_Flag.bit.VELO_CONTROL_FLAG = 1;
			Ctrl_Flag.bit.POS_CONTROL_FLAG = 1;
 		    Ctrl_Flag.bit.STOP_VELO_FLAG = 0;
 			Ctrl_Flag.bit.TORQ_CONTROL_FLAG	= 1;
 			SCI_SendData[0] = 0;
  		}
		if(SCIB.FLAG.bit.TXBO_FLAG ==1)
		{
			Sci_DataPackage_Tx(SCI_SendData,0x0076,1);
			SCIB.FLAG.bit.TXBO_FLAG = 0;
	   		//ScicRegs.SCITXBUF = *SCIB.p_Tx_Buffer++;
			//ScicRegs.SCIFFTX.bit.TXFFIENA = 1;//Ê¹ÄÜ·¢ËÍÖÐ¶Ï
		}

		break;

   	/*--------------------------ËÙ¶È¿ØÖÆÖ¸Áî--------------------------*/
	case 0x0007:		//ËÙ¶È¿ØÖÆÖ¸Áî

		Sci_DataCombine_Rx(SCI_RX_BUFFER);

		if( Sys_Flag.bit.STOP_PWM_Flag_Driv == 1)
 		{
    		SCI_SendData[0] = 1;
 		}
  		else if( (Ctrl_Flag.bit.FIRST_PWM_Flag == 1 && Velo_Elec_abs > Velo_Start) )
   	 	{
   	    	SCI_SendData[0] = 2;
   	 	}
  		else
  		{
    	  	Isd_Set = SCI_ReceData[0];
 		    Veloover_Set = SCI_ReceData[1];
 		    if(Veloover_Set > Vel_Speed)
				Veloover_Set = Vel_Speed;
 		    if(Veloover_Set < -Vel_Speed)
				Veloover_Set = -Vel_Speed;
			TorqueAngleA = SCI_ReceData[2];
 	  	    Ctrl_Flag.bit.VELO_CONTROL_FLAG = 1;
			Ctrl_Flag.bit.POS_CONTROL_FLAG = 0;
 		    Ctrl_Flag.bit.STOP_VELO_FLAG = 0;
 			Ctrl_Flag.bit.TORQ_CONTROL_FLAG	= 1;
 			SCI_SendData[0] = 0;
  		}
		if(SCIB.FLAG.bit.TXBO_FLAG ==1)
		{
			Sci_DataPackage_Tx(SCI_SendData,0x0077,1);
			SCIB.FLAG.bit.TXBO_FLAG = 0;
	   		//ScicRegs.SCITXBUF = *SCIB.p_Tx_Buffer++;
			//ScicRegs.SCIFFTX.bit.TXFFIENA = 1;//Ê¹ÄÜ·¢ËÍÖÐ¶Ï
		}

		break;
   	/*--------------------------×ª¾Ø¿ØÖÆÖ¸Áî--------------------------*/
   	case 0x0008:
   		Sci_DataCombine_Rx(SCI_RX_BUFFER);
		Choose_Mche = SCI_ReceData[0];
		Choose_Mode = SCI_ReceData[1];
		SCI_SendData[0] = 0;
		SCI_SendData[1] = 0;
		if( Sys_Flag.bit.STOP_PWM_Flag_Driv == 1)
		{
			SCI_SendData[0] = 1;
		}
		else if( (Ctrl_Flag.bit.FIRST_PWM_Flag == 1 && Velo_Elec_abs > Velo_Start) )
		{
			SCI_SendData[0] = 2;
		}
		else
		{
			if(Choose_Mode == 0)
			{
				Isd_Set = SCI_ReceData[2];
				Isdq_Set1 = -SCI_ReceData[3];
				TorqueAngleA = SCI_ReceData[4];
				Ctrl_Flag.bit.TORQ_CONTROL_FLAG = 1;			//ÖÃ×ª¾Ø¿ØÖÆ±êÖ¾
				Ctrl_Flag.bit.VELO_CONTROL_FLAG = 0;
				Ctrl_Flag.bit.POS_CONTROL_FLAG = 0;
				Ctrl_Flag.bit.STOP_VELO_FLAG = 0;
			}
   	  		else if(Choose_Mode == 1)
   	  		{
   	    	    Isd_Set = SCI_ReceData[2];
   	 		    Veloover_Set = SCI_ReceData[3];
   	 		    if(Veloover_Set > Vel_Speed)
					Veloover_Set = Vel_Speed;
   	 		    if(Veloover_Set < -Vel_Speed)
					Veloover_Set = -Vel_Speed;
				TorqueAngleA = SCI_ReceData[4];
   	 	  	    Ctrl_Flag.bit.VELO_CONTROL_FLAG = 1;
   	 		    Ctrl_Flag.bit.STOP_VELO_FLAG = 0;
				Ctrl_Flag.bit.POS_CONTROL_FLAG = 0;
   	 			Ctrl_Flag.bit.TORQ_CONTROL_FLAG	= 1;
   	  		}
   	  		else
   	  		{
   	    	    Isd_Set = SCI_ReceData[2];
   	 		    Pos_Set = SCI_ReceData[3];
				if(Pos_Set > POSMAX)
					Pos_Set = POSMAX;
				if(Pos_Set < POSMIN)
					Pos_Set = POSMIN;
				TorqueAngleA = SCI_ReceData[4];
   	 	  	    Ctrl_Flag.bit.VELO_CONTROL_FLAG = 1;
   	 		    Ctrl_Flag.bit.STOP_VELO_FLAG = 0;
				Ctrl_Flag.bit.POS_CONTROL_FLAG = 1;
   	 			Ctrl_Flag.bit.TORQ_CONTROL_FLAG	= 1;
   	  		}
			SCI_SendData[0] = 0;
		}

   		if(SCIB.FLAG.bit.TXBO_FLAG ==1)
		{
   			Sci_DataPackage_Tx(SCI_SendData,0x0078,2);
   			SCIB.FLAG.bit.TXBO_FLAG = 0;
	   		//ScicRegs.SCITXBUF = *SCIB.p_Tx_Buffer++;
			//ScicRegs.SCIFFTX.bit.TXFFIENA = 1;//Ê¹ÄÜ·¢ËÍÖÐ¶Ï
		}
   		break;
   	/*--------------------------¶ÁÈë´«¸ÐÆ÷--------------------------*/
   	case 0x0009:

   		Sci_DataCombine_Rx(SCI_RX_BUFFER);
				Choose_Mche= SCI_ReceData[0];
	 			Sys_Flag.bit.RDC_FAULT = rdc1.FAULT;
   	    	   	SCI_SendData[0] = Velo_Elec;
   	 		    SCI_SendData[1] = Udc_Mche_1;
   	    	   	SCI_SendData[2] = Id;
   	    	   	SCI_SendData[3] = Iq;
   	 		    SCI_SendData[4] = Sys_Flag.all;
   	    	   	SCI_SendData[5] = IdZ_Set;
   	    	   	SCI_SendData[6] = IqZ_Set;
   	    	   	SCI_SendData[7] = Us;
	   	 		SCI_SendData[8] = pi_Id_Kc.ui_reg;
				SCI_SendData[9] = pi_Iq_Kc.ui_reg;
   	    	   	SCI_SendData[10] = Ud;
   	    	   	SCI_SendData[11] = Uq;
				SCI_SendData[12] = Time_Cal;
   	    	   	//SCI_SendData[12] = Id_5;
   	    	   	SCI_SendData[13] = Temperature1;
   	    	   	SCI_SendData[14] = Veloover_Set;
   	    	   	SCI_SendData[15] = Temperature3;
   	    	   	//SCI_SendData[16] = Iq_5;
   	 		    SCI_SendData[17] = (Ctrl_Flag.all & 0x000F)+(rdc2.FAULT<<4);
   	    	   	SCI_SendData[18] = Temperature5;
   	    	   	SCI_SendData[19] = Idc;
   	    	   	//SCI_SendData[20] = Id_7;
   	    	   	//SCI_SendData[21] = Iq_7;
   	    if(SCIB.FLAG.bit.TXBO_FLAG ==1)
   		{
   			Sci_DataPackage_Tx(SCI_SendData,0x0079,22);
   			SCIB.FLAG.bit.TXBO_FLAG = 0;
	   		//ScicRegs.SCITXBUF = *SCIB.p_Tx_Buffer++;
			//ScicRegs.SCIFFTX.bit.TXFFIENA = 1;//Ê¹ÄÜ·¢ËÍÖÐ¶Ï
	   	}
   		break;
   	/*--------------------------µç»úÍ£×ªÖ¸Áî--------------------------*/
   	case 0x000A:
   		Sci_DataCombine_Rx(SCI_RX_BUFFER);
		Choose_Mche= SCI_ReceData[0];
		Ctrl_Flag.bit.STOP_VELO_FLAG = 1;

		if(SCIB.FLAG.bit.TXBO_FLAG ==1)
		{
			Sci_DataPackage_Tx(SCI_SendData,0x007A,0);
			SCIB.FLAG.bit.TXBO_FLAG = 0;
	   		//ScicRegs.SCITXBUF = *SCIB.p_Tx_Buffer++;
			//ScicRegs.SCIFFTX.bit.TXFFIENA = 1;//Ê¹ÄÜ·¢ËÍÖÐ¶Ï
		}
		break;
   	/*--------------------------Ð´¿ØÖÆ²ÎÊýÖ¸Áî--------------------------*/
   	case 0x000B:
   		Sci_DataCombine_Rx(SCI_RX_BUFFER);
   	   		pi_velo.Kp_reg 		= SCI_ReceData[0];
   	   		pi_velo.Ki_reg 		= SCI_ReceData[3];
   			pi_velo.ui_out_max		= SCI_ReceData[6];
   			pi_velo.pi_out_max		= SCI_ReceData[6];
   			pi_velo.ui_out_min		= -SCI_ReceData[6];
   			pi_velo.pi_out_min		= -SCI_ReceData[6];
   	   		pi_velo.i_period		= SCI_ReceData[8];
   	   		pi_velo.ctrl_period		= SCI_ReceData[9];

   	   		pi_usLimit.Kp_reg 		= SCI_ReceData[1];
   	   		pi_usLimit.Ki_reg		= SCI_ReceData[2];
   			pi_usLimit.ui_out_max		= 0;
   			pi_usLimit.pi_out_max		= 0;
   			pi_usLimit.ui_out_min		= -SCI_ReceData[7];
   			pi_usLimit.pi_out_min		= -SCI_ReceData[7];
   	   		pi_usLimit.i_period 		= SCI_ReceData[5];
   			pi_usLimit.ctrl_period		= SCI_ReceData[4];

			pi_Pos.Kp_reg 		= SCI_ReceData[10];
   			pi_Pos.Ki_reg 		= SCI_ReceData[13];

   	   		pi_Pos.ui_out_max		= SCI_ReceData[16];
   			pi_Pos.pi_out_max		= SCI_ReceData[16];
   			pi_Pos.ui_out_min		= -SCI_ReceData[16];
   			pi_Pos.pi_out_min		= -SCI_ReceData[16];
   	   		pi_Pos.i_period 		= SCI_ReceData[18];
   			pi_Pos.ctrl_period		= SCI_ReceData[19];

   			/*pi_Id.Kp_reg 		= SCI_ReceData[10];
   			pi_Id.Ki_reg 		= SCI_ReceData[13];

   	   		pi_Id.ui_out_max		= SCI_ReceData[16];
   			pi_Id.pi_out_max		= SCI_ReceData[16];
   			pi_Id.ui_out_min		= -SCI_ReceData[16];
   			pi_Id.pi_out_min		= -SCI_ReceData[16];
   	   		pi_Id.i_period 			= SCI_ReceData[18];
   			pi_Id.ctrl_period		= SCI_ReceData[19];

   			pi_Iq.Kp_reg 		= SCI_ReceData[10];
   			pi_Iq.Ki_reg 		= SCI_ReceData[13];

   	   		pi_Iq.ui_out_max		= SCI_ReceData[16];
   			pi_Iq.pi_out_max		= SCI_ReceData[16];
   			pi_Iq.ui_out_min		= -SCI_ReceData[16];
   			pi_Iq.pi_out_min		= -SCI_ReceData[16];
   	   		pi_Iq.i_period 			= SCI_ReceData[18];
   			pi_Iq.ctrl_period		= SCI_ReceData[19];*/

   			Idq_Kc = SCI_ReceData[11];
   			Radq = SCI_ReceData[12];
   			pi_Id_Kc.i_period 		= SCI_ReceData[15];
   			pi_Id_Kc.ctrl_period	= SCI_ReceData[14];
   			pi_Id_Kc.Kp_reg 	= _IQmpy(Idq_Kc,Ld);

			pi_Id_Kc.Ki_reg		= _IQmpy(V_Base,Per_Ctrl);
			pi_Id_Kc.Ki_reg		= _IQmpy(pi_Id_Kc.Ki_reg,(pi_Id_Kc.i_period<<20));
			pi_Id_Kc.Ki_reg		= _IQmpy(Idq_Kc,pi_Id_Kc.Ki_reg);

   			pi_Id_Kc.ui_out_max		= SCI_ReceData[17];
   			pi_Id_Kc.pi_out_max		= SCI_ReceData[17];
   			pi_Id_Kc.ui_out_min		= -SCI_ReceData[17];
   			pi_Id_Kc.pi_out_min		= -SCI_ReceData[17];

			pi_Iq_Kc.i_period 		= pi_Id_Kc.i_period;
			pi_Iq_Kc.ctrl_period	= pi_Id_Kc.ctrl_period;
			pi_Iq_Kc.Kp_reg 	= _IQmpy(Idq_Kc,Lq);
			pi_Iq_Kc.Ki_reg		= pi_Id_Kc.Ki_reg;
			pi_Iq_Kc.ui_out_max		= pi_Id_Kc.ui_out_max;
			pi_Iq_Kc.pi_out_max		= pi_Id_Kc.pi_out_max;
			pi_Iq_Kc.ui_out_min		= pi_Id_Kc.ui_out_min;
			pi_Iq_Kc.pi_out_min		= pi_Id_Kc.pi_out_min;
		if(SCIB.FLAG.bit.TXBO_FLAG ==1)
		{
			Sci_DataPackage_Tx(SCI_SendData,0x007B,0);
			SCIB.FLAG.bit.TXBO_FLAG = 0;
	   		//ScicRegs.SCITXBUF = *SCIB.p_Tx_Buffer++;
			//ScicRegs.SCIFFTX.bit.TXFFIENA = 1;//Ê¹ÄÜ·¢ËÍÖÐ¶Ï
		}
		break;
   	/*--------------------------¶Á¿ØÖÆ²ÎÊýÖ¸Áî--------------------------*/
   	case 0x000C:
   	  	 		SCI_SendData[0] 	= 	pi_velo.Kp_reg;
   	   		 	SCI_SendData[3] 	= 	pi_velo.Ki_reg;
   				SCI_SendData[6] 	= 	pi_velo.pi_out_max;
   				SCI_SendData[8] 	= 	pi_velo.i_period;
   	   		 	SCI_SendData[9] 	= 	pi_velo.ctrl_period;

   	  	 		SCI_SendData[1] 	= 	pi_usLimit.Kp_reg;
   	   		 	SCI_SendData[2] 	= 	pi_usLimit.Ki_reg;
   				SCI_SendData[7] 	= 	-pi_usLimit.pi_out_min;
   				SCI_SendData[5] 	= 	pi_usLimit.i_period;
   	   		 	SCI_SendData[4] 	= 	pi_usLimit.ctrl_period;

   	   		 	SCI_SendData[10]	= 	pi_Pos.Kp_reg;
   	   		 	SCI_SendData[13] 	= 	pi_Pos.Ki_reg;
   		   		SCI_SendData[16] 	= 	pi_Pos.pi_out_max;
   	   			SCI_SendData[18] 	= 	pi_Pos.i_period ;
   			 	SCI_SendData[19] 	= 	pi_Pos.ctrl_period;

   	  	 		SCI_SendData[11] 	= 	Idq_Kc;
   	   		 	SCI_SendData[12] 	= 	Radq;
   				SCI_SendData[17] 	= 	pi_Id_Kc.pi_out_max;
   				SCI_SendData[15] 	= 	pi_Id_Kc.i_period;
   	   		 	SCI_SendData[14] 	= 	pi_Id_Kc.ctrl_period;

   				//SCI_SendData[21] 	= 	HarmCtrl_Kp;
   	   			//SCI_SendData[22] 	= 	HarmCtrl_Ki;
   		   		//SCI_SendData[27] 	= 	pi_Id_5.pi_out_max;
   	   			//SCI_SendData[25] 	=	pi_Id_5.i_period;
   	   			//SCI_SendData[24] 	=	pi_Id_5.ctrl_period;
  		if(SCIB.FLAG.bit.TXBO_FLAG ==1)
  		{
  			Sci_DataPackage_Tx(SCI_SendData,0x007C,30);
	   		SCIB.FLAG.bit.TXBO_FLAG = 0;
	   		//ScicRegs.SCITXBUF = *SCIB.p_Tx_Buffer++;
			//ScicRegs.SCIFFTX.bit.TXFFIENA = 1;//Ê¹ÄÜ·¢ËÍÖÐ¶Ï
	   	}
  		break;
	/*--------------------------Ð´Ä£ÄâÍ¨µÀ²ÎÊýÏµÍ³Ö¸Áî--------------------------*/
   	case 0x0010:
   		Sci_DataCombine_Rx(SCI_RX_BUFFER);

   	    		K_Udc 	= SCI_ReceData[0];
   	    		Off_Udc = SCI_ReceData[1];
   	    		K_Isa 	= SCI_ReceData[2];
   	    		Off_Isa = SCI_ReceData[3];
   	    		K_Isc 	= SCI_ReceData[4];
   	    		Off_Isc = SCI_ReceData[5];
				//K_IsbB 	= SCI_ReceData[6];
				//Off_IsbB = SCI_ReceData[7];
				K_Idc 	= SCI_ReceData[8];
				Off_Idc = SCI_ReceData[9];
				//K_IscB 	= SCI_ReceData[10];
				//Off_IscB = SCI_ReceData[11];
				PWMA_a = SCI_ReceData[12];
				PWMA_b = SCI_ReceData[13];
				PWMA_c = SCI_ReceData[14];
				//PWMB_a = SCI_ReceData[15];
				//PWMB_b = SCI_ReceData[16];
				//PWMB_c = SCI_ReceData[17];
				//Sys_Flag.bit.HARM_Flag = SCI_ReceData[18];
				//Freq_Min_G = SCI_ReceData[19];
				//Freq_Min_K = SCI_ReceData[20];
   	 		    Us_M = SCI_ReceData[22];
   	 		    K_Isb 	= SCI_ReceData[23];
   	 		    Off_Isb = SCI_ReceData[24];
   	 			//K_IsaB 	= SCI_ReceData[25];
   	 			//Off_IsaB = SCI_ReceData[26];
		if(SCIB.FLAG.bit.TXBO_FLAG ==1)
		{
			Sci_DataPackage_Tx(SCI_SendData,0x0080,0);
			SCIB.FLAG.bit.TXBO_FLAG = 0;
	   		//ScicRegs.SCITXBUF = *SCIB.p_Tx_Buffer++;
			//ScicRegs.SCIFFTX.bit.TXFFIENA = 1;//Ê¹ÄÜ·¢ËÍÖÐ¶Ï
		}
		break;
   	/*--------------------------¶ÁÄ£ÄâÍ¨µÀ²ÎÊýÏµÍ³Ö¸Áî--------------------------*/
   	case 0x0011:
   	   	 		SCI_SendData[0] = K_Udc;
   	    		SCI_SendData[1] = Off_Udc;
   	   	 		SCI_SendData[2] = K_Isa;
   	    		SCI_SendData[3] = Off_Isa;
   	   	 		SCI_SendData[4] = K_Isc;
   	    		SCI_SendData[5] = Off_Isc;
   	   	 		//SCI_SendData[6] = K_IsbB;
   	    		//SCI_SendData[7] = Off_IsbB;
   	 			SCI_SendData[8] = K_Idc;
   	    		SCI_SendData[9] = Off_Idc;
		   	    //SCI_SendData[10] = K_IscB;
		   	    //SCI_SendData[11] = Off_IscB;
				SCI_SendData[12] = PWMA_a;
				SCI_SendData[13] = PWMA_b;
				SCI_SendData[14] = PWMA_c;
				//SCI_SendData[15] = PWMB_a;
				//SCI_SendData[16] = PWMB_b;
				//SCI_SendData[17] = PWMB_c;
				//SCI_SendData[18] = Sys_Flag.bit.HARM_Flag;
				//SCI_SendData[19] = Freq_Min_G;
				//SCI_SendData[20] = Freq_Min_K;
   	 			SCI_SendData[22] = Us_M;
   	   	 		SCI_SendData[23] = K_Isb;
   	    		SCI_SendData[24] = Off_Isb;
   	   	 		//SCI_SendData[25] = K_IsaB;
   	    		//SCI_SendData[26] = Off_IsaB;
   		if(SCIB.FLAG.bit.TXBO_FLAG ==1)
   		{
   			Sci_DataPackage_Tx(SCI_SendData,0x0081,27);
	   		SCIB.FLAG.bit.TXBO_FLAG = 0;
	   		//ScicRegs.SCITXBUF = *SCIB.p_Tx_Buffer++;
			//ScicRegs.SCIFFTX.bit.TXFFIENA = 1;//Ê¹ÄÜ·¢ËÍÖÐ¶Ï
	   	}
   		break;
   	/*--------------------------Ð´ÆäËû²ÎÊý--------------------------*/
   	case 0x0012:
   		Sci_DataCombine_Rx(SCI_RX_BUFFER);

   	    	Temperature1_K = SCI_ReceData[0];
   	 		UdcLimit1_Set = SCI_ReceData[1];
   	 		//TemperatureB1_K = SCI_ReceData[2];
   	 		//Harm_LdivR = SCI_ReceData[3];
   	 		ProtectNum = SCI_ReceData[4];
   	 		UsSet = SCI_ReceData[5];
   	 		//Ctrl_Flag.bit.CanCtrl_Flag = SCI_ReceData[6];
   	 		TorqueAngle = SCI_ReceData[7];
			//TemperatureB5_K = SCI_ReceData[8];
			//TemperatureB5_Off = SCI_ReceData[9];
   	 		Temperature3_K = SCI_ReceData[10];
   	 		//TemperatureB3_K = SCI_ReceData[11];
   	 		Ctrl_Flag.bit.RotorWay_Flag = SCI_ReceData[12];
			//Sys_Flag.bit.RotorWay_Flag = SCI_ReceData[13];
   	 		UdcLimit_Set = SCI_ReceData[14];
   	 		Velo_Start = SCI_ReceData[15];	// Æô¶¯ËÙ¶È
   	 		Vel_Speed = SCI_ReceData[16];
   	 		Ctrl_Flag.bit.VelWay_Flag = SCI_ReceData[17];
   	  		TorquetoIs = SCI_ReceData[18];	// ×ª¾ØµçÁ÷±È
   	 		IdProtectNum = SCI_ReceData[19];          //????
   	 		VeloProtectNum = SCI_ReceData[20];      //???
   	 		Velo_Calc_Num = SCI_ReceData[21];
   	  		Temperature1_Off = SCI_ReceData[22];
   	  		//TemperatureB1_Off = SCI_ReceData[23];
   	  		Temperature3_Off = SCI_ReceData[24];
   	  		//TemperatureB3_Off = SCI_ReceData[25];
			//CtrlB_Flag.bit.VelWay_Flag = SCI_ReceData[26];
   	 		Ctrl_Flag.bit.UsCtrl_Flag = SCI_ReceData[27];
   	 		Temperature5_K = SCI_ReceData[28];
   	 		Temperature5_Off = SCI_ReceData[29];
   	  		IstoTorque = _IQdiv(_IQ(1),TorquetoIs);   //????
   	  	    K_Velo_Cal = _IQdiv(_IQ(62.83185307179586476925286766559), V_Base);//?????
   	   		K_Velo_Cal = _IQmpy(K_Velo_Cal,Velo_Duty0);   //¶¨Ê±Æ÷ÖÜÆÚÎª100us
   			K_Velo_Cal = _IQdiv(K_Velo_Cal,Velo_Calc_Num<<20);
   	   		K_Velo_Cal = _IQmpy(K_Velo_Cal,p);
   	   	if(SCIB.FLAG.bit.TXBO_FLAG ==1)
		{
			Sci_DataPackage_Tx(SCI_SendData,0x0082,0);
			SCIB.FLAG.bit.TXBO_FLAG = 0;
	   		//ScicRegs.SCITXBUF = *SCIB.p_Tx_Buffer++;
			//ScicRegs.SCIFFTX.bit.TXFFIENA = 1;//Ê¹ÄÜ·¢ËÍÖÐ¶Ï
	   	}
   		break;
   	/*--------------------------¶ÁÆäËû²ÎÊý--------------------------*/
   	case 0x0013:
		SCI_SendData[0] = Temperature1_K;
   			SCI_SendData[1] = UdcLimit1_Set;
   			//SCI_SendData[2] = TemperatureB1_K;
   			//SCI_SendData[3] = Harm_LdivR;
   			SCI_SendData[4] = ProtectNum;
   			SCI_SendData[5] = UsSet;
   			//SCI_SendData[6] = Ctrl_Flag.bit.CanCtrl_Flag;
   			SCI_SendData[7] = TorqueAngle;
   			//SCI_SendData[8] = TemperatureB5_K;
   			//SCI_SendData[9] = TemperatureB5_Off;
   	   		SCI_SendData[10] = Temperature3_K;
   	   		//SCI_SendData[11] = TemperatureB3_K;
   			SCI_SendData[12] = Ctrl_Flag.bit.RotorWay_Flag;
   			//SCI_SendData[13] = Sys_Flag.bit.RotorWay_Flag;
   			SCI_SendData[14] = UdcLimit_Set;
   			SCI_SendData[15] = Velo_Start;	// Æô¶¯ËÙ¶È
   			SCI_SendData[16] = Vel_Speed;
   			SCI_SendData[17] = Ctrl_Flag.bit.VelWay_Flag;
   	 		SCI_SendData[18] = TorquetoIs;	// ×ª¾ØµçÁ÷±È
   			SCI_SendData[19] = IdProtectNum;
   			SCI_SendData[20] = VeloProtectNum;
   			SCI_SendData[21] = Velo_Calc_Num;
   			SCI_SendData[22] = Temperature1_Off;
   			//SCI_SendData[23] = TemperatureB1_Off;
   			SCI_SendData[24] = Temperature3_Off;
   			//SCI_SendData[25] = TemperatureB3_Off;
   			//SCI_SendData[26] = CtrlB_Flag.bit.VelWay_Flag;
   			SCI_SendData[27] = Ctrl_Flag.bit.UsCtrl_Flag;
   			SCI_SendData[28] = Temperature5_K;
   			SCI_SendData[29] = Temperature5_Off;


		if(SCIB.FLAG.bit.TXBO_FLAG ==1)
		{
			Sci_DataPackage_Tx(SCI_SendData,0x0083,30);
			SCIB.FLAG.bit.TXBO_FLAG = 0;
	   		//ScicRegs.SCITXBUF = *SCIB.p_Tx_Buffer++;
			//ScicRegs.SCIFFTX.bit.TXFFIENA = 1;//Ê¹ÄÜ·¢ËÍÖÐ¶Ï
		}
		break;
   	//---------------------------------------------------------------------------
	case 0x0060:            //Ð´ÈëEEPROMÖ¸Áî

		WriteEEProm_Flag = 1;
		if(SCIB.FLAG.bit.TXBO_FLAG ==1)
		{
			Sci_DataPackage_Tx(SCI_SendData,0x00D0,0);
			SCIB.FLAG.bit.TXBO_FLAG = 0;
	   		//ScicRegs.SCITXBUF = *SCIB.p_Tx_Buffer++;
			//ScicRegs.SCIFFTX.bit.TXFFIENA = 1;//Ê¹ÄÜ·¢ËÍÖÐ¶Ï
		}

		break;

   default :
   		break;


   	}

}

void WriteEEProm(void)
{
	volatile	Uint16	i=0;
	Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
	Forbid_PWMA();

		DINT;   
		RDC_DIS_B;
		RDC_DIS_A;
		DIS_WP = 1;
        SCI_SendData[0] = Rs;
        SCI_SendData[1] = Ld;
        SCI_SendData[2] = Lq;
        SCI_SendData[3] = PHI;
        SCI_SendData[4] = Mp;
        SCI_SendData[5] = Rp;
        SCI_SendData[6]	= Udc_Setg;
        SCI_SendData[7] = U_Base;
        SCI_SendData[8] = I_Base;
        SCI_SendData[9] = V_Base;			// µçÐµËÙ¶È»ùÖµ(rad/s)
        SCI_SendData[10] = Angle_Init_Digital;
        SCI_SendData[11] = Velo_Max;
        SCI_SendData[12] = id_Max;
        SCI_SendData[13] = Is_Add;
        SCI_SendData[14] = Control_Period;
        SCI_SendData[15] = id_Filter;
        SCI_SendData[16] = Udc_Filter;
     	SCI_SendData[17] = Velo_Filter;
     	SCI_SendData[18] = Deadtime;
        SCI_SendData[19] = Velo_Add;
        SCI_SendData[20] = Is_DownAdd;
        SCI_SendData[21] = Temp_Filter;
        SCI_SendData[22] = Angle_Init_Fa;
        SCI_SendData[23] = Com_angle_U;
        SCI_SendData[24] = Com_angle_I;
        SCI_SendData[25] = idSet_Max;
        //SCI_SendData[26] = Freq_cut;
        SCI_SendData[27] = IsLimit_Set;
        SCI_SendData[28] = MotorTM;
        SCI_SendData[29] = IgbtTM;

		EEPROM_DataPackage(SCI_SendData,30);
		for(i = 0;i < 127;i++)
			WRITE_X25040(&EEPROM_BUFFER[0+i],128+i,1,0);


		SCI_SendData[0] 	= 	pi_velo.Kp_reg;
		SCI_SendData[3] 	= 	pi_velo.Ki_reg;
		SCI_SendData[6] 	= 	pi_velo.pi_out_max;
		SCI_SendData[8] 	= 	pi_velo.i_period;
		SCI_SendData[9] 	= 	pi_velo.ctrl_period;

		SCI_SendData[1] 	= 	pi_usLimit.Kp_reg;
		SCI_SendData[2] 	= 	pi_usLimit.Ki_reg;
		SCI_SendData[7] 	= 	-pi_usLimit.pi_out_min;
		SCI_SendData[5] 	= 	pi_usLimit.i_period;
		SCI_SendData[4] 	= 	pi_usLimit.ctrl_period;

		SCI_SendData[10]	= 	pi_Pos.Kp_reg;
		SCI_SendData[13] 	= 	pi_Pos.Ki_reg;
		SCI_SendData[16] 	= 	pi_Pos.pi_out_max;
		SCI_SendData[18] 	= 	pi_Pos.i_period ;
		SCI_SendData[19] 	= 	pi_Pos.ctrl_period;

 		SCI_SendData[11] 	= 	Idq_Kc;
	 	SCI_SendData[12] 	= 	Radq;
		SCI_SendData[17] 	= 	pi_Id_Kc.pi_out_max;
		SCI_SendData[15] 	= 	pi_Id_Kc.i_period;
	 	SCI_SendData[14] 	= 	pi_Id_Kc.ctrl_period;

		//SCI_SendData[21] 	= 	HarmCtrl_Kp;
		//SCI_SendData[22] 	= 	HarmCtrl_Ki;
		//SCI_SendData[27] 	= 	pi_Id_5.pi_out_max;
		//SCI_SendData[25] 	=	pi_Id_5.i_period;
		//SCI_SendData[24] 	=	pi_Id_5.ctrl_period;

		EEPROM_DataPackage(SCI_SendData,30);
		for(i = 0;i < 127;i++)
			WRITE_X25040(&EEPROM_BUFFER[0+i],0+i,1,0);


		SCI_SendData[0] = K_Udc;
     	SCI_SendData[1] = Off_Udc;
     	SCI_SendData[2] = K_Isa;
     	SCI_SendData[3] = Off_Isa;
     	SCI_SendData[4] = K_Isc;
     	SCI_SendData[5] = Off_Isc;
 		//SCI_SendData[6] = K_IsbB;
		//SCI_SendData[7] = Off_IsbB;
		SCI_SendData[8] = K_Idc;
		SCI_SendData[9] = Off_Idc;
     	//SCI_SendData[10] = K_IscB;
     	//SCI_SendData[11] = Off_IscB;
		SCI_SendData[12] = PWMA_a;
		SCI_SendData[13] = PWMA_b;
		SCI_SendData[14] = PWMA_c;
		//SCI_SendData[15] = PWMB_a;
		//SCI_SendData[16] = PWMB_b;
		//SCI_SendData[17] = PWMB_c;
		//SCI_SendData[18] = Sys_Flag.bit.HARM_Flag;
		//SCI_SendData[19] = Freq_Min_G;
		//SCI_SendData[20] = Freq_Min_K;
     	SCI_SendData[22] = Us_M;
     	SCI_SendData[23] = K_Isb;
     	SCI_SendData[24] = Off_Isb;
     	//SCI_SendData[25] = K_IsaB;
     	//SCI_SendData[26] = Off_IsaB;

		EEPROM_DataPackage(SCI_SendData,27);
		for(i = 0;i < 127;i++)
			WRITE_X25040(&EEPROM_BUFFER[0+i],0+i,1,8);

		SCI_SendData[0] = Temperature1_K;
 		SCI_SendData[1] = UdcLimit1_Set;
 		//SCI_SendData[2] = TemperatureB1_K;
 		//SCI_SendData[3] = Harm_LdivR;
 		SCI_SendData[4] = ProtectNum;
 		SCI_SendData[5] = UsSet;
 		//SCI_SendData[6] = Ctrl_Flag.bit.CanCtrl_Flag;
 		SCI_SendData[7] = TorqueAngle;
		//SCI_SendData[8] = TemperatureB5_K;
		//SCI_SendData[9] = TemperatureB5_Off;
 		SCI_SendData[10] = Temperature3_K;
 		//SCI_SendData[11] = TemperatureB3_K;
 		SCI_SendData[12] = Ctrl_Flag.bit.RotorWay_Flag;
		//SCI_SendData[13] = Sys_Flag.bit.RotorWay_Flag;
 		SCI_SendData[14] = UdcLimit_Set;
 		SCI_SendData[15] = Velo_Start;	// Æô¶¯ËÙ¶È
 		SCI_SendData[16] = Vel_Speed;
 		SCI_SendData[17] = Ctrl_Flag.bit.VelWay_Flag;
 		SCI_SendData[18] = TorquetoIs;	// ×ª¾ØµçÁ÷±È
 		SCI_SendData[19] = IdProtectNum;
 		SCI_SendData[20] = VeloProtectNum;
 		SCI_SendData[21] = Velo_Calc_Num;
 		SCI_SendData[22] = Temperature1_Off;
 		//SCI_SendData[23] = TemperatureB1_Off;
 		SCI_SendData[24] = Temperature3_Off;
 		//SCI_SendData[25] = TemperatureB3_Off;
		//SCI_SendData[26] = CtrlB_Flag.bit.VelWay_Flag;
 		SCI_SendData[27] = Ctrl_Flag.bit.UsCtrl_Flag;
 		SCI_SendData[28] = Temperature5_K;
 		SCI_SendData[29] = Temperature5_Off;

		EEPROM_DataPackage(SCI_SendData,30);
		for(i = 0;i < 127;i++)
			WRITE_X25040(&EEPROM_BUFFER[0+i],128+i,1,8);
		EN_WP = 1;
	
  		EINT;

	WriteEEProm_Flag = 0;
}
	
/*===========================================================================
 No more.
===========================================================================*/
