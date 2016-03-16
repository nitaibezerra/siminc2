<?


function professoresAlfabetizadoresAusentes($dados) {
	global $db;

	$sql = "select ".(($dados['retornarsql'])?"foo.uncid,":"")." foo.iuscpf, foo.iusnome, foo.uni, foo.rede, round((sum(foo.ausen)*10),0) as aus, count(*) as totaval from (
			select i.uncid,
				   '<img src=\"../imagens/seta_cima.png\" style=cursor:pointer; onclick=\"window.location=\'sispacto2.php?modulo=consultarcpfpacto&acao=A&iuscpf='||i.iuscpf||'\';\"> <span style=font-size:x-small;>'||replace(to_char(i.iuscpf::numeric, '000:000:000-00'), ':', '.')||'</span>' as iuscpf,
				   '<span style=font-size:x-small;>'||i.iusnome||'</span>' as iusnome,
				   '<span style=font-size:x-small;>'||uu.unisigla||'</span>' as uni,
				   '<span style=font-size:x-small;>'||case when p.muncod is not null then mu.estuf||'/'||mu.mundescricao||'(municipal)'
														   when p.estuf is not null then es.estuf||'/'||es.estdescricao||'(estadual)' end||'</span>' as rede,
				   case when mavfrequencia='0.0' then 1
						when mavfrequencia='0.5' then 0.5
						else 0 end as ausen
			from sispacto2.mensario m
			inner join sispacto2.mensarioavaliacoes ma on ma.menid = m.menid and pflcodavaliador=1120
			inner join sispacto2.identificacaousuario i on i.iusd = m.iusd and i.iusstatus='A' 
			inner join seguranca.usuario_sistema us on us.usucpf = i.iuscpf and us.suscod='A' and us.sisid=181
			inner join sispacto2.universidadecadastro u on u.uncid = i.uncid
			inner join sispacto2.universidade uu on uu.uniid = u.uniid
			inner join sispacto2.tipoperfil t on t.iusd = i.iusd and t.pflcod = m.pflcod
			inner join sispacto2.pactoidadecerta p on p.picid = i.picid
			left join territorios.municipio mu on mu.muncod = p.muncod
			left join territorios.estado es on es.estuf = p.estuf
			where m.pflcod=1118 and mavfrequencia in('0.0','0.5') ".(($dados['uncid'])?"and i.uncid='".$dados['uncid']."'":"")."
			) foo
			group by foo.iuscpf, foo.iusnome, foo.uni, foo.rede, foo.uncid
			having sum(foo.ausen)>2.5";



	if($dados['retornarsql']) return $sql;

	$cabecalho = array("<span style=font-size:x-small;>CPF","<span style=font-size:x-small;>Nome</span>","<span style=font-size:x-small;>IES</span>","<span style=font-size:x-small;>Rede</span>","<span style=font-size:x-small;>%</span>","<span style=font-size:x-small;>Tot</span>");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%',$par2);


}


function carregarDetalhesStatusUsuarios($dados) {
	global $db;

	$sql = "select
uu.uninome,
(select count(*) from sispacto2.identificacaousuario u
 inner join sispacto2.tipoperfil t on t.iusd=u.iusd and t.pflcod=".$dados['pflcod']." and u.uncid=un.uncid) as usutotal,

(select count(*) from sispacto2.identificacaousuario u
 inner join sispacto2.tipoperfil t on t.iusd=u.iusd and t.pflcod=".$dados['pflcod']."
 inner join seguranca.perfilusuario pu on pu.usucpf=u.iuscpf and pu.pflcod=t.pflcod
 inner join seguranca.usuario_sistema us on us.usucpf=u.iuscpf and us.sisid=".SIS_SISPACTO."
 where us.suscod='A' and u.uncid=un.uncid) as usuativos,

(select count(*) from sispacto2.identificacaousuario u
 inner join sispacto2.tipoperfil t on t.iusd=u.iusd and t.pflcod=".$dados['pflcod']."
 inner join seguranca.perfilusuario pu on pu.usucpf=u.iuscpf and pu.pflcod=t.pflcod
 inner join seguranca.usuario_sistema us on us.usucpf=u.iuscpf and us.sisid=".SIS_SISPACTO."
 where us.suscod='P' and u.uncid=un.uncid) as usupendentes,

(select count(*) from sispacto2.identificacaousuario u
 inner join sispacto2.tipoperfil t on t.iusd=u.iusd and t.pflcod=".$dados['pflcod']."
 inner join seguranca.perfilusuario pu on pu.usucpf=u.iuscpf and pu.pflcod=t.pflcod
 inner join seguranca.usuario_sistema us on us.usucpf=u.iuscpf and us.sisid=".SIS_SISPACTO."
 where us.suscod='B' and u.uncid=un.uncid) as usubloqueado,

 (select count(*) from sispacto2.identificacaousuario u
 inner join sispacto2.tipoperfil t on t.iusd=u.iusd and t.pflcod=".$dados['pflcod']."
 left join seguranca.perfilusuario pu on pu.usucpf=u.iuscpf and pu.pflcod=t.pflcod
 left join seguranca.usuario_sistema us on us.usucpf=u.iuscpf and us.sisid=".SIS_SISPACTO."
 where (us.suscod is null or pu.pflcod is null) and u.uncid=un.uncid) as usunaocadastrado


from sispacto2.universidadecadastro un
inner join sispacto2.universidade uu on uu.uniid = un.uniid";

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
					ROUND(( (COUNT(*)*100)::numeric / (SELECT COUNT(*) FROM sispacto2.pactoidadecerta)::numeric ),2) as porcent
			FROM sispacto2.pactoidadecerta p
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

	$sql = "select m.estuf, m.mundescricao, e.esddsc from sispacto2.pactoidadecerta p
			inner join territorios.municipio m on m.muncod = p.muncod
			inner join workflow.documento d on d.docid = p.docid
			inner join workflow.estadodocumento e on e.esdid = d.esdid
			where picstatus='A' and m.estuf='".$dados['estuf']."' and p.muncod is not null and p.muncod not in (select muncod from sispacto2.abrangencia where esfera='M')";

	$cabecalho = array("UF","Município","Situação");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%',$par2);

	echo "<p>Municípios sem abrangência - Rede Estadual</p>";

	$sql = "select  distinct m.estuf, m.mundescricao, e.esddsc from sispacto2.identificacaousuario i
inner join sispacto2.pactoidadecerta p on p.picid = i.picid and p.muncod is null
inner join territorios.municipio m on m.muncod = i.muncodatuacao
inner join workflow.documento d on d.docid = p.docid
inner join workflow.estadodocumento e on e.esdid = d.esdid
where p.estuf='".$dados['estuf']."' and m.estuf='".$dados['estuf']."' and i.muncodatuacao not in (select muncod from sispacto2.abrangencia where esfera='E')";

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
	COALESCE((SELECT AVG(mavtotal) FROM sispacto2.mensarioavaliacoes ma  WHERE ma.menid=m.menid),0.00) as mensarionota,
	uu.uninome,
	i.iusdocumento,
	i.iustermocompromisso,
	m.fpbid,
	d.esdid,
	t.pflcod,
	i.iustipoprofessor,
	(SELECT COUNT(mavid) FROM sispacto2.mensarioavaliacoes ma  WHERE ma.menid=m.menid) as numeroavaliacoes
	FROM sispacto2.mensario m
	INNER JOIN sispacto2.identificacaousuario i ON i.iusd = m.iusd
	INNER JOIN sispacto2.universidadecadastro un ON un.uncid = i.uncid
	INNER JOIN sispacto2.universidade uu ON uu.uniid = un.uniid
	INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd
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
			INNER JOIN sispacto2.pagamentobolsista pb ON pb.pflcod = p.pflcod
			INNER JOIN sispacto2.universidadecadastro un ON un.uniid = pb.uniid
			INNER JOIN sispacto2.universidade uu ON uu.uniid = un.uniid
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
		
			INNER JOIN sispacto2.pagamentoperfil pp ON pp.pflcod = '".$dados['pflcod']."'";

	$cabecalho = array("Universidade","Aguardando autorização IES","R$","Autorizado IES","R$","Aguardando autorização SGB","R$","Aguardando pagamento","R$","Enviado ao Banco","R$","Pagamento efetivado","R$","Pagamento recusado","R$","Pagamento não autorizado FNDE","R$");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%',$par2);

}

