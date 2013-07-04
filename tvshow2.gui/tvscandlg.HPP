#include <iframe.hpp>
#include <icmdhdr.hpp>
#include <islider.hpp>
#include <isldahdr.hpp>
#include <iradiobt.hpp>
#include <icombobx.hpp>
#include <iselhdr.hpp>
#include <ientryfd.hpp>
#include <ithread.hpp>

#include "globals.h"


// #include <ipushbut.hpp>

class TVScanDialog;
class TVProgress;
class TVScanDialogCommandHandler : public ICommandHandler {
public:
    TVScanDialogCommandHandler(TVScanDialog *dialogFrame);
    virtual ~TVScanDialogCommandHandler() { }
protected:
    virtual Boolean command(ICommandEvent& cmdEvent);

private:
    TVScanDialog *frame;
};
class TVScanSelectCommandHandler : public ISelectHandler {
public:
    TVScanSelectCommandHandler(TVScanDialog *dialogFrame);
    virtual ~TVScanSelectCommandHandler() { }
protected:
    virtual Boolean selected(IControlEvent& selectEvent);

private:
    TVScanDialog *selectframe;
};
class TVProgress : public IProgressIndicator
{
public:
    TVProgress(ULONG windowId, IWindow* parent);
    virtual ~TVProgress(){}
    void setValue(UCHAR value);
};

class TVScanDialog : public IFrameWindow
{
public:
    TVScanDialog(IWindow * ownerWnd);
virtual ~TVScanDialog();                     //Destructor for this class
    void scan();
    void startscanthread();
    void stopscanthread();
    void addFoundChannels();
    enum scanModeEnum {Slow, Fast};
    void setScanMode(scanModeEnum scanMode);
  private:
    IThread scanthread;
    // scanfunref;
    TVScanDialogCommandHandler scandialogCommandHandler;
    TVScanSelectCommandHandler scanselectCommandHandler;
    TVProgress scanProgress;
    IEntryField channelsfound;
    IEntryField currentBand;
    IRadioButton slowScan, fastScan;
    IComboBox region;
    scanModeEnum scanningMode;
    Boolean scanningNow;
    UINT numchannels;
    TVCHANNEL foundchannels[200];

    TVScanDialog(const TVScanDialog&);    //Default copy constructor
TVScanDialog
    &operator=(const TVScanDialog&);     //Default assignment operator
};

