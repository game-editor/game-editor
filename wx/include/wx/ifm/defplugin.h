/*!
    Defines the default plugin

    \file   defplugin.h
    \author Robin McNeill
    \date   Created: 02/24/05

    Copyright (c) Robin McNeill
    Licensed under the terms of the wxWindows license
*/

#ifndef _IFM_DEFPLUGIN_H_
#define _IFM_DEFPLUGIN_H_

#include "wx/ifm/plugin.h"

#define IFM_DEF_CAPTION_HEIGHT  15 // if wxSM_SYS_CAPTION isn't supported use this
#define IFM_DEF_FRAME_X         4  // if wxSY_FRAMEBORDER_X isn't supported use this

/*!
    Implements some default functionality.
    
    This plugin is added to every interface plugin unconditionally. It will be added
    so that it processes events after the interface plugin and all of its extensions.
*/
class wxIFMDefaultPlugin : public wxEvtHandler
{
private:
    wxIFMInterfacePluginBase *m_ip;
    wxInterfaceManager *GetManager() { return m_ip->GetManager(); }
    wxIFMInterfacePluginBase *GetIP() { return m_ip; }

public:

    wxIFMDefaultPlugin() { }

    wxIFMDefaultPlugin(wxIFMInterfacePluginBase *ip);

private:

    void OnSetRect(wxIFMRectEvent &event); //!< handles Set[Client|Background]Rect
    void OnGetRect(wxIFMRectEvent &event); //!< handles Get[Client|Background]Rect
    void OnConvertRect(wxIFMConvertRectEvent &event); //!< handles ConvertRect
    void OnUpdate(wxIFMUpdateComponentEvent &event); //!< handles UpdateComponent
    void OnBeginPaint(wxIFMBeginPaintEvent &event);
    void OnEndPaint(wxIFMEndPaintEvent &event);
    void OnPaintBg(wxIFMPaintEvent &event); //!< handles PaintBG
    void OnPaintBorder(wxIFMPaintEvent &event);
    void OnGetDesiredSize(wxIFMRectEvent &event);
    void OnSetDesiredSize(wxIFMRectEvent &event);
    void OnSetMinSize(wxIFMRectEvent &event);
    void OnSetMaxSize(wxIFMRectEvent &event);
    void OnGetMinSize(wxIFMRectEvent &event);
    void OnGetMaxSize(wxIFMRectEvent &event);
    void OnHitTest(wxIFMHitTestEvent &event);
    void OnUndock(wxIFMUndockEvent &event);
    void OnDeleteComponent(wxIFMDeleteComponentEvent &event);
    void OnSetChild(wxIFMSetChildEvent &event);
    void OnSetChildSize(wxIFMSetChildSizeEvent &event);
    void OnCalcRects(wxIFMCalcRectsEvent &event);
    void OnShowChild(wxIFMShowChildEvent &event);
    void OnShowComponent(wxIFMShowComponentEvent &event);
    void OnVisibilityChanged(wxIFMComponentVisibilityChangedEvent &event);
    void OnDock(wxIFMDockEvent &event);

#if IFM_CANFLOAT
    void OnFloat(wxIFMFloatEvent &event);
    void OnFloatingSize(wxIFMFloatingSizeEvent &event);
    void OnFloatingMove(wxIFMFloatingMoveEvent &event);
    void UpdateFloatingPos(wxIFMComponent *component);
    void OnFloatNotify(wxIFMFloatNotifyEvent &event);
#endif

    DECLARE_EVENT_TABLE()

    DECLARE_DYNAMIC_CLASS_NO_COPY(wxIFMDefaultPlugin)
};

#endif
