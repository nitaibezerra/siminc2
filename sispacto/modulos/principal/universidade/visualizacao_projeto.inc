<?

$esdid = $db->pegaUm("SELECT esdid FROM workflow.documento WHERE docid='".$_SESSION['sispacto']['universidade']['docid']."'");

if($esdid == ESD_ELABORACAO_COORDENADOR_IES) {
	if(!$db->testa_superuser() || !$versao_html) verificarValidacaoVisualizacaoProjeto(array("uncid" => $_SESSION['sispacto']['universidade']['uncid']));
}
?>
<!-- DADOS CURSO  -->
<?
$info = carregarCadastroIESProjeto(array("uncid"=>$_SESSION['sispacto']['universidade']['uncid']));
extract($info['universidade']);
extract($info['curso']);

if($versao_html) {
	echo '<script language="JavaScript" src="../includes/funcoes.js"></script>';
	echo '<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>';
	echo '<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>';
	echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>';
	echo '<script language="javascript" type="text/javascript" src="./js/sispacto.js"></script>';
}

?>
<div id="modalVersoes" style="display:none;">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td id="listaversoes"></td>
</tr>


</table>
</div>
<script>

function visualizarVersao(vpnid) {
	window.open('sispacto.php?modulo=principal/universidade/universidade&acao=A&requisicao=visualizarVersao&vpnid='+vpnid,'Versão','scrollbars=yes,height=720,width=1024,status=no,toolbar=no,menubar=no,location=no');
}

function abrirVersoes(uncid) {

	divCarregando();
	
	ajaxatualizar('requisicao=carregarVersoes&uncid='+uncid,'listaversoes');
	
	jQuery("#modalVersoes").dialog({
	                        draggable:true,
	                        resizable:true,
	                        width: 1024,
	                        height: 720,
	                        modal: true,
	                     	close: function(){} 
	                    });
	                    
	divCarregado();
}

