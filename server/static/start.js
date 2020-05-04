var img = new Image();            
var canvas;
var ctx;

$(document).ready(function() {			
        // once connected, inform server about the new connection
        socket.on('connect', function() {
            socket.emit('event', {data: 'connected'});

            canvas = document.getElementById("container").querySelector("#frame");
            ctx = canvas.getContext('2d'); 
            //console.log(ctx);
            //img.onload = updateImage();
            //img.onload = () => update_frame = true; 
        });       

		$(window).on("gamepadconnected", function() {
			hasGP = true;			
            socket.emit('event', {data: 'INIT'});
		/* var req = new XMLHttpRequest();
			req.open('POST', '/', true);
			req.setRequestHeader('content-type', 'application/x-www-form-urlencoded;charset=UTF-8');
			req.send("key=INIT");
        */

            //ctx.scale(-1, 1);

			window.requestAnimationFrame(update);       
		});

});
