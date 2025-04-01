#include <stdio.h>
#include <sqlite3.h>

// Callback function to print query results
static int callback(void *data, int argc, char **argv, char **col_name) {
    for (int i = 0; i < argc; i++) {
        printf("%s: %s\n", col_name[i], argv[i] ? argv[i] : "NULL");
    }
    printf("---\n");
    return 0;
}

static int count_rows(void *data, int argc, char **argv, char **col_names) {
    int *count = (int *)data;  // Cast void* back to int*
    (*count)++;                // Increment the counter
    return 0;                  // Return 0 to continue
}

int main() {
    sqlite3 *db;
    char *err_msg = 0;

    // Open the Chinook database
    int rc = sqlite3_open("../chinook.db", &db);
    if (rc != SQLITE_OK) {
        printf("Cannot open database: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    // SQL query: Get the 10 shortest tracks with artist/album info
    const char *sql = 
        "SELECT t.Name AS Track, a.Title AS Album, ar.Name AS Artist, t.Milliseconds "
        "FROM Track t "
        "JOIN Album a ON t.AlbumId = a.AlbumId "
        "JOIN Artist ar ON a.ArtistId = ar.ArtistId "
        "ORDER BY t.Milliseconds ASC "
        "LIMIT 10;";

    // Execute the query
    rc = sqlite3_exec(db, sql, callback, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    }

    int total_rows = 0;  // Variable to hold the count

    // Execute query and pass the counter's address as void* data
    char* query2 = "SELECT * FROM Track;";
    if (sqlite3_exec(db, query2, count_rows, &total_rows, &err_msg) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    }

    printf("Total tracks: %d\n", total_rows);  // Print the result

    // Close the database
    sqlite3_close(db);
    return 0;
}