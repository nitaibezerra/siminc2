 <?php

switch ($_SESSION) {
	case 'simec_desenvolvimento':

		define("TPD_PROJOVEMCAMPO", 150);

		define("ESD_EMELABORACAO", 981);
		define("ESD_EMANALISEMEC", 982);
		define("ESD_VALIDADOMEC", 983);
	
		
		define("PFL_EQUIPE_MEC", 1106);
		define("PFL_SUPER_USUARIO", 1105);
		define("PFL_SECRETARIO_MUNICIPAL", 1109);
		define("PFL_SECRETARIO_ESTADUAL", 1108);
		define("PFL_COORDENADOR_ESTADUAL", 1110);
		define("PFL_COORDENADOR_MUNICIPAL", 1111);
		define("PFL_COORDENADOR_TURMA", 1320);
		define("PFL_DIRETOR_ESCOLA", 1216);
		define("PFL_ADMINISTRADOR", 1107);
		define("PFL_CONSULTA", 1223);
		
		define("OPP_EFTIVO30H",1);
		define("OPP_EFTIVOCOMP",2);
		define("OPP_EFTIVORECPROG",3);
		define("OPP_EFTIVORECPROP",4);
		
		define("WORKFLOW_TIPODOCUMENTO_PAGAMENTO",237);
		
		define("ESD_PAGAMENTO_",1);
		define("OPP_EFTIVOCOMP",2);
		define("OPP_EFTIVORECPROG",3);
		define("OPP_EFTIVORECPROP",4);
		break;
		
	default:
		define("TPD_PROJOVEMCAMPO", 173);

		define("ESD_EMELABORACAO", 1077);
		define("ESD_EMANALISEMEC", 1078);
		define("ESD_VALIDADOMEC", 1079);
	
		
		define("PFL_EQUIPE_MEC", 1179);
		define("PFL_SUPER_USUARIO", 1178);
		define("PFL_SECRETARIO_MUNICIPAL", 1182);
		define("PFL_SECRETARIO_ESTADUAL", 1181);
		define("PFL_COORDENADOR_ESTADUAL", 1183);
		define("PFL_COORDENADOR_MUNICIPAL", 1184);
		define("PFL_COORDENADOR_TURMA", 1320);
		define("PFL_DIRETOR_ESCOLA", 1216);
		define("PFL_ADMINISTRADOR", 1180);
		define("PFL_CONSULTA", 1223);
		
		define("OPP_EFTIVO30H",1);
		define("OPP_EFTIVOCOMP",2);
		define("OPP_EFTIVORECPROG",3);
		define("OPP_EFTIVORECPROP",4);
		
// 		define('SITUACAO_DIARIO_ABERTO', 1);
// 		define('SITUACAO_DIARIO_ENCERRADO', 2);
		
// 		define('WORKFLOW_TIPODOCUMENTO_DIARIO', 75);
// 		define('WORKFLOW_TIPODOCUMENTO_PAGAMENTO', 76);
		
		define('ESTADO_DIARIO_ABERTO', 1); // Diretor de Escola
		define('ESTADO_DIARIO_FECHADO', 12); // Diretor de Escola
		define('ESTADO_DIARIO_COORDTURMA', 2); // Diretor de Escola
		define('ESTADO_DIARIO_COORDGERAL', 3); // Coordenador de Turma
		define('ESTADO_DIARIO_MEC', 4); // MEC
		
		define('ESTADO_DIARIO_DEVOLVIDO_DIRESCOLA', 5); // Diretor de Escola
		define('ESTADO_DIARIO_DEVOLVIDO_COORDTURMA', 6); // Coordenador de Turma
		define('ESTADO_DIARIO_DEVOLVIDO_COORDGERAL', 7); // Coordenador Geral
		
		define('ESTADO_DIARIO_PAGAMENTO_ENVIADO', 8); // MEC
		
		define('ESTADO_PAGAMENTO_PAGO', 9); // MEC
		define('ESTADO_PAGAMENTO_RECUSADO', 10); //QUEM PAGA
		define('ESTADO_PAGAMENTO_AUTORIZADO', 11); // MEC
		
		define("WORKFLOW_TIPODOCUMENTO_PAGAMENTO",237);
		
		define("ESD_PAGAMENTO_AUTORIZADO",1582);
		define("ESD_PAGAMENTO_ENVIADO",1583);
		define("ESD_PAGAMENTO_PAGO",1584);
		define("ESD_PAGAMENTO_REJEITADO",1585);
		
		define("AED_PAGAMENTO_ENVIAR",3723);
		define("AED_PAGAMENTO_PAGAR",3724);
		define("AED_PAGAMENTO_REJEITAR",3725);
		define("AED_PAGAMENTO_RETORNAR",3726);
		
		define("SGB_ENVIADOBANCO",					 6);
		define("SGB_AUTORIZADA",					 1);
		define("SGB_HOMOLOGADA",					 2);
		define("SGB_PREAPROVADA",					 3);
		define("SGB_ENVIADOAOSIGEF",				 4);
		define("SGB_CREDITADA",						 7);
		define("SGB_SACADA",						 8);
		define("SGB_RESTITUIDO",					 9);
		
		if(strstr($_SERVER['HTTP_HOST'],"simec-local") || strstr($_SERVER['HTTP_HOST'],"simec-d.mec.gov.br") || strstr($_SERVER['HTTP_HOST'],"simec-d")){
			// desenvolvimento
			define( 'SISTEMA_SGB',  'PCA' );
			define( 'USUARIO_SGB',  'PCA' );
			define( 'PROGRAMA_SGB', 'PCA' );
			define( 'SENHA_SGB',    'PROJOVEMALUNO_HOMOLOG' );
			define( 'WSDL_CAMINHO', 'https://hmg.fnde.gov.br/spba/Servicos?wsdl');
			define( 'WSDL_CAMINHO_CADASTRO', 'http://sgbhmg.fnde.gov.br/sistema/ws/?wsdl');
		
 		} else {
 			// produção
 			define( 'SISTEMA_SGB',  'PCA' );
 			define( 'USUARIO_SGB',  'PCA' );
 			define( 'PROGRAMA_SGB', 'PCA' );
 			define( 'SENHA_SGB',    '1QUY9-S|LQ!O#J-ZH08:WQ,WBTQIWLRB' );
 			define( 'WSDL_CAMINHO', 'http://www.fnde.gov.br/spba/Servicos?wsdl');
 			define( 'WSDL_CAMINHO_CADASTRO', 'http://sgb.fnde.gov.br/sistema/ws/?wsdl');
 		}
		
// 		define('WF_ESTADO_PAGAMENTO_PENDENTE', 527); // Pagamento Pendente
// 		define('WF_ESTADO_PAGAMENTO_AUTORIZADO', 528); // Pagamento Autorizado
// 		define('WF_ESTADO_PAGAMENTO_ENVIADO', 529); // Pagamento Enviado
// 		define('WF_ESTADO_PAGAMENTO_RECUSADO', 560); // Pagamento Enviado
		
		break;
}

?>