const notification = require.main.require("./Discord/utils/notification")

class Command{
	constructor(args){ 
		this.commands 			= args["commands"]; //Liste de text string
		this.description 		= args["description"] || "";
		this.usage 				= args["usage"] || "";//Example "[args]" for "!help args"
		this.onCommand 			= args["onCommand"] || null;
		this.subCommands		= args["subCommands"] || [];
		this.deleteMessage 		= args["deleteMessage"] == false ? false : true;
		this.isNsfw 			= args["isNsfw"] == true ? true : false;
		this.displayedInHelp 	= args["displayedInHelp"] == false ? false : true;
		this.neededPermission 	= args["neededPermission"] || "";
		this.minArgs 			= args["minArgs"] || 0;
	}
	
	execute(message, i=1){//i est la position qui est regarder pour les subcommands
		var content = message.content;
		var splittedContent = message.content.split(" ");
		var isExecuted = false;
		for(var subCommand of this.subCommands){
			var subCommandIsExecuted = false;
			if(splittedContent[i]){
				for(var subCommandContent of subCommand.commands){
					if(splittedContent[i].toLowerCase() == subCommandContent)
						subCommandIsExecuted = true;
				}
			}
			if(subCommandIsExecuted == true){
				this.testPerm(message, ((subCommand) => {
					subCommand.execute(message, i+1);
				}).bind(this, subCommand));
				isExecuted = true;
			}
		}
		if(isExecuted == false){
			if(this.deleteMessage)
				message.delete();
				
			if(this.onCommand != null){
				this.testPerm(message, () => {
					if(splittedContent.length < i + this.minArgs){
						notification("Utilisation incorrecte (mauvais nombre d'arguments), utiliser la commande help pour plus d'informations.", message.channel, 10);
						return;
					}
					this.onCommand(message);
										
				});
				
			}else{
				notification("Utilisation incorrecte\n\"!help [la commande]\" pour plus d'informations.", message.channel, 180);	
			}
		}
	}
	testPerm(message, callback){
		if(!this.isNsfw || (this.isNsfw && message.channel.nsfw)){
			if(this.neededPermission)
				permissionsManager.testPermission(message.member.id, message.channel.id, this.neededPermission, (result) => {
					if(result.result){
							callback();					
					}
					else								
						notification(message.author.username + " vous n'avez pas la permission.", message.channel, 10);		
				});
			else
				callback();
		
		}else								
			notification("Ce salon n'accepte pas les commandes nsfw.", message.channel, 10);
	}
}

module.exports = Command;