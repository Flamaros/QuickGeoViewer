#include "Application.h"

#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlContext>

#include <QRegularExpression>

#include "Object.h"
#include "Point.h"

Application::Application(int& argc, char** argv, int flags)
    : QGuiApplication(argc, argv, flags)
    , mEngine(new QQmlApplicationEngine)
{
}

Application::~Application()
{
    mEngine->rootContext()->setContextProperty("application", nullptr);

    delete mScene;
    delete mEngine;
}

void    Application::initialize()
{
    Scene::qmlContext = mEngine->rootContext();

    qmlRegisterType<Scene>("Scene", 1, 0, "GeoScene");

    mEngine->rootContext()->setContextProperty("application", this);
    mEngine->load(QUrl(QLatin1String("qrc:/main.qml")));

    Scene::singleton()->initialize();
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

    return parseCommands(commands);
}

void    Application::clear()
{
    Scene::singleton()->clear();
    clearObjects(mObjects);
}

bool    Application::parseCommands(const QStringList& commands)
{
    static QString  pointObject = "Point";

    QVector<Object*>    objects;

    for (int i = 0; i < commands.size(); i++)
    {
        QStringList nameAndObject;

        nameAndObject = commands[i].split('=');

        if (nameAndObject.size() != 2)
        {
            clearObjects(objects);
            return false;
        }

        if (nameAndObject[1].startsWith(pointObject))
        {
            QString     position = nameAndObject[1];
            QStringList coordinates;

            position.remove(0, pointObject.size());
            position.remove("[{");
            position.remove("}]");
            coordinates = position.split(',');

            if (coordinates.size() < 2)
            {
                clearObjects(objects);
                return false;
            }

            Point*  point = new Point;

            point->name = nameAndObject[0];
            point->position = QVector3D(
                        coordinates[0].toFloat()
                    , coordinates[1].toFloat()
                    , coordinates.size() > 2 ? coordinates[2].toFloat() : 0.0f);

            objects.push_back(point);
        }
    }

    Scene::singleton()->instantiate(objects);
    mObjects += objects;
    return objects.empty() == false;
}

void    Application::clearObjects(QVector<Object*> objects)
{
    for (int i = 0; i < objects.size(); i++)
        delete objects[i];
}
