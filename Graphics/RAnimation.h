#ifndef RANIMATION_H_INCLUDED
#define RANIMATION_H_INCLUDED


#include <vector>

#include <glm/glm.hpp>

#include "../Utils/Resource.h"


template <typename TYPE>
struct KeyValue final
{
	TYPE value;
	float timestamp;

};


typedef KeyValue<glm::vec3> KeyPosition;
typedef KeyValue<glm::quat> KeyRotation;
typedef KeyValue<glm::vec3> KeyScale;


class Bone final
{
	friend class AnimationLoader;

	private:
		int	_id;
		std::string _name;
		glm::mat4 _localTransform;

		std::vector<KeyPosition> _positions;
		std::vector<KeyRotation> _rotations;
		std::vector<KeyScale>	 _scales;

	public:
		Bone(int id, const std::string& name);
		~Bone();

		void update();

};


struct AnimationNodeData final
{
	glm::mat4 transformation;
	std::string name;
	std::vector<AnimationNodeData> children;

};


class RAnimation final : public Resource
{
	friend class AnimationLoader;

	private:
		float _duration;
		int _ticksPerSecond;

		std::vector<Bone> _bones;
		AnimationNodeData _rootNode;

	public:
		RAnimation(const std::string& path);
		~RAnimation();

};


#endif // RANIMATION_H_INCLUDED
