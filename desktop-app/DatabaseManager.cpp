#include "DatabaseManager.h"
#include <QSqlError>
#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

DatabaseManager::DatabaseManager(QObject* parent)
    : QObject(parent)
{
    db = QSqlDatabase::addDatabase("QPSQL");
}

DatabaseManager::~DatabaseManager()
{
    close();
}

bool DatabaseManager::open(const QString& host, int port, const QString& dbName,
                           const QString& user, const QString& password)
{
    db.setHostName(host);
    db.setPort(port);
    db.setDatabaseName(dbName);
    db.setUserName(user);
    db.setPassword(password);

    if (!db.open()) {
        qWarning() << "Failed to open DB:" << db.lastError().text();
        return false;
    }
    return true;
}

void DatabaseManager::close()
{
    if (db.isOpen()) db.close();
}

bool DatabaseManager::isOpen() const
{
    return db.isOpen();
}

// --- ADD PROJECT ---
bool DatabaseManager::addProject(const QString& name,
                                 const QString& summary,
                                 const QString& imagePath)
{
    if (!db.isOpen()) return false;

    QSqlQuery query(db);
    query.prepare("INSERT INTO projects (name, summary, image_path) "
                  "VALUES (:name, :summary, :imagePath)");
    query.bindValue(":name", name);
    query.bindValue(":summary", summary);
    query.bindValue(":imagePath", imagePath);

    if (!query.exec()) {
        qWarning() << "Failed to insert project:" << query.lastError().text();
        return false;
    }

    return true;
}



bool DatabaseManager::addCharacter(int projectId, const QString &name, const QString &summary, const QString &imagePath)
{
    QSqlQuery query;
    query.prepare("INSERT INTO characters (project_id, name, summary, image_path) "
                  "VALUES (:project_id, :name, :summary, :image_path)");
    query.bindValue(":project_id", projectId);
    query.bindValue(":name", name);
    query.bindValue(":summary", summary);
    query.bindValue(":image_path", imagePath);
    return query.exec();
}

QVariantList DatabaseManager::getCharacters(int projectId)
{
    QVariantList characters;
    QSqlQuery query;
    query.prepare("SELECT id, name, summary, image_path FROM characters WHERE project_id = :project_id");
    query.bindValue(":project_id", projectId);
    if (query.exec()) {
        while (query.next()) {
            QVariantMap charMap;
            charMap["id"] = query.value("id").toInt();
            charMap["name"] = query.value("name").toString();
            charMap["summary"] = query.value("summary").toString();
            charMap["imagePath"] = query.value("image_path").toString();
            characters.append(charMap);
        }
    }
    return characters;
}

bool DatabaseManager::updateCharacter(int id, const QString &name, const QString &summary, const QString &imagePath)
{
    QSqlQuery query;
    query.prepare("UPDATE characters SET name = :name, summary = :summary, image_path = :image_path WHERE id = :id");
    query.bindValue(":name", name);
    query.bindValue(":summary", summary);
    query.bindValue(":image_path", imagePath);
    query.bindValue(":id", id);
    return query.exec();
}

bool DatabaseManager::deleteCharacter(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM characters WHERE id = :id");
    query.bindValue(":id", id);
    return query.exec();
}


//Locations
bool DatabaseManager::addLocation(int projectId, const QString &name, const QString &summary, const QString &imagePath)
{
    QSqlQuery query;
    query.prepare("INSERT INTO locations (project_id, name, summary, image_path) "
                  "VALUES (:project_id, :name, :summary, :image_path)");
    query.bindValue(":project_id", projectId);
    query.bindValue(":name", name);
    query.bindValue(":summary", summary);
    query.bindValue(":image_path", imagePath);
    return query.exec();
}

QVariantList DatabaseManager::getLocations(int projectId)
{
    QVariantList locations;
    QSqlQuery query;
    query.prepare("SELECT id, name, summary, image_path FROM locations WHERE project_id = :project_id");
    query.bindValue(":project_id", projectId);
    if (query.exec()) {
        while (query.next()) {
            QVariantMap charMap;
            charMap["id"] = query.value("id").toInt();
            charMap["name"] = query.value("name").toString();
            charMap["summary"] = query.value("summary").toString();
            charMap["imagePath"] = query.value("image_path").toString();
            locations.append(charMap);
        }
    }
    return locations;
}

bool DatabaseManager::updateLocation(int id, const QString &name, const QString &summary, const QString &imagePath)
{
    QSqlQuery query;
    query.prepare("UPDATE locations SET name = :name, summary = :summary, image_path = :image_path WHERE id = :id");
    query.bindValue(":name", name);
    query.bindValue(":summary", summary);
    query.bindValue(":image_path", imagePath);
    query.bindValue(":id", id);
    return query.exec();
}

bool DatabaseManager::deleteLocation(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM locations WHERE id = :id");
    query.bindValue(":id", id);
    return query.exec();
}


//Events

