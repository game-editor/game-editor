// PanelProperty.cpp: implementation of the PanelProperty class.
//
//////////////////////////////////////////////////////////////////////

#include "PanelProperty.h"
#include "PanelActors.h"
//#include "PanelEvents.h"
#include "Path.h"
#include "../gameEditor/UndoControl.h"
#include "Behavior/wxJigsawEditor/wxJigsawEditorCanvas.h"


#define ID_MENU_ANIM_ADD_ANIMATION		1000
#define ID_MENU_ANIM_ADD_SEQUENCE		1000
#define ID_MENU_ANIM_REMOVE				1001
#define ID_MENU_PATH_ADD				1002
#define ID_MENU_PATH_REMOVE				1003

int getOffset(const char *varName);

void GridSelected(Actor *_actor)
{
	//No grid selection now
	//Can confuse the user
	//Grid is not an object!

	PanelProperty::Update(_actor);
	PanelActors::Update(NULL);
}

void UpdateActorSize()
{
	PanelProperty::UpdateActorSize();
}


IMPLEMENT_CLASS(PanelProperty, PanelGenericProperty)

BEGIN_EVENT_TABLE(PanelProperty, PanelGenericProperty)
    EVT_PG_CHANGED( PANEL_PROPERTY_ID, PanelProperty::OnPropertyGridChange )
	EVT_PG_SELECT( PANEL_PROPERTY_ID, PanelProperty::OnPropertyGridSelect )
	EVT_PG_ITEM_COLLAPSED(PANEL_PROPERTY_ID, PanelProperty::OnItemCollapsed)
	EVT_PG_ITEM_EXPANDED(PANEL_PROPERTY_ID, PanelProperty::OnItemExpanded)
	EVT_BUTTON( PANEL_PROPERTY_ID, PanelProperty::OnPropertyGridButtonClick )
	EVT_MENU( wxID_ANY, PanelProperty::OnMenuClick )
	EVT_COMMAND(wxID_ANY, wxEVT_BEHAVIOR_BLOCK_SELECTED, PanelProperty::OnBehaviorBlockSelected)
END_EVENT_TABLE()



