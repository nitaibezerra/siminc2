<?php
//$ws_tutor = new Ws_Tutor();
//dbg($ws_tutor->getSupervisoresWS());
//dbg($ws_tutor->getTutoresWS());
//die;

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

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
print '<br/>';
monta_titulo( 'Lista de Instituições Supervisoras', '&nbsp;' );

$db->cria_aba($abacod_tela, $url, $parametros);

$universidade = new Universidade();
if($_POST){
	extract($_POST);
}

?>
<style>
	.link{cursor:pointer}
</style>
<script type="text/javascript" src="../includes/JQuery/jquery-1.10.2.min.js"></script>
<form name="formulario" id="pesquisar" method="POST" action="">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
	<tr>
		<td width="25%" class="subtitulodireita">Instituição Supervisora</td>
		<td>
			<?php $universidade->comboUniversidades(); ?>
		</td>
	</tr>
	<tr>
		<td class="subtitulodireita">UF</td>
		<td>
			<?php comboEstado(); ?>
		</td>
	</tr>
	<tr>
		<td class="subtitulodireita">Município</td>
		<td id="td_muncod" >
			<?php comboMunicipio(); ?>
		</td>
	</tr>
	<!-- 
	<tr>
		<td class="subtitulodireita">Pré-Adesão</td>
		<td>
			<input type="radio" id="preadesao_sim" name="preadesao" <?php echo $_POST['preadesao'] == "sim" ? "checked='checked'" : "" ?> value="sim" /> <label for="preadesao_sim" >Sim</label>
			<input type="radio" id="preadesao_nao" name="preadesao" <?php echo $_POST['preadesao'] == "nao" ? "checked='checked'" : "" ?> value="nao" /> <label for="preadesao_nao" >Não</label>
			<input type="radio" id="preadesao_ambos" name="preadesao" <?php echo !$_POST['preadesao'] ? "checked='checked'" : "" ?> value="" /> <label for="preadesao_ambos" >Ambos</label>
		</td>
	</tr>
	 -->
	<tr>
		<td class="subtitulodireita">Adesão</td>
		<td>
			<input type="radio" id="adesao_sim" name="adesao" <?php echo $_POST['adesao'] == "sim" ? "checked='checked'" : "" ?> value="sim" /> <label for="adesao_sim" >Sim</label>
			<input type="radio" id="adesao_nao" name="adesao" <?php echo $_POST['adesao'] == "nao" ? "checked='checked'" : "" ?> value="nao" /> <label for="adesao_nao" >Não</label>
			<input type="radio" id="adesao_ambos" name="adesao" <?php echo !$_POST['adesao'] ? "checked='checked'" : "" ?> value="" /> <label for="adesao_ambos" >Ambos</label>
		</td>
	</tr>
	<tr bgcolor="#CCCCCC" >
		<td></td>
		<td>
			<input style="cursor:pointer;" type="submit" value="Pesquisar" />
			<input style="cursor:pointer;" type="button" value="Ver Todos" onClick="window.location=window.location" />
		</td>
	</tr>
</table>
</form>
<?php 
$universidade->listarUniversidades();
?>
<script>
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
function editarTermoAdesao(uniid)
{
	window.location.href="maismedicos.php?modulo=principal/maisMedicosTermoAdesao&acao=A&uniid="+uniid;
}
</script>