#include <string>
#include <map>
#include "OrderBookEntry.h"

class Wallet
{
    public:
        Wallet();
        //insert currency to the wallet
        void insertCurrency(std::string type, double amount);
        //remove currency from the Wallet
        bool removeCurrency(std::string type, double amount);
        // check if the wallet contains this much currency or more
        bool containsCurrency(std::string type, double amount);
        // get the currencies and the respective amounts in the wallet
        std::string toString();
        
        // check if the wallet can cop with this ask for bid
        bool canFulfilOrder(OrderBookEntry order);

        /** update the contents of the wallet
         * assumes the order was made by the owner of the wallet
         */
        void processSale(OrderBookEntry &sale);

    private:
        std::map<std::string, double> currencies;
};
