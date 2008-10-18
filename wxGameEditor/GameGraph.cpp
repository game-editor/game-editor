// GameGraph.cpp: implementation of the GameGraph class.
//
//////////////////////////////////////////////////////////////////////

#include "wxgameeditor.h"
#include "GameGraph.h"
#include "wx/dcbuffer.h"


#include "config.h"
#include "gvc.h"
#include "globals.h"
#include "render.h"
#include "agxbuf.h"
//#include "wx/ogl/ogl.h"

//Defined in arrows.c
extern "C" point ptArrow[16];
extern "C" int nPtArrow;


/*int AddInGameGraphControl::n = 0;
Actor *AddInGameGraphControl::to = NULL;
int AddInGameGraphControl::interation = 0;*/

void AddToGameGraph(Actor *to, int interation, Actor *from)
{
	if(!GameControl::Get()->getGameMode()) return;

	//if(AddInGameGraphControl::CanAdd())
	{
		if(!from) from = Action::getActualEventActor();
		if(!from) return;
		
		GameGraph::Get()->AddActorInteration(from->getActorName(), to->getActorName(), interation);
	}
	/*else
	{
		//Add later
		AddInGameGraphControl::Add(to, interation);
	}*/
}

void GenerateGameGraph()
{
	GameGraph::Get()->GenerateGraph();
}

void ClearGameGraph()
{
	GameGraph::Get()->Clear();	
}

/* setYInvert:
 * Set parameters used to flip coordinate system (y=0 at top).
 * Values do not need to be unset, since if Y_invert is set, it's
 * set for * all graphs during current run, so each will 
 * reinitialize the values for its bbox.
 */
static void setYInvert(graph_t * g)
{
	Y_invert = TRUE;
    
	Y_off = GD_bb(g).UR.y + GD_bb(g).LL.y;
	YF_off = PS2INCH(Y_off);    
}


IMPLEMENT_CLASS(GameGraph, wxPanel)

BEGIN_EVENT_TABLE(GameGraph, wxPanel)

    EVT_PAINT(GameGraph::OnPaint)
    EVT_ERASE_BACKGROUND(GameGraph::OnEraseBackground)
	
END_EVENT_TABLE()

GameGraph *GameGraph::gameGraph = NULL;
static GVC_t *gvc;
static graph_t *g;

GameGraph::GameGraph(wxWindow *parent) 
: wxScrolledWindow(parent, ID_GAME_GRAPH_PANEL)
{
	gameGraph = this;

	nSequence = 0;
	oldMaxX = oldMaxY = -1;
	bShowSelfRelationship = false;
	bShowEdgeLabels = true;
	bShowSequenceNumber = true;

	gvc = NULL;
	g = NULL;
}


GameGraph::~GameGraph()
{
	gameGraph = NULL;
}

GameGraph *GameGraph::Get()
{
	return gameGraph;
}

void GameGraph::Clear()
{
	nSequence = 0;
	oldMaxX = oldMaxY = -1;
	mapActorInteration.Clear();
	//AddInGameGraphControl::Reset();

	CloseEngine();
	SetScrollbars(1, 1, 100, 100);	
}

char *Info[] = {
    "dot",			/* Program */
    ".",			/* Version */
    "."			/* Build Date */
};

void GameGraph::OpenEngine(char *fileName)
{
	CloseEngine();	

	FILE *fp = fopen(fileName, "r");
	if(fp)
	{
		gvc = gvContext();
		g = agread(fp);
		gvLayout(gvc, g, "dot");
		setYInvert(g);

		fclose(fp);

		////////////////////////////////
		//Generate xdot test file
		/*char *argv[] = {"oi", "-Txdot", "ged_graph.txt"};
		char *out = "graph.txt";
		gvc = gvNEWcontext(Info, gvUsername());
		gvParseArgs(gvc, 3, argv);
		
		
		g = next_input_graph();
		gvLayoutJobs(gvc, g); 
		GVJ_t *job = gvrender_first_job(gvc);
		job->output_filename = "graph.txt";

		gvRenderJobs(gvc, g);
		fclose(job->output_file);
		CloseEngine();*/
		////////////////////////////////
		
		if(!agfstnode(g))
		{
			//Graph is empty
			CloseEngine();
		}
	}
}

