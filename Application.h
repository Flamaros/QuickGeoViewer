#pragma once

#include <QtGui/QGuiApplication>

#include <QtCore/QVector>

#include "Scene.h"

class Object;

class QQmlApplicationEngine;

class Application: public QGuiApplication
{
    Q_OBJECT

public:
    Application(int &argc, char **argv, int flags = ApplicationFlags);
    ~Application();

    void    initialize();

    Q_INVOKABLE bool    parseUserInput(const QString& text);    /// return true if the input is valid
    Q_INVOKABLE void    clear();

private:
    bool    parseCommands(const QStringList &commands);
    void    clearObjects(QVector<Object*> objects);

    QQmlApplicationEngine*  mEngine;
    Scene*                  mScene;
    QVector<Object*>        mObjects;
};
