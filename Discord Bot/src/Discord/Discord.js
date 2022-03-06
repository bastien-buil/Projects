
const DarkMillionsBot = require("./DarkMillionsBot");
const getModulesFiles = require.main.require("./utils/getModulesFiles");

class Discord{
	constructor(){
		this.bot = new DarkMillionsBot('NDY0NjIwMTc2NjI1NjMxMjQz.DiBm0A.h9VX0OIEyprD9ytaKnHCZQwb9rU');
		//this.bot = new DarkMillionsBot('MTg4NTk3Njk0OTE3Mzc4MDQ4.D3fmdQ.uDBc-7jBpvnDCS4qxtwmC7hJ790');
		this.bot.loadModules(getModulesFiles())
		logger.log("Bot loaded", "Discord")
	}
}

module.exports = Discord;