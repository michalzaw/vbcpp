#include "LoadMaterial.h"


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




    // Alpha texture
    const char* alphaTextureName = materialElement->Attribute("alpha_texture");
    if (alphaTextureName != NULL)
    {
    texStr = std::string(alphaTextureName);

    std::cout << "Alpha texture: " << texStr << std::endl;

    for(unsigned int i = 0; i < texStr.size(); i++ )
        texStr[i] = tolower(texStr[i]);

    texturePath = texPath + texStr;


    if(texStr != "")
        sMaterial.alphaTexture = ResourceManager::getInstance().loadTexture(texturePath);
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
    else if (strcmp(type, "transparency") == 0)
        sMaterial.shader = TRANSPARENCY_MATERIAL;
    else if (strcmp(type, "grass") == 0)
        sMaterial.shader = GRASS_MATERIAL;

    return sMaterial;
}
