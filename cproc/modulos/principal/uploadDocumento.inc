<?php

// dependências --------------------------------------------------------------------------------------------------
include APPRAIZ . 'includes/classes/Modelo.class.inc';
include_once APPRAIZ . "cproc/classes/ModeloCproc.class.inc";
include_once APPRAIZ . "cproc/classes/Processo.class.inc";
include_once APPRAIZ . "cproc/classes/WorkflowCproc.class.inc";
// --------------------------------------------------------------------------------------------------

include APPRAIZ . "includes/cabecalho.inc";


$titulo_modulo = "Controle de Processos - Upload de Documento";
$modulo = "CPROC";
monta_titulo($titulo_modulo, $modulo);


// valida presenca de prcid
if( $_SESSION['cproc']['prcid'] )
	$prcid = $_SESSION['cproc']['prcid'];
else{
	echo "<script>
		alert('Processo não identificado.');
		window.location.href = 'cproc.php?modulo=principal/lista_grid_processo&acao=A';
	</script>";
	exit;
}


// Processo --------------------------------------
if( $prcid )
    $Processo = new Processo( $prcid );
//ver($_FILES, d);
if( $_FILES && $_FILES['arquivo']['error'] == 0 ){
	if( WorkflowCproc::posAcaoAnexarDocumento() )
		echo "<script>
			alert('Documento enviado e registrado com sucesso!');
			window.location.href = 'cproc.php?modulo=principal/cad_dados_processo&acao=A';
		</script>";
	else
		echo "<script>
			alert('Houve um erro ao enviar o Documento.');
			window.location.href = 'cproc.php?modulo=principal/cad_dados_processo&acao=A';
		</script>";
	exit;
} else {
	switch ($_FILES['arquivo']['error']){
		case 1:
			echo "<script>
					alert('O arquivo enviado é muito grande.');
					window.location.href = 'cproc.php?modulo=principal/uploadMinuta&acao=A';
				  </script>";
		case 2:
			echo "<script>
				alert('O arquivo enviado é muito grande.');
				window.location.href = 'cproc.php?modulo=principal/uploadMinuta&acao=A';
			  </script>";
		case 3:
			echo "<script>
					alert('O upload do arquivo foi feito parcialmente.');
					window.location.href = 'cproc.php?modulo=principal/uploadMinuta&acao=A';
				  </script>";
		case 4:
			echo "<script>
					alert('Informe o arquivo.');
					window.location.href = 'cproc.php?modulo=principal/uploadMinuta&acao=A';
				  </script>";
					
	}
}

?>

<form method="POST" enctype="multipart/form-data">
<input type="hidden" name="prcid" value="<?= $prcid ?>"/>
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" border="0">
    <tr>
        <td class="subtituloDireita" style="width:20%;">Arquivo:</td>
        <td>
            <input type="file" name="arquivo" id="arquivo" /> 
            <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif" />
        </td>
    </tr>
    <tr>
        <td class="subtituloDireita" style="width:20%;">Tipo Documento:</td>
        <td>
            <?php 
                $sql = " SELECT 
                            tdpid AS codigo, 
                            TRIM(tdpdsc) AS descricao
                         FROM cproc.tipodocprocesso
                ";
                $db->monta_combo('tdpid', $sql, 'S', 'Selecione...', '', '', '', '455', 'S', '', '','','Tipo de Documento');
            ?>
        </td>
    </tr>
    <tr>
    	<td colspan="2" style="text-align:center">
    		<input type="submit" value="Cadastrar Documento"/>
    	</td>
    </tr>
</table>
</form>

