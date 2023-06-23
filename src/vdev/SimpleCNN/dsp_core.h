#pragma once
#include <fstream>
#include <string>

#include "memhub.h"
#include "tools.h"

class DSPCore
{
public:
    int op_init();
    int op_input();
	int op_output();
    int op_calculate(std::string fromMemName, std::string toMemName, Range inRange, Range outRange);

    MemHub memhub;
};