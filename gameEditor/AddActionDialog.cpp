/**************************************************************************

Game Editor - The Cross Platform Game Creation Tool
Copyright (C) 2009  Makslane Araujo Rodrigues, http://game-editor.com, http://game-editor.com

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
Be a Game Editor developer: Be a Game Editor developer: <http://game-editor.com/Sharing_Software_Revenues_in_Open_Source>  .

***************************************************************************/


// AddActionDialog.cpp: implementation of the AddActionDialog class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STAND_ALONE_GAME

#include "AddActionDialog.h"
#include "ActorProperty.h"
#include "GameControl.h"
#include "Tutorial.h"
#include "Config.h"
#include "AddActor.h"






enum
{
	BT_OK,
	BT_CANCEL,
	LS_FILETYPE,
	LS_FILE,
	BT_WEB,
	BT_EDIT_FRAME,
	BT_PLAY,
	LS_PLAY
};

#define WIDTH			346
#define HEIGHT			185
#define SAMPLE_SIDE		126
#define SAMPLE_X		(WIDTH - SAMPLE_SIDE - 5)
#define SAMPLE_Y		23

#define SINGLE_FILE		"Single File"
#define MULTIPLE_FILE   "Multiple Files"

char imgPath[PATH_LIMIT];


class EditBox;
static bool bInitPath = false;

static int getHeight(bool bReplaceFile)
{
#ifdef WIN32
	if(bReplaceFile && Tutorial::IsCompatible(VERSION_PREFERENCES))
	{
		return HEIGHT + 25;
	}
#endif

	return HEIGHT;
}

static int getWidth()
{
#ifdef WIN32
	if(Tutorial::IsCompatible(VERSION_PREFERENCES))
	{
		return WIDTH + 10;
	}
#endif

	return WIDTH;
}

