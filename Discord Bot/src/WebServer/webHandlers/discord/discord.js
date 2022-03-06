const querystring = require("querystring");
const fs = require("fs");
const url = require("url");
const dateToHttpDate = require.main.require("./WebServer/utils/DateToHttpDate");

const WebHandler = require.main.require("./WebServer/class/WebHandler");


class DiscordWebHandler extends WebHandler{
	onRequest(webPath, postData, request, response){
		var args = url.parse(request.url).search;
		if(args == null){
			redirect(response, request);
			return;
		}
		args = args.substr(1);
		var get = querystring.parse(args);
		if(get["code"]){
			webServer.discordOAuth.checkConnectionCode(get["code"], (success, discordToken)=>{
				if(!success)
					redirect(response, request);
				else
					webServer.discordOAuth.getInfoFromToken(discordToken, (success, info)=>{
						if(!success)
							redirect(response, request);
						else
							webServer.sessionsManager.connect(info["id"], info["username"], discordToken, request.socket.remoteAddress, (result)=>{
								if(!result.success){
									//logger.log("Can't connect, "+date);
									redirect(response, request);
								}else{
									response.setHeader('Set-Cookie', "sessionId=" + result.result["cookie"] + "; Expires=" + dateToHttpDate(result.result["date"]) + ";HttpOnly" + ";Secure");
									response.statusCode = 302;
									response.setHeader('Location', '/');
									response.end();
								}
							});
					});
			});
			
		}else{
			redirect(response, request);
		}
	}
}

function redirect(response, request){
	response.statusCode = 302;
	response.setHeader('Location', "//" + request.headers.host);
	response.write("Contenu non trouvé");
	response.end();
}




module.exports = DiscordWebHandler;