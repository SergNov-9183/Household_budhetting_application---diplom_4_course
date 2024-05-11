#include "EditorController.h"

#include <QGuiApplication>
#include <QDebug>
#include <QDir>
#include <QFontDatabase>
#include <DataManager.hpp>
#include <Enums.h>
#include <Utils.h>

namespace {
    static void initializingFonts() {
        auto append = [](const QString& name) {
            auto path = QString(":/Fonts/%1.ttf").arg(name);
            if (QFontDatabase::addApplicationFont(path) < 0) {
                qDebug() << "Failed to add font from resource: " << path.toStdString().c_str();
            }
        };
        append("Roboto-Regular");
        append("Roboto-Bold");
        append("Roboto-Medium");
        append("Manrope-Bold");
        append("Manrope-ExtraBold");
        append("Manrope-ExtraLight");
        append("Manrope-Light");
        append("Manrope-Medium");
        append("Manrope-Regular");
        append("Manrope-SemiBold");
    }
}

EditorController::EditorController(QObject* parent)
    : QObject{parent}
    , m_settings(new HFSettings(QSettings::Format::NativeFormat, QSettings::Scope::UserScope, "Sergey & Company", "Home Finance", this))
    , m_dataManager(DataManager::dataManager()) {
    m_dataManager->setListener(this);
    initializingFonts();
}

std::shared_ptr<Categories> EditorController::categories() const {
    return m_dataManager->categories();
}

std::shared_ptr<Accounts> EditorController::accounts() const {
    return m_dataManager->accounts();
}

std::shared_ptr<Operations> EditorController::operations() const {
    return m_dataManager->operations();
}

void EditorController::load() {
    if (Utils::fileExists(m_settings->projectName().toStdString())) {
        createOrOpen(m_settings->projectName());
    }
    else {
        emit needCreateOrOpenProject();
    }
}

void EditorController::createOrOpen(const QUrl& projectPath) {
    if (!projectPath.isEmpty()) {
        createOrOpen(QDir::toNativeSeparators(projectPath.toLocalFile()));
    }
    else {
        emit needCreateOrOpenProject();
    }
}

void EditorController::createOrOpen(const QString& projectPath) {
    if (!projectPath.isEmpty()) {
        auto result = m_dataManager->openProject(projectPath.toStdString());
        if (result != OpenProjectResult::Error) {
            if (result == OpenProjectResult::Created) {
                m_settings->setProjectName(projectPath);
            }
            emit projectLoaded();
            setHasProject(true);
            return;
        }
    }
    emit needCreateOrOpenProject();
}

void EditorController::renameCategory(const QString &name, int id) {
    m_dataManager->renameCategory(name.toStdString(), id);
}

void EditorController::appendCategory(const QString& name, int level, bool income, int parentId) {
    m_dataManager->appendCategory({ -1, name.toStdString() + " " + std::to_string(categories()->size()), level, income, parentId });
}

void EditorController::renameAccount(const QString &name, int id) {
    m_dataManager->renameAccount(name.toStdString(), id);
}

void EditorController::appendAccount(const QString& name, int type, int parentId) {
    m_dataManager->appendAccount({ -1, name.toStdString() + " " + std::to_string(accounts()->size()), type, parentId });
}

void EditorController::changeOperation(const QString& description, int categoryId, float price, int id) {
    Operation operation{ id, description.toStdString(), categoryId, -1, std::string(), price};
    m_dataManager->changeOperation(operation);
}

void EditorController::appendOperation(int accountId, int categoryId) {
    auto dateTime = QDateTime::currentDateTime().toString().toStdString();
    m_dataManager->appendOperation({ -1, std::string(), categoryId, accountId, dateTime, 0.0 });
}

void EditorController::removeOperation(int id) {
    m_dataManager->removeOperation(id);
}

void EditorController::onCategoryAppend() {
    emit categoryAppended();
}

void EditorController::onCategoryRenamed(int id) {
    emit categoryRenamed(id);
}

void EditorController::onAccountAppend() {
    emit accountAppended();
}

void EditorController::onAccountRenamed(int id) {
    emit accountRenamed(id);
}

void EditorController::onOperationAppend() {
    emit operationAppended();
}

void EditorController::onOperationChanged(int id) {
    emit operationChanged(id);
}

void EditorController::onOperationRemoved(int id, int accountId) {
    emit operationRemoved(id, accountId);
}

HFSettings *EditorController::settings() {
    return m_settings;
}

QString EditorController::userName() const {
    return tr("Сергей");
}

QString EditorController::userMiddleName() const {
    return tr("Иванович");
}

QString EditorController::userSurName() const {
    return tr("Новосельцев");
}

bool EditorController::hasProject() const {
    return m_hasProject;
}

void EditorController::setHasProject(bool value) {
    if (m_hasProject != value) {
        m_hasProject = value;
        emit hasProjectChanged(value);
    }
}
