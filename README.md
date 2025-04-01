# SQLITE3 in Python and C

Databases are useful on their own, but we generally want to interface them with a programming language to provide some sort of front end. We can use either C or Python to do this (or basically any other programming language, but we're just looking at the 2 you learned this year!) and examples of how to do this for each are provided in their respective folders.

# Using C

In C, we use a library 'sqlite3' which is already installed on Codespace - if you look at the makefile, you can see we also need to **link** this as it's an external library (like using math.h).

## Key Concepts

### 1. Database Connection Lifecycle
```c
sqlite3 *db;
int rc = sqlite3_open("../chinook.db", &db);  // Open

if (rc != SQLITE_OK) {
    fprintf(stderr, "Error: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return 1;
}

sqlite3_close(db);  // Always close at the end!
```
- **Critical Checks**: Always verify `SQLITE_OK` return codes
- **Error Handling**: Use `sqlite3_errmsg()` for diagnostics

### 2. Query Execution Methods

#### Callback Style (`sqlite3_exec`)
```c
static int callback(void *data, int argc, char **argv, char **col_name) {
    for (int i = 0; i < argc; i++) {
        printf("%s: %s\n", col_name[i], argv[i] ? argv[i] : "NULL");
    }
    printf("---\n");
    return 0;
}

const char *sql = 
        "SELECT t.Name AS Track, a.Title AS Album, ar.Name AS Artist, t.Milliseconds "
        "FROM Track t "
        "JOIN Album a ON t.AlbumId = a.AlbumId "
        "JOIN Artist ar ON a.ArtistId = ar.ArtistId "
        "ORDER BY t.Milliseconds ASC "
        "LIMIT 10;";

sqlite3_exec(db, sql, callback, NULL, &err_msg);
```

This defines how each row is processed - the function must always have the exact parameters shown! `void* data` is used when you want to do something with the data, e.g. return a count, or put it in a struct.

### 3. Data Passing Techniques

#### Using `void *data` Parameter
```c
static int count_rows(void *data, int argc, char **argv, char **col_names) {
    int *count = (int *)data;  // Cast void* back to int*
    (*count)++;                // Increment the counter
    return 0;                  // Return 0 to continue
}

int total_rows = 0;  // Variable to hold the count

// Execute query and pass the counter's address as void* data
char* query2 = "SELECT * FROM Track;";
if (sqlite3_exec(db, query2, count_rows, &total_rows, &err_msg) != SQLITE_OK) {
    fprintf(stderr, "SQL error: %s\n", err_msg);
    sqlite3_free(err_msg);
}

    printf("Total tracks: %d\n", total_rows);
```

The `void *data` can be cast to any other type, which lets us do things like the above where we count rows.

### 4. Useful Code Snippets

#### Error Handling Template
```c
if (rc != SQLITE_OK) {
    fprintf(stderr, "SQL Error: %s\n", err_msg);
    sqlite3_free(err_msg);  // Must free error messages!
    sqlite3_close(db);
    return 1;
}
```

You should always check and handle errors!

# Using Python

Similarly to C, we use a library in Python too - `import sqlite3`.

## Key Concepts

### 1. Connection Management
```python
import sqlite3
with sqlite3.connect("../chinook.db") as conn:
    # Database operations here
```
- `with` statement automatically closes the connection
- Creates/connects to `chinook.db` file (in this case, it's 1 folder back)

### 2. Executing Queries
Basic query execution:
```python
cursor = conn.cursor()
cursor.execute("SELECT COUNT(*) FROM track;")
```

Parameterised queries (slightly safer):
```python
cursor.execute("SELECT * FROM track WHERE Milliseconds < ?", (300000,))
```

### 3. Fetching Results
| Method          | Description                          | Example Use Case              |
|-----------------|--------------------------------------|-------------------------------|
| `fetchone()`    | Gets single row                      | Counting records              |
| `fetchall()`    | Gets all rows as list                | Small result sets             |
| Iterate cursor  | Processes rows one-by-one (memory efficient) | Large datasets |

### 4. Table Joins
The example demonstrates a 3-table join:
```sql
query = """
SELECT t.Name, a.Title, ar.Name 
FROM track t
JOIN album a ON t.AlbumId = a.AlbumId
JOIN artist ar ON a.ArtistId = ar.ArtistId;
"""
```

### 5. Practical Patterns

**Counting records:**
```python
cursor.execute("SELECT COUNT(*) FROM track;")
count = cursor.fetchone()[0]  # Returns single value
```

**Formatting output:**
```python
print(f"{'Track':<30} | {'Artist':<20}")
for row in cursor:
    print(f"{row[0]:<30} | {row[2]:<20}")
```
This adds spaces which can be nice to keep things looking neater! You can also use the zfill() function to do this.

## Complete Workflow

1. **Connect** to database
2. **Create cursor** for executing queries
3. **Execute SQL** statements
4. **Process results** (count, display, etc.)
5. **Close connection** (automatic with `with`)