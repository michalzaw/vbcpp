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

		std::vector<std::unordered_map<std::string, std::string>> _components;

	public:
		RObject(std::string path, std::string author, std::string name, std::string comment);

		std::vector<std::unordered_map<std::string, std::string>>& getComponents();

		std::string getAuthor();
		std::string getName();
		std::string getComment();
};


#endif // ROBJECT_H_INCLUDED
