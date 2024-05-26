#ifndef ANALYTICSPROXYMODEL_H
#define ANALYTICSPROXYMODEL_H

#include <QSortFilterProxyModel>

class AnalyticsProxyModel : public QSortFilterProxyModel {
    Q_OBJECT
    Q_PROPERTY(QAbstractListModel* model READ model WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(int currentCategory READ currentCategory WRITE setCurrentCategory NOTIFY currentCategoryChanged)

public:
    explicit AnalyticsProxyModel(QObject *parent = nullptr);

signals:
    void modelChanged(QAbstractListModel*);
    void currentCategoryChanged(int);

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const override;
    bool lessThan(const QModelIndex& source_left, const QModelIndex& source_right) const override;

private:
    QAbstractListModel* model();
    void setModel(QAbstractListModel* value);

    int currentCategory() const;
    void setCurrentCategory(int value);

    int m_currentCategory = 0;
};

#endif // ANALYTICSPROXYMODEL_H
