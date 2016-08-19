#pragma once

#include "Object.h"

#include <QtGui/QVector3D>

class Point : public Object
{
    Q_OBJECT

public:
    Q_PROPERTY(QVector3D position READ position WRITE setPosition NOTIFY positionChanged)

    void        setPosition(const QVector3D& position) {mPosition = position;}
    QVector3D   position() const {return mPosition;}

signals:
    void    positionChanged();

private:
    QVector3D   mPosition;
};
