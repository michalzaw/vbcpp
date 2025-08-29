const { findAllComponents } = require('./components')
const fs = require('fs');
const mustache = require('mustache');

let components = findAllComponents();

components[components.length - 1].last = true;

//console.log(components);

const template = fs.readFileSync("templates/ComponentsGettersTemplate.h").toString();
const view = {
    components: components
}

const data = mustache.render(template, view);

fs.writeFileSync("../../Scripting/Bindings/Generated/ComponentsGetters.h", data);
