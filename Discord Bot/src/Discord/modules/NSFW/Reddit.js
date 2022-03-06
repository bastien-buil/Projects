
const Command = require.main.require("./Discord/class/Command");
const notification = require.main.require("./Discord/utils/notification");
const imageExtractor = require.main.require("./utils/imagesExtractor/imagesExtractor");
const request = require.main.require("./utils/request");
const Module = require.main.require("./Discord/class/Module");

module.exports = new class RedditModule extends Module{
	constructor(){
		super({
			name: "reddit",
			version: "0.0.1",
					
			category: "NSFW",
			commands: [new Command({
				commands: ["rdreddit"], 
				description: "Affiche une image de reddit",
				neededPermission: "nsfw",
				isNsfw: true,
				onCommand: (message)=>{
					var subreddit = message.content.split(" ")[1] || "";
					this.isSubBanned(subreddit, (isBanned)=>{
						if(isBanned){
							notification("Subreddit banni", message.channel, 10);														
						} else this.postRandomImage(message, subreddit, 0);						
					});
					
				},
				subCommands: []
			})]
		});
	}
	isSubBanned(subreddit, callback){
		if(["shit","blood"].includes(subreddit.toLocaleLowerCase())) callback(true);
		else callback(false);		
	}

	postRandomImage(message, subreddit, i){
		var username;
		if(message.member.nickname)
			username = message.member.nickname + " (" + message.author.username +  ")"
		else
			username = message.author.username;
		request('https://www.reddit.com/r/' + subreddit + "/random.json", (info)=>{
			if(info["type"] == "redirect"){
				request(info["info"], (info)=>{
					if(info["type"] == "loaded"){
						var response = JSON.parse(info["body"]);
						if(response["data"]){
							notification("Aucun subreddit trouvé. " + username + ", on a pas de " + subreddit, message.channel, 10);
							return;
						}
						
						var link = response[0]["data"]["children"][0]["data"]["url"];
						imageExtractor(link, (link, info)=>{
							if(link != null){
								var links = "";
								if(typeof link == "string")
									links = link;
								else if(Array.isArray(link)){
									if(info && info["origin"])
										links += "<" + info["origin"] + ">\n"
									links += link.join("\n");
								}				
								message.channel.send("Voila pour toi, " + username + ", un média de " + subreddit + "\n" + links);								
							}else if(i < 10){
								this.postRandomImage(message, subreddit, i+1);
							}else
								notification("Pas d'image trouvé, " + username + ", veuillez réessayer ou essayer un autre subreddit", message.channel, 10);
						});						
					}else{
						message.channel.send("Erreur de chargement de l'image, désolé " + username);
						return;
					}
				});
			}else{
				notification("Aucun subreddit trouvé. " + username + ", on a pas de " + subreddit + "\nDésolé", message.channel, 10);
				return;
			}
		});
	}

}




