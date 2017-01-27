#include "Load3ds.h"

#include "LoadShader.h"

#include "../Utils/ResourceManager.h"

using namespace tinyxml2;


Load3ds::Load3ds()
    : _minCreaseCosAngle(0.7f)
{

}


Load3ds::~Load3ds()
{
    if ( _file3ds )
	{
		lib3ds_file_free(_file3ds);

		_file3ds = 0;
	}
}


void Load3ds::setMinCreaseCosAngle(float cosAngle)
{
    _minCreaseCosAngle = cosAngle;
}


Model* Load3ds::loadModel(std::string fileName, std::string texturesPath)
{
	_file3ds = lib3ds_file_load(fileName.c_str());

	assert(_file3ds != NULL);

    // Check for XML material file, create new one if it's not present
    /*std::string xmlFileName = fileName;
    xmlFileName.erase(xmlFileName.size() - 4, 4);
    xmlFileName += matFilenamePostfix;*/
    std::string xmlFileName = MaterialLoader::createMaterialFileName(fileName);


	if (!isMaterialXmlFileExists(xmlFileName))
    {
        saveMaterialsDataToXml(xmlFileName);
    }
    // --------------


	std::vector<MeshMender::Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Mesh> meshes;


	Lib3dsMesh* mesh;
	//Lib3dsFace* face;

	for(mesh = _file3ds->meshes; mesh != NULL; mesh = mesh->next)
	{
	    Lib3dsUserData user;
	    user.d = vertices.size();
	    mesh->user = user;

	    for (unsigned int i = 0; i < mesh->points; ++i)
        {
            MeshMender::Vertex vertex;
            //memcpy(&vertex.Position, mesh->pointL[i].pos, sizeof(float) * 3);
            vertex.pos.x = mesh->pointL[i].pos[0];
            vertex.pos.y = mesh->pointL[i].pos[1];
            vertex.pos.z = mesh->pointL[i].pos[2];

            //printf("Vertex: %f %f %f\n", vertex.pos.x, vertex.pos.y, vertex.pos.z);

            //memcpy(&vertex.TexCoord, mesh->texelL[i], sizeof(float) * 2);
            if (i < mesh->texels)
            {
                vertex.s = mesh->texelL[i][0];
                vertex.t = -mesh->texelL[i][1];
            }

            vertices.push_back(vertex);
        }
	}

	/*XMLDocument materialDoc;
	materialDoc.LoadFile(xmlFileName.c_str());*/
	MaterialLoader matLoader;
	matLoader.openFile(xmlFileName.c_str());

	Lib3dsMaterial* material;

	for (material = _file3ds->materials; material != NULL; material = material->next)
	{
	    //std::cout << "Material name: " << material->name << std::endl;

	    if (strcmp(material->name, "Collision") != 0)
        {
            Mesh m;

            m.material = matLoader.loadMaterial(material->name, texturesPath);//loadMaterialDataFromXml(&materialDoc, material->name, texturesPath);
            //m.material = loadMaterialData(material, texturesPath);

            m.firstVertex = indices.size();

            loadGeometryByMaterial(m.material, indices);

            m.quantumOfVertice = indices.size() - m.firstVertex;

            if (m.quantumOfVertice > 0)
                meshes.push_back(m);
        }
	}


	// Calculate normals with MeshMender
	MeshMender mender;
	std::vector<unsigned int> temp;
	mender.Mend(vertices, indices, temp, _minCreaseCosAngle, 0.7, 0.7, 1.0, MeshMender::CALCULATE_NORMALS, MeshMender::DONT_RESPECT_SPLITS, MeshMender::DONT_FIX_CYLINDRICAL);


	Vertex* vert = new Vertex[vertices.size()];
	for (unsigned int i = 0; i < vertices.size(); ++i)
    {
        vert[i].position = vertices[i].pos;
        vert[i].texCoord = glm::vec2(vertices[i].s, vertices[i].t);
        vert[i].normal = vertices[i].normal;
        vert[i].tangent = vertices[i].tangent;
        vert[i].bitangent = vertices[i].binormal;
    }

    unsigned int* ind = new unsigned int[indices.size()];
	for (unsigned int i = 0; i < indices.size(); ++i)
    {
        ind[i] = indices[i];
    }

    Mesh* me = new Mesh[meshes.size()];
	for (unsigned int i = 0; i < meshes.size(); ++i)
    {
        me[i] = meshes[i];
    }

    std::vector<glm::vec3> collisionMesh;
    loadCollisionMesh(&collisionMesh);
	glm::vec3* collisionMeshVertices = new glm::vec3[collisionMesh.size()];
	for (unsigned int i = 0; i < collisionMesh.size(); ++i)
    {
        collisionMeshVertices[i] = collisionMesh[i];
    }

    std::cout << "Collision mesh vertices: " << collisionMesh.size() << std::endl;


	Model* model = new Model(vert, vertices.size(), ind, indices.size(), me, meshes.size(), collisionMeshVertices, collisionMesh.size());


	lib3ds_file_free(_file3ds);
    _file3ds = NULL;


	return model;
}

