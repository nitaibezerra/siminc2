<?php
if($_GET['arquivo'])
{
       include_once APPRAIZ . "includes/classes/file.class.inc";
       include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
       $file = new FilesSimec();
       $file-> getDownloadArquivo($_GET['arquivo']);
       exit;
}
if($_REQUEST['requisicaoAjax'] && $_REQUEST['classe']){
	$n = new $_REQUEST['classe'];
	$n->$_REQUEST['requisicaoAjax']();
	die;
}
if($_REQUEST['requisicao'] && $_REQUEST['classe']){
	$n = new $_REQUEST['classe'];
	$n->$_REQUEST['requisicao']();
}
if(!$_GET['pagid']){
	echo "<script>alert('Não existem ações disponíveis para este pagamento.');window.location.href='obras2.php?modulo=principal/listaPagamento&acao=A';</script>";
	die;
}
//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";

if($_GET['pagid']){
	$pag = new Pagamento($_GET['pagid']);
	extract($pag->getDados());
	if($sgeid){
		$grupoEmpresa  = new Supervisao_Grupo_Empresa($sgeid);
		$sgrid = $grupoEmpresa->sgrid;
		$empresa = $grupoEmpresa->pegaEmpresaPorSgeid($sgeid);
	}
	$totalSupervisaoPorPagamento = $pag->verificaSupervisao($_GET['pagid']);
	$totalSupervisaoPorPagamento = $totalSupervisaoPorPagamento ? $totalSupervisaoPorPagamento : 0;
}
if($docid){
	$arrEstadoAtual = wf_pegarEstadoAtual( $docid );
	if($arrEstadoAtual['esdid'] != WF_ESDID_PAGAMENTO_SUPERVISAO_PAGAMENTO_SOLICITADO){
		echo "<script>alert('Não existem ações disponíveis para este pagamento.');window.location.href='obras2.php?modulo=principal/listaPagamento&acao=A';</script>";
		die;
	}
}

//criaAbaPagamento();
$db->cria_aba($abacod_tela,$url,$parametro);
monta_titulo( "Realizar Pagamento", '' );

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<style>.link{cursor:pointer}</style>
<form id="formulario" name="formulario" method="post" enctype="multipart/form-data" action="">
<input type="hidden" name="pagid" id="pagid" value="<?php echo $pagid ?>" />
<input type="hidden" name="pagqtdelaudo" id="pagqtdelaudo" value="<?php echo $totalSupervisaoPorPagamento ?>" />
<input type="hidden" name="pagqtdelaudoatrasado" id="pagqtdelaudoatrasado" value="<?php echo $pagqtdelaudoatrasado ?>" />
<input type="hidden" name="requisicao" id="requisicao" value="" />
<input type="hidden" name="classe" id="classe" value="" />
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0">
	<tr>
        <td class="SubTituloDireita" width="35%">Empresa</td>
		<td>
			<?php echo $empresa ?>
		</td>
		<td width="10%" rowspan="6" valign="top" >
		<?php
			if($docid){
            	// Barra de estado WORKFLOW
				wf_desenhaBarraNavegacao($docid, array('pagid'=>$pagid,'aedid'=>WF_AEDID_LAUDO_SUPERVISAO_REALIZAR_PAGAMENTO));
			}
            ?>
		</td>
	</tr>
	<tr>
        <td class="SubTituloDireita">ID do Pagamento</td>
		<td>
			<?php echo $pagid; ?>
		</td>
	</tr>
	<tr>
        <td class="SubTituloDireita" >Nota Fiscal</td>
		<td>
			<?php if($arqid): ?>
				<?php $arrArquivo = recuperaArquivo($arqid)  ?>
				<a href="javascript:downloadArquivo(<?php echo $arqid ?>)" ><?php echo $arrArquivo['arqnome'] ?>.<?php echo $arrArquivo['arqextensao'] ?></a>
			<?php endif; ?> 
		</td>
	</tr>
	<tr>
        <td class="SubTituloDireita">Valor</td>
		<td id="td_valor" ></td>
	</tr>
	<tr>
        <td class="SubTituloDireita">Quantidade de Laudos</td>
		<td><?php echo $totalSupervisaoPorPagamento ?></td>
	</tr>
	<tr>
        <td class="SubTituloDireita">Quantidade de Laudos Atrasados</td>
		<td id="td_fora_prazo" ></td>
	</tr>
	<tr>
        <td colspan="2" class="SubTituloCentro">Lista de Laudos</td>
	</tr>
	<tr>
        <td colspan="2" ><?php $pag->listaSupervisoesPagamento() ?></td>
	</tr>
	<tr>
        <td class="SubTituloDireita">Número da Ordem Bancária</td>
		<td><?php echo campo_texto("pagnumob","N","S","",20,255,"[#]",'') ?></td>
	</tr>
	<tr>
        <td class="SubTituloDireita">Data da Solicitação de Pagamento</td>
		<td><?php echo campo_data2( 'pagdatasolicitacaopagamento', 'S', "S", '', 'S','','' ); ?></td>
	</tr>
	<tr>
        <td class="SubTituloDireita">Observações</td>
		<td><?php echo campo_textarea("pagobs","N","S","",60,5,255,""); ?></td>
	</tr>
	<tr>
        <td class="SubTituloDireita">Anexo</td>
		<td id="td_arquivo">
			<?php if($paganexoarqid): ?>
				<?php $arrArquivo = recuperaArquivo($paganexoarqid)  ?>
				<a href="javascript:downloadArquivo(<?php echo $paganexoarqid ?>)" ><?php echo $arrArquivo['arqnome'] ?>.<?php echo $arrArquivo['arqextensao'] ?></a>
				<?php if(!$somente_leitura): ?>
					<img src="../imagens/excluir.gif" class="link" onclick="excluirAnexo()" />
				<?php endif; ?>
				<input type="hidden" name="arquivo" id="arquivo" value="<?php echo $paganexoarqid ?>" />
			<?php else: ?>
				<input type="file" name="arquivo" id="arquivo" />
			<?php endif; ?> 
		</td>
	</tr>
	<tr>
        <td class="SubTituloDireita">Valor Total</td>
		<td><div style="font-weight:bold" id="div_valor_total"><?php echo number_format($pagvalortotal, 2, ',', '.') ?></div><input type="hidden" name="pagvalortotal" id="pagvalortotal" value="<?php echo $pagvalortotal ?>" /></td>
	</tr>
	<tr>
        <td colspan="2" align="center">
			<input type="button" name="btn_pagar" value="Salvar" onclick="salvarRealizacaoPagamento()" />
			<input type="button" name="btn_pagar" value="Voltar" onclick="window.location.href='obras2.php?modulo=principal/listaPagamento&acao=A'" />
		</td>
	</tr>
