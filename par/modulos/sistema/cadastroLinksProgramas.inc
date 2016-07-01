<?php 

if( $_REQUEST['submetido'] )
{
	// Instrumento
	$itrid = ( $_REQUEST['itrid'] && $_REQUEST['itrid'] != '' ) ? $_REQUEST['itrid'] : 'NULL';
	// Função
	$lprfuncao = ( $_REQUEST['lprfuncao'] && $_REQUEST['lprfuncao'] != '' ) ? "'".$_REQUEST['lprfuncao']."'" : 'NULL';
	// Período
	if( $_REQUEST['lprdatainicio'] && $_REQUEST['lprdatainicio'] != '')
	{
		$lprdatainicio 	= explode("/", $_REQUEST['lprdatainicio']);
		$lprdatainicio 	= $lprdatainicio[2].'-'.$lprdatainicio[1].'-'.$lprdatainicio[0];
		$lprdatainicio	= "'".$lprdatainicio."'";
	}
	else
	{
		$lprdatainicio	= 'NULL';
	}
	
	if( $_REQUEST['lprdatafim'] && $_REQUEST['lprdatafim'] != '')
	{
		$lprdatafim	 	= explode("/", $_REQUEST['lprdatafim']);
		$lprdatafim 	= $lprdatafim[2].'-'.$lprdatafim[1].'-'.$lprdatafim[0];
		$lprdatafim		= "'".$lprdatafim."'";
	}
	else
	{
		$lprdatafim		= 'NULL';
	}
		
	if( $_REQUEST['lprid'] && $_REQUEST['lprid'] != '' )
	{
		$lprid = $_REQUEST['lprid'];
		
		if( $_FILES['arquivo'] )
		{
			$arqid = $db->pegaUm("SELECT a.arqid FROM par.linkprograma l INNER JOIN par.anexolinkprograma a ON a.alpid = l.alpid WHERE l.lprid = ".$lprid);
			
			include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
		
			$campos = array("alpstatus" => "'A'");
			$file = new FilesSimec("anexolinkprograma", $campos, "par");
			
			$db->executar("UPDATE par.anexolinkprograma SET alpstatus = 'I' WHERE arqid = ".$arqid);
			$db->executar("UPDATE public.arquivo SET arqstatus = 'I' WHERE arqid = ".$arqid);
			$file->excluiArquivoFisico($arqid);
			
			$arqdescricao = 'arquivo_anexolinkprograma';
			$file->setUpload($arqdescricao, "arquivo", true, "alpid");
			
			$alpid = ", alpid = ".$file->getCampoRetorno()." ";
		}
		
		$db->executar("DELETE FROM par.linkprogramaperfil WHERE lprid = ".$lprid);
		
		$sql = "UPDATE par.linkprograma 
				SET lprnome = '".pg_escape_string($_REQUEST['lprnome'])."', 
					lprlink = '".pg_escape_string($_REQUEST['lprlink'])."', 
					lprpopup = '".$_REQUEST['lprpopup']."', 
					itrid = ".$itrid.", 
					lprdatainicio = ".$lprdatainicio.", 
					lprdatafim = ".$lprdatafim.", 
					lprfuncao = ".$lprfuncao.",
					lprstatus = '".$_REQUEST['lprstatus']."'  
					".$alpid." 
				WHERE lprid = ".$lprid;
		$db->executar($sql);
	}
	else
	{
		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	
		$campos = array("alpstatus" => "'A'");
		$file = new FilesSimec("anexolinkprograma", $campos, "par");
		$arqdescricao = 'arquivo_anexolinkprograma';
		$file->setUpload($arqdescricao, "arquivo", true, "alpid");
		
		$sql = "INSERT INTO par.linkprograma(lprnome,lprlink,alpid,lprpopup,itrid,lprdatainicio,lprdatafim,lprfuncao,lprstatus) 
				VALUES('".pg_escape_string($_REQUEST['lprnome'])."', '".pg_escape_string($_REQUEST['lprlink'])."', ".$file->getCampoRetorno().", '".$_REQUEST['lprpopup']."', ".$itrid.", ".$lprdatainicio.", ".$lprdatafim.", ".$lprfuncao.", '".$_REQUEST['lprstatus']."')
				RETURNING lprid";
		$lprid = $db->pegaUm($sql);
	}
	
	if( $_REQUEST['pflcod'] && $_REQUEST['pflcod'][0] != '' )
	{
		foreach($_REQUEST['pflcod'] as $perfil)
		{
			$db->executar("INSERT INTO par.linkprogramaperfil(lprid,pflcod) VALUES(".$lprid.", ".$perfil.")");
		}
	}
	
	$db->commit();
	echo '<script>
			alert("Dados gravados com sucesso.");
			window.location.href = "par.php?modulo=sistema/listaLinksProgramas&acao=A";
		  </script>';
	die;
}

