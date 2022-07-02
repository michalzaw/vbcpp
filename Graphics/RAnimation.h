#ifndef RANIMATION_H_INCLUDED
#define RANIMATION_H_INCLUDED


#include <vector>

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
		int	_id; // todo: animation - prawdopodobnie niepotrzebne
		std::string _name;
		glm::mat4 _localTransform;

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
		Bone(int id, const std::string& name);
		~Bone();

		inline const std::string& getName() { return _name; }
		inline const glm::mat4 getLocalTransform() { return _localTransform; }

		void update(float animationTime);

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

		Bone* findBone(const std::string& name); // todo: animation u¿ycie mapy z Bone zamiast vectora

		inline float getDuration() { return _duration; }
		inline float getTicksPerSecond() { return _ticksPerSecond; }

		inline AnimationNodeData* getRootNode() { return &_rootNode; }

};


#endif // RANIMATION_H_INCLUDED
