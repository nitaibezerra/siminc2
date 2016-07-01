<?php

/**
 * Sistema Simec
 * Setor responsvel: MEC
 * Desenvolvedor: Equipe Consultores Simec
 * Analista: Luiz Fernando Mendonça
 * Programador: Gustavo Fernandes da Guarda
 */
unset($_SESSION['escolaativa']['boPerfilSuperUsuario'],
	  $_SESSION['escolaativa']['muncod'],
	  $_SESSION['escolaativa']['estuf'],
	  $_SESSION['escolaativa']['montipo'],
	  $_SESSION['escolaativa']['boPerfilPesquisador'],
	  $_SESSION['escolaativa']['boPerfilSupervisor'],
	  $_SESSION['escolaativa']['boPerfilMultiplicador'],
	  $_SESSION['escolaativa']['arMuncod'],
	  $_SESSION['escolaativa']['arEstuf'],
	  $_SESSION['escolaativa']['boAbaMunicipio'],
	  $_SESSION['escolaativa']['boAbaEstado'],
	  $_SESSION['escolaativa']['entid'],
	  $_SESSION['escolaativa']['tecid']
	  );

if( possuiPerfil(array(ESCOLAATIVA_PERFIL_SUPER_USUARIO, ESCOLAATIVA_PERFIL_ADMINISTRADOR,ESCOLAATIVA_PERFIL_COORDENADOR_MEC, ESCOLAATIVA_PERFIL_TECNICO_SECAD) ) ){
	$_SESSION['escolaativa']['boPerfilSuperUsuario'] = true;
} else {
//if( !possuiPerfil(array(ESCOLAATIVA_PERFIL_SUPER_USUARIO, ESCOLAATIVA_PERFIL_ADMINISTRADOR,ESCOLAATIVA_PERFIL_COORDENADOR_MEC) ) ){
	if(possuiPerfil(ESCOLAATIVA_PERFIL_MULTIPLICADOR)){
		if($arMuncod = pegaMunicipioAssociado(ESCOLAATIVA_PERFIL_MULTIPLICADOR)){
			$_SESSION['escolaativa']['boPerfilMultiplicador'] = true;
			# verifica se usuario logado tem registro em tecnico 
			$entid = verificaProfMultiplicador();
			if(!$entid){
				header( "Location: escolaativa.php?modulo=principal/professorMultiplicador&acao=A&cpf=".$_SESSION['usucpf'] );
				exit;
			}
			
			$_SESSION['escolaativa']['entid'] = $entid;
			
			$_SESSION['escolaativa']['tecid'] = pegaTecid();
			
			$_SESSION['escolaativa']['montipo'] = "M";			
			if(count($arMuncod) > 1){
				$arMuncod = ($arMuncod) ? $arMuncod : array();
				$arMuncodTemp = array();
				foreach($arMuncod as $muncod){
					$arMuncodTemp[] = $muncod['muncod'];
				}
				$_SESSION['escolaativa']['boAbaMunicipio'] = true;
				$_SESSION['escolaativa']['arMuncod'] = $arMuncodTemp;
				header( "Location: escolaativa.php?modulo=principal/listaMunicipios&acao=A" );				
			} else {
				$_SESSION['escolaativa']['muncod'] = $arMuncod[0]['muncod'];
				header( "Location: escolaativa.php?modulo=principal/estruturaAvaliacao&acao=A" );
			}
			die;
		} else {
			mensagemAssossiacao('Município');
		}
		
	} elseif(possuiPerfil(ESCOLAATIVA_PERFIL_SUPERVISOR)){
		if($arEstuf = pegaEstadoAssociado(ESCOLAATIVA_PERFIL_SUPERVISOR)){
			$_SESSION['escolaativa']['boPerfilSupervisor'] = true;
			# verifica se usuario logado tem registro em tecnico 
			$entid = verificaProfMultiplicador();
			if(!$entid){
				header( "Location: escolaativa.php?modulo=principal/professorMultiplicador&acao=A&cpf=".$_SESSION['usucpf'] );
				exit;	
			}
			$_SESSION['escolaativa']['entid'] = $entid;
			
			$_SESSION['escolaativa']['tecid'] = pegaTecid();
					
			$_SESSION['escolaativa']['montipo'] = "E";
			if(count($arEstuf) > 1){
				$arEstuf = ($arEstuf) ? $arEstuf : array();
				$arEstufTemp = array();
				foreach($arEstuf as $estuf){
					$arEstufTemp[] = $estuf['estuf'];
				}
				$_SESSION['escolaativa']['boAbaEstado'] = true;
				$_SESSION['escolaativa']['arEstuf'] = $arEstufTemp;
				header( "Location: escolaativa.php?modulo=principal/listaEstados&acao=A" );				
			} else {
				$_SESSION['escolaativa']['estuf'] = $arEstuf[0]['estuf'];
				header( "Location: escolaativa.php?modulo=principal/estruturaAvaliacao&acao=A" );
			}
			die;
		} else {
			mensagemAssossiacao('Estado');
		}
		
	} elseif(possuiPerfil(ESCOLAATIVA_PERFIL_PESQUISADOR)){
		if($arEstuf = pegaEstadoAssociado(ESCOLAATIVA_PERFIL_PESQUISADOR)){
			$_SESSION['escolaativa']['boPerfilPesquisador'] = true;
			$_SESSION['escolaativa']['montipo'] = "E";
			if(count($arEstuf) > 1){
				$arEstuf = ($arEstuf) ? $arEstuf : array();
				$arEstufTemp = array();
				foreach($arEstuf as $estuf){
					$arEstufTemp[] = $estuf['estuf'];
				}
				$_SESSION['escolaativa']['boAbaEstado'] = true;
				$_SESSION['escolaativa']['arEstuf'] = $arEstufTemp;
				header( "Location: escolaativa.php?modulo=principal/listaEstados&acao=A" );				
			} else {
				$_SESSION['escolaativa']['estuf'] = $arEstuf[0]['estuf'];
				header( "Location: escolaativa.php?modulo=principal/estruturaAvaliacao&acao=A" );
			}
			die;
		} else {
			mensagemAssossiacao('Estado');
		}
	}
}
header( "Location: escolaativa.php?modulo=principal/listaEstados&acao=A" );

?>