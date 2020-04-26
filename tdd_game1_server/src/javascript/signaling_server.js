
var exports = module.exports = {}

function SignalingServer(on_player_session_ready_func) {
    var http = require('http');
    var WebSocketServer = require('websocket').server;
    var pendingPackets = [];
    var sessions = [];
    var nextSessionId = 0;

    var server = http.createServer(function(request, response) {
        console.log((new Date()) + ' Received request for ' + request.url);
        response.writeHead(404);
        response.end();
    });

    server.listen(4242, function() {
        console.log((new Date()) + ' Signaling server is listening on port 4242');
    });

    wsServer = new WebSocketServer({
        httpServer: server,
        autoAcceptConnections: false
    });

    wsServer.on('request', function(request) {
        var connection = request.accept('signaling-procotol', request.origin);

        console.log("New connection accepted");

        var session = require('./tdd_webrtc_session.js').createTddWebRtcSession(connection, nextSessionId++);

        session.onReady = function() {
            on_player_session_ready_func(session.session_id, session.nickname);
        }

        session.onPacket = function(packet) {
            pendingPackets.push(packet);
        };

        sessions.push(session);

        session.connect();
    });

    this.dequeuePacket = function() {
        return pendingPackets.shift();
    };

    this.sendUnreliablePacket = function(packet, session_id) {
        sessions[session_id].sendUnreliable(packet);
    };

    this.sendReliablePacket = function(packet, session_id) {
        sessions[session_id].sendReliable(packet);
    };

    this.broadcastReliablePacket = function(packet) {
        sessions.forEach(s => { s.sendReliable(packet); });
    };

    this.broadcastUnreliablePacket = function(packet) {
        sessions.forEach(s => { s.sendUnreliable(packet); });
    };
}

exports.createSignalingServer = function(notify_player_session_ready_func) {
    return new SignalingServer(notify_player_session_ready_func);
}