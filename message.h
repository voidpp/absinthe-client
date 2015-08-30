#ifndef MESSAGE_H
#define MESSAGE_H

#include <QString>
#include <QMap>
#include <QVariant>

class Message
{
    public:
        Message(const QString& rawMassage);
        Message(const QString& name, const QString& command);

        operator QString() const {
            return toString();
        }

        const QString& getName() const {
            return m_name;
        }

        const QString& getCommand() const {
            return m_command;
        }

        bool isValid() const {
            return m_valid;
        }

        void setArgument(const QString& name, const QVariant& value);
        const QVariant& getArgument(const QString& name) const;

        const QVariant& operator [](const QString& name) const {
            return getArgument(name);
        }

        QVariant& operator [](const QString& name);

        QString toString() const;

    private:
        bool parse(const QString& msg);

        bool m_valid;

        QString m_name;
        QString m_command;
        QMap<QString, QVariant> m_arguments;
};

#endif // MESSAGE_H
