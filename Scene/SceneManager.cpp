#include "SceneManager.h"

// XML reader
#include "../Utils/tinyxml2.h"
#include <sstream>
#include <cstdlib>
using namespace tinyxml2;

#include "../Utils/Helpers.hpp"
#include "../Graphics/LoadTerrainModel.h"

SceneManager::SceneManager(PhysicsManager* pMgr)
    : _sky(NULL), _physicsManager(pMgr)
{
    #ifdef _DEBUG_MODE
        std::cout << "Create SceneManager\n";
    #endif // _DEBUG_MODE
    _physicsManager->grab();

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
            delete *i;
    }
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

    XMLElement* terrElement = scnElement->FirstChildElement("Terrain");
    if (terrElement == nullptr)
    {
        std::cout << "Terrain element not found!" << std::endl;
        return;
    }

    std::string terrainHeightmap(terrElement->Attribute("heightmap"));
    std::string terrainTexture(terrElement->Attribute("texture"));
    std::string terrainNormalmap(terrElement->Attribute("normalmap"));

    std::cout << "*** TERRAIN DATA ***" << std::endl;
    std::cout << "Heightmap: " << terrainHeightmap << std::endl;
    std::cout << "Texture: " << terrainTexture << std::endl;
    std::cout << "Normalmap: " << terrainNormalmap << std::endl;

    // load terrain here

    Material terrainMaterial;
    terrainMaterial.diffuseTexture = ResourceManager::getInstance().loadTexture(dirPath + terrainTexture);
    terrainMaterial.normalmapTexture = ResourceManager::getInstance().loadTexture(dirPath + terrainNormalmap);
    terrainMaterial.shader = NORMALMAPPING_MATERIAL;
    terrainMaterial.scale = glm::vec2(100, 100);

    std::string heightmapFullPath = dirPath + terrainHeightmap;
    Model* terrModel = loadTerrainModel(heightmapFullPath.c_str(), terrainMaterial, 20);
    RModel* terrain = new RModel("", terrModel);
    RenderObject* terrainObj = GraphicsManager::getInstance().addRenderObject(new RenderObject(terrain));
    SceneObject* terrainObject = addSceneObject("terrain");
    int collidesWith = COL_WHEEL | COL_BUS | COL_ENV | COL_DOOR;
    PhysicalBodyBvtTriangleMesh* terrainMesh = _physicsManager->createPhysicalBodyBvtTriangleMesh(terrain, btVector3(0,0,0), COL_TERRAIN, collidesWith);
    terrainMesh->setRestitution(0.9f);
    terrainMesh->getRigidBody()->setFriction(1.0f);
    terrainObject->addComponent(terrainObj);
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

        loadObject(name, position);

        objectElement = objectElement->NextSiblingElement("Object");
    }
}


void SceneManager::loadObject(std::string name, glm::vec3 position)
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

    std::cout << "Object path: " << fullPath << std::endl;

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

    glm::vec3 pos(-10,3,-10);

    XMLElement* components = objElement->FirstChildElement("Components");

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

            sceneObject = addSceneObject(modelName);
            model = ResourceManager::getInstance().loadModel(modelPath, dirPath);
            RenderObject* renderObject = GraphicsManager::getInstance().addRenderObject(new RenderObject(model));
            sceneObject->addComponent(renderObject);
            //sceneObject->setPosition(glm::vec3(-10,3,-10));
            //sceneObject->setPosition(pos);
            sceneObject->setPosition(position);
            //sceneObject->setPosition(glm::vec3(0,0,0));
        }
        else
        if (componentType == "physics")
        {



            std::string bodyType(componentElement->Attribute("body"));

            PhysicalBody* physicalBody;
            if (bodyType == "box")
            {
                int collidesWith = COL_TERRAIN | COL_BUS;
                float halfExtents = atof(componentElement->Attribute("halfExtents"));
                float mass = atof(componentElement->Attribute("mass"));

                physicalBody = _physicsManager->createPhysicalBodyBox(btVector3(halfExtents,halfExtents,halfExtents), mass, btVector3(position.x,position.y,position.z), COL_ENV, collidesWith);
                //physicalBody = _physicsManager->createPhysicalBodyBox(btVector3(halfExtents,halfExtents,halfExtents), mass, btVector3(0,0,0), COL_ENV, collidesWith);
                //physicalBody->setRestitution(0.1f);
                sceneObject->addComponent(physicalBody);
            }
            else
            if (bodyType == "convex")
            {

            }
            else
            if (bodyType == "static")
            {
                std::cout << "<><><><><><>>####  Model: " << model << std::endl;
                float mass = atoi(componentElement->Attribute("mass"));

                /*
                RModel* domekModel = ResourceManager::getInstance().loadModel("Objects/domek/domek_vbcpp.3ds", "Objects/domek/");
                RenderObject* domekRender = GraphicsManager::getInstance().addRenderObject(new RenderObject(domekModel));
                SceneObject* domekObject = sceneMgr->addSceneObject("domek");
                int collidesWith = COL_WHEEL | COL_BUS | COL_DOOR;
                PhysicalBodyBvtTriangleMesh* domekBody = physMgr->createPhysicalBodyBvtTriangleMesh(domekModel, btVector3(0,0,0), COL_ENV, collidesWith);
                domekObject->addComponent(domekRender);
                domekObject->addComponent(domekBody);
                */

                int collidesWith = COL_WHEEL | COL_BUS | COL_DOOR;

                //PhysicalBodyBvtTriangleMesh* physicalBody = _physicsManager->createPhysicalBodyBvtTriangleMesh(model, btVector3(position.x,position.y,position.z), COL_ENV, collidesWith);
                PhysicalBodyConvexHull* physicalBody = _physicsManager->createPhysicalBodyConvexHull(model->getCollisionMesh(), model->getCollisionMeshSize(), mass, btVector3(position.x, position.y, position.z), COL_ENV, collidesWith);
                //PhysicalBodyConvexHull* physicalBody = _physicsManager->createPhysicalBodyConvexHull(model->getCollisionMesh(), model->getCollisionMeshSize(), mass, btVector3(0, 0, 0), COL_ENV, collidesWith);
                //terrainMesh->setRestitution(0.9f);
                //terrainMesh->getRigidBody()->setFriction(1.0f);
                //terrainObject->addComponent(terrainObj);
                //terrainObject->addComponent(terrainMesh);
                sceneObject->addComponent(physicalBody);
            }
        }

        //sceneObject->setPosition(position);

        componentElement = componentElement->NextSiblingElement("Component");
    }
}
