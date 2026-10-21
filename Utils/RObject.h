#ifndef ROBJECT_H_INCLUDED
#define ROBJECT_H_INCLUDED


#include <vector>
#include <unordered_map>

#include "Resource.h"


class RObject : public Resource
{
	private:
		std::string _author;
		std::string _name;
		std::string _comment;

		std::string _originalName;	// path relative to "Objects" directory

		glm::vec3 _position;	// filed only for children objects, not set in root object
		glm::vec3 _rotation; // filed only for children objects, not set in root object

		std::vector<std::unordered_map<std::string, std::string>> _components;

		std::vector<RObject*> _children;

	public:
		RObject(const std::string& path, const std::string& author, const std::string& name, const std::string& comment, const std::string& originalName);
		~RObject();

		void setPosition(const glm::vec3& position);
		void setRotation(const glm::vec3& rotation);

		const glm::vec3& getPosition();
		const glm::vec3& getRotation();

		std::vector<std::unordered_map<std::string, std::string>>& getComponents();

		std::vector<RObject*>& getChildren();

		const std::string& getAuthor();
		const std::string& getName();
		const std::string& getComment();
		const std::string& getOriginalName();
};


#endif // ROBJECT_H_INCLUDED
