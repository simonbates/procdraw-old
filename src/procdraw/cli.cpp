#include "cli.h"
#include <readline/readline.h>
#include <iostream>

#define TOP_LEVEL_PROMPT "> "
#define CONTINUED_LINE_PROMPT ""

namespace procdraw {

    static CommandProcessor *cmdProcessor = NULL;

    static std::string cmd("");

    static void LineHandler(char *line)
    {
        // TODO support quit with ^D by testing for line == NULL
        // TODO if line is all white space, ignore it
        if (line != NULL) {
            cmd += "\n";
            cmd += line;
            free(line);
            switch (cmdProcessor->CheckCommand(cmd)) {
            case BalancedState::Balanced:
                std::cout << cmdProcessor->DoCommand(cmd) << std::endl;
                cmd = "";
                rl_set_prompt(TOP_LEVEL_PROMPT);
                break;
            case BalancedState::TooManyClosingParens:
                std::cout << "Too many closing parens" << std::endl;
                cmd = "";
                rl_set_prompt(TOP_LEVEL_PROMPT);
                break;
            case BalancedState::NotClosed:
                rl_set_prompt(CONTINUED_LINE_PROMPT);
                break;
            }
        }
    }

    CLI::CLI(CommandProcessor *p)
    {
        cmdProcessor = p;
        rl_callback_handler_install(TOP_LEVEL_PROMPT, LineHandler);
    }

    CLI::~CLI()
    {
        rl_callback_handler_remove();
    }

    void CLI::Poll()
    {
        fd_set fds;
        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 0;
        int rl_instream_no = fileno(rl_instream);

        bool more = true;
        while (more) {
            FD_ZERO(&fds);
            FD_SET(rl_instream_no, &fds);

            int ready = select(rl_instream_no + 1, &fds, NULL, NULL, &timeout);

            if (ready < 0) {
                perror("select");
                break;
            }

            if ((ready > 0) && FD_ISSET(rl_instream_no, &fds)) {
                rl_callback_read_char();
            }
            else {
                more = false;
            }
        }
    }

}
