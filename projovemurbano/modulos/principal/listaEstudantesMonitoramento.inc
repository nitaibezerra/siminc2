	<?

checkAno();
if( $_REQUEST['req'] ){
	$_REQUEST['req']($_REQUEST);
	die();
}

header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

include  APPRAIZ . 'includes/cabecalho.inc';
include_once APPRAIZ.'projovemurbano/modulos/principal/filtroAnoExercicio.inc';
echo '<br />';
monta_titulo('Projovem Urbano', 'Lista de estudantes. Selecione os filtros abaixo.');

$bloq = 'S';
$disabled = '';


if($_REQUEST['caeid']) {
	
	$requisicao = "atualizarEstudantes";
	
	$sql = "SELECT * FROM projovemurbano.cadastroestudante WHERE caeid='".$_REQUEST['caeid']."'";
	$cadastroestudante = $db->pegaLinha($sql);
	extract($cadastroestudante);
	$caedatanasc = formata_data($caedatanasc);
	$caedataemissaorg = formata_data($caedataemissaorg);
	$caedataemissaorg = formata_data($caedataemissaorg);
	$caecep = mascaraglobal($caecep,"#####-###");
	$caecpf = mascaraglobal($caecpf,"###.###.###-##");
	if( trim($caenispispasep) != '' ){
		$bloq = 'N';
		$disabled = 'disabled="disabled"';
	}
	
} else {
	$requisicao = "inserirEstudantes";
}

?>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script type="text/javascript" src="./js/projovemurbano.js"></script>
<script language="javascript" type="text/javascript" src="../includes/webservice/cpf.js" /></script>
<script language="javascript" type="text/javascript">

jQuery(document).ready(function()
{
	jQuery('[name="caehistorico"]').click(function(){
		if(jQuery(this).val()=='TRUE'){
			jQuery('[name="caetestepro"][value="FALSE"]').attr('checked',true);
		}else{
			jQuery('[name="caetestepro"][value="TRUE"]').attr('checked',true);
		}
	});
	jQuery('[name="caetestepro"]').click(function(){
		if(jQuery(this).val()=='TRUE'){
			jQuery('[name="caehistorico"][value="FALSE"]').attr('checked',true);
		}else{
			jQuery('[name="caehistorico"][value="TRUE"]').attr('checked',true);
		}
	});
	jQuery('[name="nomesocial"]').click(function(){
		if( jQuery(this).val() == 'S' ){
			jQuery('#tr_nomesocial').show();
		}else{
			jQuery('#tr_nomesocial').hide();
			jQuery('#caenomesocial').val('');
		}
	});
	jQuery('.excluir').click(function(){
		  var caeid = jQuery(this).attr('id');
	      jQuery('#caeid').val(caeid);
	      jQuery('#req').val('excluirEstudante');
	      jQuery('#form_busca').submit();
	});
});

function carregarMunicipios2(estuf) {
	var pjuesfera = jQuery('#pjuesfera').val();
	estuf = jQuery('#estuf').val();
	if( pjuesfera == 'M' && estuf != '' ){
		jQuery('#tr_muncod').show();
		jQuery('#td_muncod').html('Carregando...');
		jQuery.ajax({
	   		type: "POST",
	   		url: window.location,
	   		data: "req=carregarMunicipios2&estuf="+estuf,
	   		async: false,
	   		success: function(msg){jQuery('#td_muncod').html(msg);}
	 	});
	}else if( pjuesfera == 'E'){
		jQuery('#tr_muncod').hide();
		jQuery('#td_muncod').html('');
		if( estuf != '' ){
			carregarPolo(estuf);
		}
	}else if( estuf != '' ){
		alert('Escolha uma esfera.');
	}
}

function carregarPolo(cod) {
	var pjuesfera = jQuery('#pjuesfera').val();
	if( pjuesfera == 'M' ){ cod = '&muncod='+jQuery('#muncod').val() }else{ cod = '&estuf='+jQuery('#estuf').val() }
	jQuery.ajax({
   		type: "POST",
   		url: window.location,
   		data: "req=testaPolo"+cod,
   		async: false,
   		success: function(msg){
   			if( msg == 'S' ){
   				jQuery.ajax({
   			   		type: "POST",
   			   		url: window.location,
   			   		data: "req=carregarPolo"+cod,
   			   		async: false,
   			   		success: function(msg){
   			   			jQuery('#td_polo').html(msg);
   			   			jQuery('#tr_polo').show();
   			   			jQuery('#td_nucleo').html('<select id="endestuf" disabled="disabled" style="width: auto" class="CampoEstilo" name="endestuf_disable"><option value="">Selecione</option></select>');
   			   		}
   			 	});
   	   		}else{
		   		jQuery('#tr_polo').hide();
			   	jQuery('#td_polo').html('');
				jQuery('#td_nucleo').html('Carregando...');
				jQuery.ajax({
			   		type: "POST",
			   		url: window.location,
			   		data: "req=buscarNucleosSemId"+cod+"&bloq=<?=$bloq ?>",
			   		async: false,
			   		success: function(msg){
				   		jQuery('#td_nucleo').html(msg);
				   	}
			 	});
   	   	   	}
		}
 	});
}

