#include <string>

enum class OrderType
{
    Bid,
    Ask
};

class OrderBookEntry
{
public:

    OrderBookEntry(){};

    OrderBookEntry(double price,
                   double amount,
                   std::string& timestamp,
                   std::string& product,
                   OrderType orderType);

    // Accessor functions
    double getPrice() const;
    double geAmount() const;
    std::string getTimeStamp() const;
    std::string getProduct() const;
    OrderType getOrderType() const;

private:
    double m_price;
    double m_amount;
    std::string m_timestamp;
    std::string m_product;
    OrderType m_orderType;

};
