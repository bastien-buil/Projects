import TwitchTokenManager from "./twitchTokenManager.js";
import Downloader from "./Downloader.js";
import Stream from "./Stream.js";


export default class StreamAlert{

    constructor() {
        this.MAX_STR_SAVED_SIZE = 6000;

        this.list = []; // [new Stream()]
                        // see StreamType

        this.sensitiveStreamSource = false;

        this.canReload = true;
        this.saveStream = {name: "", webLink:"", type: 0, info:""};

        this.logger = new Logger();

        this.youtubeApiKey = "";
        this.twitchTokenManager = new TwitchTokenManager("", this.logger);


        this.apiKeys = {youtubeApiKey: this.youtubeApiKey, twitchTokenManager: this.twitchTokenManager};

    }

    updateTokens(){
        this.twitchTokenManager.getToken(()=>{}, true);
    }

    loadConfig(){
        browser.storage.sync.get(["streamsList", "streamsList1", "sensitiveStreamSource", "youtubeApiKey"]).then((obj) => {
            if(typeof obj["sensitiveStreamSource"] != "undefined"){
                this.sensitiveStreamSource = obj["sensitiveStreamSource"];
            }
            if(typeof obj["youtubeApiKey"] != "undefined"){
                this.youtubeApiKey = obj["youtubeApiKey"];
            }
            if(typeof obj["streamsList1"] != 'undefined'){
                this.getStreamList(1, (str) =>{
                    this.setStreamsList(JSON.parse(str));
                    this.checkOnline();
                });

            }else {
                if(typeof obj["streamsList"] != "undefined"){
                    this.setStreamsList(obj["streamsList"]);
                    this.checkOnline();
                }
            }
        });
    }

    getStreamList(i, callback){
        browser.storage.sync.get(["streamsList"+i]).then((obj) => {
            if(typeof obj["streamsList" + i] == 'string') {
                this.getStreamList(i+1, (str)=>{
                    callback(obj["streamsList" + i] + str);
                });
            }else{
                callback("");
            }
        });
    }

    setStreamsList(streams) {
        this.list = streams;
        if(this.list.length > 0 && this.list[0] instanceof Stream){
            this.list = Stream.listOfObjectToListOfClass(this.list);
        }else if(this.list.length > 0){
            for (let stream of this.list) {
                stream.online = false;
            }
        }
    }

    getStreamsSavedList(){
        let saveList = JSON.parse(JSON.stringify(this.list));
        for(let stream of saveList){
            delete stream.slowUpdate;//It's ignoring if not exist
            delete stream.online;
        }
        return saveList;
    }

    saveConfig(){
        browser.storage.sync.set({youtubeApiKey: this.youtubeApiKey, sensitiveStreamSource: this.sensitiveStreamSource});
        this.saveStreamList();
    }

    saveStreamList(){
        let str = JSON.stringify(this.getStreamsSavedList());
        let i = 1;
        while(str.slice(0, this.MAX_STR_SAVED_SIZE) !== ""){
            browser.storage.sync.set({['streamsList' + i]: str.slice(0, this.MAX_STR_SAVED_SIZE)});
            str = str.slice(this.MAX_STR_SAVED_SIZE);
            i++;
        }
    }

    checkOnline() {
        if(this.canReload){
            if(this.list.length > 0){
                this.canReload = false;
                new Downloader(this.list, this.apiKeys, this.logger, function (){
                    this.canReload = true;
                    if(browser.extension.getViews({ type: "popup" }).length > 0)	//Check if popup is opened
                        browser.runtime.sendMessage({"loadList": this.list});
                }).execute();
            }else{
                browser.browserAction.setBadgeText({text:"0"});
                browser.browserAction.setBadgeBackgroundColor({color:"gray"});
            }
        }

    }
}