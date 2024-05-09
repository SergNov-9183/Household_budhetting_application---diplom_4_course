#include "AccountsModel.h"

#include <QTimer>

namespace {
    QString getIcon(int accountType) {
        switch (accountType) {
            case 0:  return QString("Cash");
            case 1:  return QString("BankAccount");
            case 2:  return QString("DebitCard");
            default: return QString("");
        }
    }
}

AccountsModel::AccountsModel(QObject *parent)
    : QAbstractListModel{parent}
    , m_accounts(new AccountsProxyModel(this))
    , m_accountsTabs(new AccountsTabsProxyModel(this)) {
    m_roles = QAbstractListModel::roleNames();
    m_roles[Name]        = "name";
    m_roles[Id]          = "id";
    m_roles[Level]       = "level";
    m_roles[Type]        = "type";
    m_roles[ParentId]    = "parentId";
    m_roles[IsExpanded]  = "isExpanded";
    m_roles[HasChildren] = "hasChildren";
    m_roles[Icon]        = "icon";
    m_accounts->setSourceModel(this);
    m_accounts->sort(0);
    m_accountsTabs->setSourceModel(this);
    m_accountsTabs->sort(0);
}

AccountsModel::~AccountsModel() {
    disconnectController();
}

bool AccountsModel::isParent(int myId, int targetId, QModelIndex& index) const {
    auto targetLevel = level(m_mapNodes.at(targetId));
    do {
        auto parentId = account(m_mapNodes.at(myId)).parentId;
        if (parentId < 1) {
            break;
        }
        if (parentId == targetId) {
            return true;
        }
        auto row   = m_mapNodes.at(parentId);
        if (targetLevel == level(row)) {
            index = createIndex(row, 0);
            break;
        }
        myId = parentId;
    } while (true);
    return false;
}

void AccountsModel::findCommonParent(int leftId, int rightId, QModelIndex& leftIndex, QModelIndex& rightIndex) {
    do {
        auto leftRow  = m_mapNodes.at(leftId);
        auto rightRow = m_mapNodes.at(rightId);
        leftId        = account(leftRow).parentId;
        rightId       = account(rightRow).parentId;
        if (leftId == rightId || leftId < 1 || rightId < 1) {
            leftIndex = createIndex(leftRow, 0);
            rightIndex = createIndex(rightRow, 0);
            break;
        }
    } while (true);
}

bool AccountsModel::isVisible(int id) const {
    auto row = m_mapNodes.at(id);
    do {
        id = account(row).parentId;
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
    case Name:        return QString::fromStdString(account(row).name);
    case Id:          return account(row).id;
    case Level:       return level(row);
    case Type:        return account(row).type;
    case ParentId:    return account(row).parentId;
    case IsExpanded:  return m_nodes[row].isExpanded;
    case HasChildren: return m_nodes[row].children.size() > 0;
    case Icon:        return getIcon(account(row).type);
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
            m_nodes.push_back({ level(i) == 0 });
            m_mapNodes[accounts->at(i).id] = i;
        }
        for (auto i = 0; i < m_nodes.size(); ++i) {
            if (auto parentId = account(i).parentId; parentId > 0) {
                m_nodes[m_mapNodes[parentId]].children.insert(account(i).id);
            }
        }
        endResetModel();
    }
}

void AccountsModel::onAppended() {
    auto index    = m_nodes.size();
    auto accounts = m_editorController->accounts();
    auto& account = accounts->at(index);
    beginInsertRows(QModelIndex(), index, index);
    m_mapNodes[account.id] = index;
    m_nodes.push_back({});
    endInsertRows();
    if (auto parentId = account.parentId; parentId > 0) {
        auto row = m_mapNodes.at(parentId);
        m_nodes.at(row).children.insert(account.id);
        QList<int> roles;
        if (m_nodes.at(row).children.size() == 1) {
            m_nodes.at(row).isExpanded = true;
            roles.append({ IsExpanded, HasChildren });
        }
        else if (!m_nodes.at(row).isExpanded) {
            m_nodes.at(row).isExpanded = true;
            roles.append(IsExpanded);
        }
        if (roles.size() > 0) {
            auto modelIndex = createIndex(row, 0);
            emit dataChanged(modelIndex, modelIndex, roles);
        }
    }
    invalidateData();
    QTimer::singleShot(200, this, [this, id = account.id]() { emit startRename(id); });
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

QSortFilterProxyModel* AccountsModel::accounts() const {
    return m_accounts;
}

QSortFilterProxyModel* AccountsModel::accountsTabs() const {
    return m_accountsTabs;
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

void AccountsModel::invalidateData() {
    m_accounts->invalidateData();
    m_accountsTabs->invalidateData();
}

int AccountsModel::level(int index) const {
    return m_editorController->accounts()->at(index).parentId > 0 ? 1 : 0;
}

const Account& AccountsModel::account(int index) const {
    return m_editorController->accounts()->at(index);
}
