
const Command = require.main.require("./Discord/class/Command");
const notification = require.main.require("./Discord/utils/notification");
const Module = require.main.require("./Discord/class/Module");

module.exports = new Module({
	name: "clear",
	version: "0.0.1",
			
	category: "Administrateur",
	
	commands: [new Command({
		commands: ["clear"], 
		description: "Enlève les derniers messages",
		neededPermission: "admin.commands.clear",
		usage: "[number_to_delete]",
		displayedInHelp: false,
		deleteMessage: false,
		onCommand: (message)=>{
			var splitMessage = message.content.split(" ")
			splitMessage.shift()
			var nbMessage = parseInt(splitMessage[0])
			if(isNaN(nbMessage)){
				notification("Nombre incorrect", message.channel, 10)
				return
			}else{
				if(nbMessage > 99){
					notification("Nombre trop grand (max 99)", message.channel, 10)
					return				
				}
				message.channel.bulkDelete(nbMessage+1).catch(() => {
					message.channel.fetchMessages({limit:nbMessage+1}).then(messages => {
						for (message of messages){
							if(message[1].deletable)
								message[1].delete()						
						}
					}).catch(console.error);				
				});;
			}
		}
	})]
});
