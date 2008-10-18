// PanelEvents.cpp: implementation of the PanelEvents class.
//
//////////////////////////////////////////////////////////////////////

#include "PanelEvents.h"
#include "PanelActors.h"
#include "../gameEditor/UndoControl.h"

#define ID_ADD_EVENT		10
#define ID_REMOVE_EVENT		11


WX_PG_IMPLEMENT_VALUE_TYPE_VOIDP(stActionPtr, PropertyEvent, stActionPtr())



IMPLEMENT_CLASS(PanelEvents, PanelGenericProperty)

BEGIN_EVENT_TABLE(PanelEvents, PanelGenericProperty)
    EVT_PG_DOUBLE_CLICK( PANEL_EVENTS_ID, PanelEvents::OnPropertyGridItemDoubleClick )
	EVT_PG_TOOL_BAR_CLICK( PANEL_EVENTS_ID, PanelEvents::OnPropertyGridToolBarClick )
	EVT_KEY_DOWN(PanelEvents::OnKeyEvent)
	EVT_MENU( wxID_ANY, PanelEvents::OnMenuClick )
	EVT_PG_SELECTED( PANEL_EVENTS_ID, PanelEvents::OnPropertyGridSelected )
	EVT_PG_SELECT( PANEL_EVENTS_ID, PanelEvents::OnPropertyGridSelect )
	EVT_PG_CHANGED( PANEL_EVENTS_ID, PanelEvents::OnPropertyGridChange )
	EVT_PG_ITEM_COLLAPSED(PANEL_EVENTS_ID, PanelEvents::OnItemCollapsed)
	EVT_PG_ITEM_EXPANDED(PANEL_EVENTS_ID, PanelEvents::OnItemExpanded)
END_EVENT_TABLE()

void UpdatePanelEvents()
{
	if(!Actor::getLoadingActors())
	{
		if(PanelActors::Get()) PanelActors::Get()->Refresh();		
	}
}

PanelEvents *PanelEvents::panelEvents = NULL;

PanelEvents::PanelEvents(wxWindow *parent)   : PanelGenericProperty(parent, PANEL_EVENTS_ID, wxPG_AUTO_SORT/*wxPG_HIDE_MARGIN*/, true)
{
	wxPGId id, idSub;
	panelEvents = this;

	if(pgManager && pgManager->GetToolBar())
	{
		wxBitmap bitmapAdd, bitmapDel, bitmapDelDisable;

		bitmapAdd.LoadFile(wxT("event_add_b.gif"), wxBITMAP_TYPE_GIF);
		bitmapDel.LoadFile(wxT("event_del_b.gif"), wxBITMAP_TYPE_GIF);
		
		
		pgManager->GetToolBar()->AddTool(ID_ADD_EVENT, bitmapAdd, wxT("Add Event"));
		pgManager->GetToolBar()->AddTool(ID_REMOVE_EVENT, bitmapDel, wxT("Remove Event"));
		
		//pgManager->GetToolBar()->SetToolBitmapSize(wxSize(image.GetWidth(), image.GetHeight()));
		pgManager->GetToolBar()->Realize();
	}



	//Use check box
	pg->SetPropertyAttributeAll(wxPG_BOOL_USE_CHECKBOX,(long)1);

	pg->CollapseAll();

	rightMargin = 0;

	m_menu = NULL;
	CreateMenu();
}

PanelEvents::~PanelEvents()
{
	panelEvents = NULL;
	delete m_menu;
}

void PanelEvents::CreateMenu()
{
	if(!m_menu)
	{
		int i;
		m_menu = new wxMenu;

		ListString listString;
		Action::PopulateEvents(listString);	

		wxArrayString array;
		
		for(i = 0; i < listString.Count(); i++)
		{			
			array.Add(listString[i].c_str());
		}

		array.Sort();

		for(i = 0; i < listString.Count(); i++)
		{
			m_menu->Append(i, array[i]);
		}	
		
		//m_menu->SetBackgroundColour(*wxWHITE);
	}
	
}

void PanelEvents::OnPropertyGridItemDoubleClick( wxPropertyGridEvent& event )
{
	event.Skip();
	//wxMessageBox(event.GetPropertyLabel(), wxT("Selected"));
}

void PanelEvents::OnPropertyGridToolBarClick( wxPropertyGridEvent& event )
{
	switch(event.GetInt())
	{
	case ID_ADD_EVENT:
		{
			//How get the tool position?
			//wxToolBarToolBase* tool = pgManager->GetToolBar()->FindById(event.GetInt());
			
			PopupMenu(m_menu, wxPoint(5, 17));			
		}
		break;

	case ID_REMOVE_EVENT:
		{
			DeleteEvent(event.GetProperty());
		}
		break;
	}	
}

void PanelEvents::OnKeyEvent ( wxKeyEvent& event )
{
	if(event.GetKeyCode() == WXK_DELETE)
	{
		DeleteEvent(pg->GetSelectedProperty());
	}
}

void PanelEvents::DeleteEvent(wxPGId id)
{
	if(id)
	{
		//Get root category
		id = id.GetProperty().GetMainParent();
		if(!pg->IsPropertyCategory(id))
		{
			id = id.GetProperty().GetParent();
		}

		int answer = wxMessageBox(wxT("Remove this event?"), wxT("Confirm"), wxYES_NO | wxCENTRE);
		
		if(answer == wxYES)
		{
			pg->Delete(id);
		}	
	}
}

void PanelEvents::OnMenuClick(wxCommandEvent& event)
{	
	static int n = 0;
	wxString s;
	s = s.Format("%s %ld", m_menu->GetLabel(event.GetId()), ++n);

	wxPGId id, idSub;
	
	id = pg->Append(wxPropertyCategory (s,wxPG_LABEL)); pg->SetPropertyEditor( id, wxPG_EDITOR(TextCtrl) );
	pg->DisableProperty (id);
	pg->SetPropertyValue( id, NO_SELECTION );
	idSub = pg->AppendIn(id, wxBoolProperty ( wxT("Prop"), wxPG_LABEL, false ));
	pg->SetPropertyColour(idSub, subCategoryColor);

	pg->Expand(id);

	//Use check box
	pg->SetPropertyAttributeAll(wxPG_BOOL_USE_CHECKBOX,(long)1);
	
	//pg->Sort();
	pg->Update();
	pg->Refresh();
}

