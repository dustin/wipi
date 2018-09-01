#include <DHTesp.h>


class DHTProtocol : public Protocol {
public:
    DHTProtocol(WiFiClient &c) : Protocol("dht22", c) {}

    bool begin() {
        int n = client.parseInt();
        if (n < 0 || n > maxPin) {
            client.println("invalid pin. Select 0-9 for D0-D8");
            return false;
        }

        dht.setup(pinMap[n], DHTesp::AUTO_DETECT);
        return true;
    }

    void go() {
        if (client.read() != -1) {
            float humidity = dht.getHumidity();
            float temperature = dht.getTemperature();

            if (dht.getStatus() == DHTesp::ERROR_NONE) {
                client.print(humidity);
                client.write("% ");
                client.print(temperature);
                client.println("C");
            } else {
                client.print("ERR: ");
                client.println(dht.getStatusString());
            }
        }
        delay(dht.getMinimumSamplingPeriod());
    }

private:
    DHTesp dht;
};

Protocol* newDHTProtocol(WiFiClient client) {
    return new DHTProtocol(client);
}
