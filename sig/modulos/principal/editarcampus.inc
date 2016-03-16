<?
/* Atribuindo variaveis para SESSÃO */
if($_REQUEST['cmpid']) {
	$_SESSION['sig_var']['cmpid'] = (integer) $_REQUEST['cmpid'];
}
if($_REQUEST['orgid']) {
	$_SESSION['sig_var']['orgid'] = (integer) $_REQUEST['orgid'];
}

// Verificando a segurança
$permissoes = verificaPerfilSig();
validaAcessoTipoEnsino($permissoes['vertipoensino'],$_SESSION['sig_var']['orgid']);
// Fim segurança

$permissoes['gravar'] = false;


/* Executando função (_funcoes.php) */
if($_REQUEST['exec_function']) {
	header('Content-Type: text/html; charset=ISO-8859-1');
	$_REQUEST['exec_function']($_REQUEST);
}

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php'; 
require_once APPRAIZ . "adodb/adodb.inc.php";
require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Endereco.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Entidade.php";
echo "<br/>";

$_SESSION['sig_var']['iscampus'] = 'sim';
echo montarAbasArray(carregardadosmenusig(), $_SERVER['REQUEST_URI']);

if($_SESSION['sig_var']['cmpid']) {
	$sql = "SELECT cam.cmpobs,
				 en.estuf AS uf,
				 m.mundsc AS municipio,
				 teu.orgid,
				 ea.entid AS unidade, 
				 cam.entid as entid_cam
			FROM academico.campus cam 
			INNER JOIN entidade.entidade e ON e.entid = cam.entid
			INNER JOIN entidade.funcaoentidade fen ON fen.entid = e.entid 
			INNER JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid 
			INNER JOIN entidade.entidade ea ON ea.entid = fea.entid 
			INNER JOIN entidade.funcaoentidade fen2 ON fen2.entid = ea.entid
			INNER JOIN academico.orgaouo teu ON teu.funid = fen2.funid	
			LEFT JOIN entidade.endereco en ON( en.entid = e.entid)
			LEFT JOIN public.municipio m on m.muncod = en.muncod 	
			WHERE cam.cmpid = '". $_SESSION['sig_var']['cmpid'] ."'";
			/* Carregando os dados do campus */
			$dadoscampus = $db->carregar($sql);
} else {
	// caso não seja passado o parametreo cmpid, enviar para o início do programa
	echo "<script>
		  alert('Erro na passagem de parâmetros. O programa será redirecionado a página principal. Caso o erro persista, entre em contato com o administrador.');
		  window.location = '?modulo=inicio&acao=C';
		  </script>";
}

