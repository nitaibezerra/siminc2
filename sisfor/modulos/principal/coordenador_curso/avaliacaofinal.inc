<?

function carregarPlanoAtividadesAF($dados) {
	global $db;

	$sql = "SELECT * FROM sisfor.atividadescurso a
			WHERE a.sifid='" . $dados['sifid'] . "' AND atiidpai IS NULL ORDER BY a.atiid";

	$atividades = $db->carregar($sql);

	echo "<table class=\"listagem\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\" width=\"100%\">";
	echo "<tr>";
	echo "<td width=\"50%\" class=\"SubTituloCentro\">ATIVIDADES / SUBATIVIDADES</td>
		  <td class=\"SubTituloCentro\" colspan=\"2\">PERÍODO DE EXECUÇÃO PACTUADO</td>
		  <td class=\"SubTituloCentro\" colspan=\"2\">PERÍODO EXECUTADOO</td>";
	echo "</tr>";

	echo "<tr>";
	echo "<td width=\"50%\" align=\"center\">&nbsp;</td><td align=\"center\">Início</td><td align=\"center\">Término</td>";
	echo "</tr>";


	if ($atividades[0]) {

		foreach ($atividades as $atidesc => $at) {

			echo "<tr>";
			echo "<td width=\"50%\" colspan=5><b><img src=../imagens/seta_filho.gif>  " . $at['atidesc'] . "</b></td>";
			echo "</tr>";

			$sql = "SELECT * FROM sisfor.atividadescurso a
					WHERE a.atiidpai='" . $at['atiid'] . "' ORDER BY a.atiid";

			$subatividades = $db->carregar($sql);

			if ($subatividades[0]) {
				foreach ($subatividades as $su) {
					echo "<tr>";
					echo "<td width=\"50%\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src=../imagens/seta_filho.gif> " . $su['atidesc'] . "</td>";
					echo "<td align=\"center\">" . formata_data($su['atidatainicio']) . "</td>";
					echo "<td align=\"center\">" . formata_data($su['atidatafim']) . "</td>";
					echo "<td>" . campo_data2('atidatainicioexec[' . $su['atiid'] . ']', 'S', (($dados['consulta'])?'N':'S'), 'Inicío', 'S', '', '', $su['atidatainicioexec'], '', '', 'atidatainicioexec' . $su['atiid']) . "</td>";
					echo "<td>" . campo_data2('atidatafimexec[' . $su['atiid'] . ']', 'S', (($dados['consulta'])?'N':'S'), 'Fim', 'S', '', '', $su['atidatafimexec'], '', '', 'atidatafimexec' . $su['atiid']) . "</td>";
					echo "</tr>";
				}
			}
			
		}
	} else {
		echo "<tr>";
		echo "<td class=\"SubTituloEsquerda\" colspan=3>Nenhuma Subatividade foi cadastrada</td>";
		echo "</tr>";
	}

	echo "</table>";
}

global $afcestruturafisicasuporte;

$avaliacaofinal = $db->pegaLinha("SELECT * FROM sisfor.avaliacaofinal WHERE sifid='".$_SESSION['sisfor']['sifid']."'");
if($avaliacaofinal) extract($avaliacaofinal);

$titulo 		 = 'Relatório final';


?>
<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>

<form method="post" id="formulario" enctype="multipart/form-data">
<input type="hidden" name="requisicao" value="gravarAvaliacaoFinalCurso">



<div id="modalFormulario" style="display:none;">
<script>
function mostrarMunicipiosAbr() {

    jQuery("#modalFormulario").dialog({
        draggable: true,
        resizable: true,
        width: 800,
        height: 600,
        modal: true,
        close: function() {
        }
    });


}

function inserirMunicipioAbrangencia() {
	
	if(jQuery('#estuf_endereco').val()=='') {
		alert('Selecione um estado');
		return false;
	}
	
	if(jQuery('#muncod_endereco').val()=='') {
		alert('Selecione um município');
		return false;
	}

	
	ajaxatualizar('requisicao=inserirMunicipioAbrangencia&sifid=<?=$_SESSION['sisfor']['sifid'] ?>&muncod='+jQuery('#muncod_endereco').val(),'');
	jQuery('#formulario').submit();
}
</script>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloDireita" width="25%">Orientações</td>
    <td>[ORIENTAÇÕES]</td>
</tr>
<tr>
	<td class="SubTituloDireita">UF</td>
    <td>
    <?
    $sql = "SELECT estuf as codigo, estuf as descricao FROM territorios.estado ORDER BY estuf";
    $db->monta_combo('estuf_endereco', $sql, 'S', 'Selecione', 'carregarMunicipiosPorUF3', '', '', '', 'S', 'estuf_endereco', '');
    ?>
    </td>
</tr>
<tr>
	<td class="SubTituloDireita">Município</td>
    <td id="td_municipio3"></td>
</tr>
<tr>
	<td class="SubTituloCentro" colspan="2">
    <input type="button" name="salvar" value="Salvar" onclick="inserirMunicipioAbrangencia();">
    </td>
</tr>
</table>
</div>
	
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloEsquerda" colspan="2"><span style=font-size:large;>1. Dados gerais</span></td>
	<td valign="top" rowspan="2" width="3%">
	<? 
	/* Barra de estado atual e ações e Historico */
	if($docid) {

		$estado = wf_pegarEstadoAtual( $docid );
		
		if($estado['esdid'] != ESD_RELATORIOFINAL_EMELABORACAO) {
			$consulta = true;
		}

		wf_desenhaBarraNavegacao( $docid, array() );
		
	}			
	?>
    </td>
</tr>
	
<tr>
	<td colspan="2">
	<?
	montaCabecalhoCoordenadorCurso();
	?>
	</td>

</tr>
<tr>
	<td class="SubTituloEsquerda" colspan="3">Equipe IES</td>
</tr>
<tr>
	<td colspan="3">
	<?
	
	$sql = "SELECT i.iusnome, pe.pfldsc, fe.foedesc, tp.tvpdsc, (SELECT count(DISTINCT m.menid) FROM sisfor.mensario m WHERE m.tpeid = t.tpeid ) as totalavaliacoes FROM sisfor.identificacaousuario i 
			INNER JOIN sisfor.tipoperfil t ON t.iusd = i.iusd AND t.pflcod IN(".PFL_COORDENADOR_CURSO.",".PFL_COORDENADOR_ADJUNTO_IES.",".PFL_SUPERVISOR_IES.",".PFL_FORMADOR_IES.",".PFL_PROFESSOR_PESQUISADOR.",".PFL_TUTOR.")
			LEFT JOIN seguranca.perfil pe ON pe.pflcod = t.pflcod 
			LEFT JOIN public.tipovinculoprofissional tp ON tp.tvpid = i.tvpid 
			LEFT JOIN sisfor.formacaoescolaridade fe ON fe.foeid = i.foeid 
			WHERE i.iusstatus='A' AND t.sifid='".$_SESSION['sisfor']['sifid']."' ORDER BY pe.pflnivel, pe.pfldsc, i.iusnome";
	
	$cabecalho = array("Nome","Função","Titulação","Vínculo","Número de avaliações");
	$db->monta_lista_simples($sql,$cabecalho,100000,5,'N','100%','',true, false, false, true);
	
	$sql = "SELECT pe.pfldsc, count(*) as totalqtd FROM sisfor.identificacaousuario i
			INNER JOIN sisfor.tipoperfil t ON t.iusd = i.iusd AND t.pflcod IN(".PFL_COORDENADOR_CURSO.",".PFL_COORDENADOR_ADJUNTO_IES.",".PFL_SUPERVISOR_IES.",".PFL_FORMADOR_IES.",".PFL_PROFESSOR_PESQUISADOR.",".PFL_TUTOR.")
			INNER JOIN seguranca.perfil pe ON pe.pflcod = t.pflcod
			WHERE i.iusstatus='A' AND t.sifid='".$_SESSION['sisfor']['sifid']."' 
			GROUP BY pe.pflnivel, pe.pfldsc			
			ORDER BY pe.pflnivel, pe.pfldsc";
	
	$cabecalho = array("Perfil","Quantitativos");
	$db->monta_lista_simples($sql,$cabecalho,100000,5,'N','100%','',false, false, false, false);
	
	?>
	</td>
</tr>
	
<tr>
	<td class="SubTituloEsquerda" colspan="3">Período de execução do projeto</td>
</tr>
<?	
$data_validada_mec = $db->pegaUm("SELECT to_char(sifvigenciadtini,'dd/mm/YYYY')||' até '||to_char(sifvigenciadtfim,'dd/mm/YYYY') as periodo FROM sisfor.sisfor WHERE sifid='".$_SESSION['sisfor']['sifid']."'");
?>
<tr>
	<td class="SubTituloDireita">Validado pelo MEC:</td><td><?=$data_validada_mec ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Executado pela IES:</td>
	<td>
	<?
	echo campo_data2('afcexecucacaoprojetoini','S', (($consulta)?'N':'S'), 'Data início', 'S', '', '', $afcexecucacaoprojetoini, '', '', 'afcexecucacaoprojetoini');
	echo 'até'; 
	echo campo_data2('afcexecucacaoprojetofim','S', (($consulta)?'N':'S'), 'Data fim', 'S', '', '', $afcexecucacaoprojetofim, '', '', 'afcexecucacaoprojetofim');
	?>
	</td>
