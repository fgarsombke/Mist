var net = require('net');
var util = require('util');

var server = net.createServer(function(socket) {
    console.log('Server Connected!');
    socket.setEncoding('ascii');

    console.log('Sending: '+'{\"s\":\"'+io+'\"}');
    socket.write('{\"s\":\"'+io+'\"}');

    socket.end();
    socket.on('end', function() {
        console.log('Server Disconnected!\n');

        console.log('\033[2J');
        console.log('ON (1) or OFF (0)?');
    }); 
});

server.listen(5001, function() {
    console.log('Server Bound!');

    console.log('ON (1) or OFF (0)?');
    var stdin = process.openStdin();   
    stdin.addListener('data', function(d) {
        io = d.toString().substring(0, d.length-1);
    }); 
});