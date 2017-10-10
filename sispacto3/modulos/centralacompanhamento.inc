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
echo '<script language="javascript" type="text/javascript" src="./js/sispacto3.js"></script>';

echo "<br>";

include_once APPRAIZ . "includes/library/simec/Grafico.php";

$grafico = new Grafico();

monta_titulo( "Central de Acompanhamento", "Informações sobre o andamento do projeto");

$fpbid_padrao = $db->pegaUm("SELECT fpbid FROM sispacto3.folhapagamento WHERE fpbanoreferencia='".date("Y")."' AND fpbmesreferencia='".date("m")."'");

$menu[] = array("id" => 1, "descricao" => 'Etapa Projeto', "link" => '/sispacto3/sispacto3.php?modulo=centralacompanhamento&acao=A&aba=projeto');
$menu[] = array("id" => 2, "descricao" => 'Etapa Execução', "link" => '/sispacto3/sispacto3.php?modulo=centralacompanhamento&acao=A&aba=execucao');
//$menu[] = array("id" => 3, "descricao" => 'Etapa Auditoria', "link" => '/sispacto2/sispacto3.php?modulo=centralacompanhamento&acao=A&aba=auditoria');
//$menu[] = array("id" => 3, "descricao" => 'Etapa Resumo', "link" => '/sispacto2/sispacto3.php?modulo=centralacompanhamento&acao=A&aba=resumo');

echo "<br>";

if(!$_REQUEST['aba']) $_REQUEST['aba']='projeto';

echo montarAbasArray($menu, '/sispacto3/sispacto3.php?modulo=centralacompanhamento&acao=A&aba='.$_REQUEST['aba']);

if($_REQUEST['aba']=='projeto') :
?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td valign="top" width="33%"><?

	$sql = "SELECT COALESCE(e.esddsc,'Não iniciou Elaboração') as descricao,
				   COUNT(*) as valor
			FROM sispacto3.pactoidadecerta p
			LEFT JOIN workflow.documento d ON d.docid = p.docid
			LEFT JOIN workflow.estadodocumento e ON e.esdid = d.esdid
			WHERE p.muncod IS NOT NULL AND p.picstatus='A'
			GROUP BY e.esddsc, e.esdid
			ORDER BY 2 DESC";
	
	$grafico->setTitulo('Cadastro OE - Municipal')->setTipo(Grafico::K_TIPO_PIZZA)->setWidth('100%')->gerarGrafico($sql);
	
	?></td>
	<td valign="top" width="33%"><?

	$sql = "SELECT COALESCE(e.esddsc,'Não iniciou Elaboração') as descricao,
				   COUNT(*) as valor
			FROM sispacto3.pactoidadecerta p
			LEFT JOIN workflow.documento d ON d.docid = p.docid
			LEFT JOIN workflow.estadodocumento e ON e.esdid = d.esdid
			WHERE p.estuf IS NOT NULL AND p.picstatus='A'
			GROUP BY e.esddsc, e.esdid
			ORDER BY 2 DESC";
	
	
	$grafico->setTitulo('Cadastro OE - Estadual')->setTipo(Grafico::K_TIPO_PIZZA)->setWidth('100%')->gerarGrafico($sql);
	
	?></td>
	<td valign="top" width="33%"><?

	$sql = "SELECT
					   CASE WHEN iustipoorientador='professorsispacto2014' THEN 'Professor Alfabetizador do Pacto 2014 recomendado para certificação'
							WHEN iustipoorientador='orientadorsispacto2014' THEN 'Orientador de Estudo do Pacto 2014 recomendado para certificação'
							WHEN iustipoorientador='tutoresproletramento' THEN 'Tutores Pró-Letramento'
						    WHEN iustipoorientador='tutoresredesemproletramento' THEN 'Professores da rede que não foram Tutores do Pró-Letramento'
						    WHEN iustipoorientador='profissionaismagisterio' THEN 'Profissionais do Magistério com experiência em formação de professores' END as descricao,
							count(*) as valor

					FROM sispacto3.identificacaousuario i
					INNER JOIN sispacto3.tipoperfil t ON t.iusd = i.iusd
					WHERE pflcod=".PFL_ORIENTADORESTUDO."
					GROUP BY iustipoorientador";
	
	
	
	$grafico->setTitulo('Tipo OE - Cadastro')->setTipo(Grafico::K_TIPO_PIZZA)->setWidth('100%')->gerarGrafico($sql);
	
	?></td>
