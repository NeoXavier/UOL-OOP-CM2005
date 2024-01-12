// CandleStick.h
//////////////////////////////////////////////////////////////////////////////
////////////////////////////// Start /////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include <string>

// Types of candlesticks, Green for bullish and Red for bearish
enum class CandleStickType
{
	bullish,
	bearish
};

class CandleStick
{
  public:
	CandleStick (std::string _timestamp, double _open, double _high,
	             double _low, double _close);

	// comparison function for sorting
	static bool compareByTimestamp (CandleStick &c1, CandleStick &c2);

	std::string timestamp;
	double open;
	double high;
	double low;
	double close;
	CandleStickType type;
};

//////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// End ////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

// CandleStick.cpp

//////////////////////////////////////////////////////////////////////////////
////////////////////////////// Start /////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#include "CandleStick.h"

// CandlesStick class constructor
CandleStick::CandleStick (std::string _timestamp, double _open, double _high,
                          double _low, double _close)
    : timestamp (_timestamp), open (_open), high (_high), low (_low),
      close (_close)
{
	// Assign candlestick type based on bearish or bullish
	if (close > open) { this->type = CandleStickType::bullish; }
	else { this->type = CandleStickType::bearish; }
}

// Fucntion for sorting candlesticks by timestamp
bool
CandleStick::compareByTimestamp (CandleStick &c1, CandleStick &c2)
{
	return c1.timestamp < c2.timestamp;
}

//////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// End ////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

// Plotter.h

//////////////////////////////////////////////////////////////////////////////
////////////////////////////// Start /////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "CandleStick.h"

class Plotter
{
  public:
	Plotter (std::vector<CandleStick> _candleSticks, int _plotHeight,
             int _plotWidth, std::string _title);
	void plot ();

  private:
	std::vector<CandleStick> candleSticks;
    std::string title;
	double minPrice;
	double maxPrice;
	int numOfCandleSticks;
	int plotHeight;
	int plotWidth;
	double step;
	int columnWidth;
	std::vector<std::vector<std::string> > plotLines;

	double getMaxPrice (std::vector<CandleStick> candleSticks);
	double getMinPrice (std::vector<CandleStick> candleSticks);
	int valueToLineNumber (double value, double step, double minPrice);
	void drawCandleStickOnPlot (CandleStick candleStick, int col);

	std::string fixLength (std::string str, int length);
};

//////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// End ////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

// Plotter.cpp

//////////////////////////////////////////////////////////////////////////////
////////////////////////////// Start /////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#include "Plotter.h"
#include <iostream>

// Plotter class constructor
Plotter::Plotter (std::vector<CandleStick> _candleSticks, int _plotHeight,
                  int _plotWidth, std::string _title)
    : candleSticks{ _candleSticks }, plotHeight{ _plotHeight },
      plotWidth{ _plotWidth }, title{ _title }
{
	std::sort (candleSticks.begin (), candleSticks.end (),
	           CandleStick::compareByTimestamp);
	this->minPrice = getMinPrice (candleSticks) * 0.99995;
	this->maxPrice = getMaxPrice (candleSticks) * 1.00005;

	this->numOfCandleSticks = candleSticks.size ();

	this->step = (maxPrice - minPrice) / plotHeight;
	this->columnWidth = (plotWidth - numOfCandleSticks) / numOfCandleSticks;

	// Minimum column width is 10, if calculated column width is less than 10,
	// reduce the number of candlesticks to plot
	if (columnWidth < 10)
		{
			columnWidth = 10;
            int oldNumOfCandleSticks = numOfCandleSticks;
			numOfCandleSticks = plotWidth / 11;
			std::vector<CandleStick> newCandleSticks (candleSticks.begin (),
			                                          candleSticks.begin ()
			                                              + numOfCandleSticks);
			candleSticks = newCandleSticks;

			// Warning message
			std::cout
			    << "Warning: Too many candlesticks to plot. Plotting first "
			    << numOfCandleSticks << " out of " << oldNumOfCandleSticks <<  " candlesticks instead." << std::endl;
		}

	std::string spacer = " ";

	// Intializing plot matrix
	for (int row = 0; row < plotHeight + 2; row++)
		{
			std::vector<std::string> line;
			// number of columns = number of candlesticks * 2 as each
			// candlestick is followed by an empty column for spacing
			for (int col = 0; col < (numOfCandleSticks * 2); col++)
				{
					// Drawing y-axis
					if (col == 0)
						{
							// Date label on y-axis
							if (row == plotHeight)
								{
									// Drawing x-axis
									line.push_back (fixLength ("Date", 10));
									continue;
								}
							// Time label on y-axis
							if (row == plotHeight + 1)
								{
									// Drawing x-axis
									line.push_back (fixLength ("Time", 10));
									continue;
								}

							std::string tick = fixLength (
							    std::to_string (maxPrice - (step * row)), 10);
							line.push_back (tick);
							continue;
						}

					line.push_back (spacer);
				}
			plotLines.push_back (line);
		}
}

