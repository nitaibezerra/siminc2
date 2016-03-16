<?php
$_SESSION["emenda"]["ptridAnalise"] = !empty( $_REQUEST["ptridAnalise"] ) ? $_REQUEST["ptridAnalise"] : $_SESSION["emenda"]["ptridAnalise"];
if( empty($_SESSION["emenda"]["ptridAnalise"]) ){
	$_SESSION["emenda"]["ptridAnalise"] = $_SESSION["emenda"]["ptrid"];
}
validaSessionPTA($_SESSION["emenda"]["ptridAnalise"]);

include_once APPRAIZ . 'includes/workflow.php';

if($_REQUEST['pegaDataFinal']){
	header('content-type: text/html; charset=ISO-8859-1');
	getDataFinalConvenio($_POST['vigdias'], $_POST['vigdatainicio']);
	exit;
}

function getDataFinalConvenio($dias, $data){
	$arData = explode('/', $data);

	$dia = $arData[0] - 1;
	$mes = $arData[1];
	$ano = $arData[2];
	$dataFinal = mktime(24*$dias, 0, 0, $mes, $dia, $ano);
	$dataFormatada = date('d/m/Y',$dataFinal);
	echo $dataFormatada;
}

$boReformulacao = pegaPaiPTA( $_SESSION["emenda"]["ptridAnalise"] );
$boReformulacao = ( !empty($boReformulacao) ? true : false );

if($_POST['action'] == 'salvar'){
	
	$ptrid = $_SESSION["emenda"]["ptridAnalise"];
	
	$refid = ( !empty($_POST['refid']) ? $_POST['refid'] : 'null' );
	$vigtipo = ( !empty($_POST['vigtipo']) ? $_POST['vigtipo'] : 'P' );
	
	if( !empty($_POST['vigid']) ){
		if( $_POST['vigdatainicio'] == "" || $_POST['vigdatafim'] == "" || $_POST['vigdias'] == "" ){
			echo "<script>alert('Faltam dados!'); history.back(-1);</script>";
			die;
		}
		$sql = "UPDATE 
				  	emenda.ptvigencia
				SET 
				  	vigdatainicio = '".formata_data_sql(trim($_POST['vigdatainicio']))."',
				  	vigdatafim = '".formata_data_sql(trim($_POST['vigdatafim']))."',
				 	vigdias = {$_POST['vigdias']},
				 	vigtipo = '{$vigtipo}',
					refid = {$refid}
				WHERE 
				  	vigid = ".$_POST['vigid'];
		
		$db->executar($sql);
	} else {
		if( $_POST['vigdatainicio'] == "" || $_POST['vigdatafim'] == "" || $_POST['vigdias'] == "" ){
			echo "<script>alert('Faltam dados!'); history.back(-1);</script>";
			die;
		}
		$pmcid = $db->pegaUm("select pmcid from emenda.ptminutaconvenio where ptrid =".$ptrid." and pmcstatus = 'A'");
		if( empty( $pmcid ) ){
			echo "<script>
					alert('Não foi encontrado nenhuma minuta convenio cadastrada!');
					window.location.href = 'emenda.php?modulo=principal/assinaturasPTA&acao=A';
				 </script>";
			exit();
		} else {
			$sql = "INSERT INTO emenda.ptvigencia( ptrid, pmcid, vigdatainicio, vigdias, vigdatafim,
							  vigstatus, vigdatainclusao, usucpfinclusao, refid, vigtipo) 
					VALUES ($ptrid,
						  $pmcid,
						  '".formata_data_sql(trim($_POST['vigdatainicio']))."',
						  {$_POST['vigdias']},
						  '".formata_data_sql(trim($_POST['vigdatafim']))."',
						  'A',
						  now(),
						  '".$_SESSION['usucpf']."',
						  {$refid},
						  '{$vigtipo}')";
			$db->executar($sql);
		}
	}
	
	$sql = "UPDATE emenda.ptminutaconvenio SET pmcdataassinatura = 'now()', 
				pmcdataalteracao = 'now()', usucpfalteracao = '".$_SESSION['usucpf']."'
			WHERE ptrid = ".$ptrid." and pmcstatus = 'A'";
	
 	$db->executar($sql);
	
	$db->commit();
	$db->sucesso('principal/assinaturasPTA');
	die;
}

include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";

$arPerfil = pegaPerfilArray( $_SESSION['usucpf'] );

$estadoAtual = pegarEstadoAtual( $_SESSION["emenda"]["ptridAnalise"] );

montaAbasAnalise( $abacod_tela, $url, '' , $estadoAtual, $boReformulacao);
monta_titulo( 'Assinaturas', '');

echo cabecalhoPlanoTrabalho($_SESSION["emenda"]["ptridAnalise"], true);

