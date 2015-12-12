#ifndef SEARCHMESSAGE_H
#define SEARCHMESSAGE_H

#include <QString>
#include <QVariant>
#include <QJsonValue>

class SearchMessage
{
    public:
        SearchMessage()
            :m_valid(false)
        {}

        SearchMessage(const QString& command)
            :m_valid(true)
        {
            m_command = command;
        }

        operator QString() const {
            return toString();
        }

        const QString& getCommand() const {
            return m_command;
        }

        bool isValid() const {
            return m_valid;
        }

        virtual QString toString() const = 0;
        virtual bool parse(const QString& msg) = 0;

    protected:
        bool m_valid;
        QString m_command;
};

class SearchRequest : public SearchMessage
{
    public:
        // Inherinted constructors not supported, just from VS2015
        SearchRequest(const QString& command)
            :SearchMessage(command)
        {}

        void setParameter(const QString& name, const QVariant& value);
        const QVariant& getParameter(const QString& name) const;

        const QVariant& operator [](const QString& name) const {
            return getParameter(name);
        }

        QVariant& operator [](const QString& name);

        QString toString() const;
        bool parse(const QString& msg);

    protected:
        QMap<QString, QVariant> m_parameters;
};

class SearchResponse : public SearchMessage
{
    public:
        QString toString() const;
        bool parse(const QString& msg);

        const QJsonValue& getBody() {
            return m_body;
        }

        unsigned short getStatus() {
            return m_status;
        }

    protected:
        unsigned short m_status;
        QJsonValue m_body;
};

#endif // SEARCHMESSAGE_H
