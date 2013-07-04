# makefile
# Created by IBM WorkFrame/2 MakeMake at 14:49:22 on 9 April 2004
#
# The actions included in this make file are:
#  Compile::C++ Compiler
#  Lib::Library Tool

.SUFFIXES:

.SUFFIXES: \
    .cpp .obj 

.cpp.obj:
    @echo " Compile::C++ Compiler "
    icc.exe /Id:\projects\warpdriver /Ti /Gm /Gd /C /Sp1 %s

{D:\Projects\tvshow2\tvshow2.lib}.cpp.obj:
    @echo " Compile::C++ Compiler "
    icc.exe /Id:\projects\warpdriver /Ti /Gm /Gd /C /Sp1 %s

all: \
    .\tvshow2.lib

.\tvshow2.lib: \
    .\tvfuncs.obj \
    .\tvaudio.obj \
    .\videocfg.obj \
    .\btfuncs.obj \
    .\bti2c.obj \
    .\bti2cs.obj \
    .\bttuner.obj \
    .\cfgfile.obj \
    .\driver.obj \
    .\fmfuncs.obj \
    .\logger.obj \
    .\maindata.obj \
    .\memory.obj \
    .\mspfunc.obj \
    .\pcifuncs.obj \
    .\phcard.obj
    @echo " Lib::Library Tool "
    ilib.exe  /nol tvshow2.lib @makefile.rf;

.\tvfuncs.obj: \
    D:\Projects\tvshow2\tvshow2.lib\tvfuncs.cpp \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}phcard.h \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}card.h \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}base.h \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}logger.h \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}btregs.h \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}btfuncs.h \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}bttuner.h \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}tvaudio.h \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}TVfuncs.h

.\tvaudio.obj: \
    D:\Projects\tvshow2\tvshow2.lib\tvaudio.cpp \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}phcard.h \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}card.h \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}base.h \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}logger.h \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}btregs.h \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}btfuncs.h \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}bti2c.h \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}bti2cs.h \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}tvaudio.h \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}mspfunc.h

.\phcard.obj: \
    D:\Projects\tvshow2\tvshow2.lib\phcard.cpp \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}phcard.h \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}base.h \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}logger.h

.\pcifuncs.obj: \
    D:\Projects\tvshow2\tvshow2.lib\pcifuncs.cpp \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}pcifuncs.h

.\mspfunc.obj: \
    D:\Projects\tvshow2\tvshow2.lib\mspfunc.cpp \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}phcard.h \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}card.h \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}base.h \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}logger.h \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}btregs.h \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}btfuncs.h \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}bti2cs.h \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}mspfunc.h

.\memory.obj: \
    D:\Projects\tvshow2\tvshow2.lib\memory.cpp \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}memory.h

.\maindata.obj: \
    D:\Projects\tvshow2\tvshow2.lib\maindata.cpp

.\logger.obj: \
    D:\Projects\tvshow2\tvshow2.lib\logger.cpp \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}logger.h

.\fmfuncs.obj: \
    D:\Projects\tvshow2\tvshow2.lib\fmfuncs.cpp \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}phcard.h \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}card.h \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}base.h \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}logger.h \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}btfuncs.h \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}bttuner.h \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}fmfuncs.h \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}tvaudio.h

.\driver.obj: \
    D:\Projects\tvshow2\tvshow2.lib\driver.cpp \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}base.h \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}logger.h \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}driver.h \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}ioctl.h

.\cfgfile.obj: \
    D:\Projects\tvshow2\tvshow2.lib\cfgfile.cpp \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}cfgfile.h \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}cfgbase.h

.\bttuner.obj: \
    D:\Projects\tvshow2\tvshow2.lib\bttuner.cpp \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}phcard.h \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}card.h \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}base.h \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}logger.h \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}bti2c.h \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}bttuner.h

.\bti2cs.obj: \
    D:\Projects\tvshow2\tvshow2.lib\bti2cs.cpp \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}phcard.h \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}card.h \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}base.h \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}logger.h \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}btregs.h \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}btfuncs.h \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}bti2cs.h

.\bti2c.obj: \
    D:\Projects\tvshow2\tvshow2.lib\bti2c.cpp \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}phcard.h \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}card.h \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}base.h \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}logger.h \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}btregs.h \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}btfuncs.h \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}bti2c.h

.\btfuncs.obj: \
    D:\Projects\tvshow2\tvshow2.lib\btfuncs.cpp \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}phcard.h \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}card.h \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}base.h \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}logger.h \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}btregs.h \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}btrisc.h \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}btfuncs.h \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}pcifuncs.h \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}pciregs.h \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}driver.h

.\videocfg.obj: \
    D:\Projects\tvshow2\tvshow2.lib\videocfg.cpp \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}phcard.h \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}card.h \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}cfgfile.h \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}videocfg.h \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}base.h \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}cfgbase.h \
    {D:\Projects\tvshow2\tvshow2.lib;d:\projects\warpdriver;$(INCLUDE);}logger.h
