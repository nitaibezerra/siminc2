<?
ini_set("memory_limit", "1024M");
set_time_limit(0);

include_once '_funcoes_centralacompanhamento.php';

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}


include  APPRAIZ."includes/cabecalho.inc";
echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>';
echo '<script language="javascript" type="text/javascript" src="./js/sispacto2.js"></script>';

echo "<br>";

$fpbid_padrao = $db->pegaUm("SELECT fpbid FROM sispacto2.folhapagamento WHERE fpbanoreferencia='".date("Y")."' AND fpbmesreferencia='".date("m")."'");

?>
<script>
function acessarCadastroOrientadores(esdid,esfera) {
	window.location='sispacto2.php?modulo=principal/coordenadorlocal/listacoordenadorlocal&acao=A&esdid='+esdid+'&esfera='+esfera;
}

function acessarComposicaoTurmas(esdid,esfera) {
	window.location='sispacto2.php?modulo=principal/coordenadorlocal/listacoordenadorlocal&acao=A&esdidcomposicaoturmas='+esdid+'&esfera='+esfera;
}

function acessarUniversidades(esdid) {
	window.location='sispacto2.php?modulo=principal/universidade/listauniversidade&acao=A&esdid='+esdid;
}

function acessarUniversidadesFormacaoInicial(esdid) {
	window.location='sispacto2.php?modulo=principal/universidade/listauniversidade&acao=A&esdidformacaoinicial='+esdid;
}


function selecionarUniversidadeMensario(x) {
	jQuery('#situacaomensario').html('Carregando...');
	var uncid = jQuery('#uncid_sit').val();
	var fpbid = jQuery('#fpbid_sit').val();
	ajaxatualizar('requisicao=exibirSituacaoMensario&uncid='+uncid+'&fpbid='+fpbid,'situacaomensario');
}

function selecionarUniversidadeAcesso(uncid) {
	ajaxatualizar('requisicao=exibirAcessoUsuarioSimec&uncid='+uncid,'acessousuario');
}

function selecionarUniversidadePagamento(x) {
	jQuery('#situacaopagamentos').html('Carregando...');
	var uncid = jQuery('#uncid_pag').val();
	var fpbid = jQuery('#fpbid_pag').val();
	ajaxatualizar('requisicao=exibirSituacaoPagamento&uncid='+uncid+'&fpbid='+fpbid,'situacaopagamentos');
}

function detalharCadastroOrientadores(esdid, esfera, obj) {
	var tabela = obj.parentNode.parentNode.parentNode;
	var linha = obj.parentNode.parentNode;
	if(obj.title=="mais") {
		divCarregando();
		obj.title    = "menos";
		obj.src      = "../imagens/menos.gif";
		var nlinha   = tabela.insertRow(linha.rowIndex);
		var ncol     = nlinha.insertCell(0);
		ncol.colSpan = 5;
		ncol.id      = 'dtl_coluna_'+nlinha.rowIndex;
		ajaxatualizar('requisicao=carregarDetalhesCadastroOrientadores&esdid='+esdid+'&esfera='+esfera,ncol.id);
		divCarregado();
	} else {
		obj.title    = "mais";
		obj.src      = "../imagens/mais.gif";
		tabela.deleteRow(linha.rowIndex);
	}
}

function detalharStatusUsuarios(pflcod, obj) {
	var tabela = obj.parentNode.parentNode.parentNode;
	var linha = obj.parentNode.parentNode;
	if(obj.title=="mais") {
		obj.title    = "menos";
		obj.src      = "../imagens/menos.gif";
		var nlinha   = tabela.insertRow(linha.rowIndex);
		var ncol     = nlinha.insertCell(0);
		ncol.colSpan = 7;
		ncol.id      = 'dtl_coluna_'+nlinha.rowIndex;
		ajaxatualizar('requisicao=carregarDetalhesStatusUsuarios&pflcod='+pflcod,ncol.id);
	} else {
		obj.title    = "mais";
		obj.src      = "../imagens/mais.gif";
		tabela.deleteRow(linha.rowIndex);
	}
}


