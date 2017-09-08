#include "Object.h"

#include "Scene.h"

namespace geo
{
    Object::Object()
        : mColor(QRgb(0xffffff))
        , mSceneData(nullptr)
    {
    }

    void    Object::setColor(const QColor& color)
    {
        mColor = color;

        Scene::singleton()->setObjectColor(color, *this);
    }
}
