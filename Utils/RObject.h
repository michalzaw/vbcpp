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

		std::vector<std::unordered_map<std::string, std::string>> _components;

	public:
		RObject(const std::string& path, const std::string& author, const std::string& name, const std::string& comment, const std::string& originalName);

		std::vector<std::unordered_map<std::string, std::string>>& getComponents();

		const std::string& getAuthor();
		const std::string& getName();
		const std::string& getComment();
		const std::string& getOriginalName();
};


#endif // ROBJECT_H_INCLUDED
