#include "AccountsProxyModel.h"

#include "AccountsModel.h"

AccountsProxyModel::AccountsProxyModel(QObject *parent)
    : QSortFilterProxyModel{parent} {}

void AccountsProxyModel::shevronClisked(int id) {
    if (auto model = qobject_cast<AccountsModel *>(sourceModel())) {
        model->shevronClisked(id);
        invalidateFilter();
    }
}

void AccountsProxyModel::invalidateFilter() {
    QSortFilterProxyModel::invalidateFilter();
}

bool AccountsProxyModel::filterAcceptsRow(int source_row, const QModelIndex& source_parent) const {
    auto index  = sourceModel()->index(source_row, 0, source_parent);
    if (auto model = qobject_cast<AccountsModel*>(sourceModel())) {
        auto id = sourceModel()->data(index, AccountsModel::AccountsRoles::Id).toInt();
        return model->isVisible(id);
    }
    return true;
}

bool AccountsProxyModel::lessThan(const QModelIndex& source_left, const QModelIndex& source_right) const {
    if (auto model = qobject_cast<AccountsModel*>(sourceModel())) {
        auto leftLevel  = sourceModel()->data(source_left, AccountsModel::AccountsRoles::Level).toInt();
        auto rightLevel = sourceModel()->data(source_right, AccountsModel::AccountsRoles::Level).toInt();
        if (leftLevel != rightLevel) {
            QModelIndex index;
            auto leftId  = sourceModel()->data(source_left, AccountsModel::AccountsRoles::Id).toInt();
            auto rightId = sourceModel()->data(source_right, AccountsModel::AccountsRoles::Id).toInt();
            return leftLevel > rightLevel
                       ? model->isParent(leftId, rightId, index) ? false : QSortFilterProxyModel::lessThan(index, source_right)
                       : model->isParent(rightId, leftId, index) ? true : QSortFilterProxyModel::lessThan(source_left, index);
        }
        auto leftParentId  = sourceModel()->data(source_left, AccountsModel::AccountsRoles::ParentId).toInt();
        auto rightParentId = sourceModel()->data(source_right, AccountsModel::AccountsRoles::ParentId).toInt();
        if (leftParentId != rightParentId) {
            QModelIndex leftIndex;
            QModelIndex rightIndex;
            model->findCommonParent(leftParentId, rightParentId, leftIndex, rightIndex);
            return QSortFilterProxyModel::lessThan(leftIndex, rightIndex);
        }
    }
    return QSortFilterProxyModel::lessThan(source_left, source_right);
}
