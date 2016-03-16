<?php
// Evita erros de Sessão Expirada
ini_set("memory_limit","512M");
if( !empty( $_SESSION['projovemurbano']['pjuid'] ) ){
	
	// Habilita Linhas
	$classeEsfera    = 'linhaEsconde';
	$classeEstado    = 'linhaEsconde';
	$classeMunicipio = 'linhaEsconde';
	$classePolo	   	 = 'linhaEsconde';
	$classeNucleo    = 'linhaEsconde';
	$classeTurma     = 'linhaEsconde';
	$classeEstudante = 'linhaEsconde';
	$btnEnviarEstado = '';
	$estadoValHabil  = array();

	// Tratamento de erros
	try{
		// Retorna informações do Polo
		$polomunicipio = buscaPolos( $_SESSION['projovemurbano']['pjuid'] );
		
		// Lista de encaminhamento
		$objLisEnc = new ProjovemUrbanoListaEncaminhamento();
		$objLisEnc->setPerfis( $perfis );
		$objLisEnc->setInfoPolo( $polomunicipio );

		// Seleciona Tipo do Perfil
		// Perfil do tipo: Super Usuário, PFL_EQUIPE_MEC e PFL_CONSULTA
		if( $db->testa_superuser() || in_array(PFL_EQUIPE_MEC, $perfis) || in_array(PFL_CONSULTA, $perfis) ){

			set_time_limit(0);
			ini_set("memory_limit", "15048M");

			$estadoPorPerfil['concluido'] 	= array( WF_ESTADO_DIARIO_PAGAMENTO );
			$estadoPorPerfil['retorno'] 	= array( WF_ESTADO_DIARIO_APROVACAO );
			$estadoPorPerfil['pendente'] 	= array( WF_ESTADO_DIARIO_ABERTO, WF_ESTADO_DIARIO_FECHADO, WF_ESTADO_DIARIO_ENCAMINHAR, WF_ESTADO_DIARIO_VALIDACAO );

			$objLisEnc->inicioPerfilEquipeMec( $estadoPorPerfil );
			$objLisEnc->setTituloPagina( "Estado / Polo / Núcleo / Turma" );
			$objLisEnc->setFormulario( $_REQUEST );
			$objLisEnc->setFormulario( array( 'perid' => $perid ) );
			
			$objLisEnc->setRegistros( listaDeEncaminhamentoPerfilEquipeMEC( $objLisEnc->getArrayDadosFormulario() ) );
			$lista 			 	= $objLisEnc->getRegistros();

			$estadoValHabil     = array( WF_ESTADO_DIARIO_APROVACAO, WF_ESTADO_DIARIO_PAGAMENTO );
			$classeEsfera 	    = 'linhaMostra';
			$btnEnviarEstado    = 'alterarEstadoDiarioCoordenador';

		// Perfil do tipo: PFL_COORDENADOR_MUNICIPAL e PFL_COORDENADOR_ESTADUAL
		}elseif( in_array(PFL_COORDENADOR_MUNICIPAL, $perfis) || in_array(PFL_COORDENADOR_ESTADUAL, $perfis) ){
			
			$estadoPorPerfil['concluido'] 	= array( WF_ESTADO_DIARIO_APROVACAO );
			$estadoPorPerfil['retorno'] 	= $objLisEnc->getPossuiPolo() ? 
												array( WF_ESTADO_DIARIO_VALIDACAO, WF_ESTADO_DIARIO_ENCAMINHAR ) : 
												array( WF_ESTADO_DIARIO_VALIDACAO );

			$estadoPorPerfil['pendente'] 	= $objLisEnc->getPossuiPolo() ? 
												array( WF_ESTADO_DIARIO_FECHADO, WF_ESTADO_DIARIO_ABERTO) : 
												array( WF_ESTADO_DIARIO_FECHADO, WF_ESTADO_DIARIO_ABERTO, WF_ESTADO_DIARIO_ENCAMINHAR);
			
			$objLisEnc->inicioPerfilCoordenador( $estadoPorPerfil );
			$objLisEnc->setTituloPagina( "Polo / Núcleo / Turma" );
			$objLisEnc->setFormulario( array('pjuid'  => $_SESSION['projovemurbano']['pjuid'], 
											 'perid'  => $perid,
											 'usucpf' => $_SESSION['usucpf'] ) );

			$objLisEnc->setRegistros( listaDeEncaminhamentoPerfilCoordenadorEstadual( $objLisEnc->getArrayDadosFormulario() ) );
			$lista 			 	= $objLisEnc->getRegistros();

			$estadoValHabil     = array(WF_ESTADO_DIARIO_VALIDACAO, WF_ESTADO_DIARIO_APROVACAO);
			$btnEnviarEstado    = 'alterarEstadoDiarioCoordenador';
				
			if( $objLisEnc->getPossuiPolo() ){
				$classePolo     = 'linhaMostra';
			}else{
				$classeNucleo   = 'linhaMostra';
			}
		
		// Perfil do tipo: PFL_DIRETOR_POLO
		}elseif( in_array(PFL_DIRETOR_POLO, $perfis) ){
			
			$estadoPorPerfil['concluido'] 	= array( WF_ESTADO_DIARIO_VALIDACAO, WF_ESTADO_DIARIO_APROVACAO, WF_ESTADO_DIARIO_PAGAMENTO );
			$estadoPorPerfil['retorno'] 	= array( WF_ESTADO_DIARIO_FECHADO, WF_ESTADO_DIARIO_ABERTO );
			$estadoPorPerfil['pendente'] 	= array( WF_ESTADO_DIARIO_ENCAMINHAR );

			$objLisEnc->inicioPerfilDiretorPolo( $estadoPorPerfil );
			$objLisEnc->setTituloPagina( "Polo / Núcleo / Turma" );
			$objLisEnc->setFormulario( array('pjuid'  => $_SESSION['projovemurbano']['pjuid'], 
											 'perid'  => $perid,
											 'usucpf' => $_SESSION['usucpf'] ) );
			
			$objLisEnc->setRegistros( listaDeEncaminhamentoPerfilDiretorDePolo( $objLisEnc->getArrayDadosFormulario() ) );
			$lista 			 = $objLisEnc->getRegistros();

			$estadoValHabil  = array( WF_ESTADO_DIARIO_ENCAMINHAR );
			$btnEnviarEstado = 'alterarEstadoDiarioCoordenador';
				
			if( $objLisEnc->getPossuiPolo() ){
				$classePolo   = 'linhaMostra';
			}else{
				$classeNucleo = 'linhaMostra';
			}
		
		// Perfil do tipo: PFL_DIRETOR_NUCLEO
		}elseif( in_array(PFL_DIRETOR_NUCLEO, $perfis) ){
			
			
			$estadoPorPerfil['concluido'] 	= array( WF_ESTADO_DIARIO_ENCAMINHAR, WF_ESTADO_DIARIO_VALIDACAO, WF_ESTADO_DIARIO_APROVACAO, WF_ESTADO_DIARIO_PAGAMENTO ) ;
			$estadoPorPerfil['retorno'] 	= array( WF_ESTADO_DIARIO_FECHADO );
			$estadoPorPerfil['pendente'] 	= array( WF_ESTADO_DIARIO_ABERTO );

			$objLisEnc->inicioPerfilDiretorNucleo( $estadoPorPerfil );
			$objLisEnc->setTituloPagina( "Núcleo / Turma" );
			$objLisEnc->setFormulario( array('pjuid'  => $_SESSION['projovemurbano']['pjuid'],
											 'perid'  => $perid,
											 'usucpf' => $_SESSION['usucpf'] ) );

			$objLisEnc->setRegistros( listaDeEncaminhamentoPerfilDiretorDeNucleo( $objLisEnc->getArrayDadosFormulario() ) );
			$lista 				        = $objLisEnc->getRegistros();
			$btnEnviarEstado            = 'alterarEstadoDiarioCoordenador';
			$estadoValHabil             = array( WF_ESTADO_DIARIO_FECHADO );
			
			if( $objLisEnc->getPossuiPolo() ){
				$classePolo     = 'linhaMostra';
			}else{
				$classeNucleo   = 'linhaMostra';
			}
		
		}else{
			throw new Exception( 'Erro ao definir tipo de perfil.' );
		}

	
	
		function habilitaBotaoEncaminhar( $parametros )
		{
		    $retorno = array('habilitaCheckbox'=>false , 'imgRetorno'=>false, 'habilitaCheckboxEnviar' => false);
		    
		    $msgPendencia    = 'Pendência de fechamento';
		    $msgReabrir      = 'Reabrir Turma(s)';
		    $msgHistorico    = 'Histórico Tramitação';
		    
		    //var_dump( $parametros );
		    
		    if( is_array($parametros['dados']) )
		    {
		        if( !empty($parametros['estuf']) ){
		            
		            $ufAlvo = $parametros['estuf'];
		            
		        }elseif( !empty($parametros['chave_polo']) ){
		
		            $poloAlvo   = $parametros['chave_polo'];
		            
		        }elseif( !empty($parametros['cpfnucleo']) ) 
		        {
		            $nucleoAlvo = $parametros['cpfnucleo'];
		        }
		        
		        $totalNucleos      = 0;
		        $totalTurma        = 0;
		        $totalParaReabrir  = 0;
		        $totalConcluidos   = 0;
		        $totalParaRetornar = 0;
		        $totalPendentes    = 0;
		        $estadoRetorno     = array();
		        $estadoConcluido   = array();
		        $estadoPendente    = array();
		        $chaveNucleo       = 0;
		
		        // Pega estados por Perfil
		        if( in_array(PFL_COORDENADOR_MUNICIPAL, $parametros['perfis'] ) || in_array(PFL_COORDENADOR_ESTADUAL, $parametros['perfis']) )
		        {
		            $estadoConcluido = array( WF_ESTADO_DIARIO_APROVACAO );
		            
		            if( $parametros['possuipolo'] == 'f'){
		            	$estadoRetorno   = array( WF_ESTADO_DIARIO_VALIDACAO, WF_ESTADO_DIARIO_ENCAMINHAR );
		            	$estadoPendente  = array( WF_ESTADO_DIARIO_FECHADO, WF_ESTADO_DIARIO_ABERTO);
		            }else{
		            	$estadoRetorno   = array( WF_ESTADO_DIARIO_VALIDACAO );
		            	$estadoPendente  = array( WF_ESTADO_DIARIO_FECHADO, WF_ESTADO_DIARIO_ABERTO, WF_ESTADO_DIARIO_ENCAMINHAR);
		            }
		        
		        }elseif( in_array(PFL_DIRETOR_POLO, $parametros['perfis']) ) {
		        
		            $estadoConcluido = array( WF_ESTADO_DIARIO_VALIDACAO, WF_ESTADO_DIARIO_APROVACAO, WF_ESTADO_DIARIO_PAGAMENTO );
		            $estadoPendente  = array( WF_ESTADO_DIARIO_FECHADO, WF_ESTADO_DIARIO_ABERTO );
		            $estadoRetorno   = array( WF_ESTADO_DIARIO_ENCAMINHAR );
		        
		        }elseif( in_array(PFL_DIRETOR_NUCLEO, $parametros['perfis']) ) {
		        
		            if( $parametros['possuipolo'] == 't'){
		                $estadoConcluido = array( WF_ESTADO_DIARIO_ENCAMINHAR, WF_ESTADO_DIARIO_VALIDACAO, WF_ESTADO_DIARIO_APROVACAO, WF_ESTADO_DIARIO_PAGAMENTO );
		            }else{
		                $estadoConcluido = array( WF_ESTADO_DIARIO_ENCAMINHAR,  WF_ESTADO_DIARIO_VALIDACAO, WF_ESTADO_DIARIO_APROVACAO, WF_ESTADO_DIARIO_PAGAMENTO );
		            }
		
		            $estadoRetorno   = array( WF_ESTADO_DIARIO_FECHADO );
		            $estadoPendente  = array( WF_ESTADO_DIARIO_ABERTO  );
		
		        }elseif( in_array(PFL_EQUIPE_MEC, $parametros['perfis']) ) {
		        
		            $estadoConcluido = array( WF_ESTADO_DIARIO_PAGAMENTO );
		            $estadoPendente  = array( WF_ESTADO_DIARIO_FECHADO, WF_ESTADO_DIARIO_ABERTO, WF_ESTADO_DIARIO_ENCAMINHAR, WF_ESTADO_DIARIO_VALIDACAO );
		            $estadoRetorno   = array( WF_ESTADO_DIARIO_APROVACAO );
		        }
		
		        foreach ( $parametros['dados'] as $chave=>$valor )
		        {
		            //Total de Núcleos
		            if( $chaveNucleo != $valor['cpfnucleo']){
		                $totalNucleos++;
		            }
		            $chaveNucleo = $valor['cpfnucleo'];
		        
		            if( $valor['estuf'] == $ufAlvo
		                    || $valor['chave_polo'] == $poloAlvo 
		                    || $valor['cpfnucleo'] == $nucleoAlvo ){
		                
		                //Total de estudantes por nucleo
		                if( $valor['cpfnucleo'] == $parametros['cpfnucleo'] ){
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
	
		        // Validação por Polo
		        ///if( isset($poloAlvo) ){
		        
		        // Validação por Núcleo
		        if ( isset($nucleoAlvo) || isset($poloAlvo) ){
		            /*echo "<pre>";
		            //print_r( $estadoPendente );
		            //print_r( $parametros['perfis'] );
		            
		            $qtd = "totalNucleos: ". $totalNucleos. "|  totalParaReabrir: ". $totalParaReabrir . " | totalParaRetornar: " . $totalParaRetornar;
		            $qtd.= " | totalConcluidos: ". $totalConcluidos ." | totalPendentes: ". $totalPendentes ." | totalTurma: ".$totalTurma;
		            var_dump( $qtd );/**/
		            
		            if( ( empty($totalConcluidos) && empty($totalTurma) ) 
		                    || ( $totalPendentes == $totalTurma ) ){
		            
		                $retorno['imgRetorno'] = '<img src="img/workflow_pendente.png" />' ;
		                //$retorno['imgRetorno'].= implode( $estadoPendente );
		                
		            }elseif ( $totalConcluidos == $totalTurma && ( $totalConcluidos > 0 && $totalTurma > 0) ){
		                
		                $retorno['imgRetorno'] = '<img src="img/workflow_concluido.png" />' ;
		                //$retorno['imgRetorno'].= implode( $estadoConcluido );
		                
		            }elseif ( $totalParaRetornar > 0 )
		            {
		                if( !empty($poloAlvo) ){
		                    $retorno['imgRetorno'] = '<img src="img/workflow_reabrir.png" onclick="reabrirDiarioPolo('. $parametros['chave_polo'] .');"  style="cursor:pointer" title="'.$msgReabrir.'" alt="'.$msgReabrir.'" />' ;
		                }else{
		                    $retorno['imgRetorno'] = '<img src="img/workflow_reabrir.png" onclick="reabrirDiarioNucleo('. $parametros['cpfnucleo'] .');"  style="cursor:pointer" title="'.$msgReabrir.'" alt="'.$msgReabrir.'" />' ;
		                } 
		                //$retorno['imgRetorno'].= implode( $estadoRetorno );
		                
		            }elseif ( ($totalConcluidos + $totalPendentes) == $totalTurma ){
		                $retorno['imgRetorno'] = '<img src="img/workflow_pendente.png" />' ;
		                //$retorno['imgRetorno'].= implode( $estadoPendente );
		            }
		            
		            // Quando existir somente um núcleo sem pendência e com diário para reabrir, habilita opção de enviar 
		            if ( $totalParaRetornar > 0 && !in_array(PFL_CONSULTA, $parametros['perfis']) ){
		                $retorno['habilitaCheckboxEnviar'] = true;
		            }
		            
		            if ( ($totalParaReabrir > 0 && $totalNucleos > 1)  && !in_array(PFL_CONSULTA, $parametros['perfis']) ){ 
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
		
		function pegaHistoricoPorTurma( $dados )
		{
		    if( is_array($dados) )
		    {
		        foreach ($dados['dados'] as $chave=>$valor )
		        {
		            if( $valor['turid'] == $dados['turid'] && !empty($valor['cmddsc']) ){
		                return true;
		            }        
		        }
		    }
		    return false;
		}
		
		function quantitativoEstudante( $dados )
		{
		    $retorno = array( 'totalestudante'=>0, 'totalapto'=>0 );
		    
		    if( is_array($dados) )
		    {
		        foreach ($dados['dados'] as $chave=>$valor )
		        {
		            if( $valor['estuf'] == $dados['estuf'] && $dados['flag'] == 'estuf' ){
		                $retorno['totalestudante']++;
		                if( $valor['aptoreceber'] == 'SIM' ){
		                    $retorno['totalapto']++;
		                }
		            }
		            if( $valor['chave_polo'] == $dados['chave_polo'] && $dados['flag'] == 'chave_polo' ){
		                $retorno['totalestudante']++;
		                if( $valor['aptoreceber'] == 'SIM' ){
		                    $retorno['totalapto']++;
		                }
		            }
		            if( $valor['cpfnucleo'] == $dados['cpfnucleo'] && $dados['flag'] == 'cpfnucleo' ){
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
		function criaImgAcao( $parametro )
		{
		    $msgPendencia    = 'Pendência de fechamento';
		    $msgReabrir      = 'Reabrir Turma(s)';
		    $msgHistorico    = 'Histórico Tramitação';
		    $retornaPara     = '';
		
			if( in_array(PFL_COORDENADOR_MUNICIPAL, $parametro['perfis'] ) || in_array(PFL_COORDENADOR_ESTADUAL, $parametro['perfis']) )
			{
			    $retornaPara = WF_ESTADO_DIARIO_ABERTO;
			    
			    if( $parametro['possuipolo'] == 'f' ){
			    	$reabrir  = array( WF_ESTADO_DIARIO_VALIDACAO, WF_ESTADO_DIARIO_ENCAMINHAR );
			    	$pendente = array( WF_ESTADO_DIARIO_FECHADO, WF_ESTADO_DIARIO_ABERTO );
			    }else{
			    	$reabrir  = array( WF_ESTADO_DIARIO_VALIDACAO );
			    	$pendente = array( WF_ESTADO_DIARIO_FECHADO, WF_ESTADO_DIARIO_ENCAMINHAR, WF_ESTADO_DIARIO_ABERTO );
			    }
		
			    // Condição para aplicar imagem
			    if( in_array( $parametro['estadodocumento'], $pendente ) ){
			    
			        // Pendente - Inclui ação do botão
			        $retorno = '<img src="img/workflow_pendente.png"  title="'.$msgPendencia.'" alt="'.$msgPendencia.'" />';
			        //$retorno.= $parametro['estadodocumento'];
			    
			    }elseif( in_array( $parametro['estadodocumento'], $reabrir ) ){
			         
			        $acao 		 = wf_pegarAcao( WF_ESTADO_DIARIO_VALIDACAO, $retornaPara );
				    $parametroJs = "'".	$acao['aedid']			."'"; // aedid
				    $parametroJs.= ", '". $parametro['docid'] 	."'"; // docid
				    $parametroJs.= ", '".$retornaPara ."'"; // esdid
				    $parametroJs.= ", 'Reabrir Diário'"			; // acao
				    $parametroJs.= ", '". $parametro['diaid'] 	."'"; // dados
				
				    // Fechado - Inclui ação do botão
				    $retorno = '<img src="img/workflow_reabrir.png" onclick="reabrirDiario('.$parametroJs.');"  style="cursor:pointer" title="'.$msgReabrir.'" alt="'.$msgReabrir.'" />';
				    //$retorno.= $parametro['estadodocumento'];
				    
			    //}elseif( $parametro['estadodocumento'] == WF_ESTADO_DIARIO_APROVACAO ){
			    //    $retorno = '<img src="img/workflow_concluido.png" />';
			    }else{
			        $retorno = '<img src="img/workflow_concluido.png" />';
			        //$retorno.= $parametro['estadodocumento'];
			    }
			    
			    // Adiciona botão de opção de visualizar historico
			    if( pegaHistoricoPorTurma( $parametro ) && !empty( $parametro['docid'] ) )
			    {
			        $retorno.= '<img src="img/workflow_historico.png" onclick="wf_exibirHistorico('.$parametro['docid'].');" style="cursor:pointer" title="'.$msgHistorico.'" alt="'.$msgHistorico.'" />';
			        //$retorno.= $parametro['estadodocumento'];
			    }
			
			}elseif(in_array(PFL_DIRETOR_POLO, $parametro['perfis'])) {
				/*
				Encaminhar
				
					Encaminhar / Vai para Validação  	(Diretor de Polo) (Reabrir)
					Encaminhar / Retorna para Fechado	(Diretor de Polo)
				*/
			    $retornaPara = WF_ESTADO_DIARIO_ABERTO;
				
				// Condição para aplicar imagem
				if( $parametro['estadodocumento'] == WF_ESTADO_DIARIO_FECHADO || $parametro['estadodocumento'] == WF_ESTADO_DIARIO_ABERTO ){
				
				    // Pendente - Inclui ação do botão
				    $retorno = '<img src="img/workflow_pendente.png"  title="'.$msgPendencia.'" alt="'.$msgPendencia.'" />';
				    //$retorno.= $parametro['estadodocumento'];
				
				}elseif( $parametro['estadodocumento'] == WF_ESTADO_DIARIO_ENCAMINHAR ){
				    	
				    $acao 		 = wf_pegarAcao( WF_ESTADO_DIARIO_ENCAMINHAR, $retornaPara );
				    $parametroJs = "'".	$acao['aedid']			."'"; // aedid
				    $parametroJs.= ", '". $parametro['docid'] 	."'"; // docid
				    $parametroJs.= ", '".$retornaPara 			."'"; // esdid
				    $parametroJs.= ", 'Reabrir Diário'"			; // acao
				    $parametroJs.= ", '". $parametro['diaid'] 	."'"; // dados
				
				    // Fechado - Inclui ação do botão
				    $retorno = '<img src="img/workflow_reabrir.png" onclick="reabrirDiario('.$parametroJs.');"  style="cursor:pointer" title="'.$msgReabrir.'" alt="'.$msgReabrir.'" />';
				    //$retorno.= $parametro['estadodocumento'];
				
				//}elseif( $parametro['estadodocumento'] == WF_ESTADO_DIARIO_VALIDACAO ){
				//    $retorno = '<img src="img/workflow_concluido.png" />';
				}else{
				    $retorno = '<img src="img/workflow_concluido.png" />';
				}
				
				// Adiciona botão de opção de visualizar historico
				if( pegaHistoricoPorTurma( $parametro )
				        && !empty( $parametro['docid'] ) 
				        && $parametro['estadodocumento'] != WF_ESTADO_DIARIO_FECHADO
				        && $parametro['estadodocumento'] != WF_ESTADO_DIARIO_ABERTO )
				{
				    $retorno.= '<img src="img/workflow_historico.png" onclick="wf_exibirHistorico('.$parametro['docid'].');" style="cursor:pointer" title="'.$msgHistorico.'" alt="'.$msgHistorico.'" />';
				   // $retorno.= $parametro['estadodocumento'];
				}
					
			}elseif(in_array(PFL_DIRETOR_NUCLEO, $parametro['perfis'])){
				/*
				Aberto
					Aberto / Vai para Fechado 		(Diretor de Núcleo)
				
				Fechado
					Fechado / Vai para Encaminhar 	(Diretor de Núcleo)
					Fechado / Vai para Validação 	(Diretor de Núcleo)
					Fechado / Retorna para Aberto	(Diretor de Núcleo) (Reabrir)
				*/
				$retornaPara = WF_ESTADO_DIARIO_ABERTO;
				
				// Condição para aplicar imagem
				if( $parametro['estadodocumento'] == WF_ESTADO_DIARIO_ABERTO ){
				
				    // Pendente - Inclui ação do botão
				    $retorno = '<img src="img/workflow_pendente.png"  title="'.$msgPendencia.'" alt="'.$msgPendencia.'" />';
				    //$retorno.= $parametro['estadodocumento'];
				
				}elseif( $parametro['estadodocumento'] == WF_ESTADO_DIARIO_FECHADO ){
				    	
				    $acao 		 = wf_pegarAcao( WF_ESTADO_DIARIO_FECHADO, $retornaPara );
				    $parametroJs = "'".	$acao['aedid']			."'"; // aedid
				    $parametroJs.= ", '". $parametro['docid'] 	."'"; // docid
				    $parametroJs.= ", '".$retornaPara 			."'"; // esdid
				    $parametroJs.= ", 'Reabrir Diário'"			; // acao
				    $parametroJs.= ", '". $parametro['diaid'] 	."'"; // dados
				
				    // Fechado - Inclui ação do botão
				    $retorno = '<img src="img/workflow_reabrir.png" onclick="reabrirDiario('.$parametroJs.');"  style="cursor:pointer" title="'.$msgReabrir.'" alt="'.$msgReabrir.'" />';
				    //$retorno.= $parametro['estadodocumento'];
				
				//}elseif( $parametro['estadodocumento'] == WF_ESTADO_DIARIO_ENCAMINHAR || $parametro['estadodocumento'] == WF_ESTADO_DIARIO_VALIDACAO ){
				}else{
				    $retorno = '<img src="img/workflow_concluido.png" />';
				}
				
				// Adiciona botão de opção de visualizar historico
				if( pegaHistoricoPorTurma( $parametro )  
				        && !empty( $parametro['docid'] ) )
				{
				    $retorno.= '<img src="img/workflow_historico.png" onclick="wf_exibirHistorico('.$parametro['docid'].');" style="cursor:pointer" title="'.$msgHistorico.'" alt="'.$msgHistorico.'" />';
				    //$retorno.= $parametro['estadodocumento'];
				}
				
			}elseif( (in_array(PFL_EQUIPE_MEC, $parametro['perfis']) && !in_array(PFL_CONSULTA, $parametro['perfis']))||in_array(PFL_SUPER_USUARIO, $parametro['perfis']))
			{
			    $retornaPara = WF_ESTADO_DIARIO_ABERTO;
			    // Condição para aplicar imagem
			    if( $parametro['estadodocumento'] == WF_ESTADO_DIARIO_ENCAMINHAR
			            || $parametro['estadodocumento'] == WF_ESTADO_DIARIO_FECHADO
			            || $parametro['estadodocumento'] == WF_ESTADO_DIARIO_ABERTO
			            || $parametro['estadodocumento'] == WF_ESTADO_DIARIO_VALIDACAO ){
			         
			        // Pendente - Inclui ação do botão
			        $retorno = '<img src="img/workflow_pendente.png"  title="'.$msgPendencia.'" alt="'.$msgPendencia.'" />';
			        //$retorno.= $parametro['estadodocumento'];
			         
			    }elseif( $parametro['estadodocumento'] == WF_ESTADO_DIARIO_APROVACAO ){
			
			        $acao 		 = wf_pegarAcao( WF_ESTADO_DIARIO_APROVACAO, $retornaPara );
			        $parametroJs = "'".	$acao['aedid']			."'"; // aedid
			        $parametroJs.= ", '". $parametro['docid'] 	."'"; // docid
			        $parametroJs.= ", '".$retornaPara ."'"; // esdid
			        $parametroJs.= ", 'Reabrir Diário'"			; // acao
			        $parametroJs.= ", '". $parametro['diaid'] 	."'"; // dados
			
			        // Fechado - Inclui ação do botão
			        $retorno = '<img src="img/workflow_reabrir.png" onclick="reabrirDiario('.$parametroJs.');"  style="cursor:pointer" title="'.$msgReabrir.'" alt="'.$msgReabrir.'" />';
			        //$retorno.= $parametro['estadodocumento'];
			
			        //}elseif( $parametro['estadodocumento'] == WF_ESTADO_DIARIO_APROVACAO ){
			        //    $retorno = '<img src="img/workflow_concluido.png" />';
			    }else{
			        $retorno = '<img src="img/workflow_concluido.png" />';
			        //$retorno.= $parametro['estadodocumento'];
			    }
			     
			    // Adiciona botão de opção de visualizar historico
			    if( pegaHistoricoPorTurma( $parametro ) && !empty( $parametro['docid'] ) )
			    {
			        $retorno.= '<img src="img/workflow_historico.png" onclick="wf_exibirHistorico('.$parametro['docid'].');" style="cursor:pointer" title="'.$msgHistorico.'" alt="'.$msgHistorico.'" />';
			        //$retorno.= $parametro['estadodocumento'];
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
				$chaveNucleo  			= 0;
				$chaveTurma   			= 0;
				$nucleoUm     			= '';
				$idLinhaEsfera			= '';
				$idLinhaEstado			= '';
				$idLinhaMunicipio		= '';
			
				foreach ( $lista as $chave => $valor ){

	
					if( $db->testa_superuser() || in_array(PFL_EQUIPE_MEC, $perfis) || in_array(PFL_CONSULTA, $perfis)  ){
						
						// Mostra esfera
						if ( $chaveEsfera != $lista[$chave]['esfera'] )
						{
							$quantitativoNucleo = quantitativoEstudante( array('dados'=>$lista, 'estuf'=>$lista[$chave]['estuf'], 'flag'=>'estuf' ) );
							$idLinhaEsfera = "esfera_" . $lista[$chave]['esfera'];
							?>
							<tr class="listaEsfera <?php echo( $classeEsfera ); ?>"	
										id="<?php echo( $idLinhaEsfera ); ?>">
								<td width="10px">
								    &nbsp; <!-- <input type="checkbox" name='chkuf_<?php echo( $lista[$chave]['esfera'] ); ?>' />  -->
								</td>
								<td colspan="13" 
									class="negrito abreListaDeEstados" style="cursor:pointer" ><?php echo( $lista[$chave]['esfera'] ); ?></td>
								<td class="negrito alinhaTextoCentro"><?php echo( $quantitativoNucleo['totalestudante'] ); ?></td>
								<td class="negrito alinhaTextoCentro"><?php echo( $quantitativoNucleo['totalapto'] ); ?></td>
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
								<td class="negrito alinhaTextoCentro"><?php echo( $quantitativoNucleo['totalestudante'] ); ?></td>
								<td class="negrito alinhaTextoCentro"><?php echo( $quantitativoNucleo['totalapto'] ); ?></td>
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
								<td colspan="9" class="negrito abreListaDePolos" style="cursor:pointer" ><?php echo( $lista[$chave]['mundescricao'] ); ?></td>
								<td class="negrito alinhaTextoCentro"><?php echo( $quantitativoNucleo['totalestudante'] ); ?></td>
								<td class="negrito alinhaTextoCentro"><?php echo( $quantitativoNucleo['totalapto'] ); ?></td>
								<td class="alinhaTextoCentro">-</td>
							</tr>
							<?php 
						}//Fim Mostra Município
					}//Fim Validação Perfil
					
					?>
					
					<?php 
					if ( $chavePolo != $lista[$chave]['chave_polo'] )
					{
					    // Quantitativo
					    $quantitativoNucleo = quantitativoEstudante( array('dados'=>$lista, 'chave_polo'=>$lista[$chave]['chave_polo'], 'flag'=>'chave_polo' ) );
					    
					    // Parâmetros
					    $parHabilitaBotaoPolo['dados']             = $lista;
					    $parHabilitaBotaoPolo['estadodocumento']   = $estadoValHabil;
					    $parHabilitaBotaoPolo['perfis']            = $perfis;
					    $parHabilitaBotaoPolo['possuipolo']        = $polomunicipio['pmupossuipolo'];
					    $parHabilitaBotaoPolo['cpfnucleo']         = $lista[$chave]['cpfnucleo'];
					    $parHabilitaBotaoPolo['diaid']             = $lista[$chave]['diaid'];
					    $parHabilitaBotaoPolo['chave_polo']        = $lista[$chave]['chave_polo'];
					    
					    //if( in_array(PFL_EQUIPE_MEC, $perfis) ){
					    //	$parHabilitaBotaoPolo['estuf']         = $lista[$chave]['estuf'];
					   // }
					    
					    $retornoBtHabilPolo = habilitaBotaoEncaminhar( $parHabilitaBotaoPolo );
					    $idPoloLinha = "polo_".$lista[$chave]['chave_polo'];
						?>
						<tr class="listaPolo <?php echo( $idLinhaMunicipio ); ?> <?php echo( $classePolo ); ?>" 
							id="<?php echo( $idPoloLinha ); ?>" >
		
							<td colspan="5">&nbsp;</td> 
							<td class="espacamentoAcao">
							    <?php 
							    if( $retornoBtHabilPolo['habilitaCheckbox'] ) { 
							    
							        $acaoCheckPolo = 'onclick="habilitaBotaoPolo( '. $lista[$chave]['chave_polo'] .' )"';
							        
							        if(  in_array(PFL_EQUIPE_MEC, $perfis) || $db->testa_superuser() || in_array(PFL_CONSULTA, $perfis) )
							        {
							            $acaoCheckPolo = ''; 
							        }
							        ?>
								    <input <?php echo($acaoCheckPolo); ?>
									     type="checkbox"
									      name='chkpolo_id[]'
									       id='<?php echo( $lista[$chave]['chave_polo'] ); ?>'
									       value='<?php echo( $lista[$chave]['chave_polo'] ); ?>' />
								<?php 
							    }else{
									echo "&nbsp;";	
								} ?>
							</td>
							<td colspan="8" style="cursor:pointer" class="negrito abreListaDeNucleos"><?php echo( $lista[$chave]['polo'] ); ?></td>
							<td class="negrito alinhaTextoCentro"><?php echo( $quantitativoNucleo['totalestudante'] ); ?></td>
							<td class="negrito alinhaTextoCentro"><?php echo( $quantitativoNucleo['totalapto'] ); ?></td>
							<td class="alinhaTextoCentro"><?php 
							echo $retornoBtHabilPolo['imgRetorno'];
							?></td>
						</tr>
						<?php
					}
					
					if ( $chaveNucleo != $lista[$chave]['cpfnucleo'] )
					{
					    $nucleoUm = $lista[$chave]['nucid'];
					    $quantitativoNucleo = quantitativoEstudante( array('dados'=>$lista, 'cpfnucleo'=>$lista[$chave]['cpfnucleo'], 'flag'=>'cpfnucleo' ) );
		
					    // Parâmetros
					    $parHabilitaBotaoNucleo['dados']             = $lista;
					    $parHabilitaBotaoNucleo['estadodocumento']   = $estadoValHabil;
					    $parHabilitaBotaoNucleo['perfis']            = $perfis;
					    $parHabilitaBotaoNucleo['possuipolo']        = $polomunicipio['pmupossuipolo'];
					    $parHabilitaBotaoNucleo['cpfnucleo']         = $lista[$chave]['cpfnucleo'];
					    $parHabilitaBotaoNucleo['diaid']             = $lista[$chave]['diaid'];
					    $parHabilitaBotaoNucleo['nucid']             = $lista[$chave]['nucid'];
					    
		                // Habilita botao encaminhar
		                // Apresenta imagens e suas respectivas funções
					    $retornoBtHabilNucleo = habilitaBotaoEncaminhar( $parHabilitaBotaoNucleo );
					    $idNucleoLinha = "nucleo_".$lista[$chave]['cpfnucleo'];
					    //var_dump( $retornoBtHabilNucleo );
						?>
						<tr class="listaNucleo <?php echo( $idPoloLinha ); ?> <?php echo( $classeNucleo ); ?>" 
									id="<?php echo( $idNucleoLinha ); ?>" >
		
							<td colspan="8">&nbsp;</td>
							<td class="espacamentoAcao">
							    <?php 
							    if( $retornoBtHabilNucleo['habilitaCheckbox'] ) {
							        
							        $acaoCheckNucleo = 'onclick="habilitaBotaoNucleo( '. $lista[$chave]['nucid'] .' )"';
							        
							        if( in_array(PFL_EQUIPE_MEC, $perfis) || $db->testa_superuser() || in_array(PFL_CONSULTA, $perfis) )
							        {
							            $acaoCheckNucleo = ''; 
							        }
							        ?>
								    <input <?php echo($acaoCheckNucleo); ?>
									     type="checkbox"
									      name='chknucleo_id[]'
									       id='<?php echo( $lista[$chave]['cpfnucleo'] ); ?>'
									       value='<?php echo( $lista[$chave]['nucid'] ); ?>'
									       class='chkpolo_<?php echo( $lista[$chave]['chave_polo'] ); ?>' />
								<?php 
							    }else{
									echo "&nbsp;";	
								} ?>
							</td>
							<td colspan="5" class="abreListaDeTurmas negrito" style="cursor:pointer"><?php echo( $lista[$chave]['nucleo'] ); ?></td>
							<td class="negrito alinhaTextoCentro"><?php echo( $quantitativoNucleo['totalestudante'] ); ?></td>
							<td class="negrito alinhaTextoCentro"><?php echo( $quantitativoNucleo['totalapto'] ); ?></td>
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
		
							echo $retornoBtHabilNucleo['imgRetorno'];
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
						<tr class="listaTurmas <?php echo( $idNucleoLinha ); ?> <?php echo( $classeTurma ); ?>" 
									id="<?php echo( $idTurmaLinha ); ?>" >
		
							<td colspan="9">&nbsp;</td>
							<td colspan="5" class="abreListaDeEstudantes negrito"
								style="cursor:pointer"><?php echo( $lista[$chave]['turdesc'] ); ?>
		
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
								$dadosImg['possuipolo']      = $polomunicipio['pmupossuipolo'];
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
							
							<th>Trabalhos Entregues</th>
							<th>Frequência</th>
							<th>Nº de auxílios à receber</th>
							
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
						
						<td class="alinhaTextoCentro celulaEstudante"><?php echo( $lista[$chave]['trabalhosentregues'] ); ?></td>
						<td class="alinhaTextoCentro celulaEstudante"><?php echo( $lista[$chave]['frequencia'] ); ?>%</td>
						<td class="alinhaTextoCentro celulaEstudante"><?php echo( $lista[$chave]['auxilios'] ); ?></td>
						
						<td class="alinhaTextoCentro"><?php echo( $lista[$chave]['aptoreceber'] ); ?></td>
						<td class="alinhaTextoCentro"><?php echo( $lista[$chave]['agencia'] ); ?></td>
						<td class="alinhaTextoCentro"><?php echo( $lista[$chave]['caenispispasep'] ); ?></td>
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
								
					//Seta chave polo
					if( $chavePolo != $lista[$chave]['chave_polo'] ){
						$chavePolo  = $lista[$chave]['chave_polo'];
					}
								
					//Seta chave nucleo
					if( $chaveNucleo != $lista[$chave]['cpfnucleo'] ){
						$chaveNucleo = $lista[$chave]['cpfnucleo'];
					}
					
					//Seta chave da turma
					if( $chaveTurma != $lista[$chave]['turid'] ){
						$chaveTurma = $lista[$chave]['turid'];
					}
				}
		
				// Parâmetros
				//$parHabilitaJsNucleo['nucid']             = $nucleoUm;
				//$parHabilitaJsNucleo['dados']             = $lista;
				//$parHabilitaJsNucleo['estadodocumento']   = array( WF_ESTADO_DIARIO_FECHADO );
				 
				// Habilita botao encaminhar
				//$retornoBtHabilNucleo = habilitaBotaoEncaminharPerfilNucleo( $parHabilitaJsNucleo );
			}
					
		?>
		</table>
		
		<table border="0" width="95%" align="center">
		<?php if( !in_array(PFL_EQUIPE_MEC, $perfis) && !in_array(PFL_CONSULTA, $perfis) ) {?>
			<tr>
				<td colspan="2">
					<div id="chkDivDeclaracao">
						<input type="checkbox" id="declaracao" />
						<input type="checkbox" id="declaracaoPolo" />
						<input type="checkbox" id="declaracaoNucleo" />
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
				    <?php if( !in_array(PFL_EQUIPE_MEC, $perfis) && !in_array(PFL_CONSULTA, $perfis) ) {?>
		    			<div id="btDivEncaminhar">
		    				<input type="button" id="btEncaminharLista" value="Validar/Encaminhar" />
		    				<input type="button" id="btEncaminharListaNucleo" value="Validar/Encaminhar" />
		    				<input type="button" id="btEncaminharListaPolo" value="Validar/Encaminhar" />
		    			</div>
					<?php }elseif( in_array(PFL_EQUIPE_MEC, $perfis) && !in_array(PFL_CONSULTA, $perfis) ){?>
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
             
             	var janela = window.open("?modulo=principal/popHistoricoDiario&acao=A&diaid="+diaid, "popHistoricoDiario", "menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=10,top=10,width=800,height=200");
     		}
		
		    function mantemConsulta()
		    {
		    	<?php if( in_array(PFL_EQUIPE_MEC, $perfis) || in_array(PFL_CONSULTA, $perfis) ) {?>
		
		            if( $('#frmEncaminharLista').valid() == true )
		            {
		    			//Gatinho para selecionar o que já "está" selecionado
		    			
		    			$('#estuf option[value != ""]').attr('selected','selected');
		    			$('#polid option[value != ""]').attr('selected','selected');
		    			$('#nucid option[value != ""]').attr('selected','selected');
		    
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
		
			function habilitaBotaoNucleo( idNucleo )
			{
				$("#declaracao").hide();
				$("#declaracaoPolo").hide();
				$("#declaracaoNucleo").show();
				
				$("#chkDivDeclaracao").show();
				
				$("#btEncaminharLista").hide();
				$("#btEncaminharListaNucleo").show();
			}
		
			function reabrirDiario( aedid, docid, esdid, acaodescricao, dados )
			{
				if ( !confirm( 'Deseja realmente ' + acaodescricao + ' ?' ) )
				{
					return;
				}
		
				var nomeRandomico = "idBtn_" + Math.floor( Math.random() * 11 );
		        var params = {
		        	    acao           : 'reabrirDiario',
		        		aedid          : aedid, 
		        		docid          : docid,
		        		esdid          : esdid,
		        		dados          : dados
		            };
		
		        var botaoJustificativa = $("<input/>"
		                , { type    : 'button'
		                    , value : 'Enviar'
		                    , id    : nomeRandomico
		                    , click : function(){
		
		            			params.justificativa = $('#textJustificativa').val();
		
		            		    $.get( "geral/ajax.php", params, function(resposta){
		                            var objRetorno = jQuery.parseJSON( resposta );
		                            alert( objRetorno.retorno );
		                            mantemConsulta();
		                            $(".ui-dialog").hide();
		                        });
		                    }
		                });
		
		        $("#labelBtJustificativa").html('');
		        $("#labelBtJustificativa").append( botaoJustificativa );
		
				$( "#boxJustificativa" ).dialog({
		            height: 200,
		            width:  400,
		            close: function(){
		            	$( '#textJustificativa' ).val('');
		            }
		        });
			}
		
			function reabrirDiarioNucleo( idCpfNucleo )
			{
		
				if ( !confirm( 'Deseja realmente Reabrir o(s) Diário(s)?' ) )
				{
					return;
				}
		
		        var objParametros          = {};
		        objParametros.acao         = 'reabrirDiario',
		        objParametros.fluxo        = 'nucleo',
		        objParametros.idcpfnucleo  = idCpfNucleo;
		        objParametros.diarioid     = new Array();
		
		        var nomeRandomico          = "idBtn_" + Math.floor( Math.random() * 11 );
		        var contadorDiario         = 0;
		
				var botaoJustificativa = $("<input/>"
		                , { type    : 'button'
		                    , value : 'Enviar'
		                    , id    : nomeRandomico
		                    , click : function(){
		
		            			objParametros.justificativa = $('#textJustificativa').val();
		
		            			nomeClasseNucleo = 'tr.nucleo_'+ objParametros.idcpfnucleo +' td input[name^="chkdiario"]';
		            			
		            			$( nomeClasseNucleo ).each(function(idx1, ele1){
		
		            				objParametros.diarioid[ contadorDiario ] = $(ele1).val();
		            				contadorDiario++;
		            			});
		
		            		    $.get( "geral/ajax.php", objParametros, function(resposta){
		                            var objRetorno = jQuery.parseJSON( resposta );
		                            mantemConsulta();
		                            alert( objRetorno.retorno );
		                            $(".ui-dialog").hide();
		                        });
		                    }
		                });
		
		        $("#labelBtJustificativa").html('');
		        $("#labelBtJustificativa").append( botaoJustificativa );
		
				$( "#boxJustificativa" ).dialog({
		            height: 200,
		            width:  400,
		            close: function(){
		            	$( '#textJustificativa' ).val('');
		            }
		        });
			}
		
			function reabrirDiarioPolo( idPolo )
			{
				if ( !confirm( 'Deseja realmente Reabrir o(s) Diário(s) ?' ) )
				{
					return;
				}
		
		        var objParametros      = {};
		        objParametros.acao     = 'reabrirDiario',
		        objParametros.fluxo    = 'polo',
		        objParametros.polid    = idPolo;
		        objParametros.diarioid = new Array();
		        
		        var nomeRandomico = "idBtn_" + Math.floor( Math.random() * 11 );
		        var contadorDiario = 0;
		
		        var botaoJustificativa = $("<input/>"
		                , { type    : 'button'
		                    , value : 'Enviar'
		                    , id    : nomeRandomico
		                    , click : function(){
		
		            			objParametros.justificativa = $('#textJustificativa').val();
		            			            			
		            			var nucleos = $('tr[class*="polo_'+ idPolo +'"][class*="listaNucleo"]');
		            			var turmas  = [];
		
		            			nucleos.each(function( idx, el ){
		            			    turmas = $('tr[class*="listaTurmas"][class*="'+ $(el).attr('id') +'"]');    
		            			    turmas.each(function(idx, elTurma ){
		            			        //console.log(  $(elTurma).find('input[name^="chkdiario"]') );
		            			        objParametros.diarioid[ contadorDiario ] =  $(elTurma).find('input[name^="chkdiario"]').val();
		                				contadorDiario++;
		            			    });
		            			});
		
		            		    $.get( "geral/ajax.php", objParametros, function(resposta){
		                            var objRetorno = jQuery.parseJSON( resposta );
		                            mantemConsulta();
		                            alert( objRetorno.retorno );
		                            $(".ui-dialog").hide();
		                        });
		                    }
		                });
		
		        $("#labelBtJustificativa").html('');
		        $("#labelBtJustificativa").append( botaoJustificativa );
		
				$( "#boxJustificativa" ).dialog({
		            height: 200,
		            width:  400,
		            close: function(){
		            	$( '#textJustificativa' ).val('');
		            }
		        });
			}
		
			function wf_exibirHistorico( docid )
			{
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
			    
				<?php if( $retornoBtHabilNucleo['habilitaCheckboxEnviar'] ){ ?>
			    $("#chkDivDeclaracao").show();
				<?php } ?>
		
				<?php if( in_array(PFL_EQUIPE_MEC, $perfis) && in_array(PFL_CONSULTA, $perfis) ) {?>
				
		    		$("input[name^='chkpolo_']").change(function (){
		    
		    		    var classeCheckPoloUsuarioMEC;
		    
		    		    $("input[name^='chkpolo_']").each(function(idx, ele){
		    		        
		    		        classeCheckPoloUsuarioMEC = '.chkpolo_'+ $(ele).attr('id');
		    		        
		    		        if( $(ele).is(':checked') )
		    		        {
		    		            $( classeCheckPoloUsuarioMEC ).attr( { checked:'checked'} );
		    
		    		        }else{
		    		            $( classeCheckPoloUsuarioMEC ).removeAttr('checked');        
		    		        }
		    		    });
		    		});
				
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
		            
		            if( id === undefined )
		            {
		                return false;
		            }
		            
		            filhos = $('#'+ id).nextAll('tr[class*="'+  id +'"]');
		            
		            if( exibeFilho == 0 )
		            {
		                $('#'+ id).nextAll('tr[class*="'+  id +'"]').hide();
		            }
		            else
		            {
		                $('#'+ id).nextAll('tr[class*="'+  id +'"]').show();
		            }
		            
		            if( filhos.length > 0 && exibeFilho == 0 )
		            {
		                filhos.each(function(idx, el){
		                    mostraEscondeLista(el, exibeFilho);
		                });
		            }
				};
		
				$('.abreListaDeEstudantes').click(function(){
		        
		            var idTrNucleo   =  $(this).parent('tr').attr('id')
		                , exibeFilho = 0;
		            
		            if( !$('#'+ idTrNucleo).nextAll('tr[class*="'+  idTrNucleo +'"]').is(':visible') )
		            {
		                exibeFilho = 1;
		            }
					
					mostraEscondeLista( $(this).parent('tr'), exibeFilho  );
				});
		
				$('.abreListaDeTurmas').click(function(){
		            var idTrNucleo   =  $(this).parent('tr').attr('id')
		                , exibeFilho = 0;
		            
		            if( !$('#'+ idTrNucleo).nextAll('tr[class*="'+  idTrNucleo +'"]').is(':visible') )
		            {
		                exibeFilho = 1;
		            }
					mostraEscondeLista( $(this).parent('tr') , exibeFilho );
				});
		
				$('.abreListaDeNucleos').click(function(){
		            
		            var idTrNucleo   =  $(this).parent('tr').attr('id')
		                , exibeFilho = 0;
		            
		            if( !$('#'+ idTrNucleo).nextAll('tr[class*="'+  idTrNucleo +'"]').is(':visible') )
		            {
		                exibeFilho = 1;
		            }
					
					mostraEscondeLista( $(this).parent('tr'), exibeFilho );
				});
		
				$('.abreListaDePolos').click(function(){
		            
		             var idTrNucleo   =  $(this).parent('tr').attr('id')
		                , exibeFilho = 0;
		            
		            if( !$('#'+ idTrNucleo).nextAll('tr[class*="'+  idTrNucleo +'"]').is(':visible') )
		            {
		                exibeFilho = 1;
		            }
					
					mostraEscondeLista( $(this).parent('tr'), exibeFilho );
				});
		
				$('.abreListaDeEstados').click(function(){
		            
		            var idTrNucleo   =  $(this).parent('tr').attr('id')
		               , exibeFilho = 0;
		           
		           if( !$('#'+ idTrNucleo).nextAll('tr[class*="'+  idTrNucleo +'"]').is(':visible') )
		           {
		               exibeFilho = 1;
		           }
					
					mostraEscondeLista( $(this).parent('tr'), exibeFilho );
				});
	
				$('.abreListaDeEsfera').click(function(){
		            
		            var idTrNucleo  = $(this).parent('tr').attr('id')
		               , exibeFilho = 0;
		           
		           if( !$('#'+ idTrNucleo).nextAll('tr[class*="'+  idTrNucleo +'"]').is(':visible') )
		           {
		               exibeFilho = 1;
		           }
					
					mostraEscondeLista( $(this).parent('tr'), exibeFilho );
				});
	
				
		
				$('table#tabelaListaDeEcaminhamento tr[class*="abreListaDeNucleos"]:odd').css("background-color", "#ffffff");
		 		$(".linhaEsconde").hide();
		 		$(".linhaMostra").show();
		 		
				//$("#btDivEncaminhar").hide();
				$("#declaracaoNucleo").hide();
				$("#declaracaoPolo").hide();
				
				$("#btEncaminharLista").attr('disabled', 'disabled');
				$("#btEncaminharListaNucleo").attr('disabled', 'disabled');
				$("#btEncaminharListaPolo").attr('disabled', 'disabled');
				$("#btEncaminharListaNucleo").hide();
				$("#btEncaminharListaPolo").hide();
		
				$("#declaracao").click(function(){
					if( $("#declaracao").is(":checked") ){
						$("#btEncaminharLista").removeAttr('disabled');
					}else{
						$("#btEncaminharLista").attr('disabled', 'disabled');
					}
				});
		
				$("#declaracaoPolo").click(function(){
					if( $("#declaracaoPolo").is(":checked") ){
						$("#btEncaminharListaPolo").removeAttr('disabled');
					}else{
						$("#btEncaminharListaPolo").attr('disabled', 'disabled');
					}
				});
		
				$("#declaracaoNucleo").click(function(){
					if( $("#declaracaoNucleo").is(":checked") ){
						$("#btEncaminharListaNucleo").removeAttr('disabled');
					}else{
						$("#btEncaminharListaNucleo").attr('disabled', 'disabled');
					}
				});
		
				$("#btEncaminharListaPolo").click(function(){
					
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
		
					//console.log( objParametros );
		
		            $.post( 'geral/ajax.php', objParametros, function(response){
		                var objRetorno = jQuery.parseJSON( response );
						mantemConsulta();
		                alert( objRetorno.retorno );
						//document.location.reload(true);
		            }, 'html' );
				});
		
				$("#btEncaminharListaNucleo").click(function(){
		
					if ( !confirm( 'Confirma encaminhamento da lista dos estudantes selecionados?' ) )
					{
						return;
					}
		
					var objParametros       = {};
					var contadorTurma       = 0;
					var nomeClasseNucleo    = 0;
					
				    objParametros.acao      = '<?php echo($btnEnviarEstado); ?>';
				    objParametros.perid     = <?php echo($perid); ?>;
				    objParametros.turma_id  = new Array();
				    
		
				    $('input[name^=chknucleo_id]:checked').each(function(idx, ele){
						
						nomeClasseNucleo = 'tr.nucleo_'+ $(ele).attr('id') +' td input[name^="chkturma_id"]';
						
						$( nomeClasseNucleo ).each(function(idx1, ele1){
		
							objParametros.turma_id[ contadorTurma ] = $(ele1).val();
						    contadorTurma++;
						});
				    });
		
				    if( objParametros.turma_id.length == 0 ){
					    alert('Selecione um Núcleo');
				    }else{
					    
		    		    $.post( 'geral/ajax.php', objParametros, function(response){
		                    var objRetorno = jQuery.parseJSON( response );
		                    mantemConsulta();
		                    alert( objRetorno.retorno );
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
					var nomeClasseNucleo    = 0;
					var nomeDoBotao			= $("#btEncaminharListaGeral").val();
					$("#btEncaminharListaGeral").val('Carregando...');
					$("#btEncaminharListaGeral").attr('disabled','disabled');
					
				    objParametros.acao      = '<?php echo($btnEnviarEstado); ?>';
				    objParametros.perid     = <?php echo($perid); ?>;
				    objParametros.turma_id  = new Array();
				    
		
				    $('input[name^=chknucleo_id]:checked').each(function(idx, ele){
						
						nomeClasseNucleo = 'tr.nucleo_'+ $(ele).attr('id') +' td input[name^="chkturma_id"]';
						
						$( nomeClasseNucleo ).each(function(idx1, ele1){
		
							objParametros.turma_id[ contadorTurma ] = $(ele1).val();
						    contadorTurma++;
						});
				    });
		
				    if( objParametros.turma_id.length == 0 ){
					    alert('Selecione um Núcleo');
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