#include "procdraw/utils/color.h"

namespace procdraw {

    // Foley and van Dam Fig. 13.34
    // h [0, 360)
    // s [0, 1]
    // v [0, 1]
    void Hsv2Rgb(float h, float s, float v, float &out_r, float &out_g, float &out_b)
    {
        out_r = 0.0f;
        out_g = 0.0f;
        out_b = 0.0f;

        if (s == 0) {
            out_r = v;
            out_g = v;
            out_b = v;
        }
        else {
            if (h == 360) {
                h = 0;
            }
            h = h / 60;
            int i = static_cast<int>(h);
            float f = h - i;
            float p = v * (1 - s);
            float q = v * (1 - (s * f));
            float t = v * (1 - (s * (1 - f)));
            switch (i) {
            case 0:
                out_r = v;
                out_g = t;
                out_b = p;
                break;
            case 1:
                out_r = q;
                out_g = v;
                out_b = p;
                break;
            case 2:
                out_r = p;
                out_g = v;
                out_b = t;
                break;
            case 3:
                out_r = p;
                out_g = q;
                out_b = v;
                break;
            case 4:
                out_r = t;
                out_g = p;
                out_b = v;
                break;
            case 5:
                out_r = v;
                out_g = p;
                out_b = q;
                break;
            }
        }
    }

}