function detalharAvaliacoesUsuario(pflcod, fpbid, obj) {
	var tabela = obj.parentNode.parentNode.parentNode;
	var linha = obj.parentNode.parentNode;
	if(obj.title=="mais") {
		obj.title    = "menos";
		obj.src      = "../imagens/menos.gif";
		var nlinha   = tabela.insertRow(linha.rowIndex);
		var ncol     = nlinha.insertCell(0);
		ncol.colSpan = 7;
		ncol.id      = 'dtl_coluna_'+nlinha.rowIndex;
		ajaxatualizar('requisicao=carregarDetalhesAvaliacoesUsuarios&pflcod='+pflcod+'&fpbid='+fpbid,ncol.id);
	} else {
		obj.title    = "mais";
		obj.src      = "../imagens/mais.gif";
		tabela.deleteRow(linha.rowIndex);
	}
}

function detalharAbrangenciaEstado(estuf, obj) {
	var tabela = obj.parentNode.parentNode.parentNode;
	var linha = obj.parentNode.parentNode;
	if(obj.title=="mais") {
		obj.title    = "menos";
		obj.src      = "../imagens/menos.gif";
		var nlinha   = tabela.insertRow(linha.rowIndex);
		var ncol     = nlinha.insertCell(0);
		ncol.colSpan = 6;
		ncol.id      = 'dtl2_coluna_'+nlinha.rowIndex;
		ajaxatualizar('requisicao=carregarDetalhesAbrangenciaEstado&estuf='+estuf,ncol.id);
	} else {
		obj.title    = "mais";
		obj.src      = "../imagens/mais.gif";
		tabela.deleteRow(linha.rowIndex);
	}
}

function detalharDetalhesPagamentosUsuarios(pflcod, obj) {
	var tabela = obj.parentNode.parentNode.parentNode;
	var linha = obj.parentNode.parentNode;
	if(obj.title=="mais") {
		divCarregando();
		var param = '';
		if(jQuery('#uncid_pag').val()!='') {
			param = '&uncid='+jQuery('#uncid_pag').val();
		}
		if(jQuery('#fpbid_pag').val()!='') {
			param = '&fpbid='+jQuery('#fpbid_pag').val();
		}
		obj.title    = "menos";
		obj.src      = "../imagens/menos.gif";
		var nlinha   = tabela.insertRow(linha.rowIndex);
		var ncol     = nlinha.insertCell(0);
		ncol.colSpan = 18;
		ncol.id      = 'dtl2_coluna_'+nlinha.rowIndex;
		ajaxatualizar('requisicao=detalharDetalhesPagamentosUsuarios&pflcod='+pflcod+param,ncol.id);
		divCarregado();
	} else {
		obj.title    = "mais";
		obj.src      = "../imagens/mais.gif";
		tabela.deleteRow(linha.rowIndex);
	}
}

function detalharMunicipiosturmaNaoFechadas(uncid, obj) {
	var tabela = obj.parentNode.parentNode.parentNode;
	var linha = obj.parentNode.parentNode;
	if(obj.title=="mais") {
		divCarregando();
		obj.title    = "menos";
		obj.src      = "../imagens/menos.gif";
		var nlinha   = tabela.insertRow(linha.rowIndex);
		var ncol     = nlinha.insertCell(0);
		ncol.colSpan = 5;
		ncol.id      = 'dtl2_coluna_'+nlinha.rowIndex;
		ajaxatualizar('requisicao=exibirMunicipiosNaoFechados&uncid='+uncid,ncol.id);
		divCarregado();
	} else {
		obj.title    = "mais";
		obj.src      = "../imagens/mais.gif";
		tabela.deleteRow(linha.rowIndex);
	}
}


function selecionarUniversidadePagamentoPorcent(x) {
	jQuery('#porcentagempagamentos').html('Carregando...');
	var uncid = jQuery('#uncid_por').val();
	ajaxatualizar('requisicao=exibirPorcentagemPagamento&uncid='+uncid,'porcentagempagamentos');
}

