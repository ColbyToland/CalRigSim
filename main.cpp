// Code borrowed heavily from the OpenCV samples

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/aruco/charuco.hpp>

#include <cctype>
#include <iostream>
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

    // Define some configuration data
    //  In the future populate these values from a config file.
    CalData* data = CalData::getInstance();
    data->m_markerDict = aruco::DICT_4X4_250;
    data->m_chessRows = 20;
    data->m_chessCols = 10;
    data->m_pxWidthTarget = 1000;
    data->m_pxHeightTarget = 2000;
    data->m_vertexShaderSourceFile = "perspective.vs";
    data->m_fragmentShaderSourceFile = "basic.fs";
    data->m_previewVSSourceFile = "preview.vs";
    data->m_previewFSSourceFile = "preview.fs";
    data->readShaders();
    data->m_camModel.width = 2048; // 3 MP = 2048x1536?
    data->m_camModel.height = 1536;
    data->m_previewWidth = 1024;
    data->m_previewHeight = 768;

    // Create texture
    CalTex charucoTex;
    charucoTex.genChArUco();

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

    // For now, break out of the mainLoop when enough images exist then enter
        // the calibration class.
    if (data->m_calImagesReady)
    {
        Calibrator charucoCal;
        charucoCal.setCalFlags(0);
        charucoCal.performCal();
    }
    
    cout << "Reprojection Error: " << data->m_calRepError << endl;

    return 0;
}
