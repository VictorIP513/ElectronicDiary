#include "newplace.h"
#include "ui_newplace.h"

NewPlace::NewPlace(QSqlTableModel *model, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NewPlace)
{
    ui->setupUi(this);
    this->model = model;
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
}

NewPlace::~NewPlace()
{
    delete ui;
}

void NewPlace::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        this->close();
    }
}

void NewPlace::on_add_pushButton_clicked()
{
    QDateTime dateTime = ui->dateTimeEdit->dateTime();
    QString place = ui->place_lineEdit->text();
    if (place.isEmpty())
    {
        QMessageBox::warning(this, "Добавление информации", "Для продолжения, заполните все поля", QMessageBox::Ok);
    }
    else
    {
        model->insertRows(model->rowCount(), 1);
        model->setData(model->index(model->rowCount() - 1, 1), dateTime);
        model->setData(model->index(model->rowCount() - 1, 2), place);
        model->submitAll();
        model->select();
        this->close();
    }
}
