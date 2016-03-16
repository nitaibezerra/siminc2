<?
include APPRAIZ ."includes/workflow.php";
include_once "_funcoes.php";

function carregarHistoricoVagas($dados) {
	global $db;

	if($dados['muncod']) {
		
		$sql = "SELECT u.usucpf||' - '||u.usunome as usuario,
					   to_char(havdatainc,'dd/mm/YYYY HH24:MI') as havdatainc,
  					   havlog
				FROM sispacto3.historicoatualizacaovagas  h 
				LEFT JOIN seguranca.usuario u ON u.usucpf = h.havcpf 
				WHERE h.muncod='".$dados['muncod']."'";
		
		$cabecalho = array("Usuário","Data","Informações alteradas");
		$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%',$par2);
	}
	
	if($dados['estuf']) {
	
		$sql = "SELECT u.usucpf||' - '||u.usunome as usuario,
					   to_char(havdatainc,'dd/mm/YYYY HH24:MI') as havdatainc,
  					   havlog
				FROM sispacto3.historicoatualizacaovagas  h
				LEFT JOIN seguranca.usuario u ON u.usucpf = h.havcpf
				WHERE estuf='".$dados['estuf']."'";
		
		$cabecalho = array("Usuário","Data","Informações alteradas");
		$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%',$par2);
	}
	
}

function atualizarNumeroVagas($dados) {
	global $db;
	
	if($dados['muncod']) {
		
		foreach($dados['muncod'] as $muncod) {
			
			$sql = "UPDATE sispacto3.totalalfabetizadores SET total_orientadores_a_serem_cadastrados=".(($dados['norientadores'][$muncod])?"'".$dados['norientadores'][$muncod]."'":"NULL").", total=".(($dados['nprofessores'][$muncod])?"'".$dados['nprofessores'][$muncod]."'":"NULL")." WHERE cod_municipio='".$muncod."' AND dependencia='MUNICIPAL'";
			$db->executar($sql);
			
			$sql = "INSERT INTO sispacto3.historicoatualizacaovagas(
            		muncod, havcpf, havlog, havdatainc)
    				VALUES ('".$muncod."', '".$_SESSION['usucpf']."', 'Orientadores de Estudo : ".(($dados['norientadores'][$muncod])?$dados['norientadores'][$muncod]:"0").", Professores Alfabetizadores : ".(($dados['nprofessores'][$muncod])?$dados['nprofessores'][$muncod]:"0")."', NOW());";
			
			$db->executar($sql);
			
			$db->commit();
		}
		
	}
	
	if($dados['estuf']) {
	
		foreach($dados['estuf'] as $estuf) {
				
			$sql = "UPDATE sispacto3.totalalfabetizadores SET total_orientadores_a_serem_cadastrados=".(($dados['norientadores'][$estuf])?"'".$dados['norientadores'][$estuf]."'":"NULL").", total=".(($dados['nprofessores'][$estuf])?"'".$dados['nprofessores'][$estuf]."'":"NULL")." WHERE sigla='".$estuf."' AND dependencia='ESTADUAL'";
			$db->executar($sql);
				
			$sql = "INSERT INTO sispacto3.historicoatualizacaovagas(
            		estuf, havcpf, havlog, havdatainc)
    				VALUES ('".$estuf."', '".$_SESSION['usucpf']."', 'Orientadores de Estudo : ".(($dados['norientadores'][$estuf])?$dados['norientadores'][$muncod]:"0").", Professores Alfabetizadores : ".(($dados['nprofessores'][$estuf])?$dados['nprofessores'][$estuf]:"0")."', NOW());";
				
			$db->executar($sql);
			
			$db->commit();
		}
	
	}
	
	$al = array("alert"=>"Dados foram gravados com sucesso","location"=>"sispacto3.php?modulo=sistema/geral/gerenciarvagas&acao=A");
	alertlocation($al);

}

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}


include  APPRAIZ."includes/cabecalho.inc";
echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>';
echo '<script language="javascript" type="text/javascript" src="./js/sispacto3.js"></script>';

echo "<br>";

monta_titulo( "Lista - Municípios/Estados", "Gerenciando número de vagas municipais e estaduais");

?>
<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script> 
<script>

function abrirHistorico(param, obj) {
	
	var tabela = obj.parentNode.parentNode.parentNode;
	var linha = obj.parentNode.parentNode;
	if(obj.title=="mais") {
		obj.title    = "menos";
		obj.src      = "../imagens/menos.gif";
		var nlinha   = tabela.insertRow(linha.rowIndex);
		var ncol     = nlinha.insertCell(0);
		ncol.colSpan = 7;
		ncol.id      = 'hst_coluna_'+nlinha.rowIndex;
		ajaxatualizar('requisicao=carregarHistoricoVagas'+param,ncol.id);
	} else {
		obj.title    = "mais";
		obj.src      = "../imagens/mais.gif";
		tabela.deleteRow(linha.rowIndex);
	}


}


function marcarRede(obj) {

	if(obj.checked) {
		jQuery("[name='norientadores["+obj.value+"]']").attr('disabled','');
		jQuery("[name='nprofessores["+obj.value+"]']").attr('disabled','');
	} else {
		jQuery("[name='norientadores["+obj.value+"]']").attr('disabled','disabled');
		jQuery("[name='nprofessores["+obj.value+"]']").attr('disabled','disabled');
	}
	
}

function carregarMunicipiosPorUF(estuf) {
	if(estuf) {
		ajaxatualizar('requisicao=carregarMunicipiosPorUF&id=muncod&name=muncod&estuf='+estuf,'td_municipio');
	} else {
		document.getElementById('td_municipio').innerHTML = "Selecione uma UF";
	}
}

