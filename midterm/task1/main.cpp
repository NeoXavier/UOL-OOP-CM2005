#include "CSVReader.h"
#include <iostream>
#include <string>
#include <vector>

int
main ()
{

	// Checking candlestick vector
	std::vector<CandleStick> candleSticks
	    = CSVReader::orderBookEntriestoCandleSticks (
	        CSVReader::readCSV ("20200317.csv"), OrderBookType::ask,
	        "ETH/BTC");

	std::cout << "\nTASK 1 Output: " << std::endl;
	std::cout << "Checking candlesticks for ETH/BTC asks" << std::endl;
	std::cout << "Number of candlesticks generated: " << candleSticks.size() << std::endl;
    
	for (CandleStick &c : candleSticks)
		{
			std::cout << c.timestamp << std::endl;
			std::cout << "Open: " << c.open << " Close: " << c.close
			          << " High: " << c.high << " Low: " << c.low << std::endl;
		}
}
