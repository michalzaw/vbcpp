#include "Load3ds.h"


Load3ds::Load3ds(OGLDriver* driver)
    : _OGLDriver(driver), _minCreaseCosAngle(0.7f)
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

	printf("3ds mesh: %x\n", _file3ds);

	assert(_file3ds != NULL);

	std::vector<MeshMender::Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Mesh> meshes;


	Lib3dsMesh* mesh;
	Lib3dsFace* face;

	for(mesh = _file3ds->meshes; mesh != NULL; mesh = mesh->next)
	{
	    Lib3dsUserData user;
	    user.d = vertices.size();
	    mesh->user = user;

	    for (int i = 0; i < mesh->points; ++i)
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

	Lib3dsMaterial* material;

	for (material = _file3ds->materials; material != NULL; material = material->next)
	{
	    if (strcmp(material->name, "CollisionMes") != 0)
        {
            Mesh m;

            m.material = loadMaterialData(material, texturesPath);

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
	for (int i = 0; i < vertices.size(); ++i)
    {
        vert[i].Position = vertices[i].pos;
        vert[i].TexCoord = glm::vec2(vertices[i].s, vertices[i].t);
        vert[i].Normal = vertices[i].normal;
        vert[i].Tangent = vertices[i].tangent;
        vert[i].Bitangent = vertices[i].binormal;
    }

    unsigned int* ind = new unsigned int[indices.size()];
	for (int i = 0; i < indices.size(); ++i)
    {
        ind[i] = indices[i];
    }

    Mesh* me = new Mesh[meshes.size()];
	for (int i = 0; i < meshes.size(); ++i)
    {
        me[i] = meshes[i];
    }

    std::vector<glm::vec3> collisionMesh;
    loadCollisionMesh(&collisionMesh);
	glm::vec3* collisionMeshVertices = new glm::vec3[collisionMesh.size()];
	for (int i = 0; i < collisionMesh.size(); ++i)
    {
        collisionMeshVertices[i] = collisionMesh[i];
    }


	Model* model = new Model(_OGLDriver, vert, vertices.size(), ind, indices.size(), me, meshes.size(), collisionMeshVertices, collisionMesh.size());


	lib3ds_file_free(_file3ds);
    _file3ds = NULL;


	return model;
}

Material Load3ds::loadMaterialData(Lib3dsMaterial* material, std::string texPath)
{
	Material sMaterial;

	sMaterial.name = material->name;
	sMaterial.transparency = material->transparency;

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

	for( int i = 0; i < texStr.size(); i++ )
		texStr[i] = tolower(texStr[i]);

	std::string texturePath = texPath + texStr;

	GLuint texId = 0;

	if(texStr != "")
	{
        texId = LoadTexture(texturePath.c_str());
	}
	sMaterial.diffuseTexture = texId;


	// Normal mapa
	texStr = std::string(material->bump_map.name);

	for( int i = 0; i < texStr.size(); i++ )
		texStr[i] = tolower(texStr[i]);

	texturePath = texPath + texStr;

	texId = 0;

	if(texStr != "")
	{
        texId = LoadTexture(texturePath.c_str());
	}
	sMaterial.normalmapTexture = texId;


	if (sMaterial.diffuseTexture > 0)
        sMaterial.shader = _OGLDriver->GetShader(SOLID_MATERIAL);
    else
        sMaterial.shader = _OGLDriver->GetShader(NOTEXTURE_MATERIAL);

    if (sMaterial.normalmapTexture > 0)
        sMaterial.shader = _OGLDriver->GetShader(NORMALMAPPING_MATERIAL);

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

			if(faceMaterial == "CollisionMes")
			{
			    std::cout << "\n\n\n\n\n\nCollisionMesh\n\n\n\n\n\n\n";
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
