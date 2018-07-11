#include "electronicdiary.h"
#include "ui_electronicdiary.h"

ElectronicDiary::ElectronicDiary(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ElectronicDiary)
{
    ui->setupUi(this);
    this->applicationDir = QDir::currentPath();
    init();
    openDatabase();
    setupTableViews();
    connectSelectionModels();
    setShortcuts();
}

ElectronicDiary::~ElectronicDiary()
{
    delete ui;
}

void ElectronicDiary::setShortcuts()
{
    ui->newRecord_pushButton->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
    ui->edit_pushButton->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R));
    ui->deleteRecord_pushButton->setShortcut(QKeySequence(Qt::Key_Delete));
}

void ElectronicDiary::slotTableViewSelectionChanged(QItemSelection, QItemSelection)
{
    selectionChanged = true;
    int currentTabIndex = ui->tabWidget->currentIndex();
    setRemoveAndEditButtonState(currentTabIndex);
}

void ElectronicDiary::init()
{
    database = QSqlDatabase::addDatabase("QODBC");
    personalGoalsTableModel = new QSqlTableModel();
    placesTableModel = new QSqlTableModel();
    studyTasksTableModel = new QSqlTableModel();
    workTasksTableModel = new QSqlTableModel();
}

void ElectronicDiary::connectSelectionModels()
{
    connect(ui->studyTasks_tableView->selectionModel(),
            SIGNAL(selectionChanged (const QItemSelection&, const QItemSelection&)),
            this, SLOT(slotTableViewSelectionChanged(QItemSelection, QItemSelection)));
    connect(ui->workTasks_tableView->selectionModel(),
            SIGNAL(selectionChanged (const QItemSelection&, const QItemSelection&)),
            this, SLOT(slotTableViewSelectionChanged(QItemSelection, QItemSelection)));
    connect(ui->personalGoals_tableView->selectionModel(),
            SIGNAL(selectionChanged (const QItemSelection&, const QItemSelection&)),
            this, SLOT(slotTableViewSelectionChanged(QItemSelection, QItemSelection)));
    connect(ui->places_tableView->selectionModel(),
            SIGNAL(selectionChanged (const QItemSelection&, const QItemSelection&)),
            this, SLOT(slotTableViewSelectionChanged(QItemSelection, QItemSelection)));
}

void ElectronicDiary::setupTableViews()
{
    personalGoalsTableModel->setTable("PersonalGoals");
    placesTableModel->setTable("Places");
    studyTasksTableModel->setTable("StudyTasks");
    workTasksTableModel->setTable("WorkTasks");

    setTableNames();

    ui->personalGoals_tableView->setModel(personalGoalsTableModel);
    ui->places_tableView->setModel(placesTableModel);
    ui->studyTasks_tableView->setModel(studyTasksTableModel);
    ui->workTasks_tableView->setModel(workTasksTableModel);

    ui->personalGoals_tableView->setColumnHidden(0, true);
    ui->places_tableView->setColumnHidden(0, true);
    ui->studyTasks_tableView->setColumnHidden(0, true);
    ui->workTasks_tableView->setColumnHidden(0, true);
    ui->personalGoals_tableView->setColumnHidden(3, true);

    ui->personalGoals_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->places_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->studyTasks_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->workTasks_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->personalGoals_tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->places_tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->studyTasks_tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->workTasks_tableView->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->personalGoals_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->places_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->studyTasks_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->workTasks_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    fillTableView(personalGoals);
    fillTableView(places);
    fillTableView(studyTasks);
    fillTableView(workTasks);

    ui->personalGoals_tableView->resizeColumnsToContents();
    ui->places_tableView->resizeColumnsToContents();
    ui->studyTasks_tableView->resizeColumnsToContents();
    ui->workTasks_tableView->resizeColumnsToContents();

    ui->personalGoals_tableView->horizontalHeader()->setStretchLastSection(true);
    ui->places_tableView->horizontalHeader()->setStretchLastSection(true);
    ui->studyTasks_tableView->horizontalHeader()->setStretchLastSection(true);
    ui->workTasks_tableView->horizontalHeader()->setStretchLastSection(true);
}

