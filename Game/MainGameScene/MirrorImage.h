#ifndef MIRRORIMAGE_H_INCLUDED
#define MIRRORIMAGE_H_INCLUDED


#include <glm/glm.hpp>


class GUIManager;
class Image;
class Framebuffer;
class RTexture;


class MirrorImage
{
	private:
		GUIManager* _gui;

		Image* _mirrorImage;
		
		RTexture* _mirrorOriginalTexture;
		Framebuffer* _mirrorImageFramebuffer;

	public:
		MirrorImage(GUIManager* gui, RTexture* texture);

		void setMirrorOriginalTexture(RTexture* texture);

		Image* getImage();

		void setIsActive(bool isActive);
		bool isActive();

		void update();

};


#endif // MIRRORIMAGE_H_INCLUDED