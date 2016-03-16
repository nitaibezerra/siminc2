<?
$info = carregarCadastroIESProjeto(array("uncid"=>$_SESSION['sispacto3']['universidade']['uncid']));
extract($info['universidade']);
extract($info['curso']);
?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>

<script type="text/javascript" src="/includes/prototype.js"></script>
<script language="javascript" type="text/javascript" src="../includes/webservice/pj.js" /></script>
<script language="javascript" type="text/javascript" src="../includes/webservice/cpf.js" /></script>


<script>

jQuery(document).ready(function() {
	jQuery("[id^='btn_turma_']").each(function() {
		jQuery(this).click();
	});

});

</script>


<!-- DADOS PROJETO -->

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloCentro" colspan="2">Dados Instituição/Projeto</td>
	</tr>
	<tr>
		<td colspan="2">
		
	<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
	<tr>
		<td class="SubTituloEsquerda" colspan="2">Dados da Instituição</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">CNPJ</td>
		<td><?=mascaraglobal($unicnpj,"##.###.###/####-##") ?></td>
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
					<td><?=$uniuf ?></td>
				</tr>
				<tr>
					<td align="right">Município</td>
					<td><?=$mundescricao ?></td>
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
		<td class="SubTituloDireita" width="20%">Meta física</td>
		<td>
		<?
		
		$orientadorestudo = totalAlfabetizadoresAbrangencia(array("uncid" => $_SESSION['sispacto3']['universidade']['uncid']));
		
		$sql = "SELECT SUM(total) FROM sispacto3.totalalfabetizadores t 
				INNER JOIN sispacto3.abrangencia ap ON ap.muncod = t.cod_municipio
		 		INNER JOIN sispacto3.estruturacurso e ON e.ecuid = ap.ecuid 
				WHERE e.uncid='".$_SESSION['sispacto3']['universidade']['uncid']."'";
		
		$professoralfabetizador = $db->pegaUm($sql);
		?>
		<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
			<tr>
				<td class="SubTituloDireita" width="30%">Orientador de Estudo Formado e Certificado</td>
				<td align="right"><?=(($orientadorestudo)?round($orientadorestudo*0.8):"0") ?> <span style="cursor:pointer;font-size:xx-small;" onclick="exibirInformacoes('Orientadores de estudo - A meta física representa um percentual sobre o número total de Orientadores de Estudo das redes alcançadas pelo projeto da IES. Este número só será exibido depois que for definida a área de abrangência do projeto, que será indicada na tela seguinte (Estrutura de curso). O valor exibido corresponde a 80% do somatório de Orientadores de Estudo da área de abrangência. Por exemplo: se a IES irá atuar em 100 municípios que, juntos, somam 300 Orientadores de Estudo, a meta física neste caso corresponderá a 300 x 0,8 = 240 Orientadores de Estudo formados e certificados.');"><img src="../imagens/ajuda.png" width="10" height="10"> Entenda este número</span></td>
			</tr>
			<tr>
				<td class="SubTituloDireita" width="30%">Professor alfabetizador certificado</td>
				<td align="right"><?=(($professoralfabetizador)?round($professoralfabetizador*0.6):"0") ?> <span style="cursor:pointer;font-size:xx-small;" onclick="exibirInformacoes('Professores Alfabetizadores - A meta física representa um percentual sobre o número total de turmas de alfabetização das redes alcançadas pelo projeto da IES. Este número só será exibido depois que for definida a área de abrangência do projeto, que será indicada na tela seguinte (Estrutura de curso). O valor exibido corresponde a 60% do somatório das turmas de alfabetização da área de abrangência (de acordo com o Censo Escolar preliminar de 2012). Por exemplo: se a IES irá atuar em 100 municípios que, juntos, somam 6.000 turmas de alfabetização, a meta física neste caso corresponderá a 6.000 x 0,6 = 3.600 professores alfabetizadores certificados.');"><img src="../imagens/ajuda.png" width="10" height="10"> Entenda este número</span></td>
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
					<td><?=formata_data($uncdatainicioprojeto) ?></td>
					<td><?=formata_data($uncdatafimprojeto) ?></td>
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
		<td class="SubTituloDireita" width="20%">Origem dos recursos</td>
		<td><? 
		$_TIPO_AO = array("A" => "Ação Orçamentária",
					  	  "D" => "Descentralização",
						  "C" => "Convênio",
						  "P" => "Plano de Ações Articuladas - PAR");

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
		 	
		$_TIPO_CT = array("E" => "Extensão",
						  "A" => "Aperfeiçoamento");

		echo $_TIPO_CT[$unctipocertificacao];
		 
		?></td>
	</tr>
	
	</table>
	
	</td>
	
	</tr>
