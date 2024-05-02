#ifndef CATEGORIESPROXYMODEL_H
#define CATEGORIESPROXYMODEL_H

#include <QSortFilterProxyModel>

class CategoriesProxyModel : public QSortFilterProxyModel {
    Q_OBJECT
public:
    enum class CategoryType {
        All,
        Income,
        Expense
    };
    explicit CategoriesProxyModel(CategoryType categoryType, QObject* parent = nullptr);

public slots:
    void shevronClisked(int id);
    void invalidateFilter();

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const override;
    bool lessThan(const QModelIndex& source_left, const QModelIndex& source_right) const override;

private:
    CategoryType m_categoryType;
};

#endif // CATEGORIESPROXYMODEL_H
