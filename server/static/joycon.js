var hasGP = false;
var repGP;
var left_pressed = false;
var right_pressed = false;
var up_pressed = false;
var down_pressed = false;
var a_pressed = false;
var b_pressed = false;
var start1_pressed = false;
var start2_pressed = false;
var select1_pressed = false;
var select2_pressed = false;

var skip = 0;
var frameskip = 0;//document.currentScript.getAttribute('frameskip'); // this will make the game run at only ~15 (4) or ~ 20 (3) FPS
var joystick_timeout = 0;

var fps = 30;

var sending = false;

var img = new Image();
img.src = '';

// create socket with namespace "retro"
namespace = '/retro';
var socket = io(namespace);
socket.on('data_out', function(data){img.src="data:image/jpg;base64, "+data;}); // update data

//var container = document.getElementById('#container');
var canvas;
var ctx;

function update(){
	reportOnGamepad();

	if (skip == frameskip){ // skip some frames 
		updateImage();				
		skip = 0;
	}
	else{
		skip++;
	}
	window.requestAnimationFrame(update);
}

function updateImage() {    
    var height = 144;
    var width = 160;
    ctx.drawImage(img, 1280/2-width/2,720/2-height);
    //console.log("test");	
    //document.getElementById("image").src="data:image/jpg;base64, " + img_data;
    
	//document.getElementById("image").src="static/img.jpg?t=" + new Date().getTime(); // reload updated image without caching
}

function sendJoystick(ax0, ax1) { // sends joystick information back to the flask app (server.py) using XML post requests
				  // since the Switch browser does not support WebSocket
	if (!(sending)){
        /*
		var req = new XMLHttpRequest();
		req.open('POST', '/', true);
		req.setRequestHeader('content-type', 'application/x-www-form-urlencoded;charset=UTF-8');
		req.send("key=" +"Stick;"+"1"+";"+ax0+";"+ax1);
        */
        socket.emit('event', {data: "Stick;"+"1"+";"+ax0+";"+ax1});
		sending = false;
	}
}

function sendKey(key){
	if (!(sending)){
        /*
		var req = new XMLHttpRequest();
		req.open('POST', '/', true);
		req.setRequestHeader('content-type', 'application/x-www-form-urlencoded;charset=UTF-8');
		req.send("key=" + key);
        */
        socket.emit('event', {data: key});
		sending = false;
	}
}

function getJoystick() {
	var gp = navigator.getGamepads()[0];
	if(joystick_timeout == 2){ //limit joystick data so it doesn't induce more lag
		joystick_timeout = 0;        
		for(var i=0;i<2; i+=2) {
			if (((gp.axes[i] > 0.1) || (gp.axes[i] < -0.1)) || ((gp.axes[i+1] > 0.1) || (gp.axes[i+1] < -0.1))) {
			sendJoystick(gp.axes[i], gp.axes[i+1])			
			}
		}
	}
	else{joystick_timeout++;}
}

function reportOnGamepad() {
	var gp = navigator.getGamepads()[0];
	
	for(var i=0;i<gp.buttons.length;i++) {			
		if(gp.buttons[i].pressed){		
			switch (i){
				case 14:
					if (!left_pressed){
					sendKey("left_down");
					left_pressed = true;};
					break;
				case 12:
					if (!up_pressed){
					sendKey("up_down");
					up_pressed = true;};
					break;
				case 15:
					if (!right_pressed){						
					sendKey("right_down");
					right_pressed = true;};
					break;
				case 13:
					if (!down_pressed){
					sendKey("down_down");
					down_pressed = true;};
					break;
				case 1:
					if (!a_pressed){
					sendKey("a_down");
					a_pressed = true;};
					break;
				case 2:
					if (!b_pressed){
					sendKey("y_down");
					b_pressed = true;};
					break;
				case 5:
					if (!start1_pressed){
					sendKey("sr_down");
					start1_pressed = true;};
					break;
				case 7:
					if (!start2_pressed){
					sendKey("zr_down");
					start2_pressed = true;};
					break;
				case 4:
					if (!select1_pressed){
					sendKey("sl_down");
					select1_pressed = true;};
					break;
				case 6:
					if (!select2_pressed){
					sendKey("zl_down");									
					select2_pressed = true;};
					break;
			}
		} else if(!(gp.buttons[i].pressed)){
			switch (i){
				case 14:
					if (left_pressed){
					sendKey("left_up");
					left_pressed = false;};
					break;
				case 12:
					if (up_pressed){
					sendKey("up_up");
					up_pressed = false;};
					break;
				case 15:
					if (right_pressed){
					sendKey("right_up");
					right_pressed = false;};
					break;
				case 13:
					if (down_pressed){
					sendKey("down_up");
					down_pressed = false;};
					break;
				case 1:
					if (a_pressed){
					sendKey("a_up");
					a_pressed = false;};
					break;
				case 2:
					if (b_pressed){
					sendKey("y_up");
					b_pressed = false;};
					break;
				case 5:
					if (start1_pressed){
					sendKey("sr_up");
					start1_pressed = false;};
					break;
				case 7:
					if (start2_pressed){
					sendKey("zr_up");
					start2_pressed = false;};
					break;
				case 4:
					if (select1_pressed){
					sendKey("sl_up");
					select1_pressed = false;};
					break;
				case 6:
					if (select2_pressed){
					sendKey("zl_up");
					select2_pressed = false;};
					break;
			}
		}
	}

	getJoystick();
}
