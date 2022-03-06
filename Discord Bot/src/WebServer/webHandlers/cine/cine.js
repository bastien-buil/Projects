var querystring = require("querystring");
var fs = require("fs");
var path = require("path");
const WebHandler = require.main.require("./WebServer/class/WebHandler"); 
const SocketIo = require.main.require("./WebServer/SocketIo");


class DiscordWebHandler extends WebHandler{
	constructor(){
		super();
		SocketIo.registerIo((io) => {
			

		});				
	}
	onRequest(webPath, postData, request, response){
		fs.readFile(path.dirname(require.main.filename) + "/WebServer/www/cine/index.html", function (err, data){	
			if (err) 
				return;
			
			response.write(data);
			response.end();				
		});;
	}
}



module.exports = DiscordWebHandler;