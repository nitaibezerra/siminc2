<?php
header('Content-Type: text/html; charset=iso-8859-1');

//teste
include_once "config.inc";
include_once APPRAIZ . 'includes/classes_simec.inc';
include_once APPRAIZ . 'includes/funcoes.inc';
include_once APPRAIZ . 'includes/workflow.php';

include_once APPRAIZ . 'www/projovemurbano/_constantes.php';
require_once APPRAIZ . 'www/projovemurbano/_componentes.php';
require_once APPRAIZ . 'www/projovemurbano/_funcoes.php';

include_once APPRAIZ . 'projovemurbano/classes/ProjovemUrbanoPerfil.php';
include_once APPRAIZ . 'projovemurbano/classes/ProjovemUrbanoListaEncaminhamento.php';
include_once APPRAIZ . 'projovemurbano/classes/ProjovemUrbanoEstadoWorkflow.php';

// Cria instância do banco
$db = new cls_banco();

function fechaDb()
{
    global $db;
    $db->close();
}

register_shutdown_function('fechaDb');

// Retorna Perfils
$objPerfis 	= new ProjovemUrbanoPerfil();
$objPerfis->setPerfis( pegaPerfilGeral() );
$perfis		= $objPerfis->getPerfis( );

// Evita erros de Sessão Expirada
if( !empty($_SESSION['projovemurbano']['pjuid']) ){

	$polomunicipio  = buscaPolos( $_SESSION['projovemurbano']['pjuid'] );
	
	$acao   = $_REQUEST['acao'];
	$nucId  = (int) $_REQUEST['nucid'];
	$turId  = (int) $_REQUEST['turid'];
	$cocId  = (int) $_REQUEST['cocid'];
	$ppuId  = (int) $_REQUEST['ppuid'];
	$difId  = (int) $_REQUEST['difid'];
	$perId  = (int) $_REQUEST['perid'];
	
	switch ( $acao ) {
		case 'listarTurma':

			buscarTurmasComAlunos(array('nucid' => $nucId));
			break;

		case 'listarPeriodos':

			buscarPeriodoDiario(array('turid' => $turId));
			break;

		case 'salvarDiarioFrequenciaMensal':

			$retorno = array();
// 	        ver($_REQUEST);
			try {
				salvarDiarioFrequenciaMensal($_REQUEST);

				$retorno = array(
					'status' => true,
					'retorno' => 'Dados alterados com sucesso'
				);

			} catch (Exception $e) {
				$retorno = array(
					'status' => false,
					'retorno' => utf8_encode('Não foi possível salvar atualizar o diário de frequência')
				);
			}

			echo simec_json_encode($retorno);

			break;

		case 'listaAgencias':
			//listaAgencias vincularAgenciaNucleo
			header('Content-Type: application/json; charset=UTF-8');
			$agencias = listaAgencias($_REQUEST);
			echo simec_json_encode($agencias);
			break;

		case 'gerarDiarioPeriodo':

			try {

				$sqlPeriodoAtual = "SELECT per.perid, per.perdtinicio, per.perdesc
	                              FROM projovemurbano.periodocurso per
	                                WHERE perid =  {$perId} ";

				$dadosPeriodo = $db->pegaLinha($sqlPeriodoAtual);


				//  $dadosPeriodoGrade = $db->pegaLinha($sqlPeriodoAtual);


				//utilizar o dado do periodo nessa
				$sqlDiarioPeriodoAnterior = "SELECT dia.diaid, per.perdesc
	                                        FROM projovemurbano.diario dia
	                                        INNER JOIN projovemurbano.periodocurso  per
	                                            ON dia.perid = per.perid
	                                        INNER JOIN workflow.documento doc
	                                            ON dia.docid = doc.docid
	                                        WHERE per.perdtfim < '{$dadosPeriodo['perdtinicio']}'	
	                                        AND dia.turid = {$turId} 
	                                        AND doc.esdid = " . WF_ESTADO_DIARIO_ABERTO;


				$diarioAbertoPeriodoAnterior = $db->pegaLinha($sqlDiarioPeriodoAnterior);

				//            if( $diarioAbertoPeriodoAnterior != false )
				//            {
				//                echo '<p class="error">Para emissão do próximo diário é necessário encerrar o diário do '. $diarioAbertoPeriodoAnterior['perdesc']. '</p>';
				//                exit;
				//            }

				$sqlDiarioGerado = "SELECT per.perid, per.perdesc
	                                            , MAX( per.perdtinicio ) as diario_gerado
	                                            , doc.esdid
	                                            , dia.diaid
	                                        FROM projovemurbano.diario dia
	                                        INNER JOIN workflow.documento doc
	                                            ON dia.docid = doc.docid                                                                        
	                                        INNER JOIN projovemurbano.periodocurso per
	                                            ON dia.perid = per.perid
	                                        WHERE dia.turid     = {$turId}                                   
	                                        AND per.perid       = {$dadosPeriodo['perid']}
	                                        GROUP BY per.perid, per.perdesc, doc.esdid
	                                                , dia.diaid";
				$diarioGerado = $db->pegaLinha($sqlDiarioGerado);

				if ($diarioGerado == false) {
					$docIdFrequencia = wf_cadastrarDocumento(WORKFLOW_TIPODOCUMENTO_DIARIO, 'Geração do Diário de Frequência');

					$sqlInsereDiario = "INSERT INTO projovemurbano.diario( perid, turid, docid)
	                						VALUES( {$dadosPeriodo['perid']}, {$turId}, {$docIdFrequencia} ) RETURNING diaid";

					$diarioGerado['diaid'] = $db->pegaUm($sqlInsereDiario);

					$sqlComponentesTrabalho = "SELECT grd.grdid, coc.cocid
	                                            FROM projovemurbano.componentecurricular coc
	                                            INNER JOIN projovemurbano.gradecurricular grd
	                                                ON coc.cocid = grd.cocid
	                                            WHERE cocdisciplina = 'T'
	                                            AND grdstatus = 'A'
	                                            AND cocstatus = 'A'";

					$arrComponentesTrab = $db->carregar($sqlComponentesTrabalho);

					foreach ($arrComponentesTrab as $componenteTrab) {
						$sqlInsereComponenteTrabalho = "INSERT INTO projovemurbano.diariofrequencia( diaid, grdid ) ";
						$sqlInsereComponenteTrabalho .= "VALUES ( {$diarioGerado['diaid']}, {$componenteTrab['grdid']} ) returning difid";
						$difId = $db->pegaUm($sqlInsereComponenteTrabalho);

					}
				}

				$parametros = array("perid" => $perId, "turid" => $turId);
				adicionaHistoricoDiario($parametros, 1);
// 	            ver();
// 	            die;
				if ($cocId == 9999) {
					$sql = "select grd.grdid, coc.cocqtdhoras, coc.cocid from projovemurbano.componentecurricular coc
					inner join projovemurbano.gradecurricular grd
					on coc.cocid = grd.cocid 
					where coc.cocstatus='A' and grd.grdstatus='A' and coc.cocdisciplina = 'D' and grd.ppuid =  " . PROJOVEMURBANO_2012;
					$listaComponente = $db->carregar($sql);

					foreach ($listaComponente as $dados) {
						$sqlVerificaDiario = "
	                                select df.difid from projovemurbano.diariofrequencia df
	                                inner join  projovemurbano.gradecurricular pg
	                                on df.grdid = pg.grdid
	                                inner join  projovemurbano.diario pd 
	                                on df.diaid = pd.diaid where pd.turid = {$turId}
	                                and pg.cocid =  {$dados['cocid']} and pd.perid = {$dadosPeriodo['perid']}";

						if (!$db->pegaUm($sqlVerificaDiario)) {

							$sqlInsereDiario = "INSERT INTO projovemurbano.diariofrequencia( diaid, grdid, difqtdaulaprevista)";
							$sqlInsereDiario .= "VALUES( {$diarioGerado['diaid']}, {$dados['grdid']}, {$dados['cocqtdhoras']} ) RETURNING difid";
							$_REQUEST['difid'] = $db->pegaUm($sqlInsereDiario);
						}

					}
					$db->commit();
					echo '<p>Os diários de frequência foram gerados com sucesso ';
					exit;
				} else {
					$sqlGradeComponente = "SELECT   grd.grdid, coc.cocqtdhoras
	                                FROM projovemurbano.gradecurricular grd
	                                INNER JOIN projovemurbano.componentecurricular coc
	                                    ON grd.cocid = coc.cocid
	                                WHERE grd.cocid = {$cocId}";
					$dadosGrade = $db->pegaLinha($sqlGradeComponente);

					$sqlDiarioAtual = "SELECT dia.diaid
	                                    FROM projovemurbano.diario dia
	                                    INNER JOIN projovemurbano.diariofrequencia dif
	                                        ON dia.diaid = dif.diaid
	                                    INNER JOIN projovemurbano.gradecurricular grd
	                                        ON dif.grdid = grd.grdid
	                                    WHERE dia.turid = {$turId}
	                                    AND grd.cocid = {$cocId}
	                                    AND dia.perid = {$dadosPeriodo['perid']}";

					$diarioAtual = $db->pegaUm(($sqlDiarioAtual));


					if ($diarioAtual != false) {
						echo '<p class="error">Para este componente curricular o diário do ' . $dadosPeriodo['perdesc'] . ' já foi gerado.</p>';
						exit;
					}


					$sqlInsereDiario = "INSERT INTO projovemurbano.diariofrequencia( diaid, grdid, difqtdaulaprevista)";
					$sqlInsereDiario .= "VALUES( {$diarioGerado['diaid']}, {$dadosGrade['grdid']}, {$dadosGrade['cocqtdhoras']} ) RETURNING difid";

					$_REQUEST['difid'] = $db->pegaUm($sqlInsereDiario);
				}

				$db->commit();
				include APPRAIZ . "projovemurbano/modulos/principal/grid_ciclo.php";

			} catch (Exception $e) {
				echo $e->getMessage();
			}


			break;


		case 'salvarDiarioTrabalho':

			try {
				salvarDiarioTrabalho($_POST['trabalho']);
				$db->commit();

				$parametros = array("perid" => $perId, "turid" => $turId);
// 	            adicionaHistoricoDiario( $parametros, 2 );

				echo 'Trabalhos salvo com sucesso';
			} catch (Exception $e) {
				$db->rollback();
				echo 'Não foi possível salvar o diário de trabalho';
			}

			break;
		case 'fecharDiario':

			$retorno = array();

			try {
				$sqlagencia = "SELECT DISTINCT
	            					agbcod
	            				FROM
	            					projovemurbano.nucleoagenciabancaria agb
	            				INNER JOIN projovemurbano.turma tur ON tur.nucid = agb.nucid
	            				INNER JOIN projovemurbano.diario dia ON dia.turid = tur.turid
	            				WHERE
	            					diaid = {$_POST['diaid']}
	            				AND agb.nabstatus = 'A'";

				$agencia = $db->pegaUm($sqlagencia);

				if (empty($agencia)) {
					throw new Exception('Erro ao fechar o diário. É necessário vincular uma agência a esse núcleo');
				}

				salvarDiarioTrabalho($_POST['trabalho']);

				$diaId = (int)$_POST['diaid'];

				if (empty($diaId)) {
					throw new Exception('Erro ao fechar o diário. Não foi possível localizar o diário');
				}

				$sqlDadosDiario = "SELECT docid
	                                FROM projovemurbano.diario dia
	                                WHERE dia.diaid = {$diaId}";

				$docId = $db->pegaUm($sqlDadosDiario);
				$acao = wf_pegarAcao(WF_ESTADO_DIARIO_ABERTO, WF_ESTADO_DIARIO_FECHADO);

				adicionaHistoricoDiarioById($diaId, 4);
				if (!wf_alterarEstado($docId, $acao['aedid'], '', array('diaid' => $diaId))) {
					throw new Exception('Não foi possível fechar o diário.');
				}


				$retorno = array(
					'status' => true,
					'retorno' => utf8_encode('Diário fechado com sucesso')
				);


			} catch (Exception $e) {
				$db->rollback();
				$retorno = array(
					'status' => false,
					'retorno' => utf8_encode($e->getMessage())
				);

			}

			echo simec_json_encode($retorno);

			break;
		case 'visualizarDiarioPeriodo':
			include APPRAIZ . "projovemurbano/modulos/principal/grid_ciclo.php";
			break;

		case 'visualizarDiario':
			include APPRAIZ . "projovemurbano/modulos/principal/diario_frequencia_componente_curricular.php";
			break;

		case 'visualizarDiarioTrabalho':
			include APPRAIZ . "projovemurbano/modulos/principal/diario_trabalho_componente_curricular.php";
			break;

		case 'visualizarDiarioFrequenciaMensal':
			include APPRAIZ . "projovemurbano/modulos/principal/diario_frequencia_mensal.php";
			break;

		case 'visualizarDiarioTrabalhoMensal':
			include APPRAIZ . "projovemurbano/modulos/principal/diario_trabalho_mensal.php";
			break;

		case 'visualizarDiarioLancamentoNotas':
			include APPRAIZ . "projovemurbano/modulos/principal/diario_lancamento_notas.php";
			break;

		case 'gerenciarListaDeEncaminhamento':
			$perid = $_REQUEST['perid'];

			include APPRAIZ . "projovemurbano/modulos/principal/gerenciarListaDeEncaminhamento.php";
			break;

		case 'retornaDtUltimoPeriodo':

			$retorno = array('status' => true);

			if ($_SESSION['projovemurbano']['ppuid']=='3') {
				$sql = "SELECT distinct
						ordem
					FROM
							projovemurbano.rangeperiodo rap
					INNER JOIN projovemurbano.projovemurbano pju ON pju.rapid = rap.rapid
					WHERE
						pju.pjuid = {$_SESSION['projovemurbano']['pjuid']}";

				$ordem = $db->pegaUm($sql);
				if(!$ordem)
				{
					$retorno = array(
						'status' => false,
						'retorno'   => utf8_encode( 'Ainda não foi selecionada uma data para o início das aulas.')
					);
				}
			}
			if($_SESSION['projovemurbano']['ppuid']!='3'||($_SESSION['projovemurbano']['ppuid']=='3'&&$ordem)) {
				 $dtUltimoPeriodoReal = $_REQUEST['dtUltimoPeriodo'];
				 $dtUltimoPeriodo = mktime(0, 0, 0, substr($dtUltimoPeriodoReal, 3, 2), substr($dtUltimoPeriodoReal, 0, 2), substr($dtUltimoPeriodoReal, 6, 4));

				 //$qtdDiasSomar       = $_REQUEST['qtdDiasSomar'];
				 //$qtdDiasSomar       = 200; // Até o dia 10/09/12 estava setado como 38

				 #Modificação feita no dia 28/01/2013 a pedido do analista Julio Cesar Batista. Onde o sistema possui uma regra, que os períodos de gerar diário de frequencia
				 #e trabalho ficam abertas do dia 18 de um mês até o dia 17 do outro mês, a partir daí são somados 12 dias úteis para lançar os diários no sistema.
				 $qtdDiasSomar = 19;
				 $hj = mktime(0, 0, 0, date('m'), date('d'), date('Y'));

				 $aposCincoDias = formata_data(somar_dias_uteis($dtUltimoPeriodoReal, $qtdDiasSomar));
				 $aposCincoDias = mktime(0, 0, 0, substr($aposCincoDias, 3, 2), substr($aposCincoDias, 0, 2), substr($aposCincoDias, 6, 4));


			}
 	        if( $hj < $dtUltimoPeriodo )
			{
				$retorno = array(
					'status' => false,
					'retorno'   => utf8_encode( 'O diário só pode ser visualizado a partir da sua data de fechamento ')
				);
			}
	        #Modificação feita no dia 09/08/2013 a pedido do analista Julio Cesar Batista. Onde o sistema possui uma regra, que trava a alteração do diário após 5 dias.
	        #Essa regra não existe mais, o diário pode ser modificado a qualquer momento.
// 	        if( $hj > $aposCincoDias ){
// 	            $retorno = array(
// 	                'status'    => false,
// 	                'retorno'   => utf8_encode( 'O período de 5 (cinco) dias úteis para edição desse diário já terminou' ) 
// 	            );
// 	        }
// 	        ver($retorno,d);
	        echo simec_json_encode( $retorno );
	        
	        break;
	
            case 'vincularAgenciaNucleo' :
	
			header('Content-Type: application/json; charset=UTF-8');
	
			$retorno = array();
	
			try {

				$dadosAgencia =  explode( '-', $_REQUEST['agbcod'] );
	
				$agbCod 	= $dadosAgencia[0];
				$dvCod 		= $dadosAgencia[1];
				$noAgencia 	= trim($dadosAgencia[2]);
				$nucId		= trim($_REQUEST['nucid']);
				
						if( empty( $nucId ) )
						{
							throw new Exception( 'Erro ao vincular agência. Não foi possível continuar.' );
						}
		
						$sql = "SELECT 
										nabid, nucid, agbcod 
								FROM 
									projovemurbano.nucleoagenciabancaria
								WHERE 
									nucid = {$nucId}";
		
						$dados = $db->pegaLinha( $sql );
		
						if( $dados == false )
						{
							$sql = "INSERT INTO projovemurbano.nucleoagenciabancaria( nucid, agbcod, agbdv, nabstatus, nabnomeagencia)";
							$sql.= " VALUES ({$nucId}, '{$agbCod}', '{$dvCod}', 'A','{$noAgencia}') RETURNING nabid";
		
						}else{
		
							$sql = "UPDATE projovemurbano.nucleoagenciabancaria";
			                $sql.= " SET nucid  = {$nucId}, agbcod = '{$agbCod}', agbdv='{$dvCod}', nabstatus='A', nabnomeagencia = '{$noAgencia}' 
			                		WHERE nabid = {$dados['nabid']}  RETURNING nabid";
						}
		
						$nabid = $db->pegaUm($sql);
						$db->commit();
		
						$retorno['status']  = true;
						$retorno['retorno'] = utf8_encode('Agência vinculada com sucesso');
				} catch( Exception $e ) {
	
					$retorno['status']  = false;
					$retorno['retorno'] = utf8_encode($e->getMessage());
				}
	
				echo simec_json_encode($retorno);
			
			break;
	
		case 'alterarEstadoDiario':
		
			$retorno 	= array();
	
			$docid   	= (int) $_POST['docid'];
			$polid   	= (int) $_POST['polid'];
			$retorna 	= (int) $_POST['retorna'];
			$envia   	= (int) $_POST['envia'];
			$nucleo_id  = $_POST['nucleo_id'];
	
			try{
				if( empty( $docid ) )
				{
					throw new Exception( 'Erro ao tramitar diário. Não foi possível continuar.' );
				}
	
				// Coordenador
				if( in_array(PFL_COORDENADOR_MUNICIPAL, $perfis) || in_array(PFL_COORDENADOR_ESTADUAL, $perfis) )
				{
	
				// Diretor de Polo
				}elseif(in_array(PFL_DIRETOR_POLO, $perfis))
				{
						
				// Diretor de Núcleo
				}elseif(in_array(PFL_DIRETOR_NUCLEO, $perfis)){
	
					if( empty( $nucleo_id )){
						
						throw new Exception( 'Erro ao tramitar diário. Não foi possível continuar.' );
						
					}else{
	
						foreach ($nucleo_id as $valor ){
							
							$diaid = $nucleo_id;
							
							if($retorna > 0){
									
								$acao = wf_pegarAcao(WF_ESTADO_DIARIO_FECHADO, WF_ESTADO_DIARIO_ABERTO);
	
								if( !wf_alterarEstado( $docid, $acao['aedid'], '', array( 'diaid' => $diaid ) ) )
								{
									throw new Exception( 'Erro ao tramitar diário. Não foi possível continuar.' );
								}
							}elseif($envia > 0){
								if( $polid > 0 ){
			
									$acao = wf_pegarAcao(WF_ESTADO_DIARIO_FECHADO, WF_ESTADO_DIARIO_ENCAMINHAR);
									if( !wf_alterarEstado( $docid, $acao['aedid'], '', array( 'diaid' => $diaid ) ) )
									{
										throw new Exception( 'Erro ao tramitar diário. Não foi possível continuar.' );
									}
									
								}else{
									$acao = wf_pegarAcao(WF_ESTADO_DIARIO_FECHADO, WF_ESTADO_DIARIO_VALIDACAO);
									if( !wf_alterarEstado( $docid, $acao['aedid'], '', array( 'diaid' => $diaid ) ) )
									{
										throw new Exception( 'Erro ao tramitar diário. Não foi possível continuar.' );
									}
								}					
							}
						}
					}
	
				// Equipe MEC
				}elseif(in_array(PFL_EQUIPE_MEC, $perfis)){
		
				}
		
				$retorno = array(
						'status'    => true,
						'retorno'   => 'Dados atualizados com sucesso.'
				);
		
		
			} catch( Exception $e )
			{
				$retorno = array(
						'status'    => false,
						'retorno'   => utf8_encode($e->getMessage())
				);
			}
			echo simec_json_encode( $retorno );
			break;
	
		case 'alterarEstadoDiarioCoordenador':
			
			    $retorno 	= array();
			    $perfis  	= pegaPerfilGeral();
	 
			    $turma_id   = $_REQUEST['turma_id'];
			    $perid      = $_REQUEST['perid'];
	
			    try{
	
			
	                if(in_array(PFL_DIRETOR_NUCLEO, $perfis)){
			
			            if( empty( $turma_id )){
			                	
			                throw new Exception( 'Erro ao tramitar diário. Não foi possível continuar.' );
			                	
			            }else{
			
			                foreach ($turma_id as $valor ){
			                    
			                    $turid = $valor;
			                    $sql   = "SELECT 
			                                  diaid, docid 
			                              FROM projovemurbano.diario 
			                              WHERE 
			                                  turid     = " . $turid . " 
			                                  AND perid = " . $perId;
	
			                    $dados = $db->pegaLinha( $sql );
			                    $docid = $dados['docid'];
			                    $diaid = $dados['diaid'];
		                    
	                            if( $polomunicipio['pmupossuipolo'] == 't' ){
	                                $acao = wf_pegarAcao(WF_ESTADO_DIARIO_FECHADO, WF_ESTADO_DIARIO_ENCAMINHAR);
		                        }else{
	                                $acao = wf_pegarAcao(WF_ESTADO_DIARIO_FECHADO, WF_ESTADO_DIARIO_VALIDACAO);
		                        }
		                        
		                        $estadoAtual = wf_pegarEstadoAtual($docid);
		                         
		                        if( WF_ESTADO_DIARIO_FECHADO == $estadoAtual['esdid'] )
		                        {
		                            $retorno = wf_alterarEstado( $docid, $acao['aedid'], '', array( 'diaid' => $diaid ) );
		                        }
		                        adicionaHistoricoDiarioById( $diaid, 2);
			                }
			                $db->commit();
	                    }
	                
	                // Diretor de Polo
	                }elseif( in_array(PFL_DIRETOR_POLO, $perfis) ){
	                
	                    if( empty( $turma_id )){
	                         
	                        throw new Exception( 'Erro ao tramitar diário. Não foi possível continuar.' );
	                         
	                    }else{
	                    
	                        foreach ($turma_id as $valor ){
	                    
	                            $turid = $valor;
	                            $sql   = "SELECT
	                                        diaid, docid
	                                      FROM projovemurbano.diario
	                                      WHERE
	                                        turid     = " . $turid . "
	                                        AND perid = " . $perId;
	                    
	                            $dados = $db->pegaLinha( $sql );
	                            $docid = $dados['docid'];
	                            $diaid = $dados['diaid'];
	                            
	                            $acao = wf_pegarAcao(WF_ESTADO_DIARIO_ENCAMINHAR, WF_ESTADO_DIARIO_VALIDACAO);
	
	                        	$estadoAtual = wf_pegarEstadoAtual($docid);
		                         
		                        if( WF_ESTADO_DIARIO_ENCAMINHAR == $estadoAtual['esdid'] )
		                        {
		                            $retorno = wf_alterarEstado( $docid, $acao['aedid'], '', array( 'diaid' => $diaid ) );
		                        }
		                         //enviar para o coordenador
		                         adicionaHistoricoDiarioById( $diaid, 3);
	                        }
	                        $db->commit();
	                    }
	
	                // Coordenador Estadual / Municipal
	                }elseif( in_array(PFL_COORDENADOR_ESTADUAL, $perfis) || in_array(PFL_COORDENADOR_MUNICIPAL, $perfis) ){
	                    
	                    if( empty( $turma_id )){
	                         
	                        throw new Exception( 'Erro ao tramitar diário. Não foi possível continuar.' );
	                         
	                    }else{
	                    
	                        foreach ($turma_id as $valor ){
	                    
	                            $turid = $valor;
	                            $sql   = "SELECT
	                                        diaid, docid
	                                    FROM projovemurbano.diario
	                                    WHERE
	                                        turid     = " . $turid . "
	                                        AND perid = " . $perId;
	                    
	                            $dados = $db->pegaLinha( $sql );
	                            $docid = $dados['docid'];
	                            $diaid = $dados['diaid'];
	                            
	                            $estadoAtual = wf_pegarEstadoAtual( $docid );
	                            
	                            if( $polomunicipio['pmupossuipolo'] == 'f' &&  WF_ESTADO_DIARIO_ENCAMINHAR == $estadoAtual['esdid'] ){
	                            
	                            	$acao    = wf_pegarAcao(WF_ESTADO_DIARIO_ENCAMINHAR, WF_ESTADO_DIARIO_VALIDACAO);
		                            $retorno = wf_alterarEstado( $docid, $acao['aedid'], '', array( 'diaid' => $diaid ) );
		                            $db->commit();
	                            }
	                            	
                            	$acao2 = wf_pegarAcao(WF_ESTADO_DIARIO_VALIDACAO, WF_ESTADO_DIARIO_APROVACAO);
                            	
                            	//Pega Estado Atual
                            	$sql = "select
											ed.esdid,
											ed.esddsc
										from workflow.documento d
											inner join workflow.estadodocumento ed on ed.esdid = d.esdid
										where
											d.docid = " . $docid;

								$estadoAtualDoc = $db->pegaLinha( $sql );

		                        if( WF_ESTADO_DIARIO_VALIDACAO == $estadoAtualDoc['esdid'] )
		                        {
		                            $retorno = wf_alterarEstado( $docid, $acao2['aedid'], '', array( 'diaid' => $diaid ) );
		                        }
		                        //enviar para o mec
		                        adicionaHistoricoDiarioById( $diaid, 5);
	                        }

	                        
	                    }
	
	                // Equipe MEc
	                }elseif( in_array(PFL_EQUIPE_MEC, $perfis) ){
	                
	                    if( empty( $turma_id )){
	                         
	                        throw new Exception( 'Erro ao tramitar diário. Não foi possível continuar.' );
	                         
	                    }else{
	                
	                        foreach ($turma_id as $valor ){
	                
	                            $turid = $valor;
	                            $sql   = "SELECT
	                                        diaid, docid
	                                      FROM projovemurbano.diario
	                                      WHERE
	                                        turid     = " . $turid . "
	                                        AND perid = " . $perId;
	                
	                            $dados = $db->pegaLinha( $sql );
	                            $docid = $dados['docid'];
	                            $diaid = $dados['diaid'];
	                             
	                            $acao = wf_pegarAcao(WF_ESTADO_DIARIO_APROVACAO, WF_ESTADO_DIARIO_PAGAMENTO);
	                
	                            $estadoAtual = wf_pegarEstadoAtual($docid);
		                         
		                        if( WF_ESTADO_DIARIO_APROVACAO == $estadoAtual['esdid'] )
		                        {
		                            $retorno = wf_alterarEstado( $docid, $acao['aedid'], '', array( 'diaid' => $diaid ) );
		                            
		                        }
// 		                        adicionaHistoricoDiarioById( $diaid, 5);
	                        }
	                        $db->commit();
	                    }
	                }
	
	                $retorno = array(
	                        'status'    => true,
	                        'retorno'   => 'Dados atualizados com sucesso.'
	                );

			
			    } catch( Exception $e )
			    {
			        $retorno = array(
			                'status'    => false,
			                'retorno'   => utf8_encode($e->getMessage())
			        );
			    }
	
			    echo simec_json_encode( $retorno );
			    break;
	
	    case 'reabrirDiario':
	
	            $retorno        = array();
	            $aedid          = $_REQUEST['aedid'];
	            $docid          = $_REQUEST['docid'];
	            $esdid          = $_REQUEST['esdid'];
	            $dados          = $_REQUEST['dados'];
	            $justificativa  = !empty($_REQUEST['justificativa']) ? utf8_decode( $_REQUEST['justificativa'] ) : '';
	            $fluxo          = $_REQUEST['fluxo'];
	            $diarioid       = $_REQUEST['diarioid'];
	            $concluidos		= array();
	
	            try {
	
	                if(in_array(PFL_DIRETOR_NUCLEO, $perfis) 
	                        || in_array(PFL_DIRETOR_POLO, $perfis)
	                        || in_array(PFL_COORDENADOR_ESTADUAL, $perfis)
	                        || in_array(PFL_COORDENADOR_MUNICIPAL, $perfis)
	                        || in_array(PFL_EQUIPE_MEC, $perfis) ){
	
	                        if( empty( $justificativa ) ){
	                             
	                            throw new Exception( 'Erro ao reabrir diário. Não foi possível continuar.' );
	                             
	                        }else{
	                            // Reabre mais de um diário
	                            if( !empty($fluxo) ){
	
	                                if( in_array(PFL_COORDENADOR_MUNICIPAL, $perfis ) || in_array(PFL_COORDENADOR_ESTADUAL, $perfis) ) {
	
	                                    $concluidos = array( WF_ESTADO_DIARIO_APROVACAO
				                                    		, WF_ESTADO_DIARIO_PAGAMENTO
				                                    		, WF_ESTADO_PAGAMENTO_AUTORIZADO
				                                    		, WF_ESTADO_PAGAMENTO_PENDENTE
				                                    		, WF_ESTADO_PAGAMENTO_ENVIADO);
	                                
	                                }elseif( in_array(PFL_DIRETOR_POLO, $perfis) ) {
	                                
	                                    $concluidos = array( WF_ESTADO_DIARIO_VALIDACAO
				                                    		, WF_ESTADO_DIARIO_APROVACAO
				                                    		, WF_ESTADO_DIARIO_PAGAMENTO
				                                    		, WF_ESTADO_PAGAMENTO_AUTORIZADO
				                                    		, WF_ESTADO_PAGAMENTO_PENDENTE
				                                    		, WF_ESTADO_PAGAMENTO_ENVIADO);
	                                
	                                }elseif( in_array(PFL_DIRETOR_NUCLEO, $perfis) ) {
	                                
	                                    $concluidos = array( WF_ESTADO_DIARIO_ENCAMINHAR
				                                    		, WF_ESTADO_DIARIO_VALIDACAO
				                                    		, WF_ESTADO_DIARIO_APROVACAO
				                                    		, WF_ESTADO_DIARIO_PAGAMENTO
				                                    		, WF_ESTADO_PAGAMENTO_AUTORIZADO
				                                    		, WF_ESTADO_PAGAMENTO_PENDENTE
				                                    		, WF_ESTADO_PAGAMENTO_ENVIADO);
	                                
	                                }elseif( in_array(PFL_EQUIPE_MEC, $perfis) ) {
	                                
	                                    $concluidos = array(  WF_ESTADO_DIARIO_PAGAMENTO
				                                    		, WF_ESTADO_PAGAMENTO_AUTORIZADO
				                                    		, WF_ESTADO_PAGAMENTO_PENDENTE
				                                    		, WF_ESTADO_PAGAMENTO_ENVIADO);
	                                }
	                                
	                                foreach ($diarioid as $valor)
	                                {
	                                    $sql   = "SELECT
	                                                docid
	                                              FROM projovemurbano.diario
	                                              WHERE
	                                                diaid = " . $valor;
	                                    // var_dump( $sql ); exit;
	                                    
	                                    $dados = $db->pegaLinha( $sql );
	                                    $docid = $dados['docid'];
	                                    
	                                    // Pega Estado Atual
	                                    $estadoAtual = wf_pegarEstadoAtual( $docid );
	                                    
	                                    if( $estadoAtual['esdid'] != WF_ESTADO_DIARIO_ABERTO && !in_array( $estadoAtual['esdid'], $concluidos ) )
	                                    {
	                                        $acao = wf_pegarAcao( $estadoAtual['esdid'], WF_ESTADO_DIARIO_ABERTO );
	
	                                        if( !wf_alterarEstado( $docid, $acao['aedid'], $justificativa, array( 'diaid' => $valor ) ) ){
	                                            throw new Exception( 'Erro ao reabrir diário. Não foi possível continuar.' );
	                                        }
	                                        $db->commit();
	                                        
	                                        adicionaHistoricoDiarioById( $valor, 6);
	                                    }                                    
	                                }
	
	                            // Reaber somente um Diário
	                            }else{
	                                //echo $docid ."|". $aedid ."|". $justificativa ."| diaid: ". $dados;
	
	                                if( !wf_alterarEstado( $docid, $aedid, $justificativa, array( 'diaid' => $dados ) ) )
	                                {
	                                    throw new Exception( 'Erro ao reabrir diário. Não foi possível continuar.' );
	                                }
	                            }
	
	                            $retorno = array(
	                                    'status'    => true,
	                                    'retorno'   => 'Dados atualizados com sucesso.'
	                            );
	                        }
	                }else{
	                    
	                    throw new Exception( 'Acesso negado.' );
	                }
	            
	            }catch (Exception $e){
	            
	                $retorno = array(
	                        'status'    => false,
	                        'retorno'   => utf8_encode($e->getMessage())
	                );
	            }
	
	            echo simec_json_encode( $retorno );
	        break;
			    
	    case 'listarHistorico':
		         
		    $docid = $_REQUEST['docid'];
	
	        try{
		        if( empty( $docid )){
		            throw new Exception( 'Erro ao abrir histórico. Não foi possível continuar.' );
	            }else{
	
		            $sql = "SELECT 
	                        	'Núcleo: ' || nuc.nucid   as nucleo
	                        	, tur.turdesc             as turma
	                        	, ac.aeddscrealizada      as acaorealizada
	                        	, ed.esddsc      as estadodocumento
	                        	, us.usunome     as nome
	                        	, hd.htddata     as data
	                        	, cd.cmddsc      as motivo
	                        FROM projovemurbano.diario dia
	                        INNER join projovemurbano.turma tur
	                        	on dia.turid = tur.turid
	                        INNER join projovemurbano.nucleo nuc
	                        	on nuc.nucid = tur.nucid
	                        	INNER join workflow.historicodocumento hd
	                        		on dia.docid = hd.docid
	                        	inner join workflow.acaoestadodoc ac on
	                        		ac.aedid = hd.aedid
	                        	inner join workflow.estadodocumento ed on
	                        		ed.esdid = ac.esdidorigem
	                        	inner join seguranca.usuario us on
	                        		us.usucpf = hd.usucpf
	                        	left join workflow.comentariodocumento cd on
	                        		cd.hstid = hd.hstid
	                        where
	                        	hd.docid = {$docid}
	                        order by
	
	                        	hd.htddata desc, hd.hstid desc";
	
	                $dados = $db->carregar( $sql );
	
	                $retorno = "<table id='tbHistorico' width='100%'>";
	                foreach ( $dados as $valor )
	                {
	                    $dataTime = explode(' ', $valor['data']);
	                    
	                    if( !empty($valor['motivo']) ){
	                        $retorno.= "<tr>";
	                        $retorno.= "    <th>Núcleo</th>";
	                        $retorno.= "    <th>Turma</th>";
	                        $retorno.= "    <th>Ação Realizada</th>";
	                        $retorno.= "    <th>Estado Documento</th>";
	                        $retorno.= "    <th>Usuário</th>";
	                        $retorno.= "    <th>Data</th>";
	                        $retorno.= "</tr>";
	                        
	                        $retorno.= "<tr>";
	                        $retorno.= "    <td style='background-color: #f4f4f4'>".$valor['nucleo']."</td>";
	                        $retorno.= "    <td style='background-color: #f4f4f4'>".$valor['turma']."</td>";
	                        $retorno.= "    <td style='background-color: #f4f4f4'>".$valor['acaorealizada']."</td>";
	                        $retorno.= "    <td style='background-color: #f4f4f4'>".$valor['estadodocumento']."</td>";
	                        $retorno.= "    <td style='background-color: #f4f4f4'>".$valor['nome']."</td>";
	                        $retorno.= "    <td style='background-color: #f4f4f4'>". formata_data($valor['data']) ." ". $dataTime[1] . "</td>";
	                        $retorno.= "</tr>";
	                        
	                        $retorno.= "<tr >";
	                        $retorno.= "    <th colspan='6'>Motivo</th>";
	                        $retorno.= "</tr>";
	                        
	                        $retorno.= "<tr>";
	                        $retorno.= "    <td colspan='6' style='background-color: #f4f4f4'>".$valor['motivo']."</td>";
	                        $retorno.= "</tr>";
	                        $retorno.= "<tr>";
	                        $retorno.= "    <td colspan='6'>&nbsp;</td>";
	                        $retorno.= "</tr>";
	                    }
	                }
	                $retorno.= "</table>";
	            }
	        }catch ( Exception $e ){
		        $retorno = false;
	        }
		    
	        echo( $retorno );
	
		    break;
            case 'salvarLancamentoNotas':
                
                    $sql = '';
                    if(isset($_POST['ciclo']) && count($_POST['ciclo']) > 0){
                        foreach($_POST['ciclo'] as $caeid => $cicclo){
                            if($cicclo['notaciclo1'] || $cicclo['notaciclo2'] || $cicclo['notaciclo3']){
                                
                                $cicclo['notaciclo1'] = ($cicclo['notaciclo1']) ? str_replace(',','.' , $cicclo['notaciclo1']) : 'NULL';
                                $cicclo['notaciclo2'] = ($cicclo['notaciclo2']) ? str_replace(',','.' , $cicclo['notaciclo2']) : 'NULL';
                                $cicclo['notaciclo3'] = ($cicclo['notaciclo3']) ? str_replace(',','.' , $cicclo['notaciclo3']) : 'NULL';
                                
                                $sqlSelect = "SELECT npcid FROM projovemurbano.notasporciclo WHERE caeid = {$caeid} AND turid = {$_POST['turid']} AND ppuid = {$_SESSION['projovemurbano']['ppuid']}";
                                $npcid = $db->pegaUm($sqlSelect);
                                if($npcid){
                                	$sqlteste="SELECT
											npc_status,
                                			caestatus
										FROM
											projovemurbano.cadastroestudante cae
										INNER JOIN  projovemurbano.notasporciclo npc ON npc.caeid = cae.caeid AND npc.turid = cae.turid
										
										WHERE
											cae.caeid = {$caeid} 
                                		AND cae.ppuid = {$_SESSION['projovemurbano']['ppuid']}
                                		AND cae.turid = {$_POST['turid']}";
                                	$testeStatus = $db->pegaLinha($sqlteste);
                                	if($testeStatus['caestatus'] == 'A'&&$testeStatus['npc_status'] == 'I'){
                                		$npcstatus = ",npc_status = 'A'" ;                               	
                                	}
                                    $sql .= "UPDATE projovemurbano.notasporciclo
                                                SET notaciclo1 = {$cicclo['notaciclo1']}, notaciclo2 = {$cicclo['notaciclo2']}, notaciclo3 = {$cicclo['notaciclo3']} $npcstatus
                                              WHERE npcid = {$npcid};";
                                } else {
                                    $sql .= "INSERT INTO projovemurbano.notasporciclo( caeid, turid, notaciclo1, notaciclo2, notaciclo3, ppuid )
                                        VALUES ({$caeid}, {$_POST['turid']}, {$cicclo['notaciclo1']}, {$cicclo['notaciclo2']}, {$cicclo['notaciclo3']}, {$_SESSION['projovemurbano']['ppuid']});";
                                }
                            }
                        }
                    }
                    if($sql){
//                     	ver($sql,d);
                        $db->executar($sql);
                        $db->commit();
                        echo '"Salvo com sucesso!';
                    } else {
                        echo '"Não pode salvar!';
                    }
                
		    break;
	    default:
	        echo 'Ação não existente';
	}
}
exit;
?>