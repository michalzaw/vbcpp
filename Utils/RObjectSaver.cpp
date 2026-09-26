#include "RObjectSaver.h"

#include "FilesHelper.h"
#include "Logger.h"
#include "Math.h"
#include "ResourceDescriptionUtils.h"
#include "tinyxml2.h"
using namespace tinyxml2;
#include "XmlUtils.h"

#include "../Game/TransformAnimatorComponent.h"

#include "../Graphics/RenderObject.h"
#include "../Graphics/RenderableSurface.h"

#include "../Scene/SceneObject.h"

#include "../Scripting/ScriptComponent.h"


namespace RObjectSaver
{
	void saveDescription(XMLElement* objectElement, XMLDocument& doc, const std::string& objectName)
	{
		XMLElement* descriptionElement = doc.NewElement("Description");

		ResourceDescriptionUtils::saveResourceDescription(descriptionElement, { " ", objectName, " " });

		objectElement->InsertEndChild(descriptionElement);
	}

	void saveRenderComponent(XMLElement* componentElement, RenderObject* component, const std::string& pathToDirectory)
	{
		std::string modelRelativePath = FilesHelper::relativePathToFile(component->getModel(0)->getPath(), pathToDirectory, false);

		componentElement->SetAttribute("type", "render");
		componentElement->SetAttribute("model", modelRelativePath.c_str());
		if (component->getNumberOfLod() > 1)
		{
			componentElement->SetAttribute("lowPolyModel", component->getModel(1)->getPath().c_str());
		}
		if (component->isSingleNodeFromModel())
		{
			componentElement->SetAttribute("modelNodeName", component->getModelRootNode(0)->getName().c_str());
		}

		componentElement->SetAttribute("dynamic", component->isDynamicObject() ? "true" : "false");
		componentElement->SetAttribute("castShadows", component->isCastShadows() ? "true" : "false");
		//componentElement->SetAttribute("normalsSmoothing", "true");
		componentElement->SetAttribute("loadWithHierarchy", component->isSingleNodeFromModel() ? "true" : "false");
		if (component->getNumberOfLod() > 1)
		{
			//componentElement->SetAttribute("lowPolyModelNormalsSmoothing", "true");
			componentElement->SetAttribute("lowPolyLoadWithHierarchy", component->isSingleNodeFromModel() ? "true" : "false");
		}
		//componentElement->SetAttribute("animated", "false");
		componentElement->SetAttribute("singleNodeFromModel", component->isSingleNodeFromModel() ? "true" : "false");

	}

	void saveScriptComponent(XMLElement* componentElement, ScriptComponent* component, const std::string& pathToDirectory)
	{
		std::string scriptRelativePath = FilesHelper::relativePathToFile(component->getScriptFile()->getPath(), pathToDirectory, false);

		componentElement->SetAttribute("type", "script");
		componentElement->SetAttribute("fileName", scriptRelativePath.c_str());
	}

	void saveRenderableSurface(XMLElement* componentElement, RenderableSurface* component)
	{
		componentElement->SetAttribute("type", "renderableSurface");
		componentElement->SetAttribute("textureWidth", component->getWidth());
		componentElement->SetAttribute("textureHeight", component->getHeight());
		componentElement->SetAttribute("backgroundColor", vec4ToString(component->getBackgroundColor()).c_str());
	}


	void saveTransformAnimatorComponent(XMLElement* componentElement, TransformAnimatorComponent* component)
	{
		componentElement->SetAttribute("type", "transformAnimator");
	}

