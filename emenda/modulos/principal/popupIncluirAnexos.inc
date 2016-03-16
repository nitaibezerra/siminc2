<?php

?>
<script type="text/javascript" src="/includes/funcoes.js"></script>
<script type="text/javascript" src="/includes/prototype.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<form id="formulario" name="formulario" method="post" action="" enctype="multipart/form-data">
<input type="hidden" name="arqid" id="arqid" value="<?php echo $_GET['arqid'] ?>" />

<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4" style="border-bottom:none;">
	<tr>
		<td bgcolor="#dedfde"><b>Anexar Documentos</b></td>
	</tr>
	<tr>
		<td colspan="2">
			<table class="listagem" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
			    <tr>
				    <th>Arquivo</th>
				    <th>Descrição</th>
				    <th>Ação</th>
			    </tr>
			    <tr>
			        <td align="left">
			            <input type="file" name="arquivo" id="arquivo" />
			        </td>
			        <td align="left">
			        	<?
						if($_GET['arqid'] && empty($_GET['tipo']) ){
							$anxdsc = $db->pegaUm("select anxdsc from emenda.anexo where arqid = {$_GET['arqid']}");
						}
			        	echo campo_texto( 'anxdsc', 'N', $habilitado, 'Descrição Anexo', 100, 100, '', '','','','','id="anxdsc"'); ?>
			        </td>
			        <td align="center">
			        	<input type="button" name="botao" value="Salvar" onclick="validaForm();" <?=disabled( $estadoAtual ); ?> />
			        </td>
			    </tr>
			    <?php
			    $perfil = pegaPerfil($_SESSION['usucpf']);
			    
			    if($arDados) {
			    	$count = 1;
				    foreach($arDados as $dados){
				    	if( empty( $dados['anxtermoref'] ) ){
				    	?>
						<tr>		    
					        <td align="left">
					        	<?php echo $count.' - '; ?><a style="cursor: pointer; color: blue;" onclick="window.location='?modulo=principal/reformulacao&acao=A&download=S&arqid=<?php echo $dados['arqid'];?>'"><?php echo $dados['arquivo'];?></a>
					        </td>
					        <td align="left">
					        	<?php echo $dados['anxdsc']; ?>
					        </td>
					        <td align="center">
					        <? if(possuiPermissao( $estadoAtual ) ) {?>
					        	<img src="../imagens/alterar.gif" onClick="window.location.href='emenda.php?modulo=principal/reformulacao&acao=A&arqid=<?php echo $dados['arqid'];?>&tipo='" style="border:0; cursor:pointer;" title="Alterar Descrição Anexo">
					        	<img src="../imagens/excluir.gif" onClick="excluirAnexo('<?php echo $dados['arqid']; ?>');" style="border:0; cursor:pointer;" title="Excluir Documento Anexo">
					        <? } else { ?>
					        	<img src="../imagens/alterar_01.gif" style="border:0; cursor:pointer;" title="Alterar Descrição Anexo">
					        	<img src="../imagens/excluir_01.gif" style="border:0; cursor:pointer;" title="Excluir Documento Anexo">
					        <? } ?>
					        </td>
					   	</tr>
				    	<?php
				    		$count++;
						}
				    }
				}
			    ?>
			</table>
		</td>
	</tr>
</table>
</form>
<script>

</script>