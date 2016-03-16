<?

/* configurações do relatorio - Memoria limite de 1024 Mbytes */
ini_set("memory_limit", "1024M");
set_time_limit(0);
/* FIM configurações - Memoria limite de 1024 Mbytes */



function carregarDetalhesStatusUsuarios($dados) {
	global $db;
	
	$sql = "select 
uu.uninome, 
(select count(*) from sispacto.identificacaousuario u 
 inner join sispacto.tipoperfil t on t.iusd=u.iusd and t.pflcod=".$dados['pflcod']." and u.uncid=un.uncid) as usutotal,

(select count(*) from sispacto.identificacaousuario u 
 inner join sispacto.tipoperfil t on t.iusd=u.iusd and t.pflcod=".$dados['pflcod']." 
 inner join seguranca.perfilusuario pu on pu.usucpf=u.iuscpf and pu.pflcod=t.pflcod 
 inner join seguranca.usuario_sistema us on us.usucpf=u.iuscpf and us.sisid=".SIS_SISPACTO." 
 where us.suscod='A' and u.uncid=un.uncid) as usuativos,

(select count(*) from sispacto.identificacaousuario u 
 inner join sispacto.tipoperfil t on t.iusd=u.iusd and t.pflcod=".$dados['pflcod']." 
 inner join seguranca.perfilusuario pu on pu.usucpf=u.iuscpf and pu.pflcod=t.pflcod 
 inner join seguranca.usuario_sistema us on us.usucpf=u.iuscpf and us.sisid=".SIS_SISPACTO." 
 where us.suscod='P' and u.uncid=un.uncid) as usupendentes,

(select count(*) from sispacto.identificacaousuario u 
 inner join sispacto.tipoperfil t on t.iusd=u.iusd and t.pflcod=".$dados['pflcod']." 
 inner join seguranca.perfilusuario pu on pu.usucpf=u.iuscpf and pu.pflcod=t.pflcod 
 inner join seguranca.usuario_sistema us on us.usucpf=u.iuscpf and us.sisid=".SIS_SISPACTO." 
 where us.suscod='B' and u.uncid=un.uncid) as usubloqueado,

 (select count(*) from sispacto.identificacaousuario u 
 inner join sispacto.tipoperfil t on t.iusd=u.iusd and t.pflcod=".$dados['pflcod']." 
 left join seguranca.perfilusuario pu on pu.usucpf=u.iuscpf and pu.pflcod=t.pflcod 
 left join seguranca.usuario_sistema us on us.usucpf=u.iuscpf and us.sisid=".SIS_SISPACTO." 
 where (us.suscod is null or pu.pflcod is null) and u.uncid=un.uncid) as usunaocadastrado
 

from sispacto.universidadecadastro un 
inner join sispacto.universidade uu on uu.uniid = un.uniid";
	
	$cabecalho = array("Universidade","Total","Ativos","Pendentes","Bloqueados","Não cadastrados");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%',$par2);
		
}

function carregarDetalhesCadastroOrientadores($dados) {
	global $db;
	
	if($dados['esdid']) {
		if($dados['esdid']=='9999999') {
			$f[] = "e.esdid IS NULL";
		} else {
			$f[] = "e.esdid='".$dados['esdid']."'";
		}
	} else {
		$f[] = "1=2";
	}
	
	if($dados['esfera']=='municipal') {
		$f[] = "p.muncod IS NOT NULL";
	} elseif($dados['esfera']=='estadual') {
		$f[] = "p.estuf IS NOT NULL";
	}
	
	$sql = "SELECT 	CASE WHEN p.muncod IS NOT NULL THEN m.estuf ELSE p.estuf END as estuf, 
					COUNT(*) as tot,
					ROUND(( (COUNT(*)*100)::numeric / (SELECT COUNT(*) FROM sispacto.pactoidadecerta)::numeric ),2) as porcent 
			FROM sispacto.pactoidadecerta p 
			LEFT JOIN territorios.municipio m ON m.muncod = p.muncod 
			LEFT JOIN workflow.documento d ON d.docid = p.docid 
			LEFT JOIN workflow.estadodocumento e ON e.esdid = d.esdid 
			WHERE ".implode(" AND ",$f)." 
			GROUP BY CASE WHEN p.muncod IS NOT NULL THEN m.estuf ELSE p.estuf END 
			ORDER BY 3 DESC";
	$cabecalho = array("UF","Quantidade","%");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%',$par2);
	
}

