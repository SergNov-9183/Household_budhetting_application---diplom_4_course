#include "CategoriesProxyModel.h"

#include "CategoriesModel.h"

CategoriesProxyModel::CategoriesProxyModel(CategoryType categoryType, QObject *parent)
    : QSortFilterProxyModel{parent}, m_categoryType(categoryType) {}

void CategoriesProxyModel::shevronClisked(int id) {
    if (auto model = qobject_cast<CategoriesModel *>(sourceModel())) {
        model->shevronClisked(id);
        invalidateFilter();
    }
}

void CategoriesProxyModel::invalidateFilter() {
    QSortFilterProxyModel::invalidateFilter();
}

bool CategoriesProxyModel::filterAcceptsRow(int source_row, const QModelIndex& source_parent) const {
    auto index  = sourceModel()->index(source_row, 0, source_parent);
    auto income = sourceModel()->data(index, CategoriesModel::CategoriesRoles::Income).toBool();
    if ((m_categoryType == CategoryType::Income && !income) ||
        (m_categoryType == CategoryType::Expense && income)) {
        return false;
    }
    if (auto model = qobject_cast<CategoriesModel *>(sourceModel())) {
        auto id = sourceModel()->data(index, CategoriesModel::CategoriesRoles::Id).toInt();
        return model->isVisible(id);
    }
    return true;
}

bool CategoriesProxyModel::lessThan(const QModelIndex& source_left, const QModelIndex& source_right) const {
    if (auto model = qobject_cast<CategoriesModel*>(sourceModel())) {
        auto leftLevel  = sourceModel()->data(source_left, CategoriesModel::CategoriesRoles::Level).toInt();
        auto rightLevel = sourceModel()->data(source_right, CategoriesModel::CategoriesRoles::Level).toInt();
        if (leftLevel != rightLevel) {
            QModelIndex index;
            auto leftId  = sourceModel()->data(source_left, CategoriesModel::CategoriesRoles::Id).toInt();
            auto rightId = sourceModel()->data(source_right, CategoriesModel::CategoriesRoles::Id).toInt();
            return leftLevel > rightLevel
                       ? model->isParent(leftId, rightId, index) ? false : QSortFilterProxyModel::lessThan(index, source_right)
                       : model->isParent(rightId, leftId, index) ? true : QSortFilterProxyModel::lessThan(source_left, index);
        }
        auto leftParentId  = sourceModel()->data(source_left, CategoriesModel::CategoriesRoles::ParentId).toInt();
        auto rightParentId = sourceModel()->data(source_right, CategoriesModel::CategoriesRoles::ParentId).toInt();
        if (leftParentId != rightParentId) {
            QModelIndex leftIndex;
            QModelIndex rightIndex;
            model->findCommonParent(leftParentId, rightParentId, leftIndex, rightIndex);
            return QSortFilterProxyModel::lessThan(leftIndex, rightIndex);
        }
    }
    return QSortFilterProxyModel::lessThan(source_left, source_right);
}
