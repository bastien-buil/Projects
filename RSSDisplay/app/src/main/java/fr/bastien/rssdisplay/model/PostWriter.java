package fr.bastien.rssdisplay.model;

import android.graphics.Bitmap;

public interface PostWriter {
    void clearPosts(PostType postType);
    void clearAllPosts();
    void addPost(int uid, Bitmap image, String imageURL, String title, String description, String date, PostType postType);
}