function atualizarNumeroVagas() {

	var input = document.createElement("input");
	input.setAttribute("type", "hidden");
	input.setAttribute("name", "requisicao");
	input.setAttribute("value", "atualizarNumeroVagas");
	document.getElementById("formulario").appendChild(input);

	
	jQuery("#formulario").submit();
	
}

</script>

<form method="post" name="filtro" id="filtro">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloDireita" width="25%">UF</td>
	<td><? 
	$sql = "SELECT estuf as codigo, estuf as descricao FROM territorios.estado ORDER BY estuf";
	$db->monta_combo('estuf', $sql, 'S', 'Selecione', 'carregarMunicipiosPorUF', '', '', '', 'S', 'estuf', '', $_REQUEST['estuf']);
	?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Município</td>
	<td id="td_municipio"><?
	
	if($_REQUEST['estuf']) {
		carregarMunicipiosPorUF(array('id'=>'muncod','name'=>'muncod','estuf'=>$_REQUEST['estuf'],'valuecombo'=>$_REQUEST['muncod']));
	} else {
		echo "Selecione uma UF";
	}
	?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Rede</td>
	<td><input type="radio" name="dependencia" value="" <?=((!$_REQUEST['dependencia'])?"checked":"") ?>> TODOS <input type="radio" name="dependencia" value="MUNICIPAL" <?=(($_REQUEST['dependencia']=='MUNICIPAL')?"checked":"") ?>> MUNICIPAL <input type="radio" name="dependencia" value="ESTADUAL" <?=(($_REQUEST['dependencia']=='ESTADUAL')?"checked":"") ?>> ESTADUAL</td>
</tr>
<tr>
	<td class="SubTituloCentro" colspan="2"><input type="submit" name="filtrar" value="Filtrar"></td>
</tr>
</table>
</form>
<?

if($_REQUEST['dependencia']) {
	$f[] = "foo.dependencia='".$_REQUEST['dependencia']."'";
}

if($_REQUEST['estuf']) {
	$f[] = "foo.estuf='".$_REQUEST['estuf']."'";
}

if($_REQUEST['muncod']) {
	$f[] = "foo.muncod='".$_REQUEST['muncod']."'";
}

$sql = "SELECT foo.mais, foo.chk, foo.estuf, foo.descricao, foo.torientadores, foo.tprofessores, foo.dependencia FROM (
		(
		SELECT p.muncod, t.dependencia, '<img src=\"../imagens/mais.gif\" title=\"mais\" style=\"cursor:pointer;\" align=\"absmiddle\" onclick=\"abrirHistorico(\'&muncod='||p.muncod||'\',this);\">' as mais,'<input type=checkbox name=muncod[] value=\"'||p.muncod||'\" onclick=\"marcarRede(this);\">' as chk, m.estuf as estuf, m.mundescricao as descricao, '<input type=text class=\" normal\" size=\"5\" onkeyup=\"this.value=mascaraglobal(\'####\',this.value);\" name=\"norientadores['||p.muncod||']\" value=\"'||total_orientadores_a_serem_cadastrados||'\" disabled>' as torientadores, '<input type=text class=\" normal\" size=\"5\" onkeyup=\"this.value=mascaraglobal(\'####\',this.value);\" name=\"nprofessores['||p.muncod||']\" value=\"'||total||'\" disabled>' as tprofessores
		FROM sispacto3.pactoidadecerta p 
		INNER JOIN sispacto3.totalalfabetizadores t ON t.cod_municipio = p.muncod 
		INNER JOIN territorios.municipio m ON m.muncod = p.muncod 
		WHERE t.dependencia='MUNICIPAL'
		) UNION ALL (
		SELECT null as muncod, t.dependencia, '<img src=\"../imagens/mais.gif\" title=\"mais\" style=\"cursor:pointer;\" align=\"absmiddle\" onclick=\"abrirHistorico(\'&estuf='||p.estuf||'\',this);\">' as mais,'<input type=checkbox name=estuf[] value=\"'||p.estuf||'\" onclick=\"marcarRede(this);\">' as chk, e.estuf as estuf, e.estdescricao as descricao, '<input type=text class=\" normal\" size=\"5\" onkeyup=\"this.value=mascaraglobal(\'####\',this.value);\" name=\"norientadores['||p.estuf||']\" value=\"'||total_orientadores_a_serem_cadastrados||'\" disabled>' as torientadores, '<input type=text class=\" normal\" size=\"5\" onkeyup=\"this.value=mascaraglobal(\'####\',this.value);\" name=\"nprofessores['||p.estuf||']\" value=\"'||total||'\" disabled>' as tprofessores
		FROM sispacto3.pactoidadecerta p 
		INNER JOIN sispacto3.totalalfabetizadores t ON t.sigla = p.estuf 
		INNER JOIN territorios.estado e ON e.estuf = p.estuf 
		WHERE t.dependencia='ESTADUAL'
		)
		) foo
		".(($f)?"WHERE ".implode(" AND ", $f):"")."
		ORDER BY foo.dependencia, foo.estuf, foo.descricao
		";

$cabecalho = array("&nbsp;","&nbsp;","UF","Descrição","Qtd Orientadores de Estudo","Qtd Professores Alfabetizadores","Rede");
$db->monta_lista($sql,$cabecalho,10,10,'N','center','N','formulario');

?>
<p align="center"><input type="button" value="Gravar" onclick="atualizarNumeroVagas();"></p>