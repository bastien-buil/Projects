package fr.bastien.rssdisplay.database;

import android.graphics.Bitmap;

import androidx.annotation.NonNull;
import androidx.room.ColumnInfo;
import androidx.room.Entity;
import androidx.room.Ignore;
import androidx.room.PrimaryKey;
import androidx.room.TypeConverters;

import fr.bastien.rssdisplay.model.Post;
import fr.bastien.rssdisplay.model.PostType;

@Entity(tableName = "posts", primaryKeys = {"uid", "postType"})
@TypeConverters({Converters.class})
public class PostDB implements Post {

    private int  uid;
    @NonNull
    private PostType postType;
    @ColumnInfo(typeAffinity = ColumnInfo.BLOB)
    private Bitmap image;
    private String imageURL;
    private String title;
    private String description;

    private String date;

    public PostDB() {
        this.uid = -1;
        this.image = null;
        this.imageURL = null;
        this.title = null;
        this.description = null;
        this.date = null;
        this.postType = PostType.ACTUALITY;
    }

    @Ignore
    public PostDB(int uid) {
        this.uid = uid;
        this.image = null;
        this.imageURL = null;
        this.title = null;
        this.description = null;
        this.date = null;
        this.postType = PostType.ACTUALITY;
    }


    @Ignore
    public PostDB(int uid, Bitmap image, String imageURL, String title, String description, String date, @NonNull PostType postType) {
        this.uid = uid;
        this.image = image;
        this.imageURL = imageURL;
        this.title = title;
        this.description = description;
        this.date = date;
        this.postType = postType;
    }

    public Bitmap getImage() {
        return image;
    }

    public void setImage(Bitmap image) {
        this.image = image;
    }

    public String getImageURL() {
        return imageURL;
    }

    public void setImageURL(String imageURL) {
        this.imageURL = imageURL;
    }

    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        this.title = title;
    }

    public void setUid(int uid) {
        this.uid = uid;
    }

    public String getDescription() {
        return description;
    }

    public void setDescription(String description) {
        this.description = description;
    }

    public String getDate() {
        return date;
    }

    public void setDate(String date) {
        this.date = date;
    }

    public int getUid() {
        return uid;
    }

    @NonNull
    public PostType getPostType() {
        return postType;
    }

    public void setPostType(@NonNull PostType postType) {
        this.postType = postType;
    }
}
