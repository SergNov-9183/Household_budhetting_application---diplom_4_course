#include "EditorController.h"

#include <QDebug>
#include <DataManager.hpp>
#include <Utils.h>

EditorController::EditorController(QObject *parent) : QObject{parent}, m_dataManager(DataManager::dataManager()) {
    m_dataManager->setListener(this);
    m_dataManager->openProject("D:/Sergey/Documents/HomeFinance.sqlite3");
}

std::shared_ptr<Categories> EditorController::categories() const {
    return m_dataManager->categories();
}

void EditorController::renameCategory(const QString &name, int id) {
    m_dataManager->renameCategory(name.toStdString(), id);
}

void EditorController::appendCategory(const QString& name, int level, bool income, int parentId) {
    m_dataManager->appendCategory({ -1, name.toStdString(), level, income, parentId });
}

void EditorController::onCategoryAppend() {
    emit categoryAppended();
}

void EditorController::onCategoryRenamed(int id) {
    emit categoryRenamed(id);
}
