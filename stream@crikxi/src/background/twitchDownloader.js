export default class TwitchDownloader {
    constructor(downloader) {
        this.streamList = [];
        this.downloader = downloader;
        this.maxStreamer = 90;
    }

    addStream(stream) {
        this.streamList.push(stream);
        if (this.streamList.length <= this.maxStreamer) {
            return this;
        } else {
            this.execute();
            return new TwitchDownloader(this.downloader);
        }
    }

    execute() {
        if (this.streamList.length > 0) {
            const streamsByName = {};
            for (let stream of this.streamList) {
                if (!streamsByName[stream.info.toLowerCase()]) {
                    streamsByName[stream.info.toLowerCase()] = [];
                }
                streamsByName[stream.info.toLowerCase()].push(stream);
            }
            this.downloader.apiKeys.twitchTokenManager.getToken((token) => {
                if (token) {
                    const twitchUserRequest = new XMLHttpRequest();
                    twitchUserRequest.open("GET", "https://api.twitch.tv/helix/users?" + Object.keys(streamsByName).map(stream => "login=" + stream).join("&"), true);
                    twitchUserRequest.setRequestHeader("Client-ID", "n1co3v1ez9k1t6nsjn0qz9ujjlj2pdz");
                    twitchUserRequest.setRequestHeader("Authorization", "Bearer " + token);
                    twitchUserRequest.onreadystatechange = () => {
                        if (twitchUserRequest.readyState === 4) {
                            try {
                                const users = JSON.parse(twitchUserRequest.responseText)['data'];
                                const streamsById = {};
                                for (let stream of users) {
                                    if (streamsByName[stream["login"]]) {
                                        if (!streamsById[stream.id]) {
                                            streamsById[stream.id] = [];
                                        }
                                        streamsById[stream.id] = streamsById[stream.id].concat(streamsByName[stream["login"]]);
                                        delete streamsByName[stream["login"]];
                                    }
                                }
                                for (let name in streamsByName) {
                                    for (let stream of streamsByName[name]) {
                                        this.downloader.logger.log("Unknown name: " + stream.info);
                                        stream.online = false;
                                        this.downloader.downloadFinish(false);
                                    }
                                }
                                this.downloader.logger.log((Object.keys(streamsByName).map(stream => "login=" + stream).join("&")));
                                const twitchRequest = new XMLHttpRequest();
                                twitchRequest.open("GET", "https://api.twitch.tv/helix/streams?" + Object.keys(streamsById).map(streamId => "user_id=" + streamId).join("&"), true);
                                twitchRequest.setRequestHeader("Client-ID", "n1co3v1ez9k1t6nsjn0qz9ujjlj2pdz");
                                twitchRequest.setRequestHeader("Authorization", "Bearer " + token);
                                twitchRequest.onreadystatechange =  () =>{
                                    if (twitchRequest.readyState === 4) {
                                        try {
                                            const streams = JSON.parse(twitchRequest.responseText)['data'];
                                            for (let stream of streams) {
                                                if (streamsById[stream["user_id"]]) {
                                                    for (let stream2 of streamsById[stream["user_id"]]) {
                                                        stream2.online = true;
                                                        stream2.
                                                        this.downloader.downloadFinish(true);
                                                    }
                                                    delete streamsById[stream["user_id"]];
                                                }
                                            }
                                            for (let id in streamsById) {
                                                for (let stream of streamsById[id]) {
                                                    stream.online = false;
                                                    this.downloader.downloadFinish(false);
                                                }
                                            }
                                        } catch (e) {
                                            this.downloader.logger.log("b");
                                            this.downloader.logger.log(e);
                                            this.downloader.logger.log(twitchRequest.responseText);

                                            for (let id in streamsById) {
                                                for (let stream of streamsById[id]) {
                                                    stream.online = false;
                                                    this.downloader.downloadFinish(false);
                                                }
                                            }
                                        }

                                    }
                                }
                                twitchRequest.send();


                            } catch (e) {//PRévoir en cas d'érreur
                                this.downloader.apiKeys.twitchTokenManager.refreshToken();
                                this.downloader.logger.log("Error A twitch|Can be just a token that need to be refresh");
                                this.downloader.logger.log(e);
                                this.downloader.logger.log(twitchUserRequest.responseText);
                                for (let name in streamsByName) {
                                    for (let stream of streamsByName[name]) {
                                        stream.online = false;
                                        this.downloader.downloadFinish(false);
                                    }
                                }
                            }

                        }
                    }
                    twitchUserRequest.send();
                } else {
                    for (let name in streamsByName) {
                        for (let stream of streamsByName[name]) {
                            stream.online = false;
                            this.downloader.downloadFinish(false);
                        }
                    }
                }
            });
        }
    }
}