Material Load3ds::loadMaterialData(Lib3dsMaterial* material, std::string texPath)
{
	Material sMaterial;

	sMaterial.name = material->name;
	sMaterial.transparency = material->transparency;

	std::cout << "Material name: " << material->name << std::endl;
    std::cout << "Material transparency: " << (float)material->transparency << std::endl;

	sMaterial.ambientColor[0] = material->ambient[0];
	sMaterial.ambientColor[1] = material->ambient[1];
	sMaterial.ambientColor[2] = material->ambient[2];
	sMaterial.ambientColor[3] = material->ambient[3];

	sMaterial.diffuseColor[0] = material->diffuse[0];
	sMaterial.diffuseColor[1] = material->diffuse[1];
	sMaterial.diffuseColor[2] = material->diffuse[2];
	sMaterial.diffuseColor[3] = material->diffuse[3];

	sMaterial.specularColor[0] = material->specular[0];
	sMaterial.specularColor[1] = material->specular[1];
	sMaterial.specularColor[2] = material->specular[2];
	sMaterial.specularColor[3] = material->specular[3];

	sMaterial.shininess = material->shininess;

	sMaterial.offset[0] = material->texture1_map.offset[0];
	sMaterial.offset[1] = material->texture1_map.offset[1];
	sMaterial.scale[0] = material->texture1_map.scale[0];
	sMaterial.scale[1] = material->texture1_map.scale[1];

	// make Texture Name lowercase
	std::string texStr = std::string(material->texture1_map.name);

	for(unsigned int i = 0; i < texStr.size(); i++ )
		texStr[i] = tolower(texStr[i]);

	std::string texturePath = texPath + texStr;


	if(texStr != "")
	    sMaterial.diffuseTexture = ResourceManager::getInstance().loadTexture(texturePath);


	// Normal mapa
	texStr = std::string(material->bump_map.name);

	std::cout << "NormalMap: " << texStr << std::endl;

	for(unsigned int i = 0; i < texStr.size(); i++ )
		texStr[i] = tolower(texStr[i]);

	texturePath = texPath + texStr;


	if(texStr != "")
        sMaterial.normalmapTexture = ResourceManager::getInstance().loadTexture(texturePath);


	if (sMaterial.diffuseTexture > 0 && sMaterial.normalmapTexture > 0)
        sMaterial.shader = NORMALMAPPING_MATERIAL; //_OGLDriver->GetShader(NORMALMAPPING_MATERIAL);
    else if (sMaterial.diffuseTexture > 0 && sMaterial.normalmapTexture == 0)
        sMaterial.shader = SOLID_MATERIAL; //_OGLDriver->GetShader(SOLID_MATERIAL);
    else
        sMaterial.shader = NOTEXTURE_MATERIAL; //_OGLDriver->GetShader(NOTEXTURE_MATERIAL);

	return sMaterial;
}


void Load3ds::loadGeometryByMaterial(Material& material, std::vector<unsigned int>& indices)
{
    Lib3dsMesh* mesh;
	Lib3dsFace* face;

    for(mesh = _file3ds->meshes; mesh != NULL; mesh = mesh->next)
	{
	    for(unsigned int currentFace = 0; currentFace < mesh->faces; currentFace++)
		{
			face = &mesh->faceL[currentFace];

			std::string faceMaterial = std::string(face->material);

			if(faceMaterial == material.name)
			{
			    for(int currentVertex = 0; currentVertex < 3; currentVertex++)
				{
				    //std::cout << "d: " << mesh->user.d << std::endl;
				    indices.push_back(face->points[currentVertex] + mesh->user.d);
				}
			}
		}
	}
}


