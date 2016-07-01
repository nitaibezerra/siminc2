<?php

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

function dateDiff1($from,$to){
	/* $diff = $to - $from;
	$info = array();
	if($diff>86400){
		$info['d'] = ($diff - ($diff%86400))/86400;
		$diff = $diff%86400;
	}
	$f = '';
	foreach($info as $k=>$v){
		if($v>0) $f .= "$v";
	}
	return ($f == 24) ? "1" : $f; */
	
	$diferenca = strtotime($from) - strtotime($to);
	$dias = floor($diferenca / (60 * 60 * 24));
	$dias += ( ($d == 0) ? 1 : $d);
	
	return $dias;
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

if($_POST['action'] == 'salvar'){
	
	//ver( $_POST,d );
	
	$ptridoriginal = $_POST['ptridoriginal'];
	$arrPtridrefor = $_POST['vigdatainiciorefor'];
	
	if( $_POST['vigidoriginal'] == 0 ){
		$pmcid = $db->pegaUm("select pmcid from emenda.ptminutaconvenio where ptrid =".$ptridoriginal." and pmcstatus = 'A'");
		$refid = $db->pegaUm("SELECT refid FROM emenda.ptminreformulacao WHERE refstatus = 'A' and refsituacaoreformulacao = 'C' and ptrid = $ptridoriginal limit 1");
		$refid = ( !empty($refid) ? $refid : 'null' );
		if( empty( $pmcid ) ){
			echo "<script>
					alert('Não foi encontrado nenhuma minuta convenio cadastrada!');
					window.location.href = 'emenda.php?modulo=principal/alterarAssinaturaVigencia&acao=A';
				 </script>";
			exit();
		} else {
			
			$vigdatainiciooriginal = formata_data_sql(trim($_POST['vigdatainiciooriginal']));
			$vigdatafim = formata_data_sql(trim($_POST['vigdatafimoriginal']));
			$vigdiasoriginal = $_POST['vigdiasoriginal'];
			
			if(!$vigdiasoriginal || strlen($vigdatainiciooriginal)!= 10 || strlen($vigdatafim)!= 10){
				$db->insucesso( 'Não foi possível cadastrar a data de assinatura!', '', 'principal/alterarAssinaturaVigencia&acao=A' );
				return false;
				die;
			}
			
			$sql = "INSERT INTO emenda.ptvigencia( ptrid, pmcid, vigdatainicio, vigdias, vigdatafim,
							  vigstatus, vigdatainclusao, usucpfinclusao, vigtipo) 
					VALUES ($ptridoriginal,
						  $pmcid,
						  '".$vigdatainiciooriginal."',
						  $vigdiasoriginal,
						  '".$vigdatafim."',
						  'A',
						  now(),
						  '".$_SESSION['usucpf']."',
						  'P')";
						  
			$db->executar($sql);
		}
	} else {
		$vigdatafim = formata_data_sql( $_POST['vigdatafimoriginal'] );
		$vigdatainicio = formata_data_sql( $_POST['vigdatainiciooriginal']);
		$vigdias = $_POST['vigdiasoriginal'];
		
		$sql = "UPDATE emenda.ptvigencia SET 
						vigdatainicio = '$vigdatainicio',
					  	vigdatafim = '$vigdatafim',
					  	vigdias = '$vigdias',
					  	refid = (select refid from emenda.ptminreformulacao where refstatus = 'A' and ptrid = ".$ptridoriginal." limit 1)				 	
					WHERE 
					  	vigid = ".$_POST['vigidoriginal']."
					  	and vigstatus = 'A'";
		$db->executar($sql);
		
		if( is_array( $_POST['vigid'] ) ){
			foreach ($_POST['vigid'] as $vigid) {
				$vigdatainicio = !empty($_POST['vigdatainicio'][$vigid]) ? "'".formata_data_sql( $_POST['vigdatainicio'][$vigid] )."'" : 'null';
				$vigdatafim = !empty($_POST['vigdatafim'][$vigid]) ? "'".formata_data_sql( $_POST['vigdatafim'][$vigid] )."'" : 'null';
				$vigdias = !empty($_POST['vigdias'][$vigid]) ? "'".$_POST['vigdias'][$vigid]."'" : 'null';
				$ptridRefor = $_POST['ptrid'][$vigid];
		
				/*if(!$ptridRefor || strlen($vigdatafim)!= 10 || !$vigdias){
					$db->insucesso( 'Falha ao cadastrar a vigencia da reformulacao!', '', 'principal/alterarAssinaturaVigencia&acao=A' );
					die;
				}*/
				
				$sql = "select refprorrogacaooficio from emenda.ptminreformulacao  where ptrid = $ptridRefor";
				$refprorrogacaooficio = $db->pegaUm($sql);
				$vigtipo = $refprorrogacaooficio == "N" ? "V" : "O";
				
				$sql = "UPDATE emenda.ptvigencia SET 
							vigdatainicio = $vigdatainicio,
						  	vigdatafim = $vigdatafim,
						  	vigdias = $vigdias,
						  	vigtipo = '$vigtipo',
						  	refid = (select refid from emenda.ptminreformulacao where refstatus = 'A' and ptrid = ".$ptridRefor." limit 1)				 	
						WHERE 
						  	vigid = ".$vigid."
						  	and vigstatus = 'A';
					  	
					  	UPDATE emenda.ptminutaconvenio SET 
							pmcdataassinatura = 'now()', 
							pmcdataalteracao = 'now()',
							usucpfalteracao = '".$_SESSION['usucpf']."'
						WHERE 
							ptrid = ".$ptridRefor." 
							and pmcstatus = 'A';";
				
				$db->executar($sql);
				
				$sql = "UPDATE emenda.ptminutaconvenio SET pmcdataassinatura = 'now()', 
							pmcdataalteracao = 'now()', usucpfalteracao = '".$_SESSION['usucpf']."'
						WHERE ptrid = ".$ptridRefor." and pmcstatus = 'A'";	
			 	$db->executar($sql);
			}
		}
	}
		
	/*if( !empty($_POST['vigidoriginal']) ){
		$refid = $db->pegaUm("SELECT refid FROM emenda.ptminreformulacao WHERE refstatus = 'A' and refsituacaoreformulacao = 'C' and ptrid = $ptridoriginal limit 1");
		$refid = ( !empty($refid) ? $refid : 'null' );
		
		$vigdatainicio = formata_data_sql(trim($_POST['vigdatainiciooriginal']));
		$vigdatafim = formata_data_sql(trim($_POST['vigdatafimoriginal']));
		$vigdiasoriginal = $_POST['vigdiasoriginal'];
		
		if(!$vigdiasoriginal || strlen($vigdatainicio)!= 10 || strlen($vigdatafim)!= 10){
			$db->insucesso( 'Não foi possível atualizar a data de assinatura!', '', 'principal/alterarAssinaturaVigencia&acao=A' );
			return false;
			die;
		}
		
		$sql = "UPDATE 
				  	emenda.ptvigencia
				SET 
				  	vigdatainicio = '".$vigdatainicio."',
				  	vigdatafim = '".$vigdatafim."',
				 	vigdias = $vigdiasoriginal,
				 	vigtipo = 'P'
				WHERE 
				  	vigid = ".$_POST['vigidoriginal'];
		
		$db->executar($sql);
	} else {
		$pmcid = $db->pegaUm("select pmcid from emenda.ptminutaconvenio where ptrid =".$ptridoriginal." and pmcstatus = 'A'");
		$refid = $db->pegaUm("SELECT refid FROM emenda.ptminreformulacao WHERE refstatus = 'A' and refsituacaoreformulacao = 'C' and ptrid = $ptridoriginal limit 1");
		$refid = ( !empty($refid) ? $refid : 'null' );
		if( empty( $pmcid ) ){
			echo "<script>
					alert('Não foi encontrado nenhuma minuta convenio cadastrada!');
					window.location.href = 'emenda.php?modulo=principal/alterarAssinaturaVigencia&acao=A';
				 </script>";
			exit();
		} else {
			
			$vigdatainiciooriginal = formata_data_sql(trim($_POST['vigdatainiciooriginal']));
			$vigdatafim = formata_data_sql(trim($_POST['vigdatafimoriginal']));
			$vigdiasoriginal = $_POST['vigdiasoriginal'];
			
			if(!$vigdiasoriginal || strlen($vigdatainicio)!= 10 || strlen($vigdatafim)!= 10){
				$db->insucesso( 'Não foi possível cadastrar a data de assinatura!', '', 'principal/alterarAssinaturaVigencia&acao=A' );
				return false;
				die;
			}
			
			$sql = "INSERT INTO emenda.ptvigencia( ptrid, pmcid, vigdatainicio, vigdias, vigdatafim,
							  vigstatus, vigdatainclusao, usucpfinclusao, vigtipo) 
					VALUES ($ptridoriginal,
						  $pmcid,
						  '".$vigdatainiciooriginal."',
						  $vigdiasoriginal,
						  '".$vigdatafim."',
						  'A',
						  now(),
						  '".$_SESSION['usucpf']."',
						  'P')";
			$db->executar($sql);
		}
	}

	if($arrPtridrefor){
		foreach($arrPtridrefor as $vigid => $ptridrefor){
			
			$vigdatainicio = formata_data_sql(trim($_POST['vigdatainiciorefor'][$vigid]));
			$vigdatafim = formata_data_sql(trim($_POST['vigdatafimrefor'][$vigid]));
			$vigdias = $_POST['vigdiasrefor'][$vigid];
		
			if(!$vigdiasoriginal || strlen($vigdatainicio)!= 10 || strlen($vigdatafim)!= 10){
				$db->insucesso( 'Não foi possível atualizar a data de assinatura de prorrogação de vigência!', '', 'principal/alterarAssinaturaVigencia&acao=A' );
				return false;
				die;
			}
			
			$sqlU.= "UPDATE 
					  	emenda.ptvigencia
					SET 
					  	vigdatainicio = '".$vigdatainicio."',
					  	vigdatafim = '".$vigdatafim."',
					 	vigdias = $vigdias
					WHERE 
					  	vigid = $vigid;
					UPDATE 
						emenda.ptminutaconvenio 
					SET 
						pmcdataassinatura = 'now()', 
						pmcdataalteracao = 'now()',
						usucpfalteracao = '".$_SESSION['usucpf']."'
					WHERE 
						ptrid = ".$_POST['ptridrefor'][$vigid]." 
					and 
						pmcstatus = 'A';";
		}
	 	$db->executar($sqlU);
	}*/
	//
	
	$sql = "UPDATE emenda.ptminutaconvenio SET pmcdataassinatura = 'now()', 
				pmcdataalteracao = 'now()', usucpfalteracao = '".$_SESSION['usucpf']."'
			WHERE ptrid = ".$ptridoriginal." and pmcstatus = 'A'";	
 	$db->executar($sql);
	
	$db->commit();
	$db->sucesso('principal/alterarAssinaturaVigencia');
	die;
}

