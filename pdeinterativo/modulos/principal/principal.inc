<?php

ini_set( "memory_limit", "2048M" );
set_time_limit(0);

$arrPerfil = pegaPerfilGeral();

unset($_SESSION['pdeinterativo_vars']['usucpfdiretor']);
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

?>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script type="text/javascript" src="../pdeinterativo/js/pdeinterativo.js"></script>
<!-- (INÍCIO) BIBLIOTECAS - PARA USO DOS COMPONENTES (CALENDÁRIO E SLIDER) -->
<script	language="javascript" type="text/javascript" src="../includes/blendtrans.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/_start.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/slider/slider.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script>	
	
	jQuery(function() {
	
		<?php if($_SESSION['pdeinterativo']['msg']): ?>
			alert('<?php echo $_SESSION['pdeinterativo']['msg'] ?>');
			<?php unset($_SESSION['pdeinterativo']['msg']) ?>
		<?php endif; ?>
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
		
		if(document.getElementById('tramitacaoiniciof').value!='') {
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
		}

		jQuery("[name=form_lista_escolas]").submit();
	}
	
	function visualizarPDE(usucpf)
	{
		jQuery("[name=form_lista_escolas]").attr("action","pdeinterativo.php?modulo=principal/identificacao&acao=A")
		jQuery("[name=usucpf]").val(usucpf);
		jQuery("[name=requisicao]").val("visualizarPDE");
		jQuery("[name=form_lista_escolas]").submit();
	}
	
	function informarPagamento(usucpf)
	{
		var url = 'pdeinterativo.php?modulo=principal/informarPagamento&acao=A&usucpf=' + usucpf;
		window.location.href = url;
	}
	
	function galeriaPDEInterativo(pdeid)
	{
		window.open("pdeinterativo.php?modulo=principal/identificacao&acao=A&exibeGaleria=1&pdeid=" + pdeid,"imagem","width=850,height=600,resizable=yes");
	}
	
</script>
<link rel="stylesheet" type="text/css" href="../pdeinterativo/css/pdeinterativo.css"/>
<form name="form_lista_escolas" id="form_lista_escolas"  method="post" action="" >
	<input type="hidden" name="pesquisar" value="1" />
	<input type="hidden" name="requisicao" value="" />
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
				<?php $dadosEsfera = array(0 => array("codigo"=>"FEDERAL","descricao"=>"Federal"),1 => array("codigo"=>"Municipal","descricao"=>"Municipal"),2 => array("codigo"=>"Estadual","descricao"=>"Estadual")); ?>
				<?php $db->monta_combo("pdiesfera",$dadosEsfera,"S","Selecione...","filtraOrgao","","","","N","","",$_POST['pdiesfera']) ?>
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
			<td class="SubTituloDireita" >Período de tramitação(PDE):</td>
			<td>
				<? echo campo_data2('tramitacaoinicio','N','S', 'Tramitação de início', 'S' ); ?> a 
				<? echo campo_data2('tramitacaofim','N','S', 'Tramitação de fim', 'S' ); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Período de tramitação(Formação):</td>
			<td>
				<? echo campo_data2('tramitacaoiniciof','N','S', 'Tramitação de início', 'S' ); ?> a 
				<? echo campo_data2('tramitacaofimf','N','S', 'Tramitação de fim', 'S' ); ?>
			</td>
		</tr>

		<tr>
			<td class="SubTituloDireita" >Programas:</td>
			<td>
				<?php $arrProgramas = array(0=>array("codigo"=>"TRUE","descricao"=>"Com PDE Escola"),1=>array("codigo"=>"FALSE","descricao"=>"Sem PDE Escola")); ?>
				<?php $db->monta_combo("pditempdeescola",$arrProgramas,"S","Selecione...","","","","","N","","",$_POST['pditempdeescola']) ?>
			</td>
		</tr>

		<tr>
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
		<tr>
			<td class="SubTituloDireita" >Situação atual do Plano de Formação Continuada:</td>
			<td>
				<?php 
				$sql = "SELECT * FROM (
						SELECT aed.aedid::text as codigo, aeddscrealizada as descricao 
						FROM workflow.acaoestadodoc aed 
						INNER JOIN workflow.estadodocumento esd ON esd.esdid = aed.esdidorigem 
						WHERE tpdid=".TPD_WF_FORMACAO."
						) foo
						UNION ALL ( SELECT 'NULL' as codigo, 'Em Elaboração' as descricao)"; ?>
				

				<?php $db->monta_combo("aedidf",$sql,"S","Selecione...","","","","","N","","",$_POST['aedidf']) ?>
			</td>
		</tr>
		<? if(in_array(PDEINT_PERFIL_ADMINISTRADOR,$arrPerfil) || $db->testa_superuser()) : ?>
		<tr>
			<td class="SubTituloDireita" >Possui energia elétrica(Censo):</td>
			<td><input type="radio" name="pdienergiaeletricacenso" value="true" <?=(($_POST['pdienergiaeletricacenso']=="true")?"checked":"") ?> > Sim <input type="radio" name="pdienergiaeletricacenso" value="false" <?=(($_POST['pdienergiaeletricacenso']=="false")?"checked":"") ?> > Não</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Possui energia elétrica:</td>
			<td><input type="radio" name="pdienergiaeletrica" value="true" <?=(($_POST['pdienergiaeletrica']=="true")?"checked":"") ?> > Sim <input type="radio" name="pdienergiaeletrica" value="false" <?=(($_POST['pdienergiaeletrica']=="false")?"checked":"") ?> > Não</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Possui coordenadas geográficas:</td>
			<td><input type="radio" name="pdipossuicoordenadasgeograficas" value="true" <?=(($_POST['pdipossuicoordenadasgeograficas']=="true")?"checked":"") ?> > Sim <input type="radio" name="pdipossuicoordenadasgeograficas" value="false" <?=(($_POST['pdipossuicoordenadasgeograficas']=="false")?"checked":"") ?> > Não</td>
		</tr>
		<? endif; ?>
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
				<?php $db->monta_combo("estuf",$sql,"S","Selecione...","filtraMunicipio","","","","N","","",$_POST['estuf']) ?>
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
					<?php $db->monta_combo("muncod",$sql,"S","Selecione...","","","","","N","","",$_POST['muncod']) ?>
				<?php else: ?>
					<?php $db->monta_combo("muncod",array(),"N","Selecione o Estado","","","","","S") ?>
				<?php endif; ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" ></td>
			<td>
				<input type="button" name="btn_pesquisar" value="Pesquisar" onclick="pesquisarEscola()" />
				<input type="button" name="btn_ver_todos" value="Minhas escolas" onclick="window.location.href='pdeinterativo.php?modulo=principal/principal&acao=A&pesquisar=1'" />
			</td>
		</tr>
	</table>
