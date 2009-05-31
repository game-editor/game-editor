// PanelProperty.h: interface for the PanelProperty class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PANELPROPERTY_H__0EDFD25C_C2E7_4143_B9C2_0A554821D742__INCLUDED_)
#define AFX_PANELPROPERTY_H__0EDFD25C_C2E7_4143_B9C2_0A554821D742__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PanelGenericProperty.h"
#include "Behavior/wxJigsawEditor/wxJigsawEditorCanvas.h"

class PanelProperty : public PanelGenericProperty  
{
	DECLARE_CLASS(PanelProperty)
    DECLARE_EVENT_TABLE()

public:
	PanelProperty(wxWindow *parent);
	virtual ~PanelProperty();

	static void Update(Actor *_actor);
	static void UpdatePaths();
	
	static void UpdateActorPosition();
	static void UpdateActorSize();

	

private:
	void OnItemCollapsed ( wxPropertyGridEvent& event );
	void OnItemExpanded ( wxPropertyGridEvent& event );
	void OnPropertyGridChange ( wxPropertyGridEvent& event );
	void OnPropertyGridSelect ( wxPropertyGridEvent& event );
	void OnPropertyGridButtonClick(wxCommandEvent &event);
	void OnMenuClick(wxCommandEvent& event);
	void OnBehaviorBlockSelected( wxCommandEvent &event );

	void CreateMenu();

	void Update();
	void UpdateOptionalProperties(bool bBehavior = false);
	void UpdateParent();
	void UpdateCreateAtStartup();
	void UpdateInheritEvents();
	void UpdateCategoryNames();
	void UpdateUser();

	static PanelProperty *panelProperty;

	wxPGId	idGeneral, idPosition, idSize, idAppearance, idEvents, idUser,
			idName, idDescription, idValueBool, idValueString, idValueNumeric,
			idCreateAtStartup, idEditorAnimation, idX, idY, idZDepth, idParent, idPath,
			idLocked, idColor, idTransparency,
			idAnimation, idAnimationName, idAnimationFile, idAnimationMultiple,
			idAnimationHorizontalFrames, idAnimationVerticalFrames, idAnimationFps, idInfinite,
			idInheritEvents, idReceiveEventsEvenIfOutOfVision,
			idWidth, idHeight,			
			idBackgroundColor, 
			idGrid, idGridShow, idGridSnap, idGridSizeX, idGridSizeY,
			idColorGrid1, idColorGrid2, idGridZoom;

	void OnTimerEvent(wxTimerEvent &event);

	wxTimer m_TimerShapeEdit;


	Actor *actor;
	wxJigsawShape *shape;
	wxMenu *m_menuAnim, *m_menuPath;
};

#endif // !defined(AFX_PANELPROPERTY_H__0EDFD25C_C2E7_4143_B9C2_0A554821D742__INCLUDED_)
