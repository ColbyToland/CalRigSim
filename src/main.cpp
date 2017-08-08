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
    bool success = ConfigParser::readFile("test.yml");
    if (!success) return -1;

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
        charucoCal.setCalFlags(CV_CALIB_FIX_ASPECT_RATIO);
        charucoCal.performCal();
    }
    
    cout << "Reprojection Error: " << data->m_calRepError << endl;
    cout << "Camera Matrix: \n" << data->m_calCamMatrix << endl;
    float halfw = data->m_camModel.m_width / 2.0f;
    float fov = 2.0f*glm::degrees(glm::atan((float)data->m_calCamMatrix.at<double>(0,0),halfw));
    printf("FOV: %f\n", fov);
    cout << "Distortion Model: \n" << data->m_calDistCoeffs << endl;

    return 0;
}