include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";

monta_titulo( 'Assinaturas', '');

if( $_POST['ptridpesquisa'] ){
	$ptrid = $_POST['ptridpesquisa'];
	$arPtrid =  $db->carregarColuna( "select ptrid from emenda.planotrabalho where ptrcod = ".$_POST['ptridpesquisa']." order by ptrid asc" );
	$ptridPai = $arPtrid[0]; 
	$ptridFilho = $arPtrid[1];
	
	$ptridEstado = ( !empty($ptridFilho) ? $ptridFilho : $ptridPai );
	
	/*if( pegarEstadoAtual( $ptridEstado ) == EM_REFORMULACAO_PROCESSO ){
		echo "<script>alert('Não é possivel alterar os dados da assinatura, pois o pta encontra em reformulação do processo. Por favor, utilize o fluxo normal do sistema, vá em \"Principal »» Plano de Trabalho »» Vigência Convênio\".');
				window.location.href = 'emenda.php?modulo=principal/alterarAssinaturaVigencia&acao=A';
			 </script>";
		die;
	}*/
	
	/*$sql = "SELECT ptv.vigid, to_char(ptv.vigdatainicio, 'DD/MM/YYYY') as vigdatainicio, to_char(ptv.vigdatafim, 'DD/MM/YYYY') as vigdatafim, 
					ptv.vigdias, ptv.refid, ptv.vigtipo
			FROM 
				emenda.ptvigencia ptv
				inner join emenda.planotrabalho ptr on ptr.ptrid = ptv.ptrid
			WHERE 
				ptv.ptrid = $ptridPai
				and ptv.vigstatus = 'A'
				and ptv.vigtipo = 'P'
			order by ptv.ptrid";
	
	if( !empty($ptridPai) )
		$arVigOriginal = $db->pegaLinha( $sql );
		
	$vigidoriginal = $arVigOriginal['vigid']; 
	$vigdatainiciooriginal = $arVigOriginal['vigdatainicio']; 
	$vigdiasoriginal = $arVigOriginal['vigdias']; 
	$vigdatafimoriginal = $arVigOriginal['vigdatafim']; 
	$refidoriginal = $arVigOriginal['refid']; 
	$vigtipooriginal = 'P';
	
	$sql = "SELECT ptv.vigid, to_char(ptv.vigdatainicio, 'DD/MM/YYYY') as vigdatainicio, to_char(ptv.vigdatafim, 'DD/MM/YYYY') as vigdatafim, 
					ptv.vigdias, ptv.refid, ptv.vigtipo, ptv.ptrid 
			FROM 
				emenda.ptvigencia ptv
			WHERE 
				ptv.ptrid = $ptridFilho
				and ptv.vigstatus = 'A'
				and ptv.vigtipo in ('V','O')
			order by ptv.ptrid";

	if( !empty($ptridFilho) )
		$arrVigRef = $db->carregar( $sql );*/
	
	$sql = "SELECT
			ptv.vigid,
			to_char(ptv.vigdatainicio, 'DD/MM/YYYY') as vigdatainicio,
			ptv.vigdatafim,
			ptv.vigdias,
			ptr.ptrsituacao,
			ptv.refid,
			ptv.vigtipo,
			ptv.ptrid,
			(select max(vigid) from emenda.ptvigencia where ptrid in (SELECT ptrid FROM emenda.planotrabalho WHERE ptrcod = {$_POST['ptridpesquisa']}) and vigtipo in ( 'P', 'V', 'O', 'C') and vigstatus = 'A' ) as vigidmax 
		FROM 
			emenda.ptvigencia ptv
			inner join emenda.planotrabalho ptr on ptr.ptrid = ptv.ptrid
		WHERE 
			--ptv.ptrid in (SELECT ptrid FROM emenda.planotrabalho WHERE ptrcod = {$_POST['ptridpesquisa']})
			ptv.ptrid in (SELECT p.ptrid FROM emenda.planotrabalho p
							left join emenda.ptminreformulacao pm on pm.ptrid = p.ptrid and pm.refsituacaoreformulacao <> 'E'
		                    WHERE p.ptrcod = (SELECT ptrcod FROM emenda.planotrabalho WHERE ptrid = {$_POST['ptridpesquisa']}
		                    					--and 'S' = (case when (select count(refid) from emenda.ptminreformulacao where ptrid = p.ptrid and refsituacaoreformulacao <> 'E') > 0 then 'S' else 'N' end)  
												)
						)                    	
			and ptv.vigstatus = 'A'
			and vigtipo in ( 'P', 'V', 'O', 'C')
			and ptv.ptrid not in (SELECT ptrid FROM emenda.ptminreformulacao WHERE refsituacaoreformulacao = 'E' and ptrid in ( SELECT p.ptrid FROM emenda.planotrabalho p WHERE ptrcod = (SELECT ptrcod FROM emenda.planotrabalho WHERE ptrid = {$_POST['ptridpesquisa']})))
		order by 
			ptr.ptrid";
	
	$arPtminuta = $db->carregar($sql);
	$arPtminuta = ( $arPtminuta ? $arPtminuta : array() );
	
	$vigid = !empty($arPtminuta[0]['vigid']) ? $arPtminuta[0]['vigid'] : 0;
}
?>
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
<input type="hidden" name="dataatual" id="dataatual" value="<?=date('d/m/Y'); ?>" />
<input type="hidden" name="vigidoriginal" id="vigidoriginal" value="<?=$vigid; ?>" />
<input type="hidden" name="refidoriginal" id="refidoriginal" value="<?=$refidoriginal; ?>" />
<input type="hidden" name="ptridoriginal" id="ptridoriginal" value="<?=$ptridPai; ?>" />

