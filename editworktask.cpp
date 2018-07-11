#include "editworktask.h"
#include "ui_editworktask.h"

EditWorkTask::EditWorkTask(QSqlTableModel *model, int row, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EditWorkTask)
{
    ui->setupUi(this);
    this->model = model;
    this->row = row;
    ui->dateTimeEdit->setDateTime(model->data(model->index(row, 1)).toDateTime());
    ui->task_lineEdit->setText(model->data(model->index(row, 2)).toString());
}

EditWorkTask::~EditWorkTask()
{
    delete ui;
}

void EditWorkTask::on_save_pushButton_clicked()
{
    if (ui->task_lineEdit->text().isEmpty())
    {
        QMessageBox::warning(this, "Изменение информации", "Для продолжения, заполните все поля", QMessageBox::Ok);
    }
    else
    {
        model->setData(model->index(row, 1), ui->dateTimeEdit->dateTime());
        model->setData(model->index(row, 2), ui->task_lineEdit->text());
        model->submitAll();
        model->select();
        this->close();
    }
}

void EditWorkTask::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        this->close();
    }
}