jQuery(document).ready(function() {
	jQuery("[id^='btn_turma_']").each(function() {
		jQuery(this).click();
	});

	jQuery("[id^='abran_']").each(function() {
		jQuery(this).css('display','none');
	});

});
</script>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloCentro" colspan="3">Visualização do Projeto</td>
	</tr>
	<? if(!$versao_html) : ?>
	<tr>
		<td class="SubTituloDireita" width="20%">Orientações</td>
		<td colspan="2"><? echo carregarOrientacao($_SERVER['REQUEST_URI']); ?></td>
	</tr>
	<? endif; ?>
	<tr>
		<td class="SubTituloEsquerda" colspan="3">Dados da Instituição</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">CNPJ</td>
		<td><?=mascaraglobal($unicnpj,"##.###.###/####-##") ?></td>
		<? if(!$versao_html) : ?>
		<td align="center" valign="top" rowspan="6">
		<?
		/* Barra de estado atual e ações e Historico */
		wf_desenhaBarraNavegacao( $_SESSION['sispacto']['universidade']['docid'], array('uncid' => $_SESSION['sispacto']['universidade']['uncid']) );
		?>
		<table width="100%" class="listagem">
		<tr>
		<td class="SubTituloCentro" style="cursor:pointer;" onclick="abrirVersoes('<?=$_SESSION['sispacto']['universidade']['uncid'] ?>');">Versões HTML</td>
		</tr>
		</table>
		</td>
		<? endif; ?>

	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Nome da Instituição</td>
		<td><?=$uninome ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Sigla</td>
		<td><?=$unisigla ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Endereço</td>
		<td>
			<table cellSpacing="1" cellPadding="3" width="100%">
				<tr>
					<td align="right" width="10%">CEP</td>
					<td><?=mascaraglobal($unicep,"#####-###") ?></td>
				</tr>
				<tr>
					<td align="right">UF</td>
					<td><?=(($uniuf)?$uniuf:"Digite CEP") ?></td>
				</tr>
				<tr>
					<td align="right">Município</td>
					<td id="td_municipio_dirigente"><?=(($muncod)?$mundescricao:"Digite CEP") ?></td>
				</tr>
				<tr>
					<td align="right">Logradouro</td>
					<td><?=$unilogradouro ?></td>
				</tr>
				<tr>
					<td align="right">Bairro</td>
					<td><?=$unibairro ?></td>
				</tr>
				<tr>
					<td align="right">Complemento</td>
					<td><?=$unicomplemento ?></td>
				</tr>
				<tr>
					<td align="right">Número</td>
					<td><?=$uninumero ?></td>
				</tr>
			</table>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Contato</td>
		<td>
			<table cellSpacing="1" cellPadding="3" width="100%">
				<tr>
					<td align="right" width="10%">Telefone</td>
					<td><?=$unidddcomercial ?> <?=$uninumcomercial ?></td>
				</tr>
				<tr>
					<td align="right" width="10%">E-mail</td>
					<td><?=$uniemail ?></td>
				</tr>
				<tr>
					<td align="right" width="10%">Site</td>
					<td><?=$unisite ?></td>
				</tr>
			</table>
		</td>
	</tr>
	
	<tr>
		<td class="SubTituloEsquerda" colspan="2">Dados do Dirigente</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">CPF</td>
		<td><?=mascaraglobal($reicpf,"###.###.###-##") ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Nome</td>
		<td><?=$reinome ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Telefone</td>
		<td><?=$reidddcomercial ?> <?=$reinumcomercial ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">E-mail</td>
		<td><?=$reiemail ?></td>
	</tr>
	<tr>
		<td class="SubTituloEsquerda" colspan="2">Dados do Projeto</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Curso</td>
			<td><?=$curid." - ".$curdesc ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Objetivo do Curso</td>
		<td><?=$curobjetivo ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Descrição do Curso</td>
		<td><?=$curementa ?></td>
	</tr>	
	
	<tr>
		<td class="SubTituloDireita" width="20%">Meta física e Financeira</td>
		<td>
		<?
		
		$orientadorestudo = totalAlfabetizadoresAbrangencia(array("uncid" => $_SESSION['sispacto']['universidade']['uncid']));
		
		$sql = "SELECT SUM(total) FROM sispacto.totalalfabetizadores t 
				INNER JOIN sispacto.abrangencia ap ON ap.muncod = t.cod_municipio
		 		INNER JOIN sispacto.estruturacurso e ON e.ecuid = ap.ecuid 
				WHERE e.uncid='".$_SESSION['sispacto']['universidade']['uncid']."'";
		
		$professoralfabetizador = $db->pegaUm($sql);
		?>
		<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
			<tr>
				<td class="SubTituloDireita">Orientador de Estudo Formado e Certificado</td>
				<td><?=(($orientadorestudo)?round($orientadorestudo*0.8):"0") ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Professor alfabetizador certificado</td>
				<td><?=(($professoralfabetizador)?round($professoralfabetizador*0.6):"0") ?></td>
			</tr>
		</table>
		</td>
	</tr>

	<tr>
		<td class="SubTituloDireita" width="20%">Vigência do projeto</td>
		<td>
			<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" width="100%">
				<tr>
					<td class="SubTituloCentro">Início</td>	
					<td class="SubTituloCentro">Término</td>			
				</tr>
				<tr>
					<td>
						<?=formata_data($uncdatainicioprojeto) ?>
					</td>
					<td>
						<?=formata_data($uncdatafimprojeto) ?>
					</td>
				</tr>
			</table>
		</td>
	</tr>	

	<tr>
		<td class="SubTituloDireita" width="20%">Público Alvo</td>
		<td>
		<?
		$sql = "select fexdesc from catalogocurso.funcaoexercida_curso_publicoalvo fcp
				inner join catalogocurso.funcaoexercida fex on fex.fexid = fcp.fexid and fex.fexstatus = 'A'
				where curid=".$curid;
		
		$db->monta_lista_simples($sql,array(),50,5,'N','100%',$par2);
		?>
		</td>
	</tr>		
	<tr>
		<td class="SubTituloDireita" width="20%">Requisitos para Participação</td>
		<td>
		<?
		$sql = "SELECT tee.no_etapa_ensino
				FROM catalogocurso.etapaensino_curso eec
				left join catalogocurso.etapaensino ee on ee.eteid = eec.eteid
				left join educacenso_2010.tab_etapa_ensino tee on tee.pk_cod_etapa_ensino = eec.cod_etapa_ensino
				where curid =".$curid;
		
		$db->monta_lista_simples($sql,array(),50,5,'N','100%',$par2);
		
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Tipo de Ação Orçamentária</td>
		<td><? 
		$_TIPO_AO = array("A"=>"Ação Orçamentária",
						  "D"=>"Descentralização",
						  "C"=>"Convênio");
		echo $_TIPO_AO[$unctipo];
		?></td>
	</tr>	
	<tr>
		<td class="SubTituloDireita" width="20%">Carga Horária</td>
		<td>
			<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" width="100%">
				<tr>
					<td class="SubTituloCentro">Minimo</td>	
					<td class="SubTituloCentro">Máximo</td>			
				</tr>
				<tr>
					<td><?=$curchmim ?></td>
					<td><?=$curchmax ?></td>
				</tr>
			</table>
		</td>		
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Tipo de Certificação</td>
		<td><?
		$_TIPO_CT = array("E"=>"Extensão", "A"=>"Aperfeiçoamento");
		echo $_TIPO_CT[$unctipocertificacao];
		?></td>
	</tr>
