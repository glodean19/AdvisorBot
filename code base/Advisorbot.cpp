#include "MerkelMain.h"
#include <iostream>
#include <vector>
#include "OrderBookEntry.h"
#include "CSVReader.h"
#include "OrderBook.h"
#include <string>
#include "Advisorbot.h"

Advisorbot::Advisorbot()
{
}
// initialise variables and set values at time of construction
void Advisorbot::init(OrderBook inputOrderBook, std::string inputCurrentTime)
{
    // set orderBook and currentTime variables from input
    orderBook = inputOrderBook;
    currentTime = inputCurrentTime;
}

// get input from the user
std::string Advisorbot::getHelpUserOption()
{
    std::string helpUserOption;
    std::getline(std::cin, helpUserOption);
    return helpUserOption;
}
// verification of the help command
void Advisorbot::processHelpCommand(std::string helpCommand)
{
    // tokenise the command
    std::vector<std::string> commandParts;

    std::string::size_type prev_pos = 0, pos = 0;
    // find position of whitespaces
    while ((pos = helpCommand.find(" ", pos)) != std::string::npos)
    { // create substrings for each inputted word
        std::string substring(helpCommand.substr(prev_pos, pos - prev_pos));
        // store substrings in vector
        commandParts.push_back(substring);
        // assign current position + 1 to previous position
        prev_pos = ++pos;
    }
    // store the last word as substring in the vector
    commandParts.push_back(helpCommand.substr(prev_pos, pos - prev_pos));
    // verification input length
    if ((int)commandParts.size() != 2)
    {
        std::cout << "Invalid command format" << std::endl;
        return;
    }

    // Verification first part for the word help
    if (commandParts[0].compare("help") == 0)
    {
        // verification second part for all other commands
        if (commandParts[1].compare("prod") == 0)
        {
            std::cout << "List of all available products" << std::endl;
            return;
        }
        else if (commandParts[1].compare("min") == 0)
        {
            std::cout << "Find minimum bid or ask for product in current time step" << std::endl;
            return;
        }
        else if (commandParts[1].compare("max") == 0)
        {
            std::cout << "Find maximum bid or ask for product in current time step" << std::endl;
            return;
        }
        else if (commandParts[1].compare("avg") == 0)
        {
            std::cout << "Compute average ask or bid for the sent product over the sent number of time steps" << std::endl;
            return;
        }
        else if (commandParts[1].compare("predict") == 0)
        {
            std::cout << "Predict max or min ask or bid for the sent product for the next time step" << std::endl;
            return;
        }
        else if (commandParts[1].compare("time") == 0)
        {
            std::cout << "State current time in dataset (i.e. which timeframe are we looking at)" << std::endl;
            return;
        }
        else if (commandParts[1].compare("step") == 0)
        {
            std::cout << "Move to next time step" << std::endl;
            return;
        }
        else if (commandParts[1].compare("growth") == 0)
        {
            std::cout << "Compute the growth ask or bid for the sent product over the sent number of time steps using the average on those times" << std::endl;
            return;
        }
        else if (commandParts[1].compare("exit") == 0)
        {
            std::cout << "Return to the MerkelMain menu" << std::endl;
            return;
        }
        else
        {
            std::cout << "Invalid command format" << std::endl;
            return;
        }
    }
}
// verification of the min command
void Advisorbot::processHelpMinCommand(std::string minCommand)
{
    // tokenise the command
    std::vector<std::string> commandParts;

    std::string::size_type prev_pos = 0, pos = 0;
    // find position of whitespaces
    while ((pos = minCommand.find(" ", pos)) != std::string::npos)
    { // create substrings for each inputted word
        std::string substring(minCommand.substr(prev_pos, pos - prev_pos));
        // store substrings in vector
        commandParts.push_back(substring);
        // assign current position + 1 to previous position
        prev_pos = ++pos;
    }
    // store the last word as substring in the vector
    commandParts.push_back(minCommand.substr(prev_pos, pos - prev_pos));
    // verification input length
    if ((int)commandParts.size() != 3)
    {
        std::cout << "Invalid command format" << std::endl;
        return;
    }
    // verification of first part of the command
    if (commandParts[0].compare("min") != 0)
    {
        std::cout << "Invalid command format" << std::endl;
        return;
    }

    // Verification of second part of command
    bool isProductDefined = false;
    std::vector<std::string> products;
    // assign products using getKnownProducts() rom MerkelMain
    products = orderBook.getKnownProducts();

    for (int i = 0; i < (int)products.size(); i++)
    { // verification of the user input, compared with the list of products
        if (commandParts[1].compare(products[i]) == 0)
        { // if product found, stop the loop
            isProductDefined = true;
            break;
        }
    }
    // if product not found, return the message
    if (!isProductDefined)
    {
        std::cout << "Invalid command format" << std::endl;
        return;
    }
    // assign user input to OrderBookEntry type
    OrderBookType type = OrderBookEntry::stringToOrderBookType(commandParts[2]);

    // Verification of third part of the command
    if (type == OrderBookType::unknown)
    { // if type is unknown, return error
        std::cout << "Invalid command format" << std::endl;
        return;
    }

    // Get selected entries using getOrders() from MerkelMain
    std::vector<OrderBookEntry> entries;
    entries = orderBook.getOrders(type, commandParts[1], currentTime);
    // calculate min using getLowPrice() from MerkelMain
    double minPrice = OrderBook::getLowPrice(entries);

    std::cout << "The min " << commandParts[2] << " for " << commandParts[1] << " at " << currentTime << " is " << minPrice << std::endl;
}

