var WebSocketIPAdress = 'ws://192.168.178.42:81';
var WebConnection;

function WebFloatToHexCmdSend(id) {
	var tosendFloat;
	
	var farr = new Float32Array(1);  // indexes each element 4 bytes
	if(id == "buttonSendXOffset") {
		tosendFloat = parseFloat(document.getElementById("CalibrationInput").value);
	}
	else if(id == "buttonSendYOffset") {
		tosendFloat = parseFloat(document.getElementById("CalibrationInput").value);
	}
	else if(id == "buttonSendZOffset") {
		tosendFloat = parseFloat(document.getElementById("CalibrationInput").value);
	}
	else if(id == "buttonSendAVGScale") {
		tosendFloat = parseFloat(document.getElementById("CalibrationInput").value);
	}
	else if(id == "buttonSendXScale") {
		tosendFloat = parseFloat(document.getElementById("CalibrationInput").value);
	}
	else if(id == "buttonSendYScale") {
		tosendFloat = parseFloat(document.getElementById("CalibrationInput").value);
	}
	else if(id == "buttonSendZScale") {
		tosendFloat = parseFloat(document.getElementById("CalibrationInput").value);
	}
	else if(id == "buttonSendKp") {
		tosendFloat = parseFloat(document.getElementById("CalibrationInput").value);
	}
	else if(id == "buttonSendKi") {
		tosendFloat = parseFloat(document.getElementById("CalibrationInput").value);
	}
	else {
		console.log("id not found");
		return;
	}
	
	farr[0] = tosendFloat;
	
	var barr = new Uint8Array(farr.buffer);   // use the buffer of Float32Array view
	var arraybuffer = new Uint8Array(10);
	arraybuffer[0] = 0x11;
	arraybuffer[1] = 0x00;
	arraybuffer[2] = 0x00;
	arraybuffer[3] = 0x00;
	arraybuffer[4] = 0x00;
	
	arraybuffer[5] = barr[0];
	arraybuffer[6] = barr[1];
	arraybuffer[7] = barr[2];
	arraybuffer[8] = barr[3];
	arraybuffer[9] = 0x12;
	
	// 11 03 01 01 00 00 00 00 00 12
	// -- -- -- -- -- ## ## ## ## --
	if(id == "buttonSendXOffset") {
		arraybuffer[1] = 0x03;
		arraybuffer[2] = 0x01;
		arraybuffer[3] = 0x01;
		arraybuffer[4] = 0x00;
	}
	// 11 03 01 02 00 00 00 00 00 12
	// -- -- -- -- -- ## ## ## ## --
	else if(id == "buttonSendYOffset") {
		arraybuffer[1] = 0x03;
		arraybuffer[2] = 0x01;
		arraybuffer[3] = 0x02;
		arraybuffer[4] = 0x00;
	}
	// 11 03 01 03 00 00 00 00 00 12
	// -- -- -- -- -- ## ## ## ## --
	else if(id == "buttonSendZOffset") {
		arraybuffer[1] = 0x03;
		arraybuffer[2] = 0x01;
		arraybuffer[3] = 0x03;
		arraybuffer[4] = 0x00;
	}
	// 11 03 01 04 00 00 00 00 00 12
	// -- -- -- -- -- ## ## ## ## --
	else if(id == "buttonSendAVGScale") {
		arraybuffer[1] = 0x03;
		arraybuffer[2] = 0x01;
		arraybuffer[3] = 0x04;
		arraybuffer[4] = 0x00;
	}
	// 11 03 01 04 00 00 00 00 00 12
	// -- -- -- -- -- ## ## ## ## --
	else if(id == "buttonSendXScale") {
		arraybuffer[1] = 0x03;
		arraybuffer[2] = 0x01;
		arraybuffer[3] = 0x05;
		arraybuffer[4] = 0x00;
	}
	// 11 03 01 04 00 00 00 00 00 12
	// -- -- -- -- -- ## ## ## ## --
	else if(id == "buttonSendYScale") {
		arraybuffer[1] = 0x03;
		arraybuffer[2] = 0x01;
		arraybuffer[3] = 0x06;
		arraybuffer[4] = 0x00;
	}
	// 11 03 01 04 00 00 00 00 00 12
	// -- -- -- -- -- ## ## ## ## --
	else if(id == "buttonSendZScale") {
		arraybuffer[1] = 0x03;
		arraybuffer[2] = 0x01;
		arraybuffer[3] = 0x07;
		arraybuffer[4] = 0x00;
	}
	// 11 03 01 04 00 00 00 00 00 12
	// -- -- -- -- -- ## ## ## ## --
	else if(id == "buttonSendKp") {
		arraybuffer[1] = 0x03;
		arraybuffer[2] = 0x01;
		arraybuffer[3] = 0x08;
		arraybuffer[4] = 0x00;
	}
	// 11 03 01 04 00 00 00 00 00 12
	// -- -- -- -- -- ## ## ## ## --
	else if(id == "buttonSendKi") {
		arraybuffer[1] = 0x03;
		arraybuffer[2] = 0x01;
		arraybuffer[3] = 0x09;
		arraybuffer[4] = 0x00;
	}
	
	WebConnection.send(arraybuffer.buffer);
	
	// console.log(barr);
	// console.log(arraybuffer);
	// console.log(tosendFloat);
}

