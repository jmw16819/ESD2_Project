/*
https://github.com/jmw16819/ESD2_Project
WeFlow - a water leak detection system
by Jordyn Washington 2021 - MIT License
*/

/* Javascript code for the monitor webpage client of the WeFlow system.
The Python server will feed in state data of the WeFlow as a dictionary,
and the JS code will parse through the object and change the HTML of the
webpage and send messages back to the server as needed
*/

var b3 = document.getElementById("b3"); // Button that controls state of valve 3
var b1 = document.getElementById("b1"); // Button that controls state of valve 1
var b2 = document.getElementById("b2"); // Button that controls state of valve 2
var v1_state = false; // valve 1 is open
var v2_state = false; // valve 2 is open
var v3_state = false; // valve 3 is open
var red = "";
var green = "";
var yellow = "";
var ws;

function init() {
    localhost = window.location.hostname;
    // Define a new WebSocket
    ws = new WebSocket("ws://" + localhost + ":8888/pagesocket"); 

    ws.onopen = function () {
        // Make request to the Websocket
        ws.send("Ready to stream data."); 
    };

    ws.onmessage = function (evt) {
        data_obj = JSON.parse(evt.data);
        onMessage(data_obj);
    };
}

function onMessage(msg) {
    // The msg sent by the server will be a dictionary containing all the data coming from the ESP32
    // clients about the system. It will be a string, and in the onmessage callback it was converted
    // into a JS object so it can be iterated through

    // The order of the data in the object is known, so each entry will be iterated through in order

    // s1 pressure: display it in the table on the webpage
    document.getElementById("s1_data").innerHTML = msg['sensors']['s1']['pressure'];

    // s1 LEDs: save them all in variables and adjust webpage messages based on valve states
    red = msg['sensors']['s1']['LED']['red'];
    green = msg['sensors']['s1']['LED']['green'];
    yellow = msg['sensors']['s1']['LED']['yellow'];
    if (green=="1" && v1_state==false){
        document.getElementById("s1_mess").innerHTML = 
        "Section 1 pressure optimal, everything looks fine.";
    }
    else if (green=="1" && v1_state==true){
        document.getElementById("s1_mess").innerHTML = 
        "Section 1 pressure optimal. Please reopen Section 1.";
    }
    else if (yellow=="1" && v1_state==false){
        document.getElementById("s1_mess").innerHTML =
        "Section 1 pressure a little low but still okay. No action needed.";
    }
    else if (yellow=="1" && v1_state==true){
        document.getElementById("s1_mess").innerHTML =
        "Section 1 pressure a little low. If making repairs continue, otherwise open section to increase flow.";
    }
    else if (red=="1" && v1_state==false){
        document.getElementById("s1_mess").innerHTML =
        "Section 1 pressure too low. Possible leak in this area. Section 1 closing. Please look into the issue.";
        ws.send("Valve 1 change: Close valve 1");
        change_button(b1,"open");
    }
    else if (red=="1" && v1_state==true){
        document.getElementById("s1_mess").innerHTML =
        "Section 1 closed. Please open when issue is resolved.";
    }

    // s2 pressure: display it in the table on the webpage
    document.getElementById("s2_data").innerHTML = msg['sensors']['s2']['pressure'];

    // s2 LEDs: save them all in variables and adjust webpage messages based on valve states
    red = msg['sensors']['s2']['LED']['red'];
    green = msg['sensors']['s2']['LED']['green'];
    yellow = msg['sensors']['s2']['LED']['yellow'];
    if (green=="1" && v2_state==false){
        document.getElementById("s2_mess").innerHTML = 
        "Section 2 pressure optimal, everything looks fine.";
    }
    else if (green=="1" && v2_state==true){
        document.getElementById("s2_mess").innerHTML = 
        "Section 2 pressure optimal. Please reopen Section 2.";
    }
    else if (yellow=="1" && v2_state==false){
        document.getElementById("s2_mess").innerHTML =
        "Section 2 pressure a little low but still okay. No action needed.";
    }
    else if (yellow=="1" && v2_state==true){
        document.getElementById("s2_mess").innerHTML =
        "Section 2 pressure a little low. If making repairs continue, otherwise open section to increase flow.";
    }
    else if (red=="1" && v2_state==false){
        document.getElementById("s2_mess").innerHTML =
        "Section 2 pressure too low. Possible leak in this area. Section 2 closing. Please look into the issue.";
        ws.send("Valve 2 change: Close valve 2");
        change_button(b2,"open");
    }
    else if (red=="1" && v2_state==true){
        document.getElementById("s2_mess").innerHTML =
        "Section 2 closed. Please open when issue is resolved.";
    }

    // s3 pressure: display it in the table on the webpage
    document.getElementById("s3_data").innerHTML = msg['sensors']['s3']['pressure'];

    // s3 LEDs: save them all in variables and adjust webpage messages based on valve states
    red = msg['sensors']['s3']['LED']['red'];
    green = msg['sensors']['s3']['LED']['green'];
    yellow = msg['sensors']['s3']['LED']['yellow'];
    if (green=="1" && v3_state==false){
        document.getElementById("s3_mess").innerHTML = 
        "Section 3 pressure optimal, everything looks fine.";
    }
    else if (green=="1" && v3_state==true){
        document.getElementById("s3_mess").innerHTML = 
        "Section 3 pressure optimal. Please reopen Section 3.";
    }
    else if (yellow=="1" && v3_state==false){
        document.getElementById("s3_mess").innerHTML =
        "Section 3 pressure a little low but still okay. No action needed.";
    }
    else if (yellow=="1" && v3_state==true){
        document.getElementById("s3_mess").innerHTML =
        "Section 3 pressure a little low. If making repairs continue, otherwise open section to increase flow.";
    }
    else if (red=="1" && v3_state==false){
        document.getElementById("s3_mess").innerHTML =
        "Section 3 pressure too low. Possible leak in this area. Section 3 closing. Please look into the issue.";
        ws.send("Valve 3 change: Close valve 3");
        change_button(b3,"open");
    }
    else if (red=="1" && v3_state==true){
        document.getElementById("s3_mess").innerHTML =
        "Section 3 closed. Please open when issue is resolved.";
    }
    
    // v1: update the valve state variable depending on the passed in data
    if (msg['valves']['v1'] == "open"){
        v1_state = false;
    }
    else if (msg['valves']['v1'] == "closed"){
        v1_state = true;
    }
    // v2: update the valve state variable depending on the passed in data
    if (msg['valves']['v2'] == "open"){
        v2_state = false;
    }
    else if (msg['valves']['v2'] == "closed"){
        v2_state = true;
    }
    // v3: update the valve state variable depending on the passed in data
    if (msg['valves']['v3'] == "open"){
        v3_state = false;
    }
    else if (msg['valves']['v3'] == "closed"){
        v3_state = true;
    }
}

