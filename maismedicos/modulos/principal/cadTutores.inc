<?php 
if($_REQUEST['requisicao'] && $_REQUEST['classe']){
	$n = new $_REQUEST['classe'];
	$n->$_REQUEST['requisicao']();
}

if($_REQUEST['requisicaoAjax'] && $_REQUEST['classe']){
	$n = new $_REQUEST['classe'];
	$n->$_REQUEST['requisicaoAjax']();
	exit;
}

if($_REQUEST['requisicaoAjax'] && $_REQUEST['funcao']){
	$_REQUEST['requisicaoAjax']();
	exit;
}

if($_GET['tutid']){
	$tutor = new Tutor($_GET['tutid']);
	$arrDados = $tutor->getDados();
	if($arrDados){
		extract($arrDados);
	}
}

$arrPerfil = pegaPerfilGeral();
if(in_array(PERFIL_CONSULTA,$arrPerfil)){
	$habilitado = "N";
}else{
	$habilitado = "S";
}

?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script type="text/javascript" src="../includes/JQuery/jquery-1.10.2.min.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script type="text/javascript">
this._closeWindows = false;
</script>
<style>
	.bold{font-weight:bold}
</style>
<?php
$uniid = $_GET['uniid'] ? $_GET['uniid'] : $_SESSION['maismedicos']['maismedicos']['uniid'];
if(!$uniid){
	echo "<script>alert('Favor informar a Universidade');window.close();</script>";
	exit;
} 
monta_titulo( ($_GET['tuttipo'] == "S" ? "Supervisor" : "Tutor") , "<img src=\"../imagens/obrig.gif\"  /> Indica campos obrigatórios.");
?>
<form name="formulario" id="formulario" method="post" action="" >
<input type="hidden" id="requisicao" name="requisicao" value="salvarTutor" />
<input type="hidden" id="classe" name="classe" value="Tutor" />
<input type="hidden" id="tutid" name="tutid" value="<?php echo $tutid ?>" />
<input type="hidden" id="uniid" name="uniid" value="<?php echo $uniid ?>" />
<input type="hidden" id="tuttipo" name="tuttipo" value="<?php echo $_GET['tuttipo'] ?>" />
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0">
<tr>
	<td width="25%" class="subtituloDireita" >CPF</td>
	<td>
		<?php
			$tutcpf = $tutcpf ? mascara_global_maismedicos_tela($tutcpf,"###.###.###-##") : null; 
			echo campo_texto('tutcpf', 'S', $habilitado, '', 20, 30, '###.###.###-##', '', 'left', '', 0, 'id="tutcpf" onchange="recuperaTutor(this.value)" ', '');
		?>
	</td>
</tr>
<tr>
	<td class="subtituloDireita" >Nome</td>
	<td>
		<?php echo campo_texto('tutnome', 'S', $habilitado, '', 40, 300, '', '', 'left', '', 0, 'id="tutnome"', ''); ?>
	</td>
</tr>
<tr>
	<td class="subtituloDireita" >Data de Nascimento</td>
	<td>
		<?php
			$tutdatanascimento = $tutdatanascimento ? formata_data($tutdatanascimento) : null; 
			echo campo_data2( 'tutdatanascimento', 'S', $habilitado, '', 'N','','' );
		?>
	</td>
</tr>
<tr>
	<td class="subtituloDireita" >E-mail</td>
	<td>
		<?php echo campo_texto('tutemail', 'S', $habilitado, '', 40, 300, '', '', 'left', '', 0, 'id="tutemail"', ''); ?>
	</td>
</tr>
<tr>
	<td class="subtituloDireita" >Telefone</td>
	<td>
		<?php echo campo_texto('tuttelefone', 'S', $habilitado, '', 20, 12, '', '', 'left', '', 0, 'id="tuttelefone"', ''); ?>
	</td>
</tr>
<tr>
	<td class="subtituloDireita" >Nome da Mãe</td>
	<td>
		<?php echo campo_texto('tutnomemae', 'S', $habilitado, '', 40, 300, '', '', 'left', '', 0, 'id="tutnomemae"', ''); ?>
	</td>
</tr>
<tr>
	<td class="subtituloDireita" >Número do Benefício</td>
	<td>
		<?php echo campo_texto('tutnumbeneficio', 'S', $habilitado, '', 10, 20, '###########', '', 'left', '', 0, 'id="tutnumbeneficio"', ''); ?>
	</td>
</tr>
<tr>
	<td colspan="2" bgcolor="#CCCCCC" class="bold" >Endereço</td>
</tr>
<tr>
	<td class="subtituloDireita" >CEP</td>
	<td>
		<?php
			$tutcep = $tutcep ? mascara_global_maismedicos_tela($tutcep,"##.###-###") : null;
			echo campo_texto('tutcep', 'S', $habilitado, '', 20, 300, '##.###-###', '', 'left', '', 0, 'id="tutcep"', ''); 
		?>
	</td>
