<?
function resumoGeralCurso($dados) {
	global $db;
	
	$sql = "SELECT count(*) as num FROM sisindigena.identificacaousuario i 
			INNER JOIN sisindigena.tipoperfil t ON t.iusd = i.iusd 
			WHERE t.pflcod='".$dados['pflcod']."'";
	
	$qtd_total_participantes = $db->pegaUm($sql);
	
	$sql = "SELECT count(*) as num FROM sisindigena.certificados c 
			INNER JOIN sisindigena.tipoperfil t ON t.iusd = c.iusd 
			WHERE t.pflcod='".$dados['pflcod']."' AND c.ceropcao='A'";
	
	$qtd_total_certificados = $db->pegaUm($sql);
	
	$sql = "SELECT count(*) as qtd, sum(pbovlrpagamento) as vlr FROM sisindigena.pagamentobolsista p 
			INNER JOIN workflow.documento d ON d.docid = p.docid  
			WHERE pflcod='".$dados['pflcod']."' AND d.esdid='".ESD_PAGAMENTO_EFETIVADO."'";
	
	$arrPagamentos = $db->pegaLinha($sql);
	
	$sql = "SELECT count(*) as qtd, sum(pbovlrpagamento) as vlr FROM sisindigena.pagamentobolsista p
			INNER JOIN workflow.documento d ON d.docid = p.docid 
			INNER JOIN sisindigena.identificacaousuario i ON i.iusd = p.iusd 
			LEFT JOIN sisindigena.certificados c ON c.iusd = p.iusd 
			WHERE pflcod='".$dados['pflcod']."' AND d.esdid='".ESD_PAGAMENTO_EFETIVADO."' AND (i.iusstatus='I' OR c.ceropcao='N')";
	
	$arrPagamentosD = $db->pegaLinha($sql);
	
	
	$pfldsc = $db->pegaUm("SELECT pfldsc FROM seguranca.perfil WHERE pflcod='".$dados['pflcod']."'");
	
	echo '<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">';
	
	echo '<tr>';
	echo '<td class="SubTituloCentro" colspan="2">'.$pfldsc.'</td>';
	echo '</tr>';
	
	echo '<tr>';
	echo '<td class="SubTituloDireita"><span style=font-size:x-small;>1 - Total participantes</span></td>';
	echo '<td><span style=font-size:x-small;>'.$qtd_total_participantes.'</span></td>';
	echo '</tr>';
	
	echo '<tr>';
	echo '<td class="SubTituloDireita"><span style=font-size:x-small;>2 - Total certificados</span></td>';
	echo '<td><span style=font-size:x-small;>'.$qtd_total_certificados.' ( '.round(($qtd_total_certificados/$qtd_total_participantes)*100,2).'% )</span></td>';
	echo '</tr>';
	
	echo '<tr>';
	echo '<td class="SubTituloDireita"><span style=font-size:x-small;>3 - Total (R$) pago</span></td>';
	echo '<td><span style=font-size:x-small;>'.number_format($arrPagamentos['vlr'],2,",",".").' - '.$arrPagamentos['qtd'].' bolsas</span></td>';
	echo '</tr>';
	
	echo '<tr>';
	echo '<td class="SubTituloDireita"><span style=font-size:x-small;>4 - Total (R$) pago para desistentes ou não certificados</span></td>';
	echo '<td><span style=font-size:x-small;>'.number_format($arrPagamentosD['vlr'],2,",",".").' ( '.round(($arrPagamentosD['vlr']/$arrPagamentos['vlr'])*100,2).'% ) - '.$arrPagamentosD['qtd'].' bolsas</span></td>';
	echo '</tr>';
	
	echo '<tr>';
	echo '<td class="SubTituloDireita"><span style=font-size:x-small;>5 - Total (R$) pago / certificado</span></td>';
	echo '<td><span style=font-size:x-small;>'.(($qtd_total_certificados)?number_format(($arrPagamentos['vlr']/$qtd_total_certificados),2,",","."):'0,00').'</span></td>';
	echo '</tr>';
	
	
	
	
	echo '</table>';
	
	
	$sql = "SELECT '<span onmouseover=\"return escape(\''||uu.uninome||'\');\">'||uu.unisigla||'</span>' as universidade, u.picid FROM sisindigena.nucleouniversidade u 
			INNER JOIN sisindigena.universidade uu ON uu.uniid = u.uniid 
			WHERE u.picstatus='A'";
	
	$nucleouniversidade = $db->carregar($sql);
	
	if($nucleouniversidade[0]) {
		
		echo '<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">';
		
		echo '<tr>';
		echo '<td class="SubTituloCentro"><span style=font-size:x-small;>IES</span></td>';
		echo '<td class="SubTituloCentro"><span style=font-size:x-small;>1</span></td>';
		echo '<td class="SubTituloCentro"><span style=font-size:x-small;>2</span></td>';
		echo '<td class="SubTituloCentro"><span style=font-size:x-small;>3</span></td>';
		echo '<td class="SubTituloCentro"><span style=font-size:x-small;>4</span></td>';
		echo '<td class="SubTituloCentro"><span style=font-size:x-small;>5</span></td>';
		echo '</tr>';
		
		
		foreach($nucleouniversidade as $nuc) {
			
			
			$sql = "SELECT count(*) as num FROM sisindigena.identificacaousuario i
			INNER JOIN sisindigena.tipoperfil t ON t.iusd = i.iusd
			WHERE t.pflcod='".$dados['pflcod']."' AND i.picid='".$nuc['picid']."'";
			
			$qtd_total_participantes = $db->pegaUm($sql);
			
			$sql = "SELECT count(*) as num FROM sisindigena.certificados c 
			INNER JOIN sisindigena.identificacaousuario i ON i.iusd = c.iusd
			INNER JOIN sisindigena.tipoperfil t ON t.iusd = c.iusd
			WHERE t.pflcod='".$dados['pflcod']."' AND c.ceropcao='A' AND i.picid='".$nuc['picid']."'";
			
			$qtd_total_certificados = $db->pegaUm($sql);
			
			$sql = "SELECT count(*) as qtd, sum(pbovlrpagamento) as vlr FROM sisindigena.pagamentobolsista p 
			INNER JOIN sisindigena.identificacaousuario i ON i.iusd = p.iusd 
			INNER JOIN workflow.documento d ON d.docid = p.docid
			WHERE pflcod='".$dados['pflcod']."' AND d.esdid='".ESD_PAGAMENTO_EFETIVADO."' AND i.picid='".$nuc['picid']."'";
			
			$arrPagamentos = $db->pegaLinha($sql);
			
			$sql = "SELECT count(*) as qtd, sum(pbovlrpagamento) as vlr FROM sisindigena.pagamentobolsista p
			INNER JOIN workflow.documento d ON d.docid = p.docid
			INNER JOIN sisindigena.identificacaousuario i ON i.iusd = p.iusd
			LEFT JOIN sisindigena.certificados c ON c.iusd = p.iusd
			WHERE pflcod='".$dados['pflcod']."' AND d.esdid='".ESD_PAGAMENTO_EFETIVADO."' AND (i.iusstatus='I' OR c.ceropcao='N') AND i.picid='".$nuc['picid']."'";
			
			$arrPagamentosD = $db->pegaLinha($sql);
				

			echo '<tr>';
			echo '<td class="SubTituloDireita"><span style=font-size:x-small;>'.$nuc['universidade'].'</span></td>';
			echo '<td><span style=font-size:x-small;float:right;>'.(($qtd_total_participantes)?$qtd_total_participantes:'0').'</span></td>';
			echo '<td><span style=font-size:x-small;float:right;>'.(($qtd_total_participantes)?(($qtd_total_certificados)?$qtd_total_certificados:'0').' ( '.round(($qtd_total_certificados/$qtd_total_participantes)*100,2).'% )':'0').'</span></td>';
			echo '<td><span style=font-size:x-small;float:right;>'.number_format($arrPagamentos['vlr'],2,",",".").' - '.$arrPagamentos['qtd'].' bolsas</span></td>';
			echo '<td><span style=font-size:x-small;float:right;>'.number_format($arrPagamentosD['vlr'],2,",",".").' ( '.(($arrPagamentos['vlr'])?round(($arrPagamentosD['vlr']/$arrPagamentos['vlr'])*100,2):'0,00').'% ) - '.$arrPagamentosD['qtd'].' bolsas</span></td>';
			echo '<td><span style=font-size:x-small;float:right;>'.(($qtd_total_certificados)?number_format(($arrPagamentos['vlr']/$qtd_total_certificados),2,",","."):'0,00').'</span></td>';
			echo '</tr>';

			
			
			
		}
		
		echo '</table>';
	}
	
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
			CASE WHEN foo.esdid=".ESD_APROVADO_MENSARIO." THEN 'Aprovado'
						 	  WHEN foo.mensarionota > 7  AND foo.iustermocompromisso=true  THEN 'Apto'
		    ELSE 'Não Apto' END resultado, foo.fpbid FROM (
	SELECT
	COALESCE((SELECT AVG(mavtotal) FROM sisindigena.mensarioavaliacoes ma  WHERE ma.menid=m.menid),0.00) as mensarionota,
	uu.uninome,
	i.iusdocumento,
	i.iustermocompromisso,
	m.fpbid,
	d.esdid,
	t.pflcod,
	i.iustipoprofessor,
	(SELECT COUNT(mavid) FROM sisindigena.mensarioavaliacoes ma  WHERE ma.menid=m.menid) as numeroavaliacoes
	FROM sisindigena.mensario m
	INNER JOIN sisindigena.identificacaousuario i ON i.iusd = m.iusd
	INNER JOIN sisindigena.universidadecadastro un ON un.uncid = i.uncid
	INNER JOIN sisindigena.universidade uu ON uu.uniid = un.uniid
	INNER JOIN sisindigena.tipoperfil t ON t.iusd = i.iusd
	INNER JOIN workflow.documento d ON d.docid = m.docid

	) foo WHERE foo.pflcod='".$dados['pflcod']."' and foo.fpbid='".$dados['fpbid']."') foo2) foo3 GROUP BY foo3.uninome";

	$cabecalho = array("Universidade","Não Apto","Apto","Aprovados");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%',$par2);


}


