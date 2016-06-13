#include "WXUtils.h"

namespace utils
{
	void WXUtils::SetFont(
		wxControl *control, 
		const int pointSize,
		const wxFontWeight weight
	)
	{
		wxFont font = control->GetFont();
		font.SetPointSize(pointSize);
		font.SetWeight(weight);
		control->SetFont(font);
	}


}
