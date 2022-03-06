package fr.bastien.rssdisplay.database;

import android.graphics.Bitmap;

import fr.bastien.rssdisplay.model.PostType;
import fr.bastien.rssdisplay.model.PostWriter;

public class PostWriterDB implements PostWriter {


    AppDatabase db;
    public PostWriterDB(AppDatabase db){
        this.db = db;
    }

    @Override
    public void clearPosts(PostType postType) {
        db.postDao().clearDBByPostType(postType);
    }

    @Override
    public void clearAllPosts() {
        db.postDao().clearDB();
    }


    @Override
    public void addPost(int uid, Bitmap image, String imageURL, String title, String description, String date, PostType postType) {
        db.postDao().insertAll(new PostDB(uid, image, imageURL, title, description, date, postType));
    }
}
