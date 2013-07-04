/*
** Module   :CHANSET.CPP
** Abstract :
**
** Copyright (C) Alex Cherkaev
**
** Log: Thu  20/02/2003 Created
**
*/

/*
 * Format:
 *  entry 0:         MAX legal channel
 *  entry 1:         IF frequency
 *           expressed as Hz
 *  entry 2:         [place holder/future]
 *  entry 3:         base of channel record 0
 *  entry 3 + (x*3): base of channel record 'x'
 *  entry LAST:      NULL channel entry marking end of records
 *
 * Record:
 *  int 0:      base channel
 *  int 1:      frequency of base channel,
 *           expressed as Hz
 *  int 2:      offset frequency between channels,
 *           expressed as Hz
 */

#define FREQFACTOR 1000000
CHANSET[CHANSET_TOTAL] chansets;

/*
 * North American Broadcast Channels:
 *
 *  2:  55.25 mHz -  4:  67.25 mHz
 *  5:  77.25 mHz -  6:  83.25 mHz
 *  7: 175.25 mHz - 13: 211.25 mHz
 * 14: 471.25 mHz - 83: 885.25 mHz
 *
 * IF freq: 45.75 mHz
 */
#define OFFSET  6.00
#define IF_FREQ 45.75
static ULONG nabcst[] = {
    83, (ULONG)( IF_FREQ * FREQFACTOR), 0,
    14, (ULONG)(471.25 * FREQFACTOR), (ULONG)(OFFSET * FREQFACTOR),
     7, (ULONG)(175.25 * FREQFACTOR), (ULONG)(OFFSET * FREQFACTOR),
     5, (ULONG)( 77.25 * FREQFACTOR), (ULONG)(OFFSET * FREQFACTOR),
     2, (ULONG)( 55.25 * FREQFACTOR), (ULONG)(OFFSET * FREQFACTOR),
     0
};
#undef OFFSET
#undef IF_FREQ
/*
 * North American Cable Channels, IRC:
 *
 *  2:  55.25 mHz -  4:  67.25 mHz
 *  5:  77.25 mHz -  6:  83.25 mHz
 *  7: 175.25 mHz - 13: 211.25 mHz
 * 14: 121.25 mHz - 22: 169.25 mHz
 * 23: 217.25 mHz - 94: 643.25 mHz
 * 95:  91.25 mHz - 99: 115.25 mHz
 *
 * IF freq: 45.75 mHz
 */
#define OFFSET  6.00
#define IF_FREQ 45.75
static ULONG irccable[] = {
    116,    (ULONG)( IF_FREQ * FREQFACTOR),     0,
    100,    (ULONG)(649.25 * FREQFACTOR),     (ULONG)(OFFSET * FREQFACTOR),
    95, (ULONG)( 91.25 * FREQFACTOR), (ULONG)(OFFSET * FREQFACTOR),
    23, (ULONG)(217.25 * FREQFACTOR), (ULONG)(OFFSET * FREQFACTOR),
    14, (ULONG)(121.25 * FREQFACTOR), (ULONG)(OFFSET * FREQFACTOR),
     7, (ULONG)(175.25 * FREQFACTOR), (ULONG)(OFFSET * FREQFACTOR),
     5, (ULONG)( 77.25 * FREQFACTOR), (ULONG)(OFFSET * FREQFACTOR),
     2, (ULONG)( 55.25 * FREQFACTOR), (ULONG)(OFFSET * FREQFACTOR),
     0
};
#undef OFFSET
#undef IF_FREQ
/*
 * North American Cable Channels, HRC:
 *
 * 2:   54 mHz  - 4:    66 mHz
 * 5:   78 mHz  - 6:    84 mHz
 * 7:  174 mHz  - 13:  210 mHz
 * 14: 120 mHz  - 22:  168 mHz
 * 23: 216 mHz  - 94:  642 mHz
 * 95:  90 mHz  - 99:  114 mHz
 *
 * IF freq: 45.75 mHz
 */
