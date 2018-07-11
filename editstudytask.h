#ifndef EDITSTUDYTASK_H
#define EDITSTUDYTASK_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QMessageBox>
#include <QKeyEvent>

namespace Ui {
class EditStudyTask;
}

class EditStudyTask : public QMainWindow
{
    Q_OBJECT

public:
    explicit EditStudyTask(QSqlTableModel *model, int row, QWidget *parent = 0);
    ~EditStudyTask();

private slots:
    void on_save_pushButton_clicked();

protected:
    virtual void keyPressEvent(QKeyEvent * event);

private:
    Ui::EditStudyTask *ui;
    QSqlTableModel *model;
    int row;
};

#endif // EDITSTUDYTASK_H