function composicaoTurmasMunicipal($dados) {
	global $db;

	echo '<p align="center" style="font-size: x-small;">Composição de Turmas - Municipal</p>';

	$sql = "SELECT '<img src=\"../imagens/consultar.gif\" style=\"cursor:pointer;\" onclick=\"acessarComposicaoTurmas('||COALESCE(e.esdid,9999999)||',\'Municipal\');\">' as acao,
							'<span style=font-size:x-small;>'||COALESCE(e.esddsc,'Não iniciou Elaboração')||'</span>' as esddsc,
							COUNT(*) as tot,
							ROUND(( (COUNT(*)*100)::numeric / (SELECT COUNT(*) FROM sispacto2.pactoidadecerta WHERE muncod IS NOT NULL AND picstatus='A')::numeric ),2) as porcent
					FROM sispacto2.pactoidadecerta p
					LEFT JOIN workflow.documento d ON d.docid = p.docidturma
					LEFT JOIN workflow.estadodocumento e ON e.esdid = d.esdid
					WHERE p.muncod IS NOT NULL AND p.picstatus='A'
					GROUP BY e.esddsc, e.esdid
					ORDER BY 3 DESC";

	$cabecalho = array("&nbsp;","<span style=font-size:x-small;>Situação</span>","<span style=font-size:x-small;>Quantidade</span>","<span style=font-size:x-small;>%</span>");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%',$par2);


}

function composicaoTurmasEstadual($dados) {
	global $db;

	echo '<p align="center" style="font-size: x-small;">Composição de Turmas - Estadual</p>';

	$sql = "SELECT '<img src=\"../imagens/consultar.gif\" style=\"cursor:pointer;\" onclick=\"acessarComposicaoTurmas('||COALESCE(e.esdid,9999999)||',\'Estadual\')\">' as acao,
							'<span style=font-size:x-small;>'||COALESCE(e.esddsc,'Não iniciou Elaboração')||'</span>' as esddsc,
							COUNT(*) as tot,
							ROUND(( (COUNT(*)*100)::numeric / (SELECT COUNT(*) FROM sispacto2.pactoidadecerta WHERE estuf IS NOT NULL)::numeric ),2) as porcent
					FROM sispacto2.pactoidadecerta p
					LEFT JOIN workflow.documento d ON d.docid = p.docidturma
					LEFT JOIN workflow.estadodocumento e ON e.esdid = d.esdid
					WHERE p.estuf IS NOT NULL AND p.picstatus='A'
					GROUP BY e.esddsc, e.esdid
					ORDER BY 3 DESC";

	$cabecalho = array("&nbsp;","<span style=font-size:x-small;>Situação</span>","<span style=font-size:x-small;>Quantidade</span>","<span style=font-size:x-small;>%</span>");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%',$par2);

}

function cadastramentoOEMunicipal($dados) {
	global $db;

	echo '<p align="center" style="font-size: x-small;">Cadastramento dos Orientadores de Estudo - Municipal</p>';

	$sql = "SELECT '<img src=\"../imagens/mais.gif\" title=\"mais\" style=\"cursor:pointer;\" onclick=\"detalharCadastroOrientadores('||COALESCE(e.esdid,9999999)||',\'municipal\',this);\"> <img src=\"../imagens/consultar.gif\" style=\"cursor:pointer;\" onclick=\"acessarCadastroOrientadores('||COALESCE(e.esdid,9999999)||',\'Municipal\')\">' as acao,
							'<span style=font-size:x-small;>'||COALESCE(e.esddsc,'Não iniciou Elaboração')||'</span>' as esddsc,
							COUNT(*) as tot,
							ROUND(( (COUNT(*)*100)::numeric / (SELECT COUNT(*) FROM sispacto2.pactoidadecerta WHERE muncod IS NOT NULL AND picstatus='A')::numeric ),2) as porcent

					FROM sispacto2.pactoidadecerta p
					LEFT JOIN workflow.documento d ON d.docid = p.docid
					LEFT JOIN workflow.estadodocumento e ON e.esdid = d.esdid
					WHERE p.muncod IS NOT NULL AND p.picstatus='A'
					GROUP BY e.esddsc, e.esdid
					ORDER BY 3 DESC";

	$cabecalho = array("&nbsp;","<span style=font-size:x-small;>Situação</span>","<span style=font-size:x-small;>Quantidade</span>","<span style=font-size:x-small;>%</span>");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%',$par2);

}

