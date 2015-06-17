#pragma once

namespace procdraw {

    class ProcdrawApp;

    class CLI {
    public:
        CLI(ProcdrawApp *a);
        ~CLI();
        void Poll();
    };

}
