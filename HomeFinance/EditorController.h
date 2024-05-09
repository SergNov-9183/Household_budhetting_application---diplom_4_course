#ifndef EDITORCONTROLLER_H
#define EDITORCONTROLLER_H

#include <memory>
#include <QObject>
#include <QUrl>
#include <IDataManager.h>

#include "HFSettings.h"

class EditorController : public QObject, private DataManager::IDataManager::Listener {
    Q_OBJECT
    Q_PROPERTY(HFSettings* settings   READ settings       CONSTANT)
    Q_PROPERTY(QString userName       READ userName       NOTIFY userNameChanged)
    Q_PROPERTY(QString userMiddleName READ userMiddleName NOTIFY userMiddleNameChanged)
    Q_PROPERTY(QString userSurName    READ userSurName    NOTIFY userSurNameChanged)
    Q_PROPERTY(bool hasProject        READ hasProject     NOTIFY hasProjectChanged)
public:
    explicit EditorController(QObject* parent = nullptr);

    std::shared_ptr<Categories> categories() const;
    std::shared_ptr<Accounts> accounts() const;
    std::shared_ptr<Operations> operations() const;

public slots:
    void load();
    void createOrOpen(const QUrl& projectPath);
    void createOrOpen(const QString& projectPath);

    void renameCategory(const QString& name, int id);
    void appendCategory(const QString& name, int level, bool income, int parentId);

    void renameAccount(const QString& name, int id);
    void appendAccount(const QString& name,int type, int parentId);

signals:
    void categoryAppended();
    void categoryRenamed(int id);

    void accountAppended();
    void accountRenamed(int id);

    void operationAppended();
    void operationChanged(int id);
    void operationDeleted(int id);

    void projectLoaded();
    void needCreateOrOpenProject();

    void userNameChanged(const QString&);
    void userMiddleNameChanged(const QString&);
    void userSurNameChanged(const QString&);
    void hasProjectChanged(bool);

private:
    void onCategoryAppend() override;
    void onCategoryRenamed(int id) override;

    void onAccountAppend() override;
    void onAccountRenamed(int id) override;

    void onOperationAppend() override;
    void onOperationChanged(int id) override;
    void onOperationDeleted(int id) override;

    HFSettings* settings();
    QString userName() const;
    QString userMiddleName() const;
    QString userSurName() const;
    bool hasProject() const;
    void setHasProject(bool value);

    HFSettings* m_settings = nullptr;
    std::shared_ptr<DataManager::IDataManager> m_dataManager = nullptr;
    bool m_hasProject = false;
};

#endif // EDITORCONTROLLER_H