function carregarDetalhesAbrangenciaEstado($dados) {
	global $db;
	
	echo "<p>Municípios sem abrangência - Rede Municipal</p>";
	
	$sql = "select m.estuf, m.mundescricao, e.esddsc from sispacto.pactoidadecerta p 
			inner join territorios.municipio m on m.muncod = p.muncod 
			inner join workflow.documento d on d.docid = p.docid 
			inner join workflow.estadodocumento e on e.esdid = d.esdid
			where picstatus='A' and m.estuf='".$dados['estuf']."' and p.muncod is not null and p.muncod not in (select muncod from sispacto.abrangencia where esfera='M')";
	
	$cabecalho = array("UF","Município","Situação");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%',$par2);
	
	echo "<p>Municípios sem abrangência - Rede Estadual</p>";
	
	$sql = "select  distinct m.estuf, m.mundescricao, e.esddsc from sispacto.identificacaousuario i 
inner join sispacto.pactoidadecerta p on p.picid = i.picid and p.muncod is null 
inner join territorios.municipio m on m.muncod = i.muncodatuacao 
inner join workflow.documento d on d.docid = p.docid 
inner join workflow.estadodocumento e on e.esdid = d.esdid
where p.estuf='".$dados['estuf']."' and i.muncodatuacao not in (select muncod from sispacto.abrangencia where esfera='E')";
	
	$cabecalho = array("UF","Município","Situação");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%',$par2);
	
}

function carregarDetalhesAvaliacoesUsuarios($dados) {
	global $db;
	$sql = "SELECT foo3.uninome, sum(napto) as napto, sum(apto) as apto, sum(aprovado) as aprovado FROM (
 SELECT uninome, 
 CASE WHEN foo2.resultado='Não Apto' THEN 1 ELSE 0 END napto, 
 CASE WHEN foo2.resultado='Apto' THEN 1 ELSE 0 END apto, 
 CASE WHEN foo2.resultado='Aprovado' THEN 1 ELSE 0 END aprovado 
 FROM (

	SELECT foo.pflcod,
		foo.uninome,
			CASE WHEN foo.esdid=657 THEN 'Aprovado'
						 	  WHEN foo.mensarionota > 7  AND foo.iustermocompromisso=true AND (CASE WHEN foo.pflcod=827 THEN 
																																					CASE WHEN foo.iusdocumento=false THEN false 
																																						 WHEN foo.numeroavaliacoes > 1 THEN true ELSE false END 
																									WHEN foo.pflcod=849 THEN 
																																						CASE WHEN foo.iustipoprofessor = 'censo' THEN true 
																																						ELSE false END
																									ELSE true END) THEN 'Apto' 
		    ELSE 'Não Apto' END resultado, foo.fpbid FROM (
	SELECT 
	COALESCE((SELECT AVG(mavtotal) FROM sispacto.mensarioavaliacoes ma  WHERE ma.menid=m.menid),0.00) as mensarionota,
	uu.uninome,
	i.iusdocumento,
	i.iustermocompromisso,
	m.fpbid,
	d.esdid,
	t.pflcod,
	i.iustipoprofessor,
	(SELECT COUNT(mavid) FROM sispacto.mensarioavaliacoes ma  WHERE ma.menid=m.menid) as numeroavaliacoes
	FROM sispacto.mensario m
	INNER JOIN sispacto.identificacaousuario i ON i.iusd = m.iusd 
	INNER JOIN sispacto.universidadecadastro un ON un.uncid = i.uncid 
	INNER JOIN sispacto.universidade uu ON uu.uniid = un.uniid 
	INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd  
	INNER JOIN workflow.documento d ON d.docid = m.docid 
	
	) foo WHERE foo.pflcod='".$dados['pflcod']."' and foo.fpbid='".$dados['fpbid']."') foo2) foo3 GROUP BY foo3.uninome";
	
	$cabecalho = array("Universidade","Não Apto","Apto","Aprovados");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%',$par2);
	

}


