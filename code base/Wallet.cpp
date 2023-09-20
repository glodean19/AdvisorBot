#include "Wallet.h"
#include <iostream>
#include "CSVReader.h"

Wallet::Wallet()
{
}
// insert currency to the wallet
/* This function
needs to check if the Wallet contains at least the specified amount of the specified
currency.*/
void Wallet::insertCurrency(std::string type, double amount)
{
    double balance;
    // if insufficient amount
    if (amount < 0)
    {
        throw std::exception{};
    }
    // not there yet
    if (currencies.count(type) == 0)
    {
        balance = 0;
    }
    else
    { // is there
        balance = currencies[type];
    }
    // increase the balance by the amount
    balance += amount;
    currencies[type] = balance;
}
// remove currency from the wallet
/* update the currencies
map accordingly by removing that much currency from the map*/
bool Wallet::removeCurrency(std::string type, double amount)
{
    // if insufficient amount
    if (amount < 0)
    {
        return false;
    }
    // not there yet
    if (currencies.count(type) == 0)
    {

        return false;
    }
    else
    {                                       // is there - do  we have enough
        if (containsCurrency(type, amount)) // we have enough
        {
            currencies[type] -= amount;
            return true;
        }
        else // they have it but not enough.
            return false;
    }
}
// check if the wallet contains this much currency or more
bool Wallet::containsCurrency(std::string type, double amount)
{
    if (currencies.count(type) == 0) // not there yet
        return false;
    else
        return currencies[type] >= amount;
}
/* iterates over the currencies map, pulls each item off there as a
std::string,double pair. While it does this, it constructs a string describing the
contents of the Wallet.*/
std::string Wallet::toString()
{
    std::string s;
    for (std::pair<std::string, double> pair : currencies)
    {
        std::string currency = pair.first;
        double amount = pair.second;
        s += currency + " : " + std::to_string(amount) + "\n";
    }
    return s;
}
// allows to check if it can fulfill a given OrderBookEntry object
bool Wallet::canFulfillOrder(OrderBookEntry order)
{
    std::vector<std::string> currs = CSVReader::tokenise(order.product, '/');
    // ask
    if (order.orderType == OrderBookType::ask)
    {
        double amount = order.amount;
        std::string currency = currs[0];
        std::cout << "Wallet::canFulfillOrder " << currency << " : " << amount << std::endl;

        return containsCurrency(currency, amount);
    }
    // bid
    if (order.orderType == OrderBookType::bid)
    {
        double amount = order.amount * order.price;
        std::string currency = currs[1];
        std::cout << "Wallet::canFulfillOrder " << currency << " : " << amount << std::endl;
        return containsCurrency(currency, amount);
    }

    return false;
}

void Wallet::processSale(OrderBookEntry &sale)
{
    std::vector<std::string> currs = CSVReader::tokenise(sale.product, '/');
    // ask
    if (sale.orderType == OrderBookType::asksale)
    {
        double outgoingAmount = sale.amount;
        std::string outgoingCurrency = currs[0];
        double incomingAmount = sale.amount * sale.price;
        std::string incomingCurrency = currs[1];

        currencies[incomingCurrency] += incomingAmount;
        currencies[outgoingCurrency] -= outgoingAmount;
    }
    // bid
    if (sale.orderType == OrderBookType::bidsale)
    {
        double incomingAmount = sale.amount;
        std::string incomingCurrency = currs[0];
        double outgoingAmount = sale.amount * sale.price;
        std::string outgoingCurrency = currs[1];

        currencies[incomingCurrency] += incomingAmount;
        currencies[outgoingCurrency] -= outgoingAmount;
    }
}
// the << function have special access to the wallet class
std::ostream &operator<<(std::ostream &os, Wallet &wallet)
{
    os << wallet.toString();
    return os;
}
