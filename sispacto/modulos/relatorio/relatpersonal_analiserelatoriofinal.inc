<?
/* configurações */
ini_set("memory_limit", "3000M");
set_time_limit(0);
/* FIM configurações */

function sobreEquipePedagogica() {

	$arr['sql'] = "select
			uu.unisigla||' - '||uu.uninome as universidade,
	i.iusnome as iusnome,
	p.pfldsc as pfldsc,
	count(distinct ma.mavid) as avaliacoes,
	round(avg(ma.mavtotal),2) as total
	from sispacto.identificacaousuario i
	inner join sispacto.tipoperfil t on t.iusd = i.iusd
	inner join seguranca.perfil p on p.pflcod = t.pflcod
	inner join sispacto.mensario me on me.iusd = i.iusd
	inner join sispacto.mensarioavaliacoes ma on ma.menid = me.menid 
	INNER JOIN sispacto.universidadecadastro u ON u.uncid = i.uncid 
	INNER JOIN sispacto.universidade uu ON uu.uniid = u.uniid
	where t.pflcod in(".PFL_FORMADORIES.",".PFL_SUPERVISORIES.",".PFL_COORDENADORADJUNTOIES.",".PFL_COORDENADORIES.")
			group by i.iusnome, p.pfldsc, uu.unisigla, uu.uninome
			order by p.pfldsc, i.iusnome";
	
	$arr['cabecalho'] = array("Universidade","Nome","Perfil","Número de avaliações","Nota Final");
	
	return $arr;
	
	
}


function sintesePerfilSexo() {
	$arr['sql'] = "select uu.unisigla||' - '||uu.uninome as universidade, i.iussexo, count(i.iusd) as num
				from sispacto.identificacaousuario i 
				INNER JOIN sispacto.universidadecadastro u ON u.uncid = i.uncid 
				INNER JOIN sispacto.universidade uu ON uu.uniid = u.uniid
				inner join sispacto.tipoperfil t on t.iusd = i.iusd and t.pflcod in(".PFL_PROFESSORALFABETIZADOR.",".PFL_ORIENTADORESTUDO.")
				where iustermocompromisso=true
				group by i.iussexo, uu.unisigla, uu.uninome
				order by count(i.iusd) desc";
	
	$arr['cabecalho'] = array("Universidade","Sexo","Total");
	
	return $arr;
	
}

function sintesePerfilFaixaEtaria() { 
	$arr['sql'] = "select universidade, faixa, count(foo.iusd) as num
		from (
		select  i.iusd,
			uu.unisigla||' - '||uu.uninome as universidade,
		CASE WHEN extract(year from age(iusdatanascimento)) >= 60 THEN '60+'
		WHEN extract(year from age(iusdatanascimento)) >= 50 AND extract(year from age(iusdatanascimento)) < 60 THEN '50-60'
		WHEN extract(year from age(iusdatanascimento)) >= 40 AND extract(year from age(iusdatanascimento)) < 50 THEN '40-50'
		WHEN extract(year from age(iusdatanascimento)) >= 30 AND extract(year from age(iusdatanascimento)) < 40 THEN '30-40'
		WHEN extract(year from age(iusdatanascimento)) >= 20 AND extract(year from age(iusdatanascimento)) < 30 THEN '20-30'
		ELSE '19-' END as faixa
		from sispacto.identificacaousuario i 
		INNER JOIN sispacto.universidadecadastro u ON u.uncid = i.uncid 
		INNER JOIN sispacto.universidade uu ON uu.uniid = u.uniid
		inner join sispacto.tipoperfil t on t.iusd = i.iusd and t.pflcod in(".PFL_PROFESSORALFABETIZADOR.",".PFL_ORIENTADORESTUDO.")
		where iustermocompromisso=true
		) foo
		group by faixa, universidade
		order by count(foo.iusd) desc";
	
	$arr['cabecalho'] = array("Universidade","Faixa Etária","Total");
	
	return $arr;
	
}

