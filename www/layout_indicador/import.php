<?php
/**
 * Sistema Integrado de Planejamento, Orçamento e Finanças do Ministério da Educação
 * Setor responsvel: DTI/SE/MEC
 * Autor: Cristiano Cabral <cristiano.cabral@gmail.com>
* Módulo: Segurança
* Finalidade: Tela de apresentação. Permite que o usuário entre no sistema.
* Data de criação: 24/06/2005
* Última modificação: 02/09/2013 por Orion Teles <orionteles@gmail.com>
*/

$_REQUEST['baselogin']  = "simec_espelho_producao";//simec_desenvolvimento

// carrega as bibliotecas internas do sistema
require_once 'config.inc';
require_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/funcoes.inc";
require_once APPRAIZ . "includes/library/simec/funcoes.inc";

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$sql = "select  i.indid
        from painel.indicador i
        where indstatus = 'A'";

$indicadores = $db->carregar($sql);
$indicadores = $indicadores ? $indicadores : array();


// Etapa
//ver(rand(1, 4), d);

// Tema
//ver(rand(1, 5), d);

$sqlTema = '';
$sqlEtapa = '';
foreach ($indicadores as $dados) {

//    $rand = rand(1, 3);
//    for ($i=0; $i<$rand; $i++) {
//        $sqlTema .= "insert into painel.indicadortemamec (indid, temid) values ({$dados['indid']}, " . rand(1, 5) . " );";
//    }

    $rand = rand(1, 2);
    for ($i=0; $i<$rand; $i++) {
        $sqlEtapa .= "insert into painel.indicadoretapaeducacao (indid, etpid) values ({$dados['indid']}, " . rand(1, 4) . ");";
    }
}


$db->executar($sqlTema);
$db->executar($sqlEtapa);
$db->commit();
