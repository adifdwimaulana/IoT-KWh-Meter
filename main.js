var firebaseConfig = {
    apiKey: "AIzaSyD8D_nBRi2FhQMpgYHaezyyr853Giwc84U",
    authDomain: "kwhmeteriot.firebaseapp.com",
    databaseURL: "https://kwhmeteriot.firebaseio.com",
    projectId: "kwhmeteriot",
    storageBucket: "",
    messagingSenderId: "501144137233",
    appId: "1:501144137233:web:d75777ca86f9a630"
  };
// Initialize Firebase
firebase.initializeApp(firebaseConfig);
var database = firebase.database();

// Init DOM
var arusElement = document.getElementById("arus");
var currentKwhElement = document.getElementById("currentkwh");
var dayaElement = document.getElementById("daya");
var kwhElement = document.getElementById("kwh");
var teganganElement = document.getElementById("tegangan");

// Get Data from Firebase
database.ref("/").on("value", (snap) => {
    var total = snap.val();
    var arus = total["arus"];
    var currentKwh = total["currentkwh"];
    var daya = total["daya"];
    var kwh = total["kwh"];
    var tagihan = total["tagihan"];
    var tegangan = total["tegangan"];
    console.log(arus);
    console.log(currentKwh);
    console.log(daya);
    console.log(kwh);
    console.log(tagihan);
    console.log(tegangan);
    arusElement.innerText = arus;
    currentKwhElement.innerText = currentKwh;
    dayaElement.innerText = daya;
    kwhElement.innerText = kwh;
    teganganElement.innerText = tegangan;
});

// Form function
document.getElementById("submitForm").addEventListener("submit", formSubmit);

function formSubmit(e){
    e.preventDefault();
    document.getElementById("submitForm").reset();
}

// Send Data to Firebase
function sendValue() {
    var token = document.querySelector("#token").value;
    var token = token / 1352;
    database.ref("/").once("value", (snap) => {
        var total = snap.val();
        var arus = total["arus"];
        var currentKwh = total["currentkwh"];
        var daya = total["daya"];
        var kwh = total["kwh"];
        var tagihan = total["tagihan"];
        var tegangan = total["tegangan"];

        var pushData = parseFloat(token) + currentKwh;
        database.ref("/").set({
            arus: arus,
            currentkwh: pushData,
            daya: daya,
            kwh: kwh,
            tagihan: tagihan,
            tegangan: tegangan
        });
    });
}