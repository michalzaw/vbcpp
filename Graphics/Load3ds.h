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


class Load3ds
{
    private:
        OGLDriver* _OGLDriver;
        Lib3dsFile* _file3ds;

        float _minCreaseCosAngle;

        Material loadMaterialData(Lib3dsMaterial* material, std::string texPath);
        void loadGeometryByMaterial(Material& material, std::vector<unsigned int>& indices);
        void loadCollisionMesh(std::vector<glm::vec3>* vertices);

    public:
        Load3ds(OGLDriver* driver);
        ~Load3ds();

        void setMinCreaseCosAngle(float CosAngle);

        //Model* loadModel(std::string fileName, std::string texturesPath);
        Model* loadModel(std::string fileName, std::string texturesPath);
};



#endif // LOAD3DS_H_INCLUDED
