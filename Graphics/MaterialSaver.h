#ifndef MATERIALSAVER_H_INCLUDED
#define MATERIALSAVER_H_INCLUDED


#include <string>
#include <vector>

#include "Material.h"


struct aiScene;


namespace MaterialSaver
{
	void saveMaterials(std::string fileName, std::vector<Material*>& materials, std::string texPath);
	void saveMaterialsFromAssimpModel(std::string fileName, const aiScene* _assimpScene);

}


#endif // MATERIALSAVER_H_INCLUDED