montaVisualizarPTA( $_SESSION["emenda"]["ptridAnalise"], false, $boReformulacao );

$habilita = 'S';
if( $boReformulacao ){
	$habilita = 'N';
}
$arTipos = pegaTipoReformulacao( $_SESSION["emenda"]["ptridAnalise"] );

$arCodigo = array();
foreach ($arTipos as $v) {
	array_push( $arCodigo, $v['codigo'] );
}

if( $boReformulacao /*&& ( in_array(8,$arCodigo) || in_array(3,$arCodigo) || in_array(1,$arCodigo) )*/ ){
	$sql = "SELECT
			ptv.vigid,
			ptv.vigdatainicio,
			to_char(ptv.vigdatafim, 'DD/MM/YYYY') as vigdatafim,
			ptv.vigdias,
			ptr.ptrsituacao,
			ptv.refid,
			ptv.vigtipo
		FROM 
			emenda.ptvigencia ptv
			inner join emenda.planotrabalho ptr on ptr.ptrid = ptv.ptrid
			left join emenda.ptminreformulacao pm on pm.ptrid = ptr.ptrid and pm.refid = ptr.refid and pm.refsituacaoreformulacao <> 'E'
		WHERE 
			ptv.ptrid in (SELECT p.ptrid FROM emenda.planotrabalho p
                        	left join emenda.ptminreformulacao pm on pm.ptrid = p.ptrid and pm.refsituacaoreformulacao <> 'E'
                    		WHERE p.ptrcod = (SELECT ptrcod FROM emenda.planotrabalho WHERE ptrid = {$_SESSION["emenda"]["ptridAnalise"]})
                    		--and 'S' = (case when (select count(refid) from emenda.ptminreformulacao where ptrid = p.ptrid and refsituacaoreformulacao <> 'E') > 0 then 'S' else 'N' end)  
						)
			and ptv.vigstatus = 'A'
			--and ptr.ptrstatus = 'A'
			and vigtipo in ('C', 'V', 'O')
		order by 
			ptr.ptrid";
	
	//--and ptv.ptrid not in (SELECT ptrid FROM emenda.ptminreformulacao WHERE refsituacaoreformulacao = 'E' and ptrid in ( SELECT p.ptrid FROM emenda.planotrabalho p WHERE ptrcod = (SELECT ptrcod FROM emenda.planotrabalho WHERE ptrid = {$_SESSION["emenda"]["ptridAnalise"]})))
	$arPtminuta = $db->carregar( $sql );
	$arPtminuta = $arPtminuta ? $arPtminuta : array();
	$vigtipo = 'A';
	
	//ver($sql,d);
	#Select do PTA original
	$sql = "SELECT
				ptv.vigid, 
				to_char(ptv.vigdatainicio, 'DD/MM/YYYY') as vigdatainicio,
				to_char(ptv.vigdatafim, 'DD/MM/YYYY') as vigdatafim,
				ptv.vigdias, ptv.refid, ptv.vigtipo 
			FROM 
				emenda.ptvigencia ptv
			WHERE 
				ptv.ptrid in (SELECT ptrid FROM emenda.planotrabalho WHERE ptrcod = (SELECT ptrcod FROM emenda.planotrabalho WHERE ptrid = {$_SESSION["emenda"]["ptridAnalise"]}))
				and ptv.vigstatus = 'A' and ptv.vigtipo = 'P'";
	$ptminutaOrig = $db->pegaLinha($sql);
}else{
	$filtro = ( $boReformulacao ? " and ptv.vigtipo = 'A'" : " and ptv.vigtipo = 'P'" );
	
	$sql = "SELECT
			ptv.vigid, 
			ptv.vigdatainicio,
			to_char(ptv.vigdatafim, 'DD/MM/YYYY') as vigdatafim,
			ptv.vigdias, ptv.refid, ptv.vigtipo 
		FROM 
			emenda.ptvigencia ptv
		WHERE 
			ptv.ptrid = ".$_SESSION["emenda"]["ptridAnalise"]."
			and ptv.vigstatus = 'A'
			$filtro";
	$ptminuta = $db->pegaLinha($sql);

	$vigdias = ( empty($ptminuta['vigdias']) ? 1 : $ptminuta['vigdias'] );
	$vigdatafim = $ptminuta['vigdatafim'];
	$vigdatainicio = $ptminuta['vigdatainicio'];
	$vigid = $ptminuta['vigid'];	
	$vigtipo = $ptminuta['vigtipo'];
}

?>
<html>
<head>
	<script type="text/javascript" src="/includes/prototype.js"></script>
	<script type="text/javascript" src="js/emenda.js"></script>
	<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
	<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
	<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
