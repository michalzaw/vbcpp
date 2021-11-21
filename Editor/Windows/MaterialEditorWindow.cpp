#include "MaterialEditorWindow.h"

#include <vector>

#include <portable-file-dialogs.h>

#include "../..//ImGui/imgui.h"

#include "../../Graphics/Material.h"
#include "../../Graphics/MaterialSaver.h"
#include "../../Graphics/GraphicsManager.h"
#include "../../Graphics/LoadMaterial.h"

#include "../../Utils/FilesHelper.h"
#include "../../Utils/Logger.h"
#include "../../Utils/ResourceManager.h"


namespace vbEditor
{
	extern GraphicsManager* _graphicsManager;

	RenderObject* currentRenderObject = nullptr;
	RStaticModel* currentStaticModel = nullptr;
	unsigned int currentModelLod = 0;
	unsigned int currentMaterialIndex = 0;

	Material* currentMaterial = nullptr;
	Material originalMaterial;

	RTexture* whiteTexture = nullptr;

	bool isOpen = false;
	bool isMaterialModified = false;

	void reloadCurrentMaterialInAllObjects()
	{
		std::list<RenderObject*>& renderObjects = _graphicsManager->getRenderObjects();
		for (RenderObject* renderObject : renderObjects)
		{
			if (renderObject->getModel() == currentStaticModel)
			{
				renderObject->updateLocalMaterialFromModel(currentMaterialIndex, currentModelLod);
			}
		}
	}

	void saveMaterial()
	{
		if (isMaterialModified)
		{
			reloadCurrentMaterialInAllObjects();

			std::string modelFileName = currentRenderObject->getModel()->getPath();
			std::string materialXmlFileName = MaterialLoader::createMaterialFileName(modelFileName);
			std::string objectDirPath = currentRenderObject->getSceneObject()->getObjectDefinition()->getPath();

			LOG_INFO("modelFileName: " + modelFileName);
			LOG_INFO("materialXmlFileName: " + materialXmlFileName);
			LOG_INFO("objectDirPath: " + objectDirPath);
			MaterialSaver::saveMaterials(materialXmlFileName, currentRenderObject->getModel()->getMaterials(), objectDirPath);

			isMaterialModified = false;
		}
	}

	void cancelChangesInMaterial()
	{
		if (isMaterialModified)
		{
			*currentMaterial = originalMaterial;
			currentRenderObject->updateLocalMaterialFromModel(currentMaterialIndex, currentModelLod);
		}
	}

