#include "TestScene.h"

#include "CameraControlComponent.h"
#include "Directories.h"
#include "GameLogicSystem.h"

#include "../Bus/BusLoader.h"

#include "../Graphics/Renderer.h"
#include "../Graphics/SkeletalAnimationComponent.h"

#include "../Utils/FilesHelper.h"
#include "../Utils/InputSystem.h"
#include "../Utils/Logger.h"
#include "../Utils/ResourceManager.h"


TestScene::TestScene(Window* window)
	: GameScene(window)
{

}


TestScene::~TestScene()
{

}


CameraFPS* TestScene::createCameraFPSGlobal()
{
	SceneObject* cameraObject = _sceneManager->addSceneObject("cameraFPSGlobal");

	CameraFPS* cameraFPS = _graphicsManager->addCameraFPS(GameConfig::getInstance().windowWidth, GameConfig::getInstance().windowHeight, degToRad(58.0f), 0.1f, 1000.0f);
	cameraObject->addComponent(cameraFPS);
	cameraFPS->setRotationSpeed(0.001f);
	cameraFPS->setMoveSpeed(50);
	cameraObject->setRotation(0, degToRad(180), 0);
	cameraObject->setPosition(0, 1.5f, 5);

	CameraControlComponent* cameraControlComponent = _sceneManager->getGameLogicSystem()->addCameraControlComponent(cameraFPS);
	cameraObject->addComponent(cameraControlComponent);

	cameraControlComponent->setIsActive(true);

	return cameraFPS;
}


void TestScene::initialize()
{
	CameraFPS* camera = createCameraFPSGlobal();

	_graphicsManager->setCurrentCamera(camera);
	_soundManager->setActiveCamera(camera);

	// light
	SceneObject* lightSceneObject = _sceneManager->addSceneObject("bus");
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

	Transform transform;
	//RStaticModel* busModel = ResourceManager::getInstance().loadModelWithHierarchy("Buses/neoplan/neoplan2.fbx", "Buses/neoplan");
	//RStaticModel* busModel = ResourceManager::getInstance().loadModelWithHierarchyOnlyNode("Buses/neoplan/neoplan.fbx", "Buses/neoplan", "tuer0E", transform);
	//RStaticModel* busModel = ResourceManager::getInstance().loadModelWithHierarchyOnlyNode("Buses/neoplan/neoplan.fbx", "Buses/neoplan", "wagenkasten_main", transform);
	//RStaticModel* busModel = ResourceManager::getInstance().loadModelWithHierarchy("Buses/MAN/pulpit_new2.fbx", "Buses/MAN/Konin/");


	//SceneObject* busObject = _sceneManager->addSceneObject("bus");
	//RStaticModel* busModel = ResourceManager::getInstance().loadModelWithHierarchyOnlyNode("Buses/neoplan/neoplan.fbx", "Buses/neoplan", "tuer0E", transform);
	//_graphicsManager->addRenderObject(new RenderObject(busModel, busModel->getNodeByName("tuer0E")), busObject);

	/*RStaticModel* busModel = ResourceManager::getInstance().loadModelWithHierarchy("Buses/neoplan/neoplan.fbx", "Buses/neoplan");


	busObject = _sceneManager->addSceneObject("bus");

	std::vector<std::string> nodesToSkip = { "tuer0E", "tuer1E", "tuer2E", "tuer3E", "tuer4E", "tuer5E", "wagenkasten_front_N40xx", "wagenkasten_front_N4xx" };
	_graphicsManager->addRenderObject(new RenderObject(busModel, nodesToSkip), busObject);

	SceneObject* doorObject = _sceneManager->addSceneObject("door");
	StaticModelNode* modelNode = busModel->getNodeByName("tuer0E");
	_graphicsManager->addRenderObject(new RenderObject(busModel, modelNode), doorObject);
	doorObject->setPosition(modelNode->transform.getPosition());
	doorObject->setRotation(modelNode->transform.getRotation());
	doorObject->setScale(modelNode->transform.getScale());

	busObject->addChild(doorObject);

	SceneObject* frontObject = _sceneManager->addSceneObject("front");
	StaticModelNode* front1lNode = busModel->getNodeByName("wagenkasten_front_N40xx");
	StaticModelNode* front2lNode = busModel->getNodeByName("wagenkasten_front_N4xx");
	_graphicsManager->addRenderObject(new RenderObject(busModel, front2lNode), frontObject);
	frontObject->setPosition(front1lNode->transform.getPosition());
	frontObject->setRotation(front1lNode->transform.getRotation());
	frontObject->setScale(front1lNode->transform.getScale());

	busObject->addChild(frontObject);*/

	/*BusLoader busLoader(_sceneManager, _graphicsManager, _physicsManager, _soundManager);
	Bus* bus = busLoader.loadBus("neoplan");
	bus->getSceneObject()->move(1.0f, 0.0f, 0.0f);*/

	//initGui();
	initAnimation();
}

