#pragma once

#include <QtGui/QGuiApplication>

#include <QtCore/QVector>

class Object;

class QQmlApplicationEngine;

class Application: public QGuiApplication
{
    Q_OBJECT

public:
    Q_PROPERTY(QList<QObject*> objects READ objects NOTIFY objectsChanged)

    Application(int &argc, char **argv, int flags = ApplicationFlags);
    ~Application();

    void    initialize();

    Q_INVOKABLE bool    parseUserInput(const QString& text);    /// return true if the input is valid
    Q_INVOKABLE void    clear();

    QList<QObject*>     objects() const {return mObjects;}

signals:
    void    objectsChanged();

private:
    bool    parseCommands(const QStringList& commands);
    void    clearObjects(QList<QObject*>& objects);

    QQmlApplicationEngine*  mEngine;
    QList<QObject*>         mObjects;
};