PanelProperty *PanelProperty::panelProperty = NULL;
PanelProperty::PanelProperty(wxWindow *parent)  : PanelGenericProperty(parent, PANEL_PROPERTY_ID)
{	
	panelProperty = this;
	actor = NULL;
	
	wxPGChoices noSelection;
    noSelection.Add ( NO_SELECTION, 1 );

	///////////////////////////////////////////////////////
	idGeneral = pg->Append ( wxPropertyCategory(wxT("General")) );
    	
    idName = pg->Append ( wxStringProperty (wxT("Name"), wxPG_LABEL, wxT("")) ); 
	pg->DisableProperty ( idName ); //Enable only when implement rename

	idDescription = pg->Append ( wxStringProperty (wxT("Description"), wxPG_LABEL, wxT("")) ); 

    idCreateAtStartup = pg->Append ( wxBoolProperty ( wxT("Create at startup"), wxPG_LABEL, true ) ); idCreateAtStartup.GetProperty().SetToolTip(TIP_ACTORCONTROL_CREATE);
	idEditorAnimation = pg->Append ( wxBoolProperty ( wxT("Editor Animation"), wxPG_LABEL, false ) ); idEditorAnimation.GetProperty().SetToolTip(TIP_ACTORCONTROL_MOVE);

	idBackgroundColor = pg->Append ( wxColourProperty(wxT("Background color"), wxPG_LABEL, wxColour(0,0,0) ) ); idBackgroundColor.GetProperty().SetToolTip(TIP_GAMESETTINGS_BACKGROUNDCOLOR);
	

	///////////////////////////////////////////////////////
	idPosition = pg->Append ( wxPropertyCategory(wxT("Position")) );
    //pg->SetPropertyHelpString ( wxT("PositionCategory"), wxT("Change in items in this category will cause respective changes in frame.") );
	
    idX = pg->Append ( wxFloatProperty (wxT("x"),wxPG_LABEL,0) ); pg->SetPropertyEditor( idX, wxPG_EDITOR(SpinCtrl) ); idX.GetProperty().SetToolTip(TIP_ACTORCONTROL_X);
    idY = pg->Append ( wxFloatProperty (wxT("y"),wxPG_LABEL,0) ); pg->SetPropertyEditor( idY, wxPG_EDITOR(SpinCtrl) ); idY.GetProperty().SetToolTip(TIP_ACTORCONTROL_Y);

	idZDepth = pg->Append ( wxFloatProperty ( wxT("Z Depth"), wxPG_LABEL, 0 ) ); pg->SetPropertyEditor( idZDepth, wxPG_EDITOR(SpinCtrl) ); idZDepth.GetProperty().SetToolTip(TIP_ACTORCONTROL_ZDEPTH);
	SetFloatConstraints(idZDepth.GetPropertyPtr(), 0, 1, .01 );

	idGridZoom = pg->Append ( wxFloatProperty ( wxT("Zoom"), wxPG_LABEL, 1 ) ); pg->SetPropertyEditor( idGridZoom, wxPG_EDITOR(SpinCtrl) ); idGridZoom.GetProperty().SetToolTip(TIP_ACTORCONTROL_GRID_ZOOM);
	SetFloatConstraints(idGridZoom.GetPropertyPtr(), 0.05, 32, .2 );
	
	
	//Parent
    idParent = pg->Append(wxTreeProperty(wxT("Parent"), wxPG_LABEL) ); idParent.GetProperty().SetToolTip(TIP_ACTORCONTROL_PARENT);
	

	//Path
	idPath = pg->Append( wxCustomProperty(wxT("Path"),wxPG_LABEL) ); idPath.GetProperty().SetToolTip(TIP_ACTORCONTROL_PATH);
	pg->SetPropertyEditor( idPath, wxPG_EDITOR(ChoiceAndButton) );


	idLocked = pg->Append ( wxBoolProperty ( wxT("Locked"), wxPG_LABEL, false ) ); idLocked.GetProperty().SetToolTip(TIP_ACTORCONTROL_LOCK);

	///////////////////////////////////////////////////////
	idGrid = pg->Append ( wxPropertyCategory(wxT("Grid")) );
    	
	idGridShow = pg->Append ( wxBoolProperty ( wxT("Show"), wxPG_LABEL, true ) ); idGridShow.GetProperty().SetToolTip(TIP_GRID_SHOW);
	idGridSnap = pg->Append ( wxBoolProperty ( wxT("Snap"), wxPG_LABEL, true ) ); idGridSnap.GetProperty().SetToolTip(TIP_GRID_SNAP);

    idGridSizeX = pg->Append ( wxFloatProperty (wxT("Horizontal space"),wxPG_LABEL,0) ); pg->SetPropertyEditor( idGridSizeX, wxPG_EDITOR(SpinCtrl) ); idGridSizeX.GetProperty().SetToolTip(TIP_GRID_X);
	SetFloatConstraints(idGridSizeX.GetPropertyPtr(), 2, 200, 1 );

	idGridSizeY = pg->Append ( wxFloatProperty (wxT("Vertical space"),wxPG_LABEL,0) ); pg->SetPropertyEditor( idGridSizeY, wxPG_EDITOR(SpinCtrl) ); idGridSizeY.GetProperty().SetToolTip(TIP_GRID_Y);
	SetFloatConstraints(idGridSizeY.GetPropertyPtr(), 2, 200, 1 );

	idColorGrid1 = pg->Append ( wxColourProperty(wxT("Color"), wxPG_LABEL, wxColour(0,0,0) ) ); idColorGrid1.GetProperty().SetToolTip(TIP_GRID_MAINCOLOR);
	idColorGrid2 = pg->Append ( wxColourProperty(wxT("Resolution guide"), wxPG_LABEL, wxColour(0,0,0) ) ); idColorGrid2.GetProperty().SetToolTip(TIP_GRID_RESOLUTIONCOLOR);
    



	///////////////////////////////////////////////////////
	idSize = pg->Append ( wxPropertyCategory(wxT("Size")) );
    	
    idWidth = pg->Append ( wxFloatProperty (wxT("Width"),wxPG_LABEL,0) ); pg->SetPropertyEditor( idWidth, wxPG_EDITOR(SpinCtrl) ); idWidth.GetProperty().SetToolTip(TIP_ACTORCONTROL_WIDTH);
    idHeight = pg->Append ( wxFloatProperty (wxT("Height"),wxPG_LABEL,0) ); pg->SetPropertyEditor( idHeight, wxPG_EDITOR(SpinCtrl) ); idHeight.GetProperty().SetToolTip(TIP_ACTORCONTROL_HEIGHT);

	SetFloatConstraints(idWidth.GetPropertyPtr(), CORNER_SIDE, 8000000, 1 );
	SetFloatConstraints(idHeight.GetPropertyPtr(), CORNER_SIDE, 8000000, 1 );
	
	
	///////////////////////////////////////////////////////
	idAppearance = pg->Append( wxPropertyCategory(wxT("Appearance")) );
	
	idColor = pg->Append ( wxColourProperty(wxT("Color"), wxPG_LABEL, wxColour(0,0,0) ) ); idColor.GetProperty().SetToolTip(TIP_ACTORCONTROL_COLOR);
	//pg->SetPropertyEditor( idColor, wxPG_EDITOR(TextCtrl) ); pg->DisableProperty ( idColor ); //Remove comment to disable the button
	

	
	idTransparency = pg->Append ( wxFloatProperty ( wxT("Transparency"), wxPG_LABEL, 0 ) ); pg->SetPropertyEditor( idTransparency, wxPG_EDITOR(SpinCtrl) ); idTransparency.GetProperty().SetToolTip(TIP_ACTORCONTROL_TRANSPARENCY);
	SetFloatConstraints(idTransparency.GetPropertyPtr(), 0, 1, .01 );	
	

	
	//Animation
	idAnimation = pg->Append( wxCustomProperty(wxT("Animation"),wxPG_LABEL) ); 
	pg->SetPropertyEditor( idAnimation, wxPG_EDITOR(ChoiceAndButton) );


	//idAnimation = pg->Append( wxEnumProperty(wxT("Animation"), wxPG_LABEL, noSelection, 1) ); 
	
    idAnimationName = pg->AppendIn(idAnimation, wxStringProperty (wxT("Name"), wxPG_LABEL, wxT("")) ); idAnimationName.GetProperty().SetToolTip(TIP_ACTORCONTROL_ANIMATION_LIST);
	pg->SetPropertyColour(idAnimationName, subCategoryColor);

	pg->SetPropertyValue( idAnimation, pg->GetPropertyValueAsString(idAnimationName) );
	
	
	idAnimationFile = pg->AppendIn(idAnimation,  wxFileProperty( wxT("File"), wxPG_LABEL, wxEmptyString)); idAnimationFile.GetProperty().SetToolTip(TIP_ANIMATION_FILE);
	pg->SetPropertyColour(idAnimationFile, subCategoryColor);

	//wxT("All Formats|*.tga;*.bmp;*.pnm;*.xpm;*.xcf;*.pcx;*.gif;*.jpg;*.jpeg;*.jpe;*.jfi;*.tif;*.tiff;*.iff;*.lbm;*.png|JPEG Files (*.jpg)|*.jpg")
	idAnimationFile.GetProperty().SetAttrib(wxPG_FILE_SHOW_FULL_PATH,(long)0);
	pg->SetPropertyAttribute(idAnimationFile,
                                 wxPG_FILE_WILDCARD,
                                 wxT("All Formats|*.bmp;*.gif;*.jpg;|Jpeg|*.jpg|Bmp|*.bmp|Gif|*.gif"));

	idAnimationMultiple = pg->AppendIn(idAnimation, wxBoolProperty ( wxT("Multiple"), wxPG_LABEL, false )); idAnimationMultiple.GetProperty().SetToolTip(TIP_ANIMATION_TYPE);
	pg->SetPropertyColour(idAnimationMultiple, subCategoryColor);

	
	
	idAnimationHorizontalFrames = pg->AppendIn( idAnimation, wxFloatProperty(wxT("Horizontal frames"), wxPG_LABEL, 1) ); pg->SetPropertyEditor( idAnimationHorizontalFrames, wxPG_EDITOR(SpinCtrl) ); idAnimationHorizontalFrames.GetProperty().SetToolTip(TIP_ANIMATION_HORIZONTAL_FRAMES);
	SetFloatConstraints(idAnimationHorizontalFrames.GetPropertyPtr(), 1, 16000, 1 );
	pg->SetPropertyColour(idAnimationHorizontalFrames, subCategoryColor);

	idAnimationVerticalFrames = pg->AppendIn( idAnimation, wxFloatProperty(wxT("Vertical frames"), wxPG_LABEL, 1) ); pg->SetPropertyEditor( idAnimationVerticalFrames, wxPG_EDITOR(SpinCtrl) ); idAnimationVerticalFrames.GetProperty().SetToolTip(TIP_ANIMATION_VERTICAL_FRAMES);
	SetFloatConstraints(idAnimationVerticalFrames.GetPropertyPtr(), 1, 16000, 1 );
	pg->SetPropertyColour(idAnimationVerticalFrames, subCategoryColor);

	idAnimationFps = pg->AppendIn( idAnimation, wxFloatProperty(wxT("Frame rate"), wxPG_LABEL, 30) ); pg->SetPropertyEditor( idAnimationFps, wxPG_EDITOR(SpinCtrl) ); idAnimationFps.GetProperty().SetToolTip(TIP_ANIMATION_FPS);
	SetFloatConstraints(idAnimationFps.GetPropertyPtr(), 1, 512, 1 );
	pg->SetPropertyColour(idAnimationFps, subCategoryColor);
	
	
	
	
	
	
	//Infinite
	wxPGChoices infinite;

      // add extra items
    infinite.Add ( wxT("No"), NO_INFINITE );
    infinite.Add ( wxT("Horizontal"), X_INFINITE );
	infinite.Add ( wxT("Vertical"), Y_INFINITE );
	infinite.Add ( wxT("Both"), BOTH_INFINITE );


    idInfinite = pg->Append(wxEnumProperty(wxT("Infinite"), wxPG_LABEL, infinite, 1) ); idInfinite.GetProperty().SetToolTip(TIP_ACTORCONTROL_INFINITE);
	
	//////////////////////////////////////////////////
	idEvents = pg->Append( wxPropertyCategory(wxT("Events")) );

	idInheritEvents = pg->Append(wxTreeProperty(wxT("Inherit from"), wxPG_LABEL) ); idInheritEvents.GetProperty().SetToolTip(TIP_INHERIT_EVENTS_FROM);
	idReceiveEventsEvenIfOutOfVision = pg->Append ( wxBoolProperty ( wxT("Receive when out of vision"), wxPG_LABEL, true ) ); idReceiveEventsEvenIfOutOfVision.GetProperty().SetToolTip(TIP_OUT_OF_VISION_OPTIMIZATION);

	wxArrayString array;
	array.Add(NO_SELECTION);
	idInheritEvents.GetProperty().SetChoices(array);
	
	
	///////////////////////////////////////////////////////
	idUser = pg->Append( wxPropertyCategory(wxT("User")) );

	rightMargin = 0;

	//Use check box
	pg->SetPropertyAttributeAll(wxPG_BOOL_USE_CHECKBOX,(long)1);



	pg->ExpandAll();
	pg->Collapse(idAnimation);	

	m_menuAnim = NULL;
	m_menuPath = NULL;
	CreateMenu();
}

