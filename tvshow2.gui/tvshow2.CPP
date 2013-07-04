#include <ibase.hpp>                  //  since that is where IC_<environ>
#include <iapp.hpp>                     //IApplication class
#include <imsgbox.hpp>
#include <string.h>
#define INCL_DOS
#define INCL_DOSDEVICES
#define INCL_DOSDEVIOCTL
#define INCL_DOSERRORS
#include <os2.h>
#include <time.h>

#include "tvviddlg.hpp"
#include "tvscandlg.hpp"

#include "tvshow2.hpp"
#include "tvshow2.h"                   //Include symbolic definitions        v2
#include "woverlay.h"

#include "base.h"
#include "btfuncs.h"
#include "bttuner.h"
#include "tvfuncs.h"
//#include "card.h"
#include "logger.h"
//#include <string.h>
#include "btregs.h"
#include "tvaudio.h"
#include "pipehndl.hpp"

//#include "fmfuncs.h"
#include "memory.h"
#include "driver.h"
#include "globals.h"
#include "mspfunc.h"
#include "savejpeg.hpp"
#define CHANNELFILENAME "channels.dat"
HFILE hfile;


TVShowWindow * pmainWindow;
BOOL threadsexitflag=FALSE;
BOOL blitterexit=FALSE;


int main()
{
    UCHAR red, green, blue;
    APIRET rc;
	int bpp;

    pbttv=&bttv;
    TVParseConfig(&cfg);
    LogLevel=cfg.loglevel;
    strcpy(LogFile,cfg.logname);

    rc=BTFindCard(pbttv);
    if (rc>0) return 4;
    if (cfg.iscolor) bpp=2; else bpp=1;

    rc=BTAllocBuffers(pbttv,cfg.videowidth,cfg.videoheight,bpp);
    if (rc!=NO_ERROR) {dprintf(LOG_ERROR,"Allocate frames failed\n"); return 4;}

    BTSelectCard(pbttv,cfg.cardtype);
    if (cfg.tunertype>0) BTSelectTuner(pbttv,cfg.tunertype);
    if (cfg.cardtype==0) BTSelectTuner(pbttv,cfg.tunertype);
    BTReset(pbttv);
    BTDisableInt(pbttv,0xffffff);
    BTWriteRegister(pbttv,GPIO_OUT_EN,pbttv->pcard->gpiomask,4);


    TVShowWindow mainWindow(WND_MAIN);
    pmainWindow=&mainWindow;
    mainWindow.setFocus();
    red=(UCHAR)((cfg.colorkey&0xff0000)>>16);
    green=(UCHAR)((cfg.colorkey&0xff00)>>8);
    blue=(UCHAR)(cfg.colorkey&0xff);
    mainWindow.client()->setBackgroundColor(IColor(red,green,blue));
    mainWindow.show();

//    BTSelectInput(pbttv,cfg.connector);

    TVSetSaturation(pbttv,cfg.saturation);
    TVSetContrast(pbttv,cfg.contrast);
    TVSetBrightness(pbttv,cfg.brightness);


//    TVMuxControl(pbttv,AMUX_TUNER);
    rc=BTSelectColorSystem(pbttv,cfg.colorsystem);
    if (!cfg.iscolor)
       {
          BTSetColorFormat(pbttv,BT_COLOR_GS,0);
//          fccColorFormat = FOURCC_GREY;
          }
        else {
                if ((cfg.blittermode==1)||(cfg.blittermode==2))
                   {
//                      fccColorFormat = FOURCC_Y422;
                      BTSetColorFormat(pbttv,BT_COLOR_YUV2422,0);
                      }
                   else {
//                      fccColorFormat = FOURCC_R565;
                      BTSetColorFormat(pbttv,BT_COLOR_RGB16,0);
                      }
//              BTWriteRegister(COLOR_CTL,0x43,1);
              }

    BTMakeProgramFrame(pbttv);
    dprintf(LOG_DEBUG,"program start phys %08xh\n",pbttv->ulRiscPhys);
    rc=BTStartCapture(pbttv,pbttv->ulRiscPhys, FALSE, FALSE, 0);
//    BTRiscDump(pbttv->ulRiscPhys,pbttv-pRiscVirt);


    IPoint position, position2;
    position=mainWindow.position();
    position2.setX(position.x()+cfg.windowwidth);
    position2.setY(position.y()+cfg.windowheight);
    mainWindow.moveSizeToClient(IRectangle(position,position2));
    mainWindow.setInfoText();

    mainWindow.runBlitterThread();
    mainWindow.runPipeHandler();

    rc=TVReadChannelList(pbttv,CHANNELFILENAME);
    if (rc==0) mainWindow.setChannel(1);

    IApplication::current().run();
    threadsexitflag=TRUE;
    do {}
        while (blitterexit==FALSE);

    TVMuxControl(pbttv,AMUX_MUTE);
    BTStopCapture(pbttv,FALSE, 0);
    BTDisableInt(pbttv,0xffffff);
    BTFreeBuffers(pbttv);
    BTFreeCard(pbttv);
    return 0;
} /* end main */

