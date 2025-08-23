const fs = require('fs')

function parseArguments(argumentsStr) {
    let arguments = argumentsStr.split(",");

    arguments = arguments
        .map(arg => arg.trim())
        .map(arg => {
            let argTypeAndNameStr = arg.split("=")[0];
            argTypeAndNameStr = argTypeAndNameStr.trim();

            if (argTypeAndNameStr.length > 0) {
                let lastSpace = argTypeAndNameStr.lastIndexOf(" ");
            
                return {
                    type: argTypeAndNameStr.substring(0, lastSpace).trim(),
                    name: argTypeAndNameStr.substring(lastSpace, argTypeAndNameStr.length).trim()
                };
            } else {
                return null;
            }
        })
        .filter(arg => !!arg);

        if (arguments.length > 0) {
            arguments[arguments.length - 1].lastArg = true;
        }

    return arguments;
}

function findFunctions(fileName) {
    const code = fs.readFileSync(fileName, "utf-8");
    const regex = /(LUAF)(\s+)(.+)(\s+)(\S+)\((.*)\)/g;

    let match, functions = [];

    while (match = regex.exec(code.toString())) {
        functions.push({
            returnType: match[3],
            name: match[5],
            args: parseArguments(match[6])
        });
    }

    return functions;
}

function createFunctionsMap(functions) {
    let map = new Map();

    for (let f of functions) {
        if (!map.has(f.name)) {
            map.set(f.name, []);
        }

        map.get(f.name).push(f);
    }

    return map;
}

function createFunctionsMapAsObject(functions) {
    let map = createFunctionsMap(functions);
    let map2 = [];

    for (let [key, value] of  map.entries()) {
        map2.push({
            name: key,
            functions: value
        })
    }

    return map2;
}

module.exports = { findFunctions, createFunctionsMap, createFunctionsMapAsObject }
