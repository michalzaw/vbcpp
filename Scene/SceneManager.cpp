#include "SceneManager.h"

// XML reader
#include "../Utils/tinyxml2.h"
#include <sstream>
#include <cstdlib>
using namespace tinyxml2;

#include "../Utils/Helpers.hpp"
#include "../Graphics/LoadTerrainModel.h"

SceneManager::SceneManager(PhysicsManager* pMgr, SoundManager* sndMgr)
    : _sky(NULL), _physicsManager(pMgr), _soundManager(sndMgr)
{
    #ifdef _DEBUG_MODE
        std::cout << "Create SceneManager\n";
    #endif // _DEBUG_MODE
    _physicsManager->grab();

    _busStart.position = glm::vec3(0,3,0);
    _busStart.rotation = glm::vec3(0,0,0);

    //_graphicsManager = new GraphicsManager;
    //_physicsManager = new PhysicsManager;

    //_physicsManager->createPhysicsWorld();
}


SceneManager::~SceneManager()
{
    #ifdef _DEBUG_MODE
        std::cout << "Destroy SceneManager\n";
    #endif // _DEBUG_MODE


    for (std::list<SceneObject*>::iterator i = _sceneObjects.begin(); i != _sceneObjects.end(); ++i)
    {
        delete *i;
    }

    _physicsManager->drop();

    //delete _graphicsManager;
    //delete _physicsManager;
}

/*
GraphicsManager* SceneManager::getGraphicsManager()
{
    return _graphicsManager;
}
*/
/*
PhysicsManager* SceneManager::getPhysicsManager()
{
    return _physicsManager;
}
*/

SceneObject* SceneManager::addSceneObject(std::string name)
{
    SceneObject* obj = new SceneObject(name, this);

    _sceneObjects.push_back(obj);

    return obj;
}


void SceneManager::removeSceneObject(SceneObject* object)
{
    for (std::list<SceneObject*>::iterator i = _sceneObjects.begin(); i != _sceneObjects.end(); ++i)
    {
        if (*i == object)
        {
            i = _sceneObjects.erase(i);

            delete object;

            return;
        }
    }
}

void SceneManager::removeSceneObject(std::string name)
{
    for (std::list<SceneObject*>::iterator i = _sceneObjects.begin(); i != _sceneObjects.end(); ++i)
    {
        if ((*i)->getName() == name)
        {
            delete *i;

            i = _sceneObjects.erase(i);


        }
    }
}

SceneObject* SceneManager::getSceneObject(std::string name)
{
    for (std::list<SceneObject*>::iterator i = _sceneObjects.begin(); i != _sceneObjects.end(); ++i)
    {
        if ((*i)->getName() == name)
            return *i;
    }

    return 0;
}


void SceneManager::addSky(RTextureCubeMap* texture)
{
    if (_sky != NULL)
        removeSceneObject(_sky->getName());

    _sky = new Skybox(texture, this);
    _sceneObjects.push_back(_sky);
}


