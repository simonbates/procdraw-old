var shell = require("shelljs"),
    Procdraw = require("./procdraw.js"),
    procdraw = new Procdraw(shell);

shell.config.fatal = true;

procdraw.jing("Documentation/docs/functions/function-spec.rnc",
    procdraw.functionSpecs());

procdraw.cpplint(procdraw.srcFiles());
