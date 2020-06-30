#include "MaterialEditorWindow.h"

#include <vector>

#include <portable-file-dialogs.h>

#include "../..//ImGui/imgui.h"

#include "../../Graphics/Material.h"

#include "../../Utils/Logger.h"
#include "../../Utils/ResourceManager.h"


namespace vbEditor
{
	Material* currentMaterial = nullptr;

	RTexture* whiteTexture = nullptr;

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
			"PBR_MATERIAL",
			"TREE_MATERIAL",
			"GRASS_MATERIAL",
			"SKY_MATERIAL",
			"GLASS_MATERIAL"
		};

		if (ImGui::Combo("Interpolation", &typeComboCurrentItem, typeComboItems, IM_ARRAYSIZE(typeComboItems)))
		{
			currentMaterial->shader = (ShaderType) typeComboCurrentItem;
		}
	}

	void showColorsEdit()
	{
		ImGui::Text("Colors");

		ImGui::ColorEdit4("Ambient", (float*)& currentMaterial->ambientColor, ImGuiColorEditFlags_NoOptions);
		ImGui::ColorEdit4("Diffuse", (float*)& currentMaterial->diffuseColor, ImGuiColorEditFlags_NoOptions);
		ImGui::ColorEdit4("Specular", (float*)& currentMaterial->specularColor, ImGuiColorEditFlags_NoOptions);
		ImGui::ColorEdit4("Emissive", (float*)& currentMaterial->emissiveColor, ImGuiColorEditFlags_NoOptions);
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

		char buffer[1024] = { '\0' };

		if (texture != nullptr)
			strncpy(buffer, texture->getPath().c_str(), sizeof buffer);

		buffer[sizeof buffer - 1] = '\0';

		ImGui::Text(name);

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
				texture = ResourceManager::getInstance().loadTexture(result[0]);

				Logger::info(result[0]);
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
		ImGui::DragFloat("Shininess", shininess, 0.01f, 0.0f, 0.0f);

		float* transparency = &(currentMaterial->transparency);
		ImGui::DragFloat("Transparency", transparency, 0.01f, 0.0f, 0.0f);

		bool fixDisappearanceAlpha = (bool) currentMaterial->fixDisappearanceAlpha;
		if (ImGui::Checkbox("Fix disappearance alpha", &fixDisappearanceAlpha))
		{
			currentMaterial->fixDisappearanceAlpha = fixDisappearanceAlpha ? 1.0f : 0.0f;
		}
	}

	bool materialEditorWindow()
	{
		bool isOpen = true;

		if (ImGui::Begin("Material editor", &isOpen))
		{
			showMaterialNameEdit();
			showShaderTypeCombo();

			ImGui::Separator();

			showColorsEdit();

			ImGui::Separator();

			showTexturesEdit();

			ImGui::Separator();

			showMaterialParamsEdit();
		}
		ImGui::End();

		return isOpen;
	}
}
