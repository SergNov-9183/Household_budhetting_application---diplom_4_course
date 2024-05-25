#ifndef OPERATIONSMODEL_H
#define OPERATIONSMODEL_H

//#include <set>
#include <QAbstractListModel>
#include "EditorController.h"
#include "OperationsProxyModel.h"

class OperationsProxyModel;
class OperationsModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(EditorController* editorController READ editorController WRITE setEditorController NOTIFY editorControllerChanged)
    Q_PROPERTY(QAbstractListModel* categoriesModel READ categoriesModel WRITE setCategoriesModel NOTIFY categoriesModelChanged)

public:
    enum OperationsRoles {
        Description = Qt::DisplayRole,
        Id =  Qt::UserRole + 1,
        CategoryId,
        AccountId,
        Date,
        Price,
        Balance
    };

    explicit OperationsModel(QObject* parent = nullptr);
    ~OperationsModel();

    bool isParent(int myId, int targetId, QModelIndex& index) const;
    void findCommonParent(int leftId, int rightId, QModelIndex& leftIndex, QModelIndex& rightIndex);
    bool isVisible(int id) const;
    void shevronClisked(int id);
    void appendProxy(OperationsProxyModel* proxy, int accountId);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

signals:
    void editorControllerChanged(EditorController*);
    void categoriesModelChanged(QAbstractListModel*);
    void startRename(int id);

private slots:
    void onPprojectLoaded();
    void onAppended();
    void onChanged(int id);
    void onRemoved(int id, int accountId);
    void onCategoryRenamed(int id);
    void onCategoryMoved(int id, int oldParentId);

private:
    struct Node {
        float balance;
    };

    EditorController* editorController() const;
    void setEditorController(EditorController* value);
    QAbstractListModel* categoriesModel() const;
    void setCategoriesModel(QAbstractListModel* value);

    bool isValidIndex(int value) const;
    void disconnectController();
    void connectController();
    void invalidateData(int accountId);
    int level(int index) const;
    const Operation& operation(int index) const;
    bool isIncome(int categoryId) const;
    void updateBalance();
    void updateCategories();

    QHash<int, QByteArray> m_roles;
    EditorController* m_editorController = nullptr;
    QAbstractListModel* m_categoriesModel = nullptr;
    std::map<int, size_t> m_mapNodes;
    std::vector<Node> m_nodes;
    std::map<int, OperationsProxyModel*> m_operations;
};

#endif // OPERATIONSMODEL_H
