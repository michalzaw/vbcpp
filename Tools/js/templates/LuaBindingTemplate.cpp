#include "{{ className }}LuaBindings.h"

#include "../../../{{{ fileName }}}"


namespace {{ className }}LuaBindings
{
    void bind(sol::state* lua)
    {
        lua->new_usertype<{{ className }}>("{{ className }}",
            {{ #baseClasses }}
            sol::base_classes, sol::bases<{{.}}>(),
            {{ /baseClasses }}

            {{ #functions }}
            "{{ name }}", &{{ className }}::{{ name }}{{^last}},{{/last}}
            {{ /functions }}

            {{ #functionsToResolve }}
            "{{ name }}", sol::resolve<{{ returnType }}({{ #args }}{{{ type }}}{{^lastArg}}, {{/lastArg}}{{ /args }})>(&{{ className }}::{{ name }}){{^last}},{{/last}}
            {{ /functionsToResolve }}

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