function detalharDetalhesPagamentosUsuarios($dados) {
	global $db;
	if($dados['pflcod']) $wh[] = "pb.pflcod='".$dados['pflcod']."'";
	if($dados['uncid']) $wh[] = "un.uncid='".$dados['uncid']."'";
	if($dados['fpbid']) $wh[] = "pb.fpbid='".$dados['fpbid']."'";
	
	
	$sql = "SELECT 
				   foo.universidade, 
				   foo.ag_autorizacao, 
				   (foo.ag_autorizacao*pp.plpvalor) as rs_ag_autorizacao,
				   foo.autorizado,
				   (foo.autorizado*pp.plpvalor) as rs_autorizado,
				   foo.ag_autorizacao_sgb,
				   (foo.ag_autorizacao_sgb*pp.plpvalor) as rs_ag_autorizacao_sgb,
				   foo.ag_pagamento,
				   (foo.ag_pagamento*pp.plpvalor) as rs_ag_pagamento,
				   foo.enviadobanco, 
				   (foo.enviadobanco*pp.plpvalor) as rs_enviadobanco,
				   foo.pg_efetivado,
				   (foo.pg_efetivado*pp.plpvalor) as rs_pg_efetivado,
				   foo.pg_recusado,
				   (foo.pg_recusado*pp.plpvalor) as rs_pg_recusado,
				   foo.pg_naoautorizado,
				   (foo.pg_naoautorizado*pp.plpvalor) as rs_pg_naoautorizado
				   
			FROM (

			SELECT fee.universidade, 
			       SUM(ag_autorizacao) as ag_autorizacao,
			       SUM(autorizado) as autorizado,
			       SUM(ag_autorizacao_sgb) as ag_autorizacao_sgb,
			       SUM(ag_pagamento) as ag_pagamento,
			       SUM(enviadobanco) as enviadobanco,
			       SUM(pg_efetivado) as pg_efetivado,
			       SUM(pg_recusado) as pg_recusado,
			       SUM(pg_naoautorizado) as pg_naoautorizado

			FROM (
			
			SELECT 
			uu.unisigla||' - '||uu.uninome as universidade,
			CASE WHEN dc.esdid='".ESD_PAGAMENTO_APTO."' THEN 1 ELSE 0 END ag_autorizacao,
			CASE WHEN dc.esdid='".ESD_PAGAMENTO_AUTORIZADO."' THEN 1 ELSE 0 END autorizado,
			CASE WHEN dc.esdid='".ESD_PAGAMENTO_AG_AUTORIZACAO_SGB."' THEN 1 ELSE 0 END ag_autorizacao_sgb,
			CASE WHEN dc.esdid='".ESD_PAGAMENTO_AGUARDANDO_PAGAMENTO."' THEN 1 ELSE 0 END ag_pagamento,
			CASE WHEN dc.esdid='".ESD_PAGAMENTO_ENVIADOBANCO."' THEN 1 ELSE 0 END enviadobanco,
			CASE WHEN dc.esdid='".ESD_PAGAMENTO_EFETIVADO."' THEN 1 ELSE 0 END pg_efetivado,
			CASE WHEN dc.esdid='".ESD_PAGAMENTO_RECUSADO."' THEN 1 ELSE 0 END pg_recusado,
			CASE WHEN dc.esdid='".ESD_PAGAMENTO_NAO_AUTORIZADO."' THEN 1 ELSE 0 END pg_naoautorizado

			
			
			FROM seguranca.perfil p 
			INNER JOIN sispacto.pagamentobolsista pb ON pb.pflcod = p.pflcod 
			INNER JOIN sispacto.universidadecadastro un ON un.uniid = pb.uniid 
			INNER JOIN sispacto.universidade uu ON uu.uniid = un.uniid 
			INNER JOIN workflow.documento dc ON dc.docid = pb.docid AND dc.tpdid=".TPD_PAGAMENTOBOLSA." 
			WHERE p.pflcod IN(
			".PFL_PROFESSORALFABETIZADOR.",
			".PFL_COORDENADORLOCAL.",
			".PFL_ORIENTADORESTUDO.",
			".PFL_COORDENADORIES.",
			".PFL_COORDENADORADJUNTOIES.",
			".PFL_SUPERVISORIES.",
			".PFL_FORMADORIES.") ".(($wh)?" AND ".implode(" AND ",$wh):"")."

			) fee 

			GROUP BY fee.universidade
			
			) foo
			
			INNER JOIN sispacto.pagamentoperfil pp ON pp.pflcod = '".$dados['pflcod']."'";
	
	$cabecalho = array("Universidade","Aguardando autorização IES","R$","Autorizado IES","R$","Aguardando autorização SGB","R$","Aguardando pagamento","R$","Enviado ao Banco","R$","Pagamento efetivado","R$","Pagamento recusado","R$","Pagamento não autorizado FNDE","R$");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%',$par2);
	
}

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}


include  APPRAIZ."includes/cabecalho.inc";
echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>';
echo '<script language="javascript" type="text/javascript" src="./js/sispacto.js"></script>';

echo "<br>";

$fpbid_padrao = $db->pegaUm("SELECT fpbid FROM sispacto.folhapagamento WHERE fpbanoreferencia='".date("Y")."' AND fpbmesreferencia='".date("m")."'");
if(!$fpbid_padrao) $fpbid_padrao = $db->pegaUm("SELECT max(fpbid) FROM sispacto.folhapagamento");

?>
<script>
function acessarCadastroOrientadores(esdid,esfera) {
	window.location='sispacto.php?modulo=principal/coordenadorlocal/listacoordenadorlocal&acao=A&esdid='+esdid+'&esfera='+esfera;
}

function acessarComposicaoTurmas(esdid,esfera) {
	window.location='sispacto.php?modulo=principal/coordenadorlocal/listacoordenadorlocal&acao=A&esdidcomposicaoturmas='+esdid+'&esfera='+esfera;
}