</head>
<body>
<form id="formAssinaturas" method="post" action="">
<input type="hidden" name="action" id="action" value="" />
<input type="hidden" name="vigid" id="vigid" value="<?=(!empty($vigid) ? $vigid : $ptminutaOrig['vigid']); ?>" />
<input type="hidden" name="vigtipo" id="vigtipo" value="<?=$vigtipo; ?>" />

<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4" style="border-bottom:none;">
	<tr>
		<td>
		<table class="tabela" align="left" width="100%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4" style="border-bottom:none;">
	<tr>
		<td colspan="3"><b>Assinaturas</b></td>		
	</tr>
	<?php if( $boReformulacao /*&& (in_array(3,$arCodigo) || in_array(8,$arCodigo) || in_array(1,$arCodigo) )*/ ){?>
		<input type="hidden" name="refid" id="refid" value="<?= $ptminutaOrig['refid']; ?>" />
		<tr>
			<td class="SubTituloCentro" colspan="2">Original</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="middle" style="width: 30%;"><b>Data Celebração:</b></td>
			<td >
			<?
				$vigdatainicio = $ptminutaOrig['vigdatainicio'];
				if( is_numeric( substr( $vigdatainicio, 0, 4 ) ) ) $vigdatainicio = formata_data( $vigdatainicio );
				echo campo_data2('vigdatainicio', 'N', $habilita,'Prazo Inicio', '', '', '', $vigdatainicio, 'calculaDiasVigencia();');
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="middle"><b>Dias de Vigência Original:</b></td>
			<td>
			<?//prddiasvigencia	
				$vigdias = $ptminutaOrig['vigdias'];		
				echo campo_texto('vigdias', 'N', 'N', '', 11, 20, '[#]', '', '', '', 0, 'id="vigdias"','',$vigdias,'calculaDiasVigencia();' );
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="middle"><b>Data Final da Vigência:</b></td>
			<td>
			<?
				$vigdatafim = $ptminutaOrig['vigdatafim'];
				if( is_numeric( substr( $vigdatafim, 0, 4 ) ) ) $vigdatafim = formata_data( $vigdatafim );
				echo campo_texto( 'vigdatafim', 'N', 'N', '', 13, 20, '', '', '', '', 0, 'id="vigdatafim"','',$vigdatafim,'' );
			?>
			</td>
		</tr>
		<?
		$vigdiasTotal = 0;
		
		foreach ($arPtminuta as $key => $v) {
			$vigdiasTotal += (int) $v['vigdias'];
			//if( $key > 0 ){
				$vigdias = $v['vigdias'];
				$vigdatafim = $v['vigdatafim'];
				$vigdatainicio = $v['vigdatainicio'];
				$tipo = ($v['vigtipo'] == 'V' ? ' Prorrogação da vigência' :  ($v['vigtipo'] == 'O' ? ' Prorrogração de Ofício' : '') );
				
				if( is_numeric( substr( $vigdatainicio, 0, 4 ) ) ) $vigdatainicio = formata_data( $vigdatainicio );
				if( is_numeric( substr( $vigdatafim, 0, 4 ) ) ) $vigdatafim = formata_data( $vigdatafim );
		 ?>
		<tr>
			<!--  <td class="SubTituloCentro" colspan="2">Prorrogação <? echo $key+1 .$tipo ?></td> -->
			<td class="SubTituloCentro" colspan="2"><? echo $tipo ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="middle"><b>Data Início de Vigência da Prorrogação de <?php echo ( in_array(8,$arCodigo) ? "Ofício" : "Data")?> <?=( sizeof($arPtminuta) > 2 ? $key : ''); ?>:</b></td>
			<td>
			<?
				echo campo_data2('vigdatainicio', 'N', 'N','Prazo Inicio', '', '', '', $vigdatainicio, '');
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="middle"><b>Dias de Vigência da Prorrogação de <?php echo ( in_array(8,$arCodigo) ? "Ofício" : "Data")?> <?=( sizeof($arPtminuta) > 2 ? $key : ''); ?>:</b></td>
			<td>
			<?//prddiasvigencia				
				echo campo_texto('vigdias_'.$key, 'N', 'N', '', 11, 20, '[#]', '', '', '', 0, 'id="vigdias"','',$vigdias,'calculaDiasVigencia();' );
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="middle"><b>Data Final de Vigência da Prorrogação de <?php echo ( in_array(8,$arCodigo) ? "Ofício" : "Data")?> <?=( sizeof($arPtminuta) > 2 ? $key : ''); ?>:</b></td>
			<td>
			<?
				echo campo_texto( 'vigdatafim', 'N', 'N', '', 13, 20, '', '', '', '', 0, 'id="vigdatafim"','','','' );
			?>
			</td>
		</tr>
		<input type="hidden" name="vigdatafim" id="vigdatafim" value="<?=$vigdatafim; ?>" />
	<?
			//}
		}
		?>
		<tr>
			<td colspan="2" ></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="middle"><b>Total de Dias de Vigência:</b></td>
			<td><?=$vigdiasTotal?></td>
		</tr>
		<?
	} else {?>
	<input type="hidden" name="refid" id="refid" value="<?=$refid; ?>" />
	<tr>
		<td class="SubTituloDireita" valign="middle" style="width: 30%;"><b>Data Celebração:</b></td>
		<td >
		<?
			$vigdatainicio = (!empty($vigdatainicio) ? formata_data( $vigdatainicio ): $vigdatainicio);			
			echo campo_data2('vigdatainicio', 'N', $habilita,'Prazo Inicio', '', '', '', $vigdatainicio, 'calculaDiasVigencia();');
		?>
		</td>
	</tr>	
	<tr>
		<td class="SubTituloDireita" valign="middle"><b>Dias de Vigência:</b></td>
		<td>
		<?//prddiasvigencia
			$habilitaDias = 'N';
			if( !in_array( 3, $arCodigo ) && $boReformulacao ) $habilitaDias = 'S';
			
			echo campo_texto( 'vigdias', 'N', $habilitaDias, '', 11, 20, '[#]', '', '', '', 0, 'id="vigdias"','','','calculaDiasVigencia();' );
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="middle"><b>Data Final da Vigência:</b></td>
		<td>
		<?
			echo campo_texto( 'vigdatafim', 'N', 'N', '', 13, 20, '', '', '', '', 0, 'id="vigdatafim"','','','' );
		?>
		</td>
	</tr>
	<?php
	}
	/*if( !in_array( TEMPORARIO_ASSINATURA, $arPerfil ) ){
		if($estadoAtual != EM_ASSINATURA_CONCEDENTE && $estadoAtual != EM_ASSINATURA_CONVENENTE ){
			$disabled = 'disabled="disabled"'; 
		} else{
			if(!possuiPermissao()) {
				$disabled = 'disabled="disabled"';
			} else {
				$disabled = '';
			}
		}
	} else {*/
	if( in_array( ADMINISTRADOR_INST, $arPerfil ) ) $estadoAtual = 'geral';
	if( $boReformulacao &&  (in_array( 3, $arCodigo ) || in_array( 8, $arCodigo ) ) ) $disabled = 'disabled="disabled"';
	if( $estadoAtual == EM_REFORMULACAO_PROCESSO ) $disabled = 'disabled="disabled"';
	if( in_array( CONSULTA_GERAL, $arPerfil ) ) $disabled = 'disabled="disabled"';
	//}
	?>
	<tr>
		<td align="center" bgcolor="#c0c0c0" colspan="4">
			<input type="button" id="bt_salvar" value="Salvar" onclick="salvarAssinatura();" <?=(!empty($disabled) ? $disabled : verificaPermissaoPerfil('analise', '', $estadoAtual, true));?> />
			&nbsp;
			<input type="button" id="bt_cancelar" value="Cancelar" onclick="history.back(-1);" />
		</td>
	</tr>
	</table>
	</td>
		<td valign="top" style="text-align: center; width: 20px;">
			<?php
				$docid = criarDocumento( $_SESSION["emenda"]["ptridAnalise"] );
				wf_desenhaBarraNavegacao( $docid , array( 'url' => $_SESSION['favurl'], 'ptrid' => $_SESSION["emenda"]["ptridAnalise"] , 'tipo' => "" ) );	
			?>
		</td>
	</tr>
	