function cadastramentoOEEstadual($dados) {
	global $db;

	echo '<p align="center" style="font-size: x-small;">Cadastramento dos Orientadores de Estudo - Estadual</p>';

	$sql = "SELECT '<img src=\"../imagens/mais.gif\" title=\"mais\" style=\"cursor:pointer;\" onclick=\"detalharCadastroOrientadores('||COALESCE(e.esdid,9999999)||',\'estadual\',this);\"> <img src=\"../imagens/consultar.gif\" style=\"cursor:pointer;\" onclick=\"acessarCadastroOrientadores('||COALESCE(e.esdid,9999999)||',\'Estadual\')\">' as acao,
							'<span style=font-size:x-small;>'||COALESCE(e.esddsc,'Não iniciou Elaboração')||'</span>' as esddsc,
							COUNT(*) as tot,
							ROUND(( (COUNT(*)*100)::numeric / (SELECT COUNT(*) FROM sispacto2.pactoidadecerta WHERE estuf IS NOT NULL)::numeric ),2) as porcent
					FROM sispacto2.pactoidadecerta p
					LEFT JOIN workflow.documento d ON d.docid = p.docid
					LEFT JOIN workflow.estadodocumento e ON e.esdid = d.esdid
					WHERE p.estuf IS NOT NULL AND p.picstatus='A'
					GROUP BY e.esddsc, e.esdid
					ORDER BY 3 DESC";

	$cabecalho = array("&nbsp;","<span style=font-size:x-small;>Situação</span>","<span style=font-size:x-small;>Quantidade</span>","<span style=font-size:x-small;>%</span>");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%',$par2);

}

function tipoOECadastrados($dados) {
	global $db;
	echo '<p align="center" style="font-size: x-small;">Tipos de Orientadores de Estudo Cadastrados</p>';

	$sql = "SELECT
							'<span style=font-size:x-small;>'||CASE WHEN iustipoorientador='professorsispacto2013' THEN 'Professor Alfabetizador do Pacto 2013 recomendado para certificação'
							WHEN iustipoorientador='orientadorsispacto2013' THEN 'Orientador de Estudo do Pacto 2013 recomendado para certificação'
							WHEN iustipoorientador='tutoresproletramento' THEN 'Tutores Pró-Letramento'
						    WHEN iustipoorientador='tutoresredesemproletramento' THEN 'Professores da rede que não foram Tutores do Pró-Letramento'
						    WHEN iustipoorientador='profissionaismagisterio' THEN 'Profissionais do Magistério com experiência em formação de professores' END||'</font>' as tipo,
							count(*) as numero,
							(count(*)::numeric/(SELECT count(*) FROM sispacto2.identificacaousuario i INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd WHERE pflcod=".PFL_ORIENTADORESTUDO.")::numeric)*100 as porcent,
							((SELECT count(*) FROM sispacto2.identificacaousuario ii INNER JOIN sispacto2.tipoperfil t ON t.iusd = ii.iusd WHERE pflcod=".PFL_ORIENTADORESTUDO." and ii.iusformacaoinicialorientador=true and ii.iustipoorientador=i.iustipoorientador)::numeric) as numero2,
							((SELECT count(*) FROM sispacto2.identificacaousuario ii INNER JOIN sispacto2.tipoperfil t ON t.iusd = ii.iusd WHERE pflcod=".PFL_ORIENTADORESTUDO." and ii.iusformacaoinicialorientador=true and ii.iustipoorientador=i.iustipoorientador)::numeric/(SELECT count(*) FROM sispacto2.identificacaousuario i INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd WHERE pflcod=".PFL_ORIENTADORESTUDO.")::numeric)*100 as porcent2

					FROM sispacto2.identificacaousuario i
					INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd
					WHERE pflcod=".PFL_ORIENTADORESTUDO."
					GROUP BY iustipoorientador";

	$cabecalho = array("<span style=font-size:x-small>Tipo</font>","<span style=font-size:x-small>Qtd Total</font>","<span style=font-size:x-small>%</font>","<span style=font-size:x-small>Qtd Formação Inicial</font>","<span style=font-size:x-small>%</font>");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%',$par2);

}

function tipoPACadastrados($dados) {
	global $db;

	echo '<p align="center" style="font-size: x-small;">Tipos de Professores Alfabetizadores Cadastrados</p>';

	$sql = "SELECT foo.tipo, foo.numero, CASE WHEN foo.tot > 0 THEN ((foo.numero/foo.tot)*100) ELSE 0 END as por FROM (
					SELECT
							'<span style=font-size:x-small;>'||CASE WHEN iustipoprofessor='censo' THEN 'Cadastrado no CENSO 2013 (Bolsista)'
							WHEN iustipoprofessor='cpflivre' THEN 'Não cadastrado no CENSO 2013 (Não bolsista)'
							ELSE 'Não identificado' END||'</font>' as tipo,
							count(*) as numero,
							(SELECT count(*) FROM sispacto2.identificacaousuario i INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd WHERE pflcod=".PFL_PROFESSORALFABETIZADOR.")::numeric as tot

					FROM sispacto2.identificacaousuario i
					INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd
					WHERE t.pflcod=".PFL_PROFESSORALFABETIZADOR." AND i.iusstatus='A'
					GROUP BY iustipoprofessor
	 				) foo";

	$cabecalho = array("<span style=font-size:x-small>Tipo</font>","<span style=font-size:x-small>Qtd</font>","<span style=font-size:x-small>%</font>");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%',$par2);
}

function preenchimentoProjetoIES($dados) {
	global $db;
	
	echo '<p align="center" style="font-size:x-small;">Preenchimento do Projeto</p>';

	$sql = "SELECT '<img src=\"../imagens/consultar.gif\" style=\"cursor:pointer;\" onclick=\"acessarUniversidades('||COALESCE(e.esdid,9999999)||')\">' as acao,
							'<span style=font-size:x-small>'||COALESCE(e.esddsc,'Não iniciou Elaboração')||'</span>' as esddsc,
							COUNT(*) as tot,
							ROUND(( (COUNT(*)*100)::numeric / (SELECT COUNT(*) FROM sispacto2.universidadecadastro)::numeric ),2) as porcent
					FROM sispacto2.universidadecadastro u
					LEFT JOIN workflow.documento d ON d.docid = u.docid
					LEFT JOIN workflow.estadodocumento e ON e.esdid = d.esdid
					GROUP BY e.esddsc, e.esdid
					ORDER BY 3 DESC";
	$cabecalho = array("&nbsp;","Situação","Quantidade","%");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%',$par2);
		
}

