#include "MaterialSaver.h"

#include <assimp/scene.h>

#include "LoadMaterial.h"

#include "../Utils/Logger2.h"
#include "../Utils/FilesHelper.h"
#include "../Utils/tinyxml2.h"
using namespace tinyxml2;


std::string mapShaderTypeToString(ShaderType shaderType)
{
	switch (shaderType)
	{
		case SOLID_MATERIAL:
			return "solid";
		case NOTEXTURE_MATERIAL:
			return "no_texture";
		case NORMALMAPPING_MATERIAL:
			return "normalmapping";
		case SOLID_EMISSIVE_MATERIAL:
			return "solid_emissive";
		case CAR_PAINT_MATERIAL:
			return "car_paint";
		case MIRROR_MATERIAL:
			return "mirror";
		case ALPHA_TEST_MATERIAL:
			return "alpha_test";
		case NEW_TREE_MATERIAL:
			return "new_tree";
		case PBR_MATERIAL:
			return "pbr";
		case TREE_MATERIAL:
			return "tree";
		case GRASS_MATERIAL:
			return "grass";
		case GLASS_MATERIAL:
			return "glass";
		case PBR_TREE_MATERIAL:
			return "pbr_tree";
		case NEW_TREE_2_MATERIAL:
			return "new_tree2";
	}

	return "solid";
}


void saveMaterial(XMLElement* materialsElement, XMLDocument& doc, Material* material, std::string texPath)
{
	XMLElement* matElement = doc.NewElement(XML_MATERIAL_ELEMENT);

	std::string type = mapShaderTypeToString(material->shader);
	matElement->SetAttribute("type", type.c_str());

	matElement->SetAttribute("name", material->name.c_str());

	std::string ambientStr = vec4ToString(material->ambientColor);
	matElement->SetAttribute("ambient", ambientStr.c_str());

	std::string diffuseStr = vec4ToString(material->diffuseColor);
	matElement->SetAttribute("diffuse", diffuseStr.c_str());

	std::string specularStr = vec4ToString(material->specularColor);
	matElement->SetAttribute("specular", specularStr.c_str());

	if (material->diffuseTexture != nullptr && material->diffuseTexture->isLoadedFromFile())
	{
		std::string diffuseTexturePath = FilesHelper::getRelativePathToDirInGameDir(material->diffuseTexture->getPath(), texPath);
		matElement->SetAttribute("diffuse_texture", diffuseTexturePath.c_str());
	}

	if (material->normalmapTexture != nullptr && material->normalmapTexture->isLoadedFromFile())
	{
		std::string normalmapPath = FilesHelper::getRelativePathToDirInGameDir(material->normalmapTexture->getPath(), texPath);
		matElement->SetAttribute("normalmap_texture", normalmapPath.c_str());
	}

	if (material->glassTexture != nullptr && material->glassTexture->isLoadedFromFile())
	{
		std::string glassTexturePath = FilesHelper::getRelativePathToDirInGameDir(material->glassTexture->getPath(), texPath);
		matElement->SetAttribute("glass_texture", glassTexturePath.c_str());
	}
	
	if (material->metalicTexture != nullptr && material->metalicTexture->isLoadedFromFile())
	{
		std::string metalicTexturePath = FilesHelper::getRelativePathToDirInGameDir(material->metalicTexture->getPath(), texPath);
		matElement->SetAttribute("metallic_texture", metalicTexturePath.c_str());
	}

	if (material->roughnessTexture != nullptr && material->roughnessTexture->isLoadedFromFile())
	{
		std::string roughnessTexturePath = FilesHelper::getRelativePathToDirInGameDir(material->roughnessTexture->getPath(), texPath);
		matElement->SetAttribute("roughness_texture", roughnessTexturePath.c_str());
	}

	if (material->aoTexture != nullptr && material->aoTexture->isLoadedFromFile())
	{
		std::string aoTexturePath = FilesHelper::getRelativePathToDirInGameDir(material->aoTexture->getPath(), texPath);
		matElement->SetAttribute("ao_texture", aoTexturePath.c_str());
	}

	if (material->emissiveTexture != nullptr && material->emissiveTexture->isLoadedFromFile())
	{
		std::string emissiveTexturePath = FilesHelper::getRelativePathToDirInGameDir(material->emissiveTexture->getPath(), texPath);
		matElement->SetAttribute("emissive_texture", emissiveTexturePath.c_str());
	}

	std::string shininessStr = toString(material->shininess);
	matElement->SetAttribute("shininess", shininessStr.c_str());

	std::string transparencyStr = toString(material->transparency);
	matElement->SetAttribute("transparency", transparencyStr.c_str());

	std::string offsetStr = toString(material->offset.x) + "," + toString(material->offset.y);
	matElement->SetAttribute("offset", offsetStr.c_str());

	std::string scaleStr = toString(material->scale.x) + "," + toString(material->scale.y);
	matElement->SetAttribute("scale", scaleStr.c_str());

	if (material->shader == ALPHA_TEST_MATERIAL || material->shader == TREE_MATERIAL || material->shader == NEW_TREE_MATERIAL || material->shader == GRASS_MATERIAL)
	{
		std::string fixDisappearanceAlphaStr = toString(material->fixDisappearanceAlpha);
		matElement->SetAttribute("fixDisappearanceAlpha", fixDisappearanceAlphaStr.c_str());

		std::string alphaTestThresholdStr = toString(material->alphaTestThreshold);
		matElement->SetAttribute("alphaTestThreshold", alphaTestThresholdStr.c_str());

		std::string shadowmappingAlphaTestThresholdStr = toString(material->shadowmappingAlphaTestThreshold);
		matElement->SetAttribute("shadowmappingAlphaTestThreshold", shadowmappingAlphaTestThresholdStr.c_str());
	}

	if (material->shader == GLASS_MATERIAL)
	{
		std::string reflectionTexture1Str = material->reflectionTexture1 == EMT_LOCAL ? "local" : "global";
		matElement->SetAttribute("reflection_texture_1", reflectionTexture1Str.c_str());

		std::string reflectionTexture2Str = material->reflectionTexture2 == EMT_LOCAL ? "local" : "global";
		matElement->SetAttribute("reflection_texture_2", reflectionTexture2Str.c_str());
	}

	if (material->shader == MIRROR_MATERIAL && material->mirrorName != "")
	{
		matElement->SetAttribute("mirror_name", material->mirrorName.c_str());
	}

	if (material->requireSeparateInstance)
	{
		matElement->SetAttribute("requireSeparateInstance", "true");
	}

	materialsElement->InsertEndChild(matElement);
}