document.getElementById("WebSocketIPScreen").innerHTML = WebSocketIPAdress;
    WebConnection = new WebSocket(WebSocketIPAdress, ['arduino']);
    WebConnection.binaryType = "arraybuffer"; //Binaertyp auf arraybuffer setzen

WebConnection.onopen = function () {
	console.log("Succesful connected!");
    screenStatus(true);
};

WebConnection.onerror = function (error) {
	console.log('WebSocket Error: ', error);
	screenStatus(2);
};

WebConnection.onmessage = function (e) {
	// receive String
	if(typeof e.data === 'string' ) {
		console.log('Server: ', e.data);
		storeReceivedData(e.data);
   }
	// receive Array
	if(e.data instanceof ArrayBuffer ){
	  var hexBuf = buf2hex(e.data);
      console.log("Received arraybuffer: " + hexBuf);
	  if(checkIfValIsKnown(hexBuf)) {
		StoreReceivedHexData(hexBuf);
	}
   }
   // console.log("Something received");
};

WebConnection.onclose = function (e) {
	screenStatus(false);
}

function refreshIPAdress(_address)
{
	WebSocketIPAdress = _address;
	retryConnection();
}

function retryConnection() {
	WebConnection = new WebSocket(WebSocketIPAdress, ['arduino']);
    WebConnection.binaryType = "arraybuffer"; //Binaertyp auf arraybuffer setzen
}

