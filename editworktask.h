#ifndef EDITWORKTASK_H
#define EDITWORKTASK_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QMessageBox>
#include <QKeyEvent>

namespace Ui {
class EditWorkTask;
}

class EditWorkTask : public QMainWindow
{
    Q_OBJECT

public:
    explicit EditWorkTask(QSqlTableModel *model, int row, QWidget *parent = 0);
    ~EditWorkTask();

private slots:
    void on_save_pushButton_clicked();

protected:
    virtual void keyPressEvent(QKeyEvent * event);

private:
    Ui::EditWorkTask *ui;
    QSqlTableModel *model;
    int row;
};

#endif // EDITWORKTASK_H
