#include "RDisplayFont.h"


RDisplayFont::RDisplayFont(std::string path)
	: Resource(RT_DISPLAY_FONT, path), _availableSizes{ 5, 7, 9, 12, 16 }
{
	for (int i = 0; i < NUMBER_OF_SIZES; ++i)
	{
		_fontSizes[i].height = _availableSizes[i];
		_fontSizes[i].chars = new int* [NUMBER_OF_CHARS];
		_fontSizes[i].charsWidth = new int[NUMBER_OF_CHARS];

		for (int j = 0; j < NUMBER_OF_CHARS; ++j)
		{
			_fontSizes[i].chars[j] = new int[_fontSizes[i].height];
		}
	}

	for (int i = 0; i < NUMBER_OF_SIZES; ++i)
	{
		std::string fileName = path + "/" + toString(_availableSizes[i]) + ".csv";
		loadOneFont(fileName, i);
	}
}


RDisplayFont::~RDisplayFont()
{
	for (int i = 0; i < NUMBER_OF_SIZES; ++i)
	{
		for (int j = 0; j < NUMBER_OF_CHARS; ++j)
		{
			delete[] _fontSizes[i].chars[j];
		}

		delete[] _fontSizes[i].chars;
		delete[] _fontSizes[i].charsWidth;
	}
}


void RDisplayFont::loadOneFont(std::string fileName, int index)
{
	std::ifstream fstream(fileName);

	std::string line;
	int charIndex = 0;
	while (std::getline(fstream, line))
	{
		std::string cell;

		std::stringstream lineStream(line);

		int i = 0;
		while (std::getline(lineStream, cell, ';'))
		{
			_fontSizes[index].chars[charIndex][i] = toInt(cell);
			++i;
		}

		++charIndex;
	}

	for (int i = 0; i < NUMBER_OF_CHARS; ++i)
	{
		int maxNumberInRow = 0;
		for (int j = 0; j < _fontSizes[index].height; ++j)
		{
			if (_fontSizes[index].chars[i][j] > maxNumberInRow)
			{
				maxNumberInRow = _fontSizes[index].chars[i][j];
			}
		}

		_fontSizes[index].charsWidth[i] = floor(log2(maxNumberInRow)) + 1;

	}

	fstream.close();
}


RDisplayFontSize* RDisplayFont::getFontInSize(int sizeIndex)
{
	return &_fontSizes[sizeIndex];
}
