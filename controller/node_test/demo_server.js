/* Server to be used for demo Mon, Mar 4 2013
 * On client connection, the server prompts for a state (1 or 0)
 * then transmits it in a JSON wrapper to the client and closes
 * the connection.
*/
var net = require('net');
var util = require('util');

var server = net.createServer(function(socket) { //'connection' listener
    var stdin = process.openStdin();   

    console.log('server connected');

    socket.setEncoding('ascii');

    socket.on('end', function() {
        console.log('server disconnected');
    }); 

    console.log('ON (1) or OFF (0)?');
    stdin.addListener('data', function(d) {
        io = d.toString().substring(0, d.length-1);
        console.log('Sending: '+'{\"s\":\"'+io+'\"}');
        socket.write('{\"s\":\"'+io+'\"}');
        socket.end();
    }); 

    /*socket.on('data', function(data) {
        console.log('Raw      : ' + data);
        console.log('Converted: ' + posix2date(parseInt(data)));
        socket.write("{\"z\":[{\"i\":[{\"s\":0,\"e\":2},{\"s\":10,\"e\":20}]},{\"i\":[{\"s\":0,
    });*/
});


/*
// function to convert from posix time to string
function posix2date(posix) {
    var date = new Date(posix*1000);
    return date.toGMTString();
}*/

server.listen(5001, function() {
    console.log('server bound');
});
