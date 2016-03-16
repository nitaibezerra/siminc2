<?php
if($_REQUEST['requisicaoAjax']){
	header('content-type: text/html; charset=ISO-8859-1');
	$_REQUEST['requisicaoAjax']();
	exit;
}

function dateDiff1($from,$to){
	$diff = $to - $from;
	$info = array();

	if($diff>86400){
		//um ou mais dias
		$info['d'] = ($diff - ($diff%86400))/86400;
		$diff = $diff%86400;
	}

	$f = '';
	foreach($info as $k=>$v){
		//if($v>0) $f .= "$v $k, ";
		if($v>0) $f .= "$v";
	}
	
	//return substr($f,0,-2);
	return ($f == 24) ? "1" : $f;
}

function calculaDiasVigencia($dados) {
	
	$arData = explode('/', $dados['ptpdatainicio']);

	$dia = $arData[0];
	$mes = $arData[1];
	$ano = $arData[2];
	$dataFinal = mktime(24*$dados['ptpvigencia'], 0, 0, $mes, $dia, $ano);
	$dataFormatada = date('d/m/Y',$dataFinal);
	return $dataFormatada;
	
}


if($_REQUEST['requisicao']){
	if($_REQUEST['requisicao'] == "downloadArquivoPregao"){
		$_REQUEST['requisicao']();
		die;
	}
	
	if($_REQUEST['requisicao'] == "calculaDataFim") {
		echo calculaDiasVigencia($_REQUEST);	
		die;	
	}
	
	if($_REQUEST['requisicao'] == "calculaDiasVigencia") {
		$vigdatainicio = formata_data_sql($_POST['ptpdatainicio']);
		$vigdatafim = formata_data_sql($_POST['ptpdatafim']);
		echo dateDiff1(strtotime($vigdatainicio),strtotime($vigdatafim));
		die;
	}
	$arrRetorno = $_REQUEST['requisicao']();
}

$arrUFPregao = array();

if($_REQUEST['ptpid']){
	$sql = "SELECT 
				pre.*,
				to_char(pre.ptpdatainicio,'DD/MM/YYYY') as ptpdatainicio,
				'<a href=\"javascript:DownloadArquivo(\'' || arq.arqid || '\')\" >' || arq.arqnome || '.' || arq.arqextensao || '</a>' as arqnome,
				arq.arqdescricao as arqdsc 
			FROM 
				par.propostatipopregao pre 
			LEFT JOIN 
				public.arquivo arq ON arq.arqid = pre.arqid 
			WHERE 
				ptpid = {$_REQUEST['ptpid']}";
	$arrDados = $db->pegaLinha($sql);
	
	if(is_array($arrDados)){
		extract($arrDados);
	}
	
	$ptpdatafim = calculaDiasVigencia(array("ptpdatainicio" => $ptpdatainicio, "ptpvigencia" => $ptpvigencia));
	$ptpano = $ptpanoata; 
	
	$sql = "SELECT 
				est.estuf as codigo,
				est.estdescricao as descricao 
			FROM 
				par.pregaouf pre 
			INNER JOIN 
				territorios.estado est ON est.estuf = pre.estuf 
			WHERE 
				ptpid = $ptpid";
	$arrUFPregao = $db->carregar($sql);
}

include  APPRAIZ."includes/cabecalho.inc";
echo '<br>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( 'Pregão', '&nbsp' );