<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4" style="border-bottom:none;">
	<tr>
		<td class="SubTituloDireita" valign="middle" style="width: 30%;"><b>Numero do PTA:</b></td>
		<td><? echo campo_texto('ptridpesquisa', 'N', 'S', '', 11, 20, '[#]', '', '', '', 0, 'id="ptridpesquisa"', '', $_POST['ptridpesquisa'] ); ?>
			<input type="button" name="btnPesquisa" value="Pesquisar" onclick="carregaPTA();"></td>		
	</tr>
	<?if(!empty($_POST['ptridpesquisa'])){ ?>
	<tr>
		<td colspan="2">
		<input type="hidden" name="vigidmax" id="vigidmax" value="<?=$arPtminuta[0]['vigidmax']; ?>" />

<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4" style="border-bottom:none;">
	<tr>
		<td>
		<table class="tabela" align="center" width="100%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4" style="border-bottom:none;">
		<tr bgcolor="#C0C0C0">
			<td colspan="3"><b>Vigência convênio original</b></td>		
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="middle" style="width: 30%;"><b>Data Celebração:</b></td>
			<td >
			<?
				$vigdatainiciooriginal = ($arPtminuta[0]['vigdatainicio'] ? $arPtminuta[0]['vigdatainicio'] : "");
				echo campo_data2('vigdatainiciooriginal', 'N', 'S','Data Inicio da Vigência', '', '', '', $vigdatainiciooriginal, "calculaDataVigencia(".$vigid.", 'P', this.id);", '', 'vigdatainiciooriginal' );
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="middle"><b>Dias de Vigência:</b></td>
			<td>
			<?
				$vigdiasoriginal = ( empty($arPtminuta[0]['vigdias']) ? 1 : $arPtminuta[0]['vigdias'] );
				echo campo_texto('vigdiasoriginal', 'N', 'S', '', 11, 20, '[#]', '', '', '', 0, 'id="vigdiasoriginal"', '', '', "calculaDataVigencia(".$vigid.", 'P', this.id);" );
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="middle"><b>Data Final da Vigência:</b></td>
			<td>
			<?
				$vigdatafimoriginal = ($arPtminuta[0]['vigdatafim'] ? formata_data($arPtminuta[0]['vigdatafim']) : "");
				echo campo_data2('vigdatafimoriginal', 'N', 'S','Data Final da Vigência', '', '', '', $vigdatafimoriginal, 'calculaDataVigencia('.$vigid.', \'P\', this.id);', '', 'vigdatafimoriginal');
			?>
			</td>
		</tr>
		<?
		foreach ($arPtminuta as $key => $v) {
			if( $key > 0 ){
				$habilita = 'S';
				$disab = '';
				/*if( $v['ptrsituacao'] != 'C' && !$db->testa_superuser() ){
					$habilita = 'N';
					$disab = 'disabled="disabled"';
				} else {*/
					$idVigdatainicio = 'vigdatainicio_'.$v['vigid'];
					$idVigdias = 'vigdias_'.$v['vigid'];
					$idVigdatafim = 'vigdatafim_'.$v['vigid'];
					
					$nomeVigdatainicio = 'vigdatainicio['.$v['vigid'].']';
					$nomeVigdias = 'vigdias['.$v['vigid'].']';
					$nomeVigdatafim = 'vigdatafim['.$v['vigid'].']';
					$nomeVigid = 'vigid['.$v['vigid'].']';
					$nomePtrid = 'ptrid['.$v['vigid'].']';
				//}
				?>
				<tr bgcolor="#C0C0C0">
					<td colspan="3"><b>Vigência convênio prorrogação <?=( sizeof($arPtminuta) > 2 ? $key : ''); ?></b></td>		
				</tr>
				<tr>
					<td class="SubTituloDireita" valign="middle"><b>Data Celebração:</b></td>
					<td >
					<input type="hidden" name="<?=$nomeVigid; ?>" id="<?=$nomeVigid; ?>" value="<?=$v['vigid']; ?>" />
					<input type="hidden" name="<?=$nomePtrid; ?>" id="<?=$nomePtrid; ?>" value="<?=$v['ptrid']; ?>" />
					<?	
						$vigdatainicio = ($v['vigdatainicio'] ? $v['vigdatainicio'] : "");
						echo campo_data2($nomeVigdatainicio, 'N', $habilita,'Data Inicio da Vigência', '', '', 'calculaDataVigencia('.$v['vigid'].', \'V\', this.id);', $vigdatainicio, '', '', $idVigdatainicio);
					?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita" valign="middle"><b>Dias de Vigência:</b></td>
					<td>
					<?
						$vigdias = ( empty($v['vigdias']) ? '' : $v['vigdias'] );
						echo campo_texto($nomeVigdias, 'N', $habilita, '', 11, 20, '[#]', '', '', '', 0, 'id="'.$idVigdias.'"','', $vigdias, 'calculaDataVigencia('.$v['vigid'].', \'V\', this.id);' );
					?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita" valign="middle"><b>Data Final da Vigência:</b></td>
					<td>
					<?
						$vigdatafim = ($v['vigdatafim'] ? formata_data($v['vigdatafim']) : "");
						echo campo_data2($nomeVigdatafim, 'N', $habilita,'Data Final da Vigência', '', '', 'calculaDataVigencia('.$v['vigid'].', \'V\', this.id);', $vigdatafim, '', '', $idVigdatafim);
					?>
					</td>
				</tr>
		<?php
			}
		}?>
	<tr>
		<td align="center" bgcolor="#c0c0c0" colspan="4">
			<input type="button" id="bt_salvar" value="Salvar" onclick="salvarAssinatura();"  />
		</td>
	</tr>
	</table>
	</td>
	</tr>
	<?} ?>
