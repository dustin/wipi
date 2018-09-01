#include <OneWire.h>

class OneWireProtocol : public Protocol {
public:
    OneWireProtocol(WiFiClient &c) : Protocol("1wire", c), ds(1) {}

    void go() {
        ds.reset_search();
        byte data[12];
        byte addr[8];

        if (!ds.search(addr)) {
            client.println("No more addresses.");
            ds.reset_search();
            delay(1000);
            return;
        }

        client.print("ROM =");
        for (int i = 0; i < 8; i++) {
            client.write(' ');
            client.print(addr[i], HEX);
        }

        if (OneWire::crc8(addr, 7) != addr[7]) {
            client.println("CRC is not valid!");
            return;
        }
        ds.reset();
    }

private:
    OneWire ds;
};

Protocol *newOneWireProtocol(WiFiClient client) {
    return new OneWireProtocol(client);
}
