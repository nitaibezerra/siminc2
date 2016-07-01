<?

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(0);
/* FIM configurações */

verificarCoordenadorIESTermoCompromisso(array("uncid"=>$_SESSION['sispacto']['universidade']['uncid']));

$relatoriofinal = $db->pegaLinha("SELECT * FROM sispacto.relatoriofinal WHERE uncid='".$_SESSION['sispacto']['universidade']['uncid']."'");

if(!$relatoriofinal) {
	$db->executar("INSERT INTO sispacto.relatoriofinal(uncid) VALUES ('".$_SESSION['sispacto']['universidade']['uncid']."');");
	$db->commit();
} else {
	extract($relatoriofinal);
}

if(!$relatoriofinal['docid']) {
	
	$docid = wf_cadastrarDocumento( TPD_FLUXOAPROVACAO, 'Relatório Final : '.$_SESSION['sispacto']['universidade']['descricao'] );
	
	$sql = "UPDATE sispacto.relatoriofinal SET docid='".$docid."' WHERE uncid='".$_SESSION['sispacto']['universidade']['uncid']."'";
	$db->executar($sql);
	$db->commit();
	
}


$estado = wf_pegarEstadoAtual( $docid );


if($estado['esdid'] != ESD_ORCAMENTO_EM_ELABORACAO) {
	$consulta = true;
}

?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>

<script>

function atualizarRelatorioFinal() {

	document.getElementById('formulario').submit();
	
}

function inserirEncontroPresencial() {

	if(jQuery('#repnome').val()=='') {
		alert('Nome do evento em branco');
		return false;
	}

	if(jQuery('#repdata').val()=='') {
		alert('Data do evento em branco');
		return false;
	}

	if(jQuery('#repcargahoraria').val()=='') {
		alert('Carga horária em branco');
		return false;
	}

	ajaxatualizar('requisicao=inserirEncontroPresencial&uncid=<?=$_SESSION['sispacto']['universidade']['uncid'] ?>&repnome='+jQuery('#repnome').val()+'&repdata='+jQuery('#repdata').val()+'&repcargahoraria='+jQuery('#repcargahoraria').val(),'');
	ajaxatualizar('requisicao=listarEncontrosPresenciais&uncid=<?=$_SESSION['sispacto']['universidade']['uncid'] ?>','td_listaencontrospresenciais');

	jQuery('#repnome').val('');
	jQuery('#repdata').val('');
	jQuery('#repcargahoraria').val('');
	
	
}

function excluirEncontroPresencial(repid) {
	var conf = confirm('Deseja realmente excluir este encontro?');

	if(conf) {
		
		ajaxatualizar('requisicao=excluirEncontroPresencial&repid='+repid,'');
		ajaxatualizar('requisicao=listarEncontrosPresenciais&uncid=<?=$_SESSION['sispacto']['universidade']['uncid'] ?>','td_listaencontrospresenciais');
		
	}
}


<? if($consulta) : ?>
jQuery(function() {
jQuery("[type='text'],textarea").attr('class','disabled');
jQuery("[type='text'],textarea").attr('disabled','disabled');
jQuery("[name='rlfestruturafisica[]']").attr('disabled','disabled');
jQuery("[id^='encontrop_']").remove();
jQuery("[id='tr_inserirencontropresencial']").remove();
jQuery("[id='inserirevento']").remove();
jQuery("[id='salvar']").remove();

});
<? endif; ?>

