#include "OperationsProxyModel.h"

#include "OperationsModel.h"

OperationsProxyModel::OperationsProxyModel(QObject *parent)
    : QSortFilterProxyModel{parent} {}


void OperationsProxyModel::invalidateData() {
    invalidateFilter();
}

bool OperationsProxyModel::filterAcceptsRow(int source_row, const QModelIndex& source_parent) const {
    auto index   = sourceModel()->index(source_row, 0);
    auto id      = sourceModel()->data(index, OperationsModel::OperationsRoles::Id).toInt();
    bool inRange = true;
    if (auto model = qobject_cast<OperationsModel*>(sourceModel())) {
        inRange = model->inRange(id);
    }
    return inRange && sourceModel()->data(index, OperationsModel::OperationsRoles::AccountId).toInt() == m_accountId;
}

QAbstractListModel* OperationsProxyModel::model() {
    return qobject_cast<QAbstractListModel*>(sourceModel());
}

void OperationsProxyModel::setModel(QAbstractListModel* value) {
    if (sourceModel() != value) {
        setSourceModel(value);
        emit modelChanged(value);
        registerInModel();
    }
}

int OperationsProxyModel::accountId() const {
    return m_accountId;
}

void OperationsProxyModel::setAccountId(int value) {
    if (m_accountId != value) {
        m_accountId = value;
        emit accountIdChanged(value);
        registerInModel();
    }
}

void OperationsProxyModel::registerInModel() {
    if (auto model = qobject_cast<OperationsModel*>(sourceModel()); model && m_accountId > 0) {
        model->appendProxy(this, m_accountId);
        invalidateFilter();
    }
}
