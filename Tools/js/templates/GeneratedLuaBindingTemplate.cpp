#include "GeneratedLuaBindings.h"

{{ #headerFiles }}
#include "{{.}}.h"
{{ /headerFiles }}


namespace GeneratedLuaBindings
{
    void bind(sol::state* lua)
    {
        {{ #headerFiles }}
        {{.}}::bind(lua);
        {{ /headerFiles }}
    }
}
