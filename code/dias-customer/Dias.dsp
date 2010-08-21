# Microsoft Developer Studio Project File - Name="Dias" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Dias - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Dias.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Dias.mak" CFG="Dias - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Dias - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Dias - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Dias - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\Release\Dias"
# PROP Intermediate_Dir "..\..\Temp\Release\Dias"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "..\Common" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 ..\..\Release\Shared\aimzip.lib ..\..\Release\Shared\tesla.lib ..\..\Release\Shared\aimapi.lib Wsock32.lib /nologo /subsystem:windows /machine:I386 /libpath:"..\..\..\aim.inet\libs\\"

!ELSEIF  "$(CFG)" == "Dias - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\Release\Dias"
# PROP Intermediate_Dir "..\..\Temp\Debug\Dias"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GR /GX /ZI /Od /I "..\Common" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ..\..\Release\Shared\aimzip.lib ..\..\Release\Shared\tesla.lib ..\..\Release\Shared\aimapi.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Dias - Win32 Release"
# Name "Dias - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\3ImgFnDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AddImagesDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AddSurveyDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AimColorCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\AimComboCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\AimComboDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\controls\AimStatusBar.cpp
# ADD CPP /I "..\..\Dias"
# End Source File
# Begin Source File

SOURCE=.\CaptureDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Channel.cpp
# End Source File
# Begin Source File

SOURCE=.\CountDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DataWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\DensProp.cpp
# End Source File
# Begin Source File

SOURCE=.\Dias.cpp
# End Source File
# Begin Source File

SOURCE=.\Dias.rc
# End Source File
# Begin Source File

SOURCE=.\DiasAPIChannels.cpp
# End Source File
# Begin Source File

SOURCE=.\DiasAPIChnlRange.cpp
# End Source File
# Begin Source File

SOURCE=.\DiasAPIDensScales.cpp
# End Source File
# Begin Source File

SOURCE=.\DiasAPIDocRange.cpp
# End Source File
# Begin Source File

SOURCE=.\DiasAPIDocs.cpp
# End Source File
# Begin Source File

SOURCE=.\DiasAPIFields.cpp
# End Source File
# Begin Source File

SOURCE=.\DiasAPIFrames.cpp
# End Source File
# Begin Source File

SOURCE=.\DiasAPIGeomScales.cpp
# End Source File
# Begin Source File

SOURCE=.\DiasAPIImages.cpp
# End Source File
# Begin Source File

SOURCE=.\DiasAPIImgRange.cpp
# End Source File
# Begin Source File

SOURCE=.\DiasAPIMarks.cpp
# End Source File
# Begin Source File

SOURCE=.\DiasAPIMarkStyles.cpp
# End Source File
# Begin Source File

SOURCE=.\DiasAPIMrkRange.cpp
# End Source File
# Begin Source File

SOURCE=.\DiasAPIPatches.cpp
# End Source File
# Begin Source File

SOURCE=.\DiasAPIPtchRange.cpp
# End Source File
# Begin Source File

SOURCE=.\DiasAPISrvRange.cpp
# End Source File
# Begin Source File

SOURCE=.\DiasAPISurveis.cpp
# End Source File
# Begin Source File

SOURCE=.\DiasStatusBar.cpp
# End Source File
# Begin Source File

SOURCE=.\DocList.cpp
# End Source File
# Begin Source File

SOURCE=.\DocNewDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DocPane.cpp
# End Source File
# Begin Source File

SOURCE=.\DocRuler.cpp
# End Source File
# Begin Source File

SOURCE=.\Field.cpp
# End Source File
# Begin Source File

SOURCE=.\GeomProp.cpp
# End Source File
# Begin Source File

SOURCE=.\Helpers.cpp
# End Source File
# Begin Source File

SOURCE=.\Image.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ImgBtnCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\ImgPropBar.cpp
# End Source File
# Begin Source File

