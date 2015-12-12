#include "searchmessage.h"

#include <QJsonObject>
#include <QJsonDocument>

QVariant nullSearchVariant;

void SearchRequest::setParameter(const QString &name, const QVariant &value)
{
    m_parameters[name] = value;
}

const QVariant &SearchRequest::getParameter(const QString &name) const
{
    auto argIt = m_parameters.find(name);

    if(argIt == m_parameters.end())
        return nullSearchVariant;

    return argIt.value();
}

QString SearchRequest::toString() const
{
    QJsonObject root;

    root["command"] = m_command;

    QJsonObject args;
    for(auto name : m_parameters.keys())
        args[name] = QJsonValue::fromVariant(m_parameters.value(name));
    root["params"] = args;

    QJsonDocument doc(root);
    return doc.toJson(QJsonDocument::Compact);
}

bool SearchRequest::parse(const QString &msg)
{
    QJsonDocument jsonMessage = QJsonDocument::fromJson(msg.toUtf8());

    QJsonObject data = jsonMessage.object();

    m_command = data["command"].toString();
    m_parameters = data["params"].toObject().toVariantMap();

    return true;
}

QVariant &SearchRequest::operator [](const QString &name)
{
    return m_parameters[name];
}

QString SearchResponse::toString() const
{
    QJsonObject root;

    root["command"] = m_command;
    root["status"] = m_status;
    root["body"] = m_body;

    QJsonDocument doc(root);
    return doc.toJson(QJsonDocument::Compact);
}

bool SearchResponse::parse(const QString &msg)
{
    QJsonDocument jsonMessage = QJsonDocument::fromJson(msg.toUtf8());

    QJsonObject data = jsonMessage.object();

    m_command = data["command"].toString();
    m_status = data["status"].toInt();
    m_body = data["body"];

    return true;
}