TVShowWindow :: TVShowWindow(unsigned long windowId)
  : IFrameWindow(IFrameWindow::defaultStyle() |
                 IFrameWindow::minimizedIcon |
                 IFrameWindow::accelerator,
                 windowId)
   ,clientCanvas(WND_CANVAS, this, this)
   ,menuBar(windowId, this)
   ,menuPopUp(windowId, this)
   ,infoArea(this)
   ,commandHandler(this)
   ,mouseHandler(this)
   ,osdProgram(ST_OSDPROGRAM,&clientCanvas,&clientCanvas)
   ,AUserMessageHandler( WM_MOVE )
{
    isWindowed=TRUE;
    isColorbar=false;
    if (pbttv->pcard->mspAddr==0)
    {
        menuBar.enableItem(IDM_MSPSTATE, false);
        menuPopUp.enableItem(IDM_MSPSTATE, false);
        menuBar.enableItem(IDM_SOUNDMODE, false);
        menuPopUp.enableItem(IDM_SOUNDMODE, false);
    }
    clientCanvas.enableDragLines();
    setClient(&clientCanvas);
    setInfoText();
    osdProgram.setText("Test");
    osdProgram.setAlignment(IStaticText::centerRight);
    osdProgram.setForegroundColor(IColor(IColor::green));
    //OSD testing
//    clientCanvas.addToCell(&osdProgram,1,1);
    //setup command handler
    deinterlaceSelect=normal;
    switch (cfg.deinterlace)
    {
        case 0:
            setDeinterlace(normal);
            break;
        case 1:
            setDeinterlace(bob);
            break;
        case 2:
            setDeinterlace(weave);
            break;
    }
    videoInputSelect=tuner;
    audioInputSelect=tv;
    switch (cfg.connector)
    {
        case 0:
            setVideoInput(tuner);
            setAudioInput(tv);
            break;
        case 1:
            setVideoInput(linein);
            setAudioInput(external);
            break;
        case 2:
            setVideoInput(svhs);
            setAudioInput(external);
            break;
        case 3:
            setVideoInput(ext);
            setAudioInput(external);

    }
//MSP init
    if (pbttv->pcard->mspAddr!=0) //msp present
    {
        dprintf(LOG_DEBUG,"MSP34XX reset on pin 5 \n");
        BTWriteRegister(&bttv,GPIO_OUT_EN,0x23,4);
        BTWriteRegister(&bttv,GPIO_DATA,0x20,4);
        MSPInit(&bttv,5);
        DosSleep(100);
        MSPReadId(&bttv,0x80);
        /* Set volume to 0db gain */
        MSPSetLoudspeakerVolume(&bttv,0x730,CLIP_VOLUME);
        MSPSetAudioMode(&bttv, AUDIO_AUTODETECT);
        MSPSetNICAMFMMode(&bttv, NICAM_FM_AUTO);
        DosSleep(500);
        setAudioSystem(AUDIO_DK_NICAM_FM);
    }
    disableUnusedMenu();
    ((AUserMessageHandler *)this)->handleEventsFor( this );
    commandHandler.handleEventsFor(this);
    mouseHandler.handleEventsFor(this);

}
//destructor
TVShowWindow::~TVShowWindow()                  //Destructor for this class       V2
{
    //stop command handler
    commandHandler.stopHandlingEventsFor(this);
}
IBase::Boolean TVShowWindow::userMessage( IEvent& evt )
{
//    dprintf(LOG_DEBUG,"user event %x \n",evt.eventId());
    ((TVVideoWindow*)(client()))->UpdateBlitter();
    return true;
}

