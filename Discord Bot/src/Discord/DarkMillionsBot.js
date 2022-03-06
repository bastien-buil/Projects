const Discord = require("discord.js");

class DarkMillionsBot {
	constructor(token){
		this.client = new Discord.Client();
		this.commandSymbols = ["!", ";;"];
		this.modulesList = [];
		
		this.client.on('message', function(that){ return function(message){that.onMessage(message)} }(this) );
		
		this.client.login(token);
	}
	loadModules(modulesList){
		for(let moduleName of modulesList){
			var module =  require(moduleName);
			this.modulesList.push(module);
			if(module.onload)
				module.onload();
		}
	}
	getModule(moduleName){
		for(let module of this.modulesList){
			if(module.name == moduleName)
				return module;
		}
		return null;
	}
	onMessage(message){
		if(!message.guild) return;
		
		var isCommand = false;
		for(var symbol of this.commandSymbols){
			if(message.content.startsWith(symbol)){
				isCommand = true;
				var currentCommand = message.content.split(" ")[0].toLowerCase().substring(symbol.length);
			}
		}
		if(!isCommand) return;		
		
		
		for(var module of this.modulesList){
			if(module.commands)
				for(var commandObj of module.commands){
					var commandIsExecuted = false;
					
					for(var command of commandObj.commands){
						if(currentCommand == command)
							commandIsExecuted = true;					
					}
					if(commandIsExecuted == true){
						commandObj.execute(message);
					}
					
				}
		}
	}
}

module.exports = DarkMillionsBot;