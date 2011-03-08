#ifndef LEFTWRAPPEDLISTITEM_H
#define LEFTWRAPPEDLISTITEM_H

#include <string>

#include <CEGUI.h>

// Based on implementation available on CEGUI's forums:
// http://www.cegui.org.uk/phpBB2/viewtopic.php?p=13333
class LeftWrappedListItem : public CEGUI::ListboxTextItem
{
private:
	CEGUI::FormattedRenderedString * d_formattedRenderedString;

public:
	//! constructor
	LeftWrappedListItem (const std::string& text)
		: CEGUI::ListboxTextItem(text.c_str())
	{
		d_formattedRenderedString = new CEGUI::RenderedStringWordWrapper
			<CEGUI::LeftAlignedRenderedString>(d_renderedString);
		setTextParsingEnabled(true);
	}

	//! destructor
	~LeftWrappedListItem ()
	{
		delete d_formattedRenderedString;
	}

	/*************************************************************************
	Required implementations of pure virtuals from the base class.
	*************************************************************************/
	CEGUI::Sizef getPixelSize(void) const
	{
		using namespace CEGUI;

		if (!d_renderedStringValid)
			parseTextString();

		CEGUI::Sizef parentsi = getOwnerWindow()->getInnerRectClipper().getSize();
		parentsi.d_width -= 30; // TODO - change constant by the real value of the scrollbar
      
		d_formattedRenderedString->format(parentsi);
		return CEGUI::Sizef(parentsi.d_width, d_formattedRenderedString->getVerticalExtent());
	}


	void draw(CEGUI::GeometryBuffer& buffer, const CEGUI::Rectf& targetRect, float alpha, const CEGUI::Rectf* clipper) const
	{
		using namespace CEGUI;

		if (!d_renderedStringValid)
			parseTextString();

		d_formattedRenderedString->format(targetRect.getSize());

		const ColourRect final_colours(getModulateAlphaColourRect(ColourRect(0xFFFFFFFF), alpha));

		d_formattedRenderedString->draw(buffer, targetRect.getPosition(), &final_colours, clipper);
	}
};


#endif
