<html>
  <head>
    <meta http-equiv="Cache-Control" content="no-cache">
    <meta http-equiv="Pragma" content="no-cache">
    <meta http-equiv="Connection" content="Keep-Alive">
    <meta http-equiv="Expires" content="Mon, 26 Jul 1997 05:00:00 GMT">
    <title><?= $titulo ?></title>

<script language="javascript" type="text/javascript" src="./js/conjur.js"></script>
<script language="javascript" type="text/javascript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/calendario.js"></script>
    
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <script type="text/javascript">
    	<? 
    		$estadosArquivar = Array(WF_AGUARDANDO_PROVIDENCIA_SEATA,
    								 WF_EM_ANALISE_GABINETE,
    								 WF_EM_ANALISE_GABINETE_AUXILIAR,
    								 WF_EM_ANALISE_COORDENADOR_GERAL,
    								 WF_EM_ANALISE_ADVOGADO,
    								 WF_ARQUIVADO);
    		if($permissoes['gravar']&&is_array($esdid,$estadosArquivar)) {
    	?>
    	alert('Estado de documento inválido para esta ação.');
    	window.close();
    	<? } ?>
    
      	this._closeWindows = false;

	  	function exibirOcultarNumeracaoUnica(value) {
	  		var visibilidade = determinarVisibilidadeProcesso(value);
	  		document.getElementById('linhaNumeracaoUnicaJudicial').style.visibility = visibilidade;
	  		document.getElementById('linhaNumeracaoUnicaJudicialAntigo').style.visibility = visibilidade;
	  	}	
	  	
	  	function determinarVisibilidadeProcesso(valorSelecionado) {
	  		if( valorSelecionado==2 ) { // 2, conforme ID na base de dados
	  			return "visible";
	  		} else {
	  			reinicializarNumeroProcessoJudicial();
	  			return "collapse";
	  		}
	  	}
	
	  	function reinicializarNumeroProcessoJudicial() {
	  		document.formulario.prcnumeroprocjudicial.value = "";
	  		document.formulario.prcnumeroprocjudantigo.value = "";
	  	}		
    </script>
  </head>
  <body>
