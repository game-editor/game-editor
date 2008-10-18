#ifndef STAND_ALONE_GAME //maks

#include "progress.h"
#include "../gui/eventmanager.h"
#include "../engine/boxresource.h"
#include "../engine/box.h"

KrProgress::KrProgress (int width,
						int height,
						KrRGBA FillColor,
						KrRGBA ChangeColor,
						KrRGBA BorderColor,
						const KrScheme& scheme) : KrWidget (scheme)
{
	m_value = 0;
	m_maxValue = 100;

	m_width = width;
	m_height = height;

	m_FillCol = FillColor;
	m_ChangeCol = ChangeColor;
	m_BorderCol = BorderColor;

	if (m_FillCol != m_ChangeCol)
		m_multicolor = true;
	else
		m_multicolor = false;

	m_progressres = new KrBoxResource( "KrProgress Bar", m_width-2, m_height-2, &m_FillCol,  1, KrBoxResource::FILL );
	m_borderres   = new KrBoxResource( "KrProgress Border", m_width, m_height,	&m_BorderCol, 1, KrBoxResource::OUTLINE);

	m_progress = new KrBox(m_progressres);
	m_border   = new KrBox(m_borderres);

	m_vertical = (m_width < m_height);
}


KrProgress::KrProgress (int width,
						int height,
						const KrScheme& scheme) : KrWidget (scheme)
{
	m_value = 0;
	m_maxValue = 100;

	m_width = width;
	m_height = height;

	m_FillCol   = scheme.primary;
	m_ChangeCol = scheme.primary;
	scheme.secondary.ApplyTransform( &m_ChangeCol );
	m_BorderCol = scheme.CalcBrightLine();

	if (m_FillCol != m_ChangeCol)
		m_multicolor = true;
	else
		m_multicolor = false;

	m_progressres = new KrBoxResource( "KrProgress Bar", m_width-2, m_height-2, &m_FillCol,  1, KrBoxResource::FILL );
	m_borderres   = new KrBoxResource( "KrProgress Border", m_width, m_height,	&m_BorderCol, 1, KrBoxResource::OUTLINE);

	m_progress = new KrBox(m_progressres);
	m_border   = new KrBox(m_borderres);

	m_vertical = (m_width < m_height);
}


KrProgress::~KrProgress ()
{
	delete m_progressres;
	delete m_borderres;
}


void KrProgress::SetBarColors (KrRGBA FillColor, KrRGBA ChangeColor, KrRGBA BorderColor)
{
	if (m_progress) {
		if (Engine()) {
			Engine()->Tree()->DeleteNode(m_progress);
			Engine()->Tree()->DeleteNode(m_border);
		} else {
			delete m_progress;
			delete m_border;
		}
	}

	delete m_progressres;
	delete m_borderres;

	m_FillCol = FillColor;
	m_ChangeCol = ChangeColor;
	m_BorderCol = BorderColor;

	if (m_FillCol != m_ChangeCol)
		m_multicolor = true;
	else
		m_multicolor = false;

	m_progressres = new KrBoxResource( "KrProgress Bar", m_width-2, m_height-2, &m_FillCol,  1, KrBoxResource::FILL );
	m_borderres   = new KrBoxResource( "KrProgress Border", m_width, m_height,	&m_BorderCol, 1, KrBoxResource::OUTLINE);

	m_progress = new KrBox(m_progressres);
	m_border   = new KrBox(m_borderres);

	if (m_progress && Engine()) {
		Engine()->Tree()->AddNode (this, m_border);
		Engine()->Tree()->AddNode (this, m_progress);
		Update();
	}
}


void KrProgress::AddedtoTree()
{
	KrWidget::AddedtoTree();

	if (m_progress && m_border) {

		Engine()->Tree()->AddNode (this, m_border);
		Engine()->Tree()->AddNode (this, m_progress);
		Update();

	}

	KrEventManager::Instance()->AddListener (this);
}


void KrProgress::SetMaxValue (int value)
{
	if (value < 0)
		value = 0;

	m_maxValue = value;

	if (m_value > m_maxValue)
		m_value = m_maxValue;

	Update();
}


void KrProgress::SetValue (int value)
{
	if (value < 0)
		value = 0;

	m_value = value;

	if (m_value > m_maxValue)
		m_value = m_maxValue;

	Update();
}


void KrProgress::Update()
{
	if (!m_progress || !Engine())
		return;

	if (!m_value) {
		m_progress->SetVisible(false);
		return;
	}

	int yPos;
	double ScaleY, ScaleX;

	if (m_vertical) {

		ScaleX = 1.0;
		ScaleY = (double)m_value/m_maxValue;
		yPos = (int)((m_height-2) * (1-ScaleY)) + 1;

	} else {

		ScaleX = (double)m_value/m_maxValue;
		ScaleY = 1.0;
		yPos = 1;

	}

	if (m_multicolor) {

		float Red1   = m_FillCol.Redf();   float Red2   = m_ChangeCol.Redf();
		float Green1 = m_FillCol.Greenf(); float Green2 = m_ChangeCol.Greenf();
		float Blue1  = m_FillCol.Bluef();  float Blue2  = m_ChangeCol.Bluef();

		float percentage = (float)m_value/(float)m_maxValue;

		float Red, Green, Blue;

		if (Red1 < Red2)
			Red = (Red2-Red1)*percentage + Red1;
		else
			Red = (Red1-Red2)*percentage + Red2;

		if (Green1 < Green2)
			Green = (Green2-Green1)*percentage + Green1;
		else
			Green = (Green1-Green2)*percentage + Green2;

		if (Blue1 < Blue2)
			Blue = (Blue2-Blue1)*percentage + Blue1;
		else
			Blue = (Blue1-Blue2)*percentage + Blue2;

		KrRGBA temp;
		temp.Set (Red*255, Green*255, Blue*255);

		if (m_progress) {
			if (Engine()) {
				Engine()->Tree()->DeleteNode(m_progress);
			} else {
				delete m_progress;
			}
		}

		delete m_progressres;

		m_progressres = new KrBoxResource( "KrProgress Bar", m_width-2, m_height-2, &temp,  1, KrBoxResource::FILL );
		m_progress = new KrBox(m_progressres);

		if (m_progress && Engine()) {
			Engine()->Tree()->AddNode (this, m_progress);
		}
	}

	m_progress->SetPos (1, yPos);
	m_progress->SetScale (GlFixed(ScaleX), GlFixed(ScaleY));
	m_progress->SetVisible (true);
}


#endif //#ifndef STAND_ALONE_GAME //maks
