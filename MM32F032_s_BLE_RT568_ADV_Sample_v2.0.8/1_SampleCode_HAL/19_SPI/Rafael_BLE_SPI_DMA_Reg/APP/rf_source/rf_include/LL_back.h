/*******************************************************************
 *
 * File Name  : LL.H
 * Description:
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

#ifndef _BLE_LL_H_
#define _BLE_LL_H_

#include "knl_pblc.h"  //LEN_LL_CH_MAP

#define MAX_NUM_CONN_SLV_HDL                    1
#define MAX_NUM_CONN_MAS_HDL                    4
#define MAX_NUM_CONN_HDL                        MAX_NUM_CONN_MAS_HDL+MAX_NUM_CONN_SLV_HDL

//LL_CONN_ID_CONN_PARA                          0~(MAX_NUM_CONN_HDL-1)
//#define LL_CONN_ID_CONN_S_PARA                  MAX_NUM_CONN_MAS_HDL
#define LL_CONN_ID_ADV_PARA                     MAX_NUM_CONN_HDL
#define LL_CONN_ID_SCAN_PARA                    (MAX_NUM_CONN_HDL+1)
#define LL_CONN_ID_INIT_PARA                    (MAX_NUM_CONN_HDL+2)
#define LL_CONN_ID_MASTER_RSV                   MAX_NUM_CONN_MAS_HDL
#define LL_CONN_ID_SLAVE_RSV                    MAX_NUM_CONN_HDL

#define LL_PATCH_MIC_FAIL_2M_PHY
#ifdef LL_PATCH_MIC_FAIL_2M_PHY
//#define LL_PATCH_MIC_FAIL_DATA_SIZE_LESS
#define LL_PATCH_MIC_FAIL_DATA_SIZE_PARTICULAR
#define LEN_LL_PATCH_MIC_FAIL_CONDITION     7                   //Could be modified by users (less than 8).
#endif

enum
{
    _TICK_BASE_125P00_ = 0U,
    _TICK_BASE_156P25_ = 1U
};
#define _TICK_BASE_                 _TICK_BASE_125P00_

#if _TICK_BASE_ ==  _TICK_BASE_125P00_
#define _TICK_BASE_MULTIPLIER_      4
#else   //(#if _TICK_BASE_ ==  _TICK_BASE_125P00_)
#define _TICK_BASE_MULTIPLIER_      5
#endif  //(#if _TICK_BASE_ ==  _TICK_BASE_125P00_)

#define LL_PATCH_CONN_UDATE_FAIL_DUE_TO_USER_DELAY
/*******************************************************************
 *      Primitive type to LL task
 *******************************************************************/


/*******************************************************************
 *      Primitive type to ENC task
 *******************************************************************/

#define LEN_CRC_INIT                3                                                   //LL__013

//LL__014
#define LEN_ADVPKT_HEADER           2
#define LEN_ADV_PAYLOAD_MAX         (LEN_BD_ADDR+LEN_ADV_SCAN_DATA_MAX)
#define LEN_ADV_PKT_MAX             (LEN_ADVPKT_HEADER+LEN_ADV_PAYLOAD_MAX)


#define LEN_AES_KEY                                             16
#define LEN_SMP_RAND                                            8
#define LEN_SMP_EDIV                                            2
#define LEN_SMP_SKDM                                            8
#define LEN_SMP_IVM                                             4
#define LEN_SMP_SKDS                                            8
#define LEN_SMP_IVS                                             4
#define LEN_SMP_IRK                                             16
#define LEN_SMP_CSRK                                            16
#define LEN_SMP_PKTCNT                                          5


#define LL_TMR_S0                               0x00
#define LL_TMR_S1                               (LL_TMR_S0 + 1)
#define LL_TMR_S2                               (LL_TMR_S0 + 2)
#define LL_TMR_S255                             (LL_TMR_S0 + 255)

#define LL_INT_S0                               0x00
#define LL_INT_S10                              (LL_INT_S0 + 10)
#define LL_INT_S11                              (LL_INT_S0 + 11)
#define LL_INT_S12                              (LL_INT_S0 + 12)
#define LL_INT_S13                              (LL_INT_S0 + 13)
#define LL_INT_S14                              (LL_INT_S0 + 14)
#define LL_INT_S15                              (LL_INT_S0 + 15)

#define LL_TMR_TICKS_RSV_WAKEUP                 3   //by user
#define LL_TMR_TICKS_RSV_BASE                   2   //by user

