#pragma once
#include <QDialog>

namespace Ui {
class TaskDialog;
}

class TaskDialog : public QDialog
{
    Q_OBJECT
public:
    TaskDialog(QWidget * parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~TaskDialog();

signals:
    void accepted(QString taskName, int priority = 2);
    void canceled();
private:
    Ui::TaskDialog* ui;
};
