var querystring = require("querystring");
var fs = require("fs");
var path = require("path");

const WebHandler = require.main.require("./WebServer/class/WebHandler");

class DefaultWebHandler extends WebHandler{
	constructor(){
		super();
		this.path = path.dirname(require.main.filename) + '/WebServer/www/default';
	}
	
	onRequest(webPath, postData, request, response){
		if(webPath == "/" || webPath == "/index.html"){
			this.loadMainFile(request, response);
		}else{
			this.loadFile(webPath, this.path, request, response);
		}
	}
	
	loadMainFile(request, response){
		fs.readFile(this.path + "/index.html", {encoding:"utf8"}, function (err, data){
			if (err) {
				response.end();
				return;
			}
			var cookies = querystring.parse(request.headers.cookie, "; ");
			var sessionId = cookies.sessionId || "";
			webServer.sessionsManager.verifyToken(sessionId, request.socket.remoteAddress, (result)=>{
				if(result.success == true){
					data = data.replace(/<!--\$disconnect\$>([\s\S]*?)<\$disconnect\$-->/g, "");
					data = data.replace(/<!--\$logged\$>([\s\S]*?)<\$logged\$-->/g, "$1");
				}else {
					data = data.replace(/<!--\$logged\$>([\s\S]*?)<\$logged\$-->/g, "");
					data = data.replace(/<!--\$disconnect\$>([\s\S]*?)<\$disconnect\$-->/g, "$1");
				}
				response.write(data);
				response.end();
			});
				
		});
		
		
	}

	
}







module.exports = DefaultWebHandler;