</table>
</form>
<script>

function salvarRealizacaoPagamento()
{
	var erro = 0;
	
//	if(!$("[name='pagnumob']").val()){
//		erro =1;
//		alert('Informe o Número da Ordem Bancária.');
//		return false;
//	}
	if(!$("[name='pagdatasolicitacaopagamento']").val()){
		erro =1;
		alert('Informe a Data da Solicitação de Pagamento.');
		return false;
	}
	
	if(erro==0){
		$("#classe").val("Pagamento");
		$("#requisicao").val("salvarRealizacaoPagamento");
		$("#formulario").submit();
	}
}

function downloadArquivo(arqid)
{
	window.location.href="obras2.php?modulo=principal/realizarPagamento&acao=A&arquivo="+arqid;
}

function excluirAnexo()
{
	if(confirm("Deseja realmente exluir o anexo?"))
	{
		var arqid = $("#arquivo").val();
		var pagid = $("#pagid").val();
		$.ajax({
		   type: "POST",
		   url: window.location,
		   data: "requisicaoAjax=excluirNotaFiscal&classe=Pagamento&pagid="+pagid+"&arqid="+arqid+"&campo=paganexoarqid",
		   success: function(msg){
		   		$("#arquivo").val("");
		   		$('#td_arquivo').html( "<input type=\"file\" name=\"arquivo\" id=\"arquivo\" />" );
		   }
		 });
	}
}

function glosaSupervisao(sueid)
{
	if($("[name='rdn_sueid_"+sueid+"']:checked").val() == "nao"){
		$("#div_glosa_sueid_"+sueid).hide();
	}
	if($("[name='rdn_sueid_"+sueid+"']:checked").val() == "sim"){
		$("#div_glosa_sueid_"+sueid).show();
	}
	somaValorTotal();
}

function somarValor()
{
	var valor = 0;
	$("[name^='hdn_valor[']").each(
		function(i){
			valor =  ($(this).val()*1) + valor; 
		}
	);
	$("#td_valor").html(mascaraglobal("[.###],##",valor.toFixed(2)));
}

function somaValorTotal()
{
	var valorTotal = 0;
	$("[name^='pgsvalor_']").each(
		function(){
			id =  $(this).attr("name");
			id = id.replace("pgsvalor_","");
			if($("[name='rdn_sueid_"+id+"']:checked").val() == "nao"){
				valor = $("[name='hdn_valor["+id+"]']").val();
			}
			if($("[name='rdn_sueid_"+id+"']:checked").val() == "sim"){
				valor = $(this).val();
				valor = valor.replace(".","");
				valor = valor.replace(".","");
				valor = valor.replace(".","");
				valor = valor.replace(".","");
				valor = valor.replace(",",".");
			}
			valorTotal =  (valor*1) + valorTotal; 
		}
	);
	$("#div_valor_total").html(mascaraglobal("[.###],##",valorTotal.toFixed(2)));
	$("#pagvalortotal").val(valorTotal.toFixed(2));
}

function fakePagamento(valor)
{
	return true;
}


$(function() {
	$("[name^='pgsvalor_']").change(function(){
		somaValorTotal();
	});
	$("#td_fora_prazo").html($("[name^='hdn_fora_prazo[']").length);
	$("#pagqtdelaudoatrasado").val($("[name^='hdn_fora_prazo[']").length);
	somarValor();
	<?php if($_SESSION['obras2']['pagamento']['alert']): ?>
		alert('<?php echo $_SESSION['obras2']['pagamento']['alert'] ?>');
		<?php unset($_SESSION['obras2']['pagamento']['alert']) ?>
	<?php endif; ?>
});
</script>