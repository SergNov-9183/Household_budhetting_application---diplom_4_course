#ifndef ACCOUNTSTABSPROXYMODEL_H
#define ACCOUNTSTABSPROXYMODEL_H

#include <QSortFilterProxyModel>

class AccountsTabsProxyModel : public QSortFilterProxyModel {
    Q_OBJECT
public:
    explicit AccountsTabsProxyModel(QObject* parent = nullptr);

    void invalidateData();

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const override;
    bool lessThan(const QModelIndex& source_left, const QModelIndex& source_right) const override;
};

#endif // ACCOUNTSTABSPROXYMODEL_H
