const { findFunctions, findFunctionsToResolve, createFunctionsMap, createFunctionsMapAsObject } = require('./functions');
const { findBaseClass } = require('./classes');
const { generateCMakeFile } = require('./cmakeGenerator');
const { generateLuaBindingsFile } = require('./allBindingsFileGenerator')
const fs = require('fs')
const mustache = require('mustache');

function createMoustacheView(className, fileName, functionsMap, functionsToResolve, baseClass) {
    const view = {
        className: className,
        fileName: fileName,
        baseClasses: [],
        functions: [],
        overloadedFunctions: [],
        functionsToResolve: functionsToResolve
    };

    if (baseClass !== null) {
        view.baseClasses.push(baseClass);
    }

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

    if (view.functions.length > 0 && view.overloadedFunctions.length === 0 && view.functionsToResolve.length === 0) {
        view.functions[view.functions.length - 1].last = true;
    }

    if (view.functionsToResolve.length > 0 && view.overloadedFunctions.length === 0) {
        view.functionsToResolve[view.functionsToResolve.length - 1].last = true;
    }

    if (view.overloadedFunctions.length > 0) {
        view.overloadedFunctions[view.overloadedFunctions.length - 1].last = true;
    }

    return view;
}

function generateHeaderFile(className) {
    const view = {
        className: className,
        classNameUpperCase: className.toUpperCase()
    };

    const template = fs.readFileSync("templates/LuaBindingTemplate.h").toString();
    
    const data = mustache.render(template, view);
    
    fs.writeFileSync("../../Scripting/Bindings/Generated/" + className + "LuaBindings.h", data);
}

function generateCppFile(className, classFileName, generateDocumentation) {

    const fileContent = fs.readFileSync("../../" + classFileName, "utf-8");
    let functions = findFunctions(fileContent);
    let functionsMap = createFunctionsMapAsObject(functions);

    let functionsToResolve = findFunctionsToResolve(fileContent);

    let baseClass = findBaseClass(fileContent);

    /*for (let f of functions) {
        console.log(f);
    }

    console.log("Mapa")
    console.log(functionsMap);
    console.log("Funkcje z resolve")
    console.log(functionsToResolve);*/

    functions[functions.length - 1].last = true;

    const view = createMoustacheView(className, classFileName, functionsMap, functionsToResolve, baseClass);
    const template = fs.readFileSync("templates/LuaBindingTemplate.cpp").toString();

    const data = mustache.render(template, view);

    fs.writeFileSync("../../Scripting/Bindings/Generated/" + className + "LuaBindings.cpp", data);

    if (generateDocumentation) {
        const template = fs.readFileSync("templates/LuaBindingsDocumentationTemplate.md").toString();
    
        const data = mustache.render(template, view);
    
        fs.writeFileSync("docs/" + className + "Docs.md", data);
    }
}

let classFileName = process.argv[2];
let className = process.argv[3];
let generateDocumentation = process.argv[4];

generateHeaderFile(className);
generateCppFile(className, classFileName, generateDocumentation);
generateLuaBindingsFile("../../Scripting/Bindings/Generated/");
generateCMakeFile("../../Scripting/Bindings/Generated/");
