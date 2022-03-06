const router = require("./router");
const socket = require("./SocketIo");


const getWebHandlers = require("./utils/getWebHandlers");
const DiscordOAuth = require("./utils/DiscordOAuth");

class WebServer{
	constructor(){
		this.sessionsManager = new (require("./managers/SessionsManager"))("dfgfd156gf4156f4fd564", null);
		
		this.webServer = require("./web")(router, new (require("./webHandlers/default.js")), getWebHandlers());
		this.socketServer = socket.init(this.webServer);
		this.discordOAuth = new DiscordOAuth("464620176625631243", "25L2qdAqBx7U2paElFn2HqIiONOnJm_T", "https://darkmillions.tk")
		
	}
}

module.exports = WebServer;