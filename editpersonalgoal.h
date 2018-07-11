#ifndef EDITPERSONALGOAL_H
#define EDITPERSONALGOAL_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QMessageBox>
#include <QKeyEvent>
#include <QFileDialog>

namespace Ui {
class EditPersonalGoal;
}

class EditPersonalGoal : public QMainWindow
{
    Q_OBJECT

public:
    explicit EditPersonalGoal(QSqlTableModel *model, int row, QWidget *parent = 0);
    ~EditPersonalGoal();

private slots:
    void on_save_pushButton_clicked();
    void on_photo_pushButton_clicked();
    void on_backPhoto_pushButton_clicked();
    void on_nextPhoto_pushButton_clicked();
    void on_deletePhoto_pushButton_clicked();

protected:
    virtual void keyPressEvent(QKeyEvent * event);
    virtual void closeEvent(QCloseEvent *event);

private:
    Ui::EditPersonalGoal *ui;
    void getPhotoList();
    void addPhotoToLabel();
    void showElements();
    void hideElements();
    void fillPhotoLabel();
    void photo();
    void fillPhotoTOBD();
    void deletePhoto();
    QSqlTableModel *model;
    QStringList photoList;
    QStringList newPhotoList;
    QStringList deletePhotoList;
    QStringList addedPhotoList;
    QString photoNames;
    int row;
    int currentPhoto;
    const QString lastPhotoIDFile = "lastPhotoID.dat";
    bool saved = false;
};

#endif // EDITPERSONALGOAL_H
