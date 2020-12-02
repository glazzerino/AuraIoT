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



getBT.on('child_added', (snapshot) =>{
    const data = snapshot.val();
    
    firebase.database().ref("Cerrojo/Proximidad").once("value").then((snapshot) => {
        isClose = snapshot.val();

        if(isClose){
            document.getElementById("A").textContent = "Abierto"; 
        }
        else if(!isClose){
            document.getElementById("A").textContent = "Cerrado";
        }
    });
});