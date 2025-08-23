const { findFunctions, createFunctionsMap, createFunctionsMapAsObject } = require('./functions');
const { readFileSync, writeFileSync } = require('fs');
const mustache = require('mustache');

function createMoustacheView(functionsMap) {
    const view = {
        className: "RenderObject",
        functions: [],
        overloadedFunctions: []
    };

    for (let f of functionsMap) {
        if (f.functions.length === 1) {
            view.functions.push(f);
        } else {
            if (f.functions.length > 1) {
                f.functions[f.functions.length - 1].lastFunction = true;
            }

            view.overloadedFunctions.push(f);
        }
    }

    if (view.functions.length > 0 && view.overloadedFunctions.length === 0) {
        view.functions[view.functions.length - 1].last = true;
    }

    if (view.overloadedFunctions.length > 0) {
        view.overloadedFunctions[view.overloadedFunctions.length - 1].last = true;
    }

    return view;
}

let functions = findFunctions("../../Graphics/RenderObject.h")
let functionsMap = createFunctionsMapAsObject(functions);

for (let f of functions) {
    console.log(f);
}

console.log("Mapa")

console.log(functionsMap);

functions[functions.length - 1].last = true;

const view = createMoustacheView(functionsMap);

const template = readFileSync("templates/LuaBindingTemplate.cpp").toString();

const data = mustache.render(template, view);

writeFileSync("RenderObjectLuaBinding_generated.cpp", data);
