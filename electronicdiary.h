#ifndef ELECTRONICDIARY_H
#define ELECTRONICDIARY_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QItemSelection>
#include <QMessageBox>
#include <QDir>
#include <QDebug>

#include "newstudytask.h"
#include "newworktask.h"
#include "newplace.h"
#include "newpersonalgoal.h"
#include "editstudytask.h"
#include "editworktask.h"
#include "editplace.h"
#include "editpersonalgoal.h"

namespace Ui {
class ElectronicDiary;
}

class ElectronicDiary : public QMainWindow
{
    Q_OBJECT

public:
    explicit ElectronicDiary(QWidget *parent = 0);
    ~ElectronicDiary();

private slots:
    void on_newRecord_pushButton_clicked();
    void on_deleteRecord_pushButton_clicked();
    void on_tabWidget_currentChanged(int index);
    void slotTableViewSelectionChanged(QItemSelection, QItemSelection);
    void on_lineEdit_textChanged(const QString &arg1);
    void on_allTime_radioButton_clicked();
    void on_week_radioButton_clicked();
    void on_currentDay_radioButton_clicked();
    void on_calendarWidget_selectionChanged();
    void on_edit_pushButton_clicked();
    void on_reportCurrentDay_action_triggered();
    void on_reportWeek_action_triggered();
    void on_reportAllTime_action_triggered();

private:
    Ui::ElectronicDiary *ui;
    enum DatabaseTables {studyTasks, workTasks, personalGoals, places};
    void init();
    void openDatabase();
    void setupTableViews();
    void setTableNames();
    void connectSelectionModels();
    void setRemoveAndEditButtonState(int currentTabIndex);
    void fillTableView(int currentTabIndex);
    void createNewStudyTask();
    void createNewWorkTask();
    void createNewPlace();
    void createNewPersonalGoal();
    void editStudyTask();
    void editWorkTask();
    void editPlace();
    void editPersonalGoal();
    void setFilter();
    void deleteFromPersonalGoals();
    void setShortcuts();
    void writeReport();
    QSqlTableModel *getModelFromCurrentTabIndex(int currentTabIndex);
    const QString databaseName = "database.mdb";
    QString applicationDir;
    QString currentFilter;
    QString searchFilter;
    QSqlDatabase database;
    QSqlTableModel *personalGoalsTableModel;
    QSqlTableModel *placesTableModel;
    QSqlTableModel *studyTasksTableModel;
    QSqlTableModel *workTasksTableModel;
    NewStudyTask *formNewStudyTask;
    NewWorkTask *formNewWorkTask;
    NewPlace *formNewPlace;
    NewPersonalGoal *formNewPersonalGoal;
    EditStudyTask *formEditStudyTask;
    EditWorkTask *formEditWorkTask;
    EditPlace *formEditPlace;
    EditPersonalGoal *formEditPersonalGoal;
    bool selectionChanged = false;
    bool newStudyTaskFormCreated = false;
    bool newWorkTaskFormCreated = false;
    bool newPlaceFormCreated = false;
    bool newPersonalGoalFormCreated = false;
    bool editStudyTaskFormCreated = false;
    bool editWorkTaskFormCreated = false;
    bool editPlaceFormCreated = false;
    bool editPersonalGoalFormCreated = false;
};

#endif // ELECTRONICDIARY_H
