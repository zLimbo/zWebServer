#define FMT_HEADER_ONLY
#include <assert.h>
#include <fmt/printf.h>
#include <mysql/mysql.h>

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

int main() {
    mysql_library_init(0, nullptr, nullptr);
    MYSQL *conn = mysql_init(NULL);
    assert(conn);

    mysql_real_connect(conn, "127.0.0.1", "root", "admin", "test", 3306,
                       nullptr, 0);
    assert(conn);

    fmt::print("mysql connect success.\n");

    for (;;) {
        std::string stmt;
        fmt::print("sql > ");
        std::getline(std::cin, stmt);

        fmt::print("stmt: {}\n", stmt);
        if (stmt == "exit") {
            break;
        }

        int ret = mysql_query(conn, stmt.c_str());
        assert(!ret);

        if (MYSQL_RES *mysqlResult = mysql_store_result(conn)) {
            std::vector<std::string> fields;
            while (MYSQL_FIELD *mysqlField = mysql_fetch_field(mysqlResult)) {
                fields.emplace_back(mysqlField->name);
            }
            std::vector<std::vector<std::string>> rows;
            while (MYSQL_ROW mysqlRow = mysql_fetch_row(mysqlResult)) {
                rows.emplace_back();
                auto &row = rows.back();
                for (size_t i = 0; i < fields.size(); ++i) {
                    row.emplace_back(mysqlRow[i]);
                }
            }

            fmt::print("========\n");
            for (std::string &field : fields) {
                fmt::print("{}\t", field);
            }
            fmt::print("\n");
            for (auto &row : rows) {
                for (auto &word : row) {
                    fmt::print("{}\t", word);
                }
                fmt::print("\n");
            }
            fmt::print("========\n");
        }
    }

    mysql_close(conn);

    return 0;
}