</tr>
<tr>
	<td class="SubTituloEsquerda" colspan="3">Plano de atividades</td>
</tr>
	
<tr>
	<td colspan="3">
	<?
	carregarPlanoAtividadesAF(array("sifid" => $_SESSION['sisfor']['sifid'], "consulta" => $consulta));
	?>
	<br>
	<p><b>Justificativa da mudança do PERÍODO DE EXECUÇÃO para PERÍODO EXECUTADO</b></p>
	<?
	if($consulta) {
		echo nl2br($afcjustificativamudancaplano);
	} else {
		echo campo_textarea( 'afcjustificativamudancaplano', 'S', 'S', '', '85', '6', '3000');
	}
	?>
	</td>
</tr>
</table>
	
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloEsquerda" colspan="2"><span style=font-size:large;>2. Organização do curso</span></td>
</tr>
<tr>
	<td class="SubTituloEsquerda" colspan="2">Municípios atendidos</td>
</tr>
<tr>
	<td colspan="2">
	<?
	$sql = "SELECT '<input ".(($consulta)?"disabled":"")." type=checkbox name=\"abrmuncod[]\" value=\"'||m.muncod||'\" '||CASE WHEN abrconfirmaatendimento=true THEN 'checked' ELSE '' END||'>' as acao, m.estuf||' / '||m.mundescricao as municipio FROM sisfor.abrangenciacurso a 
			INNER JOIN territorios.municipio m ON m.muncod = a.muncod 
			WHERE a.sifid='".$_SESSION['sisfor']['sifid']."' GROUP BY m.estuf, m.mundescricao, m.muncod, a.abrconfirmaatendimento  ORDER BY m.estuf, m.mundescricao";
	
	$cabecalho = array("&nbsp;","UF / Município");
	$db->monta_lista_simples($sql,$cabecalho,100000,5,'N','100%','',false, false, false, false);
	
	?>
	
	<? if(!$consulta): ?>
	<p align="center"><input type="button" name="inserirmunicipio" value="Inserir município" onclick="mostrarMunicipiosAbr();"></p>
	<? endif; ?>
	
	</td>
</tr>
<tr>
	<td class="SubTituloEsquerda" colspan="2">Planejamento Pedagógico do Curso <img src="../imagens/ajuda.gif" align="absmiddle" onmouseover="return escape('Descrever a organização do processo formativo da equipe (os processos de discussão da proposta teórico-metodológica do curso, os agentes envolvidos nessa discussão, tramitação e indicadores da proposta aprovada na Universidade/Secretaria de Educação, sistemática de reuniões para o planejamento, definição de conteúdos e material didático complementar, estratégias pedagógicas e de avaliação etc.)');"></td>
</tr>
<tr>
	<td colspan="2">
	<?
	if($consulta) {
		echo nl2br($afcplanejamentopedagogico);
	} else {
		echo campo_textarea( 'afcplanejamentopedagogico', 'S', 'S', '', '85', '6', '3000');
	}
	?>
	</td>
</tr>
<tr>
	<td class="SubTituloEsquerda" colspan="2">Organização Pedagógica do Curso <img src="../imagens/ajuda.gif" align="absmiddle" onmouseover="return escape('Descrever periocidade das reuniões dos coordenadores, supervisores e formadores, encaminhamentos acadêmicos no que diz respeito à participação nas atividades, à recuperação de atividades etc.');"></td>
</tr>
<tr>
	<td colspan="2">
	<?
	if($consulta) {
		echo nl2br($afcorganizacaopedagogica);
	} else {
		echo campo_textarea( 'afcorganizacaopedagogica', 'S', 'S', '', '85', '6', '3000');
	}
	?>
	</td>
</tr>
<tr>
	<td class="SubTituloEsquerda" colspan="2">Estrutura Física e Suporte</td>
</tr>
<tr>
	<td colspan="2">
	<p>Selecione as instalações e os equipamentos utilizados durante os encontros de formação com os Formadores Regionais.</p>
	<?
	
	$afcestruturafisicasuporte = explode(";",trim($afcestruturafisicasuporte));
	if(!$afcestruturafisicasuporte) $afcestruturafisicasuporte = array();
	
	?>
	<table width=100%>
	<tr><td><input <?=(($consulta)?"disabled":"") ?> type=checkbox name="afcestruturafisicasuporte[]" value="1" <?=((in_array('1',$afcestruturafisicasuporte))?'checked':'')?> ></td><td>Sala de aula da IES</td><td><input <?=(($consulta)?"disabled":"") ?> type=checkbox name="afcestruturafisicasuporte[]" value="4" <?=((in_array('4',$afcestruturafisicasuporte))?'checked':'')?>></td><td>Equipamentos de som e imagem</td>
	<tr><td><input <?=(($consulta)?"disabled":"") ?> type=checkbox name="afcestruturafisicasuporte[]" value="2" <?=((in_array('2',$afcestruturafisicasuporte))?'checked':'')?>></td><td>Sala de aula de escola pública</td><td><input <?=(($consulta)?"disabled":"") ?> type=checkbox name="afcestruturafisicasuporte[]" value="5" <?=((in_array('5',$afcestruturafisicasuporte))?'checked':'')?>></td><td>Equipamentos de informática</td>
	<tr><td><input <?=(($consulta)?"disabled":"") ?> type=checkbox name="afcestruturafisicasuporte[]" value="3" <?=((in_array('3',$afcestruturafisicasuporte))?'checked':'')?>></td><td>Locação de espaço</td><td><input <?=(($consulta)?"disabled":"") ?> type=checkbox name="afcestruturafisicasuporte[]" value="6" <?=((in_array('6',$afcestruturafisicasuporte))?'checked':'')?>></td><td>Outros materiais pedagógicos</td>
	</table>
	
	</td>
	
</tr>
	
<tr>
	<td class="SubTituloEsquerda" colspan="2">Encontros Presenciais</td>
</tr>
<script>

function gravarEncontroPresencial() {
	if(jQuery('#aofnome').val()==''){alert('Preencha o nome do evento');return false;}
	if(jQuery('#aofdata').val()==''){alert('Preencha a data do evento');return false;}
	if(jQuery('#aofcargahoraria').val()==''){alert('Preencha a carga horária');return false;}
	ajaxatualizar('requisicao=gravarEncontroPresencial&aofnome='+jQuery('#aofnome').val()+'&aofdata='+jQuery('#aofdata').val()+'&aofcargahoraria='+jQuery('#aofcargahoraria').val(),'');
	ajaxatualizar('requisicao=carregarEncontroPresencial','td_encontropresencial');
}
	
function removerEncontroPresencial(aofid) {
	var conf = confirm('Deseja realmente remover o encontro presencial?');
	if(conf) {
		ajaxatualizar('requisicao=removerEncontroPresencialCoordenador&aofid='+aofid,'');
		ajaxatualizar('requisicao=carregarEncontroPresencial','td_encontropresencial');
	}
}
</script>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<? if(!$consulta) : ?>
<tr>
	<td class="SubTituloDireita">Atividade realizada:</td>
	<td><?=campo_texto('aofnome', "N", "S", "Nome do evento", 30, 60, "", "", '', '', 0, 'id="aofnome"') ?></td><td class="SubTituloDireita">Data da atividade realizada:</td><td><?=campo_data2('aofdata','S', 'S', 'Data do evento', 'S', '', '', '', '', '', 'aofdata') ?></td><td class="SubTituloDireita">Carga horária:</td><td><?=campo_texto('aofcargahoraria', "N", "S", "Carga horária", 6, 10, "######", "", '', '', 0, 'id="aofcargahoraria"') ?></td>
</tr>
<tr>
	<td class="SubTitulocentro" colspan="6"><input type="button" name="gravar" value="Inserir Evento" onclick="gravarEncontroPresencial();"></td>
</tr>
<? endif; ?>
<tr>
	<td colspan="6" id="td_encontropresencial">
	<?
	carregarEncontroPresencial(array("consulta" => $consulta));
	?>
	</td>
</tr>
</table>

<script>