/*
 * Function that plots the candlesticks on the plot and prints entire plot
 */
void
Plotter::plot ()
{
	for (int i = 0; i < candleSticks.size (); i++)
		{
			drawCandleStickOnPlot (candleSticks[i], (i * 2) + 1);
		}
	// printing plot
    std::cout << std::endl;
    std::cout << title << std::endl;
	for (std::vector<std::string> &line : plotLines)
		{
			for (std::string s : line) { std::cout << s; }
			std::cout << std::endl;
		}
    std::cout << std::endl;
}

/*
 * Function that draws a single candlestick on the plot
 *
 * @param candleStick: candlestick object to be plotted
 * @param col: column number on the plot
 */
void
Plotter::drawCandleStickOnPlot (CandleStick candleStick, int col)
{
	int highLineNumber = valueToLineNumber (candleStick.high, step, maxPrice);
	int lowLineNumber = valueToLineNumber (candleStick.low, step, maxPrice);
	int closeLineNumber
	    = valueToLineNumber (candleStick.close, step, maxPrice);
	int openLineNumber = valueToLineNumber (candleStick.open, step, maxPrice);

	std::string blankPrinter = std::string (columnWidth, ' ');

	std::string wickPrinter;
	if (columnWidth % 2 == 1)
		{
			wickPrinter = std::string (columnWidth / 2, ' ') + "|"
			              + std::string (columnWidth / 2, ' ');
		}
	else
		{
			wickPrinter = std::string (columnWidth / 2, ' ') + "|"
			              + std::string (columnWidth / 2 - 1, ' ');
		}

	// Bullish order from top to bottom: High, Close, Open, Low
	// Bearish order from top to bottom: High, Open, Close, Low
	int startBodyRow;
	int endBodyRow;
	std::string bodyPrinter;
	if (candleStick.type == CandleStickType::bullish)
		{
			startBodyRow = closeLineNumber;
			endBodyRow = openLineNumber;
			bodyPrinter = "|" + std::string (columnWidth - 2, '+') + "|";
		}
	if (candleStick.type == CandleStickType::bearish)
		{
			startBodyRow = openLineNumber;
			endBodyRow = closeLineNumber;
			bodyPrinter = "|" + std::string (columnWidth - 2, '-') + "|";
		}

	std::string printer = blankPrinter;
	for (int row = 0; row < plotHeight; row++)
		{
			// Start of wick
			if (row == highLineNumber) { printer = wickPrinter; }
			// Start of body
			if (row == startBodyRow) { printer = bodyPrinter; }
			// End of body
			if (row == endBodyRow + 1) { printer = wickPrinter; }
			// End of wick
			if (row == lowLineNumber + 1) { printer = blankPrinter; }
			plotLines[row][col] = printer;
		}
	// Print x-axis timestamps
	// Date
	plotLines[plotHeight][col]
	    = fixLength (candleStick.timestamp.substr (0, 10), columnWidth);

	// Time (if timstamp includes time)
	if (candleStick.timestamp.length () > 10)
		{
			plotLines[plotHeight + 1][col] = fixLength (
			    candleStick.timestamp.substr (11, 8), columnWidth);
		}
}

