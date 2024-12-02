#include "company.hpp"

// Template do wprowadzania danych od u�ytkownika
template < typename T >
T getInput(const std::string& prompt)
{
    T value;
    std::cout << prompt;
    std::cin >> value;
    return value;
}

// Wy�wietlanie podsumowania na koniec tury
void displaySummary(const Company& company)
{
    std::cout << "\n### Podsumowanie przychodow na koniec tury ###\n";
    std::cout << "Przychody: " << company.getLastIncome() << " PLN\n";
    std::cout << "Produkcja: " << company.getLastProduction() << " szt.\n";
    std::cout << "Sprzedaz: " << company.getLastSales() << " szt.\n";
    std::cout << "Potencjalna sprzedaz: " << std::min(company.getLastProduction(), company.getLastSales()) << " szt.\n";
    std::cout << "Pojemnosc magazynu: " << company.getWarehouseCapacity() << " szt.\n";
    std::cout << "###################################\n";
}

int main()
{
    Company company;

    // Pocz�tkowe zatrudnienie - po jednym pracowniku ka�dego typu
    company.hireEmployee< Engineer >("Alice", "Mechanical");
    company.hireEmployee< WarehouseWorker >("Bob", true);
    company.hireEmployee< Marketer >("Carol", 5000);
    company.hireEmployee< Worker >("Dave", 42.5);

    double targetValue = 5000000.0; // Cel warto�ci firmy
    bool   gameRunning = true; // Flaga kontroluj�ca g��wn� p�tl� gry

    while (gameRunning)
    {
        // Wy�wietlenie podsumowania na pocz�tku tury
        std::cout << "\n\n NOWA TURA \n\n";
        std::cout << "### Stan firmy ###\n";
        std::cout << "Saldo konta: " << company.getBankBalance() << " PLN\n";

        double totalDebt       = company.getTotalDebt();
        double installment     = company.getCurrentInstallment();
        int    repaymentMonths = company.getRemainingRepaymentMonths();

        std::cout << "Zadluzenie: " << totalDebt << " PLN\n";
        std::cout << "Rata kredytu: " << installment << " PLN\n";
        std::cout << "Czas splaty: " << repaymentMonths << " miesiecy\n";
        std::cout << "###################################\n";

        // Menu akcji gracza
        while (true)
        {
            std::cout << "\nDostepne akcje:\n"
                      << "lp - Wylistuj pracownikow\n"
                      << "zinz - Zatrudnij inzyniera\n"
                      << "zmag - Zatrudnij magazyniera\n"
                      << "zmkt - Zatrudnij marketera\n"
                      << "zrob - Zatrudnij robotnika\n"
                      << "kred - Wez kredyt\n"
                      << "kt - Zakoncz ture\n"
                      << "Wybrana akcja: ";

            std::string action;
            std::cin >> action;

            // Wy�wietla list� wszystkich pracownik�w
            if (action == "lp")
            {
                std::cout << "\n### Lista zatrudnionych pracownik�w ###\n";
                company.printEmployees();
                std::cout << "###################################\n";
            }
            // Zatrudnia nowego in�yniera
            else if (action == "zinz")
            {
                std::string department = getInput< std::string >("Podaj nazwe wydzialu inzyniera: ");
                company.hireEmployee< Engineer >("Nowy Inzynier", department);
                std::cout << "Zatrudniono nowego inzyniera!\n";
            }
            // Zatrudnia nowego magazyniera
            else if (action == "zmag")
            {
                bool forklift =
                    getInput< std::string >("Czy magazynier ma licencje na wozek widlowy? (tak/nie): ") == "tak";
                company.hireEmployee< WarehouseWorker >("Nowy Magazynier", forklift);
                std::cout << "Zatrudniono nowego magazyniera!\n";
            }
            // Zatrudnia nowego marketera
            else if (action == "zmkt")
            {
                int followers = getInput< int >("Podaj liczbe obserwujacych marketera: ");
                company.hireEmployee< Marketer >("Nowy Marketer", followers);
                std::cout << "Zatrudniono nowego marketera!\n";
            }
            // Zatrudnia nowego robotnika
            else if (action == "zrob")
            {
                double shoeSize = getInput< double >("Podaj rozmiar buta robotnika: ");
                company.hireEmployee< Worker >("Nowy Robotnik", shoeSize);
                std::cout << "Zatrudniono nowego robotnika!\n";
            }
            // Obs�uguje proces zaci�gania kredytu
            else if (action == "kred")
            {
                double amount = getInput< double >("Kwota kredytu: ");
                int    months = getInput< int >("Czas splaty (miesiace): ");
                company.takeLoan(amount, months);
                std::cout << "Zaciagnieto kredyt w wysokosci " << amount << " PLN na " << months << " miesiecy.\n";
            }
            // Ko�czy tur�, oblicza stan firmy i sprawdza warunki gry
            else if (action == "kt")
            {
                company.endMonth();
                displaySummary(company); // Wy�wietl podsumowanie po zako�czeniu tury

                if (company.isBankrupt())
                {
                    std::cout << "Bankructwo!\n";
                    gameRunning = false;
                }
                else if (company.hasWon(targetValue))
                {
                    std::cout << "Gratulacje! Osiagnales wartosc firmy " << targetValue << "!\n";
                    gameRunning = false;
                }
                break; // Wyj�cie z p�tli akcji, przej�cie do nowej tury
            }
            else
            {
                std::cout << "Nieznana komenda. Sprobuj ponownie.\n";
            }
        }
    }

    return 0;
}
