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
    protected:
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

		bool _normalsSmoothing;

        std::unique_ptr<Gearbox> _gearbox;
        std::unique_ptr<Engine> _engine;

        std::unordered_map<std::string, std::string> _variables;

        BusRaycast* _bus;

        RStaticModel* _currentBusModel;

        BusRaycast* loadBusRaycast(XMLElement* busElement);
        void loadEngineAndGearbox(XMLElement* busElement);
        void loadBusDescription(XMLElement* busElement);
        void loadAvailableVariables(XMLElement* busElement);
        bool loadBusModules(XMLElement* busElement);

        void fetchOptionalModelNodes(XMLElement* moduleElement, std::vector<std::string>& modelNodesNames);

        void loadModuleElements(XMLElement* moduleElement, XMLElement* busElement, BusRayCastModule& busModule);
        virtual void loadModuleConditionalElements(XMLElement* moduleElement, XMLElement* busElement, BusRayCastModule& busModule);

        virtual void loadWheels(XMLElement* moduleElement, BusRayCastModule& busModule);
        virtual void loadInteriorLights(XMLElement* moduleElement, BusRayCastModule& busModule);
        virtual void loadDriverParams(XMLElement* busElement, BusRayCastModule& busModule);
        virtual void loadSteeringWheel(XMLElement* moduleElement, BusRayCastModule& busModule);
        virtual void loadDesktop(XMLElement* moduleElement, BusRayCastModule& busModule);
        virtual void loadHeadlights(XMLElement* moduleElement, BusRayCastModule& busModule);
        virtual void loadDoors(XMLElement* moduleElement, BusRayCastModule& busModule);
        virtual void loadEnvironmentCaptureComponents(XMLElement* moduleElement, BusRayCastModule& busModule);
        virtual void loadMirrors(XMLElement* moduleElement, BusRayCastModule& busModule);
        virtual void loadDisplays(XMLElement* moduleElement, BusRayCastModule& busModule);
        virtual void loadCustomElements(XMLElement* parentElement, BusRayCastModule& busModule);
        virtual void loadModulesConnectionData(XMLElement* moduleElement, BusRayCastModule& busModule);

        void loadDoorSimple(XMLElement* doorElement, BusRayCastModule& busModule, SceneObject* doorObj, RStaticModel* doorModel, float mass, char group,
                            SoundComponent* openSoundComp, SoundComponent* closeSoundComp);
        void loadDoorSE(XMLElement* doorElement, BusRayCastModule& busModule, SceneObject* doorObj, RStaticModel* doorModel, float mass, char group,
                        SoundComponent* openSoundComp, SoundComponent* closeSoundComp);
        void loadDoorClassic(XMLElement* doorElement, BusRayCastModule& busModule, SceneObject* doorObj, RStaticModel* doorModel, StaticModelNode* doorModelNode, StaticModelNode* doorCollisionNode,
                            float mass, char group, SoundComponent* openSoundComp, SoundComponent* closeSoundComp, bool isDoorLoadedFromSeparateModel);

        SoundComponent* createSound(SceneObject* soundObject, const SoundDefinition& soundDefinition);
        void createRequireSoundComponents();

    public:
        BusLoader(SceneManager* smgr, PhysicsManager* pmgr, SoundManager* sndMgr);

        Bus* loadBus(const std::string& busName, const std::unordered_map<std::string, std::string>& variables = {});

};


#endif // BUSLOADER_H_INCLUDED
