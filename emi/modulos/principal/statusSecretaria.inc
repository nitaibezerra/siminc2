<?php
 /*
   Sistema Simec
   Setor responsvel: MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Cristiano Cabral (cristiano.cabral@gmail.com)
   Mdulo:inicio.inc
   Finalidade: permitir abrir a página inicial
    */

if ( $_REQUEST["carga"] ){
	
	$emi->listaEstadoSecretaria("carga", $_REQUEST["carga"]);	
	die;
	
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";

$emiAbas = array( 0 => array( "descricao" => "Lista de Estados", "link" => "/emi/emi.php?modulo=inicio&acao=C" ),
				  1 => array( "descricao" => "Status das Secretarias", "link" => "/emi/emi.php?modulo=principal/statusSecretaria&acao=A" ) );  
echo montarAbasArray($emiAbas, "/emi/emi.php?modulo=principal/statusSecretaria&acao=A" );

monta_titulo("Status das Secretarias", "");

$emi->listaEstadoSecretaria();

?>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript">

var params;

function desabilitarConteudo( id ){
	var url ='/emi/emi.php?modulo=principal/statusSecretaria&acao=A&carga='+id;
	if ( document.getElementById('img'+id).name == '-' ) {
		url = url + '&subAcao=retirarCarga';
		var myAjax = new Ajax.Request(
			url,
			{
				method: 'post',
				asynchronous: false
			});
	}
}

</script>