</tr>
<tr>
	<td valign="top" width="33%"><?

	$sql = "SELECT COALESCE(e.esddsc,'Não iniciou Elaboração') as descricao,
				   COUNT(*) as valor
			FROM sispacto3.pactoidadecerta p
			LEFT JOIN workflow.documento d ON d.docid = p.docidturma
			LEFT JOIN workflow.estadodocumento e ON e.esdid = d.esdid
			WHERE p.muncod IS NOT NULL AND p.picstatus='A'
			GROUP BY e.esddsc, e.esdid
			ORDER BY 2 DESC";
	
	$grafico->setTitulo('Cadastro PA - Municipal')->setTipo(Grafico::K_TIPO_PIZZA)->setWidth('100%')->gerarGrafico($sql);
	
	?></td>
	<td valign="top" width="33%"><?

	$sql = "SELECT COALESCE(e.esddsc,'Não iniciou Elaboração') as descricao,
				   COUNT(*) as valor
			FROM sispacto3.pactoidadecerta p
			LEFT JOIN workflow.documento d ON d.docid = p.docidturma
			LEFT JOIN workflow.estadodocumento e ON e.esdid = d.esdid
			WHERE p.estuf IS NOT NULL AND p.picstatus='A'
			GROUP BY e.esddsc, e.esdid
			ORDER BY 2 DESC";
	
	
	$grafico->setTitulo('Cadastro PA - Estadual')->setTipo(Grafico::K_TIPO_PIZZA)->setWidth('100%')->gerarGrafico($sql);
	
	?></td>
	<td valign="top" width="33%"><?

	$sql = "SELECT
					   CASE WHEN iustipoprofessor='cpflivre' THEN 'Não bolsista'
							WHEN iustipoprofessor='censo' THEN 'Bolsista'
							ELSE 'Não definido' END as descricao,
							count(*) as valor

					FROM sispacto3.identificacaousuario i
					INNER JOIN sispacto3.tipoperfil t ON t.iusd = i.iusd
					WHERE pflcod=".PFL_PROFESSORALFABETIZADOR."
					GROUP BY iustipoprofessor";
	
	
	
	$grafico->setTitulo('Tipo PA - Cadastro')->setTipo(Grafico::K_TIPO_PIZZA)->setWidth('100%')->gerarGrafico($sql);
	
	?></td>
</tr>

</table>
<? elseif($_REQUEST['aba']=='execucao') : ?>
<script>

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

function selecionarUniversidadeMensario(x) {
	jQuery('#situacaomensario').html('Carregando...');
	var uncid = jQuery('#uncid_sit').val();
	var fpbid = jQuery('#fpbid_sit').val();
	ajaxatualizar('requisicao=exibirSituacaoMensario&uncid='+uncid+'&fpbid='+fpbid,'situacaomensario');
}


function selecionarUniversidadePagamento(x) {
	jQuery('#situacaopagamentos').html('Carregando...');
	var uncid = jQuery('#uncid_pag').val();
	var fpbid = jQuery('#fpbid_pag').val();
	ajaxatualizar('requisicao=exibirSituacaoPagamento&uncid='+uncid+'&fpbid='+fpbid,'situacaopagamentos');
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
    $sql = "SELECT uncid as codigo, uninome as descricao FROM sispacto3.universidadecadastro un INNER JOIN sispacto3.universidade uni ON uni.uniid = un.uniid ORDER BY uni.uninome";
    $db->monta_combo('uncid_pag', $sql, 'S', 'TODAS', 'selecionarUniversidadePagamento', '', '', '', 'N', 'uncid_pag','', $_REQUEST['uncid_pag']);
    $sql = "SELECT fpbid as codigo, mes.mesdsc||'/'||fpbanoreferencia as descricao FROM sispacto3.folhapagamento un INNER JOIN public.meses mes ON mes.mescod::integer = un.fpbmesreferencia";
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
		<p align="center">Acesso dos usuários ao <?php echo SIGLA_SISTEMA; ?></p>
		<p align="center"><?
	    $sql = "SELECT uncid as codigo, uninome as descricao FROM sispacto3.universidadecadastro un INNER JOIN sispacto3.universidade uni ON uni.uniid = un.uniid ORDER BY uni.uninome";
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
				(select count(*) from sispacto3.abrangencia a 
				inner join sispacto3.estruturacurso e on e.ecuid = a.ecuid 
				inner join sispacto3.pactoidadecerta p on p.muncod = a.muncod 
				inner join workflow.documento d on d.docid = p.docidturma 
				where a.esfera='M' AND e.uncid=un.uncid and d.esdid=".ESD_FECHADO_TURMA.") as fechados,
				
				(select count(*) from sispacto3.abrangencia a 
				inner join sispacto3.estruturacurso e on e.ecuid = a.ecuid 
				where a.esfera='M' AND e.uncid=un.uncid) as total
				from sispacto3.universidadecadastro un 
				inner join sispacto3.universidade u on u.uniid = un.uniid 
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
	    $sql = "SELECT uncid as codigo, uninome as descricao FROM sispacto3.universidadecadastro un INNER JOIN sispacto3.universidade uni ON uni.uniid = un.uniid ORDER BY uni.uninome";
	    $db->monta_combo('uncid_sit', $sql, 'S', 'TODAS', 'selecionarUniversidadeMensario', '', '', '', 'N', 'uncid_sit','', $_REQUEST['uncid_sit']);
	    $sql = "SELECT fpbid as codigo, mes.mesdsc||'/'||fpbanoreferencia as descricao FROM sispacto3.folhapagamento un INNER JOIN public.meses mes ON mes.mescod::integer = un.fpbmesreferencia";
	    $db->monta_combo('fpbid_sit', $sql, 'S', 'TODAS', 'selecionarUniversidadeMensario', '', '', '', 'N', 'fpbid_sit','', $fpbid_padrao); 
		?></p>
		<div id="situacaomensario">A situação de <b>Aptos/Não Aptos/Aprovados</b> demora cerca de 2 minutos para ser processada. Caso queira visualizar essa informação <a style="cursor:pointer;" onclick="exibirSituacaoMensarioPadrao();"><b>CLIQUE AQUI</b></a></div>
		</td>
	</tr>
	<tr>
		<td>
		<p align="center">Porcentagem dos Pagamentos</p>
		<p align="center"><?
	    $sql = "SELECT uncid as codigo, uninome as descricao FROM sispacto3.universidadecadastro un INNER JOIN sispacto3.universidade uni ON uni.uniid = un.uniid ORDER BY uni.uninome";
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