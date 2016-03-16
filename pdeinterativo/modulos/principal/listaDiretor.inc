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

$arrPerfil = pegaPerfilGeral();

if(!$db->testa_superuser()) {
	if(in_array(PDEINT_PERFIL_COMITE_MUNICIPAL,$arrPerfil) || in_array(PDEINT_PERFIL_COMITE_PAR_MUNICIPAL,$arrPerfil)){
		$arrTravaConsulta['pdiesfera'] = true;
		$arrTravaConsulta['muncod']    = true;
		$arrTravaConsulta['estuf'] 	   = true;
	}
	if(in_array(PDEINT_PERFIL_COMITE_ESTADUAL,$arrPerfil) || in_array(PDEINT_PERFIL_COMITE_PAR_ESTADUAL,$arrPerfil)){
		$arrTravaConsulta['pdiesfera'] = true;
		$arrTravaConsulta['estuf'] 	   = true;
	}
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
	
	function pesquisarDiretor()
	{
		jQuery("[name=form_lista_diretor]").submit();
	}
	
	function editarDiretor(usucpf,pdicodinep)
	{
		jQuery("[name=form_lista_diretor]").attr("action","pdeinterativo.php?modulo=principal/cadastroDiretor&acao=A")
		jQuery("[name=pdicodinep]").val(pdicodinep);
		jQuery("[name=usucpf]").val(usucpf);
		jQuery("[name=requisicao]").val("carregarDiretor");
		jQuery("[name=form_lista_diretor]").submit();
	}
	
	function excluirDiretor(usucpf,pdicodinep)
	{
		if(confirm("Deseja realmente excluir o diretor?")){
			jQuery("[name=usucpf]").val(usucpf);
			jQuery("[name=requisicao]").val("excluirDiretor");
			jQuery("[name=form_lista_diretor]").submit();
		}
	}
	
</script>
<link rel="stylesheet" type="text/css" href="../pdeinterativo/css/pdeinterativo.css"/>
<form name="form_lista_diretor" id="form_lista_diretor"  method="post" action="" >
	<input type="hidden" name="requisicao" value="" />
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td width="25%" class="SubTituloDireita" >Código INEP:</td>
			<td class="blue">
			<p>Esta tela exibe a situação do cadastro de todas as escolas do estado ou município e seus(suas) respectivos(a)s diretores(as).</p>
			<p>Utilize os filtros abaixo para localizar o cadastro de um(a) diretor(a) uma escola ou de um grupo de diretores(as).Caso não tenha sido atribuído um(a) diretor(a) para alguma escola, clique em “Inserir Diretor/ Escola”, preencha os campos e clique em “Salvar”. Mas lembre-se: cada escola só poderá ter um(a) diretor(a) cadastrado. Caso a escola não possua diretor(a), a Secretaria deve designar um representante que possa acessar o PDE Interativo (e que não seja diretor(a) de outra escola).</p>
			<p>Caso o cadastro do diretor esteja “Pendente” ou “Bloqueado”, clique no ícone   ao lado do nome e, na tela seguinte, confirme os dados, clique em “Ativo” e depois em “Salvar”.</p>
			<p><font color="red">ATENÇÃO: Em todos os estados e nos municípios que possuem pelo menos uma escola priorizada pelo PDE Escola, os cadastros e acessos de todos(as) os(as) diretores(as) da rede ao PDE Interativo só poderão ser gerenciados pela equipe do Comitê de Análise e Aprovação. Nos municípios que não possuem nenhuma escola priorizada pelo PDE Escola, os cadastros e acessos de todos(as) os(as) diretores(as) da rede só poderão gerenciados pelos membros da equipe do PAR que possuem o perfil "Equipe Municipal" e "Equipe Municipal Aprovação". 
			   </font></p>
			</td>
		</tr>

		<tr>
			<td width="25%" class="SubTituloDireita" >Código INEP:</td>
			<td><?php echo campo_texto("pdicodinep","N","S","CPF","16","14","[#]","","","","","","",$_POST['pdicodinep'])?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Nome da Escola:</td>
			<td><?php echo campo_texto("pdenome","N","S","Nome","40","40","","","","","","","",$_POST['pdenome'])?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Tipo de Esfera da Escola:</td>
			<td id="td_tpocod">
				<?php $dadosEsfera = array(0 => array("codigo"=>"FEDERAL","descricao"=>"Federal"),1 => array("codigo"=>"Municipal","descricao"=>"Municipal"),2 => array("codigo"=>"Estadual","descricao"=>"Estadual")); ?>
				<?php $db->monta_combo("pdiesfera",$dadosEsfera,($arrTravaConsulta['pdiesfera'] ? "N" : "S"),"Selecione...","filtraOrgao","","","","N","","",$_POST['pdiesfera']) ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Programas:</td>
			<td>
				<?php $arrProgramas = array(0=>array("codigo"=>"TRUE","descricao"=>"Com PDE Escola"),1=>array("codigo"=>"FALSE","descricao"=>"Sem PDE Escola")); ?>
				<?php $db->monta_combo("pditempdeescola",$arrProgramas,($arrTravaConsulta['pditempdeescola'] ? "N" : "S"),"Selecione...","","","","","N","","",$_POST['pditempdeescola']) ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Diretor:</td>
			<td>
				<?php $arrDiretor = array(0=>array("codigo"=>"diretor.usucpf IS NULL","descricao"=>"Escolas sem diretores"),1=>array("codigo"=>"diretor.usucpf IS NOT NULL","descricao"=>"Escolas com diretores")); ?>
				<?php $db->monta_combo("filtrodiretor",$arrDiretor,"S","Selecione...","","","","","N","","",$_POST['filtrodiretor']) ?>
			</td>
		</tr>

		<tr>
			<td width="25%" class="SubTituloDireita" >CPF do Diretor:</td>
			<td><?php echo campo_texto("usucpf","N","S","CPF","16","14","###.###.###-##","","","","","","",$_POST['usucpf'])?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Nome do Diretor:</td>
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
				<input type="button" name="btn_pesquisar" value="Pesquisar" onclick="pesquisarDiretor()" />
				<input type="button" name="btn_ver_todos" value="Ver Todos" onclick="window.location.href='pdeinterativo.php?modulo=principal/listaDiretor&acao=A'" />
			</td>
		</tr>
		<!-- 
		<tr>
			<td class="SubTituloEsquerda" ><a href="javascript:window.location.href='pdeinterativo.php?modulo=principal/cadastroDiretor&acao=A'"> <img src="../imagens/gif_inclui.gif" class="link img_middle"  > Inserir Diretor / Escola</a></td>
			<td>
			</td>
		</tr>
		 -->
	</table>
</form>
<?php listaDiretor() ?>