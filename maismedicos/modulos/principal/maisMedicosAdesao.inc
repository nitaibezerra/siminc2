<?php
if($_GET['uniid']){
	$uniid = $_GET['uniid'];	
	$_SESSION['maismedicos']['maismedicos']['uniid'] = $uniid; 
}else{
	$uniid = $_SESSION['maismedicos']['maismedicos']['uniid'];
}

if($_GET['arquivo'])
{
	include_once APPRAIZ . "includes/classes/file.class.inc";
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$sql = "select 
				sis.sisdiretorio
			from 
				public.arquivo arq
			inner join
				seguranca.sistema sis ON sis.sisid = arq.sisid
			where 
					arqid = {$_GET['arquivo']}";
	$schema = $db->pegaUm($sql);
	$file = new FilesSimec(null,null,$schema);
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

require_once APPRAIZ . "includes/cabecalho.inc";
echo '<br/>';
$db->cria_aba( $abacod_tela, $url, $parametros);
monta_titulo( "Adesão", "&nbsp;");

$maisMedicos = new MaisMedicos();
$arrDados = $maisMedicos->recupaPorUnidade($uniid);
if($arrDados){
	extract($arrDados);
}
if($maisMedicos->msminteresse != "t"){
	$exibe_termo_aceite = true;
}else{
	$exibe_termo_aceite = false;
}

cabecalhoUniversidade($uniid);

$arrPerfil = pegaPerfilGeral();
if(in_array(PERFIL_APOIADOR,$arrPerfil) || in_array(PERFIL_TUTOR,$arrPerfil)){
	$somente_leitura = true;
}else{
	$somente_leitura = false;
}

?>
<script>
function abreTermo(){

	return windowOpen('maismedicos.php?modulo=principal/termo&acao=A', 'blank', 'height=600,width=750,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes');
}
</script>
<style>.link{cursor:pointer}</style>
<script type="text/javascript" src="../includes/JQuery/jquery-1.10.2.min.js"></script>
<form name="formulario" id="formulario" method="post" action="" enctype="multipart/form-data" >
<input type="hidden" id="url" name="url" value="maismedicos.php?modulo=principal/maisMedicosAdesao&acao=A" />
<input type="hidden" id="requisicao" name="requisicao" value="salvarMaisMedicos" />
<input type="hidden" id="classe" name="classe" value="MaisMedicos" />
<input type="hidden" id="msminteresse" name="msminteresse" value=""" />
<input type="hidden" id="msmid" name="msmid" value="<?php echo $maisMedicos->msmid?>" />
<input type="hidden" id="uniid" name="uniid" value="<?php echo $uniid?>" />
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<?php if($exibe_termo_aceite): ?>
		<tr>
			<td colspan="2" align="center" >
				<div style="width:90%;height:400px;border:solid 1px black;overflow:auto;padding:3px;background-color:#fff">
				<?php echo textoHTML2($uniid) ?>
				</div>
			</td>
		</tr>
		<tr>
			<td colspan="2" align="center">
				<?php $data = date("Ymd")?>
				<?php if( ($data >= 20130711 && $data <= 20130725) || $db->testa_superuser()):?>
					<input type="button" name="btn_aceitar" value="Realizar Pré-Adesão" onclick="aceitarTermo()" />
				<?php else: ?>
					A data para realizar a adesão ao Programas Mais Médicos ocorreu entre os dias 11 e 25 de julho de 2013. Aguadem novas informações.
				<?php endif; ?>
			</td>
		</tr>
	<?php else: ?>
		<tr>
			<td colspan="2" align="center" >
				<div style="width:90%;height:400px;border:solid 1px black;overflow:auto;padding:3px;background-color:#fff">
				<?php echo textoHTML2($uniid) ?>
				</div>
			</td>
		</tr>
		<tr>
    		<td colspan="2" class="SubtituloCentro" >Anexo do Termo de Pré-Adesão</td>
		</tr>
		<tr>
			<td colspan="2" align="center" >
				<p>Favor anexar abaixo o <A href="javascript:abreLinkAnexo(<?php echo $uniid ?>);" >termo de pré-adesão</a> assinado.</p>
			</td>
		</tr>
		<tr>
	    	<td width="25%" class="SubTituloDireita">Termo de Pré-Adesão Assinado</td>
	        <td id="td_arquivo">
			<?php if($arqid): ?>
				<?php $arrArquivo = recuperaArquivo($arqid)  ?>
				<a href="javascript:downloadAnexo(<?php echo $arqid ?>)" ><?php echo $arrArquivo['arqnome'] ?>.<?php echo $arrArquivo['arqextensao'] ?></a>
				<?php if(!$somente_leitura): ?>
					<img src="../imagens/excluir.gif" class="link" onclick="excluirAnexo(<?php echo $arqid ?>)" />
				<?php endif; ?>
				<input type="hidden" name="arquivo" id="arquivo" value="<?php echo $arqid ?>" />
			<?php else: ?>
				<input type="file" name="arquivo" id="arquivo" />
			<?php endif; ?> 
			</td>
		</tr>
	<?php endif; ?>
</table>
</form>
<?php if(!$exibe_termo_aceite): ?>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
	    	<td class="subtituloCentro" colspan="2" >
	    		<input type="button" name="btn_salvar" value="Salvar" onclick="salvarMaisMedicos()" />
	    	</td>
		</tr>
	</table>
<?php endif; ?>
<script>
function aceitarTermo()
{
	$("#msminteresse").val("sim");
	$("#classe").val("MaisMedicos");
	$("#requisicao").val("salvarMaisMedicos");
	$("#formulario").submit();
}

function cadNovoTutor(tuttipo)
{
	if(!tuttipo){
		tuttipo = "T";
	} 
	return windowOpen('maismedicos.php?modulo=principal/cadTutores&acao=A&tuttipo='+tuttipo,'blank','height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
}

function editarTutor(tutid,tuttipo)
{
	if(!tuttipo){
		tuttipo = "T";
	} 
	return windowOpen('maismedicos.php?modulo=principal/cadTutores&acao=A&tutid='+tutid+'&tuttipo='+tuttipo,'blank','height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
}

function salvarMaisMedicos()
{
	var erro = 0;
	$("[class~=obrigatorio]").each(function() { 
		if(!this.value || this.value == "Selecione..."){
			erro = 1;
			alert('Favor preencher todos os campos obrigatórios.');
			this.focus();
			return false;
		}
	});
	/*if(erro == 0 && !$("#arquivo").val()){
		erro = 1;
		alert('Favor informar o Anexo do Termo de Pré-Adesão.');
		return false;
	}*/
	if(erro == 0){
		$("#classe").val("MaisMedicos");
		$("#requisicao").val("salvarMaisMedicos");
		$("#formulario").submit();
	}
}

function excluirTutor(tutid,tuttipo)
{
	if(!tuttipo){
		tuttipo = "T";
	}
	if(tuttipo == "T"){
		var desc = "Tutor";
		var div = "div_lista_tutores";
	}else{
		var desc = "Supervisor";
		var div = "div_lista_supervisores";
	}
	if(confirm("Deseja realmente exluir este "+desc+"?"))
	{
		$.ajax({
		   type: "POST",
		   url: window.location,
		   data: "requisicaoAjax=excluirTutor&classe=Tutor&tutid="+tutid+"&tuttipo="+tuttipo+"&uniid=<?php echo $uniid ?>",
		   success: function(msg){
			   	alert('Operação realizada com sucesso.');
		   		$('#'+div).html( msg );
		   }
		 });
	}
}

function listaTutoresAjax(uniid,tuttipo)
{
	if(!tuttipo){
		tuttipo = "T";
	}
	if(tuttipo == "T"){
		var div = "div_lista_tutores";
	}else{
		var div = "div_lista_supervisores";
	}
	$.ajax({
	   type: "POST",
	   url: window.location,
	   data: "requisicaoAjax=listarTutores&classe=Tutor&uniid="+uniid+'&tuttipo='+tuttipo,
	   success: function(msg){
	   		$('#'+div).html( msg );
	   }
	 });
}

function abreLinkAnexo(uniid)
{
	return windowOpen('maismedicos.php?modulo=principal/anexoPortaria&acao=A&uniid='+uniid,'blank','height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
}

function downloadAnexo(arqid)
{
	window.location.href="maismedicos.php?modulo=principal/maisMedicos&acao=A&arquivo="+arqid;
}

function excluirAnexo(arqid)
{
	if(confirm("Deseja realmente exluir este arquivo?"))
	{
		$.ajax({
		   type: "POST",
		   url: window.location,
		   data: "requisicaoAjax=excluirAnexo&classe=MaisMedicos&arqid="+arqid,
		   success: function(msg){
		   		$("#arquivo").val("");
		   		$('#td_arquivo').html( "<input type=\"file\" name=\"arquivo\" id=\"arquivo\" />" );
		   }
		 });
	}
}

function excluirAnexoTermo(arqid)
{
	if(confirm("Deseja realmente exluir este arquivo?"))
	{
		$.ajax({
		   type: "POST",
		   url: window.location,
		   data: "requisicaoAjax=excluirAnexo&classe=MaisMedicos&arqidtermo="+arqid,
		   success: function(msg){
		   		$("#arquivotermo").val("");
		   		$('#td_arquivo_termo').html( "<input type=\"file\" name=\"arquivotermo\" id=\"arquivotermo\" />" );
		   }
		 });
	}
}

$(function() {
	<?php if($_SESSION['maismedicos']['maismedicos']['alert']): ?>
		alert('<?php echo $_SESSION['maismedicos']['maismedicos']['alert'] ?>');
		<?php unset($_SESSION['maismedicos']['maismedicos']['alert']) ?>
	<?php endif; ?>
});
</script>