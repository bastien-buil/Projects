
const Result = require.main.require("./class/Result");

class ChannelsManager{
	constructor(){
		
	}
	registerCategory(name, description, callback=()=>{}){
		database.count("categories", {name: name},  (result)=>{
			if(result.success && (result.result == -1 || result.result >= 1)){
				callback(new Result(false, {errorId: "alreadyExistErr"}));	
				return;
			}
			
			database.insert("categories", {name: name, description: description, permissions: {}}, function(result){
				callback(result);
			});
		});
	}
	removeCategory(name, callback=()=>{}){
		database.delete("categories", {name: name}, function(result){
			callback(result);
		});
	}
	registerChannel(channelId, callback=()=>{}){
		database.count("channels", {id: channelId}, (result)=>{
			if(result.success && (result.result == -1 || result.result >= 1)){				
				callback(new Result(false, {errorId: "alreadyExistErr"}));	
				return;
			}
			
			database.insert("channels", {id: channelId, categories: []}, function(result){
				callback(result);
			});
		});
	}
	
	addCategoryToChannel(channelId, categoryName, callback=()=>{}){
		this.registerChannel(channelId, ()=>{
			database.addToList("channels", {id: channelId}, {categories: categoryName}, function(result){
				callback(result);
			});	
		});
	}
	removeCategoryFromChannel(){
		this.registerChannel(channelId, ()=>{
			database.removeFromList("channels", {id: channelId}, {categories: categoryName}, function(result){
				callback(result);
			});			
		});
	}
}

module.exports = ChannelsManager;