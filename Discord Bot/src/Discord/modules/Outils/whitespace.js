
const Command = require.main.require("./Discord/class/Command");
const notification = require.main.require("./Discord/utils/notification");
const Module = require.main.require("./Discord/class/Module");

module.exports = new Module({
	name: "whitespace",
	version: "0.0.1",
			
	category: "Outils",
	
	commands: [new Command({
		commands: ["whitespace", "ws"], 
		description: "Mets un espace blanc",
		neededPermission: "tools.commands",
		usage: "[size]",
		displayedInHelp: false,
		onCommand: (message)=>{			
			var splitMessage = message.content.split(" ")
			splitMessage.shift()
			var nbSpace = parseInt(splitMessage[0])
			if(isNaN(nbSpace)){
				nbSpace = 42;
			}
			message.channel.send("\\"+ " \n".repeat(nbSpace)+"/");
		}
	})]
});
