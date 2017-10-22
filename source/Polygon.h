#pragma once

#include "Object.h"

#include <QtGui/QVector3D>
#include <QVector3D>
#include <QVector>

namespace geo
{
    class Polygon : public Object
    {
        Q_OBJECT

    public:
        QVector<QVector3D>& polygon() {return mPosition;}

    private:
        QVector<QVector3D>  mPosition;
    };
}
