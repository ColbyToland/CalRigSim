// Code borrowed heavily from the OpenCV samples

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/aruco/charuco.hpp>

#include <cctype>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "CalData.hpp"
#include "Calibrator.hpp"
#include "CalTarget.hpp"
#include "CalTex.hpp"
#include "Renderman.hpp"

using namespace cv;
using namespace std;
using namespace epilog;

// Work around for static rendering function
//  Admittedly a little ugly but simple and effective.
Renderman * p_renderer = 0;
void renderScene()
{
    p_renderer->renderScene();
}

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

    // Create target
    CalTarget charucoTarget(charucoTex);

    // Create renderer
    Renderman renderer(&charucoTarget);
    renderer.init(argc, argv);
    p_renderer = &renderer;

	// Register render callback
	glutDisplayFunc(renderScene);

	// Enter GLUT event processing cycle
	glutMainLoop();

    return 0;
}
