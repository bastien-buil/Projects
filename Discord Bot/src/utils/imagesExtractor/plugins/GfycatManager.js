const UrlManager = require("./UrlManager.js")
const request = require.main.require('./utils/request');


class GfycatManager extends UrlManager{
	constructor(){
		super();
		this.NEEDAUTH = false;
		this.token = "";
		this.expirationDate = new Date();
	}

  isCompatible(urlLower){
    return urlLower.includes("gfycat.com/");
  }
	isTokenExpired(){
		if(this.expirationDate > new Date()) return false;
		else return true;
	}
	getToken(callback){
		if(this.isTokenExpired()){
			renewToken(callback);
		}else callback(this.token);
	}
	renewToken(callback){
		request({
			link:"https://api.gfycat.com/v1/oauth/token",
			data:'{"client_id":"2_N4L2t1", "client_secret": "-TApLAC8fLHuWyiZVaOiMiozjW8WUsHW9JatwGTn8pBir5IjY6xJoXVvt84RdlNW", "grant_type": "client_credentials"}'
		}, (infos)=>{
			if(infos["type"] == "loaded"){
				var objResult = JSON.parse(infos["body"]);
				this.token = objResult["access_token"];
				this.expirationDate = new Date(objResult["expires_in"]*1000 + Date.now());
				callback(this.token);
			}
		});
	}
	getImage(url, callback){
		if(url.endsWith("/"))
		  url = url.slice(0, -1);
		var split = url.split("/");
		var imageId = split[split.length - 1];
		var requestOptions;
		if(this.NEEDAUTH){
		  request({link: "https://api.gfycat.com/v1/gfycats/" + imageId, headers: {Authorization: "Bearer " + this.token}}, onRequest);;
		}else{
			request("https://api.gfycat.com/v1/gfycats/" + imageId, onRequest);
		}
		function onRequest(infos){
			if(infos["type"] == "loaded"){
				var response = JSON.parse(infos["body"]);
				if(response["errorMessage"])
					callback(null);
				else
					callback(response["gfyItem"]["mp4Url"]);
			}
		}
	}
}

module.exports = GfycatManager;
