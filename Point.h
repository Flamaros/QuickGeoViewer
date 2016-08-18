#pragma once

#include "Object.h"

#include <QtGui/QVector3D>

class Point : public Object
{
public:
    QVector3D   position;
};
