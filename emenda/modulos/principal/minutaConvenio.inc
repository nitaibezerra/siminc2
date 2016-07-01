<?php

if($_REQUEST['alterarminuta']){
	header('content-type: text/html; charset=ISO-8859-1');
	alterarDadosMinuta($_POST);
	exit;
}

header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

/*include  APPRAIZ."emenda/classes/Habilita.class.inc";
include  APPRAIZ."emenda/classes/LogErroWS.class.inc";
require_once APPRAIZ . "emenda/classes/Fnde_Webservice_Client.class.inc";
include_once APPRAIZ . 'emenda/classes/MinutaConvenio.class.inc';*/
include_once APPRAIZ . 'includes/workflow.php';

$_SESSION["emenda"]["ptridAnalise"] = !empty( $_REQUEST["ptridAnalise"] ) ? $_REQUEST["ptridAnalise"] : $_SESSION["emenda"]["ptridAnalise"];

/*if($_REQUEST['pegaDataFinal']){
	header('content-type: text/html; charset=ISO-8859-1');
	getDataFinalConvenio($_POST['pmcdiasvigencia'], $_POST['pmcdatainicio']);
	exit;
}*/

if( empty($_SESSION["emenda"]["ptridAnalise"]) ){
	echo "<script>
			alert('Faltam dados na sessão sobre este PTA.');
			window.location.href = 'emenda.php?modulo=principal/listaPtaAnalise&acao=A';
		  </script>";
	die;
} else {
	$ptridAnalise = $_SESSION["emenda"]["ptridAnalise"];
}

if($_REQUEST['excluir']){
	header('content-type: text/html; charset=ISO-8859-1');
	excluirIntervenienteConvenio($_POST['itcid']);
	exit;
}
if($_REQUEST['listainterveniente']){
	header('content-type: text/html; charset=ISO-8859-1');
	listaIntervenienteConvenio($_POST['pmcid']);
	exit;
}
$obConvenio = new MinutaConvenio($ptridAnalise);

function excluirIntervenienteConvenio($itcid){
	global $db;
	
	$sql = "UPDATE 
			  emenda.intervenienteconvenio  
			SET 
			  itcstatus = 'I'
			 
			WHERE 
			  itcid = ".$itcid;
		
	$db->executar($sql);
	$db->commit();
}

if( $_POST['salvar1'] == 'alterar' ){		
	$ptminutaconvenio = $obConvenio->carregaMinutaConvenio( '', $_POST['pmcid'] );
} else {
	$ptminutaconvenio = $obConvenio->carregaMinutaConvenio( $ptridAnalise );
}

$mdoid = ($_REQUEST['mdoid'] ? $_REQUEST['mdoid'] : $ptminutaconvenio['mdoid']);

