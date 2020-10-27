#include <QDir>
#include <QString>
#include <QSettings>
#include <QtDebug>

//! Класс для создания конфигурационного файла
class SettingsWrapper {

public:

    SettingsWrapper(const QDir & appDataDir);
    ~SettingsWrapper() { delete settings; }

    const QVariant getOrSetValue(const QString & key, const QVariant & value);
    const QVariant getValue(const QString & key);

    const QString getFileName() const {
        return settings->fileName();
    }

private:

    QSettings * settings;

};

inline SettingsWrapper::SettingsWrapper(const QDir & appDataDir) {
    settings = new QSettings(appDataDir.relativeFilePath("settings.ini"), QSettings::IniFormat);
    settings->sync();
}

inline const QVariant SettingsWrapper::getOrSetValue(const QString & key, const QVariant & value) {
    if (!settings->contains(key)) {
        settings->setValue(key, value);
        return value;
    }
    return settings->value(key, value);
}

inline const QVariant SettingsWrapper::getValue(const QString &key) {
    return settings->value(key);
}
