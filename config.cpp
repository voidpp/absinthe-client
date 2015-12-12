#include "config.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>
#include <QFile>

bool Config::load()
{
    qDebug() << "Loading config file:" << m_filename;

    QFile file(m_filename);

    if (!file.open(QIODevice::ReadOnly))
        return false;

    QJsonDocument loadDoc(QJsonDocument::fromJson(file.readAll()));

    m_data = loadDoc.object();

    file.close();

    return true;
}

bool Config::save()
{
    qDebug() << "Save config file:" << m_filename;

    QJsonDocument doc(m_data);

    QFile file(m_filename);

    if (!file.open(QIODevice::WriteOnly))
        return false;

    file.write(doc.toJson(QJsonDocument::Indented));

    file.close();

    return true;
}

QStringList Config::getSessionsWithSearch() const
{
    QStringList result;
    QJsonObject& hosts = m_data["hosts"].toObject();

    for (auto && it : m_data["sessions"].toArray()) {
        QJsonObject session = it.toObject();
        if(hosts[session["host"].toString()].toObject()["search-port"].toInt() > 0)
            result.append(session["name"].toString());
    }

    return result;
}
