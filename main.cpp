#include <QGuiApplication>

#include "Application.h"
#include "Scene.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    Application app(argc, argv);

    app.initialize();

    return app.exec();
}
