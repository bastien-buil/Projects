
const Command = require.main.require("./Discord/class/Command");
const notification = require.main.require("./Discord/utils/notification");
const Module = require.main.require("./Discord/class/Module");

module.exports = new Module({
	name: "repost",
	version: "0.0.1",
			
	category: "Administrateur",
	
	commands: [new Command({
		commands: ["repost"], 
		description: "Enlève le message",
		neededPermission: "admin.commands",
		usage: "[message_id] [channel_id]",
		displayedInHelp: false,
		minArgs: 2,
		onCommand: (message)=>{
			message.channel.fetchMessage(message.content.split(" ")[1])
			.then(messageSearched => {
				message.guild.channels.get(message.content.split(" ")[2]).send("Reupload du message de <@!" + message.author.id +">:\n" + messageSearched.content, {files: Array.from(messageSearched.attachments.values()).map((x) => x.url)} )
				messageSearched.delete(10);
			}).catch((err) => {
				notification('Erreur message ou channel introuvable.', message.channel, 10);
			});
			
		}
	})]
});
