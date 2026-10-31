#include <cstdint>
#include <iostream>
#include <format>
#include <utility>

using namespace std;

class Sensore {
    std::string nome;
    int valore;

public:
    Sensore(std::string nomeC, int valoreC) {
        nome = std::move(nomeC);
        valore = valoreC;
    }
    virtual ~Sensore() = default;
    void report() {
        std::cout << std::format("{}: {} {} {}\n", nome, vconv(), unita(), allarme() ? "<-- ALLARME" : "");
    }
    int get_valore() const { return valore; }
    bool allarme() { return vconv() > soglia_allarme() && soglia_allarme() > 0; }

    virtual double vconv() { return valore; }
    virtual double soglia_allarme() { return 0; }
    virtual std::string unita() { return ""; }
};

class SensoreLuce : public Sensore {
public:
    SensoreLuce(std::string nome, int valore) : Sensore(std::move(nome), valore) {}

    std::string unita() override { return "lux"; }
};

class SensoreUmidita : public Sensore {
public:
    SensoreUmidita(std::string nome, int valore) : Sensore(std::move(nome), valore) {}

    std::string unita() override { return "%"; }
    double vconv() override { return get_valore() * 100.0 / 1023.0; }
    double soglia_allarme() override { return 70.0; }
};

class SensoreTemperatura : public Sensore {
public:
    SensoreTemperatura(std::string nome, int valore) : Sensore(std::move(nome), valore) {}
    std::string unita() override { return "°C"; }
    double vconv() override { return static_cast<double>(get_valore()) / 10; }
    double soglia_allarme() override { return 30.0; }
};

class Centralina {
    Sensore* sensori[10] = {};
    uint8_t q = 0;
public:
    bool aggiungi(Sensore* s) {
        if (q >= 10)
            return false;
        sensori[q] = s;
        q++;
        return true;
    }

    void report() const {
        int in_allarme = 0;
        for (int i = 0; i < q; i++) {
            if (sensori[i]->allarme())
                in_allarme++;
            sensori[i]->report();
        }
        std::cout << "In allarme: " << in_allarme << "/" << static_cast<int>(q) << std::endl;
    }
};

int main() {
    Centralina* c = new Centralina();
    Sensore* s[4] = {
        new SensoreTemperatura("Temp. aula", 215),
        new SensoreTemperatura("Temp. server", 342),
        new SensoreUmidita("Umidità bagno", 818),
        new SensoreLuce("Luce corridoio", 350)
    };

    for (int i = 0; i < 4; i++)
        c->aggiungi(s[i]);

    c->report();

    for (int i = 0; i < 4; i++)
        delete s[i];

    delete c;
}
