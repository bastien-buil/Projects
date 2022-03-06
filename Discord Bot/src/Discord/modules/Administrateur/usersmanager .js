
const Command = require.main.require("./Discord/class/Command")
const notification = require.main.require("./Discord/utils/notification")
const Module = require.main.require("./Discord/class/Module");

module.exports = new Module({
	name: "usersmanager",
	version: "0.0.1",
			
	category: "Administrateur",
	
	commands: [
		new Command({
			commands: ["usersmanager", "usermanager",  "um"], 
			description: "Gère les utilisateurs et les roles.",
			neededPermission: "admin.commands",
			displayedInHelp: false,
			subCommands: [
				new Command({
					commands: ["registerrole", "rgrole", "rr"],
					description: "Enregistre un role",
					usage: "(role) [Description]",
					minArgs: 1,
					onCommand: (message)=>{
						var splittedContent = message.content.split(" ");					
						var roleName = splittedContent[2];					
						var description = splittedContent.slice(3).join(" ") || "";
						
						usersManager.registerRole(roleName, description, (result)=>{
							if(result.errorId == "alreadyExistErr")
								notification("Le role " + roleName + " existe déjà.", message.channel, 10);							
							else if(result.errorId == "dbErr")
								notification("Erreur de la base de donnée", message.channel, 10);										
							else
								notification("Le role " + roleName + " a été ajouté.", message.channel, 10);						
						});					
					}
				}),
				new Command({
					commands: ["removerole", "rmrole", "rmr"],
					description: "Supprime un role",
					usage: "(role)",
					minArgs: 1,
					onCommand: (message)=>{
						var splittedContent = message.content.split(" ");
						
						var role = splittedContent[2];					
						usersManager.removeRole(role, (result)=>{
							if(result.errorId == "dbErr")
								notification("Erreur de la base de donnée", message.channel, 10);										
							else
								notification("Role " + permission + " enlevé.", message.channel, 10);						
						});					
					}
				}),
				new Command({
					commands: ["addrole", "arole", "aro"],
					description: "Assigne un role à un joueur",
					usage: "(userId) (role)",
					minArgs: 2,
					onCommand: (message)=>{
						var splittedContent = message.content.split(" ");
						
						var userId = splittedContent[2];
						var role = splittedContent[3];
						
						usersManager.addRole(userId, role, (result)=>{
							if(result.errorId == "dbErr")
								notification("Erreur de la base de données", message.channel, 10);							
							else if(result.errorId == "noUpdateErr")
								notification("Joueur inconnu ou non enregistré", message.channel, 10);										
							else
								notification("Role " + role + " ajouté au joueur ayant pour id " + userId, message.channel, 10);						
						});					
					}
				}),
				new Command({
					commands: ["removerole", "rmrole", "rmro"],
					description: "Enlève un role à un joueur",
					usage: "(userId) (role)",
					minArgs: 2,
					onCommand: (message)=>{
						var splittedContent = message.content.split(" ");
						
						var userId = splittedContent[2];
						var role = splittedContent[3];
						
						usersManager.removeRole(userId, role, (result)=>{
							if(result.errorId == "dbErr")
								notification("Erreur de la base de données", message.channel, 10);							
							else if(result.errorId == "noUpdateErr")
								notification("Joueur non enregistré ou le joueur ne possède pas le role", message.channel, 10);										
							else
								notification("Role " + role + " enlevé au joueur ayant pour id " + userId, message.channel, 10);						
						});					
					}
				}),
				new Command({
					commands: ["registeruser", "rgu", "rguser"],
					description: "Enregistre un utilisateur",
					usage: "(userId)",
					minArgs: 1,
					onCommand: (message)=>{
						var splittedContent = message.content.split(" ");
						
						var userId = splittedContent[2];
						message.guild.fetchMember(userId).then((member) => {
							var username = member.nickname || member.user.username;
							usersManager.updateUser(userId, username, member.user.avatar, (result)=>{
								if(result.errorId == "dbErr")
									notification("Erreur de la base de données", message.channel, 10);							
								else if(result.errorId == "noUpdateErr")
									notification("Utilisateur déjà enregistré", message.channel, 10);										
								else
									notification("Utilisateur " + username + " enregistré", message.channel, 10);						
							});
							
						}).catch(()=>{
							notification("Utilisateur non trouvé", message.channel, 10);					
						});
											
					}
				})
			]
	
		})
	]
});
	