</table>
</body>
<script type="text/javascript">
var btSalvar	= document.getElementById("bt_salvar");
var btCancelar	= document.getElementById("bt_cancelar");
var form		= document.getElementById("formAssinaturas");

function carregaPTA(){
	//document.getElementById('ptrid').value;
	form.submit();
}

/*function calculaDiasVigencia( vigid, tipo ){	
	if( tipo == 'P' ){
		var datainicio = $('vigdatainiciooriginal').value;
		var nomediavigencia = 'vigdiasoriginal';
		var nomevigdatafim = 'vigdatafimoriginal';
		var datafim = $('vigdatafimoriginal').value;
		var diavigencia = $('vigdiasoriginal').value;
	} else {
		var datainicio = $('vigdatainicio_'+vigid).value;
		var nomediavigencia = 'vigdias_'+vigid;
		var nomevigdatafim = 'vigdatafim_'+vigid;
		var datafim = $('vigdatafim_'+vigid).value;
		var diavigencia = $('vigdias_'+vigid).value;
	}
	
	if( diavigencia != '' && datainicio != '' ){
		var myajax = new Ajax.Request('emenda.php?modulo=principal/alterarAssinaturaVigencia&acao=A', {
		        method:     'post',
		        parameters: '&pegaDataFinal=true&vigdias='+diavigencia+'&vigdatainicio='+datainicio,
		        asynchronous: false,
		        onComplete: function (res){
					$(nomevigdatafim).value = res.responseText;
		        }
		  });
	}
}

function calculaDataFimVigencia( vigid, tipo ){
	if( tipo == 'P' ){
		var datainicio = $('vigdatainiciooriginal').value;
		var nomediavigencia = 'vigdiasoriginal';
		var nomevigdatafim = 'vigdatafimoriginal';
		var datafim = $('vigdatafimoriginal').value;
		var diavigencia = $('vigdiasoriginal').value;
	} else {
		var datainicio = $('vigdatainicio_'+vigid).value;
		var nomediavigencia = 'vigdias_'+vigid;
		var nomevigdatafim = 'vigdatafim_'+vigid;
		var datafim = $('vigdatafim_'+vigid).value;
		var diavigencia = $('vigdias_'+vigid).value;
	}
	
	if( datafim != '' && datainicio != '' ){
		var myajax = new Ajax.Request('emenda.php?modulo=principal/alterarAssinaturaVigencia&acao=A', {
		        method:     'post',
		        parameters: '&pegaDataFinal=true&vigdatafim='+datafim+'&vigdatainicio='+datainicio,
		        asynchronous: false,
		        onComplete: function (res){
		        	$(nomediavigencia).value = res.responseText;
		        }
		  });
	} else if( diavigencia != '' && datainicio != '' ){
		var myajax = new Ajax.Request('emenda.php?modulo=principal/alterarAssinaturaVigencia&acao=A', {
		        method:     'post',
		        parameters: '&pegaDataFinal=true&vigdias='+diavigencia+'&vigdatainicio='+datainicio,
		        asynchronous: false,
		        onComplete: function (res){
		        	$(nomevigdatafim).value = res.responseText;
		        }
		  });
	}	
}*/