function checkIfValIsKnown(value) {
	var retVal = 1;
	var dev = value.substring(2, 10);
	switch(dev) {
		case "01001000":
			payload = value.substring(10, 18);
			document.getElementById("BatteryStatusDisplay").innerHTML = parseInt(payload, 16).toString() + "%";
		break;
		case "01000800":
			payload = value.substring(10, 18);
			document.getElementById("UltrasonicValuesDisplay").innerHTML = parseInt(payload, 16).toString() + "mm";
		break;
		case "01000000":
			payload = value.substring(10, 18);
			document.getElementById("buttonLidar0ValuesDisplay").innerHTML = parseInt(payload, 16).toString() + "mm";
		break;
		case "01000100":
			payload = value.substring(10, 18);
			document.getElementById("buttonLidar1ValuesDisplay").innerHTML = parseInt(payload, 16).toString() + "mm";
		break;
		case "01000200":
			payload = value.substring(10, 18);
			document.getElementById("buttonLidar2ValuesDisplay").innerHTML = parseInt(payload, 16).toString() + "mm";
		break;
		case "01000300":
			payload = value.substring(10, 18);
			document.getElementById("buttonLidar3ValuesDisplay").innerHTML = parseInt(payload, 16).toString() + "mm";
		break;
		case "01000400":
			payload = value.substring(10, 18);
			document.getElementById("buttonLidar4ValuesDisplay").innerHTML = parseInt(payload, 16).toString() + "mm";
		break;
		case "01000500":
			payload = value.substring(10, 18);
			document.getElementById("buttonLidar5ValuesDisplay").innerHTML = parseInt(payload, 16).toString() + "mm";
		break;
		case "01000600":
			payload = value.substring(10, 18);
			document.getElementById("buttonLidar6ValuesDisplay").innerHTML = parseInt(payload, 16).toString() + "mm";
		break;
		case "01000c00":
			payload = value.substring(10, 18);
			document.getElementById("LTFLRawDisplay").innerHTML = parseInt(payload, 16).toString();
		break;
		case "01000d00":
			payload = value.substring(10, 18);
			document.getElementById("LTFRRawDisplay").innerHTML = parseInt(payload, 16).toString();
		break;
		case "01000e00":
			payload = value.substring(10, 18);
			document.getElementById("LTBLRawDisplay").innerHTML = parseInt(payload, 16).toString();
		break;
		case "01000f00":
			payload = value.substring(10, 18);
			document.getElementById("LTBRRawDisplay").innerHTML = parseInt(payload, 16).toString();
		break;
		case "01001100":
			payload = value.substring(10, 18);
			if(parseInt(payload, 16).toString() == "1") { // floor
				document.getElementById("LTFLDisplay").style.backgroundColor= "white";
			}
			else if(parseInt(payload, 16).toString() == "0") { // line
				document.getElementById("LTFLDisplay").style.backgroundColor= "black";
			}
			else if(parseInt(payload, 16).toString() == "2") { // enemy goal
				document.getElementById("LTFLDisplay").style.backgroundColor= "yellow";
			}
			else if(parseInt(payload, 16).toString() == "3") { // our goal
				document.getElementById("LTFLDisplay").style.backgroundColor= "green";
			}
			else { // error
				document.getElementById("LTFLDisplay").style.backgroundColor= "red";
			}
		break;
		case "01001200":
			payload = value.substring(10, 18);
			if(parseInt(payload, 16).toString() == "1") { // floor
				document.getElementById("LTFRDisplay").style.backgroundColor= "white";
			}
			else if(parseInt(payload, 16).toString() == "0") { // line
				document.getElementById("LTFRDisplay").style.backgroundColor= "black";
			}
			else if(parseInt(payload, 16).toString() == "2") { // enemy goal
				document.getElementById("LTFRDisplay").style.backgroundColor= "yellow";
			}
			else if(parseInt(payload, 16).toString() == "3") { // our goal
				document.getElementById("LTFRDisplay").style.backgroundColor= "green";
			}
			else { // error
				document.getElementById("LTFRDisplay").style.backgroundColor= "red";
			}
		break;
		case "01001300":
			payload = value.substring(10, 18);
			if(parseInt(payload, 16).toString() == "1") { // floor
				document.getElementById("LTBLDisplay").style.backgroundColor= "white";
			}
			else if(parseInt(payload, 16).toString() == "0") { // line
				document.getElementById("LTBLDisplay").style.backgroundColor= "black";
			}
			else if(parseInt(payload, 16).toString() == "2") { // enemy goal
				document.getElementById("LTBLDisplay").style.backgroundColor= "yellow";
			}
			else if(parseInt(payload, 16).toString() == "3") { // our goal
				document.getElementById("LTBLDisplay").style.backgroundColor= "green";
			}
			else { // error
				document.getElementById("LTBLDisplay").style.backgroundColor= "red";
			}
		break;
		case "01001400":
			payload = value.substring(10, 18);
			if(parseInt(payload, 16).toString() == "1") { // floor
				document.getElementById("LTBRDisplay").style.backgroundColor= "white";
			}
			else if(parseInt(payload, 16).toString() == "0") { // line
				document.getElementById("LTBRDisplay").style.backgroundColor= "black";
			}
			else if(parseInt(payload, 16).toString() == "2") { // enemy goal
				document.getElementById("LTBRDisplay").style.backgroundColor= "yellow";
			}
			else if(parseInt(payload, 16).toString() == "3") { // our goal
				document.getElementById("LTBRDisplay").style.backgroundColor= "green";
			}
			else { // error
				document.getElementById("LTBRDisplay").style.backgroundColor= "red";
			}
		break;
		case "01001a00":
			// Color Sensor
			payload = value.substring(10, 18);
			if(parseInt(payload, 16).toString() == "1") { // floor
				document.getElementById("CSDisplay").style.backgroundColor= "white";
			}
			else if(parseInt(payload, 16).toString() == "0") { // line
				document.getElementById("CSDisplay").style.backgroundColor= "black";
			}
			else if(parseInt(payload, 16).toString() == "2") { // enemy goal
				document.getElementById("CSDisplay").style.backgroundColor= "yellow";
			}
			else if(parseInt(payload, 16).toString() == "3") { // our goal
				document.getElementById("CSDisplay").style.backgroundColor= "green";
			}
			else { // error
				document.getElementById("CSDisplay").style.backgroundColor= "red";
			}
		break;
		/*
		case "01001700":
			payload = value.substring(10, 18);
			var buf = new ArrayBuffer(4);
			var view = new DataView(buf);
			data.forEach(function(b,i) {
				view.setUint8(i, b);
			});
			var num = view.getFloat32(0);
			document.getElementById("Pitch").innerHTML = parseFloat(payload, 16).toString();
		break;
		
		case "01001800":
			payload = value.substring(10, 18);
			var buf = new ArrayBuffer(4);
			var view = new DataView(buf);
			data.forEach(function(b,i) {
				view.setUint8(i, b);
			});
			var num = view.getFloat32(0);
			document.getElementById("Roll").innerHTML = parseFloat(payload, 16).toString();
		break;
		*/
		case "01001b00":
			// speed
			payload = value.substring(10, 18);
			
			var p0 = payload.substring(0, 2);
			var p1 = payload.substring(2, 4);
			var p2 = payload.substring(4, 6);
			var p3 = payload.substring(6, 8);
			
			var data = [ parseInt(p3, 16), parseInt(p2, 16), parseInt(p1, 16), parseInt(p0, 16)];
			
			var buf = new ArrayBuffer(4);
			var view = new DataView(buf);
			data.forEach(function(b,i) {
				view.setUint8(i, b);
			});
			var num = view.getFloat32(0);
			document.getElementById("buttonSpeedValuesDisplay").innerHTML = (num.toString().substring(0,5) + "m/s");
		break;
		case "01001600":
			// heading
			payload = value.substring(10, 18);
			
			var p0 = payload.substring(0, 2);
			var p1 = payload.substring(2, 4);
			var p2 = payload.substring(4, 6);
			var p3 = payload.substring(6, 8);
			
			var data = [ parseInt(p3, 16), parseInt(p2, 16), parseInt(p1, 16), parseInt(p0, 16)];
			
			var buf = new ArrayBuffer(4);
			var view = new DataView(buf);
			data.forEach(function(b,i) {
				view.setUint8(i, b);
			});
			var num = view.getFloat32(0);
			document.getElementById("GetHeadingDisplay").innerHTML = (num.toString().substring(0,5) + "°");
			break;
		// get x offset
		case "01001f00":
			payload = value.substring(10, 18);
			
			var p0 = payload.substring(0, 2);
			var p1 = payload.substring(2, 4);
			var p2 = payload.substring(4, 6);
			var p3 = payload.substring(6, 8);
			
			var data = [ parseInt(p3, 16), parseInt(p2, 16), parseInt(p1, 16), parseInt(p0, 16)];
			
			var buf = new ArrayBuffer(4);
			var view = new DataView(buf);
			data.forEach(function(b,i) {
				view.setUint8(i, b);
			});
			var num = view.getFloat32(0);
		document.getElementById("buttonGetXOffsetDisplay").innerHTML = (num.toString().substring(0,5));
		break;
		// get y offset
		case "01002000":
			payload = value.substring(10, 18);
			
			var p0 = payload.substring(0, 2);
			var p1 = payload.substring(2, 4);
			var p2 = payload.substring(4, 6);
			var p3 = payload.substring(6, 8);
			
			var data = [ parseInt(p3, 16), parseInt(p2, 16), parseInt(p1, 16), parseInt(p0, 16)];
			
			var buf = new ArrayBuffer(4);
			var view = new DataView(buf);
			data.forEach(function(b,i) {
				view.setUint8(i, b);
			});
			var num = view.getFloat32(0);
		document.getElementById("buttonGetYOffsetDisplay").innerHTML = (num.toString().substring(0,5));
		break;
		// get z offset
		case "01002100":
			payload = value.substring(10, 18);
			
			var p0 = payload.substring(0, 2);
			var p1 = payload.substring(2, 4);
			var p2 = payload.substring(4, 6);
			var p3 = payload.substring(6, 8);
			
			var data = [ parseInt(p3, 16), parseInt(p2, 16), parseInt(p1, 16), parseInt(p0, 16)];
			
			var buf = new ArrayBuffer(4);
			var view = new DataView(buf);
			data.forEach(function(b,i) {
				view.setUint8(i, b);
			});
			var num = view.getFloat32(0);
		document.getElementById("buttonGetZOffsetDisplay").innerHTML = (num.toString().substring(0,5));
		break;
		// get avg scale display
		case "01002200":
			payload = value.substring(10, 18);
			
			var p0 = payload.substring(0, 2);
			var p1 = payload.substring(2, 4);
			var p2 = payload.substring(4, 6);
			var p3 = payload.substring(6, 8);
			
			var data = [ parseInt(p3, 16), parseInt(p2, 16), parseInt(p1, 16), parseInt(p0, 16)];
			
			var buf = new ArrayBuffer(4);
			var view = new DataView(buf);
			data.forEach(function(b,i) {
				view.setUint8(i, b);
			});
			var num = view.getFloat32(0);
		document.getElementById("buttonGetGetAVGScaleDisplay").innerHTML = (num.toString().substring(0,5));
		break;
		case "01002300":
			payload = value.substring(10, 18);
			
			var p0 = payload.substring(0, 2);
			var p1 = payload.substring(2, 4);
			var p2 = payload.substring(4, 6);
			var p3 = payload.substring(6, 8);
			
			var data = [ parseInt(p3, 16), parseInt(p2, 16), parseInt(p1, 16), parseInt(p0, 16)];
			
			var buf = new ArrayBuffer(4);
			var view = new DataView(buf);
			data.forEach(function(b,i) {
				view.setUint8(i, b);
			});
			var num = view.getFloat32(0);
		document.getElementById("buttonGetXScaleDisplay").innerHTML = (num.toString().substring(0,5));
		break;
		case "01002400":
			payload = value.substring(10, 18);
			
			var p0 = payload.substring(0, 2);
			var p1 = payload.substring(2, 4);
			var p2 = payload.substring(4, 6);
			var p3 = payload.substring(6, 8);
			
			var data = [ parseInt(p3, 16), parseInt(p2, 16), parseInt(p1, 16), parseInt(p0, 16)];
			
			var buf = new ArrayBuffer(4);
			var view = new DataView(buf);
			data.forEach(function(b,i) {
				view.setUint8(i, b);
			});
			var num = view.getFloat32(0);
		document.getElementById("buttonGetYScaleDisplay").innerHTML = (num.toString().substring(0,5));
		break;
		case "01002500":
			payload = value.substring(10, 18);
			
			var p0 = payload.substring(0, 2);
			var p1 = payload.substring(2, 4);
			var p2 = payload.substring(4, 6);
			var p3 = payload.substring(6, 8);
			
			var data = [ parseInt(p3, 16), parseInt(p2, 16), parseInt(p1, 16), parseInt(p0, 16)];
			
			var buf = new ArrayBuffer(4);
			var view = new DataView(buf);
			data.forEach(function(b,i) {
				view.setUint8(i, b);
			});
			var num = view.getFloat32(0);
		document.getElementById("buttonGetZScaleDisplay").innerHTML = (num.toString().substring(0,5));
		break;
		case "01002600":
			payload = value.substring(10, 18);
			
			var p0 = payload.substring(0, 2);
			var p1 = payload.substring(2, 4);
			var p2 = payload.substring(4, 6);
			var p3 = payload.substring(6, 8);
			
			var data = [ parseInt(p3, 16), parseInt(p2, 16), parseInt(p1, 16), parseInt(p0, 16)];
			
			var buf = new ArrayBuffer(4);
			var view = new DataView(buf);
			data.forEach(function(b,i) {
				view.setUint8(i, b);
			});
			var num = view.getFloat32(0);
		document.getElementById("buttonGetKpDisplay").innerHTML = (num.toString().substring(0,5));
		break;
		case "01002700":
			payload = value.substring(10, 18);
			
			var p0 = payload.substring(0, 2);
			var p1 = payload.substring(2, 4);
			var p2 = payload.substring(4, 6);
			var p3 = payload.substring(6, 8);
			
			var data = [ parseInt(p3, 16), parseInt(p2, 16), parseInt(p1, 16), parseInt(p0, 16)];
			
			var buf = new ArrayBuffer(4);
			var view = new DataView(buf);
			data.forEach(function(b,i) {
				view.setUint8(i, b);
			});
			var num = view.getFloat32(0);
		document.getElementById("buttonGetKiDisplay").innerHTML = (num.toString().substring(0,5));
		break;
	  default:
	  console.log("unknown binary stream");
	  retVal = 1;
	} 
	return retVal;
}

