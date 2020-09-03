/**************************************************************************//**
* @file       _rafael_phy.h
* @brief
*
* @copyright  All rights reserved. \n
*             (C) 2020 Rafael Microelectronics Co. Ltd. Taiwan, R.O.C.
*
*
*****************************************************************************/

#ifndef ___RAFAEL_PHY_H__
#define ___RAFAEL_PHY_H__


#define NUM_BLE_CH      40

//========= Radio ===========/
#define RADIO_TOTAL_REG_NUM      128
#define RADIO_RF_INIT_REG_NUM    39  //R8~R46   
#define RADIO_BB_INIT_REG_NUM    81  //R47~R127 


//========= RF PHY options ===========/
#define CRC_IN_RX_FIFO                           0    //0: not put CRC/MIC in RX_FIFO;   1: put CRC/MIC in RX_FIFO
#define ENABLE_VCO_CAL                           1    //0: PLL hybrid/binary mode;  1:PLL linear mode & VCO calibration
#define ENABLE_AUTOMATIC_SWITCH_MODE             1    //Auto TX/RX switch
#define ENABLE_TX_PAYLOAD_CHECK                  1    //TX payload check
#define ENABLE_RF_VOLT_DETECT                    1    //RF Voltage detect
#define ENABLE_RF_RSSI                           1    //RF RSSI calculation

#define MAX_PAYLOAD_LEN                          255  //max is 255(include MIC). If (received header[1]<MAX_PAYLOAD_LEN), CRC_err!
//#define MAX_PAYLOAD_LEN                          37



/****************************************************************************\
 * AES MODULE CONSTANTS definitions
\****************************************************************************/
#define RAFAEL_AES_MODE_BYPASS              (0u)
#define RAFAEL_AES_MODE_CCM_ENABLE          (1u)
#define RAFAEL_AES_MODE_CCM_TX_ENCRY        (2u)
#define RAFAEL_AES_MODE_CCM_RX_DECRY        (3u)
#define RAFAEL_AES_MODE_RESERVED            (4u)  //4, 5 is reserved
#define RAFAEL_AES_MODE_RESERVED2           (5u)
#define RAFAEL_AES_MODE_LOCAL_AES_ECB       (6u)
#define RAFAEL_FIFO_LOOP_BACK               (7u)



/****************************************************************************\
 * Register definitions
\****************************************************************************/
// RFIP_REG_254   (Data Write Port)
// RFIP_REG_255   (Data Read Port)
// RFIP_REG_254 and RFIP_REG_255 are defined in rf_phy.h

#define RFIP_REG_0              0U      //chip id and external wakeup RF by write R0
#define RFIP_REG_1              1U
#define RFIP_REG_4              4U
#define RFIP_REG_8              8U
#define RFIP_REG_12             12U     //VCO bank
#define RFIP_REG_23             23U
#define RFIP_REG_28             28U
#define RFIP_REG_36             36U     //PLL mode
#define RFIP_REG_40             40U     //switch BUCK,LDO
#define RFIP_REG_41             41U     //41, 42 adjust current for power down.
#define RFIP_REG_42             42U
#define RFIP_REG_43             43U     //FPGA use
#define RFIP_REG_45             45U
#define RFIP_REG_47             47U
#define RFIP_REG_57             57U
#define RFIP_REG_61             61U     //INT enable mask
#define RFIP_REG_62             62U     //INT status
#define RFIP_REG_74             74U
#define RFIP_REG_88             88U     //PLL manual mode or hybrid(auto)
#define RFIP_REG_91             91U
#define RFIP_REG_94             94U
#define RFIP_REG_98             98U
#define RFIP_REG_102            102U    //T3: B7 is update RTC counter
#define RFIP_REG_103            103U    //T2: B7 is update RTC counter (no use in T3)
#define RFIP_REG_104            104U    //T3: HW_send_TX_buffer_start_addr[7:0]             T2: TX_FIFO_Cnt
#define RFIP_REG_105            105U    //T3: HW_send_TX_buffer_start_addr[8] is B0         T2: TX_FIFO_Cnt & TX_FIFO_Clear 
#define RFIP_REG_106            106U
#define RFIP_REG_107            107U    //B7 is RX_FIFO_Clear, B6 is TX_BUFFER_Clear
#define RFIP_REG_112            112U    //R112~R114: CRC_Init
#define RFIP_REG_115            115U
#define RFIP_REG_119            119U
#define RFIP_REG_120            120U    //See REG_120 definition
#define RFIP_REG_121            121U    //B5~0:channel_idx, B6:first T/R, B7:reset MAC
#define RFIP_REG_122            122U    //R122~R125:Access Address
#define RFIP_REG_126            126U    //R126~R138:AES_CCM Nonce
#define RFIP_REG_131            131U
#define RFIP_REG_139            139U    //R139~R154:AES_CCM Key
#define RFIP_REG_155            155U    //AES configure
#define RFIP_REG_156            156U    //R156~R159:period RTC, R159[6]:1 is period; 0 is one-shot(see R115~R119)
#define RFIP_REG_159            159U
#define RFIP_REG_165            165U
#define RFIP_REG_166            166U
#define RFIP_REG_172            172U
#define RFIP_REG_175            175U
#define RFIP_REG_250            250U