function carregarQtdTurmas_<?=PFL_FORMADORIES ?>(filtroconsulta) {
	ajaxatualizarAsync('requisicao=mediaTurmasPerfil&pflcod=<?=PFL_FORMADORIES ?>&filtroconsulta='+filtroconsulta,'mediaTurmasPerfilFormador');
}
function carregarQtdTurmas_<?=PFL_ORIENTADORESTUDO ?>(filtroconsulta) {
	ajaxatualizarAsync('requisicao=mediaTurmasPerfil&pflcod=<?=PFL_ORIENTADORESTUDO ?>&filtroconsulta='+filtroconsulta,'mediaTurmasPerfilOrientadoresEstudo');
}

function carregarHierarquia(req) {
	ajaxatualizarAsync('requisicao='+req,'correlacaoParentescoOEePA');
	
}

</script>
<?
monta_titulo( "Central de Acompanhamento", "Informações sobre o andamento do projeto");

$menu[] = array("id" => 1, "descricao" => 'Etapa Projeto', "link" => '/sispacto2/sispacto2.php?modulo=centralacompanhamento&acao=A&aba=projeto');
$menu[] = array("id" => 2, "descricao" => 'Etapa Execução', "link" => '/sispacto2/sispacto2.php?modulo=centralacompanhamento&acao=A&aba=execucao');
$menu[] = array("id" => 3, "descricao" => 'Etapa Auditoria', "link" => '/sispacto2/sispacto2.php?modulo=centralacompanhamento&acao=A&aba=auditoria');
$menu[] = array("id" => 3, "descricao" => 'Etapa Resumo', "link" => '/sispacto2/sispacto2.php?modulo=centralacompanhamento&acao=A&aba=resumo');

echo "<br>";

if(!$_REQUEST['aba']) $_REQUEST['aba']='projeto';

echo montarAbasArray($menu, '/sispacto2/sispacto2.php?modulo=centralacompanhamento&acao=A&aba='.$_REQUEST['aba']);

