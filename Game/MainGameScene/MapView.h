#ifndef MAPVIEW_H_INCLUDED
#define MAPVIEW_H_INCLUDED


#include <glm/glm.hpp>


class GUIManager;
class Image;
class Framebuffer;
class CameraStatic;
class SceneManager;
class Bus;


class MapView
{
	private:
		GUIManager* _gui;

		Image* _image;

		SceneManager* _sceneManager;

		Framebuffer* _framebuffer;

		CameraStatic* _camera;

	public:
		MapView(GUIManager* gui, SceneManager* sceneManager);

		void init(SceneManager* sceneManager);

		void update(Bus* bus);

};


#endif // MAPVIEW_H_INCLUDED
