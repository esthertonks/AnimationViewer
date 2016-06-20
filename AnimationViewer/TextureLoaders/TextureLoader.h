#pragma once

#include <string>
#include "boost\algorithm\string.hpp"
#include "../Utils/StringToEnumConvertor.h"

class TextureLoader
{
public:

	TextureLoader(){};
	~TextureLoader(){};

	void Load(
		const std::string &texturePath
	);

private:
	void LoadWithSOIL(
		const std::string &texturePath
	);

	void LoadWithLibTiff(
		const std::string &texturePath
	);

};