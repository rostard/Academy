#include "TaskDialog.h"
#include "ui_TaskDialog.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QMessageBox>
#include <iostream>
TaskDialog::TaskDialog(QWidget* parent, Qt::WindowFlags flags): QDialog( parent, flags ), ui(new Ui::TaskDialog)
{
    ui->setupUi(this);

    connect(ui->okButton, &QAbstractButton::clicked,[this]{this->accept(); this->accepted(ui->textEdit->toPlainText()); std::cout<<"HI";});
    connect(ui->cancelButton, &QAbstractButton::clicked, [this]{this->close(); this->canceled();});
}

TaskDialog::~TaskDialog(){}
