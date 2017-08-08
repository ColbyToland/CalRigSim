#pragma once

#include <opencv2/aruco/charuco.hpp>

namespace epilog
{

class Calibrator
{
public:
    Calibrator(void);

    // Replace current flags with a new flags value
    void setCalFlags(int flags);

    // OR in new flags
    void addCalFlags(int flags);

    bool performCal(void);

private:
    int m_flags;

};

} /// namespace epilog
