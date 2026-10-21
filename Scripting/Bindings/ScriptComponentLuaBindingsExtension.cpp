#include "ScriptComponentLuaBindingsExtension.h"

#include "../../Scripting/ScriptComponent.h"


namespace ScriptComponentLuaBindingsExtension
{
    sol::object callOldIndex(sol::object oldIndex, sol::object object, sol::object key)
    {
        if (!oldIndex.valid())
        {
            return sol::nil;
        }

        if (oldIndex.get_type() == sol::type::function)
        {
            sol::function function = oldIndex.as<sol::function>();

            return function(object, key);
        }

        if (oldIndex.get_type() == sol::type::table)
        {
            sol::table table = oldIndex.as<sol::table>();

            return table[key];
        }
    }

    sol::object wrapLuaFunctionAndRemoveSelfArg(sol::object object, sol::state_view* lua)
    {
        if (object.is<sol::function>())
        {
            sol::function function = object.as<sol::function>();

            return sol::make_object(*lua, [function](sol::this_state ts, sol::variadic_args args)
                {
                    std::vector<sol::object> newArgs;

                    // Zaczynamy od drugiego argumentu - pomijamy self
                    for (auto i = args.begin() + 1; i != args.end(); ++i)
                    {
                        newArgs.push_back(*i);
                    }

                    return function.call(sol::as_args(newArgs));
                }
            );
        }

        return object;
    }

	void bind(sol::state* lua)
	{
        sol::usertype<ScriptComponent> scriptComponentUserType = (*lua)["ScriptComponent"];
        sol::table metatable = scriptComponentUserType[sol::metatable_key];
        sol::object oldIndex = metatable[sol::meta_function::index];

        auto customNewIndex = [](sol::object object, sol::object key, sol::object value)
        {
            sol::state_view lua(object.lua_state());

            ScriptComponent& scriptCompennt = object.as<ScriptComponent>();
            scriptCompennt.addCustomMember(key, wrapLuaFunctionAndRemoveSelfArg(value, &lua));
        };

        auto customIndex = [oldIndex](sol::object object, sol::object key) -> sol::object
        {
            ScriptComponent& scriptCompennt = object.as<ScriptComponent>();
            sol::object value = scriptCompennt.getCustomMember(key);

            if (value.valid() && value.get_type() != sol::type::lua_nil)
            {
                return value;
            }

            return callOldIndex(oldIndex, object, key);
        };

        scriptComponentUserType[sol::meta_function::new_index] = customNewIndex;
        scriptComponentUserType[sol::meta_function::index] = customIndex;
	}
}
