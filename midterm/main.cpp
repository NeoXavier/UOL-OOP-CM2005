// #include "OrderBookEntry.h"
// #include "Wallet.h"
// #include <iostream>
#include <string>
// #include <vector>
// #include "MerkelMain.h"
#include "CSVReader.h"
#include "Plotter.h"

int
main ()
{
	// MerkelMain app{};
	// app.init();

	// Checking candlestick vector
	std::vector<CandleStick> candleSticks
	    = CSVReader::orderBookEntriestoCandleSticks (
	        CSVReader::readCSV ("20200601.csv"), OrderBookType::ask,
	        "ETH/BTC");
	std::vector<CandleStick> firstTenCandleSticks (candleSticks.begin (),
	                                               candleSticks.begin () + 20);
	// for(CandleStick& c: candleSticks){
	// std::cout << c.timestamp << std::endl;
	// std::cout << "Open: " << c.open << " Close: " << c.close << " High: " <<
	// c.high << " Low: " << c.low << std::endl;
	//}

	Plotter plt{ firstTenCandleSticks };
	plt.plot ();
}
