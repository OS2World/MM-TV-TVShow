#include <os2.h>
//**************************************************************************
// window IDs - used to construct windows, e.g. IStaticText, AHelloWindow  *
//**************************************************************************
#define WND_MAIN           4096         //Main window window ID

#define WND_CANVAS        32776         //Client window ID
#define WND_OSD           32777         //OSD window ID
#define WND_INFO           4114         //Information area window ID


//**************************************************************************
// String IDs - used to relate resources to IStaticText and ITitle         *
//**************************************************************************
#define STR_HELLO          4608         //Hello World string ID
#define STR_NONE           4609
#define STR_INFO           4640         //Inactive text string ID

//**************************************************************************
// Menu IDs - used to relate command ID to menu items, buttons, and keys   *
//**************************************************************************
#define MI_OSD       5376         //OSD menu item command ID      V3
#define MI_ON          5377         //OSD On menu item command ID         V3
#define MI_OFF            5378         //OSD Off menu item command ID           V3
#define MI_TEST            5379         //TEST menu item command ID           V3

#define MI_HELP 5380
#define IDM_HELP 5382
#define IDM_KEYSHELP 5384
#define IDM_PRODUCTINFORMATION 5385
#define IDM_TEST 5379
#define IDM_HWVIDEOTEST 5390
#define IDM_CHANNELS 5391
#define IDM_PREVISIOUSCHANNEL 5392
#define IDM_NEXTCHANNEL 5393
#define IDM_LOADCHANNELLIST 5394
#define IDM_SAVECHANNELLIST 5395






#define IDC_DECBRIGHT 6000
#define IDC_INCBRIGHT 6001
#define IDC_DECCONTRAST 6010
#define IDC_INCCONTRAST 6011
#define IDC_DECSATURATION 6020
#define IDC_INCSATURATION 6021
#define IDM_VIDEO 6024
#define IDM_BRIGHTNESS 6026
#define IDM_CONTRAST 6027
#define IDM_SATURATION 6028
#define DLG_VIDEOCONTROL 6034
#define SL_BRIGHTNESS 6036
#define SL_CONTRAST 6037
#define SL_SATURATION 6041
#define IDM_SETTINGS 6042
#define SL_VOLUME 6046
#define RB_NTSC_M 6049
#define RB_NTSCJAPAN 6050
#define RB_PAL 6051
#define RB_PALM 6052
#define RB_SECAM 6054
#define RB_PALN 6053
#define RB_PALNCOMBINATION 6055
#define EF_CHANNELNAME 6057
#define DLG_SCANNING 32777
#define PB_SCAN 32779
#define EF_CHANNELS 32782
#define IDM_SCANCHANNELS 32783
#define PI_SCANNING 32784
#define EF_BAND 32786
#define RB_SLOW 32788
#define RB_FAST 32789
#define CX_REGION 32790
#define EF_FREQUENCY 32791
#define ST_FREQUENCY 32792
#define PB_FREQDEC 32794
#define PB_FREQINC 32795
#define IDM_INPUT 32807
#define IDM_TUNER 32797
#define IDM_LINEIN 32798
#define IDM_SVHS 32799
#define IDM_EXT 32800
#define IDM_AUDIOINPUT0 32802
#define IDM_AUDIOINPUT1 32803
#define IDM_AUDIOINPUT2 32804
#define IDM_AUDIOINPUT3 32805
#define MI_AUIDO 32806
#define IDM_AUIDO 32806
#define IDM_AUDIO 32806
#define IDC_TUNER 32797
#define IDC_LINEIN 32798
#define IDC_SVHS 32799
#define IDC_EXT 32800
#define IDM_VOLUME 32808
#define IDC_INCVOLUME 32809
#define IDM_DECVOLUME 32810
#define RB_SOUNDMFM 32813
#define RB_SOUNDBGFM 32814
#define RB_SOUNDDKFM 32816
#define RB_SOUNDBGNICAM 32818
#define RB_SOUNDLNICAM 32819
#define RB_INICAM 32820
#define RB_SOUNDDKNICAM 32821
#define RB_SOUNDINICAM 32820
#define DLG_AUDIOCONTROL 32811
#define IDM_AUDIOSETTING 32822
#define RB_SOUNDDK1FM 32816
#define RB_SOUNDDK2FM 32817
#define IDM_SOUNDMODE 32823
#define IDM_BGNICAM 32824
#define IDM_DKNICAM 32825
#define IDM_LNICAM 32826
#define IDM_I_NICAM 32827
#define IDM_MFM 32829
#define IDM_BGFM 32830
#define IDM_DK1FM 32831
#define IDM_DK2FM 32832
#define IDM_INICAM 32827
#define IDM_MSPSTATE 32833
#define IDM_FULLSCREEN 32834
#define IDM_COLORBARS 32835
#define ST_OSDPROGRAM 32836
#define IDM_SEINTERLACE 32837
#define IDM_DEINTERLACE 32837
#define IDM_DEINTOFF 32838
#define IDM_DEINTBOB 32839
#define IDM_DEINTWEAVE 32840
#define IDM_RECORD 32841
#define IDM_RECSTREAM 32842
#define IDM_RECFRAME 32843
#define IDM_RECOPTIONS 32844
