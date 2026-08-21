#ifndef DISPLAYTEXT_H_INCLUDED
#define DISPLAYTEXT_H_INCLUDED


#include <string>

#include "../Scripting/Utils/LuaMacros.h"


enum DisplayTextType
{
	ONLY_HEAD,
	ONE_LINE,
	TWO_LINE,
	TWO_LINE_FIRST_BIG,
	TWO_LINE_SECOND_BIG
};


struct DisplayText
{
	LUAV std::string head;
	LUAV std::string line1;
	LUAV std::string line2;

	LUAV DisplayTextType type;

};


#endif // DISPLAYTEXT_H_INCLUDED