function acessarUniversidades(esdid) {
	window.location='sispacto.php?modulo=principal/universidade/listauniversidade&acao=A&esdid='+esdid;
}

function acessarUniversidadesFormacaoInicial(esdid) {
	window.location='sispacto.php?modulo=principal/universidade/listauniversidade&acao=A&esdidformacaoinicial='+esdid;
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



</script>
<?
monta_titulo( "Central de Acompanhamento", "Informações sobre o andamento do projeto");

$menu[] = array("id" => 1, "descricao" => 'Etapa Projeto', "link" => '/sispacto/sispacto.php?modulo=centralacompanhamento&acao=A&aba=projeto');
$menu[] = array("id" => 2, "descricao" => 'Etapa Execução', "link" => '/sispacto/sispacto.php?modulo=centralacompanhamento&acao=A&aba=execucao');
echo "<br>";

if(!$_REQUEST['aba']) $_REQUEST['aba']='projeto';

echo montarAbasArray($menu, '/sispacto/sispacto.php?modulo=centralacompanhamento&acao=A&aba='.$_REQUEST['aba']);

if($_REQUEST['aba']=='projeto') :
?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloCentro" valign="top" width="50%">
	
	<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
	<tr>
		<td class="SubTituloCentro">Coordenador Local</td>
	</tr>
	<tr>
		<td>
		<p align="center">Composição de Turmas - Municipal</p>
		<?
		$sql = "SELECT '<img src=\"../imagens/consultar.gif\" style=\"cursor:pointer;\" onclick=\"acessarComposicaoTurmas('||COALESCE(e.esdid,9999999)||',\'Municipal\');\">' as acao, 
						COALESCE(e.esddsc,'Não iniciou Elaboração'), 
						COUNT(*) as tot,
						ROUND(( (COUNT(*)*100)::numeric / (SELECT COUNT(*) FROM sispacto.pactoidadecerta WHERE muncod IS NOT NULL AND picstatus='A')::numeric ),2) as porcent 
				FROM sispacto.pactoidadecerta p 
				LEFT JOIN workflow.documento d ON d.docid = p.docidturma 
				LEFT JOIN workflow.estadodocumento e ON e.esdid = d.esdid 
				WHERE p.muncod IS NOT NULL AND p.picstatus='A'
				GROUP BY e.esddsc, e.esdid 
				ORDER BY 3 DESC";
		$cabecalho = array("&nbsp;","Situação","Quantidade","%");
		$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%',$par2);
		?>
		</td>
	</tr>
	<tr>
		<td>
		<p align="center">Cadastramento dos Orientadores de Estudo - Municipal</p>
		<?
		$sql = "SELECT '<img src=\"../imagens/mais.gif\" title=\"mais\" style=\"cursor:pointer;\" onclick=\"detalharCadastroOrientadores('||COALESCE(e.esdid,9999999)||',\'municipal\',this);\"> <img src=\"../imagens/consultar.gif\" style=\"cursor:pointer;\" onclick=\"acessarCadastroOrientadores('||COALESCE(e.esdid,9999999)||',\'Municipal\')\">' as acao, 
						COALESCE(e.esddsc,'Não iniciou Elaboração'), 
						COUNT(*) as tot,
						ROUND(( (COUNT(*)*100)::numeric / (SELECT COUNT(*) FROM sispacto.pactoidadecerta WHERE muncod IS NOT NULL AND picstatus='A')::numeric ),2) as porcent 
				FROM sispacto.pactoidadecerta p 
				LEFT JOIN workflow.documento d ON d.docid = p.docid 
				LEFT JOIN workflow.estadodocumento e ON e.esdid = d.esdid 
				WHERE p.muncod IS NOT NULL AND p.picstatus='A'
				GROUP BY e.esddsc, e.esdid 
				ORDER BY 3 DESC";
		$cabecalho = array("&nbsp;","Situação","Quantidade","%");
		$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%',$par2);
		?>
		</td>
	</tr>
	<tr>
		<td>
		<p align="center">Composição de Turmas - Estadual</p>
		<?
		$sql = "SELECT '<img src=\"../imagens/consultar.gif\" style=\"cursor:pointer;\" onclick=\"acessarComposicaoTurmas('||COALESCE(e.esdid,9999999)||',\'Estadual\')\">' as acao, 
						COALESCE(e.esddsc,'Não iniciou Elaboração'), 
						COUNT(*) as tot,
						ROUND(( (COUNT(*)*100)::numeric / (SELECT COUNT(*) FROM sispacto.pactoidadecerta WHERE estuf IS NOT NULL)::numeric ),2) as porcent 
				FROM sispacto.pactoidadecerta p 
				LEFT JOIN workflow.documento d ON d.docid = p.docidturma 
				LEFT JOIN workflow.estadodocumento e ON e.esdid = d.esdid 
				WHERE p.estuf IS NOT NULL AND p.picstatus='A'
				GROUP BY e.esddsc, e.esdid 
				ORDER BY 3 DESC";
		$cabecalho = array("&nbsp;","Situação","Quantidade","%");
		$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%',$par2);
		?>
		</td>
	</tr>

	<tr>
		<td>
		<p align="center">Cadastramento dos Orientadores de Estudo - Estadual</p>
		<?
		$sql = "SELECT '<img src=\"../imagens/mais.gif\" title=\"mais\" style=\"cursor:pointer;\" onclick=\"detalharCadastroOrientadores('||COALESCE(e.esdid,9999999)||',\'estadual\',this);\"> <img src=\"../imagens/consultar.gif\" style=\"cursor:pointer;\" onclick=\"acessarCadastroOrientadores('||COALESCE(e.esdid,9999999)||',\'Estadual\')\">' as acao, 
						COALESCE(e.esddsc,'Não iniciou Elaboração'), 
						COUNT(*) as tot,
						ROUND(( (COUNT(*)*100)::numeric / (SELECT COUNT(*) FROM sispacto.pactoidadecerta WHERE estuf IS NOT NULL)::numeric ),2) as porcent 
				FROM sispacto.pactoidadecerta p 
				LEFT JOIN workflow.documento d ON d.docid = p.docid 
				LEFT JOIN workflow.estadodocumento e ON e.esdid = d.esdid 
				WHERE p.estuf IS NOT NULL AND p.picstatus='A'
				GROUP BY e.esddsc, e.esdid 
				ORDER BY 3 DESC";
		$cabecalho = array("&nbsp;","Situação","Quantidade","%");
		$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%',$par2);
		?>
		</td>
	</tr>
	<tr>
		<td>
		<p align="center">Tipos de Orientadores de Estudo Cadastrados</p>
		<?
		$sql = "SELECT CASE WHEN iustipoorientador='tutoresproletramento' THEN 'Tutores Pró-Letramento'
	    WHEN iustipoorientador='tutoresredesemproletramento' THEN 'Professores da rede que não foram Tutores do Pró-Letramento'
	    WHEN iustipoorientador='profissionaismagisterio' THEN 'Profissionais do Magistério com experiência em formação de professores' END as tipo,
				count(*) as numero, 
				(count(*)::numeric/(SELECT count(*) FROM sispacto.identificacaousuario i INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd WHERE pflcod=827)::numeric)*100 as porcent,
				((SELECT count(*) FROM sispacto.identificacaousuario ii INNER JOIN sispacto.tipoperfil t ON t.iusd = ii.iusd WHERE pflcod=".PFL_ORIENTADORESTUDO." and ii.iusformacaoinicialorientador=true and ii.iustipoorientador=i.iustipoorientador)::numeric) as numero2,
				((SELECT count(*) FROM sispacto.identificacaousuario ii INNER JOIN sispacto.tipoperfil t ON t.iusd = ii.iusd WHERE pflcod=".PFL_ORIENTADORESTUDO." and ii.iusformacaoinicialorientador=true and ii.iustipoorientador=i.iustipoorientador)::numeric/(SELECT count(*) FROM sispacto.identificacaousuario i INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd WHERE pflcod=".PFL_ORIENTADORESTUDO.")::numeric)*100 as porcent2
				FROM sispacto.identificacaousuario i 
				INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd
				WHERE pflcod=".PFL_ORIENTADORESTUDO."
				GROUP BY iustipoorientador";
		$cabecalho = array("Tipo","Quantidade Total","%","Quantidade Formação Inicial","%");
		$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%',$par2);
		?>
		</td>
	</tr>
	
	<tr>
		<td>
		<p align="center">Tipos de Professores Alfabetizadores Cadastrados</p>
		<?
		$sql = "	SELECT CASE WHEN iustipoprofessor='censo' THEN 'Cadastrado no CENSO 2012 (Bolsista)'
							WHEN iustipoprofessor='cpflivre' THEN 'Não cadastrado no CENSO 2012 (Não bolsista)'
							ELSE 'Não identificado' END as tipo,
				count(*) as numero, 
				(count(*)::numeric/(SELECT count(*) FROM sispacto.identificacaousuario i INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd WHERE pflcod=849)::numeric)*100 as porcent
				FROM sispacto.identificacaousuario i 
				INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd
				WHERE t.pflcod=849 AND i.iusstatus='A' 
				GROUP BY iustipoprofessor";
		$cabecalho = array("Tipo","Quantidade","%");
		$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%',$par2);
		?>
		</td>
	</tr>
	
	<tr>
		<td>
		<p align="center">Materiais</p>
		<? exibirMateriais(array()); ?>
		</td>
	</tr>
	
	

	</table>	
	
	
	<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
	<tr>
		<td class="SubTituloCentro">Informações sobre pagamentos</td>
	</tr>
	<tr>
		<td>
		<p align="center">Orçamento detalhado necessário para viabilizar a Formação dos Orientadores de Estudo</p>
		<div style="height:100px;overflow:auto;">
		<?
		$sql = "SELECT foo.descricao, foo.total, CASE WHEN foo.totalpolo > 0 THEN round(foo.total/foo.totalpolo,2) ELSE 0.00 END as totalpormun, CASE WHEN foo.totalorientador > 0 THEN round(foo.total/foo.totalorientador,2) ELSE 0.00 END as totalporori FROM  (
						SELECT uu.unisigla||' - '||uu.uninome as descricao, 
					   (SELECT COALESCE(SUM(orcvlrunitario),0.00) FROM sispacto.orcamento WHERE orcstatus='A' AND uncid=u.uncid) as total,
					   (SELECT COUNT(distinct t.muncod) FROM sispacto.turmas t WHERE t.turstatus='A' AND t.uncid=u.uncid) as totalpolo,
					   (SELECT COUNT(*) FROM sispacto.identificacaousuario i INNER JOIN sispacto.tipoperfil t ON t.iusd=i.iusd AND t.pflcod=827 WHERE i.uncid=u.uncid) as totalorientador
				FROM sispacto.universidadecadastro u 
				INNER JOIN sispacto.universidade uu ON uu.uniid = u.uniid 
				ORDER BY 2 DESC
				) foo";
		$cabecalho = array("&nbsp;","R$ total","R$/Polo","R$/Orientador de Estudo");
		$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%',$par2);
		?>
		</div>
		</td>
	</tr>

	<tr>
		<td>
		<p align="center">Número de Bolsistas / Remuneração(R$)</p>
		<?
		
		$sql = "SELECT pf.pfldsc, count(i.iusd) as tot, round(count(i.iusd)*pp.plpvalor,2) as vlr
				FROM sispacto.identificacaousuario i 
				INNER JOIN sispacto.tipoperfil t on t.iusd = i.iusd 
				INNER JOIN sispacto.pagamentoperfil pp on pp.pflcod = t.pflcod 
				INNER JOIN seguranca.perfil pf on pf.pflcod = t.pflcod 
				WHERE CASE WHEN pp.pflcod=".PFL_PROFESSORALFABETIZADOR." THEN i.iustipoprofessor='censo' ELSE true END AND CASE WHEN pp.pflcod=".PFL_ORIENTADORESTUDO." THEN i.iusformacaoinicialorientador=true ELSE true END		
				GROUP BY pf.pfldsc, pp.plpvalor ORDER BY 2 DESC";
		
		$cabecalho = array("&nbsp;","Quantidade","R$ previsto / Mês");
		$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%',$par2);
		
		?>
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
		<p align="center">Preenchimento do Projeto</p>
		<?
		$sql = "SELECT '<img src=\"../imagens/consultar.gif\" style=\"cursor:pointer;\" onclick=\"acessarUniversidades('||COALESCE(e.esdid,9999999)||')\">' as acao, 
						COALESCE(e.esddsc,'Não iniciou Elaboração'), 
						COUNT(*) as tot,
						ROUND(( (COUNT(*)*100)::numeric / (SELECT COUNT(*) FROM sispacto.universidadecadastro)::numeric ),2) as porcent 
				FROM sispacto.universidadecadastro u 
				LEFT JOIN workflow.documento d ON d.docid = u.docid 
				LEFT JOIN workflow.estadodocumento e ON e.esdid = d.esdid 
				GROUP BY e.esddsc, e.esdid 
				ORDER BY 3 DESC";
		$cabecalho = array("&nbsp;","Situação","Quantidade","%");
		$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%',$par2);
		?>
		</td>
	</tr>
	<tr>
		<td>
		<p align="center">Preenchimento da Formação Inicial</p>
		<?
		$sql = "SELECT '<img src=\"../imagens/consultar.gif\" style=\"cursor:pointer;\" onclick=\"acessarUniversidadesFormacaoInicial('||COALESCE(e.esdid,9999999)||')\">' as acao, e.esddsc, count(*) as tot, ROUND(( (COUNT(*)*100)::numeric / (SELECT COUNT(*) FROM sispacto.universidadecadastro)::numeric ),2) as porcent 
				FROM sispacto.universidadecadastro u 
				INNER JOIN workflow.documento d ON d.docid = u.docidformacaoinicial 
				INNER JOIN workflow.estadodocumento e ON e.esdid = d.esdid 
				GROUP BY e.esdid, e.esddsc";
		$cabecalho = array("&nbsp;","Situação","Quantidade","%");
		$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%',$par2);
		?>
		</td>
	</tr>
	<tr>
		<td>
		<p align="center">Abrangência das Universidades</p>
		<div style="height:100px;overflow:auto;">
		<?
		$sql = "select 
				su.unisigla,
				su.uninome,
				(select count(*) from sispacto.abrangencia a inner join sispacto.pactoidadecerta p on p.muncod = a.muncod inner join sispacto.estruturacurso e on e.ecuid = a.ecuid where abrstatus='A' and uncid=u.uncid and a.esfera='M') as qtdmun,
				round((select count(*) from sispacto.abrangencia a inner join sispacto.pactoidadecerta p on p.muncod = a.muncod inner join sispacto.estruturacurso e on e.ecuid = a.ecuid where abrstatus='A' and uncid=u.uncid and a.esfera='M')*100::numeric/(select count(*) from sispacto.pactoidadecerta where picstatus='A' and muncod is not null)::numeric,2) as mun,
				(select count(distinct a.muncod) from sispacto.abrangencia a 
inner join sispacto.identificacaousuario i on a.muncod = i.muncodatuacao
inner join sispacto.pactoidadecerta p on p.picid = i.picid and p.muncod is null 
inner join sispacto.estruturacurso e on e.ecuid = a.ecuid where e.uncid=u.uncid and a.esfera='E') as qtdest,
				round((select count(distinct a.muncod) from sispacto.abrangencia a 
inner join sispacto.identificacaousuario i on a.muncod = i.muncodatuacao
inner join sispacto.pactoidadecerta p on p.picid = i.picid and p.muncod is null 
inner join sispacto.estruturacurso e on e.ecuid = a.ecuid where e.uncid=u.uncid and a.esfera='E')*100::numeric/(select count(distinct i.muncodatuacao) from sispacto.identificacaousuario i 
inner join sispacto.pactoidadecerta p on p.picid = i.picid and p.muncod is null 
)::numeric,2) as est
				from sispacto.universidadecadastro u 
				inner join sispacto.universidade su ON su.uniid = u.uniid 
				order by 3 desc
		";
		$cabecalho = array("Sigla","Universidade","Qtd Municípios","%Municipal","Qtd Municípios","%Estadual");
		$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%',$par2);
		?>
		</div>
		</td>
	</tr>
	
	<tr>
		<td>
		<p align="center">Abrangência por Estado</p>
		<div style="height:360px;overflow:auto;">
		<?
		$sql = "select 
