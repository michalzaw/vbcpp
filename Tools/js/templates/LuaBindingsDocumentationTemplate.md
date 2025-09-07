### `{{ className }}`

TODO

{{ #functions }}
{{ #functions }}
- `{{name}}({{ #args }}{{{ name }}}{{^lastArg}}, {{/lastArg}}{{ /args }})`

    Parametry:

    {{ #args }}
    - `{{{ name }}}` *({{{ type }}})*
    {{ /args }}

    Zwracana wartość:

    - *{{ returnType }}*

{{ /functions }}
{{ /functions }}

{{ #functionsToResolve }}
- `{{name}}({{ #args }}{{{ name }}}{{^lastArg}}, {{/lastArg}}{{ /args }})`

    Parametry:

    {{ #args }}
    - `{{{ name }}}` *({{{ type }}})*
    {{ /args }}

    Zwracana wartość:

    - *{{ returnType }}*
    
{{ /functionsToResolve }}

{{ #overloadedFunctions }}
{{ #functions }}
- `{{name}}({{ #args }}{{{ name }}}{{^lastArg}}, {{/lastArg}}{{ /args }})`

    Parametry:

    {{ #args }}
    - `{{{ name }}}` *({{{ type }}})*
    {{ /args }}

    Zwracana wartość:

    - *{{ returnType }}*
    
{{ /functions }}
{{ /overloadedFunctions }}

{{ #fields }}
- `{{name}}`
    
{{ /fields }}