/*
 * Function that maps a value to the line number on the plot
 * @param value: value to be mapped
 * @param step: step size between each line
 * @param maxPrice: maximum price on the plot
 * @return: line number on the plot
 */
int
Plotter::valueToLineNumber (double value, double step, double maxPrice)
{
	int lineNumber;
	for (int i = 0; i <= plotHeight; i++)
		{
			if (value > (maxPrice - (step * i)))
				{
					lineNumber = i - 1;
					break;
				}
		}
	return lineNumber;
}

// Helper Functions

// getMinPrice and getMaxPrice functions are used to calculate the minimum and
// maximum prices
double
Plotter::getMaxPrice (std::vector<CandleStick> candleSticks)
{
	double maxPrice = 0;
	for (CandleStick &c : candleSticks)
		{
			if (c.high > maxPrice) maxPrice = c.high;
		}
	return maxPrice;
}

double
Plotter::getMinPrice (std::vector<CandleStick> candleSticks)
{
	double minPrice = candleSticks[0].low;
	for (CandleStick &c : candleSticks)
		{
			if (c.low < minPrice) minPrice = c.low;
		}
	return minPrice;
}

/*
 * Function that fixes the length of a string by adding spaces at the back
 * @param str: string to be fixed
 * @param length: desired length of the string
 * @return: fixed string
 */
std::string
Plotter::fixLength (std::string str, int length)
{
	// If the string is too long, trim it from the back
	if (str.size () > length) str = str.substr (0, length);
	// If the string is too short, add spaces at the back
	if (str.size () < length) str.append (length - str.size (), ' ');
	return str;
}

//////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// End ////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

// MerkelMain.cpp

#include "MerkelMain.h"
#include "CSVReader.h"
#include "OrderBookEntry.h"
#include "Plotter.h"
#include <iostream>
#include <vector>

MerkelMain::MerkelMain () {}

void
MerkelMain::init ()
{
	int input;
	currentTime = orderBook.getEarliestTime ();

	wallet.insertCurrency ("BTC", 10);

	while (true)
		{
			printMenu ();
			input = getUserOption ();
			processUserOption (input);
		}
}

void
MerkelMain::printMenu ()
{
	// 1 print help
	std::cout << "1: Print help " << std::endl;
	// 2 print exchange stats
	std::cout << "2: Print exchange stats" << std::endl;
	// 3 make an offer
	std::cout << "3: Make an offer " << std::endl;
	// 4 make a bid
	std::cout << "4: Make a bid " << std::endl;
	// 5 print wallet
	std::cout << "5: Print wallet " << std::endl;
	// 6 continue
	std::cout << "6: Continue " << std::endl;

	std::cout << "7: Show Candle Stick data " << std::endl;  // edited

	std::cout << "8: Plot Candle Stick data " << std::endl;  // edited

	std::cout << "9: Plot Candle Stick data (DAI-USD) "
	          << std::endl;  // edited

	std::cout << "============== " << std::endl;

	std::cout << "Current time is: " << currentTime << std::endl;
}

///////////////////////////////////////////////////////////////////////////////
////////////////////////////// Start //////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void
MerkelMain::showCandleStickData ()
{
	// User input date from 2 options
	std::string line;
	std::cout << "Choose date:\n 1. 2020-03-17\n 2. 2020-06-01" << std::endl;
	std::getline (std::cin, line);
	std::string csvFilename;
	if (line == "1") { csvFilename = "20200317.csv"; }
	else if (line == "2") { csvFilename = "20200601.csv"; }
	else
		{
			std::cout << "Invalid choice. Choose 1 or 2" << std::endl;
			return;
		}

	// User input product and type
	std::cout << "Enter Product and type (e.g ETH/BTC ask)" << std::endl;
	std::getline (std::cin, line);
	std::vector<std::string> tokens = CSVReader::tokenise (line, ' ');
	if (tokens.size () != 2)
		{
			std::cout << "Bad input! " << line << std::endl;
			return;
		}
	std::string product = tokens[0];

	// Convert user input type to OrderBookType
	OrderBookType type;
	if (tokens[1] == "ask") { type = OrderBookType::ask; }
	else if (tokens[1] == "bid") { type = OrderBookType::bid; }
	else
		{
			std::cout << "Invalid input for product and/or type" << std::endl;
			return;
		}

	// Create CandleStick vector and print each candlestick
	std::vector<CandleStick> candleSticks
	    = CSVReader::orderBookEntriestoCandleSticks (
	        CSVReader::readCSV (csvFilename), type, product);

	std::cout << std::endl;
	std::cout << "Showing candlestick data for " << product << " " << tokens[1]
	          << std::endl;
	for (CandleStick &c : candleSticks)
		{
			std::cout << std::endl;
			std::cout << c.timestamp << std::endl;
			std::cout << "Open: " << c.open << " Close: " << c.close
			          << " High: " << c.high << " Low: " << c.low << std::endl;
		}
}

