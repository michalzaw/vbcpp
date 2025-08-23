#include "SceneObjectLuaBindings.h"

#include "../../Scene/SceneObject.h"


namespace {{ className }}LuaBindings
{
    void bind(sol::state* lua)
    {
        lua->new_usertype<{{ className }}>("{{ className }}",
            {{ #functions }}
            "{{ name }}", &{{ className }}::{{ name }}{{^last}},{{/last}}
            {{ /functions }}

            {{ #overloadedFunctions }}
            "{{ name }}", sol::overload(
                {{ #functions }}
                sol::resolve<{{ returnType }}({{ #args }}{{{ type }}}{{^lastArg}}, {{/lastArg}}{{ /args }})>(&{{ className }}::{{ name }}){{^lastFunction}},{{/lastFunction}}
                {{ /functions }}
            ){{^last}},{{/last}}
            {{ /overloadedFunctions }}
        );
    }
}
