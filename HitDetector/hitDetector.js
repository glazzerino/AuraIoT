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

//Initialize Firebase
firebase.initializeApp(firebaseConfig);
var database = firebase.database();

function toggleOff() {
    var trigger = firebase.database().ref("/HitDetector/Alarm");
    trigger.set(false);
    console.log("Alarm set off");
}

function toggleOn() {
    var trigger = firebase.database().ref("/HitDetector/Alarm");
    trigger.set(true);
    console.log("Alarm set on");
}

var logList = []
function showTable() {
    var logs = firebase.database().ref("/HitDetector/Alarm_Log");
    logList = [];
    logs.on("value", (snapshot) => {
        var data = snapshot.val();
        for (key in data){
            logList.push(data[key]);
        }
    });
    setTimeout(function displayValues () {
        for (let i = logList.length-1; i > -1; i--) {
            console.log(logList[i]);
        };
    }, 250);
    setTimeout(function addToTable(){
        var tbodyRef = document.getElementById('log').getElementsByTagName('tbody')[0];
        tbodyRef.innerHTML = "";
        for (let i = logList.length-1; i > -1; i--) {
            var data = logList[i];
            var newRow = tbodyRef.insertRow();
            var newCell = newRow.insertCell();
            var newText = document.createTextNode(data);
            newCell.appendChild(newText);
        };      
    }, 500);
}