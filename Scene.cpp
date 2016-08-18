#include "Scene.h"

#include "Camera.h"

#include <QtQml/QQmlContext>

#include <Qt3DRender/QCameraSelector>
#include <Qt3DRender/QRenderSettings>
#include <Qt3DRender/QTechniqueFilter>
#include <Qt3DRender/QViewport>
#include <Qt3DRender/QClearBuffers>
#include <Qt3DRender/QRenderPassFilter>
#include <Qt3DRender/QMesh>
#include <Qt3DRender/QCullFace>
#include <Qt3DRender/QRenderAspect>
#include <Qt3DRender/QRenderSurfaceSelector>

#include <Qt3DInput/QInputAspect>
#include <Qt3DInput/QInputSettings>

#include <Qt3DCore/QEntity>
#include <Qt3DCore/QAspectEngine>
#include <Qt3DCore/QTransform>

#include <Qt3DExtras/QCuboidMesh>
#include <Qt3DExtras/QCylinderMesh>
#include <Qt3DExtras/QConeMesh>
#include <Qt3DExtras/QSphereGeometry>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DExtras/QOrbitCameraController>

#include <QtGui/QSurface>

#include <QtCore/QtMath>

#include "Object.h"
#include "Point.h"

using namespace Qt3DCore;
using namespace Qt3DRender;
using namespace Qt3DInput;

Scene*          Scene::mInstance = nullptr;
QQmlContext*	Scene::qmlContext = nullptr;

Scene*  Scene::singleton()
{
    if (mInstance == nullptr)
        new Scene();    // The constructor assign mInstance because the Qml can invoke the constructor directly instead of using this singleton method
    return mInstance;
}

void    Scene::initialize()
{
    mRootEntity = new QEntity();
    mAxesEntity = new QEntity(mRootEntity);
    mElementsEntity = new QEntity(mRootEntity);

    mRootEntity->setObjectName(QStringLiteral("rootEntity"));

    // Camera
    mCameraEntity = new Camera(mRootEntity);
    mCameraEntity->setObjectName(QStringLiteral("cameraEntity"));

    QRenderSettings*        renderSettings = new QRenderSettings();
    QInputSettings*         inputSettings = new QInputSettings();
    QTechniqueFilter*       forwardRenderer = new QTechniqueFilter();
    QRenderSurfaceSelector* surfaceSelector = new QRenderSurfaceSelector(forwardRenderer);
    QViewport*			    viewport = new QViewport(surfaceSelector);
    QClearBuffers*		    clearBuffers = new QClearBuffers(viewport);
    QCameraSelector*	    cameraSelector = new QCameraSelector(clearBuffers);
    QCullFace*			    cullFace = new QCullFace(cameraSelector);
    QRenderPassFilter*	    renderPassFilter = new QRenderPassFilter(cullFace);

    // TechiqueFilter and renderPassFilter are not implement yet

    surfaceSelector->setSurface(nullptr);
    viewport->setNormalizedRect(QRectF(0.0f, 0.0f, 1.0f, 1.0f));
    clearBuffers->setClearColor(QColor(64, 128, 128));
    clearBuffers->setBuffers(QClearBuffers::ColorDepthBuffer);
    cameraSelector->setCamera(mCameraEntity);
    renderSettings->setActiveFrameGraph(forwardRenderer);
    cullFace->setEnabled(false);

    // Setting the FrameGraph
    mRootEntity->addComponent(renderSettings);
    mRootEntity->addComponent(inputSettings);

    mAspectEngine = new QAspectEngine;
    mAspectEngine->registerAspect(new QRenderAspect);
    mAspectEngine->registerAspect(new QInputAspect);
    mAspectEngine->setRootEntity(QEntityPtr(mRootEntity));

    // Setting the camera
    mCameraEntity->setProjectionType(QCameraLens::PerspectiveProjection);
    mCameraEntity->setNearPlane(0.2f);
    mCameraEntity->setFarPlane(100.0f);
    mCameraEntity->setFieldOfView(90.0f);
    mCameraEntity->setAspectRatio(width() / height());

    qreal   width1 = width();
    qreal   height1 = height();
    qreal   implicitWidth1 = implicitWidth();
    qreal   implicitHeight1 = implicitHeight();

    mCameraEntity->setViewCenter(QVector3D(0.0f, 0.0f, 0.f));
    mCameraEntity->setUpVector(QVector3D(0.0f, 1.0f, 0.0f));
    mCameraEntity->setPosition(QVector3D(0.0f, 0.0f, 20.f));

    mCameraControler = new Qt3DExtras::QOrbitCameraController(mRootEntity);
    mCameraControler->setZoomInLimit(10.0f);
    mCameraControler->setLinearSpeed(50.0f);
    mCameraControler->setLookSpeed(180.0f);
    mCameraControler->setCamera(mCameraEntity);

//    mCameraEntity->setLeft(-1.0f);
//    mCameraEntity->setRight(1.0f);
//    mCameraEntity->setTop(1.0f);
//    mCameraEntity->setBottom(-1.0f);

    generateAxis();

    updateQmlProperties();
}

