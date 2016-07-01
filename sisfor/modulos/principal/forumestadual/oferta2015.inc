<?

function carregarDetalhesOferta2015($dados) {
	global $db;
	
	echo '<form method=post name=formuf id=formuf>';
	echo '<input type="hidden" name="requisicao" value="salvarOferta2015_UF">';
	echo '<input type="hidden" name="curid" value="'.$dados['curid'].'">';
	
	$sql = "SELECT u.uniabrev, 
				   u.unidsc, 
				   '<input type=\"text\" style=\"text-align:right;\" name=\"ies['||si.unicod||']\" size=\"6\" maxlength=\"5\" value=\"'||coalesce((select ofeqtdies::text from sisfor.oferta2015 o WHERE o.unicod= si.unicod AND o.estuf='{$dados['estuf']}' and o.curid='{$dados['curid']}'),'0')||'\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);this.value=mascaraglobal(\'#####\',this.value);somarTotal();\" id=\"oferta_'||si.unicod||'\" title=\"Oferta 2015\" onkeyup=\"this.value=mascaraglobal(\'#####\',this.value);somarTotal();\" class=\"disabled\" readonly>' as txties,
				   '<input type=\"text\" style=\"text-align:right;\" name=\"uf['||si.unicod||']\" size=\"6\" maxlength=\"5\" value=\"'||coalesce((select ofeqtdforum::text from sisfor.oferta2015 o WHERE o.unicod= si.unicod AND o.estuf='{$dados['estuf']}' and o.curid='{$dados['curid']}'),'')||'\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);this.value=mascaraglobal(\'#####\',this.value);somarTotal();\" id=\"oferta_'||si.unicod||'\" title=\"Oferta 2015\" onkeyup=\"this.value=mascaraglobal(\'#####\',this.value);somarTotal();\" ".(($sieoferta2015ies=='t')?" class=\"disabled\" readonly":" class=\" normal\"").">'
			FROM sisfor.sisfories si 
			INNER JOIN public.unidade u ON u.unicod = si.unicod
			INNER JOIN sisfor.oferta2015iesuf iu ON u.unicod = iu.unicod
			WHERE iu.estuf='".$dados['estuf']."'";
	
	$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%');
	
	//echo '<p align=center><input type=button name=salvar value=Salvar onclick="salvarOferta2015();"></p>';
	
	echo '</form>';

}


if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

include  APPRAIZ."includes/cabecalho.inc";
echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>';
echo '<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>';
echo '<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>';
echo '<script language="javascript" type="text/javascript" src="js/sisfor.js"></script>';


echo "<br>";


$perfis = pegaPerfilGeral();

if($db->testa_superuser() || in_array(PFL_ADMINISTRADOR,$perfis) || in_array(PFL_EQUIPE_MEC,$perfis) || in_array(PFL_COORDENADOR_MEC,$perfis) || in_array(PFL_DIRETOR_MEC,$perfis)) {

	if($_REQUEST['estuf']) {
		$_SESSION['sisfor']['forumestadual']['estuf'] = $_REQUEST['estuf'];
	}

	echo "<p align=center>UF : ";
	$sql = "SELECT estuf as codigo, estuf as descricao FROM territorios.estado ORDER BY estuf";
	$db->monta_combo('estuf', $sql, 'S', 'Selecione', 'acessarUF', '', '', '', 'S', 'estuf', '', $_SESSION['sisfor']['forumestadual']['estuf']);
	echo "</p>";

} elseif(in_array(PFL_FORUM_ESTADUAL_PERMANENTE,$perfis)) {

	$sql = "SELECT estuf FROM sisfor.usuarioresponsabilidade WHERE usucpf='".$_SESSION['usucpf']."' AND pflcod='".PFL_FORUM_ESTADUAL_PERMANENTE."' AND rpustatus='A'";
	$estufs = $db->carregarColuna($sql);

	if(count($estufs)==1) $_SESSION['sisfor']['forumestadual']['estuf'] = $estufs[0];
	else die("desenvolver");
}


