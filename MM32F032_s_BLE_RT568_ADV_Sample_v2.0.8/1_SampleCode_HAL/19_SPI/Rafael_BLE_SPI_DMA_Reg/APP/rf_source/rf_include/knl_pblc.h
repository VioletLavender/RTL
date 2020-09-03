/*******************************************************************
 *
 * File Name  : KNL_PBLC.H
 * Description: This file provide the kernel declerations that all
 *              layers needed.
 *
 * $Log:  $
 *
 *
 *
 *******************************************************************
 *
 *      Copyright (c) 2020, All Right Reserved
 *      Rafael Microelectronics Co. Ltd.
 *      Taiwan, R.O.C.
 *
 *******************************************************************/
/*! \file knl_pblc.h
    \brief  Provide the kernel declerations that all
            layers needed.
*/

#ifndef _KNL_PBLC_H_
#define _KNL_PBLC_H_

#include "config.h"
#include "ble_basicType.h"

///*--------------------------------------*/
///* Basic constant definition            */
///*--------------------------------------*/
//#define SUCCESS         0
//#define FAIL            (!SUCCESS)

//#define ACCEPT          0
//#define REJECT          (!ACCEPT)

//#define RESET           0
//#define SET             (!RESET)

//#define OFF             0
//#define ON              (!OFF)

//#define NORMAL          0
//#define ABNORMAL        (!NORMAL)

//#define NO              0
//#define YES             (!NO)

//#define LOW             0
//#define HIGH            (!LOW)

//#define BIT_SET         1
//#define BIT_CLEAR       0

//#ifdef _USED_EXCEPT_SDK_
/*--------------------------------------*/
/* Block number definition              */
/*--------------------------------------*/
#define MAX_MBLK_RSV_LL_ROLE    5
#define MAX_MBLK_RSV_LL     (MAX_MBLK_RSV_LL_ROLE*5+5)  /* reserved message block number, should be same as MAX_MBLK_RSV_LL_ROLE*MAX_NUM_CONN_HDL+5 */
#define MAX_MBLK_RSV_L2     MAX_MBLK_RSV_LL
#define MAX_MBLK_RSV_L1     (MAX_MBLK_RSV_LL_ROLE+5)
#define MAX_MBLK_NO         (MAX_MBLK_RSV_LL+6*5)          /* maximum message block number */

#define LEN_BD_ACCESS_ADDR      4
#define LEN_INIT_LL_DATA        22
#define LEN_CONN_DATA_MAX       27
#define LEN_CONN_MIC            0

#define LEN_CONN_DATA_DEFAULT       27
#define LEN_CONN_PKT_EXCEPT_DATA    10
#define LEN_CONN_MIC_REF            4
#define LEN_CONN_PKT_EXCEPT_DATA_W_CCM  (LEN_CONN_PKT_EXCEPT_DATA+LEN_CONN_MIC_REF)
#define LEN_LL_CH_MAP           5
#define LEN_LL_INSTANT          2
//#endif  //(#ifdef _USED_EXCEPT_SDK_)

#define LEN_BD_ADDR             6
#define LEN_ADV_SCAN_DATA_MAX   31
//#define MAX_MBLK_SIZE           (LEN_BD_ADDR+LEN_ADV_SCAN_DATA_MAX+9)      //9: rest of MHC_Le_Adv_Report_Para
#define MAX_MBLK_SIZE           (LEN_BD_ADDR+LEN_ADV_SCAN_DATA_MAX+9+2)      //9: rest of MHC_Le_Adv_Report_Para, 2:padding for 32bit MCU

/*******************************************************************
 *              TYPE DEFINITION
 *******************************************************************/
///*--------------------------------------*/
///* Basic type definition                */
///*--------------------------------------*/
//typedef unsigned char           Uint8;
//typedef unsigned short int      Uint16;
//typedef unsigned int            Uint32;
//typedef signed char             Sint8;
//typedef signed short int        Sint16;
//typedef signed int              Sint32;

//#ifdef _USED_EXCEPT_SDK_

typedef struct TBLK_LL
{
    Uint8   Next;       /* next TBLK in list */
    Uint16  Ticks;      /* timeout duration (ticks no.) */
    Uint8   TmrId;      /* Timer Id */
    Uint8   ConnId;      /* Timer Id */
} TBLK_LL;
typedef TBLK_LL TBLK_LLx;
//typedef TBLK_LL xdata TBLK_LLx;
//#endif  //(#ifdef _USED_EXCEPT_SDK_)

