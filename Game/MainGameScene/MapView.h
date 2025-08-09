#ifndef MAPVIEW_H_INCLUDED
#define MAPVIEW_H_INCLUDED


#include <vector>

#include <glm/glm.hpp>


class GUIManager;
class Image;
class Framebuffer;
class CameraStatic;
class SceneManager;
class Bus;


enum MapViewMode
{
	MVM_WORLD_MAP,
	MVM_NAVIGATION,
	MVM_DISABLE,

	MVM_COUNT
};


class MapView
{
	private:
		GUIManager* _gui;

		std::vector<Image*> _images;

		SceneManager* _sceneManager;

		std::vector<Framebuffer*> _framebuffers;

		std::vector<CameraStatic*> _cameras;

		MapViewMode _mode;

		bool _showPavements;

		CameraStatic* createCameraForWorldMapMode();
		CameraStatic* createCameraFormNavigationMode();

	public:
		MapView(GUIManager* gui, SceneManager* sceneManager);

		void setMode(MapViewMode mode);
		MapViewMode getMode();

		inline void setShowPavements(bool showPavements) { _showPavements = showPavements; }
		inline bool getShowPavements() { return _showPavements; }

		void init(SceneManager* sceneManager);

		void update(Bus* bus);

};


#endif // MAPVIEW_H_INCLUDED
