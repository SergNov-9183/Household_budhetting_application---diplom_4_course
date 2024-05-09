#include "CategoriesModel.h"

#include <QTimer>

CategoriesModel::CategoriesModel(QObject *parent)
    : QAbstractListModel{parent}
    , m_allCategories(new CategoriesProxyModel(CategoriesProxyModel::CategoryType::All, this))
    , m_incomeCategories(new CategoriesProxyModel(CategoriesProxyModel::CategoryType::Income, this))
    , m_expenseCategories(new CategoriesProxyModel(CategoriesProxyModel::CategoryType::Expense, this)) {
    m_roles = QAbstractListModel::roleNames();
    m_roles[Name]        = "name";
    m_roles[Id]          = "id";
    m_roles[Level]       = "level";
    m_roles[Income]      = "income";
    m_roles[ParentId]    = "parentId";
    m_roles[IsExpanded]  = "isExpanded";
    m_roles[HasChildren] = "hasChildren";
    m_allCategories->setSourceModel(this);
    m_allCategories->sort(0);
    m_incomeCategories->setSourceModel(this);
    m_incomeCategories->sort(0);
    m_expenseCategories->setSourceModel(this);
    m_expenseCategories->sort(0);
}

CategoriesModel::~CategoriesModel() {
    disconnectController();
}

bool CategoriesModel::isParent(int myId, int targetId, QModelIndex& index) const {
    auto targetLevel = level(m_mapNodes.at(targetId));
    do {
        auto parentId = category(m_mapNodes.at(myId)).parentId;
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

void CategoriesModel::findCommonParent(int leftId, int rightId, QModelIndex& leftIndex, QModelIndex& rightIndex) {
    do {
        auto leftRow  = m_mapNodes.at(leftId);
        auto rightRow = m_mapNodes.at(rightId);
        leftId        = category(leftRow).parentId;
        rightId       = category(rightRow).parentId;
        if (leftId == rightId || leftId < 1 || rightId < 1) {
            leftIndex = createIndex(leftRow, 0);
            rightIndex = createIndex(rightRow, 0);
            break;
        }
    } while (true);
}

bool CategoriesModel::isVisible(int id) const {
    auto row = m_mapNodes.at(id);
    do {
        id = category(row).parentId;
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

void CategoriesModel::shevronClisked(int id) {
    auto row   = m_mapNodes.at(id);
    auto& node = m_nodes.at(row);
    node.isExpanded = !node.isExpanded;
    auto modelIndex = createIndex(row, 0);
    emit dataChanged(modelIndex, modelIndex, { IsExpanded });
}

int CategoriesModel::rowCount(const QModelIndex&) const {
    return m_nodes.size();
}

QVariant CategoriesModel::data(const QModelIndex& index, int role) const {
    auto row = index.row();
    if (!isValidIndex(row)) {
        return QVariant();
    }

    switch (role) {
    case Name:        return QString::fromStdString(category(row).name);
    case Id:          return category(row).id;
    case Level:       return level(row);
    case Income:      return category(row).income;
    case ParentId:    return category(row).parentId;
    case IsExpanded:  return m_nodes[row].isExpanded;
    case HasChildren: return m_nodes[row].children.size() > 0;
    default:          return QVariant();
    }
}

QHash<int, QByteArray> CategoriesModel::roleNames() const {
    return m_roles;
}

void CategoriesModel::onPprojectLoaded() {
    if (m_editorController) {
        beginResetModel();
        m_mapNodes.clear();
        m_nodes.clear();
        auto categories = m_editorController->categories();
        for (auto i = 0; i < categories->size(); ++i) {
            m_nodes.push_back({ level(i) == 0 });
            m_mapNodes[categories->at(i).id] = i;
        }
        for (auto i = 0; i < m_nodes.size(); ++i) {
            if (auto parentId = category(i).parentId; parentId > 0) {
                m_nodes[m_mapNodes[parentId]].children.insert(category(i).id);
            }
        }
        endResetModel();
    }
}

void CategoriesModel::onAppended() {
    auto index      = m_nodes.size();
    auto categories = m_editorController->categories();
    auto& category  = categories->at(index);
    beginInsertRows(QModelIndex(), index, index);
    m_mapNodes[category.id] = index;
    m_nodes.push_back({});
    endInsertRows();
    if (auto parentId = category.parentId; parentId > 0) {
        auto row = m_mapNodes.at(parentId);
        m_nodes.at(row).children.insert(category.id);
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
    QTimer::singleShot(200, this, [this, id = category.id]() { emit startRename(id); });
}

void CategoriesModel::onRenamed(int id) {
    auto row        = m_mapNodes.at(id);
    auto modelIndex = createIndex(row, 0);
    emit dataChanged(modelIndex, modelIndex, { Name });
}

EditorController* CategoriesModel::editorController() const {
    return m_editorController;
}

void CategoriesModel::setEditorController(EditorController* value) {
    if (m_editorController != value) {
        disconnectController();
        m_editorController = value;
        connectController();
        emit editorControllerChanged(value);
    }
}

QSortFilterProxyModel *CategoriesModel::allCategories() const {
    return m_allCategories;
}

QSortFilterProxyModel *CategoriesModel::incomeCategories() const {
    return m_incomeCategories;
}

QSortFilterProxyModel *CategoriesModel::expenseCategories() const {
    return m_expenseCategories;
}

bool CategoriesModel::isValidIndex(int value) const {
    return value >= 0 && value < m_nodes.size();
}

void CategoriesModel::disconnectController() {
    if (m_editorController) {
        disconnect(m_editorController, &EditorController::projectLoaded, this, &CategoriesModel::onPprojectLoaded);
        disconnect(m_editorController, &EditorController::categoryAppended, this, &CategoriesModel::onAppended);
        disconnect(m_editorController, &EditorController::categoryRenamed, this, &CategoriesModel::onRenamed);
    }
    beginResetModel();
    m_mapNodes.clear();
    m_nodes.clear();
    endResetModel();
}

void CategoriesModel::connectController() {
    if (m_editorController) {
        connect(m_editorController, &EditorController::projectLoaded, this, &CategoriesModel::onPprojectLoaded);
        connect(m_editorController, &EditorController::categoryAppended, this, &CategoriesModel::onAppended);
        connect(m_editorController, &EditorController::categoryRenamed, this, &CategoriesModel::onRenamed);
    }
}

void CategoriesModel::invalidateData() {
    m_allCategories->invalidate();
    m_incomeCategories->invalidate();
    m_expenseCategories->invalidate();
}

int CategoriesModel::level(int index) const {
    return m_editorController->categories()->at(index).level;
}

const Category& CategoriesModel::category(int index) const {
    return m_editorController->categories()->at(index);
}