void Advisorbot::processHelpMaxCommand(std::string maxCommand)
{
    // tokenise the command
    std::vector<std::string> commandParts;

    std::string::size_type prev_pos = 0, pos = 0;
    // find position of whitespaces
    while ((pos = maxCommand.find(" ", pos)) != std::string::npos)
    { // create substrings for each inputted word
        std::string substring(maxCommand.substr(prev_pos, pos - prev_pos));
        // store substrings in vector
        commandParts.push_back(substring);
        // assign current position + 1 to previous position
        prev_pos = ++pos;
    }
    // store the last word as substring in the vector
    commandParts.push_back(maxCommand.substr(prev_pos, pos - prev_pos));

    if ((int)commandParts.size() != 3)
    {
        std::cout << "Invalid command format" << std::endl;
        return;
    }
    // verification input length
    if (commandParts[0].compare("max") != 0)
    {
        std::cout << "Invalid command format" << std::endl;
        return;
    }

    // Verification of second part of command
    bool isProductDefined = false;
    std::vector<std::string> products;
    // assign products using getKnownProducts() rom MerkelMain
    products = orderBook.getKnownProducts();

    for (int i = 0; i < (int)products.size(); i++)
    { // verification of the user input, compared with the list of products
        if (commandParts[1].compare(products[i]) == 0)
        { // if product found, stop the loop
            isProductDefined = true;
            break;
        }
    }
    // if product not found, return the message
    if (!isProductDefined)
    {
        std::cout << "Invalid command format" << std::endl;
        return;
    }
    // assign user input to OrderBookEntry type
    OrderBookType type = OrderBookEntry::stringToOrderBookType(commandParts[2]);

    // Verification of third part of the command
    if (type == OrderBookType::unknown)
    { // if type is unknown, return error
        std::cout << "Invalid command format" << std::endl;
        return;
    }

    // Get selected entries using getOrders() from MerkelMain
    std::vector<OrderBookEntry> entries;
    entries = orderBook.getOrders(type, commandParts[1], currentTime);
    // calculate max using getLowPrice() from MerkelMain
    double maxPrice = OrderBook::getHighPrice(entries);

    std::cout << "The max " << commandParts[2] << " for " << commandParts[1] << " at " << currentTime << " is " << maxPrice << std::endl;
}

