#include <QApplication>
#include "MainWindow.h"
#include <QStateMachine>
#include <QTimer>
#include <QFileInfo>
#include <QMessageBox>
int main(int argc, char** argv)
{

    QApplication a(argc, argv);
    MainWindow w;

    TaskList taskList;
    w.SetTaskList(&taskList);


    QStateMachine machine;
    QState *stateLoad = new QState();
    QState *stateIdle = new QState();
    QState *stateDelete = new QState();
    QState *stateCreate = new QState();
    QState *stateEdit = new QState();
    QState *stateExit = new QState();

    stateIdle->addTransition(&w, &MainWindow::startCreatingTask, stateCreate);
    stateCreate->addTransition(&w, &MainWindow::cancelCreatingTask, stateIdle);
    stateCreate->addTransition(&w, &MainWindow::createdTask, stateIdle);


    machine.addState(stateIdle);
    machine.addState(stateDelete);
    machine.addState(stateCreate);
    machine.addState(stateEdit);
    machine.addState(stateExit);
    machine.setInitialState(stateIdle);


    QObject::connect(&w, &MainWindow::createdTask, &taskList, &TaskList::addTask);
    QObject::connect(&w, &MainWindow::createdTask, &w, &MainWindow::updateTasks);
    QObject::connect(&w, &MainWindow::deletedTask, &taskList, &TaskList::deleteTask);

    QObject::connect(&a, &QApplication::aboutToQuit, [&taskList]{taskList.saveFile("tasks.csv");});
    w.show();

    QFileInfo file("tasks.csv");
    if(file.exists() && file.isFile())
    {
        w.askToLoadTasks();
    }




    QTimer *timer = new QTimer(&w);
    QObject::connect(timer, &QTimer::timeout, &taskList, &TaskList::incrementTaskTimers);
    QObject::connect(timer, &QTimer::timeout, &w, &MainWindow::updateTasks);
    timer->start(1000);

    return a.exec();
}
