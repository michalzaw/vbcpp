#ifndef SKY_H_INCLUDED
#define SKY_H_INCLUDED


#include "Prefab.h"
#include "RenderObject.h"


class Sky : public Cube
{
private:


public:
	Sky(RTexture* texture)
		: Cube(1, new Material)
	{
		_material->diffuseTexture = texture;
		_material->shader = SKY_MATERIAL;

		_type = CT_SKY;

		init();

		setCastShadows(false);
	}

	~Sky()
	{

	}

};


#endif // SKY_H_INCLUDED
