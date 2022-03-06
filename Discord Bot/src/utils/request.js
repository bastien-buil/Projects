const http = require('http');
const https = require('https');
const url = require('url');

function request(options, callback=()=>{}){
	if(typeof options == "string"){
		var urlLink = url.parse(options)
		var get = getByProtocol(urlLink.protocol);
	}else{
		if(options["data"]){
			var data = options["data"];
			delete options["data"];
			options["method"] = "POST";
		}
		if(options["link"]){			
			var urlLink = url.parse(options["link"])
			delete options["link"];
			var get = getByProtocol(urlLink.protocol);
			options["protocol"] = urlLink.protocol;
			options["hostname"] = urlLink.hostname;
			options["port"] = urlLink.port;
			options["path"] = urlLink.path;
		}else{			
			var get = getByProtocol(options["protocol"]);
		}
	}
	var req = get(options, function(res){
		if(res["statusCode"] == 302 || res["statusCode"] == 301){
			callback({type:"redirect", info: res["headers"]['location'], statusCode:res["statusCode"]});
		}else{
			var body = '';
			res.on('data', function(chunk){
				body += chunk;			
			});
			res.on('end', function(){
				callback({type:"loaded", request: res, body:body, statusCode:res["statusCode"]});			
			});			
		}
	}).on('error', function(e){
		callback({type:"error", info: e});
	});
	if(data)
		req.write(data);
	req.end();
	
}

function getByProtocol(protocol){
	if(protocol == "https:")
		return https.request;
	else
		return http.request;
}

module.exports = request;