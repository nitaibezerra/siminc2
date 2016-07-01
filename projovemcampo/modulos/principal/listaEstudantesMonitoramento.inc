	<?
if( $_REQUEST['req'] ){
	$_REQUEST['req']($_REQUEST);
	die();
}

header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

include  APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';
monta_titulo('Projovem Campo', 'Lista de estudantes. Selecione os filtros abaixo.');

$bloq = 'S';
$disabled = '';


if($_REQUEST['estid']) {
	
	$requisicao = "atualizarEstudantes";
	
	$sql = "SELECT * FROM projovemcampo.estudante WHERE estid='".$_REQUEST['estid']."'";
	$cadastroestudante = $db->pegaLinha($sql);
	extract($estudante);
    $estcpf_sem_masc = $estcpf;
  	$estdatanascimento = formata_data($estdatanascimento);
  	$estdataemissaorg = formata_data($estdataemissaorg);
  //$estdataemissaorg = formata_data($estdataemissaorg);
 	 $estendcep = mascaraglobal($estendcep, "#####-###");
 	 $estcpf = mascaraglobal($estcpf, "###.###.###-##");
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
<script type="text/javascript" src="./js/projovemcampo.js"></script>
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
			jQuery('#estnomesocial').val('');
		}
	});
});

function carregarMunicipios2(estuf) {
	var apcesfera = jQuery('#apcesfera').val();
	estuf = jQuery('#estuf').val();
	if( apcesfera == 'M' && estuf != '' ){
		jQuery('#tr_muncod').show();
		jQuery('#td_muncod').html('Carregando...');
	  	jQuery.ajax({
		    type: "POST",
		    url: "projovemcampo.php?modulo=principal/listaEstudantesMonitoramento&acao=A",
		    data: "req=carregarMunicipios2&estuf=" + estuf,
		    async: false,
		    success: function(msg){document.getElementById('td_muncod').innerHTML = msg;}
	  });
	}else if( apcesfera == 'E'){
		jQuery('#tr_muncod').hide();
		jQuery('#td_muncod').html('');
		if( estuf != '' ){
			pegarEscolas2(estuf);
		}
	}else if( estuf != '' ){
		alert('Escolha uma esfera.');
	}
}


function pegarEscolas2(cod) {
		var apcesfera = jQuery('#apcesfera').val();	
		estuf = jQuery('#estuf').val();	
		muncod = jQuery('#muncod').val();	
		
		jQuery('td_escola').html('Carregando...');
		
		if(apcesfera!=''){
			if(estuf!=''&& muncod== ''){
				cod = estuf;
				jQuery.ajax({
			   		type: "POST",
			   		url: window.location,
			   		data: "req=pegarEscolas2&estuf="+estuf+"&bloq=<?=$bloq ?>",
			   		async: false,
			   		success: function(msg){document.getElementById('td_escola').innerHTML = msg;}
			 	});
			}else{
				jQuery.ajax({
			   		type: "POST",
			   		url: window.location,
			   		data: "req=pegarEscolas2&muncod="+muncod+"&bloq=<?=$bloq ?>",
			   		async: false,
			   		success: function(msg){document.getElementById('td_escola').innerHTML = msg;}
			 	});
			}
		}
}

function buscarTurmas(entid) {
	 if (entid != '') {	
	    document.getElementById('td_turma').innerHTML = 'Carregando...';
	    jQuery.ajax({
	      type: "POST",
	      url: window.location,
	      data: "req=buscarTurmas&entid="+entid+"&bloq=<?=$bloq ?>",
	      async: false,
	      success: function(msg){document.getElementById('td_turma').innerHTML = msg;}
	    });
	 }
}

</script>

