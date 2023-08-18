#ifndef MIRRORIMAGE_H_INCLUDED
#define MIRRORIMAGE_H_INCLUDED


#include <glm/glm.hpp>


class GUIManager;
class Image;
class Framebuffer;
class MirrorComponent;


class MirrorImage
{
	private:
		GUIManager* _gui;

		Image* _mirrorImage;
		
		MirrorComponent* _mirrorComponent;
		Framebuffer* _mirrorImageFramebuffer;

	public:
		MirrorImage(GUIManager* gui, MirrorComponent* mirrorComponent);

		void setMirrorComponent(MirrorComponent* mirrorComponent);

		Image* getImage();

		void setIsActive(bool isActive);
		bool isActive();

		void update();

};


#endif // MIRRORIMAGE_H_INCLUDED