/**************************************************************************

Game Editor - The Cross Platform Game Creation Tool
Copyright (C) 2009  Makslane Araujo Rodrigues

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

If GPL v3 does not work for you, we offer a "non-GPL" license 
<http://game-editor.com/License>, thats will help us to pay our developers 
<http://game-editor.com/Sharing_Software_Revenues_in_Open_Source>.

***************************************************************************/


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
