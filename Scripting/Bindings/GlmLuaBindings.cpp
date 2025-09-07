#include "GlmLuaBindings.h"

#include <glm/glm.hpp>


namespace GlmLuaBindings
{
	void createVec2Bindings(sol::state* lua)
	{
		auto vec2AdditionFunctions = sol::overload(
			[](const glm::vec2& v1, const glm::vec2& v2) { return v1 + v2; },
			[](const glm::vec2& vector, float value) { return vector + value; },
			[](float value, const glm::vec2& vector) { return value + vector; }
		);

		auto vec2SubtractionFunctions = sol::overload(
			[](const glm::vec2& v1, const glm::vec2& v2) { return v1 - v2; },
			[](const glm::vec2& vector, float value) { return vector - value; },
			[](float value, const glm::vec2& vector) { return value - vector; }
		);

		auto vec2MultiplicationFunctions = sol::overload(
			[](const glm::vec2& v1, const glm::vec2& v2) { return v1 * v2; },
			[](const glm::vec2& vector, float value) { return vector * value; },
			[](float value, const glm::vec2& vector) { return value * vector; }
		);

		auto vec2DivisionFunctions = sol::overload(
			[](const glm::vec2& v1, const glm::vec2& v2) { return v1 / v2; },
			[](const glm::vec2& vector, float value) { return vector / value; },
			[](float value, const glm::vec2& vector) { return value / vector; }
		);


		lua->new_usertype<glm::vec2>("vec2",
			sol::call_constructor,
			sol::constructors<glm::vec2(float, float)>(),
			"x", &glm::vec2::x,
			"y", &glm::vec2::y,
			sol::meta_function::addition, vec2AdditionFunctions,
			sol::meta_function::subtraction, vec2SubtractionFunctions,
			sol::meta_function::multiplication, vec2MultiplicationFunctions,
			sol::meta_function::division, vec2DivisionFunctions
		);
	}

	void createVec3Bindings(sol::state* lua)
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
			sol::call_constructor,
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
	}

	void createVec4Bindings(sol::state* lua)
	{
		auto vec4AdditionFunctions = sol::overload(
			[](const glm::vec4& v1, const glm::vec4& v2) { return v1 + v2; },
			[](const glm::vec4& vector, float value) { return vector + value; },
			[](float value, const glm::vec4& vector) { return value + vector; }
		);

		auto vec4SubtractionFunctions = sol::overload(
			[](const glm::vec4& v1, const glm::vec4& v2) { return v1 - v2; },
			[](const glm::vec4& vector, float value) { return vector - value; },
			[](float value, const glm::vec4& vector) { return value - vector; }
		);

		auto vec4MultiplicationFunctions = sol::overload(
			[](const glm::vec4& v1, const glm::vec4& v2) { return v1 * v2; },
			[](const glm::vec4& vector, float value) { return vector * value; },
			[](float value, const glm::vec4& vector) { return value * vector; }
		);

		auto vec4DivisionFunctions = sol::overload(
			[](const glm::vec4& v1, const glm::vec4& v2) { return v1 / v2; },
			[](const glm::vec4& vector, float value) { return vector / value; },
			[](float value, const glm::vec4& vector) { return value / vector; }
		);


		lua->new_usertype<glm::vec4>("vec4",
			sol::call_constructor,
			sol::constructors<glm::vec4(float, float, float, float)>(),
			"x", &glm::vec4::x,
			"y", &glm::vec4::y,
			"z", &glm::vec4::z,
			"w", &glm::vec4::w,
			"r", &glm::vec4::r,
			"g", &glm::vec4::g,
			"b", &glm::vec4::b,
			"a", &glm::vec4::a,
			sol::meta_function::addition, vec4AdditionFunctions,
			sol::meta_function::subtraction, vec4SubtractionFunctions,
			sol::meta_function::multiplication, vec4MultiplicationFunctions,
			sol::meta_function::division, vec4DivisionFunctions
		);
	}

	void createGlobalMathFunctions(sol::state* lua)
	{
		(*lua)["dot"] = sol::overload(
			sol::resolve<float(glm::vec2 const&, glm::vec2 const&)>(&glm::dot),
			sol::resolve<float(glm::vec3 const&, glm::vec3 const&)>(&glm::dot),
			sol::resolve<float(glm::vec4 const&, glm::vec4 const&)>(&glm::dot)
		);

		(*lua)["cross"] = sol::resolve<glm::vec3(glm::vec3 const&, glm::vec3 const&)>(&glm::cross);

		(*lua)["length"] = sol::overload(
			sol::resolve<float(glm::vec2 const&)>(&glm::length),
			sol::resolve<float(glm::vec3 const&)>(&glm::length),
			sol::resolve<float(glm::vec4 const&)>(&glm::length)
		);

		(*lua)["distance"] = sol::overload(
			sol::resolve<float(glm::vec2 const&, glm::vec2 const&)>(&glm::distance),
			sol::resolve<float(glm::vec3 const&, glm::vec3 const&)>(&glm::distance),
			sol::resolve<float(glm::vec4 const&, glm::vec4 const&)>(&glm::distance)
		);

		(*lua)["normalize"] = sol::overload(
			sol::resolve<glm::vec2(glm::vec2 const&)>(&glm::normalize),
			sol::resolve<glm::vec3(glm::vec3 const&)>(&glm::normalize),
			sol::resolve<glm::vec4(glm::vec4 const&)>(&glm::normalize)
		);
	}

	void bind(sol::state* lua)
	{
		createVec2Bindings(lua);
		createVec3Bindings(lua);
		createVec4Bindings(lua);

		createGlobalMathFunctions(lua);
	}
}
