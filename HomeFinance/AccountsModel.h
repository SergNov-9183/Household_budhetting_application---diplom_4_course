#ifndef ACCOUNTSMODEL_H
#define ACCOUNTSMODEL_H

#include <set>
#include <QAbstractListModel>
#include "EditorController.h"
#include "AccountsProxyModel.h"
#include "AccountsTabsProxyModel.h"

class AccountsModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(EditorController* editorController READ editorController WRITE setEditorController NOTIFY editorControllerChanged)
    Q_PROPERTY(QSortFilterProxyModel* accounts READ accounts CONSTANT)
    Q_PROPERTY(QSortFilterProxyModel* accountsTabs READ accountsTabs CONSTANT)

public:
    enum AccountsRoles {
        Name = Qt::DisplayRole,
        Id =  Qt::UserRole + 1,
        Level,
        Type,
        ParentId,
        IsExpanded,
        HasChildren,
        Icon
    };

    explicit AccountsModel(QObject* parent = nullptr);
    ~AccountsModel();

    bool isParent(int myId, int targetId, QModelIndex& index) const;
    void findCommonParent(int leftId, int rightId, QModelIndex& leftIndex, QModelIndex& rightIndex);
    bool isVisible(int id) const;
    void shevronClisked(int id);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

signals:
    void editorControllerChanged(EditorController*);
    void startRename(int id);

private slots:
    void onPprojectLoaded();
    void onAppended();
    void onRenamed(int id);

private:
    struct Node {
        bool isExpanded = false;
        std::set<int> children;
    };

    EditorController* editorController() const;
    void setEditorController(EditorController* value);

    QSortFilterProxyModel* accounts() const;
    QSortFilterProxyModel* accountsTabs() const;

    bool isValidIndex(int value) const;
    void disconnectController();
    void connectController();
    void invalidateData();
    int level(int index) const;
    const Account& account(int index) const;

    AccountsProxyModel* m_accounts = nullptr;
    AccountsTabsProxyModel* m_accountsTabs = nullptr;


    QHash<int, QByteArray> m_roles;
    EditorController* m_editorController = nullptr;
    std::vector<Node> m_nodes;
    std::map<int, size_t> m_mapNodes;
};

#endif // ACCOUNTSMODEL_H