function sendCommandOnClick(element) { // sends commands to the client
	var tosend;
	tosend = document.getElementById(element).value; // get the data, which has to be sent
	WebConnection.send(tosend);
	console.log(tosend);
	storeSentData(tosend, true);
}

function buf2hex(buffer) { // buffer is an ArrayBuffer
  return Array.prototype.map.call(new Uint8Array(buffer), x => ('00' + x.toString(16)).slice(-2)).join('');
}

function StrToByteArr(str) {
	var charArr = str.split('');
	var hexPos = str.length - 1;
	var numOfBytes = 0;
	if ( str.length % 2 == 0)
		 numOfBytes = parseInt(str.length/2);
	else
		numOfBytes = parseInt(str.length/2) + 1;
	
	var binary = new Uint8Array(numOfBytes);
	for(var i = 0; i < numOfBytes; i++) {
		binary[i] = parseInt(charArr[hexPos], 16);
		hexPos--;
		if (hexPos >= 0) {
			var shifted = parseInt(charArr[hexPos], 16) << 4;
			hexPos--;
			binary[i] = binary[i] | shifted;
		}
		//console.log(binary[i].toString(16));
		
	}
	return binary.reverse();
	//return binary;
}

function StoreReceivedHexData(toStore) {
	try { 																	// code that may cause an error
        var inStorage = document.getElementById("StoreHexReceived").value; 		// get old data
        toStore = toStore + "\r"; 											// add newline to new data
        inStorage = inStorage + toStore;									// connect data
        document.getElementById("StoreHexReceived").value = inStorage; 			// write new data on page
    }
    catch (e) { 																// on error
        console.log("Could not store data in StoreHexReceived");
    }
}

