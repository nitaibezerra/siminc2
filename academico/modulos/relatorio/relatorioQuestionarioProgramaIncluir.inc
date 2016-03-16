<?php
if($_SESSION['baselogin'] == 'simec_desenvolvimento' ){	
	define("QUEID_PROGRAMA_INCLUIR", 87);	
} else {
	define("QUEID_PROGRAMA_INCLUIR", 88);
}

if($_POST['requisicao'] == 'filtrar_campus'){
	header( 'Content-type: text/html; charset=iso-8859-1');
	
	$aryWhereI[] = "e.entstatus = 'A' AND ef.funid in (17, 18)";
	if($_POST['instituicao']){
		$aryWhereI[] = "ea.entid = {$_POST['instituicao']}";
	}
		
	$sql = "SELECT 			e.entid as codigo,e.entnome as descricao
			FROM			entidade.entidade e2
			INNER JOIN		entidade.entidade e ON e2.entid = e.entid
			INNER JOIN		entidade.funcaoentidade ef ON ef.entid = e.entid
			INNER JOIN		entidade.funentassoc ea ON ea.fueid = ef.fueid 
			LEFT JOIN		entidade.endereco ed ON ed.entid = e.entid
			LEFT JOIN		territorios.municipio mun ON mun.muncod = ed.muncod
							".(is_array($aryWhereI) ? ' WHERE '.implode(' AND ', $aryWhereI) : '')."
			ORDER BY		e.entnome";
	
	$db->monta_combo('campus',$sql,'S','Selecione...','','','','350','N','','',$campus);
	exit();
}

include APPRAIZ . 'includes/cabecalho.inc';

monta_titulo( 'Relatório de Questionário - Programa Incluir', '&nbsp;' );

$arrQueid = array(QUEID_PROGRAMA_INCLUIR);

function getNumMaxQuestoes($arrQueid){
	global $db;
	$sql = "SELECT COUNT(perid) FROM questionario.pergunta WHERE grpid IN ( SELECT grpid FROM questionario.grupopergunta WHERE queid IN (".QUEID_PROGRAMA_INCLUIR."))";
	$numMax = $db->pegaUm($sql);
	return $numMax;
}
?>

<title>Relatório de Questionário - Programa Incluir</title>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript">
	function filtraCampus(entid){
		$.ajax({
			type: "POST",
			url: "academico.php?modulo=relatorio/relatorioQuestionarioProgramaIncluir&acao=C",
			data: { requisicao: 'filtrar_campus', ies: entid},
			async: false,
			success: function(response){
				$('#td_campus').html(response);
			}
		});
	}

	function abrirPagina(entid,entidcampus,incano){
		window.location.href = 'academico.php?modulo=principal/monitoramentoProgramasAcoes&acao=A&aba=ProgramaIncluir&req=alterar&entid='+entid+'&entidcampus='+entidcampus+'&incano='+incano;
	}
</script>

<form id="formulario" method="post" name="formulario" action="">
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
		<tr>
			<td class="SubTituloDireita" valign="top">Preenchimento:</td>	
			<td>
			<?php
				$arDados = array(
					array('codigo' => 'completo', 'descricao' => 'Completo'),
					array('codigo' => 'incompleto', 'descricao' => 'Incompleto'),
					array('codigo' => 'naoiniciado', 'descricao' => 'Não Iniciado'));
				$preenchimento = $_POST['preenchimento'];					
				$db->monta_combo('preenchimento', $arDados, 'S', 'Selecione...','','','','350','N','',''); ?>		
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top">Instituição:</td>	
			<td>
			<?php
			$sql = "SELECT			e.entid as codigo,
									entnome as descricao 
					FROM			entidade.entidade e 
					INNER JOIN		entidade.funcaoentidade ef ON ef.entid = e.entid
					WHERE 			ef.funid in (12)
					ORDER BY		descricao";
			
			$instituicao = $_POST['instituicao'];	
			$db->monta_combo('instituicao',$sql,'S','Selecione...','filtraCampus','','','350','S','',''); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top">Campus:</td>	
			<td id="td_campus">
			<?php 
			$aryWhereI[] = "e.entstatus = 'A' AND ef.funid in (17, 18)";
			
			if($_POST['instituicao']){
				$aryWhereI[] = "ea.entid = {$_POST['instituicao']}";
			}			
			
			$sql = "SELECT 			e.entid as codigo,e.entnome as descricao
					FROM			entidade.entidade e2
					INNER JOIN		entidade.entidade e ON e2.entid = e.entid
					INNER JOIN		entidade.funcaoentidade ef ON ef.entid = e.entid
					INNER JOIN		entidade.funentassoc ea ON ea.fueid = ef.fueid 
					LEFT JOIN		entidade.endereco ed ON ed.entid = e.entid
					LEFT JOIN		territorios.municipio mun ON mun.muncod = ed.muncod
									".(is_array($aryWhereI) ? ' WHERE '.implode(' AND ', $aryWhereI) : '')."
					ORDER BY		e.entnome";
			
			$campus = $_POST['campus'];
			$db->monta_combo('campus',$sql,'S','Selecione...','','','','350','N','',''); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Exercício&nbsp;</td>
			<td>
			<?php 
				$arDados = array(
					array('codigo'=>''	  ,	'descricao'=>'Selecione...'),
					array('codigo'=>'2010', 'descricao'=>'2010'),
					array('codigo'=>'2011', 'descricao'=>'2011'),
					array('codigo'=>'2012', 'descricao'=>'2012'),
					array('codigo'=>'2013', 'descricao'=>'2013'),
					array('codigo'=>'2014', 'descricao'=>'2014'));
				$incano = $_POST['incano'];
				$db->monta_combo('incano', $arDados, 'S', '', '', '','','200','N','',''); ?>
			</td>
		</tr>
		<tr>
			<td align="center" colspan="2" bgcolor="#DCDCDC">
				<input type="submit" name="pesquisar" value="Pesquisar"/>
				<input type="reset" name="limpar" value="Limpar Pesquisa"/>
			</td>
		</tr>
	</table>