if($_REQUEST["submetido"]) {

	if($_REQUEST['salvar1'] == "1") {
		
		$arMinuta = $ptminutaconvenio;
		$arMinuta = ($arMinuta ? $arMinuta : array());
		
		$arMinuta['vigdatainicio'] = ( $arMinuta['vigdatainicio'] ? "'".$arMinuta['vigdatainicio']."'" : 'null' );
		$arMinuta['vigdatafim'] = ( $arMinuta['vigdatafim'] ? "'".$arMinuta['vigdatafim']."'" : 'null' );
			
		$arMinuta['usucpfalteracao'] = ( $arMinuta['usucpfalteracao'] ? "'".$arMinuta['usucpfalteracao']."'" : 'null' );
		$arMinuta['pmcdataalteracao'] = ( $arMinuta['pmcdataalteracao'] ? "'".$arMinuta['pmcdataalteracao']."'" : 'null' );
		$arMinuta['pubdatapublicacao'] = ( $arMinuta['pubdatapublicacao'] ? "'".$arMinuta['pubdatapublicacao']."'" : 'null' );
		$arMinuta['pmcdtatualizaconveniosiafi'] = ( $arMinuta['pmcdtatualizaconveniosiafi'] ? "'".$arMinuta['pmcdtatualizaconveniosiafi']."'" : 'null' );
		$arMinuta['pmcdataassinatura'] = ( $arMinuta['pmcdataassinatura'] ? "'".$arMinuta['pmcdataassinatura']."'" : 'null' );
		$arMinuta['pmcdataconversaosiafi'] = ( $arMinuta['pmcdataconversaosiafi'] ? "'".$arMinuta['pmcdataconversaosiafi']."'" : 'null' );
		$arMinuta['pmcdatarap'] = ( $arMinuta['pmcdatarap'] ? "'".$arMinuta['pmcdatarap']."'" : 'null' );
		$arMinuta['mdoid'] = ( $_REQUEST['mdoid'] ? $_REQUEST['mdoid'] :  ($arMinuta['mdoid'] ? $arMinuta['mdoid'] : 'null') );		
		$arMinuta['moidpublicacao'] = ( $arMinuta['moidpublicacao'] ? $arMinuta['moidpublicacao'] : 'null' );
		
		if( strpos($_REQUEST["texto"], '<p style=\"page-break-before: always;\"><!-- pagebreak --></p>') ) {
			$_REQUEST["texto"] = str_replace('<p style=\"page-break-before: always;\"><!-- pagebreak --></p>', '<p style="page-break-before:always"><!-- pagebreak --></p>', $_REQUEST["texto"] );
		} else {	
			$_REQUEST["texto"] = str_replace("<!-- pagebreak -->", '<p style="page-break-before:always"><!-- pagebreak --></p>', $_REQUEST["texto"] );
		}
					
		$sql = "insert into emenda.ptminutaconvenio (ptrid, pmctexto, pmcstatus,
						pmcresolucao, usucpfinclusao, pmcdatainclusao,  
			            pmcdataconversaosiafi, pmcnumconveniosiafi, pmcdtatualizaconveniosiafi, 
			            pmcdataassinatura, pmcdatarap, mdoid, pmcdataalteracao, usucpfalteracao)
			    values(".$ptridAnalise.", '".simec_htmlspecialchars($_REQUEST["texto"], ENT_QUOTES)."', 'A',  
			    		'".$_REQUEST["pmcresolucao"]."', '".$_SESSION['usucpf']."', 'now()', 
			    		".$arMinuta['pmcdataconversaosiafi'].", '".$arMinuta['pmcnumconveniosiafi']."', ".$arMinuta['pmcdtatualizaconveniosiafi'].", 
			    		".$arMinuta['pmcdataassinatura'].", ".$arMinuta['pmcdatarap'].", 
			    		".$arMinuta['mdoid'].", ".$arMinuta['pmcdataalteracao'].", ".$arMinuta['usucpfalteracao']." ) RETURNING pmcid";

		$pcmid = $db->pegaUm($sql);

		if( !empty($_POST['obcid'][0]) ){
			$sql = "DELETE FROM emenda.objetominutaconvenio WHERE pmcid = ".$pcmid;
			$db->executar( $sql );
			foreach ($_POST['obcid'] as $obcid) {
				$sql = "INSERT INTO emenda.objetominutaconvenio( obcid, pmcid) 
						VALUES ( $obcid, $pcmid)";
				$db->executar( $sql );	
			}
		} else {
			$db->rollback();
			echo "<script>alert('O campo Objeto do convênio é obrigatório!'); history.back(-1);</script>";
			die;
		}
		
		if( empty($_REQUEST['pubid']) ){	
			$sql = "INSERT INTO emenda.ptpublicacao( pmcid, refid, pubtxtpublicacao, pubdatapublicacao, pubpagdou, pubdatainclusao, usucpfinclusao, mdoid, pubstatus) 
					VALUES (".$pcmid.", null, '".$arMinuta['pubtxtpublicacao']."',
						 	".$arMinuta['pubdatapublicacao'].", '".$arMinuta['pubpagdou']."', 'now()', 
						 	'".$_SESSION['usucpf']."', ".$arMinuta['moidpublicacao'].", 'A')";
		} else {
			$sql = "UPDATE emenda.ptpublicacao SET pmcid = ".$pcmid.", pubtxtpublicacao = '".$arMinuta['pubtxtpublicacao']."',
	  					pubdatapublicacao = ".$arMinuta['pubdatapublicacao'].", pubpagdou = '".$arMinuta['pubpagdou']."', mdoid = ".$arMinuta['moidpublicacao']." 
	  				WHERE
	  					pubid = ".$_REQUEST['pubid'];
		}
		
		$db->executar( $sql );

		if( $_REQUEST['vigdias'] == "" ){
			$db->rollback();
			echo "<script>alert('O campo Dias de Vigência é obrigatório!'); history.back(-1);</script>";
			die;
		}
		
		if( $_REQUEST['vigid'] ){
			$sql = "UPDATE emenda.ptvigencia SET pmcid = $pcmid, vigdatainicio = ".$arMinuta['vigdatainicio'].", 
						vigdias = '".$_REQUEST['vigdias']."', vigdatafim = ".$arMinuta['vigdatafim'].", vigstatus = 'A',
  						usucpfinclusao = '".$_SESSION['usucpf']."'
  					WHERE
  						vigid = ".$_REQUEST['vigid'];
		} else {
			$sql = "INSERT INTO emenda.ptvigencia( ptrid, pmcid, vigdatainicio, vigdias, vigdatafim, vigstatus, vigdatainclusao, usucpfinclusao, vigtipo) 
					VALUES ( ".$ptridAnalise.", $pcmid, ".$arMinuta['vigdatainicio'].", '".$_REQUEST['vigdias']."', ".$arMinuta['vigdatafim'].",
							 'A', 'now()', '".$_SESSION['usucpf']."', 'P')";	
		}
		$db->executar( $sql );
		
		if( $_REQUEST['pmcid'] ){
			$sql = "UPDATE emenda.intervenienteconvenio SET pmcid = $pcmid WHERE pmcid = ".$_REQUEST['pmcid']." and itcstatus = 'A'";
			$db->executar($sql);
		}
		
		// inativa a inuta existente
		$sql = "UPDATE emenda.ptminutaconvenio SET pmcstatus = 'I' WHERE ptrid = ".$ptridAnalise." and pmcid <> $pcmid and pmcstatus = 'A'";			
		$db->executar($sql);
		
		$db->commit();
		$db->sucesso('principal/minuta');
	}
	if($_REQUEST['mdoid'] != "") {
		
		$sql = "SELECT mdoconteudo FROM emenda.modelosdocumentos WHERE mdostatus = 'A' AND mdoid = ".$_REQUEST['mdoid'];
		$imitexto = $db->pegaUm($sql);

		if($imitexto){
			$obConvenio->alteraMacrosConvenio($imitexto, $_REQUEST['pmcid']);
		}
		else{
			$imitexto = "Não existem informações sobre esta minuta.";
		}
	}
	if($_REQUEST['pmcid'] != "" && !$_REQUEST['mdoid']) {
		$imitexto = $ptminutaconvenio["pmctexto"];
	}
} else {
	if($ptminutaconvenio) {
		$imitexto = $ptminutaconvenio["pmctexto"];
	}
}