#ifdef _HCI_HW_
#define HCLL_HCI_HW_OPCODE      Uint8 HCI_Pckt_Typ; Uint8 HCI_Ocf; Uint8 HCI_Ogf; Uint8 HCI_Cmd_Length;
#define SIZE_HCLL_HCI_HW_OPCODE     4
#define HCI_CONN_HDL            Uint8 HCI_Conn_Hdl_L; Uint8 HCI_Conn_Hdl_H;
#define SIZE_HCI_CONN_HDL           2
#define MHC_HCI_HW_OPCODE       Uint8 HCI_Ocf; Uint8 HCI_Ogf;
#define SIZE_MHC_HCI_HW_OPCODE      2
#define MHC_HCI_HW_PKT_TYPE     Uint8 HCI_Pckt_Typ; Uint8 HCI_Event_Code;
#define SIZE_MHC_HCI_HW_PKT_TYPE    2
#define HCI_DATA_CONN_HDL       Uint8 HCI_Pckt_Typ; Uint8 HCI_Conn_Hdl_L; Uint8 HCI_Conn_Hdl_H;
#define SIZE_HCI_DATA_CONN_HDL      3
#define HCI_DATA_PKT_LTH_BYTE   Uint8 HCI_DataPkt_LthL; Uint8 HCI_DataPkt_LthH;
#define SIZE_HCI_DATA_PKT_LTH_BYTE  2
#else
#define HCLL_HCI_HW_OPCODE
#define SIZE_HCLL_HCI_HW_OPCODE     0
#define HCI_CONN_HDL            Uint8 HCI_Conn_Hdl_L;
#define SIZE_HCI_CONN_HDL           1
#define MHC_HCI_HW_OPCODE       Uint8 By_Primitive;
#define SIZE_MHC_HCI_HW_OPCODE      1
#define MHC_HCI_HW_PKT_TYPE
#define SIZE_MHC_HCI_HW_PKT_TYPE    0
#define HCI_DATA_CONN_HDL       Uint8 HCI_Conn_Hdl_L; Uint8 HCI_Conn_Hdl_H;    //HCI_Conn_Hdl_H: LL_LLid
#define SIZE_HCI_DATA_CONN_HDL      2
#define HCI_DATA_PKT_LTH_BYTE   Uint8 HCI_DataPkt_LthL;
#define SIZE_HCI_DATA_PKT_LTH_BYTE  1
#endif  //#ifdef _HCI_HW_

struct HCLL_LE_Set_Adv_Param_Para                        //OGF: LE Controller, 0x08
{
    HCLL_HCI_HW_OPCODE
    Uint16 HCI_Adv_Interval_Min;
    Uint16 HCI_Adv_Interval_Max;
    Uint8 HCI_Adv_Type;
    Uint8 HCI_Own_Addr_Type;
    Uint8 HCI_Direct_Addr_Type;
    Uint8 HCI_Direct_Addr[LEN_BD_ADDR];
    Uint8 HCI_Adv_Channel_Map;
    Uint8 HCI_Adv_Filter_Policy;
};
#define LEN_HCLL_LE_SET_ADV_PARAM_PARA       (2*2+1*5+1*LEN_BD_ADDR)



/* Message block */
typedef struct MBLK
{
    void  *Next;       /* point to next mblk */
    Uint32 Primitive;  /* primitive type */
    union
    {
        Uint8 Data[MAX_MBLK_SIZE];
    } Para;             /* parameters */
} MBLK;
//typedef MBLK xdata MBLK;

//#endif  //(#ifdef _USED_EXCEPT_SDK_)
extern void Knl_MemCpy(Uint8 *pDst, Uint8 *pSrc, Uint8 len);
extern void Knl_MemCpy_Fwd(Uint8 *pDst, Uint8 *pSrc, Uint8 len);
extern void Knl_MemCpyInv(Uint8 *pDst, Uint8 *pSrc, Uint8 len);
extern void Knl_CodeCpy(Uint8 *pDst, Uint8 const *pSrc, Uint8 len);
extern void Knl_CodeCpyInv(Uint8 *pDst, Uint8 const *pSrc, Uint8 len);
extern Uint8 Knl_MemComp(Uint8 *pDst, Uint8 *pSrc, Uint8 len);
extern Uint8 Knl_CodeComp(Uint8 *pDst, Uint8 const *pSrc, Uint8 len);
extern void Knl_MemCpy_Isr(Uint8 *pDst, Uint8 *pSrc, Uint8 len);
extern void Knl_MemCpyInv_Isr(Uint8 *pDst, Uint8 *pSrc, Uint8 len);
extern void Knl_CodeCpy_Isr(Uint8 *pDst, Uint8 const *pSrc, Uint8 len);
extern void Knl_CodeCpyInv_Isr(Uint8 *pDst, Uint8 const *pSrc, Uint8 len);
extern Uint8 Knl_MemComp_Isr(Uint8 *pDst, Uint8 *pSrc, Uint8 len);
extern Uint8 Knl_CodeComp_Isr(Uint8 *pDst, Uint8 const *pSrc, Uint8 len);

extern void ErrorEntry(Uint32 errID);
#endif
