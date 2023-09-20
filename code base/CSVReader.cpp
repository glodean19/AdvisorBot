#include "CSVReader.h"
#include <iostream>
#include <fstream>

CSVReader::CSVReader()
{
}

std::vector<OrderBookEntry> CSVReader::readCSV(std::string csvFilename)
{
    std::vector<OrderBookEntry> entries;
    // the file we are processing
    std::ifstream csvFile{csvFilename};
    std::string line;
    if (csvFile.is_open())
    {
        // reads a line from the csv file
        while (std::getline(csvFile, line))
        {
            try
            { // assign the object to the function passing the tokenise method
                OrderBookEntry obe = stringsToOBE(tokenise(line, ','));
                entries.push_back(obe);
            }
            catch (const std::exception &e)
            {
                std::cout << "CSVReader::readCSV bad data" << std::endl;
            }
        } // end of while
    }

    std::cout << "CSVReader::readCSV read " << entries.size() << " entries" << std::endl;

    return entries;
}
// tokenise function with file and separator as parameters
std::vector<std::string> CSVReader::tokenise(std::string csvLine, char separator)
{
    // stores the tokens
    std::vector<std::string> tokens;
    // used to delineate the position of the tokens
    signed int start, end;
    std::string token;
    // Searches the string for the first character that does not match with the separator
    start = csvLine.find_first_not_of(separator, 0);
    do
    {
        // next separator after start
        end = csvLine.find_first_of(separator, start);
        // if nothing more to find
        if (start == csvLine.length() || start == end)
            break;
        // if found the separator
        if (end >= 0)
            token = csvLine.substr(start, end - start);
        // end if it is invalid
        else
            token = csvLine.substr(start, csvLine.length() - start);
        // add token to the end of the tokens vector
        tokens.push_back(token);
        // move past this token
        start = end + 1;
    }
    // continue loop condition
    while (end > 0);

    return tokens;
}

OrderBookEntry CSVReader::stringsToOBE(std::vector<std::string> tokens)
{
    double price, amount;
    // we have 5 tokens
    if (tokens.size() != 5)
    { // bad
        std::cout << "Bad line " << std::endl;
        throw std::exception{};
    }

    try
    { // set price and amount to token 3 and 4
        price = std::stod(tokens[3]);
        amount = std::stod(tokens[4]);
    }
    catch (const std::exception &e)
    {
        std::cout << "CSVReader::stringsToOBE Bad float! " << tokens[3] << std::endl;
        std::cout << "CSVReader::stringsToOBE Bad float! " << tokens[4] << std::endl;
        throw;
    }

    OrderBookEntry obe{price,
                       amount,
                       tokens[0],
                       tokens[1],
                       OrderBookEntry::stringToOrderBookType(tokens[2])};

    return obe;
}
// pass the tokens to stringsToOBE
OrderBookEntry CSVReader::stringsToOBE(std::string priceString,
                                       std::string amountString,
                                       std::string timestamp,
                                       std::string product,
                                       OrderBookType orderType)
{
    double price, amount;
    try
    {
        // it converts strings to double
        price = std::stod(priceString);
        amount = std::stod(amountString);
    }
    catch (const std::exception &e)
    {
        std::cout << "CSVReader::stringsToOBE Bad float! " << priceString << std::endl;
        std::cout << "CSVReader::stringsToOBE Bad float! " << amountString << std::endl;
        throw;
    }
    OrderBookEntry obe{price,
                       amount,
                       timestamp,
                       product,
                       orderType};

    return obe;
}