SOURCE=.\KrnlComboCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\KrnlComboDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\Mark.cpp
# End Source File
# Begin Source File

SOURCE=.\MarkComboCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\MarkComboDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MarkStyle.cpp
# End Source File
# Begin Source File

SOURCE=.\MenuCntDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MrkPropBar.cpp
# End Source File
# Begin Source File

SOURCE=.\NumEditCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\Patch.cpp
# End Source File
# Begin Source File

SOURCE=.\Report.cpp
# End Source File
# Begin Source File

SOURCE=.\ReportWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\Resources.cpp
# End Source File
# Begin Source File

SOURCE=.\Scales.cpp
# End Source File
# Begin Source File

SOURCE=.\ScaleWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\Settings.cpp
# End Source File
# Begin Source File

SOURCE=.\ShapeComboCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\ShapeComboDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SmallImgCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\Splash.cpp
# End Source File
# Begin Source File

SOURCE=.\StartDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StorageDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\StorageFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\StorageView.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\StringFile.cpp
# End Source File
# Begin Source File

SOURCE=.\Survey.cpp
# End Source File
# Begin Source File

SOURCE=.\TitlesDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TxtBtnCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\util.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\3ImgFnDlg.h
# End Source File
# Begin Source File

SOURCE=.\AddImagesDlg.h
# End Source File
# Begin Source File

SOURCE=.\AddSurveyDlg.h
# End Source File
# Begin Source File

SOURCE=.\AimColorCtrl.h
# End Source File
# Begin Source File

SOURCE=.\AimComboCtrl.h
# End Source File
# Begin Source File

SOURCE=.\AimComboDlg.h
# End Source File
# Begin Source File

SOURCE=..\Common\controls\AimStatusBar.h
# End Source File
# Begin Source File

SOURCE=.\CaptureDlg.h
# End Source File
# Begin Source File

SOURCE=.\Channel.h
# End Source File
# Begin Source File

SOURCE=.\CountDlg.h
# End Source File
# Begin Source File

SOURCE=.\DataWnd.h
# End Source File
# Begin Source File

SOURCE=.\DensProp.h
# End Source File
# Begin Source File

SOURCE=.\DensScale.h
# End Source File
# Begin Source File

SOURCE=.\Dias.h
# End Source File
# Begin Source File

SOURCE=.\diasapi.h
# End Source File
# Begin Source File

SOURCE=.\DiasAPIChannels.h
# End Source File
# Begin Source File

SOURCE=.\DiasAPIChnlRange.h
# End Source File
# Begin Source File

SOURCE=.\DiasAPIDensScales.h
# End Source File
# Begin Source File

SOURCE=.\DiasAPIDocRange.h
# End Source File
# Begin Source File

SOURCE=.\DiasAPIDocs.h
# End Source File
# Begin Source File

SOURCE=.\DiasAPIFields.h
# End Source File
# Begin Source File

SOURCE=.\DiasAPIFrames.h
# End Source File
# Begin Source File

SOURCE=.\DiasAPIGeomScales.h
# End Source File
# Begin Source File

SOURCE=.\DiasAPIImages.h
# End Source File
# Begin Source File

SOURCE=.\DiasAPIImgRange.h
# End Source File
# Begin Source File

SOURCE=.\DiasAPIMarks.h
# End Source File
# Begin Source File

SOURCE=.\DiasAPIMarkStyles.h
# End Source File
# Begin Source File

SOURCE=.\DiasAPIMrkRange.h
# End Source File
# Begin Source File

SOURCE=.\DiasAPIPatches.h
# End Source File
# Begin Source File

SOURCE=.\DiasAPIPtchRange.h
# End Source File
# Begin Source File

SOURCE=.\DiasAPISrvRange.h
# End Source File
# Begin Source File

SOURCE=.\DiasAPISurveis.h
# End Source File
# Begin Source File

SOURCE=.\DiasKs.h
# End Source File
# Begin Source File

