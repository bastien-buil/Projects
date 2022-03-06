const querystring = require("querystring");
const dateToHttpDate = require.main.require("./WebServer/utils/DateToHttpDate");

const WebHandler = require.main.require("./WebServer/class/WebHandler");

class LogoutWebHandler extends WebHandler{	
	onRequest(webPath, postData, request, response){
		var cookies = querystring.parse(request.headers.cookie, "; ");
		if(cookies["sessionId"]){
			webServer.sessionsManager.removeCookieId(cookies["sessionId"], request.socket.remoteAddress);
		}
		response.setHeader('Set-Cookie', "sessionId=" + ";HttpOnly" + ";Secure");
		response.statusCode = 302;
		response.setHeader('Location', '/');
		response.end()
	}		
}

module.exports = LogoutWebHandler;