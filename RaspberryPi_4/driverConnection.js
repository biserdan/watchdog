
var pigpio = require('pigpio');
var Gpio = pigpio.Gpio;
var status = require('/home/pi/projects/underground/src/car/js/status.js').status
var infiniteTime = require('/home/pi/projects/underground/src/car/js/status.js').INFINITE;
var player = require('play-sound')(opts = {player: '/usr/bin/mplayer'});

//-----------------------------------------------------------------------------
//RECEIVE STEERING COMMANDS FROM DRIVER
//-----------------------------------------------------------------------------

var sound = -1;
var sound_counter = 0;
var last_sound = -999999;

const STEER_TUNE = -8;

const STEER_RIGHT_MOST = 100 + STEER_TUNE - 25;
const STEER_LEFT_MOST = 200 + STEER_TUNE;
const STEER_MIDDLE = 150 + STEER_TUNE;

const MIN_SPEED = 165;
const MAX_SPEED = 200;

const STEER_SPEED = 5;
const STEER_RETURN = 10;

//const MIN_SPEED = 190;
//const MAX_SPEED = 250;

module.exports.speed = 0;
module.exports.acceleration = 1;
module.exports.steer = STEER_MIDDLE;

const startup_acceleration = 165;

var gear = 0;
const gears = [165, 170, 175, 180, 185, 190];

//initialize gpio pins

var speedPin = new Gpio(26, {mode: Gpio.OUTPUT});

speedPin.pwmFrequency(400);

speedPin.pwmWrite(150);

const steerPin = new Gpio(24, {mode: Gpio.OUTPUT});
steerPin.pwmFrequency(400);

//stop();
start();

steerMiddle();

function isSteerRight() {
	return module.exports.steer < STEER_MIDDLE;
}

function isSteerLeft() {
	return module.exports.steer > STEER_MIDDLE;
}

function isForward() {
	return module.exports.speed >= MIN_SPEED;
}

function isBackward() {
	return -module.exports.speed >= MIN_SPEED;
}

function steerRightMOST() {
	module.exports.steer = STEER_RIGHT_MOST;
	steer();
}

function steerLeftMOST() {
	module.exports.steer = STEER_LEFT_MOST;
	steer();
}

function steerMiddle() {
	module.exports.steer = STEER_MIDDLE;
	steer();
}

function steerMiddleIfNeeded() {
	if (module.exports.steer != STEER_MIDDLE) {
		module.exports.steer = STEER_MIDDLE;
                last_right_steer = steer_counter;
		last_left_steer = steer_counter;
		steer();
	}
}

function steer() {
	steerPin.pwmWrite(module.exports.steer);
}

var sr_counter = 0;

function steerRight() {
	
	sl_counter = 0;
	if (sr_counter < 7) {
		sr_counter += 1;
		return;
	}
	sr_counter = 0;

	module.exports.steer -= STEER_SPEED;
	if (module.exports.steer <= STEER_RIGHT_MOST) {
		module.exports.steer = STEER_RIGHT_MOST;
	}	
	//module.exports.steer = STEER_RIGHT_MOST;
	steer();
	last_right_steer = steer_counter;
}

function steerRightReturn() {
	module.exports.steer -= STEER_RETURN;
	if (module.exports.steer <= STEER_MIDDLE) {
		module.exports.steer = STEER_MIDDLE;
	}
	steer();
	last_right_steer = steer_counter;
}

var sl_counter = 0;

function steerLeft() {

	sr_counter = 0;
	if (sl_counter < 7) {
		sl_counter += 1;
		return;
	}
	sl_counter = 0;

	module.exports.steer += STEER_SPEED;
	if (module.exports.steer >= STEER_LEFT_MOST) {
		module.exports.steer = STEER_LEFT_MOST;
	}
	//module.exports.steer = STEER_LEFT_MOST;
	steer();
	last_left_steer = steer_counter;
}

function steerLeftReturn() {
	module.exports.steer += STEER_RETURN;
	if (module.exports.steer >= STEER_MIDDLE) {
		module.exports.steer = STEER_MIDDLE;
	}
	steer();
	last_left_steer = steer_counter;
}

var test_i = 0;

var num_iter = 1;

