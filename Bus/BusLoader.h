#ifndef BUSLOADER_H_INCLUDED
#define BUSLOADER_H_INCLUDED


#include "Bus.h"
#include "BusRaycast.h"
#include "BusConstraint.h"


namespace tinyxml2
{
    class XMLElement;
}


struct ModelNodeAndTransform
{
    RStaticModel* model;
    Transform transform;

    ModelNodeAndTransform()
    {}
    ModelNodeAndTransform(RStaticModel* m, Transform& t)
        : model(m), transform(t)
    {}
};


class BusLoader
{
    private:
        static constexpr char* BUS_CONFIG_FILENAME = "config.xml";

        int _busCollidesWith;
        int _wheelCollidesWith;
        int _doorCollidesWith;

        SceneManager* _sMgr;
        PhysicsManager* _pMgr;
        SoundManager* _sndMgr;

        std::string _objName;
        std::string _busPath;
        std::string _texturePath;

        std::unique_ptr<Gearbox> _gearbox;
        std::unique_ptr<Engine> _engine;

        std::unordered_map<std::string, ModelNodeAndTransform> _cachedModelNodes;

        BusRaycast* _bus;

        BusRaycast* loadBusRaycast(XMLElement* busElement, const std::string& busName);
        void loadEngineAndGearbox(XMLDocument& doc);
        bool loadBusModules(XMLElement* busElement);

        void loadModelNodes(XMLElement* moduleElement, std::string modelPath, std::string texturePath, std::vector<std::string>& modelNodesNames,
                            std::unordered_map<std::string, ModelNodeAndTransform>& modelNodes);

        void loadWheels(XMLElement* moduleElement, BusRayCastModule& busModule);
        void loadInteriorLights(XMLElement* moduleElement, BusRayCastModule& busModule);
        void loadDriverParams(XMLElement* busElement, BusRayCastModule& busModule);
        void loadSteeringWheel(XMLElement* moduleElement, BusRayCastModule& busModule);
        void loadDesktop(XMLElement* moduleElement, BusRayCastModule& busModule);
        void loadHeadlights(XMLElement* moduleElement, BusRayCastModule& busModule);
        void loadDoors(XMLElement* moduleElement, BusRayCastModule& busModule);
        void loadEnvironmentCaptureComponents(XMLElement* moduleElement, BusRayCastModule& busModule);
        void loadMirrors(XMLElement* moduleElement, BusRayCastModule& busModule);
        void loadModulesConnectionData(XMLElement* moduleElement, BusRayCastModule& busModule);

        void loadDoorSimple(XMLElement* doorElement, BusRayCastModule& busModule, SceneObject* doorObj, RStaticModel* doorModel, float mass, char group,
                            SoundComponent* openSoundComp, SoundComponent* closeSoundComp);
        void loadDoorSE(XMLElement* doorElement, BusRayCastModule& busModule, SceneObject* doorObj, RStaticModel* doorModel, float mass, char group,
                        SoundComponent* openSoundComp, SoundComponent* closeSoundComp);
        void loadDoorClassic(XMLElement* doorElement, BusRayCastModule& busModule, SceneObject* doorObj, RStaticModel* doorModel, float mass, char group,
                            SoundComponent* openSoundComp, SoundComponent* closeSoundComp, bool isDoorLoadedFromSeparateModel);

        void createRequireSoundComponents();

    public:
        BusLoader(SceneManager* smgr, PhysicsManager* pmgr, SoundManager* sndMgr);

        Bus* loadBus(const std::string& busName);

};


#endif // BUSLOADER_H_INCLUDED
