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
        emit colorChanged();
    }

    void Object::setVisible(bool flag)
    {
        Scene::singleton()->setObjectVisibility(flag, *this);
        emit visibleChanged();
    }

    bool Object::visible() const
    {
        return Scene::singleton()->isObjectVisible(*this);
    }
}
