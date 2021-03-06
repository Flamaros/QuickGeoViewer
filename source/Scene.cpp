#include "Scene.h"

#include "Camera.h"
#include "Object.h"
#include "Point.h"
#include "Polygon.h"

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
#include <Qt3DRender/QBuffer>
#include <Qt3DRender/QAttribute>

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

using namespace Qt3DCore;
using namespace Qt3DRender;
using namespace Qt3DInput;
using namespace Qt3DExtras;

namespace geo
{
    SceneEntity::SceneEntity(QNode* parent)
        : QEntity(parent)
    {
        if (Scene::singleton()->rootEntity() == nullptr)
            Scene::singleton()->initialize();

        Scene::singleton()->rootEntity()->setParent(this);
    }

    //==========================================================================

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

        generateAxis();

        updateQmlProperties();
    }

    void Scene::destroy()
    {
        clear();

        delete mElementsEntity;
        mElementsEntity = nullptr;
        delete mAxesEntity;
        mAxesEntity = nullptr;
        delete mRootEntity;
        mRootEntity = nullptr;

        updateQmlProperties();
    }

    void    Scene::instantiate(const QList<QObject*>& objects)
    {
        for (int i = 0; i < objects.size(); i++)
        {
            Point*          point;
            geo::Polygon*   polygon;

            point = dynamic_cast<Point*>(objects[i]);
            if (point != nullptr)
                instantiate(*point);
            polygon = dynamic_cast<geo::Polygon*>(objects[i]);
            if (polygon != nullptr)
                instantiate(*polygon);
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

    void    Scene::setObjectColor(const QColor& color, const Object& object)
    {
        QEntity*    entity = static_cast<QEntity*>(object.mSceneData);

        foreach (QComponent* component, entity->components())
        {
            QPhongMaterial* material = dynamic_cast<QPhongMaterial*>(component);

            if (material != nullptr)
            {
                material->setDiffuse(color);
                material->setAmbient(color);
            }
        }
    }

    void Scene::setObjectVisibility(bool visible, const Object& object)
    {
        QEntity*    entity = static_cast<QEntity*>(object.mSceneData);

        if (visible)
            entity->setParent(mElementsEntity);
        else
            entity->setParent(static_cast<QNode*>(nullptr));
    }

    bool Scene::isObjectVisible(const Object& object) const
    {
        QEntity*    entity = static_cast<QEntity*>(object.mSceneData);

        return entity->parent() == mElementsEntity;
    }

    //==============================================================================

    Scene::Scene()
        : mRootEntity(nullptr)
        , mAxesEntity(nullptr)
        , mElementsEntity(nullptr)
        , mAspectEngine(nullptr)
    {
        if (mInstance == nullptr)
            mInstance = this;
        updateQmlProperties();
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
            QCylinderMesh*          mesh = new QCylinderMesh();
            Qt3DCore::QTransform*   transform = new Qt3DCore::QTransform();
            QPhongMaterial*         material = new QPhongMaterial();
            QEntity*                entity = new QEntity(mAxesEntity);

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

    void    Scene::instantiate(Point& point)
    {
        QCuboidMesh*            mesh = new QCuboidMesh();
        Qt3DCore::QTransform*   transform = new Qt3DCore::QTransform();
        QPhongMaterial*         material = new QPhongMaterial();
        QEntity*                entity = new QEntity(mElementsEntity);

        Q_ASSERT(point.mSceneData == nullptr);

        mesh->setXExtent(0.1f);
        mesh->setYExtent(0.1f);
        mesh->setZExtent(0.1f);

        transform->setTranslation(point.position());

        material->setDiffuse(point.color());
        material->setAmbient(point.color());

        entity->addComponent(mesh);
        entity->addComponent(material);
        entity->addComponent(transform);

        point.mSceneData = entity;
    }

    void    Scene::instantiate(geo::Polygon& polygon)
    {
        static const int        strideLength = 3; // position : 3 floats
        QGeometryRenderer*      geometryRenderer = new QGeometryRenderer();
        Qt3DRender::QGeometry*  geometry = new Qt3DRender::QGeometry(geometryRenderer);
        Qt3DRender::QBuffer*    vertexDataBuffer = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer, geometry);
        QPhongMaterial*         material = new QPhongMaterial();
        QEntity*                entity = new QEntity(mElementsEntity);
        QByteArray              vertexBufferData;

        Q_ASSERT(polygon.mSceneData == nullptr);

        // geometry
        {
            vertexBufferData.resize(polygon.polygon().size() * (strideLength) * sizeof(float));

            float*  rawVertexArray = reinterpret_cast<float*>(vertexBufferData.data());

            int index = 0;
            for (const QVector3D& point : polygon.polygon())
            {
                rawVertexArray[index++] = point.x();
                rawVertexArray[index++] = point.y();
                rawVertexArray[index++] = point.z();
            }

            vertexDataBuffer->setData(vertexBufferData);

            // Attributes
            Qt3DRender::QAttribute* positionAttribute = new Qt3DRender::QAttribute();

            positionAttribute->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
            positionAttribute->setBuffer(vertexDataBuffer);
            positionAttribute->setDataType(Qt3DRender::QAttribute::Float);
            positionAttribute->setDataSize(3);
            positionAttribute->setByteOffset(0);
            positionAttribute->setByteStride(strideLength * sizeof(float));
            positionAttribute->setCount(polygon.polygon().size());
            positionAttribute->setName(Qt3DRender::QAttribute::defaultPositionAttributeName());

            geometry->addAttribute(positionAttribute);

            geometryRenderer->setInstanceCount(1);
            geometryRenderer->setIndexOffset(0);
            geometryRenderer->setFirstInstance(0);
            geometryRenderer->setPrimitiveType(Qt3DRender::QGeometryRenderer::LineLoop);
            geometryRenderer->setGeometry(geometry);

            geometryRenderer->setVertexCount(polygon.polygon().size());
        }

        // material
        {
            material->setDiffuse(polygon.color());
            material->setAmbient(polygon.color());
        }

        entity->addComponent(geometryRenderer);
        entity->addComponent(material);

        polygon.mSceneData = entity;
    }

    void    Scene::releaseModel(QNode& model)
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
}