void ElectronicDiary::setTableNames()
{
    studyTasksTableModel->setHeaderData(1, Qt::Horizontal, "Дата");
    studyTasksTableModel->setHeaderData(2, Qt::Horizontal, "Предмет");
    studyTasksTableModel->setHeaderData(3, Qt::Horizontal, "Задание");

    workTasksTableModel->setHeaderData(1, Qt::Horizontal, "Дата");
    workTasksTableModel->setHeaderData(2, Qt::Horizontal, "Задание");

    personalGoalsTableModel->setHeaderData(1, Qt::Horizontal, "Дата");
    personalGoalsTableModel->setHeaderData(2, Qt::Horizontal, "Цель");

    placesTableModel->setHeaderData(1, Qt::Horizontal, "Дата");
    placesTableModel->setHeaderData(2, Qt::Horizontal, "Место");
}

void ElectronicDiary::fillTableView(int currentTabIndex)
{
    switch (currentTabIndex)
    {
    case studyTasks:
        studyTasksTableModel->select();
        break;
    case personalGoals:
        personalGoalsTableModel->select();
        break;
    case places:
        placesTableModel->select();
        break;
    case workTasks:
        workTasksTableModel->select();
        break;
    }
}

void ElectronicDiary::openDatabase()
{
    QString databasePatch = applicationDir + "/" + databaseName;
    database.setDatabaseName("DRIVER={Microsoft Access Driver (*.mdb)};FIL={MS Access};DBQ=" + databasePatch);
    if (!database.open())
    {
        qDebug() << "db not open";
        // TODO restoreDatabase
    }
}

void ElectronicDiary::on_newRecord_pushButton_clicked()
{
    int currentTabIndex = ui->tabWidget->currentIndex();
    switch (currentTabIndex)
    {
    case studyTasks:
        createNewStudyTask();
        break;
    case workTasks:
        createNewWorkTask();
        break;
    case places:
        createNewPlace();
        break;
    case personalGoals:
        createNewPersonalGoal();
        break;
    }
}

void ElectronicDiary::createNewStudyTask()
{
    if (newStudyTaskFormCreated)
    {
        newStudyTaskFormCreated = false;
        delete formNewStudyTask;
    }
    formNewStudyTask = new NewStudyTask(studyTasksTableModel, this);
    formNewStudyTask->show();
    ui->studyTasks_tableView->selectionModel()->clearSelection();
    newStudyTaskFormCreated = true;
}

void ElectronicDiary::createNewWorkTask()
{
    if (newWorkTaskFormCreated)
    {
        newWorkTaskFormCreated = false;
        delete formNewWorkTask;
    }
    formNewWorkTask = new NewWorkTask(workTasksTableModel, this);
    formNewWorkTask->show();
    ui->workTasks_tableView->selectionModel()->clearSelection();
    newWorkTaskFormCreated = true;
}

void ElectronicDiary::createNewPlace()
{
    if (newPlaceFormCreated)
    {
        newPlaceFormCreated = false;
        delete formNewPlace;
    }
    formNewPlace = new NewPlace(placesTableModel, this);
    formNewPlace->show();
    newPlaceFormCreated = true;
}

void ElectronicDiary::createNewPersonalGoal()
{
    if (newPersonalGoalFormCreated)
    {
        newPersonalGoalFormCreated = false;
        delete formNewPersonalGoal;
    }
    formNewPersonalGoal = new NewPersonalGoal(personalGoalsTableModel, this);
    formNewPersonalGoal->show();
    ui->personalGoals_tableView->selectionModel()->clearSelection();
    newPersonalGoalFormCreated = true;
}

