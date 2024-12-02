#ifndef EMPLOYEES_HPP
#define EMPLOYEES_HPP

#include <iostream>
#include <string>

//Sta³e wydajnoœci nie s¹ optymalne, obecne wartoœci maj¹ na celu tylko unikniêcie bankructwa po pierwszej turze

// Klasa ogólna dla pracowników
struct Employee
{
    std::string name;   // Imiê pracownika
    double      salary; // Wynagrodzenie

    Employee(const std::string& name, double salary) : name(name), salary(salary) {}

    // Funkcja do wyœwietlania informacji o pracowniku
    virtual void print() const = 0;

    virtual ~Employee() = default;
};

// Klasa dla in¿ynierów
struct Engineer : public Employee
{
    static constexpr double CI = 50.0; // Wydajnoœæ CI - cena produktu
    std::string             department; // Ukoñczony wydzia³

    Engineer(const std::string& name, const std::string& department) : Employee(name, 6000), department(department) {}

    void print() const override
    {
        std::cout << "Inzynier: " << name << ", Wydzial: " << department << ", Wynagrodzenie: " << salary << " PLN\n";
    }
};

// Klasa dla magazynierów
struct WarehouseWorker : public Employee
{
    static constexpr double CMag = 500.0; // Wydajnoœæ CMag
    bool                    forkliftLicense; // Czy magazynier posiada licencjê na wózek wid³owy

    WarehouseWorker(const std::string& name, bool forkliftLicense) : Employee(name, 4000), forkliftLicense(forkliftLicense)
    {}

    void print() const override
    {
        std::cout << "Magazynier: " << name << ", Licencja na wozek widlowy: " << (forkliftLicense ? "Tak" : "Nie")
                  << ", Wynagrodzenie: " << salary << " PLN\n";
    }
};

// Klasa dla marketerów
struct Marketer : public Employee
{
    static constexpr double CMkt = 1000.0; // Wydajnoœæ CMkt
    int                     followers; // Liczba followersów na social mediach

    Marketer(const std::string& name, int followers) : Employee(name, 5000), followers(followers) {}

    void print() const override
    {
        std::cout << "Marketer: " << name << ", Obserwujacy: " << followers << ", Wynagrodzenie: " << salary
                  << " PLN\n";
    }
};

// Klasa dla robotników
struct Worker : public Employee
{
    static constexpr double CR = 500.0; // Wydajnoœæ CR
    double                  shoeSize; // Rozmiar buta

    Worker(const std::string& name, double shoeSize) : Employee(name, 4500), shoeSize(shoeSize) {}

    void print() const override
    {
        std::cout << "Robotnik: " << name << ", Rozmiar buta: " << shoeSize << ", Wynagrodzenie: " << salary
                  << " PLN\n";
    }
};

#endif // EMPLOYEES_HPP
