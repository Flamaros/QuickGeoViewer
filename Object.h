#pragma once

#include <QObject>
#include <QString>

#include <QtGui/QColor>

class Object : public QObject
{
    Q_OBJECT

    friend class Scene;

public:
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString label READ label WRITE setLabel NOTIFY labelChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

    Object();

    void    setName(const QString& name) {mName = name;}
    QString name() const {return mName;}

    void    setLabel(const QString& label) {mLabel = label;}
    QString label() const {return mLabel;}

    void    setColor(const QColor& color);
    QColor  color() const {return mColor;}

signals:
    void    nameChanged();
    void    labelChanged();
    void    colorChanged();

private:
    QString mName;
    QString mLabel;
    QColor  mColor;

    void*   mSceneData;
};
