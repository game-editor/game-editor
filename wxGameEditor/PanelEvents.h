/**************************************************************************

Game Editor - The Cross Platform Game Creation Tool
Copyright (C) 2009  Makslane Araujo Rodrigues, http://game-editor.com

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
More info at http://game-editor.com/License
Be a Game Editor developer: http://game-editor.com/Sharing_Software_Revenues_in_Open_Source

***************************************************************************/


// PanelEvents.h: interface for the PanelEvents class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PANELEVENTS_H__721C4D92_A22B_4720_8A41_2FF2AAAD9C06__INCLUDED_)
#define AFX_PANELEVENTS_H__721C4D92_A22B_4720_8A41_2FF2AAAD9C06__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PanelGenericProperty.h"
#include "../gameEngine/Action.h"

enum //1, 2, 4, 8, 16, ...
{
	ADD_CLONES = 1,
	ADD_ALL_ACTORS = 2,
	ADD_ANY_ACTORS = 4,
	ADD_ANY_ANIMATION = 8,
	REMOVE_VIEW = 16,
	ADD_ANY_PATH = 32,
	REMOVE_EVENT_ACTOR = 64,
	ADD_PARENT_ACTOR = 128,
	ADD_EVENT_ACTOR = 256,
	USE_ACTION_ACTOR = 512
};

//Pointer wrap for use in property class
class stActionPtr
{
public:
	stActionPtr()
	{
		pAction = NULL;
		actor = NULL;
		actionActor = NULL;
	}

	stActionPtr(Actor *_actor, stAction *_pAction)
	{
		actor = _actor;
		pAction = _pAction;
		actionActor = _actor;
	}

	stActionPtr& operator=(const stActionPtr& other)
	{
		if( &other != this )
		{
			actor = other.actor;
			actionActor = other.actionActor;
			pAction = other.pAction;
		}
		
		return *this;
	}

	bool operator==(const stActionPtr& other) const 
	{ 
		if( &other != this )
		{
			return pAction == other.pAction; 
		}

		return true;
	}
	
	stAction *pAction;
	Actor *actor, *actionActor;
};

WX_PG_DECLARE_VALUE_TYPE_VOIDP(stActionPtr)
WX_PG_DECLARE_PROPERTY(PropertyEvent, const stActionPtr&, stActionPtr())
WX_PG_DECLARE_PROPERTY(PropertyAction, const stActionPtr&, stActionPtr())

class PanelEvents : public PanelGenericProperty  
{
	DECLARE_CLASS(PanelEvents)
    DECLARE_EVENT_TABLE()

public:
	PanelEvents(wxWindow *parent);
	virtual ~PanelEvents();
	
	static PanelEvents *Get() {return panelEvents;}
	static void Update(Actor *_actor);

private:

	bool ExistsEvent(stEvent &event);
	void PopulateEvents(Actor *actor);

	void OnPropertyGridItemDoubleClick( wxPropertyGridEvent& event );
	void OnPropertyGridToolBarClick( wxPropertyGridEvent& event );
	void OnPropertyGridSelected( wxPropertyGridEvent& event );
	void OnPropertyGridSelect( wxPropertyGridEvent& event );
	void OnPropertyGridChange ( wxPropertyGridEvent& event );
	void OnItemCollapsed ( wxPropertyGridEvent& event );
	void OnItemExpanded ( wxPropertyGridEvent& event );
	
	void OnKeyEvent ( wxKeyEvent& event );
	void OnMenuClick(wxCommandEvent& event);

	void DeleteEvent(wxPGId id);
	void CreateMenu();

	void UpdateCategoryNames();

	wxMenu *m_menu;
	static PanelEvents *panelEvents;
};

class PropertyActionClass : public wxPGPropertyWithChildren
{
    WX_PG_DECLARE_PROPERTY_CLASS()
public:

    PropertyActionClass ( const wxString& label, const wxString& name, stActionPtr value );
    virtual ~PropertyActionClass ();

	stAction *GetAction() {return m_value.pAction;}

    WX_PG_DECLARE_PARENTAL_TYPE_METHODS()
    WX_PG_DECLARE_PARENTAL_METHODS()

