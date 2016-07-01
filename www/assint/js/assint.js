function ctrlDisplay(idAbre, idFecha){
	var d			 = document;
	var displayAbre  = ( navigator.appName.indexOf('Explorer') > -1 ? 'block' : 'table-row');
	var displayFecha = 'none';
	
	if (typeof(idAbre) != 'object' && idAbre != ''){
		idAbre = new Array(idAbre);
	}
	
	if (typeof(idFecha) != 'object'  && idFecha != ''){
		idFecha = new Array(idFecha);
	}
	
	// Abre
	for (i=0; i < idAbre.length; i++){
		obj = d.getElementById(idAbre[i]);
		obj.style.display = displayAbre;
	}
	
	// Fecha
	for (i=0; i < idFecha.length; i++){
		obj = d.getElementById(idFecha[i]);
		obj.style.display = displayFecha;
	}
}

function redireciona(url){
	location.href = url;
}

function confirmExcluir(msg, url){
	if ( confirm(msg) )
		location.href = url;
	return;
}

AbrirPopUp = function(url,nome,param){
	var a = window.open(url,nome,param);
	a.focus();
}	
