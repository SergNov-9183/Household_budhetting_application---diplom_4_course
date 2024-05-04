#ifndef HFSETTINGS_H
#define HFSETTINGS_H

#include <QSettings>

class HFSettings : public QSettings {
    Q_OBJECT
    Q_PROPERTY(QString projectName READ projectName NOTIFY projectNameChanged)
public:
    explicit HFSettings(QSettings::Format format, QSettings::Scope scope, const QString& organization, const QString& application, QObject* parent = nullptr);

    QString projectName() const;
    void setProjectName(const QString& value, bool needSave = true);

signals:
    void projectNameChanged(const QString&);

private:

    void readSettings();

    QString m_projectName;

};

#endif // HFSETTINGS_H