	void OnPropertyGridSelect(wxPGProperty *p);
	void UpdateCategoryName(bool bExpanded);
	int GetChoiceInfo ( wxPGChoiceInfo* choiceinfo );	
	bool SetValueFromInt ( long index, int arg_flags );
	int GetIndexForValue( int value ) const;
	wxString GetValueAsString ( int arg_flags ) const;

protected:

	int GetValueFromIndex( int index ) const;

	void CreateProperties();
	void UpdateProperties(bool bFromInterface);

	void CreateChangeAnimationProperties();
	void UpdateChangeAnimationProperties(bool bFromInterface);


	wxPGId	idActor, 
			idAnimation, idDirection,
			idRepeat,
			idPath,
			idTimer;

    stActionPtr            m_value;
	stAction copyEditAction;

	wxPGChoices             m_choices;
	int m_index;
};

class PropertyEventClass : public wxPGPropertyWithChildren
{
    WX_PG_DECLARE_PROPERTY_CLASS()
public:

    PropertyEventClass ( const wxString& label, const wxString& name, stActionPtr value );
    virtual ~PropertyEventClass ();

	stAction *GetAction() {return m_value.pAction;}

    WX_PG_DECLARE_PARENTAL_TYPE_METHODS()
    WX_PG_DECLARE_PARENTAL_METHODS()

	void OnPropertyGridSelect(wxPGProperty *p);
	void UpdateCategoryName(bool bExpanded);

	static void UpdateActorProperty(stActionPtr &m_value, wxPGId &id);
	static void UpdateAnimationProperty(stActionPtr &m_value, wxPGId &id);
	static void UpdatePathProperty(wxPGId &id);
	static void UpdateTimerProperty(wxPGId &id);

protected:
	void CreateProperties();
	void UpdateProperties(bool bFromInterface);
	void CreateActions();

	void CreateActivationEventProperties();
	void CreateAnimationFinishProperties();
	void CreateCollisionProperties();
	void CreateCollisionFinishProperties();
	void CreateCreateActorProperties();
	void CreateDestroyActorProperties();
	void CreateDrawActorProperties();
	void CreateKeyDownProperties();
	void CreateKeyUpProperties();
	void CreateMouseButtonDownProperties();
	void CreateMouseButtonUpProperties();
	void CreateMouseEnterProperties();
	void CreateMouseLeaveProperties();
	void CreateMoveFinishProperties();
	void CreateOutOfVisionProperties();
	void CreatePathFinishProperties();
	void CreateTimerProperties();

	void UpdateActivationEventProperties(bool bFromInterface);
	void UpdateAnimationFinishProperties(bool bFromInterface);
	void UpdateCollisionProperties(bool bFromInterface);
	void UpdateCollisionFinishProperties(bool bFromInterface);
	void UpdateCreateActorProperties(bool bFromInterface);
	void UpdateDestroyActorProperties(bool bFromInterface);
	void UpdateDrawActorProperties(bool bFromInterface);
	void UpdateKeyDownProperties(bool bFromInterface);
	void UpdateKeyUpProperties(bool bFromInterface);
	void UpdateMouseButtonDownProperties(bool bFromInterface);
	void UpdateMouseButtonUpProperties(bool bFromInterface);
	void UpdateMouseEnterProperties(bool bFromInterface);
	void UpdateMouseLeaveProperties(bool bFromInterface);
	void UpdateMoveFinishProperties(bool bFromInterface);
	void UpdateOutOfVisionProperties(bool bFromInterface);
	void UpdatePathFinishProperties(bool bFromInterface);
	void UpdateTimerProperties(bool bFromInterface);


	void UpdateEvents(const stEvent &oldEvent, const stEvent &newEvent);
	
	void UpdateKeyText();
	

	wxPGId	idDescription,
			idActor, idAnimation,
			idCollisionSide, 
			idRepeat,
			idKeys, idKeyDownMode,
			idMouseButton, idDrag,
			idPath,
			idTimer,
			idActions;

	

    stActionPtr            m_value;
	stAction copyEditAction;

	//For Key Down event
	unsigned short keySequence[MAX_KEY_SEQUENCE]; //Maximum MAX_KEY_SEQUENCE keys
	int nKeys;
};



#endif // !defined(AFX_PANELEVENTS_H__721C4D92_A22B_4720_8A41_2FF2AAAD9C06__INCLUDED_)
