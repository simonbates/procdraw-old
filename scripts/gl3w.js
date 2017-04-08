var shell = require("shelljs");

shell.config.fatal = true;

shell.mkdir("-p", "third_party/gl3w_out");
shell.cd("third_party/gl3w_out");
shell.exec("python ../gl3w/gl3w_gen.py");
