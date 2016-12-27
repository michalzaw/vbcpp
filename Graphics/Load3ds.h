#ifndef LOAD3DS_H_INCLUDED
#define LOAD3DS_H_INCLUDED

#include <string>
#include <vector>
#include <cstring>
#include <iostream>

#include <lib3ds/file.h>
#include <lib3ds/material.h>
#include <lib3ds/mesh.h>

#include "Model.h"
#include "OGLDriver.h"
#include "NVMeshMender/NVMeshMender.h"
#include "LoadTexture.h"

#include "../Utils/tinyxml2.h"
#include "../Utils/Strings.h"
#include "../Utils/Helpers.hpp"

using namespace tinyxml2;


class Load3ds
{
    private:
        //OGLDriver* _OGLDriver;
        std::string _fileName;
        Lib3dsFile* _file3ds;

        float _minCreaseCosAngle;

        Material loadMaterialData(Lib3dsMaterial* material, std::string texPath);
        void loadGeometryByMaterial(Material& material, std::vector<unsigned int>& indices);
        void loadCollisionMesh(std::vector<glm::vec3>* vertices);

        bool isMaterialXmlFileExists();
        void saveMaterialsDataToXml(std::string fileName);
        Material loadMaterialDataFromXml(XMLDocument* xmlFile, std::string materialName, std::string texPath);

    public:
        Load3ds();
        ~Load3ds();

        void setMinCreaseCosAngle(float CosAngle);

        //Model* loadModel(std::string fileName, std::string texturesPath);
        Model* loadModel(std::string fileName, std::string texturesPath);
};



#endif // LOAD3DS_H_INCLUDED
