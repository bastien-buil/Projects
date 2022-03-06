
const Result = require.main.require("./class/Result");

class UsersManager{
	constructor(){
		
	}
	registerUser(id, username, avatarHash, callback=()=>{}){
		database.count("users", {id: id}, (result)=>{
			if(result.success && (result.result == -1 || result.result >= 1)){
				callback(new Result(false, {errorId: "alreadyExistErr"}));	
				return;
			}
			
			database.insert("users", {id: id, username: username, avatarHash: avatarHash, roles: []}, function(result){
				callback(result);
			});
		});
	}
	updateUser(id, username, avatarHash, callback=()=>{}){
		database.update("users", {id: id}, {username: username, avatarHash: avatarHash}, function(that){
			return function(result){
				if(!result.success){	
					that.registerUser(id, username, avatarHash, callback);
					return;
				}
				callback(result);
			}
		}(this));
	}
	removeUser(id, callback=()=>{}){
		database.delete("users", {id: id}, function(result){
			callback(result);
		});
	}
	
	registerRole(roleName, description, callback=()=>{}){
		database.count("roles", {roleName: roleName}, (result)=>{
			if(result.success && (result.result == -1 || result.result >= 1)){
				callback(new Result(false, {errorId: "alreadyExistErr"}));	
				return;
			}
			
			database.insert("roles", {roleName: roleName, description:description, permissions: {}}, function(result){
				callback(result);
			});
		});
	}
	
	removeRole(roleName, callback=()=>{}){
		database.delete("roles", {roleName: roleName}, function(result){
			callback(result);
		});
	}
	
	addRole(userId, roleName, callback=()=>{}){
		database.addToList("users", {id: userId},{roles: roleName}, function(result){
			callback(result);
		});
	}
	
	removeRole(userId, roleName, callback=()=>{}){
		database.removeFromList("users", {id: userId},{roles: roleName}, function(result){
			callback(result);
		});
	}
	
}

module.exports = UsersManager;