#define OFFSET  6.00
#define IF_FREQ 45.75
static ULONG hrccable[] = {
    116,    (ULONG)( IF_FREQ * FREQFACTOR),     0,
    100,    (ULONG)(648.00 * FREQFACTOR),     (ULONG)(OFFSET * FREQFACTOR),
    95, (ULONG)( 90.00 * FREQFACTOR), (ULONG)(OFFSET * FREQFACTOR),
    23, (ULONG)(216.00 * FREQFACTOR), (ULONG)(OFFSET * FREQFACTOR),
    14, (ULONG)(120.00 * FREQFACTOR), (ULONG)(OFFSET * FREQFACTOR),
    7,  (ULONG)(174.00 * FREQFACTOR), (ULONG)(OFFSET * FREQFACTOR),
    5,  (ULONG)( 78.00 * FREQFACTOR), (ULONG)(OFFSET * FREQFACTOR),
    2,  (ULONG)( 54.00 * FREQFACTOR), (ULONG)(OFFSET * FREQFACTOR),
    0
};
#undef OFFSET
#undef IF_FREQ
/*
 * Western European broadcast channels:
 *
 * (there are others that appear to vary between countries - rmt)
 *
 * here's the table Philips provides:
 * caution, some of the offsets don't compute...
 *
 *  1    4525   700 N21
 *
 *  2    4825   700 E2
 *  3    5525   700 E3
 *  4    6225   700 E4
 *
 *  5   17525   700 E5
 *  6   18225   700 E6
 *  7   18925   700 E7
 *  8   19625   700 E8
 *  9   20325   700 E9
 * 10   21025   700 E10
 * 11   21725   700 E11
 * 12   22425   700 E12
 *
 * 13    5375   700 ITA
 * 14    6225   700 ITB
 *
 * 15    8225   700 ITC
 *
 * 16   17525   700 ITD
 * 17   18325   700 ITE
 *
 * 18   19225   700 ITF
 * 19   20125   700 ITG
 * 20   21025   700 ITH
 *
 * 21   47125   800 E21
 * 22   47925   800 E22
 * 23   48725   800 E23
 * 24   49525   800 E24
 * 25   50325   800 E25
 * 26   51125   800 E26
 * 27   51925   800 E27
 * 28   52725   800 E28
 * 29   53525   800 E29
 * 30   54325   800 E30
 * 31   55125   800 E31
 * 32   55925   800 E32
 * 33   56725   800 E33
 * 34   57525   800 E34
 * 35   58325   800 E35
 * 36   59125   800 E36
 * 37   59925   800 E37
 * 38   60725   800 E38
 * 39   61525   800 E39
 * 40   62325   800 E40
 * 41   63125   800 E41
 * 42   63925   800 E42
 * 43   64725   800 E43
 * 44   65525   800 E44
 * 45   66325   800 E45
 * 46   67125   800 E46
 * 47   67925   800 E47
 * 48   68725   800 E48
 * 49   69525   800 E49
 * 50   70325   800 E50
 * 51   71125   800 E51
 * 52   71925   800 E52
 * 53   72725   800 E53
 * 54   73525   800 E54
 * 55   74325   800 E55
 * 56   75125   800 E56
 * 57   75925   800 E57
 * 58   76725   800 E58
 * 59   77525   800 E59
 * 60   78325   800 E60
 * 61   79125   800 E61
 * 62   79925   800 E62
 * 63   80725   800 E63
 * 64   81525   800 E64
 * 65   82325   800 E65
 * 66   83125   800 E66
 * 67   83925   800 E67
 * 68   84725   800 E68
 * 69   85525   800 E69
 *
 * 70    4575   800 IA
 * 71    5375   800 IB
 * 72    6175   800 IC
 *
 * 74    6925   700 S01
 * 75    7625   700 S02
 * 76    8325   700 S03
 *
 * 80   10525   700 S1
 * 81   11225   700 S2
 * 82   11925   700 S3
 * 83   12625   700 S4
 * 84   13325   700 S5
 * 85   14025   700 S6
 * 86   14725   700 S7
 * 87   15425   700 S8
 * 88   16125   700 S9
 * 89   16825   700 S10
 * 90   23125   700 S11
 * 91   23825   700 S12
 * 92   24525   700 S13
 * 93   25225   700 S14
 * 94   25925   700 S15
 * 95   26625   700 S16
 * 96   27325   700 S17
 * 97   28025   700 S18
 * 98   28725   700 S19
 * 99   29425   700 S20
 *
 *
 * Channels S21 - S41 are taken from
 * http://gemma.apple.com:80/dev/technotes/tn/tn1012.html
 *
 * 100  30325   800 S21
 * 101  31125   800 S22
 * 102  31925   800 S23
 * 103  32725   800 S24
 * 104  33525   800 S25
 * 105  34325   800 S26
 * 106  35125   800 S27
 * 107  35925   800 S28
 * 108  36725   800 S29
 * 109  37525   800 S30
 * 110  38325   800 S31
 * 111  39125   800 S32
 * 112  39925   800 S33
 * 113  40725   800 S34
 * 114  41525   800 S35
 * 115  42325   800 S36
 * 116  43125   800 S37
 * 117  43925   800 S38
 * 118  44725   800 S39
 * 119  45525   800 S40
 * 120  46325   800 S41
 *
 * 121   3890   000 IFFREQ
 *
 */
