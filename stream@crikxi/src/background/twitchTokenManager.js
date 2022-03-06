export default class TwitchTokenManager {
    constructor(token, logger) {
        this.token = token || "";
        this.logger = logger;
    }

    refreshToken() {
        this.token = null;
    }

    getToken(callback, forceInteractive = false) {
        if (!this.token) {
            browser.identity.launchWebAuthFlow({
                url: "https://id.twitch.tv/oauth2/authorize?client_id=n1co3v1ez9k1t6nsjn0qz9ujjlj2pdz&response_type=token&scope=&redirect_uri=" + encodeURIComponent(browser.identity.getRedirectURL()),
                interactive: forceInteractive
            }).then((url) => {
                const hash = url.split("#");
                hash.shift();
                this.token = new URLSearchParams(hash.join("")).get("access_token");
                callback(this.token);
            }).catch((err) => {
                this.logger.log(err);
                callback();
            });
        } else {
            callback(this.token)
        }
    }

}