void Advisorbot::processHelpAverageCommand(std::string avgCommand)
{
    // tokenise the command
    std::vector<std::string> commandParts;

    std::string::size_type prev_pos = 0, pos = 0;
    // find position of whitespaces
    while ((pos = avgCommand.find(" ", pos)) != std::string::npos)
    { // create substrings for each inputted word
        std::string substring(avgCommand.substr(prev_pos, pos - prev_pos));
        // store substrings in vector
        commandParts.push_back(substring);
        // assign current position + 1 to previous position
        prev_pos = ++pos;
    }
    // store the last word as substring in the vector
    commandParts.push_back(avgCommand.substr(prev_pos, pos - prev_pos));

    if ((int)commandParts.size() != 4)
    {
        std::cout << "Invalid command format" << std::endl;
        return;
    }
    // verification input length
    if (commandParts[0].compare("avg") != 0)
    {
        std::cout << "Invalid command format" << std::endl;
        return;
    }

    // Verification of second part of command
    bool isProductDefined = false;
    std::vector<std::string> products;
    // assign products using getKnownProducts() rom MerkelMain
    products = orderBook.getKnownProducts();

    for (int i = 0; i < (int)products.size(); i++)
    { // verification of the user input, compared with the list of products
        if (commandParts[1].compare(products[i]) == 0)
        { // if product found, stop the loop
            isProductDefined = true;
            break;
        }
    }
    // if product not found, return the message
    if (!isProductDefined)
    {
        std::cout << "Invalid command format" << std::endl;
        return;
    }
    // assign user input to OrderBookEntry type
    OrderBookType type = OrderBookEntry::stringToOrderBookType(commandParts[2]);

    // Verification of third part of the command
    if (type == OrderBookType::unknown)
    { // if type is unknown, return error
        std::cout << "Invalid command format" << std::endl;
        return;
    }

    // Verification of fourth part of the command
    int steps = stoi(commandParts[3]);

    if (steps < 0)
    {
        std::cout << "Invalid number of time steps entered. The number of steps must be positive" << std::endl;
        return;
    }

    std::vector<OrderBookEntry> entries;
    std::vector<std::string> alltimestamps;
    double sumavg = 0;
    // get all available timestamps from getAllTimestamps() in orderBook and store it in vector
    alltimestamps = orderBook.getAllTimestamps();
    // check if there's enough available data backward for inputted steps
    if (steps > nTime)
    {
        std::cout << "Invalid number of time steps entered. Maximum available data is " << nTime << " steps." << std::endl;
    }
    else
    {

        // loop through current time and all steps backwards
        for (int i = nTime - steps; i <= nTime; i++)
        {
            // Get selected entries using getOrders() from MerkelMain
            entries = orderBook.getOrders(type, commandParts[1], alltimestamps[i]);
            // calculate the average passing entries in getAveragePrice() from MerkelMain
            double avgPrice = OrderBook::getAveragePrice(entries);
            // sums all averages returned from getAveragePrice()
            sumavg = sumavg + avgPrice;
        }

        std::cout << "The average " << commandParts[1] << " " << commandParts[2] << " price over the last " << commandParts[3] << " timesteps"
                  // divide the sum of averages by the number of steps + current
                  << " was " << sumavg / (steps + 1) << std::endl;
    }
}

void Advisorbot::processHelpPredCommand(std::string preCommand)
{
    // tokenise the command
    std::vector<std::string> commandParts;

    std::string::size_type prev_pos = 0, pos = 0;
    // find position of whitespaces
    while ((pos = preCommand.find(" ", pos)) != std::string::npos)
    { // create substrings for each inputted word
        std::string substring(preCommand.substr(prev_pos, pos - prev_pos));
        // store substrings in vector
        commandParts.push_back(substring);
        // assign current position + 1 to previous position
        prev_pos = ++pos;
    }
    // store the last word as substring in the vector
    commandParts.push_back(preCommand.substr(prev_pos, pos - prev_pos));
    // verification input length
    if ((int)commandParts.size() != 4)
    {
        std::cout << "Invalid command format" << std::endl;
        return;
    }

    if (commandParts[0].compare("predict") != 0)
    {
        std::cout << "Invalid command format" << std::endl;
        return;
    }

    // Verification of second part of the command
    if (commandParts[1].compare("max") != 0)
    {

        if (commandParts[1].compare("min") != 0)
        {
            std::cout << "Invalid command format" << std::endl;
            return;
        }
    }

    // Verification of second part of command
    bool isProductDefined = false;
    std::vector<std::string> products;
    // assign products using getKnownProducts() rom MerkelMain
    products = orderBook.getKnownProducts();

    for (int i = 0; i < (int)products.size(); i++)
    { // verification of the user input, compared with the list of products
        if (commandParts[2].compare(products[i]) == 0)
        { // if product found, stop the loop
            isProductDefined = true;
            break;
        }
    }
    // if product not found, return the message
    if (!isProductDefined)
    {
        std::cout << "Invalid command format" << std::endl;
        return;
    }
    // assign user input to OrderBookEntry type
    OrderBookType type = OrderBookEntry::stringToOrderBookType(commandParts[3]);

    // Verification of third part of the command
    if (type == OrderBookType::unknown)
    { // if type is unknown, return error
        std::cout << "Invalid command format" << std::endl;
        return;
    }

    std::vector<OrderBookEntry> entries;
    std::vector<std::string> alltimestamps;
    std::vector<double> max_min;
    // get all available timestamps from getAllTimestamps() in orderBook and store it in vector
    alltimestamps = orderBook.getAllTimestamps();
    // check if there is a minimum of 2 time steps backwards
    if (nTime < 2)
    {
        std::cout << "Not enough data for a prediction" << std::endl;
    }
    else
    {

        // loops through current time and previous 2 time steps
        for (int i = nTime - 2; i <= nTime; i++)
        {
            // Get selected entries using getOrders() from MerkelMain
            entries = orderBook.getOrders(type, commandParts[2], alltimestamps[i]);
            // verification of the user input for max or min
            if (commandParts[1].compare("max") == 0)
            {
                // calcuate max for each time step, passing entries in the getHighPrice() from MerkelMain
                double max = OrderBook::getHighPrice(entries);
                // store returned value for each time step in a vector
                max_min.push_back(max);
            }
            else
            { // calcuate min for each time step, passing entries in the getLowPrice() from MerkelMain
                double min = OrderBook::getLowPrice(entries);
                // store returned value for each time step in a vector
                max_min.push_back(min);
            }
        }

        std::cout << "The predicted  " << commandParts[1] << " value for " << commandParts[2] << " " << commandParts[3] << " at " << alltimestamps[nTime + 1]
                  // applies weight to each value for prediction
                  << " is " << (max_min[0] * 0.1) + (max_min[1] * 0.3) + (max_min[2] * 0.6) << std::endl;
    }
}

