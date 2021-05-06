"use strict"
const socket = new WebSocket('ws://192.168.1.3:2051');
var upDownPosition = 90;
var leftRightPosition = 90;

document.addEventListener('keypress', keyCheck);

function keyCheck(pressKey){
    var keyNum = pressKey.keyCode;
    var keyChar = String.fromCharCode(keyNum);
    if (keyChar == "W" || keyChar == "w"){
        upDownPosition -= 5;
    }
    else
        if (keyChar == "s" || keyChar == "s"){
            upDownPosition += 5;
        }
        else
            if (keyChar == "a" || keyChar == "A"){
                leftRightPosition += 5;
            }
            else
                if (keyChar == "d" || keyChar == "D"){
                    leftRightPosition -= 5;
                }
    upDownPosition = Math.max(upDownPosition, 50);
    upDownPosition = Math.min(upDownPosition, 180);
    leftRightPosition = Math.max(leftRightPosition, 0);
    leftRightPosition = Math.min(leftRightPosition, 180);
    var upDownCmd = "mvCm 0 " + String(upDownPosition);
    var leftRightCmd = "mvCm 1 " + String(leftRightPosition);
    console.log(upDownCmd);
    console.log(leftRightCmd);
    socket.send(upDownCmd);
    socket.send(leftRightCmd);
}
