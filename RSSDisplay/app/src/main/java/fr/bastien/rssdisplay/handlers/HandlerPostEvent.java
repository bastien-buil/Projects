package fr.bastien.rssdisplay.handlers;

import java.util.EventListener;

public interface HandlerPostEvent extends EventListener {
    void onNewPost(int postCount);
}
