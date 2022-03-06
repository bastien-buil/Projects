
const Result = require.main.require("./class/Result");

class PermissionsManager{
	constructor(){
		
	}
	registerPermission(permission, defaultPerm, description="", callback=()=>{}){
		database.count("permissions", {permission: permission}, (result)=>{
			if(!result.success || result.result >= 1){				
				callback(new Result(false, {errorId: "alreadyExistErr"}));	
				return;
			}
			
			database.insert("permissions", {permission: permission, description:description, defaultPerm: defaultPerm}, function(result){
				callback(result);
			});
		});		
	}
	removePermission(permission, callback=()=>{}){
		database.delete("permissions", {permission: permission}, function(result){
			callback(result);
		});		
	}
	
	setPermToRole(role, permission, value, callback=()=>{}){
		database.count("permissions", {permission: permission}, (result)=>{
			if(!result.success || result.result < 1){				
				callback(new Result(false, {errorId: "unknownPermError"}));	
				return;
			}
			
			database.update("roles", {roleName: role}, {permissions: {[permission]: value}}, function(result){
				callback(result);
			});	
		});
	}
	
	setPermToCategory(category, role, permission, value, callback=()=>{}){
		database.count("permissions", {permission: permission}, (result)=>{
			if(!result.success || result.result < 1){				
				callback(new Result(false, {errorId: "unknownPermError"}));	
				return;
			}
			
			database.update("categories", {name: category}, {permissions: {[role]:{[permission]: value}}}, function(result){
				callback(result);
			});	
		});
	}
	
	testPermission(user, channel, permission, callback){
		var splitPerm = permission.split(".");
		function checkPermissionfromCategory(categories, roles, value=null, power=0){
			var category = categories.shift();
			database.find("categories", {name: category}, (result)=>{
				if(result.success && result.result){
					var permissions = result.result.permissions;
					for(var role of roles){
						for(var i = splitPerm.length; i > 0; i--){
							var perm = splitPerm.slice(0,i).join(".");
							if(typeof permissions[role][perm] != "undefined" && power < i){
								power = i;
								value = permissions[role][perm];
							}
						}
					}
				}
				if(categories.length > 1 && power != splitPerm.length)
					checkPermissionfromCategory(categories, roles, value, power);	
				else if (value != null)
					callback(new Result(true, {result: value}));
				else
					checkPermissionfromRole(roles);
					
			});		
		}
		
		function checkPermissionfromRole(roles, value=null, power=0){
			var role = roles.shift();
			database.find("roles", {roleName: role}, (result)=>{
				if(result.success && result.result){
					var permissions = result.result.permissions;
					for(var i = splitPerm.length; i > 0; i--){
						var perm = splitPerm.slice(0,i).join(".");
						if(typeof permissions[perm] != "undefined" && power < i){
							power = i;					
							value = permissions[perm];
						}
					}
				}
				if(roles.length > 1)
					checkPermissionfromRole(roles, value, power);	
				else if (value != null)
					callback(new Result(true, {result: value}));
				else
					getDefaultPermissionValue();
					
			});		
		}
		
		function getDefaultPermissionValue(){
			
			database.find("permissions", {permission: permission}, (result)=>{
				var value = null;
				if(result.success && result.result){
					value = result.result.defaultPerm;
				}
				callback(new Result(true, {result: value}));
					
			});		
		}
		
		database.find("users", {id: user}, (result)=>{
			var roles;
			
			if(result.success && result.result)
				roles = result.result.roles;			
			else
				roles = [];		
			
			database.find("channels", {id: channel}, (result)=>{
				var categories;
				if(result.success && result.result)
					categories = result.result.categories;				
				else
					categories = [];
				checkPermissionfromCategory(categories, roles);
			});
			
		});
		/*get user roles  DONE
		get channel categories  DONE 
		check perms for each player's role for each categories
		if there is a value stop 
		else
			check for each player's roles check if there is a permission
			if there is a value stop 
			else 
				check default permission value
		*/
	}
	
}

module.exports = PermissionsManager;