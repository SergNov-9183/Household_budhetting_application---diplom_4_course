#include "OperationsModel.h"

#include <QDateTime>
#include <QTimer>

#include "OperationsProxyModel.h"
#include "CategoriesModel.h"

OperationsModel::OperationsModel(QObject *parent)
    : QAbstractListModel{parent} {
    m_roles = QAbstractListModel::roleNames();
    m_roles[Description] = "description";
    m_roles[Id]          = "id";
    m_roles[CategoryId]  = "categoryId";
    m_roles[AccountId]   = "accountId";
    m_roles[Date]        = "date";
    m_roles[Price]       = "price";
    m_roles[Balance]     = "balance";
}

OperationsModel::~OperationsModel() {
    disconnectController();
}

void OperationsModel::appendProxy(OperationsProxyModel *proxy, int accountId) {
    m_operations[accountId] = proxy;
}

int OperationsModel::rowCount(const QModelIndex& parent) const {
    return m_nodes.size();
}

QVariant OperationsModel::data(const QModelIndex& index, int role) const {
    auto row = index.row();
    if (!isValidIndex(row)) {
        return QVariant();
    }

    switch (role) {
    case Description: return QString::fromStdString(operation(row).description);
    case Id:          return operation(row).id;
    case CategoryId:  return operation(row).categoryId;
    case AccountId:   return operation(row).accountId;
    case Date:        return QDateTime::fromString(QString::fromStdString(operation(row).date)).toString("dd.MM.yyyy");
    case Price:       return operation(row).price;
    case Balance:     return m_nodes[row].balance;
    default:          return QVariant();
    }
}

QHash<int, QByteArray> OperationsModel::roleNames() const {
    return m_roles;
}

void OperationsModel::onPprojectLoaded() {
    if (m_editorController) {
        std::map<int, float> sum;
        beginResetModel();
        m_nodes.clear();
        m_mapNodes.clear();
        auto operations = m_editorController->operations();
        for (auto i = 0; i < operations->size(); ++i) {
            auto accountId = operations->at(i).accountId;
            auto price = operations->at(i).price;
            sum[accountId] = sum[accountId] + (isIncome(operations->at(i).categoryId) ? price : -price);
            m_nodes.push_back({sum[accountId]});
            m_mapNodes[operations->at(i).id] = i;
        }
        endResetModel();
    }
}

void OperationsModel::onAppended() {
    auto index      = m_nodes.size();
    auto operations = m_editorController->operations();
    auto& operation = operations->at(index);
    beginInsertRows(QModelIndex(), index, index);
    m_mapNodes[operation.id] = index;
    m_nodes.push_back({});
    endInsertRows();
    invalidateData(operation.accountId);
    updateBalance();
}

void OperationsModel::onChanged(int id) {
    auto row        = m_mapNodes.at(id);
    auto modelIndex = createIndex(row, 0);
    emit dataChanged(modelIndex, modelIndex, { Description, CategoryId, Price });
    updateBalance();
}

void OperationsModel::onRemoved(int id, int accountId) {
    auto row   = m_mapNodes.at(id);
    beginRemoveRows(QModelIndex(), row, row);
    if (m_nodes.begin() != m_nodes.end()) {
        m_nodes.erase(m_nodes.begin());
    }
    m_mapNodes.erase(id);
    for (auto& item : m_mapNodes) {
        if (item.second > row) {
            item.second = item.second - 1;
        }
    }
    endRemoveRows();
    invalidateData(accountId);
    updateBalance();
}

void OperationsModel::onCategoryRenamed(int id) {
    updateCategories();
}

void OperationsModel::onCategoryMoved(int id, int oldParentId) {
    updateCategories();
}

EditorController* OperationsModel::editorController() const {
    return m_editorController;
}

void OperationsModel::setEditorController(EditorController* value) {
    if (m_editorController != value) {
        disconnectController();
        m_editorController = value;
        connectController();
        emit editorControllerChanged(value);
    }
}

QAbstractListModel *OperationsModel::categoriesModel() const {
    return m_categoriesModel;
}

void OperationsModel::setCategoriesModel(QAbstractListModel* value) {
    if (m_categoriesModel != value) {
        m_categoriesModel = value;
        emit categoriesModelChanged(value);
    }
}

bool OperationsModel::isValidIndex(int value) const {
    return value >= 0 && value < m_nodes.size();
}

void OperationsModel::disconnectController() {
    if (m_editorController) {
        disconnect(m_editorController, &EditorController::projectLoaded, this, &OperationsModel::onPprojectLoaded);
        disconnect(m_editorController, &EditorController::operationAppended, this, &OperationsModel::onAppended);
        disconnect(m_editorController, &EditorController::operationChanged, this, &OperationsModel::onChanged);
        disconnect(m_editorController, &EditorController::operationRemoved, this, &OperationsModel::onRemoved);
        disconnect(m_editorController, &EditorController::categoryRenamed, this, &OperationsModel::onCategoryRenamed);
        disconnect(m_editorController, &EditorController::categoryMoved, this, &OperationsModel::onCategoryMoved);
    }
    beginResetModel();
    m_mapNodes.clear();
    m_nodes.clear();
    endResetModel();
}

void OperationsModel::connectController() {
    if (m_editorController) {
        connect(m_editorController, &EditorController::projectLoaded, this, &OperationsModel::onPprojectLoaded);
        connect(m_editorController, &EditorController::operationAppended, this, &OperationsModel::onAppended);
        connect(m_editorController, &EditorController::operationChanged, this, &OperationsModel::onChanged);
        connect(m_editorController, &EditorController::operationRemoved, this, &OperationsModel::onRemoved);
        connect(m_editorController, &EditorController::categoryRenamed, this, &OperationsModel::onCategoryRenamed);
        connect(m_editorController, &EditorController::categoryMoved, this, &OperationsModel::onCategoryMoved);
    }
}

void OperationsModel::invalidateData(int accountId) {
    for (auto proxy : m_operations) {
        proxy.second->invalidateData();
    }
}

const Operation& OperationsModel::operation(int index) const {
    return m_editorController->operations()->at(index);
}

bool OperationsModel::isIncome(int categoryId) const {
    if (auto model = qobject_cast<CategoriesModel*>(m_categoriesModel)) {
        return model->isIncome(categoryId);
    }
    return false;
}

void OperationsModel::updateBalance() {
    if (m_editorController) {
        std::map<int, float> sum;
        auto operations = m_editorController->operations();
        for (auto i = 0; i < operations->size(); ++i) {
            auto accountId = operations->at(i).accountId;
            auto price     = operations->at(i).price;
            auto row       = m_mapNodes[operations->at(i).id];
            sum[accountId] = sum[accountId] + (isIncome(operations->at(i).categoryId) ? price : -price);
            m_nodes.at(row).balance = sum[accountId];
        }
        auto beginModelIndex = createIndex(0, 0);
        auto endModelIndex = createIndex(m_nodes.size() - 1, 0);
        emit dataChanged(beginModelIndex, endModelIndex, { Balance });
    }
}

void OperationsModel::updateCategories() {
    auto beginModelIndex = createIndex(0, 0);
    auto endModelIndex = createIndex(m_nodes.size() - 1, 0);
    emit dataChanged(beginModelIndex, endModelIndex, { CategoryId });
}
