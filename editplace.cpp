#include "editplace.h"
#include "ui_editplace.h"

EditPlace::EditPlace(QSqlTableModel *model, int row, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EditPlace)
{
    ui->setupUi(this);
    this->model = model;
    this->row = row;
    ui->dateTimeEdit->setDateTime(model->data(model->index(row, 1)).toDateTime());
    ui->place_lineEdit->setText(model->data(model->index(row, 2)).toString());
}

EditPlace::~EditPlace()
{
    delete ui;
}

void EditPlace::on_save_pushButton_clicked()
{
    if (ui->place_lineEdit->text().isEmpty())
    {
        QMessageBox::warning(this, "Изменение информации", "Для продолжения, заполните все поля", QMessageBox::Ok);
    }
    else
    {
        model->setData(model->index(row, 1), ui->dateTimeEdit->dateTime());
        model->setData(model->index(row, 2), ui->place_lineEdit->text());
        model->submitAll();
        model->select();
        this->close();
    }
}

void EditPlace::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        this->close();
    }
}
