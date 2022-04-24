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
        ~Cube();

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
        ~PlanePrefab();

        void setSize(const glm::vec2& size);
        const glm::vec2& getSize();

};


class CylinderPrefab final : public Prefab
{
    protected:
        float _radius;
        float _height;
        int _axis;      // 0 - x, 1 - y, 2 - z
        int _quality;
        Material* _material;

        RStaticModel* generateModel() override;

    public:
        CylinderPrefab(float radius, float height, Material* material);
        ~CylinderPrefab();

        void setRadius(float radius);
        float getRadius();
        void setHeight(float height);
        float getHeight();
        void setAxis(int axis);
        int getAxis();
        void setQuality(int quality);
        int getQuality();

};


class SpherePrefab final : public Prefab
{
    protected:
        float _radius;
        int _quality;
        Material* _material;

        RStaticModel* generateModel() override;

    public:
        SpherePrefab(float radius, Material* material);
        ~SpherePrefab();

        void setRadius(float radius);
        float getRadius();
        void setQuality(int quality);
        int getQuality();

};


#endif // PREFAB_H_INCLUDED
