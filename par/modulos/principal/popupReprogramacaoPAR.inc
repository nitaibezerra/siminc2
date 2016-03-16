<?php

if( $_POST['calculaDiasPAR'] == true ){
	global $db;

	if( $_REQUEST['dataAtual'] >= formata_data_sql($_REQUEST['data']) ){
		echo 'data';
		die();
	}
	
	if( !validaData( formata_data_sql($_REQUEST['data']) ) ){
		echo 'erro';
		die();
	}

	$sql = "SELECT date '".formata_data_sql($_REQUEST['data'])."' - date '".$_REQUEST['dataAtual']."' as dias";
	$dias = $db->pegaUm($sql);

	echo $dias;
	die();
}

if( $_POST['requisicao'] == 'salva' ){
	global $db;
	
	if( $_POST['prazoFinalProrrogacao'] && $_POST['dias'] && is_numeric($_POST['dias']) && $_POST['dprjustificativa'] ){

		if( substr($_POST['prazoFinalProrrogacao'], 0, 2) == '02' ){
			$var = '28';
		} else {
			$var = '30';
		}

		$data = $var."/".$_POST['prazoFinalProrrogacao'];

		$sql = "INSERT INTO par.documentoparreprogramacao ( dopid, usucpf, dprqtddiassolicitado, dprdataprazo, dprstatus, dprdata, dprjustificativa ) VALUES ( ".$_POST['dopid'].", '".$_SESSION['usucpf']."', ".$_POST['dias'].", '".formata_data_sql($data)."', 'P', 'NOW()', '".$_POST['dprjustificativa']."' ); ";
		$db->executar($sql);
		$db->commit();
		
		$dopIdLog = $_POST['dopid'];
		$numeroprotocolo =  Date('YdHis').$dopIdLog; 
		$justificativa = $_POST['dprjustificativa'];
		$cpfLog = $_SESSION['usucpf'];
		
		if( $dopIdLog )
		{
			$prpidLog = $db->pegaUm(" SELECT prpid from par.documentopar where dopid = {$dopIdLog}");
		}
		
		if( $numeroprotocolo && $cpfLog && $justificativa && $dopIdLog  && $prpidLog)
		{
			$sqlInsert = "
			INSERT into par.historicosolicitacaoreprogramacao
				( hsrprotocolo, hsrdata, hsrtipo, hsrcpf, hsrjustificativa, dopid , prpid )
			VALUES
				( '{$numeroprotocolo}', 'now()', 'P', '{$cpfLog}', '{$justificativa}', {$dopIdLog}, {$prpidLog} )
			RETURNING hsrid
			";
		
			$hsrid = $db->pegaUm($sqlInsert);
			$db->commit();
		}
		
		if($hsrid)
		{
			enviaEmailProtocoloReprogramacao($hsrid , 'P', $dopIdLog );
		}
		
		enviaEmailReprogramacao('prazo', $_POST['dopid'], $_POST['prazoFinalProrrogacao']);
		
		echo '<script>alert("Protocolo '.$numeroprotocolo.' Dados salvos com sucesso! Para imprimir o protocolo dessa solicitção acesse na coluna \"Diálogo\" na aba \"Execução e Acompanhamento\"."); window.opener.location.reload(); window.close();</script>';
	}
}

/* Verifica se é uma subação ativa e valida documento submetido*/
if( $_REQUEST['dopid'] ){
	if( $_REQUEST['dopid'] == 'undefined' ){
		echo "
			<script>
				alert('Documento inválido.');
				window.close();
			</script>";
		die();
	}
	$sql = "SELECT dopstatus FROM par.documentopar WHERE dopid = {$_REQUEST['dopid']}";
	$dopstatus = $db->pegaUm($sql);
	if( $dopstatus != 'A' ){
		echo "
			<script>
				alert('Documento inválido.');
				window.close();
			</script>";
		die();
	}
}else{
	echo "
		<script>
			alert('Documento inválido.');
			window.close();
		</script>";
	die();
}
/* FIM - Verifica se é uma subação ativa e valida documento submetido*/

