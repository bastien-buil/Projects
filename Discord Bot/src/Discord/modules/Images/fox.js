
const Command = require.main.require("./Discord/class/Command");
const notification = require.main.require("./Discord/utils/notification");
const request = require.main.require("./utils/request");
const Module = require.main.require("./Discord/class/Module");

module.exports = new Module({
	name: "fox",
	version: "0.0.1",
			
	category:  "Images",
	
	commands: [new Command({
		commands: ["fox"], 
		description: "Affiche une image aléatoire de renard",
		neededPermission: "images.commands",
		onCommand: (message)=>{
			var username = message.member.nickname || message.author.username;
			request({link: "https://randomfox.ca/floof/"}, (info)=>{
				var url = "";
				if(info["type"] == "loaded"){
					var response = JSON.parse(info["body"]);
					url = response["image"];
					message.channel.send({
						"embed": {
							title: getRandomResponse(username),
							"image": {
								"url": url
							}
						}
					});
				}else{
					message.channel.send("Erreur de chargement de l'image, désolé " + username);
				}
					
			});			
		}
	})]
	
	
	
	
});

function getRandomResponse(username){
	var responses = ["Voici un renard pour toi, " + username];
	return responses[Math.floor(Math.random() * responses.length)];
}