namespace MaterialSaver
{
	void saveMaterials(std::string fileName, std::vector<Material*>& materials, std::string texPath)
	{
		LOG_INFO("Saving materials. Path: " + fileName);
		XMLDocument doc;

		XMLDeclaration* declaration = doc.NewDeclaration();
		doc.InsertFirstChild(declaration);

		XMLElement* rootNode = doc.NewElement(XML_MATERIAL_ROOT);
		doc.InsertEndChild(rootNode);

		for (int i = 0; i < materials.size(); ++i)
		{
			saveMaterial(rootNode, doc, materials[i], texPath);
		}

		XMLError err = doc.SaveFile(fileName.c_str());

		LOG_INFO("Saved file with code: " + err);
	}


	void saveMaterialsFromAssimpModel(std::string fileName, const aiScene* assimpScene)
	{
		if (assimpScene == NULL)
			return;

		XMLDocument doc;

		XMLDeclaration* dec = doc.NewDeclaration();
		doc.InsertFirstChild(dec);

		XMLNode* root = doc.NewElement(XML_MATERIAL_ROOT);

		for (int i = 0; i < assimpScene->mNumMaterials; ++i)
		{
			const aiMaterial* material = assimpScene->mMaterials[i];

			aiColor4D ambientColor;
			aiColor4D diffuseColor;
			aiColor4D specularColor;
			float opacity;
			float shinines;
			aiString diffuseTexturePath;
			aiString normalmapPath;
			aiString alphaTexturePath;

			material->Get(AI_MATKEY_COLOR_AMBIENT, ambientColor);
			material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
			material->Get(AI_MATKEY_COLOR_SPECULAR, specularColor);
			material->Get(AI_MATKEY_OPACITY, opacity);
			material->Get(AI_MATKEY_SHININESS, shinines);

			bool isDiffuseTextureExist = material->GetTexture(aiTextureType_DIFFUSE, 0, &diffuseTexturePath, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS;
			bool isNormalmapExist = material->GetTexture(aiTextureType_NORMALS, 0, &normalmapPath, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS;
			bool isAlphaTextureExist = material->GetTexture(aiTextureType_OPACITY, 0, &alphaTexturePath, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS;

			std::string type;
			if (isDiffuseTextureExist && isNormalmapExist)
				type = "normalmapping";
			else if (isDiffuseTextureExist && !isNormalmapExist)
				type = "solid";
			else
				type = "no_texture";

			XMLElement* matElement = doc.NewElement(XML_MATERIAL_ELEMENT);

			matElement->SetAttribute("type", type.c_str());

			aiString materialName;
			material->Get(AI_MATKEY_NAME, materialName);
			matElement->SetAttribute("name", materialName.C_Str());

			std::string ambientStr = toString(ambientColor.r) + "," + toString(ambientColor.g) + "," + toString(ambientColor.b) + "," + toString(ambientColor.a);
			matElement->SetAttribute("ambient", ambientStr.c_str());

			std::string diffuseStr = toString(diffuseColor.r) + "," + toString(diffuseColor.g) + "," + toString(diffuseColor.b) + "," + toString(diffuseColor.a);
			matElement->SetAttribute("diffuse", diffuseStr.c_str());

			std::string specularStr = toString(specularColor.r) + "," + toString(specularColor.g) + "," + toString(specularColor.b) + "," + toString(specularColor.a);
			matElement->SetAttribute("specular", specularStr.c_str());

			matElement->SetAttribute("diffuse_texture", diffuseTexturePath.C_Str());

			matElement->SetAttribute("normalmap_texture", normalmapPath.C_Str());

			matElement->SetAttribute("alpha_texture", alphaTexturePath.C_Str());

			std::string shininessStr = toString(shinines);
			matElement->SetAttribute("shininess", shininessStr.c_str());

			std::string transparencyStr = toString(opacity);
			matElement->SetAttribute("transparency", transparencyStr.c_str());

			std::string offsetStr = "0,0";//toString(material->texture1_map.offset[0]) + "," + toString(material->texture1_map.offset[1]);
			matElement->SetAttribute("offset", offsetStr.c_str());

			std::string scaleStr = "1,1";//toString(material->texture1_map.scale[0]) + "," + toString(material->texture1_map.scale[1]);
			matElement->SetAttribute("scale", scaleStr.c_str());

			root->InsertEndChild(matElement);
		}

		doc.InsertEndChild(root);

		doc.SaveFile(fileName.c_str());
	}
}
