<?php
validaSessionPTA($_SESSION["emenda"]["ptrid"]);

include_once APPRAIZ . 'includes/workflow.php';

if($_REQUEST['pegaDataFinal']){
	header('content-type: text/html; charset=ISO-8859-1');
	if( !empty($_POST['vigdias']) )
		getDataFinalConvenio($_POST['vigdias'], $_POST['vigdatainicio']);
	else{
		$vigdatainicio = formata_data_sql($_POST['vigdatainicio']);
		$vigdatafim = formata_data_sql($_POST['vigdatafim']);
		$dias = dateDiff1($vigdatafim, $vigdatainicio);
 		echo $dias;
	}		
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

function dateDiff1($from,$to){
	/*$diff = $to - $from;
	$info = array();
	if($diff>86400){
		$info['d'] = ($diff - ($diff%86400))/86400;
		$diff = $diff%86400;
	}
	$f = '';
	foreach($info as $k=>$v){
		if($v>0) $f .= "$v";
	}
	return ($f == 24) ? "1" : $f;*/
	
	$diferenca = strtotime($from) - strtotime($to);
	$dias = floor($diferenca / (60 * 60 * 24));
	$dias += ( ($d == 0) ? 1 : $d);
	
	return $dias;
}

$boReformulacao = pegaPaiPTA( $_SESSION["emenda"]["ptrid"] );
$boReformulacao = ( !empty($boReformulacao) ? true : false );

if($_POST['action'] == 'cancelar'){
	$ptrid = $_SESSION["emenda"]["ptrid"];
	$ptridpai = pegaPaiPTA( $ptrid );

	/*$sql = "UPDATE 
			  	emenda.ptvigencia
			SET 
			  	--vigdatainicio = (SELECT vigdatainicio FROM emenda.ptvigencia WHERE ptrid = ".$ptridpai." and vigstatus = 'A'),
			  	vigdatafim = (SELECT vigdatafim FROM emenda.ptvigencia WHERE ptrid = ".$ptridpai." and vigstatus = 'A'),
			  	vigdias = (SELECT vigdias FROM emenda.ptvigencia WHERE ptrid = ".$ptridpai." and vigstatus = 'A'),
			  	refid = (SELECT refid FROM emenda.ptvigencia WHERE ptrid = ".$ptridpai." and vigstatus = 'A')
			 
			WHERE 
			  	ptrid = ".$ptrid."
			  	and vigid = ".$_POST['vigidmax']."
			  	and vigstatus = 'A'";*/
	
	$sql = "UPDATE 
			  	emenda.ptvigencia
			SET 
			  	vigdatafim = null,
			  	vigdias = null
			 
			WHERE 
			  	ptrid = ".$ptrid."
			  	and vigid = ".$_POST['vigidmax']."
			  	and vigstatus = 'A'";
	
	$db->executar($sql);
	$db->commit();
	$db->sucesso('principal/reformulacaoVigenciaPTA');
	die;
}

$habilitaOrig = 'N';
if( $_SESSION['usucpf'] == '' || $_SESSION['usucpf'] == '' ) $habilitaOrig = 'S';

if($_POST['action'] == 'salvar'){
	
	$ptrid = $_SESSION["emenda"]["ptrid"];
	if( $habilitaOrig == 'S' && $_POST['vigdatainiciooriginal'] && $_POST['vigdatafimoriginal'] && $_POST['vigdiasoriginal'] ){
		$vigdatainiciooriginal = $_POST['vigdatainiciooriginal'] ? "'".formata_data_sql($_POST['vigdatainiciooriginal'])."'" : null; 
		$vigdiasoriginal = $_POST['vigdiasoriginal'] ? $_POST['vigdiasoriginal'] : null; 
		$vigdatafimoriginal = $_POST['vigdatafimoriginal'] ? "'".formata_data_sql($_POST['vigdatafimoriginal'])."'" : null; 
		$vigidoriginal = $_POST['vigidoriginal']; 
		$vigtipooriginal = $_POST['vigtipooriginal']; 
		
		$sql = "UPDATE emenda.ptvigencia SET
				  vigdatainicio = $vigdatainiciooriginal,
				  vigdias = $vigdiasoriginal,
				  vigdatafim = $vigdatafimoriginal,
				  refid = null,
				  vigtipo = '$vigtipooriginal'			 
				WHERE 
				  vigid = $vigidoriginal";
		$db->executar($sql);
	}
	
	foreach ($_POST['vigid'] as $vigid) {
		$vigdatafim = formata_data_sql( $_POST['vigdatafim'][$vigid] );
		$vigdatainicio = formata_data_sql( $_POST['vigdatainicio'][$vigid] );
		$vigdias = $_POST['vigdias'][$vigid];
		$ptridRefor = $_POST['ptrid'][$vigid];
		
		if( $habilitaOrig == 'S' ){
			$vigtipo = $_POST['tipo'][$vigid];
		} else {
			$vigtipo = $_POST['vigtipo'][$vigid];			
		}		
		//$vigtipoAtual = $_POST['vigtipo'][$vigid];
		
		/*if(!$ptridRefor || strlen($vigdatafim)!= 10 || !$vigdias){
			$db->insucesso( 'Falha ao cadastrar a vigencia da reformulacao!', '', 'principal/reformulacaoVigenciaPTA' );
			die;
		}*/
		
		if( $vigdatainicio || $vigdatafim || $vigdias ){
		
			if( $vigtipo != 'V' && $vigtipo != 'P' ){
				$sql = "select refprorrogacaooficio from emenda.ptminreformulacao  where ptrid = $ptridRefor and refstatus = 'A' and refsituacaoreformulacao = 'C'";
				$refprorrogacaooficio = $db->pegaUm($sql);
				$vigtipo = $refprorrogacaooficio == "N" ? "V" : "O";
			} else {
				$vigtipo = $vigtipo;			
			}
						
			if( $db->testa_superuser() ){
				$vigdatainicio = formata_string_sql($vigdatainicio);
				$dataini = "vigdatainicio = $vigdatainicio,";
			}
			$vigdatafim = formata_string_sql($vigdatafim);
			$vigdias = formata_string_sql($vigdias);
			
			$sql = "UPDATE 
				  	emenda.ptvigencia
				SET 
					$dataini
				  	vigdatafim = $vigdatafim,
				  	vigdias = $vigdias,
				  	vigtipo = '$vigtipo',
				  	refid = (select refid from emenda.ptminreformulacao where refstatus = 'A' and ptrid = ".$ptridRefor." order by refid desc limit 1)
				 	
				WHERE 
				  	vigid = ".$vigid."
				  	and vigstatus = 'A'";
			
			/*
			 * ptrid = ".$ptrid." and 
			*/
			$db->executar($sql);
					
			$sql = "UPDATE emenda.ptminutaconvenio SET pmcdataassinatura = 'now()', 
						pmcdataalteracao = 'now()', usucpfalteracao = '".$_SESSION['usucpf']."'
					WHERE ptrid = ".$ptridRefor." and pmcstatus = 'A'";
			
		 	$db->executar($sql);
		}
	}
	if($db->commit())
		$db->sucesso('principal/reformulacaoVigenciaPTA');
	else
		$db->insucesso( 'Falha ao cadastrar a vigencia da reformulacao', '', 'principal/reformulacaoVigenciaPTA' );
	die;
}

$sql = "SELECT
			ptv.vigid,
			to_char(ptv.vigdatainicio, 'DD/MM/YYYY') as vigdatainicio,
			ptv.vigdatafim,
			ptv.vigdias,
			ptr.ptrsituacao,
			ptv.refid,
			ptv.vigtipo,
			ptv.ptrid,
			(select max(vigid) from emenda.ptvigencia where ptrid in (SELECT p.ptrid FROM emenda.planotrabalho p
																			left join emenda.ptminreformulacao pm on pm.ptrid = p.ptrid and pm.refsituacaoreformulacao <> 'E'
																		WHERE ptrcod = (SELECT ptrcod FROM emenda.planotrabalho WHERE ptrid = {$_SESSION["emenda"]["ptrid"]}) 
																			and ptrstatus = 'A' ) and vigtipo in ( 'P', 'V', 'O', 'C') and vigstatus = 'A' ) as vigidmax 
		FROM 
			emenda.ptvigencia ptv
			inner join emenda.planotrabalho ptr on ptr.ptrid = ptv.ptrid
			left join emenda.ptminreformulacao pm on pm.ptrid = ptr.ptrid and pm.refid = ptr.refid and pm.refsituacaoreformulacao <> 'E'
		WHERE 
			ptv.ptrid in (SELECT p.ptrid FROM emenda.planotrabalho p
	                        left join emenda.ptminreformulacao pm on pm.ptrid = p.ptrid and pm.refsituacaoreformulacao <> 'E'
	                    WHERE p.ptrcod = (SELECT ptrcod FROM emenda.planotrabalho WHERE ptrid = {$_SESSION["emenda"]["ptrid"]}) 
							--and 'S' = (case when (select count(refid) from emenda.ptminreformulacao where ptrid = p.ptrid and refsituacaoreformulacao <> 'E') > 0 then 'S' else 'N' end)
						)
			and ptv.vigstatus = 'A'
			--and ptr.ptrstatus = 'A'
			and vigtipo in ( 'V', 'O', 'C')
		order by 
			ptr.ptrid";

//--and ptv.ptrid not in (SELECT ptrid FROM emenda.ptminreformulacao WHERE refsituacaoreformulacao = 'E' and ptrid in ( SELECT p.ptrid FROM emenda.planotrabalho p WHERE ptrcod = (SELECT ptrcod FROM emenda.planotrabalho WHERE ptrid = {$_SESSION["emenda"]["ptrid"]})))

$arPtminuta = $db->carregar($sql);
$arPtminuta = ( $arPtminuta ? $arPtminuta : array() );

if( $_SESSION['emenda']['tpopup'] != 'popup' ){
	include  APPRAIZ."includes/cabecalho.inc";
} else {
	echo '<script type="text/javascript" src="/includes/funcoes.js"></script>
		  <script type="text/javascript" src="/includes/prototype.js"></script>
		  <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		  <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>';
}
print "<br/>";

$arMnuid = array();
$estadoAtual = pegarEstadoAtual( $_SESSION["emenda"]["ptrid"] );

montaAbasPTA( $abacod_tela, $url, '', $estadoAtual );
monta_titulo( 'Assinaturas', '');

echo cabecalhoPlanoTrabalho($_SESSION["emenda"]["ptrid"]);

#Select do PTA original
$sql = "SELECT
			ptv.vigid, 
			to_char(ptv.vigdatainicio, 'DD/MM/YYYY') as vigdatainicio,
			to_char(ptv.vigdatafim, 'DD/MM/YYYY') as vigdatafim,
			ptv.vigdias, ptv.refid, ptv.vigtipo 
		FROM 
			emenda.ptvigencia ptv
		WHERE 
			ptv.ptrid in (SELECT ptrid FROM emenda.planotrabalho WHERE ptrcod = (SELECT ptrcod FROM emenda.planotrabalho WHERE ptrid = {$_SESSION["emenda"]["ptrid"]}))
			and ptv.vigstatus = 'A' and ptv.vigtipo = 'P'";
$ptminutaOrig = $db->pegaLinha($sql);
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
<input type="hidden" name="bohabilita" id="bohabilita" value="<?=$habilitaOrig; ?>" />
<input type="hidden" name="vigidmax" id="vigidmax" value="<?=$arPtminuta[0]['vigidmax']; ?>" />

<table class="tabela" align="center" bgcolor="#f5f5f5" width="95%" cellspacing="1" cellpadding="4" style="border-bottom:none;">
	<tr>
		<td style="width: 100%">
		<table class="tabela" align="center" width="100%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4" style="border-bottom:none;">
		<tr bgcolor="#C0C0C0">
			<td colspan="3"><b>Vig�ncia conv�nio original</b></td>		
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="middle" style="width: 30%;"><b>Data Celebra��o:</b></td>
			<td >
			<!--  <input type="hidden" name="vigtipooriginal" id="vigtipooriginal" value="<?=$ptminutaOrig['vigtipo']; ?>" /> -->
			<input type="hidden" name="vigidoriginal" id="vigidoriginal" value="<?=$ptminutaOrig['vigid']; ?>" />
			<?
				$vigdatainiciooriginal = ($ptminutaOrig['vigdatainicio'] ? $ptminutaOrig['vigdatainicio'] : "");
				echo campo_texto( 'vigdatainiciooriginal', 'N', $habilitaOrig, '', 11, 20, '[#]', '', '', '', 0, 'id="vigdatainiciooriginal"' );
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="middle"><b>Dias de Vig�ncia:</b></td>
			<td>
			<?
				$vigdiasoriginal = ( empty($ptminutaOrig['vigdias']) ? 1 : $ptminutaOrig['vigdias'] );
				echo campo_texto( 'vigdiasoriginal', 'N', $habilitaOrig, '', 11, 20, '[#]', '', '', '', 0, 'id="vigdiasoriginal"' );
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="middle"><b>Data Final da Vig�ncia:</b></td>
			<td>
			<?
				//$vigdatafimoriginal = ($ptminutaOrig['vigdatafim'] ? formata_data($ptminutaOrig['vigdatafim']) : "");
				$vigdatafimoriginal = $ptminutaOrig['vigdatafim'];
				echo campo_texto( 'vigdatafimoriginal', 'N', $habilitaOrig, '', 11, 20, '', '', '', '', 0, 'id="vigdatafimoriginal"');
			?>
			</td>
		</tr>
		<?if( $habilitaOrig == 'S' ){ ?>
		<tr>
			<td class="SubTituloDireita" valign="middle"><b>Tipo da Vig�ncia:</b></td>
			<td>
			<?
				$vigtipo = $ptminutaOrig['vigtipo'];
				$arrTipos = array(
						array('codigo' => 'P', 'descricao' => 'Vigencia do PTA original'),
						array('codigo' => 'A', 'descricao' => 'Vigencia de Assinatura'),
						array('codigo' => 'V', 'descricao' => 'Prorrogacao de Vig�ncia'),
						array('codigo' => 'O', 'descricao' => 'Prorroga��o de Of�cio')
					);
				echo $db->monta_combo('vigtipooriginal', $arrTipos, 'S','-- Selecione um tipo --','', '', '',250,'N', 'vigtipooriginal', '', $vigtipo, 'Tipos de Vig�ncia');
			?>
			</td>
		</tr>
		<?}
		
		foreach ($arPtminuta as $key => $v) {
			//if( $key > 0 ){
				$habilita = 'S';
				$disab = '';
				if( $v['ptrsituacao'] != 'C' && !$db->testa_superuser() ){
					$habilita = 'N';
					$disab = 'disabled="disabled"';
				} else {
					$idVigdatainicio = 'vigdatainicio_'.$v['vigid'];
					$idVigdias = 'vigdias_'.$v['vigid'];
					$idVigdatafim = 'vigdatafim_'.$v['vigid'];
					$idTipo = 'vigitpo_'.$v['vigid'];
					
					$nomeVigdatainicio = 'vigdatainicio['.$v['vigid'].']';
					$nomeVigdias = 'vigdias['.$v['vigid'].']';
					$nomeVigdatafim = 'vigdatafim['.$v['vigid'].']';
					$nomeVigid = 'vigid['.$v['vigid'].']';
					$nomePtrid = 'ptrid['.$v['vigid'].']';
					$nomeTipo = 'tipo['.$v['vigid'].']';
				}
				$tipo = ($v['vigtipo'] == 'V' ? ' - Prorroga��o da vig�ncia' :  ($v['vigtipo'] == 'O' ? ' - Prorroga��o de Of�cio' : '') );
				
				$labelHml = '';
				if( $_SESSION['usucpf'] == '' ) $labelHml = " - ptrid: ".$v['ptrid'];
				?>
				<tr bgcolor="#C0C0C0">
					<td colspan="3"><b>Vig�ncia conv�nio prorroga��o <?=( sizeof($arPtminuta) > 1 ? /*$key+1 .*/ $tipo : ''); echo $labelHml; ?></b></td>		
				</tr>
				<tr>
					<td class="SubTituloDireita" valign="middle"><b>Data Celebra��o:</b></td>
					<td >
					<input type="hidden" <?=$disab; ?> name="<?=$nomeVigid; ?>" id="<?=$nomeVigid; ?>" value="<?=$v['vigid']; ?>" />
					<input type="hidden" <?=$disab; ?> name="<?=$nomePtrid; ?>" id="<?=$nomePtrid; ?>" value="<?=$v['ptrid']; ?>" />
					<input type="hidden" name="vigtipo[<?=$v['vigid'];?>]" id="vigtipo_<?=$v['vigid']; ?>" value="<?=$v['vigtipo']; ?>" />
					<?	
						$vigdatainicio = ($v['vigdatainicio'] ? $v['vigdatainicio'] : "");
						if( is_numeric( substr( $vigdatainicio, 0, 4 ) ) ) $vigdatainicio = formata_data( $vigdatainicio );
						if( $db->testa_superuser() ){
							echo campo_data2($nomeVigdatainicio, 'N', $habilita,'Data Inicio da Vig�ncia', '', '', 'calculaDataInicioVigencia('.$v['vigid'].');', $vigdatainicio, '', '', $idVigdatainicio);
						} else {
							echo campo_texto($nomeVigdatainicio, 'N', 'N', '', 11, 20, '', '', '', '', 0, 'id="'.$idVigdatainicio.'"', '', $vigdatainicio);
						}
					?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita" valign="middle"><b>Dias de Vig�ncia:</b></td>
					<td>
					<?
						$vigdias = ( empty($v['vigdias']) ? '' : $v['vigdias'] );
						echo campo_texto($nomeVigdias, 'N', $habilita, '', 11, 20, '[#]', '', '', '', 0, 'id="'.$idVigdias.'"','', $vigdias, 'calculaDiasVigencia('.$v['vigid'].');' );
					?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita" valign="middle"><b>Data Final da Vig�ncia:</b></td>
					<td>
					<?
						$vigdatafim = ($v['vigdatafim'] ? formata_data($v['vigdatafim']) : "");
						if( is_numeric( substr( $vigdatafim, 0, 4 ) ) ) $vigdatafim = formata_data( $vigdatafim );
						//echo campo_data2($nomeVigdatafim, 'N', 'S','Data Final da Vig�ncia', '', '', 'calculaDataFimVigencia('.$v['vigid'].');', $vigdatafim, '', '', $idVigdatafim);
					?>
					<input title="Data Final da Vig�ncia" type="text" id="<?php echo $idVigdatafim;?>" name="<?php echo $nomeVigdatafim;?>" value="<?php echo $vigdatafim;?>" size="12" 
						style="text-align: right;" maxlength="10" class="normal " onkeyup="this.value=mascaraglobal('##/##/####',this.value);" onchange="" onmouseover="MouseOver(this);" 
						onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="validando_data(this);MouseBlur(this);this.value=mascaraglobal('##/##/####',this.value)">
					</td>
				</tr>
				<?if( $habilitaOrig == 'S' ){ ?>
				<tr>
					<td class="SubTituloDireita" valign="middle"><b>Tipo da Vig�ncia:</b></td>
					<td>
					<?
						$vigtipo = $v['vigtipo'];
						$arrTipos = array(
								array('codigo' => 'P', 'descricao' => 'Vigencia do PTA original'),
								array('codigo' => 'A', 'descricao' => 'Vigencia de Assinatura'),
								array('codigo' => 'V', 'descricao' => 'Prorrogacao de Vig�ncia'),
								array('codigo' => 'O', 'descricao' => 'Prorroga��o de Of�cio')
							);
						echo $db->monta_combo($nomeTipo, $arrTipos, $habilita,'-- Selecione um tipo --','', '', '',250,'N', $idTipo, '', $vigtipo, 'Tipos de Vig�ncia');
					?>
					</td>
				</tr>
		<?php }			
			//}
		}
		if($estadoAtual != EM_REFORMULACAO_PROCESSO && !$db->testa_superuser()){
			$disabled = 'disabled="disabled"'; 
		}
		?>
		<tr>
			<td align="center" bgcolor="#c0c0c0" colspan="4">
				<input type="button" id="btsalvar" value="Salvar" onclick="salvarAssinatura();" <?=$disabled; ?> />
				&nbsp;
				<input type="button" id="btcancelar" value="Cancelar" onclick="cancelarAssinatura();" />
			</td>
		</tr>
	</table></td>
		<td><?
			$boHistorico = null;
			if( in_array( INSTITUICAO_BENEFICIADA, pegaPerfilArray( $_SESSION['usucpf'] ) ) ){
				$boHistorico = array('historico' => true);
			} else {
				$boHistorico = array();
			}
			$docid = criarDocumento( $_SESSION["emenda"]["ptrid"] );
			wf_desenhaBarraNavegacao( $docid , array( 'ptrid' => $_SESSION["emenda"]["ptrid"], 'resid' => $arPTA['resid'] , 'url' => $_SESSION['favurl'], 'refid' => $arPTA['refid'] ), $boHistorico );
			 ?></td>
	</tr>
</table>
<div id="erro"></div>
</body>
<script type="text/javascript">
var btSalvar	= document.getElementById("btsalvar");
var btCancelar	= document.getElementById("btcancelar");
var form		= document.getElementById("formAssinaturas");

/*if($('vigdatainicio') && $('vigdias')){
	calculaDiasVigencia();
}*/

function calculaDiasVigencia( vigid ){
	var datainicio = $('vigdatainicio_'+vigid).value;
	var diavigencia = $('vigdias_'+vigid).value;	 
	
	if( diavigencia != '' && datainicio != '' ){
		var myajax = new Ajax.Request('emenda.php?modulo=principal/reformulacaoVigenciaPTA&acao=A', {
		        method:     'post',
		        parameters: '&pegaDataFinal=true&vigdias='+diavigencia+'&vigdatainicio='+datainicio,
		        asynchronous: false,
		        onComplete: function (res){
					$('vigdatafim_'+vigid).value = res.responseText;
		        }
		  });
	}	
}
function calculaDataFimVigencia( vigid ){
	var datainicio = $('vigdatainicio_'+vigid).value;
	var datafim = $('vigdatafim_'+vigid).value;	 
	
	if( datainicio != '' && datafim != '' ){
		var myajax = new Ajax.Request('emenda.php?modulo=principal/reformulacaoVigenciaPTA&acao=A', {
		        method:     'post',
		        parameters: '&pegaDataFinal=true&vigdatafim='+datafim+'&vigdatainicio='+datainicio,
		        asynchronous: false,
		        onComplete: function (res){
		        	$('vigdias_'+vigid).value = res.responseText;
					//$('erro').innerHTML = res.responseText;
		        }
		  });
	}	
}

function calculaDataInicioVigencia( vigid ){
	var datainicio = $('vigdatainicio_'+vigid).value;
	var datafim = $('vigdatafim_'+vigid).value;
	var diavigencia = $('vigdias_'+vigid).value;
		 
	if( datafim != '' ){
		var myajax = new Ajax.Request('emenda.php?modulo=principal/reformulacaoVigenciaPTA&acao=A', {
		        method:     'post',
		        parameters: '&pegaDataFinal=true&vigdatafim='+datafim+'&vigdatainicio='+datainicio,
		        asynchronous: false,
		        onComplete: function (res){
		        	//$('erro').innerHTML = res.responseText;
		        	$('vigdias_'+vigid).value = res.responseText;
		        }
		  });
	} else if( diavigencia != '' ){
		var myajax = new Ajax.Request('emenda.php?modulo=principal/reformulacaoVigenciaPTA&acao=A', {
		        method:     'post',
		        parameters: '&pegaDataFinal=true&vigdias='+diavigencia+'&vigdatainicio='+datainicio,
		        asynchronous: false,
		        onComplete: function (res){
		        	$('vigdatafim_'+vigid).value = res.responseText;
		        }
		  });
	}	
}
function salvarAssinatura(){
	var boVazio = false;
	var bohabilita = $('bohabilita').value;
	
	for(i=0; i<form.length; i++){
		if(form.elements[i].id.indexOf('_') != -1 ){
			if(form.elements[i].value == ''){
				boVazio = true;
			}
		}	
	}
	if( boVazio && bohabilita == 'N' ){
		alert('Campos de preenchimento obrigat�rio est�o em brancos!');
		return false;
	}
	/*if( $('vigdatainicio').value == "" ){
		alert( 'O campo Data Celebra��o � de preenchimento obrigat�rio!' );
		$('vigdatainicio').focus();
		return false;
	}
	if(!validaData(document.getElementById('vigdatainicio'))) {
		alert('Data de inicio incorreta');
		return false;
	}*/
	document.getElementById("action").value = "salvar";
	form.submit();
}

function cancelarAssinatura(){
	if(confirm('Deseja realmente cancelar a reformula��o altera��o de vig�ncia?')) {
		document.getElementById("action").value = "cancelar";
		form.submit();
	}
}

</script>
</html>