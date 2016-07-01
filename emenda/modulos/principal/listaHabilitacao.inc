<?php

if( $_SESSION["emenda"]["ptrid"] ) {
	$sql = "SELECT
			  entid
			FROM 
			  emenda.planotrabalho
			WHERE
			  ptrid = ".$_SESSION["emenda"]["ptrid"];
	
	$entid = $db->pegaUm($sql);
	$_SESSION["emenda"]["entid"] = $entid;
} else{
	if($_SESSION["emenda"]["entid"] || $_REQUEST['entid']){
			$entid = ($_SESSION["emenda"]["entid"] ? $_SESSION["emenda"]["entid"] : $_REQUEST['entid']);
	}else{
		echo "<script>
				window.location.href = 'emenda.php?modulo=principal/listaInstituicoes&acao=A&retorno=listaHabilitacao';
			  </script>";
		exit();
	}
}

include  APPRAIZ."includes/cabecalho.inc";
require_once APPRAIZ . "includes/classes/AbaAjax.class.inc";
print "<br/>";
$db->cria_aba( $abacod_tela, $url, '' );

$sql = "SELECT 
		  count(hen.henid) as total
		FROM 
		  emenda.habilitaentidade hen
          inner join entidade.entidade ent
		  	on ent.entnumcpfcnpj = hen.hencnpj
        WHERE
        	ent.entid = $entid";
        	
if( $db->pegaUm($sql) == 0 ){
	print '<br/>';
	monta_titulo('','<b>Dados de Habilitação</b>');
	print '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem" >';
	print '<tr><td align="left" style="color:#cc0000;">Não há registro de habilitação para esta entidade.</td></tr>';
	print '</table>';
}else{

	echo cabecalhoHabilitacao($entid);
	print "<br/>";
	
	
	# DEFINE ABA PADRÃO
	/**
	 * CRIA ABA
	 */
	$obAbaAjax = new AbaAjax(null,true, true, true);
	$arAba = array(
				  0 => array("descricao" => "Documentos não apresentados", 
			   				 "padrao" => true,
				  			 "url" => 'ajax_habilita.php',
			   				 "parametro" => "habilida=docnaoApresentado&entid=".$entid),
				  1 => array("descricao" => "Documentos com erro", 
			   				 "url" => "ajax_habilita.php",
			   				 "parametro" => "habilida=doccomErro&entid=".$entid),
				  2 => array("descricao" => "Documentos sem erro", 
			   				 "url" => "ajax_habilita.php",
			   				 "parametro" => "habilida=docsemErro&entid=".$entid),
				  3 => array("descricao" => "Diligências", 
			   				 "url" => "ajax_habilita.php",
			   				 "parametro" => "habilida=diligencia&entid=".$entid)
		  	      );
	$obAbaAjax->criaAba($arAba, 'divAcompanhamento');
}
?>