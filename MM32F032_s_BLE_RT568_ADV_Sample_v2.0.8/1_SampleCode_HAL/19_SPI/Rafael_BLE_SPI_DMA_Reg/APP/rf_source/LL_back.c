/*******************************************************************
 *
 * File Name  : LL.C
 * Description:
 *
 *******************************************************************
 *
 *      Copyright (c) 2020, All Right Reserved
 *      Rafael Microelectronics Co. Ltd.
 *      Taiwan, R.O.C.
 *
 *******************************************************************/
#pragma push
//#pragma Otime
#pragma Ospace

/*******************************************************************
 *      Include List
 *******************************************************************/
#include "BleAppSetting.h"
#include "rf_phy.h"
#include "knl_pblc.h"
#include "LL.h"
#include "hci.h"
#include "bsp_spi_ble.h"
#ifdef _DEBUG_MSG_USER_
    #include <stdio.h>
#endif  //(_DEBUG_MSG_USER_)

#include "_rafael_phy.h"
BLE_Device_Param ble_device_param;

LL_Adv          LL_Adv_Para;
LL_Conn         LL_Conn_Para[MAX_NUM_CONN_HDL];
LL_WhiteList    LL_WhiteList_Para[SIZE_WHITE_LIST_ENTRIES];
LL_Para_Itrvl   LL_Para_Interval;
Uint8           LL_ConnID_Remaining;

Uint8 LL_Ref_ChMap[LEN_LL_CH_MAP];

TBLK_LLx TmrBlk_LL[MAX_TBLK_LL_NO];
Uint8 TBlk_Free_LL;
Uint8 TBlk_InUse_LL;
TBLK_LLx *tblk_LL_pi;
TBLK_LLx *tblk_LL_pi2;

Uint8 status_LL_Tmr;
Uint8 anchor_LL_Tmr;
LL_Conn *LL_conn_pi;
Uint16 LL_DurRxPktAccu;

Uint8 BD_Rand_Addr[LEN_BD_ADDR] = {0};
Uint32 LL_Slv_Win_Width;
Uint32 LL_Slv_Win_Width_Base;
uint32_t seedR16;
Uint8 RX_CRC_valid_flag = TRUE;

uint64_t Tmr37;


const Uint8 LL_CHMAP_DEFAUT[LEN_LL_CH_MAP] = {0xFF, 0xFF, 0xFF, 0xFF, 0x1F};

const Uint16 LL_LENGTH[] =
{
    32,
    ((32 + LEN_CONN_PKT_EXCEPT_DATA_W_CCM) * 8), //328
    32,
    ((32 + LEN_CONN_PKT_EXCEPT_DATA_W_CCM) * 8), //328
};
#define SIZE_LL_LENGTH_PREFER   (sizeof(LL_LENGTH)/sizeof(LL_LENGTH[0]))

Uint16 LL_Length_Prefer[MAX_NUM_CONN_HDL][SIZE_LL_LENGTH_PREFER];   //"/2":uint16_t -> uint8_t

Uint8 LL_REF_ACS_ADDR_ADVSCN[4] = {0xD6, 0xBE, 0x89, 0x8E}; //const Uint32 LL_REF_ACS_ADDR_ADVSCN = 0x8E89BED6;

Uint8 LL_REF_CRC_INI_ADVSCN[LEN_CRC_INIT] =
{
    //const
    0x55, 0x55, 0x55,
};

Uint8 LL_DUR_RSV[] =
{
    //const
    11 + DUR_LL_RSV_PRECAL, /* ADV_IND */
    8 + DUR_LL_RSV_PRECAL,  /* ADV_DIRECT_IND */
    4 + DUR_LL_RSV_PRECAL,
    DUR_LL_RSV_SCN_BASE,
    DUR_LL_RSV_SCN_BASE,
    DUR_LL_RSV_INIT_BASE,
    11 + DUR_LL_RSV_PRECAL,
};

uint8_t LL_RF_DATA_CH[] =                                                      //LL__001
{
    //const
    0,  1,  2,  3,  4,  5,  6,  7,
    8,  9, 10, 11, 12, 13, 14, 15,
    16, 17, 18, 19, 20, 21, 22, 23,
    24, 25, 26, 27, 28, 29, 30, 31,
    32, 33, 34, 35, 36,  0,  1,  2,
    3,  4,  5,  6,  7,  8,  9, 10,
    11, 12, 13, 14, 15, 16, 17, 18,
    19, 20, 21, 22, 23, 24, 25, 26,
    27, 28, 29, 30, 31, 32, 33, 34,
    35, 36,
};

uint8_t CH_ADV_SEL_TABLE[] =
{
    //const
    0,  37,  38, 39
};

