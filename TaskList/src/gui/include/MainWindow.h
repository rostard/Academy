#pragma once

#include "gui_export.h"
#include <QMainWindow>

#include "TaskList.h"

namespace Ui {
class MainWindow;
}

class GUI_EXPORT MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    void AddDialog();

    void SetTaskList(TaskList* list);

    ~MainWindow();
signals:
    void startCreatingTask();
    void cancelCreatingTask();
    void createdTask(Task task);

    void startDeletingTask();
    void cancelDeletingTask();
    void deletedTask(int index);

public slots:
    void updateTasks();
    void askToLoadTasks();
private slots:
    void askToDeleteTask(int index);
private:
    Ui::MainWindow *ui;
    TaskList* mp_tasks;

};
