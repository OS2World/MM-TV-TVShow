#include <ibase.hpp>
#include <istring.hpp>
#include <ireslib.hpp>


#include "base.h"
#include "tvshow2.h"
#include "tvscandlg.hpp"
#include "logger.h"
#include "tvfuncs.h"

#include "btregs.h"
#include "btfuncs.h"
#include "bttuner.h"

TVScanDialog::TVScanDialog(IWindow * ownerWnd)
             : IFrameWindow(IResourceId(DLG_SCANNING))
               ,scandialogCommandHandler(this)
               ,scanselectCommandHandler(this)
               ,scanProgress(PI_SCANNING,this)
               ,channelsfound(EF_CHANNELS,this)
               ,currentBand(EF_BAND,this)
               ,slowScan(RB_SLOW,this)
               ,fastScan(RB_FAST,this)
               ,region(CX_REGION,this)
{
  numchannels=0;
  scanningNow=FALSE;
  //scanfunref=new IThreadMemberFn<TVScanDialog>(*this,TVScanDialog::scan);
  setScanMode(Slow);
  slowScan.select();
  region.add(1,"test");
  region.add(2,"test1");
  channelsfound.setText(IString(numchannels));
  currentBand.setText(" ");
  scanProgress.enableRibbonStrip();
  scandialogCommandHandler.handleEventsFor(this);
  scanselectCommandHandler.handleEventsFor(this);
}

TVScanDialog :: ~TVScanDialog()
{
  scanselectCommandHandler.stopHandlingEventsFor(this);
  scandialogCommandHandler.stopHandlingEventsFor(this);
} /* end TVVideoDialog :: ~TVVideoDialog() */
void TVScanDialog::scan()
{
    ULONG freq;
    UCHAR dstat,tunerstat,i,value;
    APIRET rc;
    BOOL inLockFlag;
    scanningNow=TRUE;
//    i=0;
    for (i=0;i<3;i++)
    {
        switch (i)
        {
            case 0:
                currentBand.setText("VHF-1");
                break;
            case 1:
                currentBand.setText("VHF-3");
                break;
            case 2:
                currentBand.setText("UHF");
                break;
        }
        for (freq=pbttv->pcard->ptuner->BandLim[i*2];freq<=pbttv->pcard->ptuner->BandLim[i*2+1];freq+=62500)
        {
            value=(UCHAR)(((float)freq-pbttv->pcard->ptuner->BandLim[i*2])*100/(float)(pbttv->pcard->ptuner->BandLim[i*2+1]-pbttv->pcard->ptuner->BandLim[i*2]));
//            dprintf(LOG_DEBUG,"Value: %d \n",value);
            scanProgress.setValue(value);
            BTSetFreq(pbttv,freq,TVMode);
            do
            {
            tunerstat=BTGetTunerStatus(pbttv);
            if (tunerstat&0x40) //FL=1?
               {inLockFlag=TRUE;}
               else
               {
//                  dprintf(LOG_DEBUG,"Phase not locked, tunerstat %X \n",tunerstat);
                  inLockFlag=FALSE;
                  DosSleep(10);
               }
            }
            while (!inLockFlag);
            dprintf(LOG_DEBUG,"TunerStat : %X, i %d numchannels %d freq %d \n",tunerstat,i,numchannels,freq);
            tunerstat=tunerstat&7;
//            dprintf(LOG_DEBUG,"TunerStat : %X\n",tunerstat);
            if (tunerstat==2)
            {
                DosSleep(1000);
                dstat=BTReadRegister(pbttv,DSTATUS, 1, rc);
                if ((dstat&BT848_DSTATUS_PRES)&&(dstat&BT848_DSTATUS_HLOC))
                {
                    DosBeep(900,300);
                    numchannels++;
                    channelsfound.setText(IString(numchannels));
                    foundchannels[numchannels].channel=numchannels;
                    foundchannels[numchannels].frequency=freq;
                    foundchannels[numchannels].colorsystem=cfg.colorsystem;
                    foundchannels[numchannels].brightness=cfg.brightness;
                    foundchannels[numchannels].contrast=cfg.contrast;
                    foundchannels[numchannels].saturation=cfg.saturation;
                    foundchannels[numchannels].volume=cfg.volume;
                    strcpy(foundchannels[numchannels].name,(IString("New channel ")+IString(numchannels)));
                }
            }
        }
    }
      scanningNow=FALSE;
}
void TVScanDialog::startscanthread()
{
scanthread.start(new IThreadMemberFn<TVScanDialog>(*this,TVScanDialog::scan));
}
void TVScanDialog::stopscanthread()
{
if (scanningNow)
   {
      scanthread.stop();
      scanningNow=FALSE;
   }
}
void TVScanDialog::addFoundChannels()
{
   int i;
   for (i=1;i<=numchannels;i++)
   {
   dprintf(LOG_DEBUG,"Adding new channel n: %d \n",i);
      TVAddChannel(pbttv, &foundchannels[i]);
   }
}
void TVScanDialog::setScanMode(scanModeEnum scanMode)
{
	scanningMode=scanMode;
	switch (scanMode) {
	  case Slow:
	    slowScan.select();
	    region.disable();
	    break;
	  case Fast:
	    fastScan.select();
	    region.enable();
	    break;
	  }

}
TVScanDialogCommandHandler::TVScanDialogCommandHandler(TVScanDialog *dialogFrame)
{
   frame=dialogFrame;                   //Save frame to be handled
}
IBase::Boolean TVScanDialogCommandHandler::command(ICommandEvent & cmdEvent)
{
  Boolean eventProcessed(true);         //Assume event will be processed

  switch (cmdEvent.commandId()) {
    case DID_OK:
      frame->stopscanthread();
      frame->addFoundChannels();
      frame->dismiss(DID_OK);
      break;
    case DID_CANCEL:
      frame->stopscanthread();
      frame->dismiss(DID_CANCEL);
      break;
    case PB_SCAN:
      DosBeep(100,100);
      frame->startscanthread();
      break;

    default:                            //Otherwise,
      eventProcessed=false;             //  the event wasn't processed
  } /* end switch */

  return(eventProcessed);
}
TVScanSelectCommandHandler::TVScanSelectCommandHandler(TVScanDialog *dialogFrame)
{
   selectframe=dialogFrame;                   //Save frame to be handled
}
IBase::Boolean TVScanSelectCommandHandler::selected(IControlEvent & selectEvent)
{
  Boolean eventProcessed(true);         //Assume event will be processed

  switch (selectEvent.controlId()) {
    case RB_SLOW:
    	selectframe->setScanMode(TVScanDialog::Slow);
    	break;
    case RB_FAST:
    	selectframe->setScanMode(TVScanDialog::Fast);
    	break;
    default:                            //Otherwise,
      eventProcessed=false;             //  the event wasn't processed
    }
  return(eventProcessed);
}

TVProgress::TVProgress(ULONG windowId, IWindow* parent)
    :IProgressIndicator(windowId, parent)
{
    setTicks(scale2,5,90);
    setTickLength(3);
    setTickText(0,"0%");
    setTickText(1,"25%");
    setTickText(2,"50%");
    setTickText(3,"75%");
    setTickText(4,"100%");

}
void TVProgress::setValue(UCHAR value)
{
UINT val;
    moveArmToPixel((armRange()-1)*value/100);
}