void ElectronicDiary::on_deleteRecord_pushButton_clicked()
{
    if (ui->delete_checkBox->isChecked())
    {
        QMessageBox::StandardButton reply = QMessageBox::information(this, "Удаление данных",
                "Вы действительно хотите удалить эту запись?", QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::No)
        {
            return;
        }
    }
    int currentTabIndex = ui->tabWidget->currentIndex();
    int selectedRow;
    switch (currentTabIndex)
    {
    case studyTasks:
        selectedRow = ui->studyTasks_tableView->selectionModel()->selectedRows().at(0).row();
        studyTasksTableModel->removeRow(selectedRow);
        break;
    case personalGoals:
        deleteFromPersonalGoals();
        break;
    case places:
        selectedRow = ui->places_tableView->selectionModel()->selectedRows().at(0).row();
        placesTableModel->removeRow(selectedRow);
        break;
    case workTasks:
        selectedRow = ui->workTasks_tableView->selectionModel()->selectedRows().at(0).row();
        workTasksTableModel->removeRow(selectedRow);
        break;
    }
    fillTableView(currentTabIndex);
    setRemoveAndEditButtonState(currentTabIndex);
}

void ElectronicDiary::deleteFromPersonalGoals()
{
    int selectedRow = ui->personalGoals_tableView->selectionModel()->selectedRows().at(0).row();
    QString photoNames = personalGoalsTableModel->data(personalGoalsTableModel->index(selectedRow, 3)).toString();
    if (!photoNames.isEmpty())
    {
        QStringList fileNames = photoNames.split("?");
        foreach (const QString &str, fileNames)
        {
            QFile(str).remove();
        }
    }
    personalGoalsTableModel->removeRow(selectedRow);
}

void ElectronicDiary::setRemoveAndEditButtonState(int currentTabIndex)
{
    bool disableButtons = false;
    switch (currentTabIndex)
    {
    case studyTasks:
        if (ui->studyTasks_tableView->selectionModel()->selectedIndexes().count() == 0 || !selectionChanged)
        {
            break;
        }
        disableButtons = studyTasksTableModel->rowCount() == 0 ? false : true;
        break;
    case personalGoals:
        if (ui->personalGoals_tableView->selectionModel()->selectedIndexes().count() == 0 || !selectionChanged)
        {
            break;
        }
        disableButtons = personalGoalsTableModel->rowCount() == 0 ? false : true;
        break;
    case places:
        if (ui->places_tableView->selectionModel()->selectedIndexes().count() == 0 || !selectionChanged)
        {
            break;
        }
        disableButtons = placesTableModel->rowCount() == 0 ? false : true;
        break;
    case workTasks:
        if (ui->workTasks_tableView->selectionModel()->selectedIndexes().count() == 0 || !selectionChanged)
        {
            break;
        }
        disableButtons = workTasksTableModel->rowCount() == 0 ? false : true;
        break;
    }
    ui->deleteRecord_pushButton->setEnabled(disableButtons);
    ui->edit_pushButton->setEnabled(disableButtons);
    ui->deleteRecord_action->setEnabled(disableButtons);
    ui->editRecord_action->setEnabled(disableButtons);
}

void ElectronicDiary::on_tabWidget_currentChanged(int index)
{
    selectionChanged = false;
    setRemoveAndEditButtonState(index);
    on_lineEdit_textChanged(ui->lineEdit->text());
}

void ElectronicDiary::on_lineEdit_textChanged(const QString &arg1)
{
    int currentTabIndex = ui->tabWidget->currentIndex();
    if (arg1.isEmpty())
    {
        searchFilter = "";
    }
    else
    {
        switch (currentTabIndex)
        {
        case studyTasks:
            searchFilter = "Date LIKE '" + arg1 + "%' OR Task LIKE '" + arg1 + "%' OR Lesson LIKE '" + arg1 + "%'";
            break;
        case workTasks:
            searchFilter = "Date LIKE '" + arg1 + "%' OR Task LIKE '" + arg1 + "%'";
            break;
        case places:
            searchFilter = "Date LIKE '" + arg1 + "%' OR Place LIKE '" + arg1 + "%'";
            break;
        case personalGoals:
            searchFilter = "Date LIKE '" + arg1 + "%' OR Goal LIKE '" + arg1 + "%'";
            break;
        }
    }
    setFilter();
    fillTableView(currentTabIndex);
}

void ElectronicDiary::on_allTime_radioButton_clicked()
{
    ui->reportAllTime_action->setEnabled(true);
    ui->reportCurrentDay_action->setEnabled(false);
    ui->reportWeek_action->setEnabled(false);
    ui->calendarWidget->setEnabled(false);
    currentFilter = "";
    setFilter();
}