'<img src=\"../imagens/mais.gif\" title=\"mais\" style=\"cursor:pointer;\" onclick=\"detalharAbrangenciaEstado(\''||foo.estuf||'\',this);\">' as acao, 

foo.descricao, 
foo.qtdmun, 
CASE WHEN foo.totmun > 0 THEN round((foo.qtdmun*100)/foo.totmun,2) ELSE 100.00 END as porcmun,
foo.qtdest, 
CASE WHEN foo.totest > 0 THEN round((foo.qtdest*100)/foo.totest,2) ELSE 100.00 END as porcest

from (
select 
				estuf,				
				estuf||' / '||estdescricao as descricao,
				(select count(*) from sispacto.abrangencia a inner join sispacto.pactoidadecerta p on p.muncod = a.muncod inner join territorios.municipio m on m.muncod = p.muncod where abrstatus='A' and m.estuf=e.estuf and a.esfera='M') as qtdmun,
				(select count(*) from sispacto.pactoidadecerta p inner join territorios.municipio m on m.muncod = p.muncod where picstatus='A' and m.estuf=e.estuf and p.muncod is not null)::numeric as totmun,
				(select count(distinct a.muncod) from sispacto.abrangencia a 
inner join sispacto.identificacaousuario i on a.muncod = i.muncodatuacao
inner join sispacto.pactoidadecerta p on p.picid = i.picid and p.muncod is null 
where p.estuf=e.estuf and a.esfera='E') as qtdest,
				(select count(distinct i.muncodatuacao) from sispacto.identificacaousuario i 
inner join sispacto.pactoidadecerta p on p.picid = i.picid and p.muncod is null 
where p.estuf=e.estuf and i.muncodatuacao!=''
)::numeric as totest
				from territorios.estado e 
) foo 
ORDER BY 1
		";
		$cabecalho = array("&nbsp;","UF","Qtd Municípios","%Municipal","Qtd Municípios","%Estadual");
		$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%',$par2);
		?>
		</div>
		</td>
	</tr>
	
	<tr>
		<td class="SubTituloCentro">Professor Alfabetizador</td>
	</tr>
	
	<tr>
		<td>
		<p align="center">Materiais</p>
		<? exibirMateriaisProfessores(array()); ?>
		</td>
	</tr>
	
	</table>	
	
	</td>