void TVShowWindow::sayError(char * error)
{
    IString errorstr("TVShow/2: ");
    IMessageBox msgBox(this);
    errorstr+=error;
    msgBox.show(errorstr,IMessageBox::catastrophic);
}
void TVShowWindow::runBlitterThread()
{
   clientCanvas.runBlitterThread();
}
void TVShowWindow::runPipeHandler()
{
   pipehandlerthread.start(&PipeThread,NULL,TRUE);
}
void TVShowWindow::disableUnusedMenu()
{
    menuBar.enableItem(IDM_RECSTREAM, false);
    menuPopUp.enableItem(IDM_RECSTREAM, false);
    menuBar.enableItem(IDM_RECOPTIONS, false);
    menuPopUp.enableItem(IDM_RECOPTIONS, false);
    menuBar.enableItem(IDM_AUDIOSETTING, false);
    menuPopUp.enableItem(IDM_AUDIOSETTING, false);
}
TVCommandHandler::TVCommandHandler(TVShowWindow *tvshowFrame)
{
    frame=tvshowFrame;
}
Boolean TVCommandHandler::command(ICommandEvent &cmdEvent)
{
    Boolean eventProcessed(true); //Event will be processed
    switch(cmdEvent.commandId())
    {
        case IDM_HWVIDEOTEST:
            HWVideoTest();
            break;
        case IDM_NEXTCHANNEL:
            frame->nextChannel();
            break;
        case IDM_PREVISIOUSCHANNEL:
            frame->prevChannel();
            break;
        case IDM_LOADCHANNELLIST:
            TVReadChannelList(pbttv,CHANNELFILENAME);
            break;
        case IDM_SAVECHANNELLIST:
            TVWriteChannelList(pbttv,CHANNELFILENAME);
            break;
        case IDC_INCBRIGHT:
            TVIncBrightness(pbttv);
            frame->setInfoText();
            break;
        case IDC_DECBRIGHT:
            TVDecBrightness(pbttv);
            frame->setInfoText();
            break;
        case IDC_INCCONTRAST:
            TVIncContrast(pbttv);
            frame->setInfoText();
            break;
        case IDC_DECCONTRAST:
            TVDecContrast(pbttv);
            frame->setInfoText();
            break;
        case IDC_INCSATURATION:
            TVIncSaturation(pbttv);
            frame->setInfoText();
            break;
        case IDC_DECSATURATION:
            TVDecSaturation(pbttv);
            frame->setInfoText();
            break;
        case IDM_SETTINGS:
            frame->videoSettings();
            frame->setInfoText();
            break;
        case IDM_SCANCHANNELS:
            frame->scanChannels();
            break;
        case IDM_TUNER:
            frame->setVideoInput(TVShowWindow::tuner);
            break;
        case IDM_LINEIN:
            frame->setVideoInput(TVShowWindow::linein);
            break;
        case IDM_SVHS:
            frame->setVideoInput(TVShowWindow::svhs);
            break;
        case IDM_EXT:
            frame->setVideoInput(TVShowWindow::ext);
            break;
        case IDM_AUDIOINPUT0:
            frame->setAudioInput(TVShowWindow::tv);
            break;
        case IDM_AUDIOINPUT1:
            frame->setAudioInput(TVShowWindow::external);
            break;
        case IDM_AUDIOINPUT2:
            frame->setAudioInput(TVShowWindow::internal);
            break;
        case IDM_AUDIOINPUT3:
            frame->setAudioInput(TVShowWindow::mute);
            break;
        case IDM_BGNICAM:
            frame->setAudioSystem(AUDIO_BG_NICAM_FM);
            break;
        case IDM_DKNICAM:
            frame->setAudioSystem(AUDIO_DK_NICAM_FM);
            break;
        case IDM_LNICAM:
            frame->setAudioSystem(AUDIO_L_NICAM_AM);
            break;
        case IDM_INICAM:
            frame->setAudioSystem(AUDIO_I_NICAM_FM);
            break;
        case IDM_MFM:
            frame->setAudioSystem(AUDIO_M_Dual_FM);
            break;
        case IDM_BGFM:
            frame->setAudioSystem(AUDIO_BG_Dual_FM);
            break;
        case IDM_DK1FM:
            frame->setAudioSystem(AUDIO_DK1_Dual_FM);
            break;
        case IDM_DK2FM:
            frame->setAudioSystem(AUDIO_DK2_Dual_FM);
            break;
        case IDM_MSPSTATE:
            dprintf(LOG_DEBUG,"audiomode %x \n",MSPReadAudioStatus(pbttv));
            dprintf(LOG_DEBUG,"C_AD_BITS %x \n",MSPReadCADBits(pbttv));
            dprintf(LOG_DEBUG,"ERROR RATE %x \n",MSPReadErrorRate(pbttv));
            dprintf(LOG_DEBUG,"channel matrix %x \n",MSPRead(pbttv,pbttv->pcard->mspAddr,0x12,0x0008));
            break;
        case IDM_FULLSCREEN:
            if (frame->isWindowed) frame->goFullscreen();
                else frame->goWindowed();
            break;
        case IDM_COLORBARS:
            if (frame->isColorbar) frame->setColorBar(false);
                else frame->setColorBar(true);
            break;
        case IDM_DEINTOFF:
            frame->setDeinterlace(normal);
            break;
        case IDM_DEINTBOB:
            frame->setDeinterlace(bob);
            break;
        case IDM_DEINTWEAVE:
            frame->setDeinterlace(weave);
            break;
        case IDM_RECFRAME:
            frame->saveFrame();
            break;

        default:
            eventProcessed=false;
    }
    return(eventProcessed);
}
TVMouseHandler::TVMouseHandler(TVShowWindow *tvshowFrame)
{
    frame=tvshowFrame;
}
Boolean TVMouseHandler::mouseClicked(IMouseClickEvent &clickEvent)
{
    Boolean eventProcessed(true); //Event will be processed
    if (clickEvent.mouseButton()==IMouseClickEvent::button2 &&
        clickEvent.mouseAction()==IMouseClickEvent::up)
        frame->showPopUp(clickEvent.mousePosition());
    else
        eventProcessed=false;
    return(eventProcessed);
}

