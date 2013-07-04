#if __cplusplus
extern "C" {
#endif

typedef struct {
    ULONG windowwidth;
    ULONG windowheight;
    ULONG videowidth;
    ULONG videoheight;
    short connector;
    short blittermode;
    short deinterlace;
    ULONG colorkey;
    short colorsystem;
    short contrast;
    short brightness;
    short saturation;
    short volume;
    BOOL  iscolor;
    short cardtype;
    short tunertype;
    short loglevel;
    char logname[255];
} CFGDATA;
typedef CFGDATA* PCFGDATA;
void TVParseConfig(PCFGDATA pconfig);

#if __cplusplus
}
#endif


