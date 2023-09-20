#include "MerkelMain.h"

int main()
{
    // call to the constructor of MerkelMain, then the constructor of OrderBook
    MerkelMain app{};
    // calls initialisation
    app.init();
}
