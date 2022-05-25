
let d = document;
let btn = d.querySelector('.cambiar');
let input = d.querySelector('input[name=meter]');
let inputValue;
let errorMensaje = d.querySelector('.mensaje_error');
let mensajeError = d.querySelector('.mensaje_error');
let listado = d.querySelectorAll('.listado');
let update = d.querySelectorAll('.update');
let meterText = d.querySelectorAll('.meter-text');
let cambiar = d.getElementById('meter');
let cambiarTexto = d.getElementById('cambiar-text');

let modal = d.getElementById("myModal");

let letterNumber = /^[0-4mM+-\s]+$/;

btn.onclick = function () {
	inputValue = d.getElementById('meter').value;

	if ( inputValue === '') {
		errorMensaje.style.cssText = 'display: block';
		errorMensaje.innerHTML = 'The field cannot be empty';
	} else if (inputValue.trim().length != inputValue.length){
		errorMensaje.style.cssText = 'display: block';
		errorMensaje.innerHTML = 'The field cannot have spaces';
	} else if (!inputValue.match(letterNumber)){
		errorMensaje.style.cssText = 'display: block';
		errorMensaje.innerHTML = 'You are entering letters and/or numbers not allowed';
	} else {
		removerMensajes();
		listado[this.getAttribute('data-id') - 1].innerHTML = inputValue;
		modal.style.display = "none";
	}
}

for (let i = 0; i < update.length ; i++) {
	update[i].onclick = function () {
		removerMensajes();
		cambiar.value = listado[i].innerText;
		btn.setAttribute('data-id',i+1);
		cambiarTexto.innerHTML = 'Here you can update meter # ' + (i+1);
		modal.style.display = "block";
	}
}

let span = d.getElementsByClassName("close")[0];

// Cierra la modal
span.onclick = function() {
  modal.style.display = "none";
}

// Cierra la modal si hace click afuera de la misma
window.onclick = function(event) {
  if (event.target == modal) {
    modal.style.display = "none";
  }
}

// Limpia los mensajes de la modal
function removerMensajes () {
		mensajeError.style.cssText = 'display: none';
		btn.previousElementSibling.value = '';
}


/* REGISTERED USERS */
let txt = '[{"nombre": "MED CPANEL","password": "12345","comentario":"Alo","status":"0"},{"nombre": "MED2 CPANEL","password": "123444445","comentario":"Alo","status":"0"}]';
let tabla_dinamica_cpanel = d.querySelector('#usuarios_cpanel');

function insert(txt) {
	let tr, td_1, td_2, boton_1;
	let aUsers = JSON.parse(txt);

	for (let i = 0; i < aUsers.length; i++) {
		tr = d.createElement('tr');
		tr.setAttribute('class','user');
		td_1 = d.createElement('td');
		td_2 = d.createElement('td');
		boton_1 = d.createElement('button');
		boton_1.innerHTML = 'DELETE';
		
		boton_1.setAttribute('class','tabla_botones');
		boton_1.setAttribute('data-id',i);
		boton_1.onclick = SacarUsuario;
		td_2.setAttribute('class','wrap_edit_delete');
		
		td_2.appendChild(boton_1);
		td_1.innerHTML = aUsers[i].nombre;
			
		tr.appendChild(td_1);
		tr.appendChild(td_2);
		
		tabla_dinamica_cpanel.appendChild(tr);
		function SacarUsuario () {
			d.querySelector('#usuarios_cpanel').removeChild(this.parentNode.parentNode);
			let id = this.getAttribute('data-id');
			Remover(id);
		}
	}
}

insert(txt);

function Remover (pos) {
	let aUsers = JSON.parse(txt);
	aUsers.splice(pos,1);
	let myJSON = JSON.stringify(aUsers);
	txt = myJSON;
	showMessage();
}

let user_deleted = d.querySelector('.user_deleted');

// Mustra el mensaje de usuario borrado
function showMessage () {
	user_deleted.innerHTML = '* The user was erased successfully';
	user_deleted.setAttribute('class','already_register green');
}