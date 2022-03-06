
var url = require("url");
var fs = require("fs");
var path = require("path");
var https = require("https");
var http = require("http");


function start(route, defaultHandle, customLinkHandle) {
	function onRequest(request, response) {
		var postData = "";
		var webPath = url.parse(request.url).pathname; 
		request.setEncoding("utf8");
		request.addListener("data", function(postDataChunk) {
			postData += postDataChunk;
		});

		request.addListener("end", function() {
			route(defaultHandle, customLinkHandle, decodeURI(webPath), postData, request, response);
		});


	}

	try{
		var server = https.createServer({
			key: fs.readFileSync(path.dirname(require.main.filename) + '/WebServer/privkey.pem'),
			cert: fs.readFileSync(path.dirname(require.main.filename) + '/WebServer/cert.pem')
		}, onRequest);


		server.listen(443);
		logger.log("Serveur HTTPS ok", "WebServer");
		
		var redirectServer = http.createServer((request, response) => {
			response.statusCode = 302;
			response.setHeader('Location', 'https://' + request.headers.host + request.url);
			response.end();		
		});		
		redirectServer.listen(80);
		
		logger.log("Serveur de redirection HTTP ok", "WebServer");
	}catch(e){
		var server = http.createServer(onRequest);
		server.listen(80);
		logger.log("Serveur HTTP ok", "WebServer");
	}
	return server;
}

module.exports = start;