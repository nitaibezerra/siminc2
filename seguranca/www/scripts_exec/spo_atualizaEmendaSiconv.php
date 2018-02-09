<?php
/**
 * Carrega os dados financeiros do SIOP para a base do SIMEC.
 *
 * Assim que termina de baixar os dados financeiros, o script roda um processamento
 * que coloca os dados na tabela <tt>spo.siopexecucao</tt>. O acompanhamento das p�ginas
 * da execu��o j� baixadas � feito na tabela <tt>spo.siopexecucao_acompanhamento</tt>.
 * Ao final da execu��o, � enviado um e-mail com o resultado do processo.
 *
 * Sequ�ncia de execu��o:<br />
 * <ol><li>Baixa os dados do webservice (WSQuantitativo.consultarExecucaoOrcamentaria);</li>
 * <li>Apaga os dados da tabela wssof.ws_execucaoorcamentaria;</li>
 * <li>Insere os dados retornados pelo webservice na tabela wssof.ws_execucaoorcamentaria;</li>
 * <li>Executa o script de atualiza��o de finaceiros na seguinte tabela: spo.siopexecucao;</li>
 * <li>Envia e-mail com resultado da execu��o.</li></ol>
 *
 * @version $Id: spo_BaixarDadosFinanceirosSIOP.php 101880 2015-08-31 19:50:33Z maykelbraz $
 * @link http://simec.mec.gov.br/seguranca/scripts_exec/spo_BaixarDadosFinanceirosSIOP.php URL de execu��o.
 */

// -- Setup
ini_set('display_errors', 1);
ini_set('error_reporting', E_ALL & ~E_NOTICE);
set_time_limit(0);
ini_set("memory_limit", "2048M");
define('BASE_PATH_SIMEC', realpath(dirname(__FILE__) . '/../../../'));
session_start();
$_SESSION['baselogin'] = "simec_espelho_producao";

// -- Includes necess�rios ao processamento
/**
 * Carrega as configura��es gerais do sistema.
 * @see config.inc
 */
require_once BASE_PATH_SIMEC . "/global/config.inc";

/**
 * Carrega as classes do simec.
 * @see classes_simec.inc
 */
require_once APPRAIZ . "includes/classes_simec.inc";

/**
 * Carrega as fun��es b�sicas do simec.
 * @see funcoes.inc
 */
require_once APPRAIZ . "includes/funcoes.inc";

/**
 * Classe de conex�o com o SIOP, servi�o WSQuantitativo.
 * @see Spo_Ws_Sof_Quantitativo
 */
//require_once(APPRAIZ . 'spo/ws/sof/Quantitativo.php');

// -- Abrindo conex�o com o banco de dados
$db = new cls_banco();


include_once APPRAIZ . "emendas/classes/model/Siconv.inc";
include_once APPRAIZ . "emendas/classes/model/Emenda.inc";
include_once APPRAIZ . "emendas/classes/model/Beneficiario.inc";
include_once APPRAIZ . "emendas/classes/model/SiconvParecer.inc";
include_once APPRAIZ . "emendas/classes/model/SiconvSituacao.inc";
include_once APPRAIZ . "emendas/classes/model/SiconvBeneficiario.inc";
include_once APPRAIZ . "siconv/classes/model/PropostaWs.inc";
//include_once APPRAIZ."emenda/classes/SoapIntegracaoSiconv.class.inc";
//include_once APPRAIZ."emenda/classes/WSIntegracaoSiconv.class.inc";
include_once APPRAIZ . "emenda\classes\WSIntegracaoSiconv.class.inc";

$urlWsdl = 'https://ws.convenios.gov.br/siconv-siconv-interfaceSiconv-1.0/InterfaceSiconvHandlerBeanImpl?wsdl';

$mPropostaWS = new Siconv_Model_PropostaWs();
//$aPropostas = $mPropostaWS->recuperarTodos('situacaopropostaws, parecerproposta, ano, idhash, objetoconvenio, sequencial, numeroconvenio, propostaprogramaws, parecerplanotrabalho', ['id = 155']);
$campos = 'situacaopropostaws, parecerproposta, ano, idhash, objetoconvenio, sequencial, numeroconvenio, propostaprogramaws, parecerplanotrabalho';
$filtro = [
    "coalesce(situacaopropostaws, '') != ''",
    "coalesce(parecerproposta, '') != ''",
];
ver(123, d);

$aPropostas = $mPropostaWS->recuperarTodos($campos, $filtro);

ver($aPropostas, d);