function buscarNucleos(polid, bloq) {
	if( polid != '' ){
		jQuery('td_nucleo').html('Carregando...');
		jQuery.ajax({
	   		type: "POST",
	   		url: window.location,
	   		data: "req=buscarNucleos&polid="+polid+"&bloq=<?=$bloq ?>",
	   		async: false,
	   		success: function(msg){document.getElementById('td_nucleo').innerHTML = msg;}
	 	});
	}
}

function buscarTurmas(nucid, bloq) {
	if( nucid != '' ){
		document.getElementById('td_turma').innerHTML = 'Carregando...';
		jQuery.ajax({
	   		type: "POST",
	   		url: window.location,
	   		data: "req=buscarTurmas&nucid="+nucid+"&bloq=<?=$bloq ?>",
	   		async: false,
	   		success: function(msg){document.getElementById('td_turma').innerHTML = msg;}
	 		});
	}
}
</script>

<form id="form_busca" name="form_busca" method="POST">
	<input type="hidden" id="req" name="req" value="">
	<input type="hidden" value="" name="caeid" id="caeid" />
	<input type="hidden" name="estuf" value="<?=$_SESSION['projovemurbano']['estuf'] ?>" />
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td class="SubTituloCentro" colspan="2" width="30%">
				<b>Filtros de pesquisa</b>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Esfera</td>
			<td>
				<? 
				$sql = "SELECT DISTINCT 
							CASE 
								WHEN estuf IS NULL  THEN 'M' 
								WHEN muncod IS NULL THEN 'E' 
							END as codigo, 
							CASE 
								WHEN estuf IS NULL  THEN 'Municipal' 
								WHEN muncod IS NULL THEN 'Estadual' 
							END as descricao
						FROM 
							projovemurbano.projovemurbano";
				$db->monta_combo('pjuesfera', $sql, $bloq, 'Selecione', 'carregarMunicipios2', '', '', '', 'S', 'pjuesfera');
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">UF</td>
			<td>
				<? 
				$sql = "SELECT estuf as codigo, estuf as descricao FROM territorios.estado ORDER BY estuf";
				$db->monta_combo('estuf', $sql, $bloq, 'Selecione', 'carregarMunicipios2', '', '', '', 'S', 'estuf');
				?>
			</td>
		</tr>
		<tr id="tr_muncod">
			<td class="SubTituloDireita">Munícipio</td>
			<td id="td_muncod">
				<? $db->monta_combo('endmuncod', array(), $bloq, 'Selecione', '', '', '', '', 'S', 'endmuncod'); ?>
			</td>
		</tr>
		<tr id="tr_polo" style="display:none">
			<td class="SubTituloDireita"><b>Polo</b></td>
			<td id="td_polo">
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita"><b>Núcleo / Escola</b></td>
			<td id="td_nucleo"><? $db->monta_combo('nucid', array(), 'N', 'Selecione', '', '', '', '', 'N', 'nucid'); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita"><b>Turma</b></td>
			<td id="td_turma"><? $db->monta_combo('endestuf', array(), 'N', 'Selecione', '', '', '', '', 'N', 'endestuf'); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Matricula:</td>
			<td><? echo campo_texto('matricula', "N", "S", "Matricula", 12, 8, "########", "", '', '', 0, 'id="matricula"' ); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">CPF:</td>
			<?php $caecpf = $_POST['caecpf']?>
			<td><? echo campo_texto('caecpf', "N", "S", "CPF", 16, 14, "###.###.###-##", "", '', '', 0, 'id="caecpf"', '', '', '' ); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Nome:</td>
			<td><? echo campo_texto('caenome', "N", "S", "Nome", 50, 255, "", "", '', '', 0, 'id="caenome"' ); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Status:</td>
			<td>
				<input type="radio" name="status" value="A" <?=($_POST['status']=='A'?'checked':'') ?>/> Ativo
				<input type="radio" name="status" value="I" <?=($_POST['status']=='I'?'checked':'') ?>/> Inativo - outros
				<input type="radio" name="status" value="D" <?=($_POST['status']=='D'?'checked':'') ?>/> Inativo - desistência
				<input type="radio" name="status" value=" " <?=($_POST['status']==' '?'checked':'') ?>/> Todos
			</td>
		</tr>
		<tr>
			<td class="SubTituloCentro" colspan="2">
				<input type="button" name="pesquisar" value="Pesquisar" onclick="document.getElementById('form_busca').submit();">
			</td>
		</tr>
	</table>
