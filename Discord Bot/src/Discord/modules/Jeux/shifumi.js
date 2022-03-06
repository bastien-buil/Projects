
const Command = require.main.require("./Discord/class/Command");
const notification = require.main.require("./Discord/utils/notification");
const request = require.main.require("./utils/request");
const Module = require.main.require("./Discord/class/Module");

module.exports = new class ShifumiModule extends Module{
	constructor(){
		super({
			name: "shifumi",
			version: "0.0.1",			
			category: "Jeux",	
			commands: [
				new Command({
					commands: ["shifumi"], 
					description: "Pour jouer à pierre feuille ciseaux",
					neededPermission: "games.shifumi",
					subCommands: [
						new Command({
							commands: ["pierre", "p"],
							description: "Joue pierre",
							onCommand: (message)=>{
								this.playShifumi(message.channel, message.member.nickname || message.member.user.username, 0);					
							}
						}),
						new Command({
							commands: ["feuille", "f"],
							description: "Joue feuille",
							onCommand: (message)=>{
								this.playShifumi(message.channel, message.member.nickname || message.member.user.username, 1);			
							}
						}),
						new Command({
							commands: ["ciseaux", "c"],
							description: "Joue ciseaux",
							onCommand: (message)=>{
								this.playShifumi(message.channel, message.member.nickname || message.member.user.username, 2);					
							}
						})
					]
				}),
				new Command({
					commands: ["pierre"], 
					description: "Pour jouer pierre à pierre feuille ciseaux",
					neededPermission: "games.shifumi",
					displayedInHelp: false,
					onCommand: (message)=>{
						this.playShifumi(message.channel, message.member.nickname || message.member.user.username, 0);		
					}			
				}),
				new Command({
					commands: ["feuille"], 
					description: "Pour jouer feuille à pierre feuille ciseaux",
					neededPermission: "games.shifumi",
					displayedInHelp: false,
					onCommand: (message)=>{
						this.playShifumi(message.channel, message.member.nickname || message.member.user.username, 1);						
					}			
				}),
				new Command({
					commands: ["ciseaux"], 
					description: "Pour jouer ciseaux à pierre feuille ciseaux",
					neededPermission: "games.shifumi",
					displayedInHelp: false,
					onCommand: (message)=>{
						this.playShifumi(message.channel, message.member.nickname || message.member.user.username, 2);		
					}			
				})
			]	
		})
	}
	
	playShifumi(channel, username, signe){ //Signe -> 0 => Piere, 1=>Feuille, 2=> ciseaux
		var state = 0;
		var message = channel.send("1").then((message) => {		
			var nextState = (function (){
				setTimeout((() => {
					switch(state){
						case 0:
							message.edit("2").then(nextState);					
						break;
						case 1:
							message.edit("3").then(nextState);					
						break;
						case 2:
							message.edit("Pierre").then(nextState);					
						break;
						case 3:
							message.edit("Feuille").then(nextState);					
						break;
						case 4:
							message.edit("ciseauxx").then(nextState);					
						break;
						case 5:
							var botSigne = Math.floor(Math.random() * 3);
							message.edit(username + " joue " + this.getSigneName(signe) + "\n Darkbot joue " + this.getSigneName(botSigne)).then(() => {
								message.delete(40 * 1000)
								if(botSigne == signe){
									notification("Egalité ! On en refait une " + username + " ?", message.channel, 40);
								}else if((signe == 0 && botSigne == 2) || (signe == 1 && botSigne == 0) || (signe == 2 && botSigne == 1)){//Bot Loose
									notification("Pff j'ai perdu, bravo " + username, message.channel, 40);
								}else{//bot win
									notification("J'ai gagné, c'était trop simple, on en refait une " + username + " ?", message.channel, 40);
								}
							});							
						break;
						
					}
					state++;
				}), 1000);			
			}).bind(this);
			nextState();
		});
	}

	getSigneName(signe){
		switch(signe){
			case 0:
				return "Pierre";
			case 1:
				return "Feuille";
			case 2:
				return "Ciseaux";
		}
	}
	
};

