
class Result{
	constructor(success, args={}){
		this.success = success;
		this.errorId = args["errorId"] || "";
		this.description = args["description"] || "";
		this.err = args["err"] || null;
		this.result = args["result"] || null;
	}
	toString(){
		if(this.success)
			return "Success";
		else{
			var message = "Error";
			if(this.errorId)
				message += ", error id: " + this.errorId;
			if(this.description)
				message += ", info: " + this.description;
				
			return message;
		}
	}
}

module.exports = Result;