#include "AccountsModel.h"

AccountsModel::AccountsModel(QObject *parent)
    : QAbstractListModel{parent}
    , m_accounts(new AccountsProxyModel(this)) {
    m_roles = QAbstractListModel::roleNames();
    m_roles[Name]        = "name";
    m_roles[Id]          = "id";
    m_roles[Level]       = "level";
    m_roles[Type]        = "type";
    m_roles[ParentId]    = "parentId";
    m_roles[IsExpanded]  = "isExpanded";
    m_roles[HasChildren] = "hasChildren";
    m_accounts->setSourceModel(this);
    m_accounts->sort(0);
}

AccountsModel::~AccountsModel() {
    disconnectController();
}

bool AccountsModel::isParent(int myId, int targetId, QModelIndex& index) const {
    auto targetLevel = level(m_nodes.at(m_mapNodes.at(targetId)).account);
    do {
        auto parentId = m_nodes.at(m_mapNodes.at(myId)).account.parentId;
        if (parentId < 1 || parentId == targetId) {
            break;
        }
        auto row   = m_mapNodes.at(parentId);
        auto& node = m_nodes.at(row);
        if (targetLevel == level(node.account)) {
            index = createIndex(row, 0);
            return false;
        }
        myId = parentId;
    } while (true);
    return true;
}

void AccountsModel::findCommonParent(int leftParentId, int rightParentId, QModelIndex& leftIndex, QModelIndex& rightIndex) {
    do {
        auto leftRow   = m_mapNodes.at(leftParentId);
        auto rightRow  = m_mapNodes.at(rightParentId);
        leftParentId   = m_nodes.at(leftRow).account.parentId;
        rightParentId  = m_nodes.at(rightRow).account.parentId;
        if (leftParentId == rightParentId || leftParentId < 1 || rightParentId < 1) {
            leftIndex = createIndex(leftRow, 0);
            rightIndex = createIndex(rightRow, 0);
            break;
        }
    } while (true);
}

bool AccountsModel::isVisible(int id) const {
    auto row = m_mapNodes.at(id);
    do {
        id = m_nodes.at(row).account.parentId;
        if (id < 1) {
            break;
        }
        row = m_mapNodes.at(id);
        if (!m_nodes.at(row).isExpanded) {
            return false;
        }
    } while (true);
    return true;
}

void AccountsModel::shevronClisked(int id) {
    auto row   = m_mapNodes.at(id);
    auto& node = m_nodes.at(row);
    node.isExpanded = !node.isExpanded;
    auto modelIndex = createIndex(row, 0);
    emit dataChanged(modelIndex, modelIndex, { IsExpanded });
}

int AccountsModel::rowCount(const QModelIndex&) const {
    return m_nodes.size();
}

QVariant AccountsModel::data(const QModelIndex& index, int role) const {
    auto row = index.row();
    if (!isValidIndex(row)) {
        return QVariant();
    }

    switch (role) {
    case Name:        return QString::fromStdString(m_nodes[row].account.name);
    case Id:          return m_nodes[row].account.id;
    case Level:       return level(m_nodes[row].account);
    case Type:        return m_nodes[row].account.type;
    case ParentId:    return m_nodes[row].account.parentId;
    case IsExpanded:  return m_nodes[row].isExpanded;
    case HasChildren: return m_nodes[row].children.size() > 0;
    default:          return QVariant();
    }
}

QHash<int, QByteArray> AccountsModel::roleNames() const {
    return m_roles;
}

void AccountsModel::onPprojectLoaded() {
    if (m_editorController) {
        beginResetModel();
        m_mapNodes.clear();
        m_nodes.clear();
        auto accounts = m_editorController->accounts();
        for (auto i = 0; i < accounts->size(); ++i) {
            auto& account = accounts->at(i);
            m_nodes.push_back({ account, level(account) == 0 });
            m_mapNodes[account.id] = i;
        }
        for (auto i = 0; i < m_nodes.size(); ++i) {
            if (auto parentId = m_nodes[i].account.parentId; parentId > 0) {
                m_nodes[m_mapNodes[parentId]].children.insert(m_nodes[i].account.id);
            }
        }
        endResetModel();
    }
}

void AccountsModel::onAppended() {
    auto index             = m_nodes.size();
    auto accounts          = m_editorController->accounts();
    auto& account          = accounts->at(index);
    m_mapNodes[account.id] = index;
    beginInsertRows(QModelIndex(), index, index);
    m_nodes.push_back({ account });
    endInsertRows();
    invalidateFilter();
    if (auto parentId = account.parentId; parentId > 0) {
        auto row   = m_mapNodes.at(parentId);
        m_nodes.at(row).children.insert(account.id);
        auto modelIndex = createIndex(row, 0);
        emit dataChanged(modelIndex, modelIndex, { IsExpanded, HasChildren });
    }
}

void AccountsModel::onRenamed(int id) {
    auto row        = m_mapNodes.at(id);
    auto modelIndex = createIndex(row, 0);
    emit dataChanged(modelIndex, modelIndex, { Name });
}

EditorController* AccountsModel::editorController() const {
    return m_editorController;
}

void AccountsModel::setEditorController(EditorController* value) {
    if (m_editorController != value) {
        disconnectController();
        m_editorController = value;
        connectController();
        emit editorControllerChanged(value);
    }
}

QSortFilterProxyModel *AccountsModel::accounts() const {
    return m_accounts;
}

bool AccountsModel::isValidIndex(int value) const {
    return value >= 0 && value < m_nodes.size();
}

void AccountsModel::disconnectController() {
    if (m_editorController) {
        disconnect(m_editorController, &EditorController::projectLoaded, this, &AccountsModel::onPprojectLoaded);
        disconnect(m_editorController, &EditorController::accountAppended, this, &AccountsModel::onAppended);
        disconnect(m_editorController, &EditorController::accountRenamed, this, &AccountsModel::onRenamed);
    }
    beginResetModel();
    m_mapNodes.clear();
    m_nodes.clear();
    endResetModel();
}

void AccountsModel::connectController() {
    if (m_editorController) {
        connect(m_editorController, &EditorController::projectLoaded, this, &AccountsModel::onPprojectLoaded);
        connect(m_editorController, &EditorController::accountAppended, this, &AccountsModel::onAppended);
        connect(m_editorController, &EditorController::accountRenamed, this, &AccountsModel::onRenamed);
    }
}

void AccountsModel::invalidateFilter() {
    m_accounts->invalidateFilter();
}

int AccountsModel::level(const Account& account) const {
    return account.parentId > 0 ? 1 : 0;
}
