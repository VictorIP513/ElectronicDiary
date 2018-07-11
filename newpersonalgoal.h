#ifndef NEWPERSONALGOAL_H
#define NEWPERSONALGOAL_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QMessageBox>
#include <QKeyEvent>
#include <QFileDialog>

namespace Ui {
class NewPersonalGoal;
}

class NewPersonalGoal : public QMainWindow
{
    Q_OBJECT

public:
    explicit NewPersonalGoal(QSqlTableModel *model, QWidget *parent = 0);
    ~NewPersonalGoal();

protected:
    virtual void keyPressEvent(QKeyEvent * event);

private slots:
    void on_add_pushButton_clicked();
    void on_photo_pushButton_clicked();

private:
    Ui::NewPersonalGoal *ui;
    void photo();
    QSqlTableModel *model;
    QString photoNames;
    QStringList photoList;
    const QString lastPhotoIDFile = "lastPhotoID.dat";
};

#endif // NEWPERSONALGOAL_H
