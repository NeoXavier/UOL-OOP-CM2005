#include "Plotter.h"
#include <iostream>

Plotter::Plotter (std::vector<CandleStick> _candleSticks, int _plotHeight,
                  int _plotWidth)
    : candleSticks{ _candleSticks }, plotHeight{ _plotHeight },
      plotWidth{ _plotWidth }
{
	std::sort (candleSticks.begin (), candleSticks.end (),
	           CandleStick::compareByTimestamp);
	this->minPrice = getMinPrice (candleSticks) * 0.995;
	this->maxPrice = getMaxPrice (candleSticks) * 1.005;

	this->numOfCandleSticks = candleSticks.size ();

	this->step = (maxPrice - minPrice) / plotHeight;
	this->columnWidth = (plotWidth - numOfCandleSticks) / numOfCandleSticks;

    // Minimum column width is 10
    if (columnWidth < 10) {
        columnWidth = 10;
        numOfCandleSticks = plotWidth / 11;
        std::vector<CandleStick> newCandleSticks(candleSticks.begin(),
                                                 candleSticks.begin() +
                                                 numOfCandleSticks);
        candleSticks = newCandleSticks;

        // Warning message
        std::cout << "Warning: Too many candlesticks to plot. Plotting "
                  << numOfCandleSticks << " candlesticks instead." << std::endl;
    }

	std::string spacer = " ";

	//// Intializing plot
	for (int row = 0; row < plotHeight+2; row++)
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
                        if(row == plotHeight)
                        {
                            // Drawing x-axis
                            line.push_back(fixLength("Date", 10));
                            continue;
                        }
                        // Time label on y-axis
                        if(row == plotHeight+1)
                        {
                            // Drawing x-axis
                            line.push_back(fixLength("Time", 10));
                            continue;
                        }

                        std::string tick = fixLength (
                                std::to_string (maxPrice - (step * row)), 10);
                        line.push_back (tick);
                        continue;
                    }

					line.push_back(spacer);
				}
			plotLines.push_back (line);
		}
}

void
Plotter::plot ()
{
	for (int i = 0; i < candleSticks.size (); i++)
		{
			drawCandleStickOnPlot (candleSticks[i], (i * 2) + 1);
		}
	// printing plot
	for (std::vector<std::string> &line : plotLines)
		{
			for (std::string s : line) { std::cout << s; }
			std::cout << std::endl;
		}
}

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
    plotLines[plotHeight][col] = fixLength(candleStick.timestamp.substr(0, 10), columnWidth);
    // Time
    plotLines[plotHeight+1][col] = fixLength(candleStick.timestamp.substr(11, 8), columnWidth);
}

int
Plotter::valueToLineNumber (double value, double step, double maxPrice)
{
	int lineNumber;
	for (int i = 0; i <= plotHeight; i++)
		{
			if (value > (maxPrice - (step * i)))
				{
                    //lineNumber = i - 1;
                    lineNumber = i;
					break;
				}
		}
	return lineNumber;
}

// Helper Functions
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

std::string
Plotter::fixLength (std::string str, int length)
{
	if (str.size () > length)
		{
			// If the string is too long, trim it from the back
			str = str.substr (0, length);
		}
	if (str.size () < length)
		{
			// If the string is too short, add spaces at the back
			str.append (length - str.size (), ' ');
		}
	return str;
}
