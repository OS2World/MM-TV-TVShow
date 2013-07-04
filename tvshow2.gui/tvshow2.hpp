/*
** Module   :TVSHOW.HPP
** Abstract :
**
** Copyright (C) Alex Cherkaev
**
** Log: Fri  17/01/2003 Created
**
*/
#ifndef __TVSHOW_HPP
#define __TVSHOW_HPP

#include <iframe.hpp>
#include <istattxt.hpp>
#include <imenubar.hpp>
#include <ipopmenu.hpp>
#include <icmdhdr.hpp>
#include <imoushdr.hpp>
#include <isizehdr.hpp>
#include <imcelcv.hpp>
#include <iinfoa.hpp>
#include <ithread.hpp>
#include "auwmhdr.hpp"
#include "atimehdr.hpp"

#include "woverlay.h"
//Forward declarations for other classes:
class TVShowWindow;
class TVVideoWindow;

class TVCommandHandler : public ICommandHandler
{
public:
    TVCommandHandler(TVShowWindow *tvshowFrame);
    virtual ~TVCommandHandler() {}
protected:
    virtual Boolean command(ICommandEvent& cmdEvent);
private:
    TVShowWindow * frame;
};
class TVMouseHandler : public IMouseHandler
{
public:
    TVMouseHandler(TVShowWindow *tvshowFrame);
    virtual ~TVMouseHandler() {}
protected:
    virtual Boolean mouseClicked(IMouseClickEvent& mouseClickEvent);
private:
    TVShowWindow * frame;
};

class TVVSizeHandler : public IResizeHandler
{
public:
    TVVSizeHandler(TVVideoWindow *videoWindow);
    virtual ~TVVSizeHandler() {}
protected:
    virtual Boolean windowResize(IResizeEvent& resizeEvent);
private:
    TVVideoWindow * videowindow;
};
class TVVTimeHandler:public ATimeHandler
{
protected:
    virtual IBase::Boolean tick(IEvent& evt);
};
enum Deinterlace {normal, bob, weave};

class TVVideoWindow : public IMultiCellCanvas
{
public:
    TVVideoWindow(unsigned long windowId,IWindow *parent, IWindow *owner);
    virtual ~TVVideoWindow();                  //Destructor for this class
    ISize windowSize;
    IPoint windowPos;
    HWVIDEOSETUP Setup;
    HWVIDEOCAPS  OverlayCaps;
    void InitBlitter();
    ULONG UpdateBlitter();
    void EndBlitter();
    void runBlitterThread();
    void setDeinterlace(Deinterlace deinterlace);
private:
    TVVSizeHandler sizeHandler;
    //TVVTimeHandler timeHandler;
    IThread blitterthread;
    Deinterlace blitterMode;
    void blit();


    TVVideoWindow(const TVVideoWindow&);  //Default copy constructor
    TVVideoWindow &operator=(const TVVideoWindow&);    //Default assignment operator
};

class TVShowWindow : public IFrameWindow, public AUserMessageHandler
{
public:
    TVShowWindow(unsigned long windowId);
    virtual ~TVShowWindow();                  //Destructor for this class
    void sayError(char *);
    void setInfoText();
    void runBlitterThread();
    void runPipeHandler();
    Boolean isWindowed, isColorbar;
    void goFullscreen();
    void goWindowed();
    void showPopUp(const IPoint& atLocation);
    void setColorBar(Boolean state);
    void setAudioSystem(UCHAR system);
    void prevChannel(void);
    void nextChannel(void);
    enum VideoInput {tuner, linein, svhs, ext};
    enum AudioInput {tv, external, internal, mute};
    void setDeinterlace(Deinterlace deinterlace);
    void setVideoInput(VideoInput videoInput);
    void setAudioInput(AudioInput audioInput);
    APIRET setChannel(UCHAR channel);
    APIRET setBrightness(UCHAR brightness);
    APIRET setContrast(UCHAR contrast);
    APIRET setSaturation(UCHAR saturation);
    APIRET setVolume(UCHAR volume);
    void saveFrame();
    virtual TVShowWindow &videoSettings();
    virtual TVShowWindow &scanChannels();

protected:
    Boolean userMessage( IEvent& evt );
    IThread pipehandlerthread;

private:
    TVVideoWindow clientCanvas;
    IMenuBar menuBar;
    IPopUpMenu menuPopUp;
    IInfoArea infoArea;
    TVCommandHandler commandHandler;
    TVMouseHandler mouseHandler;
    IRectangle windowed;
    IStaticText osdProgram;
    Deinterlace deinterlaceSelect;
    VideoInput videoInputSelect;
    AudioInput audioInputSelect;
    void clearDeinterlaceMenu();
    void setDeinterlaceMenu();
    void clearVideoInputMenu();
    void setVideoInputMenu();
    void clearAudioInputMenu();
    void setAudioInputMenu();
    void disableUnusedMenu();

//    Deinterlace deinterlaceMode;


    TVShowWindow(const TVShowWindow&);  //Default copy constructor
    TVShowWindow &operator=(const TVShowWindow&);    //Default assignment operator

};

#endif  /*__TVSHOW_HPP*/

