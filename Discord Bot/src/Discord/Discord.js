
const DarkMillionsBot = require("./DarkMillionsBot");
const getModulesFiles = require.main.require("./utils/getModulesFiles");

class Discord{
	constructor(){
		this.bot = new DarkMillionsBot('Put your token here');
		this.bot.loadModules(getModulesFiles())
		logger.log("Bot loaded", "Discord")
	}
}

module.exports = Discord;
