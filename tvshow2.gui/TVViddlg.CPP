#include <ibase.hpp>                  //  since that is where IC_<environ>
#include <istring.hpp>                  //Include IString class header        V4
#include <ireslib.hpp>                  //Include IResourceId class header    V4

#include "tvshow2.h"                   //Include symbolic definitions        V4
#include "tvshow2.hpp"
#include "tvviddlg.hpp"                 //Include TVVideoDialog class header    V4
#include "logger.h"
#include "tvfuncs.h"
#include "bttuner.h"
extern TVShowWindow* pmainWindow;

TVVideoDialog::TVVideoDialog(IWindow * ownerWnd)        //V4
             : IFrameWindow(IResourceId(DLG_VIDEOCONTROL))                     //V4
               ,dialogCommandHandler(this)
               ,sliderCommandHandler(this)
               ,selectCommandHandler(this)
               ,brightnessSlider(SL_BRIGHTNESS,this)
               ,contrastSlider(SL_CONTRAST,this)
               ,saturationSlider(SL_SATURATION,this)
               ,volumeSlider(SL_VOLUME,this)
               ,ntscmButton(RB_NTSC_M,this)
               ,ntscjButton(RB_NTSCJAPAN,this)
               ,palButton(RB_PAL,this)
               ,palmButton(RB_PALM,this)
               ,palnButton(RB_PALN,this)
               ,secamButton(RB_SECAM,this)
               ,palncButton(RB_PALNCOMBINATION,this)
               ,channelName(EF_CHANNELNAME,this)
               ,channelFreq(EF_FREQUENCY,this)
               ,incFreq(PB_FREQINC,this)
               ,decFreq(PB_FREQDEC,this)
{
  oldBrightness=pbttv->brightness;
  oldContrast=pbttv->contrast;
  oldSaturation=pbttv->saturation;
  oldVolume=pbttv->volume;
  oldColor=pbttv->colorsystem;
  oldName=pbttv->channelname;
  oldFreq=pbttv->tuner.frequency;
  freq=oldFreq;
  displayFreq();
  brightnessSlider.setValue(oldBrightness);
  contrastSlider.setValue(oldContrast);
  saturationSlider.setValue(oldSaturation);
  volumeSlider.setValue(oldVolume);
  channelName.setText(oldName);
  SetColorState(oldColor);
  displayFreq();

  dialogCommandHandler.handleEventsFor(this);
  sliderCommandHandler.handleEventsFor(this);
  selectCommandHandler.handleEventsFor(this);
} /* end TVVideoDialog :: TVVideoDialog(...) */

/**************************************************************************
* TVVideoDialog :: ~TVVideoDialog - Destructor for the dialog frame window    *
*   Stop handling command events for the frame.                           *
**************************************************************************/
TVVideoDialog :: ~TVVideoDialog()
{
  selectCommandHandler.stopHandlingEventsFor(this);
  sliderCommandHandler.stopHandlingEventsFor(this);
  dialogCommandHandler.stopHandlingEventsFor(this);
} /* end TVVideoDialog :: ~TVVideoDialog() */
void TVVideoDialog::SetColorState(UCHAR color)
{
    switch (color) {
        case 1:
            ntscmButton.select();
            break;
        case 2:
            ntscjButton.select();
            break;
        case 3:
            palButton.select();
            break;
        case 4:
            palmButton.select();
            break;
        case 5:
            palnButton.select();
            break;
        case 6:
            secamButton.select();
            break;
        case 7:
            palncButton.select();
            break;
        }
}
void TVVideoDialog::getSliders()
{
dprintf(LOG_DEBUG,"bright slider pos %d, range %d, value %d \n",brightnessSlider.armPixelOffset(),brightnessSlider.armRange(),brightnessSlider.getValue());
dprintf(LOG_DEBUG,"contrast slider pos %d, range %d, value %d \n",contrastSlider.armPixelOffset(),contrastSlider.armRange(),contrastSlider.getValue());
dprintf(LOG_DEBUG,"saturation slider pos %d, range %d, value %d \n",saturationSlider.armPixelOffset(),saturationSlider.armRange(),saturationSlider.getValue());
}
void TVVideoDialog::setVideoValues()
{
    pmainWindow->setBrightness(brightnessSlider.getValue());
    pmainWindow->setContrast(contrastSlider.getValue());
    pmainWindow->setSaturation(saturationSlider.getValue());
    pmainWindow->setVolume(volumeSlider.getValue());
}
void TVVideoDialog::setOldVideoValues()
{
    pmainWindow->setBrightness(oldBrightness);
    pmainWindow->setContrast(oldContrast);
    pmainWindow->setSaturation(oldSaturation);
    pmainWindow->setVolume(oldVolume);

    TVSetColorSystem(pbttv,oldColor);
    BTSetFreq(pbttv,oldFreq,TVMode);
}
void TVVideoDialog::getText()
{
    strcpy(pbttv->channelname,(char *)channelName.text());
    strcpy(pbttv->tvchannels[pbttv->channel].name,(char *)channelName.text());
}
void TVVideoDialog::IncFreq()
{
    freq+=62500;
}
void TVVideoDialog::DecFreq()
{
    freq-=62500;
}
ULONG TVVideoDialog::GetFreq()
{
    return freq;
}
void TVVideoDialog::displayFreq()
{
    channelFreq.setText(IString(freq));
}
TVVideoDialogCommandHandler::TVVideoDialogCommandHandler(TVVideoDialog *dialogFrame)    //V4
{
   frame=dialogFrame;                   //Save frame to be handled            V4
}
IBase::Boolean TVVideoDialogCommandHandler::command(ICommandEvent & cmdEvent)                //V4
{                                                                           //V4
  Boolean eventProcessed(true);         //Assume event will be processed      V4

  switch (cmdEvent.commandId()) {                                           //V4
    case DID_OK:
      frame->getSliders();
      frame->setVideoValues();
      frame->getText();
      frame->dismiss(DID_OK);                                               //V4
      break;                                                                //V4
    case DID_CANCEL:
      frame->setOldVideoValues();                                                      //V4
      frame->dismiss(DID_CANCEL);                                           //V4
      break;                                                                //V4
    case PB_FREQINC:
      frame->IncFreq();
      frame->displayFreq();
      BTSetFreq(pbttv,frame->GetFreq(),TVMode);
      break;
    case PB_FREQDEC:
      frame->DecFreq();
      frame->displayFreq();
      BTSetFreq(pbttv,frame->GetFreq(),TVMode);
      break;

    default:                            //Otherwise,                          V4
      eventProcessed=false;             //  the event wasn't processed        V4
  } /* end switch */                                                        //V4

  return(eventProcessed);
}
TVSelectCommandHandler::TVSelectCommandHandler(TVVideoDialog *dialogFrame)
{
   selectframe=dialogFrame;                   //Save frame to be handled            V4
}
IBase::Boolean TVSelectCommandHandler::selected(IControlEvent & selectEvent)                //V4
{
  Boolean eventProcessed(true);         //Assume event will be processed

  switch (selectEvent.controlId()) {
    case RB_NTSC_M:
        TVSetColorSystem(pbttv,1);
        break;
    case RB_NTSCJAPAN:
        TVSetColorSystem(pbttv,2);
        break;
    case RB_PAL:
        TVSetColorSystem(pbttv,3);
        break;
    case RB_PALM:
        TVSetColorSystem(pbttv,4);
        break;
    case RB_SECAM:
        TVSetColorSystem(pbttv,6);
        break;
    case RB_PALN:
        TVSetColorSystem(pbttv,5);
        break;
    case RB_PALNCOMBINATION:
        TVSetColorSystem(pbttv,7);
        break;

    default:                            //Otherwise,
      eventProcessed=false;             //  the event wasn't processed
    }
  return(eventProcessed);
}

