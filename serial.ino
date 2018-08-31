class SerialProtocol : public Protocol {
public:
    SerialProtocol(WiFiClient &c) : SerialProtocol(c) {}

    ~SerialProtocol() {
        Serial.end();
    }

    bool begin() {
        int rate = client.parseInt();
        if (rate == 0) {
            Serial.write("Invalid baud rate.");
            return false;
        }
        client.flush();
        Serial.begin(rate);
        return true;
    }

    void go() {
        if (Serial.available()) {
            size_t len = Serial.available();
            uint8_t sbuf[len];
            Serial.readBytes(sbuf, len);
            client.write(sbuf, len);
        }
        while (client.available()) {
            Serial.write(client.read());
            delay(1);
        }
    }
};

Protocol *newSerialProtocol(WiFiClient client) {
    return new SerialProtocol(client);
}