void    Scene::instantiate(const QVector<Object*>& objects)
{
    for (int i = 0; i < objects.size(); i++)
    {
        Point*  point;

        point = dynamic_cast<Point*>(objects[i]);
        if (point != nullptr)
            instantiate(*point);
    }
}

void    Scene::clear()
{
    QNodeVector objectsNodes = mElementsEntity->childNodes();

    for (int i = 0; i < objectsNodes.size(); i++)
    {
        releaseModel(*objectsNodes[i]);
        objectsNodes[i]->setParent(static_cast<QNode*>(nullptr));
        delete objectsNodes[i];
    }

    Q_ASSERT(mElementsEntity->childNodes().size() == 0);
}

//==============================================================================

Scene::Scene()
    : mCameraEntity(nullptr)
    , mCameraControler(nullptr)
    , mRootEntity(nullptr)
    , mAxesEntity(nullptr)
    , mElementsEntity(nullptr)
    , mAspectEngine(nullptr)
{
    if (mInstance == nullptr)
        mInstance = this;
    updateQmlProperties();
}

void    Scene::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    QQuickItem::geometryChanged(newGeometry, oldGeometry);

    if (mCameraEntity)
        mCameraEntity->setAspectRatio(newGeometry.width() / newGeometry.height());
}

void    Scene::generateAxis()
{
    struct Axis
    {
        QVector3D   vector;
        QColor      color;
        QQuaternion quaternion;
    };

    static Axis axis[] = {
        {QVector3D(1.0f, 0.0f, 0.0f), QColor(QRgb(0xff0000)), QQuaternion::fromEulerAngles(0.0f, 0.0f, 90.0f)}, // Red      X
        {QVector3D(0.0f, 1.0f, 0.0f), QColor(QRgb(0x00ff00)), QQuaternion::fromEulerAngles(0.0f, 0.0f, 0.0f)},  // Green    Y
        {QVector3D(0.0f, 0.0f, 1.0f), QColor(QRgb(0x0000ff)), QQuaternion::fromEulerAngles(90.0f, 0.0f, 0.0f)}  // Blue     Z
    };

    for (int i = 0; i < 3; i++)
    {
        Qt3DExtras::QCylinderMesh*  mesh = new Qt3DExtras::QCylinderMesh();
        Qt3DCore::QTransform*       transform = new Qt3DCore::QTransform();
        Qt3DExtras::QPhongMaterial* material = new Qt3DExtras::QPhongMaterial();
        Qt3DCore::QEntity*          entity = new Qt3DCore::QEntity(mAxesEntity);

        mesh->setRadius(0.05f);
        mesh->setLength(100.0f);

        transform->setRotation(axis[i].quaternion);

        material->setDiffuse(axis[i].color);
        material->setAmbient(axis[i].color);

        entity->addComponent(mesh);
        entity->addComponent(material);
        entity->addComponent(transform);
    }
}

void    Scene::updateQmlProperties()
{
    qmlContext->setContextProperty(rootEntityProperty, mRootEntity);
    qmlContext->setContextProperty(axesProperty, mAxesEntity);
    qmlContext->setContextProperty(elementsProperty, mElementsEntity);
}

void    Scene::instantiate(const Point &point)
{
    Qt3DExtras::QCuboidMesh*    mesh = new Qt3DExtras::QCuboidMesh();
    Qt3DCore::QTransform*       transform = new Qt3DCore::QTransform();
    Qt3DExtras::QPhongMaterial* material = new Qt3DExtras::QPhongMaterial();
    Qt3DCore::QEntity*          entity = new Qt3DCore::QEntity(mElementsEntity);

    mesh->setXExtent(0.1f);
    mesh->setYExtent(0.1f);
    mesh->setZExtent(0.1f);

    transform->setTranslation(point.position);

    material->setDiffuse(QColor(QRgb(0xbeb32b)));
    material->setAmbient(QColor(QRgb(0xbeb32b)));

    entity->addComponent(mesh);
    entity->addComponent(material);
    entity->addComponent(transform);
}

void    Scene::releaseModel(Qt3DCore::QNode& model)
{
    QEntity*    entity = dynamic_cast<QEntity*>(&model);

    if (entity)
    {
        QComponentVector    components = entity->components();

        foreach (QComponent* component, components)
        {
            entity->removeComponent(component);
            delete component;
        }
    }

    QNodeVector nodes = model.childNodes();

    foreach (QNode* node, nodes)
    {
        releaseModel(*node);
        delete node;
    }
}