bool DatabaseManager::addEvent(int projectId, const QString &name, const QString &summary, const QString &imagePath)
{
    QSqlQuery query;
    query.prepare("INSERT INTO events (project_id, name, summary, image_path) "
                  "VALUES (:project_id, :name, :summary, :image_path)");
    query.bindValue(":project_id", projectId);
    query.bindValue(":name", name);
    query.bindValue(":summary", summary);
    query.bindValue(":image_path", imagePath);
    return query.exec();
}

QVariantList DatabaseManager::getEvents(int projectId)
{
    QVariantList events;
    QSqlQuery query;
    query.prepare("SELECT id, name, summary, image_path FROM events WHERE project_id = :project_id");
    query.bindValue(":project_id", projectId);
    if (query.exec()) {
        while (query.next()) {
            QVariantMap charMap;
            charMap["id"] = query.value("id").toInt();
            charMap["name"] = query.value("name").toString();
            charMap["summary"] = query.value("summary").toString();
            charMap["imagePath"] = query.value("image_path").toString();
            events.append(charMap);
        }
    }
    return events;
}

bool DatabaseManager::updateEvent(int id, const QString &name, const QString &summary, const QString &imagePath)
{
    QSqlQuery query;
    query.prepare("UPDATE events SET name = :name, summary = :summary, image_path = :image_path WHERE id = :id");
    query.bindValue(":name", name);
    query.bindValue(":summary", summary);
    query.bindValue(":image_path", imagePath);
    query.bindValue(":id", id);
    return query.exec();
}

bool DatabaseManager::deleteEvent(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM events WHERE id = :id");
    query.bindValue(":id", id);
    return query.exec();
}



// --- LOAD PROJECTS ---
QVariantList DatabaseManager::getAllProjects()
{
    QVariantList projectList;

    if (!db.isOpen()) return projectList;

    QSqlQuery query(db);
    if (!query.exec("SELECT id, name, summary, image_path FROM projects ORDER BY id")) {
        qWarning() << "Failed to load projects:" << query.lastError().text();
        return projectList;
    }

    while (query.next()) {
        QVariantMap project;
        project["id"] = query.value("id");
        project["name"] = query.value("name");
        project["summary"] = query.value("summary");
        project["imagePath"] = query.value("image_path");

        projectList.append(project);
    }

    return projectList;
}


// Export data
bool DatabaseManager::exportProject(int projectId, const QString &filePath)
{
    if (!db.isOpen()) {
        qWarning() << "Export failed: DB not open";
        return false;
    }

    // Convert file:///... to local path
    QString localPath = QUrl(filePath).toLocalFile();
    qDebug() << "Exporting project to:" << localPath;

    QSqlQuery q;

    // 1) Fetch project info
    q.prepare("SELECT id, name, summary FROM projects WHERE id = :id");
    q.bindValue(":id", projectId);
    if (!q.exec() || !q.next()) {
        qWarning() << "Export failed: cannot load project info:" << q.lastError().text();
        return false;
    }

    QJsonObject root;
    QJsonObject proj;
    proj["id"]      = q.value("id").toInt();
    proj["name"]    = q.value("name").toString();
    proj["summary"] = q.value("summary").toString();
    root["project"] = proj;

    // 2) Characters
    QJsonArray chars;
    q.prepare("SELECT id, name, summary FROM characters WHERE project_id = :pid");
    q.bindValue(":pid", projectId);
    if (!q.exec()) {
        qWarning() << "Failed to load characters:" << q.lastError().text();
        return false;
    }
    while (q.next()) {
        QJsonObject o;
        o["id"]      = q.value("id").toInt();
        o["name"]    = q.value("name").toString();
        o["summary"] = q.value("summary").toString();
        chars.append(o);
    }
    root["characters"] = chars;

    // 3) Locations
    QJsonArray locs;
    q.prepare("SELECT id, name, summary FROM locations WHERE project_id = :pid");
    q.bindValue(":pid", projectId);
    if (!q.exec()) {
        qWarning() << "Failed to load locations:" << q.lastError().text();
        return false;
    }
    while (q.next()) {
        QJsonObject o;
        o["id"]      = q.value("id").toInt();
        o["name"]    = q.value("name").toString();
        o["summary"] = q.value("summary").toString();
        locs.append(o);
    }
    root["locations"] = locs;

    // 4) Events
    QJsonArray evs;
    q.prepare("SELECT id, name, summary FROM events WHERE project_id = :pid");
    q.bindValue(":pid", projectId);
    if (!q.exec()) {
        qWarning() << "Failed to load events:" << q.lastError().text();
        return false;
    }
    while (q.next()) {
        QJsonObject o;
        o["id"]      = q.value("id").toInt();
        o["name"]    = q.value("name").toString();
        o["summary"] = q.value("summary").toString();
        evs.append(o);
    }
    root["events"] = evs;

    // 5) Write to file
    QJsonDocument doc(root);
    QFile f(localPath);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        qWarning() << "Failed to open file for writing:" << localPath << f.errorString();
        return false;
    }

    f.write(doc.toJson(QJsonDocument::Indented));
    f.close();

    qDebug() << "Export successful!";
    return true;
}

