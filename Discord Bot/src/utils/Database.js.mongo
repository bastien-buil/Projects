var fs = require('fs');
var path = require('path');
var MongoClient = require('mongodb').MongoClient

const Result = require.main.require("./class/Result");

class Database {
	constructor(){
		this.dbUrl = 'mongodb://localhost:27017/';
		MongoClient.connect(this.dbUrl, { useNewUrlParser: true }, function(dtbase){
			return function(err, client) {
				if(err){
					logger.error(err, "Database")
					logger.error("Can't connect to server", "Database")
					process.exit(1);
					return;
				}
				logger.log("Connected successfully to database", "Database");
				dtbase.db = client.db('darkmillions');


			}
		}(this));
	}


	find(file, data, callback){
		if(!this.db){
			setTimeout(()=>{this.find(file, data, callback)}, 20);
			return;
		}
		this.db.collection(file).findOne(data, function(err, doc){
			if(err){
				callback(new Result(false, {err: err, errorId: "dbErr"}));
			}else
				callback(new Result(true, {result: doc}));
		});
	}
	findAll(file, data, callback){
		if(!this.db){
			setTimeout(()=>{this.findAll(file, data, callback)}, 20);
			return;
		}
		this.db.collection(file).find(data).toArray(function(err, list){
			if(err){
				callback(new Result(false, {err: err, errorId: "dbErr"}));
			}else
				callback(new Result(true, {result: list}));
		});
	}
	insert(file, data, callback){
		if(!this.db){
			setTimeout(()=>{this.insert(file, data, callback)}, 20);
			return;
		}
		this.db.collection(file).insertOne(data, function(err, result){
			if(err){
				callback(new Result(false, {err: err, errorId: "dbErr"}));
			}else
				callback(new Result(true));
		});
	}
	count(file, data, callback){
		if(!this.db){
			setTimeout(()=>{this.count(file, data, callback)}, 20);
			return;
		}
		this.db.collection(file).countDocuments(data, function(err, count){
			if(err){
				callback(new Result(false, {err: err, errorId: "dbErr"}));
			}else
				callback(new Result(true, {result: count}));
		});
	}
	delete(file, data, callback){
		if(!this.db){
			setTimeout(()=>{this.delete(file, data, callback)}, 20);
			return;
		}
		this.db.collection(file).deleteMany(data, function(err, result){
			if(err){
				callback(new Result(false, {err: err, errorId: "dbErr"}));
			}else
				callback(new Result(true));
		});
	}
	update(file, filter, data, callback){
		if(!this.db){
			setTimeout(()=>{this.update(file, filter, data, callback)}, 20);
			return;
		}
		this.db.collection(file).updateMany(filter, {$set: data}, function(err, result){
			if(err){
				callback(new Result(false, {err: err, errorId: "dbErr"}));
			}else{
				if(result.result.n == 0)
					callback(new Result(false, {errorId: "noUpdateErr"}));
				else
					callback(new Result(true));
			}
		});
	}
	addToList(file, filter, data, callback){
		if(!this.db){
			setTimeout(()=>{this.addToList(file, filter, data, callback)}, 20);
			return;
		}
		this.db.collection(file).updateMany(filter, {$addToSet: data}, function(err, result){
			if(err){
				callback(new Result(false, {err: err, errorId: "dbErr"}));
			}else{
				if(result.result.n == 0)
					callback(new Result(false, {errorId: "noUpdateErr"}));
				else
					callback(new Result(true));
			}
		});
	}
	removeFromList(file, filter, data, callback){
		if(!this.db){
			setTimeout(()=>{this.removeFromList(file, filter, data, callback)}, 20);
			return;
		}
		this.db.collection(file).updateMany(filter, {$pull: data}, function(err, result){
			if(err){
				callback(new Result(false, {err: err, errorId: "dbErr"}));
			}else{
				if(result.result.n == 0)
					callback(new Result(false, {errorId: "noUpdateErr"}));
				else
					callback(new Result(true));
			}
		});
	}

}


module.exports = Database;