//if add this part, also modify RFIP_REG_IDX[SIZE_RFIP_REG]
enum
{
    RFIP_REG_MEM_23 = 0U,
    RFIP_REG_MEM_36,
    RFIP_REG_MEM_40,
    RFIP_REG_MEM_41,
    RFIP_REG_MEM_42,
    RFIP_REG_MEM_43,
    RFIP_REG_MEM_57,
    RFIP_REG_MEM_61,
    RFIP_REG_MEM_105,
    RFIP_REG_MEM_107,
    RFIP_REG_MEM_119,
    RFIP_REG_MEM_120,
    RFIP_REG_MEM_121,
    RFIP_REG_MEM_166,
    SIZE_RFIP_REG
};




#define REG_61_HEADER_RDY       0x01
#define REG_61_BLOCK_RDY        0x02
#define REG_61_TX_END           0x04
#define REG_61_RX_TMROUT        0x08
#define REG_61_RX_END           0x10
#define REG_61_ACCESS_MATCH     0x20
#define REG_61_WAKEUP           0x40
#define REG_62_HEADER_RDY       0x01
#define REG_62_BLOCK_RDY        0x02
#define REG_62_TX_END           0x04
#define REG_62_RX_TMROUT        0x08
#define REG_62_RX_END           0x10
#define REG_62_ACCESS_MATCH     0x20
#define REG_62_WAKEUP           0x40
#define REG_74_EDGE             0x08   //1:GPIO edge trigger mode   
#define REG_96_WHITEN_EN        0x08

#define INT_SETTING_R           (REG_62_TX_END|REG_62_RX_END|REG_62_WAKEUP)
#define INT_SETTING_W           (REG_61_TX_END|REG_61_RX_END|REG_61_WAKEUP)
#define INT_SETTING_W_WTR       (REG_61_TX_END|REG_61_RX_END)
#define INT_SETTING_W_TMR       (REG_61_WAKEUP)

#define REG_107_CRC_OK          0x04   //0:CRC fail            1:CRC ok     
#define REG_107_MIC_OK          0x02   //0:MIC fail            1:MIC ok     

#define REG_119_WAKE_TMR_US     0x0F   //RTC[36:32]                                               
#define REG_119_TR_TRIG_MODE    0x40   //0:T/R trigger manual  1:T/R auto trigger when RTC match 
#define REG_119_MANUAL_TRX_EN   0x80   //0:                    1:manual enable T/R(HW auto clear) 

#define REG_120_WAKEUP_MODE     0x02   //0:RTC;                1:external SPI                 
#define REG_120_AUTO_TRX        0x04   //0:disable             1:enable                       
#define REG_120_DATA_RATE       0x08   //0:1M                  1:2M                           
#define REG_120_PLL_CONF_DIRECT 0x10   //0:PLL by R121         1:PLL by R32~R34               
#define REG_120_RX_TMROUT_EN    0x20   //0:disable             1:enable                       
#define REG_120_CRC_IN_FIFO     0x40   //0:CRC/MIC in RX_FIFO  1:CRC/MIC not put in RX_FIFO   
#define REG_120_UPD_WAKE_TMR    0x80   //0:not Sleep in RTC    1:Sleep in using RTC           