</form>

<?
$where = Array();

if( $_POST['pjuesfera'] == 'E' ){
	if( $_POST['estuf'] != '' ){
		$where[] = "(pju.estuf = '".$_POST['estuf']."')";
	}else{
		$where[] = "(pju.estuf IS NOT NULL)";
	}
}
if( $_POST['pjuesfera'] == 'M' ){
	if( $_POST['muncod'] != '' ){
		$where[] = "(pju.muncod = '".$_POST['muncod']."')";
	}else{
		$where[] = "(pju.muncod IS NOT NULL)";
	}
}
if( $_POST['status'] != '' && $_POST['status'] != ' ' ){
	$where[] = "cae.caestatus = '".$_POST['status']."'";
}
if( $_POST['justificativaD'] ){
	$where[] = "cae.caejustificativainativacao = 'DESISTENTE'";
}
if( $_POST['justificativaF'] ){
	$where[] = "cae.caejustificativainativacao = 'FREQUENCIA INSUFICIENTE'";
}
if( $_POST['justificativaO'] ){
	$where[] = "cae.caejustificativainativacao NOT IN ('DESISTENTE','FREQUENCIA INSUFICIENTE')";
}
if( $_POST['polid'] ){
	$where[] = "cae.polid = ".$_POST['polid'];
}
if( $_POST['nucid'] || $_POST['nucid_disable'] ){
	$_POST['nucid'] = $_POST['nucid'] ? $_POST['nucid'] : $_POST['nucid_disable'];
	$where[] = "cae.nucid = ".$_POST['nucid'];
}
if( $_POST['turid'] ){
	$where[] = "cae.turid = ".$_POST['turid'];
}
if( $_POST['matricula'] ){
	$where[] = "caeano||lpad(cae.caeid::varchar,6,'0') ilike '%".$_POST['matricula']."%'";
}
if( $_POST['caenome'] ){
	$where[] = "UPPER(cae.caenome) ilike '%'||UPPER('".$_POST['caenome']."')||'%'";
}
if( $_POST['caecpf'] ){
	$where[] = "cae.caecpf = '".str_replace(Array('.','-'),'',$_POST['caecpf'])."'";
}
$INNER_NUCLEO = "LEFT  JOIN projovemurbano.nucleo         nuc ON nuc.nucid  = tur.nucid";
if($_SESSION['projovemurbano']['ppuano'] == 2013){
		$ano = 2013;
		$tprid = "AND  nuc.tprid IN (3,1,2)";
	}
if($_SESSION['projovemurbano']['ppuano'] == 2014){
	$ano = 2014;
	$tprid = "AND  nuc.tprid IN (3,1,2)";
	$INNER_NUCLEO = "LEFT  JOIN projovemurbano.nucleo         nuc ON nuc.nucid  = cae.nucid";
}
$acoes = "<img src=../imagens/alterar.gif title=\"Editar\" border=0 style=cursor:pointer; onclick=\"window.location=\'projovemurbano.php?modulo=principal/monitoramento$ano&acao=A&aba=cadastroEstudantes&caeid='||cae.caeid||'&pjuid='||cae.pjuid||'\';\"> 
          <img src=\"../imagens/excluir.gif\" title=\"Excluir\" style=\"cursor:pointer;\" name=\"caeid\" value=\"'|| cae.caeid ||'\" id='|| cae.caeid ||' class=excluir / >  ";
