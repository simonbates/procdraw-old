var Procdraw = function (shell) {
    this.shell = shell;
};

Procdraw.prototype.srcFiles = function () {
    return this.shell.find("src", "tests").filter((filename) => {
        return filename.endsWith(".cpp") || filename.endsWith(".h");
    });
};

Procdraw.prototype.functionSpecs = function () {
    return this.shell.find("Documentation/docs/functions").filter((filenname) => {
        return filenname.endsWith(".xml");
    });
};

Procdraw.prototype.cpplint = function (files) {
    this.shell.exec("python third_party/google/styleguide/cpplint/cpplint.py "
        + files.join(" "));
};

Procdraw.prototype.jing = function (schema, files) {
    this.shell.exec("jing -c " + schema + " " + files.join(" "));
};

Procdraw.prototype.clangFormat = function (files) {
    this.shell.exec("clang-format -i -style=file " + files.join(" "));
};

module.exports = Procdraw;