</table>

<!-- FIM DADOS PROJETO -->


<?

$_SESSION['sispacto3']['universidade']['ecuid'] = pegarEstruturaCurso(array("uncid" => $_SESSION['sispacto3']['universidade']['uncid']));

$estruturacurso = carregarEstruturaCurso(array("ecuid"=>$_SESSION['sispacto3']['universidade']['ecuid']));
$articulacaoinstitucional = carregarArticulacaoInstitucional(array("ecuid"=>$_SESSION['sispacto3']['universidade']['ecuid']));
$info = carregarCadastroIESProjeto(array("uncid"=>$_SESSION['sispacto3']['universidade']['uncid']));

if($estruturacurso) {
	extract($estruturacurso);
}

if($articulacaoinstitucional) {
	extract($articulacaoinstitucional);
}

?>

<script>

function abrirDetalhamentoAbrangencia(muncod, esfera, obj) {

	var tabela = obj.parentNode.parentNode.parentNode.parentNode;
	var linha = obj.parentNode.parentNode.parentNode;
	
	if(obj.title=="mais") {
		obj.title    = "menos";
		obj.src      = "../imagens/menos.gif";
		var nlinha   = tabela.insertRow(linha.rowIndex);
		var ncol     = nlinha.insertCell(0);
		ncol.colSpan = 8;
		ncol.id      = 'coluna_'+nlinha.rowIndex;
		ajaxatualizar('requisicao=carregarDetalhamentoAbrangencia&ecuid=<?=$_SESSION['sispacto3']['universidade']['ecuid'] ?>&muncod='+muncod+'&esfera='+esfera,'coluna_'+nlinha.rowIndex);
	} else {
		obj.title    = "mais";
		obj.src      = "../imagens/mais.gif";
		tabela.deleteRow(linha.rowIndex);
	}
	 
}


</script>

