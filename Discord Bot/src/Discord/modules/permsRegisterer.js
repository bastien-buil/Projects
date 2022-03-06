const Command = require.main.require("./Discord/class/Command");
const notification = require.main.require("./Discord/utils/notification");
const Module = require.main.require("./Discord/class/Module");

module.exports = new Module({
	name: "permsregisterer",
	version: "0.0.1",
	onload: () => {
		permissionsManager.registerPermission("admin.commands", false, "Les commandes d'admin");
		permissionsManager.registerPermission("nsfw", true, "Les commandes nsfw");
		permissionsManager.registerPermission("images.commands", true, "Les commandes affichant des images aléatoires");
		permissionsManager.registerPermission("games.shifumi", true, "Les commandes Shifumi");
		permissionsManager.registerPermission("music", true, "Les permissions pour gérer la musique");
		permissionsManager.registerPermission("tools.commands", true, "Les permissions pour les commandes outils");
	}
	
	
});
