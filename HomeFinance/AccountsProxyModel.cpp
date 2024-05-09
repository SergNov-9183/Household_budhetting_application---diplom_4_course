#include "AccountsProxyModel.h"

#include "AccountsModel.h"

AccountsProxyModel::AccountsProxyModel(QObject *parent)
    : QSortFilterProxyModel{parent} {}

void AccountsProxyModel::invalidateData() {
    invalidateFilter();
}

void AccountsProxyModel::shevronClisked(int id) {
    if (auto model = qobject_cast<AccountsModel *>(sourceModel())) {
        model->shevronClisked(id);
        invalidateData();
    }
}

bool AccountsProxyModel::filterAcceptsRow(int source_row, const QModelIndex& source_parent) const {
    auto index  = sourceModel()->index(source_row, 0);
    if (auto model = qobject_cast<AccountsModel*>(sourceModel())) {
        auto id = sourceModel()->data(index, AccountsModel::AccountsRoles::Id).toInt();
        return model->isVisible(id);
    }
    return true;
}

bool AccountsProxyModel::lessThan(const QModelIndex& source_left, const QModelIndex& source_right) const {
    if (auto model = qobject_cast<AccountsModel*>(sourceModel())) {
        auto leftParentId  = sourceModel()->data(source_left, AccountsModel::AccountsRoles::ParentId).toInt();
        auto rightParentId = sourceModel()->data(source_right, AccountsModel::AccountsRoles::ParentId).toInt();
        if (leftParentId != rightParentId) {
            auto leftLevel  = sourceModel()->data(source_left, AccountsModel::AccountsRoles::Level).toInt();
            auto rightLevel = sourceModel()->data(source_right, AccountsModel::AccountsRoles::Level).toInt();
            auto leftIndex  = source_left;
            auto rightIndex = source_right;
            auto leftId     = sourceModel()->data(source_left, AccountsModel::AccountsRoles::Id).toInt();
            auto rightId    = sourceModel()->data(source_right, AccountsModel::AccountsRoles::Id).toInt();
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
            leftId  = sourceModel()->data(leftIndex, AccountsModel::AccountsRoles::Id).toInt();
            rightId = sourceModel()->data(rightIndex, AccountsModel::AccountsRoles::Id).toInt();
            model->findCommonParent(leftId, rightId, leftIndex, rightIndex);
            return QSortFilterProxyModel::lessThan(leftIndex, rightIndex);
        }
    }
    return QSortFilterProxyModel::lessThan(source_left, source_right);
}
