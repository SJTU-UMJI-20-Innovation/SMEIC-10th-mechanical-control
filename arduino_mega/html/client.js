"use strict"
const socket = new WebSocket('ws://192.168.1.3:2051');
const inputArea = document.getElementById("inputArea");
const outputArea = document.getElementById("outputArea");

const intervalTime = 100;//毫秒
var registerTime = 0;//毫秒
var timer = [];

function sendCmdList() {
    // console.log("hi");
    // prompt("hello");
    // console.log(inputArea.value);
    outputArea.value = "processing\n";
    var cmdList = inputArea.value.split("\n");
    for (var i = 0; i < cmdList.length; ++i) {
        if (!checkSend(cmdList[i], i)) {
            for (var j = 0; j <= i - 1; ++j) {
                clearTimeout(timer[j]);
            }
            break;
        }
    }
    if (outputArea.value === "processing\n")
        outputArea.value = "";
}

function checkSend(cmd, line){
    var message = cmd.split(" ");
    // console.log(message[0]);
    switch (message[0]) {
        case "stop":
            if (message[1] == "MEGA")
            registerSend(cmd);
            break;
        case "cgSg": case "changeSignal":
            if (message.length != 2) {
                outputArea.value += "Error in changeSignal on line " + line + ": wrong input number\n";
                return false;
            }
            break;
        case "rtRtBs": case "rotateRotationBase":
            if (message[1] != "0" && message[1] != "1"){
                outputArea.value += "Error in rotateRotationBase on line " + line + ": invalid armNum\n";
                return false;
            }
            if (message.length < 3){
                outputArea.value += "Error in rotateRotationBase on line " + line + ": wrong input number\n";
                return false;
            }
            break;
        case "dlRtBs": case "delayRotationBase":
            if (message[1] != "0" && message[1] != "1"){
                outputArea.value += "Error in delayRotationBase on line " + line + ": invalid armNum\n";
                return false;
            }
            if (message.length < 3){
                outputArea.value += "Error in delayRotationBase on line " + line + ": wrong input number\n";
                return false;
            }
            break;
        case "lfLfBs": case "liftLiftBase":
            if (message[1] != "0" && message[1] != "1"){
                outputArea.value += "Error in liftLiftBase on line " + line + ": invalid armNum\n";
                return false;
            }
            if (message.length < 3){
                outputArea.value += "Error in liftLifeBase on line " + line + ": wrong input number\n";
                return false;
            }
            break;
        case "dlLfBs": case "delayLiftBase":
            if (message[1] != "0" && message[1] != "1"){
                outputArea.value += "Error in delayLiftBase on line " + line + ": invalid armNum\n";
                return false;
            }
            if (message.length < 3){
                outputArea.value += "Error in delayLiftBase on line " + line + ": wrong input number\n";
                return false;
            }
            break;
        case "mvMvBs": case "moveMovingBase":
            if (message.length < 2){
                outputArea.value += "Error in moveMovingBase on line " + line + ": wrong input number\n";
                return false;
            }
            break;
        case "dlMvBs": case "delayMovingBase":
            if (message.length < 2){
                outputArea.value += "Error in delayMovingBase on line " + line + ": wrong input number\n";
                return false;
            }
            break;
        case "mvAm": case "mvMvAm": case "moveArm":
            if (message[1] != "0" && message[1] != "1"){
                outputArea.value += "Error in moveArm on line " + line + ": invalid armNum\n";
                return false;
            }
            if (message.length < 3){
                outputArea.value += "Error in moveArm on line " + line + ": wrong input number\n";
                return false;
            }
            break;
        case "dlAm": case "dlMvAm": case "delayMoveArm":
            if (message[1] != "0" && message[1] != "1"){
                outputArea.value += "Error in delayMoveArm on line " + line + ": invalid armNum\n";
                return false;
            }
            if (message.length < 3){
                outputArea.value += "Error in delayMoveArm on line " + line + ": wrong input number\n";
                return false;
            }
            break;
        case "rtSvOne": case "rotateServoOne":
            if (message[1] != "0" && message[1] != "1"){
                outputArea.value += "Error in rotateServoOne on line " + line + ": invalid armNum\n";
                return false;
            }
            if (message.length < 3){
                outputArea.value += "Error in rotateServoOne on line " + line + ": wrong input number\n";
                return false;
            }
            break;
        case "rtSvTwo": case "rotateServoTwo":
            if (message[1] != "0" && message[1] != "1"){
                outputArea.value += "Error in rotateServoTwo on line " + line + ": invalid armNum\n";
                return false;
            }
            if (message.length < 3){
                outputArea.value += "Error in rotateServoTwo on line " + line + ": wrong input number\n";
                return false;
            }
            break;
        case "cgMs": case "changeMass":
            if (message[1] != "0" && message[1] != "1"){
                outputArea.value += "Error in changeMass on line " + line + ": invalid armNum\n";
                return false;
            }
            if (message.length < 3){
                outputArea.value += "Error in changeMass on line " + line + ": wrong input number\n";
                return false;
            }
            break;
        case "gtLq":
            if (message[1] != "0" && message[1] != "1" && message[1] != "2" && message[1] != "3"){
                outputArea.value += "Error in getLiquid on line " + line + ": invalid liquidNum\n";
                return false;
            }
            break;
        case "gtSl":
            if (message.length < 2){
                outputArea.value += "Error in getSolid on line " + line + ": wrong input number\n";
                return false;
            }
            break;
        case "mvCm":
            if (message[1] != "0" && message[1] != "1"){
                outputArea.value += "Error in moveCamera on line " + line + ": invalid direction\n";
                return false;
            }
            break;
        default:
            outputArea.value += ("Unknown Command on line " + line + ": " + message[0] + "\n");
            return false;
    }
    registerSend(cmd, line)
    return true;
}

function registerSend(cmd, line){
    // console.log(cmd);
    // console.log("before ", registerTime, ' ', Date.now());
    if (registerTime < Date.now()){
        registerTime = Date.now() + intervalTime;
        timer[line] = setTimeout(function(){
            console.log("send " + cmd);
            socket.send(cmd);
        },registerTime - Date.now());
    }
    else{
        registerTime += intervalTime;
        timer[line] = setTimeout(function(){
            console.log("send " + cmd);
            socket.send(cmd);
        },registerTime - Date.now());
    }
    // console.log("after ", registerTime, ' ', Date.now());
}
