package fr.bastien.rssdisplay;

import androidx.appcompat.app.AppCompatActivity;
import androidx.room.Room;

import android.content.Context;
import android.graphics.Bitmap;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.AsyncTask;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import fr.bastien.rssdisplay.database.AppDatabase;
import fr.bastien.rssdisplay.database.PostDB;
import fr.bastien.rssdisplay.database.PostWriterDB;
import fr.bastien.rssdisplay.handlers.MyRSSsaxHandler;
import fr.bastien.rssdisplay.model.PostType;
import fr.bastien.rssdisplay.task.DownloadRssTask;
import fr.bastien.rssdisplay.task.DownloadRssTaskEvent;

public class MainActivity extends AppCompatActivity {


    private AppDatabase db;
    public final String ACTUALITYLINK = "https://www.lemonde.fr/international/rss_full.xml";
    public final String SPORTLINK = "https://www.lemonde.fr/sport/rss_full.xml";
    public final String ECONOMYLINK = "https://www.lemonde.fr/economie/rss_full.xml";
    private PostType postType = PostType.ACTUALITY;
    private int currentNews = 0;
    private int maxButtonValue = 0;
    private AsyncTask<MyRSSsaxHandler, Void, MyRSSsaxHandler> task;

    public void setMaxButtonValue(int maxButtonValue) {
        this.maxButtonValue = maxButtonValue;
    }

    public void updateMaxButtonValue(){
        maxButtonValue = db.postDao().getPostCount(postType) - 1;
    }

    public void updateButton(){
        runOnUiThread(() ->{
            findViewById(R.id.button).setEnabled(currentNews != 0);
            (findViewById(R.id.button2)).setEnabled(currentNews < maxButtonValue);
        });
    }

    public void nextNews(View v){
        if(currentNews < maxButtonValue){
            currentNews++;
        }
        updateButton();
        loadFromDb(currentNews, postType);
    }

    public void previousNews(View v){
        if(currentNews > 0){
            currentNews--;
        }
        updateButton();
        loadFromDb(currentNews, postType);
    }

    public void resetDisplay(String title, String date, Bitmap image, String description){
        ((TextView)findViewById(R.id.imageTitle)).setText(title);
        ((TextView)findViewById(R.id.imageDate)).setText(date);
        ((TextView)findViewById(R.id.imageDescription)).setText(description);
        ((ImageView)findViewById(R.id.imageDisplay)).setImageBitmap(image);
    }

    public void resetDisplay(PostDB post){
        resetDisplay(post.getTitle(), post.getDate(), post.getImage(), post.getDescription());
    }

    String getLinkByPostType(PostType postType){
        switch (postType){
            default:
            case ACTUALITY:
                return ACTUALITYLINK;
            case SPORT:
                return SPORTLINK;
            case ECONOMY:
                return ECONOMYLINK;
        }
    }

    public void loadToDb(PostType type){
        if(task != null && task.getStatus() == AsyncTask.Status.RUNNING){
            Toast.makeText(this, "Posts are already loading", Toast.LENGTH_LONG).show();
        } else{
            MyRSSsaxHandler handler = new MyRSSsaxHandler();
            handler.setUrl(getLinkByPostType(postType));
            handler.setPostWriter(new PostWriterDB(db));
            handler.setPostType(postType);
            /*

             */
            task = new DownloadRssTask(new DownloadRssTaskEvent() {
                @Override
                public void onPostExecute(MyRSSsaxHandler handler) {
                }

                @Override
                public void onNewPost(int postCount) {
                    if(postCount == 0){
                        loadFromDb(0, type);
                    }
                    setMaxButtonValue(postCount);
                    updateButton();
                }
            }).execute(handler);
        }
    }


    public void loadFromDb(int number, PostType type){
        runOnUiThread(() -> {
            updateButton();
            PostDB post = db.postDao().findById(number, type);
            if(post != null){
                resetDisplay(post);
            }else{
                Toast.makeText(this, "Post non préchargé et pas de connexion à internet", Toast.LENGTH_LONG).show();
            }
        });
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.main_menu, menu);
        return true;
    }

    public void setPostType(PostType type){
        postType = type;
        currentNews = 0;
        updateButton();
        loadNews();
    }


    @Override
    public boolean onOptionsItemSelected(MenuItem item)
    {

        item.setChecked(true);
        if (item.getItemId() == R.id.actualityButton) {
            setPostType(PostType.ACTUALITY);
            return true;
        }else if (item.getItemId() == R.id.sportButton) {
            setPostType(PostType.SPORT);
            return true;

        }else if (item.getItemId() == R.id.economyButton) {
            setPostType(PostType.ECONOMY);
            return true;
        }else if (item.getItemId() == R.id.reloadNews) {
            if(isNetworkAvailable()){
                currentNews = 0;
                loadToDb(postType);
            }else{
                Toast.makeText(this, "Connexion impossible à internet", Toast.LENGTH_LONG).show();
            }
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    private void loadNews(){
        if(isNetworkAvailable()){
            loadToDb(postType);
        }else{
            loadFromDb(0, postType);
            updateMaxButtonValue();
            updateButton();
        }
    }

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        updateButton();
        db = Room.databaseBuilder(getApplicationContext(), AppDatabase.class, "postDb").allowMainThreadQueries().build();
        loadNews();

    }

    private boolean isNetworkAvailable() {
        ConnectivityManager connectivityManager
                = (ConnectivityManager) getSystemService(Context.CONNECTIVITY_SERVICE);
        NetworkInfo activeNetworkInfo = connectivityManager.getActiveNetworkInfo();
        return activeNetworkInfo != null && activeNetworkInfo.isConnected();
    }
}