#define SIZE_WHITE_LIST_ENTRIES                 4
#define MAX_TBLK_LL_NO                          (2*(MAX_NUM_CONN_HDL+1))    //1: reserved for Flash control
#define DEF_EMPTY_WHITE_LIST                    0xFC

#define NUM_LL_ADV_CH                           3                           //LL__017
#define NUM_LL_DATA_CH                          37                          //LL__017

#define NUM_LL_CONN_FAIL_ESTB                   6                           //LL__029

#define LL_ADV_FLTR_POLICY_NOT_USED             0x00                        //LL__022
#define LL_ADV_FLTR_POLICY_SCAN_WHT_LST         0x01                        //LL__022
#define LL_ADV_FLTR_POLICY_CONN_WHT_LST         0x02                        //LL__022
#define LL_ADV_FLTR_POLICY_SCAN_CONN_WHT_LST    0x03                        //LL__022
#define LL_SCN_FLTR_POLICY_NOT_USED             0x00                        //HCI__027
#define LL_SCN_FLTR_POLICY_WHT_LST              0x01                        //HCI__027
#define LL_INIT_FLTR_POLICY_NOT_USED            0x00                        //LL__003
#define LL_INIT_FLTR_POLICY_WHT_LST             0x01                        //LL__003

//LL__020
#define LE_LL_SCA_500PPM    0
#define LE_LL_SCA_250PPM    1
#define LE_LL_SCA_150PPM    2
#define LE_LL_SCA_100PPM    3
#define LE_LL_SCA_075PPM    4
#define LE_LL_SCA_050PPM    5
#define LE_LL_SCA_030PPM    6
#define LE_LL_SCA_020PPM    7
#define LE_LL_SCA           LE_LL_SCA_500PPM

//LL__021
#define LE_ADV_TYPE_ADV_IND             0x00 // undirected,connectable
#define LE_ADV_TYPE_ADV_DIRECT_IND      0x01 // directed  ,connecetable
#define LE_ADV_TYPE_ADV_NONCONN_IND     0x02 // undirected,non-scannable,non-connectable
#define LE_ADV_TYPE_SCAN_REQ            0x03
#define LE_ADV_TYPE_SCAN_RSP            0x04
#define LE_ADV_TYPE_CONNECT_REQ         0x05
#define LE_ADV_TYPE_ADV_SCAN_IND        0x06 // scannable, undirected
#define LE_ADV_TYPE_RESERVED            0x07

#define FLD_MSK_LL_ADV_TXADDR_TYPE      0x40
#define FLD_MSK_LL_ADV_RXADDR_TYPE      0x80
#define FLD_MSK_LL_ADV_TYPE             0x0F
#define FLD_MSK_LL_ADV_LEN              0x3F


#define LL_TX_PHYS_1M_PHY               0x01
#define LL_TX_PHYS_2M_PHY               0x02
#define LL_TX_PHYS_CODED_PHY            0x04
#define LL_TX_PHYS_RESERVED             0xF8
#define LL_TX_PHYS_DEFAULT              LL_TX_PHYS_1M_PHY
#define LL_RX_PHYS_1M_PHY               0x01
#define LL_RX_PHYS_2M_PHY               0x02
#define LL_RX_PHYS_CODED_PHY            0x04
#define LL_RX_PHYS_RESERVED             0xF8
#define LL_RX_PHYS_DEFAULT              LL_RX_PHYS_1M_PHY
#define LL_RX_PHYS_TX_NO_SUPPORT        (0|LL_TX_PHYS_CODED_PHY|LL_TX_PHYS_RESERVED)
#define LL_RX_PHYS_RX_NO_SUPPORT        (0|LL_RX_PHYS_CODED_PHY|LL_RX_PHYS_RESERVED)    //symmetric, ==LL_RX_PHYS_TX_NO_SUPPORT

#define LL_PHY_OPTIONS_CODED_PHY_PREPER_NONE    0x00
#define LL_PHY_OPTIONS_CODED_PHY_PREPER_S2      0x01
#define LL_PHY_OPTIONS_CODED_PHY_PREPER_S8      0x02
#define LL_PHY_OPTIONS_CODED_PHY_PREPER_MSK     0x03

#define LL_ALL_PHYS_PREFER_TX_Y_RX_Y    0x00
#define LL_ALL_PHYS_PREFER_TX_N_RX_Y    0x01
#define LL_ALL_PHYS_PREFER_TX_Y_RX_N    0x02
#define LL_ALL_PHYS_PREFER_TX_N_RX_N    (LL_ALL_PHYS_PREFER_TX_N_RX_Y|LL_ALL_PHYS_PREFER_TX_Y_RX_N)