PanelProperty::~PanelProperty()
{
	panelProperty = NULL;
	delete m_menuAnim;
	delete m_menuPath;
}


void PanelProperty::OnItemCollapsed ( wxPropertyGridEvent& event )
{
	UpdateCategoryNames();
}

void PanelProperty::OnItemExpanded ( wxPropertyGridEvent& event )
{
	UpdateCategoryNames();
}

void PanelProperty::OnPropertyGridChange ( wxPropertyGridEvent& event )
{
	if(!IS_VALID_ACTOR(actor)) return;

	wxPGId id = event.GetProperty(), id1 = event.GetMainParent();
	wxVariant value = event.GetPropertyValue();

	if( id.GetProperty().GetParent() == idUser)
    {
		//User
		
		int offset = getOffset(id.GetProperty().GetLabel());
		if(offset >= 0)
		{
			char *type = Script::getLocalVars()[id.GetProperty().GetLabel()];

			if(type)
			{
				void *dst = ((char *)actor->getScriptVars()) + offset;

				switch(*type)
				{
					case INTEGER_VAR:
						{
							long val = value.GetLong();
							memcpy(dst, &val, sizeof(long));							
						}
						break;

					case REAL_VAR:
						{
							double val = value.GetDouble();
							memcpy(dst, &val, sizeof(double));
						}
						break;

					case STRING_VAR:
						strcpy((char *)dst, value.GetString().c_str());							
						break;
				}
			}
		}
	}
    else if( event.GetMainParent() == idAnimation)
    {
		//Animation
   		
        if ( id == idAnimationFile )
        {
            // Changing OnCustomPaint image
            wxImage image;
            wxString path = event.GetPropertyValueAsString();
			
            if ( ::wxFileExists(path) )
                image.LoadFile(path);
			
            if ( image.Ok() )
            {
                wxBitmap bmp(image);
                pg->SetPropertyImage( idAnimation, bmp );
            }
            else
            {
                pg->SetPropertyImage( idAnimation, wxNullBitmap );
            }

            Refresh();
        }
		else if ( id == idAnimationName )
        {
            // Changing label
            pg->SetPropertyValue( idAnimation, event.GetPropertyValueAsString() );
        }
		else if ( id == idAnimationMultiple )
        {
			if(event.GetPropertyValueAsBool())
			{
				pg->DisableProperty ( idAnimationHorizontalFrames );
				pg->DisableProperty ( idAnimationVerticalFrames );
			}
			else
			{
				pg->EnableProperty ( idAnimationHorizontalFrames );
				pg->EnableProperty ( idAnimationVerticalFrames );
			}
        }
    }
	else if(id == idDescription)
	{
		//Description
		actor->setDescription(idDescription.GetProperty().GetValueAsString().GetData());
	}
	else if(id == idX || id == idY)
	{
		//Position
		actor->SetPos(idX.GetProperty().GetValueAsDouble(), idY.GetProperty().GetValueAsDouble());
	}
	else if(id == idGridZoom)
	{
		//Grid Zoom
		GameControl::Get()->GetAxis()->SetScale(idGridZoom.GetProperty().GetValueAsDouble());
	}
	else if(id == idWidth || id == idHeight)
	{
		//Size
		actor->SetSize(idGridZoom.GetProperty().GetValueAsDouble(), idHeight.GetProperty().GetValueAsDouble());
	}
	else if(id == idCreateAtStartup)
	{
		if(id.GetProperty().GetValueAsBool()) actor->setCreateAtStartup(true);
		else 
		{
			if(actor->getParent() == GameControl::Get()->GetAxis())
			{
				actor->setCreateAtStartup(false);
			}
		}
	}
	else if(id == idEditorAnimation)
	{
		if(id.GetProperty().GetValueAsBool()) actor->setEditAnim(true);		
		else
		{
			actor->setEditAnim(false);	
			actor->setFramePathX(0);
			actor->setFramePathY(0);
			actor->getImage()->SetPos(actor->getPathIniX(), actor->getPathIniY());
			if(actor->getSprite()) actor->getSprite()->SetFrame(0);
		}
	}
	else if(id == idZDepth)
	{
		int zdepth = (int)(id.GetProperty().GetValueAsDouble()*MAX_ACTOR_DEPTH);

		if(actor != GameControl::Get()->GetAxis())
		{
			actor->getImage()->SetZDepth(zdepth);
		}
		else
		{
			GameControl::Get()->GetAxis()->SetGridZDepth(zdepth);
		}
		
	}
	else if(id == idTransparency)
	{
		actor->SetTransparency(id.GetProperty().GetValueAsDouble());
	}
	else if(id == idLocked)
	{
		actor->setLocked(id.GetProperty().GetValueAsBool());
	}
	else if(id == idColor)
	{
		wxColour color = *((wxColour*)idColor.GetProperty().DoGetValue().GetWxObjectPtr());
		actor->SetColor(color.Red(), color.Green(), color.Blue());
	}
	else if(id == idBackgroundColor)
	{
		wxColour color = *((wxColour*)idBackgroundColor.GetProperty().DoGetValue().GetWxObjectPtr());		
		GameControl::Get()->setBackGroundColor(KrRGBA(color.Red(), color.Green(), color.Blue()));
	}
	else if(id == idPath)
	{
		wxString path(id.GetProperty().GetValueAsString());
		if(path != NO_PATH)
		{
			actor->SetPath(path.GetData(), BOTH_AXIS);	
		}
		else
		{
			actor->SetPath("", BOTH_AXIS);
		}
	}
	else if(id == idInfinite)
	{
		actor->SetInfinite(id.GetProperty().DoGetValue().GetLong());	
	}
	else if(id == idParent)
	{
		int res;		

		if((res = actor->SetParent(idParent.GetProperty().GetValueAsString().GetData())) != PARENT_OK)
		{
			if(res != PARENT_NOT_FOUND)
			{
				//The remotion in UpdateParent may don't works 100%
				//So, keep this message here
				wxMessageBox(wxT("Actors can't be parented their own descendants"), wxT("Error"));
				actor->SetParent(NO_PARENT);
				pg->SetPropertyValue(idParent, NO_SELECTION);			
			}
		}

		UpdateCreateAtStartup();
	}
	else if(id == idInheritEvents)
	{
		wxString actorName(idInheritEvents.GetProperty().GetValueAsString());

		//Child
		if(!actor->getAction())
		{
			//Add an empty action
			Action::Call(actor);
		}
		
		
		if(actorName == NO_SELECTION) actor->getAction()->setInheritedEvents("");
		else 
		{
			//Parent
			Actor *parentEvent = GameControl::Get()->GetActor(actorName.GetData());
			if(!parentEvent->getAction())
			{
				//Add an empty action
				Action::Call(parentEvent);
			}
			
			
			if(!actor->getAction()->setInheritedEvents(actorName.GetData()))
			{
				wxMessageBox(wxT("Actors can't inherit events from your own descendants"), wxT("Error"));
			}
			
			if(actor->getAction()->getInheritedEventsActorName().empty())
			{
				pg->SetPropertyValue(idInheritEvents, NO_SELECTION);		
			}
			else
			{
				pg->SetPropertyValue(idInheritEvents, actor->getAction()->getInheritedEventsActorName().c_str());	
			}
		}
	}
	else if(id == idReceiveEventsEvenIfOutOfVision)
	{
		if(!actor->isView())
		{
			if(!actor->getAction())
			{
				//Add an empty action
				Action::Call(actor);
			}
			
			actor->setOutOfVisionOptimization(!id.GetProperty().GetValueAsBool());					
		}
	}
	else if(event.GetProperty().GetProperty().GetParent() == idGrid)
    {
	   		
        if(id == idGridShow)
        {
			GameControl::Get()->GetAxis()->setGridShow(idGridShow.GetProperty().GetValueAsBool());
		}
		else if(id == idGridSnap)
        {
			GameControl::Get()->GetAxis()->setGridSnap(idGridShow.GetProperty().GetValueAsBool());
		}
		else if(id == idGridSizeX)
        {
			GameControl::Get()->GetAxis()->setGridX(idGridSizeX.GetProperty().GetValueAsDouble());
		}
		else if(id == idGridSizeY)
        {
			GameControl::Get()->GetAxis()->setGridY(idGridSizeY.GetProperty().GetValueAsDouble());
		}
		else if(id == idColorGrid1)
        {
			wxColour color = *((wxColour*)idColorGrid1.GetProperty().DoGetValue().GetWxObjectPtr());
			GameControl::Get()->GetAxis()->setMainColor(KrRGBA(color.Red(), color.Green(), color.Blue()));
		}
		else if(id == idColorGrid2)
        {
			wxColour color = *((wxColour*)idColorGrid2.GetProperty().DoGetValue().GetWxObjectPtr());
			GameControl::Get()->GetAxis()->setResolutionColor(KrRGBA(color.Red(), color.Green(), color.Blue()));
		}

		GameControl::Get()->GetAxis()->getImage()->Invalidate();
	}

	UpdateCategoryNames();
	
	wxWindow *wndMouse = GetCapture();
	if(!wndMouse)
	{
		//Update undo only if the mouse is not capturated
		//Solve the slow down when change spin position
		UndoControl::Get()->PushCurrentState();
	}
}

