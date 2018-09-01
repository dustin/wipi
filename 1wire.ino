#include <OneWire.h>
#include <DallasTemperature.h>

#define ADDR_LEN 8

class OneWireProtocol : public Protocol {
public:
    OneWireProtocol(WiFiClient &c) : Protocol("1wire", c), ds(D6) {}

    void printAddr() {
        for (int i = 0; i < ADDR_LEN; i++) {
            if (addr[i] < 0x10) {
                client.print("0");
            }
            client.print(addr[i], HEX);
        }
        client.println("");
    }

    void list() {
        ds.reset_search();
        int found = 0;

        while (ds.search(addr)) {
            if (OneWire::crc8(addr, 7) != addr[7]) {
                client.println("ERR: CRC is not valid!");
                return;
            }

            printAddr();
            found++;
        }

        if (found == 0) {
            client.println("ERR:  no devices found");
            return;
        } else {
            client.print("found ");
            client.println(found);
        }

        ds.reset_search();
        ds.reset();
    }

    int readHalfHex() {
        const char hexdigits[] = "0123456789abcdef";
        const char HEXDIGITS[] = "0123456789ABCDEF";
        int c = client.read();
        for (int i = 0; i < 16; i++) {
            if (c == hexdigits[i] || c == HEXDIGITS[i]) {
                return i;
            }
        }

        client.print("ERR: invalid hex digit: ");
        client.println(c);

        return -1;
    }

    int readHexDigit() {
        int c1 = readHalfHex();
        if (c1 == -1) {
            return -1;
        }
        int c2 = readHalfHex();
        if (c1 == -1) {
            return -1;
        }
        return (c1 << 4) | c2;
    }

    void select() {
        for (int i = 0; i < ADDR_LEN; i++) {
            int d = readHexDigit();
            if (d == -1) {
                client.println("ERR: read hex digits");
                return;
            }
            addr[i] = (byte)d;
        }
        if (OneWire::crc8(addr, 7) != addr[7]) {
            client.println("ERR: CRC is not valid!");
        }
        ds.select(addr);
    }

    void temp() {
        DallasTemperature sensors(&ds);
        sensors.requestTemperatures();
        client.println(sensors.getTempC(addr));
    }

    void go() {
        if (!client.available()) {
            return;
        }
        char c = client.read();
        switch (c) {
        case -1:
            return;
        case 'l': list(); break;
        case 's': select(); break;
        case 't': temp(); break;
        }
    }

private:
    OneWire ds;
    byte data[12];
    byte addr[ADDR_LEN];
};

Protocol *newOneWireProtocol(WiFiClient client) {
    return new OneWireProtocol(client);
}
