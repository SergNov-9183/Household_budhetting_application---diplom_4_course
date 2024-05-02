#ifndef CATEGORIESMODEL_H
#define CATEGORIESMODEL_H

#include <set>
#include <QAbstractListModel>
#include "EditorController.h"
#include "CategoriesProxyModel.h"

class CategoriesModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(EditorController* editorController READ editorController WRITE setEditorController NOTIFY editorControllerChanged)
    Q_PROPERTY(QSortFilterProxyModel* allCategories READ allCategories CONSTANT)
    Q_PROPERTY(QSortFilterProxyModel* incomeCategories READ incomeCategories CONSTANT)
    Q_PROPERTY(QSortFilterProxyModel* expenseCategories READ expenseCategories CONSTANT)

public:
    enum CategoriesRoles {
        Name = Qt::DisplayRole,
        Id =  Qt::UserRole + 1,
        Level,
        Income,
        ParentId,
        IsExpanded,
        HasChildren
    };

    explicit CategoriesModel(QObject* parent = nullptr);
    ~CategoriesModel();

    bool isParent(int myId, int targetId, QModelIndex& index) const;
    void findCommonParent(int leftParentId, int rightParentId, QModelIndex& leftIndex, QModelIndex& rightIndex);
    bool isVisible(int id) const;
    void shevronClisked(int id);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

signals:
    void editorControllerChanged(EditorController*);

private slots:
    void onAppended();
    void onRenamed(int id);

private:
    struct Node {
        Category& category;
        bool isExpanded = false;
        std::set<int> children;
    };

    EditorController* editorController() const;
    void setEditorController(EditorController* value);

    QSortFilterProxyModel* allCategories() const;
    QSortFilterProxyModel* incomeCategories() const;
    QSortFilterProxyModel* expenseCategories() const;

    bool isValidIndex(int value) const;
    void fillNodes();
    void clearNodes();
    void invalidateFilter();

    CategoriesProxyModel* m_allCategories = nullptr;
    CategoriesProxyModel* m_incomeCategories = nullptr;
    CategoriesProxyModel *m_expenseCategories = nullptr;

    QHash<int, QByteArray> m_roles;
    EditorController* m_editorController = nullptr;
    std::vector<Node> m_nodes;
    std::map<int, size_t> m_mapNodes;
};

#endif // CATEGORIESMODEL_H