function WebHexCmdSend(commandSrc) {
	var tosendStr = document.getElementById(commandSrc).value;
	var toSend = StrToByteArr(tosendStr);
	WebConnection.send(toSend.buffer);
	// StoreSentData("0x" + tosendStr);
	
	try { 																	// code that may cause an error
        var inStorage = document.getElementById("StoreHexSent").value; 		// get old data
        var toStore = tosendStr + "\r"; 											// add newline to new data
        inStorage = inStorage + toStore;									// connect data
        document.getElementById("StoreHexSent").value = inStorage; 			// write new data on page
    }
    catch (e) { 																// on error
        console.log("Could not store data in StoreHexSent");
    }
};

function EndWebsocketClient() {
	console.log("close");
	WebConnection.close();
	screenStatus(0);
}
function screenStatus(status) {
	if(status == 1) {
		document.getElementById("WebSocketConnectionStateScreen").innerHTML = "<span style='color:green'> Connected! </span>";
	}
	else if(status == 0) {
		document.getElementById("WebSocketConnectionStateScreen").innerHTML = "<span style='color:blue'> Disconnected! </span>";
	}
	else if(status == 2) {
		document.getElementById("WebSocketConnectionStateScreen").innerHTML = "<span style='color:red'> ERROR! </span>";
	}
}

