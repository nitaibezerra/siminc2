<?php
global $arrPermissoes;

/************* UO_COORDENADOR_EQUIPE_TECNICA **********************/
// EM_CADASTRAMENTO
$arrPermissoes[UO_COORDENADOR_EQUIPE_TECNICA]["principal/liberacaoorcamentaria"][EM_CADASTRAMENTO]		 = true;
$arrPermissoes[UO_COORDENADOR_EQUIPE_TECNICA]["principal/dadosGeraisCDO"][EM_CADASTRAMENTO]				 = true;
$arrPermissoes[UO_COORDENADOR_EQUIPE_TECNICA]["principal/liberacaoorcamentariapopup"][EM_CADASTRAMENTO]	 = true;
$arrPermissoes[UO_COORDENADOR_EQUIPE_TECNICA]["principal/liberacaoorcamentariaresumo"][EM_CADASTRAMENTO] = true;

/************* CGO_EQUIPE_ORCAMENTARIA **********************/
// EM_ANALISE_SERVIDOR_CPRO
$arrPermissoes[CGO_EQUIPE_ORCAMENTARIA]["principal/liberacaoorcamentaria"][EM_ANALISE_SERVIDOR_CPRO]	 	= true;
$arrPermissoes[CGO_EQUIPE_ORCAMENTARIA]["principal/dadosGeraisCDO"][EM_ANALISE_SERVIDOR_CPRO]				= true;
$arrPermissoes[CGO_EQUIPE_ORCAMENTARIA]["principal/liberacaoorcamentariapopup"][EM_ANALISE_SERVIDOR_CPRO]	= true;
$arrPermissoes[CGO_EQUIPE_ORCAMENTARIA]["principal/liberacaoorcamentariaresumo"][EM_ANALISE_SERVIDOR_CPRO] 	= true;

/************* COORDENADOR **********************/
// EM_AVALIACAO_CPRO
$arrPermissoes[COORDENADOR]["principal/liberacaoorcamentaria"][EM_AVALIACAO_CPRO]	 	= true;
$arrPermissoes[COORDENADOR]["principal/dadosGeraisCDO"][EM_AVALIACAO_CPRO]				= true;
$arrPermissoes[COORDENADOR]["principal/liberacaoorcamentariapopup"][EM_AVALIACAO_CPRO]	= true;
$arrPermissoes[COORDENADOR]["principal/liberacaoorcamentariaresumo"][EM_AVALIACAO_CPRO] = true;

/************* CGO_COORDENADOR_ORCAMENTO **********************/
// EM_VALIDACAO_CGO
$arrPermissoes[CGO_COORDENADOR_ORCAMENTO]["principal/liberacaoorcamentaria"][EM_VALIDACAO_CGO]	 		= true;
$arrPermissoes[CGO_COORDENADOR_ORCAMENTO]["principal/dadosGeraisCDO"][EM_VALIDACAO_CGO]					= true;
$arrPermissoes[CGO_COORDENADOR_ORCAMENTO]["principal/liberacaoorcamentariapopup"][EM_VALIDACAO_CGO]		= true;
$arrPermissoes[CGO_COORDENADOR_ORCAMENTO]["principal/liberacaoorcamentariaresumo"][EM_VALIDACAO_CGO]	= true;

/************* SPO **********************/
// EM_APROVACAO_SPO
$arrPermissoes[SPO]["principal/liberacaoorcamentaria"][EM_APROVACAO_SPO]		= true;
$arrPermissoes[SPO]["principal/dadosGeraisCDO"][EM_APROVACAO_SPO]				= true;
$arrPermissoes[SPO]["principal/liberacaoorcamentariapopup"][EM_APROVACAO_SPO]	= true;
$arrPermissoes[SPO]["principal/liberacaoorcamentariaresumo"][EM_APROVACAO_SPO]	= true;


// funчуo para retornar se o usuсrio tem acesso(true/false) р partir dos parтmetros
function permissoesPerfil($perfil, $pagina, $categoria) {
	global $arrPermissoes;
	//ver($perfil,$pagina,$categoria);
	return $arrPermissoes[$perfil][$pagina][$categoria];	
}

?>