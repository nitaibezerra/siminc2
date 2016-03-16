<?
//recupera perfis de usuario

$pflini = arrayPerfil();

if(in_array(PERFIL_SUPER_USUARIO,$pflini)){
    //include  APPRAIZ."fabrica/modulos/principal/painelFinanceiro.inc";
	//include  APPRAIZ."fabrica/modulos/principal/listarSolicitacoes.inc";
	include  APPRAIZ."fabrica/modulos/principal/painelOperacionalgProjeto.inc";
}else{
    if( in_array(PERFIL_GERENTE_PROJETO, $pflini)){
        include  APPRAIZ."fabrica/modulos/principal/painelOperacionalgProjeto.inc";
    }else{
    	if(in_array(PERFIL_CONSULTA_GERAL,$pflini) && count($pflini) == 1){
    		include  APPRAIZ."fabrica/modulos/principal/listarSolicitacoesCon.inc";
    	}else{
    		include  APPRAIZ."fabrica/modulos/principal/listarSolicitacoes.inc";
    	}
    }
}

/*
//permissao PERFIL_REQUISITANTE
if(in_array(PERFIL_REQUISITANTE,$pflini)){
	include  APPRAIZ."fabrica/modulos/principal/listarSolicitacoes.inc";
	exit;
}

//permissao PERFIL_CONTAGEM_PF -> EMPRESA CONTRATADA
if(in_array(PERFIL_CONTAGEM_PF,$pflini)){
	include  APPRAIZ."fabrica/modulos/principal/listarOrdemServico.inc";
	exit;
}
*/