#include "editpersonalgoal.h"
#include "ui_editpersonalgoal.h"

EditPersonalGoal::EditPersonalGoal(QSqlTableModel *model, int row, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EditPersonalGoal)
{
    ui->setupUi(this);
    this->model = model;
    this->row = row;
    ui->dateTimeEdit->setDateTime(model->data(model->index(row, 1)).toDateTime());
    ui->goal_lineEdit->setText(model->data(model->index(row, 2)).toString());
    currentPhoto = 0;
    getPhotoList();
    if (photoList.isEmpty())
    {
        hideElements();
    }
    else
    {
        addPhotoToLabel();
    }
}

EditPersonalGoal::~EditPersonalGoal()
{
    delete ui;
}

void EditPersonalGoal::closeEvent(QCloseEvent*)
{
    if (!saved)
    {
        foreach (const QString &str, addedPhotoList)
        {
            QFile(str).remove();
        }
    }
}

void EditPersonalGoal::showElements()
{
    ui->image_label->show();
    ui->backPhoto_pushButton->show();
    ui->nextPhoto_pushButton->show();
    ui->deletePhoto_pushButton->show();
}

void EditPersonalGoal::hideElements()
{
    ui->image_label->hide();
    ui->backPhoto_pushButton->hide();
    ui->nextPhoto_pushButton->hide();
    ui->deletePhoto_pushButton->hide();
}

void EditPersonalGoal::getPhotoList()
{
    QString photos = model->data(model->index(row, 3)).toString();
    if (!photos.isEmpty())
    {
        photoList = photos.split("?");
    }
    fillPhotoLabel();
}

void EditPersonalGoal::addPhotoToLabel()
{
    QPixmap *pixmap = new QPixmap(photoList.at(currentPhoto));
    if (pixmap->width() > 750 || pixmap->height() > 330)
    {
        QSize pixmapSize(750, 330);
        *pixmap = pixmap->scaled(pixmapSize, Qt::KeepAspectRatio);
    }
    ui->image_label->setPixmap(*pixmap);
    if (currentPhoto == 0)
    {
        ui->backPhoto_pushButton->setEnabled(false);
    }
    else
    {
        ui->backPhoto_pushButton->setEnabled(true);
    }
    if (currentPhoto == photoList.length() - 1)
    {
        ui->nextPhoto_pushButton->setEnabled(false);
    }
    else
    {
        ui->nextPhoto_pushButton->setEnabled(true);
    }
}

void EditPersonalGoal::on_save_pushButton_clicked()
{
    if (ui->goal_lineEdit->text().isEmpty())
    {
        QMessageBox::warning(this, "Изменение информации", "Для продолжения, заполните все поля", QMessageBox::Ok);
    }
    else
    {
        fillPhotoTOBD();
        deletePhoto();
        model->setData(model->index(row, 1), ui->dateTimeEdit->dateTime());
        model->setData(model->index(row, 2), ui->goal_lineEdit->text());
        model->setData(model->index(row, 3), photoNames);
        model->submitAll();
        model->select();
        saved = true;
        this->close();
    }
}

void EditPersonalGoal::deletePhoto()
{
    foreach (const QString &str, deletePhotoList)
    {
        QFile(str).remove();
    }
}

void EditPersonalGoal::fillPhotoTOBD()
{
    foreach (const QString &str, photoList)
    {
        photoNames += str + "?";
    }
    photoNames.remove(photoNames.length() - 1, 1);
}

void EditPersonalGoal::on_photo_pushButton_clicked()
{
    QString currentDir = QDir::currentPath();
        newPhotoList = QFileDialog::getOpenFileNames(this, "Выбор фотографий", currentDir,
            "Файлы изображений (*.png *.jpg *.bmp *.gif)");
    if (!newPhotoList.isEmpty())
    {
        showElements();
        photo();
        addPhotoToLabel();
        fillPhotoLabel();
    }
}

void EditPersonalGoal::photo()
{
    QString currentDir = QDir::currentPath();
    foreach (const QString &str, newPhotoList)
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
        photoList << newFile;
        addedPhotoList << newFile;
    }
}

void EditPersonalGoal::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        this->close();
    }
}

void EditPersonalGoal::fillPhotoLabel()
{
    if (photoList.isEmpty())
    {
        ui->photo_label->setText("Фотографии отсутствуют");
    }
    else
    {
        ui->photo_label->setText("Имеется " + QString::number(photoList.length()) + " фото");
    }
}

void EditPersonalGoal::on_backPhoto_pushButton_clicked()
{
    --currentPhoto;
    addPhotoToLabel();
}

void EditPersonalGoal::on_nextPhoto_pushButton_clicked()
{
    ++currentPhoto;
    addPhotoToLabel();
}

void EditPersonalGoal::on_deletePhoto_pushButton_clicked()
{
    if (photoList.length() == 1)
    {
        deletePhotoList << photoList.at(currentPhoto);
        photoList.removeAt(currentPhoto);
        currentPhoto = 0;
        hideElements();
    }
    else
    {
        deletePhotoList << photoList.at(currentPhoto);
        photoList.removeAt(currentPhoto);
        if (currentPhoto != 0)
        {
            --currentPhoto;
        }
        addPhotoToLabel();
    }
    fillPhotoLabel();
}