void Advisorbot::processHelpGrowthCommand(std::string groCommand)
{
    // tokenise the command
    std::vector<std::string> commandParts;

    std::string::size_type prev_pos = 0, pos = 0;
    // find position of whitespaces
    while ((pos = groCommand.find(" ", pos)) != std::string::npos)
    { // create substrings for each inputted word
        std::string substring(groCommand.substr(prev_pos, pos - prev_pos));
        // store substrings in vector
        commandParts.push_back(substring);
        // assign current position + 1 to previous position
        prev_pos = ++pos;
    }
    // store the last word as substring in the vector
    commandParts.push_back(groCommand.substr(prev_pos, pos - prev_pos));
    // verification input length
    if ((int)commandParts.size() != 4)
    {
        std::cout << "Invalid command format" << std::endl;
        return;
    }

    if (commandParts[0].compare("growth") != 0)
    {
        std::cout << "Invalid command format" << std::endl;
        return;
    }

    // Verification of second part of command
    bool isProductDefined = false;
    std::vector<std::string> products;
    // assign products using getKnownProducts() rom MerkelMain
    products = orderBook.getKnownProducts();

    for (int i = 0; i < (int)products.size(); i++)
    { // verification of the user input, compared with the list of products
        if (commandParts[1].compare(products[i]) == 0)
        { // if product found, stop the loop
            isProductDefined = true;
            break;
        }
    }
    // if product not found, return the message
    if (!isProductDefined)
    {
        std::cout << "Invalid command format" << std::endl;
        return;
    }
    // assign user input to OrderBookEntry type
    OrderBookType type = OrderBookEntry::stringToOrderBookType(commandParts[2]);

    // Verification of third part of the command
    if (type == OrderBookType::unknown)
    { // if type is unknown, return error
        std::cout << "Invalid command format" << std::endl;
        return;
    }

    // Verification of fourth part of the command
    int steps = stoi(commandParts[3]);

    if (steps <= 0)
    {
        std::cout << "Invalid number of time steps entered. The number of steps must be at least 1" << std::endl;
        return;
    }

    // Get selected entries
    std::vector<OrderBookEntry> entries;
    std::vector<std::string> alltimestamps;
    double currentAvg = 0;
    double previousAvg = 0;
    // get all available timestamps from getAllTimestamps() in orderBook and store it in vector
    alltimestamps = orderBook.getAllTimestamps();
    // check if there's enough available data backward for inputted steps
    if (steps > nTime)
    {
        std::cout << "Invalid number of time steps entered. Maximum available data is " << nTime << " steps." << std::endl;
    }
    else
    {
        // Get selected entries using getOrders() from MerkelMain for current time
        entries = orderBook.getOrders(type, commandParts[1], alltimestamps[nTime]);
        // assign current time average, passing entries in the getAveragePrice() from MerkelMain
        currentAvg = OrderBook::getAveragePrice(entries);
        // Get selected entries using getOrders() from MerkelMain going backwards from the step
        entries = orderBook.getOrders(type, commandParts[1], alltimestamps[nTime - steps]);
        // assign previous time average, passing entries in the getAveragePrice() from MerkelMain
        previousAvg = OrderBook::getAveragePrice(entries);

        std::cout << "The growth for " << commandParts[1] << " " << commandParts[2] << " from " << alltimestamps[nTime - steps] << " to "
                  // calculate the growth percentage between 2 timestamps
                  << alltimestamps[nTime] << " is " << ((currentAvg - previousAvg) / previousAvg) * 100 << "%" << std::endl;
    }
}