</table>

<!-- FIM - DADOS CURSO -->


<!-- ESTRUTURA CURSO -->
<?

$_SESSION['sispacto']['universidade']['ecuid'] = pegarEstruturaCurso(array("uncid" => $_SESSION['sispacto']['universidade']['uncid']));

$estruturacurso = carregarEstruturaCurso(array("ecuid"=>$_SESSION['sispacto']['universidade']['ecuid']));
$articulacaoinstitucional = carregarArticulacaoInstitucional(array("ecuid"=>$_SESSION['sispacto']['universidade']['ecuid']));

if($estruturacurso) {
	extract($estruturacurso);
}

if($articulacaoinstitucional) {
	extract($articulacaoinstitucional);
}

?>
<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>

<script>

function carregarSubAtividades(atiid) {
	ajaxatualizar('requisicao=carregarSubAtividades&atiid='+atiid,'td_subatividade');
}

function abrirDetalhamentoAbrangencia(muncod, esfera, obj) {

	var tabela = obj.parentNode.parentNode.parentNode.parentNode;
	var linha = obj.parentNode.parentNode.parentNode;
	
	if(obj.title=="mais") {
		obj.title    = "menos";
		obj.src      = "../imagens/menos.gif";
		var nlinha   = tabela.insertRow(linha.rowIndex);
		var ncol     = nlinha.insertCell(0);
		ncol.colSpan = 5;
		ncol.id      = 'coluna_'+nlinha.rowIndex;
		ajaxatualizar('requisicao=carregarDetalhamentoAbrangencia&ecuid=<?=$_SESSION['sispacto']['universidade']['ecuid'] ?>&muncod='+muncod+'&esfera='+esfera,'coluna_'+nlinha.rowIndex);
	} else {
		obj.title    = "mais";
		obj.src      = "../imagens/mais.gif";
		tabela.deleteRow(linha.rowIndex);
	}
	 
}