function sintesePerfilFormacao() {
	$arr['sql'] = "select uu.unisigla||' - '||uu.uninome as universidade, f.foedesc as foedesc, count(i.iusd) as num
				from sispacto.identificacaousuario i 
				INNER JOIN sispacto.universidadecadastro u ON u.uncid = i.uncid 
				INNER JOIN sispacto.universidade uu ON uu.uniid = u.uniid
				inner join sispacto.tipoperfil t on t.iusd = i.iusd and t.pflcod in(".PFL_PROFESSORALFABETIZADOR.",".PFL_ORIENTADORESTUDO.")
				inner join sispacto.formacaoescolaridade f on f.foeid = i.foeid
				where iustermocompromisso=true 
				group by f.foedesc, uu.unisigla, uu.uninome
				order by count(i.iusd) desc";
	
	$arr['cabecalho'] = array("Universidade","Formação","Total");
	
	return $arr;
	
}

function sintesePerfilAreaFormacao() {
	$arr['sql'] = "select uu.unisigla||' - '||uu.uninome as universidade, f.cufcursodesc as cufcursodesc, count(i.iusd) as num
		from sispacto.identificacaousuario i 
		INNER JOIN sispacto.universidadecadastro u ON u.uncid = i.uncid 
		INNER JOIN sispacto.universidade uu ON uu.uniid = u.uniid
		inner join sispacto.tipoperfil t on t.iusd = i.iusd and t.pflcod in(".PFL_PROFESSORALFABETIZADOR.",".PFL_ORIENTADORESTUDO.")
		inner join sispacto.identiusucursoformacao ic on ic.iusd = i.iusd
		inner join sispacto.cursoformacao f on f.cufid = ic.cufid
		where iustermocompromisso=true 
		group by f.cufcursodesc,  uu.unisigla, uu.uninome
		order by count(i.iusd) desc";
	
	$arr['cabecalho'] = array("Universidade","Formação","Total");
	
	return $arr;
	
}


function situacaoFinal() {
	
	$arr['sql'] = "
		SELECT
		foo2.universidade,
		p.pfldsc as perfil,
		count(*) as total,
		SUM(foo2.recomendados) as recomendados,
		SUM(foo2.naorecomendados) as naorecomendados,
		SUM(foo2.naoavaliado) as naoavaliado
		FROM (
	
		SELECT
		i.iusd,
		t.pflcod,
		uu.unisigla||' - '||uu.uninome as universidade,
		CASE WHEN foo.mavrecomendadocertificacao='1' THEN 1 ELSE 0 END as recomendados,
		CASE WHEN foo.mavrecomendadocertificacao='2' THEN 1 ELSE 0 END as naorecomendados,
		CASE WHEN foo.mavrecomendadocertificacao IS NULL THEN 1 ELSE 0 END as naoavaliado
		FROM sispacto.identificacaousuario i 
		INNER JOIN sispacto.universidadecadastro u ON u.uncid = i.uncid 
		INNER JOIN sispacto.universidade uu ON uu.uniid = u.uniid
			
		INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd
		LEFT JOIN (
	
		SELECT distinct m.iusd, mavrecomendadocertificacao FROM sispacto.mensarioavaliacoes ma
		INNER JOIN sispacto.mensario m ON m.menid = ma.menid
		INNER JOIN sispacto.tipoperfil t ON t.iusd = ma.iusdavaliador AND t.pflcod IN(".PFL_FORMADORIES.",".PFL_ORIENTADORESTUDO.")
		WHERE mavrecomendadocertificacao IS NOT NULL
	
	
		) foo ON foo.iusd = i.iusd
		WHERE t.pflcod IN(".PFL_PROFESSORALFABETIZADOR.",".PFL_ORIENTADORESTUDO.")
	
		) foo2
		INNER JOIN seguranca.perfil p ON p.pflcod = foo2.pflcod
		GROUP BY foo2.universidade, p.pfldsc
		";
	$arr['cabecalho'] = array("Universidade","Perfil","Total","Recomendados para certificação","Não recomendados para certificação","Não avaliado");
	
	return $arr;
	
	
}


