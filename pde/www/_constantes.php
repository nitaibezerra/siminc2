<?php

// INDICA O PROJETO PDE
define('PROJETO_PDE', 3);
define('PROJETOREVISAO', 68216);
define('PROJETO_PDE2011', 115284);

define('PROJETOENEM', 114098);
define('PROJETOSEB', 120363);
define('PROJETOCI', 120008);
define('PROJETOACAOESTRATEGICA', 122414);
define('PROJETOTABLET', 133598);
define('PROJETOPACTO', 133517);

// INDICA OS PERFIS EM RELAÇÃO AO MÓDULO
switch ($_SESSION['sisid']) {
    case 10: # PDE
        define('PERFIL_GESTOR', 82);
        define('PERFIL_GERENTE', 90);
        define('PERFIL_EQUIPE_APOIO_GESTOR', 85);
        define('PERFIL_EQUIPE_APOIO_GERENTE', 91);
        define('PERFIL_ASSESSOR', 159);
        define('PERFIL_SUPERUSUARIO', 100);
        define('PERFIL_CGD', 868);

        $_SESSION['pde']['exercicio'] = $_SESSION['pde']['exercicio'] ? $_SESSION['pde']['exercicio'] : $db->pegaUm("select p.prsano from pde.programacaoexercicio p where p.prsexerccorrente = true and p.prsstatus = 'A'");
        $_SESSION['pde']['exercicio'] = ( $_REQUEST['exercicio'] != '' && $_REQUEST['exercicio'] != $_SESSION['pde']['exercicio'] ) ? $_REQUEST['exercicio'] : $_SESSION['pde']['exercicio'];

        switch ($_SESSION['pde']['exercicio']) {
            case '2009':
                $_SESSION['projeto'] = PROJETO_PDE;
                break;
            case '2010':
                $_SESSION['projeto'] = PROJETOREVISAO;
                break;
            case '2011':
                $_SESSION['projeto'] = PROJETO_PDE2011;
                break;
            case '2012':
                $_SESSION['projeto'] = PROJETO_PDE2011;
                break;
        }

        break;
    case 24: # ENEM
		define('PERFIL_SUPERUSUARIO', 521);
		define('PERFIL_ADMINISTRADOR', 532);
        define('PERFIL_GERENTE', 890); //Gestor de Processos
        define('PERFIL_EQUIPE_APOIO_GERENTE', 891); //Gestor de Risco
        define('PERFIL_EXECUTOR', 518);
        define('PERFIL_VALIDADOR', 519);
        define('PERFIL_CERTIFICADOR', 520);
        define('PERFIL_SOMENTE_CONSULTA', 531);
		define('PERFIL_GESTOR_ATIVIDADE', 1166); //Gestor da Atividade (Etapa)
		define('PERFIL_EQUIPE_APOIO_GESTOR_ATIVIDADE', 1167); //Equipe de Apoio do Gestor da Atividade (Etapa)
		
		//Módulo Gerência de Projetos
		define('PERFIL_GESTOR', 98); //Gestor do Projeto
		define('PERFIL_EQUIPE_APOIO_GESTOR', 99); //Equipe de Apoio do Gestor do Projeto
        
        // workflow
        define('TPDID_ENEM', 39);

        define('ENEM_EST_EM_EXECUCAO', 281);
        define('ENEM_EST_EM_VALIDACAO', 282);
        define('ENEM_EST_EM_CERTIFICACAO', 283);
        define('ENEM_EST_EM_FINALIZADO', 284);

        define('ENEM_AEDID_EXECUTAR', 719);
        define('ENEM_AEDID_VALIDAR', 720);
        define('ENEM_AEDID_INVALIDAR', 721);
        define('ENEM_AEDID_CERTIFICAR', 722);
        define('ENEM_AEDID_NAOCERTIFICAR', 723);
        define('ENEM_AEDID_EXFINALIZAR', 729);
        define('ENEM_AEDID_VLFINALIZAR', 730);

        // funções
        define('FUNID_EXECUTOR_ENEM', 83);
        define('FUNID_VALIDADOR_ENEM', 84);
        define('FUNID_CERTIFICADOR_ENEM', 85);
        define('FUNID_RESPONSAVEL_ENEM', 86);

        define('FUNID_VALIDADORJUR_ENEM', 90);
        define('FUNID_EXECUTORJUR_ENEM', 92);
        define('FUNID_CERTIFICADORJUR_ENEM', 91);

        $_SESSION['pde']['exercicio'] = $_SESSION['pde']['exercicio'] ? $_SESSION['pde']['exercicio'] : '2010';
        $_SESSION['pde']['exercicio'] = ( $_REQUEST['exercicio'] != '' && $_REQUEST['exercicio'] != $_SESSION['pde']['exercicio'] ) ? $_REQUEST['exercicio'] : $_SESSION['pde']['exercicio'];

        $_SESSION['projeto'] = PROJETOENEM;

        break;
    case 11: # PROJETOS
        // usado na tela de listagem de projetos
        define('PERFIL_ADMINISTRADOR', 103); //Aba
        define('PERFIL_CONSULTA', 104);

        define('PERFIL_GESTOR', 98); //Aba
        define('PERFIL_GERENTE', 101);
        define('PERFIL_EQUIPE_APOIO_GESTOR', 99); //Aba
        define('PERFIL_EQUIPE_APOIO_GERENTE', 102);

        define('PERFIL_ALOCACAO_SALAS', 391); //Aba

        define('PERFIL_SUPERUSUARIO', 100);
        define('PERFIL_CGD', 868);
        define('PERFIL_GESTOR_CELULA', 870);

        if ($_SESSION['projeto'] == PROJETO_PDE) {
            $_SESSION['projeto'] = null;
        }
        break;
    case 70: # DEMANDAS SEB
        // usado na tela de listagem de projetos
        if ($_SESSION['baselogin'] == 'simec_desenvolvimento') {
            define('PERFIL_ADMINISTRADOR', 579); //Aba
            define('PERFIL_CONSULTA', 575);
            define('PERFIL_GESTOR', 576); //Aba
            define('PERFIL_GERENTE', 578);
            define('PERFIL_EQUIPE_APOIO_GESTOR', 577); //Aba
            define('PERFIL_EQUIPE_APOIO_GERENTE', 582);
            define('PERFIL_ALOCACAO_SALAS', 581); //Aba
        } else {
            define('PERFIL_ADMINISTRADOR', 595); //Aba
            define('PERFIL_CONSULTA', 590);
            define('PERFIL_GESTOR', 591); //Aba
            define('PERFIL_GERENTE', 593);
            define('PERFIL_EQUIPE_APOIO_GESTOR', 592); //Aba
            define('PERFIL_EQUIPE_APOIO_GERENTE', 594);
            define('PERFIL_ALOCACAO_SALAS', 597);
        }

        $_SESSION['projeto'] = PROJETOSEB;
        break;
    case 71: # CONTROLE INTERNO
        // usado na tela de listagem de projetos
        define('PERFIL_ADMINISTRADOR', 603); //Aba
        define('PERFIL_CONSULTA', 598);
        define('PERFIL_GESTOR', 599); //Aba
        define('PERFIL_GERENTE', 601);
        define('PERFIL_EQUIPE_APOIO_GESTOR', 600); //Aba
        define('PERFIL_EQUIPE_APOIO_GERENTE', 602);
        define('PERFIL_ALOCACAO_SALAS', 605); //Aba

        $_SESSION['projeto'] = PROJETOCI;
        break;

    case 101: # ENEM
        define('PERFIL_EXECUTOR', 518);
        define('PERFIL_VALIDADOR', 519);
        define('PERFIL_CERTIFICADOR', 520);
        define('PERFIL_SUPERUSUARIO', 521);

        define('PERFIL_ALTAGESTAO', 530);
        define('PERFIL_SOMENTE_CONSULTA', 531);
        define('PERFIL_ADMINISTRADOR', 532);

        define('PERFIL_GESTOR', 82);
        define('PERFIL_GERENTE', 90);
        define('PERFIL_EQUIPE_APOIO_GESTOR', 85);
        define('PERFIL_EQUIPE_APOIO_GERENTE', 91);
        define('PERFIL_ASSESSOR', 159);

        // workflow
        define('TPDID_ENEM', 39);

        define('ENEM_EST_EM_EXECUCAO', 281);
        define('ENEM_EST_EM_VALIDACAO', 282);
        define('ENEM_EST_EM_CERTIFICACAO', 283);
        define('ENEM_EST_EM_FINALIZADO', 284);

        define('ENEM_AEDID_EXECUTAR', 719);
        define('ENEM_AEDID_VALIDAR', 720);
        define('ENEM_AEDID_INVALIDAR', 721);
        define('ENEM_AEDID_CERTIFICAR', 722);
        define('ENEM_AEDID_NAOCERTIFICAR', 723);
        define('ENEM_AEDID_EXFINALIZAR', 729);
        define('ENEM_AEDID_VLFINALIZAR', 730);

        // funções
        define('FUNID_EXECUTOR_ENEM', 83);
        define('FUNID_VALIDADOR_ENEM', 84);
        define('FUNID_CERTIFICADOR_ENEM', 85);
        define('FUNID_RESPONSAVEL_ENEM', 86);

        define('FUNID_EXECUTOR_ENEM', 83);
        define('FUNID_VALIDADOR_ENEM', 84);

        define('FUNID_VALIDADORJUR_ENEM', 90);
        define('FUNID_EXECUTORJUR_ENEM', 92);
        define('FUNID_CERTIFICADORJUR_ENEM', 91);
        //define( 'FUNID_CERTIFICADOR_ENEM', 86);

        $_SESSION['pde']['exercicio'] = $_SESSION['pde']['exercicio'] ? $_SESSION['pde']['exercicio'] : '2010';
        $_SESSION['pde']['exercicio'] = ( $_REQUEST['exercicio'] != '' && $_REQUEST['exercicio'] != $_SESSION['pde']['exercicio'] ) ? $_REQUEST['exercicio'] : $_SESSION['pde']['exercicio'];

        $_SESSION['projeto'] = PROJETOACAOESTRATEGICA;

        //
        //Ação estratégica
        //

		//Metas Checklist


        define('PRAZO', 1);
        define('PRAZO_QUANTIDADE', 2);
        define('QUANTIDADE', 3);


        break;
    case 132: # Monitoramento estrategico

        define("UNIDADEMEDICAO_MOEDA", 5);
        define("UNIDADEMEDICAO_NUM_INTEIRO", 3);
        define("UNIDADEMEDICAO_PERCENTUAL", 1);
        define("UNIDADEMEDICAO_RAZAO", 2);
        define("UNIDADEMEDICAO_NUM_INDICE", 4);
        define("UNIDADEMEDICAO_BOLEANA", 6);

        // usado na tela de listagem de projetos
        define('PERFIL_SUPER_USUARIO', 727);
        define('PERFIL_ADMINISTRADOR', 736); //Aba
        define('PERFIL_CONSULTA', 731);
        define('PERFIL_GESTOR', 732); //Aba
        define('PERFIL_GERENTE', 734);
        define('PERFIL_EQUIPE_APOIO_GESTOR', 733); //Aba
        define('PERFIL_EQUIPE_APOIO_GERENTE', 735);
        define('PERFIL_ALOCACAO_SALAS', 738); //Aba
        define('PERFIL_EXECUTOR', 739); //Executor
        define('PERFIL_VALIDADOR', 740); //Validador
        define('PERFIL_DATA_EXECUCAO', 696); //Edição de Data de Execução


        if ($_SESSION['baselogin'] == 'simec_desenvolvimento') {
            define('PERFIL_CONSULTA_EXTERNA', 1087); #CONSULTA EXTERNA. - DEV
            define('PERFIL_CONSULTA_COCKPIT', 1088); #CONSULTA COCKPIT. - DEV
        } else {
            define('PERFIL_CONSULTA_EXTERNA', 1089); #CONSULTA EXTERNA. - ESP
            define('PERFIL_CONSULTA_COCKPIT', 1091); #CONSULTA COCKPIT. - ESP
        }

        //Funções Entidade
        define('FUNID_EXECUTOR_PF', 98);
        define('FUNID_EXECUTOR_PJ', 99);
        define('FUNID_VALIDADOR_PF', 100);
        define('FUNID_VALIDADOR_PJ', 101);

        //Workflow
        define('TIPO_FLUXO_MONITORAMENTO', 60);
        define('WK_ESTADO_DOC_EM_EXECUCAO', 443);
        define('WK_ESTADO_DOC_EM_VALIDACAO', 444);
        define('WK_ESTADO_DOC_FINALIZADO', 445);

        define('WK_MON_EST_AEDID_NAO_EXECUTAR', 1178);
        define('WK_MON_EST_AEDID_ENVIAR_VALIDACAO', 1177);
        define('WK_MON_EST_AEDID_VALIDAR', 1180);
        define('WK_MON_EST_AEDID_INVALIDAR', 1181);
        define('WK_MON_EST_AEDID_FINALIZAR', 1179);
        define('WK_MON_EST_AEDID_NAO_FINALIZAR', 1179);

        //Tipo de indicador
        define('MON_MTIID_IMPACTO', 1);
        define('MON_MTIID_PRODUTO', 2);
        define('MON_MTIID_PROCESSO', 3);

        if ($_SESSION['projeto'] == PROJETO_PDE) {
            $_SESSION['projeto'] = null;
        }
        break;
    case 144: # Tablets
        // usado na tela de listagem de projetos
//		if( $_SESSION['baselogin'] == 'simec_desenvolvimento' ){
        define('PERFIL_EXECUTOR', 791);
        define('PERFIL_CERTIFICADOR', 793);
        define('PERFIL_VALIDADOR', 792);
        define('PERFIL_SUPERUSUARIO', 785);

        define('PERFIL_ALTAGESTAO', 790);
        define('PERFIL_SOMENTE_CONSULTA', 796);
        define('PERFIL_ADMINISTRADOR', 795);

        define('PERFIL_GESTOR', 82);
        define('PERFIL_GERENTE', 90);
        define('PERFIL_EQUIPE_APOIO_GESTOR', 85);
        define('PERFIL_EQUIPE_APOIO_GERENTE', 91);
        define('PERFIL_ASSESSOR', 159);

        // workflow
        define('TPDID_TABLETS', 82);

        define('TABLETS_EST_EM_EXECUCAO', 553);
        define('TABLETS_EST_EM_VALIDACAO', 554);
        define('TABLETS_EST_EM_CERTIFICACAO', 555);
        define('TABLETS_EST_EM_FINALIZADO', 556);

        define('TABLETS_AEDID_EXEC_FINALIZAR', 1430);
        define('TABLETS_AEDID_EXEC_N_EXEC', 1431);
        define('TABLETS_AEDID_EXEC_EXEC', 1432);

        define('TABLETS_AEDID_VAL_FINALIZAR', 1434);
        define('TABLETS_AEDID_VAL_INVAL', 1435);
        define('TABLETS_AEDID_VAL_VAL', 1433);

        define('TABLETS_AEDID_CERT_N_CERT_EXEC', 1436);
        define('TABLETS_AEDID_CERT_N_CERT', 1437);
        define('TABLETS_AEDID_CERT_CERT', 1438);

        // funções
        define('FUNID_EXECUTOR_TABLETS', 104);
        define('FUNID_VALIDADOR_TABLETS', 105);
        define('FUNID_CERTIFICADOR_TABLETS', 106);
        define('FUNID_RESPONSAVEL_TABLETS', 129);

        define('FUNID_VALIDADORJUR_TABLETS', 130);
        define('FUNID_EXECUTORJUR_TABLETS', 131);
        define('FUNID_CERTIFICADORJUR_TABLETS', 132);

        $_SESSION['pde']['exercicio'] = $_SESSION['pde']['exercicio'] ? $_SESSION['pde']['exercicio'] : '2010';
        $_SESSION['pde']['exercicio'] = ( $_REQUEST['exercicio'] != '' && $_REQUEST['exercicio'] != $_SESSION['pde']['exercicio'] ) ? $_REQUEST['exercicio'] : $_SESSION['pde']['exercicio'];

        $_SESSION['projeto'] = PROJETOTABLET;

        define('PRAZO', 1);
        define('PRAZO_QUANTIDADE', 2);
        define('QUANTIDADE', 3);

//		} else {
//		}

        if ($_SESSION['projeto'] == PROJETO_PDE) {
            $_SESSION['projeto'] = null;
        }
        break;
    case 145: # PACTO
        // usado na tela de listagem de projetos
//		if( $_SESSION['baselogin'] == 'simec_desenvolvimento' ){
        define('PERFIL_ALTAGESTAO', 798);
        define('PERFIL_EXECUTOR', 799);
        define('PERFIL_VALIDADOR', 800);
        define('PERFIL_CERTIFICADOR', 801);
        define('PERFIL_ADMINISTRADOR', 803);
        define('PERFIL_SOMENTE_CONSULTA', 804);

        define('PERFIL_SUPERUSUARIO', 802);

        // Legado
        define('PERFIL_GESTOR', 82);
        define('PERFIL_GERENTE', 90);
        define('PERFIL_EQUIPE_APOIO_GESTOR', 85);
        define('PERFIL_EQUIPE_APOIO_GERENTE', 91);
        define('PERFIL_ASSESSOR', 159);
        // FIM Legado
        // workflow
        define('TPDID_PACTO', 81);

        define('PACTO_EST_EM_EXECUCAO', 549);
        define('PACTO_EST_EM_VALIDACAO', 550);
        define('PACTO_EST_EM_CERTIFICACAO', 551);
        define('PACTO_EST_EM_FINALIZADO', 552);

        define('PACTO_AEDID_EXECUTAR', 1423);
        define('PACTO_AEDID_VALIDAR', 1424);
        define('PACTO_AEDID_INVALIDAR', 1426);
        define('PACTO_AEDID_CERTIFICAR', 1429);
        define('PACTO_AEDID_NAOCERTIFICAR', 1428);
        define('PACTO_AEDID_EXFINALIZAR', 1421);
        define('PACTO_AEDID_VLFINALIZAR', 1425);

        // funções
        define('FUNID_EXECUTOR_PACTO', 112);
        define('FUNID_VALIDADOR_PACTO', 113);
        define('FUNID_CERTIFICADOR_PACTO', 114);
        define('FUNID_RESPONSAVEL_PACTO', 86);

        define('FUNID_EXECUTOR_PACTO', 112);
        define('FUNID_VALIDADOR_PACTO', 113);

        define('FUNID_VALIDADORJUR_PACTO', 90);
        define('FUNID_EXECUTORJUR_PACTO', 92);
        define('FUNID_CERTIFICADORJUR_PACTO', 91);

        $_SESSION['pde']['exercicio'] = $_SESSION['pde']['exercicio'] ? $_SESSION['pde']['exercicio'] : '2010';
        $_SESSION['pde']['exercicio'] = ( $_REQUEST['exercicio'] != '' && $_REQUEST['exercicio'] != $_SESSION['pde']['exercicio'] ) ? $_REQUEST['exercicio'] : $_SESSION['pde']['exercicio'];

        $_SESSION['projeto'] = PROJETOPACTO;

        define('PRAZO', 1);
        define('PRAZO_QUANTIDADE', 2);
        define('QUANTIDADE', 3);

//		} else {
//		}

        if ($_SESSION['projeto'] == PROJETO_PDE) {
            $_SESSION['projeto'] = null;
        }
        break;
}

#CONSTANTE PERFIL "Consulta de Reitor/Diretor" PERFIL DE CONSULTA QUE É "USADO" POR USUÁRIO DO SISTEMA REDE FEDERAL
define('CONSULTA_REITOR_DIRETOR', 1428);

// INDICA O PROJETO A SER TRABALHADO
define('PROJETO', $_SESSION['projeto'] ? $_SESSION['projeto'] : null );

// INDICA OS ESTADOS DAS ATIVIDADES
define('STATUS_NAO_INICIADO', 1);
define('STATUS_EM_ANDAMENTO', 2);
define('STATUS_SUSPENSO', 3);
define('STATUS_CANCELADO', 4);
define('STATUS_CONCLUIDO', 5);
?>