void GameGraph::CloseEngine()
{
	if(gvc && g)
	{
		gvFreeLayout(gvc, g);		
		agclose(g);		
		gvFreeContext(gvc);
	}

	gvc = NULL;
	g = NULL;
}

void GameGraph::AddActorInteration(char *actorName1, char *actorName2, int interation)
{
	stActorsName actorsInInteration(actorName1, actorName2, interation);

	if(mapActorInteration[actorsInInteration] == NULL)
	{
		mapActorInteration.Add(actorsInInteration, ++nSequence);
	}	
}

void GameGraph::GenerateGraph()
{
	GenerateGraphvizFile();
	//GenerateVCGFile();
}



void GameGraph::GenerateGraphvizFile()
{
	//Graphviz file
	//graphviz.org

	EditorDirectory dir;
	char *fileName = "ged_graph.txt";

	//Get actors
	MapString mapActorNames;

	MapActorsNameIterator itActors(mapActorInteration);	
	for(itActors.Begin(); !itActors.Done(); itActors.Next())
	{
		stActorsName *p = itActors.Key();

		if((bShowSelfRelationship || p->actorName1 != p->actorName2) && 
			(onlyRelationsWithActor.empty() || p->actorName1 == onlyRelationsWithActor || p->actorName1 == onlyRelationsWithActor))
		{			
			mapActorNames.Add(p->actorName1, 1);
			mapActorNames.Add(p->actorName2, 1);
		}
	}

	FILE *arq = fopen(fileName, "w");
	if(!arq) return;
	
	//Header
	fprintf(arq, "digraph game {\n");

	fprintf(arq, "rankdir=LR;\n");
	//fprintf(arq, "size=\"1,1\";\n");
	//fprintf(arq, "ratio=fill;\n");

	//Colors
	wxColour	fillColor(*wxLIGHT_GREY),
				nodeColor(*wxBLACK),
				edgeColor(*wxBLACK);

	//Nodes
	MapStringIterator itNodes(mapActorNames);	
	for(itNodes.Begin(); !itNodes.Done(); itNodes.Next())
	{
		fprintf(arq, "%s [color = \"#%02X%02X%02X\", fillcolor = \"#%02X%02X%02X\", shape = record, style = filled];\n", 
			(*itNodes.Key()).c_str(),

			nodeColor.Red(),
			nodeColor.Green(),
			nodeColor.Blue(),

			fillColor.Red(),
			fillColor.Green(),
			fillColor.Blue()
			);
	}

	//Edges
	for(itActors.Begin(); !itActors.Done(); itActors.Next())
	{
		stActorsName *p = itActors.Key();
		gedString sInteration;

		stAction tmp;
		tmp.actionType = p->interation;

		sInteration = Action::GetActionName(&tmp);

		if(bShowSequenceNumber)
		{
			char s[64];

			sprintf(s, "%ld: ", *itActors.Value());

			sInteration = gedString(s) + sInteration;
		}
		
		if((bShowSelfRelationship || p->actorName1 != p->actorName2) && 
			(onlyRelationsWithActor.empty() || p->actorName1 == onlyRelationsWithActor || p->actorName1 == onlyRelationsWithActor))
		{
			fprintf(arq, "%s -> %s [color = \"#%02X%02X%02X\", label=\"%s\"];\n", 
				p->actorName1.c_str(), 
				p->actorName2.c_str(), 

				edgeColor.Red(),
				edgeColor.Green(),
				edgeColor.Blue(),

				sInteration.c_str());
		}
	}

	//Footer
	fprintf(arq, "}");

	fclose(arq);

	OpenEngine(fileName);
	remove(fileName); 
}