</form>
<br>
<?php 
$arrQueid = array(QUEID_PROGRAMA_INCLUIR);
	
$aryWhere[] = "ef.funid IN (12)";
		
if($_POST['instituicao']){
	$aryWhere[] = "e.entid = {$_POST['instituicao']}";
}		
	
$sqlies = "SELECT 			e.entid, e.entsig, e.entnome 
		   FROM 			entidade.entidade e 
		   INNER JOIN 		entidade.funcaoentidade ef ON ef.entid = e.entid
			     			".(is_array($aryWhere) ? ' WHERE '.implode(' AND ', $aryWhere) : '')."
		   ORDER BY 		e.entnome";
	
$resultIES = $db->carregar($sqlies);
$numMaxQuestoes = getNumMaxQuestoes($arrQueid); ?>
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
	<tr>
		<td colspan="2">
		<?php if($resultIES){ ?>
			<table class="listagem" style="width: 95%;" align="center" cellpadding="2">
			<?php foreach( $resultIES as $dadoIES ){
				$sqlcampus = "SELECT 	CASE WHEN incano IS NOT NULL 											
				   							THEN '<a onclick=\"abrirPagina('|| qry.entid ||','|| qry.entidcampus ||','|| qry.incano ||');\">'|| entnome ||'</a>' 
				   							ELSE entnome END AS nome, 
				   						situacao,
				   						incano
				   			  FROM 		(SELECT 		DISTINCT e.entnome,
														CASE WHEN (COUNT(resdsc)::INTEGER = {$numMaxQuestoes}) THEN 'Completo'
															 WHEN (COUNT(resdsc)::INTEGER > 0) THEN 'Incompleto' 
															 WHEN (COUNT(resdsc)::INTEGER = 0) THEN 'Não Iniciado' END AS situacao, 
														inc.incano,
														inc.entid,
														inc.entidcampus
										  FROM 			entidade.entidade e2
										  INNER JOIN 	entidade.entidade e ON e2.entid = e.entid
										  INNER JOIN 	entidade.funcaoentidade ef ON ef.entid = e.entid
										  INNER JOIN 	entidade.funentassoc ea ON ea.fueid = ef.fueid 
										  LEFT JOIN 	entidade.endereco ed ON ed.entid = e.entid
										  LEFT JOIN 	territorios.municipio mun ON mun.muncod = ed.muncod
										  LEFT JOIN 	academico.incluirquestionario inc ON inc.entidcampus = e.entid
										  LEFT JOIN 	questionario.questionarioresposta as quest on quest.qrpid = inc.qrpid
										  LEFT JOIN 	questionario.resposta as resp on quest.qrpid = resp.qrpid
										  WHERE 		ef.funid in (17, 18) and e.entstatus = 'A'"; 
				
				if($_POST['campus']){ 
					$sqlcampus .= " AND e.entid = {$_POST['campus']}";
				} else{ 
					$sqlcampus .= " AND ea.entid = {$dadoIES['entid']}";
				}	
				
				if( $_POST['incano']){
					$sqlcampus .= " AND inc.incano = {$_POST['incano']}";
				}
							  
				$sqlcampus .=" GROUP BY		inc.entidcampus, e.entnome, inc.incano, inc.entid
							   ORDER BY 		inc.incano, e.entnome) AS qry
							   WHERE 1 = 1";

				if( $_POST['preenchimento'] == 'naoiniciado'){
					$sqlcampus .= " AND qry.situacao = 'Não Iniciado'"; 
				} elseif( $_POST['preenchimento'] == 'completo'){
					$sqlcampus .= " AND  qry.situacao = 'Completo'"; 
				} elseif($_POST['preenchimento'] == 'incompleto'){
					$sqlcampus .=  "AND  qry.situacao = 'Incompleto'"; 
				}				
				
				$resultCampus = $db->carregar($sqlcampus); ?>
				<tr>
					<td>
						<span style="font-size: 10pt; font-weight: bold;"><?php echo ( !$dadoIES['entsig'] ? $dadoIES['entsig'] : $dadoIES['entsig'] . ' - ' ) . $dadoIES['entnome']; ?></span>
						<br /><br />
						<?php 
						$cabecalho = array('Campus', 'Preenchimento','Ano');
						$db->monta_lista($sqlcampus, $cabecalho, '50','10', '', '', '', '');?>
				<?php }	?>
			</table>
		<?php } ?>
		</td>
	</tr>
</table>