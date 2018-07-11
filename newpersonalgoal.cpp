#include "newpersonalgoal.h"
#include "ui_newpersonalgoal.h"

NewPersonalGoal::NewPersonalGoal(QSqlTableModel *model, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NewPersonalGoal)
{
    ui->setupUi(this);
    this->model = model;
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
}

NewPersonalGoal::~NewPersonalGoal()
{
    delete ui;
}

void NewPersonalGoal::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        this->close();
    }
}

void NewPersonalGoal::on_add_pushButton_clicked()
{
    QDateTime dateTime = ui->dateTimeEdit->dateTime();
    QString goal = ui->goal_lineEdit->text();
    if (goal.isEmpty())
    {
        QMessageBox::warning(this, "Добавление информации", "Для продолжения, заполните все поля", QMessageBox::Ok);
    }
    else
    {
        photo();
        model->insertRows(model->rowCount(), 1);
        model->setData(model->index(model->rowCount() - 1, 1), dateTime);
        model->setData(model->index(model->rowCount() - 1, 2), goal);
        if (!photoNames.isEmpty())
        {
            model->setData(model->index(model->rowCount() - 1, 3), photoNames);
        }
        model->submitAll();
        model->select();
        this->close();
    }
}

void NewPersonalGoal::on_photo_pushButton_clicked()
{
    if (!photoList.isEmpty())
    {
        QMessageBox::StandardButton reply = QMessageBox::warning(this, "Добавление информации",
                "Вы уже прикрепляли фотографии. Прикрепить фотографии заного?", QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::No)
        {
            return;
        }
    }
    ui->countPhoto_label->setText("");
    QString currentDir = QDir::currentPath();
    photoList = QFileDialog::getOpenFileNames(this, "Выбор фотографий", currentDir,
            "Файлы изображений (*.png *.jpg *.bmp *.gif)");
    if (!photoList.isEmpty())
    {
        ui->countPhoto_label->setText("Прикреплено " + QString::number(photoList.length()) + " фото");
    }
}

void NewPersonalGoal::photo()
{
    QString currentDir = QDir::currentPath();
    foreach (const QString &str, photoList)
    {
        QFileInfo *file = new QFileInfo(str);
        QString suffix = file->suffix();
        QString lastPhotoID;
        if (!QFile::exists(currentDir + "\\" + lastPhotoIDFile))
        {
            QDir().mkdir(currentDir + "\\photo");
            QFile *file = new QFile(currentDir + "\\" + lastPhotoIDFile);
            file->open(QIODevice::WriteOnly);
            file->write("0");
            file->close();
            lastPhotoID = "0";
        }
        else
        {
            QFile *file = new QFile(currentDir + "\\" + lastPhotoIDFile);
            file->open(QIODevice::ReadOnly);
            lastPhotoID = file->readAll();
            file->close();
            QByteArray arr;
            arr.setNum(lastPhotoID.toInt() + 1);
            file->open(QIODevice::WriteOnly);
            file->write(arr);
            file->close();
        }
        QString newFile = currentDir + "\\photo\\photo" + lastPhotoID + "." + suffix;
        QFile::copy(str, newFile);
        photoNames += newFile + "?";
    }
    photoNames.remove(photoNames.length() - 1, 1);
}