include APPRAIZ . 'includes/cabecalho.inc';
echo "<br/>";

$menu = array(
0 => array("id" => 1, "descricao" => "Lista", "link" => "/par/par.php?modulo=sistema/listaLinksProgramas&acao=A"),
1 => array("id" => 2, "descricao" => "Cadastro", "link" => "/par/par.php?modulo=sistema/cadastroLinksProgramas&acao=A")
);

$abaAtiva = "/par/par.php?modulo=sistema/cadastroLinksProgramas&acao=A";

echo montarAbasArray($menu, $abaAtiva);

echo monta_titulo("PAR 2010", "Cadastro de Link de Programa");

if( $_GET['lprid'] )
{
	$sql 	= "SELECT * FROM par.linkprograma WHERE lprid = ".$_GET['lprid'];
	$dados	= $db->carregar($sql);
	
	extract($dados[0]);
	
	if( $lprdatainicio )
	{
		$lprdatainicio	= explode('-', $lprdatainicio);
		$lprdatainicio	= $lprdatainicio[2].'/'.$lprdatainicio[1].'/'.$lprdatainicio[0];
	}
	if( $lprdatafim )
	{
		$lprdatafim		= explode('-', $lprdatafim);
		$lprdatafim		= $lprdatafim[2].'/'.$lprdatafim[1].'/'.$lprdatafim[0];
	}
}

?>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<script type="text/javascript">

$(document).ready(function()
{
	$('#btSalvar').click(function()
	{
		if( $('#lprnome').val() == '' )
		{
			alert("O campo 'Nome' deve ser preenchido.");
			$('#lprnome').focus();
			return;
		}
		if( $('#lprlink').val() == '' )
		{
			alert("O campo 'Link' deve ser preenchido.");
			$('#lprlink').focus();
			return;
		}
		if( $('#arquivo').length > 0 )
		{
			if( $('#arquivo').val() == '' )
			{
				alert("O campo 'Ícone' deve ser preenchido.");
				$('#arquivo').focus();
				return;
			}
		}
		if( ($('#lprdatainicio').val() != '' && $('#lprdatafim').val() == '') || ($('#lprdatainicio').val() == '' && $('#lprdatafim').val() != '') )
		{
			alert('Deve-se informar as duas datas no campo "Período"');
			$('#lprdatainicio').focus();
			return false;
		}

		selectAllOptions( document.getElementById( 'pflcod' ) );
		
		$('#formLink').submit();
	});

	$('#btCancelar').click(function()
	{
		window.location.href = "par.php?modulo=sistema/listaLinksProgramas&acao=A";
	});
});

/**
 * Alterar visibilidade de um campo.
 * 
 * @param string indica o campo a ser mostrado/escondido
 * @return void
 */
function onOffCampo( campo )
{
	var div_on = document.getElementById( campo + '_campo_on' );
	var div_off = document.getElementById( campo + '_campo_off' );
	var input = document.getElementById( campo + '_campo_flag' );
	if ( div_on.style.display == 'none' )
	{
		div_on.style.display = 'block';
		div_off.style.display = 'none';
		input.value = '1';
	}
	else
	{
		div_on.style.display = 'none';
		div_off.style.display = 'block';
		input.value = '0';
	}
}

function habilitaArquivo()
{
	$('#span_arquivo').html('<input type="file" id="arquivo" name="arquivo" /><img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">');
}

</script>

