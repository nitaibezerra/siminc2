<?php
$_REQUEST['preid'] = simec_strip_tags($_REQUEST['preid']);

if( $_POST['calculaDiasPAC'] == true ){
	
	global $db;
	
	if( $_REQUEST['dataAtual'] >= formata_data_sql($_REQUEST['data']) ){
		echo 'erro';
		die();
	}
	
	if( !validaData( $_REQUEST['dataAtual'] ) || !validaData( formata_data_sql($_REQUEST['data']) ) ){
		echo 'invalido';
		die();
	}
	
	if( $_REQUEST['calcula'] > 0 ){
		$data = $db->pegaUm("SELECT cast('".$_REQUEST['dataAtual']."' AS date) + interval'".$_REQUEST['calcula']." months' AS data");

		if( formata_data_sql($_REQUEST['data']) > $data ){
			echo 'erro2';
			die();
		}
	}
	
	$dt = explode('/',$_REQUEST['data']);
	
	if( count($dt) < 3 ){
		$_REQUEST['data'] = "01/".$_REQUEST['data'];
	}
	
	$sql = "SELECT date '".formata_data_sql($_REQUEST['data'])."' - date '".$_REQUEST['dataAtual']."' as dias";
	$dias = $db->pegaUm($sql);
	
	echo $dias;
	die();
}

if( $_POST['requisicao'] == 'salva' ){
	global $db;
	
	$sql = "SELECT docid FROM obras.preobra WHERE preid = {$_POST['preid']}";
	$docid = $db->pegaUm( $sql );
	
	$sql = "SELECT esdid FROM workflow.documento WHERE docid = $docid";
	$esdidorigem = $esdid = $db->pegaUm( $sql );
	
	$perfil = pegaPerfilGeral();
	
	if( !is_numeric($_POST['dias']) ){
		unset($_POST['dias']);
	}
	
	if( $_POST['prazoFinalProrrogacao'] && $_POST['dias'] && $_POST['popjustificativa'] && !in_array($esdidorigem,Array(WF_TIPO_OBRA_AGUARDANDO_PRORROGACAO, WF_PAR_OBRA_AGUARDANDO_PRORROGACAO)) ){
		
		$str = substr($_POST['prazoFinalProrrogacao'], 0, 4);
		if( is_numeric($str) ){
			$prazoFinalProrrogacao = $_POST['prazoFinalProrrogacao'];
		} else {
			$prazoFinalProrrogacao = formata_data_sql($_POST['prazoFinalProrrogacao']);
		}
		
		include_once APPRAIZ . "includes/workflow.php";
			
		$sql = "SELECT tooid FROM obras.preobra WHERE preid = {$_POST['preid']}";
		$tooid = $db->pegaUm($sql);
		
		// Se a obra for PAC
		if( $tooid == 1 ){
			$esdiddestino = WF_TIPO_OBRA_AGUARDANDO_PRORROGACAO;
		}else{// Sea obra não for PAC
			$esdiddestino = WF_PAR_OBRA_AGUARDANDO_PRORROGACAO;
		}
		
		$sql = "SELECT aedid FROM workflow.acaoestadodoc WHERE esdidorigem = $esdidorigem AND esdiddestino = $esdiddestino";
		$aedid = $db->pegaUm( $sql );
		
		$_POST['popjustificativa'] = str_ireplace("'", "", substituir_char_especiais_word($_POST['popjustificativa']));
		$sql = "INSERT INTO obras.preobraprorrogacao ( preid, usucpf, popqtddiassolicitado, popdataprazo, popstatus, popdata, popjustificativa, esdidorigem ) 
				VALUES ( ".$_POST['preid'].", '".$_SESSION['usucpf']."', ".$_POST['dias'].", '".$prazoFinalProrrogacao."', 'P', 'NOW()', '".$_POST['popjustificativa']."', ".$esdidorigem." ); ";
		$db->executar($sql);
		
		if( empty($aedid) ){
			$sql = "SELECT aeddscrealizar, aeddscrealizada FROM workflow.acaoestadodoc WHERE aedstatus = 'A' and esdiddestino = $esdiddestino limit 1";
			$arAcoes = $db->pegaLinha( $sql );
				
			$sql = "INSERT INTO workflow.acaoestadodoc(esdidorigem, esdiddestino, aeddscrealizar, aeddscrealizada, aedcondicao, aedobs, aedposacao, aedvisivel, aedicone, aedcodicaonegativa)
					VALUES ($esdidorigem, $esdiddestino, 'Solicitar Prorrogação de Vigência', 'Prorrogação de VIgência Solicitada', '', '', '', false, '', false) RETURNING aedid";
				
			$aedid = $db->pegaUm( $sql );
		}
		
		$result = wf_alterarEstado( $docid, $aedid, true, $d = array('preid' => $_POST['preid']));
		
		// Verifico se a obra já está vencida
		if( $_POST['dataAtual'] && (date('Y-m-d') >= $_POST['dataAtual']) && (in_array(PAR_PERFIL_SUPER_USUARIO, $perfil)) ){
			$result = gerarDocumentoProrrogacao( $_POST['preid'], $_POST['popjustificativa'], $prazoFinalProrrogacao, $_POST['dias'], $tipo = 2 );		
		}
		
		if( $result ){
			$db->commit();
			echo '<script>alert("Dados salvos com sucesso!"); window.opener.location.reload(); window.close();</script>';
		} else {
			$db->rollback();
			echo '<script>alert("Problemas na execução!"); window.close();</script>';
		}
	}
}