?>
<script>

function verDetalhes(curid) {

	ajaxatualizar('requisicao=carregarDetalhesOferta2015&estuf=<?=$_SESSION['sisfor']['forumestadual']['estuf'] ?>&curid='+curid,'modalDetalhes');
	
	jQuery("#modalDetalhes").dialog({
	                        draggable:true,
	                        resizable:true,
	                        width: 1024,
	                        height: 720,
	                        modal: true,
	                     	close: function(){} 
	                    });
}

function salvarOferta2015() {

	return false;

	var embranco = false;
	
	jQuery("[name^='uf[']").each(function() {
		if(jQuery.trim(jQuery(this).val())=='') {
			alert('Valor é obrigatório');
			jQuery(this).focus();
			embranco = true;
			return false;
		}
	});

	if(embranco) {
		return false;
	}

	var input = document.createElement("input");
	input.setAttribute("type", "hidden");
	input.setAttribute("name", "estuf");
	input.setAttribute("value", "<?=$_SESSION['sisfor']['forumestadual']['estuf'] ?>");
	document.getElementById("formuf").appendChild(input);
	
	//document.getElementById("formuf").submit();

}

function acessarUF(uf) {
	window.location='sisfor.php?modulo=principal/forumestadual/oferta2015&acao=A&estuf='+uf;
	
}


</script>
<div id="modalDetalhes" style="display:none;"></div>
<?