void PanelProperty::UpdateCreateAtStartup()
{
	if(actor->getParent() && actor->getParent() != GameControl::Get()->GetAxis())
	{
		//Actor has a parent: disable create at startup
		actor->setCreateAtStartup(true);
		pg->SetPropertyValue(idCreateAtStartup, wxT("disable due parent"));
		pg->DisableProperty ( idCreateAtStartup );
	}
	else
	{
		pg->SetPropertyValue(idCreateAtStartup, wxT(""));
		pg->EnableProperty ( idCreateAtStartup );
	}

	pg->SetPropertyValue(idCreateAtStartup, actor->getCreateAtStartup());
}

void PanelProperty::Update(Actor *_actor)
{
	if(!panelProperty) return;

	if(!IS_VALID_ACTOR(_actor))
	{
		//Select view
		_actor = GameControl::Get()->GetActor("view");
	}

	panelProperty->actor = _actor;
	panelProperty->Update();
	
	//PanelEvents::Update(_actor);
}

void PanelProperty::Update()
{
	if(!IS_VALID_ACTOR(actor)) return;

	//pg->Freeze();

	//Update name
	if(actor != GameControl::Get()->GetAxis())
	{
		pg->SetPropertyValue(idName, actor->getCloneName());
	}
	else
	{
		pg->SetPropertyValue(idName, wxT("Stage"));
	}

	//Update description
	pg->SetPropertyValue(idDescription, actor->getDescription().c_str());

	//Update Create at Startup
	UpdateCreateAtStartup();

	//Net Actor
	//pg->SetPropertyValue(idNetActor, actor->getNetActor());

	//Update Editor Animation
	pg->SetPropertyValue(idEditorAnimation, actor->editAnim());

	//Update Position
	UpdateActorPosition();	

	//Update Size
	UpdateActorSize();

	//Update Z Depth
	if(actor != GameControl::Get()->GetAxis())
	{
		pg->SetPropertyValue(idZDepth, actor->getImage()->ZDepth()/(double)MAX_ACTOR_DEPTH);
	}
	else
	{
		pg->SetPropertyValue(idZDepth, GameControl::Get()->GetAxis()->GetGridZDepth()/(double)MAX_ACTOR_DEPTH);
	}

	//Update Parent
	UpdateParent();
	if(actor->getParent() && actor->getParent() != GameControl::Get()->GetAxis())
	{
		if(mapActors.FindString(actor->getParent()->getActorName())->Count() > 1)
		{
			if(GameControl::Get()->GetActor(actor->getParent()->getCloneName(), false))
			{
				pg->SetPropertyValue(idParent, actor->getParent()->getCloneName()); //solve the issue 119
			}
			else
			{
				pg->SetPropertyValue(idParent, actor->getParent()->getActorName());
			}
		}
		else
		{
			pg->SetPropertyValue(idParent, actor->getParent()->getActorName());
		}
	}
	else
	{		
		pg->SetPropertyValue(idParent, NO_SELECTION);
	}

	//Update idInheritEvents	
	if(actor->getAction() && !actor->getAction()->getInheritedEventsActorName().empty())
	{
		UpdateInheritEvents();
		pg->SetPropertyValue(idInheritEvents, actor->getAction()->getInheritedEventsActorName().c_str());
	}
	else
	{
		pg->SetPropertyValue(idInheritEvents, NO_SELECTION);
	}
	

	//Update path
	if(actor->getPathX())
	{
		pg->SetPropertyValue(idPath, actor->getPathX()->getName().getCharBuf());
	}
	else if(actor->getPathY())
	{
		pg->SetPropertyValue(idPath, actor->getPathY()->getName().getCharBuf());
	}
	else
	{
		pg->SetPropertyValue(idPath, NO_SELECTION);
	}

	//Update lock
	pg->SetPropertyValue(idLocked, actor->IsLocked());

	//Update color
	pg->SetPropertyValue(idColor, wxColour(actor->getScriptVars()->r, actor->getScriptVars()->g, actor->getScriptVars()->b));

	//Update Background Color
	pg->SetPropertyValue(idBackgroundColor, wxColour(GameControl::Get()->getBackGroundColor().c.red, GameControl::Get()->getBackGroundColor().c.green, GameControl::Get()->getBackGroundColor().c.blue));


	//Update transparency
	pg->SetPropertyValue(idTransparency, (255 - actor->getImage()->CTransform().Alpha())/255.0);

	//Update infinite
	pg->SetPropertyValue(idInfinite, actor->getInfinite());	
	
	//Update idReceiveEventsEvenIfOutOfVision
	pg->SetPropertyValue(idReceiveEventsEvenIfOutOfVision, !actor->getOutOfVisionOptimization());

	//Update grid
	if(actor == GameControl::Get()->GetAxis())
	{
		pg->SetPropertyValue(idGridShow, GameControl::Get()->GetAxis()->getGridShow());
		pg->SetPropertyValue(idGridSnap, GameControl::Get()->GetAxis()->getGridSnap());

		pg->SetPropertyValue(idGridSizeX, (double)GameControl::Get()->GetAxis()->getGridX());
		pg->SetPropertyValue(idGridSizeY, (double)GameControl::Get()->GetAxis()->getGridY());

		pg->SetPropertyValue(idColorGrid1, wxColour(GameControl::Get()->GetAxis()->getMainColor().c.red, GameControl::Get()->GetAxis()->getMainColor().c.green, GameControl::Get()->GetAxis()->getMainColor().c.blue));
		pg->SetPropertyValue(idColorGrid2, wxColour(GameControl::Get()->GetAxis()->getResolutionColor().c.red, GameControl::Get()->GetAxis()->getResolutionColor().c.green, GameControl::Get()->GetAxis()->getResolutionColor().c.blue));
	}

	UpdateOptionalProperties();
	UpdateCategoryNames();
	UpdateUser();

	//pg->Thaw();
	//Refresh();
}

