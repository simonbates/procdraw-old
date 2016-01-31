#pragma once

#include <alsa/asoundlib.h>

namespace procdraw {

    class ProcdrawApp;

    class AlsaMidiClient {
    public:
        AlsaMidiClient();
        void Poll(ProcdrawApp *app);
    private:
        snd_seq_t *seq_;
        int portid_;
    };

}
