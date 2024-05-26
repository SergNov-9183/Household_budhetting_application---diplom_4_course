#ifndef OPERATIONSMODEL_H
#define OPERATIONSMODEL_H

#include <QAbstractListModel>
#include "EditorController.h"
#include "GlobalDefines.h"

class OperationsModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(EditorController* editorController READ editorController WRITE setEditorController NOTIFY editorControllerChanged)
    Q_PROPERTY(QAbstractListModel* categoriesModel READ categoriesModel WRITE setCategoriesModel NOTIFY categoriesModelChanged)
    Q_PROPERTY(int periodType READ periodType NOTIFY periodTypeChanged)
    Q_PROPERTY(QString beginPeriodDate READ beginPeriodDate NOTIFY beginPeriodDateChanged)
    Q_PROPERTY(QString endPeriodDate READ endPeriodDate NOTIFY endPeriodDateChanged)

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
    void appendProxy(IInvalidateFilter* proxy, int accountId);
    bool inRange(int id) const;
    bool inRange(int id, const QDate& beginPeriodDate, const QDate& endPeriodDate) const;

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

public slots:
    void setPeriodType(int value, const QDate& beginPeriodDate = QDate(), const QDate& endPeriodDate = QDate());

signals:
    void editorControllerChanged(EditorController*);
    void periodTypeChanged(int);
    void beginPeriodDateChanged(const QString&);
    void endPeriodDateChanged(const QString&);
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

    int periodType() const;
    QString beginPeriodDate() const;
    QString endPeriodDate() const;

    bool isValidIndex(int value) const;
    void disconnectController();
    void connectController();
    void invalidateData();
    int level(int index) const;
    const Operation& operation(int index) const;
    bool isIncome(int categoryId) const;
    void updateBalance();
    void updateCategories();
    void setPeriodDates(const QDate& beginPeriodDate = QDate(), const QDate& endPeriodDate = QDate());

    QHash<int, QByteArray> m_roles;
    EditorController* m_editorController = nullptr;
    QAbstractListModel* m_categoriesModel = nullptr;

    PeriodType::Enum m_periodType = PeriodType::Enum::Week;

    std::map<int, size_t> m_mapNodes;
    std::vector<Node> m_nodes;
    std::map<int, IInvalidateFilter*> m_operations;
    QDate m_beginPeriodDate;
    QDate m_endPeriodDate;
};

#endif // OPERATIONSMODEL_H
