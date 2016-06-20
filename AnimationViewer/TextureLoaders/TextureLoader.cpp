#include "TextureLoader.h"
#include "SOIL.h"
#include "tiffio.h"
#include "../Render/RenderFwdDecl.h"
#include <wx/log.h>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

enum SupportedTextureTypes
{
	eTIF,
	eTGA,
	ePNG,
	eJPG,
	eBMP,
	eDDS,
	eNotValid
};

StringToEnumConvertor<SupportedTextureTypes>::StringToEnumConvertor()
{
	enumToString["tiff"] = eTIF;
	enumToString["tif"] = eTIF;
	enumToString["tga"] = eTGA;
	enumToString["png"] = ePNG;
	enumToString["jpg"] = eJPG;
	enumToString["jpeg"] = eJPG;
	enumToString["bmp"] = eBMP;
	enumToString["dds"] = eDDS;
}

void TextureLoader::Load(
	const std::string &texturePath
)
{
	if (boost::filesystem::exists(texturePath))
	{
		std::string fileExtension = boost::filesystem::extension(texturePath);
		fileExtension.erase(std::remove(fileExtension.begin(), fileExtension.end(), '.'), fileExtension.end());

		StringToEnumConvertor<SupportedTextureTypes> stringToEnumConvertor;
		SupportedTextureTypes textureType = stringToEnumConvertor.GetEnumValue(fileExtension);
		switch (textureType)
		{
		case eTIF:
			LoadWithLibTiff(texturePath);
			break;
		case eTGA:
		case eJPG:
		case eBMP:
		case ePNG:
		case eDDS:
			LoadWithSOIL(texturePath);
			break;
		case eNotValid:
			wxLogDebug("Trying to load invalid texture type: %s, with path %s, texture load failed. ", fileExtension, texturePath);
			break;
		default:
			wxLogDebug("Trying to load unsupported texture type: %s, with path %s, texture load failed. ", fileExtension, texturePath);
		}
	}
}

void TextureLoader::LoadWithSOIL(
	const std::string &texturePath
	)
{
	int width;
	int height;
	int channels;
	unsigned char* texture = SOIL_load_image(texturePath.c_str(), &width, &height, &channels, SOIL_LOAD_RGBA);
	if (texture)
	{
		glActiveTexture(GL_TEXTURE0);
		GLuint textureId;
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture);

		SOIL_free_image_data(texture);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
	else
	{
		wxLogDebug("Failed to load texture, image may not display correctly. Error is %s", SOIL_last_result());
	}
}

void TextureLoader::LoadWithLibTiff(
	const std::string &texturePath
)
{
	TIFFRGBAImage image;
	char errorMessage[1024];
	size_t numPixels;
	int imageWidth, imageHeight;

	TIFF *tif = TIFFOpen(texturePath.c_str(), "r");
	if (tif) 
	{
		if (TIFFRGBAImageBegin(&image, tif, 0, errorMessage))
		{
			numPixels = image.width * image.height;
			uint32 *raster = (uint32 *)_TIFFmalloc(numPixels * sizeof(uint32));

			if (TIFFRGBAImageGet(&image, raster, image.width, image.height) == 0) 
			{
				TIFFError(texturePath.c_str(), errorMessage);
				wxLogDebug("Failed to read texture image from file %s", errorMessage);
				return;
			}

			TIFFRGBAImageEnd(&image);

			imageWidth = image.width;
			imageHeight = image.height;

			glActiveTexture(GL_TEXTURE0);
			GLuint textureId;
			glGenTextures(1, &textureId);
			glBindTexture(GL_TEXTURE_2D, textureId);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, raster);

			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}
		else 
		{
			TIFFError(texturePath.c_str(), errorMessage);
			return;
		}
	}
	else
	{
		wxLogDebug("Failed to read texture %s", texturePath.c_str());
	}
}