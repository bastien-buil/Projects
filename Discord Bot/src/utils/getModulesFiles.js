const fs = require('fs');
const path = require('path');

function getModulesFiles(dir=path.dirname(require.main.filename) + "/Discord/modules", _files=[]){
    var files = fs.readdirSync(dir);
    for (var i in files){
        var name = dir + '/' + files[i];
        if (fs.statSync(name).isDirectory()){
            getModulesFiles(name, _files);
        }else if(name.toLowerCase().endsWith(".js")){
            _files.push(name);
        }
    }
    return _files;
}

module.exports = getModulesFiles;