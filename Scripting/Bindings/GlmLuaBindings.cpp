#include "GlmLuaBindings.h"

#include <glm/glm.hpp>


namespace GlmLuaBindings
{
	void bind(sol::state* lua)
	{
		auto vec3AdditionFunctions = sol::overload(
			[](const glm::vec3& v1, const glm::vec3& v2) { return v1 + v2; },
			[](const glm::vec3& vector, float value) { return vector + value; },
			[](float value, const glm::vec3& vector) { return value + vector; }
		);

		auto vec3SubtractionFunctions = sol::overload(
			[](const glm::vec3& v1, const glm::vec3& v2) { return v1 - v2; },
			[](const glm::vec3& vector, float value) { return vector - value; },
			[](float value, const glm::vec3& vector) { return value - vector; }
		);

		auto vec3MultiplicationFunctions = sol::overload(
			[](const glm::vec3& v1, const glm::vec3& v2) { return v1 * v2; },
			[](const glm::vec3& vector, float value) { return vector * value; },
			[](float value, const glm::vec3& vector) { return value * vector; }
		);

		auto vec3DivisionFunctions = sol::overload(
			[](const glm::vec3& v1, const glm::vec3& v2) { return v1 / v2; },
			[](const glm::vec3& vector, float value) { return vector / value; },
			[](float value, const glm::vec3& vector) { return value / vector; }
		);


		lua->new_usertype<glm::vec3>("vec3",
			sol::constructors<glm::vec3(float, float, float)>(),
			"x", &glm::vec3::x,
			"y", &glm::vec3::y,
			"z", &glm::vec3::z,
			"r", &glm::vec3::r,
			"g", &glm::vec3::g,
			"b", &glm::vec3::b,
			sol::meta_function::addition, vec3AdditionFunctions,
			sol::meta_function::subtraction, vec3SubtractionFunctions,
			sol::meta_function::multiplication, vec3MultiplicationFunctions,
			sol::meta_function::division, vec3DivisionFunctions
		);

		(*lua)["dot"] = sol::resolve<float(glm::vec3 const &, glm::vec3 const &)>(&glm::dot);
		(*lua)["cross"] = sol::resolve<glm::vec3(glm::vec3 const &, glm::vec3 const &)>(&glm::cross);
		(*lua)["length"] = sol::resolve<float(glm::vec3 const &)>(&glm::length);
		(*lua)["distance"] = sol::resolve<float(glm::vec3 const&, glm::vec3 const&)>(&glm::distance);
		(*lua)["normalize"] = sol::resolve<glm::vec3(glm::vec3 const &)>(&glm::normalize);
	}
}