#define LL_CONN_ID_NORMAL                       0x00                                        //LL__026, LL__027
#define LL_CONN_ID_STBY                         0x01                                        //LL__026, LL__027
#define LL_CONN_ID_ADV                          0x02                                        //LL__026, LL__027
#define LL_CONN_ID_INIT                         0x03                                        //LL__026, LL__027
#define LL_CONN_ID_CONN                         0x04                                        //LL__026, LL__027
#define LL_CONN_ID_LL_TERM_WAIT                 0x05
#define LL_CONN_ID_LL_CONN_UPD                  0x06
#define LL_CONN_ID_LL_MSK_RCVD                  0x80


#define LL_CONN_ID_RSV                          0xFF

#define LL_CONN_ID_HDL_H                        0x01                        //LL__026, HCI__007

#define LL_SMP_GATE_OFF                         0
#define LL_SMP_GATE_ENC_WAIT                    1
#define LL_SMP_GATE_ENC_PAUSE                   2

#define LL_SMP_DATA_CH_R_T_NORMAL               0
#define LL_SMP_DATA_CH_T_CCM                    0x01
#define LL_SMP_DATA_CH_R_CCM                    0x02
#define LL_SMP_DATA_CH_MASTER                   0x08
#define LL_SMP_DATA_CH_R_T_CCM                  (LL_SMP_DATA_CH_T_CCM|LL_SMP_DATA_CH_R_CCM)
#define LL_SMP_DATA_CH_MSK                      (LL_SMP_DATA_CH_T_CCM|LL_SMP_DATA_CH_R_CCM)

#define MSK_LL_SMP_DATA_CH_MASTER               0x80

//Below reference to FLD_MSK_LL_DATA_RSV
#define LL_ACK_TX_NEW_N_EPT             0x20    //Ack w. Not empty LL_data packet
#define LL_PKT_TX_EI                    0x40    //End for integrated.

#define LL_ACK_RX_NEW                   0x40
#define LL_ACK_TX_NEW                   0x80

#define LL_ACK_TO_RO                    0x00
#define LL_ACK_TO_RN                    LL_ACK_RX_NEW
#define LL_ACK_TN_RO                    LL_ACK_TX_NEW
#define LL_ACK_TN_RN                    (LL_ACK_TX_NEW|LL_ACK_RX_NEW)

#define FLD_LL_SCN_REQ_BGN_SCN_ADDR     0
#define FLD_LL_SCN_REQ_BGN_ADV_ADDR     FLD_LL_SCN_REQ_BGN_SCN_ADDR+LEN_BD_ADDR
#define FLD_LL_SCN_RSP_BGN_ADV_ADDR     0
#define FLD_LL_CONN_REQ_BGN_INI_ADDR    0
#define FLD_LL_CONN_REQ_BGN_ADV_ADDR    FLD_LL_SCN_REQ_BGN_SCN_ADDR+LEN_BD_ADDR

#define FLD_LL_CTRL_OPCODE_TRXFIFO      0
#define FLD_LL_CTRL_PARAM_BGN_TRXFIFO   1
#define FLD_LL_DATA_PARAM_BGN_TRXFIFO   0

#define LL_PARA_MIN_SCN_WINDOW          (_TICK_BASE_MULTIPLIER_<<2) //HCI__027
#define LL_PARA_MAX_SCN_WINDOW          65535                       //HCI__027
#define LL_PARA_MIN_INIT_WINDOW         (_TICK_BASE_MULTIPLIER_<<2) //LL__003
#define LL_PARA_MAX_INIT_WINDOW         65535                       //LL__003

#define DUR_LL_TMR_TICK_BASE            1
#define DUR_LL_RSV_PRECAL               1
#define DUR_LL_RSV_SCN_BASE             (11+DUR_LL_RSV_PRECAL)
#define DUR_LL_RSV_INIT_BASE            (8+DUR_LL_RSV_PRECAL)
#define DUR_LL_RSV_CONN_NXTCH_CAL       1
#define DUR_LL_RSV_CONN_BASE            (7+DUR_LL_RSV_CONN_NXTCH_CAL)
#define DUR_LL_RSV_CONN_MD              (8+DUR_LL_RSV_CONN_NXTCH_CAL)
#define DUR_LL_RSV_CONN_MD_SMS          (12+DUR_LL_RSV_CONN_NXTCH_CAL)
#define DUR_LL_RSV_CONN_MD_MSMS         (15+DUR_LL_RSV_CONN_NXTCH_CAL)
#define DUR_LL_RSV_CONN_WINDELAY        10
#define DUR_LL_RSV_CONN_WINOFFSET       10
#define DUR_LL_RSV_INIT_ADVEND_2_WINDELAY   (5+DUR_LL_RSV_CONN_WINDELAY)

