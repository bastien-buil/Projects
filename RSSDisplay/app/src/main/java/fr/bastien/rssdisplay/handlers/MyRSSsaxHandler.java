package fr.bastien.rssdisplay.handlers;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.util.Log;

import org.xml.sax.Attributes;
import org.xml.sax.InputSource;
import org.xml.sax.XMLReader;
import org.xml.sax.helpers.DefaultHandler;

import java.io.IOException;
import java.io.InputStream;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.ArrayList;
import java.util.List;

import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;

import fr.bastien.rssdisplay.model.Post;
import fr.bastien.rssdisplay.model.PostType;
import fr.bastien.rssdisplay.model.PostWriter;

public class MyRSSsaxHandler extends DefaultHandler {

    private final List<HandlerPostEvent> postListeners = new ArrayList<>();

    protected void callPostListener(){
        for(HandlerPostEvent listener : postListeners){
            listener.onNewPost(currentIterItem);
        }
    }

    public void addMoveListener(HandlerPostEvent listener) {
        postListeners.add(listener);
    }


    private String url = null;
    private PostWriter postWriter = null;
    private PostType postType = null;

    private String imageURL = null;
    private StringBuilder title = new StringBuilder() ;
    private StringBuilder description = new StringBuilder() ;
    private String date;

    private int currentIterItem = -1;

    public void setUrl(String url){
        this.url = url;
    }

    public void setPostType(PostType postType){
        this.postType = postType;
    }

    public void setPostWriter(PostWriter postWriter){
        this.postWriter = postWriter;
    }

    private Bitmap getBitmap(String imageURL){
        URL url;
        try {
            url = new URL(imageURL);
            return BitmapFactory.decodeStream(url.openConnection().getInputStream());
        } catch (MalformedURLException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
        return null;
    }


    private enum BaliseType {NONE, TITLE, DESCRIPTION, PUBDATE, ENCLOSURE}
    private BaliseType baliseType = BaliseType.NONE;

    @Override
    public void startElement(String uri, String localName, String qName, Attributes attributes) {
        switch (qName) {
            case "title":

                Log.println(Log.INFO, "B", "Title");
                baliseType = BaliseType.TITLE;
                break;
            case "pubDate":
                baliseType = BaliseType.PUBDATE;
                break;
            case "description":
                baliseType = BaliseType.DESCRIPTION;
                break;
            case "media:content":
                baliseType = BaliseType.ENCLOSURE;
                imageURL = attributes.getValue("url");

                break;
            case "item":

                Log.println(Log.INFO, "B", "new item");
                currentIterItem++;
                break;
            default:
                baliseType = BaliseType.NONE;
                break;
        }
    }
    @Override
    public void endElement(String uri, String localName, String qName) {
        if(qName.equals("item")) {
            Log.println(Log.INFO, "B", "Insert image");
            postWriter.addPost(currentIterItem, getBitmap(imageURL), imageURL, title.toString(), description.toString(), date, postType);
            Log.println(Log.INFO, "B", "end image");
            callPostListener();
            imageURL = "";
            title = new StringBuilder();
            description = new StringBuilder();
            date = "";

        }
    }

    public void characters(char[] ch, int start, int length){
        String substring = new String(ch).substring(start, start + length);
        switch (baliseType){
            case ENCLOSURE:
            case NONE:
                break;
            case TITLE:
                title.append(substring);
                break;
            case PUBDATE:
                date = substring;
                break;
            case DESCRIPTION:
                description.append(substring);
                break;
        }

    }


    public void processFeed(){
        postWriter.clearPosts(postType);
        try{
            SAXParserFactory factory = SAXParserFactory.newInstance();
            SAXParser parser = factory.newSAXParser();
            XMLReader reader = parser.getXMLReader();
            reader.setContentHandler(this);
            InputStream inputStream = new URL(url).openStream();
            reader.parse(new InputSource(inputStream));
        }catch (Exception e){
            Log.e("rssException", "processFeed Exception " + e.getMessage());
        }
    }
}
