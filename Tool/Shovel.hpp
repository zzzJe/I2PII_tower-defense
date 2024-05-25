#ifndef SHOVELTOOL_HPP
#define SHOVELTOOL_HPP

#include "Tool.hpp"

class ShovelTool: public Tool {
public:
    static const int Price;
    ShovelTool(float x, float y);
};
#endif
