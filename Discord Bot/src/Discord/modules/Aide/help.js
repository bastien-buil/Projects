/*

TO maybe optimize
and manage when more than 25 commands

*/
const Command = require.main.require("./Discord/class/Command");
const notification = require.main.require("./Discord/utils/notification");
const Module = require.main.require("./Discord/class/Module");

module.exports = new Module({
	name:  "help",
	version: "0.0.2",
			
	category: "Aide",
	
	commands: [new Command({
		commands: ["help", "commands", "aide"], 
		description: "Vous aide pour les commandes.",
		usage: "[commande]",
		neededPermission: "",
		onCommand: (message)=>{			
			var splittedContent = message.content.split(" ");
			var commands = {};
			
			var fields = [];
			
			if(splittedContent[1]){
				for(var module of discord.bot.modulesList){
					var category = module.category || "Main";
					if(module.commands)
						for(var commandObj of module.commands){
							for(var command of commandObj.commands){
								if(splittedContent[1] == command){
									if(!commands[category])
										commands[category] = [];
									commands[category].push(commandObj);	
								}
							}
						}
				}
				for(var category in commands){
					for(var command of commands[category]){
						var fieldValue = "";
						for(var subCommand of command.subCommands){
							if(fieldValue)
								fieldValue += "\n";
							
							fieldValue +=  discord.bot.commandSymbols[0] + command.commands[0] + " " + subCommand.commands[0] + " " + subCommand.usage + " \u200b \u200b \u200b \u200b \u200b " + subCommand.description;
						}
						if(!fieldValue)
								fieldValue = "Pas de sous-commandes";
						fields.push({
							"name": "__" + discord.bot.commandSymbols[0] + command.commands[0] + " " + command.usage +  "__	     "  + command.description,
							"value": fieldValue
						});
					}
					
				}	
			}else{
				for(var module of discord.bot.modulesList){
					var category = module.category || "Main";
					if(module.commands)
						for(var commandObj of module.commands){
							if(commandObj.displayedInHelp){
								if(!commands[category])
									commands[category] = [];
								commands[category].push(commandObj);	
							}
						}
				}
				for(var category in commands){
					var fieldValue = "";
					for(var command of commands[category]){
						if(fieldValue)
							fieldValue += "\n";
						
						fieldValue += discord.bot.commandSymbols[0] + command.commands[0] + "    " + command.description;
					}
					fields.push({
						"name": " 	__" + category + "__",
						"value": fieldValue
					});
				}	
			}
			
			if(fields.length == 0)
				fields = [{
						"name": " 	__Commande non trouvée__",
						"value": "Usage: !help [commande]"
					}];
						
			notification({
			   "embed": {
					"title": "__**Aide DarkBot**__",
					"color": 7143168,
					"footer": {
						"icon_url": "https://cdn.discordapp.com/embed/avatars/0.png",
						"text": "Darkmillions"
					},
					"fields": fields
				}
			}, message.channel, 180);			
		}
	
	})]
});
	