AddActionDialog::AddActionDialog(ActorProperty *actorProperty, Actor *actor, KrAction *replaceAnimInfo, bool _bFromWizard)
: Panel("AddActionDialog", (GameControl::Get()->Width() - getWidth())/2, 
				          (GameControl::Get()->Height() - getHeight(replaceAnimInfo != NULL))/2,
						  getWidth(), getHeight(replaceAnimInfo != NULL))
{
	SetModal();
	SetToolTip(TIP_ANIMATION);

	this->actorProperty = actorProperty;
	this->actor = actor;
	bReplaceFile = replaceAnimInfo != NULL;
	bFromWizard = _bFromWizard;

	sampleAdition = 0;

	editFrame = NULL;
	listPlay = NULL;

	Text *text;
	Button *button;	
	int y;

	text = AddText(bReplaceFile?"Replace Animation":"Add Animation", CENTER_TEXT, 5);
	DrawHLine(text->Down() + 2);

	text = AddText("Name: ", 10, text->Down() + 8);
	name = AddEditBox(text->Right(), text->Top(), 128);

	text = AddText("File: ", 10, name->Down() + 2);
	path = AddListDirectory(text->Right(), text->Top(), 128, 0, LS_FILE); path->SetToolTip(TIP_ANIMATION_FILE);

	//button = AddButton("Web", path->Right() + 2, path->Top(), 35, 0, BT_WEB); button->SetToolTip(TIP_ANIMATION_WEB);


	fileType = AddListPop(path->Left(), path->Down(), 128, 0, LS_FILETYPE); fileType->SetToolTip(TIP_ANIMATION_TYPE);

	text = AddText("Horizontal Frames: ", 10, fileType->Down() + 5);
	hframes = AddEditBox(text->Right(), text->Top(), 40); hframes->SetNumeric(1, 16000, 1);
	hframes->SetText("1");

	text = AddText("  Vertical Frames: ", 10, hframes->Down() + 2);
	vframes = AddEditBox(text->Right(), text->Top(), 40); vframes->SetNumeric(1, 16000, 1);
	vframes->SetText("1");

	text = AddText("       Frame rate: ", 10, vframes->Down() + 2);
	editFps = AddEditBox(text->Right(), text->Top(), 40); editFps->SetNumeric(1, GameControl::Get()->getFrameRate(), 1);
	text = AddText("fps", editFps->Right() + 2, editFps->Top());
	editFps->SetText(GameControl::Get()->getFrameRate());

	
	if(bReplaceFile)
	{
		name->SetText(replaceAnimInfo->Name());
		name->Disable(); //Disable here before get focus
	}	

	y = editFps->Down();

#ifdef WIN32
	if(Tutorial::IsCompatible(VERSION_PREFERENCES))
	{
		if(bReplaceFile)
		{
			button = AddButton("Edit", SAMPLE_X, SAMPLE_Y + SAMPLE_SIDE + 2, 40, 0, BT_EDIT_FRAME); button->SetToolTip(TIP_EDIT_FRAME);
			editFrame = AddEditBox(button->Right() + 2, button->Top() + 2, 40); editFrame->SetNumeric(0, replaceAnimInfo->NumFrames() - 1, 1);
			button = AddButton("Play", editFrame->Right() + 3, button->Top(), 40, 0, BT_PLAY); button->SetToolTip(TIP_ANIMATION_PLAY);
			
			y = button->Down();
		}
		
		sampleAdition = 10;
	}
#endif

	
	//End dialog
	y = DrawHLine(y + 5);
	button = AddButton("Ok", (getWidth() - 135)/2, y, 0, 0, BT_OK); //SetConfirmButton(button);
	button = AddButton("Cancel", button->Right() + 5, button->Top(), 0, 0, BT_CANCEL); //SetCancelButton(button);

	path->AddFilter("tga");
	path->AddFilter("bmp");
	path->AddFilter("pnm");
	path->AddFilter("xpm");
	path->AddFilter("xcf");
	path->AddFilter("pcx");
	path->AddFilter("gif");
	path->AddFilter("jpg");
	path->AddFilter("jpeg");
	path->AddFilter("jpe");
	path->AddFilter("jfif");
	path->AddFilter("tif");
	path->AddFilter("tiff");
	path->AddFilter("iff");
	path->AddFilter("lbm");
	path->AddFilter("png");


	fileType->AddText(SINGLE_FILE);
	fileType->AddText(MULTIPLE_FILE);
	fileType->SetItem(SINGLE_FILE);

	
	if(!Tutorial::IsOff())
	{
		chdir(Tutorial::Get()->getTutorialPath());
	}
	else
	{
		if(!bInitPath)
		{
			imgPath[0] = 0;
			bInitPath = true;

			if(!Config::Get()->getPathAnimations().empty())
			{
				chdir(Config::Get()->getPathAnimations().c_str());
			}
		}
		else chdir(imgPath);
	}

	if(bReplaceFile)
	{
		//Don't use GetFilePath()
		//SplitPath update the path relative to game directory

		gedString file, dir;
		SplitPath(replaceAnimInfo->getimgName(), file, dir);
		chdir(dir.c_str());

		if(replaceAnimInfo->getimgName().find('*') != gedString::npos)
		{
			hframes->SetText("*");
			vframes->SetText("0");

			fileType->SetItem(MULTIPLE_FILE);

			//Get first valid file
			//Find *
			int numberPos = file.find('*');
			long hnd = ged_findfirst(file.c_str(), A_ANY);
			if(hnd != GED_NOTFOUND)
			{	
				//Read file names and sort
				ListString listFiles;
				do
				{
					char *fileName = ged_filename(hnd), *name = fileName + numberPos;

					//Add only the files in same sequence
					if(isdigit(*name))
					{
						file = fileName;
						break;
					}

				} while((hnd = ged_findnext(hnd)) != GED_NOTFOUND);
				
				ged_findclose(hnd);
			}
		}
		else
		{
			hframes->SetText(replaceAnimInfo->getnFramesH());
			vframes->SetText(replaceAnimInfo->getnFramesV());
		}

		editFps->SetText(replaceAnimInfo->getFrameRate());

		path->AddText(file);
		path->SetItem(file);		
	}

	//Sample
	animationSample = new Actor("AnimationSample", this);
	animationSample->AddAnimation(ACTOR_GED_ICON, "maksiconActor.gif", 1, 1);
	animationSample->getImage()->SetVisible(false);
	animationSample->setEditAnim(true);

	

	//Fill background
	KrRGBA black;
	black.c.red		= 0;
	black.c.green	= 0;
	black.c.blue	= 0;
	black.c.alpha	= 255;

	
	KrRGBA *pixels = getCanvasResource()->Pixels();
	for(int i = SAMPLE_X; i < SAMPLE_X + SAMPLE_SIDE + sampleAdition; i++)
	{
		for(int j = SAMPLE_Y; j < SAMPLE_Y + SAMPLE_SIDE; j++)
		{
			pixels[ j*Width() + i ] = black;
		}
	}


	

	if(Tutorial::IsCompatible(VERSION_OUT_OF_VISION_OPTIMIZATION))
	{
		bAutopopulateName = true;
	}
	else
	{
		bAutopopulateName = false;
	}

	if(bReplaceFile)
	{
		UpdateSample(false);
	}
}