SOURCE=.\DiasStatusBar.h
# End Source File
# Begin Source File

SOURCE=.\DocList.h
# End Source File
# Begin Source File

SOURCE=.\DocNewDlg.h
# End Source File
# Begin Source File

SOURCE=.\DocPane.h
# End Source File
# Begin Source File

SOURCE=.\DocRuler.h
# End Source File
# Begin Source File

SOURCE=.\Field.h
# End Source File
# Begin Source File

SOURCE=.\GeomProp.h
# End Source File
# Begin Source File

SOURCE=.\GeomScale.h
# End Source File
# Begin Source File

SOURCE=..\Include\hash.h
# End Source File
# Begin Source File

SOURCE=.\Helpers.h
# End Source File
# Begin Source File

SOURCE=.\Image.h
# End Source File
# Begin Source File

SOURCE=.\ImageCtrl.h
# End Source File
# Begin Source File

SOURCE=.\ImageWnd.h
# End Source File
# Begin Source File

SOURCE=.\ImgBtnCtrl.h
# End Source File
# Begin Source File

SOURCE=.\ImgPropBar.h
# End Source File
# Begin Source File

SOURCE=.\KrnlComboCtrl.h
# End Source File
# Begin Source File

SOURCE=.\KrnlComboDlg.h
# End Source File
# Begin Source File

SOURCE=.\MainFrame.h
# End Source File
# Begin Source File

SOURCE=.\Mark.h
# End Source File
# Begin Source File

SOURCE=.\MarkComboCtrl.h
# End Source File
# Begin Source File

SOURCE=.\MarkComboDlg.h
# End Source File
# Begin Source File

SOURCE=.\MarkStyle.h
# End Source File
# Begin Source File

SOURCE=..\Include\matrix.h
# End Source File
# Begin Source File

SOURCE=.\MenuCntDlg.h
# End Source File
# Begin Source File

SOURCE=.\MrkPropBar.h
# End Source File
# Begin Source File

SOURCE=.\NumEditCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Patch.h
# End Source File
# Begin Source File

SOURCE=..\Include\platform.h
# End Source File
# Begin Source File

SOURCE=.\Report.h
# End Source File
# Begin Source File

SOURCE=.\ReportWnd.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\Resources.h
# End Source File
# Begin Source File

SOURCE=.\ScaleWnd.h
# End Source File
# Begin Source File

SOURCE=.\Settings.h
# End Source File
# Begin Source File

SOURCE=.\ShapeComboCtrl.h
# End Source File
# Begin Source File

SOURCE=.\ShapeComboDlg.h
# End Source File
# Begin Source File

SOURCE=.\SmallImgCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Splash.h
# End Source File
# Begin Source File

SOURCE=.\StartDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\StorageDoc.h
# End Source File
# Begin Source File

SOURCE=.\StorageFrame.h
# End Source File
# Begin Source File

SOURCE=.\StorageView.h
# End Source File
# Begin Source File

SOURCE=..\Include\StringFile.h
# End Source File
# Begin Source File

SOURCE="\orlovsky\it\shared\c-include\strsafe.h"
# End Source File
# Begin Source File

SOURCE=.\Survey.h
# End Source File
# Begin Source File

SOURCE=.\TitlesDlg.h
# End Source File
# Begin Source File

SOURCE=..\Include\tree.h
# End Source File
# Begin Source File

SOURCE=.\TxtBtnCtrl.h
# End Source File
# Begin Source File

SOURCE=..\Include\util.h
# End Source File
# Begin Source File

SOURCE=..\Include\vector.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\button.ico
# End Source File
# Begin Source File

SOURCE=.\res\cm_bw.ico
# End Source File
# Begin Source File

SOURCE=.\res\cm_cmy.ico
# End Source File
# Begin Source File

SOURCE=.\res\cm_cmyk.ico
# End Source File
# Begin Source File

SOURCE=.\res\cm_gray.ico
# End Source File
# Begin Source File