function preenchimentoFormacaoInicialIES($dados) {
	global $db;
	
	echo '<p align="center" style="font-size:x-small;">Preenchimento da Formação Inicial</p>';

	$sql = "SELECT '<img src=\"../imagens/consultar.gif\" style=\"cursor:pointer;\" onclick=\"acessarUniversidadesFormacaoInicial('||COALESCE(e.esdid,9999999)||')\">' as acao, '<span style=font-size:x-small>'||e.esddsc||'</font>' as esddsc, count(*) as tot, ROUND(( (COUNT(*)*100)::numeric / (SELECT COUNT(*) FROM sispacto2.universidadecadastro)::numeric ),2) as porcent
					FROM sispacto2.universidadecadastro u
					INNER JOIN workflow.documento d ON d.docid = u.docidformacaoinicial
					INNER JOIN workflow.estadodocumento e ON e.esdid = d.esdid
					GROUP BY e.esdid, e.esddsc";
	
	$cabecalho = array("&nbsp;","<span style=font-size:xx-small;>Situação</font>","<span style=font-size:xx-small;>Quantidade</font>","<span style=font-size:xx-small;>%</font>");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%',$par2);

}

function abrangenciaIES($dados) {
	global $db;
	
	echo '<p align="center" style="font-size:x-small;">Abrangência das Universidades</p>';
	echo '<div style="height:100px;overflow:auto;">';
	
	$sql = "select
					'<span style=font-size:x-small>'||su.unisigla||'</font>',
					(select count(*) from sispacto2.abrangencia a inner join sispacto2.pactoidadecerta p on p.muncod = a.muncod inner join sispacto2.estruturacurso e on e.ecuid = a.ecuid where abrstatus='A' and uncid=u.uncid and a.esfera='M') as qtdmun,
					round((select count(*) from sispacto2.abrangencia a inner join sispacto2.pactoidadecerta p on p.muncod = a.muncod inner join sispacto2.estruturacurso e on e.ecuid = a.ecuid where abrstatus='A' and uncid=u.uncid and a.esfera='M')*100::numeric/(select count(*) from sispacto2.pactoidadecerta where picstatus='A' and muncod is not null)::numeric,2) as mun,
					(select count(distinct a.muncod) from sispacto2.abrangencia a
	inner join sispacto2.identificacaousuario i on a.muncod = i.muncodatuacao
	inner join sispacto2.pactoidadecerta p on p.picid = i.picid and p.muncod is null
	inner join sispacto2.estruturacurso e on e.ecuid = a.ecuid where e.uncid=u.uncid and a.esfera='E') as qtdest,
					round((select count(distinct a.muncod) from sispacto2.abrangencia a
	inner join sispacto2.identificacaousuario i on a.muncod = i.muncodatuacao
	inner join sispacto2.pactoidadecerta p on p.picid = i.picid and p.muncod is null
	inner join sispacto2.estruturacurso e on e.ecuid = a.ecuid where e.uncid=u.uncid and a.esfera='E')*100::numeric/(select count(distinct i.muncodatuacao) from sispacto2.identificacaousuario i
	inner join sispacto2.pactoidadecerta p on p.picid = i.picid and p.muncod is null
	)::numeric,2) as est
					from sispacto2.universidadecadastro u
					inner join sispacto2.universidade su ON su.uniid = u.uniid
					order by 3 desc
			";
	
	$cabecalho = array("<span style=font-size:xx-small;>IES</span>","<span style=font-size:xx-small;>Qtd Municípios</span>","<span style=font-size:xx-small;>%Municipal</span>","<span style=font-size:xx-small;>Qtd Municípios</span>","<span style=font-size:xx-small;>%Estadual</span>");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%',$par2);

	echo '</div>';
		
}

function abrangenciaEstado($dados) {
	global $db;
	
	echo '<p align="center" style="font-size:x-small;">Abrangência por Estado</p>';
	echo '<div style="height:100px;overflow:auto;">';

	$sql = "select
	'<img src=\"../imagens/mais.gif\" title=\"mais\" style=\"cursor:pointer;\" onclick=\"detalharAbrangenciaEstado(\''||foo.estuf||'\',this);\">' as acao,
	
	'<span style=font-size:x-small>'||foo.descricao||'</font>',
	foo.qtdmun,
	CASE WHEN foo.totmun > 0 THEN round((foo.qtdmun*100)/foo.totmun,2) ELSE 100.00 END as porcmun,
	foo.qtdest,
	CASE WHEN foo.totest > 0 THEN round((foo.qtdest*100)/foo.totest,2) ELSE 100.00 END as porcest
	
	from (
	select
					estuf,
					estuf||' / '||estdescricao as descricao,
					(select count(*) from sispacto2.abrangencia a inner join sispacto2.pactoidadecerta p on p.muncod = a.muncod inner join territorios.municipio m on m.muncod = p.muncod where abrstatus='A' and m.estuf=e.estuf and a.esfera='M') as qtdmun,
					(select count(*) from sispacto2.pactoidadecerta p inner join territorios.municipio m on m.muncod = p.muncod where picstatus='A' and m.estuf=e.estuf and p.muncod is not null)::numeric as totmun,
					(select count(distinct a.muncod) from sispacto2.abrangencia a
	inner join sispacto2.identificacaousuario i on a.muncod = i.muncodatuacao
	inner join sispacto2.pactoidadecerta p on p.picid = i.picid and p.muncod is null
	inner join territorios.municipio m on m.muncod = a.muncod
	where p.estuf=e.estuf and m.estuf=e.estuf and a.esfera='E') as qtdest,
					(select count(distinct i.muncodatuacao) from sispacto2.identificacaousuario i
	inner join sispacto2.pactoidadecerta p on p.picid = i.picid and p.muncod is null
	inner join territorios.municipio m on m.muncod = i.muncodatuacao
	where p.estuf=e.estuf and m.estuf=e.estuf
	)::numeric as totest
					from territorios.estado e
	) foo
	ORDER BY 1
			";
	$cabecalho = array("&nbsp;","<span style=font-size:xx-small;>UF</font>","<span style=font-size:xx-small;>Qtd Municípios</font>","<span style=font-size:xx-small;>%Municipal</font>","<span style=font-size:xx-small;>Qtd Municípios</font>","<span style=font-size:xx-small;>%Estadual</font>");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%',$par2);
	
	echo '</div>';
}