</tr>
<tr>
	<td class="subtituloDireita" >Logradouro</td>
	<td>
		<?php echo campo_texto('tutlogradouro', 'S', $habilitado, '', 40, 300, '', '', 'left', '', 0, 'id="tutlogradouro"', ''); ?>
	</td>
</tr>
<tr>
	<td class="subtituloDireita" >Número</td>
	<td>
		<?php echo campo_texto('tutnumero', 'S', $habilitado, '', 10, 300, '', '', 'left', '', 0, 'id="tutnumero"', ''); ?>
	</td>
</tr>
<tr>
	<td class="subtituloDireita" >Complemento</td>
	<td>
		<?php echo campo_texto('tutcomplemento', 'N', $habilitado, '', 20, 300, '', '', 'left', '', 0, 'id="tutcomplemento"', ''); ?>
	</td>
</tr>
<tr>
	<td class="subtituloDireita" >Bairro</td>
	<td>
		<?php echo campo_texto('tutbairro', 'N', $habilitado, '', 40, 300, '', '', 'left', '', 0, 'id="tutbairro"', ''); ?>
	</td>
</tr>
<tr>
	<td class="subtitulodireita">UF</td>
	<td>
		<?php comboEstado(null,$habilitado); ?>
	</td>
</tr>
<tr>
	<td class="subtitulodireita">Município</td>
	<td id="td_muncod" >
		<?php comboMunicipio($estuf,$habilitado); ?>
	</td>
</tr>
<tr>
	<td colspan="2" bgcolor="#CCCCCC" class="bold" >Dados Bancários</td>
</tr>
<tr>
	<td class="subtitulodireita">Banco</td>
	<td>
		<?php comboBanco(null,$habilitado); ?>
	</td>
</tr>
<tr>
	<td class="subtituloDireita" >Agência Bancária</td>
	<td>
		<?php echo campo_texto('tutagencia', 'S', $habilitado, '', 20, 300, '', '', 'left', '', 0, 'id="tutagencia"', ''); ?>
	</td>
</tr>
<tr>
	<td class="subtituloDireita" >Conta Bancária</td>
	<td>
		<?php echo campo_texto('tutconta', 'S', $habilitado, '', 20, 300, '', '', 'left', '', 0, 'id="tutconta"', ''); ?>
	</td>
</tr>
<tr>
	<td colspan="2" bgcolor="#CCCCCC" align="center" >
		<?php if($habilitado == "S"): ?>
			<input type="button" name="btn_salvar" value="Salvar" onclick="salvarTutor()" />
		<?php endif;?>
		<input type="button" name="btn_fechar" value="Fechar" onclick="window.close()"  />
	</td>
</tr>
</table>
</form>
<script>
function filtrarMunicipio(estuf,muncod)
{
	$.ajax({
	   type: "POST",
	   url: window.location,
	   data: "requisicaoAjax=comboMunicipio&funcao=1&estuf="+estuf,
	   success: function(msg){
	   		$('#td_muncod').html( msg );
	   		if(muncod){
	   			$("[name='muncod']").val(muncod);
	   		}
	   }
	 });
}

function salvarTutor()
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
	if(erro == 0){
		$("#formulario").submit();
	}
}

function listaTutoresAjaxPaginaAnterior(uniid,tuttipo)
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
	   		$('#'+div, window.opener.document).html( msg );
	   }
	 });
}

function recuperaTutor(usucpf)
{
	$.ajax({
		url: window.location,
		type: "POST",
		dataType: "json", //Tipo de Retorno
		data: { classe: "Tutor", requisicaoAjax: "recuperaTutor", usucpf: usucpf , ajaxCPF : " "},
		success: function(json){
			if(json.tutnome){
				$("[name='tutnome']").val(json.tutnome);
				$("[name='tutdatanascimento']").val(json.tutdatanascimento);
				$("[name='tutemail']").val(json.tutemail);
				$("[name='tuttelefone']").val(json.tuttelefone);
				$("[name='tutnomemae']").val(json.tutnomemae);
				$("[name='tutcep']").val(mascaraglobal('###.###.###-##',json.tutcep));
				$("[name='tutlogradouro']").val(json.tutlogradouro);
				$("[name='tutnumero']").val(json.tutnumero);
				$("[name='tutcomplemento']").val(json.tutcomplemento);
				$("[name='tutbairro']").val(json.tutbairro);
				if(json.estuf){
					$("[name='estuf']").val(json.estuf);
					filtrarMunicipio(json.estuf,json.muncod);
				}
				$("[name='bncid']").val(json.bncid);
				$("[name='tutagencia']").val(json.tutagencia);
				$("[name='tutconta']").val(json.tutconta);
			}
         }
    });
}

$(function() {
	<?php if($_SESSION['maismedicos']['tutor']['alert']): ?>
		alert('<?php echo $_SESSION['maismedicos']['tutor']['alert'] ?>');
		listaTutoresAjaxPaginaAnterior('<?php echo $uniid ?>','<?php echo $_GET['tuttipo'] ? $_GET['tuttipo'] : "T" ?>');
		<?php unset($_SESSION['maismedicos']['tutor']['alert']) ?>
	<?php endif; ?>
});
</script>