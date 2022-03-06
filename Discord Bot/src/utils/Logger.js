var readline = require('readline');
var path = require("path");


class Logger {
	constructor(){
		process.title = "DarkBot";
		this.rl = readline.createInterface({
			input: process.stdin,
			output: process.stdout,
			completer: function(line){return [];}, //TO DO
			terminal: true,
			prompt: '> '
		});
		this.rl.prompt(true);

		this.rl.on('line', function(logger){
			return function(input) {
				var args = input.split(" ");
				switch(args[0].toLowerCase()) {
					case "clear":			
						logger.clear();
						logger.rl.prompt(true);
						break;
					default:	
						logger.warn('Commandes inconnues: ' + input, "Terminale");
				}
			}
		}(this));
	}
	log(message, module="DarkBot"){   
		this.clearLine();
		console.log(this.getTime() + ": [" + module + "] " + message);
		this.rl.prompt(true);
	}
	warn(message, module="DarkBot"){
		this.clearLine();
		console.log("\x1b[33m" + this.getTime() + ": [" + module + "] " + message + "\x1b[0m");	
		this.rl.prompt(true);
	}
	error(message, module="DarkBot"){
		this.clearLine();
		console.log("\x1b[31m" + this.getTime() + ": [" + module + "] " + message + "\x1b[0m");	
		this.rl.prompt(true);
	}
	clear(){
		readline.cursorTo(process.stdout, 0, 0);
		readline.clearScreenDown(process.stdout);
	}
	clearLine(){
		readline.cursorTo(process.stdout, 0);
	}
	getTime(){
		var date = new Date();
		return date.toLocaleDateString()+"T"+date.toLocaleTimeString();
	}
}


module.exports = Logger;

/*Couleurs 
https://stackoverflow.com/questions/9781218/how-to-change-node-jss-console-font-color	

Below you can find colors reference of text to command when running node.js application:

console.log('\x1b[36m%s\x1b[0m', 'I am cyan');  //cyan
console.log('\x1b[33m%s\x1b[0m', stringToMakeYellow);  //yellow

Note %s is where in the string (the second argument) gets injected. \x1b[0m resets the terminal color so it doesn't continue to be the chosen color anymore after this point.

Colors reference

Reset = "\x1b[0m"
Bright = "\x1b[1m"
Dim = "\x1b[2m"
Underscore = "\x1b[4m"
Blink = "\x1b[5m"
Reverse = "\x1b[7m"
Hidden = "\x1b[8m"

FgBlack = "\x1b[30m"
FgRed = "\x1b[31m"
FgGreen = "\x1b[32m"
FgYellow = "\x1b[33m"
FgBlue = "\x1b[34m"
FgMagenta = "\x1b[35m"
FgCyan = "\x1b[36m"
FgWhite = "\x1b[37m"

BgBlack = "\x1b[40m"
BgRed = "\x1b[41m"
BgGreen = "\x1b[42m"
BgYellow = "\x1b[43m"
BgBlue = "\x1b[44m"
BgMagenta = "\x1b[45m"
BgCyan = "\x1b[46m"
BgWhite = "\x1b[47m"
*/

