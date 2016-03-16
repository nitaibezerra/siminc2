<?php
$perfil = pegaArrayPerfil($_SESSION['usucpf']);

function carregarDados(){
	
	global $db;
	
	$sql = "SELECT
				prp.prpid,
				prp.prpbanco,
				prp.prpagencia,
				prp.prpseqconta,
				prp.seq_conta_corrente,
				prp.nu_conta_corrente,
				prp.prptipoexecucao,
				prp.prpnumeroconveniofnde,
				prp.prpanoconveniofnde,
				prp.prpnumeroconveniosiafi,
				prp.prpdocumenta
			FROM 
				par.processopar prp
			WHERE
				prp.prpstatus = 'A'
				and
				prpid = ".$_REQUEST['prpid'];
	
	$dados = $db->pegaLinha( $sql );
	
	echo $dados['prpid']."|".$dados['prpbanco']."|".$dados['prpagencia']."|".$dados['prpseqconta']."|".$dados['seq_conta_corrente']."|".$dados['nu_conta_corrente']."|".$dados['prptipoexecucao']."|".$dados['prpnumeroconveniofnde']."|".$dados['prpanoconveniofnde']."|".$dados['prpnumeroconveniosiafi']."|".$dados['prpdocumenta'];
	
}

if($_REQUEST['requisicao'] == "salvarDados"){

	$anofnde = $_REQUEST['prpanoconveniofnde'] ? $_REQUEST['prpanoconveniofnde'] : 'NULL';
	
	$sql = "UPDATE 
				par.processopar
			SET 
				prpbanco = '".$_REQUEST['prpbanco']."',
				prpagencia = '".$_REQUEST['prpagencia']."',
				prpseqconta = '".$_REQUEST['prpseqconta']."',
				seq_conta_corrente = '".$_REQUEST['seq_conta_corrente']."',
				nu_conta_corrente = '".$_REQUEST['nu_conta_corrente']."',
				prptipoexecucao = '".$_REQUEST['prptipoexecucao']."',
				prpnumeroconveniofnde = '".$_REQUEST['prpnumeroconveniofnde']."',
				prpanoconveniofnde = ".$anofnde.",
				prpnumeroconveniosiafi = '".$_REQUEST['prpnumeroconveniosiafi']."',
				prpdocumenta = '".$_REQUEST['prpdocumenta']."',
				usucpf = '".$_SESSION['usucpf']."',
				prpdatainclusao = 'NOW()'
			WHERE 
				prpid =".$_REQUEST['prpid'];
	$db->executar($sql);
	
	$sql = "INSERT INTO par.historicoprocessopar(
		            prpid, usucpf, hppdata)
		    VALUES (".$_REQUEST['prpid'].", '".$_SESSION['usucpf']."', 'NOW()');";
	$db->executar($sql);
			
	$db->commit();
	alert('Dados atualizados com sucesso.');
	
}

if($_POST['requisicaoAjax']){
	header('content-type: text/html; charset=ISO-8859-1');
	$_POST['requisicaoAjax']();
	die;
}

/***************** DECLARAÇÃO DE VARIAVEIS & INSTANCIA OBJETOS ***************/
include APPRAIZ . 'includes/cabecalho.inc';
print '<br />';
monta_titulo( 'Processo PAR', '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.'  );
?>
<html>
<head>
	<script type="text/javascript" src="../includes/funcoes.js"></script>
	<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
</head>
<body>
<form name="formularioLista" id="formularioLista" enctype="multipart/form-data"  method="post" action="" >
	<input type="hidden" name="requisicao" value="" />
	<input type="hidden" name="proid" value="<?php echo $proid ?>" />
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
		<tr>
			<th colspan="2" > </th>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Banco:</td>
			<td colspan="2"><input type="hidden" name="prpid" id="prpid" value="">
				<?php echo campo_texto("prpbanco","S","S","prpbanco","30","",'',"","","","","","",""); ?>
			</td> 
		</tr>
		<tr>
			<td class="SubTituloDireita" >Agencia:</td>
			<td colspan="2">
				<?php echo campo_texto("prpagencia","S","S","prpagencia","30","",'[#]',"","","","","","",""); ?>
			</td> 
		</tr>
		<tr>
			<td class="SubTituloDireita" >Seqüencial:</td>
			<td colspan="2">
				<?php echo campo_texto("prpseqconta","S","S","prpseqconta","30","",'[#]',"","","","","","",""); ?>
			</td> 
		</tr>
		<tr>
			<td class="SubTituloDireita" >seq_conta_corrente:</td>
			<td colspan="2">
				<?php echo campo_texto("seq_conta_corrente","S","S","seq_conta_corrente","30","",'[#]',"","","","","","",""); ?>
			</td> 
		</tr>
		<tr>
			<td class="SubTituloDireita" >nu_conta_corrente:</td>
			<td colspan="2">
				<?php echo campo_texto("nu_conta_corrente","S","S","nu_conta_corrente","30","",'',"","","","","","",""); ?>
			</td> 
		</tr>
		<tr id="trprpnumeroconveniofnde">
			<td class="SubTituloDireita" >prpnumeroconveniofnde:</td>
			<td colspan="2">
				<?php echo campo_texto("prpnumeroconveniofnde","N","S","prpnumeroconveniofnde","30","",'[#]',"","","","","","",""); ?>
			</td> 
		</tr>
		<tr id="trprpanoconveniofnde">
			<td class="SubTituloDireita" >prpanoconveniofnde:</td>
			<td colspan="2">
				<?php echo campo_texto("prpanoconveniofnde","N","S","prpanoconveniofnde","30","",'[#]',"","","","","","",""); ?>
			</td> 
		</tr>
		<tr id="trprpnumeroconveniosiafi">
			<td class="SubTituloDireita" >prpnumeroconveniosiafi:</td>
			<td colspan="2">
				<?php echo campo_texto("prpnumeroconveniosiafi","N","S","prpnumeroconveniosiafi","30","",'[#]',"","","","","","",""); ?>
			</td> 
		</tr>
		<tr>
			<td class="SubTituloDireita" >prpdocumenta:</td>
			<td colspan="2">
				<?php echo campo_texto("prpdocumenta","S","S","prpdocumenta","30","",'[#]',"","","","","","",""); ?>
			</td> 
		</tr>
		<tr>
			<td  class="SubTituloDireita"></td>
			<td><input type="button" id="btn_salvar" name="btn_salvar" value="Salvar" onclick="salvar();"  /></td>
		</tr>
	</table>