?>
<link rel="stylesheet" type="text/css" href="/includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="/includes/listagem.css"/>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
    
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script type="text/javascript">
$(document).ready(function(){
	<? if( validaPossibilidadeProrrogacao($_REQUEST['preid']) == 'vermelho' ){ ?>
		alert('Esta obra encontra-se a mais de 60 dias sem atualização no Simec – módulo Monitoramento de Obras.\n\nAtualize por meio de inserção de vistoria e preenchimento das demais abas do sistema.');
	<? } ?>

	$('.salvar').click(function(){
		
		if( !$('#prazoFinalProrrogacao').val() ){
			alert('Informe uma data!');
			return false;
		}
		if( !$('#popjustificativa').val() ){
			alert('Informe uma justificativa!');
			return false;
		}
		$('#requisicao').val('salva');		
		$('#form_prorrogacao').submit();	
	});
});
function calculaDias(data){
	var dataAtual 	= $('#dataAtual').val();
	var calcula 	= $('#calcula').val();
	
	$.ajax({
		type: 	"POST",
		url: 	window.location.href,
		data: 	'&calculaDiasPAC=true&data='+data+'&dataAtual='+dataAtual+'&calcula='+calcula,
		async: 	false,
		success: function(msg){
			
			if( msg == 'invalido' ){
        		alert('A data informada é inválida.');
        		$('#prazoFinalProrrogacao').val('');
        		$('#qtddias').val('');
        	}else if( msg == 'erro' ){
        		alert('A data solicitada deve ser maior que o fim da vigência atual.');
        		$('#prazoFinalProrrogacao').val('');
        		$('#qtddias').val('');
        	} else if( msg == 'erro2' ){
        		alert('A data solicitada deve respeitar o prazo máximo a ser concedido');
        		$('#prazoFinalProrrogacao').val('');
        		$('#qtddias').val('');
        	} else {
        		$('#qtddias').val(msg);
        		$('#dias').val(msg);
        	}
		}
	});
}
</script>
<form name="form_prorrogacao" id="form_prorrogacao" method="post" action="" >
<?php
global $db;

$sql = "SELECT tooid FROM obras.preobra WHERE preid = {$_REQUEST['preid']}";
$tooid = $db->pegaUm($sql);

// Se a obra for PAC
if( $tooid == 1 ){
	$sql = "select 
				MIN(pag.pagdatapagamentosiafi) as data_primeiro_pagamento,
				MIN(pag.pagdatapagamentosiafi) + 720 as prazo
			from 
				par.pagamentoobra po 
			inner join par.pagamento pag ON pag.pagid = po.pagid AND pag.pagstatus = 'A'
			where 
				po.preid = ".$_REQUEST['preid'];
}else{// Sea obra não for PAC
	$sql = "SELECT 
				MIN(pag.pagdatapagamentosiafi) as data_primeiro_pagamento,
				MIN(pag.pagdatapagamentosiafi) + 720 as prazo
			FROM 
				par.pagamentoobrapar po 
			INNER JOIN par.pagamento pag ON pag.pagid = po.pagid AND pag.pagstatus = 'A'
			WHERE 
				po.preid = {$_REQUEST['preid']}";
}