#define IF_FREQ 38.9
static ULONG weurope[] = {
       121,     (ULONG)( IF_FREQ * FREQFACTOR),     0,
       100,     (ULONG)(303.25 * FREQFACTOR),     (ULONG)(8.00 * FREQFACTOR),
        90,     (ULONG)(231.25 * FREQFACTOR),     (ULONG)(7.00 * FREQFACTOR),
        80,     (ULONG)(105.25 * FREQFACTOR),     (ULONG)(7.00 * FREQFACTOR),
        74,     (ULONG)( 69.25 * FREQFACTOR),     (ULONG)(7.00 * FREQFACTOR),
        21,     (ULONG)(471.25 * FREQFACTOR),     (ULONG)(8.00 * FREQFACTOR),
        17,     (ULONG)(183.25 * FREQFACTOR),     (ULONG)(9.00 * FREQFACTOR),
        16,     (ULONG)(175.25 * FREQFACTOR),     (ULONG)(9.00 * FREQFACTOR),
        15,     (ULONG)(82.25 * FREQFACTOR),      (ULONG)(8.50 * FREQFACTOR),
        13,     (ULONG)(53.75 * FREQFACTOR),      (ULONG)(8.50 * FREQFACTOR),
         5,     (ULONG)(175.25 * FREQFACTOR),     (ULONG)(7.00 * FREQFACTOR),
         2,     (ULONG)(48.25 * FREQFACTOR),      (ULONG)(7.00 * FREQFACTOR),
     0
};
#undef IF_FREQ
/*
 * Japanese Broadcast Channels:
 *
 *  1:  91.25MHz -  3: 103.25MHz
 *  4: 171.25MHz -  7: 189.25MHz
 *  8: 193.25MHz - 12: 217.25MHz  (VHF)
 * 13: 471.25MHz - 62: 765.25MHz  (UHF)
 *
 * IF freq: 45.75 mHz
 *  OR
 * IF freq: 58.75 mHz
 */
#define OFFSET  6.00
#define IF_FREQ 45.75
static ULONG jpnbcst[] = {
    62,     (ULONG)(IF_FREQ * FREQFACTOR),    0,
    13,     (ULONG)(471.25 * FREQFACTOR),     (ULONG)(OFFSET * FREQFACTOR),
     8,     (ULONG)(193.25 * FREQFACTOR),     (ULONG)(OFFSET * FREQFACTOR),
     4,     (ULONG)(171.25 * FREQFACTOR),     (ULONG)(OFFSET * FREQFACTOR),
     1,     (ULONG)( 91.25 * FREQFACTOR),     (ULONG)(OFFSET * FREQFACTOR),
     0
};
#undef IF_FREQ
#undef OFFSET

/*
 * Japanese Cable Channels:
 *
 *  1:  91.25MHz -  3: 103.25MHz
 *  4: 171.25MHz -  7: 189.25MHz
 *  8: 193.25MHz - 12: 217.25MHz
 * 13: 109.25MHz - 21: 157.25MHz
 * 22: 165.25MHz
 * 23: 223.25MHz - 63: 463.25MHz
 *
 * IF freq: 45.75 mHz
 */
#define OFFSET  6.00
#define IF_FREQ 45.75
static ULONG jpncable[] = {
    63,     (ULONG)(IF_FREQ * FREQFACTOR),    0,
    23,     (ULONG)(223.25 * FREQFACTOR),     (ULONG)(OFFSET * FREQFACTOR),
    22,     (ULONG)(165.25 * FREQFACTOR),     (ULONG)(OFFSET * FREQFACTOR),
    13,     (ULONG)(109.25 * FREQFACTOR),     (ULONG)(OFFSET * FREQFACTOR),
     8,     (ULONG)(193.25 * FREQFACTOR),     (ULONG)(OFFSET * FREQFACTOR),
     4,     (ULONG)(171.25 * FREQFACTOR),     (ULONG)(OFFSET * FREQFACTOR),
     1,     (ULONG)( 91.25 * FREQFACTOR),     (ULONG)(OFFSET * FREQFACTOR),
     0
};
#undef IF_FREQ
#undef OFFSET

