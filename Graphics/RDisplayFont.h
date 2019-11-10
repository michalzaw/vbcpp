#ifndef RDISPLAYFONTT_H_INCLUDED
#define RDISPLAYFONTT_H_INCLUDED


#include <fstream>

#include "../Utils/Resource.h"
#include "../Utils/Strings.h"


struct RDisplayFontSize
{
	int height;
	int** chars;
	int* charsWidth;
};


class RDisplayFont : public Resource
{
	private:
		static const int NUMBER_OF_SIZES = 5;
		static const int NUMBER_OF_CHARS = 48;
		
		int _availableSizes[NUMBER_OF_SIZES];

		RDisplayFontSize _fontSizes[NUMBER_OF_SIZES];

		void loadOneFont(std::string fileName, int index);

	public:
		RDisplayFont(std::string path);
		~RDisplayFont();

		RDisplayFontSize* getFontInSize(int sizeIndex);

};


#endif // RDISPLAYFONTT_H_INCLUDED
