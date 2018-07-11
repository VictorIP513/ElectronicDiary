#include "newstudytask.h"
#include "ui_newstudytask.h"

NewStudyTask::NewStudyTask(QSqlTableModel *model, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NewStudyTask)
{
    ui->setupUi(this);
    this->model = model;
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
}

NewStudyTask::~NewStudyTask()
{
    delete ui;
}

void NewStudyTask::on_add_pushButton_clicked()
{
    QDateTime dateTime = ui->dateTimeEdit->dateTime();
    QString lesson = ui->lesson_lineEdit->text();
    QString task = ui->task_lineEdit->text();
    if (lesson.isEmpty() || task.isEmpty())
    {
        QMessageBox::warning(this, "Добавление информации", "Для продолжения, заполните все поля", QMessageBox::Ok);
    }
    else
    {
        model->insertRows(model->rowCount(), 1);
        model->setData(model->index(model->rowCount() - 1, 1), dateTime);
        model->setData(model->index(model->rowCount() - 1, 2), lesson);
        model->setData(model->index(model->rowCount() - 1, 3), task);
        model->submitAll();
        model->select();
        this->close();
    }
}

void NewStudyTask::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        this->close();
    }
}
