#include "RObject.h"


RObject::RObject(std::string path, std::string author, std::string name, std::string comment)
	: Resource(RT_OBJECT, path),
	_author(author), _name(name), _comment(comment)
{

}


std::vector<std::unordered_map<std::string, std::string>>& RObject::getComponents()
{
	return _components;
}


std::string RObject::getAuthor()
{
	return _author;
}


std::string RObject::getName()
{
	return _name;
}


std::string RObject::getComment()
{
	return _comment;
}