</script>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloCentro" colspan="2">Estrutura do Curso</td>
	</tr>
	<?
	$dadoscurso = carregarCurso(array("curid"=>$_SESSION['sispacto']['universidade']['curid']));
	$cursonome = $dadoscurso['curid']." - ".$dadoscurso['curdesc']; 
	?>
	<tr>
		<td class="SubTituloDireita" width="20%">Curso</td>
		<td><?=$cursonome ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Sede do Curso</td>
		<td>
			<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
				<tr>
					<td align="right" width="20%">UF</td>
					<td><?=$estuf ?></td>
				</tr>
				<tr>
					<td align="right" width="20%">Município</td>
					<td id="td_municipio3">
					<? 
					if($estuf) : 
						$sql = "SELECT mundescricao as descricao FROM territorios.municipio WHERE muncod='".$muncod."' ORDER BY mundescricao";
						echo $db->pegaUm($sql);
					else :
						echo "Selecione UF";
					endif;
					?>
					</td>
				</tr>
			</table>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Abrangência</td>
		<td>
		<div id="div_abrangencia">
		<? definirAbrangencia(array("consulta"=>true,"ecuid"=>$_SESSION['sispacto']['universidade']['ecuid'],"uncid"=>$_SESSION['sispacto']['universidade']['uncid'])); ?>
		</div>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Plano de Atividades</td>
		<td>

		<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
		<tr>
			<td class="SubTituloCentro" colspan="2">Cronograma de atividades</td>
		</tr>
		<tr>
			<td colspan="2" id="td_planoatividades">
			<? carregarPlanoAtividades(array("consulta"=>true,"ecuid"=>$_SESSION['sispacto']['universidade']['ecuid'])); ?>
			</td>
		</tr>
		</table>
		</td>
	</tr>
	
	<tr>
		<td class="SubTituloDireita" width="20%">Articulação Institucional</td>
		<td>
			<table>
				<tr>
					<td>Foi feita articulação com a SEDUC?</td>
					<td><?=(($ainseduc=="t")?"Sim":"") ?><?=(($ainseduc=="f")?"Não":"") ?></td>
				</tr>
				<tr>
					<td colspan="2">Comente<br><?=$ainseducjustificativa ?></td>
				</tr>
				<tr>
					<td>Foi feita articulação com a UNDIME?</td>
					<td><?=(($ainundime=="t")?"Sim":"") ?><?=(($ainundime=="f")?"Não":"") ?></td>
				</tr>
				<tr>
					<td colspan="2">Comente<br><?=$ainundimejustificativa ?></td>
				</tr>
				<tr>
					<td>Foi feita articulação com a UNCME?</td>
					<td><?=(($ainuncme=="t")?"Sim":"") ?><?=(($ainuncme=="f")?"Não":"") ?></td>
				</tr>
				<tr>
					<td colspan="2">Comente<br><?=$ainuncmejustificativa ?></td>
				</tr>
			</table>
		</td>
	</tr>
</table>

<!-- ESTRUTURA CURSO -->


<!-- RECURSOS HUMANOS -->

<? 
$_SESSION['sispacto']['universidade']['ecuid'] = pegarEstruturaCurso(array("uncid" => $_SESSION['sispacto']['universidade']['uncid'])); 
?>

<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>

<script>
function abrirDocumentosRecursosHumanos(iusd) {
	window.open('sispacto.php?modulo=principal/universidade/inserirdocumento&acao=A&iusd='+iusd,'Documento','scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no');
}

</script>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloCentro" colspan="2">Equipe IES</td>
	</tr>
	<?
	$dadoscurso = carregarCurso(array("curid"=>$_SESSION['sispacto']['universidade']['curid']));
	$cursonome = $dadoscurso['curid']." - ".$dadoscurso['curdesc']; 
	?>
	<tr>
		<td class="SubTituloDireita" width="20%">Curso</td>
		<td><?=$cursonome ?></td>
	</tr>
	<tr>
		<td colspan="2" id="td_equipeRecursosHumanos">
		<?=carregarEquipeRecursosHumanos(array("consulta"=>true,"uncid" => $_SESSION['sispacto']['universidade']['uncid'])); ?>
		</td>
	</tr>
</table>
<!-- FIM - RECURSOS HUMANOS -->

<!-- ORÇAMENTO -->
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloCentro" colspan="2">Orçamento</td>
	</tr>
	<?
	$dadoscurso = carregarCurso(array("curid"=>$_SESSION['sispacto']['universidade']['curid']));
	$cursonome = $dadoscurso['curid']." - ".$dadoscurso['curdesc']; 
	?>
	<tr>
		<td class="SubTituloDireita" width="20%">Curso</td>
		<td><?=$cursonome ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Itens Financiáveis</td>
		<td>
		<div id="div_listacustos"><?
		carregarListaCustos(array("consulta"=>true,"uncid"=>$_SESSION['sispacto']['universidade']['uncid']));
		?></div>
		</td>
	</tr>
</table>
<!-- FIM - ORÇAMENTO -->

