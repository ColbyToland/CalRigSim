#pragma once

#ifdef __DEBUG_COUT__
    #include <iostream>
    #define DEBUG_OUT(_x)   std::cout << _x << std::endl;
#else
    #define DEBUG_OUT(_x)
#endif
