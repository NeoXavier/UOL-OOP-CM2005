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
