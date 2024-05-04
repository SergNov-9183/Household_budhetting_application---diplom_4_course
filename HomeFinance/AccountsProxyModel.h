#ifndef ACCOUNTSPROXYMODEL_H
#define ACCOUNTSPROXYMODEL_H

#include <QSortFilterProxyModel>

class AccountsProxyModel : public QSortFilterProxyModel {
    Q_OBJECT
public:
    explicit AccountsProxyModel(QObject* parent = nullptr);

public slots:
    void shevronClisked(int id);
    void invalidateFilter();

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const override;
    bool lessThan(const QModelIndex& source_left, const QModelIndex& source_right) const override;

};

#endif // ACCOUNTSPROXYMODEL_H
