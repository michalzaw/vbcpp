#include "MathFunctionsLuaBindings.h"

#include "../../Utils/Helpers.hpp"
#include "../../Utils/Math.h"


namespace MathFunctionsLuaBindings
{
	void bind(sol::state* lua)
	{
		(*lua)["math"]["lerp"] = sol::resolve<float(float, float, float)>(&lerp);

		(*lua)["math"]["mapRange"] = &mapRange;

		(*lua)["math"]["clamp"] = sol::overload(
			&clampValue<unsigned int>,
			&clampValue<int>,
			&clampValue<float>,
			&clampValue<double>
		);
	}
}