$dataPrimeiroPagamento = $db->carregar($sql);

$sql = "SELECT 
			pre.predescricao, 
			mun.mundescricao || '/' || mun.estuf as entidade ,
			COALESCE(obr.obrpercentultvistoria, 0) as percexec,
			doc.esdid, 
			esd2.esddsc as situacao, 
			pto.ptoclassificacaoobra as classificacao,
			CASE 
				WHEN ov.supdtinclusao IS NOT NULL THEN 
					ov.supdtinclusao
				WHEN obr.obrdtvistoria IS NOT NULL THEN 
					obr.obrdtvistoria
				ELSE 
					obr.obrdtinclusao
			END as ultatualizacao
		FROM 
			obras.preobra pre
		INNER JOIN territorios.municipio mun ON mun.muncod = pre.muncod
		LEFT  JOIN obras2.obras obr 
			INNER JOIN workflow.documento 		doc ON doc.docid = obr.docid
			INNER JOIN workflow.estadodocumento esd2 ON esd2.esdid = doc.esdid
		ON obr.preid = pre.preid
		INNER JOIN obras.pretipoobra 			pto ON pto.ptoid = pre.ptoid 
		INNER JOIN workflow.documento 			d   ON d.docid = pre.docid
		INNER JOIN workflow.estadodocumento 	esd ON esd.esdid = d.esdid
		LEFT  JOIN
			(SELECT
				supvid as supervisao, rsuid,obrid, supdtinclusao
			FROM
				obras.supervisao s
			WHERE
				supvid = (SELECT supvid FROM obras.supervisao ss 
					  	  WHERE ss.supstatus = 'A' AND ss.obrid = s.obrid 
					  	  ORDER BY supvdt desc, supvid DESC LIMIT 1)
			) AS ov 
		ON ov.obrid = obr.obrid
		WHERE 
			pre.prestatus = 'A' AND pre.preid = {$_REQUEST['preid']}";

$dadosObra = $db->pegaLinha( $sql );

if( $dataPrimeiroPagamento[0]['data_primeiro_pagamento'] ){
	$sql = "SELECT popdataprazoaprovado FROM obras.preobraprorrogacao WHERE popstatus = 'A' AND preid = ".$_REQUEST['preid'];
	$prorrogado = $db->pegaUm($sql);
	if( $prorrogado ){
		$dataAtual = $prorrogado;
	} else {
		$dataAtual = $dataPrimeiroPagamento[0]['prazo'];
	}
}
/*
 * 	stoid = 12 (Aguardando registro de preços) -> 12 meses
	stoid = 99 (Em Planejamento pelo Proponente) -> 12 meses
	stoid = 5 (Em Licitação) -> 12 meses
	stoid = 1 (Em Execução) -> entre 0 e 25 (12 meses), 25 a 50 (9 meses), 50 a 75 (6 meses), 75 a 100 (4 meses)
	stoid = 9 (Em Reformulação) ou 2 (Paralisada) -> análise individual
	
	Q
	stoid = 99 (Em Planejamento pelo Proponente) -> 12 meses
	stoid = 5 (Em Licitação) -> 9 meses
	stoid = 1 (Em Execução) -> 0 a 50 (6 meses), 50 a 100 (3 meses)
	stoid = 9 (Em Reformulação) ou 2 (Paralisada) -> análise individual
 */

$arrCondicao1 = Array(OBR_ESDID_AGUARDANDO_REGISTRO_DE_PRECOS, 
					  OBR_ESDID_EM_PLANEJAMENTO_PELO_PROPONENTE, 
					  OBR_ESDID_EM_LICITACAO);
$arrCondicao2 = Array(OBR_ESDID_EM_REFORMULACAO, 
					  OBR_ESDID_PARALISADA);
