#pragma once
        
#define AXIS_X_TAG "x"
#define AXIS_Y_TAG "y"
#define AXIS_Z_TAG "z" 

#define TRANSFORMS_TAG "transforms"
#define TRANSFORM_SEQUENCE_ID_TAG "seq_id"
#define TRANSFORM_TYPE_TAG "type"
    #define TRANSFORM_AXIS_TAG "axis"
    #define ROTATION_ANGLE_TAG "angle"   

#define CAMERA_TAG "camera"
    #define CAM_WIDTH_TAG "width"
    #define CAM_HEIGHT_TAG "height"
    #define CAM_FOCAL_LEN_TAG "focal_len_px"

#define RENDERER_TAG "renderer"
    #define RENDERER_PREVIEW_WIDTH_TAG "preview_width"
    #define RENDERER_PREVIEW_HEIGHT_TAG "preview_height"
    #define RENDERER_VERTEX_SHADER_FILENAME_TAG "vs_filename"
    #define RENDERER_FRAGMENT_SHADER_FILENAME_TAG "fs_filename"
    #define RENDERER_PREVIEW_VERTEX_SHADER_FILENAME_TAG "preview_vs_filename"
    #define RENDERER_PREVIEW_FRAGMENT_SHADER_FILENAME_TAG "preview_fs_filename"
        
#define TEXTURES_TAG "textures"
    #define TEXTURE_ID_TAG "id"
    #define TEXTURE_TYPE_TAG "type"
    #define TEXTURE_IMAGE_FILENAME_TAG "image_filename"
    #define TEXTURE_CHARUCO_TAG "charuco"
        #define CHARUCO_DICT_TAG "aruco_dict"
        #define CHARUCO_ROWS_TAG "rows"
        #define CHARUCO_COLS_TAG "cols"
        #define CHESS_SQUARE_SIZE_TAG "chess_sz"
        #define ARUCO_SIZE_TAG "aruco_sz"
        #define CHARUCO_PIXEL_WIDTH_TAG "px_width"
        #define CHARUCO_PIXEL_HEIGHT_TAG "px_height"    
        
#define TARGETS_TAG "targets"
    #define TARGET_ID_TAG "id"
    #define TARGET_TEXTURE_ID_TAG "tex_id"
    #define TARGET_VERTEX_DATA_TAG "vertex_data"
        #define VERTEX_POS_TAG "pos"
        #define VERTEX_TEXTURE_COORDINATE_TAG "tex_coord"
    #define TRIANGLE_INDICES_TAG "tri_indices"
        #define TRIANGLE_FIRST_INDEX_TAG "A"
        #define TRIANGLE_SECOND_INDEX_TAG "B"
        #define TRIANGLE_THIRD_INDEX_TAG "C"
        
#define CAPTURES_TAG "captures"
    #define CAPTURE_TARGET_ID_TAG "target_id"