TVSliderCommandHandler::TVSliderCommandHandler(TVVideoDialog *dialogFrame)
{
   sliderframe=dialogFrame;                   //Save frame to be handled            V4
}

IBase::Boolean TVSliderCommandHandler::moving(IControlEvent & moveEvent)                //V4
{                                                                           //V4
  Boolean eventProcessed(true);         //Assume event will be processed      V4

  switch (moveEvent.controlId()) {                                           //V4
    case SL_BRIGHTNESS:
      dprintf(LOG_DEBUG,"Brightness move event \n");
      sliderframe->setVideoValues();
      break;                                                                //V4
    case SL_CONTRAST:
      dprintf(LOG_DEBUG,"Contrast move event \n");
      sliderframe->setVideoValues();
      break;                                                                //V4
    case SL_SATURATION:
      dprintf(LOG_DEBUG,"Saturation move event \n");
      sliderframe->setVideoValues();
      break;                                                                //V4
    default:                            //Otherwise,                          V4
      eventProcessed=false;             //  the event wasn't processed        V4
}
  return(eventProcessed);
}
IBase::Boolean TVSliderCommandHandler::positionChanged(IControlEvent & positionEvent)                //V4
{                                                                           //V4
  Boolean eventProcessed(true);         //Assume event will be processed      V4

  switch (positionEvent.controlId()) {                                           //V4
    case SL_BRIGHTNESS:
      dprintf(LOG_DEBUG,"Brightness position event \n");
      sliderframe->setVideoValues();
      break;                                                                //V4
    case SL_CONTRAST:
      dprintf(LOG_DEBUG,"Contrast position event \n");
      sliderframe->setVideoValues();
      break;                                                                //V4
    case SL_SATURATION:
      dprintf(LOG_DEBUG,"Saturation position event \n");
      sliderframe->setVideoValues();
      break;                                                                //V4
    default:                            //Otherwise,                          V4
      eventProcessed=false;             //  the event wasn't processed        V4
  } /* end switch */                                                        //V4
  return(eventProcessed);                                                   //V4
}

TVSlider::TVSlider(ULONG windowId, IWindow* parent)
    :ISlider(windowId, parent)
{
    setTicks(2,5,10);
    setTickLength(3);
    setTickText(0,"0%");
    setTickText(1,"25%");
    setTickText(2,"50%");
    setTickText(3,"75%");
    setTickText(4,"100%");
}
TVSlider::setValue(UCHAR value)
{
UINT val;
    moveArmToPixel((armRange()-1)*value/100);
}
UCHAR TVSlider::getValue()
{
    int total,current,ret;
    total=armRange();
    current=armPixelOffset();
    ret=current*100/(total-1);
    return (UCHAR)ret;
}

