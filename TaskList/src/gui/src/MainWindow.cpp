#include "MainWindow.h"
#include <QGuiApplication>
#include <QQmlContext>
MainWindow::MainWindow(QWidget *parent)
{
    connect(&engine, &QQmlApplicationEngine::objectCreated, this, &MainWindow::qmlObjectCreated);
}

MainWindow::~MainWindow()
{
}

void MainWindow::setModel(QString name, QAbstractItemModel *model)
{
    engine.rootContext()->setContextProperty(name, model);
}

void MainWindow::LoadEngine(const QUrl &url, QGuiApplication *app)
{

    QObject::connect(&engine,  &QQmlApplicationEngine::objectCreated,
                     app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);
}