void PanelProperty::UpdateCategoryNames()
{
	//Default values
	wxString	sGeneral(wxT("General")),
				sPosition(wxT("Position")),
				sSize(wxT("Size")),
				sAppearance(wxT("Appearance")),
				sEvents(wxT("Events"));

	//Update strings
	if(!wxPropertyContainerMethods::IsPropertyExpanded(idGeneral)) 
	{
		wxString sDescription(idDescription.GetProperty().GetValueAsString());
		if(!sDescription.IsEmpty())
		{
			sGeneral += wxT(" (") + sDescription + wxT(")");
		}
		else
		{
			sGeneral += wxT(" (") + idName.GetProperty().GetValueAsString() + wxT(")");
		}
	}

	if(!wxPropertyContainerMethods::IsPropertyExpanded(idPosition)) 
		sPosition += wxT(" (") + idX.GetProperty().GetValueAsString() + wxT(", ") + idY.GetProperty().GetValueAsString() + wxT(")");

	if(!wxPropertyContainerMethods::IsPropertyExpanded(idSize)) 
		sSize += wxT(" (") + idWidth.GetProperty().GetValueAsString() + wxT(", ") + idHeight.GetProperty().GetValueAsString() + wxT(")");

	wxString sAnimationName(idAnimationName.GetProperty().GetValueAsString());
	if(!wxPropertyContainerMethods::IsPropertyExpanded(idAppearance) && !sAnimationName.IsEmpty() && sAnimationName != NO_SELECTION) 
		sAppearance += wxT(" (") + sAnimationName + wxT(")");

	wxString sEventInherit(idInheritEvents.GetProperty().GetValueAsString());
	if(!wxPropertyContainerMethods::IsPropertyExpanded(idEvents) && sEventInherit != NO_SELECTION) 
		sEvents += wxT(" (Inherit from ") + sEventInherit + wxT(")");

	//Update grid
	pg->SetPropertyLabel(idGeneral, sGeneral);
	pg->SetPropertyLabel(idPosition, sPosition);
	pg->SetPropertyLabel(idSize, sSize);
	pg->SetPropertyLabel(idAppearance, sAppearance);
	pg->SetPropertyLabel(idEvents, sEvents);
}


