#include "CandleStick.h"

CandleStick::CandleStick (std::string _timestamp, double _open, double _high,
                          double _low, double _close)
    : timestamp (_timestamp), open (_open), high (_high), low (_low),
      close (_close)
{
  // Assign candlestick type based on bearish or bullish
  if (close > open)
    {
      this->type = CandleStickType::bullish;
    }
  else
    {
      this->type = CandleStickType::bearish;
    }
}

bool
CandleStick::compareByTimestamp (CandleStick &c1, CandleStick &c2)
{
  return c1.timestamp < c2.timestamp;
}
