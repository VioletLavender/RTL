#pragma push
#pragma Ospace
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


extern BLE_Device_Param ble_device_param;

extern LL_Adv          LL_Adv_Para;
extern LL_Conn         LL_Conn_Para[MAX_NUM_CONN_HDL];
extern LL_WhiteList    LL_WhiteList_Para[SIZE_WHITE_LIST_ENTRIES];
extern LL_Para_Itrvl   LL_Para_Interval;
extern Uint8           LL_ConnID_Remaining;

extern Uint8 LL_Ref_ChMap[LEN_LL_CH_MAP];

extern TBLK_LLx TmrBlk_LL[MAX_TBLK_LL_NO];
extern Uint8 TBlk_Free_LL;
extern Uint8 TBlk_InUse_LL;
extern TBLK_LLx *tblk_LL_pi;
extern TBLK_LLx *tblk_LL_pi2;

extern Uint8 status_LL_Tmr;
extern Uint8 anchor_LL_Tmr;
extern LL_Conn *LL_conn_pi;
extern Uint16 LL_DurRxPktAccu;

extern Uint8 BD_Rand_Addr[LEN_BD_ADDR];
extern Uint32 LL_Slv_Win_Width;
extern Uint32 LL_Slv_Win_Width_Base;
extern uint32_t seedR16;
extern Uint8 RX_CRC_valid_flag;

extern uint64_t Tmr37;

extern Uint8 LL_CHMAP_DEFAUT[LEN_LL_CH_MAP];

extern Uint16 LL_LENGTH[];

extern Uint32 LL_REF_ACS_ADDR_ADVSCN;

extern Uint8 LL_REF_CRC_INI_ADVSCN[LEN_CRC_INIT];

extern Uint8 LL_DUR_RSV[];

extern uint8_t LL_RF_DATA_CH[];

extern uint8_t CH_ADV_SEL_TABLE[];


extern Uint8 CH_ADV_CH_HOP_BY_MAP_TABLE[][4];
extern Uint8 Ch_ADV_Ch_Hop_Table[4];


extern Uint8 LL_Msg_AdvScnConn;
#define LL_MSG_ADVSCNCONN_ADV_EN                0x80
#define LL_MSG_ADVSCNCONN_SCN_EN                0x40
#define LL_MSG_ADVSCNCONN_INIT_EN               0x20
#define LL_MSG_ADVSCNCONN_ADV_CONT              0x10
#define LL_MSG_ADVSCNCONN_SCN_CONT              0x08
#define LL_MSG_ADVSCNCONN_INIT_CONT             0x04


#define MAX_INTVL_ADV_CONSTI                    80
#define MAX_INTVL_ADV_DIRECT_H                  10240

extern uint8_t RF_Msg_RF0INT;

#pragma Otime
void LL_TmrBlk_Rls_NxtIntvl(void)
{
    if(TBlk_InUse_LL == MAX_TBLK_LL_NO)
    {
        RF_Tmr_Periodic_set_ISR(DUR_LL_WAKEUP_MIN);
    }
    else
    {
        RF_Tmr_Periodic_set_ISR(tblk_LL_pi->Ticks);
    }
}

void LL_RxEnter_Isr(void)
{
    uint8_t i;

    //First T/R=R
    //R121[6]=1: MAC go to RX state after wakeup
    i = RFIP_reg_MEM[RFIP_REG_MEM_121] | REG_121_T_R_SEL_WKP;
    SPI_1BYT_SetTx_Isr(RFIP_REG_121, i);
    RFIP_reg_MEM[RFIP_REG_MEM_121] = i;

    RF_Msg_RF0INT = RF_MSG_RF0INT_WTR;

    if(ble_device_param.ble_deviceChipId==MP_A1)
    {
        ((uint8_t *)&Tmr37)[4] |= (RFIP_reg_MEM[RFIP_REG_MEM_119] | REG_119_TR_TRIG_MODE);   //set R119 to enable RX
        SPI_PDMA_SetTx(RFIP_REG_115, (uint32_t)&Tmr37, 5+1);
    }
    else
    {
        i = RFIP_reg_MEM[RFIP_REG_MEM_119] | REG_119_TR_TRIG_MODE;
        SPI_1BYT_SetTx_Isr(RFIP_REG_119, i);
    }
}

