#include <FastLED.h>

class WS2812Protocol : public Protocol {
public:

    WS2812Protocol(WiFiClient &c) : Protocol("ws2812", c), current(0) {}

    ~WS2812Protocol() {
        if (leds) {
            free(leds);
        }
    }

    bool begin() {
        nleds = client.parseInt();
        if (nleds < 1 || nleds > 255) {
            return false;
        }

        leds = (CRGB*)malloc(nleds * sizeof(CRGB));

        FastLED.addLeds<NEOPIXEL, D6>(leds, nleds);
        for (int i = 0; i < nleds; i++) {
            leds[i] = CRGB::Black;
        }
        FastLED.show();
        return true;
    }

    void go() {
        if (!client.available()) {
            return;
        }

        char c = client.read();

        switch (c) {
        case -1:
            return;
        case 'l': current = client.parseInt(); break;
        case 'r': leds[current].r = client.parseInt(); break;
        case 'g': leds[current].g = client.parseInt(); break;
        case 'b': leds[current].b = client.parseInt(); break;
        case 's': FastLED.show(); break;
        }
    }
private:
    CRGB *leds;
    int nleds;
    int current;
};

Protocol* newWS2812Protocol(WiFiClient client) {
    return new WS2812Protocol(client);
}
