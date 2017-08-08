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

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"

#include "config/ConfigParser.hpp"
#include "Calibrator.hpp"
#include "CalTarget.hpp"
#include "CalTex.hpp"
#include "Renderman.hpp"

#include "Debugger.hpp"

using namespace cv;
using namespace std;
using namespace epilog;

int main( int argc, char** argv )
{    
    // Command line argument parsing
    CommandLineParser parser(argc, argv,
        "{help h usage ?|               | print this message        }"        
        "{@config       |               | config file name          }"
        );
        
    if (parser.has("help") || !parser.has("@config"))
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
    
    string configFilename = parser.get<string>("@config");

    // Define some configuration data
    //  In the future populate these values from a config file.
    DEBUG_OUT("Read yml configuration");
    CalData* data = CalData::getInstance();
    bool success = ConfigParser::readFile(configFilename);
    if (!success) return -1;
    
    DEBUG_OUT(std::string(*data));

    // Create renderer
    DEBUG_OUT("Create renderer");
    Renderman renderer;
    DEBUG_OUT("Initialize renderer");
    if (!renderer.init()) 
    {
        std::cout << "RENDERER INIT FAIL" << std::endl;
        return -1;
    }
    DEBUG_OUT("Begin render loop");
    renderer.oldMainLoop();
    //renderer.mainLoop();

    // Calibrate
    DEBUG_OUT("Calibrate");
    if (data->m_calImagesReady)
    {
        Calibrator charucoCal;
        charucoCal.setCalFlags(CV_CALIB_FIX_ASPECT_RATIO);
        charucoCal.performCal();
    
        cout << "Reprojection Error: " << data->m_calRepError << endl;
        cout << "Camera Matrix: \n" << data->m_calCamMatrix << endl;
        cout << "Distortion Model: \n" << data->m_calDistCoeffs << endl;
    }
    else
    {
        cout << "Could not calibrate." << endl;
    }

    return 0;
}
