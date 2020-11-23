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

var images = firebase.database().ref("CAMERA/IMAGE/");

images.on("child_added", (snapshot) => {
    const data = snapshot.val();
    console.log(data);
});

function take_picture() {
    var trigger = firebase.database().ref("CAMERA/TRIGGER");
    trigger.set(true);
    console.log("Pic taken");

}

function load_cam_iframe() {
    var main = document.getElementById("main");
    var iframe = document.createElement("iframe");
    main.innerHTML = "";
    iframe.src = "cam.html";
    main.appendChild(iframe);
}