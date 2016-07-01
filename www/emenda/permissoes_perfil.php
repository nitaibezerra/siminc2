<?php


global $arrPermissoes;


/************* ADMINISTRADOR **********************/
// GERAL
$arrPermissoes[ADMINISTRADOR_INST]["principal/cadastroEmendas"]["geral"] 					= true;
$arrPermissoes[ADMINISTRADOR_INST]["principal/cadastraDetalheEmenda"]["geral"] 				= true;
$arrPermissoes[ADMINISTRADOR_INST]["principal/liberaEmenda"]["geral"] 						= true;
$arrPermissoes[ADMINISTRADOR_INST]["principal/listaEntidadesBeneficiadas"]["geral"] 		= true;
$arrPermissoes[ADMINISTRADOR_INST]["principal/entidadebeneficiada/contrapartida/definir"]["geral"] 				= true;
$arrPermissoes[ADMINISTRADOR_INST]["principal/listaAcao"]["geral"] 							= true;
$arrPermissoes[ADMINISTRADOR_INST]["principal/cadastroAcao"]["geral"] 						= true;
$arrPermissoes[ADMINISTRADOR_INST]["principal/emendasIniciativa"]["geral"] 					= true;
$arrPermissoes[ADMINISTRADOR_INST]["principal/listaBeneficiarios"]["geral"] 				= true;
$arrPermissoes[ADMINISTRADOR_INST]["principal/listaEspecificacoes"]["geral"] 				= true;
$arrPermissoes[ADMINISTRADOR_INST]["principal/cadastroEspecificacoes"]["geral"] 			= true;
$arrPermissoes[ADMINISTRADOR_INST]["principal/listaTema"]["geral"]	 						= true;
$arrPermissoes[ADMINISTRADOR_INST]["principal/listaResponsavel"]["geral"] 					= true;
$arrPermissoes[ADMINISTRADOR_INST]["principal/cadastraResponsavel"]["geral"] 				= true;
$arrPermissoes[ADMINISTRADOR_INST]["principal/emendasResponsavel"]["geral"] 				= true;
$arrPermissoes[ADMINISTRADOR_INST]["principal/listaPlanoTrabalho"]["geral"] 				= true;
$arrPermissoes[ADMINISTRADOR_INST]["principal/listaDeExtraOrcamento"]["geral"] 				= true;
//$arrPermissoes[ADMINISTRADOR_INST]["principal/listaRecursoExtra"]["geral"] 				= true;
$arrPermissoes[ADMINISTRADOR_INST]["principal/insereRecursoExtra"]["geral"] 				= true;
$arrPermissoes[ADMINISTRADOR_INST]["principal/analiseDadosPTA"]["geral"] 					= true;
$arrPermissoes[ADMINISTRADOR_INST]["principal/informacoesGerais"]["geral"] 					= true;
$arrPermissoes[ADMINISTRADOR_INST]["principal/documentosAnalisePTA"]["geral"] 				= true;

$arrPermissoes[ADMINISTRADOR_INST]["principal/alteraDefinirRecursoPTA"][EM_ELABORACAO] 		= true;
$arrPermissoes[ADMINISTRADOR_INST]["principal/acoesPlanoTrabalho"][EM_ELABORACAO]			= true;
$arrPermissoes[ADMINISTRADOR_INST]["principal/insereAcaoPlanoTrabalho"][EM_ELABORACAO] 		= true;
$arrPermissoes[ADMINISTRADOR_INST]["principal/insereEspecificacaoAcao"][EM_ELABORACAO] 		= true;
$arrPermissoes[ADMINISTRADOR_INST]["principal/insereBeneficiario"][EM_ELABORACAO] 			= true;
$arrPermissoes[ADMINISTRADOR_INST]["principal/cronogramaExecuccaoDesembolso"][EM_ELABORACAO]= true;
$arrPermissoes[ADMINISTRADOR_INST]["principal/cadastrarEscolaBeneficiada"][EM_ELABORACAO] 	= true;
$arrPermissoes[ADMINISTRADOR_INST]["principal/alterarPlanoTrabalho"][EM_ELABORACAO] 		= true;
$arrPermissoes[ADMINISTRADOR_INST]["principal/manterAnexos"][EM_ELABORACAO] 				= true;