function orcamentoIES($dados) {
	global $db;
	
	echo '<p align="center" style="font-size: x-small;">Orçamento para viabilizar a Formação dos OEs</p>';
	
	echo '<div style="height:170px;overflow:auto;">';
	
	$sql = "SELECT foo.descricao, foo.total, CASE WHEN foo.totalpolo > 0 THEN round(foo.total/foo.totalpolo,2) ELSE 0.00 END as totalpormun, CASE WHEN foo.totalorientador > 0 THEN round(foo.total/foo.totalorientador,2) ELSE 0.00 END as totalporori FROM  (
								SELECT '<span style=font-size:x-small;>'||uu.unisigla||'<span>' as descricao,
							   (SELECT COALESCE(SUM(orcvlrunitario),0.00) FROM sispacto2.orcamento WHERE orcstatus='A' AND uncid=u.uncid) as total,
							   (SELECT COUNT(distinct t.muncod) FROM sispacto2.turmas t WHERE t.turstatus='A' AND t.uncid=u.uncid) as totalpolo,
							   (SELECT COUNT(*) FROM sispacto2.identificacaousuario i INNER JOIN sispacto2.tipoperfil t ON t.iusd=i.iusd AND t.pflcod=".PFL_ORIENTADORESTUDO." WHERE i.uncid=u.uncid) as totalorientador
						FROM sispacto2.universidadecadastro u
						INNER JOIN sispacto2.universidade uu ON uu.uniid = u.uniid
						ORDER BY 2 DESC
						) foo";
	
	$cabecalho = array("&nbsp;","<span style=font-size:x-small>R$ total</span>","<span style=font-size:x-small>R$/Polo</span>","<span style=font-size:x-small>R$/Orientador de Estudo</span>");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%',$par2);
	
	echo '</div>';
		
}

function remuneracaoBolsistas($dados) {
	global $db;
	
	echo '<p align="center" style="font-size: x-small;">Número de Bolsistas / Remuneração(R$)</p>';
	
	$sql = "SELECT '<span style=font-size:x-small;>'||pf.pfldsc||'</font>' as pfldsc, count(i.iusd) as tot, round(count(i.iusd)*pp.plpvalor,2) as vlr
						FROM sispacto2.identificacaousuario i
						INNER JOIN sispacto2.tipoperfil t on t.iusd = i.iusd
						INNER JOIN sispacto2.pagamentoperfil pp on pp.pflcod = t.pflcod
						INNER JOIN seguranca.perfil pf on pf.pflcod = t.pflcod
						WHERE i.iusstatus='A' AND CASE WHEN pp.pflcod=".PFL_PROFESSORALFABETIZADOR." THEN i.iustipoprofessor='censo' ELSE true END AND CASE WHEN pp.pflcod=".PFL_ORIENTADORESTUDO." THEN i.iusformacaoinicialorientador=true ELSE true END
						GROUP BY pf.pfldsc, pp.plpvalor ORDER BY 2 DESC";
	
	$cabecalho = array("&nbsp;","<span style=font-size:x-small>Qtd</span>","<span style=font-size:x-small>R$ previsto / Mês</span>");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%',$par2);
	

}

function ausenciaOEIES($dados) {
	global $db;

	echo '<p align="center">Orientadores de Estudo (Ausência na IES > 25% e Ativos)</p>';
	echo '<div style="height:200px;overflow:auto;">';

	$sql = "select '<img src=\"../imagens/seta_cima.png\" style=cursor:pointer; onclick=\"window.location=\'sispacto2.php?modulo=consultarcpfpacto&acao=A&iuscpf='||foo.iuscpf||'\';\"> <span style=font-size:x-small;>'||replace(to_char(foo.iuscpf::numeric, '000:000:000-00'), ':', '.')||'</span>' as iuscpf, foo.iusnome, foo.uni, foo.rede, round((sum(foo.ausen)*10),0) as au, count(*) as totavl from (
			select i.iuscpf,
				   '<span style=font-size:x-small;>'||i.iusnome||'</span>' as iusnome,
				   '<span style=font-size:x-small;>'||uu.unisigla||'</span>' as uni,
				   '<span style=font-size:x-small;>'||case when p.muncod is not null then mu.estuf||'/'||mu.mundescricao||'(municipal)'
														   when p.estuf is not null then es.estuf||'/'||es.estdescricao||'(estadual)' end||'</span>' as rede,
				   case when mavfrequencia='0.0' then 1.0
						when mavfrequencia='0.5' then 0.5
						else 0 end as ausen
			from sispacto2.mensario m
			inner join sispacto2.mensarioavaliacoes ma on ma.menid = m.menid and pflcodavaliador=1131
			inner join sispacto2.identificacaousuario i on i.iusd = m.iusd and i.iusstatus='A' 
			inner join seguranca.usuario_sistema us on us.usucpf = i.iuscpf and us.suscod='A' and us.sisid=181
			inner join sispacto2.universidadecadastro u on u.uncid = i.uncid
			inner join sispacto2.universidade uu on uu.uniid = u.uniid
			inner join sispacto2.tipoperfil t on t.iusd = i.iusd and t.pflcod = m.pflcod
			inner join sispacto2.pactoidadecerta p on p.picid = i.picid
			left join territorios.municipio mu on mu.muncod = p.muncod
			left join territorios.estado es on es.estuf = p.estuf
			where m.pflcod=1120 and mavfrequencia in('0.0','0.5') ".(($dados['uncid'])?"and i.uncid='".$dados['uncid']."'":"")."
			) foo
			group by foo.iuscpf, foo.iusnome, foo.uni, foo.rede
			having sum(foo.ausen)>2.5
			order by foo.iusnome";
	
	$cabecalho = array("<span style=font-size:x-small>CPF","<span style=font-size:x-small>Nome</span>","<span style=font-size:x-small>IES</span>","<span style=font-size:x-small>Rede</span>","<span style=font-size:x-small>%</span>","<span style=font-size:x-small>Tot</span>");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%','', true, false, false, true);
	
	echo '</div>';
	
}

function ausenciaPAMunicipio($dados) {
	global $db;
	echo '<p align="center">Professores Alfabetizadores (Ausência > 25% e Ativos)</p>';
	echo '<div '.(($dados['uncid'])?'':'style="height:200px;overflow:auto;"').'>';
	
	$sql = "select '<img src=../imagens/mais.gif title=mais onclick=\"detalharProfessoresAusentes('||foo2.uncid||', this);\"> '||foo2.uni as uni,
				   count(distinct foo2.iuscpf) as qtd,
				   (count(distinct foo2.iuscpf)::numeric /
				   (select count(*) from sispacto2.identificacaousuario i
					inner join sispacto2.tipoperfil t on t.iusd = i.iusd
					where t.pflcod=".PFL_PROFESSORALFABETIZADOR." and i.iusstatus='A' and i.uncid=foo2.uncid)::numeric)*100 as por
			from (
		
			".professoresAlfabetizadoresAusentes(array('retornarsql'=>true,'uncid'=>$dados['uncid']))."
	
			) foo2
			group by foo2.uni, foo2.uncid";
	
	$cabecalho = array("<span style=font-size:x-small>IES</span>","<span style=font-size:x-small>Qtd Evadidos</span>","<span style=font-size:x-small>%</span>");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%','', true, false, false, true);
	
	echo '</div>';
	
}

