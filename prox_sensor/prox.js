
function setup() {
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
    
    // Get lock status
    var lockStatus;
    var lock_get = firebase.database().ref("GARAGE_DOOR/lock");
    lock_get.on('value', (snapshot) =>{
        const data = snapshot.val();
        lockStatus = parseInt(data, 10);
        //console.log(lockStatus);
        if (lockStatus == 1){
            check();
        } else {
            uncheck();
        }
    });

    // get log 
}





var logList = []
function loadValues(){
    var logs = firebase.database().ref("GARAGE_DOOR/DOOR_STATUS_log/");
    logs.on("value", (snapshot) => {
        var data = snapshot.val();
        for (key in data){
            logList.push(data[key]);
        }
    });
}

function displayValues () {
    loadValues();
    for (let i = 0; i < 1; i++) {
        console.log(logList[i]);
    };
}

function reload() {
    //get logs
    loadValues();
    displayValues();
}


function check() {
    let switchStatus = document.getElementById('togBtn');
    switchStatus.checked = true;  
}


function uncheck() {
    let switchStatus = document.getElementById('togBtn');
    switchStatus.checked = false;    
}


function lock() {
    let switchStatus = document.getElementById('togBtn');
    var trigger = firebase.database().ref("GARAGE_DOOR/lock");

    if (switchStatus.checked == true){
        trigger.set(1);
    } else {
        trigger.set(0);
    }
}

setup();
reload();
