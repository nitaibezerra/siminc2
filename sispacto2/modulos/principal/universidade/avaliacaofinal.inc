<?

$avaliacaofinalcg = $db->pegaLinha("SELECT * FROM sispacto2.avaliacaofinalcg WHERE iusd='".$_SESSION['sispacto2']['universidade']['iusd']."'");
if($avaliacaofinalcg) extract($avaliacaofinalcg);

if($docid) {

	$estado = wf_pegarEstadoAtual( $docid );
	
	if($estado['esdid'] != ESD_RELATORIOFINAL_EMELABORACAO) {
		$consulta = true;
	}

}

?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<?
$universidadecadastro = $db->pegaLinha("SELECT unisigla||' - '||uninome as uni, to_char(uncdatainicioprojeto,'dd/mm/YYYY') as uncdatainicioprojeto, to_char(uncdatafimprojeto,'dd/mm/YYYY') as uncdatafimprojeto FROM sispacto2.universidadecadastro u INNER JOIN sispacto2.universidade uu ON uu.uniid = u.uniid WHERE u.uncid='".$_SESSION['sispacto2']['universidade']['uncid']."'");

$coordenadories = $db->pegaUm("SELECT iusnome FROM sispacto2.identificacaousuario WHERE iusd='".$_SESSION['sispacto2']['universidade']['iusd']."'");

