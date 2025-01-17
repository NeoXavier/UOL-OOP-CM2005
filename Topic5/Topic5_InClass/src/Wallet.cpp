#include "Wallet.h"
#include "CSVReader.h"
#include <iostream>

Wallet::Wallet() {}

// insert currency to the wallet
void Wallet::insertCurrency(std::string type, double amount) {
  double balance;
  if (amount < 0) {
    throw std::exception{};
  }
  if (currencies.count(type) == 0) {
    balance = 0;
  } else {
    balance = currencies[type];
  }
  balance += amount;
  currencies[type] = balance;
}

// remove currency from the Wallet
bool Wallet::removeCurrency(std::string type, double amount) {
    if (amount < 0 || currencies.count(type) == 0 || currencies[type] < amount) {
        return false;
    }

    currencies[type] -= amount;
    return true;
}

// check if the wallet contains this much currency or more
bool Wallet::containsCurrency(std::string type, double amount) {
    if (currencies.count(type) == 0){
        return false;
    }
    return currencies[type] >= amount;
}

bool Wallet::canFulfilOrder(OrderBookEntry order)
{
    /**
     * Currency1/Currency2, price, amount
     * Currency1 is the currency you own
     * Currency2 is the currency you want
     **/
    std::vector<std::string> currencies = CSVReader::tokenise(order.product, '/');

    // ask: check if you own enough currency 1 to but currency2
    if (order.orderType == OrderBookType::ask)
    {
        double amount = order.amount;
        std::string currency = currencies[0];
        std::cout << "Wallet::canFulfillOrder: currency = "<< currency << ", amount = " << amount << std::endl;
        return containsCurrency(currency, amount);
    }

    if (order.orderType == OrderBookType::bid)
    {
        double amount = order.amount * order.price;
        std::string currency = currencies[1];
        std::cout << "Wallet::canFulfillOrder: currency = "<< currency << ", amount = " << amount << std::endl;
        return containsCurrency(currency, amount);
    }
    return false;
}

void Wallet::processSale(OrderBookEntry &sale)
{
    std::vector<std::string> currs = CSVReader::tokenise(sale.product, '/');
    if (sale.orderType == OrderBookType::asksale)
    {
        double outgoingAmount = sale.amount;
        std::string outgoingCurrency = currs[0];

        double incomingAmount = sale.amount * sale.price;
        std::string incomingCurrency = currs[1];

        currencies[incomingCurrency] += incomingAmount;
        currencies[outgoingCurrency] += outgoingAmount;
    }

    if (sale.orderType == OrderBookType::bidsale){
        double incomingAmount = sale.amount;
        std::string incomingCurrency = currs[0];

        double outgoingAmount = sale.amount;
        std::string outgoingCurrency = currs[1];

        currencies[incomingCurrency] += incomingAmount;
        currencies[outgoingCurrency] += outgoingAmount;
    }
}


// get the currencies and the respective amounts in the wallet
std::string Wallet::toString() {
    std::string s;
    for (const std::pair<std::string, double> &pair : currencies){
        std::string currency = pair.first;
        double amount = pair.second;
        s += currency + ": " = std::to_string(amount) + "\n";
    }
    return s; 
}