function storeSentData(toStore) {
	try{ 																	// code that may cause an error
		var inStorage = document.getElementById("commandStore").value; 		// get old data
		toStore = toStore + "\r"; 											// add newline to new data
		inStorage = inStorage + toStore;									// connect data
		document.getElementById("commandStore").value = inStorage; 			// write new data on page
	}
	catch(e){ 																// on error
		console.log("Could not store data in commandStore");
	}
	try{ 																	// code that may cause an error
		inStorage = document.getElementById("AllStore").value; 				// get old data
		toStore = new Date() + " cmd : " + toStore; 						// add newline to new data
		inStorage = inStorage + toStore;									// connect data
		document.getElementById("AllStore").value = inStorage; 				// write new data on page
	}
	catch(e){ 																// on error
		console.log("Could not store data in AllStore");
	}
}

function storeReceivedData(toStore) {
	try{ 																	// code that may cause an error
		var inStorage = document.getElementById("receiveStore").value; 		// get old data
		toStore = toStore + "\r"; 											// add newline to new data
		inStorage = inStorage + toStore;									// connect data
		document.getElementById("receiveStore").value = inStorage; 			// write new data on page
	}
	catch(e){ 																// on error
		console.log("Could not store data in receiveStore");
    }

	try{ 																	// code that may cause an error
		inStorage = document.getElementById("AllStore").value; 			// get old data
		toStore = new Date() + " " + WebSocketIPAdress + " : " + toStore; 		// add newline to new data
		inStorage = inStorage + toStore;									// connect data
		document.getElementById("AllStore").value = inStorage; 				// write new data on page
	}
	catch(e){ 																// on error
		console.log("Could not store data in AllStore");
	}
}