void ElectronicDiary::on_week_radioButton_clicked()
{
    ui->reportAllTime_action->setEnabled(false);
    ui->reportCurrentDay_action->setEnabled(false);
    ui->reportWeek_action->setEnabled(true);
    ui->calendarWidget->setEnabled(false);
    currentFilter = "";
    currentFilter = "Date LIKE '" + QDate::currentDate().toString("dd.MM.yyyy") + "%'";
    for (int i = 0; i < 7; ++i)
    {
        QDate date(QDate::currentDate());
        currentFilter += " OR Date LIKE '" + date.addDays(i + 1).toString("dd.MM.yyyy") + "%'";
    }
    setFilter();
}

void ElectronicDiary::on_currentDay_radioButton_clicked()
{
    ui->reportAllTime_action->setEnabled(false);
    ui->reportCurrentDay_action->setEnabled(true);
    ui->reportWeek_action->setEnabled(false);
    ui->calendarWidget->setEnabled(true);
    setFilter();
}

QSqlTableModel* ElectronicDiary::getModelFromCurrentTabIndex(int currentTabIndex)
{
    switch (currentTabIndex)
    {
    case studyTasks:
        return studyTasksTableModel;
    case workTasks:
       return workTasksTableModel;
    case places:
        return placesTableModel;
    case personalGoals:
        return personalGoalsTableModel;
    }
    return nullptr;
}

void ElectronicDiary::setFilter()
{
    if (ui->calendarWidget->isEnabled())
    {
        on_calendarWidget_selectionChanged();
    }
    else
    {
        int currentTabIndex = ui->tabWidget->currentIndex();
        QSqlTableModel *model = getModelFromCurrentTabIndex(currentTabIndex);

        if (currentFilter.isEmpty())
        {
            model->setFilter(searchFilter);
        }
        else
        {
            if (searchFilter.isEmpty())
            {
                model->setFilter(currentFilter);
            }
            else
            {
                model->setFilter("(" + currentFilter + ") AND (" + searchFilter + ")");
            }
        }
        fillTableView(currentTabIndex);
    }
}

void ElectronicDiary::on_calendarWidget_selectionChanged()
{
    int currentTabIndex = ui->tabWidget->currentIndex();
    QSqlTableModel *model = getModelFromCurrentTabIndex(currentTabIndex);
    QString selectedDate = ui->calendarWidget->selectedDate().toString("dd.MM.yyyy");
    currentFilter = "Date LIKE '" + selectedDate + "%'";
    if (searchFilter.isEmpty())
    {
        model->setFilter(currentFilter);
    }
    else
    {
        model->setFilter(currentFilter + " AND (" + searchFilter + ")");
    }
    fillTableView(currentTabIndex);
}

void ElectronicDiary::on_edit_pushButton_clicked()
{
    int currentTabIndex = ui->tabWidget->currentIndex();
    switch (currentTabIndex)
    {
    case studyTasks:
        editStudyTask();
        break;
    case workTasks:
        editWorkTask();
        break;
    case places:
        editPlace();
        break;
    case personalGoals:
        editPersonalGoal();
        break;
    }
}

void ElectronicDiary::editStudyTask()
{
    if (editStudyTaskFormCreated)
    {
        editStudyTaskFormCreated = false;
        delete formEditStudyTask;
    }
    int row = ui->studyTasks_tableView->selectionModel()->selectedIndexes().at(0).row();
    formEditStudyTask = new EditStudyTask(studyTasksTableModel, row, this);
    formEditStudyTask->show();
    ui->studyTasks_tableView->selectionModel()->clearSelection();
    editStudyTaskFormCreated = true;
}

void ElectronicDiary::editWorkTask()
{
    if (editWorkTaskFormCreated)
    {
        editWorkTaskFormCreated = false;
        delete formEditWorkTask;
    }
    int row = ui->workTasks_tableView->selectionModel()->selectedIndexes().at(0).row();
    formEditWorkTask = new EditWorkTask(workTasksTableModel, row, this);
    formEditWorkTask->show();
    ui->workTasks_tableView->selectionModel()->clearSelection();
    editWorkTaskFormCreated = true;
}

