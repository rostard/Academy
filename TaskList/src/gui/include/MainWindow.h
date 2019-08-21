#pragma once

#include "gui_export.h"
#include <QObject>
#include <QQmlApplicationEngine>
#include <QAbstractItemModel>
class QGuiApplication;

class GUI_EXPORT MainWindow : public QObject
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
\
    void LoadEngine(const QUrl& url, QGuiApplication* app);

    ~MainWindow();

    void setModel(QString name, QAbstractItemModel* model);
signals:
    void qmlObjectCreated(QObject *object, const QUrl &url);

private:
    QQmlApplicationEngine engine;
};
