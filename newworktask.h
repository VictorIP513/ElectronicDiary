#ifndef NEWWORKTASK_H
#define NEWWORKTASK_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QMessageBox>
#include <QKeyEvent>

namespace Ui {
class NewWorkTask;
}

class NewWorkTask : public QMainWindow
{
    Q_OBJECT

public:
    explicit NewWorkTask(QSqlTableModel *model, QWidget *parent = 0);
    ~NewWorkTask();

private slots:
    void on_add_pushButton_clicked();

protected:
    virtual void keyPressEvent(QKeyEvent * event);

private:
    Ui::NewWorkTask *ui;
    QSqlTableModel *model;
};

#endif // NEWWORKTASK_H
