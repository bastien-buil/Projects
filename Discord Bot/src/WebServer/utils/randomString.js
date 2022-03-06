var	randomStringCharacters= "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

function randomString(length){
	var randomString = "";
	for(var i = 0; i < length; i++){
		randomString += randomStringCharacters[Math.floor(Math.random() * randomStringCharacters.length)];
	} 
	return randomString;
}

module.exports = randomString;