</form>
<div id="lista">
<?php
	$sql = "SELECT
				'<img style=\"cursor:pointer\" onclick=\"alteraProcesso( ' || prp.prpid || ' )\" src=\"/imagens/alterar.gif\" title=\"Altera Processo\" />' AS acao,
				prp.prpnumeroprocesso,
				mun.mundescricao, 
				prp.prpbanco,
				prp.prpagencia,
				prp.prpseqconta,
				prp.seq_conta_corrente,
				prp.nu_conta_corrente,
				prp.prpnumeroconveniofnde,
				prp.prpanoconveniofnde,
				prp.prpnumeroconveniosiafi,
				prp.prpdocumenta
			FROM 
				par.processopar prp
			INNER JOIN territorios.municipio mun ON mun.muncod = prp.muncod
			where prp.prpstatus = 'A'
			ORDER BY
				prp.muncod, prp.prpnumeroprocesso";

	$cabecalho = array("Ação","Processo","Município","Banco","prpagencia", "prpseqconta", "seq_conta_corrente", "nu_conta_corrente", "Nº convênio FNDE", "Ano convênio FNDE", "Nº convênio SIAFI", "prpdocumenta");
	
	$db->monta_lista($sql,$cabecalho,50,5,"N","center",100);
?>
</div>

<script type="text/javascript">
	function alteraProcesso(prpid){
		jQuery.ajax({
			type: "POST",
			url: window.location,
			data: "requisicaoAjax=carregarDados&prpid=" + prpid,
			success: function(res){
				dados = res;
		   	    dados = dados.split('|');
		   	    
				jQuery("[name=prpid]").val(dados[0]);
				jQuery("[name=prpbanco]").val(dados[1]);
				jQuery("[name=prpagencia]").val(dados[2]);
				jQuery("[name=prpseqconta]").val(dados[3]);
				jQuery("[name=seq_conta_corrente]").val(dados[4]);
				jQuery("[name=nu_conta_corrente]").val(dados[5]);
				if( dados[6] == 'T' ){
					jQuery("[name=prpnumeroconveniofnde]").val('');
					jQuery("[name=prpanoconveniofnde]").val('');
					jQuery("[name=prpnumeroconveniosiafi]").val('');
					jQuery("#trprpnumeroconveniofnde").attr("style", "display: none");
					jQuery("#trprpanoconveniofnde").attr("style", "display: none");
					jQuery("#trprpnumeroconveniosiafi").attr("style", "display: none");
				} else {
					jQuery("[name=prpnumeroconveniofnde]").val(dados[7]);
					jQuery("[name=prpanoconveniofnde]").val(dados[8]);
					jQuery("[name=prpnumeroconveniosiafi]").val(dados[9]);
					jQuery("#trprpnumeroconveniofnde").attr("style", "display: ''");
					jQuery("#trprpanoconveniofnde").attr("style", "display: ''");
					jQuery("#trprpnumeroconveniosiafi").attr("style", "display: ''");
				}
				jQuery("[name=prpdocumenta]").val(dados[10]);
				
			}
		});
	}
	
	function salvar(){
		var erro = 0;
		jQuery("[class~=obrigatorio]").each(function() { 
			if(this.value == ''){
				alert('Favor preencher todos os campos obrigatório');
				this.focus();
				erro = 1;
				return false;
			}
		});
	
		if(erro == 0){
			jQuery("[name=requisicao]").val('salvarDados');
			jQuery("#formularioLista").submit();
		}
	}
	
</script>
</body>
</html>