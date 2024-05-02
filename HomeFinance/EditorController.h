#ifndef EDITORCONTROLLER_H
#define EDITORCONTROLLER_H

#include <memory>
#include <QObject>
#include <IDataManager.h>

class EditorController : public QObject, private DataManager::IDataManager::Listener {
    Q_OBJECT
public:
    explicit EditorController(QObject* parent = nullptr);

    std::shared_ptr<Categories> categories() const;

public slots:
    void renameCategory(const QString& name, int id);
    void appendCategory(const QString& name, int level, bool income, int parentId);

signals:
    void categoryAppended();
    void categoryRenamed(int id);

private:
    void onCategoryAppend() override;
    void onCategoryRenamed(int id) override;

    std::shared_ptr<DataManager::IDataManager> m_dataManager = nullptr;
};

#endif // EDITORCONTROLLER_H
