
const logger = new (require('./utils/Logger'))();
global.logger = logger;

const database = new (require('./utils/Database'))();
global.database = database;

const webServer = new (require('./WebServer/WebServer'))();
global.webServer = webServer;

const permissionsManager = new (require('./managers/PermissionsManager'))();
global.permissionsManager = permissionsManager;

const usersManager = new (require('./managers/UsersManager'))();
global.usersManager = usersManager;

const channelsManager = new (require('./Discord/managers/ChannelsManager'))();
global.channelsManager = channelsManager;

const discord = new (require('./Discord/Discord'))();
global.discord = discord;
