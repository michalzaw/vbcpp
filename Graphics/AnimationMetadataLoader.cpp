#include "AnimationMetadataLoader.h"

#include "../Utils/tinyxml2.h"
using namespace tinyxml2;


const std::string animationMetadataFilenamePostfix = ".xml";
const char* XML_ANIMATION_METADATA_ROOT = "AnimationMetadata";


std::string AnimationMetadataLoader::createAnimationMetadataFileName(const std::string& animationFileName)
{
	std::string xmlFileName = animationFileName;
	xmlFileName.erase(xmlFileName.size() - 4, 4);
	xmlFileName += animationMetadataFilenamePostfix;

	return xmlFileName;
}


void AnimationMetadataLoader::loadAnimationMetadata(const std::string& fileName, RAnimation* animation)
{
	XMLDocument doc;
	doc.LoadFile(fileName.c_str());

	XMLElement* root = doc.FirstChildElement(XML_ANIMATION_METADATA_ROOT);

	int animationTicksPerSecond = root->IntAttribute("animationTicksPerSecond");
	int startFrame = root->IntAttribute("startFrame");
	int endFrame = root->IntAttribute("endFrame");

	animation->_ticksPerSecond = animationTicksPerSecond != 0 ? animationTicksPerSecond : animation->_ticksPerSecond;
	animation->_startFrame = startFrame;
	animation->_endFrame = endFrame != 0 ? endFrame : animation->_originalDuration;
}
