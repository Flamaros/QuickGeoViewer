#pragma once

#include <QtGlobal>

#include <Qt3DRender/QCamera>

class Camera : public Qt3DRender::QCamera
{
public:
    Camera(QNode* parent = nullptr);
};