function avaliacaoCoordenadoresLocais() {
	
	$arr['sql'] = "select
				uu.unisigla||' - '||uu.uninome as universidade,
				i.iusnome,
				case when m.muncod is not null then m.estuf||' / '||m.mundescricao||' ( Municipal )'
				     when e.estuf is not null then e.estuf||' / '||e.estdescricao||' ( Estadual )' end as esfera,
				count(distinct me.menid) as avaliacoes,
				round(avg(ma.mavtotal),2) as total
				from sispacto.identificacaousuario i
				inner join sispacto.tipoperfil t on t.iusd = i.iusd
				inner join sispacto.pactoidadecerta p on p.picid = i.picid
				inner join sispacto.mensario me on me.iusd = i.iusd
				inner join sispacto.mensarioavaliacoes ma on ma.menid = me.menid 
				INNER JOIN sispacto.universidadecadastro u ON u.uncid = i.uncid 
				INNER JOIN sispacto.universidade uu ON uu.uniid = u.uniid
				left join territorios.municipio m on m.muncod = p.muncod
				left join territorios.estado e on e.estuf = p.estuf
				where t.pflcod=".PFL_COORDENADORLOCAL." 
				group by i.iusnome, m.muncod, e.estuf, m.estuf, m.mundescricao, e.estdescricao, uu.unisigla, uu.uninome
				order by i.iusnome";
	
		
	$arr['cabecalho'] = array("Universidade","Nome","UF / Município","Número de avaliações","Nota final");
	
	return $arr;
	
	
	
}

function bolsasPagasPorPerfis() {
	
	$arr['sql'] = "SELECT uu.unisigla||' - '||uu.uninome as universidade, pp.pfldsc, count(*) as numbolsas, sum(pbovlrpagamento) as valorpago FROM sispacto.pagamentobolsista p 
				INNER JOIN sispacto.universidadecadastro u ON u.uniid = p.uniid 
				INNER JOIN seguranca.perfil pp ON pp.pflcod = p.pflcod 
				INNER JOIN workflow.documento d ON d.docid = p.docid 
				INNER JOIN sispacto.universidade uu ON uu.uniid = u.uniid
				WHERE d.esdid=".ESD_PAGAMENTO_EFETIVADO." 
				GROUP BY pp.pfldsc, uu.unisigla, uu.uninome
				  ";
	
	$arr['cabecalho'] = array("Universidade","Perfil","Número de bolsas","Valor pago(R$)");
	
	return $arr;	
	
}

function avaliacaoCursistasConteudo() {
	$arr['sql'] = "select
				uu.unisigla||' - '||uu.uninome as universidade, 
				i.iacdsc,
				count(distinct r.iusdavaliador) as avaliadores,
				avg(a.iccvalor) as nota
			from sispacto.respostasavaliacaocomplementar r
			inner join sispacto.identificacaousuario ii on ii.iusd = r.iusdavaliador
			inner join sispacto.tipoperfil t on t.iusd = ii.iusd
			inner join sispacto.itensavaliacaocomplementarcriterio a on a.iccid = r.iccid
			inner join sispacto.itensavaliacaocomplementar i on i.iacid = r.iacid
			inner join seguranca.perfil pp on pp.pflcod = t.pflcod 
			INNER JOIN sispacto.universidadecadastro u ON u.uncid = ii.uncid
			INNER JOIN sispacto.universidade uu ON uu.uniid = u.uniid
			where i.gicid=1 and ii.iusstatus='A' AND pp.pflcod in(".PFL_PROFESSORALFABETIZADOR.",".PFL_ORIENTADORESTUDO.")
			group by i.iacdsc, uu.unisigla, uu.uninome
			order by 3
				  ";
	
	$arr['cabecalho'] = array("Universidade","Quanto aos conteúdos","Número de avaliadores","Nota");
	
	return $arr;
}

function avaliacaoCursistasAutoavaliacao() {
	$arr['sql'] = "select
				uu.unisigla||' - '||uu.uninome as universidade,
			i.iacdsc,
				count(distinct r.iusdavaliador) as avaliadores,
				avg(a.iccvalor) as nota
			from sispacto.respostasavaliacaocomplementar r
			inner join sispacto.identificacaousuario ii on ii.iusd = r.iusdavaliador
			inner join sispacto.tipoperfil t on t.iusd = ii.iusd
			inner join sispacto.itensavaliacaocomplementarcriterio a on a.iccid = r.iccid
			inner join sispacto.itensavaliacaocomplementar i on i.iacid = r.iacid
			inner join seguranca.perfil pp on pp.pflcod = t.pflcod 
			INNER JOIN sispacto.universidadecadastro u ON u.uncid = ii.uncid
			INNER JOIN sispacto.universidade uu ON uu.uniid = u.uniid
			where i.gicid in(4,6) and ii.iusstatus='A' AND pp.pflcod in(".PFL_PROFESSORALFABETIZADOR.",".PFL_ORIENTADORESTUDO.")
			group by i.iacdsc, uu.unisigla, uu.uninome
			order by 3
				  ";

	$arr['cabecalho'] = array("Universidade","Autoavaliação","Número de avaliadores","Nota");

	return $arr;
}


