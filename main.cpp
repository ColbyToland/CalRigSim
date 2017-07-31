// Code borrowed heavily from the OpenCV samples

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/aruco/charuco.hpp>

#include <cctype>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "glad.h"

#include "CalData.hpp"
#include "Calibrator.hpp"
#include "CalTarget.hpp"
#include "CalTex.hpp"
#include "Renderman.hpp"

using namespace cv;
using namespace std;
using namespace epilog;

int main( int argc, char** argv )
{    
    // Command line argument parsing
    CommandLineParser parser(argc, argv,
        "{help h usage ?|               | print this message        }"
        );
        
    if (parser.has("help"))
    {
        parser.printMessage();
        return 0;
    }
    
    if (!parser.check())
    {
        parser.printMessage();
        parser.printErrors();
        return -1;
    }

    // Create texture
    CalTex charucoTex;
    charucoTex.genChArUco(aruco::DICT_4X4_250, 16, 10);
    charucoTex.genTexture(600, 500);

    // Create target
    CalTarget charucoTarget(charucoTex);

    // Create renderer
    Renderman renderer(&charucoTarget);
    if (!renderer.init()) 
    {
        std::cout << "FAIL" << std::endl;
        char a = 0;
        cin >> a;
        return -1;
    }
    renderer.mainLoop();

    return 0;
}
