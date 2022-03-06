package fr.bastien.rssdisplay.task;

import android.os.AsyncTask;
import android.util.Log;

import fr.bastien.rssdisplay.MainActivity;
import fr.bastien.rssdisplay.handlers.HandlerPostEvent;
import fr.bastien.rssdisplay.handlers.MyRSSsaxHandler;

public class DownloadRssTask extends AsyncTask<MyRSSsaxHandler, Void, MyRSSsaxHandler> implements HandlerPostEvent {
    private final DownloadRssTaskEvent downloadRssTaskEvent;

    public DownloadRssTask(DownloadRssTaskEvent downloadRssTaskEvent){
        this.downloadRssTaskEvent = downloadRssTaskEvent;
    }
    protected MyRSSsaxHandler doInBackground(MyRSSsaxHandler... handler){
        handler[0].addMoveListener(this);
        handler[0].processFeed();
        return handler[0];
    }

    /** The system calls this to perform work in the UI thread and delivers
     * the result from doInBackground() */
    protected void onPostExecute(MyRSSsaxHandler handler)
    {
        downloadRssTaskEvent.onPostExecute(handler);
    }

    @Override
    public void onNewPost(int postCount) {
        downloadRssTaskEvent.onNewPost(postCount);
    }




}