</tr>
</table>
<? elseif($_REQUEST['aba']=='execucao') : ?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloCentro" valign="top" colspan="2">
	<p align="center">Situação dos Pagamentos</p>
	<p align="center">
	<?
    $sql = "SELECT uncid as codigo, uninome as descricao FROM sispacto.universidadecadastro un INNER JOIN sispacto.universidade uni ON uni.uniid = un.uniid ORDER BY uni.uninome";
    $db->monta_combo('uncid_pag', $sql, 'S', 'TODAS', 'selecionarUniversidadePagamento', '', '', '', 'N', 'uncid_pag','', $_REQUEST['uncid_pag']);
    $sql = "SELECT fpbid as codigo, mes.mesdsc||'/'||fpbanoreferencia as descricao FROM sispacto.folhapagamento un INNER JOIN public.meses mes ON mes.mescod::integer = un.fpbmesreferencia";
    $db->monta_combo('fpbid_pag', $sql, 'S', 'TODAS', 'selecionarUniversidadePagamento', '', '', '', 'N', 'fpbid_pag','', $fpbid_padrao); 
	?></p>
	<div id="situacaopagamentos">
	<?
	exibirSituacaoPagamento(array('fpbid'=>$fpbid_padrao));
	?>
	</div>
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
	    $sql = "SELECT uncid as codigo, uninome as descricao FROM sispacto.universidadecadastro un INNER JOIN sispacto.universidade uni ON uni.uniid = un.uniid ORDER BY uni.uninome";
	    $db->monta_combo('uncid', $sql, 'S', 'TODAS', 'selecionarUniversidadeAcesso', '', '', '', 'N', 'uncid','', $_REQUEST['uncid']); 
		?></p>
		<div id="acessousuario">
		<?
		exibirAcessoUsuarioSimec(array());
		?>
		</div>
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
	    $sql = "select '<img src=../imagens/mais.gif title=mais style=\"cursor:pointer;\" onclick=\"detalharMunicipiosturmaNaoFechadas('||foo.uncid||',this);\">' as mais,foo.uninome, foo.fechados, foo.total, round((foo.fechados::numeric/foo.total::numeric)*100,0) from (