<form name="formulario" id="formulario" method="post">
<input type="hidden" name="requisicao" value="pesquisarprojeto">
<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
	<tr>
		<td bgcolor="#CCCCCC" colspan="2">
			<b>Argumentos da Pesquisa</b>
		</td>
	</tr>
 	<tr>
		<td class="SubTituloDireita">Tipo de Processo :</td>
		<td><?
			$tprid = $_REQUEST['tprid'];
			$sqlTipoProcesso = "SELECT tprid as codigo, tprdsc as descricao 
								FROM conjur.tipoprocesso 
								WHERE tprstatus = 'A'
								ORDER BY tprdsc";
			$db->monta_combo('tprid', $sqlTipoProcesso, 'S', 'Selecione...', 'exibirOcultarNumeracaoUnica', '', '', 150, 'N', 'tprid');
			?></td>
	</tr>
 	<tr>
		<td class="SubTituloDireita">Número do Processo SIDOC :</td>
		<td><?
			$pcjnumerosidoc = $_REQUEST['prcnumsidoc'];
			echo campo_texto('prcnumsidoc', 'N', 'S', '', 35, 25, '', '', 'left', '', 0, '','conjurJs_digitoVerificador(this);','','');
			?></td>
	</tr>
	
	<tr id="linhaNumeracaoUnicaJudicial" style="visibility: collapse;">
		<td class="SubTituloDireita" style="width:20%;">Numeração Única Judicial :</td>
		<td><? echo campo_texto('prcnumeroprocjudicial', 'N', 'S', '', 30, 25, '#######-##.####.#.##.####', '', 'right', '', 0, ' style="text-align:right;" '); ?></td>
	</tr>
	
	<tr id="linhaNumeracaoUnicaJudicialAntigo" style="visibility: collapse;">
		<td class="SubTituloDireita" style="width:20%;">Numeração Judicial Antiga:</td>
		<td><? echo campo_texto('prcnumeroprocjudantigo', 'N', 'S', '', 30, 25, '#######-##.####.#.##.####', '', 'right', '', 0, ' style="text-align:right;" '); ?></td>
	</tr>	
	
	<tr>
		<td class="SubTituloDireita">Prioritário :</td>
		<td>
			<input type="radio" name="prcprioritario" id="prcprioritario" value="sim" /> Sim
			<input type="radio" name="prcprioritario" id="prcprioritario" value="nao" /> Não
		</td>
	</tr>			
	
	<tr>
		<td class="SubTituloDireita">Data de Entrada do Processo :</td>
		<td>
			<?
				$prcdtentradaini = $_REQUEST['prcdtentradaini'];
				$prcdtentradafim = $_REQUEST['prcdtentradafim'];
				echo "Início " . campo_data('prcdtentradaini', 'N', 'S', '', 'S');
				echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
				echo "Fim " . campo_data('prcdtentradafim', 'N', 'S', '', 'S' );
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Advogado(s) :</td>
		<td><?
			$advid = $_REQUEST['advid'];
			$sqlAdvogado = "SELECT adv.advid as codigo, ent.entnome as descricao 
							FROM conjur.advogados adv 
							LEFT JOIN entidade.entidade ent ON ent.entid = adv.entid 
							ORDER BY ent.entnome";
			$db->monta_combo('advid', $sqlAdvogado, 'S', 'Selecione...', '', '', '', 150, 'N', 'advid');
			?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Procedência :</td>
		<td><?
			$unicod = $_REQUEST['unicod'];
			$sqlProcedencia = "SELECT unicod || '&' || unitpocod as codigo,	unidsc as descricao 
						   FROM public.unidade
						   WHERE orgcod = '".ORGCOD."' AND unistatus = 'A'
						   ORDER BY	unidsc";
			$db->monta_combo('unicod', $sqlProcedencia, 'S', 'Selecione...', '', '', '', 300, 'N','unicod');
			?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Tema :</td>
		<td><?
		$tasid = $_REQUEST['tasid'];
		$sqlAssunto = "SELECT tasid as codigo, tasdsc as descricao
					   FROM	conjur.tipoassunto
					   WHERE tasstatus = 'A' 
					   ORDER BY tasdsc";
		$db->monta_combo('tasid', $sqlAssunto, 'S', 'Selecione...', '', '', '', 300, 'N', 'tasid');
		?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Localização na CONJUR :</td>
		<td><?
		$cooid = $_REQUEST['cooid'];
		$sqlCoordenacao = "SELECT coonid as codigo, coodsc as descricao 
						   FROM conjur.coordenacao
						   ORDER BY	coodsc";
		
		$db->monta_combo('cooid', $sqlCoordenacao, 'S', 'Selecione...', '', '', '', 300, 'N','cooid');
		 ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Situação do Andamento :</td>
		<td><?
		$esdid = $_REQUEST['esdid'];
		$sqlSituacaoAndamento = "SELECT esdid as codigo, esddsc as descricao 
						   		 FROM workflow.estadodocumento 
						   		 WHERE tpdid='".TPD_CONJUR_NOVO."' AND esdstatus='A' 
						   		 ORDER BY esddsc";
		$db->monta_combo('esdid', $sqlSituacaoAndamento, 'S', 'Selecione...', '', '', '', 300, 'N','cooid');
		 ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Expressão Chave:</td>
		<td>
			<?			
				$expressaochave = $_REQUEST['expressaochave'];
				echo campo_texto('expressaochave', 'N', 'S', '', 60, 60, '', '', 'left', '', 0, '');
			?>
		</td>
	</tr>
	<tr>
		<td bgcolor="#CCCCCC" colspan="2" align="center">
			<input type="button" name="visualizar" value="Visualizar" style="cursor:pointer;" onclick="this.disabled=true;document.getElementById('formulario').submit();">  
			<input type="button" name="vertodos" value="Ver Todos" style="cursor:pointer;" onclick="this.disabled=true;window.location='?modulo=principal/vincular_processo&acao=A&vertodos=sim';">
		</td>
	</tr>
