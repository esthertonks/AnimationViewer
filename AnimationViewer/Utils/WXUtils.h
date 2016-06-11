#pragma once

#include "wx/wx.h"

namespace utils
{
class WXUtils
{
public:

	static void SetFont(
		wxControl *control,
		const int pointSize,
		const wxFontWeight weight
	);
};

}