function calcularOrcamentoExecucao() {

	var totalvalorexecutado  = 0;
	var totalsaldo           = 0;
	
	jQuery("[id^='valorprevisto_']").each(function() {
	
		var orcid = replaceAll(jQuery(this).attr('id'),'valorprevisto_','');
		
		var valorprevisto  = parseFloat(replaceAll(replaceAll(jQuery('#valorprevisto_'+orcid).val(),'.',''),',','.'));
		
		var valorexecutado = 0;
		if(jQuery('#valorexecutado_'+orcid).val()!='') {
			valorexecutado = parseFloat(replaceAll(replaceAll(jQuery('#valorexecutado_'+orcid).val(),'.',''),',','.'));
		}

		var porc = (valorexecutado/valorprevisto)*100;
		jQuery('#porcvalorexecutado_'+orcid).val(porc.toFixed(1));

		var saldo          = valorprevisto-valorexecutado;
		
		if(saldo < 0) {
			jQuery('#saldo_'+orcid).val('-'+mascaraglobal('###.###.###,##',saldo.toFixed(2)));
		} else {
			jQuery('#saldo_'+orcid).val(mascaraglobal('###.###.###,##',saldo.toFixed(2)));
		}
		
		totalvalorexecutado  += valorexecutado;
		totalsaldo           += saldo;

	});
	
	jQuery('#totalvalorexecutado').val(mascaraglobal('###.###.###,##',totalvalorexecutado.toFixed(2)));
	
	var totalvalorprevisto = parseFloat(replaceAll(replaceAll(jQuery('#totalvalorprevisto').val(),'.',''),',','.'));

	var porc2 = (totalvalorexecutado/totalvalorprevisto)*100;
	jQuery('#porctotalvalorexecutado').val(porc2.toFixed(1));
	
	
	if(totalsaldo < 0) {
		jQuery('#totalsaldo').val('-'+mascaraglobal('###.###.###,##',totalsaldo.toFixed(2)));
	} else {
		jQuery('#totalsaldo').val(mascaraglobal('###.###.###,##',totalsaldo.toFixed(2)));
	}
	
}
</script>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloEsquerda" colspan="2"><span style=font-size:large;>3. Execução financeira</span></td>
</tr>
<?	
$custoaluno = $db->pegaLinha("SELECT 
									to_char(round(((SELECT SUM(o.orcvlrunitario) FROM sisfor.orcamento o WHERE o.sifid=s.sifid AND o.orcstatus='A')/sifprofmagisterio),2),'999g999g999d99') as previsto,
								    to_char(round(((SELECT COALESCE(SUM(o.orcvlrexecutado),0.00) FROM sisfor.orcamento o WHERE o.sifid=s.sifid AND o.orcstatus='A')/sifprofmagisterio),2),'999g999g999d99') as executado  
							  FROM sisfor.sisfor s WHERE sifid='".$_SESSION['sisfor']['sifid']."'");
?>	
<tr>
	<td class="SubTituloDireita" width="25%">Custo aluno previsto (custeio)</td>
	<td>R$ <?=$custoaluno['previsto'] ?></td>
</tr>
<tr>
	<td class="SubTituloDireita" width="25%">Custo aluno final (custeio)</td>
	<td>R$ <?=$custoaluno['executado'] ?></td>
</tr>
<tr>
	<td colspan="2">
	<?
	carregarListaCustos(array('sifid' => $_SESSION['sisfor']['sifid'],'execucao'=> true,'consulta'=> $consulta));
	?>
	</td>
</tr>
</table>
	
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloEsquerda" colspan="2"><span style=font-size:large;>4. Situação das Bolsas</span></td>
</tr>
	
<tr>
	<td colspan="2">
	<?
	
	$sql = "SELECT foo.pfldsc, foo.epiqtd, foo.tpeqtdbolsa, foo.qtdautorizadas, foo.qtdpagas, '<span style=float:right>'||case when foo.qtdautorizadas>0 then round((foo.qtdpagas::numeric/foo.qtdautorizadas::numeric)*100,1) else 0.0 end||'</span>' as porcentagem, foo.valorpagos FROM (
			SELECT  pe.pfldsc, 
				es.epiqtd, 
				SUM(t.tpeqtdbolsa) as tpeqtdbolsa, 
				(SELECT count(*) FROM sisfor.pagamentobolsista pp INNER JOIN sisfor.tipoperfil tt ON tt.tpeid = pp.tpeid WHERE tt.sifid=es.sifid AND pp.pflcod=pe.pflcod) as qtdautorizadas,
				(SELECT count(*) FROM sisfor.pagamentobolsista pp INNER JOIN sisfor.tipoperfil tt ON tt.tpeid = pp.tpeid INNER JOIN workflow.documento d ON d.docid = pp.docid WHERE tt.sifid=es.sifid AND pp.pflcod=pe.pflcod AND d.esdid=".ESD_PAGAMENTO_EFETIVADO.") as qtdpagas,
		  		(SELECT round(sum(pbovlrpagamento),2) as valor  FROM sisfor.pagamentobolsista pp INNER JOIN sisfor.tipoperfil tt ON tt.tpeid = pp.tpeid INNER JOIN workflow.documento d ON d.docid = pp.docid WHERE tt.sifid=es.sifid AND pp.pflcod=pe.pflcod AND d.esdid=".ESD_PAGAMENTO_EFETIVADO.") as valorpagos
			FROM sisfor.equipeies es
			INNER JOIN seguranca.perfil pe ON pe.pflcod = es.pflcod 
			INNER JOIN sisfor.tipoperfil t ON t.sifid = es.sifid AND t.pflcod = es.pflcod  
			WHERE es.sifid=".$_SESSION['sisfor']['sifid']." AND epiqtd!=0 
			GROUP BY pe.pfldsc, es.epiqtd, es.sifid, pe.pflcod, pe.pflnivel 
			ORDER BY pe.pflnivel
			) foo";
	
	$cabecalho = array("Perfil","Número de bolsas previstas","Número de bolsas destinadas","Número de bolsas autorizadas","Número de bolsas pagas","% Número de bolsas pagas","Valor pago (R$)");
	$db->monta_lista_simples($sql,$cabecalho,100000,5,'S','100%','',true, false, false, true);
	?>
	</td>
</tr>
	
</table>
<?	
$meta = $db->pegaUm("SELECT sifprofmagisterio FROM sisfor.sisfor WHERE sifid='".$_SESSION['sisfor']['sifid']."'");