if($_SESSION['sisfor']['forumestadual']['estuf']) {

	$estado = $db->pegaUm("SELECT estuf||' - '||estdescricao as universidade FROM territorios.estado WHERE estuf='".$_SESSION['sisfor']['forumestadual']['estuf']."'");
	
	
	monta_titulo( "Planejamento da Oferta 2015", 
				  "<table width=100% cellspacing=1 cellpadding=3>
					<tr>
						<td class=subtituloCentro>".$estado."</td>
					</tr>
					<tr>
						<td class=subtituloEsquerda>Orientações</td>
					</tr>
					<tr>
						<td>
						<p>Observando a demanda de vagas das escolas e das redes de ensino (disponíveis no endereço http://pddeinterativo.mec.gov.br), e considerando a capacidade de atendimento da Instituição, informe o número de vagas novas que poderão ser ofertadas em 2015. Não considere as vagas dos cursos em andamento ou submetidos à análise em 2014. Caso a Instituição não deseje ou não disponha de profissionais habilitados para ofertar o curso, indique 0 (zero). Nenhum campo poderá ficar em branco.</p>
						<p>Ao finalizar o preenchimento da tabela, clique em Finalizar Oferta 2015.</p>
						<p>Os valores indicados pela IES subsidiarão o Ministério da Educação no planejamento orçamentário (de custeio e bolsas) para o exercício seguinte, considerando os limites orçamentários.</p>
						</td>
					</tr>
					</table>");
	
	$sql = "(SELECT DISTINCT
	                cur.curid::text,
	                cur.curdesc AS nomecurso,
	                ncu.ncudesc AS nivelcurso,
	                esp.espdesc AS etapaensino,
					'<input type=\"text\" style=\"text-align:right;\" name=\"ofertaies['||cur.curid||']\" size=\"6\" maxlength=\"5\" value=\"'||coalesce((select sum(ofeqtdies)::text from sisfor.oferta2015 o INNER JOIN sisfor.oferta2015iesuf iu ON o.unicod = iu.unicod where iu.estuf='".$_SESSION['sisfor']['forumestadual']['estuf']."' and curid=cur.curid),'')||'\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);this.value=mascaraglobal(\'#####\',this.value);somarTotal();\" id=\"oferta_'||cur.curid||'\" title=\"Oferta 2015\" onkeyup=\"this.value=mascaraglobal(\'#####\',this.value);somarTotal();\" class=\"disabled\" readonly>' as oferta2015ies,
					'<input type=\"text\" style=\"text-align:right;\" name=\"ofertauf['||cur.curid||']\" size=\"6\" maxlength=\"5\" value=\"'||coalesce((select sum(ofeqtdforum)::text from sisfor.oferta2015 o INNER JOIN sisfor.oferta2015iesuf iu ON o.unicod = iu.unicod where iu.estuf='".$_SESSION['sisfor']['forumestadual']['estuf']."' and curid=cur.curid),'')||'\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);this.value=mascaraglobal(\'#####\',this.value);somarTotal();\" id=\"oferta_'||cur.curid||'\" title=\"Oferta 2015\" onkeyup=\"this.value=mascaraglobal(\'#####\',this.value);somarTotal();\" class=\"disabled\" readonly> <img src=\"../imagens/consultar.gif\" style=\"cursor:pointer;\" onclick=\"verDetalhes('||cur.curid||')\">' as oferta2015forum
	
	                FROM
	                catalogocurso2014.curso cur
	                LEFT JOIN catalogocurso2014.abrangenciacurso abc ON cur.curid = abc.curid
	                INNER JOIN catalogocurso2014.cursorede crd ON crd.curid = cur.curid
	                INNER JOIN catalogocurso2014.rede red ON crd.redid = red.redid
	                LEFT JOIN catalogocurso2014.nivelcurso ncu ON ncu.ncuid = cur.ncuid
	                INNER JOIN workflow.documento doc ON doc.docid = cur.docid
	                INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
	                LEFT JOIN catalogocurso2014.iesofertante ieo ON ieo.curid = cur.curid
	                LEFT JOIN entidade.endereco een ON een.entid = ieo.entid
	                LEFT JOIN catalogocurso2014.areatematica ate ON ate.ateid = cur.ateid
	                LEFT JOIN catalogocurso2014.subarea sua ON sua.suaid = cur.suaid
	                LEFT JOIN catalogocurso2014.especialidade esp ON esp.espid = cur.espid
	                LEFT JOIN catalogocurso2014.usuarioresponsabilidade ur1 ON ur1.coordid = cur.coordid
	                LEFT JOIN catalogocurso2014.usuarioresponsabilidade ur2 ON ur2.curid = cur.curid
	                LEFT JOIN catalogocurso2014.localizacaoescola les ON cur.lesid = les.lesid 
			ORDER BY cur.curdesc
			) UNION ALL (
			SELECT '', 
				   '', 
				   '', 
				   '<span style=float:right><b>TOTAL:</b></span>', 
				   '<input type=\"text\" style=\"text-align:right;\" name=\"totalies\" size=\"6\" maxlength=\"5\" value=\"'||coalesce((select sum(ofeqtdies)::text from sisfor.oferta2015 o INNER JOIN sisfor.oferta2015iesuf iu ON o.unicod = iu.unicod where iu.estuf='".$_SESSION['sisfor']['forumestadual']['estuf']."'),'')||'\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);this.value=mascaraglobal(\'#####\',this.value);\" id=\"total\" title=\"Oferta 2015\" onkeyup=\"this.value=mascaraglobal(\'#####\',this.value);\" class=\"disabled\" readonly>' as totaloferta2015ies, 
				   '<input type=\"text\" style=\"text-align:right;\" name=\"totaluf\" size=\"6\" maxlength=\"5\" value=\"'||coalesce((select sum(ofeqtdforum)::text from sisfor.oferta2015 o INNER JOIN sisfor.oferta2015iesuf iu ON o.unicod = iu.unicod where iu.estuf='".$_SESSION['sisfor']['forumestadual']['estuf']."'),'')||'\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);this.value=mascaraglobal(\'#####\',this.value);\" id=\"total\" title=\"Oferta 2015\" onkeyup=\"this.value=mascaraglobal(\'#####\',this.value);\" class=\"disabled\" readonly>' as totaloferta2015uf
			)";
	
	
	$cabecalho = array("Código","Nome do curso","Nível do curso","Especialidade","Oferta 2015 IES","Oferta 2015 Forum");
	$db->monta_lista($sql,$cabecalho,1000,10,'N','center','N');

}

?>