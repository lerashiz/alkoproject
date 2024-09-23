#include <database.h>
#include <QCoreApplication>

Database* Database::p_instance = 0;

    Database::~Database() {
        if (m_database.isOpen())
            disconnect();
        if (p_instance)
            delete p_instance;
    }
    Database* Database::getInstance() //Используется для получения единого экземпляра базы данных. Все методы к базе данных выполняются через вызов этого мето
    {
        if (!p_instance)
        {
            p_instance = new Database();
        }
        return p_instance;
    }

    QStringList Database::send_query(QString queryStr, bool is_selection) //Отправляет запрос и возвращает список значений, возвращенных запросом типа string. Аргумент: SQL запрос в виде строки
    {
        QSqlQuery query;
        QStringList list;
        query.prepare(queryStr);
        if (!query.exec()){
            qDebug() << "Invalid query:" << query.lastError().text();
            list.append("!0!");
            return list;
        }
        if (!is_selection){
            list.append("!1!");
            return list;
        }

        while (query.next())
            list.append(query.value(0).toString());
        return list;
    }

    bool Database::connect()
    {
        // Устанавливаем соединение с базой данных
        m_database = QSqlDatabase::addDatabase("QSQLITE");
        m_database.setDatabaseName("../../sql_app.db");

        // Создание таблицы пользователей при инициализации объекта
        if (m_database.open()) {
            QSqlQuery query(m_database);
            if (!query.exec("CREATE TABLE IF NOT EXISTS Users (id INTEGER PRIMARY KEY, username TEXT, password TEXT, email TEXT)")) {
                qDebug() << "Failed to create table:" << query.lastError();
            }
        }
        if (!m_database.isOpen())
        {
            qDebug() << "Ошибка соединения с базой данных";
            return false;
        }

        return true;
    }

    void Database::disconnect()
    {
        // Отключаемся от базы данных
        m_database.close();
    }



