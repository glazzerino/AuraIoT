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
var index = 0;
var images = firebase.database().ref("CAMERA/IMAGE/");
var photos = []
sync();
console.log("started");
function sync() {
    images.once("value").then((snapshot) => {
        var data = snapshot.val();
        photos = [];
        for (key in data){
            // console.log(key);
            photos.push(data[key]);
        }
        index = photos.length-1;
        console.log("SYNC DONE");
        console.log(index);
        show_image(photos.length-1);

    });
}

function scroll_image(direction) {
    
    if (direction) {
        index += 1;
    } else {
        index -= 1;
    }
    if (index < 0 ) {
        index = photos.length - 1;
    }
    else if (index > photos.length-1) {
        index = 0;
    }
    console.log(index);
    // False for left, true for right
    
    show_image(index);
}

function show_image(i) {
    var picframe = document.getElementById("image_frame");
    picframe.innerHTML = "";

    var image = document.createElement("img");
    image.src = photos[i];
    picframe.appendChild(image);
}

function take_picture() {
    var trigger = firebase.database().ref("CAMERA/TRIGGER");
    trigger.set(true);
    console.log("Pic taken");
    sync();
    show_image(index);
}

function toggle_flash() {
    var flash_setting = document.getElementById("togBtn").checked;

    firebase.database().ref("CAMERA/FLASH").set({flash_setting});
    console.log("flash control toggled");
}