if(!$db->testa_superuser()) {
	
        $perfis = pegaPerfilGeral();
	
	
	if(in_array(PFL_SECRETARIO_MUNICIPAL, $perfis)) {
		$inner_municipio = "INNER JOIN projovemurbano.usuarioresponsabilidade ur ON ur.usucpf='".$_SESSION['usucpf']."' AND (ur.muncod=ende.muncod) AND rpustatus='A'";
		$acoes = '';
	}

	if(in_array(PFL_COORDENADOR_MUNICIPAL, $perfis)) {
		$inner_municipio = "INNER JOIN projovemurbano.coordenadorresponsavel cr ON pju.pjuid=cr.pjuid AND cr.corcpf='".$_SESSION['usucpf']."'";
		$acoes = '';
	}
	
	if(in_array(PFL_SECRETARIO_ESTADUAL, $perfis)) {
		$inner_estado = "INNER JOIN projovemurbano.usuarioresponsabilidade ur ON ur.usucpf='".$_SESSION['usucpf']."' AND (ur.estuf=ende.estuf)";
		$acoes = '';
	}
	
	if(in_array(PFL_COORDENADOR_ESTADUAL, $perfis)) {
		$inner_estado = "INNER JOIN projovemurbano.coordenadorresponsavel cr ON pju.pjuid=cr.pjuid AND cr.corcpf='".$_SESSION['usucpf']."'";
		$acoes = '';
	}
	
	if(in_array(PFL_DIRETOR_POLO, $perfis)) {
		$inner_estado = "INNER JOIN projovemurbano.usuarioresponsabilidade ur ON ur.usucpf='".$_SESSION['usucpf']."' AND (ur.polid=cae.polid)";
		$acoes = '';
	}
	
	if(in_array(PFL_DIRETOR_NUCLEO, $perfis)) {
		$inner_estado = "INNER JOIN projovemurbano.usuarioresponsabilidade ur ON ur.usucpf='".$_SESSION['usucpf']."' AND (ur.entid=nes.entid)";
		$acoes = '';
	}
	
	if(in_array(PFL_CONSULTA, $perfis)) {
		$acoes = '';
	}
}

//$acoes2 = "";

$sql = "SELECT DISTINCT
			'$acoes
			<img src=../imagens/report.gif border=0 width=\"13px\"  title=\"Imprimir Comprovante de Inscrição\" style=cursor:pointer; onclick=\"window.open(\'projovemurbano.php?modulo=principal/popComprovante&acao=A&caeid='||cae.caeid||'&pjuid='||cae.pjuid||'\',\'Comprovante\',\'width=480,height=265,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1\');\">
			<img src=../imagens/lista_verde.gif border=0 width=\"13px\"  title=\"Imprimir Ficha de Matricula\" style=cursor:pointer; onclick=\"window.open(\'projovemurbano.php?modulo=principal/popFichaMatricula$ano&acao=A&caeid='||cae.caeid||'&pjuid='||cae.pjuid||'\',\'Comprovante\',\'width=800,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1\');\">' as acao,
			'<center><img src=../imagens/'||
			CASE WHEN caestatus = 'A'
				THEN '0_ativo.png title=\"Aluno Ativo\"'
				ELSE '0_inativo.png title=\"Aluno Inativo\"'
			END ||' border=0 width=\"13px\" style=cursor:pointer;/></center>' as status,
			caeano||lpad(cae.caeid::varchar,6,'0') as ninscricao,   
			caecpf, 
			caenome, 
			caeemail, 
			'NÚCLEO '||nuc.nucid||
			CASE WHEN nuetipo = 'S' THEN ', SEDE: ' ELSE ', ANEXO:' END||ent.entnome as entnome,
			turdesc 
		FROM 
			projovemurbano.cadastroestudante cae
		LEFT  JOIN projovemurbano.turma          tur ON tur.turid  = cae.turid
		$INNER_NUCLEO 
		LEFT JOIN projovemurbano.tipoprograma 	 tpr ON nuc.tprid  = tpr.tprid 
		LEFT  JOIN projovemurbano.nucleoescola   nes ON nes.nucid  = nuc.nucid AND nes.entid = tur.entid
		LEFT  JOIN entidade.entidade 	         ent ON ent.entid  = tur.entid
		INNER JOIN projovemurbano.projovemurbano pju ON pju.pjuid  = cae.pjuid
		LEFT  JOIN entidade.entidadeendereco     etd ON etd.entid  = ent.entid
		LEFT  JOIN entidade.endereco 	        ende ON ende.endid = etd.endid
		{$inner_polo_filtro}
		$inner_estado
		$inner_municipio
		WHERE
			1=1
		AND cae.ppuid = {$_SESSION['projovemurbano']['ppuid']}
		$tprid
			".(($filtro_nucleo)?" AND nuc.nucid IN('".implode("','",$filtro_nucleo)."')":"")
			.(($where)?" AND ".implode(" AND ",$where) :"");
// ver($sql);
echo "<form name=form_excluir id=form_excluir method=POST>";
$cabecalho = array("","Status","Matrícula","CPF","Nome","E-mail","Núcleo","Turma");
echo "</form>";
$db->monta_lista($sql, $cabecalho, 100, 5, 'N', 'center', $par2);
?>