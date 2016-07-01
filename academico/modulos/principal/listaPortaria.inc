<?php
if ( $_REQUEST["carga"] ){
	$academico_lista = new autoriazacaoconcursos();
	$academico_lista->listaportariasprov( $_REQUEST["carga"], true );
	die;
}

require_once APPRAIZ . 'includes/cabecalho.inc';
echo '<br/>';

$_SESSION['academico']['entidcampus'] = $_REQUEST['entid'] ? $_REQUEST['entid'] : $_SESSION['academico']['entidcampus'];  
$entidcampus = $_SESSION['academico']['entidcampus'] ? $_SESSION['academico']['entidcampus'] : $_REQUEST['entid']; 

$ano			= $_REQUEST['ano'] ?  $_REQUEST['ano'] : '2008';

// objeto da classe do módulo
$autoriazacaoconcursos = new autoriazacaoconcursos();

// Monta as abas e o título da tela
#$db->cria_aba($abacod_tela,$url,$parametros);
monta_titulo( "Lista de Portarias de Autorização de Concursos", "");

// mensagem de bloqueio
$bloqueado = academico_mensagem_bloqueio( $_SESSION['academico']['orgid']);

?>
<div class='col-md-2' style='position:static;'>
    <?=cria_aba_2($abacod_tela, $url, $parametros);?>
</div>
<div class='col-md-9' style='position:static;'>
    <? echo $cabecalhoEdital; ?>
<?
if ($_SESSION["academico"]["entidcampus"]){
	echo $autoriazacaoconcursos->cabecalho_entidade($_SESSION['academico']['entidcampus']);	
}
?>
<table class='tabela' bgcolor='#f5f5f5' cellSpacing='1' cellPadding='3' align='center' style="border-top: 0px;">
	<tr>
		<td class="SubTituloEsquerda">Portarias</td>
	</tr>
	<tr>
		<td>&nbsp;</td>
	</tr>
	<tr>
		<td>
<?
// Monta abas de ano
echo montarAbasArray(carregardadosmenuacademico('listaportaria'), "/academico/academico.php?modulo=principal/listaPortaria&acao=A&entidcampus={$_SESSION['academico']['entidcampus']}&ano=$ano");	
echo "<center><div style='background: #DCDCDC; width:95%; height:3px;'>&nbsp;</div></center>";
$existecampus = academico_existecampus( $entidcampus );

if( !$existecampus ){
	echo "<script>
			alert('O Campus informado não existe!');
			history.back(-1);
		  </script>";
	die;
}

$possui_permisao = academico_verificapermissao( $_REQUEST["prtid"], $entidcampus );

if( !$possui_permisao ){
	echo "<script>
			alert('Você não possui permissão para visualizar as portarias deste campus!');
			history.back(-1);
		  </script>";
	die;
}

// Lista de portarias do campus	
$autoriazacaoconcursos->listacampusportarias($entidcampus, $ano);
?>
		</td>
	</tr>
	<tr>
		<td>&nbsp;</td>
	</tr>
</table>
</div>
<!-- biblioteca javascript local -->
<link rel="stylesheet" type="text/css" href="../includes/superTitle.css"/>
<script type="text/javascript" src="../includes/remedial.js"></script>
<script type="text/javascript" src="../includes/superTitle.js"></script>
<script language="javascript" type="text/javascript" src="../includes/calendario.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/listagem2.css">
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript">

	function usaPortaria(prtid, entid, prgid){
		location.href='academico.php?modulo=principal/listareditais&acao=C&evento=A&entidcampus='+entid+'&prtid='+ prtid+'&prgid='+prgid;
	}

	function alteraexercicio( entid, ano ){
		return window.location = '?modulo=principal/listaPortaria&acao=A&entidcampus=' + entid + '&ano=' + ano;
	}

/*
 *
 *
*/
	var params = '';
	function desabilitarConteudo( id ){
		var url = 'academico.php?modulo=principal/planodistribuicaocargos&acao=C&carga='+id;
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
	
	function abreportaria( prtid ){
		location.href= 'academico.php?modulo=principal/cadportaria&acao=C&evento=A&prtid=' + prtid;
		
	}

</script>