void SceneManager::loadScene(std::string filename)
{
    std::string dirPath = "Maps/" + filename + "/";
    std::string fullPath = dirPath + "/scene.xml";

    XMLDocument doc;
    XMLError result = doc.LoadFile( fullPath.c_str() );
    std::cout << result << std::endl;

    // Search for main element - Object
    XMLElement* scnElement = doc.FirstChildElement("Scene");
    if (scnElement == nullptr)
    {
        std::cout << "Scene element not found!" << std::endl;
        return;
    }

    XMLElement* startElement = scnElement->FirstChildElement("Start");
    if (startElement == nullptr)
    {
        std::cout << "Start point not found!" << std::endl;
    }
    else
    {
        const char* cPosition(startElement->Attribute("position"));
        _busStart.position = XMLstringToVec3(cPosition);

        const char* cRotation(startElement->Attribute("rotation"));
        _busStart.rotation = XMLstringToVec3(cRotation);
    }

    XMLElement* terrElement = scnElement->FirstChildElement("Terrain");
    if (terrElement == nullptr)
    {
        std::cout << "Terrain element not found!" << std::endl;
        return;
    }

    std::string terrainHeightmap(terrElement->Attribute("heightmap"));
    //std::string terrainTexture(terrElement->Attribute("texture"));
    //std::string terrainNormalmap(terrElement->Attribute("normalmap"));
    std::string materialName(terrElement->Attribute("material"));

    std::cout << "*** TERRAIN DATA ***" << std::endl;
    std::cout << "Heightmap: " << terrainHeightmap << std::endl;
    //std::cout << "Texture: " << terrainTexture << std::endl;
    //std::cout << "Normalmap: " << terrainNormalmap << std::endl;
    std::cout << "Material: " << materialName << std::endl;

    // load terrain here

    /*Material terrainMaterial;
    terrainMaterial.diffuseTexture = ResourceManager::getInstance().loadTexture(dirPath + terrainTexture);
    terrainMaterial.normalmapTexture = ResourceManager::getInstance().loadTexture(dirPath + terrainNormalmap);
    terrainMaterial.shader = NORMALMAPPING_MATERIAL;
    terrainMaterial.scale = glm::vec2(100, 100);
    terrainMaterial.shininess = 96.0f;*/

    std::string heightmapFullPath = dirPath + terrainHeightmap;
    std::string materialFullPath = dirPath + MaterialLoader::createMaterialFileName(terrainHeightmap);
    Model* terrModel = loadTerrainModel(heightmapFullPath.c_str(), materialFullPath, materialName, dirPath, 20);//, terrainMaterial, 20);
    RModel* terrain = new RModel("", terrModel);
    SceneObject* terrainObject = addSceneObject("terrain");
    RenderObject* terrainObj = GraphicsManager::getInstance().addRenderObject(new RenderObject(terrain), terrainObject);
    int collidesWith = COL_WHEEL | COL_BUS | COL_ENV | COL_DOOR;
    PhysicalBodyBvtTriangleMesh* terrainMesh = _physicsManager->createPhysicalBodyBvtTriangleMesh(terrain, COL_TERRAIN, collidesWith);
    terrainMesh->setRestitution(0.9f);
    terrainMesh->getRigidBody()->setFriction(1.0f);
    //terrainObject->addComponent(terrainObj);
    terrainObject->addComponent(terrainMesh);//terrainObj->setIsActive(false);


    XMLElement* objects = scnElement->FirstChildElement("Objects");

    XMLElement* objectElement = objects->FirstChildElement("Object");

    while (objectElement != nullptr)
    {
        std::string name(objectElement->Attribute("name"));
        const char* cPosition = objectElement->Attribute("position");
        glm::vec3 position(XMLstringToVec3(cPosition));

        const char* cRotation = objectElement->Attribute("rotation");
        glm::vec3 rotation(XMLstringToVec3(cRotation));

        std::cout << "==> SCENE OBJECT" << std::endl;
        std::cout << "Name: " << name << std::endl;
        std::cout << "Position: " << cPosition << std::endl;
        std::cout << "Rotation: " << cRotation << std::endl;

        loadObject(name, position, rotation);

        objectElement = objectElement->NextSiblingElement("Object");
    }


    // Roads
    XMLElement* roads = scnElement->FirstChildElement("Roads");

    std::map<std::string, std::vector<RoadLane>> profiles;

    XMLElement* roadElement = roads->FirstChildElement("Road");
    while (roadElement != nullptr)
    {
        std::string name = roadElement->Attribute("name");
        std::string profileName = roadElement->Attribute("profile");

        std::cout << "==> ROAD" << std::endl;
        std::cout << "profile name: " << profileName << std::endl;


        if (profiles.find(profileName) == profiles.end())
        {
            loadRoadProfile(profileName, &profiles);
        }


        std::vector<RoadSegment> segments;

        XMLElement* segmentElement = roadElement->FirstChildElement("Segment");
        while (segmentElement != nullptr)
        {
            RoadSegment segment;

            const char* type = segmentElement->Attribute("type");
            if (strcmp(type, "arc") == 0)
                segment.type = RST_ARC;
            else if (strcmp(type, "line"))
                segment.type = RST_LINE;
            segment.r = toFloat(segmentElement->Attribute("radius"));
            segment.begin = XMLstringToVec3(segmentElement->Attribute("beginPoint"));
            segment.end = XMLstringToVec3(segmentElement->Attribute("endPoint"));
            segment.pointsCount = toInt(segmentElement->Attribute("points"));
            const char* interpolation = segmentElement->Attribute("interpolation");
            if (strcmp(interpolation, "lin") == 0)
                segment.interpolation = RI_LIN;
            else if (strcmp(interpolation, "cos") == 0)
                segment.interpolation = RI_COS;

            segments.push_back(segment);

            segmentElement = segmentElement->NextSiblingElement("Segment");
        }
        //std::cout << profiles[profileName].size() << std::endl;
        //std::cout << segments.size() << std::endl;
        // create road
        Model* roadModel = createRoadModel(profiles[profileName], profiles[profileName].size(), segments);
        RModel* roadModel2 = new RModel("", roadModel);
        SceneObject* roadSceneObject = addSceneObject(name);
        RenderObject* roadRenderObject = GraphicsManager::getInstance().addRenderObject(new RenderObject(roadModel2), roadSceneObject);
        //roadSceneObject->addComponent(roadRenderObject);
        int collidesWith = COL_WHEEL | COL_BUS | COL_ENV | COL_DOOR;
        PhysicalBodyBvtTriangleMesh* roadMesh = _physicsManager->createPhysicalBodyBvtTriangleMesh(roadModel2, COL_TERRAIN, collidesWith);
        roadMesh->setRestitution(0.9f);
        roadMesh->getRigidBody()->setFriction(1.0f);
        roadSceneObject->addComponent(roadMesh);

        roadElement = roadElement->NextSiblingElement("Road");
    }
}


