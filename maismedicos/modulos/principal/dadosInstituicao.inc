<?php
if($_GET['uniid']){
	$uniid = $_GET['uniid'];	
	$_SESSION['maismedicos']['maismedicos']['uniid'] = $uniid; 
}else{
	$uniid = $_SESSION['maismedicos']['maismedicos']['uniid'];
}

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

require_once APPRAIZ . "includes/cabecalho.inc";
echo '<br/>';
$db->cria_aba( $abacod_tela, $url, $parametros);
monta_titulo( "Dados da Instituição", "&nbsp;");

cabecalhoUniversidade($uniid);

$universidade = new Universidade($uniid);
$arrDados = $universidade->getDados();
if($arrDados){
	extract($arrDados);
}

$arrPerfil = pegaPerfilGeral();
if(in_array(PERFIL_CONSULTA,$arrPerfil) || in_array(PERFIL_APOIADOR,$arrPerfil) || in_array(PERFIL_TUTOR,$arrPerfil)){
	$habilitado = "N";
}else{
	$habilitado = "S";
}

?>
<style>.link{cursor:pointer}</style>
<script type="text/javascript" src="../includes/JQuery/jquery-1.10.2.min.js"></script>
<form name="formulario" id="formulario" method="post" action="" >
<input type="hidden" id="requisicao" name="requisicao" value="salvarUniversidade" />
<input type="hidden" id="classe" name="classe" value="Universidade" />
<input type="hidden" id="uniid" name="uniid" value="<?php echo $uniid?>" />
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td width="25%" class="subtituloDireita" >Nome</td>
		<td>
			<?php echo campo_texto('uninome', 'S', $habilitado, '', 60, 255, '', '', 'left', '', 0, 'id="uninome"', ''); ?>
		</td>
	</tr>
	<tr>
		<td class="subtituloDireita" >Sigla</td>
		<td>
			<?php echo campo_texto('unisigla', 'S', $habilitado, '', 20, 40, '', '', 'left', '', 0, 'id="unisigla"', ''); ?>
		</td>
	</tr>
	<tr>
		<td class="subtituloDireita" >CNPJ</td>
		<td>
			<?php
				$unicnpj = $unicnpj ? mascara_global_maismedicos_tela($unicnpj,"##.###.###/####-##") : null; 
				echo campo_texto('unicnpj', 'S', $habilitado, '', 20, 40, '##.###.###/####-##', '', 'left', '', 0, 'id="unicnpj"', ''); 
			?>
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
		<td class="subtitulodireita">Tipo de Instituição</td>
		<td>
			<?php 
			$sql = "select
				tpuid as codigo,
				tpudsc as descricao
			from
				maismedicos.tipouniversidade
			where
				tpustatus = 'A'
			order by
				tpudsc";
			$db->monta_combo("tpuid", $sql, $habilitado, "Selecione...", '', '', '', '', 'S','tpuid');
			?>
		</td>
	</tr>
	<tr>
		<td class="subtitulodireita">CPF do(a) <?php echo retornaNome($arrDados['tpuid']) ?>(a)</td>
		<td>
			<?php
				$arrDadosReitor = $universidade->recuperaReitor($uniid);
				$usucpfreitor = $arrDadosReitor ? mascara_global_maismedicos_tela($arrDadosReitor['usucpf'],"###.###.###-##") : null;
				echo campo_texto('usucpfreitor', 'S', $habilitado, '', 20, 255, '###.###.###-##', '', 'left', '', 0, 'id="usucpfreitor" onchange="carregarNomeReitor(this.value)" ', '');
			?>
		</td>
	</tr>
	<tr>
		<td class="subtitulodireita">Nome do(a) <?php echo retornaNome($arrDados['tpuid']) ?>(a)</td>
		<td id="td_nome_reitor" ><?php echo $arrDadosReitor['usunome'] ?></td>
	</tr>
	<?php if($habilitado == "S"): ?>
		<tr>
			<td colspan="2" align="center" >
				<input type="button" name="btn_salvar" value="Salvar" onclick="salvarInstituicao()" />
			</td>
		</tr>
	<?php endif; ?>
</table>
</form>
<script>
function salvarInstituicao()
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
		$("#classe").val("Universidade");
		$("#requisicao").val("salvarUniversidade");
		$("#formulario").submit();
	}
}

function filtrarMunicipio(estuf)
{
	$.ajax({
	   type: "POST",
	   url: window.location,
	   data: "requisicaoAjax=comboMunicipio&funcao=1&estuf="+estuf,
	   success: function(msg){
	   		$('#td_muncod').html( msg );
	   }
	 });
}

function carregarNomeReitor(usucpf)
{
	$.ajax({
		   type: "POST",
		   url: window.location,
		   data: "requisicaoAjax=carregarNomeReitor&classe=Universidade&usucpf="+usucpf,
		   success: function(msg){
		   		$('#td_nome_reitor').html( msg );
		   }
		 });
}

$(function() {
	<?php if($_SESSION['maismedicos']['universidade']['alert']): ?>
		alert('<?php echo $_SESSION['maismedicos']['universidade']['alert'] ?>');
		<?php unset($_SESSION['maismedicos']['universidade']['alert']) ?>
	<?php endif; ?>
});
</script>