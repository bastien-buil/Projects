
class UrlManager{
	constructor(){
	}
	isCompatible(urlLower){
		return urlLower.endsWith(".jpg") || urlLower.endsWith(".png") || urlLower.endsWith(".gif") || urlLower.endsWith(".mp4");
	}
	getImage(url, callback){
		callback(url);
	}

}

module.exports = UrlManager;
