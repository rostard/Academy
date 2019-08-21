#include <QGuiApplication>
#include "MainWindow.h"
#include "TaskList.h"
#include <QUrl>
#include <QTimer>
int main(int argc, char** argv)
{

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);
    TaskModel taskModel;
    taskModel.readFile("tasks.json");

    MainWindow w;
    w.setModel("tasks", &taskModel);

    w.LoadEngine(QStringLiteral("qrc:/main.qml"), &app);

    QTimer *timer = new QTimer(&w);
    QObject::connect(timer, &QTimer::timeout, &taskModel, &TaskModel::incrementTaskTimers);
    timer->start(1000);

    QObject::connect(&app, &QGuiApplication::aboutToQuit, &taskModel, [&taskModel]{taskModel.saveFile("tasks.json");});

    return app.exec();
}
