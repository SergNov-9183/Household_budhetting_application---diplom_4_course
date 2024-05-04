#include "HFSettings.h"

namespace {
    const char* const project_name = "ProjectName";
}

HFSettings::HFSettings(QSettings::Format format, QSettings::Scope scope, const QString& organization, const QString& application, QObject* parent) : QSettings{organization, application, parent} {
    readSettings();
}

QString HFSettings::projectName() const {
    return m_projectName;
}

void HFSettings::setProjectName(const QString& value, bool needSave) {
    if (m_projectName != value) {
        m_projectName = value;
        if (needSave) {
            setValue(project_name, value);
        }
        emit projectNameChanged(value);
    }
}

void HFSettings::readSettings() {
    setProjectName(value(project_name, "").toString(), false);
}