void Advisorbot::processHelpUserOption()
{
    std::string helpUserOption;
    // back to MerkelMain menu with exit user input
    while (helpUserOption.compare("exit") != 0)
    {

        // C1 list all available commands
        std::cout << "The available commands are: " << std::endl;
        std::cout << "help <cmd> " << std::endl;
        std::cout << "prod " << std::endl;
        std::cout << "min " << std::endl;
        std::cout << "max " << std::endl;
        std::cout << "avg " << std::endl;
        std::cout << "predict " << std::endl;
        std::cout << "time " << std::endl;
        std::cout << "step " << std::endl;
        std::cout << "growth " << std::endl;
        std::cout << "exit " << std::endl;

        // std::string helpUserOption;
        helpUserOption = getHelpUserOption();

        // C2 output help for the specified command
        if (helpUserOption == "help <cmd>")
        {
            std::cout << "Help command syntax: help prod, help min, help max, help avg, " << std::endl;
            std::cout << "help predict, help time, help step, help growth, help exit" << std::endl;

            std::string helpCommand;
            std::getline(std::cin, helpCommand);

            processHelpCommand(helpCommand);
        }

        // C3 list available products for the current time
        if (helpUserOption == "prod")
        {
            // assign products using getKnownProducts() rom MerkelMain
            std::vector<std::string> products;
            products = orderBook.getKnownProducts();

            for (int i = 0; i < (int)products.size(); i++)
            { // print list of products
                std::cout << products[i] << std::endl;
            }
        }

        // C4 min product bid/ask
        if (helpUserOption == "min")
        {
            // find minimum bid or ask for product in current time step
            std::cout << "Min command syntax: min <product> <type>" << std::endl;
            std::cout << "For example: min ETH/BTC ask" << std::endl;

            std::string minCommand;
            std::getline(std::cin, minCommand);

            processHelpMinCommand(minCommand);
        }

        // C5 find maximum bid or ask for product in current time step
        if (helpUserOption == "max")
        {

            std::cout << "Max command syntax: max <product> <type>" << std::endl;
            std::cout << "For example: max ETH/BTC ask" << std::endl;

            std::string maxCommand;
            std::getline(std::cin, maxCommand);

            processHelpMaxCommand(maxCommand);
        }

        // C6 compute average ask or bid for the sent product over the sent number of time steps.
        if (helpUserOption == "avg")
        {

            std::cout << "Average command syntax: avg <product> <type> <number>" << std::endl;
            std::cout << "For example: avg ETH/BTC ask 10" << std::endl;

            std::string avgCommand;
            std::getline(std::cin, avgCommand);

            processHelpAverageCommand(avgCommand);
        }

        // C7 predict max or min ask or bid for the sent product for the next time step
        if (helpUserOption == "predict")
        {
            std::cout << "Average command syntax: predict <max/min> <product> <type>" << std::endl;
            std::cout << "For example: predict max ETH/BTC ask" << std::endl;

            std::string preCommand;
            std::getline(std::cin, preCommand);

            processHelpPredCommand(preCommand);
        }

        // C8 state current time in dataset, i.e. which timeframe are we looking at
        if (helpUserOption == "time")
        {

            std::cout << "The current time is " << currentTime << "." << std::endl;
        }

        // C9 move to next time step
        if (helpUserOption == "step")
        {

            std::string nextTime;
            nextTime = orderBook.getNextTime(currentTime);
            currentTime = nextTime;
            nTime++;
            // merkelMain.gotoNextTimeframe();

            std::cout << "The current time is " << currentTime << "." << std::endl;
        }

        // C10 calculates the growth of a currency
        if (helpUserOption == "growth")
        {
            std::cout << "Growth command syntax: growth <product> <type> <number>" << std::endl;
            std::cout << "For example: growth ETH/BTC ask 10" << std::endl;

            std::string groCommand;
            std::getline(std::cin, groCommand);

            processHelpGrowthCommand(groCommand);
        }
    }
}