void GameGraph::GenerateVCGFile()
{
	//Visualization of Compiler Graphs  file
	//rw4.cs.uni-sb.de/~sander/html/gsvcg1.html

	EditorDirectory dir;

	//Get actors
	MapString mapActorNames;

	MapActorsNameIterator itActors(mapActorInteration);	
	for(itActors.Begin(); !itActors.Done(); itActors.Next())
	{
		stActorsName *p = itActors.Key();

		if((bShowSelfRelationship || p->actorName1 != p->actorName2) && 
			(onlyRelationsWithActor.empty() || p->actorName1 == onlyRelationsWithActor || p->actorName1 == onlyRelationsWithActor))
		{			
			mapActorNames.Add(p->actorName1, 1);
			mapActorNames.Add(p->actorName2, 1);
		}
	}

	FILE *arq = fopen("graph_vcg.txt", "w");
	if(!arq) return;
	
	//Header
	fprintf(arq, "graph: { title: \"game\"\
	 height: 700\
	 width: 2048\
	 x: 400\
	 y: 30\
	 color: lightcyan\
	 layout_downfactor: 10\
	 layout_upfactor:   1\
	 layout_nearfactor: 5\
     layoutalgorithm: dfs\
	 display_edge_labels: yes\n");

	//Nodes
	MapStringIterator itNodes(mapActorNames);	
	for(itNodes.Begin(); !itNodes.Done(); itNodes.Next())
	{
		fprintf(arq, "node: { title:\"%s\" label:\"%s\"  color: lightgreen textcolor: blue}\n", (*itNodes.Key()).c_str(), (*itNodes.Key()).c_str());
	}

	//Edges
	for(itActors.Begin(); !itActors.Done(); itActors.Next())
	{
		stActorsName *p = itActors.Key();
		gedString sInteration;

		stAction tmp;
		tmp.actionType = p->interation;

		sInteration = Action::GetActionName(&tmp);

		if(bShowSequenceNumber)
		{
			char s[64];

			sprintf(s, "%ld: ", *itActors.Value());

			sInteration = gedString(s) + sInteration;
		}
		
		if((bShowSelfRelationship || p->actorName1 != p->actorName2) && 
			(onlyRelationsWithActor.empty() || p->actorName1 == onlyRelationsWithActor || p->actorName1 == onlyRelationsWithActor))
		{
			fprintf(arq, "edge: { sourcename:\"%s\" targetname:\"%s\" color: red thickness: 2 priority: 8 label: \"%s\"}\n", p->actorName1.c_str(), p->actorName2.c_str(), sInteration.c_str());
		}
	}

	//Footer
	fprintf(arq, "}");

	fclose(arq);
}



