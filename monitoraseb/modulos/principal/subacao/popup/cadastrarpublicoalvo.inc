<?php

if($_REQUEST['requisicaoAjax']){
	header('content-type: text/html; charset=ISO-8859-1');
	$_REQUEST['requisicaoAjax']();
	exit;
}

include APPRAIZ."monitoraseb/classes/Subacao.class.inc";
$subacao = new Subacao();

if($_REQUEST['requisicao']=='salvar'){
	/*$mensagem = $categoria->salvarCategoria($_REQUEST[catid],
									$_REQUEST[catnome],
									$_REQUEST[catvalidacao], 
									$_REQUEST[catbolsista], 
									$_REQUEST[catnumdoclegal], 
									$_REQUEST[catvalunibolsa]);
		
	$categoria->commit();*/
	
	if($mensagem != ""){?>
		<script>
			self.close();
			alert('<?=$mensagem?>');
		</script>
	<?php }
}

?>
<html>
	<head>
		<title>Público alvo do Curso Mestre</title>
	</head>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
	<script language="JavaScript" src="../includes/funcoes.js"></script>

	<script type="text/javascript">

		function gravar(){
			if(validaFormulario()){
				$('#formularioCadastro').submit();
			}
		}
		
		function validaFormulario(){
			var erro = 0;
			$("[class~=obrigatorio]").each(function() { 
				if(!this.value){
					erro = 1;
					alert('Favor preencher todos os campos obrigatórios!');
					this.focus();
					return false;
				}
			});
			if(erro == 0){
				return true;
			}
		}
		
	</script>
	<body>
		<form method="POST"  name="formularioCadastro" action="" id="formularioCadastro">
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="99%" >
				<tr>
					<th class="TituloTela" colspan="2">Público alvo do Curso Mestre</th>
				</tr>
				<tr>
					<td width="25%" class="SubtituloDireita">Nome:</td>
					<td>
						<?php $subacao ? '' : ""; ?>
						<?=campo_texto('nome','S','S','Nome',30,30,'','');?>
					</td>
				</tr>
				<tr>
					<td width="25%" class="SubtituloDireita">Descrição:</td>
					<td>
						<?php $categoria ? '' : ""; ?>
						<?=campo_texto('descricao','S','S','Descrição',30,30,'','');?>
					</td>
				</tr>
				<tr>
					<th></th>
					<th style="text-align: left;">
						<input type="hidden" name="catid" value="<?=$catid?>"/>
						<input type="hidden" name="requisicao" value="salvar" />
						<input style="cursor: pointer;"  type="button" value="Salvar" name="btnGravar" onclick="gravar();">
						<input style="cursor: pointer;"  type="button" value="Cancelar" name="btnCancelar" onclick="self.close();">
					</th>
				</tr>
			</table>
		</form>
		<?php if($msg): ?>
			
		<?php endif; ?>
	</body>
</html>