function calculaDataVigencia( vigid, tipo, campo ){
	var id = campo.split('_');
	
	if( tipo == 'P' ){
		var nomevigdatainicio = 'vigdatainiciooriginal';
		var nomediavigencia = 'vigdiasoriginal';
		var nomevigdatafim = 'vigdatafimoriginal';
		
		var datainicio = $('vigdatainiciooriginal').value;
		var datafim = $('vigdatafimoriginal').value;
		var diavigencia = $('vigdiasoriginal').value;
	} else {
		var nomevigdatainicio = 'vigdatainicio_'+vigid;
		var nomediavigencia = 'vigdias_'+vigid;
		var nomevigdatafim = 'vigdatafim_'+vigid;
		
		var datainicio = $('vigdatainicio_'+vigid).value;
		var datafim = $('vigdatafim_'+vigid).value;
		var diavigencia = $('vigdias_'+vigid).value;
	}
	
	if( datainicio == '' ){
		$(nomevigdatainicio).value = $('dataatual').value;
		datainicio = $('dataatual').value;
	}
	//alert( id[0]+' - '+datainicio+' - '+diavigencia+' - '+datafim );
	if( ( id[0] == 'vigdias' || id[0] == 'vigdiasoriginal' ) && diavigencia != '' ){
		var myajax = new Ajax.Request('emenda.php?modulo=principal/alterarAssinaturaVigencia&acao=A', {
		        method:     'post',
		        parameters: '&pegaDataFinal=true&vigdias='+diavigencia+'&vigdatainicio='+datainicio,
		        asynchronous: false,
		        onComplete: function (res){
		        	$(nomevigdatafim).value = res.responseText;
		        }
		  });
	} else if( (id[0] == 'vigdatainicio' || id[0] == 'vigdatainiciooriginal' || id[0] == 'vigdatafim' || id[0] == 'vigdatafimoriginal' ) &&  datafim != '' ){
		var myajax = new Ajax.Request('emenda.php?modulo=principal/alterarAssinaturaVigencia&acao=A', {
		        method:     'post',
		        parameters: '&pegaDataFinal=true&vigdatafim='+datafim+'&vigdatainicio='+datainicio,
		        asynchronous: false,
		        onComplete: function (res){
		        	$(nomediavigencia).value = res.responseText;
		        }
		  });
	}
	/*
	var myajax = new Ajax.Request('emenda.php?modulo=principal/alterarAssinaturaVigencia&acao=A', {
		        method:     'post',
		        parameters: '&pegaDataFinal=true&vigdias='+diavigencia+'&vigdatafim='+datafim,
		        asynchronous: false,
		        onComplete: function (res){
		        	$(nomevigdatafim).value = res.responseText;
		        }
		  });
	var myajax = new Ajax.Request('emenda.php?modulo=principal/alterarAssinaturaVigencia&acao=A', {
		        method:     'post',
		        parameters: '&pegaDataFinal=true&vigdatafim='+datafim+'&vigdatainicio='+datainicio,
		        asynchronous: false,
		        onComplete: function (res){
		        	$(nomediavigencia).value = res.responseText;
		        }
		  });
		  */
}

function salvarAssinatura(){
	document.getElementById("action").value = "salvar";
	form.submit();
}

</script>