void ElectronicDiary::editPlace()
{
    if (editPlaceFormCreated)
    {
        editPlaceFormCreated = false;
        delete formEditPlace;
    }
    int row = ui->places_tableView->selectionModel()->selectedIndexes().at(0).row();
    formEditPlace = new EditPlace(placesTableModel, row, this);
    formEditPlace->show();
    ui->places_tableView->selectionModel()->clearSelection();
    editPlaceFormCreated = true;
}

void ElectronicDiary::editPersonalGoal()
{
    if (editPersonalGoalFormCreated)
    {
        editPersonalGoalFormCreated = false;
        delete formEditPersonalGoal;
    }
    int row = ui->personalGoals_tableView->selectionModel()->selectedIndexes().at(0).row();
    formEditPersonalGoal = new EditPersonalGoal(personalGoalsTableModel, row, this);
    formEditPersonalGoal->show();
    ui->personalGoals_tableView->selectionModel()->clearSelection();
    editPersonalGoalFormCreated = true;
}

void ElectronicDiary::writeReport()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Сохранение отчета", applicationDir,
            "Текстовые файлы (*.txt);;Все файлы (*)");
    if (!fileName.isEmpty())
    {
        QFile file(fileName);
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream textStream(&file);
        textStream.setCodec("UTF-8");
        QStringList list1;
        QStringList list2;
        QStringList list3;
        textStream << QString("Учебные задания:\n===================\n");
        for (int i = 0; i < studyTasksTableModel->rowCount(); ++i)
        {
            studyTasksTableModel->sort(1, Qt::AscendingOrder);
            list1 << studyTasksTableModel->data(studyTasksTableModel->index(i, 1)).toDateTime().toString("dd.MM.yyyy h:mm");
            list2 << studyTasksTableModel->data(studyTasksTableModel->index(i, 2)).toString();
            list3 << studyTasksTableModel->data(studyTasksTableModel->index(i, 3)).toString();
            textStream << list1.at(i) + "\n\tПредмет: " + list2.at(i) + "\n\tЗадание: " + list3.at(i) + "\n";
        }
        list1.clear();
        list2.clear();
        textStream << QString("\nРабочие задания:\n===================\n");
        for (int i = 0; i < workTasksTableModel->rowCount(); ++i)
        {
            workTasksTableModel->sort(1, Qt::AscendingOrder);
            list1 << workTasksTableModel->data(workTasksTableModel->index(i, 1)).toDateTime().toString("dd.MM.yyyy h:mm");
            list2 << workTasksTableModel->data(workTasksTableModel->index(i, 2)).toString();
            textStream << list1.at(i) + "\n\tЗадание: " + list2.at(i) + "\n";
        }
        list1.clear();
        list2.clear();
        textStream << QString("\nЛичные цели:\n===================\n");
        for (int i = 0; i < personalGoalsTableModel->rowCount(); ++i)
        {
            personalGoalsTableModel->sort(1, Qt::AscendingOrder);
            list1 << personalGoalsTableModel->data(personalGoalsTableModel->index(i, 1)).toDateTime().toString("dd.MM.yyyy h:mm");
            list2 << personalGoalsTableModel->data(personalGoalsTableModel->index(i, 2)).toString();
            textStream << list1.at(i) + "\n\tЦель: " + list2.at(i) + "\n";
        }
        list1.clear();
        list2.clear();
        textStream << QString("\nПосещаемые места:\n===================\n");
        for (int i = 0; i < placesTableModel->rowCount(); ++i)
        {
            placesTableModel->sort(1, Qt::AscendingOrder);
            list1 << placesTableModel->data(placesTableModel->index(i, 1)).toDateTime().toString("dd.MM.yyyy h:mm");
            list2 << placesTableModel->data(placesTableModel->index(i, 2)).toString();
            textStream << list1.at(i) + "\n\tМесто: " + list2.at(i) + "\n";
        }
        file.close();
    }
}

void ElectronicDiary::on_reportCurrentDay_action_triggered()
{
    writeReport();
}

void ElectronicDiary::on_reportWeek_action_triggered()
{
   writeReport();
}

void ElectronicDiary::on_reportAllTime_action_triggered()
{
    writeReport();
}
