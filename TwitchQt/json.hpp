#ifndef JSON_HPP
#define JSON_HPP

#include <QString>
#include "json/json.hpp"

namespace nlohmann {
template <>
struct adl_serializer<QString> {
    static QString from_json(const json& j)
    {
        if (j.is_null())
            return QString("");
        else if (j.is_number()) {
            return QString::number(j.get<qint64>());
        } else if (j.is_string()) {
            return QString::fromStdString(j.get<std::string>());
        }
        return QString("");
    }

    static void to_json(json& j, const QString& t)
    {
        j = t.toStdString();
    }
};

template <class T>
struct adl_serializer<QVector<T>> {
    static QVector<T> from_json(const json& j)
    {
        return QVector<T>::fromStdVector(j);
    }

    static void to_json(json& j, const QVector<T>& vector)
    {
        j = vector.toStdVector();
    }
};
}

using JSON = nlohmann::json;

#endif // JSON_HPP
