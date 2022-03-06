var querystring = require("querystring");
var fs = require("fs");
var path = require("path");

class WebHandler{
	constructor(){
		this.errorPath = path.dirname(require.main.filename) + '/WebServer/www/error';		
	}
	
	onRequest(webPath, postData, request, response){}
	
	headContentType(file, response){
		if(file.endsWith(".css")){
			response.writeHead(202, {"Content-Type": "text/css"});	
		}
	}
	
	loadFile(file, webPath, request, response){
		fs.readFile(webPath + "/" + file, function(that){
			return function (err, data){
				if (err) {
					that.display404Error(request, response);
					return;
				}	
				that.headContentType(file, response);
				response.write(data);
				response.end();
				
			}
		}(this));
	}
	
	display404Error(request, response){
		response.statusCode = 404;
		this.loadFile("404.html", this.errorPath, request, response);
	}
}

module.exports = WebHandler;