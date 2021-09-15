#ifndef JSON_HPP
#define JSON_HPP

#include <QString>
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <QVector>
#endif
#include <QList>
#include <QMultiMap>
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

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)

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

#endif

template <class T>
struct adl_serializer<QList<T>> {
    static QList<T> from_json(const json& j)
    {
        return QList<T>(j.begin(), j.end());
    }

    static void to_json(json& j, const QList<T>& list)
    {
        j = json::array();
        for (const auto& value : list)
            j.push_back(value);
    }
};

template <class K, class V>
struct adl_serializer<QMultiMap<K, V>> {
    static QMultiMap<K, V> from_json(const json& j)
    {
        QMultiMap<K, V> multimap;
        for (auto it = j.begin(); it != j.end(); ++it) {
            const K key = it.key();
            json values = j[key];
            for (const auto& valueObject : values) {
                V value = valueObject;
                multimap.insert(key, value);
            }
        }
        return multimap;
    }

    static void to_json(json& j, const QMultiMap<K, V>& map)
    {
        for (const auto& key : map.keys()) {
            QList<V> values = map.values(key);
            j[key] = values;
        }
    }
};
}

using JSON = nlohmann::json;

#endif // JSON_HPP
