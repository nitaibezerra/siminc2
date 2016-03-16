<?php


if($_REQUEST['requisicao']){
	$form = new FormularioTecnologiaMi();
	$form->salvarFormulario();
}

if($_GET['arquivo'])
{
       include_once APPRAIZ . "includes/classes/file.class.inc";
       include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
       $file = new FilesSimec();
       $file-> getDownloadArquivo($_GET['arquivo']);
       exit;
}

//if($_POST['ftiid'] && $_POST['arqid']){
//    $form = new Formulario_TecnologiaMI();
//    $form->excluirArquivo($_POST['arqid']);
//    exit;
//}


//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";
$db->cria_aba($abacod_tela,$url,$parametro);

monta_titulo( ($_GET['ftiid'] ? "Editar" : "Cadastrar")." Formulário MI", '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> Indica Campo Obrigatório.' );

if($_GET['ftiid']){
	$form = new FormularioTecnologiaMi($_GET['ftiid']);
	extract($form->getDados());
}

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<style>
	.hidden{display:none}
	.link{cursor:pointer}
</style>
<form id="formulario" name="formulario" method="post" action="" enctype="multipart/form-data" >
<input type="hidden" name="ftiid" id="ftiid" value="<?php echo $ftiid ?>" />
<input type="hidden" name="requisicao" id="requisicao" value="" />
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0">
    <tr>
        <td class="SubTituloDireita" width="35%">Tecnologia</td>
        <td>
            <?php
                $tecnologia = new TecnologiaMi();
                $dados = $tecnologia->listaCombo();
                $db->monta_combo("tmiid", $dados, 'S', "Selecione...", "", '', '', '', 'S', 'tmiid');
            ?>
        </td>
    </tr>
	<tr>
		<td class="SubTituloDireita">Nome</td>
		<td>
			<?php echo campo_texto('ftinome', 'S', "S", '', 100, 100, '', '', '', '', 0, 'id="ftinome"', '', $ftinome); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Ordem</td>
		<td>
			<?php echo campo_texto( 'ftiordem', 'S', 'S', '', 11, 30, '#########', '', 'right', '', 0, '', $ftiordem); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Obrigatório</td>
		<td>
            <input type="checkbox" value="true" name="ftisnobrigatorio" id="ftisnobrigatorio"<?php echo (($ftisnobrigatorio == 't') ? 'checked="checked"' : ''); ?> />
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Arquivo de modelo</td>
        <td id="td_arquivo">
			<?php if($arqid): ?>
				<?php $arrArquivo = recuperaArquivo($arqid)  ?>
				<a href="javascript:downloadArquivo(<?php echo $arqid ?>)" ><?php echo $arrArquivo['arqnome'] ?>.<?php echo $arrArquivo['arqextensao'] ?></a>
				<?php if(!$somente_leitura): ?>
					<img src="../imagens/excluir.gif" class="link" onclick="excluirArquivo()" />
				<?php endif; ?>
				<input type="hidden" name="arquivo" id="arquivo" value="<?php echo $arqid ?>" />
			<?php else: ?>
				<input type="file" name="arquivo" id="arquivo" />
			<?php endif; ?> 
		</td>
	</tr>
	<tr>
        <td bgcolor="#c0c0c0" colspan="2" align="center">
			<input type="button" value="Salvar"      name="btn_salvar"   onclick="salvarFormularioMI();">
            <input type="button" value="Cancelar"    name="btn_cancelar" onclick="window.location='obras2.php?modulo=principal/cadFormularioMI&acao=A'">
        </td>
    </tr>
</table>
</form>
<script type="text/javascript">

function salvarFormularioMI()
{
	var erro = 0;
	$("[class~=obrigatorio]").each(function() { 
		if(!this.value || this.value == "Selecione..."){
			erro = 1;
			alert('Favor preencher todos os campos obrigatórios!');
			this.focus();
			return false;
		}
	});
    if(!$("#arquivo").val()){
		erro = 1;
		alert('Insira o arquivo de modelo.');
		return false;
	}
	if(erro == 0){
        $("#requisicao").val("salvarFormulario");
    	$('#formulario').submit();
	}
}


function downloadArquivo(arqid)
{
	window.location.href="obras2.php?modulo=principal/cadFormularioMI&acao=A&arquivo="+arqid;
}

function excluirArquivo()
{
	if(confirm("Deseja realmente exluir este arquivo?"))
	{
        $("#arquivo").val("");
        $('#td_arquivo').html( "<input type=\"file\" name=\"arquivo\" id=\"arquivo\" />" );
	}
}

</script>