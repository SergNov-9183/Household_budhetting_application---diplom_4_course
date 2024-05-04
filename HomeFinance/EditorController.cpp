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
    //m_dataManager->openProject("D:/Sergey/Documents/HomeFinance.sqlite3");
    initializingFonts();
}

std::shared_ptr<Categories> EditorController::categories() const {
    return m_dataManager->categories();
}

std::shared_ptr<Accounts> EditorController::accounts() const {
    return m_dataManager->accounts();
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
    m_dataManager->appendCategory({ -1, name.toStdString(), level, income, parentId });
}

void EditorController::renameAccount(const QString &name, int id) {
    m_dataManager->renameAccount(name.toStdString(), id);
}

void EditorController::appendAccount(const QString& name, int type, int parentId) {
    m_dataManager->appendAccount({ -1, name.toStdString(), type, parentId });
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
