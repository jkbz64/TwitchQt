inline Reply::Reply(QNetworkReply* reply)
    : QObject(nullptr)
    , m_currentState(ReplyState::Pending)
    , m_reply(reply)
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
        }
        emit finished();
        m_reply->deleteLater();
    });
}

inline QVariant& Reply::data()
{
    return m_data;
}

inline const Reply::ReplyState& Reply::currentState() const
{
    return m_currentState;
}
