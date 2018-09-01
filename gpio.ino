class GPIOProtocol : public Protocol {
public:
    GPIOProtocol(WiFiClient &c) : Protocol("GPIO", c), currentPin(0) {}

    void go() {
        if (!client.available()) {
            return;
        }
        char c = client.read();
        switch (c) {
        case -1:
            return;
        case '+': digitalWrite(currentPin, HIGH); break;
        case '-': digitalWrite(currentPin, LOW); break;
        case 'p': digitalWrite(currentPin, INPUT_PULLUP); break;
        case 'i': pinMode(currentPin, INPUT); break;
        case 'o': pinMode(currentPin, OUTPUT); break;
        case '0': currentPin = D0; break;
        case '1': currentPin = D1; break;
        case '2': currentPin = D2; break;
        case '3': currentPin = D3; break;
        case '4': currentPin = D4; break;
        case '5': currentPin = D5; break;
        case '6': currentPin = D6; break;
        case '7': currentPin = D7; break;
        case '8': currentPin = D8; break;
        }
        delay(1);
    }

private:
    int currentPin;
};

Protocol *newGPIOProtocol(WiFiClient client) {
    return new GPIOProtocol(client);
}