foreach($aPropostas as $proposta){

    // SITUA��O
    $situacao = json_decode($proposta['situacaopropostaws']);

    $sql = "select sitid from emendas.siconvsituacao where sitcodigo = '{$situacao->value}'";
    $sitid = $db->pegaUm($sql);

    if(!$sitid){
        $mSiconvSituacao = new Emendas_Model_SiconvSituacao();

        $mSiconvSituacao->sitdsc = $situacao->value;
        $mSiconvSituacao->sitcodigo = $situacao->value;
        
        $mSiconvSituacao->salvar();
        $sitid = $mSiconvSituacao->sitid;
    }

    $sql = "select sicid from emendas.siconv where sicsequencial = " . (int) $proposta['sequencial'];
    $sicid = $db->pegaUm($sql);
    $mSiconv = new Emendas_Model_Siconv($sicid);

    $mSiconv->sitid = $sitid;
    $mSiconv->prsano = $proposta['ano'];
    $mSiconv->idhash = $proposta['idhash'];
    $mSiconv->sicobjeto = $proposta['objetoconvenio'];
    $mSiconv->sicsequencial = $proposta['sequencial'];
    $mSiconv->numeroconvenio = $proposta['numeroconvenio'];

    $mSiconv->salvar();

    $aParecer[Emendas_Model_SiconvParecer::K_TIPO_PROPOSTA] = json_decode($proposta['parecerproposta']);
    $aParecer[Emendas_Model_SiconvParecer::K_TIPO_PLANO_TRABALHO] = json_decode($proposta['parecerplanotrabalho']);

    $mSiconvParecer = new Emendas_Model_SiconvParecer();

    $aCampos['data'] = [Emendas_Model_SiconvParecer::K_TIPO_PROPOSTA => 'dataWS', Emendas_Model_SiconvParecer::K_TIPO_PLANO_TRABALHO => 'data'];

    foreach($aParecer as $spatipo => $pareceres){
        if(is_array($pareceres)){
            foreach($pareceres as $parecer){
                $mSiconvParecer->sicid = $mSiconv->sicid;
                $mSiconvParecer->spadsc = $parecer->parecer;
                $mSiconvParecer->spadata = $parecer->{$aCampos['data'][$spatipo]};
                $mSiconvParecer->idhash = $parecer->idHash;
                $mSiconvParecer->spatipo = $spatipo;

                $mSiconvParecer->salvar();
                $mSiconvParecer->spaid = null;
            }
        }
    }

    $aPrograma = json_decode($proposta['propostaprogramaws']);
    $aPrograma = is_array($aPrograma) ? $aPrograma : [$aPrograma];


    foreach($aPrograma as $programa){

        $origemRecursoPropProgramaWS = $programa->origemRecursoPropProgramaWS;
        $origemRecursoPropProgramaWS = is_array($origemRecursoPropProgramaWS) ? $origemRecursoPropProgramaWS : [$origemRecursoPropProgramaWS];

        foreach($origemRecursoPropProgramaWS as $propProgramaWS){

            $cnpjProgramaEmendaWS = isset($propProgramaWS->cnpjProgramaEmendaWS) ? $propProgramaWS->cnpjProgramaEmendaWS : $propProgramaWS;

            // BENEFICI�RIO
            $procnpj = $cnpjProgramaEmendaWS->cnpj;
            $emenumero = $proposta['ano'] . $cnpjProgramaEmendaWS->numeroEmendaParlamentar;


            $sql = "select ben.benid
                    from emendas.emenda eme
                        inner join emendas.beneficiario ben on ben.emeid = eme.emeid
                        inner join emendas.proponente pro on pro.proid = ben.proid
                    where eme.emenumero = '{$emenumero}'
                    and pro.procnpj = '{$procnpj}'";
            $benid = $db->pegaUm($sql);

            $sql = "select 1 from emendas.siconvbeneficiario where emenumero = '{$emenumero}' and procnpj = '{$procnpj}' and sicid = {$mSiconv->sicid}";
            $existeSiconv = $db->pegaUm($sql);
            if(!$existeSiconv){
                $mSiconvBeneficiario = new Emendas_Model_SiconvBeneficiario();
                $mSiconvBeneficiario->sicid = $mSiconv->sicid;
                $mSiconvBeneficiario->emenumero = $emenumero;
                $mSiconvBeneficiario->procnpj = $procnpj;
                $mSiconvBeneficiario->benid = (int)$benid;
                $mSiconvBeneficiario->salvar();
            }

            if(!$benid){
                $aBen[$benid][] = $sql;
               continue;
            }


            $mBeneficiario = new Emendas_Model_Beneficiario($benid);

            $mBeneficiario->sicid = $mSiconv->sicid;
            $mBeneficiario->salvar();

        }
    }
    $mSiconv->commit();
    $mSiconv->sicid = null;
}
ver($aBen, "Fim", d);



/**********************************
*   CRIA��O DO DDL DAS TABELAS    *
**********************************/
//    $create = "
//CREATE TABLE siconv.PropostaWS
//(
//    id serial NOT NULL,";
//
//    if(isset($retorno->propostas)){
//        foreach ($retorno->propostas as $indice => $PropostaWS){
////            foreach ($PropostaWS as $indice => $dados){
//            foreach (array_keys((array)$PropostaWS) as $indice => $campo){
//                $tipo = 'character varying (2000)';
//
//                if(strpos($campo, 'data')!== false){
//                    $tipo = 'timestamp with time zone';
//                } elseif('WS' == substr($campo, -2)){
//                    $tipo = 'text';
//                } elseif(strpos($campo, 'valor')!== false){
//                    $tipo = 'numeric(15, 2)';
//                }
//
//                $create .= "
//    {$campo} {$tipo},";
//            }
//            $create .= "
//    datacarga timestamp with time zone,
//    CONSTRAINT pk_PropostaWS_id PRIMARY KEY (id)
//);
//
//GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE siconv.propostaws TO usr_simec;
//GRANT SELECT, USAGE ON SEQUENCE siconv.propostaws_id_seq TO usr_simec;
//
//";
//            ver(strtolower($create), array_keys((array)$PropostaWS), $indice, $PropostaWS, d);
//        }
//    }
//}