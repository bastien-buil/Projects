
const Command = require.main.require("./Discord/class/Command");
const notification = require.main.require("./Discord/utils/notification");
const request = require.main.require("./utils/request");
const Module = require.main.require("./Discord/class/Module");

module.exports = new class MusicPlayerModule extends Module{
	constructor(){
		super({
			name: "musicPlayer",
			version: "0.0.1",			
			category: "Musique",	
			commands: [
				new Command({
					commands: ["music", "musique"], 
					description: "Gérer la musique",
					neededPermission: "music",
					subCommands: [
						new Command({
							commands: ["join", "connect"],
							description: "Connecte le bot dans votre salon",
							onCommand: (message)=>{
								this.connectBot(message.channel, message.member.voiceChannel)
							}
						}),
						new Command({
							commands: ["disconnect", "leave"],
							description: "Déconnecte le bot du salon",
							onCommand: (message)=>{
								this.disconnectBot(message.channel, message.guild)
							}
						}),
						new Command({
							commands: ["next", "skip"],
							description: "Passe la musique",
							onCommand: (message)=>{
								if(this.musiquePlayers[message.guild.id] && this.musiquePlayers[message.guild.id].currentDispatcher){
									this.musiquePlayers[message.guild.id].currentDispatcher.end();
								}
							}
						}),
						new Command({
							commands: ["list"],
							description: "Affiche les musiques en attente",
							displayedInHelp: false,
							onCommand: (message)=>{
								if(this.musiquePlayers[message.guild.id]){
									for(var i=0; i < this.musiquePlayers[message.guild.id].songQueue.length; i++){
										notification((i+1) + '. ' + this.musiquePlayers[message.guild.id].songQueue[i].name, message.channel, 10);										
								
									}
								}
							}
						}),
						new Command({
							commands: ["info"],
							description: "Affiche la musique actuelle",
							displayedInHelp: false,
							onCommand: (message)=>{
								if(this.musiquePlayers[message.guild.id]){
									if(this.musiquePlayers[message.guild.id].currentMusic)
										notification("Musique en cours: " + this.musiquePlayers[message.guild.id].currentMusic.name, message.channel, 10);
									else
										notification("Pas de musique en cours", message.channel, 10);									
								}
							}
						}),
						new Command({
							commands: ["time", "size"],
							description: "Affiche le temps restant",
							onCommand: (message)=>{
								if(this.musiquePlayers[message.guild.id]){
									if(this.musiquePlayers[message.guild.id].currentMusic && this.musiquePlayers[message.guild.id].currentDispatcher)
										notification('Time: ' + parseInt(this.musiquePlayers[message.guild.id].currentDispatcher.time/1000) + "/" + this.musiquePlayers[message.guild.id].currentMusic.size, message.channel, 10);										
								}
							}
						})
					]
				})
			]	
		});
		this.musiquePlayers = {};
	}
	
	connectBot(channel, voiceChannel){
		if (voiceChannel) {
			voiceChannel.join()
			.then(thatConnection => { // Connection is an instance of VoiceConnection
				if(!this.musiquePlayers[voiceChannel.guild.id])
					this.musiquePlayers[voiceChannel.guild.id] = new MusiquePlayer();
				
				this.musiquePlayers[voiceChannel.guild.id].connection = thatConnection;
			})
			.catch(function(err){
				console.log('err: ' + err);
			});
		} else {
			notification('You need to join a voice channel first!', channel, 10);
		}	
	}
	
	disconnectBot(channel, guild){
		for(var connection of channel.client.voiceConnections.values()){
			if(connection.channel.guild == guild)
				connection.disconnect()
		}
	}
	
	addMusic(guildId, song){
		if(this.musiquePlayers[guildId]){
			this.musiquePlayers[guildId].songQueue.push(song);
			if(!this.musiquePlayers[guildId].play)
				this.musiquePlayers[guildId].startMusic();
		}
	}
	
};


class MusiquePlayer{	
	
	constructor(){
		this.songQueue = [];
		this.autoPlay = false;
		this.repeat = false;
		this.play = false;
		this.currentMusic = null;
		this.currentDispatcher = null;
		this.connection = null;
	}
	
	
	startMusic(){
		if(this.play || !this.connection)
			return;
		this.play = true;
		this.playMusio()
	}

	playMusio(){
		
		if(this.songQueue.length < 1 && !(this.repeat && this.currentMusic != null)){
			if(this.autoPlay){
				this.addRandom(function(){
					this.playMusio();
				});						
			}else{
				this.play = false;
				this.currentMusic = null;
			}
			return;
		}
		if((!this.repeat) || this.currentMusic == null){
			this.currentMusic = this.songQueue[0];
			this.songQueue.shift();
		}
			
		this.currentDispatcher = this.currentMusic.play(this.connection)
		this.currentDispatcher.on("end", function(){
			this.currentDispatcher = null;
			this.playMusio();
		}.bind(this));

		this.currentDispatcher.player.once('error', console.trace);
	}


	
}
	

class Song{		
	constructor(name, type, info){
		this.name = name
		this.type = type;
		this.info = info;
		this.size = size;
	}
	play(voiceConnection){
		
	}
	getLength(){
		return this.size;
	}
	
}
	
	
	