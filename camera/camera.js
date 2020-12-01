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
var photos = []
images.on("value", (snapshot) => {
    var data = snapshot.val();
    for (key in data){
        // console.log(key);
        photos.push(data[key]);
    }
});

function show() {
    
    for (i in photos) {
        console.log(photos[i]);
    }
    var picframe = document.getElementById("image_frame");
    var image = document.createElement("img");
    image.src = photos[photos.length-1];
    picframe.appendChild(image);
}

function take_picture() {
    var trigger = firebase.database().ref("CAMERA/TRIGGER");
    trigger.set(true);
    console.log("Pic taken");
}