if($_REQUEST['aba']=='projeto') :
?>
<script>
jQuery(document).ready(function() {
	ajaxatualizarAsync('requisicao=composicaoTurmasMunicipal','composicaoTurmasMunicipal');
	ajaxatualizarAsync('requisicao=composicaoTurmasEstadual','composicaoTurmasEstadual');

	ajaxatualizarAsync('requisicao=cadastramentoOEMunicipal','cadastramentoOEMunicipal');
	ajaxatualizarAsync('requisicao=cadastramentoOEEstadual','cadastramentoOEEstadual');

	ajaxatualizarAsync('requisicao=tipoOECadastrados','tipoOECadastrados');
	ajaxatualizarAsync('requisicao=tipoPACadastrados','tipoPACadastrados');

	ajaxatualizarAsync('requisicao=preenchimentoProjetoIES','preenchimentoProjetoIES');
	ajaxatualizarAsync('requisicao=preenchimentoFormacaoInicialIES','preenchimentoFormacaoInicialIES');

	ajaxatualizarAsync('requisicao=abrangenciaIES','abrangenciaIES');
	ajaxatualizarAsync('requisicao=abrangenciaEstado','abrangenciaEstado');
	
	ajaxatualizarAsync('requisicao=orcamentoIES','orcamentoIES');
	ajaxatualizarAsync('requisicao=remuneracaoBolsistas','remuneracaoBolsistas');
	
	
});
</script>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloCentro" valign="top" width="50%">
	
	<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
	<tr>
		<td class="SubTituloCentro">Coordenador Local</td>
	</tr>
	<tr>
		<td>
		<table width="100%">
		<tr>
			<td width="50%" valign="top" id="composicaoTurmasMunicipal"><img src="../imagens/carregando.gif" align="absmiddle"> Carregando...</td>
			<td width="50%" valign="top" id="composicaoTurmasEstadual"><img src="../imagens/carregando.gif" align="absmiddle"> Carregando...</td>
		</tr>
		</table>
		</td>
	</tr>
	<tr>
		<td>
		<table width="100%">
		<tr>
			<td width="50%" valign="top" id="cadastramentoOEMunicipal"><img src="../imagens/carregando.gif" align="absmiddle"> Carregando...</td>
			<td width="50%" valign="top" id="cadastramentoOEEstadual"><img src="../imagens/carregando.gif" align="absmiddle"> Carregando...</td>
		</tr>
		</table>
		
		</td>
	</tr>

	<tr>
		<td>
		<table width="100%">
		<tr>
			<td width="60%" valign="top" id="tipoOECadastrados"><img src="../imagens/carregando.gif" align="absmiddle"> Carregando...</td>
			<td width="40%" valign="top" id="tipoPACadastrados"><img src="../imagens/carregando.gif" align="absmiddle"> Carregando...</td>
		</tr>
		</table>
		
		</td>
	</tr>

	</table>	
	
	</td>
	
	<td class="SubTituloCentro" valign="top" width="50%">
	
	<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
	<tr>
		<td class="SubTituloCentro">Coordenador IES</td>
	</tr>
	<tr>
		<td>
		<table width="100%">
		<tr>
			<td width="50%" valign="top" id="preenchimentoProjetoIES"><img src="../imagens/carregando.gif" align="absmiddle"> Carregando...</td>
			<td width="50%" valign="top" id="preenchimentoFormacaoInicialIES"><img src="../imagens/carregando.gif" align="absmiddle"> Carregando...</td>
		</tr>
		</table>
			
		</td>
	</tr>

	<tr>
		<td>
		<table width="100%">
		<tr>
			<td width="50%" valign="top" id="abrangenciaIES"><img src="../imagens/carregando.gif" align="absmiddle"> Carregando...</td>
			<td width="50%" valign="top" id="abrangenciaEstado"><img src="../imagens/carregando.gif" align="absmiddle"> Carregando...</td>
			
		</tr>
		</table>
		
		</td>
	</tr>
	
	<tr>
		<td>
	
		<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
		<tr>
			<td class="SubTituloCentro">Informações sobre pagamentos</td>
		</tr>
		<tr>
			<td>
			<table width="100%">
			<tr>
				<td width="50%" valign="top" id="orcamentoIES"><img src="../imagens/carregando.gif" align="absmiddle"> Carregando...</td>
				<td width="50%" valign="top" id="remuneracaoBolsistas"><img src="../imagens/carregando.gif" align="absmiddle"> Carregando...</td>
			</tr>
			</table>
			
			</td>
		</tr>
	
		</table>
		
		</td>
	</tr>

	</table>	
	
	</td>
</tr>
</table>
<? elseif($_REQUEST['aba']=='execucao') : ?>
<script>
function exibirSituacaoMensarioPadrao() {
	jQuery('#dv_situacao_mensario').html('<img src="../imagens/carregando.gif" align="absmiddle"> Carregando...');
	ajaxatualizarAsync('requisicao=exibirSituacaoMensario&fpbid=<?=$fpbid_padrao ?>','situacaomensario');
}


jQuery(document).ready(function() {
	ajaxatualizarAsync('requisicao=exibirSituacaoPagamento&fpbid=<?=$fpbid_padrao ?>','situacaopagamentos');
	ajaxatualizarAsync('requisicao=exibirAcessoUsuarioSimec','acessousuario');
	ajaxatualizarAsync('requisicao=exibirPorcentagemPagamento','porcentagempagamentos');
	
});
</script>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloCentro" valign="top" colspan="2">
	<p align="center">Situação dos Pagamentos</p>
	<p align="center">
	<?
    $sql = "SELECT uncid as codigo, uninome as descricao FROM sispacto2.universidadecadastro un INNER JOIN sispacto2.universidade uni ON uni.uniid = un.uniid ORDER BY uni.uninome";
    $db->monta_combo('uncid_pag', $sql, 'S', 'TODAS', 'selecionarUniversidadePagamento', '', '', '', 'N', 'uncid_pag','', $_REQUEST['uncid_pag']);
    $sql = "SELECT fpbid as codigo, mes.mesdsc||'/'||fpbanoreferencia as descricao FROM sispacto2.folhapagamento un INNER JOIN public.meses mes ON mes.mescod::integer = un.fpbmesreferencia";
    $db->monta_combo('fpbid_pag', $sql, 'S', 'TODAS', 'selecionarUniversidadePagamento', '', '', '', 'N', 'fpbid_pag','', $fpbid_padrao); 
	?></p>
	<div id="situacaopagamentos"><img src="../imagens/carregando.gif" align="absmiddle"> Carregando...</div>
	</td>