/*
setTimeout(()=>{
	gas_test = setInterval(()=>{
		if (num_iter > 0){
			if (test_i%4==0){
				forward(MIN_SPEED);
			}
			if (test_i%4==1){
				stop();
			}
			if (test_i%4==2){
				backward(MIN_SPEED);
			}
			if (test_i%4==3){
				stop();
			}
			test_i = test_i + 1;
			if (test_i == 4) {
				if (num_iter > 0){
					num_iter -= 1;
				} else {
					clearInterval(gas_test);
				}
			}
		} else {
			clearInterval(gas_test);
		}
	}, 300);
},2000);
*/

var steer_counter = 0;
var last_right_steer= 0;
var last_left_steer = 0;

const STEER_RELEASE = 15;

var gas_counter = 0;
var last_gas_forward = 0;
var last_gas_backward = 0;
const GAS_RELEASE = 100;

setInterval(()=>{
	
	if (isSteerRight()) {
		if (steer_counter - last_right_steer > STEER_RELEASE) {
			steerLeftReturn();
			//steerMiddleIfNeeded();
		} 
	} else if (isSteerLeft()) {
		if (steer_counter - last_left_steer > STEER_RELEASE) {
			steerRightReturn();
			//steerMiddleIfNeeded();
		}
	}
	
	if (isForward()) {
		if (gas_counter - last_gas_forward > GAS_RELEASE) {
			stop();
		}
	}
	if (isBackward()) {
		if (gas_counter - last_gas_backward > GAS_RELEASE) {
			stop();
		}
	}

	gas_counter += 1;
	steer_counter += 1;
	sound_counter += 1;
}, 10);


const SOUND_RELEASE_HORN = 300;
const SOUND_RELEASE_STARTUP = 300;

function horn() {
	//if (sound_counter - last_sound > SOUND_RELEASE_HORN) {
		if (sound != -1) {
			sound.kill();		
		}
		last_sound = sound_counter;
		sound = player.play('/home/pi/projects/underground/src/car/sounds/horn1.mp3');
	//}
}

function startupSound() {
	//if (sound_counter - last_sound > SOUND_RELEASE_STARTUP) {
		if (sound != -1) {
			sound.kill();
		}
		last_sound = sound_counter;
		sound = player.play('/home/pi/projects/underground/src/car/sounds/startup1.mp3');
	//}
}

setTimeout(function(){startupSound();},500);

function forward(s) {

	if (status.limit === 'ZERO') {
		return;
	}


	//console.log('forward');
	if ((s > MAX_SPEED)||(s < MIN_SPEED)) {
		stop();
		return;
	}

	if (module.exports.speed > 0) {
		return;
	}

	s = gears[gear];

	speedPin.pwmWrite(0);
	speedPin.pwmWrite(150);
	speedPin.pwmWrite(0);
	speedPin.pwmWrite(s);
	last_gas_forward = gas_counter;
}

function backward(s) {

	if (status.limit === 'ZERO') {
		return;
	}

	//console.log('backward');
	if ((s > MAX_SPEED)||(s < MIN_SPEED)) {
		stop();
		return;
	}

	if (module.exports.speed < 0) {
		return;
	}

	s = MIN_SPEED - s + MIN_SPEED;
	s = 125;

	speedPin.pwmWrite(s);
	setTimeout(()=>{
		speedPin.pwmWrite(150);
		setTimeout(()=>{
			speedPin.pwmWrite(s);
		},10);
	},10);
	last_gas_backward = gas_counter;
}

function start() {

	speedPin.pwmWrite(0);
	setTimeout(()=>{
		speedPin.pwmWrite(150);
		setTimeout(()=>{
			speedPin.pwmWrite(0);
			setTimeout(()=>{
				speedPin.pwmWrite(150);
			},100);
		},100);
	},100);

}

function stop() {
	//console.log('stop');
	speedPin.pwmWrite(150);
	module.exports.speed = 0;
}

