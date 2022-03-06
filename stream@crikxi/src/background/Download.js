import StreamType from "./StreamType.js";


export default class Download{
    constructor(stream, apiKeys, logger){
        this.stream = stream;
        this.apiKeys = apiKeys;
        this.logger = logger;
    }
    execute(downloader) {
        if(this.stream.type === StreamType.DAILYMOTION){
            const dailyRequest = new XMLHttpRequest();
            dailyRequest.open("GET", "https://api.dailymotion.com/video/" + this.stream.info + "?fields=onair", true);
            dailyRequest.onreadystatechange  = function (stream, downloader){
                return function(){
                    if(dailyRequest.readyState === 4){
                        try	{
                            const isOnline = JSON.parse(dailyRequest.responseText)['onair'];
                            if(isOnline) {
                                if(!stream.online)
                                    createNotification('Live !!!', stream.name + ' a démaré son live.');
                                stream.online=true;
                                downloader.downloadFinish(true);
                            }else {
                                stream.online=false;
                                downloader.downloadFinish(false);
                            }
                        } catch(e){
                            this.logger.log(e);
                            stream.online=false;
                            downloader.downloadFinish(false);
                        }
                    }
                }
            }(this.stream, downloader);
            dailyRequest.send();
        } else if(this.stream.type === StreamType.TWITCH){
            downloader.useDownloader("twitch", this.stream);
        }else if(this.stream.type === StreamType.SMASHCAST){
            const smashcastRequest = new XMLHttpRequest();
            smashcastRequest.open("GET", "https://api.smashcast.tv/media/status/" + this.stream.info, true);
            smashcastRequest.onreadystatechange  = function (stream){
                return function(){
                    if(smashcastRequest.readyState === 4){
                        try	{
                            const responseObj = JSON.parse(smashcastRequest.responseText);
                            if(responseObj["media_is_live"] && parseInt(responseObj["media_is_live"]) === 1) {
                                if(!stream.online)
                                    createNotification('Live !!!', stream.name + ' a démaré son live.');
                                stream.online=true;
                                downloader.downloadFinish(true);
                            }else {
                                stream.online=false;
                                downloader.downloadFinish(false);
                            }
                        } catch(e){
                            this.logger.log(e);
                            stream.online=false;
                            downloader.downloadFinish(false);
                        }
                    }
                }
            }(this.stream);
            smashcastRequest.send();
        }else if(this.stream.type === StreamType.YOUTUBE){ // Disabled if no api key is set
            if(this.apiKeys.youtubeApiKey){
                const youtubeRequest = new XMLHttpRequest();
                youtubeRequest.open("GET", "https://www.googleapis.com/youtube/v3/search?part=snippet&channelId=" + this.stream.info + "&eventType=live&type=video&key=" + this.apiKeys.youtubeApiKey, true);
                youtubeRequest.onreadystatechange  = function (stream){
                    return function(){
                        if(youtubeRequest.readyState === 4){
                            try	{
                                const streams = JSON.parse(youtubeRequest.responseText)['items'];
                                if(streams[0] != null) {
                                    if(!stream.online)
                                        createNotification('Live !!!', stream.name + ' a démaré son live.');
                                    stream.online=true;
                                    downloader.downloadFinish(true);
                                }else {
                                    stream.online=false;
                                    downloader.downloadFinish(false);
                                }
                            } catch(e){

                                this.logger.log("Youtube: ");
                                this.logger.log(e);
                                this.logger.log(youtubeRequest.responseText);
                                stream.online=false;
                                downloader.downloadFinish(false);
                            }
                        }
                    }
                }(this.stream);
                youtubeRequest.send();
            }else{
                this.stream.online=false;
                downloader.downloadFinish(false);
            }
        }else if(this.stream.type === StreamType.LEVELDOWN){
            const leveldownRequest = new XMLHttpRequest();
            // noinspection HttpUrlsUsage
            leveldownRequest.open("GET", "http://leveldown.fr/api/getStreamer", true);
            leveldownRequest.onreadystatechange = function (stream){
                return function(){
                    if(leveldownRequest.readyState === 4){
                        this.logger.log("Leveldown is down but you use leveldown with: " + stream.info)
                        try	{
                            if(JSON.parse(leveldownRequest.responseText)['statut'] === "success"){
                                const ldStreams = JSON.parse(leveldownRequest.responseText)['response'];
                                let online = false;
                                for(let ldStream of ldStreams){
                                    if(ldStream.slug === stream.info){
                                        if(ldStream["live"] === true){
                                            online = true;
                                            break;
                                        }
                                    }
                                }
                                if(online) {
                                    if(!stream.online)
                                        createNotification('Live !!!', stream.name + ' a démaré son live.');
                                    stream.online=true;
                                    downloader.downloadFinish(true);
                                }
                                else{
                                    stream.online=false;
                                    downloader.downloadFinish(false);
                                }
                            }else{
                                stream.online=false;
                                downloader.downloadFinish(false);

                            }
                        } catch(e){
                            this.logger.log(e);
                            stream.online=false;
                            downloader.downloadFinish(false);
                        }
                    }
                }
            }(this.stream);
            leveldownRequest.send();
        }else if(this.stream.type === StreamType.CHATURBATE){
            if(typeof this.stream.slowUpdate == "undefined")
                this.stream.slowUpdate = 0;
            if(this.stream.slowUpdate <= 0){
                const chaturbateRequest = new XMLHttpRequest();
                chaturbateRequest.open("GET", "https://en.chaturbate.com/" + this.stream.info + "/", true);
                chaturbateRequest.onreadystatechange = function (stream){
                    return function(){
                        if(chaturbateRequest.readyState === 4){
                            if(chaturbateRequest.responseText.search("playlist.m3u8") !== -1){
                                stream.online=true;
                                downloader.downloadFinish(true);
                            }else {
                                stream.online=false;
                                downloader.downloadFinish(false);
                            }
                        }
                    }
                }(this.stream);
                chaturbateRequest.send();
                this.stream.slowUpdate = 8 + Math.floor(Math.random() * Math.floor(16));
            }else{
                this.stream.slowUpdate--;
                downloader.downloadFinish(this.stream.online);
            }
        }else if(this.stream.type === StreamType.CAMMODEL){
            if(typeof this.stream.slowUpdate == "undefined")
                this.stream.slowUpdate = 0;
            if(this.stream.slowUpdate <= 0){
                const cammodelRequest = new XMLHttpRequest();
                cammodelRequest.open("GET", "https://sea1b-ls.naiadsystems.com/sea1b-edge-ls/80/live/s:" + this.stream.info + ".json?last=load&format=mp4-hls", true);
                cammodelRequest.onreadystatechange = function (stream){
                    return function(){
                        if(cammodelRequest.readyState === 4){
                            if (this.status === 200) {
                                stream.online=true;
                                downloader.downloadFinish(true);
                            }else {
                                stream.online=false;
                                downloader.downloadFinish(false);
                            }
                        }
                    }
                }(this.stream);
                cammodelRequest.send();
                this.stream.slowUpdate = 8 + Math.floor(Math.random() * Math.floor(16));
            }else{
                this.stream.slowUpdate--;
                downloader.downloadFinish(this.stream.online);
            }
        }else if(this.stream.type === StreamType.MYFREECAMS){
            if(typeof this.stream.slowUpdate == "undefined")
                this.stream.slowUpdate = 0;
            if(this.stream.slowUpdate <= 0){
                const myfreecamsRequest = new XMLHttpRequest();
                myfreecamsRequest.open("GET", "https://profiles.myfreecams.com/" + this.stream.info, true);
                myfreecamsRequest.onreadystatechange = function (stream){
                    return function(){
                        if(myfreecamsRequest.readyState === 4){
                            const regex = /profileState: {"number":(.*),"string":"(.*)"}/g;
                            const res = regex.exec(myfreecamsRequest.responseText);
                            if (res && res[2] && res[2] === "Online") {
                                stream.online=true;
                                downloader.downloadFinish(true);
                            }else {
                                stream.online=false;
                                downloader.downloadFinish(false);
                            }
                        }
                    }
                }(this.stream);
                myfreecamsRequest.send();
                this.stream.slowUpdate = 8 + Math.floor(Math.random() * Math.floor(16));
            }else{
                this.stream.slowUpdate--;
                downloader.downloadFinish(this.stream.online);
            }
        }else{
            this.stream.online=false;
            downloader.downloadFinish(false);
        }



    }

}


function createNotification(title, message, thisImage) {
    if(typeof thisImage != "undefined"){
        browser.notifications.create(
            {type: "basic",title: title, message: message, iconUrl: thisImage}
        );
    }
    else
        browser.notifications.create(
            {type: "basic",title: title, message: message, iconUrl: browser.extension.getURL("data/buttonTools.png")}
        );
}