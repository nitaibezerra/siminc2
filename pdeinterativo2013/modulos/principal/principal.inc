<?php

ini_set( "memory_limit", "2048M" );
set_time_limit(0);

$arrPerfil = pegaPerfilGeral();

unset($_SESSION['pdeinterativo2013_vars']['usucpfdiretor']);
if($_POST['requisicao']){
	$arrDados = $_POST['requisicao']();
}

if($_POST['requisicaoAjax']){
	$_POST['requisicaoAjax']();
	exit;
}
if($_REQUEST['requisicao']){
	$_REQUEST['requisicao']();
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

if($_POST){
	extract($_POST);
}

?>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script type="text/javascript" src="../pdeinterativo2013/js/pdeinterativo2013.js"></script>
<!-- (INÍCIO) BIBLIOTECAS - PARA USO DOS COMPONENTES (CALENDÁRIO E SLIDER) -->
<script	language="javascript" type="text/javascript" src="../includes/blendtrans.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/_start.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/slider/slider.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script>	
	
	jQuery(function() {
	
		<?php if($_SESSION['pdeinterativo2013']['msg']): ?>
			alert('<?php echo $_SESSION['pdeinterativo2013']['msg'] ?>');
			<?php unset($_SESSION['pdeinterativo2013']['msg']) ?>
		<?php endif; ?>
	
	if(jQuery("[name='pditempdeescola']").val() != "PDE"){
		jQuery("[id^='filtropde']").hide();
		jQuery("[id='tramitacao']").hide();
	}
	
	jQuery("[name='pditempdeescola']").change(function() {
  		if (jQuery(this).val() != 'PDE')
  		{
  			jQuery("[id^='filtropde']").hide();
  		}
  		else
  		{
  			jQuery("[id^='filtropde']").show();
  		}
	});

	
	});
	
	function pesquisarEscola()
	{
		if(document.getElementById('tramitacaoinicio').value!='') {
			if(!validaData(document.getElementById('tramitacaoinicio'))) {
				alert('Data de tramitação do início inválida');
				return false;
			}
		}
		if(document.getElementById('tramitacaofim').value!='') {
			if(!validaData(document.getElementById('tramitacaofim'))) {
				alert('Data de tramitação do fim inválida');
				return false;
			}
		}
		
		/*if(document.getElementById('tramitacaoiniciof').value!='') {
			if(!validaData(document.getElementById('tramitacaoiniciof'))) {
				alert('Data de tramitação do início inválida');
				return false;
			}
		}
		if(document.getElementById('tramitacaofimf').value!='') {
			if(!validaData(document.getElementById('tramitacaofimf'))) {
				alert('Data de tramitação do fim inválida');
				return false;
			}
		}*/

		jQuery("[name=form_lista_escolas]").submit();
	}
	
	function visualizarPDE(pdeid)
	{
		jQuery("[name=form_lista_escolas]").attr("action","pdeinterativo2013.php?modulo=principal/identificacao&acao=A")
		jQuery("[name=pdeid]").val(pdeid);
		jQuery("[name=requisicao]").val("visualizarPDE");
		jQuery("[name=form_lista_escolas]").submit();
	}
	
	function informarPagamento(usucpf)
	{
		var url = 'pdeinterativo2013.php?modulo=principal/informarPagamento&acao=A&usucpf=' + usucpf;
		window.location.href = url;
	}
	
	function galeriaPDEInterativo(pdeid)
	{
		window.open("pdeinterativo2013.php?modulo=principal/identificacao&acao=A&exibeGaleria=1&pdeid=" + pdeid,"imagem","width=850,height=600,resizable=yes");
	}
	
	function cadastrarDiretor(cpf, inep, pdeid)
	{
		window.open("pdeinterativo2013.php?modulo=principal/cadastroDiretor&acao=A&pdeid="+pdeid+"&carregaDiretorEscola=1&requisicao=carregarDiretor&usucpf="+cpf+"&pdicodinep="+inep,"imagem","width=850,height=600,resizable=yes,scrollbars=1");
	}
	
	function wf_exibirHistorico( docid )
	{
		var url = 'http://simec-d.mec.gov.br/geral/workflow/historico.php' +
			'?modulo=principal/tramitacao' +
			'&acao=C' +
			'&docid=' + docid;
		window.open(
			url,
			'alterarEstado',
			'width=675,height=500,scrollbars=yes,scrolling=no,resizebled=no'
		);
	}
	
	
</script>
<link rel="stylesheet" type="text/css" href="../pdeinterativo2013/css/pdeinterativo2013.css"/>
<form name="form_lista_escolas" id="form_lista_escolas"  method="post" action="" >
	<input type="hidden" name="pesquisar" value="1" />
	<input type="hidden" name="requisicao" value="" />
	<input type="hidden" name="pdeid" id="pdeid" value="" />
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="SubTituloDireita"><font class="blue">Orientações</font></td>
			<td class="blue">
			<p>Esta tela exibe todas as escolas do estado ou município aptas a utilizar o PDE Interativo. Utilize os filtros abaixo para localizar uma escola ou um grupo de escolas.</p>
			<p>Caso não tenha sido atribuído um diretor para a escola, clique no link “Principal” (no canto superior esquerdo desta tela) e escolha a opção “Lista de Diretores”. Caso deseje incluir um(a) diretor(a), clique em “Inserir Diretor/ Escola” e preencha os campos, lembrando que cada escola só poderá ter um(a) diretor(a) cadastrado. Caso a escola não possua diretor(a), a Secretaria deve designar um representante que possa acessar o PDE Interativo (e que não seja diretor(a) de outra escola).</p>
			<p>Caso o cadastro do diretor esteja “Pendente” ou “Bloqueado”, clique no ícone <img src="../imagens/alterar.gif">  ao lado do nome dele e, na tela seguinte, confirme os dados, clique em “Ativo” e depois em “Salvar”.</p>
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
				<?php $dadosEsfera = array(0 => array("codigo"=>"Federal","descricao"=>"Federal"),1 => array("codigo"=>"Municipal","descricao"=>"Municipal"),2 => array("codigo"=>"Estadual","descricao"=>"Estadual")); ?>
				<?php $db->monta_combo("pdiesfera",$dadosEsfera,($arrTravaConsulta['pdiesfera'] ? "N" : "S"),"Selecione...","filtraOrgao","","","","N","","",$_POST['pdiesfera']) ?>
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
		
		<!--
		<tr id="tramitacao">
			<td class="SubTituloDireita" >Período de tramitação(Formação):</td>
			<td>
				<? echo campo_data2('tramitacaoiniciof','N','S', 'Tramitação de início', 'S' ); ?> a 
				<? echo campo_data2('tramitacaofimf','N','S', 'Tramitação de fim', 'S' ); ?>
			</td>
		</tr>
		  -->

		<tr>
			<td class="SubTituloDireita" >Programas:</td>
			<td>
				<?php $arrProgramas = array(0=>array("codigo"=>"PDE","descricao"=>"PDE Escola")); ?>
				<?php $db->monta_combo("pditempdeescola",$arrProgramas,"S","Selecione...","","","","","N","","",$_POST['pditempdeescola']) ?>
			</td>
		</tr>

		<tr  id = "filtropde0">
			<td class="SubTituloDireita" >Período de tramitação(PDE):</td>
			<td>
				<? echo campo_data2('tramitacaoinicio','N','S', 'Tramitação de início', 'S' ); ?> a 
				<? echo campo_data2('tramitacaofim','N','S', 'Tramitação de fim', 'S'); ?>
			</td>
		</tr>
		<tr id = "filtropde1">
			<td class="SubTituloDireita" >Situação atual do PDE Escola:</td>
			<td>
				<?php 
				$sql = "SELECT * FROM (
						SELECT aed.aedid::text as codigo, aeddscrealizada || ' - Com PDE' as descricao 
						FROM workflow.acaoestadodoc aed 
						INNER JOIN workflow.estadodocumento esd ON esd.esdid = aed.esdidorigem 
						WHERE tpdid=".TPD_WF_FLUXO."
						) foo
						UNION ALL ( 
						SELECT aed.aedid::text as codigo, aeddscrealizada || ' - Sem PDE' as descricao 
						FROM workflow.acaoestadodoc aed 
						INNER JOIN workflow.estadodocumento esd ON esd.esdid = aed.esdidorigem 
						WHERE tpdid=".TPD_WF_FLUXO_SEMPDE."
						)
						UNION ALL ( SELECT 'NULL' as codigo, 'Não iniciado' as descricao) 
						UNION ALL ( SELECT 'emelaboracao' as codigo, 'Em Elaboração' as descricao) "; ?>

				<?php $db->monta_combo("aedid",$sql,"S","Selecione...","","","","","N","","",$_POST['aedid']) ?>
			</td>
		</tr>
		<tr id = "filtropde3">
			<td class="SubTituloDireita" >Situação do pagamento:</td>
			<td>
				<?
				 $arrDados = array( 0 => array( "codigo" => "Pago", "descricao" => "Pago" ), 1 => array( "codigo" => "Pendente", "descricao" => "Pendente" ) );
				 echo $db->monta_combo("spasituacao",$arrDados,"S","Selecione","","","",200) 
				 ?>
			</td>
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
				<?php $db->monta_combo("estuf",$sql,($arrTravaConsulta['estuf'] ? "N" : "S"),"Selecione...","filtraMunicipio","","","","N","","") ?>
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
					<?php $db->monta_combo("muncod",$sql,($arrTravaConsulta['muncod'] ? "N" : "S"),"Selecione...","","","","","N","","") ?>
				<?php else: ?>
					<?php $db->monta_combo("muncod",array(),"N","Selecione o Estado","","","","","S") ?>
				<?php endif; ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" ></td>
			<td>
				<input type="button" name="btn_pesquisar" value="Pesquisar" onclick="pesquisarEscola()" />
				<input type="button" name="btn_ver_todos" value="Minhas escolas" onclick="window.location.href='pdeinterativo2013.php?modulo=principal/principal&acao=A&pesquisar=1'" />
			</td>
		</tr>
	</table>
