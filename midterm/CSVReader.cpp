#include "CSVReader.h"
#include <fstream>
#include <iostream>

CSVReader::CSVReader () {}

//////////////////////////////////////////////////////////////////////////////
////////////////////////////// Start /////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

/*
 * This function converts order book entries to candleSticks
 *
 * @param entries: vector of order book entries (needs to be sorted by
 * timestamp)
 * @param type: type of order book entry (ask or bid)
 * @param product: product of order book entry (e.g. ETH/BTC)
 * @return candleSticks: vector of candlesticks
 */
std::vector<CandleStick>
CSVReader::orderBookEntriestoCandleSticks (std::vector<OrderBookEntry> entries,
                                           OrderBookType type,
                                           std::string product)
{
	std::vector<CandleStick> candleSticks;
	std::string timestamp = "";
	double totalValue = 0;
	double totalAmount = 0;
	double open;
	double close;
	double high;
	double low;

	for (OrderBookEntry &e : entries)
		{
			if (e.orderType == type && e.product == product)
				{
					double value = e.price * e.amount;
					// entry in the current timestamp
					if (e.timestamp == timestamp)
						{
							// update variables
							totalValue += value;
							if (e.price > high) high = e.price;
							if (e.price < low) low = e.price;
							totalAmount += e.amount;
						}
					// next timestamp or first order book entry
					if (e.timestamp != timestamp)
						{
							// if not the first order book entry
							if (timestamp != "")
								{
									// create candlestick for previous
									// timestamp
									close = totalValue / totalAmount;
									CandleStick candleStick{ timestamp, open,
										                     high, low,
										                     close };
									candleSticks.push_back (candleStick);

									open = close;
								}
							else open = e.price;

							// initialize variables
							timestamp = e.timestamp;
							totalValue = value;
							high = e.price;
							low = e.price;
							totalAmount = e.amount;
						}
				}
		}
	// create candlestick for last timestamp
	close = totalValue / totalAmount;
	CandleStick candleStick{ timestamp, open, high, low, close };
	candleSticks.push_back (candleStick);

	return candleSticks;
}

/*
 * This function reads a csv file of candlestick data and converts it to
 * candleSticks CSV file must be in the format: timestamp, open, high, low,
 * close,...
 *
 * @param csvFileName: name of csv file
 * @return candleSticks: vector of candlesticks
 */
std::vector<CandleStick>
CSVReader::readCSVtoCandleSticks (std::string csvFileName)
{
	std::vector<CandleStick> candleSticks;

	std::ifstream csvFile{ csvFileName };
	std::string line;
	if (csvFile.is_open ())
		{
			// Skip first line (header)
			std::getline (csvFile, line);
			while (std::getline (csvFile, line))
				{
					std::vector<std::string> tokens = tokenise (line, ',');
					if (tokens.size () < 5)
						{
							std::cout << "Bad line " << std::endl;
							throw std::exception{};
						}

					try
						{
							std::vector<std::string> candleStickTokens (
							    tokens.begin (), tokens.begin () + 5);

							std::string timestamp = tokens[0];
							double open = std::stod (tokens[1]);
							double high = std::stod (tokens[2]);
							double low = std::stod (tokens[3]);
							double close = std::stod (tokens[4]);

							CandleStick candleStick{ timestamp, open, high,
								                     low, close };
							candleSticks.push_back (candleStick);
						}
					catch (const std::exception &e)
						{
							std::cout << "CSVReader::readCSV bad data"
							          << std::endl;
							std::cout << line << std::endl;
						}
				}
		}

	return candleSticks;
}
//////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// End ////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

std::vector<OrderBookEntry>
CSVReader::readCSV (std::string csvFilename)
{
	std::vector<OrderBookEntry> entries;

	std::ifstream csvFile{ csvFilename };
	std::string line;
	if (csvFile.is_open ())
		{
			while (std::getline (csvFile, line))
				{
					try
						{
							OrderBookEntry obe
							    = stringsToOBE (tokenise (line, ','));
							entries.push_back (obe);
						}
					catch (const std::exception &e)
						{
							std::cout << "CSVReader::readCSV bad data"
							          << std::endl;
						}
				}  // end of while
		}

	std::cout << "CSVReader::readCSV read " << entries.size () << " entries"
	          << std::endl;
	return entries;
}

std::vector<std::string>
CSVReader::tokenise (std::string csvLine, char separator)
{
	std::vector<std::string> tokens;
	signed int start, end;
	std::string token;
	start = csvLine.find_first_not_of (separator, 0);
	do {
			end = csvLine.find_first_of (separator, start);
			if (start == csvLine.length () || start == end) break;
			if (end >= 0) token = csvLine.substr (start, end - start);
			else token = csvLine.substr (start, csvLine.length () - start);
			tokens.push_back (token);
			start = end + 1;
		}
	while (end > 0);

	return tokens;
}

OrderBookEntry
CSVReader::stringsToOBE (std::vector<std::string> tokens)
{
	double price, amount;

	if (tokens.size () != 5)  // bad
		{
			std::cout << "Bad line " << std::endl;
			throw std::exception{};
		}
	// we have 5 tokens
	try
		{
			price = std::stod (tokens[3]);
			amount = std::stod (tokens[4]);
		}
	catch (const std::exception &e)
		{
			std::cout << "CSVReader::stringsToOBE Bad float! " << tokens[3]
			          << std::endl;
			std::cout << "CSVReader::stringsToOBE Bad float! " << tokens[4]
			          << std::endl;
			throw;
		}

	OrderBookEntry obe{ price, amount, tokens[0], tokens[1],
		                OrderBookEntry::stringToOrderBookType (tokens[2]) };

	return obe;
}

OrderBookEntry
CSVReader::stringsToOBE (std::string priceString, std::string amountString,
                         std::string timestamp, std::string product,
                         OrderBookType orderType)
{
	double price, amount;
	try
		{
			price = std::stod (priceString);
			amount = std::stod (amountString);
		}
	catch (const std::exception &e)
		{
			std::cout << "CSVReader::stringsToOBE Bad float! " << priceString
			          << std::endl;
			std::cout << "CSVReader::stringsToOBE Bad float! " << amountString
			          << std::endl;
			throw;
		}
	OrderBookEntry obe{ price, amount, timestamp, product, orderType };

	return obe;
}
