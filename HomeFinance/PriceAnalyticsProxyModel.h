#ifndef PRICEANALYTICSPROXYMODEL_H
#define PRICEANALYTICSPROXYMODEL_H

#include <QSortFilterProxyModel>
#include "GlobalDefines.h"

class CategoriesModel;
class PriceAnalyticsProxyModel : public QSortFilterProxyModel, public IInvalidateFilter {
    Q_OBJECT
    Q_PROPERTY(QAbstractListModel* operations READ operationsModel WRITE setOperationsModel NOTIFY operationsChanged)
    Q_PROPERTY(QAbstractListModel* categories READ categoriesModel WRITE setCategoriesModel NOTIFY categoriesChanged)
    Q_PROPERTY(int currentCategory READ currentCategory WRITE setCurrentCategory NOTIFY currentCategoryChanged)

public:
    explicit PriceAnalyticsProxyModel(QObject* parent = nullptr);

    void invalidateData() override;

signals:
    void operationsChanged(QAbstractListModel*);
    void categoriesChanged(QAbstractListModel*);
    void currentCategoryChanged(int);
    void analyzeDataComplited();

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const override;

private slots:
    void onPeriodTypeChanged(int);

private:
    QAbstractListModel* operationsModel();
    void setOperationsModel(QAbstractListModel* value);

    QAbstractListModel* categoriesModel();
    void setCategoriesModel(QAbstractListModel* value);

    int currentCategory() const;
    void setCurrentCategory(int value);

    void registerInModel();
    bool inRange(int id) const;

    CategoriesModel* m_categoriesModel = nullptr;
    int m_currentCategory = 0;
};

#endif // PRICEANALYTICSPROXYMODEL_H