#define DUR_LL_RSV_MIN_SCAN             6
#define DUR_LL_RSV_MIN_INIT             4
#define DUR_LL_RSV_MIN_CONN_SLV         3
#ifdef LL_PATCH_MIC_FAIL_DATA_SIZE_LESS
#define DUR_LL_RSV_MIN_AES_CCM          3   //2: normal, 3:for CCM
#else
#define DUR_LL_RSV_MIN_AES_CCM          2
#endif
#define LL_INTERVAL_DIR_ADV_HIGHDUTY    (10*3)



#define DUR_LL_RSV_MIN_TBLK_LL_ADV      1
#define DUR_LL_RSV_MIN_TBLK_LL_SCN      1
#define DUR_LL_RSV_MIN_TBLK_LL_INIT     1
#define DUR_LL_RSV_MIN_TBLK_LL_CONN     1

#if     ((DUR_LL_RSV_MIN_TBLK_LL_ADV<=DUR_LL_RSV_MIN_TBLK_LL_SCN)&&(DUR_LL_RSV_MIN_TBLK_LL_INIT<=DUR_LL_RSV_MIN_TBLK_LL_SCN))
#define DUR_LL_RSV_MIN_TBLK_LL_CMMN     DUR_LL_RSV_MIN_TBLK_LL_SCN
#elif   ((DUR_LL_RSV_MIN_TBLK_LL_ADV<=DUR_LL_RSV_MIN_TBLK_LL_INIT)&&(DUR_LL_RSV_MIN_TBLK_LL_SCN<=DUR_LL_RSV_MIN_TBLK_LL_INIT))
#define DUR_LL_RSV_MIN_TBLK_LL_CMMN     DUR_LL_RSV_MIN_TBLK_LL_INIT
#else
#define DUR_LL_RSV_MIN_TBLK_LL_CMMN     DUR_LL_RSV_MIN_TBLK_LL_ADV
#endif

#define LL_TOUT_LL_CONN                 0x6400
#define LL_TOUT_LL_CTRL                 0x7D00
#define LL_INST_MOD_LMT                 32767

#define RF_MSG_RF0INT_WTR               0x01
#define RF_MSG_RF0INT_WTR_T             0x02
//LL__022
typedef struct Adv_Para
{
    Uint8 LL_AdvMap_ID;             //addr: 00
    Uint8 LL_AdvConn_ID;            //addr: 01
    Uint16 LL_Adv_Interval_Min;     //addr: 02
    Uint16 LL_Adv_Interval_Max;     //addr: 04
    Uint8 LL_Adv_Type;              //addr: 06
    Uint8 LL_Own_Addr_Type;         //addr: 07
    Uint8 LL_DirectAddr_Type;       //addr: 08
    Uint8 LL_DirectAddr[LEN_BD_ADDR];   //addr: 09
    Uint8 LL_Adv_Channel_Map;           //addr: 0F
    Uint8 LL_Adv_Filter_Policy;         //addr: 10
    Uint8 LL_Adv_Data_Length;           //addr: 11
    Uint8 LL_Adv_Data[LEN_ADV_SCAN_DATA_MAX];   //addr: 12-30
    Uint8 LL_ScanRsp_Data_Length;       //addr: 31
    Uint8 LL_ScanRsp_Data[LEN_ADV_SCAN_DATA_MAX];   //addr: 32-50
    Uint8 LL_Tx_PowerLevel;             //addr: 51
} Adv_Para;
//#define LEN_LE_ADV_PARA     (1*9+2*2+LEN_BD_ADDR+2*LEN_ADV_SCAN_DATA_MAX)
#define LEN_LE_ADV_PARA         sizeof(Adv_Para)