function carregarDetalhesStatusUsuarios($dados) {
	global $db;

	$sql = "select
uu.uninome,
(select count(*) from sisindigena.identificacaousuario u
 inner join sisindigena.tipoperfil t on t.iusd=u.iusd and t.pflcod=".$dados['pflcod']." and u.uncid=un.uncid) as usutotal,

(select count(*) from sisindigena.identificacaousuario u
 inner join sisindigena.tipoperfil t on t.iusd=u.iusd and t.pflcod=".$dados['pflcod']."
 inner join seguranca.perfilusuario pu on pu.usucpf=u.iuscpf and pu.pflcod=t.pflcod
 inner join seguranca.usuario_sistema us on us.usucpf=u.iuscpf and us.sisid=".SIS_INDIGENA."
 where us.suscod='A' and u.uncid=un.uncid) as usuativos,

(select count(*) from sisindigena.identificacaousuario u
 inner join sisindigena.tipoperfil t on t.iusd=u.iusd and t.pflcod=".$dados['pflcod']."
 inner join seguranca.perfilusuario pu on pu.usucpf=u.iuscpf and pu.pflcod=t.pflcod
 inner join seguranca.usuario_sistema us on us.usucpf=u.iuscpf and us.sisid=".SIS_sisindigena."
 where us.suscod='P' and u.uncid=un.uncid) as usupendentes,

(select count(*) from sisindigena.identificacaousuario u
 inner join sisindigena.tipoperfil t on t.iusd=u.iusd and t.pflcod=".$dados['pflcod']."
 inner join seguranca.perfilusuario pu on pu.usucpf=u.iuscpf and pu.pflcod=t.pflcod
 inner join seguranca.usuario_sistema us on us.usucpf=u.iuscpf and us.sisid=".SIS_INDIGENA."
 where us.suscod='B' and u.uncid=un.uncid) as usubloqueado,

 (select count(*) from sisindigena.identificacaousuario u
 inner join sisindigena.tipoperfil t on t.iusd=u.iusd and t.pflcod=".$dados['pflcod']."
 left join seguranca.perfilusuario pu on pu.usucpf=u.iuscpf and pu.pflcod=t.pflcod
 left join seguranca.usuario_sistema us on us.usucpf=u.iuscpf and us.sisid=".SIS_sisindigena."
 where (us.suscod is null or pu.pflcod is null) and u.uncid=un.uncid) as usunaocadastrado


from sisindigena.universidadecadastro un
inner join sisindigena.universidade uu on uu.uniid = un.uniid";

	$cabecalho = array("Universidade","Total","Ativos","Pendentes","Bloqueados","Não cadastrados");
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
			INNER JOIN sisindigena.pagamentobolsista pb ON pb.pflcod = p.pflcod
			INNER JOIN sisindigena.universidadecadastro un ON un.uniid = pb.uniid
			INNER JOIN sisindigena.universidade uu ON uu.uniid = un.uniid
			INNER JOIN workflow.documento dc ON dc.docid = pb.docid AND dc.tpdid=".TPD_PAGAMENTOBOLSA."
			WHERE p.pflcod IN(
			".PFL_PROFESSORALFABETIZADOR.",
			".PFL_COORDENADORLOCAL.",
			".PFL_ORIENTADORESTUDO.",
			".PFL_COORDENADORIES.",
			".PFL_COORDENADORADJUNTOIES.",
			".PFL_SUPERVISORIES.",
			".PFL_FORMADORIES.",
			".PFL_CONTEUDISTA.",
			".PFL_PESQUISADOR.") ".(($wh)?" AND ".implode(" AND ",$wh):"")."

			) fee

			GROUP BY fee.universidade
		
			) foo
		
			INNER JOIN sisindigena.pagamentoperfil pp ON pp.pflcod = '".$dados['pflcod']."'";
	

	$cabecalho = array("Universidade","Aguardando autorização IES","R$","Autorizado IES","R$","Aguardando autorização SGB","R$","Aguardando pagamento","R$","Enviado ao Banco","R$","Pagamento efetivado","R$","Pagamento recusado","R$","Pagamento não autorizado FNDE","R$");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%',$par2);

}

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>';
echo '<script language="javascript" type="text/javascript" src="./js/sisindigena.js"></script>';

