<?php
header('Content-Type: text/html; charset=iso-8859-1');

//teste
include_once "config.inc";
include_once APPRAIZ . 'includes/classes_simec.inc';
include_once APPRAIZ . 'includes/funcoes.inc';
include_once APPRAIZ . 'includes/workflow.php';

include_once APPRAIZ . 'www/projovemcampo/_constantes.php';
require_once APPRAIZ . 'www/projovemcampo/_componentes.php';
require_once APPRAIZ . 'www/projovemcampo/_funcoes.php';

include_once APPRAIZ . 'projovemcampo/classes/ProjovemCampoPerfil.php';
include_once APPRAIZ . 'projovemcampo/classes/ProjovemCampoListaEncaminhamento.php';
include_once APPRAIZ . 'projovemcampo/classes/ProjovemCampoEstadoWorkflow.php';

// Cria instância do banco
$db = new cls_banco();

function fechaDb()
{
    global $db;
    $db->close();
}

register_shutdown_function('fechaDb');

// Retorna Perfils
$objPerfis 	= new projovemcampoPerfil();
$objPerfis->setPerfis( pegaPerfilGeral() );
$perfis		= $objPerfis->getPerfis( );
// Evita erros de Sessão Expirada
if( !empty($_SESSION['projovemcampo']['apcid']) ){
	$arrPerRap = spliti('-',$_REQUEST['perid'],2);
	$acao   = $_REQUEST['acao'];
	$entid  = (int) $_REQUEST['entid'];
	$turId  = (int) $_REQUEST['turid'];
	$diaid  = (int) $_REQUEST['diaid'];
	$perId  = (int) $arrPerRap[0];
	$rapId 	= (int) $arrPerRap[1];
	$gerarDiario = (int) $_REQUEST['gerarDiario'];
	switch ( $acao )
	{
		case 'alteraRange':
			$retorno    = array( );
			try {
				$sql = "UPDATE projovemcampo.diario
						SET
							rapid = {$rapId}
						WHERE
							turid ={$turId}";
				
				$db->executar($sql);
				
				$db->commit();
				
				$retorno = array(
						'status'  => true,
						'retorno' => 'Dados alterados com sucesso.'
				);
				 
			}catch(Exception $e)
			{
				$retorno = array(
						'status'  => false,
						'retorno' => utf8_encode('Não foi possível alterar a data de início do Diário.')
				);
			}
			 
			echo simec_json_encode( $retorno );
			
			break;
		case 'listarTurma':
	        buscarTurmas( array( 'entid' => $entid,'alunos' => 't' ) );
	        break;
	        
		case 'listarPeriodos':
	        
			buscarPeriodoDiario( array( 'turid' => $turId,'gerarDiario'=>$gerarDiario, 'rapid'=>$rapId) );
			break;
			
		case 'salvarDiarioFrequenciaMensal':
	        
	        $retorno    = array( );
	        try {
	            salvarDiarioFrequenciaMensal( array( 'turid' => $turId,'diaid'=>$diaid, 'perid'=>$perId, 'qtdaulas' =>$_REQUEST['qtdaulas'],'qtdaulasdadas'=>$_REQUEST['qtdaulasdadas'])  );
	            
	            $retorno = array( 
	                'status'  => true,
	                'retorno' => 'Dados alterados com sucesso'
	            );
	        }catch(Exception $e)
	        {
	            $retorno = array( 
	                'status'  => false,
	                'retorno' => utf8_encode('Não foi possível salvar atualizar o diário de frequência')
	            );
	        }
	        
	        echo simec_json_encode( $retorno );
	        
			break;
	
		case 'listaAgencias': 
		//listaAgencias vincularAgenciaNucleo 
			header('Content-Type: application/json; charset=UTF-8');
			$agencias = listaAgencias( $_REQUEST );
			
			echo simec_json_encode($agencias);
			break;			 
	
	    case 'gerarDiarioPeriodo':
	    	
	        try {
	        
	         	$sqlPeriodoAtual = "SELECT per.perid                                       
	                              FROM projovemcampo.periodo  per
	                                WHERE perid =  {$perId} ";
	         
	           $dadosPeriodo = $db->pegaLinha($sqlPeriodoAtual);
	           
	           //Verifica se diário já existe
	           $sqlDiarioExiste = "SELECT 
	           							per.perdescricao
						           FROM projovemcampo.diario dia
						           INNER JOIN projovemcampo.periodo per ON dia.perid = per.perid
						           WHERE
						          		dia.perid = {$perId}
						           AND 	turid = {$turId}
	           " ;
	            
	           
	           $DiarioExiste = $db->pegaUm($sqlDiarioExiste);
	           
	           if( $DiarioExiste != false )
	           {
		           echo '<p class="error">Já existe um diário emitido para o '.$DiarioExiste. '</p>';
		           exit;
	           }
	           //utilizar o dado do periodo nessa            
	           $sqlDiarioPeriodoAnterior = "SELECT dia.diaid, per.perdescricao
											FROM projovemcampo.diario dia
											INNER JOIN projovemcampo.periodo per ON dia.perid = per.perid
											INNER JOIN projovemcampo.historico_diario hid ON hid.diaid = dia.diaid AND dia.hidid = hid.hidid
											WHERE 
												hid.stdid in(1,12)
											AND dia.turid = {$turId}
	           								GROUp BY 
	           									dia.diaid, per.perdescricao
											" ;                                       
	                                        
	                                                                   
// 	            ver($sqlDiarioPeriodoAnterior,d);
	            $diarioAbertoPeriodoAnterior = $db->pegaLinha($sqlDiarioPeriodoAnterior);
	           
	           if( $diarioAbertoPeriodoAnterior != false )
	           {
	               echo '<p class="error">Para emissão do próximo diário é necessário enviar o diário do '. $diarioAbertoPeriodoAnterior['perdescricao']. '</p>';
	               exit;
	           }
	            
	            $sqlDiarioGerado    = "SELECT 
	            							per.perid, 
	            							per.perdescricao, 
	            							MAX( rap.datainicio ) as diario_gerado, 
	            							hid.stdid, 
	            							dia.diaid
                                        FROM projovemcampo.diario dia
                                        INNER JOIN projovemcampo.periodo  per ON dia.perid = per.perid
                                        INNER JOIN projovemcampo.historico_diario hid ON hid.diaid = dia.diaid
                                        INNER JOIN projovemcampo.rangeperiodo rap ON rap.rapid = dia.rapid 
                                        WHERE dia.turid     = {$turId}                                   
                                        AND per.perid       = {$dadosPeriodo['perid']}
                                        GROUP BY 
                                        	per.perid, 
                                        	per.perdescricao, 
                                        	dia.diaid,
                                        	hid.stdid";
	            $diarioGerado = $db->pegaLinha($sqlDiarioGerado);
	            
	            if( $diarioGerado == false ){
	                
	
	                $sqlInsereDiario    = "INSERT INTO projovemcampo.diario(
										            perid, turid,rapid)
										    VALUES ({$dadosPeriodo['perid']}, {$turId},{$rapId});
	                ";
	
	                $db->executar($sqlInsereDiario);
	            }
	           
	            $parametros = array( "perid"=>$perId, "turid" => $turId, "status" => 1);
	            
	            adicionaHistoricoDiario( $parametros);

	            $db->commit();

	            echo '<p>Os diários de frequência foram gerados com sucesso ';
                exit;
	                          
	
// 	            include APPRAIZ . "projovemcampo/modulos/principal/grid_ciclo.php";
	            
		        } catch (Exception $e){
		            echo $e->getMessage();
		        }
	        
	        
	        break;
	        
	        
	    case 'fecharDiario':
	    	
	        $retorno = 0;

	        try{
				salvarDiarioFrequenciaMensal( array( 'turid' => $turId,'diaid'=>$diaid, 'perid'=>$perId, 'qtdaulas' =>$_REQUEST['qtdaulas'],'qtdaulasdadas'=>$_REQUEST['qtdaulasdadas']) );

				$sqlagencia = "SELECT DISTINCT
	        						agbcod
	        					FROM
					        		projovemcampo.agenciabancariaescola abe
					        	INNER JOIN projovemcampo.turma tur ON tur.entid = abe.entid
					        	INNER JOIN projovemurbano.diario dia ON dia.turid = tur.turid
					        	WHERE
					        		dia.turid = {$turId}
					            AND dia.perid = {$perId}
					        	AND abe.nabstatus = 'A'";
	        	$agencia    = $db->pegaUm($sqlagencia);

	        	if( empty($agencia)||$agencia=='')
	        	{
	        			throw new Exception(4);
	        	}else{

					$sqlDadosDiario = "
										SELECT
											diaid
										FROM projovemcampo.diario dia
										WHERE
											dia.turid = {$turId}
										AND dia.perid = {$perId}";

					$diaId    = $db->pegaUm($sqlDadosDiario);

					if( empty( $diaId ) ){
						throw new Exception(2);
					}else{
							$status = ESTADO_DIARIO_FECHADO;

						$parametros = array( "perid"=>$perId, "turid" => $turId, "status" => $status);

						if( !adicionaHistoricoDiario($parametros)){
							throw new Exception(3);
						}else{
							$db->commit();
							$retorno = 1;
						}

					}
				}
	           
	        } catch( Exception $e )
	        {
	            $db->rollback();
	            $retorno = ($e->getMessage());
	            
	        }
	        ob_clean();
	        echo $retorno;
	        die;
	        
	        break;
	    case 'visualizarDiarioPeriodo':
	        include APPRAIZ . "projovemcampo/modulos/principal/grid_ciclo.php";
	        break;
	    
	    case 'visualizarDiario':
	        include APPRAIZ . "projovemcampo/modulos/principal/monitoramento/diario_frequencia_componente_curricular.php";
	        break;
	        
        case 'visualizarDiarioFrequenciaMensal':
	    	include APPRAIZ . "projovemcampo/modulos/principal/monitoramento/diario_frequencia_mensal.php";
	        break;

		case 'gerenciarListaDeEncaminhamento':
			$perid = $_REQUEST['perid'];

			include APPRAIZ . "projovemcampo/modulos/principal/monitoramento/gerenciarListaDeEncaminhamento.php"; 
	        break;
	    
	    case 'retornaDtUltimoPeriodo':
// 	    	 ver($_REQUEST['dtUltimoPeriodo'],d);
	        $dtUltimoPeriodoReal  = $_REQUEST['dtUltimoPeriodo'];
	        $dtUltimoPeriodo    = mktime(0,0,0,substr($dtUltimoPeriodoReal,3,2), substr($dtUltimoPeriodoReal,0,2), substr($dtUltimoPeriodoReal,6,4) );
	        
	        //$qtdDiasSomar       = $_REQUEST['qtdDiasSomar'];
	        //$qtdDiasSomar       = 200; // Até o dia 10/09/12 estava setado como 38
	        
	        #Modificação feita no dia 28/01/2013 a pedido do analista Julio Cesar Batista. Onde o sistema possui uma regra, que os períodos de gerar diário de frequencia
	        #e trabalho ficam abertas do dia 18 de um mês até o dia 17 do outro mês, a partir daí são somados 12 dias úteis para lançar os diários no sistema.
	        $qtdDiasSomar       = 19;
	        $hj                 = mktime(0,0,0,date('m'), date('d'), date('Y') );

	        $aposCincoDias      = formata_data( somar_dias_uteis( $dtUltimoPeriodoReal, $qtdDiasSomar ) );
	        $aposCincoDias      = mktime(0,0,0,substr($aposCincoDias,3,2), substr($aposCincoDias,0,2), substr($aposCincoDias,6,4) );

	        $retorno = array( 'status' => true );
	        
	        if( $hj < $dtUltimoPeriodo )
	        {
	            $retorno = array(
	                'status' => false,
	                'retorno'   => utf8_encode( 'O diário só pode ser visualizado a partir da sua data de fechamento ') 
	            );
	        }
	        #Modificação feita no dia 09/08/2013 a pedido do analista Julio Cesar Batista. Onde o sistema possui uma regra, que trava a alteração do diário após 5 dias.
	        #Essa regra não existe mais, o diário pode ser modificado a qualquer momento.
//	        if( $hj > $aposCincoDias ){
//	            $retorno = array(
//	                'status'    => false,
//	                'retorno'   => utf8_encode( 'O período de 5 (cinco) dias úteis para edição desse diário já terminou' ) 
//	            );
//	        }
	        
	        echo simec_json_encode( $retorno );
	        
	        break;
	
            case 'vincularAgenciaEscola' :
	
			header('Content-Type: application/json; charset=UTF-8');
	
			$retorno = array();
	
			try {

				$dadosAgencia =  explode( '-', $_REQUEST['agbcod'] );
					
	
				$agbCod 	= $dadosAgencia[0];
				$dvCod 		= $dadosAgencia[1];
				$noAgencia 	= trim($dadosAgencia[2]);
				$entid		= trim($_REQUEST['entid']);
				
						if( empty( $entid ) )
						{
							throw new Exception( 'Erro ao vincular agência. Não foi possível continuar.' );
						}
		
						$sql = "SELECT 
										abeid, entid, agbcod 
								FROM 
									projovemcampo.agenciabancariaescola
								WHERE 
									entid = {$entid}";
		
						$dados = $db->pegaLinha( $sql );
		
						if( $dados == false )
						{
							$sql = "INSERT INTO projovemcampo.agenciabancariaescola( entid, agbcod, agbdv, nabstatus, nabnomeagencia)";
							$sql.= " VALUES ({$entid}, '{$agbCod}', '{$dvCod}', 'A','{$noAgencia}') RETURNING abeid";
		
						}else{
		
							$sql = "UPDATE projovemcampo.agenciabancariaescola";
			                $sql.= " SET entid  = {$entid}, agbcod = '{$agbCod}', agbdv='{$dvCod}', nabstatus='A', nabnomeagencia = '{$noAgencia}' 
			                		WHERE abeid = {$dados['abeid']}  RETURNING abeid";
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
	
		case 'alterarEstadoDiarioCoordenador':
			    $retorno 	= array();
			    $perfis  	= pegaPerfilGeral();
			    $temcoordturma = testacoordturma();
			    
			    $turma_id   = $_REQUEST['turma_id'];
			    $perid      = $_REQUEST['perid'];
			   
			    try{
	
			
                	if( in_array(PFL_COORDENADOR_TURMA, $perfis) ){
	                
	                    if( empty( $turma_id )){
	                         
	                        throw new Exception( 'Erro ao tramitar diário. Não foi possível continuar.' );
	                         
	                    }else{
	                    
		            		$status = ESTADO_DIARIO_COORDGERAL;
		            	
			            	$parametros = array( "perid"=>$perid, "turid" => $turma_id, "status" => $status);
	            
			            	adicionaHistoricoDiario($parametros);
				            	
	                    }
	
	                // Coordenador Estadual / Municipal
	                }elseif( in_array(PFL_COORDENADOR_ESTADUAL, $perfis) || in_array(PFL_COORDENADOR_MUNICIPAL, $perfis) ){
	                    
	                    if( empty( $turma_id )){
	                         
	                        throw new Exception( 'Erro ao tramitar diário. Não foi possível continuar.' );
	                         
	                    }else{
	                    	
		            		$status = ESTADO_DIARIO_MEC;
		            		
		            		$parametros = array( "perid"=>$perid, "turid" => $turma_id, "status" => $status);
		            		 
		            		adicionaHistoricoDiario($parametros);
		            		 
	                    }
	
	                // Coordenador Estadual / Municipal
	                }elseif( in_array(PFL_DIRETOR_ESCOLA, $perfis)){
	                    
	                    if( empty( $turma_id )){
	                         
	                        throw new Exception( 'Erro ao tramitar diário. Não foi possível continuar.' );
	                         
	                    }else{
// 	                    	if(testacoordturma()){
// 	                    		$status = ESTADO_DIARIO_COORDTURMA;
// 	                    	}else{
	                    		$status = ESTADO_DIARIO_COORDGERAL;
// 	                    	}
		            		
		            		$parametros = array( "perid"=>$perid, "turid" => $turma_id, "status" => $status);
		            		
		            		adicionaHistoricoDiario($parametros);
		            		 
	                    }
	
	                // Equipe MEc
	                }elseif( in_array(PFL_EQUIPE_MEC, $perfis) ||in_array(PFL_ADMINISTRADOR, $perfis) || in_array(PFL_SUPER_USUARIO, $perfis)){
	                
	                    if( empty( $turma_id )){
	                         
	                        throw new Exception( 'Erro ao tramitar diário. Não foi possível continuar.' );
	                         
	                    }else{
	                		$sql = "";
		            		$status = ESTADO_DIARIO_PAGAMENTO_ENVIADO;
		            	
			            	$parametros = array( "perid"=>$perid, "turid" => $turma_id, "status" => $status);
	            
			            	adicionaHistoricoDiario($parametros);
	                        
	                    }
	                }
	                
	                $db->commit();
	                
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
	    	
// 	            $justificativa  = !empty($_REQUEST['justificativa']) ? utf8_decode( $_REQUEST['justificativa'] ) : '';
	            $perid         	= $_REQUEST['perid'];
	            $turid        	= $_REQUEST['turid'];
	            $status		    = $_REQUEST['status'];
	            $entid		    = $_REQUEST['entid'];
	            $concluidos		= array();
	           
	            try {
	            	
	                if(		in_array(PFL_COORDENADOR_ESTADUAL, $perfis)
	                        || in_array(PFL_COORDENADOR_MUNICIPAL, $perfis)
	                        || in_array(PFL_COORDENADOR_TURMA, $perfis)
	                        || in_array(PFL_DIRETOR_ESCOLA, $perfis)
	                        ||in_array(PFL_ADMINISTRADOR, $perfis)
	                        || in_array(PFL_EQUIPE_MEC, $perfis)
	            			|| in_array(PFL_SUPER_USUARIO,$perfis)){
						
	                        if( 1!=1/*empty( $justificativa ) */){
	                             
	                            throw new Exception( 'Erro ao reabrir diário. Não foi possível continuar.' );
	                             
	                        }else{
	                        	
	                            // Reabre mais de um diário
	                            if($entid){
	
	                                if( in_array(PFL_COORDENADOR_MUNICIPAL, $perfis ) || in_array(PFL_COORDENADOR_ESTADUAL, $perfis) ) {
	
// 	                                	if(testacoordturma()){
	                                		
// 						            		$status = ESTADO_DIARIO_DEVOLVIDO_COORDTURMA;
						            		
// 						            	}else{
						            		
						            		$status = ESTADO_DIARIO_DEVOLVIDO_DIRESCOLA;
						            		
// 						            	}
	                                
	                                }elseif( in_array(PFL_COORDENADOR_TURMA, $perfis) ) {
	                                
	                                    $status = ESTADO_DIARIO_DEVOLVIDO_DIRESCOLA;
	                                
	                                }elseif( in_array(PFL_DIRETOR_ESCOLA, $perfis)) {
	                                	
	                                    $status = ESTADO_DIARIO_ABERTO;
	                                    
	                                }elseif( in_array(PFL_EQUIPE_MEC, $perfis)|| in_array(PFL_SUPER_USUARIO, $perfis)||in_array(PFL_ADMINISTRADOR, $perfis)) {
	                                	
	                                    $status = ESTADO_DIARIO_DEVOLVIDO_COORDGERAL;
	                                    
	                                }
	                              
                                    $sql   = "
                                    			SELECT
													turid
												FROM 
													projovemcampo.turma
												WHERE
													entid = $entid
												AND	turstatus = 'A'";
                                    
                                    $turid = $db->carregarColuna( $sql );
	                                    
	                                	
                                	$parametros = array( "perid"=>$perid, "turid" => $turid, "status" => $status);
	                                	
									if(!adicionaHistoricoDiario($parametros)){
                                        	
										throw new Exception( 'Erro ao reabrir diário. Não foi possível continuar.' );
                                            
									}
									
								
	                            // Reaber somente um Diário
	                            }else{
	                            	
	                            	if( in_array(PFL_COORDENADOR_MUNICIPAL, $perfis ) || in_array(PFL_COORDENADOR_ESTADUAL, $perfis) ) {
	                            	
// 	                            		if(testacoordturma()){
	                            			 
// 	                            			$status = ESTADO_DIARIO_DEVOLVIDO_COORDTURMA;
	                            	
// 	                            		}else{
	                            	
	                            			$status = ESTADO_DIARIO_DEVOLVIDO_DIRESCOLA;
	                            	
// 	                            		}
	                            		 
	                            	}elseif( in_array(PFL_COORDENADOR_TURMA, $perfis) ) {
	                            		 
	                            		$status = ESTADO_DIARIO_DEVOLVIDO_DIRESCOLA;
	                            		 
	                            	}elseif( in_array(PFL_DIRETOR_ESCOLA, $perfis)) {
	                            	
	                            		$status = ESTADO_DIARIO_ABERTO;
	                            		 
	                            	}elseif( in_array(PFL_EQUIPE_MEC, $perfis)|| in_array(PFL_SUPER_USUARIO, $perfis)||in_array(PFL_ADMINISTRADOR, $perfis)) {
	                            	
	                            		$status = ESTADO_DIARIO_DEVOLVIDO_COORDGERAL;
	                            		 
	                            	}
	                            	
	                            	$parametros = array( "perid"=>$perid, "turid" => $turid, "status" => $status);
		            
	                                if( !adicionaHistoricoDiario($parametros)){
	                                	
	                                    throw new Exception( 'Erro ao reabrir diário. Não foi possível continuar.' );
	                                    
	                                }
	                            }
	                            
	                            $db->commit();
	                            
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
								'Escola: '|| entnome  as escola, 
								tur.turdescricao             as turma,
								std.stddesc      as estadodocumento,
								us.usunome     as nome,
								hsd.datahora     as data--,
								--cd.cmddsc      as motivo
							FROM projovemcampo.diario dia
							INNER JOIN projovemcampo.turma tur on dia.turid = tur.turid
							INNER JOIN entidade.entidade ent ON ent.entid = tur.entid
							INNER JOIN projovemcampo.historico_diario hsd ON hsd.diaid = dia.diaid
							INNER JOIN projovemcampo.status_diario std ON std.stdid = hsd.stdid
							INNER JOIN seguranca.usuario us on us.usucpf = hsd.usucpfquemfez
							
							order by
							
								hsd.hidid, hsd.datahora desc ";
	
	                $dados = $db->carregar( $sql );
	
	                $retorno = "<table id='tbHistorico' width='100%'>";
	                foreach ( $dados as $valor )
	                {
	                    $dataTime = explode(' ', $valor['data']);
	                    
	                    if( !empty($valor['motivo']) ){
	                        $retorno.= "<tr>";
	                        $retorno.= "    <th>Escola</th>";
	                        $retorno.= "    <th>Turma</th>";
// 	                        $retorno.= "    <th>Ação Realizada</th>";
	                        $retorno.= "    <th>Estado Documento</th>";
	                        $retorno.= "    <th>Usuário</th>";
	                        $retorno.= "    <th>Data</th>";
	                        $retorno.= "</tr>";
	                        
	                        $retorno.= "<tr>";
	                        $retorno.= "    <td style='background-color: #f4f4f4'>".$valor['escola']."</td>";
	                        $retorno.= "    <td style='background-color: #f4f4f4'>".$valor['turma']."</td>";
// 	                        $retorno.= "    <td style='background-color: #f4f4f4'>".$valor['acaorealizada']."</td>";
	                        $retorno.= "    <td style='background-color: #f4f4f4'>".$valor['estadodocumento']."</td>";
	                        $retorno.= "    <td style='background-color: #f4f4f4'>".$valor['nome']."</td>";
	                        $retorno.= "    <td style='background-color: #f4f4f4'>". formata_data($valor['data']) ." ". $dataTime[1] . "</td>";
	                        $retorno.= "</tr>";
	                        
	                        $retorno.= "<tr >";
	                        $retorno.= "    <th colspan='6'>Motivo</th>";
	                        $retorno.= "</tr>";
	                        
// 	                        $retorno.= "<tr>";
// 	                        $retorno.= "    <td colspan='6' style='background-color: #f4f4f4'>".$valor['motivo']."</td>";
// 	                        $retorno.= "</tr>";
// 	                        $retorno.= "<tr>";
// 	                        $retorno.= "    <td colspan='6'>&nbsp;</td>";
// 	                        $retorno.= "</tr>";
	                    }
	                }
	                $retorno.= "</table>";
	            }
	        }catch ( Exception $e ){
		        $retorno = false;
	        }
		    
	        echo( $retorno );
		    break;
	    default:
	        echo 'Ação não existente';
	}
}
exit;
?>