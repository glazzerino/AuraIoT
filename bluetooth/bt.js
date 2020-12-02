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

var isClose;
var getBT = firebase.database().ref("Cerrojo/Storage");
var dataList;

getBT.on('child_added', (snapshot) =>{
    firebase.database().ref("Cerrojo/Proximidad").once("value").then((snapshot) => {
        isClose = snapshot.val();

        if(isClose){
            document.getElementById("A").textContent = "La puerta esta abierta"; 
        }
        else if(!isClose){
            document.getElementById("A").textContent = "La puerta esta cerrada";
        }

        
    });

    getBT.on('value', (snapshot) => {
        const data = snapshot.val();
        dataList = [];

        for (element in data){
            dataList.push(data[element]);
        }

        var tbodyRef = document.getElementById('T').getElementsByTagName('tbody')[0];
        tbodyRef.innerHTML = "";
        for (let i = dataList.length-1; i > -1; i--) {
            var log = dataList[i].split(", ");
            var newRow = tbodyRef.insertRow();
            var newCell = newRow.insertCell(); var newCell1 = newRow.insertCell();
            var newText = document.createTextNode(log[0]); var newText1 = document.createTextNode(log[1]);
            newCell.appendChild(newText); newCell1.appendChild(newText1);
        };   

    });
    

    

});