if( $dadosObra['classificacao'] == 'P' ){
	if( in_array($dadosObra['esdid'],$arrCondicao1) ){
		$prazoMaximo = "12 Meses";
		$calcula = 12;
	} elseif( $dadosObra['esdid'] == OBR_ESDID_EM_EXECUCAO ){
		if( $dadosObra['percexec'] < 26 ){
			$prazoMaximo = "12 Meses";
			$calcula = 12;
		} elseif( $dadosObra['percexec'] > 25 && $dadosObra['percexec'] < 51 ){
			$prazoMaximo = "9 Meses";
			$calcula = 9;
		} elseif( $dadosObra['percexec'] > 50 && $dadosObra['percexec'] < 76 ){
			$prazoMaximo = "6 Meses";
			$calcula = 6;
		} elseif( $dadosObra['percexec'] > 75 ){
			$prazoMaximo = "4 Meses";
			$calcula = 4;
		}
	} elseif( in_array($dadosObra['esdid'],$arrCondicao2) ){
		$prazoMaximo = "Análise Individual";
		$calcula = 0;
	}
} elseif( $dadosObra['classificacao'] == 'Q' ){
	if( $dadosObra['esdid'] == OBR_ESDID_EM_PLANEJAMENTO_PELO_PROPONENTE ){
		$prazoMaximo = "12 Meses";
		$calcula = 12;
	} elseif( $dadosObra['esdid'] == OBR_ESDID_EM_LICITACAO ){
		$prazoMaximo = "9 Meses";
		$calcula = 9;
	} elseif( $dadosObra['esdid'] == OBR_ESDID_EM_EXECUCAO ){
		if( $dadosObra['percexec'] < 51 ){
			$prazoMaximo = "6 Meses";
			$calcula = 6;
		} elseif( $dadosObra['percexec'] > 50 ){
			$prazoMaximo = "3 Meses";
			$calcula = 3;
		}
	} elseif( in_array($dadosObra['esdid'],$arrCondicao2) ){
		$prazoMaximo = "Análise Individual";
		$calcula = 0;
	}
}

?>
<input type="hidden" id="dataAtual" name="dataAtual" value="<?php echo $dataAtual; ?>">
<input type="hidden" id="preid" name="preid" value="<?php echo $_REQUEST['preid']; ?>">
<input type="hidden" id="calcula" name="calcula" value="<?php echo $calcula; ?>">
<input type="hidden" id="requisicao" name="requisicao" value="">
<input type="hidden" id="dias" name="dias" value="">
<table align="center" cellspacing="0" cellpadding="3" border="0" bgcolor="#DCDCDC" style="border-top: none; border-bottom: none;" class="tabela">
	<tr>
		<td align="center" bgcolor="#e9e9e9" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')">
			<b>Ajuste de Prazo</b>
		</td>
	</tr>
</table>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td colspan="2">
			<span style="color: red; font-size: 13; text-align: justify;"> 
				 <table cellspacing="0" cellpadding="2" border="0" align="center" width="95%" class="listagem" style="margin-top: 10px;">
					<thead>
                    	<tr align="center">
	                        <td valign="top" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title"><b>Situação da Obra</b></td>
	                        <td valign="top" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title"><b>Última Atualização</b></td>
	                        <td valign="top" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title"><b>% Executado</b></td>
	                        <td valign="top" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title"><b>Fim da vigência atual</b></td>
	                        <td valign="top" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title"><b>Prazo Máximo a ser concedido após o fim da vigência atual</b></td>
						</tr>
					</thead>
					<tr>
						<td><?php echo $dadosObra['situacao']; ?></td>
						<td><?php echo formata_data($dadosObra['ultatualizacao']); ?></td>
						<td><?php echo $dadosObra['percexec']; ?></td>
						<td><?php echo formata_data($dataAtual); ?></td>
						<td><?php echo $prazoMaximo; ?></td>
					</tr>
				</table>
			</span>
		</td>
	</tr>
</table>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<colgroup>
			<col width="40%" />
			<col width="65%" />
	</colgroup>
	<tr>
		<td class="SubTituloDireita">
			Data final do prazo requisitado:
		</td>
		<td>
			<?php echo campo_data2('prazoFinalProrrogacao', 'S', 'S','Data final do prazo requisitado', '', '', 'calculaDias(this.value);', '', 'calculaDias(this.value);', ''); ?> Dias: <input type="text" id="qtddias" name="qtddias" disabled>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">
			Justificativa:
		</td>
		<td>
			<?php echo campo_textarea('popjustificativa', 'S', 'S', 'Justificativa', 60, 10, 200) ?>
		</td>
	</tr>
	<tr align="center">
		<td style="BACKGROUND-COLOR: #dcdcdc" colspan="2"><input type="button" class="salvar" value="salvar"></td>
	</tr>
</table>
</form>