$arrPermissoes[ADMINISTRADOR_INST]["principal/alteraDefinirRecursoPTA"][EM_ELABORACAO_IMPOSITIVO] 		= true;
$arrPermissoes[ADMINISTRADOR_INST]["principal/acoesPlanoTrabalho"][EM_ELABORACAO_IMPOSITIVO]			= true;
$arrPermissoes[ADMINISTRADOR_INST]["principal/insereAcaoPlanoTrabalho"][EM_ELABORACAO_IMPOSITIVO] 		= true;
$arrPermissoes[ADMINISTRADOR_INST]["principal/insereEspecificacaoAcao"][EM_ELABORACAO_IMPOSITIVO] 		= true;
$arrPermissoes[ADMINISTRADOR_INST]["principal/insereBeneficiario"][EM_ELABORACAO_IMPOSITIVO] 			= true;
$arrPermissoes[ADMINISTRADOR_INST]["principal/cronogramaExecuccaoDesembolso"][EM_ELABORACAO_IMPOSITIVO]= true;
$arrPermissoes[ADMINISTRADOR_INST]["principal/cadastrarEscolaBeneficiada"][EM_ELABORACAO_IMPOSITIVO] 	= true;
$arrPermissoes[ADMINISTRADOR_INST]["principal/alterarPlanoTrabalho"][EM_ELABORACAO_IMPOSITIVO] 		= true;
$arrPermissoes[ADMINISTRADOR_INST]["principal/manterAnexos"][EM_ELABORACAO_IMPOSITIVO] 				= true;

$arrPermissoes[ADMINISTRADOR_INST]["principal/listaPlanoTrabalho"][EM_EMPENHO] 				= true;
$arrPermissoes[ADMINISTRADOR_INST]["principal/alteraDefinirRecursoPTA"][EM_EMPENHO] 		= true;
$arrPermissoes[ADMINISTRADOR_INST]["principal/acoesPlanoTrabalho"][EM_EMPENHO] 				= true;
$arrPermissoes[ADMINISTRADOR_INST]["principal/insereAcaoPlanoTrabalho"][EM_EMPENHO] 		= true;
$arrPermissoes[ADMINISTRADOR_INST]["principal/insereEspecificacaoAcao"][EM_EMPENHO] 		= true;
$arrPermissoes[ADMINISTRADOR_INST]["principal/insereBeneficiario"][EM_EMPENHO] 				= true;
$arrPermissoes[ADMINISTRADOR_INST]["principal/cronogramaExecuccaoDesembolso"][EM_EMPENHO]	= true;
$arrPermissoes[ADMINISTRADOR_INST]["principal/cadastrarEscolaBeneficiada"][EM_EMPENHO] 		= true;
$arrPermissoes[ADMINISTRADOR_INST]["principal/alterarPlanoTrabalho"][EM_EMPENHO] 			= true;
$arrPermissoes[ADMINISTRADOR_INST]["principal/manterAnexos"][EM_EMPENHO] 					= true;
$arrPermissoes[ADMINISTRADOR_INST]["principal/manterAnexos"][EM_DILIGENCIA] 				= true;
$arrPermissoes[ADMINISTRADOR_INST]["principal/listaPlanoTrabalho"][EM_EMPENHO_IMPOSITIVO] 				= true;
$arrPermissoes[ADMINISTRADOR_INST]["principal/alteraDefinirRecursoPTA"][EM_EMPENHO_IMPOSITIVO] 		= true;
$arrPermissoes[ADMINISTRADOR_INST]["principal/acoesPlanoTrabalho"][EM_EMPENHO_IMPOSITIVO] 				= true;
$arrPermissoes[ADMINISTRADOR_INST]["principal/insereAcaoPlanoTrabalho"][EM_EMPENHO_IMPOSITIVO] 		= true;
$arrPermissoes[ADMINISTRADOR_INST]["principal/insereEspecificacaoAcao"][EM_EMPENHO_IMPOSITIVO] 		= true;
$arrPermissoes[ADMINISTRADOR_INST]["principal/insereBeneficiario"][EM_EMPENHO_IMPOSITIVO] 				= true;
$arrPermissoes[ADMINISTRADOR_INST]["principal/cronogramaExecuccaoDesembolso"][EM_EMPENHO_IMPOSITIVO]	= true;
$arrPermissoes[ADMINISTRADOR_INST]["principal/cadastrarEscolaBeneficiada"][EM_EMPENHO_IMPOSITIVO] 		= true;
$arrPermissoes[ADMINISTRADOR_INST]["principal/alterarPlanoTrabalho"][EM_EMPENHO_IMPOSITIVO] 			= true;
$arrPermissoes[ADMINISTRADOR_INST]["principal/manterAnexos"][EM_EMPENHO_IMPOSITIVO] 					= true;
$arrPermissoes[ADMINISTRADOR_INST]["principal/manterAnexos"][EM_DILIGENCIA_IMPOSITIVO] 				= true;
$arrPermissoes[ADMINISTRADOR_INST]["principal/minutaConvenioDOU"]["geral"]					= true;
$arrPermissoes[ADMINISTRADOR_INST]["principal/minutaConvenio"]["geral"] 					= true;
$arrPermissoes[ADMINISTRADOR_INST]["principal/minuta"]["geral"] 							= true;

