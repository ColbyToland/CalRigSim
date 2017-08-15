#include "config/ConfigParser.hpp"

using namespace std;
using namespace cv;

namespace epilog
{

bool ConfigParser::readFile(string filename)
{
    CalData* config = CalData::getInstance();
    bool success = true;

    FileStorage fs;
    fs.open(filename, FileStorage::READ);
    
    FileNode cameraNode = fs[CAMERA_TAG];
    readCamSettings(cameraNode);
    
    FileNode rendererNode = fs[RENDERER_TAG];
    readRendererSettings(rendererNode);

    FileNode textureNode = fs[TEXTURES_TAG];
    if (textureNode.type() != FileNode::SEQ)
    {
        cerr << TEXTURES_TAG << " is not a sequence!" << endl;
        return false;
    }
    for (auto node : textureNode)
    {
        readTextureSettings(node);
    }
    config->generateTextures();
    
    FileNode targetNode = fs[TARGETS_TAG];
    if (targetNode.type() != FileNode::SEQ)
    {
        cerr << TARGETS_TAG << " is not a sequence!" << endl;
        return false;
    }
    for (auto node : targetNode)
    {
        success = readTargetSettings(node);
        if (!success) return false;
    }
    config->generateTargets();
    
    FileNode captureNode = fs[CAPTURES_TAG];
    if (captureNode.type() != FileNode::SEQ)
    {
        cerr << CAPTURES_TAG << " is not a sequence!" << endl;
        return false;
    }
    for (auto node : captureNode)
    {
        readCaptureSettings(node);
    }
    
    return true;
}

void ConfigParser::readCamSettings(FileNode& cameraNode)
{
    CalData* config = CalData::getInstance();
    config->m_camModel.m_width = (int)cameraNode[CAM_WIDTH_TAG];
    config->m_camModel.m_height = (int)cameraNode[CAM_HEIGHT_TAG];
    config->m_camModel.m_focal_len = (float)cameraNode[CAM_FOCAL_LEN_TAG];
    
    // Read transforms
    FileNode tNode = cameraNode[TRANSFORMS_TAG];
    for (auto node : tNode)
    {
        int seq_id = node[TRANSFORM_SEQUENCE_ID_TAG];
        GeomTransform transform;
        transform = readTransforms(node);
        config->m_camModel.m_transforms[seq_id] = transform;
    }
}

void ConfigParser::readRendererSettings(FileNode& rendererNode)
{
    CalData* config = CalData::getInstance();
    config->m_previewWidth = (int)rendererNode[RENDERER_PREVIEW_WIDTH_TAG];
    config->m_previewHeight = (int)rendererNode[RENDERER_PREVIEW_HEIGHT_TAG];
    config->m_vertexShaderSourceFile = 
            (string)rendererNode[RENDERER_VERTEX_SHADER_FILENAME_TAG];
    config->m_fragmentShaderSourceFile = 
            (string)rendererNode[RENDERER_FRAGMENT_SHADER_FILENAME_TAG];
    config->m_previewVSSourceFile = 
            (string)rendererNode[RENDERER_PREVIEW_VERTEX_SHADER_FILENAME_TAG];
    config->m_previewFSSourceFile = 
            (string)rendererNode[RENDERER_PREVIEW_FRAGMENT_SHADER_FILENAME_TAG];
    config->readShaders();
}

void ConfigParser::readTextureSettings(FileNode& textureNode)
{
    CalData* config = CalData::getInstance();
    int texID = textureNode[TEXTURE_ID_TAG];
    TextureConfigData texture;
    texture.m_type = 
        TextureConfigData::mapStringToTextureType(textureNode[TEXTURE_TYPE_TAG]);
    FileNode charucoNode;
    switch (texture.m_type)
    {
        case TextureType::IMAGE_TEXTURE:
            texture.m_imgFilename = (string)textureNode[TEXTURE_IMAGE_FILENAME_TAG];
            break;
    
        case TextureType::CHARUCO_TEXTURE:
            charucoNode = textureNode[TEXTURE_CHARUCO_TAG];
            texture.m_markerDict = 
                TextureConfigData::mapStringToArUcoDict(charucoNode[CHARUCO_DICT_TAG]);
            texture.m_chessRows = charucoNode[CHARUCO_ROWS_TAG];
            texture.m_chessCols = charucoNode[CHARUCO_COLS_TAG];
            texture.m_chessSqSz = charucoNode[CHESS_SQUARE_SIZE_TAG]; 
            texture.m_arucoSz = charucoNode[ARUCO_SIZE_TAG]; 
            texture.m_pxWidthTarget = charucoNode[CHARUCO_PIXEL_WIDTH_TAG];
            texture.m_pxHeightTarget = charucoNode[CHARUCO_PIXEL_HEIGHT_TAG];
            break;
            
        case TextureType::INVALID:
            cerr << "Invalid texture: " << texID << endl;
            break;
            
        default:
            break;
    }
    
    config->m_textureSettings[texID] = texture;
}

bool ConfigParser::readTargetSettings(FileNode& targetNode)
{
    CalData* config = CalData::getInstance();
    bool success = true;
    int targetID = targetNode[TARGET_ID_TAG];
    TargetConfigData target;
    target.m_texID = targetNode[TARGET_TEXTURE_ID_TAG];
    
    FileNode vertexNode = targetNode[TARGET_VERTEX_DATA_TAG];
    if (vertexNode.type() != FileNode::SEQ)
    {
        cerr << "Vertex data is not a sequence in target " << targetID << "!" << endl;
        return false;
    }
    vector<float> vertexArray;
    int vertexCount = 0;
    for (auto node : vertexNode)
    {
        ++vertexCount;
        FileNode posNode = node[VERTEX_POS_TAG];
        vertexArray.push_back(posNode[AXIS_X_TAG]);
        vertexArray.push_back(posNode[AXIS_Y_TAG]);
        vertexArray.push_back(posNode[AXIS_Z_TAG]);
        FileNode texCoordNode = node[VERTEX_TEXTURE_COORDINATE_TAG];
        vertexArray.push_back(texCoordNode[AXIS_X_TAG]);
        vertexArray.push_back(texCoordNode[AXIS_Y_TAG]);
    }
    target.m_vertexData.reset(new float[vertexArray.size()], std::default_delete<float[]>());
    for (size_t ind = 0; ind < vertexArray.size(); ++ind)
    {
        target.m_vertexData.get()[ind] = vertexArray[ind];
    }
    target.m_vertexCount = vertexCount;
    
    FileNode indexNode = targetNode[TRIANGLE_INDICES_TAG];
    if (indexNode.type() != FileNode::SEQ)
    {
        cerr << "Index data is not a sequence in target " << targetID << "!" << endl;
        return false;
    }
    vector<unsigned int> indexArray;
    int triCount = 0;
    for (auto triNode : indexNode)
    {
        ++triCount;
        int triInd = triNode[TRIANGLE_FIRST_INDEX_TAG]; 
        indexArray.push_back(triInd);
        triInd = triNode[TRIANGLE_SECOND_INDEX_TAG];
        indexArray.push_back(triInd);
        triInd = triNode[TRIANGLE_THIRD_INDEX_TAG];
        indexArray.push_back(triInd);
    }
    target.m_indices.reset(new unsigned int[indexArray.size()], 
                                std::default_delete<unsigned int[]>());
    for (size_t ind = 0; ind < indexArray.size(); ++ind)
    {
        target.m_indices.get()[ind] = indexArray[ind];
    }
    target.m_triCount = triCount;
    
    // Read transforms
    FileNode tNode = targetNode[TRANSFORMS_TAG];
    for (auto node : tNode)
    {
        int seq_id = node[TRANSFORM_SEQUENCE_ID_TAG];
        GeomTransform transform;
        transform = readTransforms(node);
        target.m_transforms[seq_id] = transform;
    }
    
    config->m_targetSettings.insert(pair<int, TargetConfigData>(targetID, target));
    
    return true;
}

void ConfigParser::readCaptureSettings(FileNode& captureNode)
{
    CalData* config = CalData::getInstance();
    CapturePositionConfig capture;
    
    capture.m_targetID = captureNode[CAPTURE_TARGET_ID_TAG];
    
    // Read transforms
    bool animation = false;
    int animation_id = -1;
    FileNode tNode = captureNode[TRANSFORMS_TAG];
    for (auto node : tNode)
    {
        int seq_id = node[TRANSFORM_SEQUENCE_ID_TAG];
        GeomTransform transform;
        transform = readTransforms(node);
        capture.m_transforms[seq_id] = transform;
        if (transform.m_type == GeomTransformType::ROTATION_CYCLE)
        {
            animation = true;
            animation_id = seq_id;
        }
    }
    
    if (animation)
    {
        float delta_angle = capture.m_transforms[animation_id].m_angle;
        float angle = 0.0f;
        do
        {
            capture.m_transforms[animation_id].m_type = GeomTransformType::ROTATION;
            capture.m_transforms[animation_id].m_angle = angle;
            
            config->m_captures.push_back(capture);
            
            angle = angle + delta_angle;
        } 
        while (abs(angle) < 360.0f);
    }
    else
    {
        config->m_captures.push_back(capture);
    }
}

GeomTransform ConfigParser::readTransforms(FileNode& node)
{
    // Read transforms
    GeomTransform transform;
    transform.m_type = 
        GeomTransform::mapStringToTransformType(node[TRANSFORM_TYPE_TAG]);
    FileNode axisNode = node[TRANSFORM_AXIS_TAG];
    transform.m_axis[0] = axisNode[AXIS_X_TAG];
    transform.m_axis[1] = axisNode[AXIS_Y_TAG];
    transform.m_axis[2] = axisNode[AXIS_Z_TAG];
    transform.m_angle = node[ROTATION_ANGLE_TAG];
    return transform;
}

} /// namespace epilog