AddActionDialog::~AddActionDialog()
{
	delete animationSample;	
}

void AddActionDialog::ResetPath()
{
	bInitPath = false;
}

bool AddActionDialog::OnListButtonDown(ListPop *list, int listId)
{
	if(listId == LS_FILE)
	{
		if(HasEditedFrames())
		{
			PanelQuestion *panel = new PanelQuestion("This will discard the edited frames.\nProceed anyway?");
			if(panel->Wait() == CANCEL_BUTTON)
			{
				delete panel;
				return false;
			}

			delete panel;
		}
	}

	return true;
}


bool AddActionDialog::OnList(ListPop *list, int index, gedString &text, int listId)
{
	if(listId == LS_FILETYPE || listId == LS_FILE)
	{
		if(listId == LS_FILETYPE)
		{
			if(text == 	MULTIPLE_FILE)
			{
				hframes->SetText("*");
				vframes->SetText("0");
			}
			else
			{
				hframes->SetText("1");
				vframes->SetText("1");
			}
		}

		if(listId == LS_FILE)
		{
			if(bAutopopulateName && !bReplaceFile)
			{
				//Autopopulate animation name with name of file selected
				gedString fileName(text);

				int i = fileName.rfind('.');
				if(i != gedString::npos)
				{
					fileName = fileName.substr(0, i);
				}

				name->SetText(fileName.substr(0, NAME_LIMIT-1));
			}
		}
		
		UpdateSample(listId == LS_FILE);
	}
	
	return true;
}

void AddActionDialog::OnAnimation()
{
	static int lastFrame = -1;
	if(editFrame && animationSample->editAnim() && animationSample->getSprite()->Frame() != lastFrame)
	{
		editFrame->SetText(animationSample->getSprite()->Frame());
		lastFrame = animationSample->getSprite()->Frame();
	}

	if(bReplaceFile)
	{
		//One second check

		static long n = 0;
		if(n > GameControl::Get()->getFrameRate())
		{			
			CheckFrameChanges();
			n = 0;
		}

		n++;
	}
}
void AddActionDialog::OnEditSelection(EditBox *edit)
{
	if(edit == editFrame)
	{
		animationSample->setEditAnim(false);
	}
}

void AddActionDialog::OnEditChar(EditBox *edit, SDLKey key, int line)
{
	if(edit == editFps)
	{
		int fps = atol(editFps->GetText().c_str());
	
		if(fps < 1) fps = 1;
		else if(fps > 512) fps = 512;

		animationSample->getSprite()->GetAction()->setFrameRate(fps);
	}
	else if(edit == hframes || edit == vframes)
	{
		UpdateSample(false);
	}
	else if(edit == name)
	{
		bAutopopulateName = false;
	}
	else if(edit == editFrame)
	{
		KrAction *anim = animationSample->getSprite()->GetAction();

		if(anim)
		{
			int frame = atol(editFrame->GetText().c_str());
			
			if(frame < 0) 
			{
				frame = 0;
				editFrame->SetText(frame);	
			}
			else if(frame > anim->NumFrames() - 1) 
			{
				frame = anim->NumFrames() - 1;
				editFrame->SetText(frame);	
			}
			
			animationSample->setEditAnim(false);
			animationSample->SetFrame(frame);
			
		}
	}
}

