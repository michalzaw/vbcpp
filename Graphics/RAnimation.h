#ifndef RANIMATION_H_INCLUDED
#define RANIMATION_H_INCLUDED


#include <vector>
#include <unordered_map>

#include <glm/glm.hpp>

#include "AnimationNodeData.h"

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
		std::string _name;

		std::vector<KeyPosition> _positions;
		std::vector<KeyRotation> _rotations;
		std::vector<KeyScale>	 _scales;

		float getTimeFactor(float lastKeyframeTimestamp, float nextKeyframeTimestamp, float animationTime);

		int getPositionIndex(float animationTime);
		int getRotationIndex(float animationTime);
		int getScaleIndex(float animationTime);

		glm::mat4 interpolatePosition(float animationTime);
		glm::mat4 interpolateRotation(float animationTime);
		glm::mat4 interpolateScale(float animationTime);

	public:
		Bone(const std::string& name);
		~Bone();

		inline const std::string& getName() { return _name; }

		glm::mat4 calculateLocalTransform(float animationTime);

};


class RAnimation final : public Resource
{
	friend class AnimationLoader;

	private:
		float _duration;
		int _ticksPerSecond;

		std::unordered_map<std::string, Bone*> _bones;
		AnimationNodeData _rootNode;

	public:
		RAnimation(const std::string& path);
		~RAnimation();

		inline float getDuration() { return _duration; }
		inline float getTicksPerSecond() { return _ticksPerSecond; }

		inline std::unordered_map<std::string, Bone*>& getBones() { return _bones; }
		inline AnimationNodeData* getRootNode() { return &_rootNode; }

};


#endif // RANIMATION_H_INCLUDED
