let meters = document.getElementsByClassName('number');
let botones = document.querySelectorAll('.contenedor_botones button');
let contenedorMeter = document.querySelectorAll('.contenedor-meter-img');
let text;

function botonSleccionado (x) {
	// for (let i = 0; i < contenedorMeter.length; i++) {
	// 	let span = document.createElement('span');
	// 	span.setAttribute('id','led');
	// 	contenedorMeter[i].appendChild(span);
	// 	setTimeout(function(){ 
	// 		contenedorMeter[i].removeChild(contenedorMeter[i].lastChild);
	// 	}, 7000);
	// }
	
	botonSeleccionado(x);

	switch (x) {
	  case 0:
	    console.log('Button Real consumption');
            socket.emit( 'botones', {data: 'real'});
	    break;
	  case 1:
	    console.log('Button Last hour');
            socket.emit( 'botones', {data: 'last_hour'});
	    break;
	  case 2:
	    console.log('Button Last day');
            socket.emit( 'botones', {data: 'last_day'});
	    break;
	  case 3:
	    console.log('Button 7 days');
            socket.emit( 'botones', {data: 'last_week'});            
	    break;
	  case 4:
	    console.log('Button 30 days');
            socket.emit( 'botones', {data: 'last_month'});            
	    break;
	}

	// Borra todos los medidores
	// for (let i = 0; i < meters.length; i++) {
	// 	meters[i].innerHTML = '';
	// }
}

function botonSeleccionado (x) {
	for (let i = 0; i < botones.length; i++) {
		botones[i].setAttribute('class','');
	}
	botones[x].setAttribute('class','seleccionado');
}

/**
 * Muestra y oculta el password
 *
 * @param number x número del medidor
 * @param number y valor para mostrar en el medidor
 */
function imprimirEnMedidor (x,y) {
	// Imprime el valor de la variable y en el medidor seleccionado
	meters[x].innerHTML = y;
	// Crea la luz intermitente durante 2 segundos
	let span = document.createElement('span');
	span.setAttribute('id','led');
	contenedorMeter[x].appendChild(span);
	setTimeout(function(){ 
		contenedorMeter[x].removeChild(contenedorMeter[x].lastChild);
	}, 1000);
}

imprimirEnMedidor(0,'waiting');
imprimirEnMedidor(1,'waiting');
imprimirEnMedidor(2,'waiting');
imprimirEnMedidor(3,'waiting');


// websocket comms
var socket = io.connect('http://' + document.domain + ':' + location.port);

socket.on('meter1', function(msg) {
    console.log(msg);
    let meter1_data = msg.data;
    console.log('meter1_data: ' + meter1_data);
    imprimirEnMedidor(0, meter1_data);
})

socket.on('meter2', function(msg) {
    console.log(msg);
    let meter2_data = msg.data;
    console.log('meter2_data: ' + meter2_data);
    imprimirEnMedidor(1, meter2_data);
})

socket.on('meter3', function(msg) {
   console.log(msg);
    let meter3_data = msg.data;
    console.log('meter3_data: ' + meter3_data);
    imprimirEnMedidor(2, meter3_data);
})

socket.on('meter4', function(msg) {
    console.log(msg);
    let meter4_data = msg.data;
    console.log('meter4_data: ' + meter4_data);
    imprimirEnMedidor(3, meter4_data);
})

socket.on('redirect', function(msg) {
    console.log(msg);
    let user_login = msg.data;
   console.log("username: " + user_login + " not allowed here");
    window.location.replace('/login');
})

socket.on('button_in', function(msg) {
    console.log(msg);
    if (msg.data === 'real')
        botonSeleccionado(0);
    else if (msg.data === 'last_hour')
        botonSeleccionado(1);
    else if (msg.data === 'last_day')
        botonSeleccionado(2);
    else if (msg.data === 'last_week')
        botonSeleccionado(3);
    else if (msg.data === 'last_month')
        botonSeleccionado(4);
    
})

