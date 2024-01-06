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
    // Testing using a bigger dataset, adjust plot width
    //
    //std::vector<CandleStick> candleSticks
        //= CSVReader::orderBookEntriestoCandleSticks (
            //CSVReader::readCSV ("20200601.csv"), OrderBookType::ask,
            //"ETH/BTC");
    //std::vector<CandleStick> firstTenCandleSticks (candleSticks.begin (),
                                                   //candleSticks.begin () + 20);

    std::vector<CandleStick> candleSticks
        = CSVReader::orderBookEntriestoCandleSticks (
            CSVReader::readCSV ("20200317.csv"), OrderBookType::ask,
            "ETH/BTC");

	Plotter plt{ candleSticks, 20, 100 };
	plt.plot ();
}
