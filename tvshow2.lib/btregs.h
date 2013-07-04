/*
** Module   :BTREGS.H
** Abstract : brooktree registers
**
** Copyright (C) Alex Cherkaev
**
** Log: Tue  15/01/2002 Created
**
*/
#ifndef __BTREGS_H
#define __BTREGS_H

#if __cplusplus
extern "C" {
#endif



#define DSTATUS       0x000

#define BT848_DSTATUS_PRES		(1<<7)
#define BT848_DSTATUS_HLOC		(1<<6)
#define BT848_DSTATUS_FIELD		(1<<5)
#define BT848_DSTATUS_NUML		(1<<4)
#define BT848_DSTATUS_CSEL		(1<<3)
#define BT848_DSTATUS_PLOCK		(1<<2)
#define BT848_DSTATUS_LOF		(1<<1)
#define BT848_DSTATUS_COF       (1<<0)

#define IFORM         0x004

#define BT848_IFORM_MUXSEL      (0x3<<5)
# define BT848_IFORM_M_MUX1     (0x03<<5)
# define BT848_IFORM_M_MUX0     (0x02<<5)
# define BT848_IFORM_M_MUX2     (0x01<<5)
# define BT848_IFORM_M_MUX3		(0x0)
# define BT848_IFORM_M_RSVD     (0x00<<5)
#define BT848_IFORM_XTSEL       (0x3<<3)
# define BT848_IFORM_X_AUTO     (0x03<<3)
# define BT848_IFORM_X_XT1      (0x02<<3)
# define BT848_IFORM_X_XT0      (0x01<<3)
# define BT848_IFORM_X_RSVD     (0x00<<3)
#define BT878_IFORM_FORMAT      (0x7)
# define BT878_IFORM_F_AUTO     (0x0)
# define BT878_IFORM_F_NTSCM    (0x1)
# define BT878_IFORM_F_NTSCJ    (0x2)
# define BT878_IFORM_F_PAL      (0x3)
# define BT878_IFORM_F_PALM     (0x4)
# define BT878_IFORM_F_PALN     (0x5)
# define BT878_IFORM_F_SECAM    (0x6)
# define BT878_IFORM_F_PALNC    (0x7)

#define TDEC          0x008
#define E_CROP        0x00c
#define O_CROP        0x08c
/* error in pdf? */
#define E_VDELAY_LO   0x090
#define O_VDELAY_LO   0x010

#define E_VACTIVE_LO  0x014
#define O_VACTIVE_LO  0x094
#define E_HDELAY_LO   0x018
#define O_HDELAY_LO   0x098
#define E_HACTIVE_LO  0x01c
#define O_HACTIVE_LO  0x09c
#define E_HSCALE_HI   0x020
#define O_HSCALE_HI   0x0a0
#define E_HSCALE_LO   0x024
#define O_HSCALE_LO   0x0a4
#define BRIGHT        0x028
#define E_CONTROL     0x02c

#define BT848_E_CONTROL_LNOTCH      (1<<7)
#define BT848_E_CONTROL_COMP        (1<<6)
#define BT848_E_CONTROL_LDEC        (1<<5)
#define BT848_E_CONTROL_CBSENSE     (1<<4)
#define BT848_E_CONTROL_RSVD        (1<<3)
#define BT848_E_CONTROL_CON_MSB     (1<<2)
#define BT848_E_CONTROL_SAT_U_MSB   (1<<1)
#define BT848_E_CONTROL_SAT_V_MSB   (1<<0)

#define O_CONTROL     0x0ac

#define BT848_O_CONTROL_LNOTCH      (1<<7)
#define BT848_O_CONTROL_COMP        (1<<6)
#define BT848_O_CONTROL_LDEC        (1<<5)
#define BT848_O_CONTROL_CBSENSE     (1<<4)
#define BT848_O_CONTROL_RSVD        (1<<3)
#define BT848_O_CONTROL_CON_MSB     (1<<2)
#define BT848_O_CONTROL_SAT_U_MSB   (1<<1)
#define BT848_O_CONTROL_SAT_V_MSB   (1<<0)

#define CONTRAST_LO   0x030
#define SAT_U_LO      0x034
#define SAT_V_LO      0x038
#define HUE           0x03c
#define E_SCLOOP      0x040

#define BT848_E_SCLOOP_RSVD1        (1<<7)
#define BT848_E_SCLOOP_CAGC     (1<<6)
#define BT848_E_SCLOOP_CKILL        (1<<5)
#define BT848_E_SCLOOP_HFILT        (0x3<<3)
# define BT848_E_SCLOOP_HFILT_ICON  (0x3<<3)
# define BT848_E_SCLOOP_HFILT_QCIF  (0x2<<3)
# define BT848_E_SCLOOP_HFILT_CIF   (0x1<<3)
# define BT848_E_SCLOOP_HFILT_AUTO  (0x0<<3)
#define BT848_E_SCLOOP_RSVD0        (0x7<<0)

#define O_SCLOOP      0x0c0

#define BT848_O_SCLOOP_RSVD1        (1<<7)
#define BT848_O_SCLOOP_CAGC     (1<<6)
#define BT848_O_SCLOOP_CKILL        (1<<5)
#define BT848_O_SCLOOP_HFILT        (0x3<<3)
#define BT848_O_SCLOOP_HFILT_ICON   (0x3<<3)
#define BT848_O_SCLOOP_HFILT_QCIF   (0x2<<3)
#define BT848_O_SCLOOP_HFILT_CIF    (0x1<<3)
#define BT848_O_SCLOOP_HFILT_AUTO   (0x0<<3)
#define BT848_O_SCLOOP_RSVD0        (0x7<<0)

#define WC_UP         0x044
#define OFORM         0x048
#define E_VSCALE_HI   0x04c
#define O_VSCALE_HI   0x0cc
#define E_VSCALE_LO   0x050
#define O_VSCALE_LO   0x0d0
#define TEST          0x054
#define ADELAY        0x060
#define BDELAY        0x064
#define ADC           0x068

#define BT848_ADC_RESERVED		(0x80)	/* required pattern */
#define BT848_ADC_SYNC_T        (1<<5)
#define BT848_ADC_AGC_EN        (1<<4)
#define BT848_ADC_CLK_SLEEP     (1<<3)
#define BT848_ADC_Y_SLEEP       (1<<2)
#define BT848_ADC_C_SLEEP       (1<<1)
#define BT848_ADC_CRUSH         (1<<0)

#define E_VTC         0x06c
#define O_VTC         0x0ec
#define SRESET        0x07c
#define WCD           0x078
#define TGLB          0x080
#define TGCTRL        0x084

#define BT848_TGCTRL_TGCKI      (3<<3)
#define BT848_TGCTRL_TGCKI_XTAL     (0<<3)
#define BT848_TGCTRL_TGCKI_PLL      (1<<3)
#define BT848_TGCTRL_TGCKI_GPCLK    (2<<3)
#define BT848_TGCTRL_TGCKI_GPCLK_I  (3<<3)

#define VTOTAL_LO     0x0b0
#define VTOTAL_HI     0x0b4
#define COLOR_FMT     0x0d4
# define BT_COLOR_RGB32 (0x0)
# define BT_COLOR_RGB24 (0x1)
# define BT_COLOR_RGB16 (0x2)
# define BT_COLOR_RGB15 (0x3)
# define BT_COLOR_YUV2422 (0x4)
# define BT_COLOR_BtYUV411 (0x5)
# define BT_COLOR_GS (0x6)
# define BT_COLOR_RGB8 (0x7)
# define BT_COLOR_YCrCb422 (0x8)
# define BT_COLOR_YCrCb411 (0x9)
# define BT_COLOR_RAW (0xe)
#define COLOR_CTL     0x0d8

#define BT848_COLOR_CTL_WSWAP_ODD   (1<<3)
#define BT848_COLOR_CTL_WSWAP_EVEN  (1<<2)
#define BT848_COLOR_CTL_BSWAP_ODD   (1<<1)
#define BT848_COLOR_CTL_BSWAP_EVEN  (1<<0)
#define BT848_COLOR_CTL_GAMMA       (1<<4)
#define BT848_COLOR_CTL_RGB_DED     (1<<5)
#define BT848_COLOR_CTL_COLOR_BARS  (1<<6)
#define BT848_COLOR_CTL_EXT_FRMRATE     (1<<7)


#define CAP_CTL       0x0dc

#define BT848_CAP_CTL_DITH_FRAME    (1<<4)
#define BT848_CAP_CTL_VBI_ODD       (1<<3)
#define BT848_CAP_CTL_VBI_EVEN      (1<<2)
#define BT848_CAP_CTL_ODD       (1<<1)
#define BT848_CAP_CTL_EVEN      (1<<0)

#define VBI_PACK_SIZE 0x0e0
#define VBI_PACK_DEL  0x0e4
#define FCAP          0x0e8
#define PLL_F_LO      0x0f0
#define PLL_F_HI      0x0f4
#define PLL_XCI       0x0f8

#define BT848_PLL_F_C           (1<<6)
#define BT848_PLL_F_X           (1<<7)

#define DVSIF         0x0fc
#define INT_STAT      0x100
#define INT_MASK      0x104

#define BT848_INT_RISCS         (0xf<<28)
#define BT848_INT_RISC_EN       (1<<27)
#define BT848_INT_RACK          (1<<25)
#define BT848_INT_FIELD         (1<<24)
#define BT848_INT_MYSTERYBIT        (1<<23)
#define BT848_INT_SCERR         (1<<19)
#define BT848_INT_OCERR         (1<<18)
#define BT848_INT_PABORT        (1<<17)
#define BT848_INT_RIPERR        (1<<16)
#define BT848_INT_PPERR         (1<<15)
#define BT848_INT_FDSR          (1<<14)
#define BT848_INT_FTRGT         (1<<13)
#define BT848_INT_FBUS          (1<<12)
#define BT848_INT_RISCI         (1<<11)
#define BT848_INT_GPINT         (1<<9)
#define BT848_INT_I2CDONE       (1<<8)
#define BT848_INT_RSV1          (1<<7)
#define BT848_INT_RSV0          (1<<6)
#define BT848_INT_VPRES         (1<<5)
#define BT848_INT_HLOCK         (1<<4)
#define BT848_INT_OFLOW         (1<<3)
#define BT848_INT_HSYNC         (1<<2)
#define BT848_INT_VSYNC         (1<<1)
#define BT848_INT_FMTCHG        (1<<0)

#define GPIO_DMA_CTL  0x10c

#define BT848_DMA_CTL_PL23TP4       (0<<6)  /* planar1 trigger 4 */
#define BT848_DMA_CTL_PL23TP8       (1<<6)  /* planar1 trigger 8 */
#define BT848_DMA_CTL_PL23TP16      (2<<6)  /* planar1 trigger 16 */
#define BT848_DMA_CTL_PL23TP32      (3<<6)  /* planar1 trigger 32 */
#define BT848_DMA_CTL_PL1TP4        (0<<4)  /* planar1 trigger 4 */
#define BT848_DMA_CTL_PL1TP8        (1<<4)  /* planar1 trigger 8 */
#define BT848_DMA_CTL_PL1TP16       (2<<4)  /* planar1 trigger 16 */
#define BT848_DMA_CTL_PL1TP32       (3<<4)  /* planar1 trigger 32 */
#define BT848_DMA_CTL_PKTP4     (0<<2)  /* packed trigger 4 */
#define BT848_DMA_CTL_PKTP8     (1<<2)  /* packed trigger 8 */
#define BT848_DMA_CTL_PKTP16        (2<<2)  /* packed trigger 16 */
#define BT848_DMA_CTL_PKTP32        (3<<2)  /* packed trigger 32 */
#define BT848_DMA_CTL_RISC_EN       (1<<1)
#define BT848_DMA_CTL_FIFO_EN       (1<<0)

#define I2C_DATA_CTL  0x110

#define BT848_DATA_CTL_I2CDB0       (0xff<<24)
#define BT848_DATA_CTL_I2CDB1       (0xff<<16)
#define BT848_DATA_CTL_I2CDB2       (0xff<<8)
#define BT848_DATA_CTL_I2CMODE      (0x1<<7)
#define BT848_DATA_CTL_I2CRATE      (0x1<<6)
#define BT848_DATA_CTL_I2CNOSTOP    (0x1<<5)
#define BT848_DATA_CTL_I2CNOS1B     (0x1<<4)
#define BT848_DATA_CTL_I2CDIV       (0xf<<4)
#define BT848_DATA_CTL_I2CSYNC      (1<<3)
#define BT848_DATA_CTL_I2CW3BRA     (1<<2)
#define BT848_DATA_CTL_I2CSCL       (1<<1)
#define BT848_DATA_CTL_I2CSDA       (1<<0)

#define RISC_STRT_ADD 0x114
#define GPIO_OUT_EN   0x118
#define RISC_COUNT    0x120
#define GPIO_DATA     0x200


#if __cplusplus
}
#endif


#endif  /*__BTREGS_H*/

