#ifndef OPERATIONSPROXYMODEL_H
#define OPERATIONSPROXYMODEL_H

#include <QSortFilterProxyModel>

class OperationsProxyModel
    : public QSortFilterProxyModel {
    Q_OBJECT
public:
    enum class OperationType {
        All
    };
    explicit OperationsProxyModel(OperationType operationType, QObject* parent = nullptr);

public slots:
    void invalidateData();


private:
    OperationType m_operationType;
};

#endif // OPERATIONSPROXYMODEL_H
