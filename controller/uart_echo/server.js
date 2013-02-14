var net = require('net');
 
var server = net.createServer(function(socket) {
        console.log('server connected');
 
        socket.setEncoding('ascii');
 
        socket.on('end', function() {
                console.log('server disconnected');
        });
 
        socket.on('data', function(data){
                console.log(data);
        });
});
 
server.listen(5001, function() { //'listening' listener
        console.log('server bound');
});
