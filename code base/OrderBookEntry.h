#pragma once

#include <string>

enum class OrderBookType
{
    bid,
    ask,
    unknown,
    asksale,
    bidsale
};

class OrderBookEntry
{
public:
    /** Create a new OrderBookEntry with the price
     * set to the sent value
     */

    // This implementation takes the incoming argument _price and assigns it to the
    // OrderBookEntry data member price.
    OrderBookEntry(double _price,
                   double _amount,
                   std::string _timestamp,
                   std::string _product,
                   OrderBookType _orderType,
                   std::string username = "dataset");

    static OrderBookType stringToOrderBookType(std::string s);
    // allows sorting by timestamp
    static bool compareByTimestamp(OrderBookEntry &e1, OrderBookEntry &e2)
    {
        return e1.timestamp < e2.timestamp;
    }
    // allows sorting bids or asks by ascending price
    static bool compareByPriceAsc(OrderBookEntry &e1, OrderBookEntry &e2)
    {
        return e1.price < e2.price;
    }
    // allows sorting bids or asks by descending price
    static bool compareByPriceDesc(OrderBookEntry &e1, OrderBookEntry &e2)
    {
        return e1.price > e2.price;
    }

    double price;
    double amount;
    std::string timestamp;
    std::string product;
    OrderBookType orderType;
    std::string username;
};
