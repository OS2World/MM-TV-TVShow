/*
** Module   :TVFUNCS.CPP
** Abstract :
**
** Copyright (C) Alex Cherkaev
**
** Log: Mon  21/10/2002 Created
**
*/
#include "logger.h"
#include "card.h"
#include "bttuner.h"
#include "btfuncs.h"
#include "TVfuncs.h"
#include "TVaudio.h"
#include "btregs.h"
#define INCL_WINDIALOG
#include <os2.h>
//#define FMSTEPSIZE 50000 //50 kHz

APIRET TVReadChannelList(PTVCARD pbttv, char * channfilename)
{
   FILE* channelfile;
   int i;
   i=1;
   dprintf(LOG_INFO,"Reading TV channels list \n");
   channelfile=fopen(channfilename,"r");
   if (channelfile==NULL)
      {
         dprintf(LOG_ERROR,"%s file not exist.\n",channfilename);
         pbttv->tvchannelcount=0;
         return 1;
      }
   char tempbuf[255], tempstring[255];
   ULONG stringlen;
   while (0==feof(channelfile))
      {

         if (fgets(tempbuf,255,channelfile)==NULL) break;
//         sscanf(tempbuf,"%d %d %d %d %d %d %d %50c ",&channels[i].channel,&channels[i].frequency,&channels[i].colorsystem,&channels[i].brightness,&channels[i].contrast,&channels[i].saturation,&channels[i].volume,channels[i].name);
         sscanf(tempbuf,"%d %d %d %d %d %d %d %50c ",
         &pbttv->tvchannels[i].channel,&pbttv->tvchannels[i].frequency,
         &pbttv->tvchannels[i].colorsystem,&pbttv->tvchannels[i].brightness,
         &pbttv->tvchannels[i].contrast,&pbttv->tvchannels[i].saturation,
         &pbttv->tvchannels[i].volume,&tempstring);
         stringlen=(ULONG)(strchr(tempstring,'\n')-tempstring);
         strncpy(pbttv->tvchannels[i].name,tempstring,stringlen);
//         printf("num: %d %d %d %s\n",i,channels[i].channel,channels[i].frequency,channels[i].name);
         dprintf(LOG_DEBUG,"num: %d %d %d %d %d %d %d %d %s\n",i,
         pbttv->tvchannels[i].channel,pbttv->tvchannels[i].frequency,
         pbttv->tvchannels[i].colorsystem,pbttv->tvchannels[i].brightness,
         pbttv->tvchannels[i].contrast,pbttv->tvchannels[i].saturation,
         pbttv->tvchannels[i].volume,pbttv->tvchannels[i].name);
         i++;
         }
   fclose(channelfile);
   pbttv->tvchannelcount=i-1;
   dprintf(LOG_DEBUG,"total channel %d \n",pbttv->tvchannelcount);

   return 0;
}

APIRET TVWriteChannelList(PTVCARD pbttv, char * channfilename)
{
   FILE* channelfile;
   int i;
   dprintf(LOG_INFO,"Writing tv channels list \n");
   channelfile=fopen(channfilename,"w");
   for (i=1;i<=pbttv->tvchannelcount;i++)
      {
         fprintf(channelfile,"%u %u %u %u %u %u %u %s\n",
         pbttv->tvchannels[i].channel,pbttv->tvchannels[i].frequency,
         pbttv->tvchannels[i].colorsystem,pbttv->tvchannels[i].brightness,
         pbttv->tvchannels[i].contrast,pbttv->tvchannels[i].saturation,
         pbttv->tvchannels[i].volume,pbttv->tvchannels[i].name);
      }
   dprintf(LOG_INFO,"%d tv channels wroted \n",i-1);
   fclose(channelfile);
   return NO_ERROR;
   }