Uint8 CH_ADV_CH_HOP_BY_MAP_TABLE[][4] =     //channel hop in one ADV interval. The number is CURRENT_ch_num and also NEXT_array_index
{
    //const
    {0, 0, 0, 0,},      //000
    {1, 0, 0, 0,},      //001  //array[0]:ch37 -> array[1]:stop -> array[0]:ch37 .....repeat
    {2, 0, 0, 0,},      //010  //array[0]:ch38 -> array[2]:stop -> array[0]:ch38 .....repeat
    {1, 2, 0, 0,},      //011  //array[0]:ch37 -> array[1]:ch38 -> array[2]:stop -> array[0] .....repeat
    {3, 0, 0, 0,},      //100  //array[0]:ch39 -> array[3]:stop -> array[0]:ch39 .....repeat
    {1, 3, 0, 0,},      //101  //array[0]:ch37 -> array[1]:ch39 -> array[3]:stop -> array[0] .....repeat
    {2, 0, 3, 0,},      //110  //array[0]:ch38 -> array[2]:ch39 -> array[3]:stop -> array[0] .....repeat
    {1, 2, 3, 0,},      //111  //array[0]:ch37 -> array[1]:ch38 -> array[2]:ch39 -> array[3]:stop -> array[0] .....repeat
};
Uint8 Ch_ADV_Ch_Hop_Table[4];

Uint8 TAB_ZERO_128[] =
{
    // const
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};  //Actually total 136 bytes.


Uint8 LL_Msg_AdvScnConn;
#define LL_MSG_ADVSCNCONN_ADV_EN                0x80
#define LL_MSG_ADVSCNCONN_SCN_EN                0x40
#define LL_MSG_ADVSCNCONN_INIT_EN               0x20
#define LL_MSG_ADVSCNCONN_ADV_CONT              0x10
#define LL_MSG_ADVSCNCONN_SCN_CONT              0x08
#define LL_MSG_ADVSCNCONN_INIT_CONT             0x04


#define MAX_INTVL_ADV_CONSTI                    80
#define MAX_INTVL_ADV_DIRECT_H                  10240

uint8_t RF_Msg_RF0INT;
extern void rafael_reset_phy_fsm_Isr(void);

/*******************************************************************
 *      Global Variable Defines
 *******************************************************************/


/*******************************************************************
 *      Function Prototype Decelaration
 *******************************************************************/

/*******************************************************************
 *      Program Code
 *******************************************************************/

void LL_ReleaseConnID(Uint8 LL_Conn_ID)
{
    LL_Conn *pLL_conn;
    extern Uint8 MBlk_depth_Remaining;
    extern Uint8 LL_ConnID_Remaining;

    pLL_conn = &LL_Conn_Para[LL_Conn_ID];

    Knl_CodeCpy(&pLL_conn->LE_Conn_Para.LL_Conn_ID, TAB_ZERO_128, LEN_LE_CONN_PARA);     //initialization
    InterruptDisable();
    pLL_conn->LE_Conn_Para.LL_Conn_ID = LL_CONN_ID_STBY;                                 //initialization
    pLL_conn->LE_Conn_Para.LL_Tx_PHYS = LL_TX_PHYS_1M_PHY;                               //initialization
    pLL_conn->LE_Conn_Para.LL_Rx_PHYS = LL_RX_PHYS_1M_PHY;                               //initialization
    pLL_conn->LE_Conn_Para.LL_Rx_length_1M = LEN_CONN_DATA_DEFAULT;                      //initialization
    pLL_conn->LE_Conn_Para.LL_Rx_length_2M = LEN_CONN_DATA_DEFAULT;                      //initialization
    pLL_conn->LE_Conn_Para.LL_Tx_length_1M = LEN_CONN_DATA_DEFAULT;                      //initialization
    pLL_conn->LE_Conn_Para.LL_Tx_length_2M = LEN_CONN_DATA_DEFAULT;                      //initialization

    InterruptEnable();

}

#pragma Otime

#pragma Ospace
void initLL(void)
{
    Uint8 i;

    for (i = 0; i < MAX_NUM_CONN_HDL; i++)
    {
        LL_ReleaseConnID(i);                                        //initialization
    }
    LL_ConnID_Remaining = MAX_NUM_CONN_HDL;                         //initialization
    LL_Adv_Para.Adv_Para.LL_AdvMap_ID = 0;                          //initialization
    LL_Adv_Para.Adv_Para.LL_AdvConn_ID = LL_CONN_ID_SLAVE_RSV;      //initialization
    LL_Adv_Para.Adv_Para.LL_ScanRsp_Data_Length = 0;                //initialization
    LL_Adv_Para.Adv_Para.LL_Adv_Data_Length = LEN_BD_ADDR;          //initialization
    LL_Msg_AdvScnConn = 0;                                          //initialization

    LL_Adv_Para.Adv_Para.LL_Tx_PowerLevel = TX_POWER_8_DBM;

    for (i = 0; i < MAX_TBLK_LL_NO; i++)
    {
        TmrBlk_LL[i].Next = i + 1;                      //initialization
    }
    TBlk_Free_LL = 0;                                   //initialization
    TBlk_InUse_LL = MAX_TBLK_LL_NO;                     //initialization

    status_LL_Tmr = LL_INT_S0;                          //initialization
    anchor_LL_Tmr = 0;                                  //initialization

    LL_Para_Interval.LL_SMP_DataCh = 0;                 //initialization

    Knl_CodeCpy(LL_Ref_ChMap, LL_CHMAP_DEFAUT, LEN_LL_CH_MAP);                          //initialization

    for (i = 0; i < SIZE_WHITE_LIST_ENTRIES; i++)
    {
        LL_WhiteList_Para[i].LE_WhiteList_Para.AddrType = DEF_EMPTY_WHITE_LIST;         //initialization
    }
}


