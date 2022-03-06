
const Command = require.main.require("./Discord/class/Command");
const notification = require.main.require("./Discord/utils/notification");
const Module = require.main.require("./Discord/class/Module");

module.exports = new Module({
	name: "permissionsmanager",
	version: "0.0.1",
			
	category: "Administrateur",
	
	commands: [new Command({
		commands: ["permissionsmanager", "permissionmanager", "permmanager", "pm"], 
		description: "Gère les permissions.",
		neededPermission: "admin.commands",
		displayedInHelp: false,
		subCommands: [
		new Command({
			commands: ["registerpermission", "rgperm", "rp"],
			description: "Enregistre une permission",
			usage: "(permission) (Valeur par défaut) [Description]",
			minArgs: 2,
			onCommand: (message)=>{
				var splittedContent = message.content.split(" ");
				
				var permission = splittedContent[2];
				var defaultPerm = splittedContent[3];
				if(defaultPerm == "true")
					defaultPerm = true;
				else if (defaultPerm == "false")
					defaultPerm = true;
				else{
					notification("Valeur par défaut incorrecte, elle doit être true ou false.", message.channel, 10);	
					return;
				}
				
				var description = splittedContent.slice(4).join(" ") || "";
				permissionsManager.registerPermission(permission, defaultPerm, description, (result)=>{
					if(result.errorId == "alreadyExistErr")
						notification("La permission " + permission + " existe déjà.", message.channel, 10);							
					else if(result.errorId == "dbErr")
						notification("Erreur de la base de donnée", message.channel, 10);										
					else
						notification("Permission " + permission + " ajoutér.", message.channel, 10);						
				});					
			}
		}),new Command({
			commands: ["removepermission", "rmperm", "rmp"],
			description: "Supprime une permission",
			usage: "(permission)",
			minArgs: 1,
			onCommand: (message)=>{
				var splittedContent = message.content.split(" ");
				
				var permission = splittedContent[2];					
				permissionsManager.removePermission(permission, (result)=>{
					if(result.errorId == "dbErr")
						notification("Erreur de la base de donnée", message.channel, 10);										
					else
						notification("Permission " + permission + " enlevée.", message.channel, 10);						
				});					
			}
		}),new Command({
			commands: ["setpermtorole", "setpermrole", "spr"],
			description: "Définit une permission pour un role",
			usage: "(role) (permission) (Valeur par défaut)",
			minArgs: 3,
			onCommand: (message)=>{
				var splittedContent = message.content.split(" ");
				
				var role = splittedContent[2];
				var permission = splittedContent[3];
				var defaultPerm = splittedContent[4];
				if(defaultPerm == "true")
					defaultPerm = true;
				else if (defaultPerm == "false")
					defaultPerm = false;
				else{
					notification("Valeur par défaut incorrecte, elle doit être true ou false.", message.channel, 10);	
					return;
				}
				
				permissionsManager.setPermToRole(role, permission, defaultPerm, (result)=>{
					if(result.errorId == "unknownPermError")
						notification("La permission " + permission + " n'existe pas.", message.channel, 10);							
					else if(result.errorId == "dbErr")
						notification("Erreur de la base de données", message.channel, 10);							
					else if(result.errorId == "noUpdateErr")
						notification("Role " + role +" inconnu ou valeur inchangée.", message.channel, 10);										
					else
						notification("Permission " + permission + " a été mise pour " + role, message.channel, 10);						
				});					
			}
		}),new Command({
			commands: ["setpermtocategory", "setpermcategory", "spc"],
			description: "Définit une permission pour une categorie",
			usage: "(category) (role) (permission) (Valeur par défaut)",
			minArgs: 4,
			onCommand: (message)=>{
				var splittedContent = message.content.split(" ");
				
				var category = splittedContent[2];
				var role = splittedContent[3];
				var permission = splittedContent[4];
				var defaultPerm = splittedContent[5];
				if(defaultPerm == "true")
					defaultPerm = true;
				else if (defaultPerm == "false")
					defaultPerm = true;
				else{
					notification("Valeur par défaut incorrecte, elle doit être true ou false.", message.channel, 10);	
					return;
				}					
				permissionsManager.setPermToCategory(category, role, permission, defaultPerm, (result)=>{
					if(result.errorId == "unknownPermError")
						notification("La permission " + permission + " n'existe pas.", message.channel, 10);							
					else if(result.errorId == "dbErr")
						notification("Erreur de la base de données", message.channel, 10);							
					else if(result.errorId == "noUpdateErr")
						notification("Categorie inconnue ou valeur inchangée.", message.channel, 10);										
					else
						notification("Permission " + permission + " a été mise pour " + role + " dans les channels avec la categorie " + category, message.channel, 10);						
				});					
			}
		})]
	
	})]
});
