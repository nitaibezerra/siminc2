<?php 
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
require_once APPRAIZ . "tarefa/classes/Tarefa.class.inc";

$campos	= array("copid"				=> $_SESSION['copid'],
				"coxdatainclusao" 	=> "now()" ,
				"usucpf"    		=> $_SESSION['usucpf'],
				"tpaid"     		=> $_POST['tpaid'],
				"coxstatus" 		=> "'A'"
				);	
if($_REQUEST['download'] == 'S'){
	$file = new FilesSimec();
	$arqid = $_REQUEST['arqid'];
    $arquivo = $file->getDownloadArquivo($arqid);
    echo"<script>window.location.href = 'evento.php?modulo=principal/cadCompraAnexo&acao=A';</script>";
    exit;
} 


$file = new FilesSimec("coanexo", $campos ,"evento");

if($_FILES["Arquivo"]){	
	 
	$arquivoSalvo = $file->setUpload( $_POST['arqdescricao']);
	if($arquivoSalvo){
		echo '<script type="text/javascript"> alert(" Operação realizada com sucesso!");</script>';
		echo"<script>window.location.href = 'evento.php?modulo=principal/cadCompraAnexo&acao=A';</script>";	
	}
}
 
if($_REQUEST['arqidDel'] != ''){
   
    $anexos = array();
    $sql = "UPDATE evento.coanexo SET coxstatus = 'I' where arqid=".$_REQUEST['arqidDel'];
    $db->executar($sql);
    $sql = "UPDATE public.arquivo SET arqstatus = 'I' where arqid=".$_REQUEST['arqidDel'];
    $db->executar($sql);

    $db->commit();
    echo"<script>window.location.href = 'evento.php?modulo=principal/cadCompraAnexo&acao=A';</script>";

}
 /*
if( $_SESSION['copid'] != '')
{ 
	$sql = "SELECT 
				co.copdsc,  
				co.copnumprocesso,
				to_char(co.copdataabertura::date,'DD/MM/YYYY') AS copdataabertura,		 
				co.cooid, 		 
				co.cocid, 		 
				co.conid, 
				to_char(co.copdatalimite::date,'DD/MM/YYYY') AS copdatalimite,	
				co.usucpf ,
				us.usunome
			FROM 
				evento.coprocesso AS co 
			LEFT JOIN seguranca.usuario AS us ON us.usucpf = co.usucpf 
			WHERE
				co.copid = '".$_SESSION['copid']."'
				";
	$rsDadosProcesso = $db->carregar( $sql ); 
} */
?>
<?php
//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';
$db->cria_aba( $abacod_tela, $url, '' );
//$titulo_modulo = "Documento";
monta_titulo( $titulo_modulo, '' );
//montaCabecalhoProcesso($_SESSION['copid']);
$tarid = $_SESSION['dados_tarefa']['tarid'];

$restricoes = array('a');

?>
<script src="../includes/prototype.js"></script>
<script language="javascript" type="text/javascript" src="../includes/tiny_mce.js"></script>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script src="../includes/entidades.js"></script>
<script src="../includes/calendario.js"></script>
<script language="JavaScript">
//Editor de textos
tinyMCE.init({
	theme : "advanced",
	mode: "specific_textareas",
	editor_selector : "text_editor_simple",
	plugins : "table,save,advhr,advimage,advlink,emotions,iespell,insertdatetime,preview,zoom,flash,searchreplace,print,contextmenu,paste,directionality,fullscreen",
	theme_advanced_buttons1 : "undo,redo,separator,link,bold,italic,underline,forecolor,backcolor,separator,justifyleft,justifycenter,justifyright, justifyfull, separator, outdent,indent, separator, bullist",
	theme_advanced_buttons2 : "",
	theme_advanced_buttons3 : "",
	theme_advanced_toolbar_location : "top",
	theme_advanced_toolbar_align : "left",
	extended_valid_elements : "a[name|href|target|title|onclick],img[class|src|border=0|alt|title|hspace|vspace|width|height|align|onmouseover|onmouseout|name],hr[class|width|size|noshade],font[face|size|color|style],span[class|align|style]",
	language : "pt_br",
	width : "450px",
	entity_encoding : "raw"
	});
</script>
<?php echo cabecalhoTarefa($tarid); ?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0" width="95%">
	<tr>
		<td align="left"><b>Dados da Restrição</b></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita" style="vertical-align:top;" width="25%">Restrição:</td>
		<td>
			<textarea name="obsdescricao" id="obsdescricao" rows="10" cols="70" class="text_editor_simple"><?= $obsdescricao ?></textarea>
		</td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita" style="vertical-align:top;">Providência:</td>
		<td><?= campo_textarea( 'obsmedida', 'N', 'S', '', 90, 3, 250 ); ?></td>
	</tr>
	<tr style="background-color: #cccccc">
		<td align='right' style="vertical-align:top;">&nbsp;</td>
		<td><input type="button" name="botao" value="Salvar" onclick="cadastrar_controle();"/></td>
	</tr>
</table>

