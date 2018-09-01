#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

//how many clients should be able to telnet to this ESP8266
#define MAX_SRV_CLIENTS 1
const char* ssid = "Spy Wireless IX";
const char* password = "monstercable";
const char* myname = "wipi";

WiFiServer server(23);

class Protocol {
public:
    Protocol(const char* n, WiFiClient &c) : name(n), client(c) {}
    virtual ~Protocol() {};

    virtual bool begin() {
        return true;
    }
    virtual void go() = 0;

    const char *name;

protected:
    WiFiClient client;
};

typedef void (*loopfun)(WiFiClient);

Protocol* newAnalogProtocol(WiFiClient);
Protocol* newSerialProtocol(WiFiClient);
Protocol* newOneWireProtocol(WiFiClient);
Protocol* newGPIOProtocol(WiFiClient);

struct {
    unsigned char initchar;
    Protocol* (*mkProtocol)(WiFiClient);
} protocols[] = {
                 {'s', newSerialProtocol},
                 {'a', newAnalogProtocol},
                 {'1', newOneWireProtocol},
                 {'g', newGPIOProtocol},
                 {'\0', 0},
};

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW); // low is on, night is day

    WiFi.mode(WIFI_STA);
    WiFi.hostname(myname);
    WiFi.begin(ssid, password);
    uint8_t i = 0;
    while (WiFi.status() != WL_CONNECTED) {
        digitalWrite(LED_BUILTIN, LOW);
        delay(500);
        digitalWrite(LED_BUILTIN, HIGH);
        delay(500);
    }

    MDNS.begin(myname);

    digitalWrite(LED_BUILTIN, HIGH); // light's off when we're connected

    server.begin();
    server.setNoDelay(true);
}

Protocol* selectProtocol(WiFiClient client) {
    while (client.connected() && client.available() < 1) {
        delay(1);
    }
    if (!client.connected()) {
        return NULL;
    }
    char c = client.read();

    for (int i = 0; protocols[i].mkProtocol; i++) {
        if (protocols[i].initchar == c) {
            return protocols[i].mkProtocol(client);
        }
    }
    client.write("# unhandled protocol\n");
    return NULL;
}

void loop() {
    digitalWrite(LED_BUILTIN, HIGH);
    if (server.hasClient()) {
        Protocol *prot(NULL);
        WiFiClient client = server.available();
        if (client.connected()) {
            client.write("# Hello.  Please select a protocol.\n");
            prot = selectProtocol(client);
            if (prot == NULL) {
                client.stop();
                return;
            }
            client.write("# found ");
            client.write(prot->name);
            client.println(" protocol, initializing...");

            if (!prot->begin()) {
                client.stop();
                delete prot;
                return;
            }
        }
        digitalWrite(LED_BUILTIN, LOW);
        while (client.connected()) {
            prot->go();
        }
        delete prot;
        client.stop();
    }
}
