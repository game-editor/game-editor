#ifndef STAND_ALONE_GAME //maks

#ifndef KYRA_PROGRESSWIDGET_INCLUDED
#define KYRA_PROGRESSWIDGET_INCLUDED

#include "../gui/widget.h"


/**	A progress bar widget.

	** Note: This is an externally submitted widget. It has a different
	   author and does not necessarily share the same license as
	   main Kyra code.
	** Also note: Send in your widgets! We'de like them to have a home
	   here where everyone can use them.

	Author:			SkyFlash
	Contact info:	--
	Contributing:	Lee Thomason:	KrScheme compatible constructor.
	Questions about this widget?	Kyra mailing list

*/
class KrProgress : public KrWidget
{
protected:
  
	unsigned int m_maxValue;
	unsigned int m_value;
	KrBox* m_progress;
	KrBox* m_border;
	KrBoxResource *m_progressres;
	KrBoxResource *m_borderres;

	bool m_vertical;
	int m_width, m_height;
	KrRGBA m_FillCol;
	KrRGBA m_ChangeCol;
	KrRGBA m_BorderCol;
	bool m_multicolor;

	// Updates the size of the inner box
	void Update();

public:
	/** if width of the resource is greater then its height then makes
		a horizontal, else makes a vertical progress bar
	*/
	KrProgress::KrProgress (int width,
							int height,
							KrRGBA FillColor,
							KrRGBA ChangeColor,
							KrRGBA BorderColor,
							const KrScheme& scheme);

	/** if width of the resource is greater then its height then makes
		a horizontal, else makes a vertical progress bar
	*/
	KrProgress::KrProgress (int width,
							int height,
							const KrScheme& scheme);

	~KrProgress ();

	// Need to insert the progress box at the proper time
	virtual void AddedtoTree();

	//! Set the bar colors
	void SetBarColors (KrRGBA FillColor, KrRGBA ChangeColor, KrRGBA BorderColor);

	//! Sets the highest value of the progress bar (maxValue)
	void SetMaxValue(int value);

	//! Sets the current progress between 0 and maxValue
	void SetValue(int value);

	//! Retrieve the current maxValue
	unsigned int GetMaxValue() { return m_maxValue; }

	//! Retrieve the current progress between 0 and maxValue
	unsigned int GetValue() { return m_value; }
};

#endif


#endif //#ifndef STAND_ALONE_GAME //maks