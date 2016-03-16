<?php
if ($_REQUEST['requisicao']){
	$_REQUEST['requisicao']();	
	die();
}

if($_SESSION['baselogin'] == 'simec_desenvolvimento'){
	define("QUEID_ACADEMICO_INDICADORES", 		76);
	define("QUEID_ACADEMICO_INFRAESTRUTURA", 	77);
	define("QUEID_ACADEMICO_SOCIOECONOMICO", 	81);
} else {
	define("QUEID_ACADEMICO_INDICADORES", 		81);
	define("QUEID_ACADEMICO_INFRAESTRUTURA", 	82);
	define("QUEID_ACADEMICO_SOCIOECONOMICO", 	83);
}

function filtraruniversidade(){
	global $db;
	header('Content-type: text/html; charset=ISO-8859-1');
	
	if($_REQUEST['estuf']){
		$aryWhere[] = "mun.estuf = '{$_REQUEST['estuf']}'";	
	}		
	
	$aryWhere[] = "e.entstatus = 'A' AND ef.funid = ".ACA_ID_UNIVERSIDADE;	
	
	$sql = "SELECT		e.entid AS codigo, e.entnome AS descricao 	
			FROM		entidade.entidade e 
			INNER JOIN	entidade.funcaoentidade ef ON ef.entid = e.entid
			LEFT JOIN	entidade.endereco ed ON ed.entid = e.entid
			LEFT JOIN	territorios.municipio mun ON mun.muncod = ed.muncod
						".(is_array($aryWhere) ? ' WHERE '.implode(' AND ', $aryWhere) : '')."
			GROUP BY 	e.entid, e.entnome , e.entsig, mun.mundescricao, mun.estuf
			ORDER BY	e.entsig, e.entnome";			
	
	mostrarComboPopup('Universidade', 'universidade', $sql, '', 'Selecione as universidades',array(),'filtrarcampusrel',true);
}

function filtrarcampusrel(){
	global $db;
	header('Content-type: text/html; charset=ISO-8859-1');
	
	$entidades = trim($_REQUEST['entid'],",");
	$entidades = str_replace("\'", "'",  $entidades);
	
	if(!empty($entidades)){
		$aryWhere[] = "unientid IN ({$entidades})";
		$campus = explode(",",$entidades);
		$qtdcampus = count($campus);
	} else {
		$qtdcampus = 0;
	}
	
	$sql = "SELECT 		cmpentid AS codigo, cmpdsc AS descricao 
			FROM 		academico.unicampus 
						".(is_array($aryWhere) ? ' WHERE '.implode(' AND ', $aryWhere) : '')." 
			ORDER BY 	cmpdsc";
	
	if($qtdcampus == 1){
		$db->monta_combo('campus', $sql, 'S', 'Selecione', '','','','','','campus');
	} else {
		$db->monta_combo('campus', $sql, 'N', 'Selecione', '','','','','','campus');
	}
}

header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");
$caminho_atual = $_SERVER["REQUEST_URI"]; ?>

<link rel="stylesheet" type="text/css" href="/includes/Estilo.css"/>
<script type="text/javascript" language="javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" language="javascript">
function onOffCampo( campo ){
	var div_on = document.getElementById(campo + '_campo_on');
	var div_off = document.getElementById(campo + '_campo_off');
	var input = document.getElementById(campo + '_campo_flag');
	if( div_on.style.display == 'none'){
		div_on.style.display = 'block';
		div_off.style.display = 'none';
		input.value = '1';
	} else {
		div_on.style.display = 'none';
		div_off.style.display = 'block';
		input.value = '0';
	}
}

function onOffBloco( bloco ){
	var div_on = document.getElementById(bloco + '_div_filtros_on');
	var div_off = document.getElementById(bloco + '_div_filtros_off');
	var img = document.getElementById(bloco + '_img');
	var input = document.getElementById(bloco + '_flag');
	if ( div_on.style.display == 'none'){
		div_on.style.display = 'block';
		div_off.style.display = 'none';
		input.value = '0';
		img.src = '/imagens/menos.gif';
	} else {
		div_on.style.display = 'none';
		div_off.style.display = 'block';
		input.value = '1';
		img.src = '/imagens/mais.gif';
	}
}
	
function enviar(){
	var formulario = document.formulario;
	var elemento = document.getElementsByName('universidade[]')[0];
	var universidades = '';			
	if (elemento.options.length > 0){
		for (var i=0; i<elemento.options.length; i++){
		if (elemento.options[i].value != ''){
			universidades += "'" + elemento.options[i].value + "',";
		}
	}
		
	$('#entidade').val(universidades);
		$('#tipoentidade').val('universidade');
	}

	if($('#campus').val() != ''){
		$('#entidade').val($('#campus').val());
		$('#tipoentidade').val('campus');
	}

	if (universidades == ''){
		alert('Selecione a "Universidade"!');
		jQuery('#universidade').focus();
		return false;
	}

	if($('#ano').val() == ''){
		alert('Selecione o "Ano"!');
		jQuery('#ano').focus();
		return false;
	}

	if($('#eixo:checked').length <= 0){
		alert('Selecione um "Eixo"!');
		$('#eixo').focus();
		return false;
	}	
	
	$('#formulario').submit();
}
	
function filtraruniversidade(){
	jQuery.ajax({
		type: "POST",
		url: window.location,
		data: "requisicao=filtraruniversidade&estuf="+$('#estuf').val(),
		success: function(msg){
			$('#tduniversidade').html( msg );
		}
	});
}
	
function filtrarcampusrel(){
	var elemento = document.getElementsByName('universidade[]')[0];
	var universidades = '';	
	
	for (var i=0; i<elemento.options.length; i++){
		if (elemento.options[i].value != ''){
			universidades += "'" + elemento.options[i].value + "',";
		}
	}

	if(elemento.options.length > 1){
		$('#campus').attr('disabled','true');
	}

	jQuery.ajax({
		type: "POST",
		url: window.location,
		data: "requisicao=filtrarcampusrel&entid="+universidades,
		success: function(msg){
			$('#tdcampus').html( msg );
		}
	});
}
</script>

