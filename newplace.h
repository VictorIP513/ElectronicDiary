#ifndef NEWPLACE_H
#define NEWPLACE_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QMessageBox>
#include <QKeyEvent>

namespace Ui {
class NewPlace;
}

class NewPlace : public QMainWindow
{
    Q_OBJECT

public:
    explicit NewPlace(QSqlTableModel *model, QWidget *parent = 0);
    ~NewPlace();

private slots:
    void on_add_pushButton_clicked();

protected:
    virtual void keyPressEvent(QKeyEvent * event);

private:
    Ui::NewPlace *ui;
    QSqlTableModel *model;
};

#endif // NEWPLACE_H