function correlacaoParentescoCLePA($dados) {
	global $db;

	$sql = "select '<span style=font-size:x-small;>'||foo2.orientador||'</font>' as oe, '<span style=font-size:x-small;>'||foo2.professor||'</font>' as pa, (foo2.r1+foo2.r2+foo2.r3+foo2.r4+foo2.r5+foo2.r6) as num from (

			select

			foo.orientador,
			foo.professor,
			case when foo.p1!='' and foo.professor ilike '% '||foo.p1||' %' then 1 else 0 end as r1,
			case when foo.p2!='' and foo.professor ilike '% '||foo.p2||' %' then 1 else 0 end as r2,
			case when foo.p3!='' and foo.professor ilike '% '||foo.p3||' %' then 1 else 0 end as r3,
			case when foo.p4!='' and foo.professor ilike '% '||foo.p4||' %' then 1 else 0 end as r4,
			case when foo.p5!='' and foo.professor ilike '% '||foo.p5||' %' then 1 else 0 end as r5,
			case when foo.p6!='' and foo.professor ilike '% '||foo.p6||' %' then 1 else 0 end as r6

			from (

			select
			case when length(split_part(i.iusnome, ' ', 2))>3 then split_part(i.iusnome, ' ', 2) else '' end as p1,
			case when length(split_part(i.iusnome, ' ', 3))>3 then split_part(i.iusnome, ' ', 3) else '' end as p2,
			case when length(split_part(i.iusnome, ' ', 4))>3 then split_part(i.iusnome, ' ', 4) else '' end as p3,
			case when length(split_part(i.iusnome, ' ', 5))>3 then split_part(i.iusnome, ' ', 5) else '' end as p4,
			case when length(split_part(i.iusnome, ' ', 6))>3 then split_part(i.iusnome, ' ', 6) else '' end as p5,
			case when length(split_part(i.iusnome, ' ', 7))>3 then split_part(i.iusnome, ' ', 7) else '' end as p6,
			replace(to_char(i.iuscpf::numeric, '000:000:000-00'), ':', '.')||' - '||i.iusnome as orientador,
			replace(to_char(i2.iuscpf::numeric, '000:000:000-00'), ':', '.')||' - '||i2.iusnome as professor
			from sispacto2.identificacaousuario i
			inner join sispacto2.tipoperfil t on t.iusd = i.iusd and t.pflcod=1119
			inner join sispacto2.identificacaousuario i2 on i2.picid = i.picid
			inner join sispacto2.tipoperfil t2 on t2.iusd = i2.iusd and t2.pflcod=1118
			where i.iusstatus='A' and i2.iusstatus='A' and i2.iuscpf not ilike 'SIS%'

			) foo
			where
			(foo.p1!='MARIA' and foo.p2!='MARIA' and foo.p3!='MARIA' and foo.p4!='MARIA' and foo.p5!='MARIA' and foo.p6!='MARIA') and
			(foo.p1!='SILVA' and foo.p2!='SILVA' and foo.p3!='SILVA' and foo.p4!='SILVA' and foo.p5!='SILVA' and foo.p6!='SILVA')

			) foo2
			where
			(foo2.r1+foo2.r2+foo2.r3+foo2.r4+foo2.r5+foo2.r6)>1
			order by 3 desc";

	echo '<p align="center">Possível nível de parentesco => Hierarquia</p>';
	echo '<div style="height:200px;overflow:auto;">';

	$cabecalho = array("<span style=font-size:x-small>Coordenador Local</span>","<span style=font-size:x-small>Professor Alfabetizador</span>","<span style=font-size:x-small>N</span>");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%','',true, false, false, true);

	echo '</div>';

}

function correlacaoParentescoCLeOE($dados) {
	global $db;
	
	$sql = "select '<span style=font-size:x-small;>'||foo2.orientador||'</font>' as oe, '<span style=font-size:x-small;>'||foo2.professor||'</font>' as pa, (foo2.r1+foo2.r2+foo2.r3+foo2.r4+foo2.r5+foo2.r6) as num from (
	
			select
		
			foo.orientador,
			foo.professor,
			case when foo.p1!='' and foo.professor ilike '% '||foo.p1||' %' then 1 else 0 end as r1,
			case when foo.p2!='' and foo.professor ilike '% '||foo.p2||' %' then 1 else 0 end as r2,
			case when foo.p3!='' and foo.professor ilike '% '||foo.p3||' %' then 1 else 0 end as r3,
			case when foo.p4!='' and foo.professor ilike '% '||foo.p4||' %' then 1 else 0 end as r4,
			case when foo.p5!='' and foo.professor ilike '% '||foo.p5||' %' then 1 else 0 end as r5,
			case when foo.p6!='' and foo.professor ilike '% '||foo.p6||' %' then 1 else 0 end as r6
		
			from (
		
			select 
			case when length(split_part(i.iusnome, ' ', 2))>3 then split_part(i.iusnome, ' ', 2) else '' end as p1, 
			case when length(split_part(i.iusnome, ' ', 3))>3 then split_part(i.iusnome, ' ', 3) else '' end as p2, 
			case when length(split_part(i.iusnome, ' ', 4))>3 then split_part(i.iusnome, ' ', 4) else '' end as p3, 
			case when length(split_part(i.iusnome, ' ', 5))>3 then split_part(i.iusnome, ' ', 5) else '' end as p4, 
			case when length(split_part(i.iusnome, ' ', 6))>3 then split_part(i.iusnome, ' ', 6) else '' end as p5, 
			case when length(split_part(i.iusnome, ' ', 7))>3 then split_part(i.iusnome, ' ', 7) else '' end as p6, 
			replace(to_char(i.iuscpf::numeric, '000:000:000-00'), ':', '.')||' - '||i.iusnome as orientador, 
			replace(to_char(i2.iuscpf::numeric, '000:000:000-00'), ':', '.')||' - '||i2.iusnome as professor
			from sispacto2.identificacaousuario i 
			inner join sispacto2.tipoperfil t on t.iusd = i.iusd and t.pflcod=1119
			inner join sispacto2.identificacaousuario i2 on i2.picid = i.picid 
			inner join sispacto2.tipoperfil t2 on t2.iusd = i2.iusd and t2.pflcod=1120
			where i.iusstatus='A' and i2.iusstatus='A' and i2.iuscpf not ilike 'SIS%'
		
			) foo
			where
			(foo.p1!='MARIA' and foo.p2!='MARIA' and foo.p3!='MARIA' and foo.p4!='MARIA' and foo.p5!='MARIA' and foo.p6!='MARIA') and
			(foo.p1!='SILVA' and foo.p2!='SILVA' and foo.p3!='SILVA' and foo.p4!='SILVA' and foo.p5!='SILVA' and foo.p6!='SILVA')
		
			) foo2
			where
			(foo2.r1+foo2.r2+foo2.r3+foo2.r4+foo2.r5+foo2.r6)>1
			order by 3 desc";
	
	echo '<p align="center">Possível nível de parentesco => Hierarquia</p>';
	echo '<div style="height:200px;overflow:auto;">';
	
	$cabecalho = array("<span style=font-size:x-small>Coordenador Local</span>","<span style=font-size:x-small>Orientador de Estudo</span>","<span style=font-size:x-small>N</span>");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%','',true, false, false, true);
	
	echo '</div>';
	
}

