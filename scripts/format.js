var shell = require("shelljs"),
    Procdraw = require("./procdraw.js"),
    procdraw = new Procdraw(shell);

shell.config.fatal = true;

procdraw.clangFormat(procdraw.srcFiles());
