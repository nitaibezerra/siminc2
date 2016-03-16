<?php

switch ($_SESSION) {
    case 'simec_desenvolvimento':
        break;
    
    case 'simec_desenvolvimento_old_2':
        define("PFL_SECRETARIO_MUNICIPAL", 596);
        define("PFL_SECRETARIO_ESTADUAL", 597);
        break;

    default:
        define("PFL_EQUIPE_MEC", 612);
        define("PFL_SUPER_USUARIO", 616);
        define("PFL_SECRETARIO_MUNICIPAL", 646);
        define("PFL_SECRETARIO_ESTADUAL", 645);
        define("PFL_COORDENADOR_ESTADUAL", 647);
        define("PFL_COORDENADOR_MUNICIPAL", 648);
        define("PFL_DIRETOR_POLO", 649);
        define("PFL_DIRETOR_NUCLEO", 650);
        define("PFL_DIRETOR_ESCOLA", 687);
        define("PFL_CONSULTA", 694);
        define("PFL_ADMINISTRADOR", 683);

        define("OPP_EFETIVO_COMPLEMENTACAO", 5);
        define("OPP_EFETIVO_40HORAS", 4);
        define("OPP_EFETIVO_RECURSOS_PROPRIOS", 6);
        define("OPP_CONTRATO_RECURSOS_PROGRAMA", 13);

        define("OPP_CONTRATO_RECURSOS_PROGRAMA_A", 7);
        define("OPP_CONTRATO_RECURSOS_PROGRAMA_P", 10);
        
        define("OPP_EFETIVO_COMPLEMENTACAO_A", 16);
        define("OPP_EFETIVO_COMPLEMENTACAO_P", 14);
        
        define("TPD_PROJOVEMURBANO", 53);

        define("ESD_EMELABORACAO", 404);
        define("ESD_VALIDADOMEC", 406);

        //CRIADO PARA SER UTILIZADO PARA VINCULO DO PROGRAMA 
        //ATUALMENTE POSSUI SOMENTE UM PROGRAMA
        define('PROJOVEMURBANO_2012', 1);

        define('SITUACAO_DIARIO_ABERTO', 1);
        define('SITUACAO_DIARIO_ENCERRADO', 2);

        define('WORKFLOW_TIPODOCUMENTO_DIARIO', 75);
        define('WORKFLOW_TIPODOCUMENTO_PAGAMENTO', 76);

        define('WF_ESTADO_DIARIO_ABERTO', 519); // Diretor de Nucleo
        define('WF_ESTADO_DIARIO_FECHADO', 520); // Diretor de Nucleo
        define('WF_ESTADO_DIARIO_ENCAMINHAR', 521); // Diretor de Polo
        define('WF_ESTADO_DIARIO_VALIDACAO', 522); // Coordenador
        define('WF_ESTADO_DIARIO_APROVACAO', 523); // Equipe MEC
        define('WF_ESTADO_DIARIO_PAGAMENTO', 524); // Equipe MEC

        define('WF_ESTADO_PAGAMENTO_PENDENTE', 527); // Pagamento Pendente
        define('WF_ESTADO_PAGAMENTO_AUTORIZADO', 528); // Pagamento Autorizado
        define('WF_ESTADO_PAGAMENTO_ENVIADO', 529); // Pagamento Enviado
        define('WF_ESTADO_PAGAMENTO_RECUSADO', 560); // Pagamento Enviado

        define('MUNCOD_SUZANO', '3552502');
        break;
}