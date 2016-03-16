<?php
if ($_SESSION['baselogin'] == 'simec_desenvolvimento'){
	define("QUEID_ACADEMICO_INDICADORES", 		76);
	define("QUEID_ACADEMICO_INFRAESTRUTURA", 	77);
	define("QUEID_ACADEMICO_SOCIOECONOMICO", 	81);
}else{
	define("QUEID_ACADEMICO_INDICADORES", 		81);
	define("QUEID_ACADEMICO_INFRAESTRUTURA", 	82);
	define("QUEID_ACADEMICO_SOCIOECONOMICO", 	83);
}

$arrQueid = array( QUEID_ACADEMICO_INDICADORES, QUEID_ACADEMICO_INFRAESTRUTURA, QUEID_ACADEMICO_SOCIOECONOMICO);

if( $_POST['listaCampus'] != '' ){
	
	header( 'Content-type: text/html; charset=iso-8859-1' );
	$sql = "SELECT 
				e.entid as codigo,e.entnome as descricao
			FROM
				entidade.entidade e2
			INNER JOIN
				entidade.entidade e ON e2.entid = e.entid
			INNER JOIN
				entidade.funcaoentidade ef ON ef.entid = e.entid
			INNER JOIN
				entidade.funentassoc ea ON ea.fueid = ef.fueid 
			LEFT JOIN
				entidade.endereco ed ON ed.entid = e.entid
			LEFT JOIN
				territorios.municipio mun ON mun.muncod = ed.muncod
			WHERE
				ea.entid = ".$_REQUEST['entid']." AND
				e.entstatus = 'A' AND ef.funid in (17, 18)
			ORDER BY
				e.entnome";

	$res = $db->carregar( $sql );
					
	?>
	<select name="campus" class="CampoEstilo" id="campus" >  
	<?php
	
	if( $res ){
		echo "<option value=\"\"> Selecione.. </option>";	
		foreach( $res as $ar ){
			echo "<option value=\"".$ar['codigo']."\"> ".$ar['descricao']." </option>";						
		}					
	}else{
		die("");
	}
	?>	
	</select>
	<?php
	die();
}

function getNumMaxQuestoes( $arrQueid ){
	
	global $db;
	$sql = "
		select count(perid) from questionario.pergunta where grpid in ( select grpid from questionario.grupopergunta where queid IN (76,77,81) )
	";
	$numMax = $db->pegaUm( $sql );
	return $numMax;
}

function getNumQuestoesRespondidas( $qrpid ){

	global $db;
	$sql = "select count(resid) from questionario.resposta  WHERE qrpid = ". $qrpid;
	
	$numMax = $db->pegaUm( $sql );
	return $numMax;
}