if( !is_numeric($_REQUEST['dopid']) ){
	echo
	"<script>
			alert('Dados insuficientes. Favor escolher um Termo.');
			window.close();
		</script>";
	die();
}

?>
<form name="form_prorrogacao" id="form_prorrogacao" method="post" action="" >
	<link rel="stylesheet" type="text/css" href="/includes/Estilo.css"/>
	<link rel="stylesheet" type="text/css" href="/includes/listagem.css"/>
	<script type="text/javascript" src="../includes/funcoes.js"></script>
	<script type="text/javascript" src="/includes/prototype.js"></script>
	<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
	<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
	<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
	<script type="text/javascript">
		function calculaDias(data){
			var dataAtual = $('dataAtual').value;
	
			teste = data.substring(2, 3);
		
			if( teste != '/' ){
				alert("O campo deve vir no seguinte formato: mm/aaaa!");
        		$('prazoFinalProrrogacao').value = '';
        		$('qtddias').value = '';
				return false;
			}
	
			mes = data.substring(0, 2);

			if( mes > 12 ){
				alert('O mês '+mes+' não existe no calendário gregoriano!');
        		$('prazoFinalProrrogacao').value = '';
        		$('qtddias').value = '';
				return false;
			}

			if( mes == '02' ){ // Fevereiro
				dia = 28;
			} else { // Outros
				dia = 30;
			}

			data = dia+'/'+data;
			
			if( data && dataAtual){
				var myajax = new Ajax.Request('par.php?modulo=principal/popupReprogramacaoPAR&acao=A', {
				        method:     'post',
				        parameters: '&calculaDiasPAR=true&data='+data+'&dataAtual='+dataAtual,
				        asynchronous: false,
				        onComplete: function (res){
				        	if( res.responseText.trim() == 'data' ){
				        		alert('A data solicitada deve ser maior que o fim da vigência atual.');
				        		$('prazoFinalProrrogacao').value = '';
				        		$('qtddias').value = '';
				        	} else if( res.responseText.trim() == 'erro' ){
				        		alert('A data solicitada é inválida.');
				        		$('prazoFinalProrrogacao').value = '';
				        		$('qtddias').value = '';
				        	} else {
				        		$('qtddias').value = res.responseText;
				        		$('dias').value = res.responseText;
				        	}
				        }
				  });
			}	
		}
		function salvar(){
			if(!$('prazoFinalProrrogacao').value){
				alert('Informe uma data!');
				return false;
			}
			if( !$('dprjustificativa').value ){
				alert('Informe uma justificativa!');
				return false;
			}
			$('requisicao').value = 'salva';		
			$('form_prorrogacao').submit();		
		}
	</script>
<?php
global $db;

$sql = "SELECT 
			dopdatafimvigencia as prazo
		FROM 
			par.documentopar
		WHERE 
			dopid = ".$_REQUEST['dopid'];

$prazo = $db->pegaUm($sql);

if( substr($prazo, 0, 2) == '02' ){
	$var = '28';
} else {
	$var = '30';
}
?>
<input type="hidden" id="dataAtual" name="dataAtual" value="<?php echo formata_data_sql($var.'/'.$prazo); ?>">
<input type="hidden" id="dopid" name="dopid" value="<?php echo $_REQUEST['dopid']; ?>">
<input type="hidden" id="requisicao" name="requisicao" value="">
<input type="hidden" id="dias" name="dias" value="">
<table align="center" cellspacing="0" cellpadding="3" border="0" bgcolor="#DCDCDC" style="border-top: none; border-bottom: none;" class="tabela">
	<tr>
		<td align="center" bgcolor="#e9e9e9" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')">
			<b>Prorrogação de Prazo (Vigência)</b>
		</td>
	</tr>
