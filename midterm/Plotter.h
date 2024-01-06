//////////////////////////////////////////////////////////////////////////////
////////////////////////////// Start /////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "CandleStick.h"

class Plotter
{
  public:
	Plotter (std::vector<CandleStick> _candleSticks, int _plotHeight,
             int _plotWidth);
	void plot ();

  private:
	std::vector<CandleStick> candleSticks;
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
