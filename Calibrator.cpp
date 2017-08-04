#include "Calibrator.hpp"
#include "CalData.hpp"

#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

namespace epilog
{

Calibrator::Calibrator(void) : m_flags(0)
{
}

void Calibrator::setCalFlags(int flags)
{
    m_flags = flags;
}

void Calibrator::addCalFlags(int flags)
{
    m_flags = m_flags | flags;
}

bool Calibrator::performCal(void)
{
    CalData* data = CalData::getInstance();

    if (!data->m_calImagesReady)
    {
        cout << "Calibration images not ready." << endl;
        return false;
    }

    if (data->m_calImages.empty()) 
    {
        cout << "No calibration images." << endl;
        return false;
    }

    // Rebuild the ChArUco board for use in detection
    Ptr<aruco::Dictionary> dict = aruco::getPredefinedDictionary(data->m_markerDict);
    Ptr<aruco::CharucoBoard> charucoboard = aruco::CharucoBoard::create(
                                                            data->m_chessCols, 
                                                            data->m_chessRows, 
                                                            data->m_chessSqSz, 
                                                            data->m_arucoSz, 
                                                            dict);
    Ptr<aruco::Board> board = charucoboard.staticCast<aruco::Board>();

    // Extract corners and ids from images
    Ptr<aruco::DetectorParameters> params = aruco::DetectorParameters::create();
    vector<vector<vector<Point2f> > > allCorners;
    vector<vector<int> > allIds;
    for (auto img : data->m_calImages) 
    {
        vector<int > ids;
        vector<vector<Point2f> > corners, rejected;

        // detect markers
        aruco::detectMarkers(img, dict, corners, ids, params, rejected);

        // refind strategy to detect more markers
        // aruco::refineDetectedMarkers(img, board, corners, ids, rejected);

        // interpolate charuco corners
        Mat currentCharucoCorners, currentCharucoIds;
        if(ids.size() > 0)
            aruco::interpolateCornersCharuco(corners, 
                                                ids, 
                                                img, 
                                                charucoboard, 
                                                currentCharucoCorners,
                                                currentCharucoIds);
                                                
        // Store results
        allCorners.push_back(corners);
        allIds.push_back(ids);
    }

    // Prep data for calibration
    vector<vector<Point2f> > allCornersConcatenated;
    vector<int> allIdsConcatenated;
    vector<int> markerCounterPerFrame;
    markerCounterPerFrame.reserve(allCorners.size());
    for(size_t frameInd = 0; 
        frameInd < allCorners.size(); 
        frameInd++) 
    {
        markerCounterPerFrame.push_back((int)allCorners[frameInd].size());
        for(size_t cornerInd = 0; 
            cornerInd < allCorners[frameInd].size(); 
            cornerInd++) 
        {
            allCornersConcatenated.push_back(allCorners[frameInd][cornerInd]);
            allIdsConcatenated.push_back(allIds[frameInd][cornerInd]);
        }
    }

    // Calibration variables
    Size imgSize = data->m_calImages[0].size();
    Mat cameraMatrix, distCoeffs;
    vector<Mat> rvecs, tvecs;
    
    // calibrate camera using aruco markers
    double arucoRepErr;
    arucoRepErr = aruco::calibrateCameraAruco(allCornersConcatenated, 
                                                allIdsConcatenated,
                                                markerCounterPerFrame, 
                                                board, 
                                                imgSize, 
                                                cameraMatrix,
                                                distCoeffs, 
                                                noArray(), 
                                                noArray(), 
                                                m_flags);
                                                
    // Prep for charcuo calibration
    size_t nFrames = allCorners.size();
    vector<Mat> allCharucoCorners;
    vector<Mat> allCharucoIds;
    allCharucoCorners.reserve(nFrames);
    allCharucoIds.reserve(nFrames);
    for(size_t i = 0; i < nFrames; i++) 
    {
        // interpolate using camera parameters
        Mat currentCharucoCorners, currentCharucoIds;
        aruco::interpolateCornersCharuco(allCorners[i], 
                                            allIds[i], 
                                            data->m_calImages[i], 
                                            charucoboard,
                                            currentCharucoCorners, 
                                            currentCharucoIds, 
                                            cameraMatrix,
                                            distCoeffs);

        allCharucoCorners.push_back(currentCharucoCorners);
        allCharucoIds.push_back(currentCharucoIds);
    }

    if(allCharucoCorners.size() < 4) 
    {
        cerr << "Not enough corners for calibration" << endl;
        return false;
    }
    
    // charuco calibration           
    double repError = cv::aruco::calibrateCameraCharuco(allCharucoCorners, 
                                                        allCharucoIds, 
                                                        charucoboard, 
                                                        imgSize, 
                                                        cameraMatrix, 
                                                        distCoeffs, 
                                                        rvecs, 
                                                        tvecs, 
                                                        m_flags);
                                                        
    // save result to calibration data
    cameraMatrix.copyTo(data->m_calCamMatrix);
    distCoeffs.copyTo(data->m_calDistCoeffs);
    data->m_calRMats.clear();
    data->m_calRMats.resize(rvecs.size());
    data->m_calTMats.clear();
    data->m_calTMats.resize(tvecs.size());
    for (size_t frameInd = 0; frameInd < rvecs.size(); ++frameInd)
    {
        rvecs[frameInd].copyTo(data->m_calRMats[frameInd]);
        tvecs[frameInd].copyTo(data->m_calTMats[frameInd]);
    }
    data->m_calFlags = m_flags;
    data->m_calRepError = repError;

    return true;
}

} /// namespace epilog
