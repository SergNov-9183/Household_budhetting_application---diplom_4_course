#include "CategoriesModel.h"

#include <QTimer>

CategoriesModel::CategoriesModel(QObject *parent)
    : QAbstractListModel{parent}
    , m_allCategories(new CategoriesProxyModel(CategoriesProxyModel::CategoryType::All, this))
    , m_incomeCategories(new CategoriesProxyModel(CategoriesProxyModel::CategoryType::Income, this))
    , m_expenseCategories(new CategoriesProxyModel(CategoriesProxyModel::CategoryType::Expense, this)) {
    setPeriodDates();
    m_roles = QAbstractListModel::roleNames();
    m_roles[Name]        = "name";
    m_roles[Id]          = "id";
    m_roles[Level]       = "level";
    m_roles[Income]      = "income";
    m_roles[ParentId]    = "parentId";
    m_roles[IsExpanded]  = "isExpanded";
    m_roles[HasChildren] = "hasChildren";
    m_roles[Sum]         = "sum";
    m_roles[TotalSum]    = "totalSum";
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

bool CategoriesModel::isIncome(int categoryId) const {
    return m_editorController && m_editorController->categories()->at(m_mapNodes.at(categoryId)).income;
}

QDate CategoriesModel::beginDate() const {
    return m_beginPeriodDate;
}

QDate CategoriesModel::endDate() const {
    return m_endPeriodDate;
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
    case Sum:         return m_nodes[row].sum;
    case TotalSum:    return m_nodes[row].totalSum;
    default:          return QVariant();
    }
}

QHash<int, QByteArray> CategoriesModel::roleNames() const {
    return m_roles;
}

int CategoriesModel::defaultCategory() const {
    return m_defaultCategory;
}

