mergeInto(LibraryManager.library, {
    server_init: function() {
        on_player_session_ready_func = Module.cwrap('on_player_session_ready', null, ['number', 'string']);

        this.signalingServer = require('../src/javascript/signaling_server.js').createSignalingServer(
            on_player_session_ready_func
        );
    },

    server_dequeue_packet: function() {
        var packet = this.signalingServer.dequeuePacket();

        if (packet) {
            var ptr = stackAlloc(packet.byteLength);
            var byteArray = new Uint8Array(packet);

            writeArrayToMemory(byteArray, ptr);
            return ptr;
        } else {
            return null;
        }
    },

    server_send_reliable_packet: function(packetPtr, packetBytes, session_id) {
        var jsBuffer = Module.HEAPU8.subarray(packetPtr, packetPtr + packetBytes);

        this.signalingServer.sendReliablePacket(jsBuffer, session_id);
    },

    server_send_unreliable_packet: function(packetPtr, packetBytes, session_id) {
        var jsBuffer = Module.HEAPU8.subarray(packetPtr, packetPtr + packetBytes);

        this.signalingServer.sendUnreliablePacket(jsBuffer, session_id);
    }
});