<?php if(!$_POST){
include_once APPRAIZ . "includes/cabecalho.inc";
echo "<br/>";

monta_titulo( 'Relatório de Assistência Estudantil - Rede Federal',''); ?>
<form name="formulario" id="formulario" target=1 method="post">
	<input type=hidden  name="entidade" value="" id="entidade">
	<!-- Hidden que diz se o relatorio e de entidade universidade ou campus -->
	<input type=hidden  name=tipoentidade value=""  id="tipoentidade">
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
		<tr>
			<td class="subtitulodireita">Estado</td>
			<td><?php 
				$sql = "SELECT estuf AS codigo, estdescricao AS descricao FROM territorios.estado ORDER BY estdescricao";
				$db->monta_combo('estuf', $sql, 'S', 'Selecione', 'filtraruniversidade','','','','','estuf'); ?>
			</td>
		</tr>
		<tr id="tduniversidade">
			<?php 
			$aryWhere[] = "e.entstatus = 'A' AND ef.funid = ".ACA_ID_UNIVERSIDADE;		
			
			$sql = "SELECT		e.entid AS codigo, e.entnome AS descricao	
					FROM		entidade.entidade e 
					INNER JOIN	entidade.funcaoentidade ef ON ef.entid = e.entid
					LEFT JOIN	entidade.endereco ed ON ed.entid = e.entid
					LEFT JOIN	territorios.municipio mun ON mun.muncod = ed.muncod
								".(is_array($aryWhere) ? ' WHERE '.implode(' AND ', $aryWhere) : '')."
					GROUP BY 	e.entid, e.entnome , e.entsig, mun.mundescricao, mun.estuf
					ORDER BY	e.entsig, e.entnome";		
			
			mostrarComboPopup('Universidade', 'universidade', $sql, '', 'Selecione as universidades',array(),'filtrarcampusrel',true); ?>
		</tr>
		<tr>
			<td class="subtitulodireita">Campus</td>
			<td id="tdcampus">
				<select name="campus" id="campus" disabled>
					<option value="">Selecione</option>
				</select>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Ano</td>
			<td>
				<select id="ano" name="ano">
					<option value="">Selecione</option>
					<?php for ($i = 2010; $i <= date('Y'); $i++){ ?>
					<option value="<?php echo $i; ?>"><?php echo $i; ?></option>
					<?php } ?>
				</select>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Eixo</td>
			<td>
				<input type="checkbox" class="normal" value="AT" name="eixoAT" id="eixo" style="margin-top: -1px;">&nbsp;Atendimentos
				<input type="checkbox" class="normal" value="IE" name="eixoIE" id ="eixo" style="margin-top: -1px;">&nbsp;Infraestrutura
				<input type="checkbox" class="normal" value="PS" name="eixoPS" id="eixo" style="margin-top: -1px;">&nbsp;Perfil Socioeconômico
			</td>
		</tr>
		<tr>
			<td align="center" bgcolor="#CCCCCC" colspan="2">
				<input type=button value="Gerar" onclick="enviar();">
			</td>
		</tr>
	</table>
</form>
<?php } else { 
	
if($_POST['tipoentidade'] == 'campus'){
	$sql_label =  "SELECT unidsc, cmpdsc FROM academico.unicampus WHERE cmpentid = {$_POST['entidade']}";
	$aryLabel = $db->pegaLinha($sql_label);	
	
	$aryWhereAT[] = "aet.entid = {$_POST['entidade']}";
	$aryWhereIE[] = "aet.entid = {$_POST['entidade']}";
	$aryWherePS[] = "aet.entid = {$_POST['entidade']}";
} else {
	$entidades = trim($_REQUEST['entidade'],",");
	$entidades = str_replace("\'", "'",  $entidades);

	$univ = explode(",",$entidades);
	$qtduniv = count($univ);	
	
	if($qtduniv == 1){
		$sql_label =  "SELECT unidsc, 'Todos' AS cmpdsc FROM academico.unicampus WHERE unientid IN ({$entidades}) GROUP BY unidsc";
		$aryLabel = $db->pegaLinha($sql_label);		
	} elseif($qtduniv == 64){ 
		$label = "Nível Brasil"; 
		$label_campus = "Todos";	
	} else {
		$sql_label =  "SELECT unidsc FROM academico.unicampus WHERE unientid IN ({$entidades}) GROUP BY unidsc";
		$label = $db->carregarColuna($sql_label);
		$label = implode("<br><br>",$label);
		$label_campus = "Todos";
	}
	
	$aryWhereC[] = "unientid IN ({$entidades})";
	$aryWhereC[] = "orgid = 1";	
	
	$sql = "SELECT		cmpentid
			FROM 		academico.unicampus
						".(is_array($aryWhereC) ? ' WHERE '.implode(' AND ', $aryWhereC) : '')."
			GROUP BY 	cmpentid, unientid, unidsc 
			ORDER BY 	unientid";

	$aryCampus = $db->carregarColuna($sql);
	$campus = implode(',',$aryCampus);
	
	$aryWhereAT[] = "aet.entid IN ({$campus})";
	$aryWhereIE[] = "aet.entid IN ({$campus})";
	$aryWherePS[] = "aet.entid IN ({$campus})";
}


if($_POST['eixoAT']){
	if($_POST['ano']){
		$aryWhereAT[] = "aet.aetano = {$_POST['ano']}";	
	}
	
	$aryWhereAT[] = "r.perid NOT IN (3088)";	
	$aryWhereAT[] = "qrp.queid = ".QUEID_ACADEMICO_INDICADORES."";
	$aryWhereAT[] = "((trim(resdsc) <> '0' AND trim(resdsc) <> '00' AND resdsc <> '') OR (trim(tbrdsc) <> '0' AND trim(tbrdsc) <> '00' AND tbrdsc <> ''))";	
	
	$sql_atend = "SELECT 	    perid, 
								aetsemestre, 
								CASE WHEN (perid = 3098 OR perid = 3099 OR perid = 3066 OR perid = 3084 OR perid = 3096 OR perid = 3106) THEN TRIM(TO_CHAR(SUM(resdsc),'9D99')) ELSE TRIM(TO_CHAR(SUM(resdsc),'999G999G999G999')) END AS resdsc, 
								TO_CHAR(SUM(tbrdsc),'999G999G999') AS tbrdsc, 
								linid, 
								COUNT(perid) AS qperg
				  FROM			(SELECT 	  	r.perid, 
				  								CASE WHEN (r.perid = 3066 OR r.perid = 3084 OR r.perid = 3096 OR r.perid = 3106) THEN REPLACE(REPLACE(resdsc,'.',''),',','.')::NUMERIC(20,2) ELSE resdsc::NUMERIC END AS resdsc, 
												aetsemestre,
												NULLIF(TRUNC(CASE WHEN (replace(replace(replace(tbrdsc,',','.'),' ',''),'O','0') ) = '' then '0' 
         														  ELSE (replace(replace(replace(tbrdsc,',','.'),' ',''),'O','0') ) END ::numeric),'0') AS tbrdsc,
				  								linid
	  	   		  				FROM	 	  	academico.assistenciaestudantil aet
						       	INNER JOIN  	academico.itemassistencia ia on ia.aetdid = aet.aetdid
						       	INNER JOIN  	questionario.questionarioresposta qrp on qrp.qrpid = ia.qrpid
						       	INNER JOIN  	questionario.resposta r on r.qrpid = qrp.qrpid 
						        LEFT JOIN   	questionario.itempergunta ip on ip.itpid = r.itpid
						        LEFT JOIN   	questionario.tabelaresposta tr on tr.qrpid = r.qrpid AND r.perid IN (3043,3056,3073) 
		          								".(is_array($aryWhereAT) ? ' WHERE '.implode(' AND ', $aryWhereAT) : '').") AS qry 
				  GROUP BY    	qry.perid, qry.aetsemestre, qry.linid
				  ORDER BY    	perid, aetsemestre";	
	
	$dados_a = $db->carregar($sql_atend);
	
	$atend = array();
	$atend_perg = array();
	if(!empty($dados_a)){
		foreach($dados_a as $a){
			if($a['perid'] == '3043' || $a['perid'] == '3056' || $a['perid'] == '3073'){
				$atend[$a['perid']][$a['linid']][$a['aetsemestre']] = str_replace(',','.',$a['tbrdsc']);	
				$atend_perg[$a['perid']][$a['linid']][$a['aetsemestre']] = $a['qperg'];
			} else if($a['perid'] == '3066' || $a['perid'] == '3084' || $a['perid'] == '3096'){
				$atend[$a['perid']][$a['aetsemestre']] = str_replace(',','.',$a['resdsc']); 
				$atend_perg[$a['perid']][$a['aetsemestre']] = $a['qperg'];				
			} else if($a['perid'] == '3098' || $a['perid'] == '3099') {
				$atend[$a['perid']][$a['aetsemestre']] = $a['resdsc'];
				$atend_perg[$a['perid']][$a['aetsemestre']] = $a['qperg'];
			} else {
				$atend[$a['perid']][$a['aetsemestre']] = str_replace(',','.',$a['resdsc']);  
				$atend_perg[$a['perid']][$a['aetsemestre']] = $a['qperg'];				
			}
		}
	}
} 

if($_POST['eixoIE']){
	if($_POST['ano']){
		$aryWhereIE[] = "aet.aetano = {$_POST['ano']}";	
	}	
	
	$aryWhereIE[] = "qrp.queid = ".QUEID_ACADEMICO_INFRAESTRUTURA."";
	$aryWhereIE[] = "(resdsc <> '0' AND resdsc <> '00' AND resdsc <> '')";	
	
	$sql_infra = "SELECT 	    perid, aetsemestre, TRIM(TO_CHAR(SUM(resdsc),'999G999G999')) AS resdsc
				  FROM          (SELECT	  	 r.perid, 
				  							 NULLIF(REPLACE(REPLACE(resdsc,'.',''),',','.'),'')::NUMERIC AS resdsc, 
				  							 aetsemestre
				  	   		     FROM 	  	 academico.assistenciaestudantil aet
						       	 INNER JOIN  academico.itemassistencia ia ON ia.aetdid = aet.aetdid
						       	 INNER JOIN  questionario.questionarioresposta qrp ON qrp.qrpid = ia.qrpid
						       	 INNER JOIN  questionario.resposta r ON r.qrpid = qrp.qrpid 
						         LEFT JOIN   questionario.itempergunta ip ON ip.itpid = r.itpid
						          			".(is_array($aryWhereIE) ? ' WHERE '.implode(' AND ', $aryWhereIE) : '').") AS qry 
				  GROUP BY    	qry.perid, qry.aetsemestre
				  ORDER BY    	perid, aetsemestre";	
	
	$d_infra = $db->carregar($sql_infra);

	$infra = array();
	if(!empty($d_infra)){
		foreach($d_infra as $i){
			$infra[$i['perid']][$i['aetsemestre']] = str_replace(',','.',$i['resdsc']);
		}
	}
}

if($_POST['eixoPS']){
	if($_POST['ano']){
		$aryWherePS[] = "aet.aetano = {$_POST['ano']}";	
	}		
	
	$aryWherePS[] = "qrp.queid = ".QUEID_ACADEMICO_SOCIOECONOMICO."";	
	$aryWherePS[] = "((trim(resdsc) <> '0' AND trim(resdsc) <> '00' AND resdsc <> '') OR (trim(tbrdsc) <> '0' AND trim(tbrdsc) <> '00' AND tbrdsc <> ''))";	
	
	$sql_perfil = "SELECT 	   perid, 
							   aetsemestre, 
							   CASE WHEN perid = 3153 OR perid = 3170 THEN TRIM(TO_CHAR(SUM(resdsc),'9D99')) ELSE TRIM(TO_CHAR(SUM(resdsc),'999G999G999G999')) END AS resdsc, 
							   TO_CHAR(SUM(tbrdsc),'999G999G999') AS tbrdsc, 
							   linid, 
							   COUNT(perid) AS qperg
				   FROM       (SELECT 	   	r.perid, 
				   							CASE WHEN r.perid = 3153 OR r.perid = 3170 THEN REPLACE(resdsc,',','.')::NUMERIC(20,2) ELSE resdsc::NUMERIC END AS resdsc,
				   							aetsemestre, 
											NULLIF(TRUNC(REPLACE(tbrdsc,',','.')::NUMERIC),'0') AS tbrdsc, 
				   							linid
					       	   FROM	 	   	academico.assistenciaestudantil aet
					       	   INNER JOIN  	academico.itemassistencia ia ON ia.aetdid = aet.aetdid
					       	   INNER JOIN  	questionario.questionarioresposta qrp on qrp.qrpid = ia.qrpid
					       	   INNER JOIN  	questionario.resposta r ON r.qrpid = qrp.qrpid 
					           LEFT JOIN  	questionario.itempergunta ip ON ip.itpid = r.itpid
					           LEFT JOIN   	questionario.tabelaresposta tr ON tr.qrpid = r.qrpid AND r.perid = 3161
					           				".(is_array($aryWherePS) ? ' WHERE '.implode(' AND ', $aryWherePS) : '').") AS qry
				   GROUP BY    qry.perid, qry.aetsemestre, qry.linid
				   ORDER BY    perid, aetsemestre, linid";	
	
	$d_perfil = $db->carregar($sql_perfil);
	
	$perfil = array();
	$perfil_perg = array();
	if(!empty($d_perfil)){
		foreach($d_perfil as $p){
			if($p['perid'] != '3161'){
				$perfil[$p['perid']][$p['aetsemestre']] = str_replace(',','.',$p['resdsc']);
				$perfil_perg[$p['perid']][$p['aetsemestre']] = $p['qperg'];
			} else {
				$perfil[$p['perid']][$p['linid']][$p['aetsemestre']] = str_replace(',','.',$p['tbrdsc']); 
				$perfil_perg[$p['perid']][$p['linid']][$p['aetsemestre']] = $p['qperg'];
			}
		}
	}
}
?>

<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="2" cellpadding="3" border="0">
	<tr>
		<td align="center" style="font-size: 14px;">
			<?php if(!empty($aryLabel)){ ?>
			<b><?php echo $aryLabel['unidsc']; ?> - <?php echo $aryLabel['cmpdsc']; ?> - <?php echo $_POST['ano']; ?></b>
			<?php } ?>
			<?php if(!empty($label)){ ?>
			<b><?php echo $_POST['ano']; ?></b><br>
			<b><?php echo $label; ?></b>
			<?php } ?>
		</td>	
	</tr>
</table>

<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="2" cellpadding="3" border="0">
	<tr>
		<td align="center" bgcolor="#CCCCCC" width="8%"><b>UNIVERSIDADE</b></td>
		<td align="center" bgcolor="#CCCCCC" width="8%"><b>CAMPUS</b></td>
		<td align="center" bgcolor="#CCCCCC" colspan="7" rowspan="2"><b>RELATÓRIO</b></td>
	</tr>
	<tr class="subtituloEsquerda">
		<td rowspan="137" valign="top"><br><br><b><?php echo $aryLabel['unidsc']; ?><?php echo $label; ?></b></td>
		<td rowspan="137" valign="top"><br><br><b><?php echo $aryLabel['cmpdsc']; ?><?php echo $label_campus; ?></b></td>	
	</tr>		
	<?php if($_POST['eixoAT']){ ?>
	<tr>
		<td align="center" bgcolor="#CCCCCC" colspan="4"><b>ATENDIMENTOS</b></td>
		<td align="center" bgcolor="#CCCCCC" width="8%"><b>1° SEM <?php echo $_POST['ano']; ?></b></td>
		<td align="center" bgcolor="#CCCCCC" width="8%"><b>2° SEM <?php echo $_POST['ano']; ?></b></td>
		<td align="center" bgcolor="#CCCCCC" width="8%"><b>CONSOLIDADO <?php echo $_POST['ano']; ?></b></td>
	</tr>	
	<tr class="subtituloEsquerda">
		<td rowspan="9">Alimentação</td>
		<td rowspan="6">Beneficiários de auxílio pecuniário para alimentação</td>
		<td colspan=2"">Beneficiários por mês - Classe A</td>
		<td><?php echo $atend['3043']['10307']['1']; ?></td>
		<td><?php echo $atend['3043']['10307']['2']; ?></td>	
		<td><?php echo $atend['3043']['10307']['2']; ?></td>		
	</tr>		
	<tr class="subtituloEsquerda">
		<td colspan="2">Beneficiários por mês - Classe B</td>
		<td><?php echo $atend['3043']['10305']['1']; ?></td>
		<td><?php echo $atend['3043']['10305']['2']; ?></td>	
		<td><?php echo $atend['3043']['10305']['2']; ?></td>	
	</tr>	
	<tr class="subtituloEsquerda">
		<td colspan="2">Beneficiários por mês - Classe C</td>
		<td><?php echo $atend['3043']['10306']['1']; ?></td>
		<td><?php echo $atend['3043']['10306']['2']; ?></td>	
		<td><?php echo $atend['3043']['10306']['2']; ?></td>	
	</tr>	
	<tr class="subtituloEsquerda">
		<td colspan="2">Beneficiários por mês - Classe D</td>
		<td><?php echo $atend['3043']['10308']['1']; ?></td>
		<td><?php echo $atend['3043']['10308']['2']; ?></td>	
		<td><?php echo $atend['3043']['10308']['2']; ?></td>	
	</tr>	
	<tr class="subtituloEsquerda">
		<td colspan="2">Beneficiários por mês - Classe E</td>
		<td><?php echo $atend['3043']['10309']['1']; ?></td>
		<td><?php echo $atend['3043']['10309']['2']; ?></td>	
		<td><?php echo $atend['3043']['10309']['2']; ?></td>	
	</tr>	
	<tr class="subtituloEsquerda">
		<td colspan="2">Total de beneficiários por mês</td>
		<td><?php echo formata_numero(limpar_numero($atend['3043']['10307']['1']) + limpar_numero($atend['3043']['10305']['1']) + limpar_numero($atend['3043']['10306']['1']) + limpar_numero($atend['3043']['10308']['1']) + limpar_numero($atend['3043']['10309']['1'])); ?></td>
		<td><?php echo formata_numero(limpar_numero($atend['3043']['10307']['2']) + limpar_numero($atend['3043']['10305']['2']) + limpar_numero($atend['3043']['10306']['2']) + limpar_numero($atend['3043']['10308']['2']) + limpar_numero($atend['3043']['10309']['2'])); ?></td>
		<td><?php echo formata_numero(limpar_numero($atend['3043']['10307']['2']) + limpar_numero($atend['3043']['10305']['2']) + limpar_numero($atend['3043']['10306']['2']) + limpar_numero($atend['3043']['10308']['2']) + limpar_numero($atend['3043']['10309']['2'])); ?></td>
	</tr>	
	<tr class="subtituloEsquerda">
		<td colspan="3">Quantidade média de refeições servidas no almoço / dia</td>
		<td><?php echo $atend['3044']['1']; ?></td>
		<td><?php echo $atend['3044']['2']; ?></td>
		<td><?php echo $atend['3044']['2']; ?></td>	
	</tr>	
	<tr class="subtituloEsquerda">
		<td colspan="3">Quantidade de refeições servidas por dia no café da manhã	</td>
		<td><?php echo $atend['3045']['1']; ?></td>
		<td><?php echo $atend['3045']['2']; ?></td>		
		<td><?php echo $atend['3045']['2']; ?></td>		
	</tr>	
	<tr class="subtituloEsquerda">
		<td colspan="3">Quantidade de refeições servidas por dia no jantar</td>
		<td><?php echo $atend['3046']['1']; ?></td>
		<td><?php echo $atend['3046']['2']; ?></td>
		<td><?php echo $atend['3046']['2']; ?></td>	
	</tr>	
	<tr class="subtituloEsquerda">
		<td rowspan="43">Moradia</td>
		<td colspan="3">Quantidade média de alunos residentes em moradia estudantil / mês</td>
		<td><?php echo $atend['3048']['1']; ?></td>
		<td><?php echo $atend['3048']['2']; ?></td>
		<td><?php echo $atend['3048']['2']; ?></td>		
	</tr>	
	<tr class="subtituloEsquerda">
		<td rowspan="20">Recorte demográfico dos residentes em moradia estudantil por etnia, gênero, classe social e portabilidade de deficiência</td>
		<td rowspan="5">Raça/Cor</td>
		<td>Quantidade de alunos brancos</td>
		<td><?php echo $atend['3049']['1']; ?></td>
		<td><?php echo $atend['3049']['2']; ?></td>
		<td><?php echo $atend['3049']['2']; ?></td>	
	</tr>	
	<tr class="subtituloEsquerda">
		<td>Quantidade de alunos pardos</td>
		<td><?php echo $atend['3050']['1']; ?></td>
		<td><?php echo $atend['3050']['2']; ?></td>
		<td><?php echo $atend['3050']['2']; ?></td>		
	</tr>	
	<tr class="subtituloEsquerda">
		<td>Quantidade de alunos pretos</td>
		<td><?php echo $atend['3051']['1']; ?></td>
		<td><?php echo $atend['3051']['2']; ?></td>
		<td><?php echo $atend['3051']['2']; ?></td>	
	</tr>	
	<tr class="subtituloEsquerda">
		<td>Quantidade de alunos amarelos</td>
		<td><?php echo $atend['3052']['1']; ?></td>
		<td><?php echo $atend['3052']['2']; ?></td>
		<td><?php echo $atend['3052']['2']; ?></td>		
	</tr>	
	<tr class="subtituloEsquerda">
		<td>Quantidade de alunos indígenas</td>
		<td><?php echo $atend['3053']['1']; ?></td>
		<td><?php echo $atend['3053']['2']; ?></td>
		<td><?php echo $atend['3053']['2']; ?></td>		
	</tr>		
	<tr class="subtituloEsquerda">
		<td rowspan="2">Gênero/Sexo</td>
		<td>Quantidade de alunos do sexo masculino</td>
		<td><?php echo $atend['3054']['1']; ?></td>
		<td><?php echo $atend['3054']['2']; ?></td>
		<td><?php echo $atend['3054']['2']; ?></td>	
	</tr>		
	<tr class="subtituloEsquerda">
		<td>Quantidade de alunos do sexo feminino</td>
		<td><?php echo $atend['3055']['1']; ?></td>
		<td><?php echo $atend['3055']['2']; ?></td>	
		<td><?php echo $atend['3055']['2']; ?></td>		
	</tr>	
	<tr class="subtituloEsquerda">
		<td rowspan="5">Beneficiários por classe socioeconômica</td>
		<td>Quantidade de alunos da classe A</td>
		<td><?php echo $atend['3056']['10310']['1']; ?></td>
		<td><?php echo $atend['3056']['10310']['2']; ?></td>
		<td><?php echo $atend['3056']['10310']['2']; ?></td>		
	</tr>	
	<tr class="subtituloEsquerda">
		<td>Quantidade de alunos da classe B</td>
		<td><?php echo $atend['3056']['10311']['1']; ?></td>
		<td><?php echo $atend['3056']['10311']['2']; ?></td>
		<td><?php echo $atend['3056']['10311']['2']; ?></td>		
	</tr>	
	<tr class="subtituloEsquerda">
		<td>Quantidade de alunos da classe C</td>
		<td><?php echo $atend['3056']['10312']['1']; ?></td>
		<td><?php echo $atend['3056']['10312']['2']; ?></td>	
		<td><?php echo $atend['3056']['10312']['2']; ?></td>	
	</tr>	
	<tr class="subtituloEsquerda">
		<td>Quantidade de alunos da classe D</td>
		<td><?php echo $atend['3056']['10313']['1']; ?></td>
		<td><?php echo $atend['3056']['10313']['2']; ?></td>
		<td><?php echo $atend['3056']['10313']['2']; ?></td>		
	</tr>	
	<tr class="subtituloEsquerda">
		<td>Quantidade de alunos da classe E</td>
		<td><?php echo $atend['3056']['10314']['1']; ?></td>
		<td><?php echo $atend['3056']['10314']['2']; ?></td>
		<td><?php echo $atend['3056']['10314']['2']; ?></td>		
	</tr>	
	<tr class="subtituloEsquerda">
		<td rowspan="8">Portabilidade de deficiência</td>
		<td>Quantidade de estudantes com cegueira</td>
		<td><?php echo $atend['3057']['1']; ?></td>
		<td><?php echo $atend['3057']['2']; ?></td>
		<td><?php echo $atend['3057']['2']; ?></td>		
	</tr>	
	<tr class="subtituloEsquerda">
		<td>Quantidade de estudantes com baixa visão</td>
		<td><?php echo $atend['3058']['1']; ?></td>
		<td><?php echo $atend['3058']['2']; ?></td>
		<td><?php echo $atend['3058']['2']; ?></td>	
	</tr>		
	<tr class="subtituloEsquerda">
		<td>Quantidade de alunos com surdez</td>
		<td><?php echo $atend['3059']['1']; ?></td>
		<td><?php echo $atend['3059']['2']; ?></td>
		<td><?php echo $atend['3059']['2']; ?></td>	
	</tr>		
	<tr class="subtituloEsquerda">
		<td>Quantidade de alunos com deficiência auditiva</td>
		<td><?php echo $atend['3060']['1']; ?></td>
		<td><?php echo $atend['3060']['2']; ?></td>
		<td><?php echo $atend['3060']['2']; ?></td>	
	</tr>		
	<tr class="subtituloEsquerda">
		<td>Quantidade de alunos com deficiência física</td>
		<td><?php echo $atend['3061']['1']; ?></td>
		<td><?php echo $atend['3061']['2']; ?></td>
		<td><?php echo $atend['3061']['2']; ?></td>		
	</tr>		
	<tr class="subtituloEsquerda">
		<td>Quantidade de alunos com surdo/cegueira</td>
		<td><?php echo $atend['3062']['1']; ?></td>
		<td><?php echo $atend['3062']['2']; ?></td>
		<td><?php echo $atend['3062']['2']; ?></td>	
	</tr>	
	<tr class="subtituloEsquerda">
		<td>Quantidade de alunos com deficiência múltipla</td>
		<td><?php echo $atend['3063']['1']; ?></td>
		<td><?php echo $atend['3063']['2']; ?></td>
		<td><?php echo $atend['3063']['2']; ?></td>		
	</tr>		
	<tr class="subtituloEsquerda">
		<td>Quantidade de alunos com deficiência mental</td>
		<td><?php echo $atend['3064']['1']; ?></td>
		<td><?php echo $atend['3064']['2']; ?></td>
		<td><?php echo $atend['3064']['2']; ?></td>		
	</tr>	
	<tr class="subtituloEsquerda">
		<td colspan="3">Quantidade média de alunos beneficiados com auxílio moradia / mês</td>
		<td><?php echo $atend['3047']['1']; ?></td>
		<td><?php echo $atend['3047']['2']; ?></td>
		<td><?php echo $atend['3047']['2']; ?></td>	
	</tr>		
	<tr class="subtituloEsquerda">
		<td colspan="3">Valor do auxilio moradia (R$)</td>
		<td><?php echo $atend['3066']['1'] == 0 ? '0' : formata_valor($atend['3066']['1']/$atend_perg['3066']['1']); ?></td>
		<td><?php echo $atend['3066']['2'] == 0 ? '0' : formata_valor($atend['3066']['2']/$atend_perg['3066']['2']); ?></td>
		<td><?php echo $atend['3066']['2'] == 0 ? '0' : formata_valor($atend['3066']['2']/$atend_perg['3066']['2']); ?></td>		
	</tr>		
	<tr class="subtituloEsquerda">
		<td rowspan="20">Recorte demográfico dos beneficiados por etnia, gênero, classe social e portabilidade de deficiência</td>
		<td rowspan="5">Raça/Cor</td>
		<td>Quantidade de alunos brancos</td>
		<td><?php echo $atend['3067']['1']; ?></td>
		<td><?php echo $atend['3067']['2']; ?></td>	
		<td><?php echo $atend['3067']['2']; ?></td>	
	</tr>	
	<tr class="subtituloEsquerda">
		<td>Quantidade de alunos pardos</td>
		<td><?php echo $atend['3068']['1']; ?></td>
		<td><?php echo $atend['3068']['2']; ?></td>
		<td><?php echo $atend['3068']['2']; ?></td>	
	</tr>	
	<tr class="subtituloEsquerda">
		<td>Quantidade de alunos pretos</td>
		<td><?php echo $atend['3171']['1']; ?></td>
		<td><?php echo $atend['3171']['2']; ?></td>
		<td><?php echo $atend['3171']['2']; ?></td>	
	</tr>	
	<tr class="subtituloEsquerda">
		<td>Quantidade de alunos amarelos</td>
		<td><?php echo $atend['3069']['1']; ?></td>
		<td><?php echo $atend['3069']['2']; ?></td>
		<td><?php echo $atend['3069']['2']; ?></td>		
	</tr>	
	<tr class="subtituloEsquerda">
		<td>Quantidade de alunos indígenas</td>
		<td><?php echo $atend['3070']['1']; ?></td>
		<td><?php echo $atend['3070']['2']; ?></td>
		<td><?php echo $atend['3070']['2']; ?></td>		
	</tr>		
	<tr class="subtituloEsquerda">
		<td rowspan="2">Gênero/Sexo</td>
		<td>Quantidade de alunos do sexo masculino</td>
		<td><?php echo $atend['3071']['1']; ?></td>
		<td><?php echo $atend['3071']['2']; ?></td>
		<td><?php echo $atend['3071']['2']; ?></td>	
	</tr>		
	<tr class="subtituloEsquerda">
		<td>Quantidade de alunos do sexo feminino</td>
		<td><?php echo $atend['3072']['1']; ?></td>
		<td><?php echo $atend['3072']['2']; ?></td>
		<td><?php echo $atend['3072']['2']; ?></td>	
	</tr>	
	<tr class="subtituloEsquerda">
		<td rowspan="5">Beneficiários por classe socioeconômica</td>
		<td>Quantidade de alunos da classe A</td>
		<td><?php echo $atend['3073']['10315']['1']; ?></td>
		<td><?php echo $atend['3073']['10315']['2']; ?></td>
		<td><?php echo $atend['3073']['10315']['2']; ?></td>		
	</tr>	
	<tr class="subtituloEsquerda">
		<td>Quantidade de alunos da classe B</td>
		<td><?php echo $atend['3073']['10316']['1']; ?></td>
		<td><?php echo $atend['3073']['10316']['2']; ?></td>
		<td><?php echo $atend['3073']['10316']['2']; ?></td>		
	</tr>	
	<tr class="subtituloEsquerda">
		<td>Quantidade de alunos da classe C</td>
		<td><?php echo $atend['3073']['10317']['1']; ?></td>
		<td><?php echo $atend['3073']['10317']['2']; ?></td>
		<td><?php echo $atend['3073']['10317']['2']; ?></td>		
	</tr>	
	<tr class="subtituloEsquerda">
		<td>Quantidade de alunos da classe D</td>
		<td><?php echo $atend['3073']['10318']['1']; ?></td>
		<td><?php echo $atend['3073']['10318']['2']; ?></td>
		<td><?php echo $atend['3073']['10318']['2']; ?></td>		
	</tr>	
	<tr class="subtituloEsquerda">
		<td>Quantidade de alunos da classe E</td>
		<td><?php echo $atend['3073']['10319']['1']; ?></td>
		<td><?php echo $atend['3073']['10319']['2']; ?></td>
		<td><?php echo $atend['3073']['10319']['2']; ?></td>		
	</tr>	
	<tr class="subtituloEsquerda">
		<td rowspan="8">Portabilidade de deficiência</td>
		<td>Quantidade de estudantes com cegueira</td>
		<td><?php echo $atend['3074']['1']; ?></td>
		<td><?php echo $atend['3074']['2']; ?></td>
		<td><?php echo $atend['3074']['2']; ?></td>		
	</tr>	
	<tr class="subtituloEsquerda">
		<td>Quantidade de estudantes com baixa visão</td>
		<td><?php echo $atend['3075']['1']; ?></td>
		<td><?php echo $atend['3075']['2']; ?></td>
		<td><?php echo $atend['3075']['2']; ?></td>	
	</tr>		
	<tr class="subtituloEsquerda">
		<td>Quantidade de alunos com surdez</td>
		<td><?php echo $atend['3076']['1']; ?></td>
		<td><?php echo $atend['3076']['2']; ?></td>
		<td><?php echo $atend['3076']['2']; ?></td>		
	</tr>		
	<tr class="subtituloEsquerda">
		<td>Quantidade de alunos com deficiência auditiva</td>
		<td><?php echo $atend['3077']['1']; ?></td>
		<td><?php echo $atend['3077']['2']; ?></td>
		<td><?php echo $atend['3077']['2']; ?></td>	
	</tr>		
	<tr class="subtituloEsquerda">
		<td>Quantidade de alunos com deficiência física</td>
		<td><?php echo $atend['3078']['1']; ?></td>
		<td><?php echo $atend['3078']['2']; ?></td>
		<td><?php echo $atend['3078']['2']; ?></td>	
	</tr>		
	<tr class="subtituloEsquerda">
		<td>Quantidade de alunos com surdo/cegueira</td>
		<td><?php echo $atend['3079']['1']; ?></td>
		<td><?php echo $atend['3079']['2']; ?></td>
		<td><?php echo $atend['3079']['2']; ?></td>		
	</tr>	
	<tr class="subtituloEsquerda">
		<td>Quantidade de alunos com deficiência múltipla</td>
		<td><?php echo $atend['3080']['1']; ?></td>
		<td><?php echo $atend['3080']['2']; ?></td>
		<td><?php echo $atend['3080']['2']; ?></td>	
	</tr>		
	<tr class="subtituloEsquerda">
		<td>Quantidade de alunos com deficiência mental</td>
		<td><?php echo $atend['3081']['1']; ?></td>
		<td><?php echo $atend['3081']['2']; ?></td>
		<td><?php echo $atend['3081']['2']; ?></td>		
	</tr>	
	<tr class="subtituloEsquerda">
		<td rowspan="2">Transporte</td>
		<td colspan="3">Quantidade média de alunos beneficiados com auxílio transporte / mês</td>
		<td><?php echo $atend['3083']['1']; ?></td>
		<td><?php echo $atend['3083']['2']; ?></td>
		<td><?php echo $atend['3083']['2']; ?></td>	
	</tr>			
	<tr class="subtituloEsquerda">
		<td colspan="3">Valor do auxílio transporte (R$)</td>
		<td><?php echo $atend['3084']['1'] == 0 ? '0' : formata_valor($atend['3084']['1']/$atend_perg['3084']['1']); ?></td>
		<td><?php echo $atend['3084']['2'] == 0 ? '0' : formata_valor($atend['3084']['2']/$atend_perg['3084']['2']); ?></td>
		<td><?php echo $atend['3084']['1'] == 0 ? '0' : formata_valor($atend['3084']['2']/$atend_perg['3084']['2']); ?></td>		
	</tr>		
	<tr class="subtituloEsquerda">
		<td rowspan="2">Atividades Culturais</td>
		<td colspan="3">Quantidade de eventos culturais / semestre</td>
		<td><?php echo $atend['3085']['1']; ?></td>
		<td><?php echo $atend['3085']['2']; ?></td>	
		<td><?php echo formata_numero(round((limpar_numero($atend['3085']['1']) + limpar_numero($atend['3085']['2']))/2)); ?></td>	
	</tr>			
	<tr class="subtituloEsquerda">
		<td colspan="3">Quantidade de público participante / semestre</td>
		<td><?php echo $atend['3086']['1']; ?></td>
		<td><?php echo $atend['3086']['2']; ?></td>
		<td><?php echo formata_numero(round((limpar_numero($atend['3086']['1']) + limpar_numero($atend['3086']['2']))/2)); ?></td>
	</tr>
	<tr class="subtituloEsquerda">
		<td rowspan="6">Atividades Esportivas</td>
		<td colspan="3">Quantidade de alunos participantes de prática esportiva curricular/semestre</td>
		<td><?php echo $atend['3089']['1']; ?></td>
		<td><?php echo $atend['3089']['2']; ?></td>
		<td><?php echo formata_numero(round((limpar_numero($atend['3089']['1']) + limpar_numero($atend['3089']['2']))/2)); ?></td>	
	</tr>		
	<tr class="subtituloEsquerda">
		<td colspan="3">Quantidade de delegações esportivas apoiadas em eventos externos / semestre</td>
		<td><?php echo $atend['3090']['1']; ?></td>
		<td><?php echo $atend['3090']['2']; ?></td>
		<td><?php echo formata_numero(round((limpar_numero($atend['3090']['1']) + limpar_numero($atend['3090']['2']))/2)); ?></td>
	</tr>
	<tr class="subtituloEsquerda">
		<td colspan="3">Quantidade Média de alunos atletas por delegação</td>
		<td><?php echo $atend['3091']['1']; ?></td>
		<td><?php echo $atend['3091']['2']; ?></td>
		<td><?php echo formata_numero(round((limpar_numero($atend['3091']['1']) + limpar_numero($atend['3091']['2']))/2)); ?></td>
	</tr>		
	<tr class="subtituloEsquerda">
		<td colspan="3">Quantidade de eventos esportivos realizados pela instituição/semestre</td>
		<td><?php echo $atend['3092']['1']; ?></td>
		<td><?php echo $atend['3092']['2']; ?></td>
		<td><?php echo formata_numero(round((limpar_numero($atend['3092']['1']) + limpar_numero($atend['3092']['2']))/2)); ?></td>
	</tr>			
	<tr class="subtituloEsquerda">
		<td colspan="3">Quantidade média de alunos atletas participantes por evento</td>
		<td><?php echo $atend['3093']['1']; ?></td>
		<td><?php echo $atend['3093']['2']; ?></td>
		<td><?php echo formata_numero(round((limpar_numero($atend['3093']['1']) + limpar_numero($atend['3093']['2']))/2)); ?></td>	
	</tr>		
	<tr class="subtituloEsquerda">
		<td colspan="3">Quantidade média de público total presente por evento</td>
		<td><?php echo $atend['3094']['1']; ?></td>
		<td><?php echo $atend['3094']['2']; ?></td>
		<td><?php echo formata_numero(round((limpar_numero($atend['3094']['1']) + limpar_numero($atend['3094']['2']))/2)); ?></td>	
	</tr>		
	<tr class="subtituloEsquerda">
		<td rowspan="4">Auxílio Creche</td>
		<td colspan="3">Qual é o valor do auxílio (R$) ?</td>
		<td><?php echo $atend['3096']['1'] == 0 ? '0' : formata_valor($atend['3096']['1']/$atend_perg['3096']['1']); ?></td>
		<td><?php echo $atend['3096']['2'] == 0 ? '0' : formata_valor($atend['3096']['2']/$atend_perg['3096']['2']); ?></td>
		<td><?php echo $atend['3096']['2'] == 0 ? '0' : formata_valor($atend['3096']['2']/$atend_perg['3096']['2']); ?></td>	
	</tr>	
	<tr class="subtituloEsquerda">
		<td colspan="3">Quantidade de alunos beneficiados por semestre</td>
		<td><?php echo $atend['3097']['1']; ?></td>
		<td><?php echo $atend['3097']['2']; ?></td>
		<td><?php echo formata_numero(round((limpar_numero($atend['3097']['1']) + limpar_numero($atend['3097']['2']))/2)); ?></td>	
	</tr>		
	<tr class="subtituloEsquerda">
		<td colspan="3">Proporção de homens entre os alunos beneficiados (%)</td>
		<?php $atend['3098']['1'] == 0 ? $atend['3098']['1'] = 0 : $atend['3098']['1'] = ($atend['3098']['1']/$atend_perg['3098']['1']);
			  $atend['3098']['2'] == 0 ? $atend['3098']['2'] = 0 : $atend['3098']['2'] = ($atend['3098']['2']/$atend_perg['3098']['2']);  ?>		
		<td><?php echo str_replace(".",",",round($atend['3098']['1'],2)); ?></td>
		<td><?php echo str_replace(".",",",round($atend['3098']['2'],2)); ?></td>
		<td><?php echo str_replace(".",",",round((($atend['3098']['1']+$atend['3098']['2'])/2),2)); ?></td>
	</tr>	
	<tr class="subtituloEsquerda">
		<td colspan="3">Proporção de mulheres entre os alunos beneficiados (%)</td>
		<?php $atend['3099']['1'] == 0 ? $atend['3099']['1'] = 0 : $atend['3099']['1'] = ($atend['3099']['1']/$atend_perg['3099']['1']);
			  $atend['3099']['2'] == 0 ? $atend['3099']['2'] = 0 : $atend['3099']['2'] = ($atend['3099']['2']/$atend_perg['3099']['2']);  ?>				
		<td><?php echo str_replace(".",",",round($atend['3099']['1'],2)); ?></td>
		<td><?php echo str_replace(".",",",round($atend['3099']['2'],2)); ?></td>
		<td><?php echo str_replace(".",",",round((($atend['3099']['1']+$atend['3099']['2'])/2),2)); ?></td>
	</tr>	
	<tr class="subtituloEsquerda">
		<td>Apoio Pedagógico</td>
		<td colspan="3">Quantidade de alunos beneficados por apoio pedagógico / semestre</td>
		<td><?php echo $atend['3100']['1']; ?></td>
		<td><?php echo $atend['3100']['2']; ?></td>
		<td><?php echo formata_numero(round((limpar_numero($atend['3100']['1']) + limpar_numero($atend['3100']['2']))/2)); ?></td>
	</tr>									
	<tr class="subtituloEsquerda">
		<td rowspan="3">Saúde</td>
		<td colspan="3">Quantidade de campanhas de prevenção</td>
		<td><?php echo $atend['3101']['1']; ?></td>
		<td><?php echo $atend['3101']['2']; ?></td>
		<td><?php echo formata_numero(limpar_numero($atend['3101']['1']) + limpar_numero($atend['3101']['2'])); ?></td>
	</tr>	
	<tr class="subtituloEsquerda">
		<td colspan="3">Quantidade de alunos com problemas com álcool e outras drogas acompanhados pelos serviços de ajuda</td>
		<td><?php echo $atend['3102']['1']; ?></td>
		<td><?php echo $atend['3102']['2']; ?></td>
		<td><?php echo formata_numero(limpar_numero($atend['3102']['1']) + limpar_numero($atend['3102']['2'])); ?></td>
	</tr>		
	<tr class="subtituloEsquerda">
		<td colspan="3">Quantidade de alunos que procuram o serviço psicológico/semestre</td>
		<td><?php echo $atend['3103']['1']; ?></td>
		<td><?php echo $atend['3103']['2']; ?></td>
		<td><?php echo formata_numero(round((limpar_numero($atend['3103']['1']) + limpar_numero($atend['3103']['2']))/2)); ?></td>	
	</tr>		
	<tr class="subtituloEsquerda">
		<td rowspan="2">Permanência</td>
		<td colspan="3">Quantidade de bolsas de permanência concedidas</td>
		<td><?php echo $atend['3105']['1']; ?></td>
		<td><?php echo $atend['3105']['2']; ?></td>
		<td><?php echo formata_numero(limpar_numero($atend['3105']['1']) + limpar_numero($atend['3105']['2'])); ?></td>	
	</tr>	
	<tr class="subtituloEsquerda">
		<td colspan="3">Valor total das bolsas concedidas (R$)</td>
		<td><?php echo formata_valor($atend['3106']['1']); ?></td>
		<td><?php echo formata_valor($atend['3106']['2']); ?></td>
		<td><?php echo formata_valor($atend['3106']['1'] + $atend['3106']['2']); ?></td>	
	</tr>
	<?php } ?>	
	<?php if($_POST['eixoIE']){?>	
	<tr>
		<td align="center" bgcolor="#CCCCCC" colspan="4"><b>INFRAESTRUTURA</b></td>
		<td align="center" bgcolor="#CCCCCC" width="8%"><b>1° SEM <?php echo $_POST['ano']; ?></b></td>
		<td align="center" bgcolor="#CCCCCC" width="8%"><b>2° SEM <?php echo $_POST['ano']; ?></b></td>
		<td align="center" bgcolor="#CCCCCC" width="8%"><b>CONSOLIDADO <?php echo $_POST['ano']; ?></b></td>
	</tr>
	<tr class="subtituloEsquerda">
		<td rowspan="2">Alimentação</td>
		<td colspan="3">Capacidade máxima de refeições servidas no almoço nos restaurantes universitários</td>
		<td><?php echo $infra['3108']['1']; ?></td>
		<td><?php echo $infra['3108']['2']; ?></td>
		<td><?php echo $infra['3108']['2']; ?></td>
	</tr>	
	<tr class="subtituloEsquerda">
		<td colspan="3">Capacidade máxima de refeições servidas no almoço nos refeitórios</td>
		<td><?php echo $infra['3110']['1']; ?></td>
		<td><?php echo $infra['3110']['2']; ?></td>
		<td><?php echo $infra['3110']['2']; ?></td>
	</tr>		
	<tr class="subtituloEsquerda">
		<td rowspan="2">Moradia</td>
		<td colspan="3">Quantidade de leitos em moradia estudantil própria</td>
		<td><?php echo $infra['3112']['1']; ?></td>
		<td><?php echo $infra['3112']['2']; ?></td>
		<td><?php echo $infra['3112']['2']; ?></td>
	</tr>	
	<tr class="subtituloEsquerda">
		<td colspan="3">Quantidade de leitos em moradia estudantil alugada</td>
		<td><?php echo $infra['3114']['1']; ?></td>
		<td><?php echo $infra['3114']['2']; ?></td>
		<td><?php echo $infra['3114']['2']; ?></td>
	</tr>		
	<tr class="subtituloEsquerda">
		<td>Transporte</td>
		<td colspan="3">Quantidade de ônibus</td>
		<td><?php echo $infra['3115']['1']; ?></td>
		<td><?php echo $infra['3115']['2']; ?></td>
		<td><?php echo $infra['3115']['2']; ?></td>
	</tr>			

	<tr class="subtituloEsquerda">
		<td rowspan="4">Atividades Culturais</td>
		<td colspan="3">Quantidade em m2 construídos de anfiteatro	</td>
		<td><?php echo $infra['3117']['1']; ?></td>
		<td><?php echo $infra['3117']['2']; ?></td>
		<td><?php echo $infra['3117']['2']; ?></td>
	</tr>	
	<tr class="subtituloEsquerda">
		<td colspan="3">Quantidade em m2 construídos de salas de cinema</td>
		<td><?php echo $infra['3119']['1']; ?></td>
		<td><?php echo $infra['3119']['2']; ?></td>
		<td><?php echo $infra['3119']['2']; ?></td>
	</tr>	
	<tr class="subtituloEsquerda">
		<td colspan="3">Quantidade em m2 construídos de teatros</td>
		<td><?php echo $infra['3121']['1']; ?></td>
		<td><?php echo $infra['3121']['2']; ?></td>	
		<td><?php echo $infra['3121']['2']; ?></td>
	</tr>	
	<tr class="subtituloEsquerda">
		<td colspan="3">Quantidade em m2 construídos de outros espaços culturais</td>
		<td><?php echo $infra['3123']['1']; ?></td>
		<td><?php echo $infra['3123']['2']; ?></td>	
		<td><?php echo $infra['3123']['2']; ?></td>
	</tr>			
	<tr class="subtituloEsquerda">
		<td rowspan="18">Atividades Esportivas</td>
		<td colspan="3">Quantidade de campos de futebol oficiais</td>
		<td><?php echo $infra['3124']['1']; ?></td>
		<td><?php echo $infra['3124']['2']; ?></td>	
		<td><?php echo $infra['3124']['2']; ?></td>
	</tr>	
	<tr class="subtituloEsquerda">
		<td colspan="3">Quantidade de campos de futebol society</td>
		<td><?php echo $infra['3125']['1']; ?></td>
		<td><?php echo $infra['3125']['2']; ?></td>	
		<td><?php echo $infra['3125']['2']; ?></td>
	</tr>	
	<tr class="subtituloEsquerda">
		<td colspan="3">Quantidade de piscinas olímpicas</td>
		<td><?php echo $infra['3126']['1']; ?></td>
		<td><?php echo $infra['3126']['2']; ?></td>	
		<td><?php echo $infra['3126']['2']; ?></td>
	</tr>
	<tr class="subtituloEsquerda">
		<td colspan="3">Quantidade de piscinas recreativas</td>
		<td><?php echo $infra['3127']['1']; ?></td>
		<td><?php echo $infra['3127']['2']; ?></td>	
		<td><?php echo $infra['3127']['2']; ?></td>
	</tr>		
	<tr class="subtituloEsquerda">
		<td colspan="3">Quantidade de piscinas semi-olímpicas</td>
		<td><?php echo $infra['3128']['1']; ?></td>
		<td><?php echo $infra['3128']['2']; ?></td>	
		<td><?php echo $infra['3128']['2']; ?></td>
	</tr>		
	<tr class="subtituloEsquerda">
		<td colspan="3">Quantidade de pistas de atletismo</td>
		<td><?php echo $infra['3129']['1']; ?></td>
		<td><?php echo $infra['3129']['2']; ?></td>	
		<td><?php echo $infra['3129']['2']; ?></td>
	</tr>	
	<tr class="subtituloEsquerda">
		<td colspan="3">Quantidade de quadras de tênis</td>
		<td><?php echo $infra['3130']['1']; ?></td>
		<td><?php echo $infra['3130']['2']; ?></td>	
		<td><?php echo $infra['3130']['2']; ?></td>
	</tr>	
	<tr class="subtituloEsquerda">
		<td colspan="3">Quantidade de salas de lutas</td>
		<td><?php echo $infra['3131']['1']; ?></td>
		<td><?php echo $infra['3131']['2']; ?></td>	
		<td><?php echo $infra['3131']['2']; ?></td>
	</tr>		
	<tr class="subtituloEsquerda">
		<td colspan="3">Quantidade de salas para musculação</td>
		<td><?php echo $infra['3132']['1']; ?></td>
		<td><?php echo $infra['3132']['2']; ?></td>	
		<td><?php echo $infra['3132']['2']; ?></td>
	</tr>	
	<tr class="subtituloEsquerda">
		<td colspan="3">Quantidade de tanques para saltos ornamentais</td>
		<td><?php echo $infra['3133']['1']; ?></td>
		<td><?php echo $infra['3133']['2']; ?></td>	
		<td><?php echo $infra['3133']['2']; ?></td>
	</tr>	
	<tr class="subtituloEsquerda">
		<td colspan="3">Quantidade de quadras de esportes de areia (volei, futvolei e futebol)</td>
		<td><?php echo $infra['3134']['1']; ?></td>
		<td><?php echo $infra['3134']['2']; ?></td>	
		<td><?php echo $infra['3134']['2'];?></td>
	</tr>		
	<tr class="subtituloEsquerda">
		<td colspan="3">Quantidade de salas para ginástica artística equipadas</td>
		<td><?php echo $infra['3135']['1']; ?></td>
		<td><?php echo $infra['3135']['2']; ?></td>	
		<td><?php echo $infra['3135']['2']; ?></td>
	</tr>	
	<tr class="subtituloEsquerda">
		<td colspan="3">Quantidade de salas para ginástica rítmica desportiva</td>
		<td><?php echo $infra['3136']['1']; ?></td>
		<td><?php echo $infra['3136']['2']; ?></td>	
		<td><?php echo $infra['3136']['2']; ?></td>
	</tr>		
	<tr class="subtituloEsquerda">
		<td colspan="3">Quantidade de Ginásios</td>
		<td><?php echo $infra['3137']['1']; ?></td>
		<td><?php echo $infra['3137']['2']; ?></td>	
		<td><?php echo $infra['3137']['2']; ?></td>
	</tr>		
	<tr class="subtituloEsquerda">
		<td colspan="3">Capacidade máxima de público em ginásio</td>
		<td><?php echo $infra['3138']['1']; ?></td>
		<td><?php echo $infra['3138']['2']; ?></td>	
		<td><?php echo $infra['3138']['2']; ?></td>
	</tr>	
	<tr class="subtituloEsquerda">
		<td colspan="3">Quantidade de quadras poliesportivas (exceto em ginásio)</td>
		<td><?php echo $infra['3139']['1']; ?></td>
		<td><?php echo $infra['3139']['2']; ?></td>	
		<td><?php echo $infra['3139']['2']; ?></td>
	</tr>		
	<tr class="subtituloEsquerda">
		<td colspan="3">Número de quadras cobertas</td>
		<td><?php echo $infra['3140']['1']; ?></td>
		<td><?php echo $infra['3140']['2']; ?></td>	
		<td><?php echo $infra['3140']['2']; ?></td>
	</tr>	
	<tr class="subtituloEsquerda">
		<td colspan="3">Número de quadras descobertas</td>
		<td><?php echo $infra['3141']['1']; ?></td>
		<td><?php echo $infra['3141']['2']; ?></td>	
		<td><?php echo $infra['3141']['2']; ?></td>
	</tr>		
	<tr class="subtituloEsquerda">
		<td>Creche</td>
		<td colspan="3">Qual é a capacidade máxima de atendimento de crianças?</td>
		<td><?php echo $infra['3143']['1']; ?></td>
		<td><?php echo $infra['3143']['2']; ?></td>	
		<td><?php echo $infra['3143']['2']; ?></td>
	</tr>		
	<tr class="subtituloEsquerda">
		<td rowspan="6">Acessibilidade</td>
		<td colspan="3">Quantidade de elevadores</td>
		<td><?php echo $infra['3144']['1']; ?></td>
		<td><?php echo $infra['3144']['2']; ?></td>	
		<td><?php echo $infra['3144']['2']; ?></td>	
	</tr>	
	<tr class="subtituloEsquerda">
		<td colspan="3">Quantidade de salas de aula acessíveis</td>
		<td><?php echo $infra['3145']['1']; ?></td>
		<td><?php echo $infra['3145']['2']; ?></td>	
		<td><?php echo $infra['3145']['2']; ?></td>
	</tr>	
	<tr class="subtituloEsquerda">
		<td colspan="3">Quantidade de banheiros acessíveis</td>
		<td><?php echo $infra['3146']['1']; ?></td>
		<td><?php echo $infra['3146']['2']; ?></td>	
		<td><?php echo $infra['3146']['2']; ?></td>
	</tr>	
	<tr class="subtituloEsquerda">
		<td colspan="3">Quantidade de rampas de acessibilidade</td>
		<td><?php echo $infra['3147']['1']; ?></td>
		<td><?php echo $infra['3147']['2']; ?></td>	
		<td><?php echo $infra['3147']['2']; ?></td>
	</tr>	
	<tr class="subtituloEsquerda">
		<td colspan="3">Metros lineares de piso tátil instalados</td>
		<td><?php echo $infra['3148']['1']; ?></td>
		<td><?php echo $infra['3148']['2']; ?></td>	
		<td><?php echo $infra['3148']['2']; ?></td>
	</tr>	
	<tr class="subtituloEsquerda">
		<td colspan="3">Quantidade de impressoras Braile</td>
		<td><?php echo $infra['3149']['1']; ?></td>
		<td><?php echo $infra['3149']['2']; ?></td>	
		<td><?php echo $infra['3149']['2']; ?></td>
	</tr>							
	<tr class="subtituloEsquerda">
		<td rowspan="2">Inclusão Digital</td>
		<td colspan="3">Quantidade de TABLETS disponibilizados para acesso estudantil</td>
		<td><?php echo $infra['3150']['1']; ?></td>
		<td><?php echo $infra['3150']['2']; ?></td>	
		<td><?php echo $infra['3150']['2']; ?></td>
	</tr>	
	<tr class="subtituloEsquerda">
		<td colspan="3">Quantidade computadores disponibilizados para acesso estudantil</td>
		<td><?php echo $infra['3151']['1']; ?></td>
		<td><?php echo $infra['3151']['2']; ?></td>	
		<td><?php echo $infra['3151']['2']; ?></td>
	</tr>	
	<?php } ?>
	<?php if($_POST['eixoPS']){ ?>	
	<tr>
		<td align="center" bgcolor="#CCCCCC" colspan="4"><b>PERFIL SOCIOECONÔMICO DOS ALUNOS ASSISTIDOS</b></td>
		<td align="center" bgcolor="#CCCCCC" width="8%"><b>1° SEM <?php echo $_POST['ano']; ?></b></td>
		<td align="center" bgcolor="#CCCCCC" width="8%"><b>2° SEM <?php echo $_POST['ano']; ?></b></td>
		<td align="center" bgcolor="#CCCCCC" width="8%"><b>CONSOLIDADO <?php echo $_POST['ano']; ?></b></td>
	</tr>
	<tr class="subtituloEsquerda">
		<td colspan="4">Total de alunos matriculados</td>
		<td><?php echo $perfil['3152']['1']; ?></td>
		<td><?php echo $perfil['3152']['2']; ?></td>
		<td><?php echo $perfil['3152']['2']; ?></td>
	</tr>	
	<tr class="subtituloEsquerda">
		<td colspan="4">Proporção de alunos egressos de escolas públicas (%)</td>
		<?php $perfil['3153']['1'] == 0 ? $perfil['3153']['1'] = 0 : $perfil['3153']['1'] = $perfil['3153']['1']/$perfil_perg['3153']['1'];
			  $perfil['3153']['2'] == 0 ? $perfil['3153']['2'] = 0 : $perfil['3153']['2'] = $perfil['3153']['2']/$perfil_perg['3153']['2'];  ?>
		<td><?php echo str_replace(".",",",round($perfil['3153']['1'],2)); ?></td>
		<td><?php echo str_replace(".",",",round($perfil['3153']['2'],2)); ?></td>
		<td><?php echo str_replace(".",",",round((($perfil['3153']['1']+$perfil['3153']['2'])/2),2));?></td>	
	</tr>	
	<tr class="subtituloEsquerda">
		<td rowspan="5">Raça/Cor</td>
		<td colspan="3">Quantidade de alunos brancos</td>
		<td><?php echo $perfil['3154']['1']; ?></td>
		<td><?php echo $perfil['3154']['2']; ?></td>
		<td><?php echo $perfil['3154']['2']; ?></td>
	</tr>	
	<tr class="subtituloEsquerda">
		<td colspan="3">Quantidade de alunos pardos</td>
		<td><?php echo $perfil['3155']['1']; ?></td>
		<td><?php echo $perfil['3155']['2']; ?></td>
		<td><?php echo $perfil['3155']['2']; ?></td>
	</tr>	
	<tr class="subtituloEsquerda">
		<td colspan="3">Quantidade de alunos negros</td>
		<td><?php echo $perfil['3156']['1']; ?></td>
		<td><?php echo $perfil['3156']['2']; ?></td>
		<td><?php echo $perfil['3156']['2']; ?></td>
	</tr>	
	<tr class="subtituloEsquerda">
		<td colspan="3">Quantidade de alunos amarelos</td>
		<td><?php echo $perfil['3157']['1']; ?></td>
		<td><?php echo $perfil['3157']['2']; ?></td>
		<td><?php echo $perfil['3157']['2']; ?></td>
	</tr>	
	<tr class="subtituloEsquerda">
		<td colspan="3">Quantidade de alunos indígenas</td>
		<td><?php echo $perfil['3158']['1']; ?></td>
		<td><?php echo $perfil['3158']['2']; ?></td>
		<td><?php echo $perfil['3158']['2']; ?></td>
	</tr>	
	<tr class="subtituloEsquerda">
		<td rowspan="2">Gênero/Sexo</td>
		<td colspan="3">Quantidade de alunos do sexo masculino</td>
		<td><?php echo $perfil['3159']['1']; ?></td>
		<td><?php echo $perfil['3159']['2']; ?></td>
		<td><?php echo $perfil['3159']['2']; ?></td>
	</tr>		
	<tr class="subtituloEsquerda">
		<td colspan="3">Quantidade de alunos do sexo feminino</td>
		<td><?php echo $perfil['3160']['1']; ?></td>
		<td><?php echo $perfil['3160']['2']; ?></td>	
		<td><?php echo $perfil['3160']['2']; ?></td>	
	</tr>
	<tr class="subtituloEsquerda">
		<td rowspan="6">Beneficiários por classe socioeconômica</td>
		<td colspan="3">Beneficiários por mês - Classe A</td>
		<td><?php echo $perfil['3161']['10320']['1']; ?></td>
		<td><?php echo $perfil['3161']['10320']['2']; ?></td>
		<td><?php echo $perfil['3161']['10320']['2']; ?></td>
	</tr>		
	<tr class="subtituloEsquerda">
		<td colspan="3">Beneficiários por mês - Classe B</td>
		<td><?php echo $perfil['3161']['10321']['1']; ?></td>
		<td><?php echo $perfil['3161']['10321']['2']; ?></td>
		<td><?php echo $perfil['3161']['10321']['2']; ?></td>
	</tr>
	<tr class="subtituloEsquerda">
		<td colspan="3">Beneficiários por mês - Classe C</td>
		<td><?php echo $perfil['3161']['10322']['1']; ?></td>
		<td><?php echo $perfil['3161']['10322']['2']; ?></td>
		<td><?php echo $perfil['3161']['10322']['2']; ?></td>	
	</tr>
	<tr class="subtituloEsquerda">
		<td colspan="3">Beneficiários por mês - Classe D</td>
		<td><?php echo $perfil['3161']['10323']['1']; ?></td>
		<td><?php echo $perfil['3161']['10323']['2']; ?></td>
		<td><?php echo $perfil['3161']['10323']['2']; ?></td>
	</tr>
	<tr class="subtituloEsquerda">
		<td colspan="3">Beneficiários por mês - Classe E</td>
		<td><?php echo $perfil['3161']['10324']['1']; ?></td>
		<td><?php echo $perfil['3161']['10324']['2']; ?></td>
		<td><?php echo $perfil['3161']['10324']['2']; ?></td>
	</tr>
	<tr class="subtituloEsquerda">
		<td colspan="3">Total de Beneficiários por mês</td>
		<td><?php echo formata_numero(limpar_numero($perfil['3161']['10320']['1']) + limpar_numero($perfil['3161']['10321']['1']) + limpar_numero($perfil['3161']['10322']['1']) + limpar_numero($perfil['3161']['10323']['1']) + limpar_numero($perfil['3161']['10324']['1'])); ?></td>
		<td><?php echo formata_numero(limpar_numero($perfil['3161']['10320']['2']) + limpar_numero($perfil['3161']['10321']['2']) + limpar_numero($perfil['3161']['10322']['2']) + limpar_numero($perfil['3161']['10323']['2']) + limpar_numero($perfil['3161']['10324']['2'])); ?></td>
		<td><?php echo formata_numero(limpar_numero($perfil['3161']['10320']['2']) + limpar_numero($perfil['3161']['10321']['2']) + limpar_numero($perfil['3161']['10322']['2']) + limpar_numero($perfil['3161']['10323']['2']) + limpar_numero($perfil['3161']['10324']['2'])); ?></td>
	</tr>
	<tr class="subtituloEsquerda">
		<td rowspan="8">Portabilidade de deficiência</td>
		<td colspan="3">Quantidade de alunos com deficiências – Cegueira</td>
		<td><?php echo $perfil['3162']['1']; ?></td>
		<td><?php echo $perfil['3162']['2']; ?></td>
		<td><?php echo $perfil['3162']['2']; ?></td>
	</tr>		
	<tr class="subtituloEsquerda">
		<td colspan="3">Quantidade de alunos com deficiências – Baixa Visão</td>
		<td><?php echo $perfil['3163']['1']; ?></td>
		<td><?php echo $perfil['3163']['2']; ?></td>
		<td><?php echo $perfil['3163']['2']; ?></td>
	</tr>	
	<tr class="subtituloEsquerda">
		<td colspan="3">Quantidade de alunos com deficiências – Surdez</td>
		<td><?php echo $perfil['3164']['1']; ?></td>
		<td><?php echo $perfil['3164']['2']; ?></td>
		<td><?php echo $perfil['3164']['2']; ?></td>
	</tr>	
	<tr class="subtituloEsquerda">
		<td colspan="3">Quantidade de alunos com deficiências – Auditiva</td>
		<td><?php echo $perfil['3165']['1']; ?></td>
		<td><?php echo $perfil['3165']['2']; ?></td>
		<td><?php echo $perfil['3165']['2']; ?></td>
	</tr>
	<tr class="subtituloEsquerda">
		<td colspan="3">Quantidade de alunos com deficiências – Física</td>
		<td><?php echo $perfil['3166']['1']; ?></td>
		<td><?php echo $perfil['3166']['2']; ?></td>	
		<td><?php echo $perfil['3166']['2']; ?></td>	
	</tr>
	<tr class="subtituloEsquerda">
		<td colspan="3">Quantidade de alunos com deficiências – Surdo Cegueira</td>
		<td><?php echo $perfil['3167']['1']; ?></td>
		<td><?php echo $perfil['3167']['2']; ?></td>
		<td><?php echo $perfil['3167']['2']; ?></td>
	</tr>
	<tr class="subtituloEsquerda">
		<td colspan="3">Quantidade de alunos com deficiências – Múltipla</td>
		<td><?php echo $perfil['3168']['1']; ?></td>
		<td><?php echo $perfil['3168']['2']; ?></td>	
		<td><?php echo $perfil['3168']['2']; ?></td>	
	</tr>
	<tr class="subtituloEsquerda">
		<td colspan="3">Quantidade de alunos com deficiências – Mental</td>
		<td><?php echo $perfil['3169']['1']; ?></td>
		<td><?php echo $perfil['3169']['2']; ?></td>
		<td><?php echo $perfil['3169']['2']; ?></td>
	</tr>					
	<tr class="subtituloEsquerda">
		<td>Ações Afirmativas</td>
		<td colspan="3">Percentual de alunos ingressos pelas cotas (%)</td>
		<?php $perfil['3170']['1'] == 0 ? $perfil['3170']['1'] = 0 : $perfil['3170']['1'] = $perfil['3170']['1']/$perfil_perg['3170']['1'];
			  $perfil['3170']['2'] == 0 ? $perfil['3170']['2'] = 0 : $perfil['3170']['2'] = $perfil['3170']['2']/$perfil_perg['3170']['2'];  ?>
		<td><?php echo str_replace(".",",",round($perfil['3170']['1'],2)); ?></td>
		<td><?php echo str_replace(".",",",round($perfil['3170']['2'],2)); ?></td>
		<td><?php echo str_replace(".",",",round((($perfil['3170']['1']+$perfil['3170']['2'])/2),2)); ?></td>	
	</tr>		
	<?php } ?>	
</table>
<?php } ?>