</form>
<?php 
if(!$db->testa_superuser()) {
	
	if( in_array(PDEESC_PERFIL_DIRETOR, $arrPerfil) && !in_array(PDEINT_PERFIL_CONSULTA_MUNICIPAL, $arrPerfil) && !in_array(PDEINT_PERFIL_COMITE_PAR_MUNICIPAL, $arrPerfil) && !in_array(PDEINT_PERFIL_CONSULTA_ESTADUAL, $arrPerfil) && !in_array(PDEINT_PERFIL_COMITE_ESTADUAL, $arrPerfil)) {
		
		$sql = "SELECT distinct
					pdeid 
				FROM 
					pdeinterativo2013.usuarioresponsabilidade ure
				inner join
					 pdeinterativo2013.pdinterativo pde ON pde.entid = ure.entid
				WHERE 
					usucpf='".$_SESSION['usucpf']."' 
				and 
					pflcod = ".PDEESC_PERFIL_DIRETOR." 
				and 
					rpustatus = 'A'";
		$arrPdeid = $db->carregarColuna($sql);
		$_POST['arrPdeid'] = $arrPdeid;
	}
	
	if( in_array(PDEINT_PERFIL_CONSULTA_ESTADUAL, $arrPerfil) || in_array(PDEINT_PERFIL_COMITE_ESTADUAL, $arrPerfil)) {
		
		if(in_array(PDEINT_PERFIL_CONSULTA_ESTADUAL, $arrPerfil)){
			$pflcod = PDEINT_PERFIL_CONSULTA_ESTADUAL;
		}else{
			$pflcod = PDEINT_PERFIL_COMITE_ESTADUAL;
		}
		$estuf = $db->pegaUm("SELECT estuf FROM pdeinterativo2013.usuarioresponsabilidade WHERE usucpf='".$_SESSION['usucpf']."' AND estuf IS NOT NULL and pflcod = $pflcod and rpustatus = 'A' order by rpuid desc");
		
		if($estuf) {
			$_POST['estuf'] = $estuf;
		} else {
			$erromsg[] = "Usuário não possui UF vinculado";
		}
	}
	
	if(in_array(PDEINT_PERFIL_COMITE_PAR_MUNICIPAL, $arrPerfil) || in_array(PDEINT_PERFIL_CONSULTA_MUNICIPAL, $arrPerfil) || in_array(PDEINT_PERFIL_COMITE_MUNICIPAL, $arrPerfil)) {
		
		if(in_array(PDEINT_PERFIL_CONSULTA_MUNICIPAL, $arrPerfil)){
			$pflcod = PDEINT_PERFIL_CONSULTA_MUNICIPAL;
		}elseif(in_array(PDEINT_PERFIL_COMITE_PAR_MUNICIPAL, $arrPerfil)){
			$pflcod = PDEINT_PERFIL_COMITE_PAR_MUNICIPAL;
		} else {
			$pflcod = PDEINT_PERFIL_COMITE_MUNICIPAL;
		}
		
		$muncod = $db->pegaUm("SELECT muncod FROM pdeinterativo2013.usuarioresponsabilidade WHERE usucpf='".$_SESSION['usucpf']."' AND muncod IS NOT NULL  and pflcod = $pflcod  and rpustatus = 'A' order by rpuid desc");
		if($muncod) {
			$_POST['muncod'] = $muncod;
		} else {
			$erromsg[] = "Usuário não possui Município vinculado";
		}
	}

}

if(count($erromsg)==2) {
	echo "<p align=center><b>".implode("<br>",$erromsg)."</b></center>";
} else {
	listarEscolas();
}

?>