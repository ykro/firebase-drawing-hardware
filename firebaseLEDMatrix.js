var spark = require('spark');
var Firebase = require('firebase');
var dataRef = new Firebase('https://led-drawing.firebaseio.com/@ykro');

var photonDevice;
spark.login({accessToken: '<ACCESS TOKEN>'}).then(
	spark.listDevices().then(
	  function(devices){
	  	for (var i = 0; i < devices.length; i++) {
	  		var currentDevice = devices[i];
	  		if (currentDevice.connected) {
	  			photonDevice = currentDevice;
				photonDevice.callFunction('allOff','', function(err, data) {
					if (err) {
						console.log('An error occurred:', err);
					} else {
						console.log('Function called succesfully:', data);
					}
				});  		  			
	  			break;
	  		}
	  	}

	  	if (photonDevice != undefined) {
	  		console.log('Device connected: ', photonDevice.name);
	  	} else {
	  		console.log('No connected devices');	
	  	}
	    
	  },
	  function(err) {
	    console.log('List devices call failed: ', err);
	  }
	)
);

function draw(x, y, color) {
  if (photonDevice != undefined) {
  	console.log(x + ' ' + y + ' ' + color);
	photonDevice.callFunction('lightLED', x + ','+ y + ',' + color, function(err, data) {
		if (err) {
			console.log('An error occurred:', err);
		} else {
			console.log('Function called succesfully:', data);
		}
	});  	
  }
}

var drawPixel = function(snapshot) { 

  if ((snapshot.key()).indexOf(':') != -1) {
  	var coords = snapshot.key().split(":");
  	var color = snapshot.val();
  	if (color == '#ffffff') {
    	color = '#000000';
	} else if (color == '#000000') {
    	color = '#ffffff';
	}    
	draw(coords[0],coords[1],color);
  }  
};

var clearPixel = function(snapshot) {
  if ((snapshot.key()).indexOf(':') != -1) {
    var coords = snapshot.key().split(":");
    draw(coords[0],coords[1],'#000000');
  }
};

dataRef.on('child_added', drawPixel);
dataRef.on('child_changed', drawPixel);
dataRef.on('child_removed', clearPixel);  