</table>
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
		//$('#td_acao_445').css('display', 'none');
	});
});

var btSalvar	= document.getElementById("bt_salvar");
var btCancelar	= document.getElementById("bt_cancelar");
var form		= document.getElementById("formAssinaturas");

if($('vigdatainicio').value && $('vigdias').value && !$('vigdatafim').value){
	calculaDiasVigencia();
}

function calculaDiasVigencia(){
	var datainicio = $('vigdatainicio').value;
	var diavigencia = $('vigdias').value;	 
	
	if( diavigencia && datainicio){
		var myajax = new Ajax.Request('emenda.php?modulo=principal/assinaturasPTA&acao=A', {
		        method:     'post',
		        parameters: '&pegaDataFinal=true&vigdias='+diavigencia+'&vigdatainicio='+datainicio,
		        asynchronous: false,
		        onComplete: function (res){
					$('vigdatafim').value = res.responseText;
					
		        }
		  });
	}	
}

function salvarAssinatura(){
	if( $('vigdatainicio').value == "" ){
		alert( 'O campo Data Celebração é de preenchimento obrigatório!' );
		$('vigdatainicio').focus();
		return false;
	}
	if(!validaData(document.getElementById('vigdatainicio'))) {
		alert('Data de inicio incorreta');
		return false;
	}
	document.getElementById("action").value = "salvar";
	form.submit();
}

</script>
</html>