<!-- ESTRUTURA DO CURSO -->

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloCentro" colspan="2">Estrutura do Curso</td>
	</tr>
	<?
	$dadoscurso = carregarCurso(array("curid"=>$_SESSION['sispacto3']['universidade']['curid']));
	$cursonome = $dadoscurso['curid']." - ".$dadoscurso['curdesc']; 
	?>
	<tr>
	
	<td colspan="2">
	
	<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
	<tr>
		<td class="SubTituloDireita" width="20%">Curso</td>
		<td><?=$cursonome ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Sede do Curso</td>
		<td>
			<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
				<tr>
					<td align="right" width="10%">UF</td>
					<td><?=$estuf ?></td>
				</tr>
				<tr>
					<td align="right" width="10%">Município</td>
					<td>
					<? 
					if($muncod) :
					 
						$sql = "SELECT mundescricao as descricao FROM territorios.municipio WHERE muncod='{$muncod}' ORDER BY mundescricao";
						$mundescricao = $db->pegaUm($sql);
						echo $mundescricao;
						
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
		<? definirAbrangencia(array("consulta"=>true,"ecuid"=>$_SESSION['sispacto3']['universidade']['ecuid'],"uncid"=>$_SESSION['sispacto3']['universidade']['uncid'])); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Plano de Atividades</td>
		<td>
		<? carregarPlanoAtividades(array("ecuid"=>$_SESSION['sispacto3']['universidade']['ecuid'],"consulta"=>true)); ?>
		<br/>
		<p><b>Comentários sobre o cronograma (plano de atividades):</b></p>
		<?=nl2br($ecuobsplanoatividades) ?>
		</td>
	</tr>
	
	<tr>
		<td class="SubTituloDireita" width="20%">Articulação Institucional</td>
		<td>
			<table>
				<tr>
					<td>Foi feita articulação com a SEDUC?</td>
					<td><input type="radio" name="ainseduc" value="TRUE" <?=(($ainseduc=="t")?"checked":"") ?>> Sim <input type="radio" name="ainseduc" value="FALSE" <?=(($ainseduc=="f")?"checked":"") ?>> Não</td>
				</tr>
				<tr>
					<td colspan="2"><b>Comente</b><br><?=nl2br($ainseducjustificativa); ?></td>
				</tr>
				<tr>
					<td>Foi feita articulação com a UNDIME?</td>
					<td><input type="radio" name="ainundime" value="TRUE" <?=(($ainundime=="t")?"checked":"") ?>> Sim <input type="radio" name="ainundime" value="FALSE" <?=(($ainundime=="f")?"checked":"") ?>> Não</td>
				</tr>
				<tr>
					<td colspan="2"><b>Comente</b><br><?=nl2br($ainundimejustificativa) ?></td>
				</tr>
				<tr>
					<td>Foi feita articulação com a UNCME?</td>
					<td><input type="radio" name="ainuncme" value="TRUE" <?=(($ainuncme=="t")?"checked":"") ?>> Sim <input type="radio" name="ainuncme" value="FALSE" <?=(($ainuncme=="f")?"checked":"") ?>> Não</td>
				</tr>
				<tr>
					<td colspan="2"><b>Comente</b><br><?=nl2br($ainuncmejustificativa) ?></td>
				</tr>
			</table>
		</td>
	</tr>
	</table>
	
	</td>
	
	</tr>

</table>

<!-- FIM ESTRUTURA CURSO -->


<!-- ORCAMENTO -->
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloCentro" colspan="2">Orçamento</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Itens Financiáveis</td>
		<td>
		<table width="100%">
		<tr>
			<td valign="top">
			<div id="div_listacustos"><?
			carregarListaCustos(array("consulta"=>true,"uncid"=>$_SESSION['sispacto3']['universidade']['uncid']));
			?></div>
			</td>
		</tr>
		</table>
		</td>
	</tr>
</table>

<!-- FIM ORCAMENTO -->


<!-- EQUIPE IES -->

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloCentro" colspan="2">Equipe IES</td>
	</tr>
	<tr>
		<td colspan="2">
		
		<table width="100%">
		<tr>
			<td valign="top">
			<?=carregarEquipeRecursosHumanos(array("consulta"=>true,"uncid" => $_SESSION['sispacto3']['universidade']['uncid'])); ?>
			</td>
		</tr>
		</table>
		
		
		</td>
	</tr>

</table>



<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloCentro" colspan="2">Turmas</td>
	</tr>
	<tr>
	
	<td colspan="2">

	<table width="100%">
	<tr>
		<td colspan="2">
		<?=carregarTurmas(array("consulta"=>true,"uncid"=>$_SESSION['sispacto3']['universidade']['uncid'],"pflcodturma"=>PFL_FORMADORIES)) ?>
		</td>

	</tr>
	<tr>
		<td colspan="2">
		<?=carregarTurmas(array("consulta"=>true,"uncid"=>$_SESSION['sispacto3']['universidade']['uncid'],"pflcodturma"=>PFL_SUPERVISORIES)) ?>
		</td>

	</tr>
	<tr>
		<td colspan="2">
		<?=carregarTurmas(array("consulta"=>true,"uncid"=>$_SESSION['sispacto3']['universidade']['uncid'],"pflcodturma"=>PFL_COORDENADORADJUNTOIES)) ?>
		</td>

	</tr>
	</table>
	
	</td>
	
	</tr>

</table>