	void saveComponent(XMLElement* componentElement, Component* component, const std::string& pathToDirectory)
	{
		if (component->getType() == CT_RENDER_OBJECT)
		{
			saveRenderComponent(componentElement, static_cast<RenderObject*>(component), pathToDirectory);
		}
		/*else if (componentType == "physics")
		{
			loadPhysicsComponent(componentElement, object, componentIndex);
		}
		else if (componentType == "tree")
		{
			loadTreeComponent(componentElement, object, componentIndex);
		}
		else if (componentType == "sound")
		{
			loadSoundComponent(componentElement, object, componentIndex);
		}
		else if (componentType == "bus-stop")
		{
			loadBusStopComponent(componentElement, object, componentIndex);
		}
		else if (componentType == "environmentCapture")
		{
			loadEnvironmentCaptureComponent(componentElement, object, componentIndex);
		}
		else if (componentType == "crossroad")
		{
			loadCrossroadComponent(componentElement, object, componentIndex);
		}
		else if (componentType == "skeletalAnimation")
		{
			loadSkeletalAnimation(componentElement, object, componentIndex);
		}
		else if (componentType == "vehicle")
		{
			loadVehicle(componentElement, object, componentIndex);
		}
		else if (componentType == "aiAgent")
		{
			loadAiAgent(componentElement, object, componentIndex);
		}
		else if (componentType == "aiAgentVehicle")
		{
			loadAiAgentVehicle(componentElement, object, componentIndex);
		}
		else if (componentType == "trafficLights")
		{
			loadTrafficLightsComponent(componentElement, object, componentIndex);
		}*/
		else if (component->getType() == CT_SCRIPT)
		{
			saveScriptComponent(componentElement, static_cast<ScriptComponent*>(component), pathToDirectory);
		}
		else if (component->getType() == CT_RENDERABLE_SURFACE)
		{
			saveRenderableSurface(componentElement, static_cast<RenderableSurface*>(component));
		}
		else if (component->getType() == CT_TRANSFORM_ANIMATOR)
		{
			saveTransformAnimatorComponent(componentElement, static_cast<TransformAnimatorComponent*>(component));
		}
	}

	void saveComponents(XMLElement* objectElement, XMLDocument& doc, SceneObject* sceneObject, const std::string& pathToDirectory)
	{
		XMLElement* componentsElement = doc.NewElement("Components");

		for (int i = 0; i < sceneObject->getComponentsCount(); ++i)
		{
			Component* component = sceneObject->getComponent(i);
			XMLElement* componentElement = doc.NewElement("Component");

			saveComponent(componentElement, component, pathToDirectory);

			componentsElement->InsertEndChild(componentElement);
		}

		objectElement->InsertEndChild(componentsElement);
	}

	void saveChildrens(XMLElement* objectElement, XMLDocument& doc, SceneObject* sceneObject, const std::string& pathToDirectory)
	{
		XMLElement* objectsElement = doc.NewElement("Objects");

		for (SceneObject* child : sceneObject->getChildren())
		{
			XMLElement* childObjectElement = doc.NewElement("Object");

			childObjectElement->SetAttribute("name", child->getName().c_str());

			RenderObject* renderObject = child->getComponentWithCasting<RenderObject>(CT_RENDER_OBJECT);
			if (renderObject != nullptr && !renderObject->isSingleNodeFromModel())
			{
				childObjectElement->SetAttribute("position", vec3ToString(child->getPosition()).c_str());
				glm::vec3 rotation = glm::vec3(radToDeg(child->getRotation().x),
											   radToDeg(child->getRotation().y),
											   radToDeg(child->getRotation().z));
				childObjectElement->SetAttribute("rotation", vec3ToString(rotation).c_str());
			}

			saveComponents(childObjectElement, doc, child, pathToDirectory);
			saveChildrens(childObjectElement, doc, child, pathToDirectory);

			objectsElement->InsertEndChild(childObjectElement);
		}

		objectElement->InsertEndChild(objectsElement);
	}

	void saveObjectToFile(SceneObject* sceneObject, const std::string& pathToDirectory)
	{
		LOG_INFO("Saving SceneObject: " + sceneObject->getName());

		std::string objectName = FilesHelper::getFileNameFromPath(pathToDirectory);

		XMLDocument doc;

		XMLDeclaration* declaration = doc.NewDeclaration();
		doc.InsertFirstChild(declaration);

		XMLElement* rootNode = doc.NewElement("Object");
		doc.InsertEndChild(rootNode);

		saveDescription(rootNode, doc, objectName);
		saveComponents(rootNode, doc, sceneObject, pathToDirectory);
		saveChildrens(rootNode, doc, sceneObject, pathToDirectory);

		XMLError err = doc.SaveFile((pathToDirectory + "/object.xml").c_str());

		LOG_INFO("Saved file with code: " + err);
	}

}
