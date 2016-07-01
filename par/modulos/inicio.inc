<?php

unset($_SESSION['par']['boPerfilSuperUsuario'],
	  $_SESSION['par']['boAbaMunicipio'],
	  $_SESSION['par']['boAbaEstado'],
	  $_SESSION['par']['arMuncod'],
	  $_SESSION['par']['arEstuf'],
	  $_SESSION['par']['muncod'],
	  $_SESSION['par']['estuf'],
	  $_SESSION['par']['itrid'],
	  $_SESSION['par']['inuid'],
	  $_SESSION['par']['boPerfilConsulta']
);

# itrid: 1 = estado, 2 = município

/*
 * PERFILS
 */
if( possuiPerfil(array(PAR_PERFIL_SUPER_USUARIO, 
					   PAR_PERFIL_ADMINISTRADOR,
					   PAR_PERFIL_ALTA_GESTAO_MEC,
					   PAR_PERFIL_EQUIPE_ANCEIRA,
					   PAR_PERFIL_EQUIPE_TEFINCNICA,
					   PAR_PERFIL_ANALISTA_MERITOS,					   
					   PAR_PERFIL_PROFUNC_ANALISEPF					   
					   ) 
				)){
	$_SESSION['par']['boPerfilSuperUsuario'] = true;	

	header( "Location: par.php?modulo=principal/listaMunicipios&acao=A" );
} else {
	
	if(possuiPerfil(PAR_PERFIL_NUTRICIONISTA) ){
		header( "Location: par.php?modulo=principal/vinculacaoNutricionista&acao=A" );
		die;
	}
	elseif( possuiPerfil(PAR_PERFIL_SECRETARIO_ESTADUAL_EDUCACAO) || possuiPerfil(PAR_PERFIL_SECRETARIO_ESTADUAL_SAUDE) ){
		header( "Location: par.php?modulo=principal/listaEstados&acao=A" );
		die;
	}elseif(possuiPerfil(PAR_PERFIL_UNIVERSIDADE_ESTADUAL) || possuiPerfil(PAR_PERFIL_ENTIDADE_EXECUTORA) ){
		header( "Location: par.php?modulo=principal/documentosParaValidar&acao=A" );
		die;
	}
	elseif(possuiPerfil(PAR_PERFIL_EQUIPE_MUNICIPAL) ){
		if($arMuncod = pegaMunicipioAssociado(PAR_PERFIL_EQUIPE_MUNICIPAL) ){
			$_SESSION['par']['boAbaMunicipio'] = true;
			if(count($arMuncod) > 1){
				$arMuncod = ($arMuncod) ? $arMuncod : array();
				$arMuncodTemp = array();
				foreach($arMuncod as $muncod){
					$arMuncodTemp[] = $muncod['muncod'];
				}
				$_SESSION['par']['arMuncod'] = $arMuncodTemp;
				header( "Location: par.php?modulo=principal/listaMunicipios&acao=A" );				
			} else {
				$oInstrumentoUnidade = new InstrumentoUnidade();
				$inuid = $oInstrumentoUnidade->verificaInuidMunicipio($arMuncod[0]['muncod']);
				if(!$inuid){
					$post['inuid'] 		 = $inuid;
					$post['entidadePar'] = 'municipio';
					$post['muncod'] 	 = $arMuncod[0]['muncod'];
					$post['estuf'] 		 = $arMuncod[0]['estuf'];
					
					$obEntidadeParControle = new EntidadeParControle();
					$inuid = $obEntidadeParControle->gravaInstrumentoUnidade($post);
				} else {
					$_SESSION['par']['inuid']  = $inuid;				
					$_SESSION['par']['muncod'] = $arMuncod[0]['muncod'];
					$_SESSION['par']['itrid']  = 2;
					
					//verificaPreenchimentoDadosUnidadePorData();
					//header( "Location: par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=arvore" );	
					header( "Location: par.php?modulo=principal/administracaoDocumentos&acao=A" );
				}
			}
			die;
		} else {
			mensagemAcossiacao('Município');
		}
	} else if(possuiPerfil(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO) ){
		if( $arMuncod = pegaMunicipioAssociado(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO) ){
			$_SESSION['par']['boAbaMunicipio'] = true;
			if(count($arMuncod) > 1){
				$arMuncod = ($arMuncod) ? $arMuncod : array();
				$arMuncodTemp = array();
				foreach($arMuncod as $muncod){
					$arMuncodTemp[] = $muncod['muncod'];
				}
				$_SESSION['par']['arMuncod'] = $arMuncodTemp;
				header( "Location: par.php?modulo=principal/listaMunicipios&acao=A" );				
			} else {
				$oInstrumentoUnidade = new InstrumentoUnidade();
				$inuid = $oInstrumentoUnidade->verificaInuidMunicipio($arMuncod[0]['muncod']);
				if(!$inuid){
					$post['inuid'] 		 = $inuid;
					$post['entidadePar'] = 'municipio';
					$post['muncod'] 	 = $arMuncod[0]['muncod'];
					$post['estuf'] 		 = $arMuncod[0]['estuf'];
					
					$obEntidadeParControle = new EntidadeParControle();
					$inuid = $obEntidadeParControle->gravaInstrumentoUnidade($post);
				} else {
					$_SESSION['par']['inuid']  = $inuid;				
					$_SESSION['par']['muncod'] = $arMuncod[0]['muncod'];
					$_SESSION['par']['itrid']  = 2;
					
					//verificaPreenchimentoDadosUnidadePorData();
					//header( "Location: par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=arvore" );	
					header( "Location: par.php?modulo=principal/administracaoDocumentos&acao=A" );	
				}
			}
			die;
		} else {
			mensagemAcossiacao('Município');
		}
	} elseif(possuiPerfil(PAR_PERFIL_PREFEITO)){
		if($arMuncod = pegaMunicipioAssociado(PAR_PERFIL_PREFEITO)){
			$_SESSION['par']['boAbaMunicipio'] = true;
			if(count($arMuncod) > 1){
				$arMuncod = ($arMuncod) ? $arMuncod : array();
				$arMuncodTemp = array();
				foreach($arMuncod as $muncod){
					$arMuncodTemp[] = $muncod['muncod'];
				}
				$_SESSION['par']['arMuncod'] = $arMuncodTemp;
				header( "Location: par.php?modulo=principal/listaMunicipios&acao=A" );				
			} else {
				$oInstrumentoUnidade = new InstrumentoUnidade();
				$inuid = $oInstrumentoUnidade->verificaInuidMunicipio($arMuncod[0]['muncod']);
				if(!$inuid){
					$post['inuid'] 		 = $inuid;
					$post['entidadePar'] = 'municipio';
					$post['muncod'] 	 = $arMuncod[0]['muncod'];
					$post['estuf'] 		 = $arMuncod[0]['estuf'];
					
					$obEntidadeParControle = new EntidadeParControle();
					$inuid = $obEntidadeParControle->gravaInstrumentoUnidade($post);
				} else {
					$_SESSION['par']['inuid']  = $inuid;				
					$_SESSION['par']['muncod'] = $arMuncod[0]['muncod'];
					$_SESSION['par']['itrid']  = 2;
					
					//verificaPreenchimentoDadosUnidadePorData();
					//header( "Location: par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=arvore" );	
					header( "Location: par.php?modulo=principal/administracaoDocumentos&acao=A" );
				}
			}
			die;
		} else {
			mensagemAcossiacao('Município');
		}
	} elseif(possuiPerfil(PAR_PERFIL_EQUIPE_ESTADUAL_BRASIL_PRO)){ 
		
		if($arEstuf = pegaEstadoAssociado(PAR_PERFIL_EQUIPE_ESTADUAL_BRASIL_PRO)){
			$_SESSION['par']['boAbaEstado'] = true;
			if(count($arEstuf) > 1){
				$arEstuf = ($arEstuf) ? $arEstuf : array();
				$arEstufTemp = array();
				foreach($arEstuf as $estuf){
					$arEstufTemp[] = $estuf['estuf'];
				}
				$_SESSION['par']['arEstuf'] = $arEstufTemp;
				header( "Location: par.php?modulo=principal/listaEstados&acao=A" );				
			} else {
				$oInstrumentoUnidade = new InstrumentoUnidade();
				$inuid = $oInstrumentoUnidade->verificaInuidEstado($arEstuf[0]['estuf']);
				if(!$inuid){
					$post['inuid'] 		 = $inuid;
					$post['entidadePar'] = 'estado';
					$post['estuf'] 		 = $arEstuf[0]['estuf'];
					
					$obEntidadeParControle = new EntidadeParControle();
					$inuid = $obEntidadeParControle->gravaInstrumentoUnidade($post);
				} else {
					$_SESSION['par']['inuid']  = $inuid;
					$_SESSION['par']['estuf'] = $arEstuf[0]['estuf'];
					$_SESSION['par']['itrid'] = 1;
					
					//verificaPreenchimentoDadosUnidadePorData();
					//header( "Location: par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=arvore" );	
					header( "Location: par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=programa" );
				}
			}
			die;
		} else {
			mensagemAcossiacao('Estado');
		}
	} elseif(possuiPerfil(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO_BRASIL_PRO)){
		if($arEstuf = pegaEstadoAssociado(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO_BRASIL_PRO)){
			$_SESSION['par']['boAbaEstado'] = true;
			if(count($arEstuf) > 1){
				$arEstuf = ($arEstuf) ? $arEstuf : array();
				$arEstufTemp = array();
				foreach($arEstuf as $estuf){
					$arEstufTemp[] = $estuf['estuf'];
				}
				$_SESSION['par']['arEstuf'] = $arEstufTemp;
				header( "Location: par.php?modulo=principal/listaEstados&acao=A" );				
			} else {
				$oInstrumentoUnidade = new InstrumentoUnidade();
				$inuid = $oInstrumentoUnidade->verificaInuidEstado($arEstuf[0]['estuf']);
				if(!$inuid){
					$post['inuid'] 		 = $inuid;
					$post['entidadePar'] = 'estado';
					$post['estuf'] 		 = $arEstuf[0]['estuf'];
					
					$obEntidadeParControle = new EntidadeParControle();
					$inuid = $obEntidadeParControle->gravaInstrumentoUnidade($post);
				} else {
					$_SESSION['par']['inuid']  = $inuid;
					$_SESSION['par']['estuf'] = $arEstuf[0]['estuf'];
					$_SESSION['par']['itrid'] = 1;
					
					//verificaPreenchimentoDadosUnidadePorData();
					//header( "Location: par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=arvore" );	
					header( "Location: par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=programa" );
				}
			}
			die;
		} else {
			mensagemAcossiacao('Estado');
		}
	} elseif(possuiPerfil(PAR_PERFIL_EQUIPE_ESTADUAL) || 
			possuiPerfil(PAR_PERFIL_SECRETARIO_ESTADUAL_EDUCACAO) || 
			possuiPerfil(PAR_PERFIL_SECRETARIO_ESTADUAL_SAUDE)){

				if(possuiPerfil(PAR_PERFIL_EQUIPE_ESTADUAL)){
					$arEstuf = pegaEstadoAssociado(PAR_PERFIL_EQUIPE_ESTADUAL);
				} else if(possuiPerfil(PAR_PERFIL_SECRETARIO_ESTADUAL_EDUCACAO)){
					$arEstuf = pegaEstadoAssociado(PAR_PERFIL_SECRETARIO_ESTADUAL_EDUCACAO);
				} elseif(possuiPerfil(PAR_PERFIL_SECRETARIO_ESTADUAL_SAUDE)){
					$arEstuf = pegaEstadoAssociado(PAR_PERFIL_SECRETARIO_ESTADUAL_SAUDE);
				}
				
				if( $arEstuf ){
					$_SESSION['par']['boAbaEstado'] = true;

					if(count($arEstuf) > 1){
						$arEstuf = ($arEstuf) ? $arEstuf : array();
						$arEstufTemp = array();
						foreach($arEstuf as $estuf){
							$arEstufTemp[] = $estuf['estuf'];
						}
						$_SESSION['par']['arEstuf'] = $arEstufTemp;
						header( "Location: par.php?modulo=principal/listaEstados&acao=A" );				
					} else {
						$oInstrumentoUnidade = new InstrumentoUnidade();
						$inuid = $oInstrumentoUnidade->verificaInuidEstado($arEstuf[0]['estuf']);
						if(!$inuid){
							$post['inuid'] 		 = $inuid;
							$post['entidadePar'] = 'estado';
							$post['estuf'] 		 = $arEstuf[0]['estuf'];
							
							$obEntidadeParControle = new EntidadeParControle();
							$inuid = $obEntidadeParControle->gravaInstrumentoUnidade($post);
						} else {
							$_SESSION['par']['inuid']  = $inuid;
							$_SESSION['par']['estuf'] = $arEstuf[0]['estuf'];
							$_SESSION['par']['itrid'] = 1;
							
							//verificaPreenchimentoDadosUnidadePorData();
							//header( "Location: par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=arvore" );	
							header( "Location: par.php?modulo=principal/administracaoDocumentos&acao=A" );
						}
					}
					die;
				} else {
					mensagemAcossiacao('Estado');
				}
				
	} elseif(possuiPerfil(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO)){
		
		if($arEstuf = pegaEstadoAssociado(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO)){
			$_SESSION['par']['boAbaEstado'] = true;
			if(count($arEstuf) > 1){
				$arEstuf = ($arEstuf) ? $arEstuf : array();
				$arEstufTemp = array();
				foreach($arEstuf as $estuf){
					$arEstufTemp[] = $estuf['estuf'];
				}
				$_SESSION['par']['arEstuf'] = $arEstufTemp;
				header( "Location: par.php?modulo=principal/listaEstados&acao=A" );				
			} else {
				$oInstrumentoUnidade = new InstrumentoUnidade();
				$inuid = $oInstrumentoUnidade->verificaInuidEstado($arEstuf[0]['estuf']);
				if(!$inuid){
					$post['inuid'] 		 = $inuid;
					$post['entidadePar'] = 'estado';
					$post['estuf'] 		 = $arEstuf[0]['estuf'];
					
					$obEntidadeParControle = new EntidadeParControle();
					$inuid = $obEntidadeParControle->gravaInstrumentoUnidade($post);
				} else {
					$_SESSION['par']['inuid']  = $inuid;
					$_SESSION['par']['estuf'] = $arEstuf[0]['estuf'];
					$_SESSION['par']['itrid'] = 1;
					
					//verificaPreenchimentoDadosUnidadePorData();
					//header( "Location: par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=arvore" );	
					header( "Location: par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=programa" );
				}
			}
			die;
		} else {
			mensagemAcossiacao('Estado');
		}
	}elseif(possuiPerfil(PAR_PERFIL_ENGENHEIRO_FNDE)){
		header( "Location: par.php?modulo=principal/listaMunicipios&acao=A" );
		exit();				
	}elseif(possuiPerfil(PAR_PERFIL_CONSULTA)){
		$_SESSION['par']['boPerfilConsulta'] = true;
		
	}elseif(possuiPerfil(PAR_PERFIL_EMPENHADOR)){
		header( "Location: par.php?modulo=principal/empenhoPar&acao=A" );
		exit();
	
		/*
	}elseif(possuiPerfil( array(PAR_PERFIL_EQUIPE_TECNICA,PAR_PERFIL_EQUIPE_FINANCEIRA))){ //PAR_PERFIL_EQUIPE_TECNICA
		//if($arMuncod = pegaMunicipioAssociadoAnalista()){
		if($arMuncod = pegaMunicipioAssociadoAnalista( array(PAR_PERFIL_EQUIPE_TECNICA,PAR_PERFIL_EQUIPE_FINANCEIRA) )){
			if(count($arMuncod) > 1){
				$arMuncod = ($arMuncod) ? $arMuncod : array();
				$arMuncodTemp = array();
				foreach($arMuncod as $muncod){
					$arMuncodTemp[] = $muncod['muncod'];
				}
				foreach($arMuncod as $muncod){
					$arEstufTemp[] = $muncod['estuf'];
				}
				$_SESSION['par']['arMuncod'] = $arMuncodTemp;
				$_SESSION['par']['arEstuf'] = $arEstufTemp;
				if( $arMuncodTemp[0] ){
					header( "Location: par.php?modulo=principal/listaMunicipios&acao=A" );
				} else {
					header( "Location: par.php?modulo=principal/listaEstados&acao=A" );						
				}
			} else {
				$oInstrumentoUnidade = new InstrumentoUnidade();
				$inuid = $arMuncod[0]['muncod'] ? $oInstrumentoUnidade->verificaInuidMunicipio($arMuncod[0]['muncod']) : $oInstrumentoUnidade->verificaInuidEstado($arMuncod[0]['estuf']);
				if(!$inuid){
					$post['inuid'] 		 = $inuid;
					$post['entidadePar'] = 'municipio';
					$post['muncod'] 	 = $arMuncod[0]['muncod'];
					$post['estuf'] 		 = $arMuncod[0]['estuf'];
					
					$obEntidadeParControle = new EntidadeParControle();
					$inuid = $obEntidadeParControle->gravaInstrumentoUnidade($post);
				} else {
					$_SESSION['par']['inuid']  = $inuid;				
					$_SESSION['par']['muncod'] = $arMuncod[0]['muncod'];
					$_SESSION['par']['estuf'] = $arMuncod[0]['estuf'];
					$_SESSION['par']['itrid']  = $arMuncod[0]['muncod'] ? 2 : 1;
					//header( "Location: par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=arvore" );	
					header( "Location: par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=programa" );					
				}
			}
			die;
		} else {
			mensagemAcossiacao('Município');
		}
*/
	}elseif(possuiPerfil(PAR_PERFIL_CONSULTA_MUNICIPAL)){
		$_SESSION['par']['boPerfilConsulta'] = true;
		if($arMuncod = pegaMunicipioAssociado(PAR_PERFIL_CONSULTA_MUNICIPAL)){
			$_SESSION['par']['boAbaMunicipio'] = true;
			if(count($arMuncod) > 1){
				$arMuncod = ($arMuncod) ? $arMuncod : array();
				$arMuncodTemp = array();
				foreach($arMuncod as $muncod){
					$arMuncodTemp[] = $muncod['muncod'];
				}
				$_SESSION['par']['arMuncod'] = $arMuncodTemp;
				header( "Location: par.php?modulo=principal/listaMunicipios&acao=A" );				
			} else {
				$oInstrumentoUnidade = new InstrumentoUnidade();
				$inuid = $oInstrumentoUnidade->verificaInuidMunicipio($arMuncod[0]['muncod']);
				if(!$inuid){
					$post['inuid'] 		 = $inuid;
					$post['entidadePar'] = 'municipio';
					$post['muncod'] 	 = $arMuncod[0]['muncod'];
					$post['estuf'] 		 = $arMuncod[0]['estuf'];
					
					$obEntidadeParControle = new EntidadeParControle();
					$inuid = $obEntidadeParControle->gravaInstrumentoUnidade($post);
				} else {
					$_SESSION['par']['inuid']  = $inuid;				
					$_SESSION['par']['muncod'] = $arMuncod[0]['muncod'];
					$_SESSION['par']['itrid']  = 2;
					header( "Location: par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=arvore" );					
				}
			}
			die;
		} else {
			mensagemAcossiacao('Município');
		}
	}elseif(possuiPerfil(PAR_PERFIL_CONTROLE_SOCIAL_MUNICIPAL)){
        $_SESSION['par']['boPerfilConsulta'] = true;
        if($arMuncod = pegaMunicipioAssociado(PAR_PERFIL_CONTROLE_SOCIAL_MUNICIPAL)){
            $_SESSION['par']['boAbaMunicipio'] = true;
            if(count($arMuncod) > 1){
                $arMuncod = ($arMuncod) ? $arMuncod : array();
                $arMuncodTemp = array();
                foreach($arMuncod as $muncod){
                    $arMuncodTemp[] = $muncod['muncod'];
                }
                $_SESSION['par']['arMuncod'] = $arMuncodTemp;
                header( "Location: par.php?modulo=principal/listaMunicipios&acao=A" );
            } else {
                $oInstrumentoUnidade = new InstrumentoUnidade();
                $inuid = $oInstrumentoUnidade->verificaInuidMunicipio($arMuncod[0]['muncod']);
                if(!$inuid){
                    $post['inuid'] 		 = $inuid;
                    $post['entidadePar'] = 'municipio';
                    $post['muncod'] 	 = $arMuncod[0]['muncod'];
                    $post['estuf'] 		 = $arMuncod[0]['estuf'];

                    $obEntidadeParControle = new EntidadeParControle();
                    $inuid = $obEntidadeParControle->gravaInstrumentoUnidade($post);
                } else {
                    $_SESSION['par']['inuid']  = $inuid;
                    $_SESSION['par']['muncod'] = $arMuncod[0]['muncod'];
                    $_SESSION['par']['itrid']  = 2;
                    header( "Location: par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=arvore" );
                }
            }
            die;
        } else {
            mensagemAcossiacao('Município');
        }
    }elseif(possuiPerfil(PAR_PERFIL_CONSULTA_ESTADUAL)){
		$_SESSION['par']['boPerfilConsulta'] = true;
		if($arEstuf = pegaEstadoAssociado(PAR_PERFIL_CONSULTA_ESTADUAL)){
			$_SESSION['par']['boAbaEstado'] = true;
			if(count($arEstuf) > 1){
				$arEstuf = ($arEstuf) ? $arEstuf : array();
				$arEstufTemp = array();
				foreach($arEstuf as $estuf){
					$arEstufTemp[] = $estuf['estuf'];
				}
				$_SESSION['par']['arEstuf'] = $arEstufTemp;
				header( "Location: par.php?modulo=principal/listaEstados&acao=A" );
			} else {
				$oInstrumentoUnidade = new InstrumentoUnidade();
				$inuid = $oInstrumentoUnidade->verificaInuidEstado($arEstuf[0]['estuf']);
				if(!$inuid){
					$post['inuid'] 		 = $inuid;
					$post['entidadePar'] = 'estado';
					$post['estuf'] 		 = $arEstuf[0]['estuf'];

					$obEntidadeParControle = new EntidadeParControle();
					$inuid = $obEntidadeParControle->gravaInstrumentoUnidade($post);
				} else {
					$_SESSION['par']['inuid']  = $inuid;
					$_SESSION['par']['estuf'] = $arEstuf[0]['estuf'];
					$_SESSION['par']['itrid'] = 1;
					header( "Location: par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=arvore" );
				}
			}
			die;
		} else {
			mensagemAcossiacao('Estado');
		}
	}elseif(possuiPerfil(PAR_PERFIL_CONTROLE_SOCIAL_ESTADUAL)){
        $_SESSION['par']['boPerfilConsulta'] = true;
        if($arEstuf = pegaEstadoAssociado(PAR_PERFIL_CONTROLE_SOCIAL_ESTADUAL)){
            $_SESSION['par']['boAbaEstado'] = true;
            if(count($arEstuf) > 1){
                $arEstuf = ($arEstuf) ? $arEstuf : array();
                $arEstufTemp = array();
                foreach($arEstuf as $estuf){
                    $arEstufTemp[] = $estuf['estuf'];
                }
                $_SESSION['par']['arEstuf'] = $arEstufTemp;
                header( "Location: par.php?modulo=principal/listaEstados&acao=A" );
            } else {
                $oInstrumentoUnidade = new InstrumentoUnidade();
                $inuid = $oInstrumentoUnidade->verificaInuidEstado($arEstuf[0]['estuf']);
                if(!$inuid){
                    $post['inuid'] 		 = $inuid;
                    $post['entidadePar'] = 'estado';
                    $post['estuf'] 		 = $arEstuf[0]['estuf'];

                    $obEntidadeParControle = new EntidadeParControle();
                    $inuid = $obEntidadeParControle->gravaInstrumentoUnidade($post);
                } else {
                    $_SESSION['par']['inuid']  = $inuid;
                    $_SESSION['par']['estuf'] = $arEstuf[0]['estuf'];
                    $_SESSION['par']['itrid'] = 1;
                    header( "Location: par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=arvore" );
                }
            }
            die;
        } else {
            mensagemAcossiacao('Estado');
        }
    } else if(possuiPerfil(PAR_PERFIL_AVAL_INSTITUCIONAL_MM) ){
            if( $arMuncod = pegaMunicipioAssociado(PAR_PERFIL_AVAL_INSTITUCIONAL_MM) ){
                $_SESSION['par']['boAbaMunicipio'] = true;
                if(count($arMuncod) > 1){
                    $arMuncod = ($arMuncod) ? $arMuncod : array();
                    $arMuncodTemp = array();
                    foreach($arMuncod as $muncod){
                            $arMuncodTemp[] = $muncod['muncod'];
                    }
                    $_SESSION['par']['arMuncod'] = $arMuncodTemp;
                    header( "Location: par.php?modulo=principal/listaMunicipios&acao=A" );				
                } else {
                    $oInstrumentoUnidade = new InstrumentoUnidade();
                    $inuid = $oInstrumentoUnidade->verificaInuidMunicipio($arMuncod[0]['muncod']);
                    if(!$inuid){
                        $post['inuid'] 		= $inuid;
                        $post['entidadePar']    = 'municipio';
                        $post['muncod'] 	= $arMuncod[0]['muncod'];
                        $post['estuf'] 		= $arMuncod[0]['estuf'];

                        $obEntidadeParControle = new EntidadeParControle();
                        $inuid = $obEntidadeParControle->gravaInstrumentoUnidade($post);
                    } else {
                        $_SESSION['par']['inuid']  = $inuid;				
                        $_SESSION['par']['muncod'] = $arMuncod[0]['muncod'];
                        $_SESSION['par']['itrid']  = 2;
                        header( "Location: par.php?modulo=principal/listaMunicipios&acao=A" );	
                    }
                }
                die;
            } else {
                mensagemAcossiacao('Município');
            }
	} elseif(possuiPerfil(PAR_PERFIL_ADMINISTRADOR_PROCESSOS)){
		header( "Location: par.php?modulo=principal/admprocessosfnde&acao=A" );					
		die;
	} elseif(possuiPerfil(PAR_PERFIL_ADMINISTRADOR_REPROGRAMAÇÃO)){
		header( "Location: par.php?modulo=principal/acompanhamento/ferramentas/reprogramacao&acao=A" );					
		die;
	} 
	header( "Location: par.php?modulo=principal/listaMunicipios&acao=A" );
}
?>