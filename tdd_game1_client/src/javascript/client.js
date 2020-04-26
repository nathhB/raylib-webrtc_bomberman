

mergeInto(LibraryManager.library, {
    client_init: function() {
        console.log('Connecting to host: ' + Module.getHost());

        var ws = new WebSocket('ws://' + Module.getHost() + ':4242', 'signaling-procotol');
        var peerConnection = new RTCPeerConnection({ iceServers: [ { url: 'stun:stun.l.google.com:19302' } ] });
        var offerSent = false;
        var client = this;

        this.pendingPackets = [];

        sendSdpOffer = function() {
            if (offerSent) return;

            console.log('Send sdp offer...');
            
            ws.send(JSON.stringify({ offer: peerConnection.localDescription, nickname: Module.getUsernameFromLocation() }));
            offerSent = true;
        }

        createDataChannels = function() {
            this.reliableChannel = peerConnection.createDataChannel('reliable-game-messages');
            this.unreliableChannel = peerConnection.createDataChannel(
                'unreliable-game-messages',
                { ordered: true, maxRetransmits: 0 }
            );

            onDataChannelError = function({ message }) {
                console.log('Data channel error: ' + message);
            }

            this.reliableChannel.onerror = onDataChannelError;
            this.unreliableChannel.onerror = onDataChannelError;

            this.reliableChannel.onopen = function() {
                set_reliable_channel_ready = Module.cwrap('set_reliable_channel_ready', null);

                set_reliable_channel_ready();
            };

            this.unreliableChannel.onopen = function() {
                set_unreliable_channel_ready = Module.cwrap('set_unreliable_channel_ready', null);

                set_unreliable_channel_ready()
            };

            this.reliableChannel.onmessage = function({ data }) {
                client.pendingPackets.push(data);
            }

            this.unreliableChannel.onmessage = function({ data }) {
                client.pendingPackets.push(data);
            }

            console.log("Data channels created");
        }

        peerConnection.onicecandidate = function({ candidate }) {
            console.log('Send candidate: ' + candidate);
                
            if (candidate) {
                ws.send(JSON.stringify(candidate));
            } else {
                sendSdpOffer();
            }
        };
 
        peerConnection.onicegatheringstatechange = function(ev) {
            if (peerConnection.iceGatheringState === 'complete') {
                console.log('Ice states gathering completed');

                sendSdpOffer();
            }
        }

        onLocalDescriptionSet = function() {
            console.log('Local description set: ' + peerConnection.localDescription);
            console.log('Send offer...');
        }

        onRemoteDescriptionSet = function() {
            console.log('Remote description set: ' + peerConnection.remoteDescription);
        }

        onOfferCreated = function(offer) {
            peerConnection.setLocalDescription(offer, onLocalDescriptionSet, onError);
        };

        onIceCandidateAdded = function() {
            console.log('Added ice candidate');
        }

        onError = function(error) {
            console.log('ERROR (in client_init): ' + error);
        };

        ws.onopen = function (event) {
            console.log('Connected with signaling server');
    
            createDataChannels();
            peerConnection.createOffer(onOfferCreated, onError, { mandatory: {OfferToReceiveAudio: true, OfferToReceiveVideo: true } });
        };

        ws.onmessage = function(msg) {
            var data = JSON.parse(msg.data);

            if (data.type == 'answer')
            {
                console.log('Received sdp answer');

                peerConnection.setRemoteDescription(data, onRemoteDescriptionSet, onError);
            }
            else if ('candidate' in data)
            {
                console.log('Got candidate');

                peerConnection.addIceCandidate(data, onIceCandidateAdded, onError);
            }
            else
            {
                console.log('Received invalid message');
            }
        }
    },

    client_send_reliable_packet: function(packetPtr, packetBytes) {
        var jsBuffer = Module.HEAPU8.subarray(packetPtr, packetPtr + packetBytes);
        var buffer = new Uint8Array(jsBuffer);

        this.reliableChannel.send(buffer);
    },

    client_send_unreliable_packet: function(packetPtr, packetBytes) {
        var jsBuffer = Module.HEAPU8.subarray(packetPtr, packetPtr + packetBytes);
        var buffer = new Uint8Array(jsBuffer);

        this.unreliableChannel.send(buffer);
    },

    client_dequeue_packet: function() {
        var packet = pendingPackets.shift();

        if (packet) {
            var ptr = stackAlloc(packet.byteLength);

            writeArrayToMemory(new Uint8Array(packet), ptr);
            return ptr;
        } else {
            return null;
        }
    },
});