void PanelEvents::OnPropertyGridSelected( wxPropertyGridEvent& event )
{
	/*if(event.GetProperty() && pg->IsPropertyCategory(event.GetProperty()))
	{
		pgManager->GetToolBar()->EnableTool(ID_REMOVE_EVENT, true);
	}
	else
	{
		pgManager->GetToolBar()->EnableTool(ID_REMOVE_EVENT, false);
	}*/
}

void PanelEvents::OnPropertyGridSelect( wxPropertyGridEvent& event )
{
	wxPGProperty *pProperty = event.GetProperty().GetPropertyPtr();
	if(!pProperty) return;

	wxPGProperty *pParent = event.GetProperty().GetPropertyPtr()->GetParent();
	if(!pParent) return;

	wxString sClassName(pParent->GetClassName());

	if(sClassName == wxT("PropertyEvent"))
	{
		((PropertyEventClass *)pParent)->OnPropertyGridSelect(pProperty);
	}
	else if(sClassName == wxT("PropertyAction"))
	{
		((PropertyActionClass *)pParent)->OnPropertyGridSelect(pProperty);
	}
}

void PanelEvents::OnPropertyGridChange ( wxPropertyGridEvent& event )
{
	UndoControl::Get()->PushCurrentState();
}

void PanelEvents::OnItemCollapsed ( wxPropertyGridEvent& event )
{
	wxPGProperty *pProperty = event.GetProperty().GetPropertyPtr();
	if(!pProperty) return;

	wxString sClassName(pProperty->GetClassName());

	if(sClassName == wxT("PropertyEvent"))
	{
		stAction *action = ((PropertyEventClass *)pProperty)->GetAction();
		if(action)
		{
			action->flags &= ~EV_FLAG_OPENED_GUI;
		}

		((PropertyEventClass *)pProperty)->UpdateCategoryName(false);
	}
	else if(sClassName == wxT("PropertyAction"))
	{
		stAction *action = ((PropertyActionClass *)pProperty)->GetAction();
		if(action)
		{
			action->flags &= ~AC_FLAG_OPENED_GUI;
		}
	}

	UpdateCategoryNames();
}

void PanelEvents::OnItemExpanded ( wxPropertyGridEvent& event )
{
	wxPGProperty *pProperty = event.GetProperty().GetPropertyPtr();
	if(!pProperty) return;

	wxString sClassName(pProperty->GetClassName());

	if(sClassName == wxT("PropertyEvent"))
	{
		stAction *action = ((PropertyEventClass *)pProperty)->GetAction();
		if(action)
		{
			action->flags |= EV_FLAG_OPENED_GUI;
		}

		((PropertyEventClass *)pProperty)->UpdateCategoryName(true);
	}
	else if(sClassName == wxT("PropertyAction"))
	{
		stAction *action = ((PropertyActionClass *)pProperty)->GetAction();
		if(action)
		{
			action->flags |= AC_FLAG_OPENED_GUI;
		}
	}

	UpdateCategoryNames();
}

void PanelEvents::UpdateCategoryNames()
{
	//Put the event description
}

void PanelEvents::Update(Actor *_actor)
{
	if(!panelEvents) return;

	panelEvents->Freeze();

	//Remove properties
	wxPGId id = panelEvents->pg->GetFirstChild(panelEvents->pg->GetRoot());
	while ( id.IsOk() )
	{		
		wxPGId aux = id;
		id = ((wxPropertyContainerMethods *)panelEvents->pg)->GetNextSibling(id);

		((wxPropertyContainerMethods *)panelEvents->pg)->Delete(aux);
	}

	//Update actions
	if(_actor && _actor->getAction())
	{
		//Get events
		panelEvents->PopulateEvents(_actor);
		

		//Use check box
		panelEvents->pg->SetPropertyAttributeAll(wxPG_BOOL_USE_CHECKBOX,(long)1);
	}
	
	panelEvents->Thaw();
	panelEvents->Refresh();
}

bool PanelEvents::ExistsEvent(stEvent &event)
{
	wxPGId id = pg->GetFirstChild(pg->GetRoot());
	while(id.IsOk())
	{		
		wxPGProperty *pProperty = id.GetPropertyPtr();
		if(pProperty)
		{			
			wxString sClassName(pProperty->GetClassName());
			
			if(sClassName == wxT("PropertyEvent"))
			{
				stAction *action = ((PropertyEventClass *)pProperty)->GetAction();
				if(action)
				{
					if(action->eventData == event)
					{
						return true;
					}
				}
			}
		}
		
		id = ((wxPropertyContainerMethods *)pg)->GetNextSibling(id);
	}

	return false;
}

void PanelEvents::PopulateEvents(Actor *actor)
{
	stAction *action = actor->getAction()->getActions();

	while(action)
	{
		if(action->actionType != SET_CONDITIONAL_ACTION)
		{
			if(!ExistsEvent(action->eventData))
			{
				wxPGId id = panelEvents->pg->Append(PropertyEvent( wxT(""), wxT(""), stActionPtr(actor, action)));
				
				if(action->flags & EV_FLAG_OPENED_GUI)
				{
					pg->Expand(id);
				}
			}
		}

		action = action->next;							
	} 	
}
































WX_PG_IMPLEMENT_PROPERTY_CLASS(PropertyEvent,wxBaseParentProperty, stActionPtr, const stActionPtr&, TextCtrl)


PropertyEventClass::PropertyEventClass ( const wxString& label, const wxString& name, stActionPtr value) 
 : wxPGPropertyWithChildren(label, name)
{
	SetFlag(wxPG_PROP_CATEGORY_LOOK);
    wxPG_INIT_REQUIRED_TYPE(stActionPtr)
	    
	m_value = value;
	CreateProperties();    
	DoSetValue((void*)&value);
}

PropertyEventClass::~PropertyEventClass() 
{ 

}