<!-- ASSINATURAS -->
<?
$sql = "SELECT re.reinome as reitor, re.reicpf as reitorcpf, COALESCE((SELECT iusnome FROM sispacto3.identificacaousuario i INNER JOIN sispacto3.tipoperfil t ON i.iusd=t.iusd WHERE i.uncid=u.uncid AND t.pflcod=".PFL_COORDENADORIES."),'Coordenador IES não cadastrado') as coordenadories, COALESCE((SELECT iuscpf FROM sispacto3.identificacaousuario i INNER JOIN sispacto3.tipoperfil t ON i.iusd=t.iusd WHERE i.uncid=u.uncid AND t.pflcod=".PFL_COORDENADORIES."),'Coordenador IES não cadastrado') as coordenadoriescpf 
		FROM sispacto3.universidadecadastro u 
		INNER JOIN sispacto3.universidade su ON su.uniid = u.uniid
		LEFT JOIN sispacto3.reitor re on re.uniid = su.uniid 
		WHERE u.uncstatus='A' AND u.uncid='".$_SESSION['sispacto3']['universidade']['uncid']."'";

$assinaturas = $db->pegaLinha($sql);

?>
<form method="post" id="formulario">
<input type="hidden" name="requisicao" value="atualizarParecer">
<input type="hidden" name="uncid" value="<?=$_SESSION['sispacto3']['universidade']['uncid'] ?>">

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

	$parecer = $db->pegaLinha("SELECT uncparecer, 
									  replace(to_char(u.usucpf::numeric, '000:000:000-00'), ':', '.') as usucpf, 
									  u.usunome, 
									  to_char(uncparecerdata,'dd/mm/YYYY HH24:MI') as uncparecerdata 
							   FROM sispacto3.universidadecadastro uc 
							   LEFT JOIN seguranca.usuario u ON u.usucpf = uc.usucpfparecer 
							   WHERE uc.uncid='".$_SESSION['sispacto3']['universidade']['uncid']."'");
	
	$perfis = pegaPerfilGeral();
	
	if(!$perfis) $perfis = array();
	
	if(!$versao_html && ($db->testa_superuser() || in_array(PFL_EQUIPEMEC,$perfis) || in_array(PFL_ADMINISTRADOR,$perfis))) :
	?>
	<tr>
		<td align="center">
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="SubTituloDireita" width="20%">Responsável MEC</td>
			<td><?=mascaraglobal($_SESSION['usucpf'],"###.###.###-##")." - ".$_SESSION['usunome'] ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="20%">Parecer MEC</td>
			<td><?=campo_textarea('uncparecer', 'N', 'S', '', '100', '7', '5000', '', 0, '', false, NULL, $parecer['uncparecer']); ?></td>
		</tr>
		<tr>
			<td class="SubTituloCentro" colspan="2"><input type="submit" name="subform" value="Atualizar parecer"></td>
		</tr>
		
		</table>
		</td>
	</tr>
	<? endif; ?>
	
	<? if($parecer['usunome']) : ?>
	<tr>
	<td align="center">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloDireita" width="20%">Parecer MEC</td>
		<td>
		<?=nl2br($parecer['uncparecer']) ?>
		<?=(($parecer['uncparecerdata'])?'<br><span style="font-size:x-small;"><b>'.$parecer['uncparecerdata'].'</b></span>':'') ?>
		</td>
	</tr>
	</table>
	</td>
	</tr>
	<tr>
		<td align="center"><br><br><br>_____________________________________________________________________________________________</td>
	</tr>
	<tr>
		<td align="center"><?=$parecer['usunome'] ?> - <?=$parecer['usucpf'] ?><br/>Equipe MEC</td>
	</tr>
	<? endif; ?>
</table>
</form>

<!-- FIM - ASSINATURAS -->

<?
$mudancatrocas = carregarMudancasTroca(array('uncid'=>$_SESSION['sispacto3']['universidade']['uncid'])); 
if($mudancatrocas[0]) { 
	echo "<p align=center><b>Registro de Substituições</b></p>";
	$cabecalho = array("Ação","Membro substituído","Membro novo","Saiu da turma","Foi para turma","Perfil","Usuário Responsável","Data da troca");
	$db->monta_lista_simples($mudancatrocas,$cabecalho,1000,5,'N','95%',$par2);

}
?>