// Function to change the HTML of the buttons when the they're changed by a
// click or change from the code
function change_button(b, state){
    b.disabled = true;
        if (b==b1){
            if (state=="open"){
                // Change the button on page to open
                b.innerHTML = "Open Section 1";
                b.title = "Open section 1";
            }
            else if (state=="close"){
                // Change the button on page to close
                b.innerHTML = "Close Section 1";
                b.title = "Close off section 1";
            }
        }
        else if (b==b2){
            if (state=="open"){
                // Change the button on page to open
                b.innerHTML = "Open Section 2";
                b.title = "Open section 2";
            }
            else if (state=="close"){
                // Change the button on page to close
                b.innerHTML = "Close Section 2";
                b.title = "Close off section 2";
            }
        }
        else if (b==b3){
            if (state=="open"){
                // Change the button on page to open
                b.innerHTML = "Open Section 3";
                b.title = "Open section 3";
            }
            else if (state=="close"){
                // Change the button on page to close
                b.innerHTML = "Close Section 3";
                b.title = "Close off section 3";
            }
        }
    b.disabled = false;
}

// Button 1 interrupt
b1.addEventListener(
    'click', function(){
        b1.disabled = true;
        if (b1.innerHTML == "Open Section 1"){
            ws.send("Valve 1 change: Open valve 1");
            change_button(b1, "close");
        }
        else if (b1.innerHTML == "Close Section 1"){
            ws.send("Valve 1 change: Close valve 1");
            change_button(b1, "open");
        }
        b1.disabled = false;
    }
);

// Button 2 interrupt
b2.addEventListener(
    'click', function(){
        b2.disabled = true;
        if (b2.innerHTML == "Open Section 2"){
            ws.send("Valve 2 change: Open valve 2");
            change_button(b2, "close");
        }
        else if (b2.innerHTML == "Close Section 2"){
            ws.send("Valve 2 change: Close valve 2");
            change_button(b2, "open");
        }
        b2.disabled = false;
    }
);

// Button 3 interrupt
b3.addEventListener(
    'click', function(){
        b3.disabled = true;
        if (b3.innerHTML == "Open Section 3"){
            ws.send("Valve 3 change: Open valve 3");
            change_button(b3, "close");
        }
        else if(b3.innerHTML == "Close Section 3"){
            ws.send("Valve 3 change: Close valve 3");
            change_button(b3, "open");
        }
        b3.disabled = false;
    }
);

// Open the WebSocket as soon as the page loads
window.addEventListener("load", init, false)
