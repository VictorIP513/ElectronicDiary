#ifndef NEWSTUDYTASK_H
#define NEWSTUDYTASK_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QMessageBox>
#include <QKeyEvent>

namespace Ui {
class NewStudyTask;
}

class NewStudyTask : public QMainWindow
{
    Q_OBJECT

public:
    explicit NewStudyTask(QSqlTableModel *model, QWidget *parent = 0);
    ~NewStudyTask();

private slots:
    void on_add_pushButton_clicked();

protected:
    virtual void keyPressEvent(QKeyEvent * event);

private:
    Ui::NewStudyTask *ui;
    QSqlTableModel *model;
};

#endif // NEWSTUDYTASK_H
