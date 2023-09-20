#pragma once

#include <vector>
#include "OrderBook.h"
#include "Wallet.h"

class Advisorbot
{
public:
    Advisorbot();

    /** Call this to start the sim */
    void init(OrderBook inputOrderBook, std::string inputCurrentTime);

    // set the counter
    int nTime;
    // set the current time
    std::string currentTime;

    // calls the commands
    std::string getHelpUserOption();
    void processHelpUserOption();
    void processHelpUserOption(std::string helpUserOption);
    void processHelpCommand(std::string helpCommand);
    void processHelpProductCommand(std::string prodCommand);
    void processHelpMinCommand(std::string minCommand);
    void processHelpMaxCommand(std::string maxCommand);
    void processHelpAverageCommand(std::string avgCommand);
    void processHelpPredCommand(std::string preCommand);
    void processHelpTimeCommand(std::string timeCommand);
    void processHelpGrowthCommand(std::string groCommand);

private:
    OrderBook orderBook;
};