$enbid = $db->pegaUm("select enbid from emenda.planotrabalho where ptrid = $ptridAnalise");

$obEntidade = new entidadeFNDE();
$obEntidade->verificaEntidadeBaseFNDE( $enbid, $ptridAnalise );

if( !empty($ptminutaconvenio) ){
	$sql = "SELECT omcid FROM emenda.objetominutaconvenio a WHERE pmcid = ".$ptminutaconvenio['pmcid'];
	$arOmcid = $db->carregarColuna( $sql );
}
$arOmcid = ( $arOmcid ? $arOmcid : array() );


$obPTA = new PTA();
$federal = $obPTA->buscaEmendaDescentraliza( $ptridAnalise );

$imitexto = str_replace('"', "'", $imitexto);

if( !$_POST['entidinterveniente'] ){
	if( strpos( strtolower($imitexto), 'interveniente#') || $ptminutaconvenio['interveniente'] == 'true' ){
		$boInterveniente = true;
		$boMostraInterveniente = true;
		$salvar = "disabled=\"disabled\"";
	} else {
		$boInterveniente = false;
		$boMostraInterveniente = false;
		$salvar = "";
	}
} else {
	if( strpos( strtolower($imitexto), 'interveniente') ){
		//$boInterveniente = true;
		$boMostraInterveniente = true;
		$salvar = "disabled=\"disabled\"";
	} else {
		$boInterveniente = false;
		$boMostraInterveniente = false;
		$salvar = "";
	}
}