</tr>

<tr>
	<td class="SubTituloCentro" valign="top" width="50%">
	
	<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
	<tr>
		<td class="SubTituloCentro">Gerenciamento de usuários</td>
	</tr>
	<tr>
		<td valign="top">
		<p align="center">Acesso dos usuários ao SIMEC</p>
		<p align="center"><?
	    $sql = "SELECT uncid as codigo, uninome as descricao FROM sispacto2.universidadecadastro un INNER JOIN sispacto2.universidade uni ON uni.uniid = un.uniid ORDER BY uni.uninome";
	    $db->monta_combo('uncid', $sql, 'S', 'TODAS', 'selecionarUniversidadeAcesso', '', '', '', 'N', 'uncid','', $_REQUEST['uncid']); 
		?></p>
		<div id="acessousuario"><img src="../imagens/carregando.gif" align="absmiddle"> Carregando...</div>
		</td>
	</tr>

	</table>
	
	<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
	<tr>
		<td class="SubTituloCentro">Municípios</td>
	</tr>
	<tr>
		<td valign="top">
		<p align="center">Composição das turmas por municípios/universidade</p>
		<div style="height:200px;overflow:auto;">
		<?
	    $sql = "select 
				'<img src=../imagens/mais.gif title=mais style=\"cursor:pointer;\" onclick=\"detalharMunicipiosturmaNaoFechadas('||foo.uncid||',this);\">' as mais,
				foo.uninome, 
				foo.fechados, 
				foo.total, 
				case when foo.total > 0 then round((foo.fechados::numeric/foo.total::numeric)*100,0) else 0 end as porc 
				from (

				select 
				un.uncid,
				u.uninome, 
				(select count(*) from sispacto2.abrangencia a 
				inner join sispacto2.estruturacurso e on e.ecuid = a.ecuid 
				inner join sispacto2.pactoidadecerta p on p.muncod = a.muncod 
				inner join workflow.documento d on d.docid = p.docidturma 
				where a.esfera='M' AND e.uncid=un.uncid and d.esdid=".ESD_FECHADO_TURMA.") as fechados,
				
				(select count(*) from sispacto2.abrangencia a 
				inner join sispacto2.estruturacurso e on e.ecuid = a.ecuid 
				where a.esfera='M' AND e.uncid=un.uncid) as total
				from sispacto2.universidadecadastro un 
				inner join sispacto2.universidade u on u.uniid = un.uniid 
				order by 1
				
				) foo ";

		$cabecalho = array("&nbsp;","Univerisdade","Turmas fechadas","Total","%");
		$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%',$par2);
	    
	  ?>
	  </div>
		</td>
	</tr>

	</table>	
	 
	</td>
	
	<td class="SubTituloCentro" valign="top" width="50%">
	<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
	<tr>
		<td class="SubTituloCentro">Avaliações</td>
	</tr>
	<tr>
		<td>
		<p align="center">Situações dos Mensários</p>
		<p align="center"><?
	    $sql = "SELECT uncid as codigo, uninome as descricao FROM sispacto2.universidadecadastro un INNER JOIN sispacto2.universidade uni ON uni.uniid = un.uniid ORDER BY uni.uninome";
	    $db->monta_combo('uncid_sit', $sql, 'S', 'TODAS', 'selecionarUniversidadeMensario', '', '', '', 'N', 'uncid_sit','', $_REQUEST['uncid_sit']);
	    $sql = "SELECT fpbid as codigo, mes.mesdsc||'/'||fpbanoreferencia as descricao FROM sispacto2.folhapagamento un INNER JOIN public.meses mes ON mes.mescod::integer = un.fpbmesreferencia";
	    $db->monta_combo('fpbid_sit', $sql, 'S', 'TODAS', 'selecionarUniversidadeMensario', '', '', '', 'N', 'fpbid_sit','', $fpbid_padrao); 
		?></p>
		<div id="situacaomensario">A situação de <b>Aptos/Não Aptos/Aprovados</b> demora cerca de 2 minutos para ser processada. Caso queira visualizar essa informação <a style="cursor:pointer;" onclick="exibirSituacaoMensarioPadrao();"><b>CLIQUE AQUI</b></a></div>
		</td>
	</tr>
	<tr>
		<td>
		<p align="center">Porcentagem dos Pagamentos</p>
		<p align="center"><?
	    $sql = "SELECT uncid as codigo, uninome as descricao FROM sispacto2.universidadecadastro un INNER JOIN sispacto2.universidade uni ON uni.uniid = un.uniid ORDER BY uni.uninome";
	    $db->monta_combo('uncid_por', $sql, 'S', 'TODAS', 'selecionarUniversidadePagamentoPorcent', '', '', '', 'N', 'uncid_por','', $_REQUEST['uncid_por']);
		?></p>
		<div id="porcentagempagamentos"><img src="../imagens/carregando.gif" align="absmiddle"> Carregando...</div>
		</td>
	</tr>
	
	</table>	
	
	</td>
