#ifndef COMPANY_HPP
#define COMPANY_HPP

#include "employees.hpp"
#include <algorithm>
#include <iostream>
#include <memory>
#include <numeric>
#include <vector>

// Klasa reprezentuj�ca firm�
// Odpowiada za zarz�dzanie pracownikami, finansami, produkcj�, magazynowaniem i sprzeda��.
class Company
{
public:
    // Sta�e definiuj�ce limity i parametry gry
    static constexpr int    MAX_LOAN_TERM       = 24;  // Maksymalny czas sp�aty kredytu
    static constexpr double MAX_DEBT_MULTIPLIER = 3.0; // Maksymalne zad�u�enie w stosunku do warto�ci firmy
    static constexpr int    AVERAGE_MONTHS      = 3;   // Liczba miesi�cy do obliczania �redniej warto�ci firmy

    // Inicjalizacja firmy z pocz�tkowym stanem finansowym
    Company()
        : m_bankBalance(20000.0),
          m_currentValue(0.0),
          m_lastIncome(0.0),
          m_lastProduction(0.0),
          m_lastSales(0.0),
          m_isInitialized(false)
    {}

    // Template do zatrudniania pracownik�w dowolnego typu
    template < typename T, typename... Args >
    void hireEmployee(Args&&... args)
    {
        m_employees.push_back(std::make_unique< T >(std::forward< Args >(args)...));
    }

    // Wy�wietla list� wszystkich pracownik�w
    void printEmployees() const
    {
        for (const auto& emp : m_employees)
        {
            emp->print();
        }
    }

    // Oblicza pensje pracownik�w i je wyp�aca z konta firmy
    void paySalaries()
    {
        double totalSalary = 0;
        for (const auto& emp : m_employees)
        {
            totalSalary += emp->salary;
        }
        m_bankBalance -= totalSalary;
    }

    // Oblicza przychody firmy
    double calculateIncome()
    {
        double pricePerProduct = 0, demand = 0, productionCapacity = 0;

        for (const auto& emp : m_employees)
        {
            if (auto engineer = dynamic_cast< Engineer* >(emp.get()))
            {
                pricePerProduct += Engineer::CI; // U�ycie CI zamiast productivity
            }
            else if (auto warehouse = dynamic_cast< WarehouseWorker* >(emp.get()))
            {
                productionCapacity += WarehouseWorker::CMag; // U�ycie CMag zamiast capacityIncrease
            }
            else if (auto marketer = dynamic_cast< Marketer* >(emp.get()))
            {
                demand += Marketer::CMkt; // U�ycie CMkt zamiast demandIncrease
            }
            else if (auto worker = dynamic_cast< Worker* >(emp.get()))
            {
                productionCapacity += Worker::CR; // U�ycie CR zamiast productionRate
            }
        }

        m_lastProduction = std::min(productionCapacity, demand);
        m_lastSales      = std::min(demand, productionCapacity);
        m_lastIncome     = m_lastSales * pricePerProduct;

        return m_lastIncome;
    }


    /* Funkcja do oblicze� tylko dla pierwszej tury
    (np. �eby nie wyskakiwa� b��d �e warto�� firmy jest zerowa
    i nie mo�na wzi�� kredytu)
    */
    void initializeCurrentValue()
    {
        if (!m_isInitialized)
        {
            m_lastIncome     = calculateIncome();
            m_lastProduction = m_lastIncome / Engineer::CI;
            m_lastSales      = m_lastProduction; // Za��my, �e sprzedajemy wszystko
            m_bankBalance += m_lastIncome;

            m_lastIncomes.push_back(m_lastIncome);
            m_currentValue  = m_lastIncome; // Pocz�tkowa warto�� firmy to pierwszy przych�d
            m_isInitialized = true;         // Flaga ustawiona na true
        }
    }

    // Koniec tury: wyp�aca pensje, oblicza przychody i sp�aca kredyty
    void endMonth()
    {
        initializeCurrentValue(); // Zapewnij inicjalizacj� tylko raz
        paySalaries();
        double income = calculateIncome();
        m_bankBalance += income;

        // Aktualizacja warto�ci firmy
        m_lastIncomes.push_back(income);
        if (m_lastIncomes.size() > AVERAGE_MONTHS)
        {
            m_lastIncomes.erase(m_lastIncomes.begin());
        }

        if (!m_lastIncomes.empty())
        {
            m_currentValue = std::accumulate(m_lastIncomes.begin(), m_lastIncomes.end(), 0.0) / m_lastIncomes.size();
        }

        repayLoans();
    }

