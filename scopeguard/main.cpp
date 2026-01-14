

#include "scope_guard.hpp"
#include <exception>
#include <iostream>
#include <stdexcept>


void rOp(bool fail)
{
    std::cout<<"Starting operation..\n";

    auto gaurd = make_scope_guard([&]()
            {
            std::cout<<"Cleaning resources (scope_guard triggered)\n";
            });

    if(fail)
    {
        std::cout<<"operation failed.. ! throwing exp\n";
        throw std::runtime_error("Something went wrong");
    }
    std::cout<<"operation success:\n";

    gaurd.dismiss();

    std::cout<<"scope_guard dismissed, no cleanup";
    
}


int main()
{
    try {
        std::cout<<"case 1: success\n";
        rOp(false);

        std::cout<<"case 2: failure\n";
        rOp(true);
    }

    catch(const std::exception& e)
    {
        std::cout<<"error:"<<e.what()<<"\n";
    }

    std::cout<<"end of program";
}
