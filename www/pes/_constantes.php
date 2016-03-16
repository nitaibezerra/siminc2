<?php

/********************************************
* 			      MENSAGENS 				*
********************************************/

define('MSG001', 'Salvo com sucesso!');
define('MSG002', 'Nгo foi possivel salvar!');
define('MSG003', 'Operaзгo realizada com sucesso.');
define('MSG004', 'Ocorreu um erro ao executar operaзгo.');

define('MSG005', 'Deseja realmente excluir?');
define('MSG006', 'Excluido com sucesso!');
define('MSG007', 'Nгo pode excluir!');
define('MSG008', 'Deseja alterar esta situaзгo?');
define('MSG009', 'Situaзгo alterada com sucesso!');
define('MSG010', 'Jб existe um contrato com o mesmo tнtulo para essa despesa!\nFavor verificar');
define('MSG011', 'Envio realizado com sucesso!');
define('MSG012', 'Envio nгo pode ser realizado!');




/********************************************/

// Situaзгo
define ('ACAO_NAO_INICIADA', 'NI'); //NI - Nao iniciada
define ('ACAO_EM_ANDAMENTO', 'EA'); //EA - Em andamento
define ('ACAO_CONCLUIDA', 'CO'); //CO - Concluida
define ('ACAO_CANCELADA', 'CA'); //CA - Cancelada
define ('ACAO_INICIO_ATRADASO', 'IA'); //IA - Inicio atrasado
define ('ACAO_TERMINO_ATRASADO', 'TA'); //TA - Termino atrasado'


// ANO EXERCICIO
if(isset($_SESSION['exercicio']) && !empty($_SESSION['exercicio']))  {
    /**
     * Ano exercicio, definido no topo da pagina.
     */
    define('AEXANO', $_SESSION['exercicio']);
} else if(!$ano)  {
    /**
     * Ano exercicio, definido no topo da pagina.
     */
    define('AEXANO', date('Y'));
}


define('K_ORGCODIGO', '26000');

/********************************************
* 			  TIPOS DE DESPESA 				*
********************************************/

define('K_DESPESA_ENERGIA_ELETRICA', 1);
define('K_DESPESA_APOIO_ADM', 2);
define('K_DESPESA_VIGILANCIA', 3);
define('K_DESPESA_TELECOMUNICACOES', 4);
define('K_DESPESA_COLETA_SELETIVA', 5);
define('K_DESPESA_AGUA_ESGOTO', 6);
define('K_DESPESA_LIMPEZA', 7);
define('K_DESPESA_LOCACAO_IMOVEIS', 8);
define('K_DESPESA_MANUTENCAO_BENS', 9);
define('K_DESPESA_LOCACAO_VEICULOS', 10);
define('K_DESPESA_PROCESSAMENTO_DADOS', 11);
define('K_DESPESA_MATERIAL_CONSUMO', 12);
define('K_DESPESA_GENERICA', 13);
define('K_DESPESA_DIARIAS', 14);
define('K_DESPESA_PASSAGENS', 15);

define('K_UNIDADE_MEDIDA_OUTROS', 9);

/********************************************/

/********************************************
* 			       PERFIS 				    *
********************************************/

if( $_SESSION['baselogin'] == 'simec_desenvolvimento' ){
    define('K_PERFIL_SUPER_USUARIO', 895);
    define('K_PERFIL_LIDER_UO', 908);
    define('K_PERFIL_CONSULTA', 912);
    define('K_PERFIL_CADASTRADOR_UO', 907);
} else {
    define('K_PERFIL_SUPER_USUARIO', 958);
    define('K_PERFIL_LIDER_UO', 959);
    define('K_PERFIL_CONSULTA', 961);
    define('K_PERFIL_CADASTRADOR_UO', 960);
}

/********************************************/
