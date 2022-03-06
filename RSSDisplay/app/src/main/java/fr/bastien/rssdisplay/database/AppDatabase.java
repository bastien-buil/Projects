package fr.bastien.rssdisplay.database;

import androidx.room.Database;
import androidx.room.RoomDatabase;

@Database(entities = {PostDB.class}, version = 1)
public abstract class AppDatabase extends RoomDatabase {
    public abstract PostDao postDao();
}
