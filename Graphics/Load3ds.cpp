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
                vertex.t = mesh->texelL[i][1];
            }

            vertex.normal = Vector3(1.0f, 1.0f, 1.0f);
            vertex.tangent = Vector3(1.0f, 1.0f, 1.0f);
            vertex.binormal = Vector3(1.0f, 1.0f, 1.0f);

            vertices.push_back(vertex);
        }
	}

	Lib3dsMaterial* material;

	for (material = _file3ds->materials; material != NULL; material = material->next)
	{
	    Mesh m;

        m.material = loadMaterialData(material, texturesPath);

        m.firstVertex = indices.size();

        loadGeometryByMatrial(m.material, indices);

        m.quantumOfVertice = indices.size() - m.firstVertex;

        if (m.quantumOfVertice > 0)
            meshes.push_back(m);

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

	Model* model = new Model(_OGLDriver, vert, vertices.size(), ind, indices.size(), me, meshes.size());

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

	//sMaterial.offset[0] = material->texture1_map.offset[0];
	//sMaterial.offset[1] = material->texture1_map.offset[1];
	//sMaterial.scale[0] = material->texture1_map.scale[0];
	//sMaterial.scale[1] = material->texture1_map.scale[1];

	// make Texture Name lowercase
	std::string texStr = std::string(material->texture1_map.name);

	for( int i = 0; i < texStr.size(); i++ )
		texStr[i] = tolower(texStr[i]);

	std::string texturePath = texPath + texStr;

	GLuint texId = 0;

	if(texStr != "")
	{

        glGenTextures(1, &texId);
        glBindTexture(GL_TEXTURE_2D, texId);

        // Użycie SOIL jedynie do wczytania pliku graficznego
        // Tworzenie tekstury wykonywane recznie, gdyz wbudowane funkcje SOILa powoduja crash na Windowsie
		int width, height;
        unsigned char* image = SOIL_load_image(texturePath.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glGenerateMipmap(GL_TEXTURE_2D);

        SOIL_free_image_data(image);
	}
	sMaterial.diffuseTexture = texId;

	sMaterial.shader = _OGLDriver->GetShader(SOLID_MATERIAL);

	return sMaterial;
}


void Load3ds::loadGeometryByMatrial(Material& material, std::vector<unsigned int>& indices)
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
				    indices.push_back(face->points[currentVertex] + mesh->user.d);
				}
			}
		}
	}
}
