<?php

if( $_POST['calculaDiasPAR'] == true ){
	global $db;
	
	if( $_REQUEST['dataAtual'] >= formata_data_sql($_REQUEST['data']) ){
		echo 'erro';
		die();
	}
	
	$sql = "SELECT date '".formata_data_sql($_REQUEST['data'])."' - date '".$_REQUEST['dataAtual']."' as dias";
	$dias = $db->pegaUm($sql);
	
	echo $dias;
	die();
}

if( $_POST['requisicao'] == 'recusa' ){
	global $db;
	
	if( $_POST['dprparecer'] && $_POST['dopid'] ){
		
		$teste = enviaEmailRecusaReprogramacao($_POST['dopid'], 'prazo', $_POST['dprparecer'] );

		$sql = "UPDATE par.documentoparreprogramacao SET dprstatus = 'I', dprdatavalidacao = 'NOW()', dprvalidacao = 't', dprparecer = '".$_POST['dprparecer']."', dprcpfvalidacao = '".$_SESSION['usucpf']."', dprtipovalidacao = 'R' WHERE dopid = ".$_POST['dopid']." AND dprstatus = 'P'";
		$db->executar($sql);
		$db->commit();
		
		if( $teste ){
			echo '<script>alert("Solicitação recusada com sucesso! O e-mail foi enviado ao dirigente."); window.opener.location.reload(); window.close();</script>';
		} else { 
			echo '<script>alert("Solicitação recusada com sucesso!"); window.opener.location.reload(); window.close();</script>';
		}
		exit;
	}
	
}
if( $_POST['requisicao'] == 'salva' ){
	global $db;
	
	//ver($_POST, d);
	set_time_limit(0);
	ini_set("memory_limit", "4000M");
	
	if( $_POST['dprdataprazoaprovado'] && $_POST['dias'] && $_POST['dprparecer'] ){
		
		if( substr($_POST['dprdataprazoaprovado'], 0, 2) == '02' ){
			$var = '28';
		} else {
			$var = '30';
		}
		
		$data = $var."/".$_POST['dprdataprazoaprovado'];
		
		$sql = "UPDATE par.documentoparreprogramacao SET 
					dprqtddiasaprovado = ".$_POST['dias'].", dprdatavalidacao = 'NOW()', --dprstatus = 'A', 
					dprdataprazoaprovado = '".formata_data_sql($data)."', dprvalidacao = 't', 
					dprparecer = '".$_POST['dprparecer']."' 
                                        , dprcpfvalidacao = '".$_SESSION['usucpf']."', dprtipovalidacao = 'A'
				WHERE 
					dopid = ".$_POST['dopid']." AND dprstatus = 'P'";
		$db->executar($sql);
		$db->commit();
		
		echo '<script>alert("Dados salvos com sucesso!"); window.opener.location.reload(); window.close();</script>';
		exit;
	}
}
if( $_POST['requisicao'] == 'cancelarReprogramacao' ){
	global $db;
	
	$sql = "UPDATE par.documentoparreprogramacao SET dprstatus = 'I' WHERE dopid = {$_POST['dopid']}";
	
	$db->executar( $sql );
	$db->commit();
	
	echo "
		<script>
			alert('Reprogramação cancelada');
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
	
			mes = data.substring(0, 2);

			if( mes > 12 ){
				alert('O mês '+mes+' não existe no calendário gregoriano!');
        		$('dprdataprazoaprovado').value = '';
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
				var myajax = new Ajax.Request('par.php?modulo=principal/popupReprogramacaoPARAprovacao&acao=A', {
				        method:     'post',
				        parameters: '&calculaDiasPAR=true&data='+data+'&dataAtual='+dataAtual,
				        asynchronous: false,
				        onComplete: function (res){
				        	if( res.responseText.trim() == 'erro' ){
				        		alert('A data solicitada deve ser maior que o fim da vigência atual.');
				        		$('dprdataprazoaprovado').value = '';
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
			if(!$('dprdataprazoaprovado').value){
				alert('Informe uma data!');
				return false;
			}
			if( !$('dprparecer').value ){
				alert('Informe um Parecer!');
				return false;
			}
			$('requisicao').value = 'salva';		
			$('form_prorrogacao').submit();		
		}
		function cancelarReprogramacao(){
			$('requisicao').value = 'cancelarReprogramacao';		
			$('form_prorrogacao').submit();		
		}
		function recusar(){
			if(confirm("Tem certeza que deseja recusar a solicitação?")){
				if( !$('dprparecer').value ){
					alert('Informe um Parecer!');
					return false;
				}
				$('requisicao').value = 'recusa';		
				$('form_prorrogacao').submit();
			}		
		}
		function atenderSolicitacao(){
			data = $('solic').value;
			$('dprdataprazoaprovado').value = data.substring(3, 10);
			calculaDias(data.substring(3, 10));
		}
	</script>
<?php
global $db;

$sql = "select 
			dopdatafimvigencia as prazo
		from 
			par.documentopar
		where 
			dopid = ".$_REQUEST['dopid'];

$prazo = $db->pegaUm($sql);

if( substr($prazo, 0, 2) == '02' ){
	$var = '28';
} else {
	$var = '30';
}

$dados = $db->pegaLinha("SELECT * FROM par.documentoparreprogramacao WHERE dprstatus = 'P' AND dopid = ".$_REQUEST['dopid']);
$dados = ($dados ? $dados : array());
extract($dados);

$disabled = 'N';
$readonly = 'readonly="readonly"';

$sql = "SELECT DISTINCT
			TRUE
		FROM
			par.processopar prp
		INNER JOIN par.vm_documentopar_ativos			dop ON dop.prpid = prp.prpid
		INNER JOIN par.modelosdocumentos  				mdo ON mdo.mdoid = dop.mdoid
		INNER JOIN par.documentoparreprogramacaosubacao	dpr ON dpr.dopid = dop.dopid
		WHERE
			dopreformulacao IS TRUE
			AND dpr.dpsstatus = 'A'
			AND prp.prpstatus = 'A'
			AND dop.dopid = {$_REQUEST['dopid']}";

$emReformulacao = $db->pegaUm($sql);

if( $emReformulacao == 't' ){

	?>
	<table align="center" cellspacing="0" cellpadding="3" border="0" bgcolor="#DCDCDC" style="border-top: none; border-bottom: none;" class="tabela">
		<tr>
			<td align="center" bgcolor="#e9e9e9" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')">
				<label style="color:red;font-size:12px;">
					<b>Favor finalizar a reprogramação de subação ativa para poder prosseguir com a reprogramação de prazo<br><br>ou<br><br></b>
				</label>
				<input type=button onclick="cancelarReprogramacao()" value="Cancelar Reformulação de Prazo">
			</td>
		</tr>
	</table>
	<div style="position:absolute;width:95%;">
		<div style="position:relative;float:right;">
			<img src="../imagens/par/carimbo-reformulacao.png" border="0">
		</div>
	</div>
	<?php 
}else{
	$disabled = 'S';
	$readonly = '';
}

?>
<input type="hidden" id="dataAtual" 	name="dataAtual" 	value="<?php echo formata_data_sql($var.'/'.$prazo); ?>" >
<input type="hidden" id="dopid" 		name="dopid" 		value="<?php echo $_REQUEST['dopid']; ?>" >
<input type="hidden" id="solic" 		name="solic" 		value="<?php echo formata_data($dprdataprazo); ?>" >
<input type="hidden" id="requisicao" 	name="requisicao" 	value="" >
<input type="hidden" id="dias" 			name="dias" 		value="" >
<table align="center" cellspacing="0" cellpadding="3" border="0" bgcolor="#DCDCDC" style="border-top: none; border-bottom: none;" class="tabela">
	<tr>
		<td align="center" bgcolor="#e9e9e9" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')">
			<b>Reprogramação</b>
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
			<?php echo substr(formata_data($dprdataprazo), 3, 7); ?>&nbsp;&nbsp;&nbsp;Dias: <input type="text" id="qtddiashd" name="qtddiashd" value="<?php echo $dprqtddiassolicitado; ?>" disabled> <a onclick="atenderSolicitacao()">Atender Solicitação</a>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">
			Data final do prazo aprovado:
		</td>
		<td>
			<?php echo campo_texto('dprdataprazoaprovado', 'S', $disabled,'Data final do prazo requisitado', 10, 7, '##/####', '', '', '', '', 'id="dprdataprazoaprovado"', '', '', 'calculaDias(this.value)'); ?> 
			<b>(mm/aaaa)</b> &nbsp;Dias: <input type="text" id="qtddias" name="qtddias" disabled>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">
			Justificativa:
		</td>
		<td>
			<?php echo $dprjustificativa; ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">
			Parecer:
		</td>
		<td>
			<?php echo campo_textarea('dprparecer', 'S', $disabled, 'Justificativa', 60, 10, 500); ?>
		</td>
	</tr>
	<?php if( $disabled == 'S' ){?>
	<tr align="center">
		<td style="BACKGROUND-COLOR: #dcdcdc" colspan="2"><input type="button" onclick="salvar()" value="salvar">&nbsp;<input type="button" onclick="recusar()" value="Recusar Solicitação"></td>
	</tr>
	<?php }?>
</table>
</form>