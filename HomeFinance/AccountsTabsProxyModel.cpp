#include "AccountsTabsProxyModel.h"

AccountsTabsProxyModel::AccountsTabsProxyModel(QObject *parent) : QSortFilterProxyModel{parent} {}

#include "AccountsModel.h"

void AccountsTabsProxyModel::invalidateData() {
    invalidateFilter();
}

bool AccountsTabsProxyModel::filterAcceptsRow(int source_row, const QModelIndex& source_parent) const {
    auto index  = sourceModel()->index(source_row, 0);
    return sourceModel()->data(index, AccountsModel::AccountsRoles::Level).toInt() > 0;
}

bool AccountsTabsProxyModel::lessThan(const QModelIndex& source_left, const QModelIndex& source_right) const {
    auto leftType = sourceModel()->data(source_left, AccountsModel::AccountsRoles::Type).toInt();
    auto rightType = sourceModel()->data(source_right, AccountsModel::AccountsRoles::Type).toInt();
    return leftType == rightType ? QSortFilterProxyModel::lessThan(source_left, source_right) : leftType < rightType;
}
