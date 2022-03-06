const request = require.main.require("./utils/request")

class DiscordOAuth{
	constructor(client_id, client_secret, url){
		this.client_id = client_id;
		this.client_secret = client_secret;
		this.redirect_uri = url + "/discord";
	}
	
	get discordConnectUrl(){	//Need to implement state for security
		return "https://discordapp.com/oauth2/authorize?client_id=" + this.client_id + "&redirect_uri=" + this.redirect_uri + "&response_type=code&scope=identify";
	}
	checkConnectionCode(code, callback){	
		var url = "https://discordapp.com/api/oauth2/token";
		var data = "client_id=" + this.client_id + "&client_secret=" + this.client_secret + "&grant_type=authorization_code&redirect_uri=" + this.redirect_uri  + "&code=" + code;
		var headers = {
			'Content-Type': 'application/x-www-form-urlencoded'
		}
		request({link: url, data: data, headers:headers}, function(info){
			if(info["type"] == "loaded"){
				var response = JSON.parse(info["body"]);
				if(response["access_token"]){
					callback(true, response["access_token"]);
					return;
				}
			}
			callback(false, info["body"]);
			
		});
	}
	getInfoFromToken(token, callback){	
		var url = "https://discordapp.com/api/users/@me";
		request({link: url, headers: {Authorization: "Bearer " + token}}, (info)=>{
			
			if(info["type"] == "loaded"){
				callback(true, JSON.parse(info["body"]));
				return;
			}			
			callback(false);
		});
	}
}

module.exports = DiscordOAuth;





