$inseridos = $db->pegaUm("SELECT count(distinct curid) FROM sisfor.cursistacurso c 
							 INNER JOIN sisfor.cursistaavaliacoes ca ON ca.cucid = c.cucid 
							 WHERE c.sifid='".$_SESSION['sisfor']['sifid']."'");

$aprovados = $db->pegaUm("SELECT count(distinct c.curid) FROM sisfor.cursistacurso c 
						  INNER JOIN sisfor.cursistaavaliacoes ca ON ca.cucid = c.cucid 
						  WHERE c.sifid='".$_SESSION['sisfor']['sifid']."' AND cavsituacao='p' AND c.fpbid IN(

							SELECT f.fpbid FROM sisfor.folhapagamentocursista f 
							INNER JOIN sisfor.folhapagamento fp ON fp.fpbid = f.fpbid 
							WHERE f.sifid=c.sifid AND to_char(NOW(),'YYYY-mm-dd')>=to_char((fpbanoreferencia::text||'-'||lpad(fpbmesreferencia::text, 2, '0')||'-15')::date,'YYYY-mm-dd') 
							ORDER BY (fpbanoreferencia::text||'-'||lpad(fpbmesreferencia::text, 2, '0')||'-15')::date DESC LIMIT 1
							
							)");

$reprovados = $db->pegaUm("SELECT count(distinct c.curid) FROM sisfor.cursistacurso c
						  INNER JOIN sisfor.cursistaavaliacoes ca ON ca.cucid = c.cucid
						  WHERE c.sifid='".$_SESSION['sisfor']['sifid']."' AND cavsituacao='r' AND c.fpbid IN(

							SELECT f.fpbid FROM sisfor.folhapagamentocursista f 
							INNER JOIN sisfor.folhapagamento fp ON fp.fpbid = f.fpbid 
							WHERE f.sifid=c.sifid AND to_char(NOW(),'YYYY-mm-dd')>=to_char((fpbanoreferencia::text||'-'||lpad(fpbmesreferencia::text, 2, '0')||'-15')::date,'YYYY-mm-dd') 
							ORDER BY (fpbanoreferencia::text||'-'||lpad(fpbmesreferencia::text, 2, '0')||'-15')::date DESC LIMIT 1
							
							)");

$evadidos = $db->pegaUm("SELECT count(distinct c.curid) FROM sisfor.cursistacurso c
						  INNER JOIN sisfor.cursistaavaliacoes ca ON ca.cucid = c.cucid
						  WHERE c.sifid='".$_SESSION['sisfor']['sifid']."' AND cavsituacao='e' AND c.fpbid IN(

							SELECT f.fpbid FROM sisfor.folhapagamentocursista f 
							INNER JOIN sisfor.folhapagamento fp ON fp.fpbid = f.fpbid 
							WHERE f.sifid=c.sifid AND to_char(NOW(),'YYYY-mm-dd')>=to_char((fpbanoreferencia::text||'-'||lpad(fpbmesreferencia::text, 2, '0')||'-15')::date,'YYYY-mm-dd') 
							ORDER BY (fpbanoreferencia::text||'-'||lpad(fpbmesreferencia::text, 2, '0')||'-15')::date DESC LIMIT 1
							
							)");

$falecidos = $db->pegaUm("SELECT count(distinct c.curid) FROM sisfor.cursistacurso c
						  INNER JOIN sisfor.cursistaavaliacoes ca ON ca.cucid = c.cucid
						  WHERE c.sifid='".$_SESSION['sisfor']['sifid']."' AND cavsituacao='f' AND c.fpbid IN(

							SELECT f.fpbid FROM sisfor.folhapagamentocursista f 
							INNER JOIN sisfor.folhapagamento fp ON fp.fpbid = f.fpbid 
							WHERE f.sifid=c.sifid AND to_char(NOW(),'YYYY-mm-dd')>=to_char((fpbanoreferencia::text||'-'||lpad(fpbmesreferencia::text, 2, '0')||'-15')::date,'YYYY-mm-dd') 
							ORDER BY (fpbanoreferencia::text||'-'||lpad(fpbmesreferencia::text, 2, '0')||'-15')::date DESC LIMIT 1
							
							)");
?>	
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloEsquerda" colspan="2"><span style=font-size:large;>5. Indicadores de Desempenho</span></td>
</tr>
<tr>
	<td class="SubTituloEsquerda" colspan="2">Taxa de conclusão</td>
</tr>
<tr>
	<td colspan="2">
	
	<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr><td class="SubTituloDireita" width="25%">Meta pactuada</td><td><?=$meta ?></td></tr>
	<tr><td class="SubTituloDireita" width="25%">Inseridos</td><td><?=$inseridos ?></td></tr>
	</table>
	<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr><td class="SubTituloCentro" width="25%">&nbsp;</td><td class="SubTituloCentro" width="25%">Quantidade</td><td class="SubTituloCentro" width="25%">Taxa sobre matriculados</td><td class="SubTituloCentro" width="25%">Taxa sobre meta</td></tr>
	<tr><td class="SubTituloDireita" width="25%">Aprovados</td><td width="25%" align="right"><?=(($aprovados)?$aprovados:'0') ?></td><td align="right"><?=(($aprovados)?round(($aprovados/$inseridos)*100,2):'0') ?>%</td><td align="right"><?=(($aprovados)?round(($aprovados/$meta)*100,2):'0') ?>%</td></tr>
	<tr><td class="SubTituloDireita" width="25%">Reprovados</td><td width="25%" align="right"><?=(($reprovados)?$reprovados:'0') ?></td><td align="right"><?=(($reprovados)?round(($reprovados/$inseridos)*100,2):'0') ?>%</td><td align="right"><?=(($reprovados)?round(($reprovados/$meta)*100,2):'0') ?>%</td></tr>
	<tr><td class="SubTituloDireita" width="25%">Evadidos</td><td width="25%" align="right"><?=(($evadidos)?$evadidos:'0') ?></td><td align="right"><?=(($evadidos)?round(($evadidos/$inseridos)*100,2):'0') ?>%</td><td align="right"><?=(($evadidos)?round(($evadidos/$meta)*100,2):'0') ?>%</td></tr>
	<tr><td class="SubTituloDireita" width="25%">Falecidos</td><td width="25%" align="right"><?=(($falecidos)?$falecidos:'0') ?></td><td align="right"><?=(($falecidos)?round(($falecidos/$inseridos)*100,2):'0') ?>%</td><td align="right"><?=(($falecidos)?round(($falecidos/$meta)*100,2):'0') ?>%</td></tr>
	</table>
	
	</td>
</tr>
<tr>
	<td class="SubTituloEsquerda" colspan="2">Relação entre o número de profissionais</td>
</tr>
<tr>
	<td colspan="2">
	<?
	
	$qtd_perfil = $db->carregar("SELECT t.pflcod, count(*) as total FROM sisfor.identificacaousuario i
								   INNER JOIN sisfor.tipoperfil t ON t.iusd = i.iusd 
								   WHERE i.iusstatus='A' AND t.pflcod IN(".PFL_PROFESSOR_PESQUISADOR.",".PFL_FORMADOR_IES.",".PFL_TUTOR.") AND t.sifid='".$_SESSION['sisfor']['sifid']."' 
								   GROUP BY t.pflcod");

	if($qtd_perfil[0]) {
		foreach($qtd_perfil as $qtd) {
			$totais[$qtd['pflcod']] = $qtd['total'];
		}
	}
	
	$arrTxRelprof[] = array('perfil'=>'Cursista/ Professor Pesquisador','rel'=>'<span style=float:right;>'.(($totais[PFL_PROFESSOR_PESQUISADOR])?round($meta/$totais[PFL_PROFESSOR_PESQUISADOR],2).' / 1':'Não se aplica').'</span>','tx'=>'<span style=float:right;>'.(($totais[PFL_PROFESSOR_PESQUISADOR])?round((($aprovados)?$aprovados:0)/$totais[PFL_PROFESSOR_PESQUISADOR],2).' / 1':'Não se aplica').'</span>');
	$arrTxRelprof[] = array('perfil'=>'Cursista/ Formador','rel'=>'<span style=float:right;>'.(($totais[PFL_FORMADOR_IES])?round($meta/$totais[PFL_FORMADOR_IES],2).' / 1':'Não se aplica').'</span>','tx'=>'<span style=float:right;>'.(($totais[PFL_FORMADOR_IES])?round((($aprovados)?$aprovados:0)/$totais[PFL_FORMADOR_IES],2).' / 1':'Não se aplica').'</span>');
	$arrTxRelprof[] = array('perfil'=>'Cursista/ Tutor','rel'=>'<span style=float:right;>'.(($totais[PFL_TUTOR])?round($meta/$totais[PFL_TUTOR],2).' / 1':'Não se aplica').'</span>','tx'=>'<span style=float:right;>'.(($totais[PFL_TUTOR])?round((($aprovados)?$aprovados:'0')/$totais[PFL_TUTOR],2):'Não se aplica').'</span>');
	
	if(!$arrTxRelprof) $arrTxRelprof = array();
	
	$cabecalho = array("Relação","Taxa prevista = Meta/Função","Taxa efetiva = Aprovados/Função");
	$db->monta_lista_simples($arrTxRelprof,$cabecalho,100000,5,'S','100%','');
	?>
	</td>
</tr>
	
<tr>
	<td class="SubTituloEsquerda" colspan="2">Qualificação da equipe docente</td>
</tr>
<tr>
	<td colspan="2">
	<?
	
	$sql = "SELECT foo.pfldsc, count(*), sum(foo.doutorado) as doutorado, '<span style=float:right>'||round((sum(foo.doutorado)::numeric/count(*)::numeric)*100,2)||'</span>' as porc_doutorado, sum(foo.mestrado) as mestrado, '<span style=float:right>'||round((sum(foo.mestrado)::numeric/count(*)::numeric)*100,2)||'</span>' as porc_mestrado, sum(foo.especializacao) as especializacao, '<span style=float:right>'||round((sum(foo.especializacao)::numeric/count(*)::numeric)*100,2)||'</span>' as porc_especializacao, sum(foo.graduacao) as graduacao, '<span style=float:right>'||round((sum(foo.graduacao)::numeric/count(*)::numeric)*100,2)||'</span>' as porc_graduacao 
			FROM (
			SELECT p.pfldsc, 
				   CASE WHEN i.foeid=".FOE_DOUTORADO." THEN 1 ELSE 0 END as doutorado, 
				   CASE WHEN i.foeid=".FOE_MESTRADO." THEN 1 ELSE 0 END as mestrado,
				   CASE WHEN i.foeid=".FOE_ESPECIALIZACAO." THEN 1 ELSE 0 END as especializacao,
				   CASE WHEN i.foeid IN(".FOE_SUPERIOR_COMPLETO_PEDAGOGIA.",".FOE_SUPERIOR_COMPLETO_LICENCIATURA.",".FOE_SUPERIOR_COMPLETO_OUTRO.") THEN 1 ELSE 0 END as graduacao,
				   p.pflnivel
			FROM sisfor.identificacaousuario i 
			INNER JOIN sisfor.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
			WHERE t.sifid='".$_SESSION['sisfor']['sifid']."' 
			) foo 
			GROUP BY foo.pfldsc, foo.pflnivel
			ORDER BY foo.pflnivel
			";	
	
	$cabecalho = array("Perfil","Total","Doutorado","%","Mestrado","%","Especialização","%","Graduação","%");
	$db->monta_lista_simples($sql,$cabecalho,100000,5,'S','100%','');
	?>
	</td>
</tr>
</table>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloEsquerda" colspan="2"><span style=font-size:large;>6. Perfil do cursista</span></td>
</tr>
<tr>
	<td colspan="2">
	
	<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloCentro" width="16%">&nbsp;</td>
		<td class="SubTituloCentro" width="28%">Aprovados</td>
		<td class="SubTituloCentro" width="28%">Reprovados</td>
		<td class="SubTituloCentro" width="28%">Evadidos</td>
	</tr>

	<tr>
		<td class="SubTituloDireita">Escolaridade</td>
		<td valign="top"><?
		$sql = "SELECT COALESCE(fr.foedesc, 'Não informado') as foedesc, count(*) as tot FROM sisfor.cursista cc 
				LEFT JOIN sisfor.formacaoescolaridade fr ON fr.foeid = cc.curescolaridade 
				WHERE cc.curid IN(
		
				SELECT distinct c.curid FROM sisfor.cursistacurso c 
				INNER JOIN sisfor.cursistaavaliacoes ca ON ca.cucid = c.cucid 
				WHERE c.sifid='".$_SESSION['sisfor']['sifid']."' AND cavsituacao='p'
			
				) 
				GROUP BY fr.foedesc";
		
		$cabecalho = array();
		$db->monta_lista_simples($sql,$cabecalho,100000,5,'S','100%','');
		
		?></td>
		<td valign="top"><?
		$sql = "SELECT COALESCE(fr.foedesc, 'Não informado') as foedesc, count(*) as tot FROM sisfor.cursista cc 
				LEFT JOIN sisfor.formacaoescolaridade fr ON fr.foeid = cc.curescolaridade 
				WHERE cc.curid IN(
		
				SELECT distinct c.curid FROM sisfor.cursistacurso c 
				INNER JOIN sisfor.cursistaavaliacoes ca ON ca.cucid = c.cucid 
				WHERE c.sifid='".$_SESSION['sisfor']['sifid']."' AND cavsituacao='r'
			
				)
				GROUP BY fr.foedesc";
		
		$cabecalho = array();
		$db->monta_lista_simples($sql,$cabecalho,100000,5,'S','100%','');
		
		?></td>
		<td valign="top"><?
		
		$sql = "SELECT COALESCE(fr.foedesc, 'Não informado') as foedesc, count(*) as tot FROM sisfor.cursista cc 
				LEFT JOIN sisfor.formacaoescolaridade fr ON fr.foeid = cc.curescolaridade 
				WHERE cc.curid IN(
		
				SELECT distinct c.curid FROM sisfor.cursistacurso c 
				INNER JOIN sisfor.cursistaavaliacoes ca ON ca.cucid = c.cucid 
				WHERE c.sifid='".$_SESSION['sisfor']['sifid']."' AND cavsituacao='e'
			
				)
				GROUP BY fr.foedesc";
		
		$cabecalho = array();
		$db->monta_lista_simples($sql,$cabecalho,100000,5,'S','100%','');
		
		?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Rede em que atua</td>
		<td valign="top"><?
		$sql = "SELECT CASE WHEN cc.currede='M' THEN 'Municipal' 
							WHEN cc.currede='E' THEN 'Estadual' 
							WHEN cc.currede='F' THEN 'Federal' 
							ELSE 'Não informado' END as currede, count(*) as tot FROM sisfor.cursista cc
				WHERE cc.curid IN(
		
				SELECT distinct c.curid FROM sisfor.cursistacurso c
				INNER JOIN sisfor.cursistaavaliacoes ca ON ca.cucid = c.cucid
				WHERE c.sifid='".$_SESSION['sisfor']['sifid']."' AND cavsituacao='p'
		
				)
				GROUP BY cc.currede";
		
		$cabecalho = array();
		$db->monta_lista_simples($sql,$cabecalho,100000,5,'S','100%','');
		
		
		
		?></td>
		<td valign="top"><?
		$sql = "SELECT CASE WHEN cc.currede='M' THEN 'Municipal' 
							WHEN cc.currede='E' THEN 'Estadual' 
							WHEN cc.currede='F' THEN 'Federal' 
							ELSE 'Não informado' END as currede, count(*) as tot FROM sisfor.cursista cc
				WHERE cc.curid IN(
		
				SELECT distinct c.curid FROM sisfor.cursistacurso c
				INNER JOIN sisfor.cursistaavaliacoes ca ON ca.cucid = c.cucid
				WHERE c.sifid='".$_SESSION['sisfor']['sifid']."' AND cavsituacao='r'
		
				)
				GROUP BY cc.currede";
		
		$cabecalho = array();
		$db->monta_lista_simples($sql,$cabecalho,100000,5,'S','100%','');
		
		
		
		?></td>
		<td valign="top"><?
		
		$sql = "SELECT CASE WHEN cc.currede='M' THEN 'Municipal' 
							WHEN cc.currede='E' THEN 'Estadual' 
							WHEN cc.currede='F' THEN 'Federal' 
							ELSE 'Não informado' END as currede, count(*) as tot FROM sisfor.cursista cc
				WHERE cc.curid IN(
		
				SELECT distinct c.curid FROM sisfor.cursistacurso c
				INNER JOIN sisfor.cursistaavaliacoes ca ON ca.cucid = c.cucid
				WHERE c.sifid='".$_SESSION['sisfor']['sifid']."' AND cavsituacao='e'
		
				)
				GROUP BY cc.currede";
		
		$cabecalho = array();
		$db->monta_lista_simples($sql,$cabecalho,100000,5,'S','100%','');
		
		?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Vínculo com a rede de ensino</td>
		<td valign="top"><?
		
		$sql = "SELECT COALESCE(c.no_contratacao, 'Não informado') as curcontratacao, count(*) as tot FROM sisfor.cursista cc 
				LEFT JOIN sisfor.tab_tipo_contratacao c ON c.pk_cod_tipo_contratacao = cc.curcontratacao 
				WHERE cc.curid IN(
		
				SELECT distinct c.curid FROM sisfor.cursistacurso c
				INNER JOIN sisfor.cursistaavaliacoes ca ON ca.cucid = c.cucid
				WHERE c.sifid='".$_SESSION['sisfor']['sifid']."' AND cavsituacao='p'
		
				)
				GROUP BY c.no_contratacao";
		
		$cabecalho = array();
		$db->monta_lista_simples($sql,$cabecalho,100000,5,'S','100%','');
		
		?></td>
		<td valign="top"><?

		$sql = "SELECT COALESCE(c.no_contratacao, 'Não informado') as curcontratacao, count(*) as tot FROM sisfor.cursista cc
				LEFT JOIN sisfor.tab_tipo_contratacao c ON c.pk_cod_tipo_contratacao = cc.curcontratacao
				WHERE cc.curid IN(
		
				SELECT distinct c.curid FROM sisfor.cursistacurso c
				INNER JOIN sisfor.cursistaavaliacoes ca ON ca.cucid = c.cucid
				WHERE c.sifid='".$_SESSION['sisfor']['sifid']."' AND cavsituacao='r'
		
				)
				GROUP BY c.no_contratacao";
		
		$cabecalho = array();
		$db->monta_lista_simples($sql,$cabecalho,100000,5,'S','100%','');
		
		
		?></td>
		<td valign="top"><?
		
		$sql = "SELECT COALESCE(c.no_contratacao, 'Não informado') as curcontratacao, count(*) as tot FROM sisfor.cursista cc
				LEFT JOIN sisfor.tab_tipo_contratacao c ON c.pk_cod_tipo_contratacao = cc.curcontratacao
				WHERE cc.curid IN(
		
				SELECT distinct c.curid FROM sisfor.cursistacurso c
				INNER JOIN sisfor.cursistaavaliacoes ca ON ca.cucid = c.cucid
				WHERE c.sifid='".$_SESSION['sisfor']['sifid']."' AND cavsituacao='e'
		
				)
				GROUP BY c.no_contratacao";
		
		$cabecalho = array();
		$db->monta_lista_simples($sql,$cabecalho,100000,5,'S','100%','');
		
		?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Sexo</td>
		<td valign="top"><?
		$sql = "SELECT CASE WHEN cc.cursexo='M' THEN 'Masculino'
							WHEN cc.cursexo='F' THEN 'Feminino' 
							ELSE 'Não informado' END as cursexo, count(*) as tot FROM sisfor.cursista cc
				WHERE cc.curid IN(
		
				SELECT distinct c.curid FROM sisfor.cursistacurso c
				INNER JOIN sisfor.cursistaavaliacoes ca ON ca.cucid = c.cucid
				WHERE c.sifid='".$_SESSION['sisfor']['sifid']."' AND cavsituacao='p'
		
				)
				GROUP BY cc.cursexo";
		
		$cabecalho = array();
		$db->monta_lista_simples($sql,$cabecalho,100000,5,'S','100%','');
		
		
		?></td>
		<td valign="top"><?
		$sql = "SELECT CASE WHEN cc.cursexo='M' THEN 'Masculino'
							WHEN cc.cursexo='F' THEN 'Feminino' 
							ELSE 'Não informado' END as cursexo, count(*) as tot FROM sisfor.cursista cc
				WHERE cc.curid IN(
		
				SELECT distinct c.curid FROM sisfor.cursistacurso c
				INNER JOIN sisfor.cursistaavaliacoes ca ON ca.cucid = c.cucid
				WHERE c.sifid='".$_SESSION['sisfor']['sifid']."' AND cavsituacao='r'
		
				)
				GROUP BY cc.cursexo";
		
		$cabecalho = array();
		$db->monta_lista_simples($sql,$cabecalho,100000,5,'S','100%','');
		
		
		?></td>
		<td valign="top"><?
		$sql = "SELECT CASE WHEN cc.cursexo='M' THEN 'Masculino'
							WHEN cc.cursexo='F' THEN 'Feminino' 
							ELSE 'Não informado' END as cursexo, count(*) as tot FROM sisfor.cursista cc
				WHERE cc.curid IN(
		
				SELECT distinct c.curid FROM sisfor.cursistacurso c
				INNER JOIN sisfor.cursistaavaliacoes ca ON ca.cucid = c.cucid
				WHERE c.sifid='".$_SESSION['sisfor']['sifid']."' AND cavsituacao='e'
		
				)
				GROUP BY cc.cursexo";
		
		$cabecalho = array();
		$db->monta_lista_simples($sql,$cabecalho,100000,5,'S','100%','');
		
		
		?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Faixa etária</td>
		<td valign="top"><?
		$sql = "SELECT 		
					CASE WHEN extract(year from age(curdatanascimento)) >= 60 THEN '60+'
				     WHEN extract(year from age(curdatanascimento)) >= 50 AND extract(year from age(curdatanascimento)) < 60 THEN '50-60'
				     WHEN extract(year from age(curdatanascimento)) >= 40 AND extract(year from age(curdatanascimento)) < 50 THEN '40-50' 
				     WHEN extract(year from age(curdatanascimento)) >= 30 AND extract(year from age(curdatanascimento)) < 40 THEN '30-40' 
				     WHEN extract(year from age(curdatanascimento)) >= 20 AND extract(year from age(curdatanascimento)) < 30 THEN '20-30'
					 WHEN extract(year from age(curdatanascimento)) < 20 THEN '19-'
				     ELSE 'Não informado' END as faixa, count(*) as tot 
			FROM sisfor.cursista cc
			WHERE cc.curid IN(
		
				SELECT distinct c.curid FROM sisfor.cursistacurso c
				INNER JOIN sisfor.cursistaavaliacoes ca ON ca.cucid = c.cucid
				WHERE c.sifid='".$_SESSION['sisfor']['sifid']."' AND cavsituacao='p'
		
			)
			GROUP BY faixa 
			ORDER BY faixa";
		
		$cabecalho = array();
		$db->monta_lista_simples($sql,$cabecalho,100000,5,'S','100%','');
		
		
		?></td>
		<td valign="top"><?
		$sql = "SELECT 		
					CASE WHEN extract(year from age(curdatanascimento)) >= 60 THEN '60+'
				     WHEN extract(year from age(curdatanascimento)) >= 50 AND extract(year from age(curdatanascimento)) < 60 THEN '50-60'
				     WHEN extract(year from age(curdatanascimento)) >= 40 AND extract(year from age(curdatanascimento)) < 50 THEN '40-50' 
				     WHEN extract(year from age(curdatanascimento)) >= 30 AND extract(year from age(curdatanascimento)) < 40 THEN '30-40' 
				     WHEN extract(year from age(curdatanascimento)) >= 20 AND extract(year from age(curdatanascimento)) < 30 THEN '20-30'
					 WHEN extract(year from age(curdatanascimento)) < 20 THEN '19-'
				     ELSE 'Não informado' END as faixa, count(*) as tot 
			FROM sisfor.cursista cc
			WHERE cc.curid IN(
		
				SELECT distinct c.curid FROM sisfor.cursistacurso c
				INNER JOIN sisfor.cursistaavaliacoes ca ON ca.cucid = c.cucid
				WHERE c.sifid='".$_SESSION['sisfor']['sifid']."' AND cavsituacao='r'
		
			)
			GROUP BY faixa 
			ORDER BY faixa";
		
		$cabecalho = array();
		$db->monta_lista_simples($sql,$cabecalho,100000,5,'S','100%','');
		
		
		?></td>
		<td valign="top"><?
		$sql = "SELECT 		
					CASE WHEN extract(year from age(curdatanascimento)) >= 60 THEN '60+'
				     WHEN extract(year from age(curdatanascimento)) >= 50 AND extract(year from age(curdatanascimento)) < 60 THEN '50-60'
				     WHEN extract(year from age(curdatanascimento)) >= 40 AND extract(year from age(curdatanascimento)) < 50 THEN '40-50' 
				     WHEN extract(year from age(curdatanascimento)) >= 30 AND extract(year from age(curdatanascimento)) < 40 THEN '30-40' 
				     WHEN extract(year from age(curdatanascimento)) >= 20 AND extract(year from age(curdatanascimento)) < 30 THEN '20-30'
					 WHEN extract(year from age(curdatanascimento)) < 20 THEN '19-'
				     ELSE 'Não informado' END as faixa, count(*) as tot 
			FROM sisfor.cursista cc
			WHERE cc.curid IN(
		
				SELECT distinct c.curid FROM sisfor.cursistacurso c
				INNER JOIN sisfor.cursistaavaliacoes ca ON ca.cucid = c.cucid
				WHERE c.sifid='".$_SESSION['sisfor']['sifid']."' AND cavsituacao='e'
		
			)
			GROUP BY faixa 
			ORDER BY faixa";
		
		$cabecalho = array();
		$db->monta_lista_simples($sql,$cabecalho,100000,5,'S','100%','');
		
		?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Raça/ Etnia</td>
		<td valign="top"><?

		$sql = "SELECT
					CASE WHEN curraca=0 THEN 'Não declarada'
				     WHEN curraca=1 THEN 'Branca'
				     WHEN curraca=2 THEN 'Preta'
				     WHEN curraca=3 THEN 'Parda'
				     WHEN curraca=4 THEN 'Amarela'
					 WHEN curraca=5 THEN 'Indígena'
				     ELSE 'Não informado' END as curraca, count(*) as tot
			FROM sisfor.cursista cc
			WHERE cc.curid IN(
		
				SELECT distinct c.curid FROM sisfor.cursistacurso c
				INNER JOIN sisfor.cursistaavaliacoes ca ON ca.cucid = c.cucid
				WHERE c.sifid='".$_SESSION['sisfor']['sifid']."' AND cavsituacao='p'
		
			)
			GROUP BY curraca
			ORDER BY curraca";
		
		$cabecalho = array();
		$db->monta_lista_simples($sql,$cabecalho,100000,5,'S','100%','');
		
		?></td>
		<td valign="top"><?

		$sql = "SELECT
					CASE WHEN curraca=0 THEN 'Não declarada'
				     WHEN curraca=1 THEN 'Branca'
				     WHEN curraca=2 THEN 'Preta'
				     WHEN curraca=3 THEN 'Parda'
				     WHEN curraca=4 THEN 'Amarela'
					 WHEN curraca=5 THEN 'Indígena'
				     ELSE 'Não informado' END as curraca, count(*) as tot
			FROM sisfor.cursista cc
			WHERE cc.curid IN(
		
				SELECT distinct c.curid FROM sisfor.cursistacurso c
				INNER JOIN sisfor.cursistaavaliacoes ca ON ca.cucid = c.cucid
				WHERE c.sifid='".$_SESSION['sisfor']['sifid']."' AND cavsituacao='r'
		
			)
			GROUP BY curraca
			ORDER BY curraca";
		
		$cabecalho = array();
		$db->monta_lista_simples($sql,$cabecalho,100000,5,'S','100%','');
		
		?></td>
		<td valign="top"><?

		$sql = "SELECT
					CASE WHEN curraca=0 THEN 'Não declarada'
				     WHEN curraca=1 THEN 'Branca'
				     WHEN curraca=2 THEN 'Preta'
				     WHEN curraca=3 THEN 'Parda'
				     WHEN curraca=4 THEN 'Amarela'
					 WHEN curraca=5 THEN 'Indígena'
				     ELSE 'Não informado' END as curraca, count(*) as tot
			FROM sisfor.cursista cc
			WHERE cc.curid IN(
		
				SELECT distinct c.curid FROM sisfor.cursistacurso c
				INNER JOIN sisfor.cursistaavaliacoes ca ON ca.cucid = c.cucid
				WHERE c.sifid='".$_SESSION['sisfor']['sifid']."' AND cavsituacao='e'
		
			)
			GROUP BY curraca
			ORDER BY curraca";
		
		$cabecalho = array();
		$db->monta_lista_simples($sql,$cabecalho,100000,5,'S','100%','');
		
		?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Possui deficiência?</td>
		<td valign="top"><?

		$sql = "SELECT
					CASE WHEN curdeficiencia=1 THEN 'Cegueira'
				     WHEN curdeficiencia=2 THEN 'Surdocegueira'
				     WHEN curdeficiencia=3 THEN 'Baixa visão'
				     WHEN curdeficiencia=4 THEN 'Deficiência física'
				     WHEN curdeficiencia=5 THEN 'Surdez'
					 WHEN curdeficiencia=6 THEN 'Deficiência intelectual'
			
				     WHEN curdeficiencia=7 THEN 'Deficiência auditiva'
				     WHEN curdeficiencia=8 THEN 'Deficiência multipla'
					 WHEN curdeficiencia=9 THEN 'Não declarado'
					 WHEN curdeficiencia=10 THEN 'Não possui deficiência'
			
				     ELSE 'Não informado' END as curdeficiencia, count(*) as tot
			FROM sisfor.cursista cc
			WHERE cc.curid IN(
		
				SELECT distinct c.curid FROM sisfor.cursistacurso c
				INNER JOIN sisfor.cursistaavaliacoes ca ON ca.cucid = c.cucid
				WHERE c.sifid='".$_SESSION['sisfor']['sifid']."' AND cavsituacao='p'
		
			)
			GROUP BY curdeficiencia
			ORDER BY curdeficiencia";
		
		$cabecalho = array();
		$db->monta_lista_simples($sql,$cabecalho,100000,5,'S','100%','');
		
		?></td>
		<td valign="top"><?

		$sql = "SELECT
					CASE WHEN curdeficiencia=1 THEN 'Cegueira'
				     WHEN curdeficiencia=2 THEN 'Surdocegueira'
				     WHEN curdeficiencia=3 THEN 'Baixa visão'
				     WHEN curdeficiencia=4 THEN 'Deficiência física'
				     WHEN curdeficiencia=5 THEN 'Surdez'
					 WHEN curdeficiencia=6 THEN 'Deficiência intelectual'
			
				     WHEN curdeficiencia=7 THEN 'Deficiência auditiva'
				     WHEN curdeficiencia=8 THEN 'Deficiência multipla'
					 WHEN curdeficiencia=9 THEN 'Não declarado'
					 WHEN curdeficiencia=10 THEN 'Não possui deficiência'
			
				     ELSE 'Não informado' END as curdeficiencia, count(*) as tot
			FROM sisfor.cursista cc
			WHERE cc.curid IN(
		
				SELECT distinct c.curid FROM sisfor.cursistacurso c
				INNER JOIN sisfor.cursistaavaliacoes ca ON ca.cucid = c.cucid
				WHERE c.sifid='".$_SESSION['sisfor']['sifid']."' AND cavsituacao='r'
		
			)
			GROUP BY curdeficiencia
			ORDER BY curdeficiencia";
		
		$cabecalho = array();
		$db->monta_lista_simples($sql,$cabecalho,100000,5,'S','100%','');
		
		?></td>
		<td valign="top"><?

		$sql = "SELECT
					CASE WHEN curdeficiencia=1 THEN 'Cegueira'
				     WHEN curdeficiencia=2 THEN 'Surdocegueira'
				     WHEN curdeficiencia=3 THEN 'Baixa visão'
				     WHEN curdeficiencia=4 THEN 'Deficiência física'
				     WHEN curdeficiencia=5 THEN 'Surdez'
					 WHEN curdeficiencia=6 THEN 'Deficiência intelectual'
			
				     WHEN curdeficiencia=7 THEN 'Deficiência auditiva'
				     WHEN curdeficiencia=8 THEN 'Deficiência multipla'
					 WHEN curdeficiencia=9 THEN 'Não declarado'
					 WHEN curdeficiencia=10 THEN 'Não possui deficiência'
			
				     ELSE 'Não informado' END as curdeficiencia, count(*) as tot
			FROM sisfor.cursista cc
			WHERE cc.curid IN(
		
				SELECT distinct c.curid FROM sisfor.cursistacurso c
				INNER JOIN sisfor.cursistaavaliacoes ca ON ca.cucid = c.cucid
				WHERE c.sifid='".$_SESSION['sisfor']['sifid']."' AND cavsituacao='e'
		
			)
			GROUP BY curdeficiencia
			ORDER BY curdeficiencia";
		
		$cabecalho = array();
		$db->monta_lista_simples($sql,$cabecalho,100000,5,'S','100%','');
		
		?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Função que exerce</td>
		<td valign="top"><?

		$sql = "SELECT
					CASE WHEN curfuncao=1 THEN 'Auxiliar de Educação Infantil'
				     WHEN curfuncao=2 THEN 'Conselheiro Escolar'
				     WHEN curfuncao=3 THEN 'Conselheiro Municipal de Educação'
				     WHEN curfuncao=4 THEN 'Coordenador Pedagógico'
				     WHEN curfuncao=5 THEN 'Diretor'
					 WHEN curfuncao=6 THEN 'Dirigente de Educação'
			
				     WHEN curfuncao=7 THEN 'Docente'
				     WHEN curfuncao=8 THEN 'Estudante'
					 WHEN curfuncao=9 THEN 'Formador de NTE'
					 WHEN curfuncao=10 THEN 'Intérprete de Libras'
		
				     WHEN curfuncao=11 THEN 'Monitor de Atividade Complementar'
				     WHEN curfuncao=12 THEN 'Profissional de Assistência'
					 WHEN curfuncao=13 THEN 'Profissional de Saúde'
					 WHEN curfuncao=14 THEN 'Técnico da Secretaria de Educação'
					 WHEN curfuncao=15 THEN 'Técnico de NTE'
					 WHEN curfuncao=16 THEN 'Vice-Diretor'
		
			
				     ELSE 'Não informado' END as curfuncao, count(*) as tot
			FROM sisfor.cursista cc
			WHERE cc.curid IN(
		
				SELECT distinct c.curid FROM sisfor.cursistacurso c
				INNER JOIN sisfor.cursistaavaliacoes ca ON ca.cucid = c.cucid
				WHERE c.sifid='".$_SESSION['sisfor']['sifid']."' AND cavsituacao='p'
		
			)
			GROUP BY curfuncao
			ORDER BY curfuncao";
		
		$cabecalho = array();
		$db->monta_lista_simples($sql,$cabecalho,100000,5,'S','100%','');
		
		?></td>
		<td valign="top"><?

		$sql = "SELECT
					CASE WHEN curfuncao=1 THEN 'Auxiliar de Educação Infantil'
				     WHEN curfuncao=2 THEN 'Conselheiro Escolar'
				     WHEN curfuncao=3 THEN 'Conselheiro Municipal de Educação'
				     WHEN curfuncao=4 THEN 'Coordenador Pedagógico'
				     WHEN curfuncao=5 THEN 'Diretor'
					 WHEN curfuncao=6 THEN 'Dirigente de Educação'
			
				     WHEN curfuncao=7 THEN 'Docente'
				     WHEN curfuncao=8 THEN 'Estudante'
					 WHEN curfuncao=9 THEN 'Formador de NTE'
					 WHEN curfuncao=10 THEN 'Intérprete de Libras'
		
				     WHEN curfuncao=11 THEN 'Monitor de Atividade Complementar'
				     WHEN curfuncao=12 THEN 'Profissional de Assistência'
					 WHEN curfuncao=13 THEN 'Profissional de Saúde'
					 WHEN curfuncao=14 THEN 'Técnico da Secretaria de Educação'
					 WHEN curfuncao=15 THEN 'Técnico de NTE'
					 WHEN curfuncao=16 THEN 'Vice-Diretor'
		
			
				     ELSE 'Não informado' END as curfuncao, count(*) as tot
			FROM sisfor.cursista cc
			WHERE cc.curid IN(
		
				SELECT distinct c.curid FROM sisfor.cursistacurso c
				INNER JOIN sisfor.cursistaavaliacoes ca ON ca.cucid = c.cucid
				WHERE c.sifid='".$_SESSION['sisfor']['sifid']."' AND cavsituacao='r'
		
			)
			GROUP BY curfuncao
			ORDER BY curfuncao";
		
		$cabecalho = array();
		$db->monta_lista_simples($sql,$cabecalho,100000,5,'S','100%','');
		
		?></td>
		<td valign="top"><?

		$sql = "SELECT
					CASE WHEN curfuncao=1 THEN 'Auxiliar de Educação Infantil'
				     WHEN curfuncao=2 THEN 'Conselheiro Escolar'
				     WHEN curfuncao=3 THEN 'Conselheiro Municipal de Educação'
				     WHEN curfuncao=4 THEN 'Coordenador Pedagógico'
				     WHEN curfuncao=5 THEN 'Diretor'
					 WHEN curfuncao=6 THEN 'Dirigente de Educação'
			
				     WHEN curfuncao=7 THEN 'Docente'
				     WHEN curfuncao=8 THEN 'Estudante'
					 WHEN curfuncao=9 THEN 'Formador de NTE'
					 WHEN curfuncao=10 THEN 'Intérprete de Libras'
		
				     WHEN curfuncao=11 THEN 'Monitor de Atividade Complementar'
				     WHEN curfuncao=12 THEN 'Profissional de Assistência'
					 WHEN curfuncao=13 THEN 'Profissional de Saúde'
					 WHEN curfuncao=14 THEN 'Técnico da Secretaria de Educação'
					 WHEN curfuncao=15 THEN 'Técnico de NTE'
					 WHEN curfuncao=16 THEN 'Vice-Diretor'
		
			
				     ELSE 'Não informado' END as curfuncao, count(*) as tot
			FROM sisfor.cursista cc
			WHERE cc.curid IN(
		
				SELECT distinct c.curid FROM sisfor.cursistacurso c
				INNER JOIN sisfor.cursistaavaliacoes ca ON ca.cucid = c.cucid
				WHERE c.sifid='".$_SESSION['sisfor']['sifid']."' AND cavsituacao='e'
		
			)
			GROUP BY curfuncao
			ORDER BY curfuncao";
		
		$cabecalho = array();
		$db->monta_lista_simples($sql,$cabecalho,100000,5,'S','100%','');
		
		?></td>
	</tr>
	</table>
	
	</td>
</tr>

</table>
	
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloEsquerda" colspan="2"><span style=font-size:large;>7. Análise Crítica</span></td>
</tr>
<tr>
	<td class="SubTituloEsquerda" colspan="2">Sobre a Articulação Institucional <img src="../imagens/ajuda.gif" align="absmiddle" onmouseover="return escape('Descrever e avaliar a participação efetiva da SEDUC, UNDIME e UNCME no planejamento do curso (ações partilhadas no Colegiado, ações orientadas ou apoio nos/pelos pólos, apoio e infraestrutura oferecidos para a efetiva participação de suas equipes no desenvolvimento do curso)');"></td>
</tr>
<tr>
	<td colspan="2">
	<?
	if($consulta) {
		echo nl2br($afcarticulacaoinstitucional);
	} else {
		echo campo_textarea( 'afcarticulacaoinstitucional', 'S', 'S', '', '85', '6', '3000');
	}
	?>
	</td>
</tr>
<tr>
	<td class="SubTituloEsquerda" colspan="2">Sobre o conteúdo do curso <img src="../imagens/ajuda.gif" align="absmiddle" onmouseover="return escape('Analisar sinteticamente conteúdos que deveriam ser revisados, justificando a proposição.');"></td>
</tr>
<tr>
	<td colspan="2">
	<?
	if($consulta) {
		echo nl2br($afcsobreconteudocurso);
	} else {
		echo campo_textarea( 'afcsobreconteudocurso', 'S', 'S', '', '85', '6', '3000');
	}
	?>
	</td>
</tr>
<tr>
	<td class="SubTituloEsquerda" colspan="2">Sobre a metodologia <img src="../imagens/ajuda.gif" align="absmiddle" onmouseover="return escape('Analisar sinteticamente a metodologia de trabalho, indicando pontos de melhoria, se for o caso.');"></td>
</tr>
<tr>
	<td colspan="2">
	<?
	if($consulta) {
		echo nl2br($afcsobremetodologia);
	} else {
		echo campo_textarea( 'afcsobremetodologia', 'S', 'S', '', '85', '6', '3000');
	}
	?>
	</td>
</tr>
	
<tr>
	<td class="SubTituloEsquerda" colspan="2">Sobre os critérios de avaliação <img src="../imagens/ajuda.gif" align="absmiddle" onmouseover="return escape('Avaliar a conveniência e adequação dos critérios de avaliação adotados pelo MEC: Frequência, Atividades realizadas e Monitoramento.');"></td>
</tr>
	
<tr>
	<td colspan="2">
	<?
	if($consulta) {
		echo nl2br($afccriteriosavaliacao);
	} else {
		echo campo_textarea( 'afccriteriosavaliacao', 'S', 'S', '', '85', '6', '3000');
	}
	?>
	</td>
</tr>
</table>
	
	
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloEsquerda" colspan="2"><span style=font-size:large;>8. Comentários finais</span></td>
</tr>
	
<tr>
	<td class="SubTituloEsquerda" colspan="2">Sobre a articulação com o COMFOR</td>
</tr>
	
<tr>
	<td colspan="2">
	<?
	if($consulta) {
		echo nl2br($afcarticulacaocomfor);
	} else {
		echo campo_textarea( 'afcarticulacaocomfor', 'S', 'S', '', '85', '6', '3000');
	}
	?>
	</td>
</tr>
<tr>
	<td class="SubTituloEsquerda" colspan="2">Sobre a articulação com MEC <img src="../imagens/ajuda.gif" align="absmiddle" onmouseover="return escape('Avaliar articulação institucional entre o MEC e a Universidade contemplando no mínimo os seguintes aspectos:<br>- Tempestividade nas respostas<br>- Agilidade no pagamento de bolsas<br>- Qualidade do suporte tecnológico<br>- Liberação de recursos.');"></td>
</tr>
<tr>
	<td colspan="2">
	<?
	if($consulta) {
		echo nl2br($afcarticulacaomec);
	} else {
		echo campo_textarea( 'afcarticulacaomec', 'S', 'S', '', '85', '6', '3000');
	}
	?>
	</td>
</tr>
<tr>
	<td class="SubTituloEsquerda" colspan="2">Lições Aprendidas <img src="../imagens/ajuda.gif" align="absmiddle" onmouseover="return escape('Descrever eventos/ situações/ experiências que contribuem para o aperfeiçoamento do programa e do curso.');"></td>
</tr>
<tr>
	<td colspan="2">
	<?
	if($consulta) {
		echo nl2br($afclicoesaprendidas);
	} else {
		echo campo_textarea( 'afclicoesaprendidas', 'S', 'S', '', '85', '6', '3000');
	}
	?>
	</td>
</tr>
<tr>
	<td class="SubTituloEsquerda" colspan="2">Outros comentários <img src="../imagens/ajuda.gif" align="absmiddle" onmouseover="return escape('Inserir informações não contempladas neste relatório consideradas relevantes para o aperfeiçoamento do programa e do curso.');"></td>
</tr>
<tr>
	<td colspan="2">
	<?
	if($consulta) {
		echo nl2br($afcoutroscomentarios);
	} else {
		echo campo_textarea( 'afcoutroscomentarios', 'S', 'S', '', '85', '6', '3000');
	}
	?>
	</td>
</tr>
<? if(!$consulta) : ?>
<tr>
	<td colspan="2" class="SubTituloCentro">
	
	<input type="submit" name="salvar" value="Salvar">
	
	</td>
</tr>
<? endif; ?>
</table>
	
	
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	
<tr>
	<td colspan="2" align="center">
	
	<p>Declaramos que as informações constantes deste Relatório Final expressam a avaliação feita por toda a equipe gestora e traduzem a situação atual de andamento do curso <Nome do curso> no âmbito desta Instituição.</p>
	
	<br><br><br><br><br><br><br>
	
	<?
	if($docid) {
		$sql = "SELECT to_char(htddata,'dd/mm/YYYY') as htddata, u.usunome FROM workflow.historicodocumento h 
				LEFT JOIN seguranca.usuario u ON u.usucpf = h.usucpf 
				WHERE h.aedid='".AED_ENVIAR_AVALFINAL_COORDENADORINST."' AND h.docid='".$docid."' ORDER BY h.htddata DESC LIMIT 1";
		
		$historicodocumento = $db->pegaLinha($sql);
	}
	
	if($historicodocumento) :
	?>
	<p style="font-size:x-small;">Encaminhado em : <b><?=$historicodocumento['htddata'] ?></b> por <b><?=$historicodocumento['usunome'] ?></b></p>
	<br><br><br>
	<?
	$coordenadorgeral = $db->pegaUm("SELECT usu.usunome FROM sisfor.sisfor s
						   			LEFT JOIN seguranca.usuario usu on usu.usucpf = s.usucpf
						   			WHERE s.sifid='".$_SESSION['sisfor']['sifid']."'");

	?>	
	_____________________________________________________________________________________<br>
	<? echo $coordenadorgeral ?><br>
	<span style="font-size:x-small;">Coordenador do curso</span>
	<br><br><br><br><br><br><br>
	<?
	endif;
	?>
	
	
	
	<?
	
	if($docid) {
		$sql = "SELECT to_char(htddata,'dd/mm/YYYY') as htddata, u.usunome FROM workflow.historicodocumento h 
				LEFT JOIN seguranca.usuario u ON u.usucpf = h.usucpf 
				WHERE h.aedid='".AED_ENVIAR_AVALFINAL_EQUIPEMEC."' AND h.docid='".$docid."' ORDER BY h.htddata DESC LIMIT 1";
		
		$historicodocumento = $db->pegaLinha($sql);
	}
	
	if($historicodocumento) :
	?>
	<p style="font-size:x-small;">Encaminhado em : <b><?=$historicodocumento['htddata'] ?></b> por <b><?=$historicodocumento['usunome'] ?></b></p>
	<br><br><br>
	<?
	$coordenadorinst = $db->pegaUm("SELECT iusnome FROM sisfor.identificacaousuario i
						   INNER JOIN sisfor.tipoperfil t ON t.iusd = i.iusd AND t.pflcod='".PFL_COORDENADOR_INST."' 
						   INNER JOIN sisfor.sisfories s ON s.tpeid = t.tpeid  
						   WHERE s.unicod='".$_SESSION['sisfor']['unicod']."' AND i.iusstatus='A'");

	?>
	
	_____________________________________________________________________________________<br>
	<? echo $coordenadorinst ?><br>
	<span style="font-size:x-small;">Coordenador Institucional do COMFOR</span>
	<br><br><br><br><br><br><br>
	<?
	endif;
	?>
	
	
	
	<?
	
	if($docid) {
		$sql = "SELECT to_char(htddata,'dd/mm/YYYY') as htddata, u.usunome FROM workflow.historicodocumento h 
				LEFT JOIN seguranca.usuario u ON u.usucpf = h.usucpf 
				WHERE h.aedid='".AED_ENVIAR_AVALFINAL_COORDEMEC."' AND h.docid='".$docid."' ORDER BY h.htddata DESC LIMIT 1";
		
		$historicodocumento = $db->pegaLinha($sql);
	}
	
	if($historicodocumento) :
	
	?>
	<p style="font-size:x-small;">Encaminhado em : <b><?=$historicodocumento['htddata'] ?></b> por <b><?=$historicodocumento['usunome'] ?></b></p>
	<br><br><br>
	
	_____________________________________________________________________________________<br>
	<? echo $historicodocumento['usunome'] ?><br>
	<span style="font-size:x-small;">Equipe MEC</span>
	<br><br><br><br><br><br><br>
	
	<?
	endif;
	?>
	
	
	
	
	
	
	
	</td>
</tr>
</table>

</form>
			
			


?>