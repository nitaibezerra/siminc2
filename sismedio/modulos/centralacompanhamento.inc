<?

ini_set("memory_limit", "1024M");
set_time_limit(0);

function resumoGeralCurso($dados) {
	global $db;

	$sql = "SELECT count(*) as num FROM sismedio.identificacaousuario i
			INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd
			WHERE t.pflcod in(".$dados['pflcod'].")";

	$qtd_total_participantes = $db->pegaUm($sql);

	$sql = "SELECT count(*) as num FROM sismedio.certificacao c
			WHERE c.pflcod in(".$dados['pflcod'].") AND c.cerfrequencia>=75";

	$qtd_total_certificados = $db->pegaUm($sql);

	$sql = "SELECT count(*) as qtd, sum(pbovlrpagamento) as vlr FROM sismedio.pagamentobolsista p
			INNER JOIN workflow.documento d ON d.docid = p.docid
			WHERE pflcod in(".$dados['pflcod'].") AND d.esdid='".ESD_PAGAMENTO_EFETIVADO."'";

	$arrPagamentos = $db->pegaLinha($sql);

	$sql = "SELECT count(*) as qtd, sum(pbovlrpagamento) as vlr FROM sismedio.pagamentobolsista p
			INNER JOIN workflow.documento d ON d.docid = p.docid
			INNER JOIN sismedio.certificacao c ON c.iusd = p.iusd
			WHERE p.pflcod in(".$dados['pflcod'].") AND d.esdid='".ESD_PAGAMENTO_EFETIVADO."' AND c.cerfrequencia<75";

	$arrPagamentosD = $db->pegaLinha($sql);


	$pfldsc = $db->carregarColuna("SELECT pfldsc FROM seguranca.perfil WHERE pflcod in(".$dados['pflcod'].")");

	echo '<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">';

	echo '<tr>';
	echo '<td class="SubTituloCentro" colspan="2">'.(($pfldsc)?implode(' / ',$pfldsc):'').'</td>';
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


	$sql = "SELECT '<span onmouseover=\"return escape(\''||uu.uninome||'\');\">'||uu.unisigla||'</span>' as universidade, u.uncid FROM sismedio.universidadecadastro u
			INNER JOIN sismedio.universidade uu ON uu.uniid = u.uniid";

	$universidadecadastro = $db->carregar($sql);

	if($universidadecadastro[0]) {

		echo '<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">';

		echo '<tr>';
		echo '<td class="SubTituloCentro"><span style=font-size:x-small;>IES</span></td>';
		echo '<td class="SubTituloCentro"><span style=font-size:x-small;>1</span></td>';
		echo '<td class="SubTituloCentro"><span style=font-size:x-small;>2</span></td>';
		echo '<td class="SubTituloCentro"><span style=font-size:x-small;>3</span></td>';
		echo '<td class="SubTituloCentro"><span style=font-size:x-small;>4</span></td>';
		echo '<td class="SubTituloCentro"><span style=font-size:x-small;>5</span></td>';
		echo '</tr>';


		foreach($universidadecadastro as $unc) {


			$sql = "SELECT count(*) as num FROM sismedio.identificacaousuario i
			INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd
			WHERE t.pflcod in(".$dados['pflcod'].") AND i.uncid='".$unc['uncid']."'";

			$qtd_total_participantes = $db->pegaUm($sql);

			$sql = "SELECT count(*) as num FROM sismedio.certificacao c
			INNER JOIN sismedio.identificacaousuario i ON i.iusd = c.iusd
			INNER JOIN sismedio.tipoperfil t ON t.iusd = c.iusd
			WHERE t.pflcod in(".$dados['pflcod'].") AND c.cerfrequencia>=75 AND i.uncid='".$unc['uncid']."'";

			$qtd_total_certificados = $db->pegaUm($sql);

			$sql = "SELECT count(*) as qtd, sum(pbovlrpagamento) as vlr FROM sismedio.pagamentobolsista p
			INNER JOIN sismedio.identificacaousuario i ON i.iusd = p.iusd
			INNER JOIN workflow.documento d ON d.docid = p.docid
			WHERE pflcod in(".$dados['pflcod'].") AND d.esdid='".ESD_PAGAMENTO_EFETIVADO."' AND i.uncid='".$unc['uncid']."'";

			$arrPagamentos = $db->pegaLinha($sql);

			$sql = "SELECT count(*) as qtd, sum(pbovlrpagamento) as vlr FROM sismedio.pagamentobolsista p
			INNER JOIN workflow.documento d ON d.docid = p.docid
			INNER JOIN sismedio.identificacaousuario i ON i.iusd = p.iusd
			INNER JOIN sismedio.certificacao c ON c.iusd = p.iusd
			WHERE p.pflcod in(".$dados['pflcod'].") AND d.esdid='".ESD_PAGAMENTO_EFETIVADO."' AND c.cerfrequencia<75 AND i.uncid='".$unc['uncid']."'";

			$arrPagamentosD = $db->pegaLinha($sql);


			echo '<tr>';
			echo '<td class="SubTituloDireita"><span style=font-size:x-small;>'.$unc['universidade'].'</span></td>';
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


function exibirEscolasNaoAtendidas($dados) {
	global $db;
	
	$sql = "select			   '<span style=font-size:x-small;>'||l.lemcodigoinep||'</span>' as inep,
							   '<span style=font-size:x-small;>'||lemnomeescola||'</span>' as escola,
							   '<span style=font-size:x-small;>'||coalesce(e.esddsc,'Não iniciou')||'</span>' as situacao
						from sismedio.listaescolasensinomedio l 
						left join workflow.documento d on d.docid = l.docid 
						left join workflow.estadodocumento e on e.esdid = d.esdid 
						left join sismedio.abrangencia a on a.lemcodigoinep = l.lemcodigoinep
						where a.abrid is null and lemuf='".$dados['lemuf']."' order by lemuf";
	
	$cabecalho = array("<span style=font-size:x-small>INEP</span>","<span style=font-size:x-small>Escola</span>","<span style=font-size:x-small>Situação</span>");
	$db->monta_lista_simples($sql,$cabecalho,10000,5,'N','100%','',true, false, false, true);
	
}

function carregarDetalhesCadastramentoEscolasUF($dados) {
	global $db;
	
	if($dados['esdid']=='9999999') $wh = "d.esdid IS NULL";
	else $wh = "d.esdid='".$dados['esdid']."'";
	
	$sql = "SELECT lemuf, count(*) as tot, round((count(*) / ( select count(*) FROM sismedio.listaescolasensinomedio WHERE lemuf=l.lemuf)::numeric)*100,2) as porc FROM sismedio.listaescolasensinomedio l 
			LEFT JOIN workflow.documento d ON d.docid = l.docid 
			WHERE {$wh} 
			GROUP BY lemuf";
	
	$cabecalho = array("UF","Total","%");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%',$par2);
	
	
}


function carregarDetalhesStatusUsuarios($dados) {
	global $db;
	
	$sql = "select 
uu.uninome, 
(select count(*) from sismedio.identificacaousuario u 
 inner join sismedio.tipoperfil t on t.iusd=u.iusd and t.pflcod=".$dados['pflcod']." and u.uncid=un.uncid) as usutotal,

(select count(*) from sismedio.identificacaousuario u 
 inner join sismedio.tipoperfil t on t.iusd=u.iusd and t.pflcod=".$dados['pflcod']." 
 inner join seguranca.perfilusuario pu on pu.usucpf=u.iuscpf and pu.pflcod=t.pflcod 
 inner join seguranca.usuario_sistema us on us.usucpf=u.iuscpf and us.sisid=".SIS_MEDIO." 
 where us.suscod='A' and u.uncid=un.uncid) as usuativos,

(select count(*) from sismedio.identificacaousuario u 
 inner join sismedio.tipoperfil t on t.iusd=u.iusd and t.pflcod=".$dados['pflcod']." 
 inner join seguranca.perfilusuario pu on pu.usucpf=u.iuscpf and pu.pflcod=t.pflcod 
 inner join seguranca.usuario_sistema us on us.usucpf=u.iuscpf and us.sisid=".SIS_MEDIO." 
 where us.suscod='P' and u.uncid=un.uncid) as usupendentes,

(select count(*) from sismedio.identificacaousuario u 
 inner join sismedio.tipoperfil t on t.iusd=u.iusd and t.pflcod=".$dados['pflcod']." 
 inner join seguranca.perfilusuario pu on pu.usucpf=u.iuscpf and pu.pflcod=t.pflcod 
 inner join seguranca.usuario_sistema us on us.usucpf=u.iuscpf and us.sisid=".SIS_MEDIO." 
 where us.suscod='B' and u.uncid=un.uncid) as usubloqueado,

 (select count(*) from sismedio.identificacaousuario u 
 inner join sismedio.tipoperfil t on t.iusd=u.iusd and t.pflcod=".$dados['pflcod']." 
 left join seguranca.perfilusuario pu on pu.usucpf=u.iuscpf and pu.pflcod=t.pflcod 
 left join seguranca.usuario_sistema us on us.usucpf=u.iuscpf and us.sisid=".SIS_MEDIO." 
 where (us.suscod is null or pu.pflcod is null) and u.uncid=un.uncid) as usunaocadastrado
 

from sismedio.universidadecadastro un 
inner join sismedio.universidade uu on uu.uniid = un.uniid";
	
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
					ROUND(( (COUNT(*)*100)::numeric / (SELECT COUNT(*) FROM sismedio.pactoidadecerta)::numeric ),2) as porcent 
			FROM sismedio.pactoidadecerta p 
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
	
	$sql = "select m.estuf, m.mundescricao, e.esddsc from sismedio.pactoidadecerta p 
			inner join territorios.municipio m on m.muncod = p.muncod 
			inner join workflow.documento d on d.docid = p.docid 
			inner join workflow.estadodocumento e on e.esdid = d.esdid
			where picstatus='A' and m.estuf='".$dados['estuf']."' and p.muncod is not null and p.muncod not in (select muncod from sismedio.abrangencia where esfera='M')";
	
	$cabecalho = array("UF","Município","Situação");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%',$par2);
	
	echo "<p>Municípios sem abrangência - Rede Estadual</p>";
	
	$sql = "select  distinct m.estuf, m.mundescricao, e.esddsc from sismedio.identificacaousuario i 
inner join sismedio.pactoidadecerta p on p.picid = i.picid and p.muncod is null 
inner join territorios.municipio m on m.muncod = i.muncodatuacao 
inner join workflow.documento d on d.docid = p.docid 
inner join workflow.estadodocumento e on e.esdid = d.esdid
where p.estuf='".$dados['estuf']."' and i.muncodatuacao not in (select muncod from sismedio.abrangencia where esfera='E')";
	
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
	COALESCE((SELECT AVG(mavtotal) FROM sismedio.mensarioavaliacoes ma  WHERE ma.menid=m.menid),0.00) as mensarionota,
	uu.uninome,
	i.iusdocumento,
	i.iustermocompromisso,
	m.fpbid,
	d.esdid,
	t.pflcod,
	i.iustipoprofessor,
	(SELECT COUNT(mavid) FROM sismedio.mensarioavaliacoes ma  WHERE ma.menid=m.menid) as numeroavaliacoes
	FROM sismedio.mensario m
	INNER JOIN sismedio.identificacaousuario i ON i.iusd = m.iusd 
	INNER JOIN sismedio.universidadecadastro un ON un.uncid = i.uncid 
	INNER JOIN sismedio.universidade uu ON uu.uniid = un.uniid 
	INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd  
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
			INNER JOIN sismedio.pagamentobolsista pb ON pb.pflcod = p.pflcod 
			INNER JOIN sismedio.universidadecadastro un ON un.uniid = pb.uniid 
			INNER JOIN sismedio.universidade uu ON uu.uniid = un.uniid 
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
			
			INNER JOIN sismedio.pagamentoperfil pp ON pp.pflcod = '".$dados['pflcod']."'";
	
	$cabecalho = array("Universidade","Aguardando autorização IES","R$","Autorizado IES","R$","Aguardando autorização SGB","R$","Aguardando pagamento","R$","Enviado ao Banco","R$","Pagamento efetivado","R$","Pagamento recusado","R$","Pagamento não autorizado FNDE","R$");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%',$par2);
	
}

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	$db->close();
	exit;
}


include  APPRAIZ."includes/cabecalho.inc";
echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>';
echo '<script language="javascript" type="text/javascript" src="./js/sismedio.js"></script>';

echo "<br>";

$fpbid_padrao = $db->pegaUm("SELECT fpbid FROM sismedio.folhapagamento WHERE fpbanoreferencia='".date("Y")."' AND fpbmesreferencia='".date("m")."'");

?>
<script>
function acessarCadastroOrientadores(esdid,esfera) {
	window.location='sismedio.php?modulo=principal/coordenadorlocal/listacoordenadorlocal&acao=A&esdid='+esdid+'&esfera='+esfera;
}

function acessarComposicaoTurmas(esdid,esfera) {
	window.location='sismedio.php?modulo=principal/coordenadorlocal/listacoordenadorlocal&acao=A&esdidcomposicaoturmas='+esdid+'&esfera='+esfera;
}

function acessarUniversidades(esdid) {
	window.location='sismedio.php?modulo=principal/universidade/listauniversidade&acao=A&esdid='+esdid;
}

function acessarEscolas(esdid) {
	window.location='sismedio.php?modulo=principal/escola/listaescola&acao=A&esdid='+esdid;
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

function detalharCadastramentoEscolasUF(esdid, obj) {
	var tabela = obj.parentNode.parentNode.parentNode;
	var linha = obj.parentNode.parentNode;
	if(obj.title=="mais") {
		obj.title    = "menos";
		obj.src      = "../imagens/menos.gif";
		var nlinha   = tabela.insertRow(linha.rowIndex);
		var ncol     = nlinha.insertCell(0);
		ncol.colSpan = 4;
		ncol.id      = 'dtl_de_coluna_'+nlinha.rowIndex;
		ajaxatualizar('requisicao=carregarDetalhesCadastramentoEscolasUF&esdid='+esdid,ncol.id);
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

function detalharEscolasNaoAtendidas(uf, obj) {
	var tabela = obj.parentNode.parentNode.parentNode;
	var linha = obj.parentNode.parentNode;
	if(obj.title=="mais") {
		divCarregando();
		obj.title    = "menos";
		obj.src      = "../imagens/menos.gif";
		var nlinha   = tabela.insertRow(linha.rowIndex);
		var ncol     = nlinha.insertCell(0);
		ncol.colSpan = 3;
		ncol.id      = 'dtl3_coluna_'+nlinha.rowIndex;
		ajaxatualizar('requisicao=exibirEscolasNaoAtendidas&lemuf='+uf,ncol.id);
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

$menu[] = array("id" => 1, "descricao" => 'Etapa Projeto', "link" => '/sismedio/sismedio.php?modulo=centralacompanhamento&acao=A&aba=projeto');
$menu[] = array("id" => 2, "descricao" => 'Etapa Execução', "link" => '/sismedio/sismedio.php?modulo=centralacompanhamento&acao=A&aba=execucao');
$menu[] = array("id" => 3, "descricao" => 'Etapa Resumo', "link" => '/sismedio/sismedio.php?modulo=centralacompanhamento&acao=A&aba=resumo');
echo "<br>";

if(!$_REQUEST['aba']) $_REQUEST['aba']='projeto';

echo montarAbasArray($menu, '/sismedio/sismedio.php?modulo=centralacompanhamento&acao=A&aba='.$_REQUEST['aba']);

if($_REQUEST['aba']=='projeto') :
?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloCentro" valign="top" width="50%">
	
	<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
	<tr>
		<td class="SubTituloCentro">Coordenador IES</td>
	</tr>
	<tr>
		<td>
		<table>
		<tr>
			<td>
			<p align="center" style="font-size: x-small;">Preenchimento do Projeto</p>
			<?
			
			$sql = "SELECT '<img src=\"../imagens/consultar.gif\" style=\"cursor:pointer;\" onclick=\"acessarUniversidades('||COALESCE(e.esdid,9999999)||')\">' as acao, 
							'<span style=font-size:x-small;>'||COALESCE(e.esddsc,'Não iniciou Elaboração')||'</span>' as sit, 
							COUNT(*) as tot,
							ROUND(( (COUNT(*)*100)::numeric / (SELECT COUNT(*) FROM sismedio.universidadecadastro)::numeric ),2) as porcent 
					FROM sismedio.universidadecadastro u 
					LEFT JOIN workflow.documento d ON d.docid = u.docid 
					LEFT JOIN workflow.estadodocumento e ON e.esdid = d.esdid 
					GROUP BY e.esddsc, e.esdid 
					ORDER BY 3 DESC";
			
			$cabecalho = array("&nbsp;","<span style=font-size:x-small>Situação</span>","<span style=font-size:x-small>Quantidade</span>","<span style=font-size:x-small>%</span>");
			$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%',$par2);
			?>
			</td>
			<td>
			<p align="center" style="font-size: x-small;">Preenchimento das Turmas FR</p>
			<?
			
			$sql = "SELECT '<img src=\"../imagens/consultar.gif\" style=\"cursor:pointer;\" onclick=\"acessarUniversidades('||COALESCE(e.esdid,9999999)||')\">' as acao, 
							'<span style=font-size:x-small;>'||COALESCE(e.esddsc,'Não iniciou Elaboração')||'</span>' as sit, 
							COUNT(*) as tot,
							ROUND(( (COUNT(*)*100)::numeric / (SELECT COUNT(*) FROM sismedio.universidadecadastro)::numeric ),2) as porcent 
					FROM sismedio.universidadecadastro u 
					LEFT JOIN workflow.documento d ON d.docid = u.docidturmaformadoresregionais 
					LEFT JOIN workflow.estadodocumento e ON e.esdid = d.esdid 
					GROUP BY e.esddsc, e.esdid 
					ORDER BY 3 DESC";
			
			$cabecalho = array("&nbsp;","<span style=font-size:x-small>Situação</span>","<span style=font-size:x-small>Quantidade</span>","<span style=font-size:x-small>%</span>");
			$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%',$par2);
			?>
			</td>
			<td>
			<p align="center" style="font-size: x-small;">Preenchimento das Turmas OE</p>
			<?
			
			$sql = "SELECT '<img src=\"../imagens/consultar.gif\" style=\"cursor:pointer;\" onclick=\"acessarUniversidades('||COALESCE(e.esdid,9999999)||')\">' as acao, 
							'<span style=font-size:x-small;>'||COALESCE(e.esddsc,'Não iniciou Elaboração')||'</span>' as sit, 
							COUNT(*) as tot,
							ROUND(( (COUNT(*)*100)::numeric / (SELECT COUNT(*) FROM sismedio.universidadecadastro)::numeric ),2) as porcent 
					FROM sismedio.universidadecadastro u 
					LEFT JOIN workflow.documento d ON d.docid = u.docidturmaorientadoresestudo 
					LEFT JOIN workflow.estadodocumento e ON e.esdid = d.esdid 
					GROUP BY e.esddsc, e.esdid 
					ORDER BY 3 DESC";
			
			$cabecalho = array("&nbsp;","<span style=font-size:x-small>Situação</span>","<span style=font-size:x-small>Quantidade</span>","<span style=font-size:x-small>%</span>");
			$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%',$par2);
			?>
			</td>
		</tr>
		</table>
		</td>
	</tr>

	<tr>
		<td class="SubTituloCentro">Diretor da escola</td>
	</tr>
	<tr>
		<td>
		<table>
			<tr>
				<td valign="top" width="50%">
				<p align="center" style="font-size: x-small;">Cadastramento de Professores</p>
				<?
				
				$sql = "SELECT '<img src=\"../imagens/mais.gif\" title=\"mais\" style=\"cursor:pointer;\" onclick=\"detalharCadastramentoEscolasUF('||COALESCE(e.esdid,9999999)||', this);\"> <img src=\"../imagens/consultar.gif\" style=\"cursor:pointer;\" onclick=\"acessarEscolas('||COALESCE(e.esdid,9999999)||')\">' as acao, 
							   '<span style=font-size:x-small;>'||coalesce(e.esddsc,'Não iniciado')||'<span>' as situacao, 
							   COUNT(*) as n,
							   ROUND(( (COUNT(*)*100)::numeric / (SELECT COUNT(*) FROM sismedio.listaescolasensinomedio)::numeric ),2) as porcent
						FROM sismedio.listaescolasensinomedio l 
						LEFT JOIN workflow.documento d on d.docid = l.docid 
						LEFT JOIN workflow.estadodocumento e on e.esdid = d.esdid 
						GROUP BY e.esdid, e.esddsc";
				
				$cabecalho = array("&nbsp;","Situação","Quantidade","%");
				$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%',$par2);
				?>
				</td>
				<td valign="top">
				<p align="center" style="font-size: x-small;">Escolas que não estão atendidas por universidades</p>
				
				<div style="height: 150px; overflow:auto;">
				<?php
				
				$sql = "select '<img src=\"../imagens/mais.gif\" title=\"mais\" style=\"cursor:pointer;\" onclick=\"detalharEscolasNaoAtendidas(\''||lemuf||'\', this);\">' as mais,
							   '<span style=font-size:x-small;>'||lemuf||'</span>' as uf, 
							   count(*) as num 
						from sismedio.listaescolasensinomedio l 
						left join sismedio.abrangencia a on a.lemcodigoinep = l.lemcodigoinep 
						where a.abrid is null  
						group by lemuf
						order by lemuf";
				
				$cabecalho = array("&nbsp;","<span style=font-size:x-small>UF</span>","<span style=font-size:x-small>Qtd Escolas</span>");
				$db->monta_lista_simples($sql,$cabecalho,10000,5,'N','100%','',true, false, false, true);
				
				?>
				</div>
				
				</td>
			</tr>
		</table>
		</td>
	</tr>
	
	
	
	
	</table>	
	
	
	</td>
	
	<td class="SubTituloCentro" valign="top" width="50%">
		<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
	<tr>
		<td class="SubTituloCentro">Número de Bolsistas / Remuneração(R$)</td>
	</tr>
	
	<tr>
		<td>
		<?
		
		$sql = "SELECT '<span style=font-size:x-small;>'||pf.pfldsc||'</span>' as pfldsc, count(i.iusd) as tot, round(count(i.iusd)*pp.plpvalor,2) as vlr
				FROM sismedio.identificacaousuario i 
				INNER JOIN sismedio.tipoperfil t on t.iusd = i.iusd 
				INNER JOIN sismedio.pagamentoperfil pp on pp.pflcod = t.pflcod 
				INNER JOIN seguranca.perfil pf on pf.pflcod = t.pflcod 
				WHERE CASE WHEN pp.pflcod=".PFL_PROFESSORALFABETIZADOR." THEN i.iustipoprofessor='censo' ELSE true END AND i.uncid IN(
						
				select u.uncid from sismedio.universidadecadastro u 
				inner join workflow.documento d1 on d1.docid = u.docidturmaformadoresregionais
				inner join workflow.documento d2 on d2.docid = u.docidturmaorientadoresestudo
				where d1.esdid=1200 and d2.esdid=1200
	
				)	
				GROUP BY pf.pfldsc, pp.plpvalor ORDER BY 2 DESC";
		
		$cabecalho = array("&nbsp;","<span style=font-size:x-small>Qtd</span>","<span style=font-size:x-small>R$ previsto / Mês</span>");
		$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%',$par2);
		
		?>		
		</td>
	</tr>
	
	<tr>
		<td class="SubTituloCentro">Valor total aprovado pelo MEC para o custeio do Pacto EM</td>
	</tr>
	
	<tr>
		<td>
		<div style="height:250px;overflow:auto;">
		<?
		
		$sql = "SELECT '<span style=font-size:x-small;>'||uu.unisigla||' - '||uu.uninome||'</span>' as ies, sum(orcvlrunitario) as orcvlrunitario FROM sismedio.orcamento o 
				INNER JOIN sismedio.universidadecadastro u on u.uncid = o.uncid 
				INNER JOIN sismedio.universidade uu on uu.uniid = u.uniid 
				INNER JOIN workflow.documento d on d.docid = u.docid 
				WHERE d.esdid=931 AND o.orcstatus='A'
				GROUP BY uu.unisigla, uu.uninome";
		
		$cabecalho = array("<span style=font-size:x-small>IES</span>","<span style=font-size:x-small>R$</span>");
		$db->monta_lista_simples($sql,$cabecalho,100000,5,'S','100%','S',true,false,false,true);
		
		?>
		</div>
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
    $sql = "SELECT uncid as codigo, uninome as descricao FROM sismedio.universidadecadastro un INNER JOIN sismedio.universidade uni ON uni.uniid = un.uniid ORDER BY uni.uninome";
    $db->monta_combo('uncid_pag', $sql, 'S', 'TODAS', 'selecionarUniversidadePagamento', '', '', '', 'N', 'uncid_pag','', $_REQUEST['uncid_pag']);
    $sql = "SELECT fpbid as codigo, mes.mesdsc||'/'||fpbanoreferencia as descricao FROM sismedio.folhapagamento un INNER JOIN public.meses mes ON mes.mescod::integer = un.fpbmesreferencia";
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
	    $sql = "SELECT uncid as codigo, uninome as descricao FROM sismedio.universidadecadastro un INNER JOIN sismedio.universidade uni ON uni.uniid = un.uniid ORDER BY uni.uninome";
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
	    $sql = "SELECT uncid as codigo, uninome as descricao FROM sismedio.universidadecadastro un INNER JOIN sismedio.universidade uni ON uni.uniid = un.uniid ORDER BY uni.uninome";
	    $db->monta_combo('uncid_sit', $sql, 'S', 'TODAS', 'selecionarUniversidadeMensario', '', '', '', 'N', 'uncid_sit','', $_REQUEST['uncid_sit']);
	    $sql = "SELECT fpbid as codigo, mes.mesdsc||'/'||fpbanoreferencia as descricao FROM sismedio.folhapagamento un INNER JOIN public.meses mes ON mes.mescod::integer = un.fpbmesreferencia";
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
	    $sql = "SELECT uncid as codigo, uninome as descricao FROM sismedio.universidadecadastro un INNER JOIN sismedio.universidade uni ON uni.uniid = un.uniid ORDER BY uni.uninome";
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
				(select sum(orcvlrunitario) from sismedio.orcamento where orcstatus='A') as total,
				(select sum(orcvlrexecutado) from sismedio.orcamento where orcstatus='A') as totalexec
				from sismedio.orcamento o 
				inner join sismedio.universidadecadastro u on u.uncid = o.uncid 
				inner join sismedio.universidade uu on uu.uniid = u.uniid 
				inner join sismedio.grupodespesa gd on gd.gdeid = o.gdeid 
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
				(select count(*) from sismedio.identificacaousuario i 
		         inner join sismedio.tipoperfil t on t.iusd = i.iusd where i.iusstatus='A' and t.pflcod = p.pflcod) as totalp,
				(select count(*) from sismedio.identificacaousuario i 
		         inner join sismedio.tipoperfil t on t.iusd = i.iusd where i.iusstatus='I' and t.pflcod = p.pflcod) as totale,
				foo.qtd as totalpgs,
				foo.pbovlrpagamento as valorpgs
				from sismedio.pagamentoperfil p
				inner join seguranca.perfil pp on pp.pflcod = p.pflcod
				inner join (
				select count(*) as qtd, sum(pbovlrpagamento) as pbovlrpagamento, pg.pflcod from sismedio.pagamentobolsista pg inner join workflow.documento d on d.docid = pg.docid where pg.pflcod not in(".PFL_PROFESSORALFABETIZADOR.",".PFL_ORIENTADORESTUDO.") and d.esdid=".ESD_PAGAMENTO_EFETIVADO." group by pg.pflcod
				) foo on foo.pflcod = p.pflcod
				where pp.pflcod not in(".PFL_PROFESSORALFABETIZADOR.",".PFL_ORIENTADORESTUDO.",".PFL_COORDENADORPEDAGOGICO.")
				order by pp.pflnivel";
		
		$participantes = $db->carregar($sql);
		
		if($participantes[0]) {
			foreach($participantes as $part) {
				$_arr_participantes[$part['pflcod']] = $part;
				$_arr_participantes[$part['pflcod']] = $part;
			}
		}
		
		$qtd_oe = $db->pegaUm("SELECT count(*) FROM sismedio.identificacaousuario i 
		         			   INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd WHERE i.iusstatus='A' AND t.pflcod = ".PFL_ORIENTADORESTUDO);
		
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
				case PFL_FORMADORREGIONAL:
					echo '<tr>';
					echo '<td class="SubTituloDireita"><span style=font-size:x-small;>4 - Orientador de Estudo / Formador Regional</span></td>';
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
					echo '<td class="SubTituloDireita"><span style=font-size:x-small;>4 - Formador IES + Formador Regional / Supervisor IES</span></td>';
					echo '<td><span style=font-size:x-small;>'.round(($_arr_participantes[PFL_FORMADORIES]['totalp']+$_arr_participantes[PFL_FORMADORREGIONAL]['totalp'])/$pa['totalp'],1).'</span></td>';
					echo '</tr>';
					break;
				case PFL_FORMADORIES:
					echo '<tr>';
					echo '<td class="SubTituloDireita"><span style=font-size:x-small;>4 -Formador Regional / Formador IES</span></td>';
					echo '<td><span style=font-size:x-small;>'.round($_arr_participantes[PFL_FORMADORREGIONAL]['totalp']/$pa['totalp'],1).'</span></td>';
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
	    resumoGeralCurso(array('pflcod'=>PFL_PROFESSORALFABETIZADOR.",".PFL_COORDENADORPEDAGOGICO)); 
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