void PropertyEventClass::DoSetValue ( wxPGVariant value )
{
    stActionPtr* pObj = (stActionPtr*)wxPGVariantToVoidPtr(value);
    m_value = *pObj;
	copyEditAction = *m_value.pAction;

	UpdateCategoryName(wxPropertyContainerMethods::IsPropertyExpanded(GetId()));	

	UpdateProperties(false);
    RefreshChildren();
}

wxPGVariant PropertyEventClass::DoGetValue () const
{
    return wxPGVariant((void*)&m_value);
}

void PropertyEventClass::UpdateCategoryName(bool bExpanded)
{
	gedString desc;

	if(bExpanded)
	{
		desc = Action::GetDescription(m_value.pAction, false, true);
	}
	else
	{
		if(!m_value.pAction->getDescription().empty())
		{
			desc = Action::GetDescription(m_value.pAction, false, true);
			desc += " (";
			desc += m_value.pAction->getDescription();
			desc += ")";
		}
		else
		{
			desc = Action::GetDescription(m_value.pAction, false, false);
		}
	}

	SetLabel(desc.c_str());
}

void PropertyEventClass::CreateProperties()
{
	//Description
	idDescription = AddChild( wxStringProperty (wxT("Description"), wxPG_LABEL, wxT(""))); 

	//Specific properties
	switch(m_value.pAction->eventData.eventType)
	{
	case EVENT_ACTIVATION_EVENT:
		CreateActivationEventProperties();
		break;

	case EVENT_ANIMATION_FINISH:
		CreateAnimationFinishProperties();
		break;

	case EVENT_COLLISION:
		CreateCollisionProperties();
		break;

	case EVENT_COLLISION_FINISH:
		CreateCollisionFinishProperties();
		break;

	case EVENT_CREATE:
		CreateCreateActorProperties();
		break;

	case EVENT_DESTROY_ACTOR:
		CreateDestroyActorProperties();
		break;

	case EVENT_ANIMATION:
		CreateDrawActorProperties();
		break;

	case EVENT_KEYDOWN:
		CreateKeyDownProperties();
		break;

	case EVENT_KEYUP:
		CreateKeyUpProperties();
		break;

	case EVENT_MOUSEBUTTONDOWN:
		CreateMouseButtonDownProperties();
		break;

	case EVENT_MOUSEBUTTONUP:
		CreateMouseButtonUpProperties();
		break;

	case EVENT_MOUSEENTER:
		CreateMouseEnterProperties();
		break;

	case EVENT_MOUSELEAVE:
		CreateMouseLeaveProperties();
		break;

	case EVENT_MOVEFINISH:
		CreateMoveFinishProperties();
		break;

	case EVENT_OUT_OF_VISION:
		CreateOutOfVisionProperties();
		break;

	case EVENT_PATH_FINISH:
		CreatePathFinishProperties();
		break;
 
	case EVENT_TIMER:
		CreateTimerProperties();
		break;
	}
	
	//Actions	
	CreateActions();	
}

void PropertyEventClass::CreateActions()
{
	idActions = AddChild( wxCustomProperty(wxT("Actions"),wxPG_LABEL) ); 
	PanelEvents::Get()->GetPropertyGrid()->SetPropertyEditor( idActions, wxPG_EDITOR(ChoiceAndButton) );

	stAction *action = m_value.actor->getAction()->getActions();

	while(action)
	{
		if(action->eventData == m_value.pAction->eventData)
		{
			//Add action in the GUI of this event
			wxPGId id = PanelEvents::Get()->GetPropertyGrid()->AppendIn(idActions, PropertyAction( wxT(""), wxT(""), stActionPtr(m_value.actor, action))); //id.GetProperty().SetToolTip(action tip);
			PanelEvents::Get()->GetPropertyGrid()->SetPropertyColour(id, subCategoryColor);

			if(action->flags & AC_FLAG_OPENED_GUI)
			{
				PanelEvents::Get()->GetPropertyGrid()->Expand(id);
			}
		}

		action = action->next;							
	}	
}

void PropertyEventClass::UpdateProperties(bool bFromInterface)
{
	//Description	 
	if(bFromInterface)
	{
		m_value.pAction->setDescription(idDescription.GetProperty().GetValueAsString().GetData());
	}
	else
	{
		idDescription.GetProperty().SetValueFromString(m_value.pAction->getDescription().c_str());
	}

	//Specific properties
	switch(m_value.pAction->eventData.eventType)
	{
	case EVENT_ACTIVATION_EVENT:
		UpdateActivationEventProperties(bFromInterface);
		break;

	case EVENT_ANIMATION_FINISH:
		UpdateAnimationFinishProperties(bFromInterface);
		break;

	case EVENT_COLLISION:
		UpdateCollisionProperties(bFromInterface);
		break;

	case EVENT_COLLISION_FINISH:
		UpdateCollisionFinishProperties(bFromInterface);
		break;

	case EVENT_CREATE:
		UpdateCreateActorProperties(bFromInterface);
		break;

	case EVENT_DESTROY_ACTOR:
		UpdateDestroyActorProperties(bFromInterface);
		break;

	case EVENT_ANIMATION:
		UpdateDrawActorProperties(bFromInterface);
		break;

	case EVENT_KEYDOWN:
		UpdateKeyDownProperties(bFromInterface);
		break;

	case EVENT_KEYUP:
		UpdateKeyUpProperties(bFromInterface);
		break;

	case EVENT_MOUSEBUTTONDOWN:
		UpdateMouseButtonDownProperties(bFromInterface);
		break;

	case EVENT_MOUSEBUTTONUP:
		UpdateMouseButtonUpProperties(bFromInterface);
		break;

	case EVENT_MOUSEENTER:
		UpdateMouseEnterProperties(bFromInterface);
		break;

	case EVENT_MOUSELEAVE:
		UpdateMouseLeaveProperties(bFromInterface);
		break;

	case EVENT_MOVEFINISH:
		UpdateMoveFinishProperties(bFromInterface);
		break;

	case EVENT_OUT_OF_VISION:
		UpdateOutOfVisionProperties(bFromInterface);
		break;

	case EVENT_PATH_FINISH:
		UpdatePathFinishProperties(bFromInterface);
		break;

	case EVENT_TIMER:
		UpdateTimerProperties(bFromInterface);
		break;
	}	
}