void AddActionDialog::UpdateSample(bool bChangeFile)
{
	WaitCursor wait;

	int nh = atol(hframes->GetText().c_str());
	int nv = atol(vframes->GetText().c_str());
	int fps = atol(editFps->GetText().c_str());
	
	if(fps < 1) fps = 1;
	else if(fps > 512) fps = 512;
	
	if(nh < 1) nh = 1;
	else if(nh > 16000) nh = 16000;
	
	if(nv < 1) nv = 1;
	else if(nv > 16000) nv = 16000;

	gedString fullPath(path->GetFullPath());
	if(fileType->GetText() == MULTIPLE_FILE)
	{
		fullPath = ListDirectory::GetMultipleFileName(fullPath);		
	}
	else if(bChangeFile)
	{
		//Calculate nh and nv
		nh = nv = -1;
	}
	
	animationSample->RemoveAnimation("SAMPLE");
	if(animationSample->AddAnimation("SAMPLE", fullPath, nh, nv, -1, -1, fps))
	{
		animationSample->getImage()->SetVisible(true);
		animationSample->SetAnimation("SAMPLE");
		animationSample->SetActorScale(1.0);
		animationSample->getImage()->CalcTransform();

		int maxWidth = 0, maxHeight = 0;

		for(int i = 0; i < animationSample->getScriptVars()->nframes; i++)
		{
			animationSample->SetFrame(i);
			
			maxWidth = max(maxWidth, animationSample->Width());
			maxHeight = max(maxHeight, animationSample->Height());
		}

				
		double scale = SAMPLE_SIDE/(double)max(maxWidth, maxHeight);
		if(scale < 1.0)
		{
			animationSample->SetActorScale(scale);
		}



		animationSample->SetPos(SAMPLE_X + (SAMPLE_SIDE + sampleAdition)/2, SAMPLE_Y + SAMPLE_SIDE/2);

		if(nh == -1)
		{
			hframes->SetText(animationSample->getSprite()->GetAction()->getnFramesH());
			vframes->SetText(animationSample->getSprite()->GetAction()->getnFramesV());
		}
	}
	else
	{
		animationSample->getImage()->SetVisible(false);
		animationSample->SetAnimation(ACTOR_GED_ICON);
	}

	if(editFrame) editFrame->SetNumeric(0, animationSample->getSprite()->GetAction()->NumFrames() - 1, 1);

	RemoveEditedFrames();
	getImage()->Invalidate();
}

void AddActionDialog::ContinueWizard(Actor *actor)
{
	if(bFromWizard)
	{
		PanelQuestion *panel = new PanelQuestion("Do you want to add another actor?");
		if(panel->Wait() == OK_BUTTON)
		{
			new AddActor(true);
		}
		else
		{
			ActorProperty::Call(actor);
			new PanelInfo("Now, you can use the Actor Control panel to\nadd events and actions in your actors,\nadd new animations to your actors, among others.\n\nYou can add other actors by using the 'Add Actor' option in main menu", "Info", ALIGN_LEFT);			
		}
		
		delete panel;
	}
}