include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";

$boReformulacao = pegaPaiPTA( $_SESSION["emenda"]["ptridAnalise"] );
$boReformulacao = ( !empty($boReformulacao) ? true : false );

montaAbasAnalise( $abacod_tela, $url, '' , $estadoAtual, $boReformulacao);
monta_titulo( 'Criação da Minuta Convênio PTA', '<img src="../imagens/obrig.gif" border="0">&nbsp;Indica Campo Obrigatório');

echo cabecalhoPlanoTrabalho($ptridAnalise, true);

$vigdatafim = ($ptminutaconvenio["vigdatafim"] ? formata_data($ptminutaconvenio["vigdatafim"]) : "");

montaVisualizarPTA( $ptridAnalise, false, $boReformulacao );

$arPerfil = pegaPerfilArray( $_SESSION['usucpf'] );

$permissao = verificaPermissaoPerfil( 'analise', 'boolean', '', true );

/*
 <script type="text/javascript" src="js/emenda.js"></script>

<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
*/
?>	
<script language="javascript" type="text/javascript" src="../includes/tinymce/tiny_mce.js"></script>
<style>

#loader-container,
#LOADER-CONTAINER{
    background: transparent;
    position: absolute;
    width: 100%;
    text-align: center;
    z-index: 8000;
    height: 100%;
}


#loader {
    background-color: #fff;
    color: #000033;
    width: 300px;
    border: 2px solid #cccccc;
    font-size: 12px;
    padding: 25px;
    font-weight: bold;
    margin: 150px auto;
}
</style>

<div id="loader-container" style="display: none">
   	<div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>
</div>
<form id="formMinutaConvenio" method="post" action="">

<input type="hidden" name="vigdatafim" id="vigdatafim" value="<?=$vigdatafim; ?>" />
<input type="hidden" name="bointerveniente" id="bointerveniente" value="<?=$boInterveniente; ?>" />
<input type="hidden" name="submetido" id="submetido" value="1" />
<input type="hidden" name="mdoid" id="mdoid" value="<?=($_REQUEST['mdoid'] ? $_REQUEST['mdoid'] : $ptminutaconvenio['mdoid'])?>" />
<input type="hidden" name="pmcid" id="pmcid" value="<?=$ptminutaconvenio['pmcid'];?>" />
<input type="hidden" name="itcid" id="itcid" value="" />
<input type="hidden" name="vigid" id="vigid" value="<?=$ptminutaconvenio['vigid']; ?>" />
<input type="hidden" name="pubid" id="pubid" value="<?=$ptminutaconvenio['pubid']; ?>" />
<input type="hidden" name="salvar1" id="salvar1" value="" />
<?php
if( empty( $arOmcid ) ) echo '<input type="hidden" name="omcid[]" id="omcid[]" value="'.$omcid.'" />';

foreach ($arOmcid as $omcid) {
	echo '<input type="hidden" name="omcid[]" id="omcid[]" value="'.$omcid.'" />';
} 


if($ptminutaconvenio){
	echo '<input type="hidden" id="imitexto" value="'. (($imitexto) ? $imitexto : 'Clique na Minuta desejada para recuperá-la.').'" />';
} else {
	echo '<input type="hidden" id="imitexto" value="" />';
}
?>


