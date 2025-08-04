// DatabaseManager.h

#pragma once

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <QVariantList>

class DatabaseManager : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseManager(QObject* parent = nullptr);
    ~DatabaseManager();

    bool open(const QString& host, int port,
              const QString& dbName,
              const QString& user,
              const QString& password);
    void close();
    bool isOpen() const;

    // --- ADD ---
    Q_INVOKABLE bool addProject(const QString& name,
                                const QString& summary,
                                const QString& imagePath);

    // --- LOAD ---
    Q_INVOKABLE QVariantList getAllProjects();

    // --- Characters ---
    Q_INVOKABLE bool addCharacter(int projectId, const QString &name, const QString &summary, const QString &imagePath);
    Q_INVOKABLE QVariantList getCharacters(int projectId);
    Q_INVOKABLE bool updateCharacter(int id, const QString &name, const QString &summary, const QString &imagePath);
    Q_INVOKABLE bool deleteCharacter(int id);

    // --- Locations ---
    Q_INVOKABLE bool addLocation(int projectId, const QString &name, const QString &summary, const QString &imagePath);
    Q_INVOKABLE QVariantList getLocations(int projectId);
    Q_INVOKABLE bool updateLocation(int id, const QString &name, const QString &summary, const QString &imagePath);
    Q_INVOKABLE bool deleteLocation(int id);

    // --- Events ---
    Q_INVOKABLE bool addEvent(int projectId, const QString &name, const QString &summary, const QString &imagePath);
    Q_INVOKABLE QVariantList getEvents(int projectId);
    Q_INVOKABLE bool updateEvent(int id, const QString &name, const QString &summary, const QString &imagePath);
    Q_INVOKABLE bool deleteEvent(int id);
    Q_INVOKABLE bool exportProject(int projectId, const QString &filePath);

private:
    QSqlDatabase db;
};
