#include "EditorController.h"

#include <QGuiApplication>
#include <QDebug>
#include <QDir>
#include <QFontDatabase>
#include <QNetworkReply>
#include <DataManager.hpp>
#include <Enums.h>
#include <Utils.h>
#include <QRegularExpression>

namespace {
    enum class CurrencyType { USD, EUR, CNY};

    static std::map<CurrencyType, QRegularExpression> reMap = {
        { CurrencyType::USD, QRegularExpression("<CharCode>USD<\\/CharCode><Nominal>1<\\/Nominal><Name>\\D*<\\/Name><Value>\\d+[\\.,]\\d+<\\/Value>") },
        { CurrencyType::EUR, QRegularExpression("<CharCode>EUR<\\/CharCode><Nominal>1<\\/Nominal><Name>\\D*<\\/Name><Value>\\d+[\\.,]\\d+<\\/Value>") },
        { CurrencyType::CNY, QRegularExpression("<CharCode>CNY<\\/CharCode><Nominal>1<\\/Nominal><Name>\\D*<\\/Name><Value>\\d+[\\.,]\\d+<\\/Value>") }
    };

    float getCurrency(const QString& xmlData, CurrencyType currencyType) {
        auto match = reMap[currencyType].match(xmlData);
        if (match.hasMatch()) {
            static QRegularExpression reValue = QRegularExpression("\\d+[\\.,]\\d+");
            auto matchValue = reValue.match(match.captured(0));
            if (matchValue.hasMatch()) {
                return matchValue.captured(0).replace(",", ".").toFloat();
            }
        }
        return 1.0;
    }

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
    , m_netManager(std::make_unique<QNetworkAccessManager>(new QNetworkAccessManager(this)))
    , m_settings(new HFSettings(QSettings::Format::NativeFormat, QSettings::Scope::UserScope, "Sergey & Company", "Home Finance", this))
    , m_dataManager(DataManager::dataManager()) {
    m_netManager->clearAccessCache();
    m_netManager->clearConnectionCache();
    connect(m_netManager.get(), &QNetworkAccessManager::finished, this, &EditorController::readAnswerProcessing);
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

void EditorController::appendCategory(const QString& name, bool income, int parentId) {
    m_dataManager->appendCategory({ -1, name.toStdString() + " " + std::to_string(categories()->size()), income, parentId });
}

void EditorController::moveCategory(int parentId, int id) {
    m_dataManager->moveCategory(parentId, id);
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

void EditorController::readAnswerProcessing(QNetworkReply *answer)
{
    if (answer->error() != QNetworkReply::NoError) {
        qDebug() << "An error occurred while executing the request: " << answer->errorString();
    }
    else {
        auto data = QString::fromStdString(answer->readAll().toStdString());
        if (data.isEmpty()) {
            qDebug() << "The answer contains no data";
        }
        else {
            setCurrencyUSD(getCurrency(data, CurrencyType::USD));
            setCurrencyEUR(getCurrency(data, CurrencyType::EUR));
            setCurrencyCNY(getCurrency(data, CurrencyType::CNY));
        }
    }
}



void EditorController::onCategoryAppend() {
    emit categoryAppended();
}

void EditorController::onCategoryRenamed(int id) {
    emit categoryRenamed(id);
}

void EditorController::onCategoryMoved(int id, int oldParentId) {
    emit categoryMoved(id, oldParentId);
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

float EditorController::currencyUSD() const {
    return m_currencyUSD;
}

float EditorController::currencyEUR() const {
    return m_currencyEUR;
}

float EditorController::currencyCNY() const {
    return m_currencyCNY;
}

void EditorController::setCurrencyUSD(float value) {
    if (m_currencyUSD != value) {
        m_currencyUSD = value;
        emit currencyUSDChanged(value);
    }
}

void EditorController::setCurrencyEUR(float value) {
    if (m_currencyEUR != value) {
        m_currencyEUR = value;
        emit currencyEURChanged(value);
    }
}

void EditorController::setCurrencyCNY(float value) {
    if (m_currencyCNY != value) {
        m_currencyCNY = value;
        emit currencyCNYChanged(value);
    }
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

void EditorController::sendRequest() {
    auto url = QUrl(QString("http://www.cbr.ru/scripts/XML_daily.asp"));
    qDebug() << "Reading: " << url.toString();
    m_netManager.get()->get(QNetworkRequest(url));
}