void CategoriesModel::onProjectLoaded() {
    if (m_editorController) {
        beginResetModel();
        m_defaultCategory = 0;
        m_incomeCategory = 0;
        m_mapNodes.clear();
        m_nodes.clear();
        auto categories = m_editorController->categories();
        for (auto row = 0; row < categories->size(); ++row) {
            m_nodes.push_back({ categories->at(row).parentId == 0 });
            m_mapNodes[categories->at(row).id] = row;
            if (categories->at(row).parentId < 1) {
                if (categories->at(row).income) {
                    m_incomeCategory = categories->at(row).id;
                }
                else {
                    m_defaultCategory = categories->at(row).id;
                }
            }
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

void CategoriesModel::onMoved(int id, int oldParentId) {
    auto row        = m_mapNodes.at(id);
    auto modelIndex = createIndex(row, 0);
    emit dataChanged(modelIndex, modelIndex, { ParentId });
    row = m_mapNodes.at(category(row).parentId);
    m_nodes[row].children.insert(id);
    modelIndex = createIndex(row, 0);
    emit dataChanged(modelIndex, modelIndex, { HasChildren });
    row = m_mapNodes.at(oldParentId);
    if (auto child = std::find_if(m_nodes.at(row).children.begin(), m_nodes.at(row).children.end(), [id](int childId) { return childId == id; });
        child != m_nodes.at(row).children.end()) {
        m_nodes.at(row).children.erase(child);
    }
    modelIndex = createIndex(row, 0);
    emit dataChanged(modelIndex, modelIndex, { HasChildren });
    invalidateData();
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

int CategoriesModel::periodType() const {
    return static_cast<int>(m_periodType);
}

QString CategoriesModel::beginPeriodDate() const {
    return m_beginPeriodDate.toString("dd.MM.yyyy");
}

QString CategoriesModel::endPeriodDate() const {
    return m_endPeriodDate.toString("dd.MM.yyyy");
}

bool CategoriesModel::isValidIndex(int value) const {
    return value >= 0 && value < m_nodes.size();
}

void CategoriesModel::disconnectController() {
    if (m_editorController) {
        disconnect(m_editorController, &EditorController::projectLoaded, this, &CategoriesModel::onProjectLoaded);
        disconnect(m_editorController, &EditorController::categoryAppended, this, &CategoriesModel::onAppended);
        disconnect(m_editorController, &EditorController::categoryRenamed, this, &CategoriesModel::onRenamed);
        disconnect(m_editorController, &EditorController::categoryMoved, this, &CategoriesModel::onMoved);
    }
    beginResetModel();
    m_defaultCategory = 0;
    m_mapNodes.clear();
    m_nodes.clear();
    endResetModel();
}

void CategoriesModel::connectController() {
    if (m_editorController) {
        connect(m_editorController, &EditorController::projectLoaded, this, &CategoriesModel::onProjectLoaded);
        connect(m_editorController, &EditorController::categoryAppended, this, &CategoriesModel::onAppended);
        connect(m_editorController, &EditorController::categoryRenamed, this, &CategoriesModel::onRenamed);
        connect(m_editorController, &EditorController::categoryMoved, this, &CategoriesModel::onMoved);
    }
}

void CategoriesModel::invalidateData() {
    m_allCategories->invalidate();
    m_incomeCategories->invalidate();
    m_expenseCategories->invalidate();
}

int CategoriesModel::level(int row) const {
    int result    = 0;
    auto parentId = category(row).parentId;
    while (parentId > 0) {
        ++result;
        row = m_mapNodes.at(parentId);
        parentId = category(row).parentId;
    }
    return result;
}

const Category& CategoriesModel::category(int row) const {
    return m_editorController->categories()->at(row);
}

QString CategoriesModel::getFullCategoryName(int id) const {
    QString result;
    while (id > 0) {
        result = QString::fromStdString(category(m_mapNodes.at(id)).name) + result;
        id = category(m_mapNodes.at(id)).parentId;
        if ( id > 0) {
            result = "/" + result;
        }
    }
    return result;
}

bool CategoriesModel::canDrop(int dragAreaId, int dropAreaId) const {
    if (dragAreaId > 1 && dropAreaId > 0 && dragAreaId != dropAreaId) {
        auto dragAreaRow = m_mapNodes.at(dragAreaId);
        auto dropAreaRow = m_mapNodes.at(dropAreaId);
        if (level(dragAreaRow) != 0 &&
            category(dropAreaRow).income == category(dragAreaRow).income &&
            dropAreaId != category(dragAreaRow).parentId) {
            while (dropAreaId > 0) {
                dropAreaRow = m_mapNodes.at(dropAreaId);
                dropAreaId  = category(dropAreaRow).parentId;
                if (dropAreaId == dragAreaId) {
                    return false;
                }
            }
            return true;
        }
    }
    return false;
}

float CategoriesModel::calculateTotalSums(int id) {
    auto totalSum = m_nodes.at(m_mapNodes.at(id)).sum;
    for(auto childId : m_nodes.at(m_mapNodes.at(id)).children) {
        totalSum += calculateTotalSums(childId);
    }
    m_nodes.at(m_mapNodes.at(id)).totalSum = totalSum;
    return totalSum;
}

void CategoriesModel::setPeriodDates(const QDate& beginPeriodDate, const QDate& endPeriodDate) {
    auto date = QDate::currentDate();
    if (m_periodType == PeriodType::Enum::Year) {
        m_beginPeriodDate = QDate(date.year(), 1, 1);
        m_endPeriodDate   = QDate(date.year(), 12, 31);
    }
    else if (m_periodType == PeriodType::Enum::Month) {
        m_beginPeriodDate = QDate(date.year(), date.month(), 1);
        m_endPeriodDate   = QDate(date.year(), date.month(), date.daysInMonth());
    }
    else if (m_periodType == PeriodType::Enum::Week) {
        m_beginPeriodDate = date.addDays(-date.dayOfWeek() + 1);
        m_endPeriodDate   = m_beginPeriodDate.addDays(6);
    }
    else {
        m_beginPeriodDate = beginPeriodDate;
        m_endPeriodDate   = endPeriodDate;
    }
    emit beginPeriodDateChanged(m_beginPeriodDate.toString("dd.MM.yyyy"));
    emit endPeriodDateChanged(m_endPeriodDate.toString("dd.MM.yyyy"));
}

void CategoriesModel::analyzeData() {
    if (m_editorController) {
        std::map<int, float> sum;
        auto operations = m_editorController->operations();
        for (auto i = 0; i < operations->size(); ++i) {
            auto date = QDateTime::fromString(QString::fromStdString(operations->at(i).date)).date();
            if (date >= m_beginPeriodDate && date <= m_endPeriodDate) {
                auto categoryId = operations->at(i).categoryId;
                sum[categoryId] = sum[categoryId] + operations->at(i).price;
            }
        }

        for (auto& node : m_nodes) {
            node.sum = 0;
        }

        for (const auto& item : sum) {
            auto row = m_mapNodes.at(item.first);
            m_nodes.at(row).sum = item.second;
        }

        calculateTotalSums(m_incomeCategory);
        calculateTotalSums(m_defaultCategory);

        auto beginModelIndex = createIndex(0, 0);
        auto endModelIndex = createIndex(m_nodes.size() - 1, 0);
        emit dataChanged(beginModelIndex, endModelIndex, { Sum, TotalSum });
        emit analyzeDataComplited();
    }
}

void CategoriesModel::setPeriodType(int value, const QDate& beginPeriodDate, const QDate& endPeriodDate) {
    auto newValue = static_cast<PeriodType::Enum>(value);
    if (m_periodType != newValue || newValue == PeriodType::Enum::Custom) {
        m_periodType = newValue;
        setPeriodDates(beginPeriodDate, endPeriodDate);
        emit periodTypeChanged(value);
        analyzeData();
    }
}