void Load3ds::loadCollisionMesh(std::vector<glm::vec3>* vertices)
{
    Lib3dsMesh* mesh;
	Lib3dsFace* face;

    for(mesh = _file3ds->meshes; mesh != NULL; mesh = mesh->next)
	{
	    for(unsigned int currentFace = 0; currentFace < mesh->faces; currentFace++)
		{
			face = &mesh->faceL[currentFace];

			std::string faceMaterial = std::string(face->material);

			if(faceMaterial == "Collision")
			{
			    //std::cout << "\n\n\n\n\n\nCollisionMesh\n\n\n\n\n\n\n";
			    for(int currentVertex = 0; currentVertex < 3; currentVertex++)
				{
				    glm::vec3 vertex;
				    vertex.x = mesh->pointL[face->points[currentVertex]].pos[0];
				    vertex.y = mesh->pointL[face->points[currentVertex]].pos[1];
				    vertex.z = mesh->pointL[face->points[currentVertex]].pos[2];

				    vertices->push_back(vertex);
				}
			}
		}
	}
}


bool Load3ds::isMaterialXmlFileExists(std::string fileName)
{
	FILE* file;
    file = fopen(fileName.c_str(), "r");
    if (file)
    {
        fclose(file);
        return 1;
    }

    fclose(file);
    return 0;
}


void Load3ds::saveMaterialsDataToXml(std::string fileName)
{
    XMLDocument doc;

    XMLDeclaration* dec = doc.NewDeclaration();
    doc.InsertFirstChild(dec);

    XMLNode* root = doc.NewElement(XML_MATERIAL_ROOT);

    Lib3dsMaterial* material;

	for (material = _file3ds->materials; material != NULL; material = material->next)
    {
        std::string type;
        if (strcmp(material->texture1_map.name, "") != 0 && strcmp(material->bump_map.name, "") != 0)
            type = "normalmapping";
        else if (strcmp(material->texture1_map.name, "") != 0 && strcmp(material->bump_map.name, "") == 0)
            type = "solid";
        else
            type = "no_texture";


        XMLElement* matElement = doc.NewElement(XML_MATERIAL_ELEMENT);

        matElement->SetAttribute("type", type.c_str());

        matElement->SetAttribute("name", material->name);

        std::string ambientStr = toString(material->ambient[0]) + "," + toString(material->ambient[1]) + "," + toString(material->ambient[2]) + "," + toString(material->ambient[3]);
        matElement->SetAttribute("ambient", ambientStr.c_str());

        std::string diffuseStr = toString(material->diffuse[0]) + "," + toString(material->diffuse[1]) + "," + toString(material->diffuse[2]) + "," + toString(material->diffuse[3]);
        matElement->SetAttribute("diffuse", diffuseStr.c_str());

        std::string specularStr = toString(material->specular[0]) + "," + toString(material->specular[1]) + "," + toString(material->specular[2]) + "," + toString(material->specular[3]);
        matElement->SetAttribute("specular", specularStr.c_str());

        matElement->SetAttribute("diffuse_texture", material->texture1_map.name);

        matElement->SetAttribute("normalmap_texture", material->bump_map.name);

        std::string shininessStr = toString(material->shininess);
        matElement->SetAttribute("shininess", shininessStr.c_str());

        std::string transparencyStr = toString(material->transparency);
        matElement->SetAttribute("transparency", transparencyStr.c_str());

        std::string offsetStr = toString(material->texture1_map.offset[0]) + "," + toString(material->texture1_map.offset[1]);
        matElement->SetAttribute("offset", offsetStr.c_str());

        std::string scaleStr = toString(material->texture1_map.scale[0]) + "," + toString(material->texture1_map.scale[1]);
        matElement->SetAttribute("scale", scaleStr.c_str());

        root->InsertEndChild(matElement);
    }

    doc.InsertEndChild(root);

    doc.SaveFile(fileName.c_str());
}


/*Material Load3ds::loadMaterialDataFromXml(XMLDocument* xmlFile, std::string materialName, std::string texPath)
{
    XMLElement* materialElement = NULL;
    XMLElement* root = xmlFile->FirstChildElement(XML_MATERIAL_ROOT);

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


    const char* type = materialElement->Attribute("type");

	if (strcmp(type, "normalmapping") == 0)
        sMaterial.shader = NORMALMAPPING_MATERIAL; //_OGLDriver->GetShader(NORMALMAPPING_MATERIAL);
    else if (strcmp(type, "solid") == 0)
        sMaterial.shader = SOLID_MATERIAL; //_OGLDriver->GetShader(SOLID_MATERIAL);
    else if (strcmp(type, "no_texture") == 0)
        sMaterial.shader = NOTEXTURE_MATERIAL; //_OGLDriver->GetShader(NOTEXTURE_MATERIAL);
    else if (strcmp(type, "tree") == 0)
        sMaterial.shader = TREE_MATERIAL;

	return sMaterial;
}*/