var my_gamepad = null;
var refreshIntervalIdJoystick = null;

function OnJoystickConnect() {
    window.addEventListener("gamepadconnected", function(e) {
		my_gamepad = e.gamepad;
	})
	refreshIntervalIdJoystick = window.setInterval(CheckJoy, 250); // CheckJoy wird alle 250ms aufgerufen
}

function OnJoystickDisconnect() {
	clearInterval(refreshIntervalIdJoystick);
	window.removeEventListener('gamepadconnected', my_gamepad);
}

function CheckJoy() {
	if(my_gamepad != null) {
		console.log("Joy %f %f", my_gamepad.axes[0], my_gamepad.axes[3]);
		var xAxes = my_gamepad.axes[0] * 128 + 128; //LeftRight
		var yAxes = 0;
		var direction = 1;
		if(my_gamepad.axes[3] > 0){
			direction = 0;
			yAxes = 255 - my_gamepad.axes[3] * 255;
		}
		else
		{
			direction = 1;
			yAxes = -my_gamepad.axes[3] * 255;
		}
		if(xAxes > 255) {
			xAxes = 255;
		}
		if(xAxes < 1) {
			xAxes = 1;
		}
		console.log("LeftRight: %f, Velocity: %f, direction: %d", xAxes, yAxes, direction);

		var binary = new Uint8Array(10);
		binary[0] = 0x11;
		binary[1] = 0x03;
		binary[2] = 0x00;
		binary[3] = 0x00;
		binary[4] = 0x00;
		binary[5] = 0x00;
		binary[6] = 0x00;
		binary[7] = direction;
		binary[8] = yAxes;
		binary[9] = 0x12;
		WebConnection.send(binary);
		binary[3] = 0x01;
		binary[7] = 0x00;
		binary[8] = xAxes;
		WebConnection.send(binary);
		if (my_gamepad.buttons[0] == true){
			binary[1] = 0x02;
			binary[2] = 0x01;
			binary[8] = 0x00;
			WebConnection.send(binary);
		}
	}
}
var coll = document.getElementsByClassName("collapsible");
var i;

for (i = 0; i < coll.length; i++) {
  coll[i].addEventListener("click", function() {
    this.classList.toggle("active");
    var content = this.nextElementSibling;
    if (content.style.display === "block") {
      content.style.display = "none";
    } else {
      content.style.display = "block";
    }
  });
}