void TestScene::initGui()
{
	{
		Label* labelTitle = _gui->addLabel(ResourceManager::getInstance().loadFont("fonts/Roboto/Roboto-BoldItalic.ttf", 64), "START");
		labelTitle->setPosition(500, 500);
		labelTitle->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

		/*Button* img = _gui->addButton(ResourceManager::getInstance().loadOneColorTexture(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
			nullptr,
			ResourceManager::getInstance().loadFont("fonts/Roboto/Roboto-Italic.ttf", 64),
			"START");
		img->setPosition(500, 500);
		img->_image->setScale(100, 50);
		img->_label->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		img->_image->setColor(glm::vec4(0.0, 0.0f, 1.0f, 0.0f));*/

		Image* imgLine = _gui->addImage(ResourceManager::getInstance().loadOneColorTexture(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
		imgLine->setPosition(480, 490);
		imgLine->setScale(150, 1);
	}


	glm::vec2 startPosition(700, 300);
	{
		Label* labelTitle = _gui->addLabel(ResourceManager::getInstance().loadFont("fonts/Roboto/Roboto-BoldItalic.ttf", 32), "KONFIGURACJA");
		labelTitle->setPosition(700, 300);
		labelTitle->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

		/*Button* img = _gui->addButton(ResourceManager::getInstance().loadOneColorTexture(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
			nullptr,
			ResourceManager::getInstance().loadFont("fonts/Roboto/Roboto-BoldItalic.ttf", 32),
			"KONFIGURACJA");
		img->setPosition(700, 300);
		img->_image->setScale(100, 50);
		img->_label->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		img->_image->setColor(glm::vec4(0.0, 0.0f, 1.0f, 0.0f));*/

		Image* imgLine = _gui->addImage(ResourceManager::getInstance().loadOneColorTexture(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
		imgLine->setPosition(680, 290);
		imgLine->setScale(200, 1);
	}

	{
		Label* label = _gui->addLabel(ResourceManager::getInstance().loadFont("fonts/Roboto/Roboto-Regular.ttf", 32), "Typ 1");
		label->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		label->setScale(0.8, 0.8);
		label->setPosition(680 + (400 - label->getWidth()) / 2.0f, 250);

		Button* b1 = _gui->addButton(ResourceManager::getInstance().loadTexture("Data/arrow_back3.png"));
		b1->setPosition(680, 250);

		Button* b2 = _gui->addButton(ResourceManager::getInstance().loadTexture("Data/arrow_forward3.png"));
		b2->setPosition(680 + 400 - b2->getRealSize().x, 250);
	}

	{
		Label* label = _gui->addLabel(ResourceManager::getInstance().loadFont("fonts/Roboto/Roboto-Regular.ttf", 32), "Front pojazdu");
		label->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		label->setPosition(680, 200);
		label->setScale(0.8, 0.8);

		Label* label2 = _gui->addLabel(ResourceManager::getInstance().loadFont("fonts/Roboto/Roboto-Regular.ttf", 32), "1");
		label2->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		label2->setPosition(680 + label->getWidth() + 50 + (400 - label->getWidth() - 50 - label2->getWidth()) / 2.0f, 200);
		label2->setScale(0.8, 0.8);

		Button* b1 = _gui->addButton(ResourceManager::getInstance().loadTexture("Data/arrow_back3.png"));
		b1->setPosition(680 + label->getWidth() + 50, 200);

		Button* b2 = _gui->addButton(ResourceManager::getInstance().loadTexture("Data/arrow_forward3.png"));
		b2->setPosition(680 + 400 - b2->getRealSize().x, 200);
	}

	{
		Label* label = _gui->addLabel(ResourceManager::getInstance().loadFont("fonts/Roboto/Roboto-Regular.ttf", 32), "Wyswietlacz");
		label->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		label->setPosition(680, 150);
		label->setScale(0.8, 0.8);

		Label* label2 = _gui->addLabel(ResourceManager::getInstance().loadFont("fonts/Roboto/Roboto-Regular.ttf", 32), "true");
		label2->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		label2->setPosition(680 + label->getWidth() + 50 + (400 - label->getWidth() - 50 - label2->getWidth()) / 2.0f, 150);
		label2->setScale(0.8, 0.8);

		Button* b1 = _gui->addButton(ResourceManager::getInstance().loadTexture("Data/arrow_back3.png"));
		b1->setPosition(680 + label->getWidth() + 50, 150);

		Button* b2 = _gui->addButton(ResourceManager::getInstance().loadTexture("Data/arrow_forward3.png"));
		b2->setPosition(680 + 400 - b2->getRealSize().x, 150);
	}



	{
		Image* imageBackground = _gui->addImage(ResourceManager::getInstance().loadOneColorTexture(glm::vec4(0.0f, 0.0f, 0.0f, 0.1f)));
		imageBackground->setScale(220, 110);
		imageBackground->setPosition(160, 120);


		RFont* fontBoldItalic32 = ResourceManager::getInstance().loadFont("fonts/Roboto/Roboto-BoldItalic.ttf", 32);
		RFont* fontRegular26 = ResourceManager::getInstance().loadFont("fonts/Roboto/Roboto-Regular.ttf", 26);
		glm::vec2 pickerMargin = glm::vec2(5.0f, 7.5f);
		//glm::vec4 pickerBackgroundColor = glm::vec4(0.0f, 0.0f, 0.0f, 0.1f);
		//glm::vec4 pickerBackgroundColor = glm::vec4(1.0f, 1.0f, 1.0f, 0.3f);
		glm::vec4 pickerBackgroundColor = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);

		Label* labelTitle = _gui->addLabel(fontBoldItalic32, "KONFIGURACJA");
		labelTitle->setPosition(200, 300);
		labelTitle->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

		Image* imgLine = _gui->addImage(ResourceManager::getInstance().loadOneColorTexture(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
		imgLine->setPosition(180, 290);
		imgLine->setScale(200, 1);

		std::vector<std::string> optionsConfigurations;
		optionsConfigurations.push_back("Typ 1");
		optionsConfigurations.push_back("Typ 2");
		optionsConfigurations.push_back("Test123");
		Picker* picker1 = _gui->addPicker(fontRegular26, optionsConfigurations, 400, 40);
		picker1->setPosition(180, 250 - pickerMargin.y);
		picker1->setBackgroundColor(pickerBackgroundColor);
		picker1->setMargin(pickerMargin);
		picker1->setOnValueChangedCallback([](int index, const std::string& value)
			{
				LOG_DEBUG("Selected item: " + value + " (" + Strings::toString(index) + ")");
			});

		Label* labelFront = _gui->addLabel(fontRegular26, "Front pojazdu");
		labelFront->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		labelFront->setPosition(180, 200);

		std::vector<std::string> optionsFront;
		optionsFront.push_back("1");
		optionsFront.push_back("2");
		Picker* picker2 = _gui->addPicker(fontRegular26, optionsFront, 200, 40);
		picker2->setPosition(380, 200 - pickerMargin.y);
		picker2->setBackgroundColor(pickerBackgroundColor);
		picker2->setMargin(pickerMargin);

		Label* labelDisplay = _gui->addLabel(fontRegular26, "Wyswietlacz");
		labelDisplay->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		labelDisplay->setPosition(180, 150);

		std::vector<std::string> optionsDisplay;
		optionsDisplay.push_back("true");
		optionsDisplay.push_back("false");
		Picker* picker3 = _gui->addPicker(fontRegular26, optionsDisplay, 200, 40);
		picker3->setPosition(380, 150 - pickerMargin.y);
		picker3->setBackgroundColor(pickerBackgroundColor);
		picker3->setMargin(pickerMargin);
	}


	std::vector<std::string> options;
	options.push_back("1");
	options.push_back("2");
	options.push_back("3");
	options.push_back("4");
	options.push_back("5");
	options.push_back("6");
	options.push_back("7");
	options.push_back("8");
	options.push_back("9");
	options.push_back("10");
	options.push_back("20");
	Picker* picker1 = _gui->addPicker(ResourceManager::getInstance().loadFont("fonts/Roboto/Roboto-Regular.ttf", 32), options, 400, 40);
	picker1->setPosition(100, 50);

	/*float y = _window->getHeight() - 50;
	for (int i = 0; i < 20; ++i)
	{
		int size = 12 + 2 * i;
		y -= size;

		Label* label = _gui->addLabel(ResourceManager::getInstance().loadFont("fonts/Roboto/Roboto-Regular.ttf", size), "Font: " + toString(size));
		label->setPosition(10, y);
	}*/
}


void TestScene::initAnimation()
{
	/*SceneObject* animatedObject = _sceneManager->addSceneObject("Krystian");

	RStaticModel* animatedModel = ResourceManager::getInstance().loadModelWithHierarchy("Objects/peoples/Krystian/Krystian.dae", "Objects/peoples/Krystian/");
	RenderObject* animatedRenderObject = _graphicsManager->addRenderObject(new RenderObject(animatedModel), animatedObject);*/

	SceneObject* animatedObject = _sceneManager->addSceneObject("Krystian");

	RAnimatedModel* animatedModel = ResourceManager::getInstance().loadAnimatedModel("C:\\Users\\Michal\\Downloads\\55-rp_nathan_animated_003_walking_fbx\\rp_nathan_animated_003_walking.fbx", "C:\\Users\\Michal\\Downloads\\55-rp_nathan_animated_003_walking_fbx\\tex\\");
	//RAnimatedModel* animatedModel = ResourceManager::getInstance().loadAnimatedModel("Objects/peoples/Krystian/people4.dae", "Objects/peoples/Krystian/");
	RenderObject* animatedRenderObject = _graphicsManager->addRenderObject(new RenderObject(animatedModel), animatedObject);



	// Krystian.dae
	std::unordered_map<std::string, std::string> _animationNodeNameToBoneNameInModelMap;
	_animationNodeNameToBoneNameInModelMap["krystian_Head"] = "head";
	_animationNodeNameToBoneNameInModelMap["krystian_Neck1"] = "neck";
	_animationNodeNameToBoneNameInModelMap["krystian_Neck"] = "neck";

	_animationNodeNameToBoneNameInModelMap["krystian_RightShoulder"] = "rCollar";
	_animationNodeNameToBoneNameInModelMap["krystian_RightArm"] = "rShldr";
	_animationNodeNameToBoneNameInModelMap["krystian_RightForeArm"] = "rForeArm";
	_animationNodeNameToBoneNameInModelMap["krystian_RightHand"] = "rHand";
	_animationNodeNameToBoneNameInModelMap["krystian_RightFingerBase"] = "rHand";
	_animationNodeNameToBoneNameInModelMap["krystian_RightHandFinger1"] = "rMid1";
	_animationNodeNameToBoneNameInModelMap["krystian_RThumb"] = "rThumb1";

	_animationNodeNameToBoneNameInModelMap["krystian_LeftShoulder"] = "lCollar";
	_animationNodeNameToBoneNameInModelMap["krystian_LeftArm"] = "lShldr";
	_animationNodeNameToBoneNameInModelMap["krystian_LeftForeArm"] = "lForeArm";
	_animationNodeNameToBoneNameInModelMap["krystian_LeftHand"] = "lHand";
	_animationNodeNameToBoneNameInModelMap["krystian_LeftFingerBase"] = "lHand";
	_animationNodeNameToBoneNameInModelMap["krystian_LeftHandFinger1"] = "lMid1";
	_animationNodeNameToBoneNameInModelMap["krystian_LThumb"] = "lThumb1";

	_animationNodeNameToBoneNameInModelMap["krystian_Spine1"] = "chest";
	_animationNodeNameToBoneNameInModelMap["krystian_Spine"] = "abdomen";
	_animationNodeNameToBoneNameInModelMap["krystian_LowerBack"] = "hip";
	_animationNodeNameToBoneNameInModelMap["krystian_Hips"] = "hip";
	_animationNodeNameToBoneNameInModelMap["krystian_RHipJoint"] = "hip";
	_animationNodeNameToBoneNameInModelMap["krystian_LHipJoint"] = "hip";

	_animationNodeNameToBoneNameInModelMap["krystian_RightUpLeg"] = "rThigh";
	_animationNodeNameToBoneNameInModelMap["krystian_RightLeg"] = "rShin";
	_animationNodeNameToBoneNameInModelMap["krystian_RightFoot"] = "rFoot";
	_animationNodeNameToBoneNameInModelMap["krystian_RightToeBase"] = "rFoot";

	_animationNodeNameToBoneNameInModelMap["krystian_LeftUpLeg"] = "lThigh";
	_animationNodeNameToBoneNameInModelMap["krystian_LeftLeg"] = "lShin";
	_animationNodeNameToBoneNameInModelMap["krystian_LeftFoot"] = "lFoot";
	_animationNodeNameToBoneNameInModelMap["krystian_LeftToeBase"] = "lFoot";

	// KrystianOriginal.dae
	/*_animationNodeNameToBoneNameInModelMap["Head"] = "head";
	_animationNodeNameToBoneNameInModelMap["Neck1"] = "neck";
	_animationNodeNameToBoneNameInModelMap["Neck"] = "neck";

	_animationNodeNameToBoneNameInModelMap["RightShoulder"] = "rCollar";
	_animationNodeNameToBoneNameInModelMap["RightArm"] = "rShldr";
	_animationNodeNameToBoneNameInModelMap["RightForeArm"] = "rForeArm";
	_animationNodeNameToBoneNameInModelMap["RightHand"] = "rHand";
	_animationNodeNameToBoneNameInModelMap["RightFingerBase"] = "rHand";
	_animationNodeNameToBoneNameInModelMap["RightHandFinger1"] = "rMid1";
	_animationNodeNameToBoneNameInModelMap["RThumb"] = "rThumb1";

	_animationNodeNameToBoneNameInModelMap["LeftShoulder"] = "lCollar";
	_animationNodeNameToBoneNameInModelMap["LeftArm"] = "lShldr";
	_animationNodeNameToBoneNameInModelMap["LeftForeArm"] = "lForeArm";
	_animationNodeNameToBoneNameInModelMap["LeftHand"] = "lHand";
	_animationNodeNameToBoneNameInModelMap["LeftFingerBase"] = "lHand";
	_animationNodeNameToBoneNameInModelMap["LeftHandFinger1"] = "lMid1";
	_animationNodeNameToBoneNameInModelMap["LThumb"] = "lThumb1";

	_animationNodeNameToBoneNameInModelMap["Spine1"] = "chest";
	_animationNodeNameToBoneNameInModelMap["Spine"] = "abdomen";
	_animationNodeNameToBoneNameInModelMap["LowerBack"] = "hip";
	_animationNodeNameToBoneNameInModelMap["Hips"] = "hip";
	_animationNodeNameToBoneNameInModelMap["RHipJoint"] = "hip";
	_animationNodeNameToBoneNameInModelMap["LHipJoint"] = "hip";

	_animationNodeNameToBoneNameInModelMap["RightUpLeg"] = "rThigh";
	_animationNodeNameToBoneNameInModelMap["RightLeg"] = "rShin";
	_animationNodeNameToBoneNameInModelMap["RightFoot"] = "rFoot";
	_animationNodeNameToBoneNameInModelMap["RightToeBase"] = "rFoot";

	_animationNodeNameToBoneNameInModelMap["LeftUpLeg"] = "lThigh";
	_animationNodeNameToBoneNameInModelMap["LeftLeg"] = "lShin";
	_animationNodeNameToBoneNameInModelMap["LeftFoot"] = "lFoot";
	_animationNodeNameToBoneNameInModelMap["LeftToeBase"] = "lFoot";*/



	//RAnimation* animation = ResourceManager::getInstance().loadAnimation("Objects/peoples/Krystian/Krystian2.dae");
	//RAnimation* animation = ResourceManager::getInstance().loadAnimation("Objects/peoples/Krystian/02_01.bvh");
	RAnimation* animation = ResourceManager::getInstance().loadAnimation("C:\\Users\\Michal\\Downloads\\55-rp_nathan_animated_003_walking_fbx\\rp_nathan_animated_003_walking2.fbx");
	SkeletalAnimationComponent* skeletalAnimation = _graphicsManager->addSkeletalAnimation(animation);
	animatedObject->addComponent(skeletalAnimation);



	// people4.dae
	/*skeletalAnimation->_animationNodeNameToBoneNameInModelMap["people4_head"] = "head";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["people4_neck_01"] = "neck";

	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["people4_clavicle_r"] = "rCollar";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["people4_upperarm_r"] = "rShldr";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["people4_lowerarm_r"] = "rForeArm";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["people4_hand_r"] = "rHand";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["people4_thumb_01_r"] = "rThumb1";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["people4_thumb_02_r"] = "rThumb1";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["people4_thumb_03_r"] = "rThumb2";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["people4_index_01_r"] = "rIndex1";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["people4_index_02_r"] = "rIndex1";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["people4_index_03_r"] = "rIndex2";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["people4_middle_01_r"] = "rMid1";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["people4_middle_02_r"] = "rMid1";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["people4_middle_03_r"] = "rMid2";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["people4_ring_01_r"] = "rRing1";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["people4_ring_02_r"] = "rRing1";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["people4_ring_03_r"] = "rRing2";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["people4_pinky_01_r"] = "rPinky1";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["people4_pinky_02_r"] = "rPinky1";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["people4_pinky_03_r"] = "rPinky2";

	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["people4_clavicle_l"] = "lCollar";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["people4_upperarm_l"] = "lShldr";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["people4_lowerarm_l"] = "lForeArm";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["people4_hand_l"] = "lHand";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["people4_thumb_01_l"] = "lThumb1";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["people4_thumb_02_l"] = "lThumb1";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["people4_thumb_03_l"] = "lThumb2";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["people4_index_01_l"] = "lIndex1";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["people4_index_02_l"] = "lIndex1";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["people4_index_03_l"] = "lIndex2";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["people4_middle_01_l"] = "lMid1";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["people4_middle_02_l"] = "lMid1";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["people4_middle_03_l"] = "lMid2";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["people4_ring_01_l"] = "lRing1";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["people4_ring_02_l"] = "lRing1";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["people4_ring_03_l"] = "lRing2";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["people4_pinky_01_l"] = "lPinky1";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["people4_pinky_02_l"] = "lPinky1";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["people4_pinky_03_l"] = "lPinky2";

	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["people4_spine_03"] = "chest";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["people4_spine_02"] = "abdomen";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["people4_spine_01"] = "abdomen";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["people4_pelvis"] = "hip";

	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["people4_thigh_r"] = "rThigh";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["people4_calf_r"] = "rShin";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["people4_foot_r"] = "rFoot";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["people4_ball_r"] = "rFoot";

	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["people4_thigh_l"] = "lThigh";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["people4_calf_l"] = "lShin";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["people4_foot_l"] = "lFoot";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["people4_ball_l"] = "lFoot";*/




	


	/*skeletalAnimation->_animationNodeNameToBoneNameInModelMap["Head"] = "head";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["Neck1"] = "neck";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["Neck"] = "neck";

	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["RightShoulder"] = "rCollar";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["RightArm"] = "rShldr";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["RightForeArm"] = "rForeArm";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["RightHand"] = "rHand";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["RightFingerBase"] = "rHand";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["RightHandFinger1"] = "rMid1";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["RThumb"] = "rThumb1";

	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["LeftShoulder"] = "lCollar";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["LeftArm"] = "lShldr";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["LeftForeArm"] = "lForeArm";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["LeftHand"] = "lHand";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["LeftFingerBase"] = "lHand";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["LeftHandFinger1"] = "lMid1";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["LThumb"] = "lThumb1";

	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["Spine1"] = "chest";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["Spine"] = "abdomen";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["LowerBack"] = "hip";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["Hips"] = "hip";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["RHipJoint"] = "hip";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["LHipJoint"] = "hip";

	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["RightUpLeg"] = "rThigh";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["RightLeg"] = "rShin";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["RightFoot"] = "rFoot";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["RightToeBase"] = "rFoot";

	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["LeftUpLeg"] = "lThigh";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["LeftLeg"] = "lShin";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["LeftFoot"] = "lFoot";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["LeftToeBase"] = "lFoot";*/




	/*skeletalAnimation->_animationNodeNameToBoneNameInModelMap["Head"] = "Head";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["Neck1"] = "Neck1";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["Neck"] = "Neck";

	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["RightShoulder"] = "RightShoulder";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["RightArm"] = "RightArm";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["RightForeArm"] = "RightForeArm";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["RightHand"] = "RightHand";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["RightFingerBase"] = "RightFingerBase";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["RightHandFinger1"] = "RightHandFinger1";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["RThumb"] = "RThumb";

	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["LeftShoulder"] = "LeftShoulder";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["LeftArm"] = "LeftArm";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["LeftForeArm"] = "LeftForeArm";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["LeftHand"] = "LeftHand";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["LeftFingerBase"] = "LeftFingerBase";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["LeftHandFinger1"] = "LeftHandFinger1";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["LThumb"] = "LThumb";

	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["Spine1"] = "Spine1";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["Spine"] = "Spine";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["LowerBack"] = "LowerBack";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["Hips"] = "Hips";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["RHipJoint"] = "RHipJoint";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["LHipJoint"] = "LHipJoint";

	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["RightUpLeg"] = "RightUpLeg";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["RightLeg"] = "RightLeg";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["RightFoot"] = "RightFoot";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["RightToeBase"] = "RightToeBase";

	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["LeftUpLeg"] = "LeftUpLeg";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["LeftLeg"] = "LeftLeg";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["LeftFoot"] = "LeftFoot";
	skeletalAnimation->_animationNodeNameToBoneNameInModelMap["LeftToeBase"] = "LeftToeBase";*/
}


void TestScene::fixedStepUpdate(double deltaTime)
{
	//busObject->rotate(0.0f, degToRad(10.0f * deltaTime), 0.0f);

	_sceneManager->getGameLogicSystem()->update(deltaTime);
}


void TestScene::update(double deltaTime)
{

}


void TestScene::readInput(double deltaTime)
{

}


void TestScene::fixedStepReadInput(float deltaTime)
{

}


void TestScene::terminate()
{

}