function setSpeed(s, a) {
	
	var maxSpeed = MAX_SPEED;
	var minSpeed = MIN_SPEED;

	if (a > 0) {
		if (s < minSpeed) {
			s += startup_acceleration;
			if (s >= minSpeed) {
				s = minSpeed;
			}
		} 
	}
	if (a < 0) {
		if (-s < minSpeed) {
			s -= startup_acceleration;
			if (-s >= minSpeed) {
				s = -minSpeed;
			}
		}
	}

	if (s > 0) {
		if (s <= maxSpeed) {
			if (s < minSpeed) {
				stop();
			} else {
				forward(s);
			}
			module.exports.speed = s;
		} else {
			forward(maxSpeed);
			module.exports.speed = maxSpeed;
		}
	}
	if (s < 0) {
		//if (module.exports.speed < 0) {
		//	return;
		//}
		if (-s <= maxSpeed) {
			if (-s < minSpeed) {
				stop();
			} else {
				backward(-s);
			}
			module.exports.speed = s;
		} else {
			backward(maxSpeed);
			module.exports.speed = -maxSpeed;
		}
	}
	if (s == 0) {
		stop();
		module.exports.speed = 0;
	}
}

function accelerate(a) {
	setSpeed(module.exports.speed + a, a);
}

//-----------------------------------------------------------------------------
//RECEIVE STEERING COMMANDS FROM DRIVER
//-----------------------------------------------------------------------------
var WebSocketServer = require('websocket').server;
var http = require('http');
var server = http.createServer(function(request, response) {
	console.log((new Date()) + ' Received request for ' + request.url);
	response.writeHead(404);
	response.end();
});
server.listen(8080, function() {
	console.log('Driver engine listening on 8080');
});

//accelerate(-module.exports.acceleration);
var stop_counter = 0;

wsServer = new WebSocketServer({
	httpServer: server,
	autoAcceptConnections: false
});
wsServer.on('request', function(request) {
	var connection = request.accept('car-engine', request.origin);
	console.log('connected.');
	startupSound();   
	connection.on('message', function(message) {
		//console.log('[driverConnetion]', message);
		//handle timing of status
		if (status.firstTime == infiniteTime) {
			status.firstTime = 0;
		}
		status.lastTime = 0;
		var steer = message.utf8Data;

		//console.log('MESSAGE WAS ', steer);

		if (steer==='(R)') {
			//console.log('R');
			steerRight();
		}
		if (steer==='(L)') {
			//console.log('L');
			steerLeft();
		}
		if (steer==='(U)') {
			stop_counter = 0;
			//console.log('U');
			accelerate(module.exports.acceleration);
		}
		if (steer==='(D)') {
			stop_counter = 0;
			//console.log('D');
			accelerate(-module.exports.acceleration);
		}
		if (steer==='(UR)') {
			stop_counter = 0;
			//console.log('UR');
			accelerate(module.exports.acceleration);
			steerRight();
		}
		if (steer==='(UL)') {
			stop_counter = 0;
			//console.log('UL');
			accelerate(module.exports.acceleration);
			steerLeft();
		}
		if (steer==='(DR)') {
			stop_counter = 0;
			//console.log('DR');
			accelerate(-module.exports.acceleration);
			steerRight();
		}
		if (steer==='(DL)') {
			stop_counter = 0;
			//console.log('DL');
			accelerate(-module.exports.acceleration);
			steerLeft();
		}
		if (steer==='(B)') {
			//console.log('B');
			stop();
		}
		if ((steer ==='(N)') || (status.limit==='ZERO')){
			//console.log('N');
			//if (module.exports.speed > 0 ) {
				if (stop_counter < 2) {
					stop_counter = 0;
					stop();
				} else {
					stop_counter += 1;
				}
			//}
		}
		if (steer==='(SPACE)') {
			//console.log('SPACE');
			stop();
		}
		if (steer==='(H)') {
			//console.log('H');
			horn();
		}
                if (steer==='(1)') {
                        //console.log('H');
                        gear = 0;
                }
                if (steer==='(2)') {
                        //console.log('H');
                        gear = 1;
                }
                if (steer==='(3)') {
                        //console.log('H');
                        gear = 2;
                }
                if (steer==='(4)') {
                        //console.log('H');
                        gear = 3;
                }
                if (steer==='(5)') {
                        //console.log('H');
                        gear = 4;
                }
                if (steer==='(6)') {
                        //console.log('H');
                        gear = 5;
                }

	});
	connection.on('close', function(reasonCode, description) {
		console.log('disconnected.');
	});
});