$arrPermissoes[ADMINISTRADOR_REFORMULACAO]["principal/minutaConvenioDOU"][EM_PUBLICACAO_REFORMULACAO] = true;

// WORKFLOW - EM_REFORMULACAO_PROCESSO
$arrPermissoes[ADMINISTRADOR_INST]["principal/alteraDefinirRecursoPTA"][EM_REFORMULACAO_PROCESSO] 		= true;
$arrPermissoes[ADMINISTRADOR_INST]["principal/insereBeneficiario"][EM_REFORMULACAO_PROCESSO] 			= true;
$arrPermissoes[ADMINISTRADOR_INST]["principal/cronogramaExecuccaoDesembolso"][EM_REFORMULACAO_PROCESSO]	= true;
$arrPermissoes[ADMINISTRADOR_INST]["principal/alterarPlanoTrabalho"][EM_REFORMULACAO_PROCESSO]			= true;
$arrPermissoes[ADMINISTRADOR_INST]["principal/manterAnexos"][EM_REFORMULACAO_PROCESSO]					= true;
$arrPermissoes[ADMINISTRADOR_INST]["principal/insereEspecificacaoAcao"][EM_REFORMULACAO_PROCESSO]		= true;

$arrPermissoes[ADMINISTRADOR_REFORMULACAO]["principal/insereEspecificacaoAcao"][EM_REFORMULACAO_PROCESSO] 		= true;
$arrPermissoes[ADMINISTRADOR_REFORMULACAO]["principal/insereAcaoPlanoTrabalho"][EM_REFORMULACAO_PROCESSO] 		= true;
$arrPermissoes[ADMINISTRADOR_REFORMULACAO]["principal/acoesPlanoTrabalho"][EM_REFORMULACAO_PROCESSO]      		= true;
$arrPermissoes[ADMINISTRADOR_REFORMULACAO]["principal/alteraDefinirRecursoPTA"][EM_REFORMULACAO_PROCESSO] 		= true;
$arrPermissoes[ADMINISTRADOR_REFORMULACAO]["principal/insereBeneficiario"][EM_REFORMULACAO_PROCESSO]      		= true;
$arrPermissoes[ADMINISTRADOR_REFORMULACAO]["principal/cronogramaExecuccaoDesembolso"][EM_REFORMULACAO_PROCESSO] = true;
$arrPermissoes[ADMINISTRADOR_REFORMULACAO]["principal/alterarPlanoTrabalho"][EM_REFORMULACAO_PROCESSO] 			= true;
$arrPermissoes[ADMINISTRADOR_REFORMULACAO]["principal/manterAnexos"][EM_REFORMULACAO_PROCESSO] 					= true;
$arrPermissoes[ADMINISTRADOR_REFORMULACAO]["principal/reformulacaoVigenciaPTA"][EM_REFORMULACAO_PROCESSO] 		= true;


$arrPermissoes[ADMINISTRADOR_REFORMULACAO]["principal/analiseTecnicaPTA"]["geral"] 		= true;

