<?php
// empreendimento || obra || orgao
verificaSessao( 'obra' );

$regAtividade = new RegistroAtividade( $_REQUEST['rgaid'] );

switch ( $_POST['requisicao'] ){
    case "download":
		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
		$arqid = $_REQUEST['arqid'];
		$file = new FilesSimec();
	    $arquivo = $file->getDownloadArquivo($arqid);
		die();
}

extract( $regAtividade->getDados() );

$arquivo = new Arquivo( $arqid );
?>
<html>
<head>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
	<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	<script type="text/javascript" src="../includes/funcoes.js"></script>
	<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
</head>
<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0">
<?php
?>
<!--Formulário do Popup-->
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
        <tr>
            <td class="SubTituloDireita" width="190px">Tipo do registro:</td>
            <td>
                <?php
                $tipos = $db->carregar('SELECT traid as codigo, tranome as descricao FROM obras2.tiporegistroatividade');
                $db->monta_combo("traid", $tipos, "N", "", "", "", "", "", "N", "traid");
                ?>
            </td>
        </tr>
		<tr>
			<td class="SubTituloDireita" width="190px">Descrição simplificada:</td>
			<td>
			<?php
			$rgadscsimplificada = trim( $rgadscsimplificada ); 
			echo campo_texto( 'rgadscsimplificada', 'N', 'N', '', 47 , 60, '', '');
			?> 
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Arquivo:</td>
			<td>
				<br>
				<?php
				if ( $arquivo->arqid ){
					echo "<a href='javascript:DownloadArquivo(" . $arquivo->arqid . ");'>(" . $arquivo->arqnome . "." . $arquivo->arqextensao . ")</a>";
				}
				?>
			</td>
		</tr>
		<tr>	
			<td class="SubTituloDireita" width="190px">Descrição detalhada: </td>
			<td>
			<?php
			$rgadsccompleta = trim( $rgadsccompleta );	
			echo campo_textarea( 'rgadsccompleta', 'N', 'N', '', 100, 20, 5000 );
			?>
			</td>
		</tr>
		<tr bgcolor="#C0C0C0">
			<td></td>
			<td>
			<div style="float: left;"> 
<?php 
	if ( $rgaautomatica != 't' ):
?>	
				<input type="hidden" name="requisicao" id="requisicao" value="salvar"/>
				<input type="submit" value="Salvar" style="cursor: pointer"/>
<?php
	endif;
?>				
				<input type="button" value="Fechar" style="cursor: pointer" onclick="window.close();">
			</div>
			</td>
		</tr>
	</table>
<script type="text/javascript">
<!--
DownloadArquivo = function(arqid){
	$('#requisicao').val('download');
	$('#arqid').val(arqid);
	$('#formulario').submit();
}
//-->
</script>
</body>
</html>


