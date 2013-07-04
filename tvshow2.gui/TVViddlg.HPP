#include <iframe.hpp>
#include <icmdhdr.hpp>
#include <islider.hpp>
#include <isldahdr.hpp>
#include <iradiobt.hpp>
#include <ipushbut.hpp>
#include <iselhdr.hpp>
#include <ientryfd.hpp>
#include "globals.h"
//#include <ipushbut.hpp>

class TVVideoDialog;
class TVSlider;

class TVVideoDialogCommandHandler : public ICommandHandler {                      //V4
public:
    TVVideoDialogCommandHandler(TVVideoDialog *dialogFrame);                          //V4
    virtual ~TVVideoDialogCommandHandler() { }                                        //V4
protected:                                                                  //V4
    virtual Boolean command(ICommandEvent& cmdEvent);

private:                                                                    //V4
    TVVideoDialog *frame;
};
class TVSelectCommandHandler : public ISelectHandler {                      //V4
public:
    TVSelectCommandHandler(TVVideoDialog *dialogFrame);                          //V4
    virtual ~TVSelectCommandHandler() { }                                        //V4
protected:                                                                  //V4
    virtual Boolean selected(IControlEvent& selectEvent);

private:                                                                    //V4
    TVVideoDialog *selectframe;
};

class TVSliderCommandHandler : public ISliderArmHandler {                      //V4
public:                                                                     //V4
    TVSliderCommandHandler(TVVideoDialog *dialogFrame);
    virtual ~TVSliderCommandHandler() { }                                        //V4
protected:                                                                  //V4
    virtual Boolean moving(IControlEvent& moveEvent);
    virtual Boolean positionChanged(IControlEvent& positionEvent);
private:
    TVVideoDialog *sliderframe;
};

class TVSlider : public ISlider
{
public:
    TVSlider(ULONG windowId, IWindow* parent);
    virtual ~TVSlider(){}
    UCHAR getValue();
    setValue(UCHAR value);
};

class TVVideoDialog : public IFrameWindow
{
public:
    TVVideoDialog(IWindow * ownerWnd);
virtual ~TVVideoDialog();
    void getSliders();
    void setVideoValues();
    void setOldVideoValues();
    void SetColorState(UCHAR color);
    void DisplayFreq(ULONG freq);
    void getText();
    void IncFreq();
    void DecFreq();
    ULONG GetFreq();
    void displayFreq();
  private:
    TVVideoDialogCommandHandler dialogCommandHandler;
    TVSliderCommandHandler sliderCommandHandler;
    TVSelectCommandHandler selectCommandHandler;
    TVSlider brightnessSlider;
    TVSlider contrastSlider;
    TVSlider saturationSlider;
    TVSlider volumeSlider;
    IRadioButton ntscmButton,ntscjButton,palButton,palmButton,palnButton,secamButton,palncButton;
    IEntryField channelName,channelFreq;
    IPushButton incFreq,decFreq;
    UCHAR oldBrightness,oldContrast,oldSaturation,oldVolume,oldColor;
    ULONG oldFreq, freq;
    IString oldName;

    TVVideoDialog(const TVVideoDialog&);    //Default copy constructor            V4
TVVideoDialog                                                                 //V4
    &operator=(const TVVideoDialog&);     //Default assignment operator         V4
};                                                                          //V4


