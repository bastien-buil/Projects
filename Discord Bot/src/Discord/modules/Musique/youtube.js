
const Command = require.main.require("./Discord/class/Command");
const notification = require.main.require("./Discord/utils/notification");
const request = require.main.require("./utils/request");
const Module = require.main.require("./Discord/class/Module");

const ytdl = require("ytdl-core");

module.exports = new class YoutubeModule extends Module{
	constructor(){
		super({
			name: "youtube",
			version: "0.0.1",			
			category: "Musique",
			commands: [
				new Command({
					commands: ["ytb"], 
					description: "Mettre des musiques de youtube",
					neededPermission: "music",
					usage: "[lien/recherche]",
					minArgs: 1,
					onCommand: (message)=>{
						var link = message.content.split(" ").slice(1).join(" ");
						if(message.member.voiceChannel)
							discord.bot.getModule("musicPlayer").connectBot(message.channel, message.member.voiceChannel);
						switch(this.getLinkType(link)){
							case "video":
								this.addYoutube(message.guild.id, link, ((channel, message)=>{
									if(message)
										notification(message, channel, 10);
								}).bind(null, message.channel));
								break;
							case "playlist":
							
								break;
							case "search":
								this.searchYtb(message.guild.id, link, ((channel, message)=>{
									if(message)
										notification(message, channel, 10);
								}).bind(null, message.channel));
								break;
								
							
						}
					},
					subCommands: [
						new Command({
							commands: ["join", "connect"],
							description: "Connecte le bot dans votre salon",
							displayedInHelp: false,
							onCommand: (message)=>{
								discord.bot.getModule("musicPlayer").connectBot(message.channel, message.member.voiceChannel);
							}
						}),
						new Command({
							commands: ["next", "skip"],
							description: "Passe la musique",
							onCommand: (message)=>{
								if(discord.bot.getModule("musicPlayer").musiquePlayers[message.guild.id]){
									discord.bot.getModule("musicPlayer").musiquePlayers[message.guild.id].currentDispatcher.end();
								}
							}
						}),
						new Command({
							commands: ["disconnect", "leave"],
							description: "Déconnecte le bot du salon",
							displayedInHelp: false,
							onCommand: (message)=>{
								discord.bot.getModule("musicPlayer").disconnectBot(message.channel, message.guild);
							}
						})
					]
				})
			]	
		});
	}
	
	getLinkType(link){
		try{
			var path = new URL(link);
			if(path.hostname.endsWith("youtube.com")){
				if(path.pathname.toLowerCase() == "/watch"){
					return "video";
				}else if(path.pathname.toLowerCase() == "/playlist"){
					return "playlist";
				}			
			}else if(path.hostname.endsWith("youtu.be")){
				return "video";
			}else
				return "search";
		}catch(ex){
			return "search";	
		}
	}
	
	searchYtb(guildId, name, callback){	
		request("https://www.googleapis.com/youtube/v3/search?type=video&maxResults=1&part=snippet&key=AIzaSyCRxhX8tui3WoPPcRbajkVbDY2oWr0xop4&q=" + name, (info)=>{
			if(info["type"] == "loaded"){
				var response = JSON.parse(info["body"]);
				if(response["items"].length == 0){
					callback("Aucune vidéo trouvée");
					return;
				}				
				this.addYoutube(guildId, "http://www.youtube.com/watch?v=" + response["items"][0]["id"]["videoId"], callback)
			}else{
                console.log("Err search ytb");
                console.log(info["body"]);
				callback("Aucune vidéo trouvée: erreur de chargement");
			}
		});
	}

	addYoutube(guildId, ytbLink, callback){	
		ytdl.getInfo(ytbLink, { filter : 'audioonly' }).then((info)=>{
			discord.bot.getModule("musicPlayer").addMusic(guildId, new YoutubeSong(info.videoDetails.title, info.videoDetails.video_url, parseInt(info.videoDetails["length_seconds"])));
			callback()
       }).catch((err)=>{
            console.log(err)
            callback("Vidéo non trouvé");
            return; 
       });;
	}

	
};


class YoutubeSong{		
	constructor(name, info, size){
		this.name = name;
		this.info = info;
		this.size = size;
	}
	play(voiceConnection){		
		const stream = ytdl(this.info, { audioonly: true });
		return voiceConnection.playStream(stream, {volume: 1});
	}
	getLength(){
		return size;
	}
}
	
	