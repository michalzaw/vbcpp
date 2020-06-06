#include "LoadMaterial.h"

#include "../Game/GameConfig.h"

#include "../Utils/XmlUtils.h"


// XML MATERIAL FILE DEFINITIONS
const char* XML_MATERIAL_ROOT = "Materials";
const char* XML_MATERIAL_ELEMENT = "Material";
std::string matFilenamePostfix("_mat.xml");
// XML END


MaterialLoader::MaterialLoader()
    : _xmlFile(NULL)
{

}


MaterialLoader::~MaterialLoader()
{
    if (_xmlFile != NULL)
    {
        closeFile();
    }
}


void MaterialLoader::openFile(const char* fileName)
{
    _xmlFile = new XMLDocument;
    _xmlFile->LoadFile(fileName);
}


void MaterialLoader::closeFile()
{
    delete _xmlFile;
    _xmlFile = NULL;
}


Material MaterialLoader::loadMaterial(std::string materialName, std::string texPath)
{
    XMLElement* materialElement = NULL;
    XMLElement* root = _xmlFile->FirstChildElement(XML_MATERIAL_ROOT);

    for (XMLElement* child = root->FirstChildElement(XML_MATERIAL_ELEMENT); child != NULL; child = child->NextSiblingElement())
    {
        std::string elementName(child->Name());

        const char* name = child->Attribute("name");
        if (strcmp(name, materialName.c_str()) == 0)
        {
            materialElement = child;
            break;
        }
    }

    Material sMaterial;
    sMaterial.name = materialName;


    if (materialElement == NULL)
        return sMaterial;


    sMaterial.transparency = (float)atof(materialElement->Attribute("transparency"));

    std::cout << "Material name: " << sMaterial.name << std::endl;
    std::cout << "Material transparency: " << (float)sMaterial.transparency << std::endl;

    sMaterial.ambientColor = XMLstringToVec4(materialElement->Attribute("ambient"));
    sMaterial.diffuseColor = XMLstringToVec4(materialElement->Attribute("diffuse"));
    sMaterial.specularColor = XMLstringToVec4(materialElement->Attribute("specular"));

    sMaterial.shininess = (float)atof(materialElement->Attribute("shininess"));

	sMaterial.fixDisappearanceAlpha = (float)XmlUtils::getAttributeBoolOptional(materialElement, "fixDisappearanceAlpha");

    sMaterial.offset = XMLstringToVec2(materialElement->Attribute("offset"));
    sMaterial.scale = XMLstringToVec2(materialElement->Attribute("scale"));

    // make Texture Name lowercase
    std::string texStr = std::string(materialElement->Attribute("diffuse_texture"));

    for(unsigned int i = 0; i < texStr.size(); i++ )
        texStr[i] = tolower(texStr[i]);

    std::string texturePath = texPath + texStr;


    if(texStr != "")
        sMaterial.diffuseTexture = ResourceManager::getInstance().loadTexture(texturePath);


    // Normal mapa
    texStr = std::string(materialElement->Attribute("normalmap_texture"));

    std::cout << "NormalMap: " << texStr << std::endl;

    for(unsigned int i = 0; i < texStr.size(); i++ )
        texStr[i] = tolower(texStr[i]);

    texturePath = texPath + texStr;


    if(texStr != "")
        sMaterial.normalmapTexture = ResourceManager::getInstance().loadTexture(texturePath);


	sMaterial.metalicTexture = ResourceManager::getInstance().loadDefaultWhiteTexture();
	sMaterial.roughnessTexture = ResourceManager::getInstance().loadDefaultWhiteTexture();
	sMaterial.aoTexture = ResourceManager::getInstance().loadDefaultWhiteTexture();
    // glass texture - okresla ktora czesc szyby jest zewnetrzna - bialy, wewnetrzna - czarny. Wykorzystywane np w autobusie.
    const char* c = materialElement->Attribute("glass_texture");
    if (c != NULL)
    {
        texStr = std::string(c);

        std::cout << "GlassTexture: " << texStr << std::endl;

        for(unsigned int i = 0; i < texStr.size(); i++ )
            texStr[i] = tolower(texStr[i]);

        texturePath = texPath + texStr;


        if(texStr != "")
            sMaterial.glassTexture = ResourceManager::getInstance().loadTexture(texturePath);
    }

	const char* c1 = materialElement->Attribute("metallic_texture");
	if (c1 != NULL)
	{
		texStr = std::string(c1);

		std::cout << "MetalicTexture: " << texStr << std::endl;

		for (unsigned int i = 0; i < texStr.size(); i++)
			texStr[i] = tolower(texStr[i]);

		texturePath = texPath + texStr;


		if (texStr != "")
			sMaterial.metalicTexture = ResourceManager::getInstance().loadTexture(texturePath);
	}

	const char* c2 = materialElement->Attribute("roughness_texture");
	if (c2 != NULL)
	{
		texStr = std::string(c2);

		std::cout << "RoughnessTexture: " << texStr << std::endl;

		for (unsigned int i = 0; i < texStr.size(); i++)
			texStr[i] = tolower(texStr[i]);

		texturePath = texPath + texStr;


		if (texStr != "")
			sMaterial.roughnessTexture = ResourceManager::getInstance().loadTexture(texturePath);
	}

	const char* c3 = materialElement->Attribute("ao_texture");
	if (c3 != NULL)
	{
		texStr = std::string(c3);

		std::cout << "AoTexture: " << texStr << std::endl;

		for (unsigned int i = 0; i < texStr.size(); i++)
			texStr[i] = tolower(texStr[i]);

		texturePath = texPath + texStr;


		if (texStr != "")
			sMaterial.aoTexture = ResourceManager::getInstance().loadTexture(texturePath);
	}

    // reflection texture - global lub local, 1 dla czesci zewnetrznej, 2 dla czesci wewnetrznej szyby
    const char* reflectionTexture1 = materialElement->Attribute("reflection_texture_1");
    if (reflectionTexture1 != NULL)
    {
        std::string reflectionTexture1Str(reflectionTexture1);
        if (reflectionTexture1Str == "local")
            sMaterial.reflectionTexture1 = EMT_LOCAL;
        else if (reflectionTexture1Str == "global")
            sMaterial.reflectionTexture1 = EMT_GLOBAL;
    }
    const char* reflectionTexture2 = materialElement->Attribute("reflection_texture_2");
    if (reflectionTexture2 != NULL)
    {
        std::string reflectionTexture2Str(reflectionTexture2);
        if (reflectionTexture2Str == "local")
            sMaterial.reflectionTexture2 = EMT_LOCAL;
        else if (reflectionTexture2Str == "global")
            sMaterial.reflectionTexture2 = EMT_GLOBAL;
    }

    // mirror_name - name komponentu MirrorComonent
    const char* mirrorName = materialElement->Attribute("mirror_name");
    if (mirrorName != NULL)
    {
        sMaterial.mirrorName = std::string(mirrorName);
    }

    const char* type = materialElement->Attribute("type");

	if (strcmp(type, "normalmapping") == 0)
		sMaterial.shader = NORMALMAPPING_MATERIAL; //_OGLDriver->GetShader(NORMALMAPPING_MATERIAL);
	else if (strcmp(type, "solid") == 0)
		sMaterial.shader = SOLID_MATERIAL; //_OGLDriver->GetShader(SOLID_MATERIAL);
	else if (strcmp(type, "no_texture") == 0)
		sMaterial.shader = NOTEXTURE_MATERIAL; //_OGLDriver->GetShader(NOTEXTURE_MATERIAL);
	else if (strcmp(type, "tree") == 0)
		sMaterial.shader = TREE_MATERIAL;
	else if (strcmp(type, "alpha_test") == 0)
		sMaterial.shader = ALPHA_TEST_MATERIAL;
	else if (strcmp(type, "glass") == 0)
		sMaterial.shader = GLASS_MATERIAL;
	else if (strcmp(type, "grass") == 0)
		sMaterial.shader = GRASS_MATERIAL;
	else if (strcmp(type, "car_paint") == 0)
		sMaterial.shader = CAR_PAINT_MATERIAL;
	else if (strcmp(type, "mirror") == 0)
		sMaterial.shader = MIRROR_MATERIAL;
	else if (strcmp(type, "pbr") == 0)
		sMaterial.shader = PBR_MATERIAL;
	else if (strcmp(type, "new_tree") == 0)
		sMaterial.shader = NEW_TREE_MATERIAL;


    if (sMaterial.shader == GLASS_MATERIAL && sMaterial.glassTexture == NULL)
    {
        sMaterial.glassTexture = ResourceManager::getInstance().loadDefaultWhiteTexture();
    }

	if (sMaterial.shader == MIRROR_MATERIAL && !GameConfig::getInstance().isMirrorsEnabled)
	{
		sMaterial.shader = SOLID_MATERIAL;
		sMaterial.diffuseTexture = ResourceManager::getInstance().loadDefaultWhiteTexture();
	}


    return sMaterial;
}