</script>
<form method="post" id="formulario">
<input type="hidden" name="goto" id="goto" value="">
<input type="hidden" name="requisicao" value="atualizarRelatorioFinal">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloCentro" colspan="3">Relatório Final</td>
	</tr>
	<tr>
		<td class="SubTituloEsquerda" colspan="3">1. DADOS GERAIS</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Instituição</td>
		<td><?=$db->pegaUm("SELECT uu.unisigla||' - '||uu.uninome as universidade FROM sispacto.universidadecadastro u INNER JOIN sispacto.universidade uu ON uu.uniid = u.uniid WHERE u.uncid='".$_SESSION['sispacto']['universidade']['uncid']."'") ?></td>
		<td rowspan="4" valign="top" width="1%">
		<?
		/* Barra de estado atual e ações e Historico */
		wf_desenhaBarraNavegacao($docid, array() );
 		?>
 		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Curso</td>
		<td>Formação Continuada de Professores Alfabetizadores e Orientadores de Estudo no âmbito do Pacto Nacional pela Alfabetização na Idade Certa</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Coordenador(a)</td>
		<td><?=$db->pegaUm("SELECT iusnome FROM sispacto.identificacaousuario WHERe iusd='".$_SESSION['sispacto']['universidade']['iusd']."'")?></td>
	</tr>
	<tr>
		<td colspan="2">
		
		<b>Equipe IES</b>
		
		<br>
		<br>
		
		<?
		
		$sql = "SELECT '<font style=font-size:xx-small;>'||i.iusnome||'</font>' as iusnome, 
					   '<font style=font-size:xx-small;>'||s.pfldsc||'</font>' as pfldsc, 
					   '<font style=font-size:xx-small;>'||f.foedesc||'</font>' as foedesc, 
			           '<font style=font-size:xx-small;>'||tp.tvpdsc||'</font>' as tvpdsc 
				FROM sispacto.identificacaousuario i 
				INNER JOIN sispacto.tipoperfil t ON i.iusd = t.iusd 
				INNER JOIN seguranca.perfil s ON s.pflcod = t.pflcod 
				LEFT JOIN sispacto.formacaoescolaridade f ON f.foeid = i.foeid 
				LEFT JOIN public.tipovinculoprofissional tp ON tp.tvpid = i.tvpid
				WHERE t.pflcod IN(".PFL_COORDENADORADJUNTOIES.",".PFL_SUPERVISORIES.",".PFL_FORMADORIES.") AND i.uncid='".$_SESSION['sispacto']['universidade']['uncid']."' 
				ORDER BY s.pfldsc, i.iusnome";
		
		$cabecalho = array("<font style=font-size:xx-small;>Nome</font>","<font style=font-size:xx-small;>Função</font>","<font style=font-size:xx-small;>Titulação</font>","<font style=font-size:xx-small;>Vínculo</font>");
		$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','95%','center');
		
		?>
		
		
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Período de Execução do Projeto</td>
		<td colspan="2"><?=campo_data2('rlfperexecinicio','S', 'S', 'Período de Execução do Projeto', 'S', '', '', '', '', '', 'rlfperexecinicio'); ?> à <?=campo_data2('rlfperexecfim','S', 'S', 'Período de Execução do Projeto', 'S', '', '', '', '', '', 'rlfperexecfim'); ?></td>
	</tr>
	<tr>
		<td class="SubTituloEsquerda" colspan="3">2. ORGANIZAÇÃO DO CURSO</td>
	</tr>
	<tr>
		<td colspan="3">
		
		<b>Pólos</b>
		
		<br>
		
		<?
		$sql = "SELECT m.estuf||' / '||m.mundescricao as polo, count(t.turid) as num FROM sispacto.turmas t
				INNER JOIN territorios.municipio m On m.muncod = t.muncod
				WHERE uncid=".$_SESSION['sispacto']['universidade']['uncid']." 
				GROUP BY m.estuf, m.mundescricao";
		
		$cabecalho = array("Pólo","Número de turmas");
		$db->monta_lista_simples($sql,$cabecalho,100000,5,'N','80%','center');
		?>
		</td>
	</tr>
	
	<tr>
		<td class="SubTituloDireita" width="20%">Estrutura Física e Suporte</td>
		<td colspan="2">
		
		<table width="100%">
		<tr>
			<td colspan="4"><b>Selecione as instalações e equipamentos utilizados durante os encontros de formação com os Orientadores de Estudo</b></td>
		</tr>
		<? $rlfestruturafisica_p = explode(";",$rlfestruturafisica); ?>
		<tr>
			<td><input type="checkbox" name="rlfestruturafisica[]" <?=((in_array("1",$rlfestruturafisica_p))?"checked":"") ?> value="1"></td>
			<td>Sala de aula da IES</td>
			<td><input type="checkbox" name="rlfestruturafisica[]" <?=((in_array("2",$rlfestruturafisica_p))?"checked":"") ?> value="2"></td>
			<td>Equipamentos de som e imagem</td>
		</tr>
		<tr>
			<td><input type="checkbox" name="rlfestruturafisica[]" <?=((in_array("3",$rlfestruturafisica_p))?"checked":"") ?> value="3"></td>
			<td>Sala de aula de escola pública</td>
			<td><input type="checkbox" name="rlfestruturafisica[]" <?=((in_array("4",$rlfestruturafisica_p))?"checked":"") ?> value="4"></td>
			<td>Equipamentos de informática</td>
		</tr>
		<tr>
			<td><input type="checkbox" name="rlfestruturafisica[]" <?=((in_array("5",$rlfestruturafisica_p))?"checked":"") ?> value="5"></td>
			<td>Locação de espaço</td>
			<td><input type="checkbox" name="rlfestruturafisica[]" <?=((in_array("6",$rlfestruturafisica_p))?"checked":"") ?> value="6"></td>
			<td>Outros materiais pedagógicos</td>
		</tr>
		</table>
		
		
		</td>
	</tr>
	
	<tr>
		<td colspan="3">
		
		<b>Encontros Presenciais</b>
		
		<br><br>
		
		
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
			<tr id="tr_inserirencontropresencial">
				<td class="SubTituloDireita">Nome do evento</td>
				<td><?=campo_texto('repnome', "S", "S", "Nome do evento", 40, 150, "", "", '', '', 0, 'id="repnome"', ''); ?></td>
				<td class="SubTituloDireita">Data do evento</td>
				<td><?=campo_data2('repdata','S', 'S', 'Data do evento', 'S', '', '', '', '', '', 'repdata'); ?></td>
				<td class="SubTituloDireita">Carga horária</td>
				<td><?=campo_texto('repcargahoraria', "S", "S", "Carga horária", 5, 4, "####", "", '', '', 0, 'id="repcargahoraria"', ''); ?></td>
				<td><input type="button" name="inserirevento" id="inserirevento" value="Inserir evento" onclick="inserirEncontroPresencial();"></td>
			</tr>
			<tr>
				<td colspan="7" id="td_listaencontrospresenciais">
				<?
				listarEncontrosPresenciais(array('uncid' => $_SESSION['sispacto']['universidade']['uncid']));
				?>
				</td>
			</tr>
			
		</table>
		
		
		</td>
	</tr>
	
	<tr>
		<td class="SubTituloEsquerda" colspan="3">3. DESENVOLVIMENTO E GESTÃO DO CURSO</td>
	</tr>
	
	<tr>
		<td class="SubTituloDireita" width="20%">Colegiado do Curso <img src="../imagens/ajuda.gif" align="absmiddle" onmouseover="return escape('Descrever e avaliar a composição do colegiado e organização do trabalho (sistemática/ periodicidade de reuniões, formas de acompanhamento do trabalho dos formadores, formas/procedimentos de  acompanhamento dos cursistas e pólos)');"></td>
		<td colspan="2"><?=campo_textarea('rlfcolegiadocurso', 'N', 'S', '', '150', '7', '2000'); ?></td>
	</tr>
	
	<tr>
		<td class="SubTituloDireita" width="20%">Articulação Institucional <img src="../imagens/ajuda.gif" align="absmiddle" onmouseover="return escape('Descrever e avaliar a participação efetiva da SEDUC, UNDIME e UNCME no planejamento do curso (ações partilhadas no Colegiado, ações orientadas ou apoio nos/pelos pólos, apoio e infraestrutura oferecidos para a efetiva participação de suas equipes no desenvolvimento do curso)');"></td>
		<td colspan="2"><?=campo_textarea('rlfarticulacaoinstitucional', 'N', 'S', '', '150', '7', '2000'); ?></td>
	</tr>
	
	<tr>
		<td colspan="3">
		
		<b>Avaliação dos Coordenadores Locais</b>
		
		<br><br>
		
		<?
		
		$sql = "select 
				'<font style=font-size:xx-small;>'||i.iusnome||'</font>' as iusnome, 
				'<font style=font-size:xx-small;>'||case when m.muncod is not null then m.estuf||' / '||m.mundescricao||' ( Municipal )' 
				     when e.estuf is not null then e.estuf||' / '||e.estdescricao||' ( Estadual )' end||'</font>' as esfera,
				'<font style=font-size:xx-small;float:right;>'||count(distinct me.menid)||'</font>' as avaliacoes,
				'<font style=font-size:xx-small;float:right;>'||round(avg(ma.mavtotal),2)||'</font>' as total
				from sispacto.identificacaousuario i 
				inner join sispacto.tipoperfil t on t.iusd = i.iusd 
				inner join sispacto.pactoidadecerta p on p.picid = i.picid 
				inner join sispacto.mensario me on me.iusd = i.iusd 
				inner join sispacto.mensarioavaliacoes ma on ma.menid = me.menid
				left join territorios.municipio m on m.muncod = p.muncod 
				left join territorios.estado e on e.estuf = p.estuf 
				where t.pflcod=".PFL_COORDENADORLOCAL." and i.uncid='".$_SESSION['sispacto']['universidade']['uncid']."' 
				group by i.iusnome, m.muncod, e.estuf, m.estuf, m.mundescricao, e.estdescricao 
				order by i.iusnome"; 
		
					
		$cabecalho = array("<font style=font-size:xx-small;>Nome</font>","<font style=font-size:xx-small;>UF / Município</font>","<font style=font-size:xx-small;>Número de avaliações</font>","<font style=font-size:xx-small;>Nota final</font>");
		$db->monta_lista_simples($sql,$cabecalho,100000,5,'N','95%','center');
		
		?>
		
		<br>
		
		<b>Comentários</b>
		
		<br>
		
		<?=campo_textarea('rlfcomentariosavaliacaocoordenadoreslocais', 'N', 'S', '', '150', '7', '2000'); ?>
		
		</td>
	</tr>
	
	<tr>
		<td class="SubTituloDireita" width="20%">Planejamento Pedagógico do Curso <img src="../imagens/ajuda.gif" align="absmiddle" onmouseover="return escape('- Descrever os processos envolvendo a discussão da proposta teórico-metodológica do curso, sua tramitação e os indicadores da proposta aprovada na Universidade<br>- Avaliar sistemática de reuniões para o planejamento (como, quem participa, ações previstas e implementadas)');"></td>
		<td colspan="2"><?=campo_textarea('rlfplanejamentopedagogicocurso', 'N', 'S', '', '150', '7', '2000'); ?></td>
	</tr>
	
	<tr>
		<td class="SubTituloDireita" width="20%">Organização Pedagógica do Curso <img src="../imagens/ajuda.gif" align="absmiddle" onmouseover="return escape('- Periodicidade de reuniões dos coordenadores geral e adjunto, supervisores e formadores;<br>- Encaminhamentos acadêmicos relacionados aos cursistas no que tange à participação nas atividades propostas, encaminhamentos de atividades, recuperação de atividades, etc;<br>- Relação pedagógica entre a Equipe da IES e os cursistas.');"></td>
		<td colspan="2"><?=campo_textarea('rlforganizacaopedagogicacurso', 'N', 'S', '', '150', '7', '2000'); ?></td>
	</tr>
	
	<tr>
		<td colspan="3">
		
		<b>Gestão Financeira</b>
		
		<br>
		
		<? carregarListaCustos(array('uncid' => $_SESSION['sispacto']['universidade']['uncid'],'relatoriofinal' => true)); ?>
		
		</td>
	</tr>
	
	<tr>
		<td class="SubTituloEsquerda" colspan="3">4. PERFIL DOS CURSISTAS E SITUAÇÃO FINAL</td>
	</tr>
	
	<tr>
		<td colspan="3">
		<b>Situação Final</b>
		
		<br><br>
		
		<?
		
		$sql = "
		SELECT
		p.pfldsc as perfil,
		count(*) as total,
		SUM(foo2.recomendados) as recomendados,
		SUM(foo2.naorecomendados) as naorecomendados,
		SUM(foo2.naoavaliado) as naoavaliado
		FROM (
		
		SELECT
		i.iusd,
		t.pflcod,
		CASE WHEN foo.mavrecomendadocertificacao='1' THEN 1 ELSE 0 END as recomendados,
		CASE WHEN foo.mavrecomendadocertificacao='2' THEN 1 ELSE 0 END as naorecomendados,
		CASE WHEN foo.mavrecomendadocertificacao IS NULL THEN 1 ELSE 0 END as naoavaliado
		FROM sispacto.identificacaousuario i
		INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd
		LEFT JOIN (
		
		SELECT distinct m.iusd, mavrecomendadocertificacao FROM sispacto.mensarioavaliacoes ma
		INNER JOIN sispacto.mensario m ON m.menid = ma.menid
		INNER JOIN sispacto.tipoperfil t ON t.iusd = ma.iusdavaliador AND t.pflcod IN(".PFL_FORMADORIES.",".PFL_ORIENTADORESTUDO.")
		WHERE mavrecomendadocertificacao IS NOT NULL
		
		
		) foo ON foo.iusd = i.iusd
		WHERE t.pflcod IN(".PFL_PROFESSORALFABETIZADOR.",".PFL_ORIENTADORESTUDO.") AND uncid='".$_SESSION['sispacto']['universidade']['uncid']."'
		
		) foo2
		INNER JOIN seguranca.perfil p ON p.pflcod = foo2.pflcod
		GROUP BY p.pfldsc
		";
		$cabecalho = array("Perfil","Total","Recomendados para certificação","Não recomendados para certificação","Não avaliado");
		$db->monta_lista_simples($sql,$cabecalho,100000,5,'N','95%','center');
		
		?>
		<p align="right"><font style=font-size:xx-small;><b>* Para maiores detalhes sobre a situação final, acesse Relatórios => Relatório de Recomendados</b></font></p>
		</td>
	</tr>
	
	
	<tr>
		<td colspan="3">
		
		<b>Síntese do Perfil dos Cursistas</b>
		
		<br><br>
		
		<b>Sexo</b>
		<br>
		<?
		
		$sql = "select '<font style=font-size:xx-small;>'||i.iussexo||'</font>' as iussexo, '<font style=font-size:xx-small;float:right;>'||count(i.iusd)||'</font>' as num, '<font style=font-size:xx-small;float:right;>'||round(( (count(i.iusd)*100)::numeric / (SELECT count(*) FROM sispacto.identificacaousuario i inner join sispacto.tipoperfil t on t.iusd = i.iusd and t.pflcod in(".PFL_PROFESSORALFABETIZADOR.",".PFL_ORIENTADORESTUDO.") where iustermocompromisso=true and i.uncid='".$_SESSION['sispacto']['universidade']['uncid']."')::numeric ),2)||'</font>' as porcent
				from sispacto.identificacaousuario i
				inner join sispacto.tipoperfil t on t.iusd = i.iusd and t.pflcod in(".PFL_PROFESSORALFABETIZADOR.",".PFL_ORIENTADORESTUDO.")
				where iustermocompromisso=true and i.uncid='".$_SESSION['sispacto']['universidade']['uncid']."'
				group by i.iussexo
				order by count(i.iusd) desc";
		
		$cabecalho = array("<font style=font-size:xx-small;>Sexo</font>","<font style=font-size:xx-small;>Total</font>","<font style=font-size:xx-small;>%</font>");
		$db->monta_lista_simples($sql,$cabecalho,100000,5,'N','80%','center');
		
		?>
		
		<b>Faixa Etária</b>
		<br>
		<?
		
		$sql = "select '<font style=font-size:xx-small;>'||faixa||'</font>' as faixa, '<font style=font-size:xx-small;float:right;>'||count(foo.iusd)||'</font>' as num, '<font style=font-size:xx-small;float:right;>'||round(( (count(foo.iusd)*100)::numeric / (SELECT count(*) FROM sispacto.identificacaousuario i inner join sispacto.tipoperfil t on t.iusd = i.iusd  and t.pflcod in(".PFL_PROFESSORALFABETIZADOR.",".PFL_ORIENTADORESTUDO.") where iustermocompromisso=true and i.uncid='".$_SESSION['sispacto']['universidade']['uncid']."')::numeric ),2)||'</font>' as porcent
		from (
		select  i.iusd,
		CASE WHEN extract(year from age(iusdatanascimento)) >= 60 THEN '60+'
		WHEN extract(year from age(iusdatanascimento)) >= 50 AND extract(year from age(iusdatanascimento)) < 60 THEN '50-60'
		WHEN extract(year from age(iusdatanascimento)) >= 40 AND extract(year from age(iusdatanascimento)) < 50 THEN '40-50'
		WHEN extract(year from age(iusdatanascimento)) >= 30 AND extract(year from age(iusdatanascimento)) < 40 THEN '30-40'
		WHEN extract(year from age(iusdatanascimento)) >= 20 AND extract(year from age(iusdatanascimento)) < 30 THEN '20-30'
		ELSE '19-' END as faixa
		from sispacto.identificacaousuario i
		inner join sispacto.tipoperfil t on t.iusd = i.iusd and t.pflcod in(".PFL_PROFESSORALFABETIZADOR.",".PFL_ORIENTADORESTUDO.")
		where iustermocompromisso=true and i.uncid='".$_SESSION['sispacto']['universidade']['uncid']."'
		) foo
		group by faixa
		order by count(foo.iusd) desc";
		
		$cabecalho = array("<font style=font-size:xx-small;>Faixa Etária</font>","<font style=font-size:xx-small;>Total</font>","<font style=font-size:xx-small;>%</font>");
		$db->monta_lista_simples($sql,$cabecalho,100000,5,'N','80%','center');
		
		?>
		
		<b>Formação</b>
		<br>
		<?
		
		$sql = "select '<font style=font-size:xx-small;>'||f.foedesc||'</font>' as foedesc, '<font style=font-size:xx-small;float:right;>'||count(i.iusd)||'</font>' as num, '<font style=font-size:xx-small;float:right;>'||round(( (count(i.iusd)*100)::numeric / (SELECT count(*) FROM sispacto.identificacaousuario i inner join sispacto.tipoperfil t on t.iusd = i.iusd and t.pflcod in(".PFL_PROFESSORALFABETIZADOR.",".PFL_ORIENTADORESTUDO.") where iustermocompromisso=true and i.uncid='".$_SESSION['sispacto']['universidade']['uncid']."')::numeric ),2)||'</font>' as porcent
				from sispacto.identificacaousuario i
				inner join sispacto.tipoperfil t on t.iusd = i.iusd and t.pflcod in(".PFL_PROFESSORALFABETIZADOR.",".PFL_ORIENTADORESTUDO.")
				inner join sispacto.formacaoescolaridade f on f.foeid = i.foeid
				where iustermocompromisso=true and i.uncid='".$_SESSION['sispacto']['universidade']['uncid']."' 
				group by f.foedesc
				order by count(i.iusd) desc";
	
		$cabecalho = array("<font style=font-size:xx-small;>Formação</font>","<font style=font-size:xx-small;>Total</font>","<font style=font-size:xx-small;>%</font>");
		$db->monta_lista_simples($sql,$cabecalho,100000,5,'N','80%','center');
		
		?>
		
		<b>Área de Formação</b>
		<br>
		<? 
		$sql = "select '<font style=font-size:xx-small;>'||f.cufcursodesc||'</font>' as cufcursodesc, '<font style=font-size:xx-small;float:right;>'||count(i.iusd)||'</font>'as num, '<font style=font-size:xx-small;float:right;>'||round(( (count(i.iusd)*100)::numeric / (SELECT count(*) FROM sispacto.identificacaousuario i inner join sispacto.tipoperfil t on t.iusd = i.iusd and t.pflcod in(".PFL_PROFESSORALFABETIZADOR.",".PFL_ORIENTADORESTUDO.") where iustermocompromisso=true and i.uncid='".$_SESSION['sispacto']['universidade']['uncid']."')::numeric ),2)||'</font>' as porcent
		from sispacto.identificacaousuario i
		inner join sispacto.tipoperfil t on t.iusd = i.iusd and t.pflcod in(".PFL_PROFESSORALFABETIZADOR.",".PFL_ORIENTADORESTUDO.")
		inner join sispacto.identiusucursoformacao ic on ic.iusd = i.iusd
		inner join sispacto.cursoformacao f on f.cufid = ic.cufid
		where iustermocompromisso=true and i.uncid='".$_SESSION['sispacto']['universidade']['uncid']."'
		group by f.cufcursodesc
		order by count(i.iusd) desc";
		
		$cabecalho = array("<font style=font-size:xx-small;>Formação</font>","<font style=font-size:xx-small;>Total</font>","<font style=font-size:xx-small;>%</font>");
		$db->monta_lista_simples($sql,$cabecalho,100000,5,'S','80%','center');
		
		?>
		
		</td>
	</tr>
	
	<tr>
		<td class="SubTituloDireita" width="20%">Acompanhamento dos Cursistas <img src="../imagens/ajuda.gif" align="absmiddle" onmouseover="return escape('- Procedimentos de acompanhamento dos cursistas (encontros presenciais, visitas às escolas, freqüência das visitas);<br>- Atuação dos formadores e supervisores (apoio tecnológico e/ou acompanhamento de conteúdos e atividades);<br>-	Mecanismos de monitoramento e/ou acompanhamento dos cursistas.');"></td>
		<td colspan="2"><?=campo_textarea('rlfacompanhamentocursistas', 'N', 'S', '', '150', '7', '2000'); ?></td>
	</tr>
	
	<tr>
		<td class="SubTituloDireita" width="20%">Avaliações Feitas pelos Cursistas</td>
		<td colspan="2">
		
		<? 
		
		$sql = "select i.iacdsc,
				count(distinct r.iusdavaliador) as avaliadores,
				avg(a.iccvalor) as nota
			from sispacto.respostasavaliacaocomplementar r
			inner join sispacto.identificacaousuario ii on ii.iusd = r.iusdavaliador
			inner join sispacto.tipoperfil t on t.iusd = ii.iusd
			inner join sispacto.itensavaliacaocomplementarcriterio a on a.iccid = r.iccid
			inner join sispacto.itensavaliacaocomplementar i on i.iacid = r.iacid
			inner join seguranca.perfil pp on pp.pflcod = t.pflcod
			where i.gicid=1 and ii.uncid='".$_SESSION['sispacto']['universidade']['uncid']."' and ii.iusstatus='A' AND pp.pflcod in(".PFL_PROFESSORALFABETIZADOR.",".PFL_ORIENTADORESTUDO.")
			group by i.iacdsc
			order by 3";
		
		$cabecalho = array("Quanto aos conteúdos","Número de avaliadores","Nota");
		$db->monta_lista_simples($sql,$cabecalho,100000,5,'N','100%','N');
		
		?>
		
		<? 
		
		$sql = "select i.iacdsc,
				count(distinct r.iusdavaliador) as avaliadores,
				avg(a.iccvalor) as nota
			from sispacto.respostasavaliacaocomplementar r
			inner join sispacto.identificacaousuario ii on ii.iusd = r.iusdavaliador
			inner join sispacto.tipoperfil t on t.iusd = ii.iusd
			inner join sispacto.itensavaliacaocomplementarcriterio a on a.iccid = r.iccid
			inner join sispacto.itensavaliacaocomplementar i on i.iacid = r.iacid
			inner join seguranca.perfil pp on pp.pflcod = t.pflcod
			where i.gicid in(4,6) and ii.uncid='".$_SESSION['sispacto']['universidade']['uncid']."' and ii.iusstatus='A' AND pp.pflcod in(".PFL_PROFESSORALFABETIZADOR.",".PFL_ORIENTADORESTUDO.")
			group by i.iacdsc
			order by 3";
		
		$cabecalho = array("Autoavaliação","Número de avaliadores","Nota");
		$db->monta_lista_simples($sql,$cabecalho,100000,5,'N','100%','N');
		
		?>
		
		</td>
	</tr>
	<tr>
		<td class="SubTituloEsquerda" colspan="3">5. BOLSAS PAGAS POR PERFIS</td>
	</tr>
	
	<tr>
		<td colspan="3">
		<? 
		$sql = "SELECT pp.pfldsc, count(*) as numbolsas, sum(pbovlrpagamento) as valorpago FROM sispacto.pagamentobolsista p 
				INNER JOIN sispacto.universidadecadastro u ON u.uniid = p.uniid 
				INNER JOIN seguranca.perfil pp ON pp.pflcod = p.pflcod 
				INNER JOIN workflow.documento d ON d.docid = p.docid 
				WHERE u.uncid='".$_SESSION['sispacto']['universidade']['uncid']."' AND d.esdid=".ESD_PAGAMENTO_EFETIVADO." 
				GROUP BY pp.pfldsc";
		
		$cabecalho = array("Perfil","Número de bolsas","Valor pago(R$)");
		$db->monta_lista_simples($sql,$cabecalho,100000,5,'S','100%','N');
		
		?>
		</td>
	</tr>	
	<tr>
		<td class="SubTituloEsquerda" colspan="3">6. ANÁLISE CRÍTICA</td>
	</tr>
	
	<tr>
		<td class="SubTituloDireita" width="20%">Sobre o Conteúdo do Curso <img src="../imagens/ajuda.gif" align="absmiddle" onmouseover="return escape('Analisar sinteticamente conteúdos que deveriam ser revisados, justificando a proposição.');"></td>
		<td colspan="2"><?=campo_textarea('rlfconteudocurso', 'N', 'S', '', '150', '7', '2000'); ?></td>
	</tr>
	
	<tr>
		<td class="SubTituloDireita" width="20%">Sobre a metologia <img src="../imagens/ajuda.gif" align="absmiddle" onmouseover="return escape('Analisar sinteticamente a metodologia de trabalho, indicando pontos de melhoria, se for o caso.');"></td>
		<td colspan="2"><?=campo_textarea('rlfmetodologia', 'N', 'S', '', '150', '7', '2000'); ?></td>
	</tr>
	
	<tr>
		<td class="SubTituloDireita" width="20%">Sobre os Critérios de Avaliações <img src="../imagens/ajuda.gif" align="absmiddle" onmouseover="return escape('Avaliar a conveniência e adequação dos critérios de avaliação adotados pelo MEC: Frequência, Atividades realizadas e Monitoramento.');"></td>
		<td colspan="2"><?=campo_textarea('rlfcriteriosavaliacoes', 'N', 'S', '', '150', '7', '2000'); ?></td>
	</tr>
	
	<tr>
		<td colspan="3">
		<b>Sobre a Equipe Pedagógica</b>
		
		<br><br>
		
		<? 
		
		$sql = "select 
				'<font style=font-size:xx-small;>'||i.iusnome||'</font>' as iusnome, 
				'<font style=font-size:xx-small;>'||p.pfldsc||'</font>' as pfldsc, 
				'<font style=font-size:xx-small;float:right;>'||count(distinct ma.mavid)||'</font>' as avaliacoes, 
				'<font style=font-size:xx-small;float:right;>'||round(avg(ma.mavtotal),2)||'</font>' as total
				from sispacto.identificacaousuario i 
				inner join sispacto.tipoperfil t on t.iusd = i.iusd 
				inner join seguranca.perfil p on p.pflcod = t.pflcod 
				inner join sispacto.mensario me on me.iusd = i.iusd 
				inner join sispacto.mensarioavaliacoes ma on ma.menid = me.menid
				where t.pflcod in(".PFL_FORMADORIES.",".PFL_SUPERVISORIES.",".PFL_COORDENADORADJUNTOIES.",".PFL_COORDENADORIES.") and i.uncid='".$_SESSION['sispacto']['universidade']['uncid']."' 
				group by i.iusnome, p.pfldsc
				order by p.pfldsc, i.iusnome";
		
		
		$cabecalho = array("<font style=font-size:xx-small;>Nome</font>","<font style=font-size:xx-small;>Perfil</font>","<font style=font-size:xx-small;>Número de avaliações</font>","<font style=font-size:xx-small;>Nota Final</font>");
		$db->monta_lista_simples($sql,$cabecalho,100000,5,'N','95%','center');
		
		?>
		
		<br>
		
		<b>Comentários</b>
		
		<br>
		
		<?=campo_textarea('rlfequipepedagogica', 'N', 'S', '', '150', '7', '2000'); ?>
		
		</td>
	</tr>
	
	<tr>
		<td class="SubTituloDireita" width="20%">Sobre a Articulação com o MEC <img src="../imagens/ajuda.gif" align="absmiddle" onmouseover="return escape('Avaliar articulação institucional entre o MEC e a Universidade contemplando no mínimo os seguintes aspectos:<br>- Tempestividade nas respostas<br>- Agilidade no pagamento de bolsas<br>- Qualidade do suporte tecnológico<br>- Liberação de recursos.');"></td>
		<td colspan="2"><?=campo_textarea('rlfarticulacaomec', 'N', 'S', '', '150', '7', '2000'); ?></td>
	</tr>
	
	<tr>
		<td class="SubTituloDireita" width="20%">Lições Aprendidas <img src="../imagens/ajuda.gif" align="absmiddle" onmouseover="return escape('Descrever eventos/ situações/ experiências que contribuem para o aperfeiçoamento do programa e do curso.');"></td>
		<td colspan="2"><?=campo_textarea('rlflicoesaprendidas', 'N', 'S', '', '150', '7', '2000'); ?></td>
	</tr>
	
	<tr>
		<td class="SubTituloEsquerda" colspan="3">6. INDICADORES E COMENTÁRIOS FINAIS</td>
	</tr>
	
	<tr>
		<td colspan="3">
		
		<?
		
		$orientadorestudo = totalAlfabetizadoresAbrangencia(array("uncid" => $_SESSION['sispacto']['universidade']['uncid']));
		
		$sql = "SELECT SUM(total) FROM sispacto.totalalfabetizadores t 
				INNER JOIN sispacto.abrangencia ap ON ap.muncod = t.cod_municipio
		 		INNER JOIN sispacto.estruturacurso e ON e.ecuid = ap.ecuid 
				WHERE e.uncid='".$_SESSION['sispacto']['universidade']['uncid']."'";
		
		$professoralfabetizador = $db->pegaUm($sql);
		
		
		///////////////////////
		
		
		$sql = "SELECT count(*) as total FROM sispacto.identificacaousuario i 
				INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd 
				WHERE t.pflcod='".PFL_ORIENTADORESTUDO."' AND i.iusstatus='A' AND i.uncid='".$_SESSION['sispacto']['universidade']['uncid']."' AND i.iuscpf NOT LIKE 'SIS%'";
		
		$numorientadorestudo = $db->pegaUm($sql);
		
		$sql = "SELECT count(*) as total FROM sispacto.identificacaousuario i
				INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd
				WHERE t.pflcod='".PFL_PROFESSORALFABETIZADOR."' AND i.iusstatus='A' AND i.uncid='".$_SESSION['sispacto']['universidade']['uncid']."'";
		
		$numprofessoralfabetizador = $db->pegaUm($sql);
		
		
		$sql = "SELECT count(*) as total FROM sispacto.mensario m 
				INNER JOIN sispacto.mensarioavaliacoes ma ON ma.menid = m.menid 
				INNER JOIN sispacto.identificacaousuario i ON i.iusd = m.iusd 
				INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd 
				WHERE t.pflcod='".PFL_ORIENTADORESTUDO."' AND i.uncid='".$_SESSION['sispacto']['universidade']['uncid']."' AND mavrecomendadocertificacao='1'";
		
		$numorientadorestudocert = $db->pegaUm($sql);
		
		$sql = "SELECT count(*) as total FROM sispacto.mensario m
				INNER JOIN sispacto.mensarioavaliacoes ma ON ma.menid = m.menid
				INNER JOIN sispacto.identificacaousuario i ON i.iusd = m.iusd
				INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd
				WHERE t.pflcod='".PFL_PROFESSORALFABETIZADOR."' AND i.uncid='".$_SESSION['sispacto']['universidade']['uncid']."' AND mavrecomendadocertificacao='1'";
		
		$numprofessoralfabetizadorcert = $db->pegaUm($sql);
		
		
		$sql = "SELECT count(*) as total FROM sispacto.historicotrocausuario WHERE pflcod='".PFL_ORIENTADORESTUDO."' AND uncid='".$_SESSION['sispacto']['universidade']['uncid']."' AND hstacao='R'";
		$numorientadorestudoevasao = $db->pegaUm($sql);
		
		$sql = "SELECT count(*) as total FROM sispacto.historicotrocausuario WHERE pflcod='".PFL_PROFESSORALFABETIZADOR."' AND uncid='".$_SESSION['sispacto']['universidade']['uncid']."' AND hstacao='R'";
		$numprofessoralfabetizadorevasao = $db->pegaUm($sql);
		
		$sql = "SELECT count(*) as total FROM (
				SELECT i.iusd,
				(select count(*) from sispacto.mensario m inner join sispacto.mensarioavaliacoes ma on ma.menid = m.menid where (ma.mavfrequencia=0 or ma.mavfrequencia is null) and m.fpbid=8 and m.iusd = i.iusd) as out2013,
				(select count(*) from sispacto.mensario m inner join sispacto.mensarioavaliacoes ma on ma.menid = m.menid where (ma.mavfrequencia=0 or ma.mavfrequencia is null) and m.fpbid=9 and m.iusd = i.iusd) as nov2013,
				(select count(*) from sispacto.mensario m inner join sispacto.mensarioavaliacoes ma on ma.menid = m.menid where (ma.mavfrequencia=0 or ma.mavfrequencia is null) and m.fpbid=10 and m.iusd = i.iusd) as dez2013
				FROM sispacto.identificacaousuario i 
				INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd 
				WHERE t.pflcod='".PFL_PROFESSORALFABETIZADOR."' AND i.iusstatus='A' AND i.uncid='".$_SESSION['sispacto']['universidade']['uncid']."' ) foo 
				WHERE out2013>0 AND nov2013>0 AND dez2013>0";
		
		$numprofessoralfabetizadorabandono = $db->pegaUm($sql);
		
		
		$sql = "SELECT count(*) as total FROM (
				SELECT i.iusd,
				(select count(*) from sispacto.mensario m inner join sispacto.mensarioavaliacoes ma on ma.menid = m.menid where (ma.mavfrequencia=0 or ma.mavfrequencia is null) and m.fpbid=8 and m.iusd = i.iusd) as out2013,
				(select count(*) from sispacto.mensario m inner join sispacto.mensarioavaliacoes ma on ma.menid = m.menid where (ma.mavfrequencia=0 or ma.mavfrequencia is null) and m.fpbid=9 and m.iusd = i.iusd) as nov2013,
				(select count(*) from sispacto.mensario m inner join sispacto.mensarioavaliacoes ma on ma.menid = m.menid where (ma.mavfrequencia=0 or ma.mavfrequencia is null) and m.fpbid=10 and m.iusd = i.iusd) as dez2013
				FROM sispacto.identificacaousuario i
				INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd
				WHERE t.pflcod='".PFL_ORIENTADORESTUDO."' AND i.iusstatus='A' AND i.uncid='".$_SESSION['sispacto']['universidade']['uncid']."' AND i.iuscpf NOT LIKE 'SIS%' ) foo
				WHERE out2013>0 AND nov2013>0 AND dez2013>0";
		
		$numorientadorestudoabandono = $db->pegaUm($sql);
		
		//////////////////////////
		
		?>
		
		<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
			<tr>
				<td class="SubTituloCentro">Perfil</td>
				<td class="SubTituloCentro">Meta Pactuada</td>
				<td class="SubTituloCentro">Cursos Matriculados</td>
				<td class="SubTituloCentro">Recomendados Certificação</td>
				<td class="SubTituloCentro">Evasão</td>
				<td class="SubTituloCentro">Abandono</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" width="30%">Orientador de Estudo</td>
				<td align="right"><?=(($orientadorestudo)?round($orientadorestudo*0.8):"0") ?></td>
				<td align="right"><?=(($numorientadorestudo)?round($numorientadorestudo):"0") ?></td>
				<td align="right"><?=(($numorientadorestudocert)?round($numorientadorestudocert):"0") ?></td>
				<td align="right"><?=(($numorientadorestudoevasao)?round($numorientadorestudoevasao):"0") ?></td>
				<td align="right"><?=(($numorientadorestudoabandono)?round($numorientadorestudoabandono):"0") ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita" width="30%">Professor alfabetizador</td>
				<td align="right"><?=(($professoralfabetizador)?round($professoralfabetizador*0.6):"0") ?></td>
				<td align="right"><?=(($numprofessoralfabetizador)?round($numprofessoralfabetizador):"0") ?></td>
				<td align="right"><?=(($numprofessoralfabetizadorcert)?round($numprofessoralfabetizadorcert):"0") ?></td>
				<td align="right"><?=(($numprofessoralfabetizadorevasao)?round($numprofessoralfabetizadorevasao):"0") ?></td>
				<td align="right"><?=(($numprofessoralfabetizadorabandono)?round($numprofessoralfabetizadorabandono):"0") ?></td>
			</tr>
		</table>
		
		</td>
	</tr>
	
	<tr>
		<td class="SubTituloDireita" width="20%">Sugestões</td>
		<td colspan="2"><?=campo_textarea('rlfsugestoes', 'N', 'S', '', '150', '7', '2000'); ?></td>
	</tr>
	
	<tr>
		<td class="SubTituloDireita" width="20%">Outros Comentários <img src="../imagens/ajuda.gif" align="absmiddle" onmouseover="return escape('Inserir informações não contempladas neste relatório consideradas relevantes para o aperfeiçoamento do programa e do curso.');"></td>
		<td colspan="2"><?=campo_textarea('rlfoutroscomentarios', 'N', 'S', '', '150', '7', '2000'); ?></td>
	</tr>
<!-- 
	<tr>
		<td colspan="2">
		
<p><i>Declaramos que as informações constantes deste Relatório Final expressam a avaliação feita por toda a equipe gestora e traduzem a situação atual de andamento do curso de formação continuada no âmbito do Pacto Nacional pela Alfabetização na Idade Certa.</i></p>

<br>

<p><b>(Município, data)</b></p>

<br>
<br>

<p align="center">_______________________________<br><br>
Coordenador(a) Geral do curso</p>
		
		</td>
	</tr>
 -->
	
	<tr>
		<td class="SubTituloDireita" width="20%">&nbsp;</td>
		<td colspan="2">
			<input type="button" value="Anterior" onclick="divCarregando();window.location='sispacto.php?modulo=principal/universidade/universidadeexecucao&acao=A&aba=orcamentoexecucao';">
			<input type="button" value="Salvar" id="salvar" onclick="atualizarRelatorioFinal();">
		</td>
	</tr>
</table>
</form>