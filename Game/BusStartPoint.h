#ifndef BUSSTARTPOINT_H_INCLUDED
#define BUSSTARTPOINT_H_INCLUDED


#include <string>

#include "../Scene/Component.h"


class BusStartPoint final : public Component
{
	private:
		std::string _name;

		// todo: add more params

	public:
		BusStartPoint(const std::string& name);

		inline void setName(const std::string& name) { _name = name; }

		inline const std::string& getName() { return _name; }

};


#endif // BUSSTARTPOINT_H_INCLUDED