void AddActionDialog::OnButton(Button *button, int buttonId)
{
	switch(buttonId)
	{
	case BT_OK:
		{
			gedString pathArq = path->GetFullPath();
			if(name->GetText().size() > 0 && name->GetText().size() < NAME_LIMIT && (bReplaceFile || !actor->ExistsAction(name->GetText())))
			{
				SDL_RWops *src = ged_SDL_RWFromFile(pathArq.c_str(), "rb");
				if(src)
				{
					int nh = atol(hframes->GetText().c_str());
					int nv = atol(vframes->GetText().c_str());
					int fps = atol(editFps->GetText().c_str());

					SDL_RWclose(src);

					SDL_Surface* surface = ged_IMG_Load(pathArq.c_str());
					if(!surface)
					{
						new PanelInfo("This format is not supported or file is corrupted\nPlease, select other file");
						return;
					}
					// Division by zero avoided AKR
					if(nh && nv && ((surface->w/nh) > 1019 || (surface->h/nv) > 1019) && Config::Get()->getShowIPhoneImageSizeReminder())
					{
						//Remove this when solve the ticket http://game-editor.com/ticket/24
						
						PanelQuestion *panel = new PanelQuestion("If you are creting an iPhone game, the max frame size must be (1019x1019). \nShow this message again?", "Warning");
						if(panel->Wait() == CANCEL_BUTTON)
						{
							Config::Get()->setShowIPhoneImageSizeReminder(false);							
						}

						delete panel;
					}

					if(surface->w >= 8192 || surface->h >= 8192)
					{
						new PanelInfo("Maximum image size is 8191 x 8191");
						SDL_FreeSurface( surface );
						return;
					}

					//Ok
					SDL_FreeSurface( surface );
					
					
					
					if((nh < 1 || nh >= 16000) && (nv < 1 || nv >= 16000) && fileType->GetText() != MULTIPLE_FILE)
					{
						new PanelInfo("Please, enter valid frame numbers");
						return;
					}

					if(fps < 1 || fps > 512)
					{
						new PanelInfo("Frame rate is invalid");
						return;
					}
					
					
					gedString fullPath = pathArq;
					if(fileType->GetText() == MULTIPLE_FILE)
					{
						fullPath = ListDirectory::GetMultipleFileName(fullPath);
						if(fullPath == pathArq)
						{
							new PanelInfo("Only one single file has found.\nPlease, select single file mode for this file");
							return;
						}
					}

					if(bReplaceFile) ConfirmEditedFrames();
					
					{
						WaitCursor wait; //Destroy wait cursor before call wizard
						
						actor->AddAnimation(name->GetText(), fullPath, nh, nv, -1, -1, fps, true, bReplaceFile);
						if(actor->getSprite())
						{
							actor->SetAnimation(name->GetText());
							if(bReplaceFile) 
							{
								//Set all clones
								ListActor *listActor = mapActors[actor->getActorName()];
								if(listActor)
								{
									for(int il = 0; listActor && il < listActor->Count(); il++)
									{
										Actor *clone = (*listActor)[il];
										if(clone->GetAnimation() == name->GetText())
										{
											clone->SetAnimation(name->GetText());
										}
									}
								}
							}
							else
							{
								if(actorProperty) actorProperty->AddAction(name->GetText());
							}
						}
						
						//Store current path
						ged_getcwd(imgPath, PATH_LIMIT);
						Config::Get()->setPathAnimations(imgPath);
						PostMessage(this, DELETE_ME, DELETE_ME);
					}

					ContinueWizard(actor);
					
				}
				else
				{
					//Erro	
					new PanelInfo("Please, select a file");
				}
			}
			else
			{
				if(name->GetText().size() <= 0 || name->GetText().size() > NAME_LIMIT)
					new PanelInfo("Please, enter valid animation name");	
				else
					new PanelInfo("Animation already exists\nEnter another animation name");	
			}
		}
		break;

	case BT_CANCEL:
		{
			if(HasEditedFrames())
			{
				PanelQuestion *panel = new PanelQuestion("This will discard the edited frames.\nProceed anyway?");
				if(panel->Wait() == CANCEL_BUTTON)
				{
					delete panel;
					return;
				}

				delete panel;
			}

			RemoveEditedFrames();
			PostMessage(this, DELETE_ME, DELETE_ME);

			ContinueWizard(actor);
		}
		break;
	case BT_WEB:
		{
			openUrl("http://game-editor.com/graphics.html");
		}
		break;

#ifdef WIN32
	case BT_EDIT_FRAME:
		{
			if(Config::Get()->getImageEditor().empty())
			{
				new PanelInfo("Please, select an image editor in the Preferences panel\n\nIf you don't have an image editor, use a search engine and\nput the query \"Image Editors with PNG Support\" to find\na good image editor.",
					"Error", ALIGN_LEFT);
				return;
			}

			if(GameControl::Get()->getGamePath().empty())
			{
				new PanelInfo("Please, save your game before edit an animation frame");
				return;
			}

			gedString imagePath(GameControl::Get()->getGamePath() + DIR_SEP + "data"), imageName;

			KrAction *anim = animationSample->getSprite()->GetAction();
			
			if(anim)
			{
				int frame = atol(editFrame->GetText().c_str());
				
				if(frame < 0) frame = 0;
				else if(frame > anim->NumFrames() - 1) frame = anim->NumFrames() - 1;

				animationSample->setEditAnim(false);

				EditorDirectory dir(imagePath);

				KrRle *rle = anim->GetFrame(frame);
				if(rle)
				{
					char buf[8];	
					sprintf(buf, "%04ld", frame);
					
					imageName = "tmp_";
					imageName += GetUpdateImageName(anim->getimgName());
					imageName += buf;

					//gedString image(imagePath + DIR_SEP + imageName);//Don't use this (imageName will change in SaveImage function)

					if(rle->SaveImage(imageName))
					{					
						if(ShellExecute(NULL, "open", Config::Get()->getImageEditor().c_str(), (imagePath + DIR_SEP + imageName).c_str(), NULL, SW_SHOWNORMAL) > (HINSTANCE)32)
						{
							mapEditedImages.Add(imageName, stFrameData(anim->Name(), frame, GetFileWriteTime(imageName)));
						}
						else
						{
							char buf[1024];
							sprintf(buf, "Can't open the image editor.\nPlease, verify if the file '%s' is a valid image editor.", Config::Get()->getImageEditor().c_str());
							new PanelInfo(buf, "Error", ALIGN_LEFT);

							remove((imagePath + DIR_SEP + imageName).c_str());
							return;
						}
					}
					else
					{
						char buf[1024];
						sprintf(buf, "Can't save the image to edit.\nPlease, verify if the file '%s' is not read only.", (imagePath + DIR_SEP + imageName).c_str());
						new PanelInfo(buf, "Error", ALIGN_LEFT);
						return;
					}
				}
			}			
		}
		break;

	case BT_PLAY:
		animationSample->setEditAnim(true);
		break;
#endif

	}
}

