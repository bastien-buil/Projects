
const Command = require.main.require("./Discord/class/Command");
const notification = require.main.require("./Discord/utils/notification");
const request = require.main.require("./utils/request");
const Module = require.main.require("./Discord/class/Module");

module.exports = new Module({
	name: "cat",
	version: "0.0.1",
			
	category: "Images",
	
	commands: [new Command({
		commands: ["cat", "kitty"], 
		description: "Affiche une image aléatoire de chat",
		neededPermission: "images.commands",
		onCommand: (message)=>{
			var options = {link: getUrl(parseInt(message.content.split(" ")[1]))};
			var username = message.member.nickname || message.author.username;
			request(options, (info)=>{
				var url = "";
				if(info["type"] == "loaded"){
					console.log(info["body"]);
					var response = JSON.parse(info["body"]);
					url = response["file"];
				}else if(info["type"] == "redirect"){
					url = info["info"];
				}else{
					message.channel.send("Erreur de chargement de l'image, désolé " + username);
					return;
				}
				message.channel.send({
					"embed": {
						title: getRandomResponse(username),
						"image": {
							"url": url
						}
					}
				});
			});
		}
	})]
});

function getRandomResponse(username){
	var responses = ["Voici un chaton pour toi, " + username, "Et un kitty pour " + username];
	return responses[Math.floor(Math.random() * responses.length)];
}

function getUrl(arg){
	if(typeof arg != "number" || isNaN(arg) || arg < 0 || arg > 1)
		arg = 0;
	switch(arg){
		case 0:
			return "http://aws.random.cat/meow";
		break;
		case 1:
			return "http://thecatapi.com/api/images/get";				
		break;
	}
	
}