void PropertyEventClass::CreateActivationEventProperties()
{	
	SetToolTip(TIP_DLG_ACTIVATIONEVENTS);

	idActor = AddChild(wxTreeProperty(wxT("From Actor"), wxPG_LABEL) ); idActor.GetProperty().SetToolTip(TIP_DLG_ACTIVATIONEVENTS_FROM);
	idActor.GetProperty().SetExtraFlags(ADD_CLONES | ADD_ALL_ACTORS | ADD_ANY_ACTORS);		
}

void PropertyEventClass::UpdateActivationEventProperties(bool bFromInterface)
{
	if(bFromInterface)
	{
		gedString cloneName(idActor.GetProperty().GetValueAsString().GetData());
		
		Actor *actor = NULL;
		if(cloneName != ANY_ACTOR && cloneName != ALL_ACTORS && (actor = GameControl::Get()->GetActor(cloneName)) != NULL)
			cloneName = actor->getCloneName();
		
		Action::EventActivationEvent(cloneName);
	}
	else
	{
		gedString name(m_value.pAction->eventData.data.name);
		if(!GameControl::Get()->GetActor(name))
		{
			//Remove clone name
			gedString textCmp(name);
			int i = textCmp.find('.');
			
			if(i != gedString::npos)
			{
				name = textCmp.substr(0, i);
			}
		}
		
		idActor.GetProperty().SetValueFromString(name.c_str());
	}
}

void PropertyEventClass::CreateAnimationFinishProperties()
{
	SetToolTip(TIP_DLG_ANIMATIONFINISH);

	idAnimation = AddChild(wxTreeProperty(wxT("Animation"), wxPG_LABEL) ); idAnimation.GetProperty().SetToolTip(TIP_DLG_ANIMATIONFINISH_ANIMATION);
	idAnimation.GetProperty().SetExtraFlags(ADD_ANY_ANIMATION);		
}

void PropertyEventClass::UpdateAnimationFinishProperties(bool bFromInterface)
{
	if(bFromInterface)
	{
		Action::EventAnimationFinish(idAnimation.GetProperty().GetValueAsString().GetData());
	}
	else
	{
		idAnimation.GetProperty().SetValueFromString(m_value.pAction->eventData.data.name);
	}
}

void PropertyEventClass::CreateCollisionProperties()
{
	SetToolTip(TIP_DLG_COLLISION);

	//Side
	wxPGChoices side;

    side.Add ( wxT("Any Side"), ANY_SIDE );
    side.Add ( wxT("Top Side"), TOP_SIDE );
	side.Add ( wxT("Bottom Side"), BOTTOM_SIDE );
	side.Add ( wxT("Left Side"), LEFT_SIDE );
	side.Add ( wxT("Right Side"), RIGHT_SIDE );
	side.Add ( wxT("Top or Bottom Side"), TOP_OR_BOTTOM_SIDE );
	side.Add ( wxT("Left or Right Side"), LEFT_OR_RIGHT_SIDE );

    idCollisionSide = AddChild(wxEnumProperty(wxT("On"), wxPG_LABEL, side, ANY_SIDE) ); idCollisionSide.GetProperty().SetToolTip(TIP_DLG_COLLISION_SIDE);

	//Actor
	idActor = AddChild(wxTreeProperty(wxT("Of Actor"), wxPG_LABEL) ); idActor.GetProperty().SetToolTip(TIP_DLG_COLLISION_ACTOR);
	idActor.GetProperty().SetExtraFlags(ADD_CLONES | ADD_ANY_ACTORS | REMOVE_VIEW | REMOVE_EVENT_ACTOR);

	//Repeat	
    idRepeat = AddChild(wxBoolProperty(wxT("Repeat this event\nwhile actor is colliding"), wxPG_LABEL, false) ); idRepeat.GetProperty().SetToolTip(TIP_DLG_COLLISION_REPEAT);
}

void PropertyEventClass::UpdateCollisionProperties(bool bFromInterface)
{
	if(bFromInterface)
	{
		Action::EventCollision(
			idActor.GetProperty().GetValueAsString().GetData(), 
			idRepeat.GetProperty().GetValueAsBool(), 
			idCollisionSide.GetProperty().DoGetValue().GetLong());
	}
	else
	{
		idActor.GetProperty().SetValueFromString(m_value.pAction->eventData.data.name);	
		idRepeat.GetProperty().SetValueFromBool(m_value.pAction->eventData.data.name[NAME_LIMIT-1] == 0); //false = repeat (look Action::EventCollision)
		idCollisionSide.GetProperty().SetValueFromInt(m_value.pAction->eventData.data.name[NAME_LIMIT-2]);	
	}
}

void PropertyEventClass::CreateCollisionFinishProperties()
{
	SetToolTip(TIP_DLG_COLLISION_FINISH);	

	//Actor
	idActor = AddChild(wxTreeProperty(wxT("Actor"), wxPG_LABEL) ); idActor.GetProperty().SetToolTip(TIP_DLG_COLLISION_ACTOR);
	idActor.GetProperty().SetExtraFlags(ADD_CLONES | ADD_ANY_ACTORS | REMOVE_VIEW | REMOVE_EVENT_ACTOR);
}

void PropertyEventClass::UpdateCollisionFinishProperties(bool bFromInterface)
{
	if(bFromInterface)
	{
		Action::EventCollisionFinish(idActor.GetProperty().GetValueAsString().GetData());
	}
	else
	{
		idActor.GetProperty().SetValueFromString(m_value.pAction->eventData.data.name);		
	}
}

void PropertyEventClass::CreateCreateActorProperties()
{

}

void PropertyEventClass::UpdateCreateActorProperties(bool bFromInterface)
{

}

void PropertyEventClass::CreateDestroyActorProperties()
{

}

void PropertyEventClass::UpdateDestroyActorProperties(bool bFromInterface)
{

}

void PropertyEventClass::CreateDrawActorProperties()
{

}

void PropertyEventClass::UpdateDrawActorProperties(bool bFromInterface)
{

}

