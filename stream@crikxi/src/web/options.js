
browser.runtime.onMessage.addListener((request, _sender, _sendResponse) => {
	if(request.sendLog != null){
		document.getElementById('logTxt').value = "";
		for(let txt of request.sendLog){
			document.getElementById('logTxt').value += txt + "\n";			
		}
	}else if(request.getStreamsSavedList != null){        
		document.getElementById('data').value = JSON.stringify(request.getStreamsSavedList);        
    }
	
});



document.getElementById('resetData').onclick = function (){
	browser.storage.sync.set({streamsList: []});
	browser.runtime.sendMessage({reloadConfig: true});
}

document.getElementById('loadData').onclick = function (){
    	browser.runtime.sendMessage({getStreamsSavedList: true});
}

document.getElementById('saveData').onclick = function () {
	let saveList = JSON.parse(document.getElementById('data').value);
	browser.runtime.sendMessage({setStreamsList: saveList});
}

document.getElementById('enableSensitiveButton').onclick = function () {
	browser.runtime.sendMessage({setSensitiveSource: true});
}
document.getElementById('disableSensitiveButton').onclick = function () {
	browser.runtime.sendMessage({setSensitiveSource: false});
}

document.getElementById('youtubeApiKeyButton').onclick = function () {
	let youtubeApiKey = document.getElementById('youtubeApiKey').value;
	browser.runtime.sendMessage({setYoutubeApiKey: youtubeApiKey});
}

document.getElementById('logButton').onclick = function () {	

	document.getElementById('logTxt').value = "Loading :O";
	browser.runtime.sendMessage({getLog: true});
}




