$(document).ready(function() {			
        // once connected, inform server about the new connection
        socket.on('connect', function() {
            socket.emit('event', {data: 'connected'});

        });       

		$(window).on("gamepadconnected", function() {
			hasGP = true;			
            socket.emit('event', {data: 'INIT'});
		/* var req = new XMLHttpRequest();
			req.open('POST', '/', true);
			req.setRequestHeader('content-type', 'application/x-www-form-urlencoded;charset=UTF-8');
			req.send("key=INIT");
        */
            container = document.getElementById("container");
            canvas = document.getElementById("container").querySelector("#frame");
            ctx = canvas.getContext('2d'); 
            //ctx.scale(-1, 1);

			window.requestAnimationFrame(update);       
		});

});
