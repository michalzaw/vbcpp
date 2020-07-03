#ifndef MATERIALSAVER_H_INCLUDED
#define MATERIALSAVER_H_INCLUDED


#include <string>


struct Material;
struct aiScene;


namespace MaterialSaver
{
	void saveMaterials(std::string fileName, Material* materials, unsigned int materialsCount, std::string texPath);
	void saveMaterialsFromAssimpModel(std::string fileName, const aiScene* _assimpScene);

}


#endif // MATERIALSAVER_H_INCLUDED
