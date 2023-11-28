#include "CSVReader.h"
#include "OrderBookEntry.h"
#include <fstream>
#include <iostream>

CSVReader::CSVReader()
{
}

std::vector<OrderBookEntry> CSVReader::readCSV(std::string csvFile)
{
    return std::vector<OrderBookEntry>();
}

std::vector<std::string> CSVReader::tokenize(std::string csvLine, char sep)
{
    return std::vector<std::string>();
}

OrderBookEntry CSVReader::stringToOBE(std::vector<std::string> tokens)
{
    double price, amount;
    std:string timestamp, product;
    OrderBookType orderType;

    if (tokens.size() != 5)
    {
        std::cout << "Bad line" << std::endl;
        throw std::exception{};
    }
    try {
        price = std::stod(tokens[3]);
        amount = std::stod(tokens[4]);
    } 
    catch (const std::exception &e) 
    {
        std::cout << "Bad float!"<< tokens[3] << std::endl;  
        std::cout << "Bad float!"<< tokens[4] << std::endl;  
        throw;
    }
    
    return OrderBookEntry(price,
            amount,
            tokens[0],
            tokens[1],
            OrderBookEntry::stringToOrderType(tokens[2]));

}
