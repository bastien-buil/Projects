


export default class Stream {
    constructor(name, webLink, type, info) {
        this.name = name;
        this.online = false;
        this.webLink = webLink;
        this.type = type;
        this.info = info;
        this.streamInfo = {
            title: "",
            gameName: ""
        };
    }


    toObject(){
        return {name: this.name, online: this.online, webLink: this.webLink, type: this.type, info: this.info};
    }

    // noinspection JSUnusedGlobalSymbols
    toJSON(_key){
        return JSON.stringify(this.toObject());
    }



    static objectToClass(obj){
        return new Stream(obj.name, obj.webLink, obj.type, obj.info);
    }


    static listOfObjectToListOfClass(lst){
        let newObj = {};
        for(let obj of lst){
            newObj.push(Stream.objectToClass(obj));
        }
        return newObj;
    }

}