/*
 * xUSSR Broadcast Channels:
 *
 *  1:  49.75MHz -  2:  59.25MHz
 *  3:  77.25MHz -  5:  93.25MHz
 *  6: 175.25MHz - 12: 223.25MHz
 * 13-20 - not exist
 * 21: 471.25MHz - 34: 575.25MHz
 * 35: 583.25MHz - 69: 855.25MHz
 *
 * Cable channels
 *
 * 70: 111.25MHz - 77: 167.25MHz
 * 78: 231.25MHz -107: 463.25MHz
 *
 * IF freq: 38.90 MHz
 */
#define IF_FREQ 38.90
static ULONG xussr[] = {
      107,     (ULONG)(IF_FREQ * FREQFACTOR),    0,
       78,     (ULONG)(231.25 * FREQFACTOR),     (ULONG)(8.00 * FREQFACTOR),
       70,     (ULONG)(111.25 * FREQFACTOR),     (ULONG)(8.00 * FREQFACTOR),
       35,     (ULONG)(583.25 * FREQFACTOR),     (ULONG)(8.00 * FREQFACTOR),
       21,     (ULONG)(471.25 * FREQFACTOR),     (ULONG)(8.00 * FREQFACTOR),
        6,     (ULONG)(175.25 * FREQFACTOR),     (ULONG)(8.00 * FREQFACTOR),
        3,     (ULONG)( 77.25 * FREQFACTOR),     (ULONG)(8.00 * FREQFACTOR),
        1,     (ULONG)( 49.75 * FREQFACTOR),     (ULONG)(9.50 * FREQFACTOR),
        0
};
#undef IF_FREQ

/*
 * Australian broadcast channels
 */
#define OFFSET  7.00
#define IF_FREQ 38.90
static ULONG australia[] = {
       83,     (ULONG)(IF_FREQ * FREQFACTOR),    0,
       28,     (ULONG)(527.25 * FREQFACTOR),     (ULONG)(OFFSET * FREQFACTOR),
       10,     (ULONG)(209.25 * FREQFACTOR),     (ULONG)(OFFSET * FREQFACTOR),
        6,     (ULONG)(175.25 * FREQFACTOR),     (ULONG)(OFFSET * FREQFACTOR),
        4,     (ULONG)( 95.25 * FREQFACTOR),     (ULONG)(OFFSET * FREQFACTOR),
        3,     (ULONG)( 86.25 * FREQFACTOR),     (ULONG)(OFFSET * FREQFACTOR),
        1,     (ULONG)( 57.25 * FREQFACTOR),     (ULONG)(OFFSET * FREQFACTOR),
        0
};
#undef OFFSET
#undef IF_FREQ

/*
 * France broadcast channels
 */
#define OFFSET 8.00
#define IF_FREQ 38.90
static ULONG france[] = {
        69,     (ULONG)(IF_FREQ * FREQFACTOR),     0,
        21,     (ULONG)(471.25 * FREQFACTOR),     (ULONG)(OFFSET * FREQFACTOR), /* 21 -> 69 */
         5,     (ULONG)(176.00 * FREQFACTOR),     (ULONG)(OFFSET * FREQFACTOR), /* 5 -> 10 */
         4,     (ULONG)( 63.75 * FREQFACTOR),     (ULONG)(OFFSET * FREQFACTOR), /* 4    */
         3,     (ULONG)( 60.50 * FREQFACTOR),     (ULONG)(OFFSET * FREQFACTOR), /* 3    */
         1,     (ULONG)( 47.75 * FREQFACTOR),     (ULONG)(OFFSET * FREQFACTOR), /* 1  2 */
         0
};
#undef OFFSET
#undef IF_FREQ

static struct {
        int     *ptr;
        char    name[50];
} freqTable[] = {
        {NULL,          ""},
        {nabcst,        "nabcst"},
        {irccable,      "cableirc"},
        {hrccable,      "cablehrc"},
        {weurope,       "weurope"},
        {jpnbcst,       "jpnbcst"},
        {jpncable,      "jpncable"},
        {xussr,         "xussr"},
        {australia,     "australia"},
        {france,        "france"},

};
int currentChnlSet=0;
void CSSetCurrent(int channelset)
{
    currentChnlSet=channelset;
}
ULONG CSGetChannelFreq(int channel)
{
    freqTable[currentChnlSet]
}