<form action="" method="post" id="formLink" enctype="multipart/form-data">
	<input type="hidden" value="1" name="submetido" />
	<input type="hidden" value="<?=$_GET['lprid']?>" name="lprid" />
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="2" cellPadding="2" align="center">
		<tr>
			<td align="right" class="SubTituloDireita" width="350px">
				Nome:
			</td>
			<td>
				<?=campo_texto('lprnome', 'S', 'S', '', '40', '180', '', 'N', 'left', '', '', 'id="lprnome"', '', null, '' )?>
			</td>
		</tr>
		<tr>
			<td align="right" class="SubTituloDireita" width="350px">
				Link:
			</td>
			<td>
				<?=campo_texto('lprlink', 'S', 'S', '', '40', '180', '', 'N', 'left', '', '', 'id="lprlink"', '', null, '' )?>
			</td>
		</tr>
		<tr>
			<td align="right" class="SubTituloDireita" width="350px">
				Ícone:
			</td>
			<td>
			<span id="span_arquivo">
			<?php
			if( $alpid )
			{
				$arqid = $db->pegaUm("SELECT arqid FROM par.anexolinkprograma WHERE alpid = ".$alpid);
				echo '<img src="../slideshow/slideshow/verimagem.php?arqid='.$arqid.'" /><br /><a onclick="habilitaArquivo();" style="cursor:pointer;">Alterar imagem</a>';
			}
			else
			{
				echo '<input type="file" id="arquivo" name="arquivo" /><img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">';
			}
			?>
			</span>
			</td>
		</tr>
		<tr>
			<td align="right" class="SubTituloDireita" width="350px">
				Instrumento:
			</td>
			<td>
			<?php
			$sql = "SELECT
						itrid as codigo,
						itrdsc as descricao
					FROM
						par.instrumento
					ORDER BY
						itrdsc ASC";
			$db->monta_combo("itrid", $sql, 'S', "Todos", '', '', '', '260', 'S', 'itrid');
			?>
			</td>
		</tr>
		<tr>
			<td align="right" class="SubTituloDireita" width="350px">
				Período:
			</td>
			<td>
			<?=campo_data2('lprdatainicio','N','S','Data de Início','','','', null,'', '', 'lprdatainicio' );?>
			&nbsp;até&nbsp;
			<?=campo_data2('lprdatafim','N','S','Data Fim','','','', null,'', '', 'lprdatafim' );?>
			</td>
		</tr>
		<?php
		$sql = "SELECT DISTINCT
					pflcod AS codigo,
					pfldsc AS descricao
				FROM 
					seguranca.perfil
				WHERE
					sisid = ".SISID_PAR."
				ORDER BY
					pfldsc ASC";
		
		if( $_GET['lprid'] )
		{
			$sqlCarregados 	= "SELECT p.pflcod AS codigo,p.pfldsc AS descricao FROM par.linkprogramaperfil l INNER JOIN seguranca.perfil p ON p.pflcod = l.pflcod WHERE l.lprid = ".$_GET['lprid'];
		}
		
		$arrVisivel 	= array("descricao");
		$arrOrdem		= array("descricao");
		mostrarComboPopup('Perfis:', 'pflcod', $sql, $sqlCarregados, 'Selecione o(s) Perfil(is)', null, null, null, null, $arrVisivel, $arrOrdem);
		?>
		<tr>
			<td align="right" class="SubTituloDireita" width="350px">
				Função(sem parênteses):
			</td>
			<td>
				<?=campo_texto('lprfuncao', 'N', 'S', '', '40', '180', '', 'N', 'left', '', '', 'id="lprfuncao"', '', null, '' )?>
			</td>
		</tr>
		<tr>
			<td align="right" class="SubTituloDireita" width="350px">
				Abrir em Popup:
			</td>
			<td>
				<input type="radio" name="lprpopup" value="N" <?=((!$lprpopup || $lprpopup == 'N') ? 'checked="checked"' : '')?> /> Não
				<input type="radio" name="lprpopup" value="S" <?=(($lprpopup == 'S') ? 'checked="checked"' : '')?> /> Sim
				<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
			</td>
		</tr>
		<tr>
			<td align="right" class="SubTituloDireita" width="350px">
				Status:
			</td>
			<td>
				<input type="radio" name="lprstatus" value="I" <?=((!$lprstatus || $lprstatus == 'I') ? 'checked="checked"' : '')?> /> Inativo
				<input type="radio" name="lprstatus" value="A" <?=(($lprstatus == 'A') ? 'checked="checked"' : '')?> /> Ativo
				<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
			</td>
		</tr>
		<tr>
			<td style="background-color:#c0c0c0;" width="350px">
			</td>
			<td style="background-color:#c0c0c0;">
				<input type="button" id="btSalvar" value="Salvar" />
				<input type="button" id="btCancelar" value="Cancelar" />
			</td>
		</tr>
	</table>
</form>