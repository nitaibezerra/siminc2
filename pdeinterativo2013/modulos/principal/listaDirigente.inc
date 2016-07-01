<?php
set_time_limit(0); 
ini_set("memory_limit", "1024M");

if($_POST['requisicao']){
	$arrDados = $_POST['requisicao']();
}

if($_POST['requisicaoAjax']){
	$_POST['requisicaoAjax']();
	exit;
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$db->cria_aba($abacod_tela,$url,'');
$titulo = str_replace("...","",$_SESSION['mnudsc']);
monta_titulo( $titulo, '&nbsp' );

$arrFiltros = recuperaFiltroPerfil();
if($arrFiltros){
	foreach($arrFiltros as $chave => $valor){
		$_POST[$chave] = $valor;
		$arrTravaConsulta[$chave] = $valor;
	}
}

?>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script type="text/javascript" src="../pdeinterativo2013/js/pdeinterativo2013.js"></script>
<script>	
	
	jQuery(function() {
	
		<?php if($_SESSION['pdeinterativo2013']['msg']): ?>
			alert('<?php echo $_SESSION['pdeinterativo2013']['msg'] ?>');
			<?php unset($_SESSION['pdeinterativo2013']['msg']) ?>
		<?php endif; ?>
	});
	
	function pesquisarDirigente()
	{
		jQuery("[name=form_lista_dirigente]").submit();
	}
	
	function editarDirigente(usucpf,pflcod)
	{
		jQuery("[name=form_lista_dirigente]").attr("action","pdeinterativo2013.php?modulo=principal/cadastroDirigente&acao=A")
		jQuery("[name=usucpf]").val(usucpf);
		jQuery("[name=pflcod]").val(pflcod);
		jQuery("[name=requisicao]").val("carregarDirigente");
		jQuery("[name=form_lista_dirigente]").submit();
	}
	
	function excluirDirigente(usucpf,pflcod)
	{
		if(confirm("Deseja realmente excluir?")){
			jQuery("[name=usucpf]").val(usucpf);
			jQuery("[name=pflcod]").val(pflcod);
			jQuery("[name=requisicao]").val("excluirDirigente");
			jQuery("[name=form_lista_dirigente]").submit();
		}
	}

	function historicoUsuario(usucpf)
	{
		janela("pdeinterativo2013.php?modulo=principal/popUpHistoricoUsuario&acao=A&usucpf="+usucpf,700,400,"Histórico do Usuário");
	}
</script>
<link rel="stylesheet" type="text/css" href="../pdeinterativo2013/css/pdeinterativo2013.css"/>
<form name="form_lista_dirigente" id="form_lista_dirigente"  method="post" action="" >
	<input type="hidden" name="requisicao" value="" />
	<input type="hidden" name="pflcod" id="pflcod" value="" />
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td width="25%" class="SubTituloDireita" >CPF:</td>
			<td><?php echo campo_texto("usucpf","N","S","CPF","16","14","###.###.###-##","","","","","","",$_POST['usucpf'])?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Nome:</td>
			<td><?php echo campo_texto("usunome","N","S","Nome","40","40","","","","","","","",$_POST['usunome'])?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Estado:</td>
			<td>
				<?php $sql = "	select
									estuf as codigo,
									estdescricao as descricao
								from
									territorios.estado
								order by
									estdescricao" ?>
				<?php $db->monta_combo("estuf",$sql,($arrTravaConsulta['estuf'] ? "N" : "S"),"Selecione...","filtraMunicipio","","","","N","","",$_POST['estuf']) ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Município:</td>
			<td id="td_muncod">
				<?php if($_POST['estuf']): ?>
					<?php $sql = "	select
										muncod as codigo,
										mundescricao as descricao
									from
										territorios.municipio
									where
										estuf = '{$_POST['estuf']}'
									order by
										mundescricao" ?>
					<?php $db->monta_combo("muncod",$sql,($arrTravaConsulta['muncod'] ? "N" : "S"),"Selecione...","","","","","N","","",$_POST['muncod']) ?>
				<?php else: ?>
					<?php $db->monta_combo("muncod",array(),"N","Selecione o Estado","","","","","S") ?>
				<?php endif; ?>
			</td>
		</tr>
		<?php if(!$arrTravaConsulta['usu_plfcod']): ?>
			<tr>
				<td class="SubTituloDireita" >Dirigente:</td>
				<td id="td_usustatus" >
					<input 						    type="radio" name="usu_plfcod" <?php echo $_POST['usu_plfcod'] == "Estadual" ? "checked='checked'" : "" ?> id="usu_plfcod_E" value="Estadual" /> Estadual
					<input style="margin-left:10px" type="radio" name="usu_plfcod" <?php echo $_POST['usu_plfcod'] == "Municipal" ? "checked='checked'" : "" ?> id="usu_plfcod_M" value="Municipal" /> Municipal
					<input style="margin-left:10px" type="radio" name="usu_plfcod" <?php echo $_POST['usu_plfcod'] == "Ambos" ? "checked='checked'" : "" ?> id="usu_plfcod_A" value="Ambos" /> Ambos
				</td>
			</tr>
		<?php endif; ?>
		<tr>
			<td class="SubTituloDireita" >Status Geral do Usuário:</td>
			<td id="td_usustatus" >
				<input 						    type="radio" name="rdo_status" <?php echo $_POST['rdo_status'] == "A" ? "checked='checked'" : "" ?> id="rdo_status_A" value="A" /> Ativo
				<input style="margin-left:10px" type="radio" name="rdo_status" <?php echo $_POST['rdo_status'] == "P" ? "checked='checked'" : "" ?> id="rdo_status_P" value="P" /> Pendente
				<input style="margin-left:10px" type="radio" name="rdo_status" <?php echo $_POST['rdo_status'] == "B" ? "checked='checked'" : "" ?> id="rdo_status_B" value="B" /> Bloqueado
				<input style="margin-left:10px" type="radio" name="rdo_status" <?php echo $_POST['rdo_status'] == "I" ? "checked='checked'" : "" ?> id="rdo_status_I" value="I" /> Inativo
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" ></td>
			<td>
				<input type="button" name="btn_pesquisar" value="Pesquisar" onclick="pesquisarDirigente()" />
				<input type="button" name="btn_ver_todos" value="Ver Todos" onclick="window.location.href='pdeinterativo2013.php?modulo=principal/listaDirigente&acao=A'" />
			</td>
		</tr>
		<tr>
			<td class="SubTituloEsquerda" ><a href="javascript:window.location.href='pdeinterativo2013.php?modulo=principal/cadastroDirigente&acao=A'"> <img src="../imagens/gif_inclui.gif" class="link img_middle"  > Inserir</a></td>
			<td>
			</td>
		</tr>
	</table>
</form>
<?php
	listaDirigentes();
?>