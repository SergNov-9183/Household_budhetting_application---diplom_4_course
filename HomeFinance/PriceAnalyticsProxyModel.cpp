#include "PriceAnalyticsProxyModel.h"
#include "OperationsModel.h"
#include "CategoriesModel.h"

PriceAnalyticsProxyModel::PriceAnalyticsProxyModel(QObject* parent) : QSortFilterProxyModel{parent} {}

void PriceAnalyticsProxyModel::invalidateData() {
    invalidateFilter();
    //emit analyzeDataComplited();
}

bool PriceAnalyticsProxyModel::filterAcceptsRow(int source_row, const QModelIndex& source_parent) const {
    auto index  = sourceModel()->index(source_row, 0);
    return sourceModel()->data(index, OperationsModel::OperationsRoles::CategoryId).toInt() == m_currentCategory &&
           inRange(sourceModel()->data(index, OperationsModel::OperationsRoles::Id).toInt());
}

void PriceAnalyticsProxyModel::onPeriodTypeChanged(int) {
    invalidateData();
}

QAbstractListModel* PriceAnalyticsProxyModel::operationsModel() {
    return qobject_cast<QAbstractListModel*>(sourceModel());
}

void PriceAnalyticsProxyModel::setOperationsModel(QAbstractListModel* value) {
    if (sourceModel() != value) {
        setSourceModel(value);
        emit operationsChanged(value);
    }
}

QAbstractListModel *PriceAnalyticsProxyModel::categoriesModel() {
    return m_categoriesModel;
}

void PriceAnalyticsProxyModel::setCategoriesModel(QAbstractListModel* value) {
    if (auto model = qobject_cast<CategoriesModel*>(value); model != m_categoriesModel) {
        m_categoriesModel = model;
        emit categoriesChanged(value);
        connect(m_categoriesModel, &CategoriesModel::periodTypeChanged, this, &PriceAnalyticsProxyModel::onPeriodTypeChanged);
    }
}

int PriceAnalyticsProxyModel::currentCategory() const {
    return m_currentCategory;
}

void PriceAnalyticsProxyModel::setCurrentCategory(int value) {
    m_currentCategory = value;
    invalidateData();
    emit currentCategoryChanged(value);
}

void PriceAnalyticsProxyModel::registerInModel() {
    if (auto model = qobject_cast<OperationsModel*>(sourceModel())) {
        model->appendProxy(this, 0);
        invalidateData();
    }
}

bool PriceAnalyticsProxyModel::inRange(int id) const {
    if (auto model = qobject_cast<OperationsModel*>(sourceModel()); model && m_categoriesModel) {
        return model->inRange(id, m_categoriesModel->beginDate(), m_categoriesModel->endDate());
    }
    return false;
}
