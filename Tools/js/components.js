const fs = require('fs');
const path = require('path');

const BASE_PATH = "../../";
const IGNORE_DIRECTORIES = [".git", ".github", "bin", "build", "libs", "Tools"];

function readFilesRecursively(directory, components, space = "") {
    const files = fs.readdirSync(directory, { withFileTypes: true });

    for (const file of files) {
        if (IGNORE_DIRECTORIES.includes(file.name)) {
            continue;
        }
        
        const fullPath = path.join(directory, file.name);

        if (file.isDirectory()) {
            //console.log(space + file.name);
            readFilesRecursively(fullPath, components, space + "  ");
        } else if (file.isFile()) {
            const fileContent = fs.readFileSync(fullPath, "utf-8");
            
            const regex = /VBCPP_COMPONENT\((\S+), (\S+)\)/g;

            let match;

            while (match = regex.exec(fileContent.toString())) {
                let className = match[1];
                let type = match[2];

                if (className !== "className" && type !== "type") {
                    components.push({
                        className: className,
                        type: type,
                        filePath: "../" + fullPath.replaceAll("\\", "/")
                    });
                }
            }
        }
    }
}

function findAllComponents() {
    let components = [];
    readFilesRecursively(BASE_PATH, components);
    
    return components;
}

module.exports = { findAllComponents }