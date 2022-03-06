
var crypto = require("crypto");
var buffer = require("buffer");
var querystring = require("querystring");
const randomString = require.main.require("./WebServer/utils/randomString");

const Result = require.main.require("./class/Result");

var maxNumberOfIp = 10;

class SessionsManager{
	constructor(masterCookieKey, io){
		this.masterCookieKey = masterCookieKey;
		//database.db.collection("users").createIndex("username", {unique: true}, function(err, result){});
		//database.db.collection("usersIpList").createIndex("username", {unique: true}, function(err, result){});
		/*io.use(function(userManager){
			return function(socket, next){
				var ip = socket.conn.remoteAddress;	
				var cookies = querystring.parse(socket.request.headers.cookie, "; ");
				if(!cookies.sessionId){
					next(new Error('authError'));
					return;
				}
					
				
				if(!userManager.verifyCookieSignature(cookies.sessionId)){
					next(new Error('authError'));
					return;
				}
				
				userManager.verifyCookieSessions(cookies.sessionId, ip, function(success, username){
					if(success){
						socket.customUser = {username: username};
						next();
					}
					else
						next(new Error('authError'));
					
				});
				
				
			}
		}(this));*/
	}
	registerUser(username, password, role, callback){
		username = username.toLowerCase();
		var salt = crypto.randomBytes(32).toString('hex');
		var key = crypto.pbkdf2Sync(password, salt, 100000, 64, 'sha512');
		database.insert("users", {username: username, hash: key.toString('hex'), salt: salt, role: role}, function(result){
			callback(result);
		});
	}
	removeUser(username, callback){
		var success = true;
		database.delete("users", {username: username}, function(result){		
			success = success || result.success;
			database.delete("usersIpList", {username: username}, function(result){				
				success = success || result.success;
				database.delete("sessions", {username: username}, function(result){		
					success = success || result.success;
					callback(new Result(success));
				});
			});
		});
	}
	
	
	removeCookieId(token, ip, callback=()=>{}){//RENAME TO DISCONNECT ????
		var splitToken = token.split(".");
		if(splitToken.length != 2){
			callback(new Result(false, {errorId: "invalidTokenErr"}));
			return;
		}
		
		var hash = crypto.createHmac('sha256', this.masterCookieKey);
		hash.update(splitToken[0], "base64");
		
		var value = hash.digest('hex');	
		if(value != splitToken[1]){
			callback(new Result(false, {errorId: "invalidSignatureErr"}));
			return;
		}
		
		var infos = Buffer.from(splitToken[0], "base64").toString("utf8");
		infos = JSON.parse(infos);
		if(infos.id){
			database.delete("sessions", {id: infos.id, ip: ip}, function(result){});
			database.delete("sessions", {id: infos.id, sessionId: infos.session}, function(result){});
			callback(new Result(true));
		}else
			callback(new Result(false, {errorId: "noIdErr"}));
		
	}
	connect(id, username, discordToken , ip, callback){//TO RENAME -> CONNECT ???? + register discordToken
		database.find("usersIpList", {id: id, ip: ip}, function(that){
			return function(result){
				
				function next() {
					var session = randomString(24);
								
					var infos = {id: id, session: session, username: username};
					var base64Info = Buffer.from(JSON.stringify(infos), "utf8").toString("base64");
					
					var hash = crypto.createHmac('sha256', that.masterCookieKey);
					hash.update(JSON.stringify(infos));
					var value = hash.digest('hex');	
					var cookie =  base64Info + "." + value;			
					var date = new Date(Date.now() + 24 * 60 * 60 * 1000);
					
					database.delete("sessions", {id: id, ip: ip}, function(result){
						database.insert("sessions", {id: id, ip: ip, expires: date, sessionId:session}, function(result){
							if(result.success == false){	
								callback(result);
							}else
								callback(new Result(true, {result: {cookie, date}}));
						});
					});			
				}
				
				if(result.result == null){
					database.count("usersIpList", {id: id}, function(result){
						if(!result.success || result.result == -1 || maxNumberOfIp <= result.result){
							callback(new Result(false, {errorId: "maxIpErr"}));
							return;						
						}else{
							database.insert("usersIpList", {id: id, ip: ip, authorised:true}, function(result){});
							next();
						}						
					});
				}else if(!result.result.authorised){
					callback(new Result(false, {errorId: "ipBannedErr"}));
					return;
				}else
					next();
							
			}
		}(this));
	}
	verifyToken(token, ip, callback){
		var splitToken = token.split(".");
		if(splitToken.length != 2){
			callback(new Result(false, {errorId: "invalidTokenErr"}));
			return;
		}
		
		var hash = crypto.createHmac('sha256', this.masterCookieKey);
		hash.update(splitToken[0], "base64");
		var value = hash.digest('hex');	
		if(value != splitToken[1])
			callback(new Result(false, {errorId: "invalidSignatureErr"}));
				
		var infos = Buffer.from(splitToken[0], "base64").toString("utf8");
		infos = JSON.parse(infos);
		console.log(infos)
		if(infos.id && infos.session && infos.username){
			var id = infos.id.toLowerCase();
			var sessionId = infos.session;
			database.delete("sessions", {id: id, expires: {$lt: new Date(Date.now())}}, function(result){
				database.find("sessions", {id: id, ip: ip, expires: {$gt: new Date(Date.now())}, sessionId:sessionId}, function(result){
					if(result.result){
						callback(new Result(true, {result: infos}));
					}else{
						callback(new Result(false, {errorId: "unknownSessionErr"}));
					}
				});
			});
		}else
			callback(new Result(false, {errorId: "noInvalidFormatErr"}));
	}
}


module.exports = SessionsManager;