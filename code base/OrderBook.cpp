#include "OrderBook.h"
#include "CSVReader.h"
#include <map>
#include <algorithm>
#include <iostream>
#include "Advisorbot.h"
#include "MerkelMain.h"

OrderBook::OrderBook()
{
}

/** construct, reading a csv data file */
OrderBook::OrderBook(std::string filename)
{
    orders = CSVReader::readCSV(filename);
}

/** return vector of all know products in the dataset*/
std::vector<std::string> OrderBook::getKnownProducts()
{
    std::vector<std::string> products;
    // map from strings to function pointers
    std::map<std::string, bool> prodMap;
    // data structure mapping a unique set of product names
    for (OrderBookEntry &e : orders)
    {
        prodMap[e.product] = true;
    }

    // now flatten the map to a vector of strings
    for (auto const &e : prodMap)
    {
        products.push_back(e.first);
    }

    return products;
}

/** return vector of all know timesteps in the dataset*/
std::vector<std::string> OrderBook::getAllTimestamps()
{
    std::vector<std::string> alltimestamps;

    std::map<std::string, bool> prodMap;

    for (OrderBookEntry &e : orders)
    {
        prodMap[e.timestamp] = true;
    }

    // now flatten the map to a vector of strings
    for (auto const &e : prodMap)
    {
        alltimestamps.push_back(e.first);
    }

    return alltimestamps;
}

/** return vector of Orders according to the sent filters*/
std::vector<OrderBookEntry> OrderBook::getOrders(OrderBookType type,
                                                 std::string product,
                                                 std::string timestamp)
{
    std::vector<OrderBookEntry> orders_sub;
    for (OrderBookEntry &e : orders)
    {
        //
        if (e.orderType == type &&
            e.product == product &&
            e.timestamp == timestamp)
        {
            orders_sub.push_back(e);
        }
    }
    return orders_sub;
}

double OrderBook::getHighPrice(std::vector<OrderBookEntry> &orders)
{
    double max = orders[0].price;
    for (OrderBookEntry &e : orders)
    {
        if (e.price > max)
            max = e.price;
    }
    return max;
}

double OrderBook::getLowPrice(std::vector<OrderBookEntry> &orders)
{
    double min = orders[0].price;
    for (OrderBookEntry &e : orders)
    {
        if (e.price < min)
            min = e.price;
    }
    return min;
}

double OrderBook::getAveragePrice(std::vector<OrderBookEntry> &orders)
{
    double sum = 0.0;
    double d_AvgPrice;
    for (OrderBookEntry &e : orders)
    {
        sum += (double)e.price;
    }
    return d_AvgPrice = sum / orders.size();
}

double OrderBook::getAverageAmount(std::vector<OrderBookEntry> &orders)
{
    double sum = 0.0;
    double d_AvgAmount;
    for (OrderBookEntry &e : orders)
    {
        sum += (double)e.amount;
    }
    return d_AvgAmount = sum / orders.size();
}

double OrderBook::getExchangeRate(std::vector<OrderBookEntry> &orders)
{
    double d_AvgAmount = OrderBook::getAverageAmount(orders);
    double d_AvgPrice = OrderBook::getAveragePrice(orders);

    return (d_AvgPrice / d_AvgAmount);
}

double OrderBook::getMedianPrice(std::vector<OrderBookEntry> &orders)
{
    std::size_t n = orders.size() / 2;
    for (OrderBookEntry &e : orders)
    {
        std::sort(orders.begin(), orders.end(), OrderBookEntry::compareByTimestamp);
        std::nth_element(orders.begin(), orders.begin() + n, orders.end(), OrderBookEntry::compareByTimestamp);
    }

    return orders[n].price;
}

std::string OrderBook::getEarliestTime()
{
    return orders[0].timestamp;
}

std::string OrderBook::getNextTime(std::string timestamp)
{
    std::string next_timestamp = "";
    for (OrderBookEntry &e : orders)
    {
        if (e.timestamp > timestamp)
        {
            next_timestamp = e.timestamp;
            break;
        }
    }
    if (next_timestamp == "")
    {
        next_timestamp = orders[0].timestamp;
    }
    return next_timestamp;
}

void OrderBook::insertOrder(OrderBookEntry &order)
{
    orders.push_back(order);
    std::sort(orders.begin(), orders.end(), OrderBookEntry::compareByTimestamp);
}

// The function returns a vector of OrderBookEntry items which have their orderType set to sale.
std::vector<OrderBookEntry> OrderBook::matchAsksToBids(std::string product, std::string timestamp)
{

    std::vector<OrderBookEntry> asks = getOrders(OrderBookType::ask,
                                                 product,
                                                 timestamp);

    std::vector<OrderBookEntry> bids = getOrders(OrderBookType::bid,
                                                 product,
                                                 timestamp);

    std::vector<OrderBookEntry> sales;

    // I put in a little check to ensure we have bids and asks
    // to process.
    if (asks.size() == 0 || bids.size() == 0)
    {
        std::cout << " OrderBook::matchAsksToBids no bids or asks" << std::endl;
        return sales;
    }

    // sort asks lowest first
    std::sort(asks.begin(), asks.end(), OrderBookEntry::compareByPriceAsc);
    // sort bids highest first
    std::sort(bids.begin(), bids.end(), OrderBookEntry::compareByPriceDesc);
    // for ask in asks:
    std::cout << "max ask " << asks[asks.size() - 1].price << std::endl;
    std::cout << "min ask " << asks[0].price << std::endl;
    std::cout << "max bid " << bids[0].price << std::endl;
    std::cout << "min bid " << bids[bids.size() - 1].price << std::endl;

    for (OrderBookEntry &ask : asks)
    {

        for (OrderBookEntry &bid : bids)
        {

            if (bid.price >= ask.price)
            {

                OrderBookEntry sale{ask.price, 0, timestamp,
                                    product,
                                    OrderBookType::asksale};

                if (bid.username == "simuser")
                {
                    sale.username = "simuser";
                    sale.orderType = OrderBookType::bidsale;
                }
                if (ask.username == "simuser")
                {
                    sale.username = "simuser";
                    sale.orderType = OrderBookType::asksale;
                }

                // now work out how much was sold and
                // create new bids and asks covering
                // anything that was not sold
                // bid completely clears ask
                if (bid.amount == ask.amount)
                {

                    sale.amount = ask.amount;

                    sales.push_back(sale);

                    bid.amount = 0;
                    // can do no more with this ask
                    // go onto the next ask
                    break;
                }
                if (bid.amount > ask.amount)
                {

                    sale.amount = ask.amount;

                    sales.push_back(sale);
                    // we adjust the bid in place
                    // so it can be used to process the next ask

                    bid.amount = bid.amount - ask.amount;
                    // ask is completely gone, so go to next ask
                    break;
                }

                if (bid.amount < ask.amount &&
                    bid.amount > 0)
                {

                    sale.amount = bid.amount;

                    sales.push_back(sale);
                    // update the ask
                    // and allow further bids to process the remaining amount
                    ask.amount = ask.amount - bid.amount;
                    // make sure the bid is not processed again
                    bid.amount = 0;
                    // some ask remains so go to the next bid
                    continue;
                }
            }
        }
    }
    return sales;
}