</table>
<?php 
/* Verifica se está em caso de reformulação Demanda PAR 322 */
$sql = "SELECT DISTINCT
			TRUE
		FROM 
			par.documentopar dop
		INNER JOIN par.vm_documentopar_ativos			dop2 ON dop2.dopnumerodocumento = dop.dopnumerodocumento
		LEFT  JOIN par.modelosdocumentos				mdo  ON mdo.mdoid = dop2.mdoid
		LEFT  JOIN par.documentoparreprogramacaosubacao	dpr  ON dpr.dopid = dop2.dopid
		LEFT  JOIN par.reprogramacao 					rep2 ON rep2.dopidreprogramado = dop2.dopid 
		LEFT  JOIN par.documentoparreprogramacaosubacao	dpr2 ON rep2.repid = dpr2.repid
		WHERE 
			(
				(dop2.dopreformulacao IS TRUE AND dpr.dpsstatus = 'A') 
				OR (dpr.dpsstatus = 'P') 
				OR rep2.repstatus = 'A'
			)
			AND dop.dopid =  {$_REQUEST['dopid']}";

$emReformulacao = $db->pegaUm($sql);

if( $emReformulacao == 't' ){

?>
	<div style="position:absolute;width:84%;">
		<div style="position:relative;float:right;color:red;text-align:center">
			<img src="../imagens/par/carimbo-reformulacao.png" border="0">
			<br>
			Este termo se encontra em reformulação.
			<br>
			Para uma nova reformulação é necessário a conclusão da atual.
		</div>
	</div>
	<?php 
	die();
}

// Ao pedir reprogramação ou acessar esta tela significa que o termo já pode novamente vencer consequentemente precisa mandar email denovo por isso o trecho abaixo
if($_REQUEST['dopid'])
{
	$sql = "SELECT
				prpnumeroprocesso
			FROM
				par.processopar prp
			INNER JOIN par.documentopar dop ON dop.prpid = prp.prpid
			WHERE
				dopid = {$_REQUEST['dopid']}";
	
	$prpnumeroprocesso = $db->pegaUm($sql);
	
	if($prpnumeroprocesso)
	{
		$db->executar("UPDATE par.emailreprogramacaobloqueada SET erbstatus = 'I' where erbnumeroprocesso = '{$prpnumeroprocesso}' AND erbstatus = 'A'");
		$db->commit();
	}
}


/* FRIM - Verifica se está em caso de reformulação*/

?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<colgroup>
			<col width="40%" />
			<col width="65%" />
	</colgroup>
	<?php 
       /*     #Verifica se o pedido pra reprogramar prazo é maior que 90 dias para o vencimento
            #Caso maior que 90 dias não permitir reprogramação.
            $sql = "SELECT 
                         to_date(dopdatafimvigencia,'MM/YYYY') - to_date('".date('m/Y')."','MM/YYYY') as dia 
                    FROM par.documentopar
                    WHERE dopid = {$_REQUEST['dopid']}";
            $qtdDiaVencimento = $db->pegaUm($sql); 
            
            if ($qtdDiaVencimento > 90) {
                $strTexto = "Não é possível solicitar Reprogramação de Prazo, pois o Termo de Compromisso vigente possui mais de 90 dias para o vencimento.";
                echo "<script>alert('{$strTexto}'); window.close();</script>";
            }*/
            ?>
	<tr>
		<td class="SubTituloDireita">
			Vigência do Documento:
		</td>
		<td>
			<?php echo $prazo; ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">
			Data final do prazo requisitado:
		</td>
		<td>
			<?php echo campo_texto('prazoFinalProrrogacao', 'S', 'S','Data final do prazo requisitado', 10, 7, '##/####', '', '', '', '', 'id="prazoFinalProrrogacao"', '', '', 'calculaDias(this.value)'); ?> <b>(mm/aaaa)</b> &nbsp;Dias: <input type="text" id="qtddias" name="qtddias" disabled>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">
			Justificativa:
		</td>
		<td>
			<?php echo campo_textarea('dprjustificativa', 'S', 'S', 'Justificativa', 60, 10, 500) ?>
		</td>
	</tr>
	<tr align="center">
		<td style="BACKGROUND-COLOR: #dcdcdc" colspan="2"><input type="button" onclick="salvar()" value="salvar"></td>
	</tr>
</table>
</form>