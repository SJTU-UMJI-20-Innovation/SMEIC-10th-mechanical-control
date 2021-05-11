"use strict"
const socket = new WebSocket('ws://192.168.1.3:2051');

function leftHandGetTube(){
    socket.send("cgMs 0 0");
    socket.send("cgSg 1");
    socket.send("mvMvBs 13.5 1 2");
    socket.send("rtRtBs 0 0.0 2 3");
    socket.send("lfLfBs 0 -1.4 3 4");//!!!
    socket.send("rtSvOne 0 103 4 5");
    socket.send("mvAm 0 77.43 5 6");//!!!
    socket.send("dlAm 0 3 6 7");
    socket.send("rtSvOne 0 88 7 8");
    socket.send("cgMs 0 1 8 9");
    socket.send("mvAm 0 77.7 9 10");
    socket.send("lfLfBs 0 -7.0 10 11");
    socket.send("mvAm 0 90 11 12");
}

function leftHandReachOutGetLiquid(){
    socket.send("cgSg 1");
    socket.send("mvMvBs 12.5 1 2");
    socket.send("lfLfBs 0 -6 2 3");
    socket.send("mvAm 0 81.5 3 4");
}

function liquid4Drop(){
    socket.send("gtLq 3 3");

}

function leftHandFinishGetLiquid(){
    socket.send("mvAm 0 90.0 4 5");
}

function rightHandGetTube() {
    socket.send("cgMs 1 0");
    socket.send("cgSg 1");
    socket.send("mvMvBs -26.5 1 2");
    socket.send("rtRtBs 1 -0.0 2 3");
    socket.send("lfLfBs 1 -4.0 3 4");
    socket.send("rtSvOne 1 103 4 5");
    socket.send("mvAm 1 77 5 6");
    socket.send("dlAm 1 3 6 7");
    socket.send("rtSvOne 1 88 7 8");
    socket.send("cgMs 1 1 8 9");
    socket.send("mvAm 1 77.2 9 10");
    socket.send("lfLfBs 1 -9.5 10 11");
    socket.send("mvAm 1 90.0 11 12");
}

function rightHandReachOutGetLiquid(){
    socket.send("cgSg 1");
    socket.send("lfLfBs 1 -7.5 1 2");
    socket.send("mvMvBs -23.6 2 3");
    socket.send("mvAm 1 81.5 3 4");
}

function liquid3Drop(){
    socket.send("gtLq 2 3");
}

function rightHandFinishGetLiquid(){
    socket.send("mvAm 1 90 4 5");

}

function turnToExperimentArea(){
    socket.send("cgSg 1");
    socket.send("lfLfBs 0 -6 1 2");
    socket.send("rtRtBs 0 -180 2 3");
    socket.send("lfLfBs 1 -6 3 4");
    socket.send("rtRtBs 1 180 4 4 5");
}

function dropLiquid(){
    socket.send("cgSg 1");
    socket.send("rtRtBs 1 182.5 1 2");
    socket.send("lfLfBs 1 -21.5 2 3");
    socket.send("mvAm 1 86.8 3 4");
    socket.send("rtSvTwo 1 20 4 5");
}

function HDCameraBeginWatch(){
    socket.send("cgSg 1");
    socket.send("rtSvTwo 1 90 1 2");
    socket.send("cgMs 1 10");
    socket.send("mvAm 1 70 2 3");
    socket.send("rtRtBs 1 193 3 4");
    socket.send("lfLfBs 1 -8 4 5");
}

function HDCameraEndWatch(){
    socket.send("lfLfBs 1 -20 5 6");
    socket.send("rtRtBs 1 180 6 7");
    socket.send("cgMs 1 1 7 8");
    socket.send("mvAm 1 90 8 9");
    socket.send("lfLfBs 1 -6 9 10");
}

function turnToStorageArea(){
    socket.send("cgSg 1");
    socket.send("rtSvTwo 1 90 1 2");
    socket.send("mvAm 1 90 2 3");
    socket.send("lfLfBs 0 -6 3 4");
    socket.send("rtRtBs 0 0 4 5");
    socket.send("lfLfBs 1 -6 5 6");
    socket.send("rtRtBs 1 0 4 6 7");
}

function rightHandHandBackTube(){
    socket.send("cgSg 1");
    socket.send("mvMvBs -26.5 1 2");
    socket.send("lfLfBs 1 -8.0 2 3");
    socket.send("mvAm 1 77 3 4");
    socket.send("lfLfBs 1 -2.5 4 5");
    socket.send("rtSvOne 1 105 5 6");
    socket.send("cgMs 1 0 6 7");
    socket.send("mvAm 1 90 7 8");
}

function leftHandHandBackTube(){
    socket.send("cgSg 1");
    socket.send("mvMvBs 13.5 1 2");
    socket.send("rtRtBs 0 0.0 2 3");
    socket.send("lfLfBs 0 -7.0 3 4");
    socket.send("mvAm 0 77.45 4 5");
    socket.send("lfLfBs 0 0.0 5 6");
    socket.send("rtSvOne 0 103 6 7");
    socket.send("cgMs 0 0 7 8");
    socket.send("mvAm 0 90 8 9");
}