echo "<br>";

$fpbid_padrao = $db->pegaUm("SELECT fpbid FROM sisindigena.folhapagamento WHERE fpbanoreferencia='".date("Y")."' AND fpbmesreferencia='".date("m")."'");

?>
<script>
function acessarCadastroOrientadores(esdid,esfera) {
	window.location='sisindigena.php?modulo=principal/coordenadorlocal/listacoordenadorlocal&acao=A&esdid='+esdid+'&esfera='+esfera;
}

function acessarComposicaoTurmas(esdid,esfera) {
	window.location='sisindigena.php?modulo=principal/coordenadorlocal/listacoordenadorlocal&acao=A&esdidcomposicaoturmas='+esdid+'&esfera='+esfera;
}

function acessarUniversidades(esdid) {
	window.location='sisindigena.php?modulo=principal/universidade/listauniversidade&acao=A&esdid='+esdid;
}

function acessarUniversidadesFormacaoInicial(esdid) {
	window.location='sisindigena.php?modulo=principal/universidade/listauniversidade&acao=A&esdidformacaoinicial='+esdid;
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

$menu[] = array("id" => 1, "descricao" => 'Etapa Projeto', "link" => '/sisindigena/sisindigena.php?modulo=centralacompanhamento&acao=A&aba=projeto');
$menu[] = array("id" => 2, "descricao" => 'Etapa Execução', "link" => '/sisindigena/sisindigena.php?modulo=centralacompanhamento&acao=A&aba=execucao');
$menu[] = array("id" => 3, "descricao" => 'Resumo geral do curso', "link" => '/sisindigena/sisindigena.php?modulo=centralacompanhamento&acao=A&aba=resumo');
echo "<br>";

$fpbid_padrao = $db->pegaUm("SELECT fpbid FROM sisindigena.folhapagamento WHERE fpbanoreferencia='".date("Y")."' AND fpbmesreferencia='".date("m")."'");

if(!$_REQUEST['aba']) $_REQUEST['aba']='projeto';

echo montarAbasArray($menu, '/sisindigena/sisindigena.php?modulo=centralacompanhamento&acao=A&aba='.$_REQUEST['aba']);

if($_REQUEST['aba']=='projeto') :
?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloCentro" colspan="2">Quantitativos de bolsistas por universidade</td>
</tr>
<tr>
	<td class="SubTituloCentro" valign="top" colspan="2">
	<? 
	$sql = "SELECT p.pflcod, p.pfldsc FROM sisindigena.pagamentoperfil pp INNER JOIN seguranca.perfil p ON p.pflcod = pp.pflcod WHERE plpstatus='A' ORDER BY pflnivel";
	$pagamentoperfil = $db->carregar($sql);
		
	$cabecalho = array("Universidade");
		
	if($pagamentoperfil[0]) {
		foreach($pagamentoperfil as $pp) {
			$cl[] = "(SELECT count(*) FROM sisindigena.identificacaousuario i INNER JOIN sisindigena.tipoperfil t ON t.iusd = i.iusd WHERE CASE WHEN t.pflcod='".PFL_COORDENADORIES."' THEN i.uncid = u.uncid AND u.picsede=true ELSE i.picid = u.picid END AND t.pflcod='".$pp['pflcod']."') as p_{$pp['pflcod']}";
			$cabecalho[] = $pp['pfldsc'];
		}
	}
		
	$sql = "SELECT uu.unisigla||' - '||uu.uninome as universidade,
        				   ".implode(",",$cl)."
        			FROM sisindigena.nucleouniversidade u
					INNER JOIN sisindigena.universidade uu ON uu.uniid = u.uniid";
		
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%',$par2);
	
	?>
	</td>
</tr>
<tr>
	<td class="SubTituloCentro" valign="top" width="50%">
	
	<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
	<tr>
		<td class="SubTituloCentro">1. Coordenador Geral da IES</td>
	</tr>
	<tr>
		<td>
		<p align="center">Situação do Cadastramento</p>
		<?
		$sql = "SELECT COALESCE(e.esddsc,'Não iniciou Elaboração'), 
       				   COUNT(*) as tot
				FROM sisindigena.universidadecadastro u 
				LEFT JOIN workflow.documento d ON d.docid = u.docid 
				LEFT JOIN workflow.estadodocumento e ON e.esdid = d.esdid 
				WHERE u.uncstatus='A'
				GROUP BY e.esddsc, e.esdid 
				ORDER BY 1 DESC";
		$cabecalho = array("Situação","Quantidade");
		$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%',$par2);
		?>
		</td>
	</tr>
	<tr>
		<td>
		<p align="center">Orçamento Total(R$) e Quantidade de Núcleos</p>
		<?
		$sql = "SELECT uu.unisigla || ' - ' || uu.uninome as universidade, 
				       SUM(orcvlrunitario) as orcvlrunitario, 
				       (SELECT COUNT(*) FROM sisindigena.nucleouniversidade WHERE uncid=o.uncid AND picstatus='A') as nnucleos
				FROM sisindigena.orcamento o 
				INNER JOIN sisindigena.universidadecadastro u ON u.uncid = o.uncid 
				INNER JOIN sisindigena.universidade uu ON uu.uniid = u.uniid 
				WHERE o.orcstatus='A'
				GROUP BY uu.unisigla, uu.uninome, o.uncid 
				ORDER BY 1";
		$cabecalho = array("Universidade","Orçamento Total(R$)","Quantidade de Núcleo");
		$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%',$par2);
		?>
		</td>
	</tr>
	

	</table>	
	
	
	</td>
	
	<td class="SubTituloCentro" valign="top" width="50%">
	
	<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
	<tr>
		<td class="SubTituloCentro">2. Coordenador Adjunto da IES</td>
	</tr>
	<tr>
		<td>
		<p align="center">Situação do Cadastramento</p>
		<?
		$sql = "SELECT COALESCE(e.esddsc,'Não iniciou Elaboração'), 
       				   COUNT(*) as tot
				FROM sisindigena.nucleouniversidade n 
				LEFT JOIN workflow.documento d ON d.docid = n.docid 
				LEFT JOIN workflow.estadodocumento e ON e.esdid = d.esdid 
				WHERE n.picstatus='A'
				GROUP BY e.esddsc, e.esdid 
				ORDER BY 1 DESC";
		$cabecalho = array("Situação","Quantidade");
		$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%',$par2);
		?>
		</td>
	</tr>
	<tr>
		<td>
		<p align="center">Perfis Cadastrados pelo Coordenadores Adjuntos da IES</p>
		<?
		$sql = "SELECT p.pfldsc, COUNT(*) as tot FROM sisindigena.identificacaousuario i 
				INNER JOIN sisindigena.tipoperfil t ON i.iusd = t.iusd 
				INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
				WHERE p.pflcod IN('".PFL_COORDENADORLOCAL."','".PFL_FORMADORIES."','".PFL_SUPERVISORIES."','".PFL_CONTEUDISTA."','".PFL_PESQUISADOR."') 
				GROUP BY p.pfldsc 
				ORDER BY 1";
		$cabecalho = array("Perfil","Quantidade");
		$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%',$par2);
		?>
		</td>
	</tr>
	
	</table>
	
	<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
	<tr>
		<td class="SubTituloCentro">3. Supervisor da IES</td>
	</tr>
	<tr>
		<td>
		<p align="center">Situação do Cadastramento</p>
		<?
		$sql = "SELECT COALESCE(e.esddsc,'Não iniciou Elaboração'), 
       				   COUNT(*) as tot
				FROM sisindigena.turmas n 
				INNER JOIN sisindigena.tipoperfil t ON t.iusd = n.iusd 
				LEFT JOIN workflow.documento d ON d.docid = n.docid 
				LEFT JOIN workflow.estadodocumento e ON e.esdid = d.esdid 
				WHERE t.pflcod=".PFL_SUPERVISORIES."
				GROUP BY e.esddsc, e.esdid 
				ORDER BY 1 DESC";
		$cabecalho = array("Situação","Quantidade");
		$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%',$par2);
		?>
		</td>
	</tr>
	<tr>
		<td>
		<p align="center">Perfis Cadastrados pelo Supervisores da IES</p>
		<?
		$sql = "SELECT p.pfldsc, COUNT(*) as tot FROM sisindigena.identificacaousuario i 
				INNER JOIN sisindigena.tipoperfil t ON i.iusd = t.iusd 
				INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
				WHERE p.pflcod IN('".PFL_ORIENTADORESTUDO."','".PFL_PROFESSORALFABETIZADOR."') 
				GROUP BY p.pfldsc 
				ORDER BY 1";
		$cabecalho = array("Perfil","Quantidade");
		$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%',$par2);
		?>
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
    $sql = "SELECT uncid as codigo, uninome as descricao FROM sisindigena.universidadecadastro un INNER JOIN sisindigena.universidade uni ON uni.uniid = un.uniid ORDER BY uni.uninome";
    $db->monta_combo('uncid_pag', $sql, 'S', 'TODAS', 'selecionarUniversidadePagamento', '', '', '', 'N', 'uncid_pag','', $_REQUEST['uncid_pag']);
    $sql = "SELECT fpbid as codigo, mes.mesdsc||'/'||fpbanoreferencia as descricao FROM sisindigena.folhapagamento un INNER JOIN public.meses mes ON mes.mescod::integer = un.fpbmesreferencia";
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
	    $sql = "SELECT uncid as codigo, uninome as descricao FROM sisindigena.universidadecadastro un INNER JOIN sisindigena.universidade uni ON uni.uniid = un.uniid ORDER BY uni.uninome";
	    $db->monta_combo('uncid', $sql, 'S', 'TODAS', 'selecionarUniversidadeAcesso', '', '', '', 'N', 'uncid','', $_REQUEST['uncid']); 
		?></p>
		<div id="acessousuario">
		<?
		exibirAcessoUsuarioSimec(array());
		?>
		</div>
		</td>
	</tr>

	<tr>
		<td valign="top">
		<p align="center">Usuários que preencheram termo de Compromisso</p>
		<p align="center"><?
	    $sql = "select foo.pfldsc, sum(foo.termo) as termo, sum(qtd), round((sum(foo.termo)::numeric/sum(qtd)::numeric)*100,0) as qtd from (
					select p.pfldsc, case when iustermocompromisso=true then 1 else 0 end termo, 1 as qtd from sisindigena.identificacaousuario i 
					inner join sisindigena.tipoperfil t on t.iusd = i.iusd 
					inner join seguranca.perfil p on p.pflcod = t.pflcod 
					where i.iusstatus='A' 
				) foo
				group by foo.pfldsc
				order by 4";
	    
	    $cabecalho = array("Perfil","Qtd termo","Qtd Total","Porc");
	    $db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%',$par2); 
		?></p>
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
	    $sql = "SELECT uncid as codigo, uninome as descricao FROM sisindigena.universidadecadastro un INNER JOIN sisindigena.universidade uni ON uni.uniid = un.uniid ORDER BY uni.uninome";
	    $db->monta_combo('uncid_sit', $sql, 'S', 'TODAS', 'selecionarUniversidadeMensario', '', '', '', 'N', 'uncid_sit','', $_REQUEST['uncid_sit']);
	    $sql = "SELECT fpbid as codigo, mes.mesdsc||'/'||fpbanoreferencia as descricao FROM sisindigena.folhapagamento un INNER JOIN public.meses mes ON mes.mescod::integer = un.fpbmesreferencia";
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
	    $sql = "SELECT uncid as codigo, uninome as descricao FROM sisindigena.universidadecadastro un INNER JOIN sisindigena.universidade uni ON uni.uniid = un.uniid ORDER BY uni.uninome";
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