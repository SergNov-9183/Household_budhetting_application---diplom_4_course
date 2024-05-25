#ifndef EDITORCONTROLLER_H
#define EDITORCONTROLLER_H

#include <memory>
#include <QObject>
#include <QNetworkAccessManager>
#include <IDataManager.h>

#include "HFSettings.h"

class EditorController : public QObject, private DataManager::IDataManager::Listener {
    Q_OBJECT
    Q_PROPERTY(HFSettings* settings   READ settings       CONSTANT)
    Q_PROPERTY(QString userName       READ userName       NOTIFY userNameChanged)
    Q_PROPERTY(QString userMiddleName READ userMiddleName NOTIFY userMiddleNameChanged)
    Q_PROPERTY(QString userSurName    READ userSurName    NOTIFY userSurNameChanged)
    Q_PROPERTY(bool hasProject        READ hasProject     NOTIFY hasProjectChanged)
    Q_PROPERTY(float currencyUSD      READ currencyUSD    NOTIFY currencyUSDChanged)
    Q_PROPERTY(float currencyEUR      READ currencyEUR    NOTIFY currencyEURChanged)
    Q_PROPERTY(float currencyCNY      READ currencyCNY    NOTIFY currencyCNYChanged)
public:
    explicit EditorController(QObject* parent = nullptr);
    void sendRequest();

    std::shared_ptr<Categories> categories() const;
    std::shared_ptr<Accounts> accounts() const;
    std::shared_ptr<Operations> operations() const;

public slots:
    void load();
    void createOrOpen(const QUrl& projectPath);
    void createOrOpen(const QString& projectPath);



    void renameCategory(const QString& name, int id);
    void appendCategory(const QString& name, bool income, int parentId);
    void moveCategory(int parentId, int id);

    void renameAccount(const QString& name, int id);
    void appendAccount(const QString& name,int type, int parentId);

    void changeOperation(const QString& description, int categoryId, float price, int id);
    void appendOperation(int accountId, int categoryId);
    void removeOperation(int id);

signals:
    void categoryAppended();
    void categoryRenamed(int id);
    void categoryMoved(int id, int oldParentId);

    void accountAppended();
    void accountRenamed(int id);

    void operationAppended();
    void operationChanged(int id);
    void operationRemoved(int id, int accountId);

    void projectLoaded();
    void needCreateOrOpenProject();

    void userNameChanged(const QString&);
    void userMiddleNameChanged(const QString&);
    void userSurNameChanged(const QString&);
    void hasProjectChanged(bool);
    void currencyUSDChanged(float);
    void currencyEURChanged(float);
    void currencyCNYChanged(float);

private slots:
    void readAnswerProcessing(QNetworkReply* answer);

private:
    void onCategoryAppend() override;
    void onCategoryRenamed(int id) override;
    void onCategoryMoved(int id, int oldParentId) override;

    void onAccountAppend() override;
    void onAccountRenamed(int id) override;

    void onOperationAppend() override;
    void onOperationChanged(int id) override;
    void onOperationRemoved(int id, int accountId) override;

    float currencyUSD() const;
    void setCurrencyUSD(float value);
    float currencyEUR() const;
    void setCurrencyEUR(float value);
    float currencyCNY() const;
    void setCurrencyCNY(float value);


    HFSettings* settings();
    QString userName() const;
    QString userMiddleName() const;
    QString userSurName() const;
    bool hasProject() const;
    void setHasProject(bool value);

    std::unique_ptr<QNetworkAccessManager> m_netManager;
    HFSettings* m_settings = nullptr;
    std::shared_ptr<DataManager::IDataManager> m_dataManager = nullptr;
    bool m_hasProject = false;
    float m_currencyUSD = 0;
    float m_currencyEUR = 0;
    float m_currencyCNY = 0;
};

#endif // EDITORCONTROLLER_H
