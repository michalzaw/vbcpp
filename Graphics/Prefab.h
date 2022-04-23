#ifndef PREFAB_H_INCLUDED
#define PREFAB_H_INCLUDED


#include "RenderObject.h"


enum class PrefabType
{
    PLANE,
    CUBE,
    SPHERE,
    CYLINDER

};


class Prefab : public RenderObject
{
    protected:
        PrefabType _prefabType;

        RStaticModel* _generatedModel;

        virtual RStaticModel* generateModel() = 0;

    public:
        Prefab(PrefabType type);
        virtual ~Prefab();

        PrefabType getPrefabType();

        void init();

};


class Cube : public Prefab
{
    protected:
        float _size;
        Material* _material;

		RStaticModel* generateModel() override;

    public:
        Cube(float size, Material* material);

        void setSize(float size);
        float getSize();

};


class PlanePrefab final : public Prefab
{
	protected:
		glm::vec2 _size;
		Material* _material;

        RStaticModel* generateModel() override;

	public:
		PlanePrefab(glm::vec2 size, Material* material);

        void setSize(const glm::vec2& size);
        const glm::vec2& getSize();

};


#endif // PREFAB_H_INCLUDED
