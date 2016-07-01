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

if($_GET['termogerado'])
{
	$caminho = APPRAIZ."arquivos/".$_SESSION['sisdiretorio'].'/termoadesao/'.$_GET['termogerado'].'.pdf';
	if(is_file($caminho)){
		$filename = $caminho;
		header( 'Content-type: application/pdf' );
		header( 'Content-Disposition: attachment; filename='.$filename);
		readfile( $caminho );
	}else{
		echo "<script>alert('Não foi possível recuperar o termo de adesão.');window.location='maismedicos.php?modulo=principal/maisMedicosTermoAdesao&acao=A';</script>";	
	}
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

$sql = "select
				*
			from
				maismedicos.universidade uni
			inner join
				maismedicos.usuarioresponsabilidade ure ON ure.uniid = uni.uniid
			where
				ure.pflcod = '".PERFIL_REITOR."'
			and
				rpustatus = 'A'
			and
				unicnpj is not null
			and
				estuf is not null
			and
				muncod is not null
			and
				uni.uniid = $uniid";
$arrDados = $db->pegaLinha($sql);
if(!$arrDados){
	echo "<script>alert('Favor informar os dados da Instituição antes de realizar a Adesão.');window.location='maismedicos.php?modulo=principal/dadosInstituicao&acao=A';</script>";
	exit;
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
if($maisMedicos->msmadesao != "t"){
	$exibe_termo_aceite = true;
}else{
	$exibe_termo_aceite = false;
}

cabecalhoUniversidade($uniid);

$arrPerfil = pegaPerfilGeral();
if(in_array(PERFIL_CONSULTA,$arrPerfil) || in_array(PERFIL_APOIADOR,$arrPerfil) || in_array(PERFIL_TUTOR,$arrPerfil)){
	$habilitado = "N";
}else{
	$habilitado = "S";
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
<input type="hidden" id="url" name="url" value="maismedicos.php?modulo=principal/maisMedicosTermoAdesao&acao=A" />
<input type="hidden" id="requisicao" name="requisicao" value="salvarMaisMedicos" />
<input type="hidden" id="classe" name="classe" value="MaisMedicos" />
<input type="hidden" id="msmadesao" name="msmadesao" value=""" />
<input type="hidden" id="gerartermoadesao" name="gerartermoadesao" value=""" />
<input type="hidden" id="msmid" name="msmid" value="<?php echo $maisMedicos->msmid?>" />
<input type="hidden" id="uniid" name="uniid" value="<?php echo $uniid?>" />
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<?php if($exibe_termo_aceite): ?>
		<tr>
			<td colspan="2" align="center" >
				<div style="width:90%;height:400px;border:solid 1px black;overflow:auto;padding:3px;background-color:#fff">
				<?php echo documentoAdesao($uniid)?>
				</div>
			</td>
		</tr>
		<tr>
			<td colspan="2" align="center">
				<?php if($habilitado == "S"): ?>
					<input type="button" name="btn_aceitar" value="Realizar Adesão" onclick="aceitarTermoAdesao()" />
				<?php endif; ?>
			</td>
		</tr>
	<?php else: ?>
		<tr>
			<td colspan="2" align="center" >
				<div style="width:90%;height:400px;border:solid 1px black;overflow:auto;padding:3px;background-color:#fff">
				<?php echo documentoAdesao($uniid)?>
				</div>
			</td>
		</tr>
		<?php if ($arqidtermo): ?>
			<?php 
			$sql = "select to_char(arqdata, 'DD/MM/YYYY') || ' ' || arqhora as gerado from public.arquivo where arqid = {$arqidtermo};"; 
			$arqGeradoData = $db->pegaUm($sql);
			?>
			<tr>
			   	<td width="25%" class="SubTituloDireita">Termo de Adesão</td>
			    <td>
			    	<a href="javascript:downloadAnexo('<?php echo $arqidtermo ?>')" >Clique aqui para visualizar o termo de adesão.</a>
			    	<?php if($habilitado == "S"): ?>
			    		<img src="../imagens/excluir.gif" class="link" onclick="excluirTermoAdesao('<?php echo $arqidtermo ?>')" title="Excluir Termo de Adesão" />
			    	<?php endif; ?>
			    	&nbsp;&nbsp;&nbsp;<i><font color="gray">Arquivo gerado em, <?php echo $arqGeradoData; ?></font></i>
			    </td>
			</tr>
		<?php else: ?>
			<tr>
				<td width="25%" class="SubTituloDireita">Termo de Adesão</td>
			    <td>
			    	<?php if($habilitado == "S"): ?>
						<input type="button" name="btn_aceitar" value="Gerar Termo de Adesão" onclick="gerarTermoAdesao()" />
					<?php endif; ?>
			    </td>
			</tr>
			<?php if($db->testa_superuser() && $habilitado == "S"): ?>
				<tr>
					<td class="SubTituloDireita">Anexar Termo de Adesão</td>
				    <td><input type="file" name="arquivotermo" id="arquivotermo" /></td>
				</tr>
				<tr>
					<td colspan="2" align="center" >
						<input type="button" name="btn_aceitar" value="Anexar" onclick="anexarTermoAdesao()" />
					</td>
				</tr>
			<?php endif; ?>
		<?php endif; ?>
	<?php endif; ?>
</table>
</form>
<script>
function aceitarTermoAdesao()
{
	$("#msmadesao").val("sim");
	$("#classe").val("MaisMedicos");
	$("#requisicao").val("salvarMaisMedicos");
	$("#formulario").submit();
}

function anexarTermoAdesao()
{
	var erro = 0;
	if(!$("#arquivotermo").val()){
		erro = 1;
		alert('Favor anexar o Termo de Adesão.');
		return false;
	}
	if(erro == 0){
		$("#classe").val("MaisMedicos");
		$("#requisicao").val("salvarMaisMedicos");
		$("#formulario").submit();
	}
}

function gerarTermoAdesao()
{
	$("#gerartermoadesao").val("1");
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

function excluirTermoAdesao(arqidtermo)
{
	if(confirm("Deseja realmente exluir este termo de adesão?"))
	{
		$.ajax({
		   type: "POST",
		   url: window.location,
		   data: "requisicaoAjax=excluirTermoAdesao&classe=MaisMedicos&arqidtermo="+arqidtermo,
		   success: function(msg){
		   		window.location = window.location;
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