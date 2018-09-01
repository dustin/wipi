class AnalogProtocol : public Protocol {
public:
    AnalogProtocol(WiFiClient &c) : Protocol("analog", c) {}
    void go() {
        if (client.read() != -1) {
            client.println(analogRead(A0));
        }
        delay(1);
    }
};

Protocol *newAnalogProtocol(WiFiClient client) {
    return new AnalogProtocol(client);
}