<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4" style="border-bottom:none;">
	<tr>
		<td>
		<table class="tabela" align="left" width="100%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4" style="border-bottom:none;">
	<tr>
		<td colspan="3"><b>Minuta de Convênio</b></td>
	</tr>
	 <tr>
		<td class="SubTituloDireita" valign="middle"><b>Tipo de Minuta:</b></td>
		<td>
			<label for="tipRadioC"><input type="radio" name="tipRadio[]" id="tipRadioC" checked="checked" onclick="carregaTipoMinuta(this.value);" value="C">Convênio</label>
			<label for="tipRadioT"><input type="radio" name="tipRadio[]" id="tipRadioT" onclick="carregaTipoMinuta(this.value);" value="T">Termo Aditivo</label>
		</td>
	</tr>
	<?if( $federal == 'N' ){?>
	<!--<tr>
		<td class="SubTituloDireita" valign="middle"><b>Situação Habilitação</b></td>
		<td>
		<?php
			$obHabilita = new Habilita();
			$cnpj = $obHabilita->pegaCnpj($ptridAnalise);
			echo $obHabilita->consultaHabilita($cnpj,true);
		?>
		</td>
	</tr>
	--><?} ?>
	<!-- <tr>
		<td class="SubTituloDireita" valign="middle"><b>Data Celebração:</b></td>
		<td >
		<?
			/*$pmcdatainicio = ($ptminutaconvenio["pmcdatainicio"] ? formata_data($ptminutaconvenio["pmcdatainicio"]) : "");
			echo campo_data2('pmcdatainicio', 'N', 'S','Prazo Inicio', '', '', 'calculaDiasVigencia();');*/
		?>
		</td>
	</tr>-->
	<tr>
		<td class="SubTituloDireita" valign="middle"><b>Dias de Vigência:</b></td>
		<td>
		<?//prddiasvigencia
			$vigdias = $ptminutaconvenio["vigdias"];
			echo campo_texto( 'vigdias', 'N', 'S', '', 11, 20, '[#]', '', '', '', 0, 'id="vigdias"','','','' );
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="middle"><b>Cronograma Desembolso:</b></td>
		<td >
			<a href="#" onclick="popUpIncluirCronogramaDesenbolso();" style="cursor:pointer">
				<label id="labelCronograma">Clique aqui para preencher o Cronograma Desembolso do Convenio</label>
			</a>
			<input type="hidden" name="boCronograma" id="boCronograma" value="<?=($prdid > 0 ? 'true' : 'false'); ?>" />
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="middle"><b>Objeto do convênio:</b></td>
		<td >
			<?php
			$sql = "SELECT oc.obcid as codigo, oc.obcdsc as descricao
					FROM 
						emenda.objetoconvenio oc
					    inner join emenda.objetominutaconvenio omc on omc.obcid = oc.obcid
					WHERE
						omc.pmcid = ". $ptminutaconvenio['pmcid']."
					    and oc.obcstatus = 'A'
					    and oc.obctpobj = 'O'";
			if(  !empty($ptminutaconvenio['pmcid']) ) $obcid = $db->carregar( $sql );
			
			$sql = "SELECT oc.obcid as codigo, oc.obcdsc as descricao FROM emenda.objetoconvenio oc
					WHERE  obcstatus = 'A'
					and oc.obctpobj = 'O'
					ORDER BY oc.obcdsc";
			
			$habilita = 'S';
			if( in_array( CONSULTA_GERAL, $arPerfil ) ) $habilita = 'N';
						
			combo_popup( 'obcid', $sql, '', '400x400', 0, array(), '', $habilita, false, false, 7, 400 );
			echo obrigatorio();
			/*$pmcobjeto = $ptminutaconvenio['pmcobjeto'];
			$valor = '';
			if( $ptridAnalise ){
				$dadosConvenio = $obPTA->carregaDadosIniciativaPTA( $ptridAnalise );
				foreach($dadosConvenio as $acao){
					$valor.= $acao['descricao']. ', ';
				}
				$tamanho = strlen($valor) - 2;
				$valor = substr( $valor, 0, $tamanho );
			}
			echo campo_textarea( 'pmcobjeto', 'N', 'N', '', '80', '6', '', '' , 0, '', '', '', $valor);*/ 
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="middle"><b>Resolução:</b></td>
		<td >
			<?php
			$pmcresolucao = $ptminutaconvenio['pmcresolucao'];
			
			if( !$pmcresolucao ){
				$sql = "SELECT
						    ini.iniresolucao
						FROM emenda.ptiniciativa pti
							inner join emenda.iniciativa ini 
						    	on ini.iniid = pti.iniid
						        and ini.inistatus = 'A'
						WHERE 
							pti.ptrid = ".$ptridAnalise."
							and ini.iniresolucao is not null";
				
				$arIniResolucao = $db->carregarColuna($sql);
				$pmcresolucao = implode(', ', $arIniResolucao);
			}
			echo campo_textarea( 'pmcresolucao', 'N', 'S', '', '80', '6', '500' , 'wrap="VIRTUAL"',0 ,''); 
			?>
		</td>
	</tr>
	<?
	if($ptminutaconvenio){
		if(!$boMostraInterveniente ){
			$enbcnpj = $_POST['enbcnpj'];
			$enbnome = $_POST['enbnome'];
			$entid_Interveniente = $_POST['entid_Interveniente'];			
			insereIntervenienteVinculadoMec( $ptridAnalise, $ptminutaconvenio['pmcid'] );
			
		?>
		<tr>
			<td class="SubTituloDireita" valign="middle"><b>Dados do Interveniente:</b></td>
			<td >
				<input type="hidden" name="bointercadastrado" id="bointercadastrado" value="<?php echo ($obConvenio->verificaExisteInterveniente($ptminutaconvenio['pmcid']) ? 'true' : 'false'); ?>">
				<?if(!in_array( CONSULTA_GERAL, $arPerfil ) && $permissao ){ ?>
					<a href="#" onclick="popUpManterInterveniente('', <?php echo $ptminutaconvenio['pmcid']; ?>);" style="cursor:pointer">
						<label id="labelCNPF"><img src="../imagens/gif_inclui.gif" border="0">Associar Interveniente</label></a>
				<?} else {?>
						<label id="labelCNPF"><img src="../imagens/gif_inclui_d.gif" border="0">Associar Interveniente</label>
				<?} ?>
				<br/>
				<br/>
				<div id="lista"><?php echo listaIntervenienteConvenio($ptminutaconvenio['pmcid'], $arPerfil); ?></div>
			</td>
		</tr>
		<?
			/*if( verificaExisteInterveniente( $ptminutaconvenio['pmcid'] ) ){
				echo "<script>document.getElementById('formMinutaConvenio').submit()</script>";
			}*/
		} 
		$sql = "SELECT 
				  count(prdid)
				FROM 
				  emenda.ptparceladesembolso
				WHERE
				  ptrid = ".$ptridAnalise."
				  and prdminuta = 'M'";
		$prdid = $db->pegaUm($sql);
		
		?>
		<tr>
			<td class="SubTituloDireita" valign="middle"><b>Modelos de Minutas Cadastradas:</b></td>
			<td >
			<?
			$modelo = (  $_REQUEST['mdoid'] ? $_REQUEST['mdoid'] : $ptminutaconvenio['mdoid'] );
			$sql = "SELECT
					mdo.mdoid as codigo,
					ini.ininome ||' - '|| mdo.mdoid ||' - '|| case when mdo.mdoid is null then 'Não existe documento vinculado' else mdo.mdonome end as descricao
					FROM emenda.ptiniciativa pti
					INNER JOIN emenda.iniciativa ini ON ini.iniid = pti.iniid
					LEFT JOIN emenda.iniciativadocumentos ido
					INNER JOIN emenda.modelosdocumentos mdo ON mdo.mdoid = ido.mdoid
					and mdo.tpdcod = 16
					and mdo.mdostatus = 'A'
					ON ini.iniid = ido.iniid and ido.idostatus = 'A'
					WHERE pti.ptrid = ".$ptridAnalise;
			
			$db->monta_combo("modelo", $sql, $habilita, 'Selecione...', 'carregaMinutaIniciativa', '', '', '', '', 'modelo', '', '', 'Lista de modelo(s) vinculado(s) a minuta convênio' );
			?>
			</td>
		</tr>
		<?php
		if($ptminutaconvenio["pmctexto"]) {
			$style = (!$mdoid) ? "style=\"color:#ff0000\"" : "";
			echo '<tr>
					<td class="SubTituloDireita" valign="middle"><b>Minuta Associada ao PTA:</b></td>
				    <td ><a href="#" '.$style.' onclick="carregaMinutaPTA('.$ptminutaconvenio["pmcid"].')">Minuta de Convênio do PTA</a>&nbsp;<input type="button" value="Imprimir" onclick="imprimirMinutaPTA('.$ptminutaconvenio["pmcid"].');" /></td>
				  </tr>';
		}
		?>
		<tr>
			<td colspan="3">
				<div>
					<textarea id="texto" name="texto" rows="30" cols="80" style="width:100%" class="emendatinymce"></textarea>
				</div>
			</td>
		</tr>
	<?php
	} else {
		echo '<input type="hidden" name="texto" id="texto" value="" />';
	}
	
	if( pegarEstadoAtual( $ptridAnalise ) != EM_GERACAO_DA_MINUTA_DE_CONVENIO )
	{
		if( in_array( ADMINISTRADOR_MINUTA, $arPerfil ) || in_array(SUPER_USUARIO, $arPerfil) || in_array(ADMINISTRADOR_REFORMULACAO, $arPerfil) || in_array(ADMINISTRADOR_INST, $arPerfil) )
		{
			$disabled = '';
		}
		else
		{
			$disabled = 'disabled="disabled"';
		}
	}
	elseif( in_array( CONSULTA_GERAL, $arPerfil ) )
	{
		$disabled = 'disabled="disabled"';
	}
	else
	{
		if( !possuiPermissao() )
		{
			$disabled = 'disabled="disabled"';
		}
		else
		{
			$disabled = '';
		}
	}
	
	?>
	<tr>
		<td align="center" bgcolor="#c0c0c0" colspan="3">
			<input type="button" id="bt_salvar" value="Salvar" onclick="salvarMinutaIniciativa();" <?=(!empty($disabled) ? $disabled : verificaPermissaoPerfil( 'analise', '', '', true ) ); ?> />
			&nbsp;
			<input type="button" id="bt_cancelar" value="Cancelar" onclick="history.back(-1);" />
		</td>
	</tr>
	</table></td>
	<td valign="top" style="text-align: center; width: 20px;">
			<?php
				$docid = criarDocumento( $ptridAnalise );
				$boAcaoSemCondicao = array('acaosemcondicao' => true);
				wf_desenhaBarraNavegacao( $docid , array( 'url' => $_SESSION['favurl'], 'ptrid' => $ptridAnalise, 'tipo' => "" ), $boAcaoSemCondicao );
			?>
		</td>
	</tr>
</table>
</form>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script language="javascript" type="text/javascript" src="js/minutaConvenio.js"></script>
<script language="javascript" type="text/javascript" src="js/emenda.js"></script>
<div id="erro"></div>
<?  
$_POST['texto'] = '';
echo $obConvenio->listaMinutaConvenioPTA( $ptridAnalise, $arPerfil );
?>
<?php
	$arFluxo = verificaFluxo( $_SESSION["emenda"]["ptridAnalise"] );
	
	if( $arFluxo ){
		$fluxo = implode(', ', $arFluxo);
	}
?>
</body>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">

jQuery.noConflict();
jQuery(document).ready(function(){
	jQuery.each([ <?=$fluxo ?> ], function( index, value ) {
		jQuery('#td_acao_'+value).css('display', 'none');
	});
});
</script>
</body>