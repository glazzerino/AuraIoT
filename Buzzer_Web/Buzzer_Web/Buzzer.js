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

var act = firebase.database().ref("Buzzer/Activate/");
var state = firebase.database().ref("Buzzer/State/")
state.on('value', (snapshot)=>{
    const data = snapshot.val();
    if(data == "On"){
        document.getElementById("E").innerHTML = "Estado: Encendido";
    }
    else{
        document.getElementById("E").innerHTML = "Estado: Apagado";
    }
});

act.on('value', (snapshot)=>{
    const data = snapshot.val();
    if(data == true){
        document.getElementById("A").innerHTML = "Activado";
    }
    else{
        document.getElementById("A").innerHTML = "Desactivado";
    }
});

function activate() {
    var aux = act.set(true);
    console.log("Turn On");
}

function desactivate() {
    var aux = act.set(false);
    console.log("Turn Off");
}
