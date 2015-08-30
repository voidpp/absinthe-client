#include "message.h"
#include <QJsonObject>
#include <QJsonDocument>

QVariant nullVariant;

Message::Message(const QString& rawMassage)
    :m_valid(false)
{
    m_valid = parse(rawMassage);
}

Message::Message(const QString& name, const QString& command)
    :m_valid(true)
{
    m_name = name;
    m_command = command;
}

void Message::setArgument(const QString &name, const QVariant &value)
{
    m_arguments[name] = value;
}

const QVariant &Message::getArgument(const QString &name) const
{
    auto argIt = m_arguments.find(name);

    if(argIt == m_arguments.end())
        return nullVariant;

    return argIt.value();
}

QVariant &Message::operator [](const QString &name)
{
    return m_arguments[name];
}

QString Message::toString() const
{
    QJsonObject root;

    root["name"] = m_name;
    root["command"] = m_command;

    QJsonObject args;
    for(auto name : m_arguments.keys())
        args[name] = QJsonValue::fromVariant(m_arguments.value(name));
    root["arguments"] = args;

    QJsonDocument doc(root);
    return doc.toJson(QJsonDocument::Compact);
}

bool Message::parse(const QString& msg)
{
    QJsonDocument jsonMessage = QJsonDocument::fromJson(msg.toUtf8());

    QJsonObject data = jsonMessage.object();

    m_name = data["name"].toString();
    m_command = data["command"].toString();
    m_arguments = data["arguments"].toObject().toVariantMap();

    return true;
}