typedef struct LE_Conn_Para
{
    Uint8 LL_Conn_ID;                   //addr: 00
    Uint8 LL_Feature;                   //addr: 01
    Uint8 LL_Feature1;                  //addr: 02
    Uint8 LL_Tx_PowerLevel;             //addr: 03
    Uint32 LL_AccessAddr;               //addr: 04
    Uint8 LL_CRC_Init[LEN_CRC_INIT];    //addr: 08-0A
    Uint8 WinSize_DataHdr;              //addr: 0B
    Uint16 WinOffset_LtcyAccu;          //addr: 0C
    Uint16 LL_ConnInterval;             //addr: 0E
    Uint16 LL_ConnLatency;              //addr: 10
    Uint16 LL_SvisionTimeout;           //addr: 12
    Uint8 LL_ChMapReM[LEN_LL_CH_MAP];   //addr: 14-18
    Uint8 LL_HopIncrement;              //addr: 19
    Uint16 LL_EventCounter;             //addr: 1A
    Uint8 LL_CurrentCH;                 //addr: 1C
    Sint8 RF_Rssi;
    Uint16 LL_ConnIntervalOrg;          //addr: 1E
    Uint16 LL_ConnIntervalOrgUpd;       //addr: 20
    Uint8 ErrCode_DisConn;              //addr: 22
    Uint8 WinSizeUpd;                   //addr: 23
    Uint16 WinOffsetUpd;                //addr: 24
    Uint16 LL_ConnIntervalUpd;          //addr: 26
    Uint16 LL_ConnLatencyUpd;           //addr: 28
    Uint16 LL_SvisionTimeoutUpd;        //addr: 2A
    Uint16 LL_EventCounterUpd;          //addr: 2C
    Uint8 LL_RF_Data_Ch_ReM[NUM_LL_DATA_CH];    //addr: 2E-52
    Uint8 LL_SMP_Gate;                  //addr: 53
    Uint8 LL_SMP_IV_DUMP[(LEN_SMP_SKDM-LEN_SMP_IVM)];   //addr: 54-57
    Uint8 LL_SMP_IV[(LEN_SMP_IVS+LEN_SMP_IVM)];         //addr: 58-5F
    Uint8 LL_SMP_Key[LEN_AES_KEY];                      //addr: 60-6F
    Uint32 LL_SMP_packetCounterT;                       //addr: 70
    Uint8 LL_SMP_packetCounterTd;                       //addr: 74
    Uint8 LL_SMP_DataCh;                                //addr: 75
    Uint8 LL_Tx_PHYsUpd;                                //addr: 76
    Uint8 LL_Rx_PHYsUpd;                                //addr: 77
    Uint32 LL_SMP_packetCounterR;                       //addr: 78
    Uint8 LL_SMP_packetCounterRd;                       //addr: 7C
    Uint8 LL_Tx_PHYS;                                   //addr: 7D
    Uint8 LL_Rx_PHYS;                                   //addr: 7E
    Uint8 LL_Rx_length_1M;                              //addr: 7F
    Uint8 LL_Rx_length_2M;                              //addr: 80
    Uint8 LL_Tx_length_1M;                              //addr: 81
    Uint8 LL_Tx_length_2M;                              //addr: 82
    Uint16 LL_SvToutAccu;                               //addr: 84
    Uint16 LL_PrToutAccu;                               //addr: 86-87
} LE_Conn_Para;         //LL__019, LL__003
#define LEN_LE_CONN_PARA        (sizeof(LE_Conn_Para))



#define LEN_LE_CONN_REQ     (1*2+2*4+4*1+LEN_BD_ADDR*2+LEN_CRC_INIT+LEN_LL_CH_MAP)

typedef struct LE_WhiteList_Para
{
    Uint8 AddrType;             //addr: 00
    Uint8 Addr[LEN_BD_ADDR];    //addr: 01
} LE_WhiteList_Para;
#define LEN_LE_WHITELIST_PARA        sizeof(LE_WhiteList_Para)


typedef union LL_WhiteList
{
    Uint8 Data[LEN_LE_WHITELIST_PARA];
    LE_WhiteList_Para LE_WhiteList_Para;
} LL_WhiteList;


typedef union LL_Adv
{
    Uint8 Data[LEN_LE_ADV_PARA];
    Adv_Para Adv_Para;
} LL_Adv;

typedef union LL_Conn
{
    Uint8 Data[LEN_LE_CONN_PARA];
    LE_Conn_Para LE_Conn_Para;
} LL_Conn;

typedef struct LL_Para_Itrvl
{
    Uint8 HeaderSts;
    Uint8 HeaderLen;
    Uint8 LL_SMP_DataCh;
    Uint8 Padding;
} LL_Para_Itrvl;


typedef struct LL_Para_Header
{
    Uint8 HeaderSts;
    Uint8 HeaderLen;
    Uint8 Padding0;
    Uint8 Padding1;
} LL_Para_Header;


//extern const Uint32 LL_ACCESS_ADDR_ADV;

#endif
