#include "AnimationMetadataSaver.h"

#include "../Utils/tinyxml2.h"
using namespace tinyxml2;


namespace AnimationMetadataSaver
{
	const char* XML_ANIMATION_METADATA_ROOT = "AnimationMetadata";

	void saveDefaultAnimationMetadata(const std::string& fileName)
	{
		XMLDocument doc;

		XMLDeclaration* dec = doc.NewDeclaration();
		doc.InsertFirstChild(dec);

		XMLElement* root = doc.NewElement(XML_ANIMATION_METADATA_ROOT);

		root->SetAttribute("animationTicksPerSecond", 0);
		root->SetAttribute("startFrame", 0);
		root->SetAttribute("endFrame", 0);

		doc.InsertEndChild(root);

		doc.SaveFile(fileName.c_str());
	}

}
