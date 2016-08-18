#pragma once

#include <QString>

class Object
{
public:
    virtual ~Object() {}

    QString name;
    QString label;
};
