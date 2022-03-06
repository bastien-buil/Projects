var querystring = require("querystring");
var fs = require("fs");
var path = require("path");

const WebHandler = require.main.require("./WebServer/class/WebHandler");


class DiscordWebHandler extends WebHandler{
	constructor(){
		super();
		this.path = path.dirname(require.main.filename) + "/WebServer/www/account/index.html";
	}
	onRequest(webPath, postData, request, response){
		if(webPath == "/account" || webPath == "/account/" || webPath == "/account/index.html"){
			this.loadMainFile(request, response);
		}else{
			this.display404Error(request, response);
		}
	}
	loadMainFile(request, response){
		var cookies = querystring.parse(request.headers.cookie, "; ");
		var sessionId = cookies.sessionId || "";
		webServer.sessionsManager.verifyToken(sessionId, request.socket.remoteAddress, (result)=>{
			if(result.success == true){
				fs.readFile(this.path, {encoding:"utf8"}, function (err, data){
					if (err) {
						response.end();
						return;
					}
					console.log(result.result)
					data = data.replace(/<!--\$username\$-->/g, result.result.username);
					
					
					response.write(data);
					response.end();
				});
			}else {
				this.display404Error(request, response)
			}	
		});	
		
	}
}






module.exports = DiscordWebHandler;