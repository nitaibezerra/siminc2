<?php
// Evita erros de Sessão Expirada
ini_set("memory_limit","512M");
if( !empty( $_SESSION['projovemcampo']['apcid'])){
	$temcoordturma = testacoordturma();
	// Habilita Linhas
	$classeEsfera    = 'linhaEsconde';
	$classeEstado    = 'linhaEsconde';
	$classeMunicipio = 'linhaEsconde';
	$classeEscola    = 'linhaEsconde';
	$classeTurma     = 'linhaEsconde';
	$classeEstudante = 'linhaEsconde';
	$btnEnviarEstado = '';
	$estadoValHabil  = array();
	// Tratamento de erros
	try{
		// Retorna informações do Polo
		
		// Lista de encaminhamento
		$objLisEnc = new ProjovemCampoListaEncaminhamento();
		$objLisEnc->setPerfis( $perfis );

		// Seleciona Tipo do Perfil
		// Perfil do tipo: Super Usuário, PFL_EQUIPE_MEC e PFL_CONSULTA
		if( $db->testa_superuser() || in_array(PFL_EQUIPE_MEC, $perfis) || in_array(PFL_CONSULTA, $perfis)|| in_array(PFL_ADMINISTRADOR, $perfis) ){

			set_time_limit(0);
			ini_set("memory_limit", "15048M");

			$estadoPorPerfil['concluido'] 	= array(ESTADO_DIARIO_MEC);
			$estadoPorPerfil['retorno'] 	= array(ESTADO_DIARIO_DEVOLVIDO_COORDGERAL);
			$estadoPorPerfil['pendente'] 	= array(ESTADO_DIARIO_COORDGERAL,ESTADO_DIARIO_ABERTO,ESTADO_DIARIO_FECHADO,ESTADO_DIARIO_COORDTURMA ,ESTADO_DIARIO_DEVOLVIDO_DIRESCOLA,ESTADO_DIARIO_DEVOLVIDO_COORDTURMA,ESTADO_DIARIO_DEVOLVIDO_COORDGERAL);

			$objLisEnc->inicioPerfilEquipeMec( $estadoPorPerfil );
			$objLisEnc->setTituloPagina( "Estado / Escola / Turma" );
			$objLisEnc->setFormulario( $_REQUEST );
			$objLisEnc->setFormulario( array( 'perid' => $perid ) );
			
			$objLisEnc->setRegistros( listaDeEncaminhamentoPerfilEquipeMEC( $objLisEnc->getArrayDadosFormulario() ) );
			$lista 			 	= $objLisEnc->getRegistros();

			$estadoValHabil     = array(ESTADO_DIARIO_COORDGERAL,ESTADO_DIARIO_DEVOLVIDO_COORDGERAL, ESTADO_DIARIO_PAGAMENTO_ENVIADO  );
			$classeEsfera 	    = 'linhaMostra';
			$btnEnviarEstado    = 'alterarEstadoDiarioCoordenador';

		// Perfil do tipo: PFL_COORDENADOR_MUNICIPAL e PFL_COORDENADOR_ESTADUAL
		}elseif( in_array(PFL_COORDENADOR_MUNICIPAL, $perfis) || in_array(PFL_COORDENADOR_ESTADUAL, $perfis) ){
			
			$estadoPorPerfil['concluido'] 	= array(ESTADO_DIARIO_COORDGERAL,ESTADO_DIARIO_PAGAMENTO_ENVIADO);
			$estadoPorPerfil['retorno'] 	= array(ESTADO_DIARIO_DEVOLVIDO_COORDGERAL);

			$estadoPorPerfil['pendente'] 	= array(ESTADO_DIARIO_ABERTO,ESTADO_DIARIO_FECHADO,ESTADO_DIARIO_COORDTURMA,ESTADO_DIARIO_COORDGERAL,ESTADO_DIARIO_DEVOLVIDO_DIRESCOLA,ESTADO_DIARIO_DEVOLVIDO_COORDTURMA);
			
			$objLisEnc->inicioPerfilCoordenador( $estadoPorPerfil );
			$objLisEnc->setTituloPagina( "Escola / Turma" );
			$objLisEnc->setFormulario( array('apcid'  => $_SESSION['projovemCampo']['apcid'], 
											 'perid'  => $perid,
											 'usucpf' => $_SESSION['usucpf'] ) );

			$objLisEnc->setRegistros( listaDeEncaminhamentoPerfilCoordenadorEstadual( $objLisEnc->getArrayDadosFormulario() ) );
			$lista 			 	= $objLisEnc->getRegistros();

			$estadoValHabil     = array(ESTADO_DIARIO_COORDGERAL,ESTADO_DIARIO_DEVOLVIDO_COORDGERAL);
			$btnEnviarEstado    = 'alterarEstadoDiarioCoordenador';
				
			$classeEscola   = 'linhaMostra';
		
		// Perfil do tipo: Coordenador de turma
		}elseif( in_array(PFL_COORDENADOR_TURMA, $perfis) ){
			
			$estadoPorPerfil['concluido'] 	= array( ESTADO_DIARIO_MEC, ESTADO_DIARIO_COORDGERAL,ESTADO_DIARIO_DEVOLVIDO_COORDGERAL,ESTADO_DIARIO_PAGAMENTO_ENVIADO);
			$estadoPorPerfil['retorno'] 	= array( ESTADO_DIARIO_COORDGERAL, ESTADO_DIARIO_FECHADO,ESTADO_DIARIO_ABERTO);
			$estadoPorPerfil['pendente'] 	= array( ESTADO_DIARIO_COORDTURMA );

			$objLisEnc->inicioPerfilCoordenadorTurma( $estadoPorPerfil );
			$objLisEnc->setTituloPagina( "Turma" );
			$objLisEnc->setFormulario( array('apcid'  => $_SESSION['projovemCampo']['apcid'], 
											 'perid'  => $perid,
											 'usucpf' => $_SESSION['usucpf'] ) );
			
			$objLisEnc->setRegistros( listaDeEncaminhamentoPerfilCoordenadorTurma( $objLisEnc->getArrayDadosFormulario() ) );
			$lista 			 = $objLisEnc->getRegistros();

			$estadoValHabil  = array( ESTADO_DIARIO_COORDTURMA );
			$btnEnviarEstado = 'alterarEstadoDiarioCoordenador';
				
			$classeTurma = 'linhaMostra';
		
		// Perfil do tipo: PFL_DIRETOR_Escola
		}elseif( in_array(PFL_DIRETOR_ESCOLA, $perfis) ){
			
			
			$estadoPorPerfil['concluido'] 	= array( ESTADO_DIARIO_MEC, ESTADO_DIARIO_COORDGERAL,ESTADO_DIARIO_DEVOLVIDO_COORDGERAL, ESTADO_DIARIO_PAGAMENTO_ENVIADO,ESTADO_DIARIO_DEVOLVIDO_COORDTURMA, ESTADO_DIARIO_COORDTURMA) ;
			$estadoPorPerfil['retorno'] 	= array( ESTADO_DIARIO_COORDGERAL,ESTADO_DIARIO_COORDTURMA );
			$estadoPorPerfil['pendente'] 	= array( ESTADO_DIARIO_ABERTO,ESTADO_DIARIO_FECHADO);

			$objLisEnc->inicioPerfilDiretorEscola( $estadoPorPerfil );
			$objLisEnc->setTituloPagina( "Escola / Turma" );
			$objLisEnc->setFormulario( array('apcid'  => $_SESSION['projovemCampo']['apcid'],
											 'perid'  => $perid,
											 'usucpf' => $_SESSION['usucpf'] ) );

			$objLisEnc->setRegistros( listaDeEncaminhamentoPerfilDiretorDeEscola( $objLisEnc->getArrayDadosFormulario() ) );
			$lista 				        = $objLisEnc->getRegistros();
			$btnEnviarEstado            = 'alterarEstadoDiarioCoordenador';
			$estadoValHabil             = array( WF_ESTADO_DIARIO_FECHADO );
			
			$classeEscola   = 'linhaMostra';
					
		}else{
			throw new Exception( 'Erro ao definir tipo de perfil.' );
		}

	
	
		function habilitaBotaoEncaminhar( $parametros ){
		    $retorno = array('habilitaCheckbox'=>false , 'imgRetorno'=>false, 'habilitaCheckboxEnviar' => false);
		    $msgPendencia    = 'Pendência de fechamento';
		    $msgReabrir      = 'Reabrir Turma(s)';
		    $msgHistorico    = 'Histórico Tramitação';
		    
		    //var_dump( $parametros );
		    
		    if( is_array($parametros['dados']) ){
		    	
		        if( !empty($parametros['estuf']) ){
		        	
		            $ufAlvo = $parametros['estuf'];
		            
		        }elseif( !empty($parametros['cpfescola']) ){
		        	
		            $EscolaAlvo = $parametros['cpfescola'];
		            
		        }
		        
		        $totalEscolas      = 0;
		        $totalTurma        = 0;
		        $totalParaReabrir  = 0;
		        $totalConcluidos   = 0;
		        $totalParaRetornar = 0;
		        $totalPendentes    = 0;
		        $estadoRetorno     = array();
		        $estadoConcluido   = array();
		        $estadoPendente    = array();
		        $chaveEscola       = 0;
		
		        // Pega estados por Perfil
		        if( in_array(PFL_COORDENADOR_MUNICIPAL, $parametros['perfis'] )||in_array(PFL_COORDENADOR_ESTADUAL, $parametros['perfis']) ){
		        	
		            $estadoConcluido = array(ESTADO_DIARIO_MEC,ESTADO_DIARIO_PAGAMENTO_ENVIADO);
		            
		            if($temcoordturma){
		            	$estadoRetorno   = array(ESTADO_DIARIO_COORDGERAL,ESTADO_DIARIO_DEVOLVIDO_COORDGERAL);
		            	$estadoPendente  = array(ESTADO_DIARIO_ABERTO,ESTADO_DIARIO_FECHADO,ESTADO_DIARIO_DEVOLVIDO_DIRESCOLA,ESTADO_DIARIO_COORDTURMA,ESTADO_DIARIO_DEVOLVIDO_COORDTURMA);
		            }else{
		            	$estadoRetorno   = array(ESTADO_DIARIO_COORDGERAL,ESTADO_DIARIO_DEVOLVIDO_COORDGERAL);
		            	$estadoPendente  = array(ESTADO_DIARIO_ABERTO,ESTADO_DIARIO_DEVOLVIDO_DIRESCOLA);
		            }
		        
		        }elseif( in_array(PFL_COORDENADOR_TURMA, $parametros['perfis']) ){
		        
		            $estadoConcluido = array(ESTADO_DIARIO_COORDGERAL,ESTADO_DIARIO_DEVOLVIDO_COORDGERAL,ESTADO_DIARIO_PAGAMENTO_ENVIADO,ESTADO_DIARIO_MEC);
		            $estadoPendente  = array(ESTADO_DIARIO_ABERTO,ESTADO_DIARIO_FECHADO,ESTADO_DIARIO_DEVOLVIDO_DIRESCOLA);
		            $estadoRetorno   = array( ESTADO_DIARIO_COORDTURMA,ESTADO_DIARIO_DEVOLVIDO_COORDTURMA );
		        
		        }elseif( in_array(PFL_DIRETOR_ESCOLA, $parametros['perfis']) ) {
		        
		            if($temcoordturma){
		            	$estadoConcluido = array(ESTADO_DIARIO_COORDTURMA,ESTADO_DIARIO_DEVOLVIDO_COORDTURMA,
		            								ESTADO_DIARIO_COORDGERAL,ESTADO_DIARIO_DEVOLVIDO_COORDGERAL,
		            									ESTADO_DIARIO_PAGAMENTO_ENVIADO,ESTADO_DIARIO_MEC);
		            }else{
		                $estadoConcluido = array(ESTADO_DIARIO_COORDGERAL,ESTADO_DIARIO_DEVOLVIDO_COORDGERAL,ESTADO_DIARIO_PAGAMENTO_ENVIADO,ESTADO_DIARIO_MEC);
		            }
		            $estadoRetorno   = array( ESTADO_DIARIO_DEVOLVIDO_DIRESCOLA , ESTADO_DIARIO_FECHADO );
		            $estadoPendente  = array( WF_ESTADO_DIARIO_ABERTO);
		
		        }elseif( in_array(PFL_EQUIPE_MEC, $parametros['perfis'])||in_array( PFL_SUPER_USUARIO,$parametros['perfis'])|| in_array(PFL_ADMINISTRADOR, $parametros['perfis'])  ) {
		        
		            $estadoConcluido = array( ESTADO_DIARIO_PAGAMENTO_ENVIADO  );
		            $estadoPendente  = array( ESTADO_DIARIO_COORDTURMA,ESTADO_DIARIO_DEVOLVIDO_COORDTURMA,
		            								ESTADO_DIARIO_COORDGERAL,ESTADO_DIARIO_DEVOLVIDO_COORDGERAL,
		            									ESTADO_DIARIO_ABERTO,ESTADO_DIARIO_DEVOLVIDO_DIRESCOLA,ESTADO_DIARIO_FECHADO);
		            $estadoRetorno   = array( ESTADO_DIARIO_MEC );
		        }
		
		        foreach ( $parametros['dados'] as $chave=>$valor ){
		        	
		            //Total de escolas
		            if( $chaveEscola != $valor['cpfescola']){
		                $totalEscolas++;
		            }
		            $chaveEscola = $valor['cpfescola'];
		        
		            if( $valor['estuf'] == $ufAlvo
		                    || $valor['cpfescola'] == $EscolaAlvo ){
		                //Total de estudantes por Escola
		                if( $valor['cpfescola'] == $parametros['cpfescola'] ){
		                    $totalTurma++;
		                    // Pega o total de diários para reabertura
		                    if( in_array($valor['estadodocumento'], $parametros['estadodocumento']) ){
		                        $totalParaReabrir++;
		                    }
		                     
		                    // Pega o total de diários concluídos
		                    //print_r( $estadoConcluido );
		                    if( in_array($valor['estadodocumento'], $estadoConcluido) ){
		                        $totalConcluidos++;
		                    }
		                    
		                    // Verifica se tem retono
		                    if( in_array($valor['estadodocumento'], $estadoRetorno) ){
		                        $totalParaRetornar++;
		                    }
		                    
		                    // Verifica se tem pendente
		                    //echo( $parametros['estadodocumento'] . '<br>');
		                    if( in_array($valor['estadodocumento'], $estadoPendente) ){
		                        $totalPendentes++;
		                    }
		                }
		            }
		        }
		        // Validação por Escola
		        if ( isset($EscolaAlvo) ){
		          
		            if( ( empty($totalConcluidos) && empty($totalTurma) ) 
		                    || ( $totalPendentes == $totalTurma ) ){
		            
		                $retorno['imgRetorno'] = '<img src="img/workflow_pendente.png" />' ;
		                
		            }elseif ( $totalConcluidos == $totalTurma && ( $totalConcluidos > 0 && $totalTurma > 0) ){
		                
		                $retorno['imgRetorno'] = '<img src="img/workflow_concluido.png" />' ;
		                
		            }elseif ( $totalParaRetornar > 0 ){
		                    $retorno['imgRetorno'] = '<img src="img/workflow_reabrir.png" onclick="reabrirDiarioEscola('. $parametros['entid'] .');"  style="cursor:pointer" title="'.$msgReabrir.'" alt="'.$msgReabrir.'" />' ;
		                
		            }elseif ( ($totalConcluidos + $totalPendentes) == $totalTurma ){
		                $retorno['imgRetorno'] = '<img src="img/workflow_pendente.png" />' ;
		                //$retorno['imgRetorno'].= implode( $estadoPendente );
		            }
		            // Quando existir somente um escola sem pendência e com diário para reabrir, habilita opção de enviar
		           
		            if ( $totalParaRetornar > 0 && !in_array(PFL_CONSULTA, $parametros['perfis']) ){
		                $retorno['habilitaCheckboxEnviar'] = true;
		            }
		            if ( ($totalParaReabrir > 0 && $totalEscolas > 1)  && !in_array(PFL_CONSULTA, $parametros['perfis']) ){ 
		                $retorno['habilitaCheckbox'] = true;
		            }
		            
		            // Remove opção de checkbox se todas as turmas estiverm concluídas
		            if ( $totalConcluidos == $totalTurma  && !in_array(PFL_CONSULTA, $parametros['perfis']) ){
		                $retorno['habilitaCheckbox'] = false;
		            } 
		        }
		    }
		    return $retorno;  
		}
		
		function pegaHistoricoPorTurma( $dados ){
		    if( is_array($dados) ){
		        foreach ($dados['dados'] as $chave=>$valor ){
		            if( $valor['turid'] == $dados['turid'] && !empty($valor['cmddsc']) ){
		                return true;
		            }        
		        }
		    }
		    return false;
		}
		
		function quantitativoEstudante( $dados ){
		    $retorno = array( 'totalestudante'=>0, 'totalapto'=>0 );
		    
		    if( is_array($dados) ){
		        foreach ($dados['dados'] as $chave=>$valor ){
		            if( $valor['estuf'] == $dados['estuf'] && $dados['flag'] == 'estuf' ){
		                $retorno['totalestudante']++;
		                if( $valor['aptoreceber'] == 'SIM' ){
		                    $retorno['totalapto']++;
		                }
		            }
		            if( $valor['cpfescola'] == $dados['cpfescola'] && $dados['flag'] == 'cpfescola' ){
		                $retorno['totalestudante']++;
		                if( $valor['aptoreceber'] == 'SIM' ){
		                    $retorno['totalapto']++;
		                }
		            }
		            if( $valor['turid'] == $dados['turid'] && $dados['flag'] == 'turid' ){
		            	$retorno['totalestudante']++;
		            	if( $valor['aptoreceber'] == 'SIM' ){
		            		$retorno['totalapto']++;
		            	}
		            }
		        }
		    }
		    return $retorno;
		}
	
		// Retorna a imagem responsável por incluir ação à linha
		function criaImgAcao( $parametro ){
			
		    $msgPendencia    = 'Pendência de fechamento';
		    $msgReabrir      = 'Reabrir Turma(s)';
		    $msgHistorico    = 'Histórico Tramitação';
		    $retornaPara     = '';
		
			if( in_array(PFL_COORDENADOR_MUNICIPAL, $parametro['perfis'] ) || in_array(PFL_COORDENADOR_ESTADUAL, $parametro['perfis']) ){
				
				if($temcoordturma){
			    	$retornaPara 	= ESTADO_DIARIO_DEVOLVIDO_COORDTURMA;
			    	$mensagem 		= 'devolver o diário para o Goordenador de Turma?';
				}else{
					$retornaPara = ESTADO_DIARIO_DEVOLVIDO_DIRESCOLA;
					$mensagem 		= 'devolver o diário para o Diretor de Escola?';
				}
				
		    	$reabrir  = array( ESTADO_DIARIO_COORDGERAL, ESTADO_DIARIO_DEVOLVIDO_COORDGERAL);
		    	$pendente = array( ESTADO_DIARIO_ABERTO,ESTADO_DIARIO_DEVOLVIDO_DIRESCOLA, ESTADO_DIARIO_COORDTURMA,ESTADO_DIARIO_DEVOLVIDO_COORDTURMA,ESTADO_DIARIO_FECHADO);
		
			    // Condição para aplicar imagem
			    
			    if( in_array( $parametro['estadodocumento'], $pendente ) ){
			    
			        // Pendente - Inclui ação do botão
			        $retorno = '<img src="img/workflow_pendente.png"  title="'.$msgPendencia.'" alt="'.$msgPendencia.'" />';
			    
			    }elseif( in_array( $parametro['estadodocumento'], $reabrir ) ){
			         
				    $parametroJs = "'". $_REQUEST['perid']."'"; // perid
					$parametroJs.= ",'". $parametro['turid']."'"; // turid
					$parametroJs.= ",".ESTADO_DIARIO_DEVOLVIDO_COORDGERAL; // turid
					$parametroJs.= ",'".$mensagem."'"; // acao
				
				    // Fechado - Inclui ação do botão
				    $retorno = '<img src="img/workflow_reabrir.png" onclick="reabrirDiario('.$parametroJs.');"  style="cursor:pointer" title="'.$msgReabrir.'" alt="'.$msgReabrir.'" />';
				    
			    }else{
			        $retorno = '<img src="img/workflow_concluido.png" />';
			    }
			    
			    // Adiciona botão de opção de visualizar historico
			    if( pegaHistoricoPorTurma( $parametro )){
			        $retorno.= '<img src="img/workflow_historico.png" onclick="wf_exibirHistorico('.$parametro['docid'].');" style="cursor:pointer" title="'.$msgHistorico.'" alt="'.$msgHistorico.'" />';
			    }
			
			}elseif( in_array(PFL_COORDENADOR_TURMA, $parametro['perfis'] )){
				
			    	$retornaPara = ESTADO_DIARIO_DEVOLVIDO_DIRESCOLA;
			    	$mensagem 		= 'devolver o diário para o Diretor de Escola?';
			    	$reabrir  = array( ESTADO_DIARIO_COORDTURMA,ESTADO_DIARIO_DEVOLVIDO_COORDTURMA);
			    	$pendente = array( ESTADO_DIARIO_ABERTO,ESTADO_DIARIO_DEVOLVIDO_DIRESCOLA,ESTADO_DIARIO_FECHADO );
		
			    // Condição para aplicar imagem
			    if( in_array( $parametro['estadodocumento'], $pendente ) ){
			    
			        // Pendente - Inclui ação do botão
			        $retorno = '<img src="img/workflow_pendente.png"  title="'.$msgPendencia.'" alt="'.$msgPendencia.'" />';
			    
			    }elseif( in_array( $parametro['estadodocumento'], $reabrir ) ){
			         
				    $parametroJs = "'". $_REQUEST['perid']."'"; // perid
					$parametroJs.= ",'". $parametro['turid']."'"; // turid
					$parametroJs.= ",".ESTADO_DIARIO_DEVOLVIDO_DIRESCOLA; // turid
					$parametroJs.= ", '".$mensagem."'"; // acao
				
				    // Fechado - Inclui ação do botão
				    $retorno = '<img src="img/workflow_reabrir.png" onclick="reabrirDiario('.$parametroJs.');"  style="cursor:pointer" title="'.$msgReabrir.'" alt="'.$msgReabrir.'" />';
				    
			    }else{
			        $retorno = '<img src="img/workflow_concluido.png" />';
			    }
			    
			    // Adiciona botão de opção de visualizar historico
			    if( pegaHistoricoPorTurma( $parametro ) ){
			        $retorno.= '<img src="img/workflow_historico.png" onclick="wf_exibirHistorico('.$parametro['docid'].');" style="cursor:pointer" title="'.$msgHistorico.'" alt="'.$msgHistorico.'" />';
			    }
			
			}elseif(in_array(PFL_DIRETOR_ESCOLA, $parametro['perfis'])){
				
				$reabrir  = array(ESTADO_DIARIO_DEVOLVIDO_DIRESCOLA,ESTADO_DIARIO_FECHADO);
				$mensagem 		= 'reabrir o diário?';
				$retornaPara = ESTADO_DIARIO_ABERTO;
				
				// Condição para aplicar imagem
				if( $parametro['estadodocumento'] == ESTADO_DIARIO_ABERTO ){
				
				    // Pendente - Inclui ação do botão
				    $retorno = '<img src="img/workflow_pendente.png"  title="'.$msgPendencia.'" alt="'.$msgPendencia.'" />';
				
				}elseif( in_array( $parametro['estadodocumento'], $reabrir ) ){
			         
					$parametroJs = "'". $_REQUEST['perid']."'"; // perid
					$parametroJs.= ",'". $parametro['turid']."'"; // turid
					$parametroJs.= ",".ESTADO_DIARIO_DEVOLVIDO_COORDGERAL; // turid
					$parametroJs.= ",'".$mensagem."'"; // acao
				
				    // Fechado - Inclui ação do botão
				    $retorno = '<img src="img/workflow_reabrir.png" onclick="reabrirDiario('.$parametroJs.');"  style="cursor:pointer" title="'.$msgReabrir.'" alt="'.$msgReabrir.'" />';
				    
			    }else{
				    $retorno = '<img src="img/workflow_concluido.png" />';
				}
				
				// Adiciona botão de opção de visualizar historico
				if( pegaHistoricoPorTurma( $parametro ) ){
				    $retorno.= '<img src="img/workflow_historico.png" onclick="wf_exibirHistorico('.$parametro['docid'].');" style="cursor:pointer" title="'.$msgHistorico.'" alt="'.$msgHistorico.'" />';
				}
				
			}elseif( (in_array(PFL_EQUIPE_MEC, $parametro['perfis']) && !in_array(PFL_CONSULTA, $parametro['perfis']))||in_array(PFL_SUPER_USUARIO, $parametro['perfis'])|| in_array(PFL_ADMINISTRADOR, $parametro['perfis']) ){
			  
			    // Condição para aplicar imagem
			    if( $parametro['estadodocumento'] == ESTADO_DIARIO_COORDGERAL
			            || $parametro['estadodocumento'] == ESTADO_DIARIO_COORDTURMA
			            || $parametro['estadodocumento'] == ESTADO_DIARIO_ABERTO
			            || $parametro['estadodocumento'] == ESTADO_DIARIO_DEVOLVIDO_COORDGERAL
			            || $parametro['estadodocumento'] == ESTADO_DIARIO_DEVOLVIDO_COORDTURMA
			            || $parametro['estadodocumento'] == ESTADO_DIARIO_DEVOLVIDO_DIRESCOLA
			            || $parametro['estadodocumento'] == ESTADO_DIARIO_FECHADO
			            ){
			         
			        // Pendente - Inclui ação do botão
			        $retorno = '<img src="img/workflow_pendente.png"  title="'.$msgPendencia.'" alt="'.$msgPendencia.'" />';
			         
			    }elseif( $parametro['estadodocumento'] == ESTADO_DIARIO_MEC ){
					
			        $parametroJs = "'". $_REQUEST['perid']."'"; // perid
			        $parametroJs.= ",'". $parametro['turid']."'"; // turid
			        $parametroJs.= ",".ESTADO_DIARIO_DEVOLVIDO_COORDGERAL; // turid
			        $parametroJs.= ", 'devolver o diário para o Goordenador Geral?'"			; // acao
			       
			        // Fechado - Inclui ação do botão
			        $retorno = '<img src="img/workflow_reabrir.png" onclick="reabrirDiario('.$parametroJs.');"  style="cursor:pointer" title="'.$msgReabrir.'" alt="'.$msgReabrir.'" />';
			    }else{
			        $retorno = '<img src="img/workflow_concluido.png" />';
			    }
			     
			    // Adiciona botão de opção de visualizar historico
			    if(pegaHistoricoPorTurma( $parametro )){
			        $retorno.= '<img src="img/workflow_historico.png" onclick="wf_exibirHistorico('.$parametro['docid'].');" style="cursor:pointer" title="'.$msgHistorico.'" alt="'.$msgHistorico.'" />';
			    }
			}
		
			return $retorno;
		}
	
		?>
		<style>
		textarea { width: 300px; height: 100px; border: 1px solid #333; background: #ffe; padding: 1em; font: normal 0.7em Arial, sans-serif;}
		.destaque{ background:#FFFF99;color:#FF0000; }
		</style>
		<table id="tabelaListaDeEcaminhamento" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0">
			<tr> 
				<th class="espacamentoAcao">&nbsp;</th> <!-- Indicador Enviado para aprovação / Checkbox -->
				<th colspan="13"><?php echo( $objLisEnc->getTituloPagina() ); ?></th>
				<th class="celulaTotalizadora">Total de Estudantes</th>
				<th class="celulaTotalizadora">Total de Aptos</th>
				<th class="celulaTotalizadora">Reabrir</th>
			</tr>
			<?php 

			if( !empty( $lista ) ){
				
				//$chaveEsferaEstadual  	= 'Estadual';
				//$chaveEsferaMunicipal 	= 'Municipal';
				$chaveEsfera  		  	= '';
				$chaveEstado  		  	= '';
				$chaveMunicipio	  		= '';
				$chavePolo 	  			= '';
				$chaveEscola  			= 0;
				$chaveTurma   			= 0;
				$EscolaUm     			= '';
				$idLinhaEsfera			= '';
				$idLinhaEstado			= '';
				$idLinhaMunicipio		= '';
				
				foreach ( $lista as $chave => $valor ){

	
					if( $db->testa_superuser() || in_array(PFL_EQUIPE_MEC, $perfis) || in_array(PFL_CONSULTA, $perfis) || in_array(PFL_ADMINISTRADOR, $perfis) ){
						
						// Mostra esfera
						if ( $chaveEsfera != $lista[$chave]['esfera'] ){
							$quantitativoEscola = quantitativoEstudante( array('dados'=>$lista, 'estuf'=>$lista[$chave]['estuf'], 'flag'=>'estuf' ) );
							$idLinhaEsfera = "esfera_" . $lista[$chave]['esfera'];
							?>
							<tr class="listaEsfera <?php echo( $classeEsfera ); ?>"	
										id="<?php echo( $idLinhaEsfera ); ?>">
								<td width="10px">
								    &nbsp; <!-- <input type="checkbox" name='chkuf_<?php echo( $lista[$chave]['esfera'] ); ?>' />  -->
								</td>
								<td colspan="13" 
									class="negrito abreListaDeEstados" style="cursor:pointer" ><?php echo( $lista[$chave]['esfera'] ); ?></td>
								<td class="negrito alinhaTextoCentro"><?php echo( $quantitativoEscola['totalestudante'] ); ?></td>
								<td class="negrito alinhaTextoCentro"><?php echo( $quantitativoEscola['totalapto'] ); ?></td>
								<td class="alinhaTextoCentro">-</td>
							</tr>
							<?php
						}//Fim Mostra Esfera
						
						
						//Mostra Estado
						if ( $chaveEstado != $lista[$chave]['estuf'] )
						{
							$idLinhaEstado = "uf_" . $lista[$chave]['esfera'].'_'.$lista[$chave]['estuf'];
							?>
							<tr class="listaEstado <?php echo( $idLinhaEsfera ); ?> <?php echo( $classeEstado ); ?>"
										id="<?php echo( $idLinhaEstado ); ?>">
								<td colspan="3">&nbsp;</td> 
								<td width="10px">
								    &nbsp; <!-- <input type="checkbox" name='chkuf_<?php echo( $lista[$chave]['esfera'] ); ?>' />  -->
								</td>
								<td colspan="10" 
									class="negrito abreListaDeEsfera" style="cursor:pointer" ><?php echo( $lista[$chave]['estuf'] ); ?></td>
								<td class="negrito alinhaTextoCentro"><?php echo( $quantitativoEscola['totalestudante'] ); ?></td>
								<td class="negrito alinhaTextoCentro"><?php echo( $quantitativoEscola['totalapto'] ); ?></td>
								<td class="alinhaTextoCentro">-</td>
							</tr>
							<?php
						}//Fim Mostra Estado
						
						//Mostra Município
						if ( $chaveMunicipio != $lista[$chave]['mundescricao'] ){
							$idLinhaMunicipio = "esferaMunicipal_" . $lista[$chave]['esfera'].'_'.str_replace(' ', '_', $lista[$chave]['mundescricao'] );  
							?>
							<tr class="listaEsferaMunicipal <?php echo( $idLinhaEstado ); ?> <?php echo( $classeMunicipio ); ?>"	
										id="<?php echo( $idLinhaMunicipio ); ?>">
								<td colspan="4">&nbsp;</td> 
								<td width="10px">
								    &nbsp; <!-- <input type="checkbox" name='chkuf_<?php echo( $lista[$chave]['mundescricao'] ); ?>' />  -->
								</td>
								<td colspan="9" class="negrito abreListaDeEscolas" style="cursor:pointer" ><?php echo( $lista[$chave]['mundescricao'] ); ?></td>
								<td class="negrito alinhaTextoCentro"><?php echo( $quantitativoEscola['totalestudante'] ); ?></td>
								<td class="negrito alinhaTextoCentro"><?php echo( $quantitativoEscola['totalapto'] ); ?></td>
								<td class="alinhaTextoCentro">-</td>
							</tr>
							<?php 
						}//Fim Mostra Município
					}//Fim Validação Perfil
					
					if ( $chaveEscola != $lista[$chave]['cpfescola']){

					    $EscolaUm = $lista[$chave]['cpfescola'];
					    $quantitativoEscola = quantitativoEstudante( array('dados'=>$lista, 'cpfescola'=>$lista[$chave]['cpfescola'], 'flag'=>'cpfescola' ) );
		
					    // Parâmetros
					    $parHabilitaBotaoEscola['dados']             = $lista;
					    $parHabilitaBotaoEscola['estadodocumento']   = $estadoValHabil;
					    $parHabilitaBotaoEscola['perfis']            = $perfis;
					    $parHabilitaBotaoEscola['cpfescola']         = $lista[$chave]['cpfescola'];
					    $parHabilitaBotaoEscola['diaid']             = $lista[$chave]['diaid'];
					    $parHabilitaBotaoEscola['entid']             = $lista[$chave]['entid'];
		                // Habilita botao encaminhar
		                // Apresenta imagens e suas respectivas funções
					    $retornoBtHabilEscola = habilitaBotaoEncaminhar( $parHabilitaBotaoEscola );
					    $idEscolaLinha = "Escola_".$lista[$chave]['cpfescola'];
					    //var_dump( $retornoBtHabilEscola );
						?>
						<tr class="listaEscola <?echo( $idLinhaMunicipio );?> <? echo( $classeEscola ); ?>" id="<?php echo( $idEscolaLinha ); ?>" >
							<td colspan="8">&nbsp;</td>
							<td class="espacamentoAcao">
							    <?php 
							    if( $retornoBtHabilEscola['habilitaCheckbox'] ) {
							        
							        $acaoCheckEscola = 'onclick="habilitaBotaoEscola( '. $lista[$chave]['entid'] .' )"';
							        
							        if( in_array(PFL_EQUIPE_MEC, $perfis) || $db->testa_superuser() || in_array(PFL_CONSULTA, $perfis) || in_array(PFL_ADMINISTRADOR, $perfis) )
							        {
							            $acaoCheckEscola = ''; 
							        }
							        ?>
								    <input <?php echo($acaoCheckEscola); ?>
									     type="checkbox"
									      name='chkEscola_id[]'
									       id='<?php echo( $lista[$chave]['cpfescola'] ); ?>'
									       value='<?php echo( $lista[$chave]['entid'] ); ?>'
								<?php 
							    }else{
									echo "&nbsp;";	
								} ?>
							</td>
							<td colspan="5" class="abreListaDeTurmas negrito" style="cursor:pointer"><?php echo( $lista[$chave]['escola'] ); ?></td>
							<td class="negrito alinhaTextoCentro"><?php echo( $quantitativoEscola['totalestudante'] ); ?></td>
							<td class="negrito alinhaTextoCentro"><?php echo( $quantitativoEscola['totalapto'] ); ?></td>
							<td class="alinhaTextoCentro"><?php 
							// Funções
							/*$dadosImg['tipo']            = 'turma';
							$dadosImg['estadodocumento'] = $lista[$chave]['estadodocumento'];
							$dadosImg['historico'] 		 = $lista[$chave]['cmddsc'];
							$dadosImg['docid'] 		 	 = $lista[$chave]['docid'];
							$dadosImg['perfis']			 = $perfis;
							$dadosImg['diaid']			 = $lista[$chave]['diaid'];
							$dadosImg['possuipolo']      = $polomunicipio['pmupossuipolo'];
							$dadosImg['turid'] 		 	 = $lista[$chave]['turid'];
							$dadosImg['dados'] 		 	 = $lista;*/
		
							echo $retornoBtHabilEscola['imgRetorno'];
							?></td>
						</tr>
						<?php
					}	
					?>
					
					<!--  Lista Turmas -->
					<?php

					if( $chaveTurma != $lista[$chave]['turid'] ){

						$quantitativoTurma = quantitativoEstudante( array('dados'=>$lista, 'turid'=>$lista[$chave]['turid'], 'flag'=>'turid' ) );
						$idTurmaLinha = "turma_".$lista[$chave]['turid'];
						?>
						<tr class="listaTurmas <?php echo( $idEscolaLinha ); ?> <?php echo( $classeTurma ); ?>" 
									id="<?php echo( $idTurmaLinha ); ?>" >
		
							<td colspan="9">&nbsp;</td>
							<td colspan="5" class="abreListaDeEstudantes negrito"
								style="cursor:pointer"><?php echo( $lista[$chave]['turdescricao'] ); ?>
		
								<input type="hidden" class="chkturma" value="<?php echo( $lista[$chave]['turid'] ); ?>"
								 name='chkturma_id[]' id='chkturma_id[]' />
								 
								<input type="hidden" class="chkdiario" value="<?php echo( $lista[$chave]['diaid'] ); ?>"
								 name='chkdiario[]' id='chkdiario[]' />
							</td>
							<td class="negrito alinhaTextoCentro"><?php echo( $quantitativoTurma['totalestudante'] ); ?></td>
							<td class="negrito alinhaTextoCentro"><?php echo( $quantitativoTurma['totalapto'] ); ?></td>
							<td class="alinhaTextoCentro"><?php
								// Funções 
								$dadosImg['tipo']            = 'turma';
								$dadosImg['estadodocumento'] = $lista[$chave]['estadodocumento'];
								$dadosImg['historico'] 		 = $lista[$chave]['cmddsc'];
								$dadosImg['docid'] 		 	 = $lista[$chave]['docid'];
								$dadosImg['perfis']			 = $perfis;
								$dadosImg['diaid']			 = $lista[$chave]['diaid'];
								$dadosImg['turid'] 		 	 = $lista[$chave]['turid'];
								$dadosImg['dados'] 		 	 = $lista;
								
								echo criaImgAcao( $dadosImg ) ; //. $lista[$chave]['estadodocumento']
							?>
							<img title="Histórico do Diário" src="/imagens/historico_diario.png"  onclick="abrirHistoricoDiario(<?php echo $lista[$chave]['diaid']; ?>);"/>
                                <br />
							</td>
						</tr>
		
						<tr class="cabecalhoEstudante 
								   <?php echo( $classeEstudante ); ?> 
								   <?php echo( $idTurmaLinha ); ?>">
	
	
							<th colspan="9">&nbsp;</th>
							<th class="negrito" style="width: 30px;">Matrícula</th>
							<th>Estudante</th>
							
							<th>Frequência</th>
<!-- 							<th>Nº de auxílios à receber</th> -->
							
							<th class="negrito alinhaTextoCentro">Apto à receber</th>
							<th class="negrito alinhaTextoCentro">Agência</th>
							<th class="alinhaTextoCentro">NIS</th>
						</tr>
						<?php 
					}
					?>
					<?php
					if( $lista[$chave]['tipo_aluno'] == 'transferido'){
						
						$estudante = "<span style=\"color: red;\">" . $lista[$chave]['estudante']  . "(Transferido) </span>";
					}else{
						
						$estudante = $lista[$chave]['estudante'];
					}
					?>
					<tr class="listaEstudantes 
							   <?php echo( $idTurmaLinha ); ?>  
							   <?php echo( $classeEstudante ); ?>">
	
						<td colspan="9">&nbsp;</td>
						<td class="celulaTotalizadora alinhaTextoCentro"><?php echo( $lista[$chave]['matricula'] ); ?></td>
						<td><?php echo( $estudante ); ?></td>
						
						<td class="alinhaTextoCentro celulaEstudante"><?php echo( $lista[$chave]['frequencia'] ); ?>%</td>
						<!-- <td class="alinhaTextoCentro celulaEstudante"><?php /*echo( $lista[$chave]['auxilios'] ); */?></td> -->
						
						<td class="alinhaTextoCentro"><?php echo( $lista[$chave]['aptoreceber'] ); ?></td>
						<td class="alinhaTextoCentro"><?php echo( $lista[$chave]['agbcod'] ); ?></td>
						<td class="alinhaTextoCentro"><?php echo( $lista[$chave]['nis'] ); ?></td>
					</tr>
		
					<?php
					
					//Seta chave Esfera
					if( $chaveEsfera != $lista[$chave]['esfera'] ){
						$chaveEsfera  = $lista[$chave]['esfera'];
					}
	
					//Seta chave estado
					if( $chaveEstado != $lista[$chave]['estuf'] ){
						$chaveEstado  = $lista[$chave]['estuf'];
					}
	
					//Seta chave Esfera
					if( $chaveMunicipio != $lista[$chave]['mundescricao'] ){
						$chaveMunicipio  = $lista[$chave]['mundescricao'];
					}
								
					//Seta chave Escola
					if( $chaveEscola != $lista[$chave]['cpfescola'] ){
						$chaveEscola = $lista[$chave]['cpfescola'];
					}
					
					//Seta chave da turma
					if( $chaveTurma != $lista[$chave]['turid'] ){
						$chaveTurma = $lista[$chave]['turid'];
					}
				}
		
				// Parâmetros
				//$parHabilitaJsEscola['entid']             = $EscolaUm;
				//$parHabilitaJsEscola['dados']             = $lista;
				//$parHabilitaJsEscola['estadodocumento']   = array( WF_ESTADO_DIARIO_FECHADO );
				 
				// Habilita botao encaminhar
// 				$retornoBtHabilEscola = habilitaBotaoEncaminharPerfilEscola( $parHabilitaJsEscola );
			}
					
		?>
		</table>
		
		<table border="0" width="95%" align="center">
		<?php if( !in_array(PFL_EQUIPE_MEC, $perfis) && !in_array(PFL_CONSULTA, $perfis)|| in_array(PFL_ADMINISTRADOR, $perfis)  ) {?>
			<tr>
				<td colspan="2">
					<div id="chkDivDeclaracao">
						<input type="checkbox" id="declaracao" />
						<input type="checkbox" id="declaracaoEscola" />
						Declaro que as informações prestadas estão em conformidade com o(s) diário(s) de frequência e entrega de trabalhos apresentados, 
						sendo mantida a fidedignidade das mesmas.
					</div>
				</td>
			</tr>
		<?php } ?>
			<tr>
				<td>
					<div id="legendasWorkflow">
						
						<img src="img/workflow_reabrir.png" />
						<span>Retornar diário </span>
						<br />
						
						<img src="img/workflow_historico.png" />
						<span>Informação de retorno</span>
						<br />
						
						<img src="img/workflow_pendente.png" />
						<span>Pendência de fechamento</span>
						<br />
						
						<img src="img/workflow_concluido.png" />
						<span>Encaminhado para aprovação/Pagamento</span>
						<br />
					</div>
				</td>
				<td>
				    <?php if( !in_array(PFL_EQUIPE_MEC, $perfis) && !in_array(PFL_CONSULTA, $perfis) || in_array(PFL_ADMINISTRADOR, $perfis) ) {?>
		    			<div id="btDivEncaminhar">
		    				<input type="button" id="btEncaminharLista" value="Validar/Encaminhar" />
		    				<input type="button" id="btEncaminharListaEscola" value="Validar/Encaminhar" />
		    			</div>
					<?php }elseif( in_array(PFL_EQUIPE_MEC, $perfis) && !in_array(PFL_CONSULTA, $perfis) || in_array(PFL_ADMINISTRADOR, $perfis) ){?>
		    			<div id="btDivEncaminhar">
		    				<input type="button" id="btEncaminharListaGeral" value="Encaminhar para Pagamento" />
		    			</div>
					<?php }?>
				</td>
			</tr>
		</table>
		<div id="boxJustificativa" name="boxJustificativa">
		    <table>
		    	<tr>
		    		<td class="SubTituloDireita">Motivo Retorno</td>
		    		<td>
		    		    <textarea id="textJustificativa" name="textJustificativa"></textarea>
		                <p>caracteres a serem digitados: <span id="left"></span></p>
		    		</td>
		    	</tr>
		    	<tr>
		    		<td class="SubTituloDireita" colspan="2">
		    		    <label id="labelBtJustificativa"></label>
		    		</td>
		    	</tr>
		    </table>
		</div>
		
		<div id="boxHistorico">
		    
		</div>
		<script type="text/javascript">

			function abrirHistoricoDiario( diaid ){
             	var janela = window.open("projovemcampo.php?modulo=principal/monitoramento/popHistoricoDiario&acao=A&diaid="+diaid, "popHistoricoDiario", "menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=10,top=10,width=800,height=200");
     		}
		
		    function mantemConsulta(){
		    	<?php if( in_array(PFL_EQUIPE_MEC, $perfis) || in_array(PFL_CONSULTA, $perfis) || in_array(PFL_ADMINISTRADOR, $perfis) ) {?>
		
		            if( $('#frmEncaminharLista').valid() == true ){
		    			//Gatinho para selecionar o que já "está" selecionado
		    			
		    			$('#estuf option[value != ""]').attr('selected','selected');
		    			$('#entid option[value != ""]').attr('selected','selected');
		    
		    			//console.log( params );
		            	$.post( 'geral/ajax.php?acao=gerenciarListaDeEncaminhamento', $('#frmEncaminharLista').serialize(), function(response){
		            		$('#container-encaminhar-lista').html( '' );
		    				$('#container-encaminhar-lista').html( response );
		            	}, 'html' );
		            }
		    	
		    	<?php }else{?>
		    	
		        	var params     = {};
		        	
		        	params['acao']  = 'gerenciarListaDeEncaminhamento';
		        	params['perid'] = <?php echo($perid); ?>;
		        
		           $.post( 'geral/ajax.php', params, function(response){
		        	    $('#container-encaminhar-lista').html( '' );
		        		$('#container-encaminhar-lista').html( response );
		           }, 'html' );
		
		       <?php }?>
		    }
		
			function habilitaBotaoEscola( idEscola )
			{
				$("#declaracao").hide();
				$("#declaracaoEscola").show();
				
				$("#chkDivDeclaracao").show();
				
				$("#btEncaminharLista").hide();
				$("#btEncaminharListaEscola").show();
			}
			function reabrirDiario(perid, turid, status, acaodescricao) {
				
				if ( !confirm( 'Deseja realmente ' + acaodescricao + ' ?' ) ){
					return;
				}
		
				var nomeRandomico = "idBtn_" + Math.floor( Math.random() * 11 );
		        var params = {
		        	    acao           : 'reabrirDiario',
		        	    perid		   : perid,
		        	    turid		   : turid,
		        	    status		   : status
		            };
		
// 		        var botaoJustificativa = $("<input/>"
// 		                , { type    : 'button'
// 		                    , value : 'Enviar'
// 		                    , id    : nomeRandomico
// 		                    , click : function(){
		
// 		            			params.justificativa = $('#textJustificativa').val();
		            		    $.get( "geral/ajax.php", params, function(resposta){
		                            var objRetorno = jQuery.parseJSON( resposta );
		                             mantemConsulta();
		                            $(".ui-dialog").hide();
		                        });
// 		                    }
// 		                });
		
// 		        $("#labelBtJustificativa").html('');
// 		        $("#labelBtJustificativa").append( botaoJustificativa );
		
// 				$( "#boxJustificativa" ).dialog({
// 		            height: 200,
// 		            width:  400,
// 		            close: function(){
// 		            	$( '#textJustificativa' ).val('');
// 		            }
// 		        });
			}
		
			function reabrirDiarioEscola( entid ){
				if ( !confirm( 'Deseja realmente Reabrir o(s) Diário(s)?' ) ){
					return;
				}
				
		      	var objParametros          = {};
		        objParametros.acao         = 'reabrirDiario',
		        objParametros.perid        = <?php echo($perid); ?>;
		        objParametros.entid  	   = entid;
		        objParametros.diarioid     = new Array();
			
		        var nomeRandomico          = "idBtn_" + Math.floor( Math.random() * 11 );
		        var contadorDiario         = 0;
// 				var botaoJustificativa = $("<input/>"
// 		                , { type    : 'button'
// 		                    , value : 'Enviar'
// 		                    , id    : nomeRandomico
// 		                    , click : function(){
		
// 		            			objParametros.justificativa = $('#textJustificativa').val();
		
// 		            			nomeClasseEscola = 'tr.Escola_'+ objParametros.idcpfescola +' td input[name^="chkdiario"]';
		            			
// 		            			$( nomeClasseEscola ).each(function(idx1, ele1){
// 		            				objParametros.diarioid[ contadorDiario ] = $(ele1).val();
// 		            			});
		
		            		    $.get( "geral/ajax.php", objParametros, function(resposta){
		                            var objRetorno = jQuery.parseJSON( resposta );
		                            mantemConsulta();
		                            alert( objRetorno.retorno );
		                            $(".ui-dialog").hide();
		                        });
// 		                    }
// 		                });
		
// 		        $("#labelBtJustificativa").html('');
// 		        $("#labelBtJustificativa").append( botaoJustificativa );
		
// 				$( "#boxJustificativa" ).dialog({
// 		            height: 200,
// 		            width:  400,
// 		            close: function(){
// 		            	$( '#textJustificativa' ).val('');
// 		            }
// 		        });
			}
		
// 			function reabrirDiarioPolo( idPolo )
// 			{
// 				if ( !confirm( 'Deseja realmente Reabrir o(s) Diário(s) ?' ) )
// 				{
// 					return;
// 				}
		
// 		        var objParametros      = {};
// 		        objParametros.acao     = 'reabrirDiario',
// 		        objParametros.fluxo    = 'polo',
// 		        objParametros.polid    = idPolo;
// 		        objParametros.diarioid = new Array();
		        
// 		        var nomeRandomico = "idBtn_" + Math.floor( Math.random() * 11 );
// 		        var contadorDiario = 0;
		
// 		        var botaoJustificativa = $("<input/>"
// 		                , { type    : 'button'
// 		                    , value : 'Enviar'
// 		                    , id    : nomeRandomico
// 		                    , click : function(){
		
// 		            			objParametros.justificativa = $('#textJustificativa').val();
		            			            			
// 		            			var Escolas = $('tr[class*="listaEscola"]');
// 		            			var turmas  = [];
		
// 		            			Escolas.each(function( idx, el ){
// 		            			    turmas = $('tr[class*="listaTurmas"][class*="'+ $(el).attr('id') +'"]');    
// 		            			    turmas.each(function(idx, elTurma ){
// 		            			        //console.log(  $(elTurma).find('input[name^="chkdiario"]') );
// 		            			        objParametros.diarioid[ contadorDiario ] =  $(elTurma).find('input[name^="chkdiario"]').val();
// 		                				contadorDiario++;
// 		            			    });
// 		            			});
		
// 		            		    $.get( "geral/ajax.php", objParametros, function(resposta){
// 		                            var objRetorno = jQuery.parseJSON( resposta );
// 		                            mantemConsulta();
// 		                            alert( objRetorno.retorno );
// 		                            $(".ui-dialog").hide();
// 		                        });
// 		                    }
// 		                });
		
// 		        $("#labelBtJustificativa").html('');
// 		        $("#labelBtJustificativa").append( botaoJustificativa );
		
// 				$( "#boxJustificativa" ).dialog({
// 		            height: 200,
// 		            width:  400,
// 		            close: function(){
// 		            	$( '#textJustificativa' ).val('');
// 		            }
// 		        });
// 			}
		
			function wf_exibirHistorico( docid ){
		        var params = {
		        		docid : docid, 
		                acao : 'listarHistorico'        
		            };
		
				$.get( "geral/ajax.php", params, function(resposta){
					$( "#boxHistorico" ).html('');
		            $( "#boxHistorico" ).html(resposta);
		            $( "#boxHistorico" ).dialog({
		                height: 400,
		                width:  600,
		                close: function(){
		            	    //document.location.reload(true);
		                    }
		               });
		            });
			}
		
			$(document).ready(function(){
		
			    $("#chkDivDeclaracao").hide();
			    
				<?php if( $retornoBtHabilEscola['habilitaCheckboxEnviar'] ){ ?>
			    	$("#chkDivDeclaracao").show();
				<?php } ?>
		
		        jQuery.ajaxSetup({
		            beforeSend: function(){
		                $("#dialogAjax").show();
		            },
		            complete: function(){
		                $("#dialogAjax").hide();
		            }
		        });
		
				$("#boxJustificativa").hide();
				$("#boxHistorico").hide();
		        
				var mostraEscondeLista = function( elemento, exibeFilho ){
					
		            var id = $(elemento).attr('id')
		                , filhos;
		            
		            if( id === undefined ){
		                return false;
		            }
		            
		            filhos = $('#'+ id).nextAll('tr[class*="'+  id +'"]');
		            
		            if( exibeFilho == 0 ){
		                $('#'+ id).nextAll('tr[class*="'+  id +'"]').hide();
		            }
		            else{
		                $('#'+ id).nextAll('tr[class*="'+  id +'"]').show();
		            }
		           
		            if( filhos.length > 0 && exibeFilho == 0 ){
		                filhos.each(function(idx, el){
		                    mostraEscondeLista(el, exibeFilho);
		                });
		            }
				};
		
				$('.abreListaDeEstudantes').click(function(){
		        
		            var idTrEscola   =  $(this).parent('tr').attr('id')
		                , exibeFilho = 0;
		            
		            if( !$('#'+ idTrEscola).nextAll('tr[class*="'+  idTrEscola +'"]').is(':visible') )
		            {
		                exibeFilho = 1;
		            }
					
					mostraEscondeLista( $(this).parent('tr'), exibeFilho  );
				});
		
				$('.abreListaDeTurmas').click(function(){
		            var idTrEscola   =  $(this).parent('tr').attr('id')
		                , exibeFilho = 0;
		            
		            if( !$('#'+ idTrEscola).nextAll('tr[class*="'+  idTrEscola +'"]').is(':visible') )
		            {
		                exibeFilho = 1;
		            }
					mostraEscondeLista( $(this).parent('tr') , exibeFilho );
				});
		
				$('.abreListaDeEscolas').click(function(){
		            
		            var idTrEscola   =  $(this).parent('tr').attr('id')
		                , exibeFilho = 0;
		            if( !$('#'+ idTrEscola).nextAll('tr[class*="'+  idTrEscola +'"]').is(':visible') )
		            {
		                exibeFilho = 1;
		            }
					mostraEscondeLista( $(this).parent('tr'), exibeFilho );
				});
				
				$('.abreListaDeEstados').click(function(){
		            
		            var idTrEscola   =  $(this).parent('tr').attr('id')
		               , exibeFilho = 0;
		           
		           if( !$('#'+ idTrEscola).nextAll('tr[class*="'+  idTrEscola +'"]').is(':visible') )
		           {
		               exibeFilho = 1;
		           }
					
					mostraEscondeLista( $(this).parent('tr'), exibeFilho );
				});
	
				$('.abreListaDeEsfera').click(function(){
		            
		            var idTrEscola  = $(this).parent('tr').attr('id')
		               , exibeFilho = 0;
		           
		           if( !$('#'+ idTrEscola).nextAll('tr[class*="'+  idTrEscola +'"]').is(':visible') )
		           {
		               exibeFilho = 1;
		           }
					
					mostraEscondeLista( $(this).parent('tr'), exibeFilho );
				});
	
				
		
				$('table#tabelaListaDeEcaminhamento tr[class*="abreListaDeEscolas"]:odd').css("background-color", "#ffffff");
		 		$(".linhaEsconde").hide();
		 		$(".linhaMostra").show();
		 		
				//$("#btDivEncaminhar").hide();
				$("#declaracaoEscola").hide();
				$("#declaracaoPolo").hide();
				
				$("#btEncaminharLista").attr('disabled', 'disabled');
				$("#btEncaminharListaEscola").attr('disabled', 'disabled');
				$("#btEncaminharListaEscola").hide();
		
				$("#declaracao").click(function(){
					if( $("#declaracao").is(":checked") ){
						$("#btEncaminharLista").removeAttr('disabled');
					}else{
						$("#btEncaminharLista").attr('disabled', 'disabled');
					}
				});
		
				$("#declaracaoEscola").click(function(){
					if( $("#declaracaoEscola").is(":checked") ){
						$("#btEncaminharListaEscola").removeAttr('disabled');
					}else{
						$("#btEncaminharListaEscola").attr('disabled', 'disabled');
					}
				});
		
		
				$("#btEncaminharListaEscola").click(function(){
					
					if ( !confirm( 'Confirma encaminhamento da lista dos estudantes selecionados?' ) )
					{
						return;
					}
		
					var objParametros       = {};
					var contadorTurma       = 0;
					var nomeClasseEscola    = 0;
					
				    objParametros.acao      = '<?php echo($btnEnviarEstado); ?>';
				    objParametros.perid     = <?php echo($perid); ?>;
				    objParametros.turma_id  = new Array();
				    
		
				    $('input[name^=chkEscola_id]:checked').each(function(idx, ele){
						
						nomeClasseEscola = 'tr.Escola_'+ $(ele).attr('id') +' td input[name^="chkturma_id"]';
						
						$( nomeClasseEscola ).each(function(idx1, ele1){
		
							objParametros.turma_id[ contadorTurma ] = $(ele1).val();
						    contadorTurma++;
						});
				    });
		
				    if( objParametros.turma_id.length == 0 ){
					    alert('Selecione uma Escola');
				    }else{
					    
		    		    $.post( 'geral/ajax.php', objParametros, function(response){
		                    var objRetorno = jQuery.parseJSON( response );
		                    mantemConsulta();
		    				//document.location.reload(true);
		                }, 'html' );
				    }
				});
		
				$("#btEncaminharLista").click(function(){
		
					if ( !confirm( 'Confirma encaminhamento da lista dos estudantes selecionados?' ) )
					{
						return;
					}
				
				    var contador            = 0;
					var objParametros       = {};
				    objParametros.acao      = '<?php echo($btnEnviarEstado); ?>';
				    objParametros.perid     = <?php echo($perid); ?>;
				    objParametros.turma_id  = new Array();
					
					$('.chkturma').each(function(idx, ele){
						objParametros.turma_id[ contador ] = $(ele).val();
					    contador++;
					});
					alert(objParametros.turma_id);
					//console.log( objParametros );
		            $.post( 'geral/ajax.php', objParametros, function(response){
		                var objRetorno = jQuery.parseJSON( response );
		                mantemConsulta();
		                alert( objRetorno.retorno );
						//document.location.reload(true);
		            }, 'html' );
				});
		
				$("#btEncaminharListaGeral").click(function(){
		
					if ( !confirm( 'Confirma encaminhamento da lista dos estudantes selecionados?' ) )
					{
						return;
					}
					
					var objParametros       = {};
					var contadorTurma       = 0;
					var nomeClasseEscola    = 0;
					var nomeDoBotao			= $("#btEncaminharListaGeral").val();
					$("#btEncaminharListaGeral").val('Carregando...');
					$("#btEncaminharListaGeral").attr('disabled','disabled');
					
				    objParametros.acao      = '<?php echo($btnEnviarEstado); ?>';
				    objParametros.perid     = <?php echo($perid); ?>;
				    objParametros.turma_id  = new Array();
				    
		
				    $('input[name^=chkEscola_id]:checked').each(function(idx, ele){
						
						nomeClasseEscola = 'tr.Escola_'+ $(ele).attr('id') +' td input[name^="chkturma_id"]';
						
						$( nomeClasseEscola ).each(function(idx1, ele1){
		
							objParametros.turma_id[ contadorTurma ] = $(ele1).val();
						    contadorTurma++;
						});
				    });
		
				    if( objParametros.turma_id.length == 0 ){
					    alert('Selecione um escola');
				    }else{
					    
		    		    $.post( 'geral/ajax.php', objParametros, function(response){
		                    var objRetorno = jQuery.parseJSON( response );
		                    alert( objRetorno.retorno );
		                    $("#btEncaminharListaGeral").val( nomeDoBotao );
		                    $("#btEncaminharListaGeral").removeAttr('disabled');
		                    //mantemConsulta();
		    				//document.location.reload(true);
		                }, 'html' );
				    }
				});
		
				$('textarea').limit('5000','#left');
				$('textarea').limit('5000');
		
					$('table#tabelaListaDeEcaminhamento tr').hover( 
					    function(){ 
					        $(this).addClass('destaque'); 
					    }, 
					    function(){ 
					        $(this).removeClass('destaque'); 
					    }
					); 
			});
		
		</script>
<?php 
	} catch( Exception $e ) {
	
		die( $e->getMessage() );
	}
} ?>