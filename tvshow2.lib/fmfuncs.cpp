/*
** Module   :FMFUNCS.CPP
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
#include "fmfuncs.h"
#include "tvaudio.h"
#include "btfuncs.h"
#define FMSTEPSIZE 50000 //50 kHz

APIRET FMReadChannelList(PTVCARD pbttv, char * channfilename)
{
   FILE* channelfile;
   int i;
   i=1;
   dprintf(LOG_INFO,"Reading fm channels list \n");
   channelfile=fopen(channfilename,"r");
   if (channelfile==NULL)
      {
         dprintf(LOG_ERROR,"%s file not exist.\n",channfilename);
         pbttv->fmchannelcount=0;
         return 1;
      }
   char tempbuf[255], tempstring[255];
   ULONG stringlen;
   while (0==feof(channelfile))
      {

         fgets(tempbuf,255,channelfile);
//         sscanf(tempbuf,"%d %d %d %d %d %d %d %50c ",&channels[i].channel,&channels[i].frequency,&channels[i].colorsystem,&channels[i].brightness,&channels[i].contrast,&channels[i].saturation,&channels[i].volume,channels[i].name);
         sscanf(tempbuf,"%d %d %d %50c ",&pbttv->fmchannels[i].channel,&pbttv->fmchannels[i].frequency,&pbttv->fmchannels[i].volume,&tempstring);
         stringlen=(ULONG)(strchr(tempstring,'\n')-tempstring);
         strncpy(pbttv->fmchannels[i].name,tempstring,stringlen);
//         printf("num: %d %d %d %s\n",i,channels[i].channel,channels[i].frequency,channels[i].name);
         dprintf(LOG_DEBUG,"num: %d %d %d %d %s\n",i,pbttv->fmchannels[i].channel,pbttv->fmchannels[i].frequency,pbttv->fmchannels[i].volume,pbttv->fmchannels[i].name);
         i++;
         }
   fclose(channelfile);
   pbttv->fmchannelcount=i-1;
   dprintf(LOG_DEBUG,"total channel %d \n",pbttv->fmchannelcount);

   return 0;
}

APIRET FMWriteChannelList(PTVCARD pbttv, char * channfilename)
{
   FILE* channelfile;
   int i;
   dprintf(LOG_INFO,"Writing fm channels list \n");
   channelfile=fopen(channfilename,"w");
   for (i=1;i<pbttv->fmchannelcount;i++)
      {
         fprintf(channelfile,"%u %u %u %s\n",pbttv->fmchannels[i].channel,pbttv->fmchannels[i].frequency,pbttv->fmchannels[i].volume,pbttv->fmchannels[i].name);
         }
   fclose(channelfile);
   return NO_ERROR;
   }
void FMSelectChannel(PTVCARD pbttv,UCHAR channum)
{
   long freq;

   pbttv->channel=channum;

   strcpy(pbttv->channelname,pbttv->fmchannels[channum].name);
//   printf("%s %s\n",tvcard.tuner.channelname,channels[channum].name);
   freq=pbttv->fmchannels[channum].frequency;
   dprintf(LOG_DEBUG,"select fm channel %d freq %d, name %s \n",channum,pbttv->fmchannels[channum].frequency,pbttv->fmchannels[channum].name);
   TVMuxControl(pbttv,AMUX_MUTE);
   BTSetFreq(pbttv,freq,FMMode);
   TVMuxControl(pbttv,AMUX_INT);
}
void FMSelectNextChannel(PTVCARD pbttv)
{
   UCHAR currchan;
   currchan=pbttv->channel;
   currchan++;
   if (currchan==pbttv->fmchannelcount) currchan=1;
   FMSelectChannel(pbttv,currchan);
}
void FMSelectPrevChannel(PTVCARD pbttv)
{
   UCHAR currchan;
   currchan=pbttv->channel;
   currchan--;
   if (currchan==0) currchan=pbttv->fmchannelcount-1;
   FMSelectChannel(pbttv,currchan);
}
void FMScanChannels(PTVCARD pbttv)
{
	UCHAR channum,tunerstat;
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
}

