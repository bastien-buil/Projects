const UrlManager = require("./UrlManager.js")
const request = require.main.require('./utils/request');


class ImgurManager extends UrlManager{
	constructor(){
		super();
		this.token = "a2e55760d5bcf9e";
	}

  isCompatible(urlLower){
    return urlLower.includes("imgur.com/");
  }

	getImagesFromAlbum(id, callback){
		id = id.split("#")[0];
		request({link: "https://api.imgur.com/3/album/" + id + "/images", headers: {Authorization: "Client-ID " + this.token}}, (infos)=>{
			if(infos["type"] == "loaded"){
				var objResult = JSON.parse(infos["body"]);
				var imagesList = [];
				if(Array.isArray(objResult["data"])){
					for(var image of objResult["data"]){
						imagesList.push(image["link"]);
					}
					callback(imagesList, {origin: "https://imgur.com/a/" + id});
				}
			}
		});;
	}

	getImage(url, callback){
		if(url.toLowerCase().endsWith(".gifv")){
			callback(url.slice(0,-5) + ".mp4");
			return;
		}
		if(url.endsWith("/"))
			url = url.slice(0, -1);
		var split = url.split("/");
		if(split[split.length - 2] == "a")
			this.getImagesFromAlbum(split[split.length - 1], callback);
		else
			callback("https://i.imgur.com/" + split[split.length - 1] + ".mp4");

	}
}

module.exports = ImgurManager;