void PanelProperty::UpdateOptionalProperties(bool bBehavior)
{
	//Manage optional properties
	Freeze();
	pg->Compact(false);	

	//Show some properties
	//pg->EnableProperty ( idName ); //only when implement rename
	idName.GetProperty().ClearFlag(wxPG_PROP_HIDEABLE);
	idX.GetProperty().ClearFlag(wxPG_PROP_HIDEABLE);
	idY.GetProperty().ClearFlag(wxPG_PROP_HIDEABLE);
	idZDepth.GetProperty().ClearFlag(wxPG_PROP_HIDEABLE);
	idDescription.GetProperty().ClearFlag(wxPG_PROP_HIDEABLE);
	idEditorAnimation.GetProperty().ClearFlag(wxPG_PROP_HIDEABLE);
	idParent.GetProperty().ClearFlag(wxPG_PROP_HIDEABLE);
	idPath.GetProperty().ClearFlag(wxPG_PROP_HIDEABLE);
	idLocked.GetProperty().ClearFlag(wxPG_PROP_HIDEABLE);
	idSize.GetProperty().ClearFlag(wxPG_PROP_HIDEABLE);
	idEvents.GetProperty().ClearFlag(wxPG_PROP_HIDEABLE);
	idUser.GetProperty().ClearFlag(wxPG_PROP_HIDEABLE);
	idCreateAtStartup.GetProperty().ClearFlag(wxPG_PROP_HIDEABLE);
	idAppearance.GetProperty().ClearFlag(wxPG_PROP_HIDEABLE);
	idColor.GetProperty().ClearFlag(wxPG_PROP_HIDEABLE);
	idTransparency.GetProperty().ClearFlag(wxPG_PROP_HIDEABLE);
	idAnimation.GetProperty().ClearFlag(wxPG_PROP_HIDEABLE);
	idInfinite.GetProperty().ClearFlag(wxPG_PROP_HIDEABLE);
	idReceiveEventsEvenIfOutOfVision.GetProperty().ClearFlag(wxPG_PROP_HIDEABLE);

	//Hide grid properties
	idGridZoom.GetProperty().SetFlag(wxPG_PROP_HIDEABLE);
	idBackgroundColor.GetProperty().SetFlag(wxPG_PROP_HIDEABLE);
	idGrid.GetProperty().SetFlag(wxPG_PROP_HIDEABLE);

	if(bBehavior)
	{
		//Behavior block selected
		idX.GetProperty().SetFlag(wxPG_PROP_HIDEABLE);
		idY.GetProperty().SetFlag(wxPG_PROP_HIDEABLE);
		idZDepth.GetProperty().SetFlag(wxPG_PROP_HIDEABLE);
		idDescription.GetProperty().SetFlag(wxPG_PROP_HIDEABLE);
		idEditorAnimation.GetProperty().SetFlag(wxPG_PROP_HIDEABLE);
		idParent.GetProperty().SetFlag(wxPG_PROP_HIDEABLE);
		idPath.GetProperty().SetFlag(wxPG_PROP_HIDEABLE);
		idLocked.GetProperty().SetFlag(wxPG_PROP_HIDEABLE);
		idSize.GetProperty().SetFlag(wxPG_PROP_HIDEABLE);
		idEvents.GetProperty().SetFlag(wxPG_PROP_HIDEABLE);
		idUser.GetProperty().SetFlag(wxPG_PROP_HIDEABLE);
		idCreateAtStartup.GetProperty().SetFlag(wxPG_PROP_HIDEABLE);
		idAppearance.GetProperty().SetFlag(wxPG_PROP_HIDEABLE);
		idColor.GetProperty().SetFlag(wxPG_PROP_HIDEABLE);
		idTransparency.GetProperty().SetFlag(wxPG_PROP_HIDEABLE);
		idAnimation.GetProperty().SetFlag(wxPG_PROP_HIDEABLE);
		idInfinite.GetProperty().SetFlag(wxPG_PROP_HIDEABLE);
		idReceiveEventsEvenIfOutOfVision.GetProperty().SetFlag(wxPG_PROP_HIDEABLE);
	}
	else if(actor == GameControl::Get()->GetAxis())
	{
		//Grid selected

		//idName.GetProperty().SetFlag(wxPG_PROP_HIDEABLE);
		//idX.GetProperty().SetFlag(wxPG_PROP_HIDEABLE);
		//idY.GetProperty().SetFlag(wxPG_PROP_HIDEABLE);
		idBackgroundColor.GetProperty().ClearFlag(wxPG_PROP_HIDEABLE);
		idSize.GetProperty().SetFlag(wxPG_PROP_HIDEABLE);
		//idZDepth.GetProperty().SetFlag(wxPG_PROP_HIDEABLE); //Remove this when implement grid depth zhange
		idGridZoom.GetProperty().ClearFlag(wxPG_PROP_HIDEABLE);
		idDescription.GetProperty().SetFlag(wxPG_PROP_HIDEABLE);
		idEditorAnimation.GetProperty().SetFlag(wxPG_PROP_HIDEABLE);
		idParent.GetProperty().SetFlag(wxPG_PROP_HIDEABLE);
		idPath.GetProperty().SetFlag(wxPG_PROP_HIDEABLE);
		idLocked.GetProperty().SetFlag(wxPG_PROP_HIDEABLE);
		idEvents.GetProperty().SetFlag(wxPG_PROP_HIDEABLE);
		idUser.GetProperty().SetFlag(wxPG_PROP_HIDEABLE);
		idCreateAtStartup.GetProperty().SetFlag(wxPG_PROP_HIDEABLE);
		idAppearance.GetProperty().SetFlag(wxPG_PROP_HIDEABLE);		
		idReceiveEventsEvenIfOutOfVision.GetProperty().SetFlag(wxPG_PROP_HIDEABLE);
		idGrid.GetProperty().ClearFlag(wxPG_PROP_HIDEABLE);
		pg->DisableProperty ( idName );
	}
	else if(!actor->getTextActor() && !actor->getTile() && !actor->isView() && actor->getType() != REGION_ACTOR && actor->getType() != REGION_ACTOR_FILLED && actor->getType() != CANVAS)
	{
		//Normal actor, hide text properties
		idSize.GetProperty().SetFlag(wxPG_PROP_HIDEABLE);	
	}
	else
	{
		//Must hide some properties
		if(actor->isView())
		{
			idCreateAtStartup.GetProperty().SetFlag(wxPG_PROP_HIDEABLE);
			idSize.GetProperty().SetFlag(wxPG_PROP_HIDEABLE);
			idAppearance.GetProperty().SetFlag(wxPG_PROP_HIDEABLE);
			idReceiveEventsEvenIfOutOfVision.GetProperty().SetFlag(wxPG_PROP_HIDEABLE);
			idEditorAnimation.GetProperty().SetFlag(wxPG_PROP_HIDEABLE);
			pg->DisableProperty ( idName );
		}
		else if(actor->getType() == REGION_ACTOR || actor->getType() == REGION_ACTOR_FILLED)
		{
			idAppearance.GetProperty().SetFlag(wxPG_PROP_HIDEABLE);
			idEditorAnimation.GetProperty().SetFlag(wxPG_PROP_HIDEABLE);
		}
		else if(actor->getType() == CANVAS)
		{
			idAnimation.GetProperty().SetFlag(wxPG_PROP_HIDEABLE);
			idEditorAnimation.GetProperty().SetFlag(wxPG_PROP_HIDEABLE);
		}
		else if(actor->getTile())
		{
			idInfinite.GetProperty().SetFlag(wxPG_PROP_HIDEABLE);
			idSize.GetProperty().SetFlag(wxPG_PROP_HIDEABLE);
		}
		else if(actor->getTextActor())
		{
			idAnimation.GetProperty().SetFlag(wxPG_PROP_HIDEABLE);
			idSize.GetProperty().SetFlag(wxPG_PROP_HIDEABLE);
			idEditorAnimation.GetProperty().SetFlag(wxPG_PROP_HIDEABLE);
		}		
	}

	pg->Compact(true);
	Thaw();
	Refresh();
}

