var firebaseConfig = {
    apiKey: "AIzaSyAobYQI2a9TjmKxlWPdbvBt9XYvn34tTF8",
    authDomain: "aura-iot-c6bc8.firebaseapp.com",
    databaseURL: "https://aura-iot-c6bc8.firebaseio.com",
    projectId: "aura-iot-c6bc8",
    storageBucket: "aura-iot-c6bc8.appspot.com",
    messagingSenderId: "878167968710",
    appId: "1:878167968710:web:dfc997fb592aedfa4000c7",
    measurementId: "G-R72RH86440"
};

// Initialize Firebase
firebase.initializeApp(firebaseConfig);
var database = firebase.database();

var lockStatus;
var lock_get = firebase.database().ref("GARAGE_DOOR/lock");
lock_get.on('value', (snapshot) =>{
    const data = snapshot.val();
    console.log(data);
    lockStatus = parseInt(data, 10);
    console.log(lockStatus);
})

var logs = firebase.database().ref("GARAGE_DOOR/DOOR_STATUS_log/");

var logList = []
logs.on("value", (snapshot) => {
    var data = snapshot.val();
    for (key in data){
        // console.log(key);
        logList.push(data[key]);
    }
});


checkUncheck();

function reload() {
    check();
}

function check() {
    let switchStatus = document.getElementById('togBtn');
    switchStatus.checked = true;
    console.log("paco es gei");    
}

function uncheck() {
    let switchStatus = document.getElementById('togBtn');
    switchStatus.checked = true;
    console.log("paco es gei");    
}


function lock() {
    var trigger = firebase.database().ref("GARAGE_DOOR/lock");
    trigger.set(0);
}

/*
function take_picture() {
    var trigger = firebase.database().ref("CAMERA/TRIGGER");
    trigger.set(true);
    console.log("Pic taken");
}*/