$ufsatendidas = $db->carregarColuna("SELECT DISTINCT m.estuf FROM sispacto2.abrangencia a 
									 INNER JOIN sispacto2.estruturacurso e on e.ecuid = a.ecuid
									 INNER JOIN territorios.municipio m on m.muncod = a.muncod 
									 WHERE e.uncid='".$_SESSION['sispacto2']['universidade']['uncid']."'");

$qtdmunatendidosM = $db->pegaUm("SELECT COUNT(*) as qtd FROM sispacto2.abrangencia a 
								INNER JOIN sispacto2.estruturacurso e on e.ecuid = a.ecuid 
								WHERE e.uncid='".$_SESSION['sispacto2']['universidade']['uncid']."' AND a.esfera='M'");

$qtdmunatendidosE = $db->pegaUm("SELECT COUNT(*) as qtd FROM sispacto2.abrangencia a
								INNER JOIN sispacto2.estruturacurso e on e.ecuid = a.ecuid
								WHERE e.uncid='".$_SESSION['sispacto2']['universidade']['uncid']."' AND a.esfera='E'");

?>

<script>

function salvarAvaliacaoFinal() {

	jQuery('#formulario_avaliacao').submit();
	
}


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
	
	if(totalsaldo < 0) {
		jQuery('#totalsaldo').val('-'+mascaraglobal('###.###.###,##',totalsaldo.toFixed(2)));
	} else {
		jQuery('#totalsaldo').val(mascaraglobal('###.###.###,##',totalsaldo.toFixed(2)));
	}
	
}

																			   		 		
function gravarEncontroPresencial() {
	if(jQuery('#aofnome').val()==''){alert('Preencha o nome do evento');return false;}
	if(jQuery('#aofdata').val()==''){alert('Preencha a data do evento');return false;}
	if(jQuery('#aofcargahoraria').val()==''){alert('Preencha a carga horária');return false;}
	ajaxatualizar('requisicao=gravarEncontroPresencialCG&aofnome='+jQuery('#aofnome').val()+'&aofdata='+jQuery('#aofdata').val()+'&aofcargahoraria='+jQuery('#aofcargahoraria').val(),'');
	ajaxatualizar('requisicao=carregarEncontroPresencialCoordenadorIES','td_encontropresencialcoordenadories');
}

function removerEncontroPresencial(aofid) {
	var conf = confirm('Deseja realmente remover o encontro presencial?');
	if(conf) {
		ajaxatualizar('requisicao=removerEncontroPresencialCoordenadorIES&aofid='+aofid,'');
		ajaxatualizar('requisicao=carregarEncontroPresencialCoordenadorIES','td_encontropresencialcoordenadories');
	}
}

								
</script>

<form method="post" id="formulario_avaliacao" class="formulario_avaliacao">
<input type="hidden" name="requisicao" value="gravarAvaliacaoFinalCG">

<table width="100%">

<tr>
<td  width="95%">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloEsquerda" colspan="2"><span style=font-size:large;>IDENTIFICAÇÃO</span></td>
</tr>
<tr>
	<td class="SubTituloEsquerda" width="40%">1. IES:</td><td><?=$universidadecadastro['uni'] ?></td>
</tr>
<tr>
	<td class="SubTituloEsquerda">2. Coordenador Geral:</td><td><?=$coordenadories ?></td>
</tr>
<tr>
	<td class="SubTituloEsquerda">3. UFs atendidas:</td><td><?=(($ufsatendidas)?implode(" , ",$ufsatendidas):"") ?></td>
</tr>
<tr>
	<td class="SubTituloEsquerda">4. Quantidade de municípios da rede municipal atendidos:</td><td><?=$qtdmunatendidosM ?></td>
</tr>
<tr>
	<td class="SubTituloEsquerda">5. Quantidade de municípios da rede estadual atendidos:</td><td><?=$qtdmunatendidosE ?></td>
</tr>
<tr>
	<td class="SubTituloEsquerda">6. Vigência:</td>
	<td>
	<? 
	echo $universidadecadastro['uncdatainicioprojeto'];
	echo ' até ';
	echo $universidadecadastro['uncdatafimprojeto']; 
	?>
	</td>
</tr>
<tr>
	<td class="SubTituloEsquerda">7. Período de certificação:</td>
	<td>
	<?
	echo campo_data2('afccertificacaoini','S', (($consulta)?'N':'S'), 'Data início', 'S', '', '', $afccertificacaoini, '', '', 'afccertificacaoini');
	echo 'até';
	echo campo_data2('afccertificacaofim','S', (($consulta)?'N':'S'), 'Data fim', 'S', '', '', $afccertificacaofim, '', '', 'afccertificacaofim');
	?>
	</td>
</tr>
</table>



<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloEsquerda" colspan="2"><span style=font-size:large;>BOLSAS DA EQUIPE</span></td>
</tr>

	
<tr>
	<td colspan="2">
	<?
	
	$sql = "SELECT
				pp.pfldsc,  
				(SELECT count(*) FROM sispacto2.identificacaousuario i INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd WHERE t.pflcod=pp.pflcod AND i.uncid='".$_SESSION['sispacto2']['universidade']['uncid']."')  as qtd,
				(SELECT count(*) FROM sispacto2.pagamentobolsista p INNER JOIN sispacto2.universidadecadastro u ON u.uniid = p.uniid WHERE p.pflcod=pp.pflcod AND u.uncid='".$_SESSION['sispacto2']['universidade']['uncid']."')  as qtdbolsas,
				(SELECT sum(pbovlrpagamento) FROM sispacto2.pagamentobolsista p INNER JOIN sispacto2.universidadecadastro u ON u.uniid = p.uniid WHERE p.pflcod=pp.pflcod AND u.uncid='".$_SESSION['sispacto2']['universidade']['uncid']."')  as pgbolsas
			FROM seguranca.perfil pp 
			INNER JOIN sispacto2.pagamentoperfil pe ON pe.pflcod = pp.pflcod
			ORDER BY pp.pflnivel";
	
	$cabecalho = array("Perfil","Qtd","Qtd total, por perfil, de bolsas concedidas","Valor total, por perfil, das bolsas concedidas");
	$db->monta_lista_simples($sql,$cabecalho,100000,5,'N','100%','',false, false, false, false);
	
	?>
	</td>
</tr>

</table>

</td>
<td width="5%" valign="top">
<? 
/* Barra de estado atual e ações e Historico */
wf_desenhaBarraNavegacao( $docid, array() );
?>
</td>
</tr>
</table>



<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloEsquerda" colspan="2"><span style=font-size:large;>QUALIFICAÇÃO DOS PARTICIPANTES</span></td>
</tr>

	
<tr>
	<td colspan="2">
	<?
	
	$sql = "SELECT foo.pfldsc, count(*), sum(foo.doutorado) as doutorado, '<span style=float:right>'||round((sum(foo.doutorado)::numeric/count(*)::numeric)*100,2)||'</span>' as porc_doutorado, sum(foo.mestrado) as mestrado, '<span style=float:right>'||round((sum(foo.mestrado)::numeric/count(*)::numeric)*100,2)||'</span>' as porc_mestrado, sum(foo.especializacao) as especializacao, '<span style=float:right>'||round((sum(foo.especializacao)::numeric/count(*)::numeric)*100,2)||'</span>' as porc_especializacao, sum(foo.graduacao) as graduacao, '<span style=float:right>'||round((sum(foo.graduacao)::numeric/count(*)::numeric)*100,2)||'</span>' as porc_graduacao 
			FROM (
			SELECT p.pfldsc, 
				   p.pflnivel,
				   CASE WHEN i.foeid=".FOE_DOUTORADO." THEN 1 ELSE 0 END as doutorado, 
				   CASE WHEN i.foeid=".FOE_MESTRADO." THEN 1 ELSE 0 END as mestrado,
				   CASE WHEN i.foeid=".FOE_ESPECIALIZACAO." THEN 1 ELSE 0 END as especializacao,
				   CASE WHEN i.foeid IN(".FOE_SUPERIOR_COMPLETO_PEDAGOGIA.",".FOE_SUPERIOR_COMPLETO_LICENCIATURA.",".FOE_SUPERIOR_COMPLETO_OUTRO.") THEN 1 ELSE 0 END as graduacao 
			FROM sispacto2.identificacaousuario i 
			INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
			INNER JOIN sispacto2.pagamentoperfil pe ON pe.pflcod = p.pflcod
			WHERE i.uncid='".$_SESSION['sispacto2']['universidade']['uncid']."'
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

<?

$inscritos = $db->pegaUm("SELECT count(*) FROM sispacto2.identificacaousuario i 
			 			  INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd AND t.pflcod='".PFL_PROFESSORALFABETIZADOR."'
			 			  WHERE i.uncid='".$_SESSION['sispacto2']['universidade']['uncid']."' AND iuscpf not ilike 'SIS%'");

$vagasoferecidas = $db->pegaUm("
								SELECT SUM(foo.total) FROM (
								(
								SELECT total::integer FROM sispacto2.totalalfabetizadores  t 
								INNER JOIN sispacto2.abrangencia a ON a.muncod = t.cod_municipio AND a.esfera='M'
								INNER JOIN sispacto2.estruturacurso e ON e.ecuid = a.ecuid 
								WHERE e.uncid='".$_SESSION['sispacto2']['universidade']['uncid']."'
								) UNION ALL (
								SELECT total::integer FROM sispacto2.totalalfabetizadoresestadual  t 
								INNER JOIN sispacto2.abrangencia a ON a.muncod = t.cod_municipio AND a.esfera='E'
								INNER JOIN sispacto2.estruturacurso e ON e.ecuid = a.ecuid 
								WHERE e.uncid='".$_SESSION['sispacto2']['universidade']['uncid']."'
								)
								) foo");

$certificados = $db->pegaUm("SELECT count(*) FROM sispacto2.certificacao c 
							 INNER JOIN sispacto2.identificacaousuario i ON i.iusd = c.iusd 
							 WHERE i.uncid='".$_SESSION['sispacto2']['universidade']['uncid']."' AND c.pflcod='".PFL_PROFESSORALFABETIZADOR."' AND cerfrequencia>=75");

$evadidos = $db->pegaUm("SELECT count(*) FROM sispacto2.certificacao c
							 INNER JOIN sispacto2.identificacaousuario i ON i.iusd = c.iusd
							 WHERE i.uncid='".$_SESSION['sispacto2']['universidade']['uncid']."' AND c.pflcod='".PFL_PROFESSORALFABETIZADOR."' AND cerfrequencia<75");

$desistentes = $db->pegaUm("SELECT count(*) FROM sispacto2.certificacao c
							 INNER JOIN sispacto2.identificacaousuario i ON i.iusd = c.iusd
							 WHERE i.uncid='".$_SESSION['sispacto2']['universidade']['uncid']."' AND c.pflcod='".PFL_PROFESSORALFABETIZADOR."' AND cerfrequencia=0");

$freqmedia = $db->pegaUm("SELECT avg(cerfrequencia) FROM sispacto2.certificacao c
							 INNER JOIN sispacto2.identificacaousuario i ON i.iusd = c.iusd
							 WHERE i.uncid='".$_SESSION['sispacto2']['universidade']['uncid']."' AND c.pflcod='".PFL_PROFESSORALFABETIZADOR."'");





?>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloEsquerda" colspan="2"><span style=font-size:large;>ASPECTOS PEDAGÓGICOS</span></td>
</tr>
<tr>
	<td class="SubTituloEsquerda" width="40%">1. Quantidade de professores inscritos:</td><td><?=$inscritos ?> (<?=(($vagasoferecidas)?round(($inscritos/$vagasoferecidas)*100):'0')?>% das vagas oferecidas)</td>
</tr>
<tr>
	<td class="SubTituloEsquerda">2. Quantidade de vagas de professores alfabetizadores oferecidas:</td><td><?=$vagasoferecidas ?></td>
</tr>
<tr>
	<td class="SubTituloEsquerda">3. Quantidade de professores alfabetizadores certificados:</td><td><?=(($certificados)?$certificados:'0') ?> (<?=(($inscritos)?round(($certificados/$inscritos)*100):'0')?>% dos inscritos)</td>
</tr>
<tr>
	<td class="SubTituloEsquerda">4. Quantidade de professores alfabetizadores evadidos <span style="font-size:x-small;">(teve presença abaixo de 75%)</span>:</td><td><?=$evadidos ?> (<?=(($inscritos)?round(($evadidos/$inscritos)*100):'0')?>% dos inscritos)</td>
</tr>
<tr>
	<td colspan="2"><b>4.a. Justificativa mais comum para evasão</b><br><br>
	<?
	if($consulta) {
		echo nl2br($afcpedagogico_4_a);
	} else {
		echo campo_textarea( 'afcpedagogico_4_a', 'S', 'S', '', '85', '6', '3000');
	} 
	?>
	</td>
</tr>
<tr>
	<td class="SubTituloEsquerda">5. Quantidade de desistentes <span style="font-size:x-small;">(foi indicado no sistema, mas não teve nenhuma presença)</span>:</td><td><?=$desistentes ?> (<?=(($inscritos)?round(($desistentes/$inscritos)*100):'0')?>% dos inscritos)</td>
</tr>
<tr>
	<td class="SubTituloEsquerda">6. Frequencia total média dos professores alfabetizadores:</td><td><?=round($freqmedia) ?>% de frequência</td>
</tr>
</table>


<?

$qtdformador = $db->pegaUm("SELECT count(*) as n FROM sispacto2.identificacaousuario i 
							INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd 
							WHERE i.uncid='".$_SESSION['sispacto2']['universidade']['uncid']."' AND t.pflcod IN('".PFL_FORMADORIES."','".PFL_FORMADORIESP."')");

$qtdsupervisor = $db->pegaUm("SELECT count(*) as n FROM sispacto2.identificacaousuario i
							INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd
							WHERE i.uncid='".$_SESSION['sispacto2']['universidade']['uncid']."' AND t.pflcod IN('".PFL_SUPERVISORIES."')");

$qtdorientador = $db->pegaUm("SELECT count(*) as n FROM sispacto2.identificacaousuario i
							INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd
							WHERE i.uncid='".$_SESSION['sispacto2']['universidade']['uncid']."' AND t.pflcod IN('".PFL_ORIENTADORESTUDO."') AND i.iuscpf NOT ILIKE 'SIS%'");

$qtdprofessor = $db->pegaUm("SELECT count(*) as n FROM sispacto2.identificacaousuario i
							INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd
							WHERE i.uncid='".$_SESSION['sispacto2']['universidade']['uncid']."' AND t.pflcod IN('".PFL_PROFESSORALFABETIZADOR."') AND i.iuscpf NOT ILIKE 'SIS%'");



?>


<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloEsquerda" colspan="2"><span style=font-size:large;>RELAÇÃO ENTRE O NÚMERO DE PROFISSIONAIS</span></td>
</tr>
<tr>
	<td class="SubTituloEsquerda" width="40%">Formador/Supervisor:</td><td><?=(($qtdsupervisor)?round($qtdformador/$qtdsupervisor).' / 1':'NA') ?></td>
</tr>
<tr>
	<td class="SubTituloEsquerda">Orientador/Formador:</td><td><?=(($qtdformador)?round($qtdorientador/$qtdformador).' / 1':'NA') ?></td>
</tr>
<tr>
	<td class="SubTituloEsquerda">Professor Alfabetizador/Orientador:</td><td><?=(($qtdorientador)?round($qtdprofessor/$qtdorientador).' / 1':'NA') ?></td>
</tr>

</table>



<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloEsquerda" colspan="2"><span style=font-size:large;>ENCONTROS PRESENCIAIS</span></td>
</tr>
<tr>
	<td>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<? if(!$consulta) : ?>
	<tr>
		<td class="SubTituloDireita">Nome do evento:</td>
		<td><?=campo_texto('aofnome', "N", "S", "Nome do evento", 30, 60, "", "", '', '', 0, 'id="aofnome"') ?></td>
		<td class="SubTituloDireita">Data do evento:</td>
		<td><?=campo_data2('aofdata','S', 'S', 'Data do evento', 'S', '', '', '', '', '', 'aofdata') ?></td>
		<td class="SubTituloDireita">Carga horária:</td>
		<td><?=campo_texto('aofcargahoraria', "N", "S", "Carga horária", 6, 10, "######", "", '', '', 0, 'id="aofcargahoraria"') ?></td>
	</tr>
	<tr>
		<td class="SubTitulocentro" colspan="6">
		<input type="button" name="gravar" value="Inserir Evento" onclick="gravarEncontroPresencial();">
		</td>
	</tr>
	<? endif; ?>
	<tr>
		<td colspan="6" id="td_encontropresencialcoordenadories">
		<?
		carregarEncontroPresencialCoordenadorIES(array('consulta'=>$consulta));
		?>
		</td>
	</tr>
	</table>
	</td>
</tr>
</table>




<?
$arrLog = explode(";", $afclogistico);
if(!$arrLog) $arrLog = array();
?>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloEsquerda" colspan="2"><span style=font-size:large;>ASPECTOS LOGISTICOS</span></td>
</tr>
<tr>
	<td class="SubTituloEsquerda" width="40%">1. Utilizou o espaço físico da IES? (salas, auditório...)</td><td><input type="checkbox" name="afclogistico[]" value="1" <?=((in_array('1',$arrLog))?'checked':'') ?> <?=(($consulta)?'disabled':'') ?> ></td>
</tr>
<tr>
	<td class="SubTituloEsquerda">2. Utilizou espaço físico da secretária de educação estadual e/ou municipal? (salas, auditório...)</td><td><input type="checkbox" name="afclogistico[]" value="2" <?=((in_array('2',$arrLog))?'checked':'') ?> <?=(($consulta)?'disabled':'') ?> ></td>
</tr>
<tr>
	<td class="SubTituloEsquerda">3. Foi realizada locação de espaço?</td><td><input type="checkbox" name="afclogistico[]" value="3" <?=((in_array('3',$arrLog))?'checked':'') ?> <?=(($consulta)?'disabled':'') ?> ></td>
</tr>
<tr>
	<td class="SubTituloEsquerda">4. Locou equipamentos?</td><td><input type="checkbox" name="afclogistico[]" value="4" <?=((in_array('4',$arrLog))?'checked':'') ?> <?=(($consulta)?'disabled':'') ?> ></td>
</tr>
<tr>
	<td class="SubTituloEsquerda">5. Teve dificuldades nas contratações e/ou articulações?</td><td><input type="checkbox" name="afclogistico[]" value="5" <?=((in_array('5',$arrLog))?'checked':'teste') ?> <?=(($consulta)?'disabled':'') ?> ></td>
</tr>
<tr>
	<td colspan="2">
	<b>5.a. Principais dificuldades encontradas nas contrações e/ou articulações</b><br><br>
	<?
	if($consulta) {
		echo nl2br($afclogistico_5_a);
	} else {
		echo campo_textarea( 'afclogistico_5_a', 'S', 'S', '', '85', '6', '3000');
	} 
	?></td>
</tr>
</table>


<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloEsquerda" colspan="2"><span style=font-size:large;>ASPECTOS ORÇAMENTÁRIOS</span></td>
</tr>
	
<tr>
	<td colspan="2">
	<?
	carregarListaCustos(array('uncid' => $_SESSION['sispacto2']['universidade']['uncid'],'execucao'=> true, 'consulta' => $consulta));
	?>
	</td>
</tr>
</table>




<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloEsquerda" colspan="2"><span style=font-size:large;>ASPECTOS DO CONTEÚDO</span></td>
</tr>
<tr>
	<td class="SubTituloEsquerda">1. O conteúdo dos cadernos da formação atende ao proposto no curso?</td><td><input type="radio" name="afcconteudo_1" value="S" <?=(($afcconteudo_1=='S')?'checked':'') ?> <?=(($consulta)?'disabled':'') ?> > Sim <input type="radio" name="afcconteudo_1" value="N" <?=(($afcconteudo_1=='N')?'checked':'') ?> <?=(($consulta)?'disabled':'') ?> > Não</td>
</tr>
<tr>
	<td colspan="2"><b>1.a. Justifique caso a resposta seja "não"</b><br><br>
	<?
	if($consulta) {
		echo nl2br($afcconteudo_1_a);
	} else {
		echo campo_textarea( 'afcconteudo_1_a', 'S', 'S', '', '85', '6', '3000');
	} 
	?></td>
</tr>
<tr>
	<td class="SubTituloEsquerda">2. No decorrer do curso, foram feitas modificações quanto ao conteúdo dos cadernos de formação?</td><td><input type="radio" name="afcconteudo_2" value="S" <?=(($afcconteudo_2=='S')?'checked':'') ?> <?=(($consulta)?'disabled':'') ?> > Sim <input type="radio" name="afcconteudo_2" value="N" <?=(($afcconteudo_2=='N')?'checked':'') ?> <?=(($consulta)?'disabled':'') ?> > Não</td>
</tr>
<tr>
	<td colspan="2"><b>2.a. Cite, caso a resposta seja "sim", das principais modificações</b><br><br>
	<?
	if($consulta) {
		echo nl2br($afcconteudo_2_a);
	} else {
		echo campo_textarea( 'afcconteudo_2_a', 'S', 'S', '', '85', '6', '3000');
	} 
	?></td>
</tr>
<tr>
	<td class="SubTituloEsquerda" colspan="2">3. Se houver, aborde sobre aspectos do conteúdo que poderiam ser melhorados</td>
</tr>
<tr>
	<td colspan="2">
	<?
	if($consulta) {
		echo nl2br($afcconteudo_3);
	} else {
		echo campo_textarea( 'afcconteudo_3', 'S', 'S', '', '85', '6', '3000');
	} 
	?>
	</td>
</tr>
</table>


<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloEsquerda" colspan="2"><span style=font-size:large;>AÇÕES PEDAGÓGICAS E CONSIDERAÇÕES FINAIS</span></td>
</tr>
<tr>
	<td class="SubTituloEsquerda" colspan="2">1. Quais as principais estratégias de ensino implementadas pela IES ao curso (combinação de metodologia, conteúdo, recursos didáticos e procedimentos viabilizando a aprendizagem dos professores)?</td>
</tr>
<tr>
	<td colspan="2">
	<?
	if($consulta) {
		echo nl2br($afcpedagogico_1);
	} else {
		echo campo_textarea( 'afcpedagogico_1', 'S', 'S', '', '85', '6', '3000');
	} 
	?>
	</td>
</tr>
<tr>
	<td class="SubTituloEsquerda" colspan="2">2. Se houver, cite sugestões da melhoria para a próxima formação:</td>
</tr>
<tr>
	<td colspan="2">
	<?
	if($consulta) {
		echo nl2br($afcpedagogico_2);
	} else {
		echo campo_textarea( 'afcpedagogico_2', 'S', 'S', '', '85', '6', '3000');
	} 
	?>
	</td>
</tr>
<tr>
	<td class="SubTituloEsquerda" colspan="2">3. Quais os meios para atender a assistência ao cursista (promoção de condições objetivas para frequência às aulas)?</td>
</tr>
<tr>
	<td colspan="2">
	<?
	if($consulta) {
		echo nl2br($afcpedagogico_3);
	} else {
		echo campo_textarea( 'afcpedagogico_3', 'S', 'S', '', '85', '6', '3000');
	} 
	?>
	</td>
</tr>
<tr>
	<td class="SubTituloEsquerda" colspan="2">4. Dos professores concluintes do curso, houve relatos quanto à melhoria e inovação em suas atividades em sala de aula? Se sim, cite os relatos mais comuns de forma sucinta:</td>
</tr>
<tr>
	<td colspan="2">
	<?
	if($consulta) {
		echo nl2br($afcpedagogico_4);
	} else {
		echo campo_textarea( 'afcpedagogico_4', 'S', 'S', '', '85', '6', '3000');
	} 
	?>
	</td>
</tr>
<tr>
	<td class="SubTituloEsquerda" colspan="2">5. Considerações finais:</td>
</tr>
<tr>
	<td colspan="2">
	<?
	if($consulta) {
		echo nl2br($afcconsideracoesfinais_4);
	} else {
		echo campo_textarea( 'afcconsideracoesfinais_4', 'S', 'S', '', '85', '6', '3000');
	} 
	?>
	</td>
</tr>
<? if(!$consulta) : ?>
<tr>
	<td class="SubTituloCentro" colspan="2"><input type="button" name="salvar" value="Salvar" onclick="salvarAvaliacaoFinal();"></td>
</tr>
<? endif; ?>
</table>
</form>