void GameGraph::Draw(wxBufferedPaintDC &dc)
{
	//Font (in user configuration don't allow < 7)
	wxFont font(9, wxFONTFAMILY_SWISS, wxNORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Arial"));
	dc.SetFont(font);

	if(!g)
	{
		wxString s(wxT("No data available. Please, run the game first."));
		int tWidth, tHeight;
		wxRect rect = GetClientRect();

		dc.SetPen(*wxBLACK);
		dc.GetTextExtent(s, &tWidth, &tHeight);
		dc.DrawText(s, (rect.GetWidth() - tWidth)/2, (rect.GetHeight() - tHeight)/2);

		return;
	}

    int i, j;
    node_t *n;
    edge_t *e;
    bezier bz;
    char *lbl;
	point pt;
	int maxX = 0, maxY = 0, minX = 10000, minY = 10000;
	gvcolor_t color;

	int margin = 30;


		
	//Draw edges
    for (n = agfstnode(g); n; n = agnxtnode(g, n)) 
	{
		for (e = agfstout(g, n); e; e = agnxtout(g, e)) 
		{
			if (ED_edge_type(e) == IGNORED)
				continue;

			if (ED_spl(e) == NULL)
				continue;	/* reported in postproc */

			colorxlate(late_nnstring(e, E_color, DEFAULT_COLOR), &color, RGBA_BYTE);

			wxColour edgeColor(color.u.rgba[0], color.u.rgba[1], color.u.rgba[2]);
			dc.SetPen(edgeColor);
			dc.SetBrush(edgeColor);

			for (i = 0; i < ED_spl(e)->size; i++) 
			{
				bz = ED_spl(e)->list[i];

				if (bz.eflag) 
				{
					nPtArrow = 0;
					arrow_gen(NULL, EMIT_HDRAW, bz.ep, bz.list[bz.size - 1], 1.0, bz.eflag);
										
					if(nPtArrow)
					{
						wxPoint *pList = new wxPoint[nPtArrow];
						if(pList)
						{
							for (j = 0; j < nPtArrow; j++) 
							{							
								pList[j].x = ptArrow[j].x + margin;
								pList[j].y = YDIR(ptArrow[j].y) + margin;								
							}
							
							//Change brush to fill
							dc.DrawPolygon(nPtArrow, pList);
							delete pList;
						}
					}
				}

				wxPoint *pList = new wxPoint[ED_spl(e)->list[i].size];
				if(pList)
				{
					for (j = 0; j < ED_spl(e)->list[i].size; j++) 
					{
						pt = ED_spl(e)->list[i].list[j];

						pList[j].x = pt.x + margin;
						pList[j].y = YDIR(pt.y) + margin;
					}

					dc.DrawSpline(ED_spl(e)->list[i].size, pList);
					delete pList;
				}
			}

			if(bShowEdgeLabels)
			{
				dc.SetPen(*wxBLACK);

				if (ED_label(e)) 
				{
					lbl = ED_label(e)->text;

					//Get text middle coordinate
					pt = ED_label(e)->p;
					pt.y = YDIR(pt.y);

					pt.x += margin;
					pt.y += margin;

					//Get real text dimensions (ED_label(e)->dimen is the calculated dimension)
					int tWidth = 0, tHeight = 0;
					dc.GetTextExtent(lbl, &tWidth, &tHeight);

					//Draw
					dc.DrawText(lbl, pt.x - tWidth/2, pt.y - tHeight/2);				
				}
			}
		}
    }

	//Draw nodes
    for (n = agfstnode(g); n; n = agnxtnode(g, n)) 
	{
		if(IS_CLUST_NODE(n))
			continue;		
		

		colorxlate(late_nnstring(n, N_color, DEFAULT_COLOR), &color, RGBA_BYTE);
		wxColour nodeColor(color.u.rgba[0], color.u.rgba[1], color.u.rgba[2]);

		colorxlate(late_nnstring(n, N_fillcolor, DEFAULT_FILL), &color, RGBA_BYTE);
		wxColour fillColor(color.u.rgba[0], color.u.rgba[1], color.u.rgba[2]);

		dc.SetPen(nodeColor);
		dc.SetBrush(fillColor);

		field_t *f = (field_t *)ND_shape_info(n);
		
		int x = f->b.LL.x + ND_coord_i(n).x + margin,
			y = YDIR(ND_coord_i(n).y) + f->b.LL.y + margin,
			width = ND_lw_i(n) + ND_rw_i(n),
			height = ND_ht_i(n),
			tWidth = 0, 
			tHeight = 0;

		lbl = ND_label(n)->text;
		
		
		dc.DrawRoundedRectangle(x, y, width, height, 4);
		
		maxX = max(maxX, x + width);
		maxY = max(maxY, y + height);
		minX = min(minX, x);
		minY = min(minY, y);
		
		
		dc.SetPen(*wxBLACK);
		dc.GetTextExtent(lbl, &tWidth, &tHeight);
		dc.DrawText(lbl, x + (width - tWidth)/2, y + (height - tHeight)/2);
    }


	//Set scroll sizes
	if(maxX != oldMaxX || maxY != oldMaxY)
	{
		//SetVirtualSize(maxX, maxY);
		SetScrollbars(1, 1, maxX + minX, maxY + minY);	

		oldMaxX = maxX;
		oldMaxY = maxY;
	}
}

void GameGraph::DoPrepareDC(wxDC& dc)
{
    int ppuX, ppuY, startX, startY;

    GetScrollPixelsPerUnit(& ppuX, & ppuY);
    GetViewStart(& startX, & startY);

    dc.SetDeviceOrigin( - startX * ppuX, - startY * ppuY );
}


void GameGraph::OnPaint(wxPaintEvent &event) 
{
	wxBufferedPaintDC dc(this);

	dc.Clear();
     
    // Paint the background
    wxRect rect = GetClientRect();
	rect.Inflate(10, 10);

	dc.SetPen(wxBorder_Pen);
    dc.SetBrush(colorBgContent);
    dc.DrawRectangle(rect);


	// Shifts the device origin so we don't have to worry
    // about the current scroll position ourselves
    DoPrepareDC(dc);

    // Paint the graphic
    Draw(dc);
}


void GameGraph::OnEraseBackground(wxEraseEvent &event) 
{

}