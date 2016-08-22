#pragma once

#include <QString>
#include <QQuickItem>

class QQmlContext;

class Object;
class Point;

namespace Qt3DCore
{
	class QEntity;
    class QAspectEngine;
    class QNode;
}

namespace Qt3DExtras
{
    class QOrbitCameraController;
}

class Camera;

static const QString	rootEntityProperty = "scene3DRootEntity";
static const QString	axesProperty = "axes";
static const QString	elementsProperty = "elements";

class Scene : public QQuickItem
{
public:
    static Scene*   singleton();

    void	initialize();
    void    instantiate(const QList<QObject*>& objects);
    void    clear();

    void    setObjectColor(const QColor& color, Object& object);

    Camera*             cameraEntity() const {return mCameraEntity;}
    Qt3DCore::QEntity*	rootEntity() const {return mRootEntity;}
    Qt3DCore::QEntity*	elementsEntity() const {return mElementsEntity;}

public:
    static QQmlContext* qmlContext;

protected:
    Scene();

    void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry) override;

private:
    void    generateAxis();
    void    updateQmlProperties();
    void    instantiate(Point& point);
    void    releaseModel(Qt3DCore::QNode& model);

    static Scene*                       mInstance;

    Camera*                             mCameraEntity;
    Qt3DExtras::QOrbitCameraController* mCameraControler;
    Qt3DCore::QEntity*                  mRootEntity;
    Qt3DCore::QEntity*                  mAxesEntity;
    Qt3DCore::QEntity*                  mElementsEntity;
    Qt3DCore::QAspectEngine*            mAspectEngine;
};
