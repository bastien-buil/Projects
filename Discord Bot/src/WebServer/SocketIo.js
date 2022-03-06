var socketio = require('socket.io');
var fs = require('fs');




class SocketIoServer{
	constructor(){
		this.isInit = false;	
		
	}
	
	init(serverWeb) {		
		this.io = socketio(serverWeb);
		
		this.io.on('connection', function(socket){
			socket.emit('logged');
		});
		
		this.isInit = true;
		logger.log("Server socket.io loaded", "Socket.io");
		return this;

	}

	 registerIo(callback) {
		if(this.isInit == false){
			setTimeout(() => {this.registerIo(callback);}, 100)
		}else{
			callback(this.io);
		}

	}
}



module.exports = new SocketIoServer();
