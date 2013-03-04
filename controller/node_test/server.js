var net = require('net');

var server = net.createServer(function(socket) { //'connection' listener
    console.log('server connected');

    socket.setEncoding('ascii');

    socket.on('end', function() {
        console.log('server disconnected');
    }); 

    socket.on('data', function(data) {
        console.log('Raw      : ' + data);
        console.log('Converted: ' + posix2date(parseInt(data)));
        socket.write("{\"z\":[{\"i\":[{\"s\":0,\"e\":2},{\"s\":10,\"e\":20}]},{\"i\":[{\"
    }); 
});

function posix2date(posix) {
    var date = new Date(posix*1000);
    return date.toGMTString();
}

server.listen(5001, function() {
    console.log('server bound');
});