<form id="form_busca" name="form_busca" method="POST">
	<input type="hidden" name="req" value="">
	<input type="hidden" name="estuf" value="<?=$_SESSION['projovemcampo']['estuf'] ?>" />
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
								WHEN apcesfera = 'M'  THEN 'M' 
								WHEN apcesfera = 'E' THEN 'E' 
							END as codigo, 
							CASE 
								WHEN apcesfera = 'M'  THEN 'Municipal' 
								WHEN apcesfera = 'E' THEN 'Estadual' 
							END as descricao
						FROM 
							projovemcampo.adesaoprojovemcampo";
				$db->monta_combo('apcesfera', $sql, $bloq, 'Selecione', 'carregarMunicipios2', '', '', '', 'S', 'apcesfera');
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
				<? $db->monta_combo('muncod', array(), $bloq, 'Selecione', '', '', '', '', 'S', 'muncod'); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita"><b>Escola</b></td>
			<td id="td_escola">
				<? $db->monta_combo('entid', array(), $bloq, 'Selecione', '', '', '', '', 'S', 'entid'); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita"><b>Turma</b></td>
			<td id="td_turma"><? $db->monta_combo('turid', array(), 'N', 'Selecione', '', '', '', '', 'N', 'turid'); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Matricula:</td>
			<td><? echo campo_texto('matricula', "N", "S", "Matricula", 12, 8, "########", "", '', '', 0, 'id="matricula"' ); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">CPF:</td>
			<?php $estcpf = $_POST['estcpf']?>
			<td><? echo campo_texto('estcpf', "N", "S", "CPF", 16, 14, "###.###.###-##", "", '', '', 0, 'id="estcpf"', '', '', '' ); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Nome:</td>
			<td><? echo campo_texto('estnome', "N", "S", "Nome", 50, 255, "", "", '', '', 0, 'id="estnome"' ); ?></td>
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

if( $_POST['apcesfera'] == 'E' ){
	if( $_POST['estuf'] != '' ){
		$where[] = "(ende.estuf = '".$_POST['estuf']."')";
		$where[] = "(ende.muncod IS NULL)";
	}else{
		$where[] = "(ende.estuf IS NOT NULL)";
	}
}
if( $_POST['apcesfera'] == 'M' ){
	if( $_POST['muncod'] != '' ){
		$where[] = "(ende.muncod = '".$_POST['muncod']."')";
	}else{
		$where[] = "(ende.muncod IS NOT NULL)";
	}
}
if( $_POST['status'] != '' && $_POST['status'] != ' ' ){
	$where[] = "est.eststatus = '".$_POST['status']."'";
}
if( $_POST['justificativaD'] ){
	$where[] = "est.estmotivoinativacao = 'DESISTENTE'";
}
if( $_POST['justificativaF'] ){
	$where[] = "est.estmotivoinativacao = 'FREQUENCIA INSUFICIENTE'";
}
if( $_POST['justificativaO'] ){
	$where[] = "est.estmotivoinativacao NOT IN ('DESISTENTE','FREQUENCIA INSUFICIENTE')";
}
if( $_POST['turid'] ){
	$where[] = "est.turid = ".$_POST['turid'];
}
if( $_POST['matricula'] ){
	$where[] = "lpad(est.estid::varchar,6,'0') ilike '%".$_POST['matricula']."%'";
}
if( $_POST['estnome'] ){
	$where[] = "UPPER(est.estnome) ilike '%'||UPPER('".$_POST['estnome']."')||'%'";
}
if( $_POST['estcpf'] ){
	$where[] = "est.estcpf = '".str_replace(Array('.','-'),'',$_POST['estcpf'])."'";
}

	$acoes = "'<img src=../imagens/alterar.gif title=\"Editar\" border=0 style=cursor:pointer; onclick=\"window.location=\'projovemcampo.php?modulo=principal/monitoramento&acao=A&aba=cadastroEstudantes&estid='||est.estid||'&apcid='||apc.apcid||'\';\"> 
			   <img src=../imagens/report.gif border=0 width=\"13px\"  title=\"Imprimir Comprovante de Inscrição\" style=cursor:pointer; onclick=\"window.open(\'projovemcampo.php?modulo=principal/popComprovante&acao=A&estid='||est.estid||'\',\'Comprovante\',\'width=480,height=265,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1\');\">
			   <img src=../imagens/lista_verde.gif border=0 width=\"13px\"  title=\"Imprimir Ficha de Matricula\" style=cursor:pointer; onclick=\"window.open(\'projovemcampo.php?modulo=principal/popFichaMatricula&acao=A&estid='||est.estid||'\',\'Comprovante\',\'width=800,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1\');\">' as acao,
			";
