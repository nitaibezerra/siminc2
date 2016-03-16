<?php
pegaSessoes();

academico_retira_autorizacao();

$orgid = $_SESSION["academico"]["orgid"];

// objeto da classe do módulo
$autoriazacaoconcursos = new autoriazacaoconcursos();

// Pega as unidades permitidas do usuário
$unidades_permitidas = academico_pegarUnidadesPermitidas();

$_SESSION["academico"]["carga"] =  $_REQUEST["carga"]?  $_REQUEST["carga"] : $_SESSION["academico"]["carga"];
$carga = $_SESSION["academico"]["carga"];

// Carrega os campus da undiade
if ( $_REQUEST["carga"] ){	
	$autoriazacaoconcursos->listacampusedital( $_REQUEST["carga"] );
	die;
}

require_once APPRAIZ . 'includes/cabecalho.inc';
echo '<br/>';

// Monta as abas e o título da tela
$menu = array(0 => array("id" => 1, "descricao" => "Página Inicial",    "link" => "/academico/academico.php?modulo=principal/listarAbas&acao=A"),
			  1 => array("id" => 2, "descricao" => "Lista de Unidades", "link" => "/academico/academico.php?modulo=principal/listareditais&acao=C")
			  );
			 			  
echo montarAbasArray($menu, "/academico/academico.php?modulo=principal/listareditais&acao=C");
monta_titulo( "Lista de Unidades", "");

// mensagem de bloqueio
academico_mensagem_bloqueio( $orgid );

// Lista as unidades
$autoriazacaoconcursos->listaunidadesedital($orgid, $unidades_permitidas);

?>
<!-- biblioteca javascript local -->
<link rel="stylesheet" type="text/css" href="../includes/superTitle.css"/>
<script type="text/javascript" src="../includes/remedial.js"></script>
<script type="text/javascript" src="../includes/superTitle.js"></script>
<script language="javascript" type="text/javascript" src="../includes/calendario.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/listagem2.css">
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript">
	var params;
	
	//mantendo a arvore expandida
	<?
	if(isset($carga) && ($carga != '') && ($_REQUEST['evento'] == '') ){
	?>
		abreconteudo('academico.php?modulo=principal/listaUnidade&acao=A&subAcao=gravarCarga&carga=<?=$carga?>&params=' + params, <?=$carga?>);
	<?
	}
	?>	
	
	function formatarParametros(){
	    params = Form.serialize($('pesquisar'));
	    
	}
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
		window.open('academico.php?modulo=principal/cadportaria&acao=C&evento=A&prtid=' + prtid, '_TOP');
		
	}

	function apagar_edital(edpid){
		if ( confirm( "Deseja apagar o Edital? (Controle "+edpid+")"))
			location.href='academico.php?modulo=principal/listareditais&acao=C&evento=E&edpid='+edpid;
	}

	function usaPortaria(prtid, entid, prgid){
		location.href='academico.php?modulo=principal/listareditais&acao=C&evento=A&entidcampus='+entid+'&prtid='+ prtid+'&prgid='+prgid;
	}

	function usaPortariaProvimentos(prtid, entid, prgid){
		location.href='academico.php?modulo=principal/listareditais&acao=C&evento=A&entidcampus='+entid+'&prtid='+ prtid+'&prgid='+prgid+'&efetivacao=true';
	}

	function submeter(evento, indid){		
			document.getElementById('evento').value = evento;		
			document.formulario.submit();
	}
	
	function alterar(evento, edpid){		
		document.getElementById('evento').value = evento;
		document.getElementById('edpid').value = edpid;
		document.formulario.submit();
	}
	
	function atualizar(campo, totais_tipoEP){	
		alert(campo);
		alert(totais_tipoEP);return;
		window.opener.document.getElementsByName(campo)[0].value = Number(window.opener.document.getElementsByName(campo)[0].value) - 1;
		window.opener.document.getElementById(totais_tipoEP).innerHTML = Number(window.opener.document.getElementById(totais_tipoEP).innerHTML) -1;		
	}
	
	function limpar(){	
		document.getElementById('evento').value = '';	
		document.formulario.submit();		
	}
	
	function graficohistorica(indid)
	{
		var janela = window.open( 'http://<?php echo $_SERVER['SERVER_NAME'] ?>/academico/academico.php?modulo=principal/graficohistorica&acao=A&indid='+indid,'blank','height=800,width=800,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no');
		janela.focus();
	}

//	function abredadoscampus( entid ){
//		return window.location = '?modulo=principal/listareditais&acao=C&evento=A&entidcampus=' + entid;
//	}

	function abredadoscampus( entid ){
		return window.location = '?modulo=principal/dadoscampus&acao=A&entid=' + entid;
	}

	function alteraexercicio( entid, ano ){
		return window.location = '?modulo=principal/listareditais&acao=C&evento=A&entidcampus=' + entid + '&ano=' + ano;
	}

</script>