select 
un.uncid,
u.uninome, 
(select count(*) from sispacto.abrangencia a 
inner join sispacto.estruturacurso e on e.ecuid = a.ecuid 
inner join sispacto.pactoidadecerta p on p.muncod = a.muncod 
inner join workflow.documento d on d.docid = p.docidturma 
where a.esfera='M' AND e.uncid=un.uncid and d.esdid=630) as fechados,

(select count(*) from sispacto.abrangencia a 
inner join sispacto.estruturacurso e on e.ecuid = a.ecuid 
where a.esfera='M' AND e.uncid=un.uncid) as total
from sispacto.universidadecadastro un 
inner join sispacto.universidade u on u.uniid = un.uniid 
order by 1

) foo

	    ";

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
	    $sql = "SELECT uncid as codigo, uninome as descricao FROM sispacto.universidadecadastro un INNER JOIN sispacto.universidade uni ON uni.uniid = un.uniid ORDER BY uni.uninome";
	    $db->monta_combo('uncid_sit', $sql, 'S', 'TODAS', 'selecionarUniversidadeMensario', '', '', '', 'N', 'uncid_sit','', $_REQUEST['uncid_sit']);
	    $sql = "SELECT fpbid as codigo, mes.mesdsc||'/'||fpbanoreferencia as descricao FROM sispacto.folhapagamento un INNER JOIN public.meses mes ON mes.mescod::integer = un.fpbmesreferencia";
	    $db->monta_combo('fpbid_sit', $sql, 'S', 'TODAS', 'selecionarUniversidadeMensario', '', '', '', 'N', 'fpbid_sit','', $fpbid_padrao); 
		?></p>
		<div id="situacaomensario">
		<?
		exibirSituacaoMensario(array('fpbid'=>$fpbid_padrao));
		?>
		</div>
		</td>
	</tr>
	<tr>
		<td>
		<p align="center">Porcentagem dos Pagamentos</p>
		<p align="center"><?
	    $sql = "SELECT uncid as codigo, uninome as descricao FROM sispacto.universidadecadastro un INNER JOIN sispacto.universidade uni ON uni.uniid = un.uniid ORDER BY uni.uninome";
	    $db->monta_combo('uncid_por', $sql, 'S', 'TODAS', 'selecionarUniversidadePagamentoPorcent', '', '', '', 'N', 'uncid_por','', $_REQUEST['uncid_por']);
		?></p>
		<div id="porcentagempagamentos">
		<?
		exibirPorcentagemPagamento(array());
		?>
		</div>
		</td>
	</tr>
	
	</table>	
	
	</td>
</tr>
</table>
<? endif; ?>