<form method="post" id="formulario_<?= $restricao['obsid'] ?>">
<input type="hidden" name="evento" value="controlar_solucao"/>
<input type="hidden" name="obsid" value="<?= $restricao['obsid'] ?>"/>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0" width="95%">
	<tbody>
		<tr style="background-color: #cccccc">
			<td align='right' style="vertical-align:top; width:25%">&nbsp;</td>
			<td align='left'  style="vertical-align:top;">
				<b>Restrição</b>
				<img src="../imagens/restricao.png" border="0" align="absmiddle" style="margin: 0 3px 0 3px;"/>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" style="vertical-align:top; width:25%;">Descrição:</td>
			<input type="hidden"  id="hiddenDesc[<?= $restricao['obsid'] ?>]" name="hiddenDesc[<?= $restricao['obsid'] ?>]" value="<?= $restricao['obsdescricao'] ?>"/>
			<input type="hidden" name="id" value="<?= $restricao['obsid'] ?>"/>
			<td id="celDescricao_<?= $restricao['obsid'] ?>" name="celDescricao_<?= $restricao['obsid'] ?>" >
				<?= $restricao['obsdescricao'] ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" style="vertical-align:top; width:25%;">Data:</td>
			<input type="hidden" id="hiddenData[<?= $restricao['obsid'] ?>]" name="hiddenData[<?= $restricao['obsid'] ?>]" value="<?= $restricao['obsdata'] ?>"/>
			<td id="celData_<?= $restricao['obsid'] ?>" name="celData_<?= $restricao['obsid'] ?>"><?= formata_data( $restricao['obsdata'] ); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" style="vertical-align:top; width:25%;">Autor:</td>
			<td>
				<div>
					<img onclick="enviar_email( '<?= $restricao['cpfautor'] ?>' );" title="enviar e-mail" src="../imagens/email.gif" align="absmiddle" style="border:0; cursor:pointer;"/>
					<?= $restricao['nomeautor'] ?>
				</div>
				<div style="color:#959595;"><?= $restricao['unidadeautor'] ?> - Tel: (<?= $restricao['dddautor'] ?>) <?= $restricao['telefoneautor'] ?></div>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" style="vertical-align:top; width:25%;">Restrição superada?</td>
			<td>
				<label title="indica que a restrição está superada">
					<input type="radio" name="obssolucao" value="t" <?= $restricao['obssolucao'] == 't' ? 'checked="checked"' : '' ?>/>
					Sim
				</label>
				&nbsp;&nbsp;
				<label title="indica que a restrição não está superada">
					<input type="radio" name="obssolucao" value="f" <?= $restricao['obssolucao'] == 'f' ? 'checked="checked"' : '' ?>/>
					Não
				</label>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" style="vertical-align:top; width:25%;">Providência:</td>
			<td>
			<?php
			$obsmedida = $restricao["obsmedida"];
			echo campo_textarea( 'obsmedida', 'N', 'S', '', 70, 2, 250 );
			?>
			</td>
		</tr>
		<!-- tr id="titulo_providencia_<?= $restricao['obsid'] ?>" style="background-color: #cccccc;">
			<td align='right' style="vertical-align:top; width:25%">&nbsp;</td>
			<td align='left' style="vertical-align:top;"><b>Superação</b></td>
		</tr>
		<tr id="datasuperacao_<?= $restricao['obsid'] ?>">
			<td class="SubTituloDireita" style="vertical-align:top; width:25%;">Data:</td>
			<td id="<?= $restricao['obsid'] ?>"><?= formata_data( $restricao['obsdatasolucao'] ) ?></td>
		</tr>
		<tr id="responsavel_<?= $restricao['obsid'] ?>">
			<td class="SubTituloDireita" style="vertical-align:top; width:25%;">Autor:</td>
			<td>
				<div>
					<img onclick="enviar_email( '<?= $restricao['cpfresponsavel'] ?>' );" title="enviar e-mail" src="../imagens/email.gif" align="absmiddle" style="border:0; cursor:pointer;"/>
					<?= $restricao['nomeresponsavel'] ?>
				</div>
				<div style="color:#959595;"><?= $restricao['unidaderesponsavel'] ?> - Tel: (<?= $restricao['dddresponsavel'] ?>) <?= $restricao['telefoneresponsavel'] ?></div>
			</td>
		</tr -->
		<tr style="background-color: #cccccc">
			<td align='right' style="vertical-align:top; width:25%">&nbsp;</td>
			<td>
			<?php
			if( $restricao['cpfautor'] == $_SESSION['usucpf'])
			{
			?>
				<input type="button" name="altera[<?= $restricao['obsid'] ?>]" id="altera[<?= $restricao['obsid'] ?>]" value="Alterar" onclick="alteraCampos(<?= $restricao['obsid'] ?>);"/>
			<?php
			}
			?>
				<input type="submit" name="botao" value="Salvar"/>
				<input type="button" name="botao" value="Excluir" onclick="excluirControle( <?= $restricao['obsid'] ?> );" />
			</td>
		</tr>
	</tbody>
</table>
</form>