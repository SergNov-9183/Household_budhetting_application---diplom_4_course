#ifndef OPERATIONSPROXYMODEL_H
#define OPERATIONSPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QAbstractListModel>

class OperationsProxyModel : public QSortFilterProxyModel {
    Q_OBJECT
    Q_PROPERTY(QAbstractListModel* model READ model WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(int accountId READ accountId WRITE setAccountId NOTIFY accountIdChanged FINAL)
public:
    explicit OperationsProxyModel(QObject* parent = nullptr);

signals:
    void modelChanged(QAbstractListModel*);
    void accountIdChanged(int);

public slots:
    void invalidateData();

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