/************* ENTIDADE BENEFICIADA **********************/
// WORKFLOW
$arrPermissoes[INSTITUICAO_BENEFICIADA]["principal/listaPlanoTrabalho"][EM_ELABORACAO] 				= true;
$arrPermissoes[INSTITUICAO_BENEFICIADA]["principal/alteraDefinirRecursoPTA"][EM_ELABORACAO] 		= true;
$arrPermissoes[INSTITUICAO_BENEFICIADA]["principal/acoesPlanoTrabalho"][EM_ELABORACAO] 				= true;
$arrPermissoes[INSTITUICAO_BENEFICIADA]["principal/insereAcaoPlanoTrabalho"][EM_ELABORACAO] 		= true;
$arrPermissoes[INSTITUICAO_BENEFICIADA]["principal/insereEspecificacaoAcao"][EM_ELABORACAO] 		= true;
$arrPermissoes[INSTITUICAO_BENEFICIADA]["principal/insereBeneficiario"][EM_ELABORACAO] 				= true;
$arrPermissoes[INSTITUICAO_BENEFICIADA]["principal/cronogramaExecuccaoDesembolso"][EM_ELABORACAO] 	= true;
$arrPermissoes[INSTITUICAO_BENEFICIADA]["principal/cadastrarEscolaBeneficiada"][EM_ELABORACAO] 		= true;
$arrPermissoes[INSTITUICAO_BENEFICIADA]["principal/alterarPlanoTrabalho"][EM_ELABORACAO] 			= true;
$arrPermissoes[INSTITUICAO_BENEFICIADA]["principal/manterAnexos"][EM_ELABORACAO] 					= true;
$arrPermissoes[INSTITUICAO_BENEFICIADA]["principal/manterAnexos"][EM_DILIGENCIA]					= true;
$arrPermissoes[INSTITUICAO_BENEFICIADA]["principal/documentosPTA"][EM_DILIGENCIA]					= true;

$arrPermissoes[INSTITUICAO_BENEFICIADA]["principal/listaPlanoTrabalho"][EM_ELABORACAO_IMPOSITIVO] 				= true;
$arrPermissoes[INSTITUICAO_BENEFICIADA]["principal/alteraDefinirRecursoPTA"][EM_ELABORACAO_IMPOSITIVO] 			= true;
$arrPermissoes[INSTITUICAO_BENEFICIADA]["principal/acoesPlanoTrabalho"][EM_ELABORACAO_IMPOSITIVO] 				= true;
$arrPermissoes[INSTITUICAO_BENEFICIADA]["principal/insereAcaoPlanoTrabalho"][EM_ELABORACAO_IMPOSITIVO] 			= true;
$arrPermissoes[INSTITUICAO_BENEFICIADA]["principal/insereEspecificacaoAcao"][EM_ELABORACAO_IMPOSITIVO] 			= true;
$arrPermissoes[INSTITUICAO_BENEFICIADA]["principal/insereBeneficiario"][EM_ELABORACAO_IMPOSITIVO] 				= true;
$arrPermissoes[INSTITUICAO_BENEFICIADA]["principal/cronogramaExecuccaoDesembolso"][EM_ELABORACAO_IMPOSITIVO] 	= true;
$arrPermissoes[INSTITUICAO_BENEFICIADA]["principal/cadastrarEscolaBeneficiada"][EM_ELABORACAO_IMPOSITIVO] 		= true;
$arrPermissoes[INSTITUICAO_BENEFICIADA]["principal/alterarPlanoTrabalho"][EM_ELABORACAO_IMPOSITIVO] 			= true;
$arrPermissoes[INSTITUICAO_BENEFICIADA]["principal/manterAnexos"][EM_ELABORACAO_IMPOSITIVO] 					= true;
$arrPermissoes[INSTITUICAO_BENEFICIADA]["principal/manterAnexos"][EM_DILIGENCIA_IMPOSITIVO]						= true;
$arrPermissoes[INSTITUICAO_BENEFICIADA]["principal/documentosPTA"][EM_DILIGENCIA_IMPOSITIVO]					= true;


