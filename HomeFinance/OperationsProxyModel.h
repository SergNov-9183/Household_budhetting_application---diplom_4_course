#ifndef OPERATIONSPROXYMODEL_H
#define OPERATIONSPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QAbstractListModel>
#include "GlobalDefines.h"

class OperationsProxyModel : public QSortFilterProxyModel, public IInvalidateFilter {
    Q_OBJECT
    Q_PROPERTY(QAbstractListModel* model READ model WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(int accountId READ accountId WRITE setAccountId NOTIFY accountIdChanged FINAL)
public:
    explicit OperationsProxyModel(QObject* parent = nullptr);

    void invalidateData() override;

signals:
    void modelChanged(QAbstractListModel*);
    void accountIdChanged(int);

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const override;

private:
    QAbstractListModel* model();
    void setModel(QAbstractListModel* value);

    int accountId() const;
    void setAccountId(int value);

    void registerInModel();

    int m_accountId = 0;
};


#endif // OPERATIONSPROXYMODEL_H