void PropertyEventClass::CreateKeyDownProperties()
{
	SetToolTip(TIP_DLG_KEYDOWN);

	//Keys
	idKeys = AddChild(wxStringProperty (wxT("Keys"), wxPG_LABEL, wxT(""))); idKeys.GetProperty().SetToolTip(TIP_DLG_KEYDOWN_KEYS);
	PanelEvents::Get()->GetPropertyGrid()->SetPropertyEditor( idKeys, wxPG_EDITOR(KeyCtrl) );

	//Execution mode
	wxPGChoices mode;

    mode.Add ( wxT("At least one key is pressed"), SDLK_AT_LEAST_ONE_KEY_IS_PRESSED );
    mode.Add ( wxT("All keys are pressed"), SDLK_ALL_KEYS_PRESSED );
	mode.Add ( wxT("Keys are pressed in order"), SDLK_KEYS_PRESSED_IN_ORDER );
	
    idKeyDownMode = AddChild(wxEnumProperty(wxT("Execute when"), wxPG_LABEL, mode, SDLK_AT_LEAST_ONE_KEY_IS_PRESSED) ); idKeyDownMode.GetProperty().SetToolTip(TIP_DLG_KEYDOWN_MODE);

	//Repeat	
    idRepeat = AddChild(wxBoolProperty(wxT("Repeat"), wxPG_LABEL, true) ); idRepeat.GetProperty().SetToolTip(TIP_DLG_KEYDOWN_REPEAT);
}

void PropertyEventClass::UpdateKeyDownProperties(bool bFromInterface)
{
	if(bFromInterface)
	{
		//Invalidate action if not have keys

		/*Action::EventKeyDown(
			(SDLKey)idKeyDownMode.GetProperty().DoGetValue().GetLong(), 
			!idRepeat.GetProperty().GetValueAsBool(), 
			m_value.pAction->eventData.data.keyEvent.keySequence);*/ 
	}
	else
	{
		nKeys = 0;
		memset(keySequence, 0, MAX_KEY_SEQUENCE*sizeof(unsigned short));

		idKeyDownMode.GetProperty().SetValueFromInt(m_value.pAction->eventData.data.keyEvent.key, wxPG_FULL_VALUE);

		if(m_value.pAction->eventData.data.keyEvent.key <= SDLK_ANY_KEY)
		{
			keySequence[0] = m_value.pAction->eventData.data.keyEvent.key;
		}
		else
		{
			memcpy(keySequence, m_value.pAction->eventData.data.keyEvent.keySequence, MAX_KEY_SEQUENCE*sizeof(unsigned short));
		}
	
		
		while(nKeys < MAX_KEY_SEQUENCE && keySequence[nKeys]) nKeys++;
	
		UpdateKeyText();
	
		idRepeat.GetProperty().SetValueFromBool(m_value.pAction->eventData.data.keyEvent.bDisableRepeat == 0);
	}
}

void PropertyEventClass::UpdateKeyText()
{
	wxString keyText;

	for(int i = 0; i < nKeys; i++)
	{
		keyText += SDL_GetKeyName((SDLKey)keySequence[i]);		
		keyText += " ";
	}

	idKeys.GetProperty().SetValueFromString(keyText);	
}

void PropertyEventClass::CreateKeyUpProperties()
{
	SetToolTip(TIP_DLG_KEYUP);

	//Keys
	idKeys = AddChild(wxStringProperty (wxT("Key"), wxPG_LABEL, wxT(""))); idKeys.GetProperty().SetToolTip(TIP_DLG_KEYUP_KEY);
}

void PropertyEventClass::UpdateKeyUpProperties(bool bFromInterface)
{
	if(bFromInterface)
	{
		//Invalidate action if not have keys

		//Action::EventKeyUp((SDLKey)keySequence[0]); 
	}
	else
	{
	}
}

void PropertyEventClass::CreateMouseButtonDownProperties()
{
	SetToolTip(TIP_DLG_MOUSEBUTTON_DOWN);
	
	//Button
	wxPGChoices button;

    button.Add ( wxT("Left"), SDL_BUTTON_LEFT );
    button.Add ( wxT("Middle"), SDL_BUTTON_MIDDLE );
	button.Add ( wxT("Right"), SDL_BUTTON_RIGHT );
	
    idMouseButton = AddChild(wxEnumProperty(wxT("Button"), wxPG_LABEL, button, SDL_BUTTON_LEFT) ); idMouseButton.GetProperty().SetToolTip(TIP_DLG_MOUSEBUTTON_BUTTON);

	//Drag	
    idDrag = AddChild(wxBoolProperty(wxT("Drag"), wxPG_LABEL, false) ); idDrag.GetProperty().SetToolTip(TIP_DLG_MOUSEBUTTON_DRAG);

}

void PropertyEventClass::UpdateMouseButtonDownProperties(bool bFromInterface)
{
	if(bFromInterface)
	{
		Action::EventMouseButtonDown(0, 0, 
			idMouseButton.GetProperty().DoGetValue().GetLong(), 
			idDrag.GetProperty().GetValueAsBool());
	}
	else
	{
		idMouseButton.GetProperty().SetValueFromInt(m_value.pAction->eventData.data.mouse.button, wxPG_FULL_VALUE);
		idDrag.GetProperty().SetValueFromBool(m_value.pAction->eventData.data.mouse.bDrag != 0);
	}
}

void PropertyEventClass::CreateMouseButtonUpProperties()
{
	SetToolTip(TIP_DLG_MOUSEBUTTON_UP);
	
	//Button
	wxPGChoices button;

    button.Add ( wxT("Left"), SDL_BUTTON_LEFT );
    button.Add ( wxT("Middle"), SDL_BUTTON_MIDDLE );
	button.Add ( wxT("Right"), SDL_BUTTON_RIGHT );
	
    idMouseButton = AddChild(wxEnumProperty(wxT("Button"), wxPG_LABEL, button, SDL_BUTTON_LEFT) ); idMouseButton.GetProperty().SetToolTip(TIP_DLG_MOUSEBUTTON_BUTTON);
}

void PropertyEventClass::UpdateMouseButtonUpProperties(bool bFromInterface)
{
	if(bFromInterface)
	{
		Action::EventMouseButtonUp(0, 0, idMouseButton.GetProperty().DoGetValue().GetLong());
	}
	else
	{
		idMouseButton.GetProperty().SetValueFromInt(m_value.pAction->eventData.data.mouse.button, wxPG_FULL_VALUE);
	}	
}

