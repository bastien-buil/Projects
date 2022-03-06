function notification(messageContent, channel, timeInSec){
	if(!timeInSec)
		timeInSec = 60;
	channel.send(messageContent).then(message => {
		setTimeout(()=>{
			if(message.deletable)
				message.delete(5);	
			else
				console.log("Can't delete notif");
		}, timeInSec*1000)	
	});	
}

module.exports = notification;