function pegaQrpidEntidade( $arrQueid, $entid, $ano, $semestre )
{
	global $db;
    global $docid;
    //include_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";
    
    $sql = "SELECT
            	it.qrpid
            FROM
            	academico.assistenciaestudantil aet
            INNER JOIN academico.itemassistencia it on it.aetdid = aet.aetdid
			INNER JOIN 
            	questionario.questionarioresposta qr ON qr.qrpid = it.qrpid
            WHERE
            	aet.entid = {$entid} 
            AND 
            	aetano = {$ano}
            AND 
            	aetsemestre = {$semestre}
            AND 
            	qr.queid IN (".implode(",", $arrQueid ).") ";
    
    $registro = $db->pegaLinha( $sql );
    
    if( $registro ){

    	return $registro['qrpid'];
    }
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

monta_titulo( 'Relatório de Questionário', '&nbsp;' );
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
	<head>
		<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
		<title>Relatório</title>
		<script src="../includes/calendario.js"></script>
		<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
		<script type="text/javascript">
		
		function filtraCampus( entid ){
			
			$.ajax({
					type: "POST",
					url: "academico.php?modulo=relatorio/relatorioQuestionario&acao=C",
					data: "&listaCampus=1&entid="+entid,
					async: false,
					success: function(response){
						$('#td_campus').html(response);
					}
				});
		}
		
		$(document).ready(function(){
			$('.pesquisar').click(function(){
				
				document.formulario.submit();
			});
			
			$('.limpar').click(function(){
				window.location = window.location;
			});
		});
		
		</script>
	</head>
	<body>
		<form name="formulario" id="formulario" action="" method="post">
			<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
				<tr>
					<td class="SubTituloDireita" valign="top">Preenchimento:</td>	
					<td >
					<?php
						$arDados = array(
							array('codigo' => 'completo', 'descricao' => 'Completo'),
							array('codigo' => 'incompleto', 'descricao' => 'Incompleto'),
							array('codigo' => 'naoiniciado', 'descricao' => 'Não Iniciado')
						);
						$db->monta_combo('preenchimento', $arDados, 'S', 'Selecione...', '', '');		?>		
					</td>
				</tr>
				
				<tr>
					<td class="SubTituloDireita" valign="top">Situação do Workflow:</td>	
					<td >
					<?php
										
					$esdid = $_POST["esdid"];
					$sql = "select esdid as codigo, esddsc as descricao from workflow.estadodocumento where tpdid = 89 order by esdordem";
					
					$res = $db->carregar( $sql );
					
					$db->monta_combo( "esdid", $res , 'S', 'Todas as Situações', '', '' );
					?>	
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita" valign="top">Instituição:</td>	
					<td >
					<?php
					$sql = "SELECT
								 e.entid as codigo,
								entnome as descricao 
							FROM
								entidade.entidade e 
							INNER JOIN
								entidade.funcaoentidade ef ON ef.entid = e.entid
							WHERE ef.funid in (12)
							ORDER BY
								 descricao";
					
					$res = $db->carregar( $sql );
					?>
					<select name="ies" id="ies" class="CampoEstilo" onchange="filtraCampus(this.value);" >  
					<?php
					echo "<option value=\"\"> Selecione.. </option>";		
					foreach( $res as $ar ){
						echo "<option value=\"".$ar['codigo']."\"> ".$ar['descricao']." </option>";						
					}					
					?>	
					</select>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita" valign="top">Campus:</td>	
					<td id="td_campus">
						<select id="campus" class="CampoEstilo"  name="campus" >
							<option value="">Seleciona uma IES</option>
						</select>
					</td>
				</tr>
				<tr>
					<td  class="SubTituloDireita">
						Exercício&nbsp;
					</td>
					<td>
					<?php 
						$arDados = array(
							array('codigo'=>'2010', 'descricao'=>'2010'),
							array('codigo'=>'2011', 'descricao'=>'2011'),
							array('codigo'=>'2012', 'descricao'=>'2012'),
							array('codigo'=>'2013', 'descricao'=>'2013'),
							array('codigo'=>'2014', 'descricao'=>'2014')
						);
						$aetano = $_REQUEST['aetano'];
						$db->monta_combo('aetano', $arDados, 'S', 'Selecione...', '', '');			
						?>
						</td>
				</tr>
				<tr>
					<td  class="SubTituloDireita">
						&nbsp;Semestre/Ano:&nbsp;
					</td>
					<td>
					<?php 
						$arDados = array(
							array('codigo' => '1', 'descricao' => '1º semestre'),
							array('codigo' => '2', 'descricao' => '2º semestre'),
							array('codigo' => '3', 'descricao' => 'Anual')
						);
						$aetsemestre = $_REQUEST['aetsemestre'];
						$db->monta_combo('aetsemestre', $arDados, 'S', 'Selecione...', '', '');			
						?>	
						</td>
				</tr>
				<tr>
					<td align="center" colspan="2">
						<input type="button"  onclick="document.formulario.submit();" class="pesquisar" value="Pesquisar"/>
						<div style="float:right">
							
						<input type="button" class="limpar" value="Limpar Pesquisa"/>
						</div>
					</td>
				</tr>
				<tr>
					<td colspan="2">
					
					<?php
					
					$arrQueid = array( QUEID_ACADEMICO_INDICADORES, QUEID_ACADEMICO_INFRAESTRUTURA, QUEID_ACADEMICO_SOCIOECONOMICO);

					$sql = "SELECT e.entid, e.entsig, e.entnome FROM entidade.entidade e 
							INNER JOIN entidade.funcaoentidade ef ON ef.entid = e.entid
							WHERE ef.funid IN (12)";
					if( $_POST['ies'] ){
						$sql .= "AND e.entid = ". $_POST['ies'];
					}
					$sql .= " ORDER BY e.entnome";
									
					$resultIES = $db->carregar( $sql );
							
					if( $resultIES ){?>
						<table class="listagem" style="width: 95%;" align=center cellpadding=2 >
						<?php foreach( $resultIES as $dadoIES ){
							
							$numMaxQuestoes = getNumMaxQuestoes( $arrQueid );?>
								
									<?php
									$sql2 = "SELECT distinct e.entid as codigo,e.entnome as descricao,estdoc.*,  aest.*, e2.entid
											FROM entidade.entidade e2
											INNER JOIN entidade.entidade e ON e2.entid = e.entid
											INNER JOIN entidade.funcaoentidade ef ON ef.entid = e.entid
											INNER JOIN entidade.funentassoc ea ON ea.fueid = ef.fueid 
											LEFT JOIN entidade.endereco ed ON ed.entid = e.entid
											LEFT JOIN territorios.municipio mun ON mun.muncod = ed.muncod
											LEFT JOIN academico.assistenciaestudantil aest ON aest.entid = e.entid
										left join academico.itemassistencia as it on it.aetdid = aest.aetdid  
										left join questionario.questionarioresposta as quest on quest.qrpid = it.qrpid
										left join questionario.resposta as resp on quest.qrpid = resp.qrpid
											LEFT JOIN workflow.documento doc ON aest.docid = doc.docid
											LEFT JOIN workflow.estadodocumento estdoc ON estdoc.esdid = doc.esdid
											WHERE ef.funid in (17, 18) and e.entstatus = 'A'
										
											";
									if( $_POST['campus'] ){ 
									$sql2 .= " AND e.entid = ".$_POST['campus'];
									}		
									else{ 
									$sql2 .= " AND ea.entid = ".$dadoIES['entid'];
									}		
									if( $_POST['esdid']){
										$sql2 .= " AND doc.esdid = ".$_POST['esdid'];
									}
									if( $_POST['aetano']){
										$sql2 .= " AND aest.aetano = ".$_POST['aetano'];
									}
									if( $_POST['aetsemestre']){
										$sql2 .= " AND aest.aetsemestre = ".$_POST['aetsemestre'];
									}
									if( $_POST['preenchimento'] == 'naoiniciado'){
										$sql2.= " AND aest.aetdid IS NULL ";
									}elseif( $_POST['preenchimento'] == 'completo' ){
										$sql2 .= " AND (select count(resid) from questionario.resposta  WHERE qrpid = quest.qrpid ) > ".$numMaxQuestoes; 
									}
									$sql2 .= " ORDER BY e.entnome, aest.aetano, aest.aetsemestre";
								
									$resultCampus = $db->carregar( $sql2 );
									
									if( $resultCampus ){?>
										<tr>
											<td>
											<span style="font-size: 10pt; font-weight: bold;">
											<?php echo ( !$dadoIES['entsig'] ? $dadoIES['entsig'] : $dadoIES['entsig'] . ' - ' ) . $dadoIES['entnome']; ?>
											</span>
											<br /><br />
											<table style="margin-left: 40px;" class="listagem" width=90%>
											<tr>
												<th>
													Campus
												</th>
												<th>
													Situação
												</th>
												<th>
													Semestre/Ano
												</th>
											</tr>
											<?php foreach( $resultCampus as $dadoCampus ){									
												
												?>
												 
													<tr>
														<td style="font-weight: bold;">
															<?php
																														
															if( $dadoCampus['aetdid']){?>
																<a href="academico.php?modulo=principal/assistenciaEstudantil&acao=A&aetsemestre=<?php echo $dadoCampus['aetsemestre']; ?>&aetano=<?php echo $dadoCampus['aetano']; ?>&entid=<?php echo $dadoCampus['codigo']; ?>" id="link_<?php echo $dadoCampus['aetdid']; ?>"><?php echo $dadoCampus['descricao']; ?></a>
															<?php }else{ ?>
																<a href="academico.php?modulo=principal/assistenciaEstudantil&acao=A&entid=<?php echo $dadoCampus['codigo']; ?>" id="link_<?php echo $dadoCampus['aetdid']; ?>"><?php echo $dadoCampus['descricao']; ?></a>
															<?php } ?>
														</td>
														<td>
															<?php 
															if( $dadoCampus['docid'] ){
																echo $dadoCampus['esddsc']; 
															}else{
																echo 'Não Iniciado';
															}?>
														</td>
														<td style="text-align: center;">
															<?php
															if( $dadoCampus['aetdid']){
		
																echo $dadoCampus['aetsemestre']. 'º / ' . $dadoCampus['aetano'];
															}
															?>
														</td>
													</tr>
												<?php }
											?>
										</table>
										<br />
										<?php
									}
									?>
								</td>
							</tr>
						<?php }	?>
						</table>
						<?php
					}
					?>
					</td>
				</tr>
			</table>
		</form>
	</body>
</html>