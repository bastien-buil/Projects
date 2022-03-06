import StreamAlert from "./StreamAlert.js";
import Stream from "./Stream.js";

let streamAlert = new StreamAlert();

function onRequest(request, _sender, _sendResponse){
	if(request.getList != null){
		browser.runtime.sendMessage({"loadList": streamAlert.list, "sensitiveStreamSource": streamAlert.sensitiveStreamSource});
		streamAlert.updateTokens();
	}else if(request.removeStream != null){
		if(streamAlert.list[request.removeStream] != null){
			streamAlert.list.splice(request.removeStream, 1);
			browser.runtime.sendMessage({"loadList": streamAlert.list});
			streamAlert.saveConfig();
			streamAlert.checkOnline();
		}
	}else if(request.addStream != null){
		streamAlert.list.push(Stream.objectToClass(request.addStream));
		streamAlert.list.sort(function(a, b){
			 return a.name.localeCompare(b.name);
		});
		browser.runtime.sendMessage({"loadList": streamAlert.list});
		streamAlert.saveConfig();
		streamAlert.checkOnline();
	}else if(request.findStream != null){
		const url = request.findStream;
		if(url.indexOf("http://") !== -1 || url.indexOf("https://") !== -1){
			const splitUrl = url.split('/');
			let name = "";
			let type = -1;
			let info = "";
			let domain = splitUrl[2].toLowerCase();  // http 0/1/2google.fr/3
			if(domain.startsWith('www.')){
				domain = domain.substr(4);
			}
			if(domain === "games.dailymotion.com"){
				type = 0;
				info = splitUrl[4];
				name = splitUrl[3];
			}else if(domain === "dailymotion.com"){
				type = 0;
				const streamInfos = splitUrl[4].split('_');
				info = streamInfos[0];
				name = streamInfos[1];
			}else if(domain === "twitch.tv"){
				type = 1;
				info = splitUrl[3];			
				name = info;
			}else if(domain === "smashcast.tv"){
				type = 2;
				info = splitUrl[3];	
				name = info;			
			}else if((domain === "youtube.com" || domain === "gaming.youtube.com") && splitUrl[3] === "channel"){
				type = 3;				
				info = splitUrl[4];		
			}else if((domain === "youtube.com" || domain === "gaming.youtube.com") && splitUrl[3] === "user"){
				type = 3;				
				name = splitUrl[4];		
			}else if((domain === "leveldown.fr") && splitUrl[3] === "stream"){
				type = 4;				
				name = splitUrl[4];	
				info = splitUrl[4];		
			}else if(streamAlert.sensitiveStreamSource && domain.endsWith(".chaturbate.com")){
				type = 5;				
				name = splitUrl[3];	
				info = splitUrl[3];	
			}else if(streamAlert.sensitiveStreamSource && (domain.endsWith(".cammodels.com") || domain === "cammodels.com") && splitUrl[3] === "cam"){
				type = 6;				
				name = splitUrl[4];	
				info = splitUrl[4];	
			}else if(streamAlert.sensitiveStreamSource && domain.endsWith(".cammodels.com") && domain.split(".")[0] !== "www"){
				type = 6;
				name = domain.split(".")[0];	
				info = name;	
			}else if(streamAlert.sensitiveStreamSource && domain === "profiles.myfreecams.com"){
				type = 7;
				name = splitUrl[3];	
				info = splitUrl[3];	
			}
			else return;
			browser.runtime.sendMessage({"findStreamResult": {name: name, webLink: url, type: type, info: info}});
		}
	}else if(request.reload != null){
		streamAlert.checkOnline();
	}else if(request.reloadConfig != null){
		streamAlert.loadConfig();
	}else if(request.saveSearch != null){
		streamAlert.saveStream = request.saveSearch;
	}else if(request.getSearch != null){
		browser.runtime.sendMessage({sendSearch: streamAlert.saveStream});
	}else if(request.getLog != null){
		browser.runtime.sendMessage({sendLog: streamAlert.logger.getLogs()});
	}else if(request.setYoutubeApiKey != null){
		streamAlert.youtubeApiKey = request.setYoutubeApiKey;
		streamAlert.saveConfig();
	}else if(request.setStreamsList != null){
		streamAlert.setStreamsList(request.setStreamsList);
		streamAlert.saveConfig();
	}else if(request.getStreamsSavedList != null){
		browser.runtime.sendMessage({getStreamsSavedList: streamAlert.getStreamsSavedList()});
	}else if(request.setSensitiveSource != null){
		streamAlert.sensitiveStreamSource = request.setSensitiveSource;
		streamAlert.saveConfig();
	}
}


browser.runtime.onMessage.addListener(onRequest);


streamAlert.logger.log('If you see "400 (Bad Request)" in the console, that is an expected result maybe due to incorrect channel name.');
setInterval(streamAlert.checkOnline, 60000);
streamAlert.loadConfig();