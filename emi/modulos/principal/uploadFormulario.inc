<?php

if( $_REQUEST["requisicao"] == "downloadArquivo" ){
	$emi->DownloadArquivo( $_REQUEST );
}

if( $_REQUEST["requisicao"] == "removerFormulario" ){
	$emi->removeFormulario( $_REQUEST );
}

if( $_REQUEST["requisicao"] == "inserirarquivo" ){
	$emi->enviaFormulario( $_REQUEST );
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";

$db->cria_aba($abacod_tela,$url,$parametros);

if ( $_REQUEST["acao"] == "A" ){

	if ( $_REQUEST["emeid"] || $_SESSION["emi"]["emeidPai"] ){
	
		$_SESSION["emi"]["emeidPai"] = !empty($_REQUEST["emeid"]) ? $_REQUEST["emeid"] : $_SESSION["emi"]["emeidPai"];
		$emeid = $_SESSION["emi"]["emeidPai"];
		
		if ( !emiVerificaEntidade( $emeid ) ){
			
			unset( $_SESSION["emi"]["emeidPai"] );
			
			print "<script>
					  alert('A secretaria selecionada não existe ou está em branco!');
					  history.back(-1);
				  </script>";
			die;
		}
		
	}
	
	monta_titulo("Upload do Formulário da Secretaria", "Envio do formulário para secretaria do estado");
	
	if( $_SESSION["emi"]["emeidPai"] ){
		$emi->montaCabecalho( $_SESSION["emi"]["emeidPai"], "secretaria" );
	}
	
}else if( $_REQUEST["acao"] == "C" ){

	if ( $_REQUEST["emeid"] || $_SESSION["emi"]["emeidEscola"] ){
	
		$_SESSION["emi"]["emeidEscola"] = !empty($_REQUEST["emeid"]) ? $_REQUEST["emeid"] : $_SESSION["emi"]["emeidEscola"];
		$emeid = $_SESSION["emi"]["emeidEscola"];
		
		if ( !emiVerificaEntidade( $emeid ) ){
			
			unset( $_SESSION["emi"]["emeidEscola"] );
			
			print "<script>
					  alert('A escola selecionada não existe ou está em branco!');
					  history.back(-1);
				  </script>";
			die;
		}
		
	}
	
	monta_titulo("Upload do Formulário da Escola", "Envio do formulário para secretaria do estado");
	
	if( $_SESSION["emi"]["emeidEscola"] ){
		$emi->montaCabecalho( $_SESSION["emi"]["emeidEscola"], "escola" );
	}
	
}

if(!$emeid) {
	die("<script>
			alert('Problemas com variáveis. Refaça o procedimento.');
			window.location='emi.php?modulo=inicio&acao=C';
		  </script>");
}

$sql = "SELECT ar.arqid, ar.arqnome, ar.arqextensao, ar.arqdescricao FROM emi.detalheentidade em 
		LEFT JOIN public.arquivo ar ON ar.arqid=em.arqid 
		WHERE em.emeid='".$emeid."' and dettipo = 'P'";

$ementidade = $db->pegaLinha($sql);

$arqdescricao = $ementidade['arqdescricao']; 

?>
<script>
function removerUpload() {
	var conf = confirm("Deseja remover arquivo?");
	if(conf) {
		window.location='emi.php?modulo=principal/uploadFormulario&acao=<?=$_REQUEST["acao"];?>&requisicao=removerFormulario';
	}
}
</script>
<form method="post" id="emiUploadFormulario" enctype="multipart/form-data" action="emi.php?modulo=principal/uploadFormulario&acao=A">
	<input type="hidden" name="requisicao" value="inserirarquivo"/>
	<input type="hidden" name="emeid" value="<?=$emeid;?>"/>
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
		<tr>
			<td width="190px;" class="SubTituloDireita">Arquivo:</td>
			<td>
			<? 
			if($ementidade['arqid']) {
				 
				echo "<a href='emi.php?modulo=principal/uploadFormulario&acao=A&requisicao=downloadArquivo&arqid=".$ementidade['arqid']."'>".$ementidade['arqnome'].".".$ementidade['arqextensao']."</a>";
				
			} else {
				
				echo "<input type=\"file\" name=\"arquivo\" id=\"arquivo\" {$emiDisabled}/>
					  <img border=\"0\" title=\"Indica campo obrigatório.\" src=\"../imagens/obrig.gif\"/>";
				
			} 
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Descrição:</td>
			<td>
			<? 
			if($ementidade['arqid']) {
				 
				echo $ementidade['arqdescricao'];
				
			} else {
				
				echo campo_textarea( 'arqdescricao', 'S', $emiSomenteLeitura, '', 60, 2, 250 );
				
			} 
			?>
			</td>
		</tr>
		<tr style="background-color: #cccccc">
			<td></td>
			<td>
			<? 
			if($ementidade['arqid']) {
				 
				echo "<input type=\"button\" value=\"Remover\" onclick=\"removerUpload();\" {$emiDisabled}/>";
				
			} else {
				
				echo "<input type=\"button\" value=\"Salvar\" onclick=\"validaUpload();\" {$emiDisabled}/>";
				
			} 
			?>
			<input type="button" value="Voltar" onclick="window.location='emi.php?modulo=principal/arvoreSecretaria&acao=A';"/>
			</td>
		</tr>
	</table>
</form>