	void showButtons()
	{
		if (ImGui::Button("Save"))
		{
			saveMaterial();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel"))
		{
			cancelChangesInMaterial();

			isOpen = false;
		}
	}

	void showMaterialNameEdit()
	{
		char buffer[50];

		strncpy(buffer, currentMaterial->name.c_str(), sizeof buffer);

		buffer[sizeof buffer - 1] = '\0';

		if (ImGui::InputText("Name", buffer, IM_ARRAYSIZE(buffer), ImGuiInputTextFlags_ReadOnly))
		{
			currentMaterial->name = std::string(buffer);
		}
	}

	void showShaderTypeCombo()
	{
		int typeComboCurrentItem = currentMaterial->shader;

		const char* typeComboItems[] = {
			"SOLID_MATERIAL",
			"NOTEXTURE_MATERIAL",
			"NORMALMAPPING_MATERIAL",
			"SOLID_EMISSIVE_MATERIAL",
			"CAR_PAINT_MATERIAL",
			"MIRROR_MATERIAL",
			"ALPHA_TEST_MATERIAL",
			"NEW_TREE_MATERIAL",
			"NEW_TREE_2_MATERIAL",
			"PBR_MATERIAL",
			"PBR_TREE_MATERIAL",
			"TREE_MATERIAL",
			"GRASS_MATERIAL",
			"SKY_MATERIAL",
			"GLASS_MATERIAL",
			"WIREFRAME_MATERIAL"
		};

		if (ImGui::Combo("Material type", &typeComboCurrentItem, typeComboItems, IM_ARRAYSIZE(typeComboItems)))
		{
			currentMaterial->shader = (ShaderType) typeComboCurrentItem;
			isMaterialModified = true;
		}
	}

	void showColorsEdit()
	{
		ImGui::Text("Colors");

		if (ImGui::ColorEdit4("Ambient", (float*)& currentMaterial->ambientColor, ImGuiColorEditFlags_NoOptions))
		{
			isMaterialModified = true;
		}
		if (ImGui::ColorEdit4("Diffuse", (float*)& currentMaterial->diffuseColor, ImGuiColorEditFlags_NoOptions))
		{
			isMaterialModified = true;
		}
		if (ImGui::ColorEdit4("Specular", (float*)& currentMaterial->specularColor, ImGuiColorEditFlags_NoOptions))
		{
			isMaterialModified = true;
		}
		if (ImGui::ColorEdit4("Emissive", (float*)& currentMaterial->emissiveColor, ImGuiColorEditFlags_NoOptions))
		{
			isMaterialModified = true;
		}
	}

	void showTextureEdit(RTexture*& texture, const char* name)
	{
		if (whiteTexture == nullptr)
		{
			whiteTexture = ResourceManager::getInstance().loadDefaultWhiteTexture();
		}

		char id[50];
		sprintf(id, "texture#%s", name);

		ImGui::PushID(id);

		ImGuiTreeNodeFlags headerFlag = texture != nullptr && texture->getPath() != ".defaultTexture" ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None;
		if (ImGui::CollapsingHeader(name, headerFlag))
		{
			char buffer[1024] = { '\0' };

			if (texture != nullptr)
				strncpy(buffer, texture->getPath().c_str(), sizeof buffer);

			buffer[sizeof buffer - 1] = '\0';

			//ImGui::Text(name);

			if (ImGui::InputText("", buffer, IM_ARRAYSIZE(buffer), ImGuiInputTextFlags_ReadOnly))
			{
				//currentMaterial->name = std::string(buffer);
			}

			ImGui::SameLine();

			if (ImGui::Button("..."))
			{
				std::vector<std::string> result = pfd::open_file("Choose texture...").result();

				if (result.size() == 1)
				{
					LOG_INFO(result[0]);

					std::string path = result[0];
					std::string objectDirPath = currentRenderObject->getSceneObject()->getObjectDefinition()->getPath();

					std::string newPath = objectDirPath + FilesHelper::getFileNameFromPath(path);
					if (!FilesHelper::isInPathSubdir(path, objectDirPath))
					{
						FilesHelper::copyFile(path, newPath);
					}
					texture = ResourceManager::getInstance().loadTexture(newPath);

					isMaterialModified = true;
				}
			}

			if (texture != nullptr)
			{
				ImGui::Image((ImTextureID)texture->getID(), ImVec2(128, 128), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(1.0f, 1.0f, 1.0f, 0.5f));
			}
			else
			{
				ImGui::Image((ImTextureID)whiteTexture->getID(), ImVec2(128, 128), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(1.0f, 1.0f, 1.0f, 0.5f));
			}
		}
		ImGui::PopID();
	}

	void showTexturesEdit()
	{
		ImGui::Text("Textures");

		showTextureEdit(currentMaterial->diffuseTexture, "Diffuse");
		showTextureEdit(currentMaterial->normalmapTexture, "Normalmap");
		showTextureEdit(currentMaterial->glassTexture, "Glass");
		showTextureEdit(currentMaterial->metalicTexture, "Metalic");
		showTextureEdit(currentMaterial->roughnessTexture, "Roughness");
		showTextureEdit(currentMaterial->aoTexture, "Ambient Occlusion");
		showTextureEdit(currentMaterial->emissiveTexture, "Emissive");
	}

	void showMaterialParamsEdit()
	{
		ImGui::Text("Parameters");

		float* shininess = &(currentMaterial->shininess);
		if (ImGui::DragFloat("Shininess", shininess, 0.01f, 0.0f, 0.0f))
		{
			isMaterialModified = true;
		}

		float* transparency = &(currentMaterial->transparency);
		if (ImGui::DragFloat("Transparency", transparency, 0.01f, 0.0f, 0.0f))
		{
			isMaterialModified = true;
		}

		bool fixDisappearanceAlpha = (bool) currentMaterial->fixDisappearanceAlpha;
		if (ImGui::Checkbox("Fix disappearance alpha", &fixDisappearanceAlpha))
		{
			currentMaterial->fixDisappearanceAlpha = fixDisappearanceAlpha ? 1.0f : 0.0f;
			isMaterialModified = true;
		}

		float* alphaTestThreshold = &(currentMaterial->alphaTestThreshold);
		if (ImGui::DragFloat("Alpha test threshold", alphaTestThreshold, 0.01f, 0.0f, 0.0f))
		{
			isMaterialModified = true;
		}

		float* shadowmappingAlphaTestThreshold = &(currentMaterial->shadowmappingAlphaTestThreshold);
		if (ImGui::DragFloat("Alpha test threshold (shadowmap rendering)", shadowmappingAlphaTestThreshold, 0.01f, 0.0f, 0.0f))
		{
			isMaterialModified = true;
		}
	}

	bool materialEditorWindow()
	{
		if (currentMaterial == nullptr)
		{
			currentMaterial = currentStaticModel->getMaterial(currentMaterialIndex);
			originalMaterial = *currentMaterial;
		}

		isOpen = true;

		if (ImGui::Begin("Material editor", &isOpen))
		{
			ImGui::PushID(currentMaterial);

			showButtons();

			ImGui::Separator();

			showMaterialNameEdit();
			showShaderTypeCombo();

			ImGui::Separator();

			showColorsEdit();

			ImGui::Separator();

			showTexturesEdit();

			ImGui::Separator();

			showMaterialParamsEdit();

			ImGui::PopID();
		}
		ImGui::End();

		if (isMaterialModified)
		{
			currentRenderObject->updateLocalMaterialFromModel(currentMaterialIndex, currentModelLod);
		}

		if (!isOpen)
		{
			cancelChangesInMaterial();
			currentMaterial = nullptr;
		}

		return isOpen;
	}
}