void PanelProperty::UpdateActorPosition()
{
	if(!panelProperty || !IS_VALID_ACTOR(panelProperty->actor)) return;

	double	x = panelProperty->actor->getX(),
			y = panelProperty->actor->getY();

	if(panelProperty->actor == GameControl::Get()->GetAxis())
	{
		x = floor(x);
		y = floor(y);

		//Update Grid Zoom
		panelProperty->pg->SetPropertyValue(panelProperty->idGridZoom, GameControl::Get()->GetAxis()->getScale());
	}

	panelProperty->pg->SetPropertyValue(panelProperty->idX, x);
	panelProperty->pg->SetPropertyValue(panelProperty->idY, y);	

	panelProperty->UpdateCategoryNames();
}

void PanelProperty::UpdateActorSize()
{
	if(!panelProperty || !IS_VALID_ACTOR(panelProperty->actor)) return;

	panelProperty->pg->SetPropertyValue(panelProperty->idWidth, (double)panelProperty->actor->Width());
	panelProperty->pg->SetPropertyValue(panelProperty->idHeight, (double)panelProperty->actor->Height());

	panelProperty->UpdateCategoryNames();
}

void PanelProperty::UpdatePaths()
{
	if(!panelProperty) return;
	
	ListString listString;
	GameControl::Get()->PopulatePaths(listString);
	
	wxArrayString array(true);
	int i;
	
	for(i = 0; i < listString.Count(); i++)
	{			
		array.Add(listString[i].c_str());
	}
	
	panelProperty->idPath.GetProperty().SetChoices(array);
	panelProperty->Update();
}

void PanelProperty::UpdateParent()
{
	//Populate actors
	ListString listString;
	GameControl::Get()->PopulateActors(listString, true, true);
	
	wxArrayString array(true); //true to speed up remotion
	int i;
	
	for(i = 0; i < listString.Count(); i++)
	{			
		array.Add(listString[i].c_str());
	}
	
	array.Add(NO_SELECTION);

	//Remove descendants
	ListString descendantlList = actor->GetDescendantsList();

	//Include yourself
	if(mapActors[actor->getActorName()]->Count() > 1)
	{
		descendantlList.PushBack(gedString(actor->getActorName()) + "," + actor->getCloneName());
	}
	else
	{
		descendantlList.PushBack(actor->getActorName());
	}
				

	for(i = 0; i < descendantlList.Count(); i++)
	{		
		if(array.Index(descendantlList[i].c_str()) != wxNOT_FOUND)
		{
			array.Remove(descendantlList[i].c_str());
		}
	}
	

	//Set property	
	idParent.GetProperty().SetChoices(array); 
}