void SceneManager::loadObject(std::string name, glm::vec3 position, glm::vec3 rotation)
{
    /*
    SceneObject* crate = sceneMgr->addSceneObject("crate");
    RModel* model = ResourceManager::getInstance().loadModel("craten.3ds", "./");
    RenderObject* object2 = GraphicsManager::getInstance().addRenderObject(new RenderObject(model));

    PhysicalBodyBox* boxBody2 = physMgr->createPhysicalBodyBox(btVector3(1,1,1), 5.0f, btVector3(0,7,0), COL_ENV, collidesWith);
    boxBody2->setRestitution(0.1f);
    crate->addComponent(object2);
    crate->addComponent(boxBody2);
    crate->setPosition(glm::vec3(-10,3,-10));
    */

    std::string dirPath = "Objects/" + name + "/";
    std::string fullPath = dirPath + "object.xml";

    //std::cout << "Object path: " << fullPath << std::endl;

    XMLDocument doc;
    XMLError result = doc.LoadFile( fullPath.c_str() );
    std::cout << result << std::endl;

    // Search for main element - Object
    XMLElement* objElement = doc.FirstChildElement("Object");
    if (objElement == nullptr)
    {
        std::cout << "Object element not found!" << std::endl;
        return;
    }

    XMLElement* objDesc = objElement->FirstChildElement("Description");
    if (objDesc == nullptr)
    {
        std::cout << "Description element not found" << std::endl;
        return;
    }

    // Load file description
    std::string author(objDesc->Attribute("author"));
    std::string modelName(objDesc->Attribute("name"));
    std::string comment(objDesc->Attribute("comment"));

    std::cout << "*** OBJECT DATA ***" << std::endl;
    std::cout << "Author: " << author << std::endl;
    std::cout << "Name: " << modelName << std::endl;
    std::cout << "Comment: " << comment << std::endl;

    //glm::vec3 pos(-10,3,-10);

    XMLElement* components = objElement->FirstChildElement("Components");

    glm::vec3 scale(1,1,1);

    RModel* model = 0;


    XMLElement* componentElement = components->FirstChildElement("Component");
    std::cout << "COMPONENTS: " << comment << std::endl;
    while (componentElement != nullptr)
    {
        std::string componentType = componentElement->Attribute("type");
        std::cout << "Component: " << componentType << std::endl;

        SceneObject* sceneObject;

        if (componentType == "render")
        {
            std::string modelFile = componentElement->Attribute("model");

            std::string modelPath = dirPath + modelFile;

            sceneObject = addSceneObject(name);
        sceneObject->setPosition(position);
        sceneObject->setRotation(degToRad(rotation.x), degToRad(rotation.y), degToRad(rotation.z) );
            model = ResourceManager::getInstance().loadModel(modelPath, dirPath);
            RenderObject* renderObject = GraphicsManager::getInstance().addRenderObject(new RenderObject(model), sceneObject);
            //sceneObject->addComponent(renderObject);

            const char* cScale = componentElement->Attribute("scale");

            if (cScale != NULL)
            {
                std::cout << "Object scale: " << cScale << std::endl;
                scale = glm::vec3(XMLstringToVec3(cScale));
            }
        }
        else
        if (componentType == "physics")
        {
            std::string bodyType(componentElement->Attribute("body"));

            PhysicalBody* physicalBody;
            if (bodyType == "box")
            {
                int collidesWith = COL_TERRAIN | COL_BUS |COL_ENV;
                float halfExtents = atof(componentElement->Attribute("halfExtents"));
                float mass = atof(componentElement->Attribute("mass"));

                physicalBody = _physicsManager->createPhysicalBodyBox(btVector3(halfExtents,halfExtents,halfExtents), mass, COL_ENV, collidesWith);
                //physicalBody = _physicsManager->createPhysicalBodyBox(btVector3(halfExtents,halfExtents,halfExtents), mass, btVector3(0,0,0), COL_ENV, collidesWith);
                //physicalBody->setRestitution(0.1f);
                sceneObject->addComponent(physicalBody);
            }
            else
            if (bodyType == "dynamic")
            {
                std::cout << "- Creating dynamic Convex Hull collision shape" << std::endl;
                float mass = atoi(componentElement->Attribute("mass"));

                int collidesWith = COL_TERRAIN | COL_WHEEL | COL_BUS | COL_DOOR |COL_ENV;

                PhysicalBodyConvexHull* physicalBody = _physicsManager->createPhysicalBodyConvexHull(model->getCollisionMesh(), model->getCollisionMeshSize(), mass,
                                                                                                     COL_ENV, collidesWith);

                sceneObject->addComponent(physicalBody);
            }
            else
            if (bodyType == "static")
            {
                std::cout << "- Creating static Convex Hull collision shape" << std::endl;
                float mass = atoi(componentElement->Attribute("mass"));


                int collidesWith = COL_WHEEL | COL_BUS | COL_DOOR |COL_ENV;

                PhysicalBodyConvexHull* physicalBody = _physicsManager->createPhysicalBodyConvexHull(model->getCollisionMesh(), model->getCollisionMeshSize(), 0,
                                                                                                     COL_ENV, collidesWith);
                //terrainMesh->setRestitution(0.9f);
                //terrainMesh->getRigidBody()->setFriction(1.0f);
                sceneObject->addComponent(physicalBody);
            }
            else
            if (bodyType == "bvh")
            {
                std::cout << "- Creating BVH Triangle Mesh collision shape" << std::endl;

                int collidesWith = COL_WHEEL | COL_BUS | COL_DOOR | COL_ENV;

                PhysicalBodyBvtTriangleMesh* physicalBody = _physicsManager->createPhysicalBodyBvtTriangleMesh(model, COL_ENV, collidesWith);
                //terrainMesh->setRestitution(0.9f);
                //terrainMesh->getRigidBody()->setFriction(1.0f);
                sceneObject->addComponent(physicalBody);
            }
        }
        else
        if (componentType == "tree")
        {
            TreeComponent* component = new TreeComponent;
            sceneObject->addComponent(component);
		}
		else
        if (componentType == "sound")
        {
            std::string soundFile = componentElement->Attribute("file");
            std::string soundLooping = componentElement->Attribute("looping");
            std::string soundPath = dirPath + soundFile;

            bool looping = (soundLooping == "true" ? true : false);

            float playDistance = atoi(componentElement->Attribute("playDistance"));
            float volume = atoi(componentElement->Attribute("volume"));

            SoundComponent* sound = new SoundComponent(soundPath, EST_AMBIENT, looping);
            _soundManager->addSoundComponent(sound);
            sound->setPlayDistance(playDistance);
            sound->setGain(volume);

            sceneObject->addComponent(sound);
        }

        //sceneObject->setPosition(position);
        //sceneObject->setRotation(degToRad(rotation.x), degToRad(rotation.y), degToRad(rotation.z) );
        //sceneObject->setScale(scale);

        componentElement = componentElement->NextSiblingElement("Component");
    }
}


