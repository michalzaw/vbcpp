#ifndef IMGUIUTILS_H_INCLUDED
#define IMGUIUTILS_H_INCLUDED


#include <string>
#include <vector>


namespace ImGuiUtils
{
	template<typename TYPE>
	void convertVectorToComboData(const std::vector<TYPE>& items, const TYPE& selectedItemValue, std::string& outItemsString, int& outSelectedItemIndex, bool addEmptyElement = true)
	{
		outSelectedItemIndex = 0;

		if (addEmptyElement)
		{
			outItemsString += " ";
			outItemsString += '\0';

			outSelectedItemIndex = -1;
		}

		for (int i = 0; i < items.size(); ++i)
		{
			outItemsString += Strings::toString(items[i]) + '\0';

			if (items[i] == selectedItemValue)
			{
				outSelectedItemIndex = i;
			}
		}

		if (addEmptyElement)
		{
			outSelectedItemIndex += 1;
		}
	}


	template<typename TYPE>
	void convertVectorToComboData(const TYPE* items, unsigned int itemsCount, const TYPE& selectedItemValue, std::string& outItemsString, int& outSelectedItemIndex, bool addEmptyElement = true)
	{
		if (addEmptyElement)
		{
			outItemsString += " ";
			outItemsString += '\0';
		}

		for (int i = 0; i < itemsCount; ++i)
		{
			outItemsString += Strings::toString(items[i]) + '\0';

			if (items[i] == selectedItemValue)
			{
				outSelectedItemIndex = i;
			}
		}

		if (addEmptyElement)
		{
			outSelectedItemIndex += 1;
		}
	}
}


#endif // IMGUIUTILS_H_INCLUDED
