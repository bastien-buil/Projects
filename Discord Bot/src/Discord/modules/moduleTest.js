const Command = require.main.require("./Discord/class/Command")
const notification = require.main.require("./Discord/utils/notification")
const Module = require.main.require("./Discord/class/Module");

module.exports = new Module({
	name: "test",
	version: "0.0.1",
	onload: () =>{},
	commands: [new Command({
		commands: ["test", "thetest"], 
		description: "Bla bla",
		displayedInHelp: false,
		neededPermission: "",
		onCommand: (message)=>{
			notification(c, message.channel, 20); 
			notification("Inscription en cours", message.channel, 20); 
			var name = message.member.nickname || message.member.user.username;
			notification("Bonjour, " + name, message.channel, 20); 
			usersManager.updateUser(message.member.id, name, message.member.user.avatar); 
		
			
		},
		subCommands: [new Command({
			commands: ["happy"],
			description: "affiche go",
			onCommand: (message)=>{
				notification("TEST MA GUEULE", message.channel, 20); 
				for(var a of message.member.roles){
					logger.log(a[1].id)
				}
				notification(message.channel.id, message.channel, 60); 
			
				
			}
		})]
	})]
});
