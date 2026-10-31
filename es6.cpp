#include <cstdint>
#include <iostream>
#include <format>
#include <utility>

using namespace std;

class Materiale {
    std::string codice;
    std::string titolo;
    uint16_t anno;

public:
    Materiale(std::string codiceC, std::string titoloC, const uint16_t annoC) {
        codice = std::move(codiceC);
        titolo = std::move(titoloC);
        anno = annoC;
    }
    virtual uint8_t prestito() {
        return 0;
    }

    std::string to_string() {
        return std::format("[{}] {} ({}){} - prestito {} giorni", codice, titolo, anno, extra(), prestito());
    }

    double mora(const uint16_t gg) {
        return mora_gg() * max(0, gg - prestito());
    }

    virtual double mora_gg() {
        return 0;
    }

    virtual std::string extra() {
        return "";
    }
};

class Libro : public Materiale {
    uint16_t n_pagine;
public:
    Libro(std::string codiceC, std::string titoloC, const uint16_t annoC, const uint16_t n_pagC) : Materiale(std::move(codiceC), std::move(titoloC), annoC) {
        n_pagine = n_pagC;
    }

    uint8_t prestito() override {
        return 30;
    }

    double mora_gg() override {
        return 0.10;
    }

    std::string extra() override {
        return std::format(", {} pagine", n_pagine);
    }
};

class Rivista : public Materiale {
    uint16_t n_rivista;
public:
    Rivista(std::string codiceC, std::string titoloC, const uint16_t annoC, const uint16_t n_rivC) : Materiale(std::move(codiceC), std::move(titoloC), annoC) {
        n_rivista = n_rivC;
    }

    uint8_t prestito() override {
        return 14;
    }

    double mora_gg() override {
        return 0.20;
    }

    std::string extra() override {
        return std::format(", n.{}", n_rivista);
    }
};

class DVD : public Materiale {
    uint16_t durata;
public:
    DVD(std::string codiceC, std::string titoloC, const uint16_t annoC, const uint16_t durataC) : Materiale(std::move(codiceC), std::move(titoloC), annoC) {
        durata = durataC;
    }

    uint8_t prestito() override {
        return 7;
    }

    double mora_gg() override {
        return 0.50;
    }

    std::string extra() override {
        return std::format(", {} minuti", durata);
    }
};

int main() {
    Materiale* mat[4];
    double mora_t;

    mat[0] = new Libro("L01", "I promessi sposi", 1827, 720);
    mat[1] = new DVD("D01", "Il sorpasso", 1962, 105);
    mat[2] = new Rivista("R01", "Informatica Oggi", 2026, 12);
    mat[3] = new Libro("L02", "Il nome della rosa", 1980, 512);

    for (int i = 0; i < 4; i++) {
        std::cout << mat[i]->to_string() << std::endl;
        std::cout << "Mora: " << mat[i]->mora(mat[i]->prestito() +10) << " euro." << std::endl;
        mora_t += mat[i]->mora(gg_r[i]);
    }
    std::cout << "Mora totale: " << mora_t << " euro." << std::endl;
}
