#ifndef ANIMATIONMETADATALOADER_H_INCLUDED
#define ANIMATIONMETADATALOADER_H_INCLUDED


#include <string>

#include "RAnimation.h"


class AnimationMetadataLoader
{
	public:
		static std::string createAnimationMetadataFileName(const std::string& animationFileName);

		static void loadAnimationMetadata(const std::string& fileName, RAnimation* animation);

};


#endif // ANIMATIONMETADATALOADER_H_INCLUDED
