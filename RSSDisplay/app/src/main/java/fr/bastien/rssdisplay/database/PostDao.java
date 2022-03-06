package fr.bastien.rssdisplay.database;

import androidx.room.Dao;
import androidx.room.Delete;
import androidx.room.Insert;
import androidx.room.Query;

import java.util.List;

import fr.bastien.rssdisplay.model.PostType;

@Dao
public interface PostDao {
    @Query("SELECT * FROM posts")
    List<PostDB> getAll();

    @Query("SELECT * FROM posts WHERE uid = :userId AND postType = :postType LIMIT 1")
    PostDB findById(int userId, PostType postType);



    @Query("SELECT COUNT(*) FROM posts")
    int getAllPostCount();

    @Query("SELECT COUNT(*) FROM posts WHERE postType = :postType")
    int getPostCount(PostType postType);

    @Query("DELETE FROM posts")
    void clearDB();


    @Query("DELETE FROM posts WHERE postType = :postType")
    void clearDBByPostType(PostType postType);

    @Insert
    void insertAll(PostDB... posts);

    @Delete
    void delete(PostDB post);

}
