
const Command = require.main.require("./Discord/class/Command");
const notification = require.main.require("./Discord/utils/notification");
const request = require.main.require("./utils/request");
const Module = require.main.require("./Discord/class/Module");

module.exports = new Module({
	name: "dog",
	version: "0.0.1",
			
	category: "Images",
	
	commands: [new Command({
		commands: ["dog"], 
		description: "Affiche une image aléatoire de chien",
		neededPermission: "images.commands",
		onCommand: (message)=>{
			var link = getUrl(parseInt(message.content.split(" ")[1]));
			var username = message.member.nickname || message.author.username;
			request({link: link}, (info)=>{
				var url = "";
				if(info["type"] == "loaded" && link == "https://random.dog/woof.json?filter=mp4,webm"){
					var response = JSON.parse(info["body"]);
					url = response["url"];
				}else if(info["type"] == "loaded" && link == "https://api.thedogapi.co.uk/v2/dog.php"){
					var response = JSON.parse(info["body"]);
					url = response["data"][0]["url"];
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
	var responses = ["Voici un chien pour toi, " + username];
	return responses[Math.floor(Math.random() * responses.length)];
}

function getUrl(arg){
	if(typeof arg != "number" || isNaN(arg) || arg < 0 || arg > 1)
		arg = 0;
	switch(arg){
		case 0:
			return "https://random.dog/woof.json?filter=mp4,webm";
		break;
		case 1:
			return "https://api.thedogapi.co.uk/v2/dog.php";//down ?			
		break;
	}
	
}
	
	