void AddActionDialog::CheckFrameChanges()
{
	MapFrameDataIterator it(mapEditedImages);
	for(it.Begin(); !it.Done(); it.Next())
	{
		long lastWriteTime = GetFileWriteTime(*it.Key());
		if(lastWriteTime)
		{
			if(it.Value()->writeTime != lastWriteTime)
			{
				//File changed
				it.Value()->bChanged = true;
				it.Value()->writeTime = lastWriteTime;

				//Update sample
				animationSample->ReplaceFrame(it.Value()->animationName, it.Value()->frame, "tmp_");
				animationSample->getSprite()->Invalidate();
			}
		}
		else
		{
			//File removed
			mapEditedImages.Remove(*it.Key());

			//Check in the next call
			break;
		}
	}	
}

void AddActionDialog::RemoveEditedFrames()
{
	if(!mapEditedImages.empty())
	{
		EditorDirectory dir(GameControl::Get()->getGamePath() + DIR_SEP + "data");
		
		MapFrameDataIterator it(mapEditedImages);
		for(it.Begin(); !it.Done(); it.Next())
		{
			remove(it.Key()->c_str());
		}

		mapEditedImages.Clear();
	}
}

bool AddActionDialog::HasEditedFrames()
{
	MapFrameDataIterator it(mapEditedImages);
	for(it.Begin(); !it.Done(); it.Next())
	{
		long lastWriteTime = GetFileWriteTime(*it.Key());
		if(lastWriteTime)
		{
			//Valid file
			if(it.Value()->bChanged)
			{
				return true;
			}
		}		
	}

	return false;
}

void AddActionDialog::ConfirmEditedFrames()
{
	if(!mapEditedImages.empty())
	{
		EditorDirectory dir(GameControl::Get()->getGamePath() + DIR_SEP + "data");
		
		MapFrameDataIterator it(mapEditedImages);
		for(it.Begin(); !it.Done(); it.Next())
		{
			long lastWriteTime = GetFileWriteTime(*it.Key());
			bool bUpdated = false;

			if(lastWriteTime)
			{
				//Valid file

				//Confirm file change
				if(it.Value()->writeTime != lastWriteTime)
				{
					//File changed
					it.Value()->bChanged = true;					
				}

				if(it.Value()->bChanged)
				{
					//Update mod file
					gedString imageName(it.Key()->substr(4));

					//Remove old mod file
					remove(imageName.c_str());

					//Rename tmp to new file
					rename(it.Key()->c_str(), imageName.c_str());

					bUpdated = true;
				}
			}

			if(!bUpdated)
			{
				remove(it.Key()->c_str());
			}
		}
	}
}


#endif //STAND_ALONE_GAME