$arrPermissoes[INSTITUICAO_BENEFICIADA]["principal/alteraDefinirRecursoPTA"][EM_REFORMULACAO_PROCESSO] 		= true;
$arrPermissoes[INSTITUICAO_BENEFICIADA]["principal/insereAcaoPlanoTrabalho"][EM_REFORMULACAO_PROCESSO] 		= true;
$arrPermissoes[INSTITUICAO_BENEFICIADA]["principal/insereEspecificacaoAcao"][EM_REFORMULACAO_PROCESSO] 		= true;
$arrPermissoes[INSTITUICAO_BENEFICIADA]["principal/insereBeneficiario"][EM_REFORMULACAO_PROCESSO] 			= true;
$arrPermissoes[INSTITUICAO_BENEFICIADA]["principal/cronogramaExecuccaoDesembolso"][EM_REFORMULACAO_PROCESSO] = true;
$arrPermissoes[INSTITUICAO_BENEFICIADA]["principal/cadastrarEscolaBeneficiada"][EM_REFORMULACAO_PROCESSO] 	= true;
$arrPermissoes[INSTITUICAO_BENEFICIADA]["principal/alterarPlanoTrabalho"][EM_REFORMULACAO_PROCESSO] 			= true;
$arrPermissoes[INSTITUICAO_BENEFICIADA]["principal/manterAnexos"][EM_REFORMULACAO_PROCESSO] 					= true;
$arrPermissoes[INSTITUICAO_BENEFICIADA]["principal/manterAnexos"][EM_REFORMULACAO_PROCESSO] 					= true;
$arrPermissoes[INSTITUICAO_BENEFICIADA]["principal/reformulacao"][EM_SOLICITACAO_REFORMULACAO] 					= true;

$arrPermissoes[ANALISTA_TECNICO]["principal/alteraDefinirRecursoPTA"][EM_REFORMULACAO_PROCESSO] 		= true;
$arrPermissoes[ANALISTA_TECNICO]["principal/insereAcaoPlanoTrabalho"][EM_REFORMULACAO_PROCESSO] 		= true;
$arrPermissoes[ANALISTA_TECNICO]["principal/insereEspecificacaoAcao"][EM_REFORMULACAO_PROCESSO] 		= true;
$arrPermissoes[ANALISTA_TECNICO]["principal/insereBeneficiario"][EM_REFORMULACAO_PROCESSO] 			= true;
$arrPermissoes[ANALISTA_TECNICO]["principal/cronogramaExecuccaoDesembolso"][EM_REFORMULACAO_PROCESSO] = true;
$arrPermissoes[ANALISTA_TECNICO]["principal/cadastrarEscolaBeneficiada"][EM_REFORMULACAO_PROCESSO] 	= true;
$arrPermissoes[ANALISTA_TECNICO]["principal/alterarPlanoTrabalho"][EM_REFORMULACAO_PROCESSO] 			= true;
$arrPermissoes[ANALISTA_TECNICO]["principal/manterAnexos"][EM_REFORMULACAO_PROCESSO] 					= true;
$arrPermissoes[ANALISTA_TECNICO]["principal/manterAnexos"][EM_REFORMULACAO_PROCESSO] 					= true;
$arrPermissoes[ANALISTA_TECNICO]["principal/reformulacao"][EM_SOLICITACAO_REFORMULACAO] 					= true;


$arrPermissoes[ADMINISTRADOR_INST]["principal/cronogramaExecuccaoDesembolso"][EM_GERACAO_TERMO_ADITIVO]	= true;
$arrPermissoes[ADMINISTRADOR_MINUTA]["principal/cronogramaExecuccaoDesembolso"][EM_GERACAO_TERMO_ADITIVO]	= true;
$arrPermissoes[ANALISTA_TECNICO]["principal/cronogramaExecuccaoDesembolso"][EM_GERACAO_TERMO_ADITIVO]	= true;
$arrPermissoes[ADMINISTRADOR_INST]["principal/assinaturasPTA"]["geral"]				= true;
$arrPermissoes[CONVENIO]["principal/assinaturasPTA"][EM_ASSINATURA_CONCEDENTE]		= true;
$arrPermissoes[CONVENIO]["principal/assinaturasPTA"][EM_ASSINATURA_CONVENENTE]		= true;
$arrPermissoes[TEMPORARIO_ASSINATURA]["principal/assinaturasPTA"]["geral"]		 	= true;

