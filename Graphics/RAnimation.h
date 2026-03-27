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

	public:
		Bone(const std::string& name);
		~Bone();

		inline const std::string& getName() { return _name; }

		glm::mat4 calculatePosition(float animationTime);
		glm::quat calculateRotationQuat(float animationTime);
		glm::mat4 calculateRotation(float animationTime);
		glm::mat4 calculateScale(float animationTime);

		glm::mat4 calculateLocalTransform(float animationTime);

};


class RAnimation final : public Resource
{
	friend class AnimationLoader;
	friend class AnimationMetadataLoader;

	private:
		float _originalDuration;
		int _startFrame;
		int _endFrame;
		int _ticksPerSecond;

		std::unordered_map<std::string, Bone*> _bones;
		AnimationNodeData _rootNode;

		std::vector<std::string> _boneNames;

	public:
		RAnimation(const std::string& path);
		~RAnimation();

		inline float getOriginalDuration() { return _originalDuration; }
		inline float getDuration() { return static_cast<float>(_endFrame - _startFrame); }
		inline int getStartFrame() { return _startFrame; }
		inline int getEndFrame() { return _endFrame; }
		inline int getTicksPerSecond() { return _ticksPerSecond; }

		inline std::unordered_map<std::string, Bone*>& getBones() { return _bones; }
		inline AnimationNodeData* getRootNode() { return &_rootNode; }

		const std::vector<std::string>& getBonesNames();

};


#endif // RANIMATION_H_INCLUDED