void PropertyEventClass::CreateMouseEnterProperties()
{

}

void PropertyEventClass::UpdateMouseEnterProperties(bool bFromInterface)
{

}

void PropertyEventClass::CreateMouseLeaveProperties()
{

}

void PropertyEventClass::UpdateMouseLeaveProperties(bool bFromInterface)
{

}

void PropertyEventClass::CreateMoveFinishProperties()
{

}

void PropertyEventClass::UpdateMoveFinishProperties(bool bFromInterface)
{

}

void PropertyEventClass::CreateOutOfVisionProperties()
{

}

void PropertyEventClass::UpdateOutOfVisionProperties(bool bFromInterface)
{

}

void PropertyEventClass::CreatePathFinishProperties()
{
	SetToolTip(TIP_DLG_PATH_FINISH);

	idPath = AddChild(wxTreeProperty(wxT("Path"), wxPG_LABEL) ); idPath.GetProperty().SetToolTip(TIP_DLG_PATHFINISH_PATH);
	idPath.GetProperty().SetExtraFlags(ADD_ANY_PATH);		
}

void PropertyEventClass::UpdatePathFinishProperties(bool bFromInterface)
{
	if(bFromInterface)
	{
		Action::EventPathFinish(idPath.GetProperty().GetValueAsString().GetData());		
	}
	else
	{
		idPath.GetProperty().SetValueFromString(m_value.pAction->eventData.data.name);
	}	
}

void PropertyEventClass::CreateTimerProperties()
{
	SetToolTip(TIP_DLG_TIMER);

	idTimer = AddChild(wxTreeProperty(wxT("Timer"), wxPG_LABEL) ); idTimer.GetProperty().SetToolTip(TIP_DLG_TIMER_TIMER);
}

void PropertyEventClass::UpdateTimerProperties(bool bFromInterface)
{
	if(bFromInterface)
	{
		Action::EventTimer(idTimer.GetProperty().GetValueAsString().GetData());		
	}
	else
	{
		idTimer.GetProperty().SetValueFromString(m_value.pAction->eventData.data.name);
	}
	

	//Don't show timer parameters now
	/*int n = listTimer->Count();

	for(int i = 0; 0 < n; i++)
	{
		gedString timerName(listTimer->GetText(i));

		int pos = timerName.find(" (");
		if(pos >= 0) timerName = timerName.substr(0, pos);

		if(timerName == editAction->eventData.data.name)
		{
			listTimer->SetItem(i);
			break;
		}
	}*/
}

void PropertyEventClass::UpdateActorProperty(stActionPtr &m_value, wxPGId &id)
{
	//Populate actors
	ListString listString;
	Uint32 flags = id.GetProperty().GetExtraFlags();
	GameControl::Get()->PopulateActors(listString, (flags & ADD_CLONES)?true:false, true);
	
	wxArrayString array(true); //true to speed up remotion
	int i;
	
	for(i = 0; i < listString.Count(); i++)
	{			
		array.Add(listString[i].c_str());
	}
	
	//array.Add(NO_SELECTION);	
	
	if(flags & ADD_ALL_ACTORS) array.Add(ALL_ACTORS.c_str());	
	if(flags & ADD_ANY_ACTORS) array.Add(ANY_ACTOR.c_str());	
	if(flags & ADD_PARENT_ACTOR) array.Add(S_PARENT_ACTOR);	
	if(flags & ADD_EVENT_ACTOR) array.Add(S_EVENT_ACTOR);	
	if(flags & REMOVE_VIEW) array.Remove(VIEW_NAME.c_str());
	if(flags & REMOVE_EVENT_ACTOR) array.Remove((gedString(m_value.actor->getActorName()) + "," + m_value.actor->getCloneName()).c_str());


	//Set property	
	id.GetProperty().SetChoices(array); 
}

void PropertyEventClass::UpdateAnimationProperty(stActionPtr &m_value, wxPGId &id)
{
	//Populate actors
	ListString listString;
	Uint32 flags = id.GetProperty().GetExtraFlags();
	GameControl::Get()->PopulateAnimations((flags & USE_ACTION_ACTOR)?m_value.actionActor:m_value.actor, listString);
	
	wxArrayString array(true); //true to speed up remotion
	int i;
	
	for(i = 0; i < listString.Count(); i++)
	{			
		array.Add(listString[i].c_str());
	}
	
	
	if(flags & ADD_ANY_ANIMATION) array.Add(ANY_ANIMATION.c_str());		


	//Set property	
	id.GetProperty().SetChoices(array);
}

void PropertyEventClass::UpdatePathProperty(wxPGId &id)
{
	//Populate paths
	ListString listString;
	Uint32 flags = id.GetProperty().GetExtraFlags();
	GameControl::Get()->PopulatePaths(listString, false);
	
	wxArrayString array(true); //true to speed up remotion
	int i;
	
	for(i = 0; i < listString.Count(); i++)
	{			
		array.Add(listString[i].c_str());
	}	
	
	if(flags & ADD_ANY_PATH) array.Add(ANY_PATH.c_str());		

	//Set property	
	id.GetProperty().SetChoices(array);
}

void PropertyEventClass::UpdateTimerProperty(wxPGId &id)
{
	//Populate paths
	ListString listString;
	Uint32 flags = id.GetProperty().GetExtraFlags();
	GameControl::Get()->PopulateTimers(listString, false);
	
	wxArrayString array(true); //true to speed up remotion
	int i;
	
	for(i = 0; i < listString.Count(); i++)
	{			
		array.Add(listString[i].c_str());
	}	

	//Set property	
	id.GetProperty().SetChoices(array);
}


void PropertyEventClass::OnPropertyGridSelect(wxPGProperty *p)
{
	if(p->GetId() == idActor)
    {
		UpdateActorProperty(m_value, idActor); 
	}
	else if(p->GetId() == idAnimation)
    {
		UpdateAnimationProperty(m_value, idAnimation); 
	}
	else if(p->GetId() == idPath)
    {
		UpdatePathProperty(idPath); 
	}
	else if(p->GetId() == idTimer)
    {
		UpdateTimerProperty(idTimer); 
	}
}

