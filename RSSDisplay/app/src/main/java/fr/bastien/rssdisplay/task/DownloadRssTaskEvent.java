package fr.bastien.rssdisplay.task;

import fr.bastien.rssdisplay.handlers.MyRSSsaxHandler;

public interface DownloadRssTaskEvent {
    void onPostExecute(MyRSSsaxHandler handler);
    void onNewPost(int postCount);
}