function correlacaoParentescoOEePA($dados) {
	global $db;
	
	$sql = "select '<span style=font-size:x-small;>'||foo2.orientador||'</font>' as oe, '<span style=font-size:x-small;>'||foo2.professor||'</font>' as pa, (foo2.r1+foo2.r2+foo2.r3+foo2.r4+foo2.r5+foo2.r6) as num from (

			select 
			
			foo.orientador,
			foo.professor,
			case when foo.p1!='' and foo.professor ilike '% '||foo.p1||' %' then 1 else 0 end as r1,
			case when foo.p2!='' and foo.professor ilike '% '||foo.p2||' %' then 1 else 0 end as r2,
			case when foo.p3!='' and foo.professor ilike '% '||foo.p3||' %' then 1 else 0 end as r3,
			case when foo.p4!='' and foo.professor ilike '% '||foo.p4||' %' then 1 else 0 end as r4,
			case when foo.p5!='' and foo.professor ilike '% '||foo.p5||' %' then 1 else 0 end as r5,
			case when foo.p6!='' and foo.professor ilike '% '||foo.p6||' %' then 1 else 0 end as r6
			
			from (
			
			select 
			case when length(split_part(i.iusnome, ' ', 2))>3 then split_part(i.iusnome, ' ', 2) else '' end as p1, 
			case when length(split_part(i.iusnome, ' ', 3))>3 then split_part(i.iusnome, ' ', 3) else '' end as p2, 
			case when length(split_part(i.iusnome, ' ', 4))>3 then split_part(i.iusnome, ' ', 4) else '' end as p3, 
			case when length(split_part(i.iusnome, ' ', 5))>3 then split_part(i.iusnome, ' ', 5) else '' end as p4, 
			case when length(split_part(i.iusnome, ' ', 6))>3 then split_part(i.iusnome, ' ', 6) else '' end as p5, 
			case when length(split_part(i.iusnome, ' ', 7))>3 then split_part(i.iusnome, ' ', 7) else '' end as p6, 
			replace(to_char(i.iuscpf::numeric, '000:000:000-00'), ':', '.')||' - '||i.iusnome as orientador, 
			replace(to_char(i2.iuscpf::numeric, '000:000:000-00'), ':', '.')||' - '||i2.iusnome as professor
			from sispacto2.identificacaousuario i 
			inner join sispacto2.tipoperfil t on t.iusd = i.iusd and t.pflcod=1120
			inner join sispacto2.turmas tu on tu.iusd = i.iusd 
			inner join sispacto2.orientadorturma ot on ot.turid = tu.turid 
			inner join sispacto2.identificacaousuario i2 on i2.iusd = ot.iusd 
			inner join sispacto2.tipoperfil t2 on t2.iusd = i2.iusd and t2.pflcod=1118
			where i.iusstatus='A' and i2.iusstatus='A'
			
			) foo 
			where 
			(foo.p1!='MARIA' and foo.p2!='MARIA' and foo.p3!='MARIA' and foo.p4!='MARIA' and foo.p5!='MARIA' and foo.p6!='MARIA') and 
			(foo.p1!='SILVA' and foo.p2!='SILVA' and foo.p3!='SILVA' and foo.p4!='SILVA' and foo.p5!='SILVA' and foo.p6!='SILVA')
			
			) foo2
			where 
			(foo2.r1+foo2.r2+foo2.r3+foo2.r4+foo2.r5+foo2.r6)>1
			order by 3 desc";
	
	echo '<p align="center">Possível nível de parentesco => Hierarquia</p>';
	echo '<div style="height:200px;overflow:auto;">';
	
	$cabecalho = array("<span style=font-size:x-small>Orientador de Estudo</span>","<span style=font-size:x-small>Professor</span>","<span style=font-size:x-small>N</span>");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%','',true, false, false, true);
	
	echo '</div>';
	
}

function bolsistasSISPACTODirigentes($dados) {
	global $db;
	
	echo '<p align="center">Bolsistas do SISPACTO + Dirigente Municipal/Estadual</p>';
	echo '<div style="height:200px;overflow:auto;">';
	
	$sql = "select distinct '<img src=\"../imagens/seta_cima.png\" style=cursor:pointer; onclick=\"window.location=\'sispacto2.php?modulo=consultarcpfpacto&acao=A&iuscpf='||i.iuscpf||'\';\"> <span style=font-size:x-small;>'||replace(to_char(i.iuscpf::numeric, '000:000:000-00'), ':', '.')||'</span>' as iuscpf,
			   '<span style=font-size:x-small;>'||i.iusnome||'</span>' as iusnome,
			   '<span style=font-size:x-small;>'||uu.unisigla||'</span>' as ies,
			   '<span style=font-size:x-small;>'||p.pfldsc||'</span>' as pfldsc,
			   '<span style=font-size:x-small;>'||i.iusemailprincipal||'</span>' as iusemailprincipal
		from sispacto2.identificacaousuario i
		inner join sispacto2.tipoperfil t on t.iusd = i.iusd AND t.pflcod=".PFL_COORDENADORLOCAL." 
		inner join seguranca.perfil p on p.pflcod = t.pflcod
		inner join sispacto2.universidadecadastro u on u.uncid = i.uncid
		inner join sispacto2.universidade uu on uu.uniid = u.uniid
		inner join seguranca.usuario_sistema us on us.usucpf = i.iuscpf and us.sisid=".SIS_PAR." and us.suscod='A'
		inner join seguranca.perfilusuario pu on pu.usucpf = us.usucpf and pu.pflcod in(".PFL_EQESTAP_PAR.",".PFL_EQMUNAP_PAR.")
		".(($dados['uncid'])?"where i.uncid='".$dados['uncid']."'":"");
	
	$cabecalho = array("<span style=font-size:x-small>CPF</span>","<span style=font-size:x-small>Nome</span>","<span style=font-size:x-small>IES</span>","<span style=font-size:x-small>Perfil</span>","<span style=font-size:x-small>E-mail</span>");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%','',true, false, false, true);
	

	echo '</div>';
	
}

