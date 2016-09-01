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

    qmlRegisterType<Scene>("Geo.Scene", 1, 0, "GeoScene");
    qmlRegisterType<Scene>("Geo.Object", 1, 0, "GeoObject");
    qmlRegisterType<Scene>("Geo.Point", 1, 0, "GeoPoint");

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

    if (parseCommands(commands) == false)
        return false;

    emit objectsChanged();
    return true;
}

void    Application::clear()
{
    Scene::singleton()->clear();
    clearObjects(mObjects);
}

bool    Application::parseCommands(const QStringList& commands)
{
    static QString  pointObject = "Point";
    static QString  parenthesisObject = "(";

    QList<QObject*> objects;

    for (int i = 0; i < commands.size(); i++)
    {
        QStringList nameAndObject;

        nameAndObject = commands[i].split('=');

        if (nameAndObject.size() == 1) // Objects are not named we generate a name
        {
            nameAndObject.push_front(QString::asprintf("point%d", int(objects.size())));
        }

        if (nameAndObject.size() != 2)
        {
            clearObjects(objects);
            return false;
        }

        if (nameAndObject[1].startsWith(pointObject) || nameAndObject[1].startsWith(parenthesisObject))
        {
            QString     position = nameAndObject[1];
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

            Point*  point = new Point();

            point->setName(nameAndObject[0]);
            point->setPosition(QVector3D(
                                   coordinates[0].toFloat()
                               , coordinates[1].toFloat()
                    , coordinates.size() > 2 ? coordinates[2].toFloat() : 0.0f));
            point->setLabel(QString("%1 (%2, %3, %4)").arg(point->name()).arg(point->position().x(), 0, 'f', 2).arg(point->position().y(), 0, 'f', 2).arg(point->position().z(), 0, 'f', 2));

            objects.append(point);
        }
    }

    Scene::singleton()->instantiate(objects);
    mObjects += objects;
    return objects.empty() == false;
}

void    Application::clearObjects(QList<QObject*>& objects)
{
    for (int i = 0; i < objects.size(); i++)
        delete objects[i];
}
