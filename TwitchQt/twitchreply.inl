
inline Reply::Reply(QNetworkReply* reply)
    : QObject(reply->manager())
    , m_reply(reply)
    , m_currentState(ReplyState::Pending)
    , m_cursor("")
{
    connect(m_reply, &QNetworkReply::finished, this, &Reply::onFinished, Qt::DirectConnection);
    connect(m_reply, &QNetworkReply::downloadProgress, this, &Reply::downloadProgress, Qt::UniqueConnection);
    connect(m_reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error),
        [this](QNetworkReply::NetworkError) {
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
}

inline RawReply::~RawReply() = default;

inline JSONReply::JSONReply(QNetworkReply* reply)
    : Reply(reply)
{
}

inline JSONReply::~JSONReply()
{
}

inline void ImageReply::parseData(const QByteArray& data)
{
    m_data.setValue(QImage::fromData(data));
}

inline void RawReply::onFinished()
{
    auto data = m_reply->readAll();
    // Check errors
    if (data.isEmpty() || data.isNull()) {
        m_currentState = ReplyState::Error;
    } else {
        m_currentState = ReplyState::Success;
        //parseData(data);
    }

    emit finished();
    m_reply->setParent(nullptr);
    m_reply->deleteLater();
}

inline void JSONReply::onFinished()
{
    JSON::parser_callback_t cb = [](int, JSON::parse_event_t event, JSON& parsed) {
        // Skip values with null value
        if (event == JSON::parse_event_t::value and parsed.is_null()) {
            return false;
        } else {
            return true;
        }
    };

    auto data = m_reply->readAll();
    {
        m_json = JSON::parse(data.constData(), cb);
        if (m_json.empty())
            m_currentState = ReplyState::Error;
        else {
            m_currentState = ReplyState::Success;
            parseData(m_json);

            if (m_json.find("pagination") != m_json.end()) // Save the pagination
                m_cursor = QString::fromStdString(m_json["pagination"]["cursor"].get<std::string>());
        }
    }

    emit finished();
    m_reply->setParent(nullptr);
    m_reply->deleteLater();
}

inline const JSON &Twitch::JSONReply::json() const
{
    return m_json;
}

inline void JSONReply::parseData(const JSON &)
{

}