</tr>
</table>
<? elseif($_REQUEST['aba']=='auditoria') : ?>
<script>
jQuery(document).ready(function() {
	ajaxatualizarAsync('requisicao=ausenciaOEIES','ausenciaOEIES');
	ajaxatualizarAsync('requisicao=ausenciaPAMunicipio','ausenciaPAMunicipio');
	ajaxatualizarAsync('requisicao=bolsistasSISPACTODirigentes','bolsistasSISPACTODirigentes');
	ajaxatualizarAsync('requisicao=mediaTurmasPerfil&pflcod=<?=PFL_FORMADORIES ?>','mediaTurmasPerfilFormador');
	ajaxatualizarAsync('requisicao=mediaTurmasPerfil&pflcod=<?=PFL_ORIENTADORESTUDO ?>','mediaTurmasPerfilOrientadoresEstudo');

	ajaxatualizarAsync('requisicao=correlacaoParentescoOEePA','correlacaoParentescoOEePA');

	
});
</script>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloCentro" valign="top" width="50%" id="ausenciaOEIES"><img src="../imagens/carregando.gif" align="absmiddle"> Carregando...</td>
	<td class="SubTituloCentro" valign="top" width="50%" id="ausenciaPAMunicipio"><img src="../imagens/carregando.gif" align="absmiddle"> Carregando...</td>
</tr>
<tr>
<td class="SubTituloCentro" valign="top" width="50%">

<div id="bolsistasSISPACTODirigentes"><img src="../imagens/carregando.gif" align="absmiddle"> Carregando...</div>
<br>
<p align="center">
<select name="dep[]" class="CampoEstilo" style="width: auto;font-size:x-small;" onchange="carregarHierarquia(this.value)">
<option value="correlacaoParentescoOEePA">Orientador de Estudo => Professor Alfabetizador</option>
<option value="correlacaoParentescoCLeOE">Coordenador Local => Orientador de Estudo</option>
<option value="correlacaoParentescoCLePA">Coordenador Local => Professor Alfabetizador</option>
</select>
</p>
<div id="correlacaoParentescoOEePA"><img src="../imagens/carregando.gif" align="absmiddle"> Carregando...</div>

</td>
<td class="SubTituloCentro" valign="top" width="50%">

 <p>Turmas de formadores</p>
<p align="center">
<select name="qtd[]" class="CampoEstilo" style="width: auto;font-size:x-small;" onchange="carregarQtdTurmas_<?=PFL_FORMADORIES ?>(this.value)">
<option value="BETWEEN 1 AND 5">Até 5 cursistas</option>
<option value="BETWEEN 6 AND 10">De 6 a 10 cursistas</option>
<option value="BETWEEN 11 AND 25">De 11 a 25 cursistas</option>
<option value="BETWEEN 26 AND 34">De 26 a 34 cursistas</option>
<option value="BETWEEN 35 AND 50">De 35 a 50 cursistas</option>
<option value="BETWEEN 51 AND 75">De 51 a 75 cursistas</option>
<option value="BETWEEN 76 AND 100">De 76 a 100 cursistas</option>
<option value=">100">Mais de 100 cursistas</option>
</select>
</p>
 
 <div id="mediaTurmasPerfilFormador"><img src="../imagens/carregando.gif" align="absmiddle"> Carregando...</div>
 <p>Turmas de Orientadores de Estudo</p>
