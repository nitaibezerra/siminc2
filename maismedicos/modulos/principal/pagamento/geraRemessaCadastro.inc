<?php
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
monta_titulo( "Remessa de Cadastro", "Selecione os tutores / supervisores abaixo para realizar o cadastro.");

$arrPerfil = pegaPerfilGeral();
if(in_array(PERFIL_CONSULTA,$arrPerfil)){
	$habilitado = "N";
}else{
	$habilitado = "S";
}
?>
<style>
	.link{cursor:pointer}
	.bold{font-weight:bold}
	.center{text-align:center}
</style>
<script type="text/javascript" src="../includes/JQuery/jquery-1.10.2.min.js"></script>
<form name="formulario" id="formulario" method="post" action="" >
<input type="hidden" id="requisicao" name="requisicao" value="salvarMaisMedicos" />
<input type="hidden" id="classe" name="classe" value="RemessaCabecalho" />
<?php 
	$sql = "select
				*
			from
				maismedicos.tutor tut
			inner join
				maismedicos.universidade uni ON uni.uniid = tut.uniid
			where
				tutstatus = 'A'
			and
				tutcpf is not null
			and
				tutdatanascimento is not null
			and
				tutnomemae is not null
			and
				tutagencia is not null
			and
				tutvalidade is true
			and
				unistatus = 'A'
			--and
				--tut.tutid not in (select det.tutid from maismedicos.remessadetalhe det where (det.strid is null or det.strid = ".STRID_REGISTRO_OK.") AND det.cs_tipo_lote = '".TPLID_CADASTRO."') 
			order by
				tutnome";

	$arrTutores = $db->carregar($sql);
	
?>
<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="95%"  >
	<?php if($arrTutores): ?>
		<tr bgcolor="#c9c9c9"  >
			<td class="bold center" >Selecionar <br/> 
			<?php if($habilitado == "S"): ?>
				<input type="checkbox" name="tutidtodos" value="" checked="checked" onclick="selecionarTodos(this)" />
			<?php endif; ?>
			</td>
			<td class="bold center" >Nome</td>
			<td class="bold center" >CPF</td>
			<td class="bold center" >Instituição</td>
		</tr>
		<?php foreach($arrTutores as $n => $t): ?>
			<tr bgcolor="<?php echo $n%2 ? "#FFFFFF" : "" ?>" >
				<td class="center" >
				<?php if($habilitado == "S"): ?>
					<input type="checkbox" name="tutid[]" checked="checked" value="<?php echo $t['tutid']; ?>" />
				<?php endif; ?>
				</td>
				<td><?php echo $t['tutnome']; ?></td>
				<td><?php echo mascara_global_maismedicos_tela($t['tutcpf'],"###.###.###-##") ?></td>
				<td><?php echo $t['uninome']; ?></td>
			</tr>
		<?php endforeach; ?>
		<?php if($habilitado == "S"): ?>
		<tr>
			<td align="center" class="bold" >
				Total 
			</td>
			<td class="bold" >
				<?php echo number_format(count($arrTutores),0,',','.') ?>
			</td>
		</tr>
		<tr>
			<td colspan="4" align="center" >
				<input type="button" value="Processar Cadastro" name="btn_cadastro" onclick="processarCadastro()" /> 
			</td>
		</tr>
		<?php endif; ?>
	<?php else: ?>
		<tr>
			<td colspan="2" align="center" >Não existem tutores / supervisores pendentes para cadastro.</td>
		</tr>
	<?php endif; ?>
</table>
</form>
<script>
function processarCadastro()
{
	var num = $( "[name='tutid[]']:checked" ).length;
	if(num > 0){
		$("#classe").val("RemessaCabecalho");
		$("#requisicao").val("processarCadastro");
		$("#formulario").submit();
	}else{
		alert('Selecione pelo menos 1 tutor para processar o cadastro.');
	}
}

function selecionarTodos(obj)
{
	if( $(obj).is(':checked') ){
		$("[name='tutid[]']").prop('checked',true);
	}else{
		$("[name='tutid[]']").prop('checked',false);
	}
}

$(function() {
	<?php if($_SESSION['maismedicos']['remessa']['alert']): ?>
		alert('<?php echo $_SESSION['maismedicos']['remessa']['alert'] ?>');
		<?php unset($_SESSION['maismedicos']['remessa']['alert']) ?>
	<?php endif; ?>
});
</script>