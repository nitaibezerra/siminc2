<?
pde_verificaSessao();

/*----------------- Configurações de Página ---------------------*/ 
header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

/*----------------- Ações - Formulario ---------------------*/ 
$pdeid 		= $_SESSION["pdeid"];
$values = array();
$cDado = array("instrumento" => 1);
$cForm = new formulario($cDado);
$cForm->direcPag('salva( pdeid )', array("pdeid" => $pdeid));

function salva ($pdeid){
	global $db;
	if($_POST["submetido"]) {
		$SQL = "delete from pdeescola.radciclo where racid in
				(SELECT RC.racid FROM pdeescola.relacaoalunodocente RDA
				INNER JOIN pdeescola.radcicloserie RCS ON RCS.radid = RDA.radid 
				INNER JOIN pdeescola.radciclo RC ON RC.rcsid = RCS.rcsid 
				WHERE RDA.pdeid = ".$pdeid." )";
		$db->executar($SQL);
		
		$SQL = "delete from pdeescola.radserie where rasid in
			(SELECT RC.rasid FROM pdeescola.relacaoalunodocente RDA
			INNER JOIN pdeescola.radcicloserie RCS ON RCS.radid = RDA.radid 
			INNER JOIN pdeescola.radserie RC ON RC.rcsid = RCS.rcsid 
			WHERE RDA.pdeid = ".$pdeid." )";
		$db->executar($SQL);
		
		$SQL = "delete from pdeescola.radcicloserie where radid in
				(SELECT RCS.radid FROM pdeescola.relacaoalunodocente RDA
				INNER JOIN pdeescola.radcicloserie RCS ON RCS.radid = RDA.radid 
				WHERE RDA.pdeid = ".$pdeid." )";
		$db->executar($SQL);

		$sqlDel = "DELETE FROM  pdeescola.relacaoalunodocente WHERE pdeid = ".$pdeid;
		$db->executar($sqlDel);
		$quantidade = 0;
		foreach($_POST['aluno'] as $turno=>$dados){
			$titid = $turno;
			$radnaodocente = ($_POST['Nnaodocente'][$titid]== NULL) ? 0 : $_POST['Nnaodocente'][$titid];
			$sqlRelacao = " INSERT INTO pdeescola.relacaoalunodocente (	pdeid, 
																			titid, 
																			radnaodocente, 
																			radanoreferencia, 
																			raddataatualizacao
																		   ) VALUES 
																		   (".$pdeid.",
																			".$titid.",
																			".$radnaodocente.",
																			".ANO_EXERCICIO_PDE_ESCOLA.",
																			current_date) returning radid";
			
			$radid = $db->pegaUm($sqlRelacao); 
			foreach($_POST['aluno'][$titid] as $chaveserieCiclo=>$v){
				if($chaveserieCiclo == "C"){
					foreach( $_POST['aluno'][$titid]['C'] as $alunodocente=>$valores ){
						$sql="INSERT INTO pdeescola.radcicloserie (	radid, 
																		rcsalunodocente
																	      ) VALUES (
																		".$radid.", 
																		'".$alunodocente."') returning rcsid";
						
						$rcsid = $db->pegaUm($sql);
						$quantidade = 0;
						foreach( $valores as $tipociclo=>$dados ){
							$tipociclo = (integer) $tipociclo;
							$quantidade = (integer) $dados;
							if( $quantidade != 0 ){
								$sqlValor = "INSERT INTO pdeescola.radciclo (	racqtd, 
																			cicid,
																			rcsid
																		      ) VALUES (
																			".$quantidade.", 
																			".$tipociclo.",
																			".$rcsid.")";
								$db->executar($sqlValor);
							}	
						}
					
					}
				}else if($chaveserieCiclo == "S"){
					foreach( $_POST['aluno'][$titid]['S'] as $alunodocente=>$valores ){
						$sql="INSERT INTO pdeescola.radcicloserie (	radid, 
																		rcsalunodocente
																	) VALUES (
																		".$radid.", 
																		'".$alunodocente."') returning rcsid";
						$rcsid = $db->pegaUm($sql);
						$quantidade = 0;
						foreach( $valores as $tipoSerie=>$dados ){
							$tipoSerie = (integer) $tipoSerie;
							$quantidade = (integer) $dados;
							if( $quantidade != 0 ){
								$sqlValor = "INSERT INTO pdeescola.radserie (	rasqtd, 
																				tmeid,
																				rcsid
																			      ) VALUES (
																				".$quantidade.", 
																				".$tipoSerie.",
																				".$rcsid.")";
								$db->executar($sqlValor);
							}
						}
					}
				}
			}		
		}
		
		$epfid = $db->pegaUm("SELECT
									epfid
								  FROM
								  	pdeescola.estruturaperfilfuncionamento
								  WHERE
								  	trim(epfnomearquivo) = 'p14_2' AND
								  	epfano = ".ANO_EXERCICIO_PDE_ESCOLA);
		
		$existe = $db->pegaUm("SELECT
								pepid
							 FROM
							 	pdeescola.pdeepf
							 WHERE
							 	pdeid = ".$pdeid." AND
							 	epfid = ".$epfid);
		
		if($existe == NULL) {
			$sql = "INSERT INTO pdeescola.pdeepf(pdeid,epfid) VALUES(".$pdeid.",".$epfid.")";
			$db->executar($sql);
		}		
		 
		$existe_preenchimento = $db->pegaUm("SELECT 
												pprid
											 FROM 
											 	pdeescola.pdepreenchimento
											 WHERE
											 	pdeid = '$pdeid'
											 AND
											 	ppritem = 'p14_2'");
		if( $existe_preenchimento == NULL )
		{
			$sql_p = "INSERT INTO pdeescola.pdepreenchimento( pdeid, pprinstrumento, ppritem, pdeanoreferencia) VALUES ( '$pdeid', 'i1', 'p14_2','{$_SESSION['exercicio_atual']}')";
			$db->executar( $sql_p ); 
		}
		$db->commit();  
	}
}

include  APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';
monta_titulo('Instrumento 1', ''); 

/*----------------- Recupera dados ---------------------*/  
$sql="	SELECT  distinct 
				RDA.titid as turno,
				RCS.rcsalunodocente as alunoOUdocente,
				RS.tmeid as serie,
				RC.cicid as ciclo,
				RS.rasqtd as quantidadeserie,  
				RC.racqtd as quantidadeciclo,   
				RDA.radnaodocente as quantidadealunonaodocente
		FROM pdeescola.relacaoalunodocente RDA
		INNER JOIN pdeescola.radcicloserie RCS ON RCS.radid = RDA.radid 
		LEFT JOIN pdeescola.radciclo RC ON RC.rcsid = RCS.rcsid 
		LEFT JOIN pdeescola.radserie RS ON RS.rcsid = RCS.rcsid
		WHERE RDA.pdeid = ".$pdeid;
$dados =  $db->carregar($sql);
if(is_array($dados)){
	foreach( $dados as $dados){
		if( ($dados['serie'] != NULL) ){ // Se serie
			${"aluno[".$dados['turno']."][S][".$dados['alunooudocente']."][".$dados['serie']."]"} = $dados['quantidadeserie'];	
		}
		if($dados['ciclo']!= NULL){ // Se ciclo
			${"aluno[".$dados['turno']."][C][".$dados['alunooudocente']."][".$dados['ciclo']."]"} = $dados['quantidadeciclo'];
		}
		${"Nnaodocente[".$dados['turno']."]"} = $dados['quantidadealunonaodocente'];
	}
}
?>
<?=cabecalhoPDE(); ?>
<?=subCabecalho('14.2 - Relação aluno / docente e aluno / não-docente'); ?>	
<form id="formulario" name="formulario" method="post" action="">
<input type="hidden" id="submetido" name="submetido" value="0">
<table class="tabela" id="relacao" border="0" align="center" cellpadding="2" cellspacing="1" width="100%">
  <tr>
    <th>Turno</th>
    <th colspan="8">Nº de alunos </th>
    <th colspan="8">Nº de docentes </th>
    <th rowspan="3">N° de não docente (R) </th>
    <th colspan="8">Relação aluno /  docente </th>
    <th rowspan="3">Relação aluno / não- docente </th>
  </tr>
  <tr>
    <th></th>
    <th>EI</th>
    <th colspan="5">EF</th>
    <th>EJA</th>
    <th>EM</th>
    <th>EI</th>
    <th colspan="5">EF</th>
    <th>EJA</th>
    <th>EM</th>
    <th>EI</th>
    <th colspan="5">EF</th>
    <th>EJA</th>
    <th>EM</th>
  </tr>
  <tr>
    <th></th>
    <th>Série / Ano</th>
    <th colspan="2">Serie / Ano</th>
    <th colspan="3">Ciclo</th>
    <th>Etapa</th>
    <th>Série / Ano</th>
    <th>Série / Ano</th>
    <th colspan="2">Serie / Ano</th>
    <th colspan="3">Ciclo</th>
    <th>Etapa</th>
    <th>Série / Ano</th>
    <th>Série /Ano</th>
    <th colspan="2">Serie / Ano</th>
    <th colspan="3">Ciclo</th>
    <th>Etapa</th>
    <th>Série / Ano</th>
  </tr>
  <tr>
    <th></th>
    <th>Pré<br>(A)</th>
    <th>Iniciais<br>(B)</th>
    <th>Finais<br>(C)</th>
    <th>1º<br>(D)</th>
    <th>2º<br>(E)</th>
    <th>3º<br>(F)</th>
    <th>1ª a 8ª<br>(G)</th>
    <th>1ª a 3ª<br>(H)</th>
    <th>Pré<br>(I)</th>
    <th>Iniciais<br>(J)</th>
    <th>Finais<br>(L)</th>
    <th>1º<br>(M)</th>
    <th>2º<br>(N)</th>
    <th>3º<br>(O)</th>
    <th>1º a 8º<br>(P)</th>
    <th>1º a 3º<br>(Q)</th>
    <th></th>
    <th>Pré<br>(A/I)</th>
    <th>Iniciais<br>(B/J)</th>
    <th>Finais<br>(C/L)</th>
    <th>1º<br>(D/M)</th>
    <th>2º<br>(E/N)</th>
    <th>3º<br>(F/O)</th>
    <th>1ª a 8ª<br>(G/P)</th>
    <th>1ª a 3ª<br>(H/Q)</th>
    <th>Soma do nº de alunos / R</th>
  </tr>
  <tr>
    <th>Mat.</th>
    <td><?=campo_texto('aluno['.TURNO_MATUTINO.'][S][A]['.PRE.']', '', 'S' , '', 5, 3, '##########', '', 'left', '', 0, 'onblur="totalizar();" " id="aluno['.TURNO_MATUTINO.'][S][A]['.PRE.']"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_MATUTINO.'][S][A]['.FUNDAMENTAL1A4SERIE.']', '', 'S' , '', 5, 3, '##########', '', 'left', '', 0, 'id="aluno['.TURNO_MATUTINO.'][S][A]['.FUNDAMENTAL1A4SERIE.']" onblur="totalizar();"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_MATUTINO.'][S][A]['.FUNDAMENTAL5A8SERIE.']', '', 'S' , '', 5, 3, '##########', '', 'left', '', 0, 'id="aluno['.TURNO_MATUTINO.'][S][A]['.FUNDAMENTAL5A8SERIE.']" onblur="totalizar();"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_MATUTINO.'][C][A]['.CICLO1.']', '', 'S' , '', 5, 3, '##########', '', 'left', '', 0, 'id="aluno['.TURNO_MATUTINO.'][C][A]['.CICLO1.']" onblur="totalizar();"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_MATUTINO.'][C][A]['.CICLO2.']', '', 'S' , '', 5, 3, '##########', '', 'left', '', 0, 'id="aluno['.TURNO_MATUTINO.'][C][A]['.CICLO2.']" onblur="totalizar();"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_MATUTINO.'][C][A]['.CICLO3.']', '', 'S' , '', 5, 3, '##########', '', 'left', '', 0, 'id="aluno['.TURNO_MATUTINO.'][C][A]['.CICLO3.']" onblur="totalizar();"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_MATUTINO.'][S][A]['.ENSINODEJOVENS1A4SERIE.']', '', 'S' , '', 5, 3, '##########', '', 'left', '', 0, 'id="aluno['.TURNO_MATUTINO.'][S][A]['.ENSINODEJOVENS1A4SERIE.']" onblur="totalizar();"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_MATUTINO.'][S][A]['.ENSINOMEDIO1A3.']', '', 'S' , '', 5, 3, '##########', '', 'left', '', 0, 'id="aluno['.TURNO_MATUTINO.'][S][A]['.ENSINOMEDIO1A3.']" onblur="totalizar();"'); ?></td>
   
    <td><?=campo_texto('aluno['.TURNO_MATUTINO.'][S][D]['.PRE.']', '', 'S' , '', 5, 3, '##########', '', 'left', '', 0, 'id="aluno['.TURNO_MATUTINO.'][S][D]['.PRE.']" onblur="totalizar();" '); ?></td>
    <td><?=campo_texto('aluno['.TURNO_MATUTINO.'][S][D]['.FUNDAMENTAL1A4SERIE.']', '', 'S' , '', 5, 3, '##########', '', 'left', '', 0, 'id="aluno['.TURNO_MATUTINO.'][S][D]['.FUNDAMENTAL1A4SERIE.']" onblur="totalizar();"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_MATUTINO.'][S][D]['.FUNDAMENTAL5A8SERIE.']', '', 'S' , '', 5, 3, '##########', '', 'left', '', 0, 'id="aluno['.TURNO_MATUTINO.'][S][D]['.FUNDAMENTAL5A8SERIE.']" onblur="totalizar();"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_MATUTINO.'][C][D]['.CICLO1.']', '', 'S' , '', 5, 3, '##########', '', 'left', '', 0, 'id="aluno['.TURNO_MATUTINO.'][C][D]['.CICLO1.']" onblur="totalizar();"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_MATUTINO.'][C][D]['.CICLO2.']', '', 'S' , '', 5, 3, '##########', '', 'left', '', 0, 'id="aluno['.TURNO_MATUTINO.'][C][D]['.CICLO2.']" onblur="totalizar();"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_MATUTINO.'][C][D]['.CICLO3.']', '', 'S' , '', 5, 3, '##########', '', 'left', '', 0, 'id="aluno['.TURNO_MATUTINO.'][C][D]['.CICLO3.']" onblur="totalizar();"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_MATUTINO.'][S][D]['.ENSINODEJOVENS1A4SERIE.']', '', 'S' , '', 5, 3, '##########', '', 'left', '', 0, 'id="aluno['.TURNO_MATUTINO.'][S][D]['.ENSINODEJOVENS1A4SERIE.']" onblur="totalizar();"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_MATUTINO.'][S][D]['.ENSINOMEDIO1A3.']', '', 'S' , '', 5, 3, '##########', '', 'left', '', 0, 'id="aluno['.TURNO_MATUTINO.'][S][D]['.ENSINOMEDIO1A3.']" onblur="totalizar();"'); ?></td>
    <td><?=campo_texto('Nnaodocente['.TURNO_MATUTINO.']', '', 'S' , '', 5, 3, '##########', '', 'left', '', 0, 'id="Nnaodocente['.TURNO_MATUTINO.']" onblur="totalizar();"'); ?></td>
    
    <td><?=campo_texto('totalSerieMatPre', '', 'N' , '', 5, 3,'','', 'left', '', 0, 'id="totalSerieMatPre"'); ?></td>
    <td><?=campo_texto('totalSerieMatF1A4', '', 'N' , '', 5, 3, '', '', 'left', '', 0, 'id="totalSerieMatF1A4"'); ?></td>
    <td><?=campo_texto('totalSerieMatF5A8', '', 'N' , '', 5, 3, '', '', 'left', '', 0, 'id="totalSerieMatF5A8"'); ?></td>
    <td><?=campo_texto('totalSerieMatC1', '', 'N' , '', 5, 3, '', '', 'left', '', 0, 'id="totalSerieMatC1"'); ?></td>
    <td><?=campo_texto('totalSerieMatC2', '', 'N' , '', 5, 3, '', '', 'left', '', 0, 'id="totalSerieMatC2"'); ?></td>
    <td><?=campo_texto('totalSerieMatC3', '', 'N' , '', 5, 3, '', '', 'left', '', 0, 'id="totalSerieMatC3"'); ?></td>
    <td><?=campo_texto('totalSerieMatJ1A4', '', 'N' , '', 5, 3, '', '', 'left', '', 0, 'id="totalSerieMatJ1A4"'); ?></td>
    <td><?=campo_texto('totalSerieMatM1A3', '', 'N' , '', 5, 3, '', '', 'left', '', 0, 'id="totalSerieMatM1A3"'); ?></td>
    <td><?=campo_texto('totalSerieMatNdocente', '', 'N' , '', 5, 3, '', '', 'left', '', 0, 'id="totalSerieMatNdocente"'); ?></td>
  </tr>
  <tr>
    <th>Vesp.</th>
    <td><?=campo_texto('aluno['.TURNO_VESPERTINO.'][S][A]['.PRE.']', '', 'S' , '', 5, 3, '##########', '', 'left', '', 0, 'onblur="totalizar();" id="aluno['.TURNO_VESPERTINO.'][S][A]['.PRE.']"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_VESPERTINO.'][S][A]['.FUNDAMENTAL1A4SERIE.']', '', 'S' , '', 5, 3, '##########', '', 'left', '', 0, 'onblur="totalizar();" id="aluno['.TURNO_VESPERTINO.'][S][A]['.FUNDAMENTAL1A4SERIE.']"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_VESPERTINO.'][S][A]['.FUNDAMENTAL5A8SERIE.']', '', 'S' , '', 5, 3, '##########', '', 'left', '', 0, 'onblur="totalizar();" id="aluno['.TURNO_VESPERTINO.'][S][A]['.FUNDAMENTAL5A8SERIE.']"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_VESPERTINO.'][C][A]['.CICLO1.']', '', 'S' , '', 5, 3, '##########', '', 'left', '', 0, 'onblur="totalizar();" id="aluno['.TURNO_VESPERTINO.'][C][A]['.CICLO1.']"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_VESPERTINO.'][C][A]['.CICLO2.']', '', 'S' , '', 5, 3, '##########', '', 'left', '', 0, 'onblur="totalizar();" id="aluno['.TURNO_VESPERTINO.'][C][A]['.CICLO2.']"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_VESPERTINO.'][C][A]['.CICLO3.']', '', 'S' , '', 5, 3, '##########', '', 'left', '', 0, 'onblur="totalizar();" id="aluno['.TURNO_VESPERTINO.'][C][A]['.CICLO3.']"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_VESPERTINO.'][S][A]['.ENSINODEJOVENS1A4SERIE.']', '', 'S' , '', 5, 3, '##########', '', 'left', '', 0, 'onblur="totalizar();" id="aluno['.TURNO_VESPERTINO.'][S][A]['.ENSINODEJOVENS1A4SERIE.']"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_VESPERTINO.'][S][A]['.ENSINOMEDIO1A3.']', '', 'S' , '', 5, 3, '##########', '', 'left', '', 0, ' onblur="totalizar();" id="aluno['.TURNO_VESPERTINO.'][S][A]['.ENSINOMEDIO1A3.']"'); ?></td>
    
    <td><?=campo_texto('aluno['.TURNO_VESPERTINO.'][S][D]['.PRE.']', '', 'S' , '', 5, 3, '##########', '', 'left', '', 0, 'onblur="totalizar();" id="aluno['.TURNO_VESPERTINO.'][S][D]['.PRE.']"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_VESPERTINO.'][S][D]['.FUNDAMENTAL1A4SERIE.']', '', 'S' , '', 5, 3, '##########', '', 'left', '', 0, 'onblur="totalizar();" id="aluno['.TURNO_VESPERTINO.'][S][D]['.FUNDAMENTAL1A4SERIE.']"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_VESPERTINO.'][S][D]['.FUNDAMENTAL5A8SERIE.']', '', 'S' , '', 5, 3, '##########', '', 'left', '', 0, 'onblur="totalizar();" id="aluno['.TURNO_VESPERTINO.'][S][D]['.FUNDAMENTAL5A8SERIE.']"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_VESPERTINO.'][C][D]['.CICLO1.']', '', 'S' , '', 5, 3, '##########', '', 'left', '', 0, 'onblur="totalizar();" id="aluno['.TURNO_VESPERTINO.'][C][D]['.CICLO1.']"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_VESPERTINO.'][C][D]['.CICLO2.']', '', 'S' , '', 5, 3, '##########', '', 'left', '', 0, 'onblur="totalizar();" id="aluno['.TURNO_VESPERTINO.'][C][D]['.CICLO2.']"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_VESPERTINO.'][C][D]['.CICLO3.']', '', 'S' , '', 5, 3, '##########', '', 'left', '', 0, 'onblur="totalizar();" id="aluno['.TURNO_VESPERTINO.'][C][D]['.CICLO3.']"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_VESPERTINO.'][S][D]['.ENSINODEJOVENS1A4SERIE.']', '', 'S' , '', 5, 3, '##########', '', 'left', '', 0, 'onblur="totalizar();" id="aluno['.TURNO_VESPERTINO.'][S][D]['.ENSINODEJOVENS1A4SERIE.']"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_VESPERTINO.'][S][D]['.ENSINOMEDIO1A3.']', '', 'S' , '', 5, 3, '##########', '', 'left', '', 0, 'onblur="totalizar();" id="aluno['.TURNO_VESPERTINO.'][S][D]['.ENSINOMEDIO1A3.']"'); ?></td>
    <td><?=campo_texto('Nnaodocente['.TURNO_VESPERTINO.']', '', 'S' , '', 5, 3, '##########', '', 'left', '', 0, 'onblur="totalizar();" id="Nnaodocente['.TURNO_VESPERTINO.']"'); ?></td>
    <td><?=campo_texto('totalSerieVesPre', '', 'N' , '', 5, 3, '', '', 'left', '', 0, 'id="totalSerieVesPre"'); ?></td>
    <td><?=campo_texto('totalSerieVesF1a4', '', 'N' , '', 5, 3, '', '', 'left', '', 0, 'id="totalSerieVesF1a4"'); ?></td>
    <td><?=campo_texto('totalSerieVesF5a8', '', 'N' , '', 5, 3, '', '', 'left', '', 0, 'id="totalSerieVesF5a8"'); ?></td>
    <td><?=campo_texto('totalSerieVesC1', '', 'N' , '', 5, 3, '', '', 'left', '', 0, 'id="totalSerieVesC1"'); ?></td>
    <td><?=campo_texto('totalSerieVesC2', '', 'N' , '', 5, 3, '', '', 'left', '', 0, 'id="totalSerieVesC2"'); ?></td>
    <td><?=campo_texto('totalSerieVesC3', '', 'N' , '', 5, 3, '', '', 'left', '', 0, 'id="totalSerieVesC3"'); ?></td>
    <td><?=campo_texto('totalSerieVesJ1a4', '', 'N' , '', 5, 3, '', '', 'left', '', 0, 'id="totalSerieVesJ1a4"'); ?></td>
    <td><?=campo_texto('totalSerieVesM1a3', '', 'N' , '', 5, 3, '', '', 'left', '', 0, 'id="totalSerieVesM1a3"'); ?></td>
    <td><?=campo_texto('totalSerieVesNdocente', '', 'N' , '', 5, 3, '', '', 'left', '', 0, 'id="totalSerieVesNdocente"'); ?></td>
  </tr>
  <tr>
    <th>Not.</th>
    <td><?=campo_texto('aluno['.TURNO_NOTURNO.'][S][A]['.PRE.']', '', 'S' , '', 5, 3, '##########', '', 'left', '', 0, 'id="aluno['.TURNO_NOTURNO.'][S][A]['.PRE.']" onblur="totalizar();"');?></td>
    <td><?=campo_texto('aluno['.TURNO_NOTURNO.'][S][A]['.FUNDAMENTAL1A4SERIE.']', '', 'S' , '', 5, 3, '##########', '', 'left', '', 0, 'onblur="totalizar();" id="aluno['.TURNO_NOTURNO.'][S][A]['.FUNDAMENTAL1A4SERIE.']"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_NOTURNO.'][S][A]['.FUNDAMENTAL5A8SERIE.']', '', 'S' , '', 5, 3, '##########', '', 'left', '', 0, 'onblur="totalizar();" id="aluno['.TURNO_NOTURNO.'][S][A]['.FUNDAMENTAL5A8SERIE.']"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_NOTURNO.'][C][A]['.CICLO1.']', '', 'S' , '', 5, 3, '##########', '', 'left', '', 0, 'onblur="totalizar();" id="aluno['.TURNO_NOTURNO.'][C][A]['.CICLO1.']"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_NOTURNO.'][C][A]['.CICLO2.']', '', 'S' , '', 5, 3, '##########', '', 'left', '', 0, 'onblur="totalizar();" id="aluno['.TURNO_NOTURNO.'][C][A]['.CICLO2.']"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_NOTURNO.'][C][A]['.CICLO3.']', '', 'S' , '', 5, 3, '##########', '', 'left', '', 0, 'onblur="totalizar();" id="aluno['.TURNO_NOTURNO.'][C][A]['.CICLO3.']"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_NOTURNO.'][S][A]['.ENSINODEJOVENS1A4SERIE.']', '', 'S' , '', 5, 3, '##########', '', 'left', '', 0, 'onblur="totalizar();" id="aluno['.TURNO_NOTURNO.'][S][A]['.ENSINODEJOVENS1A4SERIE.']"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_NOTURNO.'][S][A]['.ENSINOMEDIO1A3.']', '', 'S' , '', 5, 3, '##########', '', 'left', '', 0, 'onblur="totalizar();" id="aluno['.TURNO_NOTURNO.'][S][A]['.ENSINOMEDIO1A3.']"'); ?></td>
    
    <td><?=campo_texto('aluno['.TURNO_NOTURNO.'][S][D]['.PRE.']', '', 'S' , '', 5, 3, '##########', '', 'left', '', 0, 'onblur="totalizar();" id="aluno['.TURNO_NOTURNO.'][S][D]['.PRE.']"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_NOTURNO.'][S][D]['.FUNDAMENTAL1A4SERIE.']', '', 'S' , '', 5, 3, '##########', '', 'left', '', 0, 'onblur="totalizar();" id="aluno['.TURNO_NOTURNO.'][S][D]['.FUNDAMENTAL1A4SERIE.']"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_NOTURNO.'][S][D]['.FUNDAMENTAL5A8SERIE.']', '', 'S' , '', 5, 3, '##########', '', 'left', '', 0, 'onblur="totalizar();" id="aluno['.TURNO_NOTURNO.'][S][D]['.FUNDAMENTAL5A8SERIE.']"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_NOTURNO.'][C][D]['.CICLO1.']', '', 'S' , '', 5, 3, '##########', '', 'left', '', 0, 'onblur="totalizar();" id="aluno['.TURNO_NOTURNO.'][C][D]['.CICLO1.']"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_NOTURNO.'][C][D]['.CICLO2.']', '', 'S' , '', 5, 3, '##########', '', 'left', '', 0, 'onblur="totalizar();" id="aluno['.TURNO_NOTURNO.'][C][D]['.CICLO2.']"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_NOTURNO.'][C][D]['.CICLO3.']', '', 'S' , '', 5, 3, '##########', '', 'left', '', 0, 'onblur="totalizar();" id="aluno['.TURNO_NOTURNO.'][C][D]['.CICLO3.']"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_NOTURNO.'][S][D]['.ENSINODEJOVENS1A4SERIE.']', '', 'S' , '', 5, 3, '##########', '', 'left', '', 0, 'onblur="totalizar();" id="aluno['.TURNO_NOTURNO.'][S][D]['.ENSINODEJOVENS1A4SERIE.']"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_NOTURNO.'][S][D]['.ENSINOMEDIO1A3.']', '', 'S' , '', 5, 3, '##########', '', 'left', '', 0, 'onblur="totalizar();" id="aluno['.TURNO_NOTURNO.'][S][D]['.ENSINOMEDIO1A3.']"'); ?></td>
    <td><?=campo_texto('Nnaodocente['.TURNO_NOTURNO.']', '', 'S' , '', 5, 3, '##########', '', 'left', '', 0, 'onblur="totalizar();" id="Nnaodocente['.TURNO_NOTURNO.']"');?></td>
     <td><?=campo_texto('totalSerieNotPre', '', 'N' , '', 5, 4, '', '', 'left', '', 0, 'id="totalSerieNotPre"'); ?></td>
    <td><?=campo_texto('totalSerieNotF1a4', '', 'N' , '', 5, 4, '', '', 'left', '', 0, 'id="totalSerieNotF1a4"'); ?></td>
    <td><?=campo_texto('totalSerieNotF5a8', '', 'N' , '', 5, 4, '', '', 'left', '', 0, 'id="totalSerieNotF5a8"'); ?></td>
    <td><?=campo_texto('totalSerieNotC1', '', 'N' , '', 5, 4, '', '', 'left', '', 0, 'id="totalSerieNotC1"'); ?></td>
    <td><?=campo_texto('totalSerieNotC2', '', 'N' , '', 5, 4, '', '', 'left', '', 0, 'id="totalSerieNotC2"'); ?></td>
    <td><?=campo_texto('totalSerieNotC3', '', 'N' , '', 5, 4, '', '', 'left', '', 0, 'id="totalSerieNotC3"'); ?></td>
    <td><?=campo_texto('totalSerieNotJ1a4', '', 'N' , '', 5, 4, '', '', 'left', '', 0, 'id="totalSerieNotJ1a4"'); ?></td>
    <td><?=campo_texto('totalSerieNotM1a3', '', 'N' , '', 5, 4, '', '', 'left', '', 0, 'id="totalSerieNotM1a3"'); ?></td>
    <td><?=campo_texto('totalSerieNotNdocente', '', 'N' , '', 5, 4, '', '', 'left', '', 0, 'id="totalSerieNotNdocente"'); ?></td>
  </tr>
  <tr>
    <th>Total</th>
    <td><?=campo_texto('totalCol1', '', 'N' , '', 5, 4, '', '', 'left', '', 0, 'id="totalCol1"'); ?></td>
    <td><?=campo_texto('totalCol2', '', 'N' , '', 5, 4, '', '', 'left', '', 0, 'id="totalCol2"'); ?></td>
    <td><?=campo_texto('totalCol3', '', 'N' , '', 5, 4, '', '', 'left', '', 0, 'id="totalCol3"'); ?></td>
    <td><?=campo_texto('totalCol4', '', 'N' , '', 5, 4, '', '', 'left', '', 0, 'id="totalCol4"'); ?></td>
    <td><?=campo_texto('totalCol5', '', 'N' , '', 5, 4, '', '', 'left', '', 0, 'id="totalCol5"'); ?></td>
    <td><?=campo_texto('totalCol6', '', 'N' , '', 5, 4, '', '', 'left', '', 0, 'id="totalCol6"'); ?></td>
    <td><?=campo_texto('totalCol7', '', 'N' , '', 5, 4, '', '', 'left', '', 0, 'id="totalCol7"'); ?></td>
    <td><?=campo_texto('totalCol8', '', 'N' , '', 5, 4, '', '', 'left', '', 0, 'id="totalCol8"'); ?></td>
    <td><?=campo_texto('totalCol9', '', 'N' , '', 5, 4, '', '', 'left', '', 0, 'id="totalCol9"'); ?></td>
    <td><?=campo_texto('totalCol10', '', 'N' , '', 5, 4, '', '', 'left', '', 0, 'id="totalCol10"'); ?></td>
    <td><?=campo_texto('totalCol11', '', 'N' , '', 5, 4, '', '', 'left', '', 0, 'id="totalCol11"'); ?></td>
    <td><?=campo_texto('totalCol12', '', 'N' , '', 5, 4, '', '', 'left', '', 0, 'id="totalCol12"'); ?></td>
    <td><?=campo_texto('totalCol13', '', 'N' , '', 5, 4, '', '', 'left', '', 0, 'id="totalCol13"'); ?></td>
    <td><?=campo_texto('totalCol14', '', 'N' , '', 5, 4, '', '', 'left', '', 0, 'id="totalCol14"'); ?></td>
    <td><?=campo_texto('totalCol15', '', 'N' , '', 5, 4, '', '', 'left', '', 0, 'id="totalCol15"'); ?></td>
    <td><?=campo_texto('totalCol16', '', 'N' , '', 5, 4, '', '', 'left', '', 0, 'id="totalCol16"'); ?></td>
    <td><?=campo_texto('totalCol17', '', 'N' , '', 5, 4, '', '', 'left', '', 0, 'id="totalCol17"'); ?></td>
    <td><?=campo_texto('totalCol18', '', 'N' , '', 5, 4, '', '', 'left', '', 0, 'id="totalCol18"'); ?></td>
    <td><?=campo_texto('totalCol19', '', 'N' , '', 5, 4, '', '', 'left', '', 0, 'id="totalCol19"'); ?></td>
    <td><?=campo_texto('totalCol20', '', 'N' , '', 5, 4, '', '', 'left', '', 0, 'id="totalCol20"'); ?></td>
    <td><?=campo_texto('totalCol21', '', 'N' , '', 5, 4, '', '', 'left', '', 0, 'id="totalCol21"'); ?></td>
    <td><?=campo_texto('totalCol22', '', 'N' , '', 5, 4, '', '', 'left', '', 0, 'id="totalCol22"'); ?></td>
    <td><?=campo_texto('totalCol23', '', 'N' , '', 5, 4, '', '', 'left', '', 0, 'id="totalCol23"'); ?></td>
    <td><?=campo_texto('totalCol24', '', 'N' , '', 5, 4, '', '', 'left', '', 0, 'id="totalCol24"'); ?></td>
    <td><?=campo_texto('totalCol25', '', 'N' , '', 5, 4, '', '', 'left', '', 0, 'id="totalCol25"'); ?></td>
    <td><?=campo_texto('totalCol26', '', 'N' , '', 5, 4, '', '', 'left', '', 0, 'id="totalCol26"'); ?></td>
  </tr>
</table>


<table class="tabela" bgcolor="#f5f5f5" align="center">
 <? $cForm->montarbuttons("submeterDados()");?>				
</table>
</form>


<script language="javascript" type="text/javascript">
function submeterDados(){
	document.getElementById('submetido').value = 1;
	document.getElementById('formulario').submit();
	return true;
}

function totalizar(){
	var somaAlunosN = 0;
	/////////// CALCULOS MATUTINO //////////////////
	var pre1M 		= document.getElementById("aluno[<?=TURNO_MATUTINO;?>][S][A][<?=PRE;?>]").value;
	var pre2M 		= document.getElementById("aluno[<?=TURNO_MATUTINO;?>][S][D][<?=PRE;?>]").value;
	var funda1M 	= document.getElementById("aluno[<?=TURNO_MATUTINO;?>][S][A][<?=FUNDAMENTAL1A4SERIE;?>]").value;
	var funda2M 	= document.getElementById("aluno[<?=TURNO_MATUTINO;?>][S][D][<?=FUNDAMENTAL1A4SERIE;?>]").value;
	var funda3M 	= document.getElementById("aluno[<?=TURNO_MATUTINO;?>][S][A][<?=FUNDAMENTAL5A8SERIE;?>]").value;
	var funda4M 	= document.getElementById("aluno[<?=TURNO_MATUTINO;?>][S][D][<?=FUNDAMENTAL5A8SERIE;?>]").value;
	var jovem1M 	= document.getElementById("aluno[<?=TURNO_MATUTINO;?>][S][A][<?=ENSINODEJOVENS1A4SERIE;?>]").value;
	var jovem2M 	= document.getElementById("aluno[<?=TURNO_MATUTINO;?>][S][D][<?=ENSINODEJOVENS1A4SERIE;?>]").value;
	var medio1M 	= document.getElementById("aluno[<?=TURNO_MATUTINO;?>][S][A][<?=ENSINOMEDIO1A3;?>]").value;
	var medio2M 	= document.getElementById("aluno[<?=TURNO_MATUTINO;?>][S][D][<?=ENSINOMEDIO1A3;?>]").value;
	var ciclo1M 	= document.getElementById("aluno[<?=TURNO_MATUTINO;?>][C][A][<?=CICLO1;?>]").value;
	var ciclo2M 	= document.getElementById("aluno[<?=TURNO_MATUTINO;?>][C][D][<?=CICLO1;?>]").value;
	var ciclo3M 	= document.getElementById("aluno[<?=TURNO_MATUTINO;?>][C][A][<?=CICLO2;?>]").value;
	var ciclo4M 	= document.getElementById("aluno[<?=TURNO_MATUTINO;?>][C][D][<?=CICLO2;?>]").value;
	var ciclo5M 	= document.getElementById("aluno[<?=TURNO_MATUTINO;?>][C][A][<?=CICLO3;?>]").value;
	var ciclo6M 	= document.getElementById("aluno[<?=TURNO_MATUTINO;?>][C][D][<?=CICLO3;?>]").value;
	var naoDocM 	= document.getElementById("Nnaodocente[<?=TURNO_MATUTINO;?>]").value;
	
	///////////////// Valores VESPERTINO ////////////////////
	
	var pre1V 		= document.getElementById("aluno[<?=TURNO_VESPERTINO;?>][S][A][<?=PRE;?>]").value;
	var pre2V 		= document.getElementById("aluno[<?=TURNO_VESPERTINO;?>][S][D][<?=PRE;?>]").value;
	var funda1V 	= document.getElementById("aluno[<?=TURNO_VESPERTINO;?>][S][A][<?=FUNDAMENTAL1A4SERIE;?>]").value;
	var funda2V 	= document.getElementById("aluno[<?=TURNO_VESPERTINO;?>][S][D][<?=FUNDAMENTAL1A4SERIE;?>]").value;
	var funda3V 	= document.getElementById("aluno[<?=TURNO_VESPERTINO;?>][S][A][<?=FUNDAMENTAL5A8SERIE;?>]").value;
	var funda4V 	= document.getElementById("aluno[<?=TURNO_VESPERTINO;?>][S][D][<?=FUNDAMENTAL5A8SERIE;?>]").value;
	var jovem1V 	= document.getElementById("aluno[<?=TURNO_VESPERTINO;?>][S][A][<?=ENSINODEJOVENS1A4SERIE;?>]").value;
	var jovem2V 	= document.getElementById("aluno[<?=TURNO_VESPERTINO;?>][S][D][<?=ENSINODEJOVENS1A4SERIE;?>]").value;
	var medio1V 	= document.getElementById("aluno[<?=TURNO_VESPERTINO;?>][S][A][<?=ENSINOMEDIO1A3;?>]").value;
	var medio2V 	= document.getElementById("aluno[<?=TURNO_VESPERTINO;?>][S][D][<?=ENSINOMEDIO1A3;?>]").value;
	var ciclo1V 	= document.getElementById("aluno[<?=TURNO_VESPERTINO;?>][C][A][<?=CICLO1;?>]").value;
	var ciclo2V 	= document.getElementById("aluno[<?=TURNO_VESPERTINO;?>][C][D][<?=CICLO1;?>]").value;
	var ciclo3V 	= document.getElementById("aluno[<?=TURNO_VESPERTINO;?>][C][A][<?=CICLO2;?>]").value;
	var ciclo4V 	= document.getElementById("aluno[<?=TURNO_VESPERTINO;?>][C][D][<?=CICLO2;?>]").value;
	var ciclo5V 	= document.getElementById("aluno[<?=TURNO_VESPERTINO;?>][C][A][<?=CICLO3;?>]").value;
	var ciclo6V 	= document.getElementById("aluno[<?=TURNO_VESPERTINO;?>][C][D][<?=CICLO3;?>]").value;
	var naoDocV 	= document.getElementById("Nnaodocente[<?=TURNO_VESPERTINO;?>]").value;
	
	///////////////// Valores Noturno ////////////////////
	
	var pre1N 		= document.getElementById("aluno[<?=TURNO_NOTURNO;?>][S][A][<?=PRE;?>]").value;
	var pre2N 		= document.getElementById("aluno[<?=TURNO_NOTURNO;?>][S][D][<?=PRE;?>]").value;
	var funda1N 	= document.getElementById("aluno[<?=TURNO_NOTURNO;?>][S][A][<?=FUNDAMENTAL1A4SERIE;?>]").value;
	var funda2N 	= document.getElementById("aluno[<?=TURNO_NOTURNO;?>][S][D][<?=FUNDAMENTAL1A4SERIE;?>]").value;
	var funda3N 	= document.getElementById("aluno[<?=TURNO_NOTURNO;?>][S][A][<?=FUNDAMENTAL5A8SERIE;?>]").value;
	var funda4N 	= document.getElementById("aluno[<?=TURNO_NOTURNO;?>][S][D][<?=FUNDAMENTAL5A8SERIE;?>]").value;
	var jovem1N 	= document.getElementById("aluno[<?=TURNO_NOTURNO;?>][S][A][<?=ENSINODEJOVENS1A4SERIE;?>]").value;
	var jovem2N 	= document.getElementById("aluno[<?=TURNO_NOTURNO;?>][S][D][<?=ENSINODEJOVENS1A4SERIE;?>]").value;
	var medio1N 	= document.getElementById("aluno[<?=TURNO_NOTURNO;?>][S][A][<?=ENSINOMEDIO1A3;?>]").value;
	var medio2N 	= document.getElementById("aluno[<?=TURNO_NOTURNO;?>][S][D][<?=ENSINOMEDIO1A3;?>]").value;
	var ciclo1N 	= document.getElementById("aluno[<?=TURNO_NOTURNO;?>][C][A][<?=CICLO1;?>]").value;
	var ciclo2N 	= document.getElementById("aluno[<?=TURNO_NOTURNO;?>][C][D][<?=CICLO1;?>]").value;
	var ciclo3N 	= document.getElementById("aluno[<?=TURNO_NOTURNO;?>][C][A][<?=CICLO2;?>]").value;
	var ciclo4N 	= document.getElementById("aluno[<?=TURNO_NOTURNO;?>][C][D][<?=CICLO2;?>]").value;
	var ciclo5N 	= document.getElementById("aluno[<?=TURNO_NOTURNO;?>][C][A][<?=CICLO3;?>]").value;
	var ciclo6N 	= document.getElementById("aluno[<?=TURNO_NOTURNO;?>][C][D][<?=CICLO3;?>]").value;
	var naoDocN 	= document.getElementById("Nnaodocente[<?=TURNO_NOTURNO;?>]").value;
	
	///////////////// VERIFICA SE O CAMPO ESTÁ EM BRANCO /////////////
	pre1M = (pre1M == '' ? 0 : pre1M);
	pre1V = (pre1V == '' ? 0 : pre1V);
	pre1N = (pre1N == '' ? 0 : pre1N);
	pre2M = (pre2M == '' ? 0 : pre2M);
	pre2V = (pre2V == '' ? 0 : pre2V);
	pre2N = (pre2N == '' ? 0 : pre2N);
	funda1M = (funda1M == '' ? 0 : funda1M);
	funda1V = (funda1V == '' ? 0 : funda1V);
	funda1N = (funda1N == '' ? 0 : funda1N);
	funda2M = (funda2M == '' ? 0 : funda2M);
	funda2V = (funda2V == '' ? 0 : funda2V);
	funda2N = (funda2N == '' ? 0 : funda2N);
	funda3M = (funda3M == '' ? 0 : funda3M);
	funda3V = (funda3V == '' ? 0 : funda3V);
	funda3N = (funda3N == '' ? 0 : funda3N);
	funda4M = (funda4M == '' ? 0 : funda4M);
	funda4V = (funda4V == '' ? 0 : funda4V);
	funda4N = (funda4N == '' ? 0 : funda4N);
	jovem1M = (jovem1M == '' ? 0 : jovem1M);
	jovem1V = (jovem1V == '' ? 0 : jovem1V);
	jovem1N = (jovem1N == '' ? 0 : jovem1N);
	jovem2M = (jovem2M == '' ? 0 : jovem2M);
	jovem2V = (jovem2V == '' ? 0 : jovem2V);
	jovem2N = (jovem2N == '' ? 0 : jovem2N);
	medio1M = (medio1M == '' ? 0 : medio1M);
	medio1V = (medio1V == '' ? 0 : medio1V);
	medio1N = (medio1N == '' ? 0 : medio1N);
	medio2M = (medio2M == '' ? 0 : medio2M);
	medio2V = (medio2V == '' ? 0 : medio2V);
	medio2N = (medio2N == '' ? 0 : medio2N);
	ciclo1M = (ciclo1M == '' ? 0 : ciclo1M);
	ciclo1V = (ciclo1V == '' ? 0 : ciclo1V);
	ciclo1N = (ciclo1N == '' ? 0 : ciclo1N);
	ciclo2M = (ciclo2M == '' ? 0 : ciclo2M);
	ciclo2V = (ciclo2V == '' ? 0 : ciclo2V);
	ciclo2N = (ciclo2N == '' ? 0 : ciclo2N);
	ciclo3M = (ciclo3M == '' ? 0 : ciclo3M);
	ciclo3V = (ciclo3V == '' ? 0 : ciclo3V);
	ciclo3N = (ciclo3N == '' ? 0 : ciclo3N);
	ciclo4M = (ciclo4M == '' ? 0 : ciclo4M);
	ciclo4V = (ciclo4V == '' ? 0 : ciclo4V);
	ciclo4N = (ciclo4N == '' ? 0 : ciclo4N);
	ciclo5M = (ciclo5M == '' ? 0 : ciclo5M);
	ciclo5V = (ciclo5V == '' ? 0 : ciclo5V);
	ciclo5N = (ciclo5N == '' ? 0 : ciclo5N);
	ciclo6M = (ciclo6M == '' ? 0 : ciclo6M);
	ciclo6V = (ciclo6V == '' ? 0 : ciclo6V);
	ciclo6N = (ciclo6N == '' ? 0 : ciclo6N);
	naoDocM = (naoDocM == '' ? 0 : naoDocM);
	naoDocV = (naoDocV == '' ? 0 : naoDocV);
	naoDocN = (naoDocN == '' ? 0 : naoDocN);
	
	///////////////// TOTAL Matutino ////////////////////////
	
	var totalPreM 		= (Math.round(pre1M / (pre2M == '' ? 1 : pre2M)*10))/10 ; 
	var totalFundM 		= (Math.round(funda1M / (funda2M == '' ? 1 : funda2M)*10))/10;
	var totalFund2M 	= (Math.round(funda3M / (funda4M == '' ? 1 : funda4M)*10))/10;
	var totalCli1M 		= (Math.round(ciclo1M / (ciclo2M == '' ? 1 : ciclo2M)*10))/10;
	var totalCli2M 		= (Math.round(ciclo3M / (ciclo4M == '' ? 1 : ciclo4M)*10))/10;
	var totalCli3M 		= (Math.round(ciclo5M / (ciclo6M == '' ? 1 : ciclo6M)*10))/10;
	var totalJovemM 	= (Math.round(jovem1M / (jovem2M == '' ? 1 : jovem2M)*10))/10;
	var totalMedioM 	= (Math.round(medio1M / (medio2M == '' ? 1 : medio2M)*10))/10;
	var somaAlunosM 	= parseInt(pre1M) + parseInt(funda1M) + parseInt(funda3M) + parseInt(ciclo1M) + parseInt(ciclo3M) + parseInt(ciclo5M) + parseInt(jovem1M) + parseInt(medio1M);
	if(naoDocM == 0){naoDocM = 0.1;}
	var totalNaoDocM 	= (Math.round((somaAlunosM / naoDocM)*10))/10;
	
	
	document.getElementById("totalSerieMatPre").value 		= totalPreM;
	document.getElementById("totalSerieMatF1A4").value 		= totalFundM;
	document.getElementById("totalSerieMatF5A8").value 		= totalFund2M;
	document.getElementById("totalSerieMatC1").value 		= totalCli1M;
	document.getElementById("totalSerieMatC2").value 		= totalCli2M;
	document.getElementById("totalSerieMatC3").value 		= totalCli3M;
	document.getElementById("totalSerieMatJ1A4").value 		= totalJovemM;
	document.getElementById("totalSerieMatM1A3").value 		= totalMedioM;
	document.getElementById("totalSerieMatNdocente").value 	= totalNaoDocM;
	//naoDocM = 0;
	
	///////////////// TOTAL Vespertino ////////////////////////
	var totalPreV 		= (Math.round(pre1V / (pre2V == '' ? 1 : pre2V)*10))/10;
	var totalFundV 		= (Math.round(funda1V / (funda2V == '' ? 1 : funda2V)*10))/10;
	var totalFund2V 	= (Math.round(funda3V / (funda4V == '' ? 1 : funda4V)*10))/10;
	var totalCli1V 		= (Math.round(ciclo1V / (ciclo2V == '' ? 1 : ciclo2V)*10))/10;
	var totalCli2V 		= (Math.round(ciclo3V / (ciclo4V == '' ? 1 : ciclo4V)*10))/10;
	var totalCli3V 		= (Math.round(ciclo5V / (ciclo6V == '' ? 1 : ciclo6V)*10))/10;
	var totalJovemV 	= (Math.round(jovem1V / (jovem2V == '' ? 1 : jovem2V)*10))/10;
	var totalMedioV 	= (Math.round(medio1V / (medio2V == '' ? 1 : medio2V)*10))/10;
	var somaAlunosV 	= parseInt(pre1V) + parseInt(funda1V) + parseInt(funda3V) + parseInt(ciclo1V) + parseInt(ciclo3V) + parseInt(ciclo5V) + parseInt(jovem1V) + parseInt(medio1V);
	if(naoDocV == 0){naoDocV = 0.1;}
	var totalNaoDocV 	= (Math.round((somaAlunosV / naoDocV)*10))/10;
	
	
	document.getElementById("totalSerieVesPre").value 		= totalPreV;
	document.getElementById("totalSerieVesF1a4").value 		= totalFundV;
	document.getElementById("totalSerieVesF5a8").value 		= totalFund2V;
	document.getElementById("totalSerieVesC1").value 		= totalCli1V;
	document.getElementById("totalSerieVesC2").value 		= totalCli2V;
	document.getElementById("totalSerieVesC3").value 		= totalCli3V;
	document.getElementById("totalSerieVesJ1a4").value 		= totalJovemV;
	document.getElementById("totalSerieVesM1a3").value 		= totalMedioV;
	document.getElementById("totalSerieVesNdocente").value 	= totalNaoDocV;
	//naoDocV = 0;
	
	///////////////// TOTAL Noturno ////////////////////////
	
	var totalPreN 		= (Math.round(pre1N / (pre2N == '' ? 1 : pre2N)*10))/10;
	var totalFundN 		= (Math.round(funda1N / (funda2N == '' ? 1 : funda2N)*10))/10;
	var totalFund2N 	= (Math.round(funda3N / (funda4N == '' ? 1 : funda4N)*10))/10;
	var totalCli1N 		= (Math.round(ciclo1N / (ciclo2N == '' ? 1 : ciclo2N)*10))/10;
	var totalCli2N 		= (Math.round(ciclo3N / (ciclo4N == '' ? 1 : ciclo4N)*10))/10;
	var totalCli3N 		= (Math.round(ciclo5N / (ciclo6N == '' ? 1 : ciclo6N)*10))/10;
	var totalJovemN 	= (Math.round(jovem1N / (jovem2N == '' ? 1 : jovem2N)*10))/10;
	var totalMedioN 	= (Math.round(medio1N / (medio2N == '' ? 1 : medio2N)*10))/10;
	var somaAlunosN 	= parseInt(pre1N) + parseInt(funda1N) + parseInt(funda3N) + parseInt(ciclo1N) + parseInt(ciclo3N) + parseInt(ciclo5N) + parseInt(jovem1N) + parseInt(medio1N);
	if(naoDocN == 0){naoDocN = 0.1;}
	var totalNaoDocN 	= (Math.round(((somaAlunosN) / (naoDocN))*10))/10;

	document.getElementById("totalSerieNotPre").value 		= totalPreN;
	document.getElementById("totalSerieNotF1a4").value 		= totalFundN;
	document.getElementById("totalSerieNotF5a8").value 		= totalFund2N;
	document.getElementById("totalSerieNotC1").value 		= totalCli1N;
	document.getElementById("totalSerieNotC2").value 		= totalCli2N;
	document.getElementById("totalSerieNotC3").value 		= totalCli3N;
	document.getElementById("totalSerieNotJ1a4").value 		= totalJovemN;
	document.getElementById("totalSerieNotM1a3").value 		= totalMedioN;
	document.getElementById("totalSerieNotNdocente").value 	= totalNaoDocN;
	//naoDocN = 0;
	
	///////////////// TOTAL RODAPE ////////////////////////
	totalPre 	= parseInt(pre1M) + parseInt(pre1V) + parseInt(pre1N);
	total1a4 	= parseInt(funda1M) + parseInt(funda1V) + parseInt(funda1N);
	total5a8 	= parseInt(funda3M) + parseInt(funda3V) + parseInt(funda3N);
	totalCiclo1 = parseInt(ciclo1M) + parseInt(ciclo1V) + parseInt(ciclo1N);
	totalCiclo2 = parseInt(ciclo3M) + parseInt(ciclo3V) + parseInt(ciclo3N);
	totalCiclo3 = parseInt(ciclo5M) + parseInt(ciclo5V) + parseInt(ciclo5N);
	totalJovem1a4 = parseInt(jovem1M) + parseInt(jovem1V) + parseInt(jovem1N);
	totalMedio1a3 = parseInt(medio1M) + parseInt(medio1V) + parseInt(medio1N);
	
	totalPreD 	= parseInt(pre2M) + parseInt(pre2V) + parseInt(pre2N);
	total1a4D 	= parseInt(funda2M) + parseInt(funda2V) + parseInt(funda2N);
	total5a8D 	= parseInt(funda4M) + parseInt(funda4V) + parseInt(funda4N);
	totalCiclo1D = parseInt(ciclo2M) + parseInt(ciclo2V) + parseInt(ciclo2N);
	totalCiclo2D = parseInt(ciclo4M) + parseInt(ciclo4V) + parseInt(ciclo4N);
	totalCiclo3D = parseInt(ciclo6M) + parseInt(ciclo6V) + parseInt(ciclo6N);
	totalJovem1a4D = parseInt(jovem2M) + parseInt(jovem2V) + parseInt(jovem2N);
	totalMedio1a3D = parseInt(medio2M) + parseInt(medio2V) + parseInt(medio2N);
	totalNaoDocente = parseInt(naoDocM) + parseInt(naoDocV) + parseInt(naoDocN);


	var totalPreT = (Number( totalPreN + totalPreM + totalPreV )).toFixed(1);
	var totalInicT = (Number( totalFundM + totalFundV + totalFundN )).toFixed(1);
	var totalFinT = (Number( totalFund2M + totalFund2V + totalFund2N )).toFixed(1);
	var totalCiclo1T = (Number( totalCli1M + totalCli1V + totalCli1N )).toFixed(1);
	var totalCiclo2T = (Number( totalCli2M + totalCli2V + totalCli2N )).toFixed(1);
	var totalCiclo3T = (Number( totalCli3M + totalCli3V + totalCli3N )).toFixed(1);
	var totalEtapa1a4T = (Number( totalJovemM + totalJovemV + totalJovemN )).toFixed(1);
	var totalSerie1a3T = (Number( totalMedioM + totalMedioV + totalMedioN )).toFixed(1);
	var totalSomaAlunosT = (Number( totalNaoDocM + totalNaoDocV + totalNaoDocN )).toFixed(1);

	
	document.getElementById("totalCol1").value = totalPre;
	document.getElementById("totalCol2").value = total1a4;
	document.getElementById("totalCol3").value = total5a8;
	document.getElementById("totalCol4").value = totalCiclo1;
	document.getElementById("totalCol5").value = totalCiclo2;
	document.getElementById("totalCol6").value = totalCiclo3;
	document.getElementById("totalCol7").value = totalJovem1a4;
	document.getElementById("totalCol8").value = totalMedio1a3;
	
	document.getElementById("totalCol9").value = totalPreD;
	document.getElementById("totalCol10").value = total1a4D;
	document.getElementById("totalCol11").value = total5a8D;
	document.getElementById("totalCol12").value = totalCiclo1D;
	document.getElementById("totalCol13").value = totalCiclo2D;
	document.getElementById("totalCol14").value = totalCiclo3D;
	document.getElementById("totalCol15").value = totalJovem1a4D;
	document.getElementById("totalCol16").value = totalMedio1a3D;
	document.getElementById("totalCol17").value = totalNaoDocente;
	document.getElementById("totalCol18").value = totalPreT;
	document.getElementById("totalCol19").value = totalInicT;
	document.getElementById("totalCol20").value = totalFinT;
	document.getElementById("totalCol21").value = totalCiclo1T;
	document.getElementById("totalCol22").value = totalCiclo2T;
	document.getElementById("totalCol23").value = totalCiclo3T;
	document.getElementById("totalCol24").value = totalEtapa1a4T;
	document.getElementById("totalCol25").value = totalSerie1a3T;
    document.getElementById("totalCol26").value = totalSomaAlunosT;	
	
}

totalizar();
</script>