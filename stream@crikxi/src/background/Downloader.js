import Download from "./Download.js";
import TwitchDownloader from "./twitchDownloader.js";

export default class Downloader{
    constructor(list, apiKeys, logger, callback) {
        this.list = list;
        this.apiKeys = apiKeys;
        this.callback = callback;
        this.logger = logger;
        this.finish = false;
        this.start = false;
        this.max = this.list.length;
        this.count = 0;
        this.online = 0;
        this.downloaders = {};
    }
    execute(){
        if(!this.start){
            this.start = true;
            browser.browserAction.setBadgeBackgroundColor({color:"red"});
            for(const stream of this.list) {
                const download = new Download(stream, this.apiKeys, this.logger);
                download.execute(this);
            }
            this.closeDownloaders();
        }
    }

    downloadFinish(online) {
        if(this.start && !this.finish){
            this.count++;
            if(online)
                this.online++;
            if(this.count === this.max){
                this.finish = true;
                browser.browserAction.setBadgeBackgroundColor({color:"blue"});
                browser.browserAction.setBadgeText({text:this.online+""});
                this.logger.log("Streams Loaded")
                if(this.callback)
                    this.callback();
            }
        }
    }

    useDownloader(name, stream){
        if(!this.downloaders[name]){
            if(name.toLowerCase() === "twitch")
                this.downloaders[name] = new TwitchDownloader(this);
            else
                this.logger.log("Unknown downloader: " + name);
            stream.online=false;
            this.downloadFinish(false);
            return;
        }
        this.downloaders[name] = this.downloaders[name].addStream(stream);
    }

    closeDownloaders(){
        for(let downloader in this.downloaders){
            this.downloaders[downloader].execute();
            delete this.downloaders[downloader];
        }
    }
}
