#include "OperationsModel.h"

#include <QTimer>

#include "OperationsProxyModel.h"

OperationsModel::OperationsModel(QObject *parent)
    : QAbstractListModel{parent} {
    m_roles = QAbstractListModel::roleNames();
    m_roles[Description] = "description";
    m_roles[Id]          = "id";
    m_roles[CategoryId]  = "categoryId";
    m_roles[AccountId]   = "accountId";
    m_roles[Date]        = "date";
    m_roles[Price]       = "price";
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
    case Date:        return QString::fromStdString(operation(row).date);
    case Price:       return operation(row).price;
    default:          return QVariant();
    }
}

QHash<int, QByteArray> OperationsModel::roleNames() const {
    return m_roles;
}

void OperationsModel::onPprojectLoaded() {
    if (m_editorController) {
        beginResetModel();
        m_nodes.clear();
        m_mapNodes.clear();
        auto operations = m_editorController->operations();
        for (auto i = 0; i < operations->size(); ++i) {
            m_nodes.push_back({});
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
}

void OperationsModel::onChanged(int id) {
    auto row        = m_mapNodes.at(id);
    auto modelIndex = createIndex(row, 0);
    emit dataChanged(modelIndex, modelIndex, { Description, CategoryId, Price });
}

void OperationsModel::onRemoved(int id, int accountId) {
    auto row   = m_mapNodes.at(id);
    beginRemoveRows(QModelIndex(), row, row);
    if (m_nodes.begin() != m_nodes.end()) {
        m_nodes.erase(m_nodes.begin());
    }
    m_mapNodes.erase(id);
    endRemoveRows();
    invalidateData(accountId);
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

bool OperationsModel::isValidIndex(int value) const {
    return value >= 0 && value < m_nodes.size();
}

void OperationsModel::disconnectController() {
    if (m_editorController) {
        disconnect(m_editorController, &EditorController::projectLoaded, this, &OperationsModel::onPprojectLoaded);
        disconnect(m_editorController, &EditorController::operationAppended, this, &OperationsModel::onAppended);
        disconnect(m_editorController, &EditorController::operationChanged, this, &OperationsModel::onChanged);
        disconnect(m_editorController, &EditorController::operationRemoved, this, &OperationsModel::onRemoved);
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
    }
}

void OperationsModel::invalidateData(int accountId) {
    for (auto proxy : m_operations) {
        if (proxy.first == accountId) {
            proxy.second->invalidateData();
            break;
        }
    }
}

const Operation& OperationsModel::operation(int index) const {
    return m_editorController->operations()->at(index);
}