</form>
<?php 
if(!$db->testa_superuser()) {
	
	if( in_array(PDEINT_PERFIL_CONSULTA_DIRETOR_PDE2013, $arrPerfil) && !in_array(PDEINT_PERFIL_COMITE_ESTADUAL, $arrPerfil) && !in_array(PDEINT_PERFIL_COMITE_MUNICIPAL, $arrPerfil)) {
		$sql = "select * from pdeinterativo.usuarioresponsabilidade where pflcod = ".PDEINT_PERFIL_CONSULTA_DIRETOR_PDE2013." and usucpf = '".$_SESSION['usucpf']."' and rpustatus = 'A' ";
		$arrEntid = $db->carregarColuna($sql);
		$_POST['entid'] = $arrEntid;
	}
	
	if( in_array(PDEINT_PERFIL_CONSULTA_ESTADUAL, $arrPerfil) || in_array(PDEINT_PERFIL_COMITE_ESTADUAL, $arrPerfil)) {
		
		if(in_array(PDEINT_PERFIL_CONSULTA_ESTADUAL, $arrPerfil)){
			$pflcod = PDEINT_PERFIL_CONSULTA_ESTADUAL;
		}else{
			$pflcod = PDEINT_PERFIL_COMITE_ESTADUAL;
		}
		$estuf = $db->pegaUm("SELECT estuf FROM pdeinterativo.usuarioresponsabilidade WHERE usucpf='".$_SESSION['usucpf']."' AND estuf IS NOT NULL and pflcod = $pflcod and rpustatus = 'A' order by rpuid desc");
		
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
		
		$muncod = $db->pegaUm("SELECT muncod FROM pdeinterativo.usuarioresponsabilidade WHERE usucpf='".$_SESSION['usucpf']."' AND muncod IS NOT NULL  and pflcod = $pflcod  and rpustatus = 'A' order by rpuid desc");
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