<!-- TURMAS -->
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloCentro" colspan="2">Turmas</td>
	</tr>
	<tr>
		<td colspan="2" id="td_turmas">
		<?=carregarTurmas(array("consulta"=>true,"uncid"=>$_SESSION['sispacto']['universidade']['uncid'])) ?>
		</td>
	</tr>
</table>
<!-- FIM - TURMAS -->

<!-- ASSINATURAS -->
<?
$sql = "SELECT re.reinome as reitor, re.reicpf as reitorcpf, COALESCE((SELECT iusnome FROM sispacto.identificacaousuario i INNER JOIN sispacto.tipoperfil t ON i.iusd=t.iusd WHERE i.uncid=u.uncid AND t.pflcod=".PFL_COORDENADORIES."),'Coordenador IES não cadastrado') as coordenadories, COALESCE((SELECT iuscpf FROM sispacto.identificacaousuario i INNER JOIN sispacto.tipoperfil t ON i.iusd=t.iusd WHERE i.uncid=u.uncid AND t.pflcod=".PFL_COORDENADORIES."),'Coordenador IES não cadastrado') as coordenadoriescpf 
		FROM sispacto.universidadecadastro u 
		INNER JOIN sispacto.universidade su ON su.uniid = u.uniid
		LEFT JOIN sispacto.reitor re on re.uniid = su.uniid 
		WHERE u.uncstatus='A' AND u.uncid='".$_SESSION['sispacto']['universidade']['uncid']."'";

$assinaturas = $db->pegaLinha($sql);

?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td align="center"><br><br><br>_____________________________________________________________________________________________</td>
	</tr>
	<tr>
		<td align="center"><?=$assinaturas['coordenadories'] ?> - <?=mascaraglobal($assinaturas['coordenadoriescpf'],"###.###.###-##") ?><br/>Coordenador-Geral da IES</td>
	</tr>
	<tr>
		<td align="center"><br><br><br>_____________________________________________________________________________________________</td>
	</tr>
	<tr>
		<td align="center"><?=$assinaturas['reitor'] ?> - <?=mascaraglobal($assinaturas['reitorcpf'],"###.###.###-##") ?><br/>Dirigente da IES</td>
	</tr>
	<?
	$sql = "SELECT * FROM workflow.documento d 
			INNER JOIN workflow.estadodocumento e ON e.esdid = d.esdid 
			INNER JOIN workflow.historicodocumento h ON h.hstid = d.hstid 
			LEFT JOIN workflow.comentariodocumento c ON c.hstid = d.hstid 
			LEFT JOIN seguranca.usuario u ON u.usucpf = h.usucpf
			WHERE d.docid='".$_SESSION['sispacto']['universidade']['docid']."'";
	$estado = $db->pegaLinha($sql);
	
	if($estado['esdid']==ESD_VALIDADO_COORDENADOR_IES) :
	?>
	<tr>
		<td align="center">
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="SubTituloDireita" width="20%">Parecer MEC</td>
			<td><?=$estado['cmddsc'] ?></td>
		</tr>
		</table>
		</td>
	</tr>
	<tr>
		<td align="center"><br><br><br>_____________________________________________________________________________________________</td>
	</tr>
	<tr>
		<td align="center"><?=$estado['usunome'] ?> - <?=mascaraglobal($estado['usucpf'],"###.###.###-##") ?><br/>Equipe MEC</td>
	</tr>

	<?
	endif;
	?>	

</table>
<!-- FIM - ASSINATURAS -->

<?
$mudancatrocas = carregarMudancasTroca(array('uncid'=>$_SESSION['sispacto']['universidade']['uncid'])); 
if($mudancatrocas[0]) { 
	echo "<p align=center><b>Registro de Substituições</b></p>";
	$cabecalho = array("Ação","Membro substituído","Membro novo","Saiu da turma","Foi para turma","Perfil","Usuário Responsável","Data da troca");
	$db->monta_lista_simples($mudancatrocas,$cabecalho,1000,5,'N','95%',$par2);

}

$erro = validarEnvioAnaliseMEC();
?>
<? if($erro !== true) : ?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloCentro">Pendências</td>
	</tr>
	<tr>
		<td style="color:red;"><?=str_replace(array('\n'),array(""),$erro) ?></td>
	</tr>
</table>
<? endif; ?>