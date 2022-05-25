/**
 * Muestra y oculta el password
 * 
 */

let ojo = document.querySelector('.ojo');

let inputPassword = document.querySelector('input[type=password]');

ojo.onclick = function () {
	inputPassword.setAttribute("type", "text");
	if (this.getAttribute('class') == 'icon icon-eye-slash ojo') {
		ojo.removeAttribute("class");
		ojo.setAttribute('class','icon icon-eye ojo');
		inputPassword.setAttribute("type", "text");
	} else {
		ojo.removeAttribute("class");
		ojo.setAttribute('class','icon icon-eye-slash ojo');
		inputPassword.setAttribute("type", "password");
	}
}
