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

if($_GET['ativaTodosUsuariosComite'] == "ativa"){
	ativaTodosMembrosComite();
	dbg("Usuários Ativados!");
}


?>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script type="text/javascript" src="../pdeinterativo/js/pdeinterativo.js"></script>
<script>	
	
	jQuery(function() {
	
		<?php if($_SESSION['pdeinterativo']['msg']): ?>
			alert('<?php echo $_SESSION['pdeinterativo']['msg'] ?>');
			<?php unset($_SESSION['pdeinterativo']['msg']) ?>
		<?php endif; ?>
	});
	
	function pesquisarMembro()
	{
		jQuery("[name=form_lista_membro]").submit();
	}
	
	function editarMembroComite(usucpf)
	{
		jQuery("[name=form_lista_membro]").attr("action","pdeinterativo.php?modulo=principal/cadastroComite&acao=A")
		jQuery("[name=usucpf]").val(usucpf);
		jQuery("[name=requisicao]").val("carregarMembroComite");
		jQuery("[name=form_lista_membro]").submit();
	}
	
	function excluirMembroComite(usucpf)
	{
		if(confirm("Deseja realmente excluir o membro do comitê?")){
			jQuery("[name=usucpf]").val(usucpf);
			jQuery("[name=requisicao]").val("excluirMembroComite");
			jQuery("[name=form_lista_membro]").submit();
		}
	}
	
</script>
<link rel="stylesheet" type="text/css" href="../pdeinterativo/css/pdeinterativo.css"/>
<form name="form_lista_membro" id="form_lista_membro"  method="post" action="" >
	<input type="hidden" name="requisicao" value="" />
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
				<?php $db->monta_combo("estuf",$sql,"S","Selecione...","filtraMunicipio","","","","S","","",$_POST['estuf']) ?>
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
					<?php $db->monta_combo("muncod",$sql,"S","Selecione...","","","","","S","","",$_POST['muncod']) ?>
				<?php else: ?>
					<?php $db->monta_combo("muncod",array(),"N","Selecione o Estado","","","","","S") ?>
				<?php endif; ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Membro do Comitê:</td>
			<td id="td_usustatus" >
				<input 						    type="radio" name="usu_plfcod" <?php echo $_POST['usu_plfcod'] == "Estadual" ? "checked='checked'" : "" ?> id="usu_plfcod_E" value="Estadual" /> Estadual
				<input style="margin-left:10px" type="radio" name="usu_plfcod" <?php echo $_POST['usu_plfcod'] == "Municipal" ? "checked='checked'" : "" ?> id="usu_plfcod_M" value="Municipal" /> Municipal
				<input style="margin-left:10px" type="radio" name="usu_plfcod" <?php echo $_POST['usu_plfcod'] == "Ambos" ? "checked='checked'" : "" ?> id="usu_plfcod_A" value="Ambos" /> Ambos
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Status Geral do Usuário:</td>
			<td id="td_usustatus" >
				<input 						    type="radio" name="rdo_status" <?php echo $_POST['rdo_status'] == "A" ? "checked='checked'" : "" ?> id="rdo_status_A" value="A" /> Ativo
				<input style="margin-left:10px" type="radio" name="rdo_status" <?php echo $_POST['rdo_status'] == "P" ? "checked='checked'" : "" ?> id="rdo_status_P" value="P" /> Pendente
				<input style="margin-left:10px" type="radio" name="rdo_status" <?php echo $_POST['rdo_status'] == "B" ? "checked='checked'" : "" ?> id="rdo_status_B" value="B" /> Bloqueado
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" ></td>
			<td>
				<input type="button" name="btn_pesquisar" value="Pesquisar" onclick="pesquisarMembro()" />
				<input type="button" name="btn_ver_todos" value="Ver Todos" onclick="window.location.href='pdeinterativo.php?modulo=principal/listaComite&acao=A'" />
			</td>
		</tr>
		<tr>
			<td class="SubTituloEsquerda" ><a href="javascript:window.location.href='pdeinterativo.php?modulo=principal/cadastroComite&acao=A'"> <img src="../imagens/gif_inclui.gif" class="link img_middle"  > Inserir Membro</a></td>
			<td>
			</td>
		</tr>
	</table>
</form>
<?php listaComite() ?>