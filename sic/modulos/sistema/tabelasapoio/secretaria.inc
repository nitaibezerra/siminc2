<?php

if($_REQUEST['requisicao'] == 'carregarSecretaria'){
	$sql = "select secid, secdsc, secemail from sic.secretaria where secid = {$_REQUEST['secid']}";	
	$rs = $db->pegaLinha($sql);
	$rs['secdsc'] = utf8_encode($rs['secdsc']);
	echo simec_json_encode($rs);
	die;
}

if($_REQUEST['requisicao'] == 'salvaDados'){
	
	if($_REQUEST['secid']){
		
		$sql = "update sic.secretaria set
					secdsc 		= '{$_REQUEST['secdsc']}',
					secemail 	= '{$_REQUEST['secemail']}'
				where 
					secid = {$_REQUEST['secid']};";
	}else{
		
		$sql = "insert into sic.secretaria
					(secdsc, secemail, secstatus)
				values
					('{$_REQUEST['secdsc']}', '{$_REQUEST['secemail']}', 'A');";
	}
	$db->executar($sql);
	$db->commit();
	$db->sucesso('sistema/tabelasapoio/secretaria');	
}

if($_REQUEST['requisicao'] == 'excluirSecretaria'){
	
	$sql = "update sic.secretaria set 
				secstatus = 'I' 
			where 
				secid = {$_REQUEST['secid']};";
	
	$db->executar($sql);
	$db->commit();
	$db->sucesso('sistema/tabelasapoio/secretaria');
}

include_once APPRAIZ . "includes/cabecalho.inc";
echo '<br/>';

$titulo2 = '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> Indica campo obrigatório.';
monta_titulo('Secretaria', $titulo2);

?>
<link rel="stylesheet" type="text/css" href="../includes/jquery-validate/css/validate.css" />
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/jquery.validate.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/localization/messages_ptbr.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/lib/jquery.metadata.js"></script>
<script>
	$(function(){
		
		jQuery("[name=secemail]").addClass("required email");
		jQuery("[name=secdsc]").addClass("required");
				
		jQuery("#formulario_id").validate();
		
		$('#bt_salvar').click(function(){	
			$('[name=requisicao]').val('salvaDados');					
			$('#formulario_id').submit();
		});
				
	});

	function editarSecretaria(secid)
	{
		$.ajax({
			url		 : 'sic.php?modulo=sistema/tabelasapoio/secretaria&acao=A',
			type	 : 'post',
			data	 : 'requisicao=carregarSecretaria&secid='+secid,
			dataType : 'json',
			success	 : function(e){
				
				if(e.secid)
					$('[name=secid]').val(e.secid);

				if(e.secdsc)
					$('[name=secdsc]').val(e.secdsc);

				if(e.secemail)
					$('[name=secemail]').val(e.secemail);
					
			}
		});
	}

	function excluirSecretaria(secid)
	{
		if(confirm('Deseja excluir a secretaria?')){
//			$.ajax({
//				url		: '',
//				type	: 'post',
//				data	: 'requisicao=excluirSecretaria&secid='+secid,
//				success	: function(e){
//				}
//			});

			document.location.href = 'sic.php?modulo=sistema/tabelasapoio/secretaria&acao=A&requisicao=excluirSecretaria&secid='+secid; 
		}
	}
</script>
<form name="formulario" id="formulario_id" method="post" action="">
	<table class="tabela" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 >
		<input type="hidden" name="requisicao" value="" />
		<input type="hidden" name="secid" value="" />
		<tr>
			<td class="subtituloDireita">Nome</td>
			<td>
				<?php echo campo_texto('secdsc', 'S', 'S', '', 70, 255, '', ''); ?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">E-mail</td>
			<td><?php echo campo_texto('secemail', 'S', 'S', '', 70, 255, '', ''); ?></td>
		</tr>
		<tr>
			<td class="subtituloDireita">&nbsp;</td>
			<td class="subtituloEsquerda">
				<input type="button" value="Salvar" id="bt_salvar" class="teste">
			</td>
		</tr>
	</table>
</form>
<?php 
$sql = "select
			'<img src=\"../imagens/alterar.gif\" onclick=\"editarSecretaria(' || secid || ')\" style=\"cursor:pointer\" />
			 <img src=\"../imagens/excluir.gif\" onclick=\"excluirSecretaria(' || secid || ')\" style=\"cursor:pointer\" />' as acao, 
			secid, 
			secdsc, 
			secemail 
		from 
			sic.secretaria 
		where
			secstatus = 'A'
		order by 
			secdsc";

$cabecalho = array('Ação','ID','Nome','E-mail');
$db->monta_lista($sql, $cabecalho, 10, 10, 'N', '', '', '', '');
?>