class Module{
	constructor(args){
		this.name 		= args["name"];
		this.version 	= args["version"];
		this.onload 	= args["onload"] || (() =>{});
		this.commands 	= args["commands"] || [];
		this.category 	= args["category"] || "";
	}	
}

module.exports = Module;