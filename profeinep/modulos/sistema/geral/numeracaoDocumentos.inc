<?php
if( $_REQUEST['req'] == 'gerarNumeracao' ) {
	if( $_REQUEST['tpdid'] != '' ){
		$sql = "SELECT 
					max(nudnumero)
	  			FROM 
	  				profeinep.numeracaodocumento
	  			WHERE
	  				tpdid = ".$_REQUEST['tpdid']."
	  				AND nudstatus = 'A'";
		$num = $db->pegaUm($sql);
		echo $num+1;
	}
	die();
}
 
if( $_REQUEST['req'] == 'gravarNumeracao' ) {
	if( $_REQUEST['tpdid'] != '' ){
		$sql = "UPDATE profeinep.numeracaodocumento
				SET
					nudstatus = 'I'
				WHERE
					tpdid = {$_REQUEST['tpdid']}
					AND nudnumero >= {$_REQUEST['nudnumero']}";
		$db->executar($sql);
		$sql = "INSERT INTO profeinep.numeracaodocumento(
            				tpdid, nudnumero, nudusucpf, nuddatageracao, nudano)
	    		VALUES ({$_REQUEST['tpdid']}, ".($_REQUEST['nudnumero']-1).", {$_SESSION['usucpf']}, now(), ".Date('Y').")";
		$db->executar($sql);
		$db->commit();
		$db->sucesso('sistema/geral/numeracaoDocumentos');
	}
}
include  APPRAIZ."includes/cabecalho.inc";
echo "<br />";
monta_titulo( 'PROFE/INEP', 'Administração de Numeração de Documentos' ); 
?>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/prototype.js"></script>
<form action="" method="POST" name="formulario" id="formulario">
	<input type='hidden' name="req" id="req" value="">
	<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td class="SubTituloDireita">Tipo de documento:</td>
			<td><?php
				$sql = "SELECT 
						 	tipodoc.tpdid  as codigo,
							tipodoc.tpddsc as descricao  
						FROM
							profeinep.tipodocumento as tipodoc
						WHERE
							tipodoc.tpdstatus = 'A'
							AND tipodoc.tpdgeranumeracao = 'S'
						ORDER BY 
							tipodoc.tpddsc ASC";
				
				$db->monta_combo('tpdid', $sql, 'S', "Selecione...", 'gerarNumeracao', '', '', '160', 'S', 'tpdid');
				?>
				<?=campo_texto('nudnumero','N','S','',10,10,'','','','','','id="nudnumero"') ?> / <?=Date('Y'); ?>
				<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita"></td>
			<td>
				<input type="button" value="Gravar Nova Numeração" onclick="validaForm()"/>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita"></td>
			<td>
				<?php 
				
				$sql1 = "SELECT 
							tpddsc, 
							max(nudnumero)
						FROM 
							profeinep.numeracaodocumento nd
						INNER JOIN
							profeinep.tipodocumento td ON td.tpdid = nd.tpdid
						WHERE
							nd.nudstatus = 'A'
							AND td.tpdgeranumeracao = 'S'
						GROUP BY
							tpddsc";
				$cabecalho = Array('Tipo de Documento','Numeração do Documento');
				$celWidth  = Array('80%','20%');
				$celAlign  = Array('Left','Center');
				$db->monta_lista($sql1,$cabecalho,20,10,'','','N','formulario',$celWidth,$celAlign);
				?>
			</td>
		</tr>
	</table>
</form>
<script>

function gerarNumeracao(tpdid){
	var num   = $('nudnumero');
	new Ajax.Request(window.location.href,{
		method: 'post',
		asynchronous: false,
		parameters: '&req=gerarNumeracao&tpdid='+tpdid,
		onComplete: function(res){
			num.value = res.responseText;
		}
	});	
}

function validaForm(){
	var campo = new Array();
	var form = $('formulario');
	campo[0] = $('tpdid');
	campo[1] = $('nudnumero');
	campo[2] = $('botaoNum');

	for( i=0; i<=1; i++ ){
		if( campo[i].value == '' ){
			if( i == 3 ){
				alert('Crie um número de documento.');
				campo[2].focus();
				return false;
			}else{
				alert('Campo obrigatório');
				campo[i].focus();
				return false;
			}
		}
	}
	var req = $('req');
	req.value = 'gravarNumeracao';
	form.submit();
}

</script>