const fs = require('fs');
const path = require('path');

function getWebHandlers(dir=path.dirname(require.main.filename) + "/WebServer/webHandlers"){
	var webHandlers = {};
    var files = fs.readdirSync(dir);
    for (var i in files){
        var name = dir + '/' + files[i];
        if (fs.statSync(name).isDirectory()){
			if(fs.statSync(dir + '/' + files[i] + "/" + files[i] + ".js").isFile())
				webHandlers[files[i]] = new (require(dir + '/' + files[i] + "/" + files[i] + ".js"));
        }
    }
    return webHandlers;
}

module.exports = getWebHandlers;