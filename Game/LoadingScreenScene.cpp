#include "LoadingScreenScene.h"


LoadingScreenScene::LoadingScreenScene(Window* window)
	: GameScene(window)
{

}


LoadingScreenScene::~LoadingScreenScene()
{

}


CameraFPS* LoadingScreenScene::createCameraFPSGlobal()
{
	SceneObject* cameraObject = _sceneManager->addSceneObject("cameraFPSGlobal");

	CameraFPS* cameraFPS = _graphicsManager->addCameraFPS(GameConfig::getInstance().windowWidth, GameConfig::getInstance().windowHeight, degToRad(58.0f), 0.1f, 1000.0f);
	cameraObject->addComponent(cameraFPS);
	cameraFPS->setRotationSpeed(0.001f);
	cameraFPS->setMoveSpeed(5);
	cameraObject->setRotation(0, 0, 0);
	cameraObject->setPosition(0, 0.5f, -15);

	return cameraFPS;
}


void LoadingScreenScene::initialize()
{
	CameraFPS* camera = createCameraFPSGlobal();

	_graphicsManager->setCurrentCamera(camera);
	_soundManager->setActiveCamera(camera);

	// light
	SceneObject* lightSceneObject = _sceneManager->addSceneObject("light");
	Light* light = _graphicsManager->addDirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f), 0.5f, 0.5f);
	lightSceneObject->addComponent(light);
	light->setShadowMapping(GameConfig::getInstance().isShadowmappingEnable);

	// sky
	std::vector<std::string> skyboxFileNamesArray;
	skyboxFileNamesArray.push_back("Skybox/skybox_posx.hdr");
	skyboxFileNamesArray.push_back("Skybox/skybox_negx.hdr");
	skyboxFileNamesArray.push_back("Skybox/skybox_posy.hdr");
	skyboxFileNamesArray.push_back("Skybox/skybox_negy.hdr");
	skyboxFileNamesArray.push_back("Skybox/skybox_posz.hdr");
	skyboxFileNamesArray.push_back("Skybox/skybox_negz.hdr");

	RTextureCubeMap* skyboxTexture = ResourceManager::getInstance().loadTextureCubeMap(&skyboxFileNamesArray[0]);
	_graphicsManager->addGlobalEnvironmentCaptureComponent(skyboxTexture);

	//glm::vec4 backgroundColor = glm::vec4(0.161f, 0.329f, 0.494f, 1.0f);
	glm::vec4 backgroundColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	Image* background = _gui->addImage(ResourceManager::getInstance().loadOneColorTexture(backgroundColor));
	background->setScale(_window->getWidth() / background->getRealSize().x, _window->getHeight() / background->getRealSize().y);

	ProgressBar* progressBar = _gui->addProgressBar();
	progressBar->setType(ProgressBar::TYPE2);
	progressBar->setSpeed(0.5);
	progressBar->setPosition((_window->getWidth() - progressBar->getSize()) / 2.0f, _window->getHeight() / 10.0f);

	Image* logo = _gui->addImage(ResourceManager::getInstance().loadTexture("Data/logo.png"));
	logo->setPosition((_window->getWidth() - logo->getRealSize().x) / 2.0f, (_window->getHeight() - logo->getRealSize().y) / 2.0f);
}


void LoadingScreenScene::fixedStepUpdate(double deltaTime)
{

}


void LoadingScreenScene::update(double deltaTime)
{

}


void LoadingScreenScene::readInput(double deltaTime)
{

}


void LoadingScreenScene::fixedStepReadInput(float deltaTime)
{

}


void LoadingScreenScene::terminate()
{

}
