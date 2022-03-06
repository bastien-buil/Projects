let size = 2;
let deleteMode = false;
let loadSearch = false;
let sensitiveStreamSource = false;

const plus = document.createElement('img');
plus.src = "../../data/plus.png";
document.getElementById('actions').appendChild(plus);
plus.onclick = function() {	
	browser.runtime.sendMessage({getSearch: true});
	loadSearch = true;
	document.getElementById('main').style.display = "none";
	document.getElementById('addStream').style.display = "block";
	document.getElementById('sensitiveStreamSource').style.display = sensitiveStreamSource? "block" : "none";	
}

const reload = document.createElement('img');
reload.src = "../../data/reload.png";
reload.onclick = function() {
	browser.runtime.sendMessage({reload: true});
}

document.getElementById('actions').appendChild(reload);

const moins = document.createElement('img');
moins.src = "../../data/moins.png";
moins.onclick = function() {
	deleteMode = !deleteMode;
	browser.runtime.sendMessage({"getList": true});
}
document.getElementById('actions').appendChild(moins);




function onRequest(request, _sender, _sendResponse){
	if(request.loadList != null) {
		while (document.getElementById('streamsListOnline').hasChildNodes()) {  
			document.getElementById('streamsListOnline').removeChild(document.getElementById('streamsListOnline').firstChild);
		}
		while (document.getElementById('streamsListOffline').hasChildNodes()) {  
			document.getElementById('streamsListOffline').removeChild(document.getElementById('streamsListOffline').firstChild);
		}
		let list = request.loadList;
		renderGuiListStreams(list);
		
		if(request.sensitiveStreamSource != null){
			sensitiveStreamSource = request.sensitiveStreamSource;
		}
	}
	else if(request.findStreamResult != null) {
		document.getElementById('addStreamName').value = request.findStreamResult['name'];	
		document.getElementById('addStreamLink').value = request.findStreamResult['webLink'];	
		document.getElementById('addStreamType').value = request.findStreamResult['type'];	
		document.getElementById('addStreamInfo').value = request.findStreamResult['info'];	
	}
	else if(request.sendSearch != null) {
		document.getElementById('addStreamName').value = request.sendSearch.name;
		document.getElementById('addStreamLink').value = request.sendSearch.webLink;
		document.getElementById('addStreamType').value = request.sendSearch.type;
		document.getElementById('addStreamInfo').value = request.sendSearch.info;	
	}
}


function renderGuiListStreams(list) {
	for(let stream of list){
				/*if(size < (name.length * 0.8 + 2.5)){
					size = name.length * 0.8 + 2.5;
					document.getElementById('streamsList').style.width = size+ "em";
				}Inutile avec bug firefox 48
				*/
			
				let thatElement = document.createElement('div');
				let imgElement = document.createElement('img');
				if(stream.online)
					imgElement.src = "../../data/online.png";
				else
					imgElement.src = "../../data/offline.png";
				let textFileElement = document.createTextNode(" " + stream.name);
				thatElement.appendChild(imgElement);
				thatElement.appendChild(textFileElement);
				if(!deleteMode){
					thatElement.className = "normalMode";
					thatElement.onclick = function (webLink){
						return function(){
							browser.tabs.create({ url: webLink });
						}
					}(stream.webLink);
				}else{
					thatElement.className = "deleteMode";
					thatElement.onclick = function (id){
						return function(){
							browser.runtime.sendMessage({"removeStream": id});
						}
					}(list.indexOf(stream));
				}
				if(stream.online)
					document.getElementById('streamsListOnline').appendChild(thatElement);
				else
					document.getElementById('streamsListOffline').appendChild(thatElement);
		}
}

function addStream(name, webLink, type, info, errorElement){
	errorElement.style.display = "none";
	if(name && webLink && type && info){
		if(webLink.indexOf(":") !== -1){
			browser.runtime.sendMessage({addStream: {name: name, online: false, webLink: webLink, type: parseInt(type), info: info} } );
			document.getElementById('addStream').style.display = "none";
			document.getElementById('main').style.display = "block";
		}else{
			errorElement.style.display = "block";		
			errorElement.innerHTML = "Lien incorrect. N'oublie pas le http://";			
		}	
	}else{
		errorElement.style.display = "block";		
		errorElement.innerHTML = "Au moins une case est vide.";
	}
	
}

document.getElementById('addStreamAuto').addEventListener("click", function(){
	browser.tabs.query({currentWindow: true, active: true}).then(function (tabs){
		browser.runtime.sendMessage({findStream: tabs[0].url});
	});
});

document.getElementById('clearStreamAuto').addEventListener("click", function(){	
	document.getElementById('addStreamName').value = "";
	document.getElementById('addStreamLink').value = "";
	document.getElementById('addStreamType').value = 0;
	document.getElementById('addStreamInfo').value = "";	
});


window.addEventListener("unload", function (){
	if(loadSearch){
		let stream = {name: document.getElementById('addStreamName').value, webLink: document.getElementById('addStreamLink').value, type: document.getElementById('addStreamType').value, info: document.getElementById('addStreamInfo').value};
		browser.runtime.sendMessage({saveSearch: stream});
	}
});

document.getElementById('addStreamValidate').addEventListener("click", function(){addStream(document.getElementById('addStreamName').value, document.getElementById('addStreamLink').value, document.getElementById('addStreamType').value, document.getElementById('addStreamInfo').value, document.getElementById('addStreamError'));});

browser.runtime.onMessage.addListener(onRequest);
browser.runtime.sendMessage({getList: true});