SOURCE=.\res\cm_lab.ico
# End Source File
# Begin Source File

SOURCE=.\res\cm_rgb.ico
# End Source File
# Begin Source File

SOURCE=.\res\colormodels.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cur00001.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00002.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00003.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00004.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00005.cur
# End Source File
# Begin Source File

SOURCE=.\res\densscal.bmp
# End Source File
# Begin Source File

SOURCE=.\res\densscale.ico
# End Source File
# Begin Source File

SOURCE=.\res\denstype.bmp
# End Source File
# Begin Source File

SOURCE=".\res\DIAS Full Logo.bmp"
# End Source File
# Begin Source File

SOURCE=.\res\Dias.ico
# End Source File
# Begin Source File

SOURCE=.\res\Dias.rc2
# End Source File
# Begin Source File

SOURCE=.\res\doc_imag.bmp
# End Source File
# Begin Source File

SOURCE=.\res\doctypes.bmp
# End Source File
# Begin Source File

SOURCE=.\res\geomscal.bmp
# End Source File
# Begin Source File

SOURCE=.\res\geomscale.ico
# End Source File
# Begin Source File

SOURCE=.\res\gripper.ico
# End Source File
# Begin Source File

SOURCE=.\res\hand.cur
# End Source File
# Begin Source File

SOURCE=.\res\handshake.cur
# End Source File
# Begin Source File

SOURCE=.\res\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00002.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00003.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00004.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00005.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00006.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00007.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00008.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00009.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00010.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00011.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00012.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon_ima.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon_sur.ico
# End Source File
# Begin Source File

SOURCE=.\res\id_marke.bmp
# End Source File
# Begin Source File

SOURCE=.\res\il_docty.bmp
# End Source File
# Begin Source File

SOURCE=.\res\il_field.bmp
# End Source File
# Begin Source File

SOURCE=.\res\il_objec.bmp
# End Source File
# Begin Source File

SOURCE=.\res\indicato.ico
# End Source File
# Begin Source File

SOURCE=.\res\magnify.cur
# End Source File
# Begin Source File

SOURCE=.\res\markcrea.cur
# End Source File
# Begin Source File

SOURCE=.\res\marker_s.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mrk_cent.ico
# End Source File
# Begin Source File

SOURCE=.\res\mrk_ptr.cur
# End Source File
# Begin Source File

SOURCE=.\res\mrkcreat.ico
# End Source File
# Begin Source File

SOURCE=.\res\mrkkrnl_cross.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mrkkrnl_none.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mrkkrnl_round.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mrkkrnl_x.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mrkptr3.cur
# End Source File
# Begin Source File

SOURCE=.\res\mrkptr4.cur
# End Source File
# Begin Source File

SOURCE=.\res\mrkptr5.cur
# End Source File
# Begin Source File

SOURCE=.\res\mrkptr6.cur
# End Source File
# Begin Source File

SOURCE=.\res\mrkptr7.cur
# End Source File
# Begin Source File

SOURCE=.\res\mrkptr8.cur
# End Source File
# Begin Source File

SOURCE=.\res\mrkptr_m.cur
# End Source File
# Begin Source File

SOURCE=.\res\mrkptr_move.cur
# End Source File
# Begin Source File

SOURCE=.\res\mrkptr_movex.cur
# End Source File
# Begin Source File

SOURCE=.\res\mrkptr_movexrot.cur
# End Source File
# Begin Source File

SOURCE=.\res\mrkptr_movexy.cur
# End Source File
# Begin Source File

SOURCE=.\res\mrkptr_movexyrot.cur
# End Source File
# Begin Source File

SOURCE=.\res\mrkptr_movey.cur
# End Source File
# Begin Source File

SOURCE=.\res\mrkptr_moveyrot.cur
# End Source File
# Begin Source File

SOURCE=.\res\mrkptr_rot.cur
# End Source File
# Begin Source File