TVVSizeHandler::TVVSizeHandler(TVVideoWindow *videoWindow)
{
    videowindow=videoWindow;
}
Boolean TVVSizeHandler::windowResize(IResizeEvent &resizeEvent)
{
    int height,width;
    Boolean eventProcessed(true); //Event will be processed
    dprintf(LOG_DEBUG,"Size Handler called \n");
    videowindow->windowSize=resizeEvent.newSize();
    height=videowindow->windowSize.height();
    width=videowindow->windowSize.width();
    dprintf(LOG_DEBUG,"width %d ,height  %d\n",width,height);
/* resizing onlu to 4:3
    if ((width*3)>(height*4))
    {
        width=(height*4/3);
        videowindow->windowSize.setWidth(width);
    }
    if ((width*3)<(height*4))
    {
        height=(width*3/4);
        videowindow->windowSize.setHeight(height);
    }

    videowindow->sizeTo(videowindow->windowSize);
    ((TVShowWindow*)videowindow->parent())->moveSizeToClient(videowindow->rect());
    dprintf(LOG_DEBUG,"newwidth %d ,newheight  %d\n",width,height);
    */
    videowindow->UpdateBlitter();
//    eventProcessed=false;
    return(eventProcessed);
}
IBase::Boolean TVVTimeHandler::tick(IEvent& evt)
{
APIRET rc;
ULONG intstat;
    intstat=BTReadRegister(pbttv,INT_STAT,4,rc);
//    dprintf(LOG_DEBUG,"Tick Handler called, INT_STAT=%X \n",intstat);
    if (intstat&BT848_INT_SCERR)
    {
        dprintf(LOG_DEBUG,"Tick Handler called, SCERR! INT_STAT=%X \n",intstat);
        BTWriteRegister(pbttv,INT_STAT,(intstat&(~BT848_INT_SCERR)),4);
        BTStopCapture(pbttv,FALSE, 0);
        DosBeep(100,100);
        BTStartCapture(pbttv,pbttv->ulRiscPhys, FALSE, FALSE, 0);

    }
//    HWVideoBlit(pbttv,(PVOID)buf);
    return(true);
}
TVVideoWindow :: TVVideoWindow(unsigned long windowId,IWindow* parent, IWindow* owner)
  : IMultiCellCanvas(windowId, parent, owner)
  ,sizeHandler(this)