</form>
	<tr>
		<td colspan="2">
		<form name="formulario2" id="formulario2" method="post">
<?

if($_REQUEST['tprid']) {
	$filtroprocesso[] = "tpr.tprid='".$_REQUEST['tprid']."'";
}
if($_REQUEST['prcnumsidoc']) {
	$filtroprocesso[] = "prcnumsidoc ilike '%".eregi_replace('([^0-9])','',$_REQUEST['prcnumsidoc'])."%'";
}
if($_REQUEST['prcdtentradaini']) {
	$filtroprocesso[] = "prcdtentrada >= '".$_REQUEST['prcdtentradaini']."'";
}
if($_REQUEST['prcdtentradafim']) {
	$filtroprocesso[] = "prcdtentrada <= '".$_REQUEST['prcdtentradafim']."'";
}
if($_REQUEST['advid']) {
	$filtroprocesso[] = "advid = '".$_REQUEST['advid']."'";
}
if($_REQUEST['unicod']) {
	$code = explode("&", $_REQUEST['unicod']);
	$filtroprocesso[] = "prc.unicod='".$code[0]."' AND prc.unitpocod='".$code[1]."'";
}
if($_REQUEST['tasid']) {
	$filtroprocesso[] = "tasid='".$_REQUEST['tasid']."'";
}
if($_REQUEST['cooid']) {
	$filtroprocesso[] = "cooid='".$_REQUEST['cooid']."'";
}
if($_REQUEST['esdid']) {
	$filtroprocesso[] = "esd.esdid='".$_REQUEST['esdid']."'";
}
if($_REQUEST['expressaochave']) {
	$filtroprocesso[] = "excdsc='".$_REQUEST['expressaochave']."'";
}
if($_REQUEST['prcnumeroprocjudicial']) {
	$filtroprocesso[] = "prc.prcnumeroprocjudicial='".$_REQUEST['prcnumeroprocjudicial']."'";
}
if($_REQUEST['prcnumeroprocjudantigo']) {
	$filtroprocesso[] = "prc.prcnumeroprocjudantigo='".$_REQUEST['prcnumeroprocjudantigo']."'";
}
if($_REQUEST['prcprioritario'] == 'sim' ) {
	$filtroprocesso[] = "prc.prcprioritario = true ";
} 
if($_SESSION['conjur_var']['prcid']){
	$filtroprocesso[] = "prc.prcid  not in ( select pro_prcid FROM conjur.processosvinculados WHERE prcid = {$_SESSION['conjur_var']['prcid']} )";
}
$filtroprocesso[] = "doc.esdid in (".implode(',',$estadosArquivar).")";

$cabecalho = array('Ações','Nº do Processo SIDOC','Tipo','Interessado','Data de Entrada','Situação CONJUR');
$sql = array();
if(count($filtroprocesso) > 1 || $_REQUEST['vertodos']=='sim') {	
$sql = 	"SELECT '<input type=\'checkbox\' name=\'prcid[]\' id=\'prcid\' value=\''||prc.prcid||'\'>' as img,
				prcnumsidoc,
				tpr.tprdsc,
				prcnomeinteressado,
				to_char(prcdtentrada, 'dd/mm/YYYY'),
				esd.esddsc  
    	FROM conjur.processoconjur prc 
    	LEFT JOIN conjur.tipoprocesso tpr ON tpr.tprid = prc.tprid 
    	LEFT JOIN conjur.estruturaprocesso esp ON prc.prcid = esp.prcid 
    	LEFT JOIN workflow.documento doc ON doc.docid = esp.docid 
    	LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid 
    	LEFT JOIN conjur.expressaochave exp ON exp.prcid = prc.prcid   
    	".((count($filtroprocesso) > 0)?"WHERE ".implode(" AND ", $filtroprocesso):"")."
    	GROUP BY prc.prcid, prcnumsidoc, tpr.tprdsc, prcnomeinteressado, prcdtentrada, esddsc";
}

