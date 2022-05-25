let meters = document.getElementsByClassName('number');
let botones = document.querySelectorAll('.contenedor_botones button');
let contenedorMeter = document.querySelectorAll('.contenedor-meter-img');
let text;

function botonSleccionado (x) {
	for (let i = 0; i < contenedorMeter.length; i++) {
		let span = document.createElement('span');
		span.setAttribute('id','led');
		contenedorMeter[i].appendChild(span);
		setTimeout(function(){ 
			contenedorMeter[i].removeChild(contenedorMeter[i].lastChild);
		}, 7000);
	}
	
	seleccionar(x);

	switch (x) {
	  case 0:
	    text = "Real";
	    break;
	  case 1:
	    text = "Last hour";
	    break;
	  case 2:
	    text = "Last day";
	    break;
	  case 3:
	    text = "7 days";
	    break;
	  case 4:
	    text = "30 days";
	    break;
	}

	for (let i = 0; i < meters.length; i++) {
		meters[i].innerHTML = text;
	}
}

function seleccionar (x) {
	for (let i = 0; i < botones.length; i++) {
		botones[i].setAttribute('class','');
	}
	botones[x].setAttribute('class','seleccionado');
}

function imprimirEnMedidor (x,y) {
	// Imprime el valor de la variable y en el medidor seleccionado
	meters[x].innerHTML = y;
	// Crea la luz intermitente durante 7 segundos
	let span = document.createElement('span');
	span.setAttribute('id','led');
	contenedorMeter[x].appendChild(span);
	setTimeout(function(){ 
		contenedorMeter[x].removeChild(contenedorMeter[x].lastChild);
	}, 7000);
}

imprimirEnMedidor(0,400);

