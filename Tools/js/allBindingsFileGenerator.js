const fs = require('fs')
const mustache = require('mustache');

const EXTENSION_H = ".h";
const IGNORE = "GeneratedLuaBindings";

function generateLuaBindingsFile(directory) {
    let files = fs.readdirSync(directory);

    let headerFiles = files
        .filter(file => file.endsWith(EXTENSION_H) && !file.startsWith(IGNORE))
        .map(file => file.substring(0, file.length - 2));

    const view = {
        headerFiles: headerFiles
    }

    //console.log(headerFiles);
    
    const template = fs.readFileSync("templates/GeneratedLuaBindingTemplate.cpp").toString();

    const data = mustache.render(template, view);

    fs.writeFileSync(directory + "GeneratedLuaBinding.cpp", data);
}

module.exports = { generateLuaBindingsFile }
