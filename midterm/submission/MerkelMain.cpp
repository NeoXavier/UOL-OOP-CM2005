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
