#pragma once

#include <QObject>
#include <QString>

class Object : public QObject
{
    Q_OBJECT

public:
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString label READ label WRITE setLabel NOTIFY labelChanged)

    void    setName(const QString& name) {mName = name;}
    QString name() const {return mName;}

    void    setLabel(const QString& label) {mLabel = label;}
    QString label() const {return mLabel;}

signals:
    void    nameChanged();
    void    labelChanged();

private:
    QString mName;
    QString mLabel;
};
