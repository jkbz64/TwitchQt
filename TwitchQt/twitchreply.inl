
inline Reply::Reply(QNetworkReply* reply)
    : QObject(nullptr)
    , m_reply(reply)
    , m_currentState(ReplyState::Pending)
    , m_cursor("")
{
    connect(m_reply, &QNetworkReply::downloadProgress, this, &Reply::downloadProgress);
    connect(m_reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error),
    [this](QNetworkReply::NetworkError)
    {
        m_currentState = ReplyState::Error;
    });
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
        JSON json = JSON::parse(m_reply->readAll().data());
        // Check errors
        if(json.empty())
            m_currentState = ReplyState::Error;
        else
        {
            m_currentState = ReplyState::Success;
            parseData(json);

            if (json.find("pagination") != json.end()) // Save the pagination
                m_cursor = QString::fromStdString(json["pagination"]["cursor"].get<std::string>());
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