void
MerkelMain::plotCandleStickData (std::vector<CandleStick> candleSticks,
                                 std::string title)
{
	// User input plot width
	std::string line;
	std::cout << "Enter Plot Width: " << std::endl;
	std::getline (std::cin, line);
	int plotWidth = std::stoi (line);

	// Plot candlesticks
	int plotHeight = 20;
	Plotter plt{ candleSticks, plotHeight, plotWidth, title };
	plt.plot ();
	return;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////// End /////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void
MerkelMain::printHelp ()
{
	std::cout << "Help - your aim is to make money. Analyse the market and "
	             "make bids and offers. "
	          << std::endl;
}

void
MerkelMain::printMarketStats ()
{
	for (std::string const &p : orderBook.getKnownProducts ())
		{
			std::cout << "Product: " << p << std::endl;
			std::vector<OrderBookEntry> entries
			    = orderBook.getOrders (OrderBookType::ask, p, currentTime);
			std::cout << "Asks seen: " << entries.size () << std::endl;
			std::cout << "Max ask: " << OrderBook::getHighPrice (entries)
			          << std::endl;
			std::cout << "Min ask: " << OrderBook::getLowPrice (entries)
			          << std::endl;
		}
	// std::cout << "OrderBook contains :  " << orders.size() << " entries" <<
	// std::endl; unsigned int bids = 0; unsigned int asks = 0; for
	// (OrderBookEntry& e : orders)
	// {
	//     if (e.orderType == OrderBookType::ask)
	//     {
	//         asks ++;
	//     }
	//     if (e.orderType == OrderBookType::bid)
	//     {
	//         bids ++;
	//     }
	// }
	// std::cout << "OrderBook asks:  " << asks << " bids:" << bids <<
	// std::endl;
}

void
MerkelMain::enterAsk ()
{
	std::cout << "Make an ask - enter the amount: product,price, amount, eg  "
	             "ETH/BTC,200,0.5"
	          << std::endl;
	std::string input;
	std::getline (std::cin, input);

	std::vector<std::string> tokens = CSVReader::tokenise (input, ',');
	if (tokens.size () != 3)
		{
			std::cout << "MerkelMain::enterAsk Bad input! " << input
			          << std::endl;
		}
	else
		{
			try
				{
					OrderBookEntry obe = CSVReader::stringsToOBE (
					    tokens[1], tokens[2], currentTime, tokens[0],
					    OrderBookType::ask);
					obe.username = "simuser";
					if (wallet.canFulfillOrder (obe))
						{
							std::cout << "Wallet looks good. " << std::endl;
							orderBook.insertOrder (obe);
						}
					else
						{
							std::cout << "Wallet has insufficient funds . "
							          << std::endl;
						}
				}
			catch (const std::exception &e)
				{
					std::cout << " MerkelMain::enterAsk Bad input "
					          << std::endl;
				}
		}
}

void
MerkelMain::enterBid ()
{
	std::cout << "Make an bid - enter the amount: product,price, amount, eg  "
	             "ETH/BTC,200,0.5"
	          << std::endl;
	std::string input;
	std::getline (std::cin, input);

	std::vector<std::string> tokens = CSVReader::tokenise (input, ',');
	if (tokens.size () != 3)
		{
			std::cout << "MerkelMain::enterBid Bad input! " << input
			          << std::endl;
		}
	else
		{
			try
				{
					OrderBookEntry obe = CSVReader::stringsToOBE (
					    tokens[1], tokens[2], currentTime, tokens[0],
					    OrderBookType::bid);
					obe.username = "simuser";

					if (wallet.canFulfillOrder (obe))
						{
							std::cout << "Wallet looks good. " << std::endl;
							orderBook.insertOrder (obe);
						}
					else
						{
							std::cout << "Wallet has insufficient funds . "
							          << std::endl;
						}
				}
			catch (const std::exception &e)
				{
					std::cout << " MerkelMain::enterBid Bad input "
					          << std::endl;
				}
		}
}

void
MerkelMain::printWallet ()
{
	std::cout << wallet.toString () << std::endl;
}

void
MerkelMain::gotoNextTimeframe ()
{
	std::cout << "Going to next time frame. " << std::endl;
	for (std::string p : orderBook.getKnownProducts ())
		{
			std::cout << "matching " << p << std::endl;
			std::vector<OrderBookEntry> sales
			    = orderBook.matchAsksToBids (p, currentTime);
			std::cout << "Sales: " << sales.size () << std::endl;
			for (OrderBookEntry &sale : sales)
				{
					std::cout << "Sale price: " << sale.price << " amount "
					          << sale.amount << std::endl;
					if (sale.username == "simuser")
						{
							// update the wallet
							wallet.processSale (sale);
						}
				}
		}

	currentTime = orderBook.getNextTime (currentTime);
}

int
MerkelMain::getUserOption ()
{
	int userOption = 0;
	std::string line;
	std::cout << "Type in 1-9" << std::endl;  // edited
	std::getline (std::cin, line);
	try
		{
			userOption = std::stoi (line);
		}
	catch (const std::exception &e)
		{
			//
		}
	std::cout << "You chose: " << userOption << std::endl;
	return userOption;
}
void
MerkelMain::processUserOption (int userOption)
{
	if (userOption == 0)  // bad input
		{
			std::cout << "Invalid choice. Choose 1-9" << std::endl;  // editted
		}
	if (userOption == 1) { printHelp (); }
	if (userOption == 2) { printMarketStats (); }
	if (userOption == 3) { enterAsk (); }
	if (userOption == 4) { enterBid (); }
	if (userOption == 5) { printWallet (); }
	if (userOption == 6) { gotoNextTimeframe (); }
	///////////////////////////////////////////////////////////////////////////
	///////////////////////// Start ///////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	if (userOption == 7) { showCandleStickData (); }
	if (userOption == 8)
		{
			// User input product and type
			std::string line;
			std::cout << "Enter Product and type (e.g ETH/BTC ask)"
			          << std::endl;
			std::getline (std::cin, line);
			std::vector<std::string> tokens = CSVReader::tokenise (line, ' ');
			if (tokens.size () != 2)
				{
					std::cout << "Bad input! " << line << std::endl;
					return;
				}
			std::string product = tokens[0];

			// Convert user input type to OrderBookType
			OrderBookType type;
			if (tokens[1] == "ask") { type = OrderBookType::ask; }
			else if (tokens[1] == "bid") { type = OrderBookType::bid; }
			else
				{
					std::cout << "Invalid input for product and/or type"
					          << std::endl;
					return;
				}

			std::string title
			    = "CandleStick plot of " + product + " " + tokens[1];

			// Calculate all candlesticks from 2020-06-01.csv
			std::vector<CandleStick> candleSticks
			    = CSVReader::orderBookEntriestoCandleSticks (
			        CSVReader::readCSV ("20200601.csv"), type, product);
			plotCandleStickData (candleSticks, title);
		}

	if (userOption == 9)
		{
			// Read candlesticks from DAI-USD.csv
			std::vector<CandleStick> candleSticks
			    = CSVReader::readCSVtoCandleSticks ("DAI-USD.csv");

			std::string title = "CandleStick plot of DAI-USD";

			plotCandleStickData (candleSticks, title);
		}

	///////////////////////////////////////////////////////////////////////////
	//////////////////////////////// End //////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////
}

// CSVReader.cpp
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