APIRET TVSelectChannel(PTVCARD pbttv,UCHAR channum)
{
   ULONG freq;
   UCHAR brightness,contrast,saturation,volume,colorsystem;

   if (channum>pbttv->tvchannelcount) return -1;

   pbttv->channel=channum;
   strcpy(pbttv->channelname,pbttv->tvchannels[channum].name);
   freq=pbttv->tvchannels[channum].frequency;
   brightness=pbttv->tvchannels[channum].brightness;
   contrast=pbttv->tvchannels[channum].contrast;
   saturation=pbttv->tvchannels[channum].saturation;
   volume=pbttv->tvchannels[channum].volume;
   colorsystem=pbttv->tvchannels[channum].colorsystem;
   dprintf(LOG_DEBUG,"select tv channel %d freq %d, bright %d, contr %d, sat %d name %s \n",
   channum,pbttv->tvchannels[channum].frequency,pbttv->tvchannels[channum].brightness,
   pbttv->tvchannels[channum].contrast,pbttv->tvchannels[channum].saturation,
   pbttv->tvchannels[channum].name);
   TVMuxControl(pbttv,AMUX_MUTE);
   TVSetColorSystem(pbttv,colorsystem);
   BTSetFreq(pbttv,freq,TVMode);
   TVSetBrightness(pbttv,brightness);
   TVSetContrast(pbttv,contrast);
   TVSetSaturation(pbttv,saturation);
   TVSetVolume(pbttv,volume);
//   TVMuxControl(pbttv,AMUX_TUNER);
   return NO_ERROR;
}
void TVSelectNextChannel(PTVCARD pbttv)
{
   UCHAR currchan;
   currchan=pbttv->channel;
   currchan++;
   if (currchan==pbttv->tvchannelcount+1) currchan=1;
   TVSelectChannel(pbttv,currchan);
}
void TVSelectPrevChannel(PTVCARD pbttv)
{
   UCHAR currchan;
   currchan=pbttv->channel;
   currchan--;
   if (currchan==0) currchan=pbttv->tvchannelcount;
   TVSelectChannel(pbttv,currchan);
}
void TVScanChannels(PTVCARD pbttv)
{
/*    UCHAR channum,tunerstat;
	ULONG currfreq;
	channum=1;
    for (currfreq=pbttv->pcard->ptuner->BandLim[6];currfreq<=pbttv->pcard->ptuner->BandLim[7];currfreq+=FMSTEPSIZE)
    {
		BTSetFreq(pbttv,currfreq,FMMode);
		DosSleep(100);
		tunerstat=BTGetTunerStatus(pbttv);
        if (((tunerstat&7)>=3)&&((tunerstat&0x10)))
            {
				dprintf(LOG_INFO, "FM station found, freq: %d, tunerstat %x \n",currfreq,tunerstat);
                sprintf(pbttv->fmchannels[channum].name,"New Station %d",channum);
                pbttv->fmchannels[channum].frequency=currfreq;
                pbttv->fmchannels[channum].channel=channum;
                pbttv->fmchannels[channum].volume=80;
				channum++;
            }
    }
    pbttv->fmchannelcount=channum-1;
*/
}
APIRET TVSetBrightness(PTVCARD pbttv,UCHAR brightness)
{
    CHAR physbrightness=0;

    if (brightness>100) return -1;
    pbttv->brightness=brightness;
    physbrightness=((UCHAR)brightness*2.55)-127;
    BTSetBrightness(pbttv, physbrightness);
    return NO_ERROR;

}
APIRET TVSetContrast(PTVCARD pbttv,UCHAR contrast)
{
    ULONG physcontrast=0;

    if (contrast>100) return -1;
    pbttv->contrast=contrast;
    physcontrast=(UINT)contrast*5.11;
    BTSetContrast(pbttv, physcontrast);
    return NO_ERROR;
}
APIRET TVSetSaturation(PTVCARD pbttv,UCHAR saturation)
{
    ULONG physsaturation=0;

    if (saturation>100) return -1;
    pbttv->saturation=saturation;
    physsaturation=(UINT)saturation*5.11;
    BTSetSaturation(pbttv, physsaturation);
    return NO_ERROR;
}
APIRET TVSetVolume(PTVCARD pbttv,UCHAR volume)
{
    if (volume>100) return -1;
    pbttv->volume=volume;
    return NO_ERROR;
}
APIRET TVSetColorSystem(PTVCARD pbttv, UCHAR colorsystem)
{
    pbttv->colorsystem=colorsystem;
    BTSelectColorSystem(pbttv,colorsystem);
    return NO_ERROR;
}
void TVIncBrightness(PTVCARD pbttv)
{
    UCHAR bright;
    bright=pbttv->brightness;
    if (bright==100)
    {
        WinAlarm(HWND_DESKTOP,WA_NOTE);
    } else
    {
        bright++;
        TVSetBrightness(pbttv,bright);
    }
}
void TVDecBrightness(PTVCARD pbttv)
{
    UCHAR bright;
    bright=pbttv->brightness;
    if (bright==0)
    {
        WinAlarm(HWND_DESKTOP,WA_NOTE);
    } else
    {
        bright--;
        TVSetBrightness(pbttv,bright);
    }
}
void TVIncContrast(PTVCARD pbttv)
{
    UCHAR contrast;
    contrast=pbttv->contrast;
    if (contrast==100)
    {
        WinAlarm(HWND_DESKTOP,WA_NOTE);
    } else
    {
        contrast++;
        TVSetContrast(pbttv,contrast);
    }
}
void TVDecContrast(PTVCARD pbttv)
{
    UCHAR contrast;
    contrast=pbttv->contrast;
    if (contrast==0)
    {
        WinAlarm(HWND_DESKTOP,WA_NOTE);
    } else
    {
        contrast--;
        TVSetContrast(pbttv,contrast);
    }
}
void TVIncSaturation(PTVCARD pbttv)
{
    UCHAR saturation;
    saturation=pbttv->saturation;
    if (saturation==100)
    {
        WinAlarm(HWND_DESKTOP,WA_NOTE);
    } else
    {
        saturation++;
        TVSetSaturation(pbttv,saturation);
    }
}
void TVDecSaturation(PTVCARD pbttv)
{
    UCHAR saturation;
    saturation=pbttv->saturation;
    if (saturation==0)
    {
        WinAlarm(HWND_DESKTOP,WA_NOTE);
    } else
    {
        saturation--;
        TVSetSaturation(pbttv,saturation);
    }
}
void TVIncVolume(PTVCARD pbttv)
{
    UCHAR volume;
    volume=pbttv->volume;
    if (volume==100)
    {
        WinAlarm(HWND_DESKTOP,WA_NOTE);
    } else
    {
        volume++;
        TVSetVolume(pbttv,volume);
    }
}
void TVDecVolume(PTVCARD pbttv)
{
    UCHAR volume;
    volume=pbttv->volume;
    if (volume==0)
    {
        WinAlarm(HWND_DESKTOP,WA_NOTE);
    } else
    {
        volume--;
        TVSetVolume(pbttv,volume);
    }

}
void TVUpdateCurrentChannel(PTVCARD pbttv)
{
    pbttv->tvchannels[pbttv->channel].frequency=pbttv->tuner.frequency;
    pbttv->tvchannels[pbttv->channel].colorsystem=pbttv->colorsystem;
    pbttv->tvchannels[pbttv->channel].brightness=pbttv->brightness;
    pbttv->tvchannels[pbttv->channel].contrast=pbttv->contrast;
    pbttv->tvchannels[pbttv->channel].saturation=pbttv->saturation;
    pbttv->tvchannels[pbttv->channel].volume=pbttv->volume;
//    strncpy(pbttv->tvchannels[pbttv->channel].name,tempstring,stringlen);
}
void TVAddChannel(PTVCARD pbttv, PTVCHANNEL newchannel)
{
    pbttv->tvchannelcount++;
    pbttv->tvchannels[pbttv->tvchannelcount].channel=pbttv->tvchannelcount;
    pbttv->tvchannels[pbttv->tvchannelcount].frequency=newchannel->frequency;
    pbttv->tvchannels[pbttv->tvchannelcount].colorsystem=newchannel->colorsystem;
    pbttv->tvchannels[pbttv->tvchannelcount].brightness=newchannel->brightness;
    pbttv->tvchannels[pbttv->tvchannelcount].contrast=newchannel->contrast;
    pbttv->tvchannels[pbttv->tvchannelcount].saturation=newchannel->saturation;
    pbttv->tvchannels[pbttv->tvchannelcount].volume=newchannel->volume;
    strcpy(pbttv->tvchannels[pbttv->tvchannelcount].name,newchannel->name);

}
void TVSelectInput(PTVCARD pbttv,UCHAR input)
{
    if ((input>=0)||(input<=3))
    {
        pbttv->videoinput=input;
        BTSelectInput(pbttv,input);
    }
}