{
    memset(&Setup,0,sizeof(Setup));
    memset(&OverlayCaps,0,sizeof(OverlayCaps));
    InitBlitter();
    sizeHandler.handleEventsFor(this);
    //timeHandler.handleEventsFor(this);

}
//destructor
TVVideoWindow::~TVVideoWindow()                  //Destructor for this class       V2
{
    sizeHandler.stopHandlingEventsFor(this);
    blitterthread.stop();
    EndBlitter();
}
void TVVideoWindow::InitBlitter()
{
    ULONG fRC;
    IString error;
    if (cfg.blittermode>1) //hwendive used
    {
        if (!LoadOverlay())
        {
            ((TVShowWindow*)parent())->sayError(" HWVideo not present");
            dprintf(LOG_ERROR,"HWVideo not present.\n");
            return;
        }
        fRC=HWVideoInit();
        if (fRC)
        {
            error=RCDecode(fRC);
            error+=" Can't init overlay!";
            ((TVShowWindow*)parent())->sayError(error);
            dprintf(LOG_ERROR,"HWVideo error: %s.\n",error);
            return;
        }
        fRC=HWVideoCaps(&OverlayCaps);
        if (fRC)
        {
            error=RCDecode(fRC);
            error+=" Can't get overlay capabilities!";
            ((TVShowWindow*)parent())->sayError(error);
            dprintf(LOG_ERROR,"HWVideo error: %s.\n",error);
            return;
        }
    }
}
void TVVideoWindow::EndBlitter()
{
    HWVideoClose(&Setup,&OverlayCaps);
}
ULONG TVVideoWindow::UpdateBlitter()
{
    Setup.ulLength=sizeof(HWVIDEOSETUP); //structure version checking
    Setup.szlSrcSize.cx=cfg.videowidth;  //source width
    Setup.szlSrcSize.cy=cfg.videoheight; //source height
    Setup.fccColor=FOURCC_Y422;          //source colorspace
    Setup.rctlSrcRect.xLeft=0;
    Setup.rctlSrcRect.yTop=0;
    if ((blitterMode==normal)||(blitterMode==weave)) //normal and weave use dual frame
    {
        Setup.rctlSrcRect.yBottom=cfg.videoheight-1;
    }
    if ((blitterMode==bob)) //bob use single frame
    {
        Setup.rctlSrcRect.yBottom=cfg.videoheight/2-1;
    }
    Setup.rctlSrcRect.xRight=cfg.videowidth-1;
      // calculate requered HW-dependent scanline aligment
    Setup.ulSrcPitch=(cfg.videowidth*2+OverlayCaps.ulScanAlign)&~OverlayCaps.ulScanAlign;
      // Determine keying color
      // We need to separate two cases:
      // screen in 256 color (indexed) and 15,16,24,32 bpp
      // if indexed colorspace used, we need to send index as KeyColor
    Setup.ulKeyColor=cfg.colorkey;
    IRectangle rect;
    IPoint point;
    rect=this->rect();
    point=mapPoint(rect.minXMinY(),parent()->handle(),desktopWindow()->handle());
    rect.moveTo(point);
    Setup.rctlDstRect=rect.asRECTL();
    return HWVideoSetup(&Setup);

}
void TVVideoWindow::blit()
{
	int fps=0;
	time_t ltime,ltime2;
	ULONG intstat;
    APIRET rc;
    BOOL bEvenField;

    time(&ltime);
    do
    {

//        intstat=BTReadRegister(pbttv,DSTATUS,1,rc);
//        dprintf(LOG_DEBUG,"Tick Handler called, DSTATUS=%X \n",intstat);
    	intstat=BTReadRegister(pbttv,INT_STAT,4,rc);
//        dprintf(LOG_DEBUG,"Tick Handler called, INT_STAT=%X \n",intstat);
//        bEvenField=intstat&BT848_INT_FIELD;
//        dprintf(LOG_DEBUG,"Tick Handler called, Even Field=%d \n",bEvenField);

    if (intstat&BT848_INT_SCERR)
    {
        dprintf(LOG_ERROR,"Blit::SCERR! INT_STAT=%X \n",intstat);
        BTWriteRegister(pbttv,INT_STAT,(BT848_INT_SCERR),4);
        BTStopCapture(pbttv,FALSE, 0);
        DosBeep(100,100);
        BTStartCapture(pbttv,pbttv->ulRiscPhys, FALSE, FALSE, 0);
    }
    BTWaitFrame(pbttv);
	intstat=BTReadRegister(pbttv,INT_STAT,4,rc);
//  dprintf(LOG_DEBUG,"Tick Handler called, INT_STAT=%X \n",intstat);
    bEvenField=((intstat&BT848_INT_FIELD));
//    dprintf(LOG_DEBUG,"Tick Handler called, Even Field=%d \n",bEvenField);
        switch (blitterMode)
        {
            case normal:
                if (bEvenField)
                {
                    HWVideoBlitNormal(pbttv,bEvenField);
// debug FPS print
    	            fps++;
        	        time(&ltime2);
            	    if (ltime2>=ltime+1)
                    {
                    	dprintf(LOG_DEBUG,"FPS: %i \n",fps);
	                    fps=0;
    	                ltime=ltime2;
                    }
                }
                break;
            case bob:
                HWVideoBlitBob(pbttv,bEvenField);
// debug FPS print
    	        fps++;
            	time(&ltime2);
    	        if (ltime2>=ltime+1)
                {
                    dprintf(LOG_DEBUG,"FPS: %i \n",fps);
    	            fps=0;
            	    ltime=ltime2;
                }
                break;
            case weave:
                HWVideoBlitWeave(pbttv,bEvenField);
// debug FPS print
    	        fps++;
            	time(&ltime2);
    	        if (ltime2>=ltime+1)
                {
                    dprintf(LOG_DEBUG,"FPS: %i \n",fps);
    	            fps=0;
            	    ltime=ltime2;
                }
                break;
        }
    }
    while (threadsexitflag==FALSE);
    blitterexit=TRUE;
}
void TVVideoWindow::runBlitterThread()
{
   blitterthread.start(new IThreadMemberFn<TVVideoWindow>(*this,TVVideoWindow::blit));
   blitterthread.setPriority(IApplication::timeCritical,15);

}
void TVVideoWindow::setDeinterlace(Deinterlace deinterlace)
{
    blitterMode=deinterlace;
    UpdateBlitter();
}
void TVShowWindow::goFullscreen()
{
    windowed=this->rect();
    this->moveSizeToClient(this->desktopWindow()->rect());
    isWindowed=FALSE;
    menuPopUp.checkItem(IDM_FULLSCREEN,true);
    menuBar.checkItem(IDM_FULLSCREEN,true);
}
void TVShowWindow::goWindowed()
{
    this->moveSizeTo(windowed);
    isWindowed=TRUE;
    menuPopUp.checkItem(IDM_FULLSCREEN,false);
    menuBar.checkItem(IDM_FULLSCREEN,false);
}
void TVShowWindow::showPopUp(const IPoint& atLocation)
{
    menuPopUp.show(atLocation);
}