function encontrosPresenciais() {
	$arr['sql'] = "SELECT uu.unisigla||' - '||uu.uninome as universidade, repnome, to_char(repdata,'dd/mm/YYYY') as repdata, repcargahoraria 
				  FROM sispacto.relatoriofinalencontrospresenciais r 
				  INNER JOIN sispacto.relatoriofinal rr ON rr.rlfid = r.rlfid 
				  INNER JOIN sispacto.universidadecadastro u ON u.uncid = rr.uncid 
				  INNER JOIN sispacto.universidade uu ON uu.uniid = u.uniid";
	
	$arr['cabecalho'] = array("Universidade","Evento","Data","Carga Horária");
	
	return $arr;
	
}

function polos() {
	
	$arr['sql'] = "SELECT uu.unisigla||' - '||uu.uninome as universidade, m.estuf||' / '||m.mundescricao as polo, count(t.turid) as num FROM sispacto.turmas t
					INNER JOIN territorios.municipio m On m.muncod = t.muncod 
					INNER JOIN sispacto.universidadecadastro u ON u.uncid = t.uncid 
					INNER JOIN sispacto.universidade uu ON uu.uniid = u.uniid
					GROUP BY m.estuf, m.mundescricao, uu.unisigla, uu.uninome";
	
	$arr['cabecalho'] = array("Universidade","Pólo","Número de turmas");
	
	return $arr;
	
	
}

function questoesDissertativas() {
	
	$arr['sql'] = "SELECT uu.unisigla||' - '||uu.uninome as universidade,
						  rlfperexecinicio,
						  rlfperexecfim,
						  REPLACE(REPLACE(REPLACE(REPLACE(REPLACE(REPLACE(rlfestruturafisica,'1;','Sala de aula da IES; '),'2;','Equipamentos de som e imagem; '),'3;','Sala de aula de escola pública; '),'4;','Equipamentos de informática; '),'5;','Locação de espaço; '),'6;','Outros materiais pedagógicos; ') as rlfestruturafisica,
						  rlfcolegiadocurso,
  						  rlfarticulacaoinstitucional,
  						  rlfcomentariosavaliacaocoordenadoreslocais,
  						  rlfplanejamentopedagogicocurso,
  						  rlforganizacaopedagogicacurso,
  						  rlfacompanhamentocursistas,
  						  rlfconteudocurso,
  						  rlfmetodologia,
  						  rlfcriteriosavaliacoes,
  						  rlfequipepedagogica,
  						  rlfarticulacaomec,
  						  rlflicoesaprendidas,
  						  rlfsugestoes,
  						  rlfoutroscomentarios 
					FROM sispacto.relatoriofinal r 
					INNER JOIN sispacto.universidadecadastro u ON u.uncid = r.uncid 
					INNER JOIN sispacto.universidade uu ON uu.uniid = u.uniid";
	
	$arr['cabecalho'] = array("Universidade","Período de Execução do Projeto(Inicío)","Período de Execução do Projeto(Fim)","Instalações e equipamentos utilizados durante os encontros de formação com os Orientadores de Estudo","Colegiado do Curso","Articulação Institucional","Comentários das avaliações dos Coordenadores Locais","Planejamento Pedagógico do Curso",
							  "Organização Pedagógica do Curso","Acompanhamento dos Cursistas","Sobre o Conteúdo do Curso","Sobre a metologia","Sobre os Critérios de Avaliações",
							  "Comentários sobre a Equipe Pedagógica","Sobre a Articulação com o MEC","Lições Aprendidas","Sugestões","Outros Comentários");
	
	return $arr;
	
	
	
}

