const fs = require('fs')
const mustache = require('mustache');

const EXTENSION_CPP = ".cpp";
const EXTENSION_H = ".h";

function generateCMakeFile(directory) {
    let files = fs.readdirSync(directory);

    let sourceFiles = files.filter(file => file.endsWith(EXTENSION_CPP));
    let headerFiles = files.filter(file => file.endsWith(EXTENSION_H));

    const view = {
        sourceFiles: sourceFiles,
        headerFiles: headerFiles
    }

    //console.log(sourceFiles);
    //console.log(headerFiles);
    
    const template = fs.readFileSync("templates/CMakeListsTemplate.txt").toString();

    const data = mustache.render(template, view);

    fs.writeFileSync(directory + "CMakeLists.txt", data);
}

module.exports = { generateCMakeFile }
