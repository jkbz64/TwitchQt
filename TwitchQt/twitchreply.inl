
inline Reply::Reply(QNetworkReply* reply)
    : QObject(nullptr)
    , m_reply(reply)
    , m_currentState(ReplyState::Pending)
    , m_cursor("")
{
}

inline Reply::~Reply()
{
}

inline QVariant& Reply::data()
{
    return m_data;
}

inline const ReplyState& Reply::currentState() const
{
    return m_currentState;
}

inline Reply::operator bool() const
{
    return currentState() == ReplyState::Success;
}

inline const QString& Reply::cursor() const
{
    return m_cursor;
}

inline RawReply::RawReply(QNetworkReply* reply)
    : Reply(reply)
{
    connect(m_reply, &QNetworkReply::finished, this, [this]() {
        auto data = m_reply->readAll();
        // Check errors
        if (data.isEmpty() || data.isNull()) {
            m_currentState = ReplyState::Error;
        } else {
            m_currentState = ReplyState::Success;
            parseData(data);
        }
        emit finished();
        m_reply->deleteLater();
        if (!parent())
            deleteLater();
    });
}

inline RawReply::~RawReply() = default;

inline JSONReply::JSONReply(QNetworkReply* reply)
    : Reply(reply)
{
    connect(m_reply, &QNetworkReply::finished, this, [this]() {
        auto json = QJsonDocument::fromJson(m_reply->readAll());
        // Check errors
        auto root = json.object();
        if (root.find("error") != root.end() || json.isEmpty()) {
            m_currentState = ReplyState::Error;
            // TODO error handling
        } else {
            m_currentState = ReplyState::Success;
            parseData(json);

            if (root.find("pagination") != root.end()) // Save the pagination
                m_cursor = root.value("pagination").toObject().value("cursor").toString();
        }
        emit finished();
        m_reply->deleteLater();
        if (!parent())
            deleteLater();
    });
}

inline JSONReply::~JSONReply() = default;

inline void ImageReply::parseData(const QByteArray &data)
{
    m_data.setValue(QImage::fromData(data));
}
