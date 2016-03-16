<?php
if($_REQUEST['requisicao']){
	$_REQUEST['requisicao']();
}
if($_POST){
	extract($_POST);
}

include  APPRAIZ."includes/cabecalho.inc";

?>
<br />
<?php $db->cria_aba($abacod_tela,$url,''); ?>
<?php monta_titulo( 'Lista de Adesões' , " &nbsp;");  ?>
<link href="../snf/geral/snf.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script>
	function filtraCidade(estuf)
	{
		jQuery.ajax({
				   type: "POST",
				   url: 'snf.php?modulo=principal/listaAdesoes&acao=A',
				   data: "requisicao=filtraCidade&estuf=" + estuf + "&obrigatorio=N",
				   success: function(msg){
				   		jQuery("#td_muncod").html(msg);
				   }
				 });
	}
	
	function pesquisarAdesoes()
	{
		jQuery("[name='formulario_pesquisa']").submit();
	}
	
	function verTodasAdesoes()
	{
		window.location.href = window.location;
	}
	
	function visualizarAdesao(insid)
	{
		window.location.href = "snf.php?modulo=principal/visualizarAdesao&acao=A&insid=" + insid;
	}
	
</script>
<form name="formulario_pesquisa" method="post" action="" >
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td width="25%" class="SubtituloDireita" >CNPJ:</td>
			<td>
				<?php echo campo_texto("inscnpj","N","S","",40,20,"##.###.###/####-##","","","","","","",$inscnpj) ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >Nome da Instituição:</td>
			<td>
				<?php echo campo_texto("insnome","N","S","",60,255,"","","","","","","",$insnome) ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >Sigla:</td>
			<td>
				<?php echo campo_texto("inssigla","N","S","",40,50,"","","","","","","",$inssigla) ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >Estado:</td>
			<td>
				<?php $sql = "	select
									estuf as codigo,
									estdescricao as descricao
								from
									territorios.estado
								order by
									estdescricao" ?>
				<?php $db->monta_combo("estuf",$sql,"S","Selecione...","filtraCidade","","","","","","",$estuf) ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >Município:</td>
			<td id="td_muncod" >
			<?php
				$sql = "select
			   				muncod as codigo,
			   				mundescricao as descricao
			   			from
			   				territorios.municipio
			   			where
			   				estuf = '$estuf'
			   			order by
			   				mundescricao";
				if(!$estuf){
					$db->monta_combo("muncod",$sql,"N","Selecione...","","","","200","N","","",$muncod);
				}else{
			   		$db->monta_combo("muncod",$sql,"S","Selecione...","","","","200","N","","",$muncod);
				}
			?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >Nome do Dirigente:</td>
			<td><?php echo campo_texto("dirnome","N","S","",60,255,"","","","","","","",$dirnome) ?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >CPF do Dirigente:</td>
			<td><?php echo campo_texto("dircpf","N","S","",16,14,"###.###.###-##","","","","","","",$dircpf) ?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >Aprovado:</td>
			<td>
				<input type="radio" name="teraprovacao" <?php echo $teraprovacao == "S" ? "checked='checked'" : "" ?> value="S" /> Sim
				<input type="radio" name="teraprovacao" <?php echo $teraprovacao == "N" ? "checked='checked'" : "" ?> value="N" /> Não
				<input type="radio" name="teraprovacao" <?php echo $teraprovacao == "" ? "checked='checked'" : "" ?> value="" /> Todos
			</td>
		</tr>
		<tr>
			<td class="SubtituloTabela center" colspan="2"  >
				<input type="button" name="btn_pesquisar" value="Pesquisar" onclick="pesquisarAdesoes()" />
				<input type="button" name="btn_ver_todos" value="Ver Todas" onclick="verTodasAdesoes()" />
			</td>
		</tr>
	</table>
</form>
<?php listaAdesoes() ?>