function mediaTurmasPerfil($dados) {
	global $db;
	
	$sql_in = "select i.iuscpf, i.iusnome, m.estuf||'-'||m.mundescricao as municipio, uu.unisigla, count(*) as qtd 
				from sispacto2.identificacaousuario i 
				inner join sispacto2.tipoperfil t on t.iusd = i.iusd 
				inner join sispacto2.turmas tu on tu.iusd = i.iusd AND (tu.picid=i.picid OR i.uncid=tu.uncid)
				inner join sispacto2.orientadorturma ot on ot.turid = tu.turid 
				inner join sispacto2.identificacaousuario i2 on i2.iusd = ot.iusd
				inner join sispacto2.universidadecadastro un on un.uncid = i.uncid 
				inner join sispacto2.universidade uu on uu.uniid = un.uniid 
				inner join workflow.documento doc on doc.docid = un.docid 
				left join territorios.municipio m on m.muncod = i.muncodatuacao
				where t.pflcod=".$dados['pflcod']." AND i.iusstatus='A' and i2.iusstatus='A' AND doc.esdid=".ESD_VALIDADO_COORDENADOR_IES." AND i.iuscpf NOT LIKE '%SIS%' ".(($dados['uncid'])?"and i.uncid='".$dados['uncid']."'":"")." 
				group by i.iuscpf, i.iusnome, uu.unisigla, m.estuf, m.mundescricao, tu.turid
				order by i.iusnome";
	
	
	$sql = "SELECT round(avg(foo.qtd),2) as media FROM (
			
			{$sql_in}
			 
			) foo WHERE foo.qtd>0";
			
	$media = $db->pegaUm($sql);
			
	
	echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">';
	echo '<tr><td class="SubTituloEsquerda" width="25%" style=font-size:x-small;>Média de participantes:</td><td style=font-size:x-small;>'.$media.'</td></tr>';
	echo '<tr><td colspan=2 align=center style=font-size:x-small;>';
	
	if(!$dados['filtroconsulta']){$dados['filtroconsulta']="BETWEEN 1 AND 5";}
	
	$sql = "SELECT '<img src=\"../imagens/seta_cima.png\" style=cursor:pointer; onclick=\"window.location=\'sispacto2.php?modulo=consultarcpfpacto&acao=A&iuscpf='||foo.iuscpf||'\';\"> <span style=font-size:x-small;>'||foo.iusnome||'</span>' as iusnome, '<span style=font-size:x-small;>'||foo.municipio||'</span>' as municipio, '<span style=font-size:x-small;>'||foo.unisigla||'</span>' as unisigla, foo.qtd FROM (
	
			{$sql_in}
			
			) foo WHERE foo.qtd {$dados['filtroconsulta']} ORDER BY foo.qtd ASC";
	
	echo '<div style=height:130;overflow:auto;>';
			
	$cabecalho = array("<span style=font-size:x-small;>Nome</span>","<span style=font-size:x-small;>Município</span>","<span style=font-size:x-small;>IES</span>","<span style=font-size:x-small;>Qtd</span>");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%','',true, false, false, true);
	
	echo '</div>';
	
	echo '</td></tr>';
	

	echo '</table>';
} 


function resumoGeralCurso($dados) {
	global $db;

	$sql = "SELECT count(*) as num FROM sispacto2.identificacaousuario i
			INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd
			WHERE t.pflcod='".$dados['pflcod']."'";

	$qtd_total_participantes = $db->pegaUm($sql);

	$sql = "SELECT count(*) as num FROM sispacto2.certificacao c
			WHERE c.pflcod='".$dados['pflcod']."' AND c.cerfrequencia>=75";

	$qtd_total_certificados = $db->pegaUm($sql);

	$sql = "SELECT count(*) as qtd, sum(pbovlrpagamento) as vlr FROM sispacto2.pagamentobolsista p
			INNER JOIN workflow.documento d ON d.docid = p.docid
			WHERE pflcod='".$dados['pflcod']."' AND d.esdid='".ESD_PAGAMENTO_EFETIVADO."'";

	$arrPagamentos = $db->pegaLinha($sql);

	$sql = "SELECT count(*) as qtd, sum(pbovlrpagamento) as vlr FROM sispacto2.pagamentobolsista p
			INNER JOIN workflow.documento d ON d.docid = p.docid
			INNER JOIN sispacto2.certificacao c ON c.iusd = p.iusd
			WHERE p.pflcod='".$dados['pflcod']."' AND d.esdid='".ESD_PAGAMENTO_EFETIVADO."' AND c.cerfrequencia<75";

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


	$sql = "SELECT '<span onmouseover=\"return escape(\''||uu.uninome||'\');\">'||uu.unisigla||'</span>' as universidade, u.uncid FROM sispacto2.universidadecadastro u
			INNER JOIN sispacto2.universidade uu ON uu.uniid = u.uniid";

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
				
				
			$sql = "SELECT count(*) as num FROM sispacto2.identificacaousuario i
			INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd
			WHERE t.pflcod='".$dados['pflcod']."' AND i.uncid='".$unc['uncid']."'";
				
			$qtd_total_participantes = $db->pegaUm($sql);
				
			$sql = "SELECT count(*) as num FROM sispacto2.certificacao c
			INNER JOIN sispacto2.identificacaousuario i ON i.iusd = c.iusd
			INNER JOIN sispacto2.tipoperfil t ON t.iusd = c.iusd
			WHERE t.pflcod='".$dados['pflcod']."' AND c.cerfrequencia>=75 AND i.uncid='".$unc['uncid']."'";
				
			$qtd_total_certificados = $db->pegaUm($sql);
				
			$sql = "SELECT count(*) as qtd, sum(pbovlrpagamento) as vlr FROM sispacto2.pagamentobolsista p
			INNER JOIN sispacto2.identificacaousuario i ON i.iusd = p.iusd
			INNER JOIN workflow.documento d ON d.docid = p.docid
			WHERE pflcod='".$dados['pflcod']."' AND d.esdid='".ESD_PAGAMENTO_EFETIVADO."' AND i.uncid='".$unc['uncid']."'";
				
			$arrPagamentos = $db->pegaLinha($sql);
				
			$sql = "SELECT count(*) as qtd, sum(pbovlrpagamento) as vlr FROM sispacto2.pagamentobolsista p
			INNER JOIN workflow.documento d ON d.docid = p.docid
			INNER JOIN sispacto2.identificacaousuario i ON i.iusd = p.iusd
			INNER JOIN sispacto2.certificacao c ON c.iusd = p.iusd
			WHERE p.pflcod='".$dados['pflcod']."' AND d.esdid='".ESD_PAGAMENTO_EFETIVADO."' AND c.cerfrequencia<75 AND i.uncid='".$unc['uncid']."'";
				
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

?>