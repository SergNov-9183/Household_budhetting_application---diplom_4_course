#include "CategoriesProxyModel.h"

#include "CategoriesModel.h"

CategoriesProxyModel::CategoriesProxyModel(CategoryType categoryType, QObject *parent)
    : QSortFilterProxyModel{parent}, m_categoryType(categoryType) {}

void CategoriesProxyModel::shevronClisked(int id) {
    if (auto model = qobject_cast<CategoriesModel *>(sourceModel())) {
        model->shevronClisked(id);
        invalidateData();
    }
}

void CategoriesProxyModel::invalidateData() {
    invalidateFilter();
}

bool CategoriesProxyModel::filterAcceptsRow(int source_row, const QModelIndex& source_parent) const {
    auto index  = sourceModel()->index(source_row, 0);
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
        auto leftParentId  = sourceModel()->data(source_left, CategoriesModel::CategoriesRoles::ParentId).toInt();
        auto rightParentId = sourceModel()->data(source_right, CategoriesModel::CategoriesRoles::ParentId).toInt();
        if (leftParentId != rightParentId) {
            auto leftLevel  = sourceModel()->data(source_left, CategoriesModel::CategoriesRoles::Level).toInt();
            auto rightLevel = sourceModel()->data(source_right, CategoriesModel::CategoriesRoles::Level).toInt();
            auto leftIndex  = source_left;
            auto rightIndex = source_right;
            auto leftId     = sourceModel()->data(source_left, CategoriesModel::CategoriesRoles::Id).toInt();
            auto rightId    = sourceModel()->data(source_right, CategoriesModel::CategoriesRoles::Id).toInt();
            if (leftLevel != rightLevel) {
                if (leftLevel > rightLevel) {
                    if (model->isParent(leftId, rightId, leftIndex)) {
                        return false;
                    }
                }
                else {
                    if (model->isParent(rightId, leftId, rightIndex)) {
                        return true;
                    }
                }
            }
            leftId  = sourceModel()->data(leftIndex, CategoriesModel::CategoriesRoles::Id).toInt();
            rightId = sourceModel()->data(rightIndex, CategoriesModel::CategoriesRoles::Id).toInt();
            model->findCommonParent(leftId, rightId, leftIndex, rightIndex);
            return QSortFilterProxyModel::lessThan(leftIndex, rightIndex);
        }
    }
    return QSortFilterProxyModel::lessThan(source_left, source_right);
}
