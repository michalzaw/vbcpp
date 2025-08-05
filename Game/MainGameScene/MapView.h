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

		bool _showPavements;

	public:
		MapView(GUIManager* gui, SceneManager* sceneManager);

		inline void setShowPavements(bool showPavements) { _showPavements = showPavements; }
		inline bool getShowPavements() { return _showPavements; }

		void init(SceneManager* sceneManager);

		void update(Bus* bus);

};


#endif // MAPVIEW_H_INCLUDED
