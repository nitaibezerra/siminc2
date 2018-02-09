<script language="JavaScript" src="/includes/funcoes.js"></script>
<script language="JavaScript" type="text/javascript" src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<link rel="stylesheet" type="text/css" href="/includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="/includes/listagem.css"/>
<script>

var problemas=false;

function funcaogeral() {
	if(!problemas) {
		jQuery('#log').html(jQuery('#log').html()+"Iniciando serviço...<br>");
		rodarScript();
	}
}

function rodarScript() {
	jQuery.ajax({
   		type: "POST",
   		url: "envio_dados_pagamento_bolsista_sgb.php",
   		data: "limit=10",
   		async: false,
   		success: function(html){
   			var part = html.split("<br/>");
   			if(part[2]=="OK") {
   				problemas=true;
   				jQuery('#log').html(jQuery('#log').html()+"Inicío:"+part[0]+"/Fim:"+part[1]+"<br/>");
   				rodarScript();
   			} else {
   				problemas=false;
   				jQuery('#log').html(jQuery('#log').html()+"Serviço fora...<br>");
   			}
   		}
 		});
	 		
}

window.setInterval('funcaogeral()', 30000);

</script>
<div id="log"></div>