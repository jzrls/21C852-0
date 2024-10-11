/**
* @projectName   :XXXXXXXXXXXXXXXXXXXX
* @title         :Mk_CRC.c
* @author        :Zhangtaipeng
* @date          :2023年7月13日 @ 上午9:55:35
* @brief         :CRC Module
*/

//--------------------------------------------------
// header file
#include "F28335_Device.h"     // F28335 外设头文件
#include "App_UserDef.h"      // F28335 应用项目头文件
//--------------------------------------------------

//--------------------------------------------------
// Global variable definition

//--------------------------------------------------

//--------------------------------------------------
//Static variable definition//--------------------------------------------------

//--------------------------------------------------
//static Function
//--------------------------------------------------

	/*_iq q = 0;
	float32 f = _IQtoF(_IQ(12.66));//5C 8F 4A 41
	float64 d = 12.66;//52 B8 1E 85 EB 51 29 40
	unsigned int ft[2] = {0};
	unsigned int ft1[2] = {0};
	unsigned int dt[4] = {0};
	float32 f2 = 0;
	float64 d2 = 0;
	ft1[0] = 0;
	ft1[1] = Float_Hex(_IQ(12.66)>>5);
	*(float32*)ft = f;
	f2 = *(float32*)ft1;

	*(float64*)dt = d;
	d2 = *(float64*)dt;
	q =   Hex_Double(0xEB,0x51,0x29,0x40);*/

_iq  Hex_Double(Uint16 ft1,Uint16 ft2,Uint16 ft3,Uint16 ft4)
{
	Uint16 ft[4];
    float64 f = 0;
    float32 fout = 0;
	_iq q = 0;
	ft[0] = 0;
	ft[1] = 0;
	ft[2] = (ft2 << 8) + ft1;
	ft[3] = (ft4 << 8) + ft3;
	f = *(float64*)ft;
	fout = f;
	q = (long)(fout * 32768.0);
	//q = _IQ15(fout);
    return q;
}

_iq  Hex_Float(Uint16 ft1,Uint16 ft2,Uint16 ft3,Uint16 ft4)
{
	Uint16 ft[2];
    float32 fout = 0;
	_iq q = 0;
	ft[0] = (ft2 << 8) + ft1;
	ft[1] = (ft4 << 8) + ft3;
	fout = *(float32*)ft;
	q = (long)(fout * 32768.0);
    return q;
}


Uint16  Float_Hex(_iq f_in)
{
	Uint16 ft[2];
    float32 f = 0;
 	f = _IQ15toF(f_in);
	*(float32*)ft = f;

    return ft[1];
}


//--------------------------------------------------
//Function instantiation
//--------------------------------------------------
/*
 * @Brief:  CRC funcation
 * @Name:   CalCRC16_Byte
 * @Param:  pbDataBuf[In] , byte data  point
 * @Param:  dwNumOfBytes[In] , byte data  cnt
 * @Return: CRC  rslt
 * @Record: 2023-7-13 Ztp  creat this file
 */
Uint16  CalCRC16_Byte(Uint16 *pbDataBuf, Uint32 dwNumOfBytes)
{
    Uint16      bData   = 0;
    Uint32      dwLen   = dwNumOfBytes;
    Uint16      wCRC    = 0x0000;

    while(0 != (dwLen--))
    {
        bData   = (wCRC >> 8) & 0x00FF;
        wCRC  <<= 8;

        wCRC    = wCRC ^ g_pusCrc16[bData ^ ((*pbDataBuf) & 0x00FF)];

        pbDataBuf++;
    }

    return wCRC;

}/* End of CalCRC16_Byte() */

