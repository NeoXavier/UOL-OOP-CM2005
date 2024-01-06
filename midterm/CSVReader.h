#pragma once

#include "CandleStick.h"
#include "OrderBookEntry.h"
#include <string>
#include <vector>

class CSVReader
{
  public:
	CSVReader ();

	////////////////////////////// Start /////////////////////////////////////
	static std::vector<CandleStick>
	orderBookEntriestoCandleSticks (std::vector<OrderBookEntry> entries,
	                                OrderBookType type, std::string product);
	static std::vector<CandleStick>
	readCSVtoCandleSticks (std::string csvFileName);
	///////////////////////////////////// End ////////////////////////////////

	static std::vector<OrderBookEntry> readCSV (std::string csvFile);
	static std::vector<std::string> tokenise (std::string csvLine,
	                                          char separator);

	static OrderBookEntry stringsToOBE (std::string price, std::string amount,
	                                    std::string timestamp,
	                                    std::string product,
	                                    OrderBookType OrderBookType);

  private:
	static OrderBookEntry stringsToOBE (std::vector<std::string> strings);
};
