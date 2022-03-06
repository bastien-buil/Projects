package fr.bastien.rssdisplay.model;

import android.graphics.Bitmap;



public interface Post {
    Bitmap getImage();
    void setImage(Bitmap image);
    String getImageURL();
    void setImageURL(String imageURL);
    String getTitle();
    void setTitle(String title);
    void setUid(int uid);
    String getDescription();
    void setDescription(String description);
    void setPostType(PostType type);
    PostType getPostType();
    String getDate();
    public void setDate(String date);
    public int getUid();
}
