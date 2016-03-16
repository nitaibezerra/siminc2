<?php

if( $_REQUEST['arqid_download'] )
{
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	
	$file = new FilesSimec("anexogeral", null, "pdeinterativo");
	
	$file->getDownloadArquivo($_REQUEST['arqid_download']);
}

if( $_REQUEST['submetido'] )
{
	if( $_REQUEST['arqid_excluir'] && $_REQUEST['arqid_excluir'] != '' )
	{
		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
		$file = new FilesSimec("anexogeral", null, "pdeinterativo");
	
		$db->executar("UPDATE pdeinterativo.anexogeral SET agestatus = 'I' WHERE arqid = ".$_REQUEST['arqid_excluir']);
		$db->executar("UPDATE public.arquivo SET arqstatus = 'I' WHERE arqid = ".$_REQUEST['arqid_excluir']);
		$file->excluiArquivoFisico($_REQUEST['arqid_excluir']);
	
		//salvarAbaResposta("primeiros_passos_passo_1");
		
		echo '<script>
				alert("Arquivo excluído com sucesso.");
			  </script>';
	}
	else
	{
		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
		
		$campos = array('tpdid' => $_POST['tpdid']);
		$file = new FilesSimec("anexogeral", $campos, "pdeinterativo");
		
		$arqdescricao = $_POST['arqdescricao'];
		
		if( $file->setUpload($arqdescricao, "arquivo", true) )
		{
			//salvarAbaResposta("anexos");
		}
		
		echo '<script>
				alert("Arquivo incluído com sucesso.");
			  </script>';
	}
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$db->cria_aba($abacod_tela,$url,'');
barraProgressoPDEInterativo();
print '<br/>';

monta_titulo( "Anexos", '&nbsp' );
$arrDados = recuperaDadosEscolaPorCPFDiretor($_SESSION['pdeinterativo_vars']['usucpfdiretor']);

?>

<link rel="stylesheet" type="text/css" href="../pdeinterativo/css/pdeinterativo.css"/>

<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script>

jQuery(document).ready(function($)
{
	$('#btSalvar').click(function()
	{
		if( $('#arquivo').val() == '' )
		{
			alert('O campo "Arquivo" deve ser preenchido.');
			$('#arquivo').focus();
			return;
		}
		if( $('#tpdid').val() == '' )
		{
			alert('O campo "Tipo" deve ser preenchido.');
			$('#tpdid').focus();
			return;
		}
		if( $('#arqdescricao').val() == '' )
		{
			alert('O campo "Descrição" deve ser preenchido.');
			$('#arqdescricao').focus();
			return;
		}
		
		$('#formAnexo').submit();
	});
});

function excluirDocumento(arqid)
{
	if( confirm("Deseja realmente excluir o documento anexado?") )
	{
		$('#arqid_excluir').val(arqid);
		$('#formAnexo').submit();
	}
}
</script>

<form name="formAnexo" id="formAnexo"  method="post" action="" enctype="multipart/form-data">
	<input type="hidden" name="submetido" value="1" />
	<input type="hidden" name="arqid_excluir" id="arqid_excluir" value="" />
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td width="25%" class="SubTituloDireita blue" >Orientações</td>
			<td class="SubTituloEsquerda blue normal" >Insira aqui os documentos referentes ao PDE.</td>
		</tr>
		<tr>
			<td width="25%" class="SubTituloDireita" >Arquivo:</td>
			<td>
				<input type="file" id="arquivo" name="arquivo" />
				<img border="0" src="/imagens/obrig.gif" />
			</td>
		</tr>
		<tr>
			<td width="25%" class="SubTituloDireita" >Tipo:</td>
			<td>
			<?php
			$sql = "SELECT
						tpdid as codigo,
						tpddsc as descricao
					FROM
						pdeinterativo.tipodocumento
					WHERE
						dpdstatus = 'A'
					ORDER BY
						tpddsc ASC";
			$db->monta_combo("tpdid", $sql, 'S', "Selecione...", '', '', '', '200', 'S', 'tpdid');
			?>
			</td>
		</tr>
		<tr>
			<td width="25%" class="SubTituloDireita" >Descrição:</td>
			<td>
				<?=campo_textarea( 'arqdescricao', 'S', 'S', '', '55', '6', '1500', '' , 0, '')?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" ></td>
			<td>
				<input type="button" id="btSalvar" value="Salvar">
			</td>
		</tr>
	</table>
</form>
<?php

$sql = "SELECT
			'<center><a href=\"#\" onclick=\"excluirDocumento('||ar.arqid||');\"><img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\"></a></center>' as acao,
			to_char(a.agedtinclusao,'DD/MM/YYYY'),
			t.tpddsc,
			'<a href=\"pdeinterativo.php?modulo=principal/anexos&acao=A&arqid_download=' || ar.arqid || '\">' || ar.arqnome || '</a>',
			ar.arqtamanho || ' kbs' as tamanho,
			ar.arqdescricao
		FROM
			pdeinterativo.anexogeral a
		INNER JOIN
			pdeinterativo.tipodocumento t ON t.tpdid = a.tpdid
		INNER JOIN
			public.arquivo ar ON ar.arqid = a.arqid
		WHERE
			a.agestatus = 'A'
		ORDER BY
			a.agedtinclusao DESC";
	
$cabecalho = array( "Excluir", 
					"Data Inclusão",
					"Tipo Arquivo",
					"Nome Arquivo",
					"Tamanho (Mb)",
					"Descrição Arquivo");
$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '' );

?>