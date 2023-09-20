#pragma once

#include <vector>
#include "OrderBookEntry.h"
#include "OrderBook.h"
#include "Wallet.h"
#include "Advisorbot.h"

class MerkelMain
{
public:
    MerkelMain();
    /** Call this to start the sim */
    void init();
    int nTime;
    std::string currentTime;
    void gotoNextTimeframe();
    void printMenu();

private:
    void printHelp();
    void printMarketStats();
    void enterAsk();
    void enterBid();
    void printWallet();

    int getUserOption();
    void processUserOption(int userOption);

    Advisorbot advisorBot;
    OrderBook orderBook{"20200601.csv"};
    Wallet wallet;
};
