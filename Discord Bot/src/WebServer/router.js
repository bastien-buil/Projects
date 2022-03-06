var fs = require('fs');


function route(defaultHandle, customLinkHandle, webPath, postData, request, response) {
	var webPathSplit = webPath.split("/");
	if (typeof customLinkHandle[webPathSplit[1]] === "object" && typeof customLinkHandle[webPathSplit[1]].onRequest === 'function') {
		customLinkHandle[webPathSplit[1]].onRequest(webPath, postData, request, response);
	}
	else {
		defaultHandle.onRequest(webPath, postData, request, response);
	}
}


module.exports = route;