void PropertyEventClass::ChildChanged ( wxPGProperty* p )
{
	if(!IS_VALID_ACTOR(m_value.actor)) return;

	//Store old event value
	stEvent oldEvent = m_value.pAction->eventData, newEvent = m_value.pAction->eventData;

	//Update values from GUI	
	UpdateProperties(true);
	newEvent = Action::getActualEvent();
	
	//Update all events equal to event in GUI
	UpdateEvents(oldEvent, newEvent);	

	Action::clearEditAction();

	/*if(copyEditAction != *m_value.pAction)
	{
		copyEditAction = *m_value.pAction;
		UndoControl::Get()->PushCurrentState();
	}*/
}

void PropertyEventClass::UpdateEvents(const stEvent &oldEvent, const stEvent &newEvent)
{
	//Update all events that match with oldEvent value

	stAction *action = m_value.actor->getAction()->getActions();

	while(action)
	{
		if(action->eventData == oldEvent)
		{
			//Update event
			action->eventData = newEvent;

			//Update event lists
			Action::setActualEvent(action->eventData);
			Action::setEditAction(action);
			Action::Call(m_value.actor)->Action::UpdateEvent();
		}

		action = action->next;							
	} 	
}


void PropertyEventClass::RefreshChildren()
{
    if ( !GetCount() ) return;
    /*Item(0)->DoSetValue( (void*) &m_value.a );
    Item(1)->DoSetValue( (void*) &m_value.b );
    Item(2)->DoSetValue( (void*) &m_value.c );*/
}















WX_PG_IMPLEMENT_PROPERTY_CLASS(PropertyAction,wxBaseParentProperty, stActionPtr, const stActionPtr&, Choice)


PropertyActionClass::PropertyActionClass ( const wxString& label, const wxString& name, stActionPtr value) 
 : wxPGPropertyWithChildren(label, name)
{	
	wxPGChoices action;

    action.Add(CHANGE_ANIMATION, SET_ANIMATION);
	action.Add(CHANGE_ANIMATION_DIRECTION, SET_ANIMATION_DIRECTION);
	action.Add(CURSOR_POINTER, SET_CURSOR);
	action.Add(CHANGE_PARENT, SET_PARENT);
	action.Add(CHANGE_PATH, SET_PATH);
	action.Add(CHANGE_TRANSPARENCY, SET_TRANSPARENCY);
	action.Add(CHANGE_ZDEPTH, SET_ZDEPTH);
	action.Add(COLLISION_STATE, SET_COLLISION_STATE);
	//action.Add(CONDITIONAL_ACTION, SET_CONDITIONAL_ACTION);
	action.Add(CREATE_ACTOR, SET_CREATE_ACTOR);
	action.Add(CREATE_TIMER, SET_CREATE_TIMER);
	action.Add(DESTROY_ACTOR, SET_DESTROY_ACTOR);
	action.Add(DESTROY_TIMER, SET_DESTROY_TIMER);
	action.Add(EVENT_ENABLE, SET_EVENT_ENABLE);
	action.Add(EVENT_ENABLE, SET_EVENT_DISABLE);
	action.Add(FOLLOW_MOUSE, SET_FOLLOW_MOUSE);
	action.Add(MOVE_TO, SET_MOVE_TO);
	action.Add(PHYSICAL_BOUNCE, SET_PHYSICAL_BOUNCE);
	action.Add(PLAY_MUSIC, SET_PLAY_MUSIC);
	action.Add(PLAY_SOUND, SET_PLAY_SOUND);
	action.Add(EXPRESSION_EDITOR, SET_EXPRESSION);
	action.Add(SET_TEXT, SET_SETTEXT);
	action.Add(TO_ANTERIOR_POSITION, SET_TO_ANTERIOR_POSITION);
	action.Add(VISIBILITY_STATE, SET_VISIBILITY_STATE);
    
	SetChoices(action);
	    
	m_index = -1;
	m_value = value;
	m_value.actionActor = m_value.actor;
	CreateProperties();    
	DoSetValue((void*)&value);
}

PropertyActionClass::~PropertyActionClass() 
{ 

}

int PropertyActionClass::GetChoiceInfo ( wxPGChoiceInfo* choiceinfo )
{
    if ( choiceinfo )
    {
        if ( !(m_flags & wxPG_PROP_STATIC_CHOICES) )
            choiceinfo->m_choices = &m_choices;

        if ( !m_choices.IsOk() )
            return -1;

        choiceinfo->m_itemCount = m_choices.GetCount();
        if ( m_choices.GetCount() )
            choiceinfo->m_arrWxString = (wxString*)&m_choices.GetLabel(0);
    }

    if ( !m_choices.IsOk() )
            return -1;

    return m_index;
}

int PropertyActionClass::GetValueFromIndex( int index ) const
{
    if ( !m_choices.IsOk() )
        return -1;

    const wxArrayInt& arrValues = m_choices.GetValues();

    if ( arrValues.GetCount() )
    {
        int value = arrValues.Item(index);

        // TODO: Use real default instead of 0.
        if ( value < 0 )
            value = 0;

        return value;
    }

    return index;
}

int PropertyActionClass::GetIndexForValue( int value ) const
{
    if ( !m_choices.IsOk() )
        return -1;

    const wxArrayInt& arrValues = m_choices.GetValues();

    if ( arrValues.GetCount() )
    {
        int intval = arrValues.Index(value);

        // TODO: Use real default instead of 0.
        if ( intval < 0 )
            intval = 0;

        return intval;
    }
    return value;
}

wxString PropertyActionClass::GetValueAsString ( int arg_flags ) const
{	
	return m_choices.GetLabels().Item(m_index);
}

bool PropertyActionClass::SetValueFromInt ( long index, int arg_flags )
{
	m_index = index;
	m_value.pAction->actionType = GetValueFromIndex(index);
	return true;
}

