#include "RObject.h"


RObject::RObject(const std::string& path, const std::string& author, const std::string& name, const std::string& comment, const std::string& originalName)
	: Resource(RT_OBJECT, path),
	_author(author), _name(name), _comment(comment),
	_originalName(originalName)
{

}


RObject::~RObject()
{
	for (int i = 0; i < _children.size(); ++i)
	{
		delete _children[i];
	}
}


void RObject::setPosition(const glm::vec3& position)
{
	_position = position;
}


void RObject::setRotation(const glm::vec3& rotation)
{
	_rotation = rotation;
}


const glm::vec3& RObject::getPosition()
{
	return _position;
}


const glm::vec3& RObject::getRotation()
{
	return _rotation;
}


std::vector<std::unordered_map<std::string, std::string>>& RObject::getComponents()
{
	return _components;
}


std::vector<RObject*>& RObject::getChildren()
{
	return _children;
}


const std::string& RObject::getAuthor()
{
	return _author;
}


const std::string& RObject::getName()
{
	return _name;
}


const std::string& RObject::getComment()
{
	return _comment;
}


const std::string& RObject::getOriginalName()
{
	return _originalName;
}