    // Obliczenie ca�kowitej pojemno�ci magazynu.
    double getWarehouseCapacity() const
    {
        double totalCapacity = 0;
        for (const auto& emp : m_employees)
        {
            if (auto warehouse = dynamic_cast< WarehouseWorker* >(emp.get()))
            {
                totalCapacity += WarehouseWorker::CMag;
            }
        }
        return totalCapacity;
    }

    // Wzi�cie kredytu

    void takeLoan(double amount, int repaymentMonths)
    {
        initializeCurrentValue(); // Zapewnij, �e m_currentValue jest obliczone
        if (repaymentMonths > MAX_LOAN_TERM)
        {
            std::cout << "Blad: Maksymalny czas splaty kredytu to " << MAX_LOAN_TERM << " miesiecy.\n";
            return;
        }

        double maxAllowedDebt = m_currentValue * MAX_DEBT_MULTIPLIER;
        double currentDebt    = getTotalDebt();
        if (currentDebt + amount > maxAllowedDebt)
        {
            std::cout << "Blad: Zadluzenie nie moze przekroczyc " << MAX_DEBT_MULTIPLIER << " razy wartosci firmy ("
                      << maxAllowedDebt << ").\n";
            return;
        }

        double interestRate = 0.05 + 0.01 * repaymentMonths;
        m_loans.push_back({amount, repaymentMonths, interestRate});
        m_bankBalance += amount;
    }

    
    bool isBankrupt() const { return m_bankBalance <= 0; } // Czy firma zbankrutowa�a
    bool hasWon(double targetValue) const { return m_currentValue >= targetValue; } // Czy firma osi�gn�a wymagany cel warto�ci

    // Gettery
    double getBankBalance() const { return m_bankBalance; }
    double getLastIncome() const { return m_lastIncome; }
    double getLastProduction() const { return m_lastProduction; }
    double getLastSales() const { return m_lastSales; }
    double getCurrentValue() const { return m_currentValue; }
    double getTotalDebt() const
    {
        double totalDebt = 0;
        for (const auto& loan : m_loans)
        {
            totalDebt += loan.amount;
        }
        return totalDebt;
    }
    double getCurrentInstallment() const
    {
        double totalInstallment = 0;
        for (const auto& loan : m_loans)
        {
            totalInstallment += loan.calculateInstallment();
        }
        return totalInstallment;
    }
    int getRemainingRepaymentMonths() const
    {
        int maxMonths = 0;
        for (const auto& loan : m_loans)
        {
            maxMonths = std::max(maxMonths, loan.repaymentMonths);
        }
        return maxMonths;
    }

private:
    // Kredyt
    struct Loan
    {
        double amount;          // Kwota kredytu
        int    repaymentMonths; // Liczba miesi�cy do sp�aty
        double interestRate;    // Oprocentowanie

        // Obliczanie wysoko�ci raty dla kredytu
        double calculateInstallment() const
        {
            if (repaymentMonths > 0)
            {
                double installment = amount / repaymentMonths;
                double interest    = amount * interestRate / repaymentMonths;
                return installment + interest;
            }
            return 0.0;
        }
    };

    // Sp�acanie kredyt�w
    void repayLoans()
    {
        double totalRepayment = 0;

        for (auto& loan : m_loans)
        {
            // Obliczanie raty
            double installment = loan.calculateInstallment();
            totalRepayment += installment;

            loan.amount -= (loan.amount / loan.repaymentMonths);
            loan.repaymentMonths--;

            // Je�li kredyt zosta� ca�kowicie sp�acony
            if (loan.repaymentMonths == 0 || loan.amount <= 0)
            {
                loan.amount = 0;
            }
        }

        // Usuwanie sp�aconych kredyt�w
        m_loans.erase(std::remove_if(m_loans.begin(), m_loans.end(), [](const Loan& loan) { return loan.amount <= 0; }),
                      m_loans.end());

        // Zmniejszenie salda o sum� sp�aconych rat
        m_bankBalance -= totalRepayment;
    }

    double m_bankBalance;    // Saldo firmy
    double m_currentValue;   // Warto�� firmy
    double m_lastIncome;     // Ostatni przych�d
    double m_lastProduction; // Ostatnia produkcja
    double m_lastSales;      // Ostatnia sprzeda�
    bool m_isInitialized;    // Flaga inicjalizacji
    std::vector< std::unique_ptr< Employee > > m_employees;      // Lista pracownik�w
    std::vector< Loan >                        m_loans;          // Lista kredyt�w
    std::vector< double >                      m_lastIncomes;    // Ostatnie przychody
};

#endif // COMPANY_HPP