SOURCE=.\res\mrkptr_sel.cur
# End Source File
# Begin Source File

SOURCE=.\res\mrkshp_ellips.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mrkshp_none.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mrkshp_rect.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mrkshp_tri.bmp
# End Source File
# Begin Source File

SOURCE=.\res\pic_down.ico
# End Source File
# Begin Source File

SOURCE=.\res\pic_downarr.ico
# End Source File
# Begin Source File

SOURCE=.\res\pic_fill.ico
# End Source File
# Begin Source File

SOURCE=.\res\pic_hand.ico
# End Source File
# Begin Source File

SOURCE=.\res\pic_lock.ico
# End Source File
# Begin Source File

SOURCE=.\res\pic_offx.ico
# End Source File
# Begin Source File

SOURCE=.\res\pic_offy.ico
# End Source File
# Begin Source File

SOURCE=.\res\pic_prev.ico
# End Source File
# Begin Source File

SOURCE=.\res\pic_remo.ico
# End Source File
# Begin Source File

SOURCE=.\res\pic_rigt.ico
# End Source File
# Begin Source File

SOURCE=.\res\pic_rota.ico
# End Source File
# Begin Source File

SOURCE=.\res\pic_size.ico
# End Source File
# Begin Source File

SOURCE=.\res\pic_sizex.ico
# End Source File
# Begin Source File

SOURCE=.\res\pic_sizey.ico
# End Source File
# Begin Source File

SOURCE=.\res\pic_togg.ico
# End Source File
# Begin Source File

SOURCE=.\res\pic_unlo.ico
# End Source File
# Begin Source File

SOURCE=.\res\pic_up.ico
# End Source File
# Begin Source File

SOURCE=.\res\pic_zoom.ico
# End Source File
# Begin Source File

SOURCE=.\res\pointer.cur
# End Source File
# Begin Source File

SOURCE=.\res\pointer.ico
# End Source File
# Begin Source File

SOURCE=.\res\question.ico
# End Source File
# Begin Source File

SOURCE=.\res\rebar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\sarrows.cur
# End Source File
# Begin Source File

SOURCE=.\res\shapes.cur
# End Source File
# Begin Source File

SOURCE=.\res\shp_p.cur
# End Source File
# Begin Source File

SOURCE=.\res\splith.cur
# End Source File
# Begin Source File

SOURCE=.\Splsh16.bmp
# End Source File
# Begin Source File

SOURCE=.\res\StorageDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\tbp_image.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tbp_image_h.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tbp_mark.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tbx_doc.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tbx_doc_img.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tbx_doc_n.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tbx_doc_rep.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tbx_doc_srv.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tbx_images.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tbx_img_h.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tbx_img_n.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tbx_s.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tbx_std.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tbx_std_d.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tbx_std_h.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tbx_std_n.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tbx_std_s.bmp
# End Source File
# Begin Source File

SOURCE=.\res\text.cur
# End Source File
# Begin Source File

SOURCE=.\res\textcng.cur
# End Source File
# Begin Source File

SOURCE=.\res\textcreate.cur
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\trck4way.cur
# End Source File
# Begin Source File

SOURCE=.\res\ws_marker_center.ico
# End Source File
# Begin Source File

SOURCE=.\res\ws_marker_zoom.ico
# End Source File
# Begin Source File

SOURCE=".\res\x-ip-m.cur"
# End Source File
# Begin Source File

SOURCE=".\res\x-zi-as.cur"
# End Source File
# Begin Source File

SOURCE=".\res\x-zo-as.cur"
# End Source File
# Begin Source File

SOURCE=.\res\zoomout.cur
# End Source File
# End Group
# Begin Source File

SOURCE=.\Dias.reg
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section Dias : {72ADFD78-2C39-11D0-9903-00A0C91BC942}
# 	1:9:IDB_LOGO1:102
# 	2:21:SplashScreenInsertKey:4.0
# End Section
