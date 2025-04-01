import sqlite3

def count_all_tracks(conn):
    """Count and display total number of tracks"""
    cursor = conn.cursor()
    cursor.execute("SELECT COUNT(*) FROM track;")
    total = cursor.fetchone()[0]
    print(f"\nTotal tracks in database: {total}\n")

def show_shortest_tracks(conn):
    """Display shortest tracks with artist/album info"""
    cursor = conn.cursor()
    query = """
        SELECT t.Name AS Track, a.Title AS Album, ar.Name AS Artist,
               ROUND(t.Milliseconds/1000.0, 2) AS Duration_sec
        FROM track t
        JOIN album a ON t.AlbumId = a.AlbumId
        JOIN artist ar ON a.ArtistId = ar.ArtistId
        ORDER BY t.Milliseconds ASC
        LIMIT 10;
    """
    cursor.execute(query)
    
    print("\n{10 Shortest Tracks:")
    print("-" * 80)
    print("{:<30} | {:<25} | {:<20} | {}".format(
        "Track", "Album", "Artist", "Duration (sec)"))
    print("-" * 80)
    
    for row in cursor:
        print("{:<30} | {:<25} | {:<20} | {}".format(*row))

def main():
    with sqlite3.connect("../chinook.db") as conn:
        while True:
            print("\nChinook Database System")
            print("1. Count all tracks")
            print("2. Show shortest tracks")
            print("3. Exit")
            
            choice = input("Enter your choice (1-3): ")
            
            if choice == "1":
                count_all_tracks(conn)
            elif choice == "2":
                show_shortest_tracks(conn)
            elif choice == "3":
                print("Goodbye!")
                break
            else:
                print("Invalid choice. Please try again.")

if __name__ == "__main__":
    main()