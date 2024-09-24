#include "application.h"

#include <memory>

// main.cpp
int main()
{
    auto app = std::make_unique<Application>();
    app->run();
    return 0;
}