if($dadoscampus) {
	$dadoscampus = current($dadoscampus);
	$entid_campus = $dadoscampus['entid_cam'];
	$estuf = $dadoscampus['uf'];
	$mundescricao = $dadoscampus['municipio'];
	$_SESSION['sig_var']['entid'] = $dadoscampus['entid_cam'];
} else {
	die("<script>
			alert('Foram encontrados problemas nos parâmetros. Caso o erro persista, entre em contato com o suporte técnico');
			window.location='?modulo=inicio&acao=C';
		 </script>");
}

?>
<script language="JavaScript" src="../includes/prototype.js"></script>
<script language="JavaScript" src="./js/sig.js"></script>
<script language="JavaScript" src="../includes/calendario.js"></script>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<script>
function removercurso(curid) {
	var conf = confirm("Essa ação irá remover todos os dados vinculados ao curso neste campus. Deseja continuar?");
	if(conf) {
		ajaxatualizar('exec_function=removercampuscurso&curid='+curid,'');
		ajaxatualizar('exec_function=carregarvagasporcurso&cmpid=<? echo $_REQUEST['cmpid']; ?>','tabela_cursos');
	}
}
</script>
<form action="?modulo=principal/editarcampus&acao=A" method="post" name="formulario" id="formulario">
<?
echo monta_cabecalho_sig($entid_campus);
?>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
	<td class="SubTituloCentro"><? echo (($tituloitens[$dadoscampus['orgid']])?$tituloitens[$dadoscampus['orgid']]:$tituloitens['default']); ?></td>
	</tr>
	<tr>
	<td>
	<?
	// Se tiver anos analisados por tipo de ensino (declarado no constantes.php), caso não, utilizar o padrão
	if($anosanalisados[$dadoscampus['orgid']]) {
		$anos = $anosanalisados[$dadoscampus['orgid']];
	} else {
		$anos = $anosanalisados['default'];
	}
	unset($cabecalho);
	$cabecalho[] = "Itens";
	foreach($anos as $ano) {
		$paramselects[] = "'<input ".(!$permissoes['gravar']?'readonly':'')." class=\"normal\" id=\"' || itm.itmid || '".$ano."\" onfocus=\"MouseClick(this);\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" name=\"gravacaocampo_' || tpi.tpicampo || '_' || tpi.tpitipocampo || '[' || itm.itmid || '][". $ano ."]\" '|| 
							CASE WHEN tpi.tpimascara is null THEN 'onkeyup=\"\"' 
							ELSE 'onkeyup=\"this.value=mascaraglobal(\'' || tpi.tpimascara || '\', this.value);\"' 
							END ||'  maxlength=\"' || tpi.tpitamanhomax || '\" size=\"14\" type=\"text\" value=\"' || 
							CASE WHEN (SELECT (coalesce(cpitexto,'') || coalesce(cast(cpivalor as varchar),'')) FROM academico.campusitem cpi WHERE itm.itmid = cpi.itmid AND cpi.cpiano = '". $ano ."' AND cpi.cmpid = '". $_REQUEST['cmpid'] ."') is null 
							THEN '' ELSE (SELECT (coalesce(cpitexto,'') || coalesce(cast(cpivalor as varchar),'')) FROM academico.campusitem cpi WHERE itm.itmid = cpi.itmid AND cpi.cpiano = '". $ano ."' AND cpi.cmpid = '". $_REQUEST['cmpid'] ."') END || '\"> '|| 
							CASE WHEN itm.itmpermiteobs IS TRUE THEN '<img  '|| 
							CASE WHEN ((SELECT cpi.cpiobs FROM academico.campusitem cpi WHERE itm.itmid = cpi.itmid AND cpi.cpiano = '". $ano ."' AND cpi.cmpid = '". $_REQUEST['cmpid'] ."') = '') OR ((SELECT cpi.cpiobs FROM academico.campusitem cpi WHERE itm.itmid = cpi.itmid AND cpi.cpiano = '". $ano ."' AND cpi.cmpid = '". $_REQUEST['cmpid'] ."') IS NULL) 
							THEN 'src=\"../imagens/edit_off.gif\" border=\"0\"' ELSE 'title=\"'|| (SELECT cpi.cpiobs FROM academico.campusitem cpi WHERE itm.itmid = cpi.itmid AND cpi.cpiano = '". $ano ."' AND cpi.cmpid = '". $_REQUEST['cmpid'] ."') ||'\" src=\"../imagens/edit_on.gif\"' END ||' id=\"img' || itm.itmid || '_". $ano ."\" onclick=\"abreobservacao(\''|| itm.itmid ||'_".$ano."\');\"><input type=\"hidden\" value=\"' || 
							CASE WHEN (SELECT cpi.cpiobs FROM academico.campusitem cpi WHERE itm.itmid = cpi.itmid AND cpi.cpiano = '". $ano ."' AND cpi.cmpid = '". $_REQUEST['cmpid'] ."') IS NULL 
							THEN '' ELSE (SELECT cpi.cpiobs FROM academico.campusitem cpi WHERE itm.itmid = cpi.itmid AND cpi.cpiano = '". $ano ."' AND cpi.cmpid = '". $_REQUEST['cmpid'] ."') END || '\" id=\"'|| itm.itmid ||'_".$ano."\" name=\"obs['|| itm.itmid ||'][".$ano."]\">' 
							ELSE '' END  ||'' AS ano_".$ano;
		$cabecalho[] = $ano;		
	}
	$paramselects = implode(",",$paramselects);
	// criando o SELECT
	$sql = "SELECT '<strong><span onmouseover=\"this.parentNode.parentNode.title=\'\';return escape(\'' ||itm.itmobs|| '\' );\" >'||itm.itmdsc||'</span></strong>',
			". $paramselects ."
			FROM academico.item itm 
			LEFT JOIN academico.tipoitem tpi ON tpi.tpiid = itm.tpiid 
			LEFT JOIN academico.orgaoitem tei ON tei.itmid = itm.itmid 
			WHERE tei.orgid = '". $dadoscampus['orgid'] ."' AND itm.itmglobal = false
			ORDER BY tei.teiordem";
	$db->monta_lista_simples( $sql, $cabecalho, 50, 10, 'N', '100%','N');
	?>
	</td>
	</tr>
	<tr>
	<td class="SubTituloCentro">Situação Atual</td>
	</tr>
	<tr>
	<td>
	<?
	unset($cabecalho);
	$cabecalho = array("Itens", "Atual");
	$paramselct = "'<input ".(!$permissoes['gravar']?'readonly':'')." class=\"normal\" id=\"' || itm.itmid || ' \" onfocus=\"MouseClick(this);\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" name=\"gravacaocampo_' || tpi.tpicampo || '_' || tpi.tpitipocampo || '[' || itm.itmid || ']\" '|| 
			CASE WHEN tpi.tpimascara is null THEN 'onkeyup=\"\"' 
					ELSE 'onkeyup=\"this.value=mascaraglobal(\'' || tpi.tpimascara || '\', this.value);\"' 
					END ||'  maxlength=\"' || tpi.tpitamanhomax || '\" size=\"14\" type=\"text\" value=\"' || 
			CASE WHEN (SELECT (coalesce(cpitexto,'') || coalesce(cast(cpivalor as varchar),'')) FROM academico.campusitem cpi WHERE itm.itmid = cpi.itmid AND cpi.cmpid = '". $_REQUEST['cmpid'] ."') is null 
					THEN '' ELSE (SELECT (coalesce(cpitexto,'') || coalesce(cast(cpivalor as varchar),'')) FROM academico.campusitem cpi WHERE itm.itmid = cpi.itmid AND cpi.cmpid = '". $_REQUEST['cmpid'] ."') END || '\"> '|| 
			CASE WHEN itm.itmpermiteobs IS TRUE THEN '<img '|| 
			CASE WHEN ((SELECT cpi.cpiobs FROM academico.campusitem cpi WHERE itm.itmid = cpi.itmid AND cpi.cmpid = '". $_REQUEST['cmpid'] ."') = '') OR ((SELECT cpi.cpiobs FROM academico.campusitem cpi WHERE itm.itmid = cpi.itmid AND cpi.cmpid = '". $_REQUEST['cmpid'] ."') IS NULL) 
					THEN 'src=\"../imagens/edit_off.gif\" border=\"0\"' ELSE 'title=\"'|| (SELECT cpi.cpiobs FROM academico.campusitem cpi WHERE itm.itmid = cpi.itmid AND cpi.cmpid = '". $_REQUEST['cmpid'] ."') ||'\" src=\"../imagens/edit_on.gif\"' END ||' id=\"img' || itm.itmid || '\" onclick=\"abreobservacao(\''|| itm.itmid ||'\');\"><input type=\"hidden\" value=\"' || 
			CASE WHEN (SELECT cpi.cpiobs FROM academico.campusitem cpi WHERE itm.itmid = cpi.itmid AND cpi.cmpid = '". $_REQUEST['cmpid'] ."') IS NULL 
					THEN '' ELSE (SELECT cpi.cpiobs FROM academico.campusitem cpi WHERE itm.itmid = cpi.itmid AND cpi.cmpid = '". $_REQUEST['cmpid'] ."') END || '\" id=\"'|| itm.itmid ||'\" name=\"obs['|| itm.itmid ||']\">' 
					ELSE '' END  ||'' AS ano";
	
	$sql = "SELECT '<strong><span onmouseover=\"this.parentNode.parentNode.title=\'\';return escape(\'' ||itm.itmobs|| '\' )\" >'||itm.itmdsc||'</span></strong>',
			". $paramselct ."
			FROM academico.item itm 
			LEFT JOIN academico.tipoitem tpi ON tpi.tpiid = itm.tpiid 
			LEFT JOIN academico.orgaoitem tei ON tei.itmid = itm.itmid 
			WHERE tei.orgid = '". $dadoscampus['orgid'] ."' AND itm.itmglobal = true
			ORDER BY tei.teiordem";
	$db->monta_lista_simples( $sql, $cabecalho, 50, 10, 'N', '100%','N');
	?>
	</td>
	</tr>
	<tr>
	<td class="SubTituloCentro"><? echo (($titulocursos[$dadoscampus['orgid']])?$titulocursos[$dadoscampus['orgid']]:$titulocursos['default']); ?></td>
	</tr>
	<tr>
	<td id="tabela_cursos">
	</td>
	
	<script>
	ajaxatualizar('exec_function=carregarvagasporcurso&orgid=<? echo $dadoscampus['orgid']; ?>&cmpid=<? echo $_REQUEST['cmpid']; ?>','tabela_cursos');
	// 	Retorno vazio da função carregarvagasporcurso (estrutura da tabela vazia)
	if(document.getElementById('tabela_cursos').innerHTML != '<table style="color: rgb(51, 51, 51);" class="listagem" align="center" border="0" cellpadding="2" cellspacing="0" width="100%"></table>') {
	<?
	if($anosanalisados[$dadoscampus['orgid']]) {
		$anos = $anosanalisados[$dadoscampus['orgid']];
	} else {
		$anos = $anosanalisados['default'];
	}
	foreach($anos as $ano) {
		echo "calculacoluna(document.formulario.tot".$ano.");";
	}
	?>
	}
	</script>
	</tr>
	<? if($permissoes['gravar']) { ?>
	<tr><td><a href="javascript:void(0);" onclick="inserirCursos(<? echo $_REQUEST['cmpid']; ?>);"><img src="/imagens/gif_inclui.gif" style="cursor:pointer;" border="0" title="Inserir Etapas">&nbsp;&nbsp;Inserir Cursos</a></td></tr>
	<? } ?>
</table>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr bgcolor="#C0C0C0">
	<td colspan="2" align="center">	
		<input type="hidden" value="<? echo $_SESSION['sig_var']['cmpid']; ?>" name="cmpid">
		<input type="hidden" name="exec_function" value="atualizardadoscampus">
		<? if($permissoes['gravar']) { ?><input type='button' onclick="document.getElementById('formulario').submit();this.disabled=true" value='Gravar'><? } ?>
	</td>
</tr>
</table>

</form>
<script type="text/javascript" src="../includes/wz_tooltip.js"></script>
<script>
aplicarmascara();
function aplicarmascara() {
	var form = document.getElementById('formulario');
	for (var j=0; j < form.length; j++){
		if(form[j].value != "" && form[j].type == "text" && form[j].name.substr(0,14) == "gravacaocampo_") {
			form[j].onkeyup();
		}
	}
}
</script>