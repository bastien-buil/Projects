
class Logger {
	constructor(maxLine = 100, mustLog = true) {
		this.logList = [];
		this.maxLine = maxLine;
		this.mustLog = mustLog;
		this.log("Logger initialized");
	}
	log(txt){
		if(this.mustLog)
			this.logList.push(txt.toString())
		while(this.logList.length >= this.maxLine){
			this.logList.splice(0, 1);
		}
	}
	getLogs(){		
		return JSON.parse(JSON.stringify(this.logList));
	}
}






