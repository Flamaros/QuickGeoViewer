#include "Application.h"

#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlContext>

#include <QRegularExpression>

#include "Scene.h"
#include "Object.h"
#include "Point.h"
#include "Polygon.h"

Application::Application(int& argc, char** argv, int flags)
    : QGuiApplication(argc, argv, flags)
    , mEngine(new QQmlApplicationEngine)
{
}

Application::~Application()
{
    geo::Scene::singleton()->destroy();
    mEngine->rootContext()->setContextProperty("application", nullptr);

    delete mEngine;
}

void    Application::initialize()
{
    geo::Scene::qmlContext = mEngine->rootContext();

//    geo::Scene::singleton();
    qmlRegisterType<geo::SceneEntity>("Geo.Scene", 1, 0, "GeoScene");
    qmlRegisterType<geo::Object>("Geo.Object", 1, 0, "GeoObject");
    qmlRegisterType<geo::Point>("Geo.Point", 1, 0, "GeoPoint");
    qmlRegisterType<geo::Polygon>("Geo.Polygon", 1, 0, "GeoPolygon");

    mEngine->rootContext()->setContextProperty("application", this);
    mEngine->load(QUrl(QLatin1String("qrc:/qml/main.qml")));

//    geo::Scene::singleton()->initialize();
}

bool    Application::parseUserInput(const QString& text)
{
    static QString  executeBegin = "Execute[{";
    static QString  executeEnd = "}]";

    QString     withoutSpaces = text;
    QStringList commands;

    withoutSpaces.remove(QRegularExpression("\\s+"));

    if (withoutSpaces.startsWith("Execute[{") && text.endsWith("}]"))
    {
        QString commandsString = withoutSpaces;

        commandsString.remove(0, executeBegin.length());
        commandsString.remove(commandsString.length() - executeEnd.length(), executeEnd.length());

        commands = commandsString.split("\",\"");

        for (int i = 0; i < commands.size(); i++)
            commands[i].remove('\"');

    }
    else
        commands.push_back(withoutSpaces);

    if (commands.empty())
        return false;

    if (parseCommands(commands) == false)
        return false;

    emit objectsChanged();
    return true;
}

void    Application::clear()
{
    geo::Scene::singleton()->clear();
    clearObjects(mObjects);
    emit objectsChanged();
}

bool    Application::parseCommands(const QStringList& commands)
{
    static QString  pointObject = "Point";
    static QString  parenthesisObject = "(";
    static QString  polygonObject = "Polygon";

    QList<QObject*> objects;

    for (int i = 0; i < commands.size(); i++)
    {
        QStringList nameAndObject;

        nameAndObject = commands[i].split('=');

        if (nameAndObject.size() == 0
            || nameAndObject.size() > 2)
        {
            clearObjects(objects);
            return false;
        }

        // Point
        if (nameAndObject.last().startsWith(pointObject) || nameAndObject.last().startsWith(parenthesisObject))
        {
            if (nameAndObject.size() == 1) // Object is not named we generate one
            {
                nameAndObject.push_front(QString::asprintf("point %03d", int(mObjects.size() + objects.size())));
            }

            QString&    position = nameAndObject[1];
            QStringList coordinates;

            position.remove(pointObject);
            position.remove("[{");
            position.remove("}]");
            position.remove("(");
            position.remove(")");
            coordinates = position.split(',');

            if (coordinates.size() < 2)
            {
                clearObjects(objects);
                return false;
            }

            geo::Point* point = new geo::Point();

            point->setName(nameAndObject[0]);
            point->setPosition(QVector3D(
                                   coordinates[0].toFloat(),
                                   coordinates[1].toFloat(),
                                   coordinates.size() > 2 ? coordinates[2].toFloat() : 0.0f));
            point->setLabel(QString("%1 (%2, %3, %4)").arg(point->name()).arg(point->position().x(), 0, 'f', 2).arg(point->position().y(), 0, 'f', 2).arg(point->position().z(), 0, 'f', 2));

            objects.append(point);
        }
        // Polygon
        else if (nameAndObject.last().startsWith(polygonObject))
        {
            if (nameAndObject.size() == 1) // Object is not named we generate one
            {
                nameAndObject.push_front(QString::asprintf("polygon %03d", int(mObjects.size() + objects.size())));
            }

            QString&    pointsString = nameAndObject[1];
            QStringList points;

            pointsString.remove(polygonObject);
            pointsString.remove("[(");
            pointsString.remove(")]");
            points = pointsString.split("),(");

            geo::Polygon*   polygon = new geo::Polygon();
            QStringList     coordinates;

            polygon->setName(nameAndObject[0]);
            polygon->setLabel(nameAndObject[0]);
            polygon->polygon().reserve(points.size());
            for (const QString& point : points)
            {
                coordinates = point.split(',');

                if (coordinates.size() < 2)
                {
                    clearObjects(objects);
                    return false;
                }

                polygon->polygon().resize(polygon->polygon().size() + 1);
                polygon->polygon().back() = QVector3D(
                                                coordinates[0].toFloat(),
                                                coordinates[1].toFloat(),
                                                coordinates.size() > 2 ? coordinates[2].toFloat() : 0.0f);
            }

            objects.append(polygon);
        }
    }

    geo::Scene::singleton()->instantiate(objects);
    mObjects += objects;
    return objects.empty() == false;
}

void    Application::clearObjects(QList<QObject*>& objects)
{
    for (int i = 0; i < objects.size(); i++)
        delete objects[i];
    objects.clear();
}
