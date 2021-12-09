function cgXAPIPlugin(userConfig) {
    return {
        name: 'cg-xapi-plugin',
        config: {
            trackEndpoint: userConfig.trackEndpoint,
            trackHomePage: userConfig.trackHomePage,
            trackToken: userConfig.trackToken,
        },
        initialize: ({config}) => {
            setLocalStorage(trackConfigKey, JSON.stringify(config))
        },
        track: ({payload}) => {
            let config = localStorage.getItem(trackConfigKey)
            if (!config) {
                return;
            }
            config = JSON.parse(config)
            let user = payload["userId"];
            let event = payload["event"];
            let timestamp = payload["meta"]["timestamp"] || new Date().getTime()
            let properties = payload["properties"]
            let verbID = normalizeURI("http://learning.educg.net/verb?word=" + event)
            let statement = {
                actor: {
                    objectType: "Agent",
                    account: {
                        homePage: config.trackHomePage,
                        name: user
                    }
                },
                verb: {
                    id: verbID
                },
                object: buildActivity(config, properties),
                timestamp: timestamp
            }
            postStatement(config.trackEndpoint, config.trackToken, statement)
        }
    }
}

let trackConfigKey = "trackConfig";

function setLocalStorage(key, value) {
    if (!value) {
        console.log("miss for" + key)
        return;
    }
    localStorage.setItem(key, value);
}

function postStatement(endpoint, token, statement) {
    if (!endpoint) {
        console.log("miss server endpoint")
        return;
    }
    let id = statement.id;
    if (!statement.id) {
        id = guid();
        statement.id = id;
    }
    let url = endpoint + "/xAPI/statements?statementId=" + id
    let formData = new FormData()
    formData.append("statement", JSON.stringify(statement))
    $.ajax({
        type: 'PUT',
        url: url,
        processData: false,
        contentType: false,
        data: formData,
        headers: {
            Authorization: "Bearer " + token
        },
        error: function (data) {
            console.log("error " + data);
        }
    })
}

function guid() {
    return 'xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx'.replace(/[xy]/g, function (c) {
        let r = Math.random() * 16 | 0,
            v = c === 'x' ? r : (r & 0x3 | 0x8);
        return v.toString(16);
    });
}

function buildActivityIdByPayload(domain, payload){
    let type = payload.type;
    let id = payload.id;
    return buildActivityId(domain, type, id)
}

function buildActivityId(domain, type, id) {
    let uri = domain + "/activity?type=" + type;
    if (id) {
        uri += "&id=" + id;
    }
    return normalizeURI(uri)
}

function buildActivity(config, payload){
    let type = payload.type;
    let id = payload.id;
    let name = payload.name;
    let parent = payload.parent;
    let activity = {
        objectType: "Activity",
        id: buildActivityId(config.trackHomePage, type, id)
    }
    if(name){
        activity["definition"] = {
            name :{
                zh : name
            }
        }
    }

    if(parent){
        activity["parentActivity"] = buildActivityIdByPayload(config.trackHomePage, parent)
    }
    return activity
}

function normalizeURI(uri){
    let ans = new URI(uri)
    ans.normalize()
    return ans.normalize().toString()
}