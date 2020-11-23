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
const facundo = "FACUNDO es gei";
console.log(facundo);

var camera = firebase.database().ref("CAMERA/IMAGE/");

var control = false;
camera.on("child_added", (snapshot) => {
    if (control) {
        const data = snapshot.val();
        console.log(data);
    }
control = true;
});
