function findBaseClass(fileContent) {
    const regex = /class(.*)\:(\s*)public(\s*)(\S+)(\s*)\{/g;

    let match = regex.exec(fileContent.toString());
    if (match) {
        return match[4];
    }

    return null;
}

module.exports = { findBaseClass }
