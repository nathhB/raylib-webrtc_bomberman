var exports = module.exports = {};

function TddWebRtcSession(connection, session_id) {    
    var webrtc = require('wrtc');
    var session = this;
    var candidates = [];
    var reliableChannel;
    var unreliableChannel;

    session.session_id = session_id

    this.connect = function() {
        this.peerConnection = new webrtc.RTCPeerConnection({ 'iceServers': [ { 'urls': 'stun:stun.l.google.com:19302' } ] });

        onError = function(error) {
            console.log(error);
        };

        onLocalDescriptionSet = function() {
            console.log('TddWebRtcSession: Local description set: ' + session.peerConnection.localDescription);
        };

        onRemoteDescriptionSet = function() {
            console.log('TddWebRtcSession: Remote description set: ' + session.peerConnection.remoteDescription);

            candidates.forEach(c => {
                session.peerConnection.addIceCandidate(c, onIceCandidateAdded, onError);
            });
            candidates = [];

            session.peerConnection.createAnswer(onAnswerCreated, onError);
        };

        onAnswerCreated = function(answer) {
            session.peerConnection.setLocalDescription(answer, onLocalDescriptionSet, onError);

            console.log('TddWebRtcSession: Send answer to ' + connection);
            connection.sendUTF(JSON.stringify(answer));
        };

        onIceCandidateAdded = function() {
            console.log('TddWebRtcSession: Added ice candidate');
        };

        onIceCandidate = function({ candidate }) {
            if (candidate) {
                console.log('TddWebRtcSession: Send candidate: ' + candidate);

                connection.sendUTF(JSON.stringify(candidate));
            }
        };

        onDataChannel = function({ channel }) {
            switch(channel.label) {
                case 'reliable-game-messages':
                    reliableChannel = channel;
                    reliableChannel.onmessage = function({ data }) {
                        session.onPacket(data);
                    }

                    if (session.onReady) session.onReady();
                    break;

                case 'unreliable-game-messages':
                    unreliableChannel = channel;
                    unreliableChannel.onmessage = function({ data }) {
                        session.onPacket(data);
                    }
                    break;

                default:
                    console.log('TddWebRtcSession: Unknown data channel: ' + channel.label);
            }
        };

        connection.on('message', function(msg) {
            var data = JSON.parse(msg.utf8Data);

            if ('offer' in data)
            {
                console.log('TddWebRtcSession: received offer: ' + data.offer);
                console.log('TddWebRtcSession: nickname: ' + data.nickname);

                var offer = new webrtc.RTCSessionDescription(data.offer);

                session.nickname = data.nickname;
                session.peerConnection.setRemoteDescription(offer, onRemoteDescriptionSet, onError);
            }
            else if ('candidate' in data)
            {
                console.log('TddWebRtcSession: got candidate');

                candidates.push(data);
            }
        });

        this.peerConnection.addEventListener('icecandidate', onIceCandidate);
        this.peerConnection.addEventListener('datachannel', onDataChannel);

        this.sendReliable = function(data) {
            reliableChannel.send(data);
        };

        this.sendUnreliable = function(data) {
            unreliableChannel.send(data);
        };
    }
}

exports.createTddWebRtcSession = function(connection, session_id) {
    console.log('Create new web rtc session');

    return new TddWebRtcSession(connection, session_id);
}