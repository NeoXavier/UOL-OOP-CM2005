#pragma once

#include "OrderBookEntry.h"
#include <vector>
#include <string>
#include "CandleStick.h"


class CSVReader
{
    public:
     CSVReader();

     static std::vector<CandleStick> orderBookEntriestoCandleSticks(std::vector<OrderBookEntry> entries, OrderBookType type, std::string product);
     static std::vector<OrderBookEntry> readCSV(std::string csvFile);
     static std::vector<std::string> tokenise(std::string csvLine, char separator);
    
     static OrderBookEntry stringsToOBE(std::string price, 
                                        std::string amount, 
                                        std::string timestamp, 
                                        std::string product, 
                                        OrderBookType OrderBookType);

    private:
     static OrderBookEntry stringsToOBE(std::vector<std::string> strings);
     
};
