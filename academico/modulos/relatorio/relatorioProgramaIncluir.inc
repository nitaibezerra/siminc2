<?php
if ($_REQUEST['requisicao']){
	$_REQUEST['requisicao']();	
	die();
}

if($_SESSION['baselogin'] == 'simec_desenvolvimento' ){	
	define("QUEID_PROGRAMA_INCLUIR", 87);	
} else {
	define("QUEID_PROGRAMA_INCLUIR", 88);
}

function filtraruniversidade(){
	global $db;
	header('Content-type: text/html; charset=ISO-8859-1');
	
	if($_REQUEST['estuf']){
		$aryWhere[] = "estuf = '{$_REQUEST['estuf']}'";	
	}		
	
	$aryWhere[] = "orgid = 1";	
	
	$sql = "SELECT 		unientid AS codigo, unidsc AS descricao 
			FROM 		academico.unicampus 
						".(is_array($aryWhere) ? ' WHERE '.implode(' AND ', $aryWhere) : '')." 
			GROUP BY 	unientid, unidsc 
			ORDER BY 	unidsc";
	
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

monta_titulo( 'Relatório Consolidado Programa Incluir - Rede Federal', '&nbsp;' ); ?>
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
			$aryWhere[] = "orgid = 1";	
			
			$sql = "SELECT		unientid AS codigo, unidsc AS descricao 
					FROM 		academico.unicampus
								".(is_array($aryWhere) ? ' WHERE '.implode(' AND ', $aryWhere) : '')."
					GROUP BY 	unientid, unidsc 
					ORDER BY 	unidsc";
			
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
		$aryWhere[] = "inc.entidcampus = {$_POST['entidade']}";
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
			$sql_label = "SELECT unidsc FROM academico.unicampus WHERE unientid IN ({$entidades}) GROUP BY unidsc";
			$label = $db->carregarColuna($sql_label);
			$label = implode("<br><br>",$label);
			$label_campus = "Todos";
		}
	
		$aryWhere[] = "inc.entid IN ({$entidades})";
	}

	if($_POST['ano']){
		$aryWhere[] = "inc.incano = {$_POST['ano']}";	
	}
	
	$aryWhere[] = "queid = ".QUEID_PROGRAMA_INCLUIR."";	
	$aryWhere[] = "rsp.perid NOT IN (3608)";
	
	$sql_rel = "SELECT 		perid, SUM(resdsc) AS resdsc
				FROM		(SELECT 	  	rsp.perid, NULLIF(replace(replace(rsp.resdsc,'.',''),',','.'),'')::numeric AS resdsc
							 FROM	 	    academico.incluirquestionario inc
							 LEFT JOIN  	questionario.questionarioresposta qrp on qrp.qrpid = inc.qrpid
							 INNER JOIN  	questionario.resposta rsp on rsp.qrpid = qrp.qrpid 
							 ".(is_array($aryWhere) ? ' WHERE '.implode(' AND ', $aryWhere) : '').") AS q 
				GROUP BY     perid
				ORDER BY     perid";	
		
	$dados_rel = $db->carregar($sql_rel);

	$rel = array();
	if(!empty($dados_rel)){
		foreach($dados_rel as $r){
			$rel[$r['perid']] = $r['resdsc']; 
		}
	}

	$sql_perg = "SELECT 		g.grptitulo, g.grpid, p.perid, p.pertitulo 
				 FROM 			questionario.grupopergunta g
				 INNER JOIN 	questionario.pergunta p ON g.grpid = p.grpid
				 WHERE 			g.queid = ".QUEID_PROGRAMA_INCLUIR." AND p.perid NOT IN (3608)
				 ORDER BY 		g.grpid, p.perordem";
	
	$perguntas = $db->carregar($sql_perg); ?>

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
	
	<?php if(empty($dados_rel)){ ?>
		<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="2" cellpadding="3" border="0">
			<tr>
				<td align="center" bgcolor="#CCCCCC"><font color="#FF0000"><b>Nenhum registro encontrado!</b></font></td>
			</tr>	
		</table>
	<?php } else { ?>
		<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="2" cellpadding="3" border="0">
			<tr>
				<td align="center" bgcolor="#CCCCCC" width="8%"><b>UNIVERSIDADE</b></td>
				<td align="center" bgcolor="#CCCCCC" width="8%"><b>CAMPUS</b></td>
				<td align="center" bgcolor="#CCCCCC" rowspan="2"><b>RELATÓRIO CONSOLIDADO</b></td>
				<td align="center" bgcolor="#CCCCCC" rowspan="2" width="8%"><b>Total</b></td>
			</tr>
			<tr class="subtituloEsquerda">
				<td rowspan="35" valign="top"><br><br><b><?php echo $aryLabel['unidsc']; ?><?php echo $label; ?></b></td>
				<td rowspan="35" valign="top"><br><br><b><?php echo $aryLabel['cmpdsc']; ?><?php echo $label_campus; ?></b></td>	
			</tr>		
			<?php $grpid = ""; ?>
			<?php foreach($perguntas AS $p){ ?>
				<?php if($p['grpid'] != $grpid){ ?>
					<tr bgcolor="#CCCCCC">
						<td colspan="2"><b><?php echo $p['grptitulo']; ?></b></td>
					</tr>
				<?php } ?>
				<tr class="subtituloEsquerda">
					<td><?php echo $p['pertitulo']; ?></td>
					<td>
						<?php foreach($dados_rel AS $r){ 
							if($p['perid'] == $r['perid']){	
								echo $r['resdsc']; 
							} 
						} ?>
					</td>
				</tr>
				<?php $grpid = $p['grpid']; ?>
			<?php } ?>
		</table>
	<?php } ?>
<?php } ?>