void PropertyActionClass::DoSetValue ( wxPGVariant value )
{
    stActionPtr* pObj = (stActionPtr*)wxPGVariantToVoidPtr(value);
    m_value = *pObj;
	copyEditAction = *m_value.pAction;

	UpdateCategoryName(wxPropertyContainerMethods::IsPropertyExpanded(GetId()));
	m_index = GetIndexForValue(m_value.pAction->actionType);	

	UpdateProperties(false);
    RefreshChildren();
}

wxPGVariant PropertyActionClass::DoGetValue () const
{
    return wxPGVariant((void*)&m_value);
}

void PropertyActionClass::UpdateCategoryName(bool bExpanded)
{
	gedString desc("Action");

	/*if(bExpanded)
	{
		desc = Action::GetDescription(m_value.pAction, false, true);
	}
	else
	{
		if(!m_value.pAction->getDescription().empty())
		{
			desc = Action::GetDescription(m_value.pAction, false, true);
			desc += " (";
			desc += m_value.pAction->getDescription();
			desc += ")";
		}
		else
		{
			desc = Action::GetDescription(m_value.pAction, false, false);
		}
	}*/

	SetLabel(desc.c_str());
}

void PropertyActionClass::CreateProperties()
{
	//Specific properties
	switch(m_value.pAction->actionType)
	{
	case SET_ANIMATION:
		CreateChangeAnimationProperties();
		break;
	}
}

void PropertyActionClass::UpdateProperties(bool bFromInterface)
{
	switch(m_value.pAction->actionType)
	{
	case SET_ANIMATION:
		UpdateChangeAnimationProperties(bFromInterface);
		break;
	}
}

void PropertyActionClass::CreateChangeAnimationProperties()
{
	SetToolTip(TIP_CHANGEANIMATION);

	//Actor
	idActor = AddChild(wxTreeProperty(wxT("Actor"), wxPG_LABEL) ); idActor.GetProperty().SetToolTip(wxString(TIP_CHANGEANIMATION_ACTOR) + TIP_ACTION_ACTOR);
	idActor.GetProperty().SetExtraFlags(ADD_CLONES | ADD_PARENT_ACTOR | ADD_EVENT_ACTOR);		
	PanelEvents::Get()->GetPropertyGrid()->SetPropertyColour(idActor, subCategoryColor);

	//Animation
	idAnimation = AddChild(wxTreeProperty(wxT("Animation"), wxPG_LABEL) ); idAnimation.GetProperty().SetToolTip(TIP_CHANGEANIMATION_ANIMATION);	
	idAnimation.GetProperty().SetExtraFlags(USE_ACTION_ACTOR);		
	PanelEvents::Get()->GetPropertyGrid()->SetPropertyColour(idAnimation, subCategoryColor);

	//Direction
	wxPGChoices direction;

    direction.Add ( wxT("Forward"), FORWARD );
    direction.Add ( wxT("Backward"), BACKWARD );
	direction.Add ( wxT("Stopped"), STOPPED );
	direction.Add ( wxT("No change"), NO_CHANGE );

    idDirection = AddChild(wxEnumProperty(wxT("Direction"), wxPG_LABEL, direction, FORWARD) ); idDirection.GetProperty().SetToolTip(TIP_CHANGEANIMATION_DIRECTION);
	PanelEvents::Get()->GetPropertyGrid()->SetPropertyColour(idDirection, subCategoryColor);
}

void PropertyActionClass::UpdateChangeAnimationProperties(bool bFromInterface)
{
	if(bFromInterface)
	{
		//Action::EventPathFinish(idPath.GetProperty().GetValueAsString().GetData());		
	}
	else
	{
		idActor.GetProperty().SetValueFromString(m_value.pAction->actorName);
		m_value.actionActor = GameControl::Get()->GetActor(m_value.pAction->actorName);
		
		if(m_value.actionActor == EVENT_ACTOR) m_value.actionActor = m_value.actor;
		else if(m_value.actionActor == PARENT_ACTOR && m_value.actor->getParent() && m_value.actor->getParent() != GameControl::Get()->GetAxis()) m_value.actionActor = m_value.actor->getParent();

		idAnimation.GetProperty().SetValueFromString(m_value.pAction->data.animation.name);
		idDirection.GetProperty().SetValueFromInt(m_value.pAction->data.animation.state, wxPG_FULL_VALUE);
	}
}



void PropertyActionClass::OnPropertyGridSelect(wxPGProperty *p)
{
	if(p->GetId() == idActor)
    {
		PropertyEventClass::UpdateActorProperty(m_value, idActor); 
	}
	else if(p->GetId() == idAnimation)
    {
		PropertyEventClass::UpdateAnimationProperty(m_value, idAnimation); 
	}
	else if(p->GetId() == idPath)
    {
		PropertyEventClass::UpdatePathProperty(idPath); 
	}
	else if(p->GetId() == idTimer)
    {
		PropertyEventClass::UpdateTimerProperty(idTimer); 
	}
}

void PropertyActionClass::ChildChanged ( wxPGProperty* p )
{
	if(!IS_VALID_ACTOR(m_value.actor)) return;

	if(p->GetId() == idActor && m_value.pAction->actionType == SET_ANIMATION)
    {
		m_value.actionActor = GameControl::Get()->GetActor(p->GetValueAsString().GetData());
		if(m_value.actionActor == EVENT_ACTOR)
		{
			m_value.actionActor = m_value.actor;
		}
		else if(m_value.actionActor == PARENT_ACTOR)
		{
			if(m_value.actor->getParent() != GameControl::Get()->GetAxis())
			{
				m_value.actionActor = m_value.actor->getParent();
			}
			/*else
			{
				new PanelInfo("This actor has no parent");
				listActor->SetItem(S_EVENT_ACTOR);
				actionActor = eventActor;
			}*/
		}
	}
	
	//Update values
	Action::setEditAction(m_value.pAction);
	Action::setActualEvent(m_value.pAction->eventData);
	UpdateProperties(true);
	
	//Update event lists
	Action::Call(m_value.actor)->Action::UpdateEvent();

	Action::clearEditAction();
}


void PropertyActionClass::RefreshChildren()
{
    if ( !GetCount() ) return;
    /*Item(0)->DoSetValue( (void*) &m_value.a );
    Item(1)->DoSetValue( (void*) &m_value.b );
    Item(2)->DoSetValue( (void*) &m_value.c );*/
}


