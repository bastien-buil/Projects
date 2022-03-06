
const Command = require.main.require("./Discord/class/Command")
const notification = require.main.require("./Discord/utils/notification")
const Module = require.main.require("./Discord/class/Module");

module.exports = new Module({
	name: "channelsmanager",
	version: "0.0.1",
			
	category: "Administrateur",
	
	commands: [new Command({
		commands: ["channelsmanager", "channelmanager",  "cm"], 
		description: "Gère les channels.",
		neededPermission: "admin.commands",
		displayedInHelp: false,
		subCommands: [
		new Command({
			commands: ["registercategory", "rgcategory", "rc"],
			description: "Enregistre une categorie",
			usage: "(categorie) [Description]",
			minArgs: 1,
			onCommand: (message)=>{
				var splittedContent = message.content.split(" ");					
				var category = splittedContent[2];					
				var description = splittedContent.slice(3).join(" ") || "";
				
				channelsManager.registerCategory(category, description, (result)=>{
					if(result.errorId == "alreadyExistErr")
						notification("La catégorie " + category + " existe déjà.", message.channel, 10);							
					else if(result.errorId == "dbErr")
						notification("Erreur de la base de donnée", message.channel, 10);										
					else
						notification("La catégorie " + category + " a été ajoutée.", message.channel, 10);						
				});					
			}
		}),new Command({
			commands: ["removecategory", "rmcategory", "rmc"],
			description: "Enlève une catégorie au channel actuel",
			usage: "(category)",
			minArgs: 1,
			onCommand: (message)=>{
				var splittedContent = message.content.split(" ");
				
				var category = splittedContent[2];					
				channelsManager.removeCategory(category, (result)=>{
					if(result.errorId == "dbErr")
						notification("Erreur de la base de donnée", message.channel, 10);										
					else
						notification("La catégorie " + category + " a été enlevé au channel actuel.", message.channel, 10);						
				});					
			}
		}),new Command({
			commands: ["addcategory", "acategory", "aca"],
			description: "Assigne une catégorie au channel actuel",
			usage: "(category)",
			minArgs: 1,
			onCommand: (message)=>{
				var splittedContent = message.content.split(" ");
				
				var category = splittedContent[2];
				
				channelsManager.addCategoryToChannel(message.channel.id, category, (result)=>{
					if(result.errorId == "dbErr")
						notification("Erreur de la base de données", message.channel, 10);							
					else if(result.errorId == "noUpdateErr")
						notification("Erreur de la base de données (noUpdate)", message.channel, 10);										
					else
						notification("La catégorie " + category + " a été ajouté au channel actuel.", message.channel, 10);						
				});					
			}
		})]
	
	})]
});
