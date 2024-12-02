#ifndef EMPLOYEES_HPP
#define EMPLOYEES_HPP

#include <iostream>
#include <string>

//Sta�e wydajno�ci nie s� optymalne, obecne warto�ci maj� na celu tylko unikni�cie bankructwa po pierwszej turze

// Klasa og�lna dla pracownik�w
struct Employee
{
    std::string name;   // Imi� pracownika
    double      salary; // Wynagrodzenie

    Employee(const std::string& name, double salary) : name(name), salary(salary) {}

    // Funkcja do wy�wietlania informacji o pracowniku
    virtual void print() const = 0;

    virtual ~Employee() = default;
};

// Klasa dla in�ynier�w
struct Engineer : public Employee
{
    static constexpr double CI = 50.0; // Wydajno�� CI - cena produktu
    std::string             department; // Uko�czony wydzia�

    Engineer(const std::string& name, const std::string& department) : Employee(name, 6000), department(department) {}

    void print() const override
    {
        std::cout << "Inzynier: " << name << ", Wydzial: " << department << ", Wynagrodzenie: " << salary << " PLN\n";
    }
};

// Klasa dla magazynier�w
struct WarehouseWorker : public Employee
{
    static constexpr double CMag = 500.0; // Wydajno�� CMag
    bool                    forkliftLicense; // Czy magazynier posiada licencj� na w�zek wid�owy

    WarehouseWorker(const std::string& name, bool forkliftLicense) : Employee(name, 4000), forkliftLicense(forkliftLicense)
    {}

    void print() const override
    {
        std::cout << "Magazynier: " << name << ", Licencja na wozek widlowy: " << (forkliftLicense ? "Tak" : "Nie")
                  << ", Wynagrodzenie: " << salary << " PLN\n";
    }
};

// Klasa dla marketer�w
struct Marketer : public Employee
{
    static constexpr double CMkt = 1000.0; // Wydajno�� CMkt
    int                     followers; // Liczba followers�w na social mediach

    Marketer(const std::string& name, int followers) : Employee(name, 5000), followers(followers) {}

    void print() const override
    {
        std::cout << "Marketer: " << name << ", Obserwujacy: " << followers << ", Wynagrodzenie: " << salary
                  << " PLN\n";
    }
};

// Klasa dla robotnik�w
struct Worker : public Employee
{
    static constexpr double CR = 500.0; // Wydajno�� CR
    double                  shoeSize; // Rozmiar buta

    Worker(const std::string& name, double shoeSize) : Employee(name, 4500), shoeSize(shoeSize) {}

    void print() const override
    {
        std::cout << "Robotnik: " << name << ", Rozmiar buta: " << shoeSize << ", Wynagrodzenie: " << salary
                  << " PLN\n";
    }
};

#endif // EMPLOYEES_HPP