#pragma Otime
Uint8 RF_cmpFIFO_BDAddr(Uint8 HeaderSts_Rx, MBLK *pMBlk)
{
    if (LL_Adv_Para.Adv_Para.LL_Own_Addr_Type == 0)
    {
        if ((HeaderSts_Rx & FLD_MSK_LL_ADV_RXADDR_TYPE))
        {
            return FAIL;
        }
        else
        {
            return Knl_MemComp_Isr(ble_device_param.ble_deviceAddr_param.addr, &pMBlk->Para.Data[LEN_BD_ADDR], LEN_BD_ADDR);

        }
    }
    else
    {
        if ((HeaderSts_Rx & FLD_MSK_LL_ADV_RXADDR_TYPE))
        {
            return Knl_MemComp_Isr(BD_Rand_Addr, &pMBlk->Para.Data[LEN_BD_ADDR], LEN_BD_ADDR);
        }
        else
        {
            return FAIL;
        }
    }
}


Uint8 RF_cmpFIFO_WhiteList(Uint8 HeaderSts_Rx, MBLK *pMBlk)
{
    Uint8 j, status;

    j = 0;
    if ((HeaderSts_Rx & FLD_MSK_LL_ADV_TXADDR_TYPE))
    {
        j = HCI_ADDR_TYPE_RANDOM;
    }
    for (status = 0; status < SIZE_WHITE_LIST_ENTRIES; status++)
    {
        if (LL_WhiteList_Para[status].LE_WhiteList_Para.AddrType == j)
        {
            if (Knl_MemComp_Isr(pMBlk->Para.Data, &(LL_WhiteList_Para[status].LE_WhiteList_Para.Addr)[0], LEN_BD_ADDR) == SUCCESS)
            {
                break;
            }
        }
    }
    if (status == SIZE_WHITE_LIST_ENTRIES)
        return FAIL;
    else
        return SUCCESS;
}


void LL_TmrBlk_Rls(void)
{
    Uint8 i;

    i = tblk_LL_pi->Next;
    tblk_LL_pi->Next = TBlk_Free_LL;
    TBlk_Free_LL = TBlk_InUse_LL;
    TBlk_InUse_LL = i;
    if (i == MAX_TBLK_LL_NO)
    {
        tblk_LL_pi = (TBLK_LLx *)0;
    }
    else
    {
        tblk_LL_pi = tblk_LL_pi2;
        i = tblk_LL_pi->Next;
    }
    if (i == MAX_TBLK_LL_NO)
    {
        tblk_LL_pi2 = (TBLK_LLx *)0;
    }
    else
    {
        tblk_LL_pi2 = &TmrBlk_LL[i];
    }
}


void LL_TmrBlk_Rls_Pair(void)
{
    Uint8 i;
    Uint16 j;

    i = tblk_LL_pi2->Next;
    tblk_LL_pi2->Next = TBlk_Free_LL;
    TBlk_Free_LL = TBlk_InUse_LL;
    TBlk_InUse_LL = i;
    j = tblk_LL_pi2->Ticks;

    if (i == MAX_TBLK_LL_NO)
    {
        tblk_LL_pi = (TBLK_LLx *)0;
    }
    else
    {
        tblk_LL_pi = &TmrBlk_LL[i];
        tblk_LL_pi->Ticks = tblk_LL_pi->Ticks + j;
        i = tblk_LL_pi->Next;
    }
    if (i == MAX_TBLK_LL_NO)
    {
        tblk_LL_pi2 = (TBLK_LLx *)0;
    }
    else
    {
        tblk_LL_pi2 = &TmrBlk_LL[i];
    }
}





void LL_TmrBlk_Pt_PairRst(void)
{
    if (TBlk_InUse_LL != MAX_TBLK_LL_NO)
    {
        tblk_LL_pi = &TmrBlk_LL[TBlk_InUse_LL];
        if (tblk_LL_pi->Next == MAX_TBLK_LL_NO)
        {
            tblk_LL_pi2 = (TBLK_LLx *)0;
        }
        else
        {
            tblk_LL_pi2 = &TmrBlk_LL[tblk_LL_pi->Next];
        }
    }
}



void LL_MsgBlk_LL_conn_Para_Rls(Uint8 ConnID)
{

}




void LLTimer_TmrRefUpd_Isr(void)
{
    extern uint32_t Timeline24;

    Tmr37 += DUR_LL_TMR_TICK_BASE * 125;
    ((uint8_t *)&Tmr37)[4] &= 0x1F;

    Timeline24 += DUR_LL_TMR_TICK_BASE;
    if (((uint8_t *)&Timeline24)[3] > 0xEF)
    {
        ((uint8_t *)&Timeline24)[3] &= 0x0F;
    }
}



#pragma pop