void LL_TxEnter_Isr(void)
{
    uint8_t i;

    //First T/R=T
    //R121[6]=0: MAC go to TX state after wakeup
    i = RFIP_reg_MEM[RFIP_REG_MEM_121] & (~REG_121_T_R_SEL_WKP);
    SPI_1BYT_SetTx_Isr(RFIP_REG_121, i);
    RFIP_reg_MEM[RFIP_REG_MEM_121] = i;

    RF_Msg_RF0INT = RF_MSG_RF0INT_WTR_T;

    if(ble_device_param.ble_deviceChipId==MP_A1)
    {
        ((uint8_t *)&Tmr37)[4] |= (RFIP_reg_MEM[RFIP_REG_MEM_119] | REG_119_TR_TRIG_MODE);   //set R119 to enable TX
        SPI_PDMA_SetTx(RFIP_REG_115, (uint32_t)&Tmr37, 5+1);
    }
    else
    {
        i = RFIP_reg_MEM[RFIP_REG_MEM_119] | REG_119_TR_TRIG_MODE;
        SPI_1BYT_SetTx_Isr(RFIP_REG_119, i);
    }
}

#define LL_RxExit_Isr()     rafael_reset_phy_fsm_Isr()

void LLTimer_Isr(void)
{
    extern uint32_t Timeline24;
    extern uint16_t Tmr16Interval;
    extern void LL_TmrBlk_Pt_PairRst(void);
    extern void LLTimer_TmrRefUpd_Isr(void);
    extern void LL_MsgBlk_LL_conn_Para_Rls(Uint8 ConnID);
    extern void LL_TmrBlk_Rls_Pair(void);
    extern void LL_TmrBlk_Rls(void);
    
    Uint8 tblk_i, j, k;
    Uint16 i16, j16;
    Uint32 *ptAddr;

    if(TBlk_InUse_LL == MAX_TBLK_LL_NO)
    {
        if((LL_Msg_AdvScnConn & (LL_MSG_ADVSCNCONN_ADV_EN|LL_MSG_ADVSCNCONN_SCN_EN|LL_MSG_ADVSCNCONN_INIT_EN)))
        {
            TBlk_InUse_LL = TBlk_Free_LL;

            LL_TmrBlk_Pt_PairRst();

            tblk_LL_pi->TmrId = LL_TMR_S1;                    //state:0 (Start)
            tblk_LL_pi->Ticks = LL_TMR_TICKS_RSV_WAKEUP;  //1


            tblk_LL_pi2->TmrId = LL_TMR_S255;
            TBlk_Free_LL = tblk_LL_pi2->Next;
            tblk_LL_pi2->Next = MAX_TBLK_LL_NO;
            if((LL_Msg_AdvScnConn & LL_MSG_ADVSCNCONN_ADV_EN))
            {
                LL_Msg_AdvScnConn &= (~LL_MSG_ADVSCNCONN_ADV_EN);
                LL_Msg_AdvScnConn |= LL_MSG_ADVSCNCONN_ADV_CONT;
                tblk_LL_pi->ConnId = LL_CONN_ID_ADV_PARA;
                tblk_LL_pi2->ConnId = LL_CONN_ID_ADV_PARA;
                tblk_LL_pi2->Ticks = LL_DUR_RSV[LL_Adv_Para.Adv_Para.LL_Adv_Type&FLD_MSK_LL_ADV_TYPE];
            }
            else if((LL_Msg_AdvScnConn & LL_MSG_ADVSCNCONN_SCN_EN))
            {

            }

            RF_Tmr_Periodic_set_ISR(LL_TMR_TICKS_RSV_WAKEUP);

        }
        else
        {
            Tmr37 += DUR_LL_WAKEUP_MIN*125;
            ((uint8_t *)&Tmr37)[4] &= 0x1F;
            Timeline24 += DUR_LL_WAKEUP_MIN;
            if(((uint8_t *)&Timeline24)[3] > 0xEF)
            {
                ((uint8_t *)&Timeline24)[3] &= 0x0F;
            }

            RF_PowerSaving_En_Isr();
        }
        return;
    }
    else
    {
        i16 = tblk_LL_pi->Ticks;
        if(i16 == Tmr16Interval)
        {
            anchor_LL_Tmr += ((Uint8 *) &i16)[0];
            if(tblk_LL_pi->TmrId < LL_TMR_S2)
            {
                if(tblk_LL_pi->TmrId != LL_TMR_S0)
                {
                    RF_Tmr_Periodic_set_ISR(LL_TMR_TICKS_RSV_WAKEUP);
                    LL_DurRxPktAccu = LL_DurRxPktAccu - LL_TMR_TICKS_RSV_WAKEUP;
                    tblk_LL_pi2->Ticks = tblk_LL_pi2->Ticks - LL_TMR_TICKS_RSV_WAKEUP;
                    tblk_LL_pi->TmrId = LL_TMR_S0;
                    tblk_LL_pi->Ticks = LL_TMR_TICKS_RSV_WAKEUP;
                    Tmr37 += LL_TMR_TICKS_RSV_WAKEUP*125;
                    ((uint8_t *)&Tmr37)[4] &= 0x1F;
                    Timeline24 += LL_TMR_TICKS_RSV_WAKEUP;
                    if(((uint8_t *)&Timeline24)[3] > 0xEF)
                    {
                        //Timeline24 &= 0x0FFFFFFF;
                        ((uint8_t *)&Timeline24)[3] &= 0x0F;
                    }
                }
                else
                {
                    RF_Tmr_Periodic_set_ISR(LL_TMR_TICKS_RSV_RF_INIT);

                    rafael_reset_phy_fsm_Isr();

                    RF_WTR_intOn();
                    tblk_i = tblk_LL_pi->ConnId;

                    if(tblk_i<LL_CONN_ID_ADV_PARA)
                    {

                    }
                    else
                    {
                        RF_CRCInit((uint8_t *)LL_REF_CRC_INI_ADVSCN);
                        SPI_PDMA_SetTx(RFIP_REG_122, (uint32_t)&LL_REF_ACS_ADDR_ADVSCN, LEN_BD_ACCESS_ADDR+1);
                        SPI_1BYT_SetTx_Isr(RFIP_REG_155, REG_155_AES_MODE_BYPASS);

                        switch(tblk_i)
                        {
                        case LL_CONN_ID_ADV_PARA:   //5:ADV
                            LL_conn_pi = &LL_Conn_Para[LL_Adv_Para.Adv_Para.LL_AdvConn_ID];
                            if((LL_Msg_AdvScnConn & LL_MSG_ADVSCNCONN_ADV_CONT)==0)     //not continuous adv
                            {
                                if(LL_Adv_Para.Adv_Para.LL_AdvConn_ID < LL_CONN_ID_SLAVE_RSV)
                                {
                                    LL_MsgBlk_LL_conn_Para_Rls(LL_Adv_Para.Adv_Para.LL_AdvConn_ID);
                                    LL_Adv_Para.Adv_Para.LL_AdvConn_ID = LL_CONN_ID_SLAVE_RSV;
                                }
                                RF_WTR_intOff();
                                LL_TmrBlk_Rls_Pair();
                                LL_TmrBlk_Rls_NxtIntvl();
                                return;
                            }
                            switch(status_LL_Tmr)
                            {
                            case LL_INT_S0:
                                RF_Set_TxPowerLevel_Isr(LL_Adv_Para.Adv_Para.LL_Tx_PowerLevel);
                                LL_TxEnter_Isr();

                                setChannel_BLE(CH_ADV_SEL_TABLE[LL_Adv_Para.Adv_Para.LL_AdvMap_ID]);

                                RF_PLL_VCO_Bank_set(CH_PLL_bank_Table[CH_ADV_SEL_TABLE[LL_Adv_Para.Adv_Para.LL_AdvMap_ID]]);


                                LL_Adv_Para.Adv_Para.LL_AdvMap_ID = Ch_ADV_Ch_Hop_Table[LL_Adv_Para.Adv_Para.LL_AdvMap_ID];

                                LL_Para_Interval.HeaderSts = LL_Adv_Para.Adv_Para.LL_Adv_Type;
                                tblk_i = LL_Adv_Para.Adv_Para.LL_Adv_Type;
                                tblk_i = tblk_i&FLD_MSK_LL_ADV_TYPE;

                                if(tblk_i == LE_ADV_TYPE_ADV_DIRECT_IND)
                                {
                                    j = (LEN_BD_ADDR+LEN_BD_ADDR);
                                }
                                else
                                {
                                    j = LL_Adv_Para.Adv_Para.LL_Adv_Data_Length;
                                }
                                LL_Para_Interval.HeaderLen = j;
                                RF_LE_HeaderStsLen_Tx(&LL_Para_Interval.HeaderSts);

                                if(LL_Adv_Para.Adv_Para.LL_Own_Addr_Type == HCI_ADDR_TYPE_PUBLIC)
                                {
                                    RF_TxFIFO_ADVaddr_set(ble_device_param.ble_deviceAddr_param.addr);
                                }
                                else
                                {
                                    RF_TxFIFO_ADVaddr_set(BD_Rand_Addr);
                                }
                                if(tblk_i == LE_ADV_TYPE_ADV_DIRECT_IND)
                                {
                                    ptAddr = (uint32_t *)LL_Adv_Para.Adv_Para.LL_DirectAddr;
                                }
                                else
                                {
                                    ptAddr = (uint32_t *)LL_Adv_Para.Adv_Para.LL_Adv_Data;
                                }

                                RF_SymbolRate_Patch_1M_2M(0);                               //RF_SymbolRate_set
                                if(tblk_i==LE_ADV_TYPE_ADV_NONCONN_IND)
                                {
                                    RF_TxAutoAckOff();     //disable auto TRX switch
                                    status_LL_Tmr = LL_INT_S10;
                                }
                                else
                                {
                                    RF_RxFIFOrst();        //Reset RxFIFO
                                    RF_TxAutoAckOn();      //enable Auto TRX switch
                                    status_LL_Tmr = LL_INT_S11;
                                }
                                RF_TxFIFO_ADVData_set((uint8_t *)ptAddr);


                                tblk_LL_pi2->Ticks = tblk_LL_pi2->Ticks - LL_TMR_TICKS_RSV_RF_INIT;
                                tblk_LL_pi->TmrId = LL_TMR_S3;
                                tblk_LL_pi->Ticks = LL_TMR_TICKS_RSV_RF_INIT;
                                break;

                            default:
                                ErrorEntry(2);
                                break;
                            }
                            break;

                        default:
                            break;
                        }
                    }
                }
            }
            else
            {
                if(tblk_LL_pi->TmrId != LL_TMR_S255)
                {
                    if(tblk_LL_pi->TmrId == LL_TMR_S3)
                    {
                        RF_Tmr_Periodic_set_ISR(DUR_LL_TMR_TICK_BASE);
                        if(ble_device_param.ble_deviceChipId==MP_A1)
                        {
                            ((uint8_t *)&Tmr37)[4] |= (RFIP_reg_MEM[RFIP_REG_MEM_119]);                 //disable trigger TRX. if not do this, ADV will TX again.
                            SPI_PDMA_SetTx(RFIP_REG_115, (uint32_t)&Tmr37, 5+1);
                        }
                        else
                        {
                            SPI_1BYT_SetTx(RFIP_REG_119, RFIP_reg_MEM[RFIP_REG_MEM_119] & 0x3F);        //R119[7:6]=0, disable manual TRX & trigger TRX
                        }
                        tblk_LL_pi->Ticks = DUR_LL_TMR_TICK_BASE;
                        tblk_LL_pi->TmrId = LL_TMR_S2;
                    }
                    tblk_i = tblk_LL_pi->ConnId;
                    if(tblk_i<LL_CONN_ID_ADV_PARA)
                    {

                    }
                    else
                    {
                        switch(tblk_i)
                        {
                        case LL_CONN_ID_ADV_PARA:
                            switch(status_LL_Tmr)
                            {
                            case LL_INT_S14:
                                break;

                            case LL_INT_S15:
                                RF_Tmr_Periodic_set_ISR(tblk_LL_pi2->Ticks);
                                LL_TmrBlk_Rls();
                                break;

                            case LL_INT_S12:
                                if(tblk_LL_pi2->Ticks == DUR_LL_TMR_TICK_BASE)
                                {
                                   // RF_Tmr_Periodic_set_ISR(tblk_LL_pi2->Ticks);
                                    RF_TxAutoAckOff();
                                    LL_RxExit_Isr();
                                    LL_TmrBlk_Rls();
                                    RF_RxFIFOrst();
                                }
                                else
                                {
                                    tblk_LL_pi2->Ticks = tblk_LL_pi2->Ticks - DUR_LL_TMR_TICK_BASE;
                                    LLTimer_TmrRefUpd_Isr();
                                }
                                break;

                            default:
                                if(tblk_LL_pi2->Ticks == DUR_LL_TMR_TICK_BASE)
                                {
                                    RF_Tmr_Periodic_set_ISR(tblk_LL_pi2->Ticks);
                                    LL_TmrBlk_Rls();
                                }
                                else
                                {
                                    tblk_LL_pi2->Ticks = tblk_LL_pi2->Ticks - DUR_LL_TMR_TICK_BASE;
                                    LLTimer_TmrRefUpd_Isr();
                                }
                                break;
                            }
                            break;

                        default:
                            ErrorEntry(9);
                            break;
                        }
                    }
                }
                else
                {
                    tblk_i = tblk_LL_pi->ConnId;
                    if(tblk_i<LL_CONN_ID_ADV_PARA)
                    {

                    }
                    else
                    {
                        switch(tblk_i)
                        {
                        case LL_CONN_ID_ADV_PARA:
                            switch(status_LL_Tmr)
                            {
                            case LL_INT_S13:
                                __NOP();
                                break;

                            case LL_INT_S14:
                                ErrorEntry(11);
                                break;

                            default:
                                break;
                            }
                            if((LL_Msg_AdvScnConn & LL_MSG_ADVSCNCONN_ADV_CONT))
                            {
                                k = LL_DUR_RSV[(LL_Adv_Para.Adv_Para.LL_Adv_Type&FLD_MSK_LL_ADV_TYPE)]+DUR_LL_RSV_MIN_TBLK_LL_CONN+DUR_LL_RSV_MIN_TBLK_LL_ADV;
                                j = tblk_LL_pi->Next;
                                tblk_i = tblk_LL_pi->Next;
                                while(tblk_i < MAX_TBLK_LL_NO)
                                {
                                    tblk_LL_pi2 = &TmrBlk_LL[tblk_i];
                                    if(tblk_LL_pi2->TmrId < LL_TMR_S2)
                                    {
                                        if(tblk_LL_pi2->Ticks-k >= 0)
                                        {
                                            j16 = tblk_LL_pi2->Ticks-k;
                                            if(LL_Adv_Para.Adv_Para.LL_AdvMap_ID)
                                            {
                                                tblk_LL_pi2->Ticks = j16;
                                                break;
                                            }
                                            else
                                            {
                                                if(LL_Adv_Para.Adv_Para.LL_Adv_Interval_Min < LL_Adv_Para.Adv_Para.LL_Adv_Interval_Max)
                                                {
                                                    tblk_LL_pi2->Ticks = j16;
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                    tblk_i = tblk_LL_pi2->Next;
                                    tblk_LL_pi = tblk_LL_pi2;
                                    LL_Adv_Para.Adv_Para.LL_Adv_Interval_Max = LL_Adv_Para.Adv_Para.LL_Adv_Interval_Max + tblk_LL_pi2->Ticks;
                                }
                                tblk_LL_pi->Next = TBlk_Free_LL;
                                tblk_LL_pi->TmrId = LL_TMR_S1;
                                tblk_LL_pi2 = &TmrBlk_LL[TBlk_Free_LL];
                                TBlk_Free_LL = tblk_LL_pi2->Next;
                                tblk_LL_pi2->Next = tblk_i;
                                k = k-(DUR_LL_RSV_MIN_TBLK_LL_CONN+DUR_LL_RSV_MIN_TBLK_LL_ADV);
                                tblk_LL_pi2->Ticks = k;
                                j16 = k;

                                tblk_LL_pi2->ConnId = LL_CONN_ID_ADV_PARA;
                                tblk_LL_pi2->TmrId = LL_TMR_S255;

                                if(tblk_i!=MAX_TBLK_LL_NO)
                                {
                                    i16 = DUR_LL_RSV_MIN_TBLK_LL_CONN;
                                }
                                else
                                {
                                    if(tblk_i != j)
                                    {
                                        i16 = DUR_LL_RSV_MIN_TBLK_LL_CONN;
                                    }
                                    else
                                    {
                                        if(LL_Adv_Para.Adv_Para.LL_Adv_Interval_Min == LL_INTERVAL_DIR_ADV_HIGHDUTY)
                                        {
                                            i16 = (LL_INTERVAL_DIR_ADV_HIGHDUTY/3)-k;
                                        }
                                        else
                                        {
                                            if(LL_Adv_Para.Adv_Para.LL_AdvMap_ID)
                                            {
                                                if(LL_Adv_Para.Adv_Para.LL_Adv_Interval_Min >= (MAX_INTVL_ADV_CONSTI<<2))
                                                {
                                                    i16 = MAX_INTVL_ADV_CONSTI-k;
                                                }
                                            }
                                            else
                                            {
                                                if(LL_Adv_Para.Adv_Para.LL_Adv_Interval_Min>LL_Adv_Para.Adv_Para.LL_Adv_Interval_Max)
                                                {
                                                    i16 = (LL_Adv_Para.Adv_Para.LL_Adv_Interval_Min-LL_Adv_Para.Adv_Para.LL_Adv_Interval_Max);
                                                }
                                                else
                                                {
                                                    i16 = DUR_LL_RSV_MIN_TBLK_LL_CONN;
                                                }
                                            }
                                        }
                                    }
                                }
                                tblk_LL_pi->Ticks = i16;
                                LL_Adv_Para.Adv_Para.LL_Adv_Interval_Max = LL_Adv_Para.Adv_Para.LL_Adv_Interval_Max + (i16+j16);
                                if(LL_Adv_Para.Adv_Para.LL_AdvMap_ID == 0)
                                {
                                    if(LL_Adv_Para.Adv_Para.LL_Adv_Interval_Min != LL_INTERVAL_DIR_ADV_HIGHDUTY)
                                    {
                                        LL_Adv_Para.Adv_Para.LL_Adv_Interval_Max = 0;
                                    }
                                    LL_Adv_Para.Adv_Para.LL_AdvMap_ID = Ch_ADV_Ch_Hop_Table[0];
                                }
                                if(j != MAX_TBLK_LL_NO)
                                {
                                    if(j != tblk_i)
                                    {
                                        TBlk_InUse_LL = j;
                                        LL_TmrBlk_Pt_PairRst();
                                    }
                                }
                                RF_Tmr_Periodic_set_ISR(tblk_LL_pi->Ticks);
                            }
                            else
                            {
                                LL_TmrBlk_Rls();
                                LL_TmrBlk_Rls_NxtIntvl();
                            }
                            break;

                        default:
                            ErrorEntry(12);
                            break;
                        }
                    }
                    status_LL_Tmr = LL_INT_S0;
                    if(RF_WTR_EnChk()==SUCCESS)
                    {
                        rafael_reset_phy_fsm_Isr();
                    }
                    if(tblk_LL_pi->TmrId < LL_TMR_S2)
                    {
                        if(tblk_LL_pi->ConnId < LL_CONN_ID_ADV_PARA) 
                        {

                        }
                    }

                    if(tblk_LL_pi->Ticks<(DUR_LL_RSV_MIN_TBLK_LL_CONN+DUR_LL_RSV_MIN_TBLK_LL_CMMN))
                    {
                        return;
                    }
                    if((LL_Msg_AdvScnConn & (LL_MSG_ADVSCNCONN_ADV_EN|LL_MSG_ADVSCNCONN_SCN_EN|LL_MSG_ADVSCNCONN_INIT_EN)))
                    {
                        if((LL_Msg_AdvScnConn & LL_MSG_ADVSCNCONN_ADV_EN))
                        {
                            LL_Msg_AdvScnConn &= (~LL_MSG_ADVSCNCONN_ADV_EN);
                            LL_Msg_AdvScnConn |= LL_MSG_ADVSCNCONN_ADV_CONT;
                            i16 = LL_DUR_RSV[(LL_Adv_Para.Adv_Para.LL_Adv_Type&FLD_MSK_LL_ADV_TYPE)]+DUR_LL_RSV_MIN_TBLK_LL_CONN+DUR_LL_RSV_MIN_TBLK_LL_ADV;

                            tblk_i = tblk_LL_pi2->Next;
                            while(tblk_i < MAX_TBLK_LL_NO)
                            {
                                tblk_LL_pi2 = &TmrBlk_LL[tblk_i];
                                if(tblk_LL_pi2->TmrId < LL_TMR_S2)
                                {
                                    if(tblk_LL_pi2->Ticks-i16 >= 0)
                                    {
                                        tblk_LL_pi2->Ticks = (tblk_LL_pi2->Ticks-i16)+DUR_LL_RSV_MIN_TBLK_LL_ADV;
                                        break;
                                    }
                                }
                                tblk_i = tblk_LL_pi2->Next;
                                tblk_LL_pi = tblk_LL_pi2;
                            }
                            tblk_LL_pi->Next = TBlk_Free_LL;
                            tblk_LL_pi = &TmrBlk_LL[TBlk_Free_LL];
                            tblk_LL_pi->TmrId = LL_TMR_S1;
                            tblk_LL_pi->Ticks = DUR_LL_RSV_MIN_TBLK_LL_CONN;
                            tblk_LL_pi2 = &TmrBlk_LL[tblk_LL_pi->Next];
                            TBlk_Free_LL = tblk_LL_pi2->Next;

                            tblk_LL_pi2->Next = tblk_i;
                            tblk_LL_pi2->Ticks = (i16-DUR_LL_RSV_MIN_TBLK_LL_CONN-DUR_LL_RSV_MIN_TBLK_LL_ADV);
                            tblk_LL_pi2->TmrId = LL_TMR_S255;

                            tblk_LL_pi->ConnId = LL_CONN_ID_ADV_PARA;
                            tblk_LL_pi2->ConnId = LL_CONN_ID_ADV_PARA;

                            LL_TmrBlk_Pt_PairRst();
                        }
                    }
                }
            }
            if(TBlk_InUse_LL != MAX_TBLK_LL_NO)
            {
                if(tblk_LL_pi->Ticks >= DUR_LL_RSV_MIN_AES_CCM)
                {
                }

                if(tblk_LL_pi->Ticks > DUR_PWR_DOWN_PERIOD_FCTR_125MULT)
                {
                    RF_PowerSaving_En_Isr();
                }

            }
        }
        else 
        {
            if(i16 > Tmr16Interval)
            {
                tblk_LL_pi->Ticks = i16 - Tmr16Interval;
                tblk_i = tblk_LL_pi->ConnId;
                if(tblk_i<LL_CONN_ID_ADV_PARA)
                {

                }
                else
                {
                    switch(tblk_i)
                    {
                    case LL_CONN_ID_ADV_PARA:
                        __NOP();
                        break;

                    case LL_CONN_ID_SCAN_PARA:
                        __NOP();
                        break;

                    default:
                        __NOP();
                        break;
                    }
                }
                __NOP();
            }
            else
            {
                __NOP();
            }
        }
    }
}

void LLWTR_Isr(void)
{
    Uint8 tblk_i;
    MBLK mblk;
    LL_Para_Header HeaderR;
    extern uint32_t Timeline24;
    extern uint8_t TIMELINE24_3750US_IDX[];
    extern Uint8 RF_cmpFIFO_WhiteList(Uint8 HeaderSts_Rx, MBLK *pMBlk);
    extern Uint8 RF_cmpFIFO_BDAddr(Uint8 HeaderSts_Rx, MBLK *pMBlk);
    
    tblk_i = tblk_LL_pi->ConnId;
    if(tblk_i<LL_CONN_ID_ADV_PARA)
    {

    }
    else
    {
        switch(tblk_i)
        {
        case LL_CONN_ID_ADV_PARA:
            switch(status_LL_Tmr)
            {
            case LL_INT_S11:
                status_LL_Tmr = LL_INT_S12;
                break;

            case LL_INT_S12:
                status_LL_Tmr = LL_INT_S15;
                if(RF_CRCchk() == FALSE)
                {
                    RF_RxFIFOrst();
                    rafael_reset_phy_fsm_Isr();
                    break;
                }
                SPI_PDMA_SetRx_Isr(RX_BUFFER_READ_PORT, (uint32_t) &HeaderR.HeaderSts, 2+1);
                tblk_i = LL_Adv_Para.Adv_Para.LL_Adv_Type& FLD_MSK_LL_ADV_TYPE;


                SPI_PDMA_SetRx_Isr(RX_BUFFER_READ_PORT, (uint32_t)mblk.Para.Data, LEN_LE_CONN_REQ+1);
                RF_RxFIFOrst();
                switch((HeaderR.HeaderSts&FLD_MSK_LL_ADV_TYPE))
                {
                case LE_ADV_TYPE_CONNECT_REQ:
                    break;

                case LE_ADV_TYPE_SCAN_REQ:
                    switch(tblk_i)
                    {
                    case LE_ADV_TYPE_ADV_IND:
                    case LE_ADV_TYPE_ADV_SCAN_IND:
                        switch(LL_Adv_Para.Adv_Para.LL_Adv_Filter_Policy)
                        {
                        case LL_ADV_FLTR_POLICY_SCAN_CONN_WHT_LST:
                        case LL_ADV_FLTR_POLICY_SCAN_WHT_LST:
                            if(RF_cmpFIFO_WhiteList(HeaderR.HeaderSts, &mblk)!=SUCCESS)
                                break;
                        default:
                            if(RF_cmpFIFO_BDAddr(HeaderR.HeaderSts, &mblk) == SUCCESS)
                            {
                                LL_Para_Interval.HeaderSts = ((LL_Adv_Para.Adv_Para.LL_Adv_Type&~FLD_MSK_LL_ADV_TYPE)|LE_ADV_TYPE_SCAN_RSP);
                                LL_Para_Interval.HeaderLen = LL_Adv_Para.Adv_Para.LL_ScanRsp_Data_Length+LEN_BD_ADDR;
                                RF_LE_HeaderStsLen_Tx(&LL_Para_Interval.HeaderSts);
                                if(LL_Adv_Para.Adv_Para.LL_Own_Addr_Type == HCI_ADDR_TYPE_PUBLIC)
                                {
                                    RF_TxFIFO_ADVaddr_set(ble_device_param.ble_deviceAddr_param.addr);
                                }
                                else
                                {
                                    RF_TxFIFO_ADVaddr_set(BD_Rand_Addr);
                                }
                                RF_TxFIFO_ADVData_set(LL_Adv_Para.Adv_Para.LL_ScanRsp_Data);
                                status_LL_Tmr = LL_INT_S13;
                            }
                            break;
                        }
                        break;

                    default:
                        break;
                    }
                    break;

                default:
                    break;
                }
                if(status_LL_Tmr == LL_INT_S15)
                {
                    rafael_reset_phy_fsm_Isr();
                }
                break;

            case LL_INT_S13:
                rafael_reset_phy_fsm_Isr();
            case LL_INT_S10:
                status_LL_Tmr = LL_INT_S15;
            default:
                break;
            }
            break;

        default:
            break;
        }
    }
}


void LL_GPIO_Isr(void)
{
    uint8_t i;
    extern void LLTimer_Isr(void);
    extern void LLWTR_Isr(void);
    extern uint8_t Content_ioInt;


    extern void RF_DC_Rstr_Isr(void);
    RF_DC_Rstr_Isr();

    //#ifdef _SW_PATCH_MP_
    if(ble_device_param.ble_deviceChipId==MP_A1)
    {
        while(1)
        {
            i = SPI_1BYT_SetRx_Isr(RFIP_REG_62);    //read current INT status
            if((i&(~INT_SETTING_R))==0)
                break;
        }
        i&=INT_SETTING_R;

        InterruptDisable();
        if(i)     //if not 0
        {
            Content_ioInt = i;                      //store current INT status
            SPI_1BYT_SetTx_Isr(RFIP_REG_62, i);     //Read and clear interrupt status, 2/2
        }
        else      //if(int status==0)
        {
            //check previous int status
            if(Content_ioInt&REG_62_WAKEUP)                 //if(last one is WAKEUP)
            {
                if(Content_ioInt == REG_62_WAKEUP)          //if(last one is WAKEUP)
                {
                    i = (INT_SETTING_R&(~REG_62_WAKEUP));   //set this one as TX_END or RX_END
                }
            }
            else                                            //else
            {
                i = REG_62_WAKEUP;                          //set this one as WAKEUP
            }
        }
        InterruptEnable();

        if((i&(INT_SETTING_R&(~REG_62_WAKEUP))))      //if INT is TX_END or RX_END
        {
            if((i&REG_62_TX_END))
            {
                rafael_spi_clear_tx_fifo_cnt();
            }

            InterruptDisable();
            LLWTR_Isr();
            InterruptEnable();
        }
        if((i&REG_62_WAKEUP))                         //if INT is WAKEUP
        {
            InterruptDisable();
            LLTimer_Isr();
            InterruptEnable();
        }
    }
    else  //not apply patch
    {
        i = (SPI_1BYT_SetRx_Isr(RFIP_REG_62)&INT_SETTING_R);    //Read and clear interrupt status, 1/2

        InterruptDisable();
        Content_ioInt = i;
        SPI_1BYT_SetTx_Isr(RFIP_REG_62, i);                     //Read and clear interrupt status, 2/2
        InterruptEnable();

        if((i&(INT_SETTING_R&(~REG_62_WAKEUP))))
        {
            if((i&REG_62_TX_END))
            {
                rafael_spi_clear_tx_fifo_cnt();
            }

            InterruptDisable();
            LLWTR_Isr();
            InterruptEnable();
        }
        if((i&REG_62_WAKEUP))
        {
            InterruptDisable();
            LLTimer_Isr();
            InterruptEnable();
        }
        SPI_PDMA_waitFinish();
    }
    //#endif  //(#ifdef _SW_PATCH_MP_)
}
#pragma pop
