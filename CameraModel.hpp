#pragma once

class CameraModel
{
public:
    CameraModel() : width(1024), height(768), fov(100.0)
        {}

    size_t width;
    size_t height;
    double fov;

private:
};
