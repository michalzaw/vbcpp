#ifndef SKY_H_INCLUDED
#define SKY_H_INCLUDED


#include "Prefab.h"
#include "RenderObject.h"


class Sky : public Cube
{
private:


public:
	Sky(RTexture* texture)
		: Cube(1, Material())
	{
		_material.diffuseTexture = texture;
		_material.shader = SKY_MATERIAL;

		_type = CT_SKY;

		init();

		setIsCastShadows(false);
	}

	~Sky()
	{

	}

};


#endif // SKY_H_INCLUDED