if(!$db->testa_superuser()) {
	
        $perfis = pegaPerfilGeral();
        $acoes = "'<img src=../imagens/alterar.gif title=\"Editar\" border=0 style=cursor:pointer; onclick=\"window.location=\'projovemcampo.php?modulo=principal/monitoramento&acao=A&aba=cadastroEstudantes&estid='||est.estid||'&apcid='||apc.apcid||'\';\"> ' as acao,";
	if(in_array(PFL_SECRETARIO_MUNICIPAL, $perfis)) {
		$inner_municipio = "INNER JOIN projovemcampo.usuarioresponsabilidade ur ON ur.usucpf='".$_SESSION['usucpf']."' AND (ur.muncod=ende.muncod) AND rpustatus='A'";
	}

	if(in_array(PFL_COORDENADOR_MUNICIPAL, $perfis)) {
		$inner_municipio = "INNER JOIN projovemcampo.secretaria sec ON sec.secaid=apc.secaid AND sec.secoordcpf='".$_SESSION['usucpf']."'";
	}
	
	if(in_array(PFL_SECRETARIO_ESTADUAL, $perfis)) {
		$inner_estado = "INNER JOIN projovemcampo.usuarioresponsabilidade ur ON ur.usucpf='".$_SESSION['usucpf']."' AND (ur.estuf=ende.estuf)";
	}
	
	if(in_array(PFL_COORDENADOR_ESTADUAL, $perfis)) {
		$inner_estado = "INNER JOIN projovemcampo.secretaria sec ON sec.secaid=apc.secaid AND sec.secoordcpf='".$_SESSION['usucpf']."'";
	}
	
	if(in_array(PFL_DIRETOR_NUCLEO, $perfis)) {
		$inner_estado = "INNER JOIN projovemcampo.usuarioresponsabilidade ur ON ur.usucpf='".$_SESSION['usucpf']."' AND (ur.entid=nes.entid)";
	}
	if(in_array(PFL_COORDENADOR_TURMA, $perfis)) {
		$inner_estado = "INNER JOIN projovemcampo.usuarioresponsabilidade ur ON ur.usucpf='".$_SESSION['usucpf']."' AND (tur.turid=ur.turid)";
	}
	
}

//$acoes2 = "";
$sql = "SELECT DISTINCT
			$acoes
			'<center><img src=../imagens/'||
			CASE WHEN eststatus = 'A'
				THEN '0_ativo.png title=\"Aluno Ativo\"'
				ELSE '0_inativo.png title=\"Aluno Inativo\"'
			END ||' border=0 width=\"13px\" style=cursor:pointer;/></center>' as status,
			2014||lpad(estid::varchar,6,'0')  as Matricula,
			estcpf, 
			estnome, 
			estemail,
			ent.entnome, 
			turdescricao 
		FROM 
			projovemcampo.estudante est
		INNER  JOIN projovemcampo.turma          tur ON tur.turid  = est.turid
		INNER  JOIN entidade.entidade 	         ent ON ent.entid  = tur.entid
		INNER JOIN projovemcampo.adesaoprojovemcampo apc ON apc.secaid  = tur.secaid
		left  JOIN entidade.entidadeendereco     etd ON etd.entid  = ent.entid
		left  JOIN entidade.endereco 	        ende ON ende.endid = etd.endid
		$inner_municipio
		$inner_estado
		WHERE
			1=1
			".(($where)?" AND ".implode(" AND ",$where) :"")."
		ORDER BY estnome";
// ver($sql,d);
$cabecalho = array("","Status","Matrícula","CPF","Nome","E-mail","Escola","Turma");
$db->monta_lista($sql,$cabecalho,100,5,'N','center',$par2);
?>