// GERAL
$arrPermissoes[INSTITUICAO_BENEFICIADA]["principal/definirRecursosPTA"]["geral"] 					= true;


/************* ASSESSORIA PARLAMENTAR **********************/
$arrPermissoes[ASSESSORIA_PARLAMENTAR]["principal/listaEntidadesBeneficiadas"]["geral"] 			= true;

/************* AUTOR EMENDA **********************/
// GERAL
$arrPermissoes[AUTOR_EMENDA]["principal/cadastraDetalheEmenda"]["geral"] 							= true;


/************* ANALISTA_MERITO **********************/
// GERAL
$arrPermissoes[ANALISTA_MERITO]["principal/informacoesGerais"]["geral"] 								= true;
$arrPermissoes[ANALISTA_MERITO]["principal/analiseMeritoPTA"]["geral"] 								= true;


/************* ANALISTA_TECNICO **********************/
// GERAL
$arrPermissoes[ANALISTA_TECNICO]["principal/analiseTecnicaPTA"]["geral"] 							= true;


/************* ANALISTA_FNDE **********************/
// GERAL
$arrPermissoes[ANALISTA_FNDE]["principal/informacoesGerais"]["geral"] 								= true;
$arrPermissoes[ANALISTA_FNDE]["principal/orientacaoPreenchimento"]["geral"] 						= true;
$arrPermissoes[ANALISTA_FNDE]["principal/documentosAnalisePTA"]["geral"] 							= true;
$arrPermissoes[ANALISTA_FNDE]["principal/minutaConvenioDOU"][EM_PUBLICACAO_REFORMULACAO] 			= true;

$arrPermissoes[ADMINISTRADOR_MINUTA]["principal/minutaConvenioDOU"][EM_PUBLICACAO_REFORMULACAO]		= true;
$arrPermissoes[ADMINISTRADOR_MINUTA]["principal/minutaTermoAditivo"]["geral"]						= true;
$arrPermissoes[ADMINISTRADOR_MINUTA]["principal/minuta"]["geral"]									= true;
$arrPermissoes[ADMINISTRADOR_MINUTA]["principal/minuta"][EM_GERACAO_TERMO_ADITIVO]					= true;



/************* CONVENIO **********************/
// GERAL
$arrPermissoes[CONVENIO]["principal/cadastroAcao"]["geral"] 										  = true;
$arrPermissoes[CONVENIO]["principal/minutaConvenio"]["geral"] 										  = true;
$arrPermissoes[CONVENIO]["principal/cronogramaExecuccaoDesembolso"]["geral"] 						  = true;

/************* EMPENHO **********************/
// GERAL
$arrPermissoes[EMPENHO]["principal/execucaoPTA"]["geral"] = true;

/************* PAGAMENTO **********************/
// GERAL
$arrPermissoes[ADMINISTRADOR_INST]["principal/analisepta/pagamento/solicitar"]["geral"] = true;
$arrPermissoes[PAGAMENTO]["principal/analisepta/pagamento/solicitar"]["geral"] = true;
$arrPermissoes[PAGAMENTO]["principal/analisepta/pagamento/solicitar"][EM_LIBERACAO_RECURSO] = true;
$arrPermissoes[ANALISTA_FNDE]["principal/analisepta/pagamento/solicitar"][EM_LIBERACAO_RECURSO] = true;
$arrPermissoes[ANALISTA_FNDE]["principal/analisepta/pagamento/solicitar"][EM_LIBERACAO_RECURSO_REFORMULACAO] = true;
$arrPermissoes[SUPER_USUARIO]["principal/analisepta/pagamento/solicitar"][EM_LIBERACAO_RECURSO_REFORMULACAO] = true;
$arrPermissoes[PAGAMENTO]["principal/analisepta/pagamento/solicitar"][EM_LIBERACAO_RECURSO_REFORMULACAO] = true;

$arrPermissoes[CONVENIO]["principal/documentosAnalisePTA"]["geral"] 							= true;

// funчуo para retornar se o usuсrio tem acesso(true/false) р partir dos parтmetros
function permissoesPerfil($perfil, $pagina, $categoria) {
	global $arrPermissoes;
	return $arrPermissoes[$perfil][$pagina][$categoria];	
}

?>