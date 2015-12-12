#include "searchdialog.h"

#include <QGridLayout>
#include <QHeaderView>
#include <QDebug>
#include <QLabel>
#include <QJsonArray>

#include "searchmessage.h"
#include "windows.h"

static QStringList s_lastFileResult;
static QString s_lastSearch;

SearchDialog::SearchDialog(Config &config, DirectFileOpenerBase *fileOpener)
    :m_timer(new QTimer())
    ,m_config(config)
    ,m_fileOpener(fileOpener)
{
    QGridLayout *mainLayout = new QGridLayout;

    m_sessionsComboBox = new QComboBox(this);

    m_projectsComboBox = new QComboBox(this);
    m_projectsComboBox->setEnabled(false);

    m_fileInput = new QLineEdit(this);
    m_fileInput->setEnabled(false);
    m_fileInput->setText(s_lastSearch);

    KeyPressEater *keyPressEater = new KeyPressEater(this);
    m_fileInput->installEventFilter(keyPressEater);

    m_filesList = new QListWidget(this);
    m_filesList->addItems(s_lastFileResult);
    m_filesList->setSelectionMode(QAbstractItemView::ExtendedSelection);

    m_sessionsComboBox->addItems(m_config.getSessionsWithSearch());

    mainLayout->addWidget(new QLabel(tr("Session:")), 0, 0);
    mainLayout->addWidget(m_sessionsComboBox, 0, 1);
    mainLayout->addWidget(new QLabel(tr("Project:")), 1, 0);
    mainLayout->addWidget(m_projectsComboBox, 1, 1);
    mainLayout->addWidget(new QLabel(tr("Pattern:")), 2, 0);
    mainLayout->addWidget(m_fileInput, 2, 1);
    mainLayout->addWidget(new QLabel(tr("Result:")), 3, 0);
    mainLayout->addWidget(m_filesList, 3, 1);

    setLayout(mainLayout);
    setWindowTitle(tr("Abinsthe Client - Search for files"));

    connect(&m_webSocket, &QWebSocket::connected, this, &SearchDialog::onConnected);
    connect(&m_webSocket, &QWebSocket::disconnected, this, &SearchDialog::onClose);
    connect(&m_webSocket, &QWebSocket::textMessageReceived, this, &SearchDialog::onTextMessageReceived);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(onSearchDelayTimer()));
    connect(m_fileInput, SIGNAL(textChanged(const QString&)), this, SLOT(onSearch(const QString&)));
    connect(m_sessionsComboBox, SIGNAL(activated(const QString&)), this, SLOT(onSession(const QString&)));    
    connect(m_filesList, SIGNAL(itemActivated(QListWidgetItem*)), this, SLOT(onFileSelected(QListWidgetItem*)));

    m_timer->setSingleShot(true);

    m_sessionsComboBox->activated(m_config.getData()["search"].toObject()["last-session"].toString());

    resize(600, 400);

    SetForegroundWindow((HWND)winId());

    if (m_config.getData()["search"].toObject().find("rect") != m_config.getData()["search"].toObject().end()) {
        QJsonObject rect = m_config.getData()["search"].toObject()["rect"].toObject();
        setGeometry(rect["x"].toInt(), rect["y"].toInt(), rect["w"].toInt(), rect["h"].toInt());
    }

    setWindowIcon(QIcon(":/search-dialog-icon.png"));
}

SearchDialog::~SearchDialog()
{
    m_webSocket.close();
    QJsonObject& data = m_config.getData();
    QJsonObject& searchData = data["search"].toObject();
    QJsonObject qrect;
    QRect frect = geometry();
    qrect["x"] = frect.x();
    qrect["y"] = frect.y();
    qrect["w"] = frect.width();
    qrect["h"] = frect.height();
    searchData["rect"] = qrect;
    if(m_sessionsComboBox->count())
        searchData["last-session"] = m_sessionsComboBox->currentText();
    if(m_projectsComboBox->count())
        searchData["last-project"] = m_projectsComboBox->currentText();
    data["search"] = searchData;

    m_config.save();
}

bool SearchDialog::onKeyEvent(QKeyEvent *keyEvent)
{
    if (keyEvent->key() == Qt::Key_Up) {
        m_filesList->setCurrentRow(m_filesList->count()-1);
        m_filesList->setFocus();
        return true;
    } else if(keyEvent->key() == Qt::Key_Down) {
        m_filesList->setCurrentRow(0);
        m_filesList->setFocus();
        return true;
    }

    return false;
}

void SearchDialog::onSearch(const QString&)
{
    m_timer->stop();
    m_timer->start(m_config.getData()["search"].toObject()["delay"].toInt());
}

void SearchDialog::onSession(const QString &text)
{
    QJsonObject session = findSession(text);

    QJsonObject& host = m_config.getData()["hosts"].toObject()[session["host"].toString()].toObject();

    QUrl url = QString("ws://%1:%2").arg(host["host"].toString(), QString::number(host["search-port"].toInt()));

    if (m_webSocket.isValid())
        m_webSocket.close();

    m_webSocket.open(url);
}

void SearchDialog::onFileSelected(QListWidgetItem *)
{
    QList<QListWidgetItem*> items = m_filesList->selectedItems();
    QStringList files;
    for (auto it : items)
        files.append(it->text());
    qDebug() << "Selected files:" << files;
    m_fileOpener->openFile(m_sessionsComboBox->currentText(), files);
    close();
}

void SearchDialog::onConnected()
{
    qDebug() << "Connected to search server";

    m_webSocket.sendTextMessage(SearchRequest("get_projects"));
}

void SearchDialog::onTextMessageReceived(const QString &message)
{
    qDebug() << "Message received from search server:" << message;

    SearchResponse response;
    response.parse(message);

    if (response.getCommand() == "get_projects") {

        m_projectsComboBox->clear();
        for (auto it : response.getBody().toArray()) {
            m_projectsComboBox->addItem(it.toString());
        }
        m_projectsComboBox->setCurrentText(m_config.getData()["search"].toObject()["last-project"].toString());
        m_projectsComboBox->setEnabled(true);
        m_fileInput->setEnabled(true);
        m_fileInput->setFocus();
        m_fileInput->selectAll();

    } else if (response.getCommand() == "find_file") {

        m_filesList->clear();
        s_lastFileResult.clear();
        for (auto it : response.getBody().toArray()) {
            m_filesList->addItem(it.toString());
            s_lastFileResult.append(it.toString());
        }

    }
}

void SearchDialog::onClose()
{
    qDebug() << "Disconnected from search server";
}

void SearchDialog::onSearchDelayTimer()
{
    s_lastSearch = m_fileInput->text();

    QJsonObject session = findSession(m_sessionsComboBox->currentText());

    SearchRequest msg("find_file");
    msg["project"] = m_projectsComboBox->currentText();
    msg["pattern"] = m_fileInput->text();
    msg["path"] = session["remote-path"].toString();

    m_webSocket.sendTextMessage(msg);
}

QJsonObject SearchDialog::findSession(const QString &name)
{
    QJsonObject session;
    for (auto && it : m_config.getData()["sessions"].toArray()) {
        session = it.toObject();
        if(session["name"] == name)
            break;
    }
    return session;
}

bool KeyPressEater::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() != QEvent::KeyPress)
        return QObject::eventFilter(obj, event);

    return m_dialog->onKeyEvent(static_cast<QKeyEvent*>(event));
}
