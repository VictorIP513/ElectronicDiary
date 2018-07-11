#include "newworktask.h"
#include "ui_newworktask.h"

NewWorkTask::NewWorkTask(QSqlTableModel *model, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NewWorkTask)
{
    ui->setupUi(this);
    this->model = model;
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
}

NewWorkTask::~NewWorkTask()
{
    delete ui;
}

void NewWorkTask::on_add_pushButton_clicked()
{
    QDateTime dateTime = ui->dateTimeEdit->dateTime();
    QString task = ui->task_lineEdit->text();
    if (task.isEmpty())
    {
        QMessageBox::warning(this, "Добавление информации", "Для продолжения, заполните все поля", QMessageBox::Ok);
    }
    else
    {
        model->insertRows(model->rowCount(), 1);
        model->setData(model->index(model->rowCount() - 1, 1), dateTime);
        model->setData(model->index(model->rowCount() - 1, 2), task);
        model->submitAll();
        model->select();
        this->close();
    }
}

void NewWorkTask::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        this->close();
    }
}