?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<style>
	.SubtituloTabela{background-color:#dcdcdc}
	.SubtituloBotoes{background-color:#cccccc}
	.negrito{font-weight:bold}
	.bold{font-weight:bold}
	.normal{font-weight:normal}
	.center{text-align: center;}
	.direita{text-align: right;}
	.esquerda{text-align: left;}
	.msg_erro{color:#990000}
	.link{cursor: pointer}
	.mini{width:12px;height:12px}
	.sucess_msg{color: blue;}
	.img_middle{vertical-align:middle}
	.hidden{display:none}
	.absolute{position:absolute;padding-top:5px;padding-bottom:5px;padding-left:5px;margin-top:5px;margin-left:50px;border:solid 1px black;background-color: #FFFFFF}
	.fechar{position:relative;right:-5px;top:-26px;}
	.img{background-color:#FFFFFF}
</style>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script type="text/javascript">

$(function() {
	$('#btn_salvar').click(function() {
		var erro = 0;
		$("[class~=obrigatorio]").each(function() { 
			if(!this.value){
				erro = 1;
				alert('Favor preencher todos os campos obrigatório');
				this.focus();
				return false;
			}
		});
		$("#estuf option").each(function() { 
			if(!this.value){
				erro = 1;
				alert('Favor preencher todos os campos obrigatório');
				$('#estuf').focus();
				return false;
			}
		});
		if(erro == 0){
			selectAllOptions( document.getElementById('estuf') );
			$("#formulario_pregao").submit();
		}
	});
});
 
function marcarTodos(obj)
{
	if( $("#" + obj.id).is(':checked') ){
		$("[type=checkbox]").attr("checked",true);
	}else{
		$("[type=checkbox]").attr("checked",false);
	}
}

function DownloadArquivo(arqid)
{
	window.location.href = "par.php?modulo=sistema/tabelasapoio/pregao&acao=A&requisicao=downloadArquivoPregao&arqid=" + arqid; 
}

function alterarPregao(ptpid)
{
	$("[name=ptpid]").val(ptpid);
	$("[name=requisicao]").val("");
	$("#formulario_pregao").submit();
	
}

function excluirPregao(ptpid)
{
	if(confirm("Deseja realmente excluir o pregão?")){
		$.ajax({
			type: "POST",
			url: window.location,
			data: "requisicaoAjax=excluirPregao&ptpid=" + ptpid,
			 success: function(msg){
			 	if(msg == "erro"){
			 		alert( "Não foi possível realizar a operação!" );
			 	}else{
			 		$("#div_lista_pregao").html(msg);
			 		$("[name=ptpid]").val("");
			 		alert( "Operação realizada com sucesso!" );
			 	}
			 }
		});
	}
}

function alterarArquivoPregao(arqid)
{
	$("[name=arqdsc]").val("");
	$("[name=arqid],[name=img_alterar_arquivo_pregao],[id=link_arquivo]").remove();
	$("[name=arquivo]").attr("style","display:block");
	
	$.ajax({
		type: "POST",
		url: window.location,
		data: "requisicaoAjax=excluirArquivoPregao&arqid" + arqid
	});
}

function calculaDataFim() {
	var ptpdatainicio = $('#ptpdatainicio').val();
	var ptpvigencia   = $('#ptpvigencia').val();	 
	if( ptpdatainicio && ptpvigencia){
		$.ajax({
			type: 'POST',
			url: 'par.php?modulo=sistema/tabelasapoio/pregao&acao=A',
   			async: false,
			data: 'requisicao=calculaDataFim&ptpvigencia='+ptpvigencia+'&ptpdatainicio='+ptpdatainicio,
			 success: function(msg){
			 	$('#ptpdatafim').val(msg);
			 }
		});
	}	
}

function calculaDiasVigencia() {
	var ptpdatainicio = $('#ptpdatainicio').val();
	var ptpdatafim   = $('#ptpdatafim').val();	 
	if( ptpdatainicio && ptpvigencia){
		$.ajax({
			type: 'POST',
			url: 'par.php?modulo=sistema/tabelasapoio/pregao&acao=A',
   			async: false,
			data: 'requisicao=calculaDiasVigencia&ptpdatafim='+ptpdatafim+'&ptpdatainicio='+ptpdatainicio,
			 success: function(msg){
			 	$('#ptpvigencia').val(msg);
			 }
		});
	}
}


</script>
<form name="formulario_pregao" id="formulario_pregao" enctype="multipart/form-data"  method="post" action="" >
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td width="25%" class="SubtituloDireita">Nº Ata:</td>
			<td>
				<?php 
					if($ptpnumeroata){
						$ptpnumeroata = substr($ptpnumeroata,0,2) .'/'. substr($ptpnumeroata,2);
					} 
				?>
				<?php echo campo_texto("ptpnumeroata","S","S","Cargo","30","",'###/####',"","","","","","",""); ?>
			</td>
		</tr>
		<tr>
			<td width="25%" class="SubtituloDireita" >Ano Ata:</td>
			<td><?php	
			$sql = "SELECT ano as codigo, ano as descricao FROM public.anos";
			if(!$ptpano) $ptpano = $_SESSION['exercicio'];
			$db->monta_combo('ptpano', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'ptpano');
			?></td>
		</tr>
		<tr>
			<td width="25%" class="SubtituloDireita" >Data Início:</td>
			<td><?php echo campo_data2('ptpdatainicio', 'S', 'S','Data Início', '', '', '', '', 'calculaDataFim();'); ?></td>
		</tr>
		<tr>
			<td width="25%" class="SubtituloDireita">Dias de vigência:</td>
			<td><?php echo campo_texto("ptpvigencia","S","S","Dias de vigência","8","","[#]","","","","","id='ptpvigencia'","calculaDataFim();"); ?></td>
		</tr>
		<tr>
			<td width="25%" class="SubtituloDireita" >Data Fim:</td>
			<td><?php echo campo_data2('ptpdatafim', 'N', 'S','Data Fim', '', '', '', '', 'calculaDiasVigencia();'); ?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >Objeto:</td>
			<td><?php echo campo_textarea("ptpobjeto","S","S",'',"80","5",'500','','','','','',''); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top">UF:</td>
			<td>
			<?
			$sqlComboInstituicao = "SELECT 
										estuf as codigo,
										estdescricao as descricao 
									FROM
										territorios.estado
									order by 
										estuf";
			combo_popup("estuf", $sqlComboInstituicao, "Estado", "400x400", 0, '', "", "S", false, false, 5, 400,"","","","",$arrUFPregao);
			?>
			<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
			</td>
		</tr>
		<tr>
			<td colspan="2" class="SubtituloTabela bold" >Anexo</td>
		</tr>
		<tr>
			<td width="25%" class="SubtituloDireita" >Anexo:</td>
			<td>
				<?php //if($arqid): ?>
					<span id="link_arquivo" ><?php echo $arqnome ?></span>
					<img style="cursor:pointer;vertical-align:middle" name="img_alterar_arquivo_pregao" onclick="alterarArquivoPregao('<?php echo $arqid ?>')" src="/imagens/alterar.gif" title="Alterar Arquivo" />
					<input type="hidden" name="arqid" value="<?php echo $arqid ?>" />
					<input type="file" name="arquivo" style="display:none" id="inp_arquivo" />
				<?php //else: ?>
					
				<?php //endif; ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >Descrição do Anexo:</td>
			<td><?php echo campo_textarea("arqdsc","N","S",'',"80","5",'500','','','','','',''); ?></td>
		</tr>
		<tr>
			<td colspan="2" class="SubtituloBotoes bold center" >
				<input type="hidden" name="ptpid" value="<?php echo $ptpid ?>" />
				<input type="hidden" name="requisicao" value="salvarPregao" />
				<input type="button" id="btn_salvar" name="btn_salvar" value="Salvar" />
				<?php if($ptpid): ?>
					<input type="button" onclick="window.location.href='par.php?modulo=sistema/tabelasapoio/pregao&acao=A'" id="btn_novo" name="btn_novo" value="Novo" />
				<?php endif; ?>
			</td>
		</tr>
	</table>
</form>
<div id="div_lista_pregao">
	<?php listarPregao() ?>
</div>
<?php if($arrRetorno['msg']): ?>
<script>
	alert('<?php echo $arrRetorno['msg'] ?>');
</script>
<?php endif; ?>