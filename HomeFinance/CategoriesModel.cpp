#include "CategoriesModel.h"

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
    clearNodes();
}

bool CategoriesModel::isParent(int myId, int targetId, QModelIndex& index) const {
    auto targetLevel = m_nodes.at(m_mapNodes.at(targetId)).category.level;
    do {
        auto parentId = m_nodes.at(m_mapNodes.at(myId)).category.parentId;
        if (parentId < 1 || parentId == targetId) {
            break;
        }
        auto row   = m_mapNodes.at(parentId);
        auto& node = m_nodes.at(row);
        if (targetLevel == node.category.level) {
            index = createIndex(row, 0);
            return false;
        }
        myId = parentId;
    } while (true);
    return true;
}

void CategoriesModel::findCommonParent(int leftParentId, int rightParentId, QModelIndex& leftIndex, QModelIndex& rightIndex) {
    do {
        auto leftRow   = m_mapNodes.at(leftParentId);
        auto rightRow  = m_mapNodes.at(rightParentId);
        leftParentId   = m_nodes.at(leftRow).category.parentId;
        rightParentId  = m_nodes.at(rightRow).category.parentId;
        if (leftParentId == rightParentId || leftParentId < 1 || rightParentId < 1) {
            leftIndex = createIndex(leftRow, 0);
            rightIndex = createIndex(rightRow, 0);
            break;
        }
    } while (true);
}

bool CategoriesModel::isVisible(int id) const {
    auto row = m_mapNodes.at(id);
    do {
        id = m_nodes.at(row).category.parentId;
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
    case Name:        return QString::fromStdString(m_nodes[row].category.name);
    case Id:          return m_nodes[row].category.id;
    case Level:       return m_nodes[row].category.level;
    case Income:      return m_nodes[row].category.income;
    case ParentId:    return m_nodes[row].category.parentId;
    case IsExpanded:  return m_nodes[row].isExpanded;
    case HasChildren: return m_nodes[row].children.size() > 0;
    default:          return QVariant();
    }
}

QHash<int, QByteArray> CategoriesModel::roleNames() const {
    return m_roles;
}

void CategoriesModel::onAppended() {
    auto index              = m_nodes.size();
    auto categories         = m_editorController->categories();
    auto& category          = categories->at(index);
    m_mapNodes[category.id] = index;
    beginInsertRows(QModelIndex(), index, index);
    m_nodes.push_back({ category });
    endInsertRows();
    invalidateFilter();
    if (auto parentId = category.parentId; parentId > 0) {
        auto row   = m_mapNodes.at(parentId);
        m_nodes.at(row).children.insert(category.id);
        auto modelIndex = createIndex(row, 0);
        emit dataChanged(modelIndex, modelIndex, { IsExpanded, HasChildren });
    }
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
        clearNodes();
        m_editorController = value;
        fillNodes();
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

void CategoriesModel::fillNodes() {
    if (m_editorController) {
        beginResetModel();
        auto categories = m_editorController->categories();
        for (auto i = 0; i < categories->size(); ++i) {
            auto& category = categories->at(i);
            m_nodes.push_back({ category, category.level == 0 });
            m_mapNodes[category.id] = i;
        }
        for (auto i = 0; i < m_nodes.size(); ++i) {
            if (auto parentId = m_nodes[i].category.parentId; parentId > 0) {
                m_nodes[m_mapNodes[parentId]].children.insert(m_nodes[i].category.id);
            }
        }
        endResetModel();
        connect(m_editorController, &EditorController::categoryAppended, this, &CategoriesModel::onAppended);
        connect(m_editorController, &EditorController::categoryRenamed, this, &CategoriesModel::onRenamed);
    }
}

void CategoriesModel::clearNodes() {
    if (m_editorController) {
        disconnect(m_editorController, &EditorController::categoryAppended, this, &CategoriesModel::onAppended);
        disconnect(m_editorController, &EditorController::categoryRenamed, this, &CategoriesModel::onRenamed);
    }
    beginResetModel();
    m_mapNodes.clear();
    m_nodes.clear();
    endResetModel();
}

void CategoriesModel::invalidateFilter() {
    m_allCategories->invalidateFilter();
    m_incomeCategories->invalidateFilter();
    m_expenseCategories->invalidateFilter();
}
