#ifndef EDITPLACE_H
#define EDITPLACE_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QMessageBox>
#include <QKeyEvent>

namespace Ui {
class EditPlace;
}

class EditPlace : public QMainWindow
{
    Q_OBJECT

public:
    explicit EditPlace(QSqlTableModel *model, int row, QWidget *parent = 0);
    ~EditPlace();

private slots:
    void on_save_pushButton_clicked();

protected:
    virtual void keyPressEvent(QKeyEvent * event);

private:
    Ui::EditPlace *ui;
    QSqlTableModel *model;
    int row;
};

#endif // EDITPLACE_H
