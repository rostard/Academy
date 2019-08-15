#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "TaskDialog.h"

#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->addButton, &QAbstractButton::clicked, this, &MainWindow::AddDialog);
    connect(ui->deleteButton, &QAbstractButton::clicked, this, &MainWindow::askToDeleteTask);
   // connect(ui->listView, &QListView::c)
    ui->listView->setModel(new QStandardItemModel);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::AddDialog()
{
    auto* d = new TaskDialog(this);
    connect(d, &TaskDialog::canceled, this, &MainWindow::cancelCreatingTask);
    connect(d, &TaskDialog::accepted, [this](QString name, int priority){createdTask(Task(name.toStdString(), priority));});
    d->exec();
}

QString getFormatedTime(std::chrono::seconds time)
{

    int hours = time.count() / 3600;
    int minutes = (time.count() % 3600) / 60;
    int seconds = time.count() % 60;
    return QString::number(hours) + ":" + QString::number(minutes) + ":" + QString::number(seconds);
}
void MainWindow::updateTasks()
{
    if(mp_tasks)
    {
        int rows = ui->listView->model()->rowCount();
        /*set number of row and column, listview only support 1 column */
        int nrow = mp_tasks->size();
        if(rows == nrow)
        {
            for(int i = 0; i < rows; ++i)
            {
                auto task = mp_tasks->getTask(i);

                QModelIndex vIndex = ui->listView->model()->index(i,0);
                ui->listView->model()->setData(vIndex, QString(task.getName().c_str()) + "\t" + getFormatedTime(task.getTimer()));

            }
            return;
        }
        QStandardItemModel *model = new QStandardItemModel( nrow, 1, this );

        //fill model value
        for( int r=0; r<nrow; r++ )
        {
            auto task = mp_tasks->getTask(r);
                QStandardItem *item = new QStandardItem(QString(task.getName().c_str()) + "\t" + getFormatedTime(task.getTimer()) );
                model->setItem(r, 0, item);
        }

        ui->listView->setModel(model);
    }
}

void MainWindow::askToLoadTasks()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Do you want to load previous session?"),
                                    "Do you want to load previous session?",
                                    QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        mp_tasks->readFile("tasks.csv");
    }
    else
    {

    }
}



void MainWindow::SetTaskList(TaskList* list)
{
    mp_tasks = list;
}

void MainWindow::askToDeleteTask()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr(""),
                                    "Do you want to delete selected task?",
                                    QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        int index = ui->listView->selectionModel()->selectedIndexes().first().row();
        deletedTask(index);
    }
}

