#include "MerkelMain.h"
#include <iostream>
#include <string>
#include "Advisorbot.h"

MerkelMain::MerkelMain()
{
}
// initialise variables and set values at time of construction
void MerkelMain::init()
{
    // set orderBook and currentTime variables from input
    int input;
    currentTime = orderBook.getEarliestTime();
    // call insertCurrency() from function
    wallet.insertCurrency("BTC", 10);
    // calls advisorbot initialization
    advisorBot.init(orderBook, currentTime);

    while (true)
    {
        // print menu, read user input, respond to user input loop
        printMenu();
        input = getUserOption();
        processUserOption(input);
    }
}
// print out menu options
void MerkelMain::printMenu()
{
    currentTime = advisorBot.currentTime;
    nTime = advisorBot.nTime;
    // 1 print help
    std::cout << "1: Print help " << std::endl;
    // 2 print exchange stats
    std::cout << "2: Print exchange stats" << std::endl;
    // 3 make an offer
    std::cout << "3: Make an offer " << std::endl;
    // 4 make a bid
    std::cout << "4: Make a bid " << std::endl;
    // 5 print wallet
    std::cout << "5: Print wallet " << std::endl;
    // 6 continue
    std::cout << "6: Continue " << std::endl;

    std::cout << "============== " << std::endl;

    std::cout << "Current time is: " << currentTime << std::endl;
}

void MerkelMain::printHelp()
{
    // it calls advisorbot menu
    advisorBot.processHelpUserOption();
}

void MerkelMain::printMarketStats()
{ // loops through products and print out the stats
    for (std::string const &p : orderBook.getKnownProducts())
    {
        std::cout << "Product: " << p << std::endl;
        std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::ask, p, currentTime);
        std::cout << "Asks seen: " << entries.size() << std::endl;
        std::cout << "Max ask price: " << OrderBook::getHighPrice(entries) << std::endl;
        std::cout << "Min ask price: " << OrderBook::getLowPrice(entries) << std::endl;
        std::cout << "Average price ask: " << OrderBook::getAveragePrice(entries) << std::endl;
        std::cout << "Average amount ask: " << OrderBook::getAverageAmount(entries) << std::endl;
        std::cout << "Median price ask: " << OrderBook::getMedianPrice(entries) << std::endl;
        std::cout << "Ask exchange rate: " << OrderBook::getExchangeRate(entries) << std::endl;
    }
}

void MerkelMain::enterAsk()
{
    std::cout << "Make an offer - enter the amount: product, price, amount, eg. ETH/BTC, 200, 0.5 " << std::endl;
    std::string input;
    std::getline(std::cin, input);

    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    // check the input length
    if (tokens.size() != 3)
    {
        std::cout << "MerkelMain::enterAsk Bad input! " << input << std::endl;
    }
    else
    {
        // ask
        try
        {
            // call CSVReader::tokenise on each line, and then pass the
            // resulting tokens to stringsToOBE
            OrderBookEntry obe = CSVReader::stringsToOBE(
                tokens[1],
                tokens[2],
                currentTime,
                tokens[0],
                OrderBookType::ask);
            obe.username = "simuser";
            // allows to check if it
            // can fulfill a given OrderBookEntry object
            if (wallet.canFulfillOrder(obe))
            {
                std::cout << "Wallet looks good. " << std::endl;
                orderBook.insertOrder(obe);
            }
            else
            {
                std::cout << "Wallet has insufficient funds . " << std::endl;
            }
        }
        catch (const std::exception &e)
        {
            std::cout << " MerkelMain::enterAsk Bad input " << std::endl;
        }
    }
    std::cout << "You typed: " << input << std::endl;
}

void MerkelMain::enterBid()
{
    std::cout << "Make an bid - enter the amount: product,price, amount, eg  ETH/BTC,200,0.5" << std::endl;
    std::string input;
    std::getline(std::cin, input);

    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    // check the input length
    if (tokens.size() != 3)
    {
        std::cout << "MerkelMain::enterBid Bad input! " << input << std::endl;
    }
    else
    {
        // bid
        try
        {
            // call CSVReader::tokenise on each line, and then pass the
            // resulting tokens to stringsToOBE
            OrderBookEntry obe = CSVReader::stringsToOBE(
                tokens[1],
                tokens[2],
                currentTime,
                tokens[0],
                OrderBookType::bid);
            obe.username = "simuser";
            // allows to check if it
            // can fulfill a given OrderBookEntry object
            if (wallet.canFulfillOrder(obe))
            {
                std::cout << "Wallet looks good. " << std::endl;
                orderBook.insertOrder(obe);
            }
            else
            {
                std::cout << "Wallet has insufficient funds . " << std::endl;
            }
        }
        catch (const std::exception &e)
        {
            std::cout << " MerkelMain::enterBid Bad input " << std::endl;
        }
    }
    std::cout << "You typed: " << input << std::endl;
}

void MerkelMain::printWallet()
{
    std::cout << wallet.toString() << std::endl;
}

void MerkelMain::gotoNextTimeframe()
{
    std::cout << "Going to next time frame. " << std::endl;
    // loops through products
    for (std::string p : orderBook.getKnownProducts())
    {
        std::cout << "matching " << p << std::endl;
        // return sale size after matching
        std::vector<OrderBookEntry> sales = orderBook.matchAsksToBids(p, currentTime);
        std::cout << "Sales: " << sales.size() << std::endl;
        for (OrderBookEntry &sale : sales)
        {
            std::cout << "Sale price: " << sale.price << " amount " << sale.amount << std::endl;
            // checks the username on the sale
            if (sale.username == "simuser")
            {
                // update the wallet
                wallet.processSale(sale);
            }
        }
    }
    // updates current time and counter
    currentTime = orderBook.getNextTime(currentTime);
    nTime++;
    advisorBot.currentTime = currentTime;
    advisorBot.nTime = nTime;
}
// prompt the user to select a choice
int MerkelMain::getUserOption()
{
    int userOption = 0;
    std::string line;
    std::cout << "Type in 1-6" << std::endl;
    std::getline(std::cin, line);
    try
    {
        userOption = std::stoi(line);
    }
    catch (const std::exception &e)
    {
        //
    }
    // return the user's choice
    std::cout << "You chose: " << userOption << std::endl;
    return userOption;
}
// Do something based on the user input
void MerkelMain::processUserOption(int userOption)
{
    // bad input
    if (userOption == 0)
    {
        std::cout << "Invalid choice. Choose 1-6" << std::endl;
    }
    if (userOption == 1)
    {
        printHelp();
    }
    if (userOption == 2)
    {
        printMarketStats();
    }
    if (userOption == 3)
    {
        enterAsk();
    }
    if (userOption == 4)
    {
        enterBid();
    }
    if (userOption == 5)
    {
        printWallet();
    }
    if (userOption == 6)
    {
        gotoNextTimeframe();
    }
}