TVShowWindow & TVShowWindow::videoSettings()
{
    TVVideoDialog videoDialog(this);
    videoDialog.showModally();
    if (videoDialog.result() == DID_OK)
    {
        TVUpdateCurrentChannel(pbttv);
    }
	return (*this);                       //Return a reference to the frame
}
TVShowWindow & TVShowWindow::scanChannels()
{

    TVScanDialog scanDialog(this);
    scanDialog.showModally();
    if (scanDialog.result() == DID_OK)
    {
//        TVUpdateCurrentChannel(pbttv);
    }
  return (*this);                       //Return a reference to the frame
}

void TVShowWindow::setInfoText()
{
    char tempstr[255];
    memset(tempstr,0,255);
    sprintf(tempstr, "Name: %s Bright: %d Contrast: %d Saturation: %d Volume: %d Freq: %.4f",
    pbttv->channelname, pbttv->brightness, pbttv->contrast, pbttv->saturation, pbttv->volume, (double) pbttv->tuner.frequency/1000000);
    infoArea.setInactiveText(tempstr);
}
void TVShowWindow::setColorBar(Boolean state)
{
    BTColorBar(pbttv, state);
    isColorbar=state;
    menuPopUp.checkItem(IDM_COLORBARS,state);
    menuBar.checkItem(IDM_COLORBARS,state);
}
void TVShowWindow::setAudioSystem(UCHAR system)
{
    ULONG item=0;
    switch (pbttv->audiosystem)
    {
        case AUDIO_BG_NICAM_FM:
            item=IDM_BGNICAM;
            break;
        case AUDIO_DK_NICAM_FM:
            item=IDM_DKNICAM;
            break;
        case AUDIO_L_NICAM_AM:
            item=IDM_LNICAM;
            break;
        case AUDIO_I_NICAM_FM:
            item=IDM_INICAM;
            break;
        case AUDIO_M_Dual_FM:
            item=IDM_MFM;
            break;
        case AUDIO_BG_Dual_FM:
            item=IDM_BGFM;
            break;
        case AUDIO_DK1_Dual_FM:
            item=IDM_DK1FM;
            break;
        case AUDIO_DK2_Dual_FM:
            item=IDM_DK2FM;
            break;
    }
    if (item!=0)
    {
        menuPopUp.checkItem(item,false);
        menuBar.checkItem(item,false);
    }
    TVSetAudioSystem(pbttv, system);
    switch (pbttv->audiosystem)
    {
        case AUDIO_BG_NICAM_FM:
            item=IDM_BGNICAM;
            break;
        case AUDIO_DK_NICAM_FM:
            item=IDM_DKNICAM;
            break;
        case AUDIO_L_NICAM_AM:
            item=IDM_LNICAM;
            break;
        case AUDIO_I_NICAM_FM:
            item=IDM_INICAM;
            break;
        case AUDIO_M_Dual_FM:
            item=IDM_MFM;
            break;
        case AUDIO_BG_Dual_FM:
            item=IDM_BGFM;
            break;
        case AUDIO_DK1_Dual_FM:
            item=IDM_DK1FM;
            break;
        case AUDIO_DK2_Dual_FM:
            item=IDM_DK2FM;
            break;
    }
    menuPopUp.checkItem(item,true);
    menuBar.checkItem(item,true);
}
void TVShowWindow::prevChannel(void)
{
    TVSelectPrevChannel(pbttv);
    setInfoText();
    setAudioInput(audioInputSelect);
}
void TVShowWindow::nextChannel(void)
{
    TVSelectNextChannel(pbttv);
    setInfoText();
    setAudioInput(audioInputSelect);
}
APIRET TVShowWindow::setChannel(UCHAR channel)
{
APIRET rc;
    rc=TVSelectChannel(pbttv,channel);
    if (rc!=NO_ERROR) return -1;
    setInfoText();
    setAudioInput(audioInputSelect);
    return NO_ERROR;
}
APIRET TVShowWindow::setBrightness(UCHAR brightness)
{
APIRET rc;
    rc=TVSetBrightness(pbttv,brightness);
    if (rc!=NO_ERROR) return -1;
    setInfoText();
    return NO_ERROR;
}
APIRET TVShowWindow::setContrast(UCHAR contrast)
{
APIRET rc;
    rc=TVSetContrast(pbttv,contrast);
    if (rc!=NO_ERROR) return -1;
    setInfoText();
    return NO_ERROR;
}
APIRET TVShowWindow::setSaturation(UCHAR saturation)
{
APIRET rc;
    rc=TVSetSaturation(pbttv,saturation);
    if (rc!=NO_ERROR) return -1;
    setInfoText();
    return NO_ERROR;
}
APIRET TVShowWindow::setVolume(UCHAR volume)
{
APIRET rc;
    rc=TVSetVolume(pbttv,volume);
    if (rc!=NO_ERROR) return -1;
    setInfoText();
    return NO_ERROR;
}
void TVShowWindow::setDeinterlace(Deinterlace deinterlace)
{
//    deinterlaceMode=deinterlace;
    clearDeinterlaceMenu();
    deinterlaceSelect=deinterlace;
    setDeinterlaceMenu();
    clientCanvas.setDeinterlace(deinterlace);
//    clientCanvas.setDeinterlace(deinterlace);
}
void TVShowWindow::clearDeinterlaceMenu()
{
    switch (deinterlaceSelect)
    {
        case normal:
            menuPopUp.checkItem(IDM_DEINTOFF,false);
            menuBar.checkItem(IDM_DEINTOFF,false);
            break;
        case bob:
            menuPopUp.checkItem(IDM_DEINTBOB,false);
            menuBar.checkItem(IDM_DEINTBOB,false);
            break;
        case weave:
            menuPopUp.checkItem(IDM_DEINTWEAVE,false);
            menuBar.checkItem(IDM_DEINTWEAVE,false);
            break;
    }
}
void TVShowWindow::setDeinterlaceMenu()
{
    switch (deinterlaceSelect)
    {
        case normal:
            menuPopUp.checkItem(IDM_DEINTOFF,true);
            menuBar.checkItem(IDM_DEINTOFF,true);
            break;
        case bob:
            menuPopUp.checkItem(IDM_DEINTBOB,true);
            menuBar.checkItem(IDM_DEINTBOB,true);
            break;
        case weave:
            menuPopUp.checkItem(IDM_DEINTWEAVE,true);
            menuBar.checkItem(IDM_DEINTWEAVE,true);
            break;
    }
}
void TVShowWindow::setVideoInput(VideoInput videoInput)
{
//    deinterlaceMode=deinterlace;
    clearVideoInputMenu();
    videoInputSelect=videoInput;
    setVideoInputMenu();
//    clientCanvas.setDeinterlace(deinterlace);
}
void TVShowWindow::clearVideoInputMenu()
{
    switch (videoInputSelect)
    {
        case tuner:
            menuPopUp.checkItem(IDM_TUNER,false);
            menuBar.checkItem(IDM_TUNER,false);
            break;
        case linein:
            menuPopUp.checkItem(IDM_LINEIN,false);
            menuBar.checkItem(IDM_LINEIN,false);
            break;
        case svhs:
            menuPopUp.checkItem(IDM_SVHS,false);
            menuBar.checkItem(IDM_SVHS,false);
            break;
        case ext:
            menuPopUp.checkItem(IDM_EXT,false);
            menuBar.checkItem(IDM_EXT,false);
            break;
    }
}
void TVShowWindow::setVideoInputMenu()
{
    switch (videoInputSelect)
    {
        case tuner:
            menuPopUp.checkItem(IDM_TUNER,true);
            menuBar.checkItem(IDM_TUNER,true);
            BTSelectInput(pbttv,0);
            break;
        case linein:
            menuPopUp.checkItem(IDM_LINEIN,true);
            menuBar.checkItem(IDM_LINEIN,true);
            BTSelectInput(pbttv,1);
            break;
        case svhs:
            menuPopUp.checkItem(IDM_SVHS,true);
            menuBar.checkItem(IDM_SVHS,true);
            BTSelectInput(pbttv,2);
            break;
        case ext:
            menuPopUp.checkItem(IDM_EXT,true);
            menuBar.checkItem(IDM_EXT,true);
            BTSelectInput(pbttv,3);
            break;
    }
}
void TVShowWindow::setAudioInput(AudioInput audioInput)
{
//    deinterlaceMode=deinterlace;
    clearAudioInputMenu();
    audioInputSelect=audioInput;
    setAudioInputMenu();
//    clientCanvas.setDeinterlace(deinterlace);
}
void TVShowWindow::clearAudioInputMenu()
{
    switch (audioInputSelect)
    {
        case tv:
            menuPopUp.checkItem(IDM_AUDIOINPUT0,false);
            menuBar.checkItem(IDM_AUDIOINPUT0,false);
            break;
        case external:
            menuPopUp.checkItem(IDM_AUDIOINPUT1,false);
            menuBar.checkItem(IDM_AUDIOINPUT1,false);
            break;
        case internal:
            menuPopUp.checkItem(IDM_AUDIOINPUT2,false);
            menuBar.checkItem(IDM_AUDIOINPUT2,false);
            break;
        case mute:
            menuPopUp.checkItem(IDM_AUDIOINPUT3,false);
            menuBar.checkItem(IDM_AUDIOINPUT3,false);
            break;
    }
}
void TVShowWindow::setAudioInputMenu()
{
    switch (audioInputSelect)
    {
        case tv:
            menuPopUp.checkItem(IDM_AUDIOINPUT0,true);
            menuBar.checkItem(IDM_AUDIOINPUT0,true);
            TVMuxControl(pbttv,AMUX_TUNER);
            break;
        case external:
            menuPopUp.checkItem(IDM_AUDIOINPUT1,true);
            menuBar.checkItem(IDM_AUDIOINPUT1,true);
            TVMuxControl(pbttv,AMUX_EXT);
            break;
        case internal:
            menuPopUp.checkItem(IDM_AUDIOINPUT2,true);
            menuBar.checkItem(IDM_AUDIOINPUT2,true);
            TVMuxControl(pbttv,AMUX_INT);
            break;
        case mute:
            menuPopUp.checkItem(IDM_AUDIOINPUT3,true);
            menuBar.checkItem(IDM_AUDIOINPUT3,true);
            TVMuxControl(pbttv,AMUX_MUTE);
            break;
    }
}
void TVShowWindow::saveFrame()
{
    PUCHAR pTempBuf;
    int bpp;
    time_t ltime;
    tm packtime;
    char filetext[255]="Testing \0x00";
    char filename[64]="test.jpg";
//    if (cfg.iscolor) bpp=2; else bpp=1;

    pTempBuf=(PUCHAR)malloc(cfg.videowidth*cfg.videoheight*2);
    if (pTempBuf==NULL)
    {
        dprintf(LOG_ERROR,"malloc for save buffer failed \n");
        return;
    }
    time(&ltime);
    packtime=*localtime(&ltime);
    sprintf(filename,"%02d%02d%02d%02d.jpg",packtime.tm_mday,packtime.tm_hour,packtime.tm_min,packtime.tm_sec);
    sprintf(filetext,"Frame saved %02d %02d:%02d:%02d",packtime.tm_mday,packtime.tm_hour,packtime.tm_min,packtime.tm_sec);
    BTCopyDualFrame(pbttv,pTempBuf);
    SaveFrameAsJPEG(filename, filetext,cfg.videowidth,cfg.videoheight,75, cfg.iscolor,pTempBuf);
    free(pTempBuf);
}