<p align="center">
<select name="qtd[]" class="CampoEstilo" style="width: auto;font-size:x-small;" onchange="carregarQtdTurmas_<?=PFL_ORIENTADORESTUDO ?>(this.value)">'
<option value="BETWEEN 1 AND 5">Até 5 cursistas</option>
<option value="BETWEEN 6 AND 10">De 6 a 10 cursistas</option>
<option value="BETWEEN 11 AND 25">De 11 a 25 cursistas</option>
<option value="BETWEEN 26 AND 34">De 26 a 34 cursistas</option>
<option value="BETWEEN 35 AND 50">De 35 a 50 cursistas</option>
<option value="BETWEEN 51 AND 75">De 51 a 75 cursistas</option>
<option value="BETWEEN 76 AND 100">De 76 a 100 cursistas</option>
<option value=">100">Mais de 100 cursistas</option>
</select>
</p>
 
 <div id="mediaTurmasPerfilOrientadoresEstudo"><img src="../imagens/carregando.gif" align="absmiddle"> Carregando...</div>

</td>
<td></td>
</tr>
</table>
<? elseif($_REQUEST['aba']=='resumo') : ?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td valign="top" colspan="2">
		<p align="center">Orçamento</p>
		<?
		$sql = "select 
				foo.gdedesc,
				foo.orcvlrunitario,
				round((foo.orcvlrunitario::numeric/foo.total::numeric)*100,2) as porc_1,
				foo.orcvlrexecutado,
				round((foo.orcvlrexecutado::numeric/foo.totalexec::numeric)*100,2) as porc_2
				from (
				
				select 
				sum(o.orcvlrunitario) as orcvlrunitario, 
				sum(o.orcvlrexecutado) as orcvlrexecutado, 
				gd.gdedesc, 
				(select sum(orcvlrunitario) from sispacto2.orcamento where orcstatus='A') as total,
				(select sum(orcvlrexecutado) from sispacto2.orcamento where orcstatus='A') as totalexec
				from sispacto2.orcamento o 
				inner join sispacto2.universidadecadastro u on u.uncid = o.uncid 
				inner join sispacto2.universidade uu on uu.uniid = u.uniid 
				inner join sispacto2.grupodespesa gd on gd.gdeid = o.gdeid 
				where o.orcstatus='A' 
				group by gd.gdedesc
				
				) foo";
		
		$cabecalho = array("Grupo de despesa","Valor previsto","%","Valor executado","%");
		$db->monta_lista_simples($sql,$cabecalho,100000,5,'S','100%','',false, false, false, false);
		
		?>
		</td>
	</tr>
	
	<tr>
		<td valign="top" colspan="2">
		<?
		$sql = "select
				pp.pflcod,
				pp.pfldsc,
				(select count(*) from sispacto2.identificacaousuario i 
		         inner join sispacto2.tipoperfil t on t.iusd = i.iusd where i.iusstatus='A' and t.pflcod = p.pflcod) as totalp,
				(select count(*) from sispacto2.identificacaousuario i 
		         inner join sispacto2.tipoperfil t on t.iusd = i.iusd where i.iusstatus='I' and t.pflcod = p.pflcod) as totale,
				foo.qtd as totalpgs,
				foo.pbovlrpagamento as valorpgs
				from sispacto2.pagamentoperfil p
				inner join seguranca.perfil pp on pp.pflcod = p.pflcod
				inner join (
				select count(*) as qtd, sum(pbovlrpagamento) as pbovlrpagamento, pg.pflcod from sispacto2.pagamentobolsista pg inner join workflow.documento d on d.docid = pg.docid where pg.pflcod not in(".PFL_PROFESSORALFABETIZADOR.",".PFL_ORIENTADORESTUDO.") and d.esdid=".ESD_PAGAMENTO_EFETIVADO." group by pg.pflcod
				) foo on foo.pflcod = p.pflcod
				where pp.pflcod not in(".PFL_PROFESSORALFABETIZADOR.",".PFL_ORIENTADORESTUDO.")
				order by pp.pflnivel";
		
		$participantes = $db->carregar($sql);
		
		if($participantes[0]) {
			foreach($participantes as $part) {
				$_arr_participantes[$part['pflcod']] = $part;
				$_arr_participantes[$part['pflcod']] = $part;
			}
		}
		
		$qtd_oe = $db->pegaUm("SELECT count(*) FROM sispacto2.identificacaousuario i 
		         			   INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd WHERE i.iusstatus='A' AND t.pflcod = ".PFL_ORIENTADORESTUDO);
		
		?>
		<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
			<tr>
			<? if($_arr_participantes) : ?>
			<? foreach($_arr_participantes as $pflcod => $pa) : ?>
			<td valign="top">
			
			<table class="listagem" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" width="100%">
			<tr>
				<td class="SubTituloCentro" colspan="2"><?=$pa['pfldsc'] ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita"><span style=font-size:x-small;>1 - Total participantes</span></td>
				<td><span style=font-size:x-small;><?=$pa['totalp'] ?></span></td>
			</tr>
			<tr>
				<td class="SubTituloDireita"><span style=font-size:x-small;>2 - Total (R$) pago</span></td>
				<td><span style=font-size:x-small;><?=number_format($pa['valorpgs'],2,",",".").' - '.$pa['totalpgs'].' bolsas' ?></span></td>
			</tr>
			<tr>
				<td class="SubTituloDireita"><span style=font-size:x-small;>3 - Total participantes excluídos</span></td>
				<td><span style=font-size:x-small;><?=$pa['totale'] ?></span></td>
			</tr>
			<?

			switch($pflcod) :
				case PFL_COORDENADORLOCAL:
					echo '<tr>';
					echo '<td class="SubTituloDireita"><span style=font-size:x-small;>4 - Orientador de Estudo / Coordenador Local</span></td>';
					echo '<td><span style=font-size:x-small;>'.round($qtd_oe/$pa['totalp'],1).'</span></td>';
					echo '</tr>';
					break;
				case PFL_COORDENADORADJUNTOIES:
					echo '<tr>';
					echo '<td class="SubTituloDireita"><span style=font-size:x-small;>4 - Supervisor IES / Coordenador Adjunto IES</span></td>';
					echo '<td><span style=font-size:x-small;>'.round($_arr_participantes[PFL_SUPERVISORIES]['totalp']/$pa['totalp'],1).'</span></td>';
					echo '</tr>';
					break;
				case PFL_SUPERVISORIES:
					echo '<tr>';
					echo '<td class="SubTituloDireita"><span style=font-size:x-small;>4 - Formador IES Mat+Port / Supervisor IES</span></td>';
					echo '<td><span style=font-size:x-small;>'.round(($_arr_participantes[PFL_FORMADORIES]['totalp']+$_arr_participantes[PFL_FORMADORIESP]['totalp'])/$pa['totalp'],1).'</span></td>';
					echo '</tr>';
					break;
				case PFL_FORMADORIES:
					echo '<tr>';
					echo '<td class="SubTituloDireita"><span style=font-size:x-small;>4 - Orientador de estudo / Formador IES</span></td>';
					echo '<td><span style=font-size:x-small;>'.round($qtd_oe/$pa['totalp'],1).'</span></td>';
					echo '</tr>';
					break;
							
						
			
			endswitch;
			
			
			
			?>
			</table>
			
			</td>
			<? endforeach; ?>
			<? endif; ?>
				
			</tr>
		</table>
		
		</td>
	</tr>

	
	<tr>
		<td valign="top">
		<?
	    resumoGeralCurso(array('pflcod'=>PFL_PROFESSORALFABETIZADOR)); 
		?>
		</td>
		<td valign="top">
		<?
	    resumoGeralCurso(array('pflcod'=>PFL_ORIENTADORESTUDO)); 
		?>
		</td>
	</tr>
</table>
<? endif; ?>