#define REG_121_MSK_CHN_IDX     0x3F
#define REG_121_T_R_SEL_WKP     0x40
#define REG_121_RST_STATE       0x80

#define REG_155_AES_MODE        0x07   //0:bypass, 1:AES-CCM Encr./Decr., 2:AES-CCM Encr., 3:AES-CCM Decr., 4:AES-128mode 
#define REG_155_AES128_ENABLE   0x08   //0:                    1:enable AES-128 
#define REG_155_MIC_OK          0x10   //0:MIC fail            1:MIC ok         
#define REG_155_CRC_OK          0x20   //0:CRC fail            1:CRC ok         
#define REG_155_AES_BUSY        0x40   //0:                    1:AES HW busy    

#define REG_155_AES_MODE_BYPASS             (0u)
#define REG_155_AES_MODE_CCM_ENABLE         (1u)
#define REG_155_AES_MODE_CCM_TX_ENCRY       (2u)
#define REG_155_AES_MODE_CCM_RX_DECRY       (3u)
#define REG_155_AES_MODE_LOCAL_AES_ECB      (6u)
#define REG_155_FIFO_LOOP_BACK              (7u)
#define REG_159_PERIOD_INT                  0x80


/****************************************************************************\
 * Defined Function
\****************************************************************************/

#define RF_CCM_RxDecrResultChk()    ((SPI_1BYT_SetRx(RFIP_REG_155)&REG_155_MIC_OK))       //0: Fail, No 
//extern void RF_CCM_NonceIV_Key16_set(uint8_t *addrIV);
//extern void RF_CCM_NoncePktCnt_set(uint8_t *addrPktCnt);
#define RF_CCM_NonceIV_Key16_set(addrIV)        SPI_PDMA_SetTx(RFIP_REG_131, (uint32_t)addrIV, LEN_SMP_IVS+LEN_SMP_IVM+LEN_AES_KEY+1)
#define RF_CCM_NoncePktCnt_set(addrPktCnt)      SPI_PDMA_SetTx(RFIP_REG_126, (uint32_t)&addrPktCnt, LEN_SMP_PKTCNT+1)



/****************************************************************************\
 * Enum
\****************************************************************************/
enum
{
    RF_SLEEP_DISABLE = 0U,
    RF_SLEEP_ENABLE = 1U
};



/****************************************************************************\
 * Extern Variable
\****************************************************************************/
extern volatile uint8_t RFIP_reg_MEM[SIZE_RFIP_REG];

/****************************************************************************\
 * Extern Function
\****************************************************************************/



// used in LL.c
extern uint8_t CH_PLL_bank_Table[NUM_BLE_CH];
extern void RF_PLL_VCO_Bank_set(uint8_t valueVCO);
extern uint8_t RF_CRCchk(void);
extern uint8_t RF_WTR_EnChk(void);
extern void RF_RxFIFOrst(void);

extern void rafael_reset_phy_fsm_Isr(void);
extern void setChannel_BLE(uint8_t ch);
extern void RF_WTR_intOff(void);
extern void RF_TxAutoAckOn(void);
extern void RF_TxAutoAckOff(void);
extern void RF_SymbolRate_set(uint8_t zSymbol_1M);
extern void RF_CRCInit(uint8_t *initParaAddr);
extern void RF_TxFIFO_ADVaddr_set(uint8_t *AdvAddr);
extern void RF_TxFIFO_ADVData_set(uint8_t *SrcAddr);
extern void RF_TxFIFO_LeData_set(uint8_t *SrcAddr, uint8_t length);
extern void RF_LE_HeaderStsLen_Tx(uint8_t *SrcAddr);

extern void RF_CCM_AES_Mode_set(uint8_t setting);
extern void RF_Tmr_Periodic_set_ISR(uint16_t period_tick);

extern void RF_PowerSaving_En_Isr(void);
extern int8_t RF_Get_LastRssi(void);
extern void RF_SymbolRate_Patch_1M_2M(uint8_t zSymbol_1M);
extern uint8_t RF_Set_TxPowerLevel_Isr(int8_t power);

extern void RF_WTR_intOn(void);
extern void rafael_spi_clear_tx_fifo_cnt(void);




#endif // ___RAFAEL_PHY_H__