$db->monta_lista_simples($sql,$cabecalho,50,20,'N','100%','');
?>
</form>
		</td>
	</tr>
</table>
<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
	<tr>
		<td bgcolor="#CCCCCC" align="left">
			<input type="button" name="vincularProcesso" onclick="processarvinculamentoprocessos();" value="Ok" style="cursor:pointer;" onclick="this.disabled=true;window.location.href = '?modulo=principal/cadastrarprocesso&acao=A';">
		</td>
	</tr>
</table>
<script language="javascript" type="text/javascript">

function processarvinculamentoprocessos(tes) {
	var form = document.getElementById('formulario2');
	var tabdest = window.opener.document.getElementById('tabela_vincprocessos');
		if(form.elements['prcid'].checked) {
			var linha =  form.elements['prcid'].parentNode.parentNode;
			var nline = tabdest.insertRow(tabdest.rows.length);
			var cel1 = nline.insertCell(0);
			cel1.innerHTML = "<center><img onclick=\"deletarlinha(this);\" src=\"/imagens/excluir.gif\" style=\"cursor:pointer\"  border=\"0\" title=\"Excluir\"></center><input type=\"hidden\" name=\"pro_prcid["+form.elements['prcid'].value+"]\" value=\"sim\">";
			var cel2 = nline.insertCell(1);
			cel2.innerHTML = linha.cells[1].innerHTML; 
			var cel3 = nline.insertCell(2);
			cel3.innerHTML = linha.cells[2].innerHTML; 
			var cel4 = nline.insertCell(3);
			cel4.innerHTML = linha.cells[3].innerHTML; 
			var cel5 = nline.insertCell(4);
			cel5.innerHTML = linha.cells[4].innerHTML; 
			var cel6 = nline.insertCell(5);
			cel6.innerHTML = linha.cells[5].innerHTML;
		}else{
				for(i=0;i<form.elements['prcid'].length;i++) {
					if(form.elements['prcid'][i].checked) {
						var linha =  form.elements['prcid'][i].parentNode.parentNode;
						var nline = tabdest.insertRow(tabdest.rows.length);
						var cel1 = nline.insertCell(0);
						cel1.innerHTML = "<center><img onclick=\"deletarlinha(this);\" src=\"/imagens/excluir.gif\" style=\"cursor:pointer\"  border=\"0\" title=\"Excluir\"></center><input type=\"hidden\" name=\"pro_prcid["+form.elements['prcid'][i].value+"]\" value=\"sim\">";
						var cel2 = nline.insertCell(1);
						cel2.innerHTML = linha.cells[1].innerHTML; 
						var cel3 = nline.insertCell(2);
						cel3.innerHTML = linha.cells[2].innerHTML; 
						var cel4 = nline.insertCell(3);
						cel4.innerHTML = linha.cells[3].innerHTML; 
						var cel5 = nline.insertCell(4);
						cel5.innerHTML = linha.cells[4].innerHTML; 
						var cel6 = nline.insertCell(5);
						cel6.innerHTML = linha.cells[5].innerHTML;
					}
				}
		}
	window.close();
}

function submeteFormPesquisa(tipo) {
	document.getElementById("tipopesquisa").value = tipo;
	document.getElementById("formulario").submit();
}

function alteraCampoCpfCnpj(tipo) {
	var cpf = document.getElementById('cpf');
	var cnpj = document.getElementById('cnpj');
	
	if(tipo == 'cpf') {
		cnpj.value = "";
		cnpj.style.display = "none";
		cpf.style.display = "inline";
	} else {
		cpf.value = "";
		cpf.style.display = "none";
		cnpj.style.display = "inline";
	}
}

</script>
    </body>
</html>