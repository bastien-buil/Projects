const getModulesFiles = require.main.require('./utils/getModulesFiles');
const url = require('url');
const path = require('path');

var pluginsList = [];
const defaultPlugins = ["UrlManager.js"]

function isDefaultPlugin(plugin){ 
	for(let defaultPlugin of defaultPlugins){
		if(plugin.includes(defaultPlugin)) return true;
	}
	return false;
}

function loadPlugins(){
	var plugins = getModulesFiles(path.dirname(require.main.filename) + "/utils/imagesExtractor/plugins");
	
	for(let plugin of plugins){
		if(isDefaultPlugin(plugin))
			pluginsList.unshift(new (require(plugin))());			
		else
			pluginsList.push(new (require(plugin))());		
	}
}

loadPlugins();

function extractImage(link, callback){	
	linkLower = link.toLowerCase()
	var isExtracted = false;
	console.log(link);
	for(var plugin of pluginsList){
		if(plugin.isCompatible(linkLower)){
			plugin.getImage(link, callback);
			isExtracted = true;
			break;
		}	
	}
	
	if(!isExtracted) callback(null);
}

module.exports = extractImage;