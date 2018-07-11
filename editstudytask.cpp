#include "editstudytask.h"
#include "ui_editstudytask.h"

EditStudyTask::EditStudyTask(QSqlTableModel *model, int row, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EditStudyTask)
{
    ui->setupUi(this);
    this->model = model;
    this->row = row;
    ui->dateTimeEdit->setDateTime(model->data(model->index(row, 1)).toDateTime());
    ui->lesson_lineEdit->setText(model->data(model->index(row, 2)).toString());
    ui->task_lineEdit->setText(model->data(model->index(row, 3)).toString());
}

EditStudyTask::~EditStudyTask()
{
    delete ui;
}

void EditStudyTask::on_save_pushButton_clicked()
{
    if (ui->lesson_lineEdit->text().isEmpty() || ui->task_lineEdit->text().isEmpty())
    {
        QMessageBox::warning(this, "Изменение информации", "Для продолжения, заполните все поля", QMessageBox::Ok);
    }
    else
    {
        model->setData(model->index(row, 1), ui->dateTimeEdit->dateTime());
        model->setData(model->index(row, 2), ui->lesson_lineEdit->text());
        model->setData(model->index(row, 3), ui->task_lineEdit->text());
        model->submitAll();
        model->select();
        this->close();
    }
}

void EditStudyTask::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        this->close();
    }
}