function equipeIES() {
	$arr['sql'] = "SELECT uu.unisigla||' - '||uu.uninome as universidade,
						  i.iusnome,
					   	  s.pfldsc,
					   	  f.foedesc,
			           	  tp.tvpdsc
				FROM sispacto.identificacaousuario i
				INNER JOIN sispacto.tipoperfil t ON i.iusd = t.iusd
				INNER JOIN seguranca.perfil s ON s.pflcod = t.pflcod 
				INNER JOIN sispacto.universidadecadastro u ON u.uncid = i.uncid 
				INNER JOIN sispacto.universidade uu ON uu.uniid = u.uniid
				LEFT JOIN sispacto.formacaoescolaridade f ON f.foeid = i.foeid 
				LEFT JOIN public.tipovinculoprofissional tp ON tp.tvpid = i.tvpid
				WHERE t.pflcod IN(".PFL_COORDENADORADJUNTOIES.",".PFL_SUPERVISORIES.",".PFL_FORMADORIES.")
				ORDER BY uu.unisigla, s.pfldsc, i.iusnome";
	
	$arr['cabecalho'] = array("Universidade","Nome","Função","Titulação","Vínculo");
	
	return $arr;
	
}

function gerarXlsRelatorioFinal($dados) {
	global $db;
	
	$arr = $dados['tipoxlsrelatoriofinal']();
	
	ob_clean();
	header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
	header ( "Pragma: no-cache" );
	header ( "Content-type: application/xls; name=rel_coordenador_".date("Ymdhis").".xls");
	header ( "Content-Disposition: attachment; filename=rel_final_".date("Ymdhis").".xls");
	header ( "Content-Description: MID Gera excel" );
	$db->monta_lista_tabulado($arr['sql'],$arr['cabecalho'],1000000,5,'N','100%','');
	
	
}


if($_REQUEST['tipoxlsrelatoriofinal']) {
	gerarXlsRelatorioFinal($_REQUEST);
	exit;
}

?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="JavaScript" src="./js/sispacto.js"></script>
<script>

function aplicarFiltros() {
	
	document.getElementById('formulario').submit();

}

</script>

<form method="post" id="formulario">
<input type="hidden" name="buscar" value="1">
<input type="hidden" name="aplicar" id="aplicar" value="1">
<input type="hidden" name="relatorio" value="relatpersonal_analiserelatoriofinal.inc">
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
       <tr>
        	<td class="SubTituloDireita" width="25%">Tipo</td>
        	<td>
        	<input type="radio" name="tipoxlsrelatoriofinal" value="questoesDissertativas"> Questões dissertativas do relatório final<br/>
        	<input type="radio" name="tipoxlsrelatoriofinal" value="equipeIES"> Equipe IES<br/>
        	<input type="radio" name="tipoxlsrelatoriofinal" value="polos"> Pólos<br/>
        	<input type="radio" name="tipoxlsrelatoriofinal" value="encontrosPresenciais"> Encontros Presenciais<br/>
        	<input type="radio" name="tipoxlsrelatoriofinal" value="avaliacaoCoordenadoresLocais"> Avaliação dos Coordenadores Locais<br/>
        	<input type="radio" name="tipoxlsrelatoriofinal" value="situacaoFinal"> Situação Final<br/>
        	<input type="radio" name="tipoxlsrelatoriofinal" value="sintesePerfilSexo"> Síntese do Perfil dos Cursistas - Sexo<br/>
        	<input type="radio" name="tipoxlsrelatoriofinal" value="sintesePerfilFaixaEtaria"> Síntese do Perfil dos Cursistas - Faixa Etária<br/>
        	<input type="radio" name="tipoxlsrelatoriofinal" value="sintesePerfilFormacao"> Síntese do Perfil dos Cursistas - Formação<br/>
        	<input type="radio" name="tipoxlsrelatoriofinal" value="sintesePerfilAreaFormacao"> Síntese do Perfil dos Cursistas - Área de Formação<br/>
        	<input type="radio" name="tipoxlsrelatoriofinal" value="avaliacaoCursistasConteudo"> Avaliação feitas pelos cursistas (sobre conteúdo)<br/>
        	<input type="radio" name="tipoxlsrelatoriofinal" value="avaliacaoCursistasAutoavaliacao"> Avaliação feitas pelos cursistas (autoavaliação)<br/>
        	<input type="radio" name="tipoxlsrelatoriofinal" value="bolsasPagasPorPerfis"> Bolsas pagas por perfis<br/>
        	<input type="radio" name="tipoxlsrelatoriofinal" value="sobreEquipePedagogica"> Sobre equipe pedagógica<br/>
        	</td>
        </tr>

        <tr>
			<td class="SubTituloCentro" colspan="2"><input type="button" onclick="aplicarFiltros();" name="aplicar" value="Ver XLS"></td>
        </tr>
</table>
</form>