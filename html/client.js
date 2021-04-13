const socket = new WebSocket('ws://192.168.1.3:2051');
const inputArea = document.getElementById("inputArea");

function send() {
    socket.send(inputArea.value);
}
