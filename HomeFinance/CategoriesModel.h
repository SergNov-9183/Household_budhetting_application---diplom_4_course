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
        HasChildren,
        Sum,
        TotalSum
    };

    explicit CategoriesModel(QObject* parent = nullptr);
    ~CategoriesModel();

    bool isParent(int myId, int targetId, QModelIndex& index) const;
    void findCommonParent(int leftId, int rightId, QModelIndex& leftIndex, QModelIndex& rightIndex);
    bool isVisible(int id) const;
    void shevronClisked(int id);
    bool isIncome(int categoryId) const;

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE int defaultCategory() const;
    Q_INVOKABLE QString getFullCategoryName(int id) const;
    Q_INVOKABLE bool canDrop(int dragAreaId, int dropAreaId) const;

public slots:
    void analyzeData();

signals:
    void editorControllerChanged(EditorController*);
    void startRename(int id);
    void analyzeDataComplited();

private slots:
    void onProjectLoaded();
    void onAppended();
    void onRenamed(int id);
    void onMoved(int id, int oldParentId);

private:
    struct Node {
        bool isExpanded = false;
        std::set<int> children;
        float sum = 0;
        float totalSum = 0;
    };

    EditorController* editorController() const;
    void setEditorController(EditorController* value);

    QSortFilterProxyModel* allCategories() const;
    QSortFilterProxyModel* incomeCategories() const;
    QSortFilterProxyModel* expenseCategories() const;

    bool isValidIndex(int value) const;
    void disconnectController();
    void connectController();
    void invalidateData();
    int level(int row) const;
    const Category& category(int row) const;
    float calculateTotalSums(int id);

    CategoriesProxyModel* m_allCategories = nullptr;
    CategoriesProxyModel* m_incomeCategories = nullptr;
    CategoriesProxyModel *m_expenseCategories = nullptr;

    QHash<int, QByteArray> m_roles;
    EditorController* m_editorController = nullptr;
    std::vector<Node> m_nodes;
    std::map<int, size_t> m_mapNodes;
    int m_defaultCategory = 0;
    int m_incomeCategory = 0;
};

#endif // CATEGORIESMODEL_H
