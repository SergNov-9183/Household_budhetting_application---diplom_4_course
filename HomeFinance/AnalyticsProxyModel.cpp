#include "AnalyticsProxyModel.h"
#include "CategoriesModel.h"

AnalyticsProxyModel::AnalyticsProxyModel(QObject *parent)
    : QSortFilterProxyModel{parent}
{}

bool AnalyticsProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const {
    auto index  = sourceModel()->index(source_row, 0);
    return sourceModel()->data(index, CategoriesModel::CategoriesRoles::ParentId).toInt() == m_currentCategory ||
           sourceModel()->data(index, CategoriesModel::CategoriesRoles::Id).toInt() == m_currentCategory;
}

bool AnalyticsProxyModel::lessThan(const QModelIndex& source_left, const QModelIndex& source_right) const {
    if (m_currentCategory == sourceModel()->data(source_left, CategoriesModel::CategoriesRoles::Id).toInt()) {
        return true;
    }
    if (m_currentCategory == sourceModel()->data(source_right, CategoriesModel::CategoriesRoles::Id).toInt()) {
        return false;
    }
    return QSortFilterProxyModel::lessThan(source_left, source_right);
}

QAbstractListModel *AnalyticsProxyModel::model() {
    return qobject_cast<QAbstractListModel*>(sourceModel());
}

void AnalyticsProxyModel::setModel(QAbstractListModel* value) {
    if (sourceModel() != value) {
        setSourceModel(value);
        sort(0);
        emit modelChanged(value);
    }
}

int AnalyticsProxyModel::currentCategory() const {
    return m_currentCategory;
}

void AnalyticsProxyModel::setCurrentCategory(int value) {
    m_currentCategory = value;
    invalidateFilter();
    emit currentCategoryChanged(value);
}