void SceneManager::loadRoadProfile(std::string name, std::map<std::string, std::vector<RoadLane>>* profiles)
{
    std::string dirPath = "RoadProfiles/" + name + "/";
    std::string fullPath = dirPath + "profile.xml";
    std::string materialFullPath = MaterialLoader::createMaterialFileName(fullPath);

    //std::cout << "Profile path: " << fullPath << std::endl;

    XMLDocument doc;
    XMLError result = doc.LoadFile( fullPath.c_str() );
    std::cout << result << std::endl;

    // Search for main element - Object
    XMLElement* profileElement = doc.FirstChildElement("Profile");
    if (profileElement == nullptr)
    {
        std::cout << "Profile element not found!" << std::endl;
        return;
    }

    XMLElement* profileDesc = profileElement->FirstChildElement("Description");
    if (profileDesc == nullptr)
    {
        std::cout << "Description element not found" << std::endl;
        return;
    }

    // Load file description
    std::string author(profileDesc->Attribute("author"));
    std::string profName(profileDesc->Attribute("name"));
    std::string comment(profileDesc->Attribute("comment"));

    std::cout << "*** PROFILE DATA ***" << std::endl;
    std::cout << "Author: " << author << std::endl;
    std::cout << "Name: " << profName << std::endl;
    std::cout << "Comment: " << comment << std::endl;

    MaterialLoader matLoader;
    matLoader.openFile(materialFullPath.c_str());

    XMLElement* lanesElement = profileElement->FirstChildElement("Lanes");

    if (lanesElement == nullptr)
    {
        std::cout << "Lanes element not found" << std::endl;
        return;
    }
    else
    {
        profiles->insert(std::make_pair(name, std::vector<RoadLane>()));

        XMLElement* laneElement = lanesElement->FirstChildElement("Lane");
        while (laneElement != nullptr)
        {
            RoadLane lane;
            lane.material = matLoader.loadMaterial(laneElement->Attribute("material"), dirPath);
            lane.r1 = toFloat(laneElement->Attribute("x1"));
            lane.r2 = toFloat(laneElement->Attribute("x2"));
            lane.height1 = toFloat(laneElement->Attribute("y1"));
            lane.height2 = toFloat(laneElement->Attribute("y2"));

            (*profiles)[name].push_back(lane);

            laneElement = laneElement->NextSiblingElement("Lane");
        }
    }
}