void PanelProperty::UpdateInheritEvents()
{
	ListString listString;
	GameControl::Get()->PopulateActors(listString, false, true);
	
	wxArrayString array(true);
	int i;
	
	for(i = 0; i < listString.Count(); i++)
	{	
		if(Action::CanSetInheritedEvents(actor->getActorName(), listString[i]))
		{
			array.Add(listString[i].c_str());
		}
	}
	
	array.Add(NO_SELECTION);

	//Set property
	idInheritEvents.GetProperty().SetChoices(array); 	
}

void PanelProperty::UpdateUser()
{
	//TODO: Must be called only when the Script::getLocalVars changes or to update the properties values for an actor
	//TODO: Put a button to remove a propertie (TextCtrlAndButton, SpinCtrlAndButton)
	//TODO: Put a button to add a propertie (beside the User)

	if(!IS_VALID_ACTOR(actor)) return;

	Freeze();
	pg->Compact(false);	
	
	//Delete old properties
	wxPGId idDel;

	while(1)
	{
		idDel = pg->GetFirstChild(idUser);
		if(idDel.IsOk()) pg->Delete(idDel);
		else break;
	} 

	//Add new
	MapUsedActorIterator it(Script::getLocalVars());	
	for(it.Begin(); !it.Done(); it.Next())
	{
		Uint8 type = *it.Value();
		int offset;
		wxPGId id;


		switch(type)
		{
		case INTEGER_VAR:
			{
				id = pg->AppendIn(idUser, /*wxIntProperty*/wxFloatProperty (it.Key()->c_str(), wxPG_LABEL,0) );
				pg->SetPropertyEditor( id, wxPG_EDITOR(SpinCtrl) );
				id.GetProperty().SetToolTip("Integer");				
			}
			break;

		case REAL_VAR:
			{
				id = pg->AppendIn(idUser, wxFloatProperty (it.Key()->c_str(), wxPG_LABEL,0) );
				pg->SetPropertyEditor( id, wxPG_EDITOR(SpinCtrl) );
				id.GetProperty().SetToolTip("Real");
			}
			break;

		case STRING_VAR:
			{
				id = pg->AppendIn(idUser, wxStringProperty (it.Key()->c_str(), wxPG_LABEL, wxT("")) );
				//pg->SetPropertyEditor(id, wxPG_EDITOR(TextCtrlAndButton));
				id.GetProperty().SetToolTip("String");
			}
			break;
		}

		//Get the value from actor
		offset = getOffset(it.Key()->c_str());
		if(offset >= 0)
		{
			void *src = ((char *)actor->getScriptVars()) + offset;
			switch(type)
			{
			case INTEGER_VAR:
				{
					long val;
					memcpy(&val, src, sizeof(long));
					pg->SetPropertyValue(id, (double)val);
				}
				break;

			case REAL_VAR:
				{
					double val;
					memcpy(&val, src, sizeof(double));
					pg->SetPropertyValue(id, val);
				}
				break;

			case STRING_VAR:
				pg->SetPropertyValue(id, (char *)src);
				break;
			}
		}
	}

	pg->Compact(true);
	Thaw();
	Refresh();
}

void PanelProperty::OnPropertyGridSelect ( wxPropertyGridEvent& event )
{
	if(!IS_VALID_ACTOR(actor)) return; 

	//Some property will be selected
	wxPGId id = event.GetProperty();
	
	if ( id == idParent )
    {
		//Populate now
		UpdateParent(); 
	}
	else if ( id == idInheritEvents)
    {
		//Populate now
		UpdateInheritEvents(); 
	}
	else if ( id == idPath)
	{
		//Populate now
		UpdatePaths();
	}
}

void PanelProperty::OnPropertyGridButtonClick(wxCommandEvent &event)
{
	wxPGId id = pg->GetSelectedProperty();

    if ( id.IsOk() )
    {
		//Set popup position
		wxButton *pButton = wxDynamicCast(event.GetEventObject(), wxButton);
		wxPoint pt = wxPoint(5, 17);
		
		if(pButton)
		{
			wxRect rect(pButton->GetRect());
			pt = rect.GetPosition();
			
			pt.x += rect.GetWidth();
			pt.y += rect.GetHeight();
		}

		//Selection
		wxString sSelection(id.GetProperty().GetValueAsString());

		//Open menu
		if(id == idAnimation)
		{
			if(!actor->NumAnimations() || sSelection.IsEmpty())
			{
				m_menuAnim->Enable(ID_MENU_ANIM_REMOVE, false);
			}
			else
			{
				m_menuAnim->Enable(ID_MENU_ANIM_REMOVE, true);
			}

			PopupMenu(m_menuAnim, pt);
		}
		else if(id == idPath)
		{
			if( !GameControl::Get()->NumPaths() || 
				sSelection == NO_SELECTION ||
				sSelection == RANDOM_PATH )
			{
				m_menuPath->Enable(ID_MENU_PATH_REMOVE, false);
			}
			else
			{
				m_menuPath->Enable(ID_MENU_PATH_REMOVE, true);
			}

			PopupMenu(m_menuPath, pt);
		}
    }

	event.Skip();
}

void PanelProperty::CreateMenu()
{
	if(!m_menuAnim)
	{
		m_menuAnim = new wxMenu;

		m_menuAnim->Append(ID_MENU_ANIM_ADD_ANIMATION, wxT("Add"));
		m_menuAnim->Append(ID_MENU_ANIM_REMOVE, wxT("Remove"));
	}

	if(!m_menuPath)
	{
		m_menuPath = new wxMenu;

		m_menuPath->Append(ID_MENU_PATH_ADD, wxT("Add"));
		m_menuPath->Append(ID_MENU_PATH_REMOVE, wxT("Remove"));
	}
	
}

void PanelProperty::OnMenuClick(wxCommandEvent& event)
{	
	switch(event.GetId())
	{
	case ID_MENU_ANIM_ADD_ANIMATION:
		break;

	case ID_MENU_ANIM_REMOVE:
		break;

	case ID_MENU_PATH_ADD:
		break;

	case ID_MENU_PATH_REMOVE:
		break;
	}
}

void PanelProperty::OnBehaviorBlockSelected( wxCommandEvent &event ) //maks:teste
{
	if(event.GetClientData()) 
	{
		pg->SetPropertyValue(idName, event.GetString());   
	}
	else
	{
		pg->SetPropertyValue(idName, "");
	}

	UpdateOptionalProperties(true);
}