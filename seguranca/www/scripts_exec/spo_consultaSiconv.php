<?php
/**
 * Carrega os dados financeiros do SIOP para a base do SIMEC.
 *
 * Assim que termina de baixar os dados financeiros, o script roda um processamento
 * que coloca os dados na tabela <tt>spo.siopexecucao</tt>. O acompanhamento das páginas
 * da execução já baixadas é feito na tabela <tt>spo.siopexecucao_acompanhamento</tt>.
 * Ao final da execução, é enviado um e-mail com o resultado do processo.
 *
 * Sequência de execução:<br />
 * <ol><li>Baixa os dados do webservice (WSQuantitativo.consultarExecucaoOrcamentaria);</li>
 * <li>Apaga os dados da tabela wssof.ws_execucaoorcamentaria;</li>
 * <li>Insere os dados retornados pelo webservice na tabela wssof.ws_execucaoorcamentaria;</li>
 * <li>Executa o script de atualização de finaceiros na seguinte tabela: spo.siopexecucao;</li>
 * <li>Envia e-mail com resultado da execução.</li></ol>
 *
 * @version $Id: spo_BaixarDadosFinanceirosSIOP.php 101880 2015-08-31 19:50:33Z maykelbraz $
 * @link http://simec.mec.gov.br/seguranca/scripts_exec/spo_BaixarDadosFinanceirosSIOP.php URL de execução.
 */

// -- Setup
ini_set('display_errors', 1);
ini_set('error_reporting', E_ALL & ~E_NOTICE);
set_time_limit(0);
ini_set("memory_limit", "2048M");
define('BASE_PATH_SIMEC', realpath(dirname(__FILE__) . '/../../../'));
session_start();
$_SESSION['baselogin'] = "simec_espelho_producao";

// -- Includes necessários ao processamento
/**
 * Carrega as configurações gerais do sistema.
 * @see config.inc
 */
require_once BASE_PATH_SIMEC . "/global/config.inc";

/**
 * Carrega as classes do simec.
 * @see classes_simec.inc
 */
require_once APPRAIZ . "includes/classes_simec.inc";

/**
 * Carrega as funções básicas do simec.
 * @see funcoes.inc
 */
require_once APPRAIZ . "includes/funcoes.inc";

/**
 * Classe de conexão com o SIOP, serviço WSQuantitativo.
 * @see Spo_Ws_Sof_Quantitativo
 */
//require_once(APPRAIZ . 'spo/ws/sof/Quantitativo.php');

// -- Abrindo conexão com o banco de dados
$db = new cls_banco();


include_once APPRAIZ . "emendas/classes/model/Siconv.inc";
include_once APPRAIZ . "emendas/classes/model/Emenda.inc";
include_once APPRAIZ . "emendas/classes/model/Beneficiario.inc";
include_once APPRAIZ . "emendas/classes/model/SiconvParecer.inc";
include_once APPRAIZ . "siconv/classes/model/PropostaWs.inc";
//include_once APPRAIZ."emenda/classes/SoapIntegracaoSiconv.class.inc";
//include_once APPRAIZ."emenda/classes/WSIntegracaoSiconv.class.inc";
include_once APPRAIZ . "emenda\classes\WSIntegracaoSiconv.class.inc";

$urlWsdl = 'https://ws.convenios.gov.br/siconv-siconv-interfaceSiconv-1.0/InterfaceSiconvHandlerBeanImpl?wsdl';

$usuario = '';
$senha = '';

$arrParam = array('ptrid' 	=> 1,
    'exfid' 	=> 2,
    'usuario' => $usuario,
    'senha' 	=> $senha,
    'url' 	=> $urlWsdl,
    'post' 	=> []
);

$exercicio = date('Y');

$obWS = new WSIntegracaoSiconv($arrParam);

$sql = "select eme.*, uno.unocod 
        from emendas.emenda eme
            inner join public.unidadeorcamentaria uno on uno.unoid = eme.unoid 
        where eme.prsano = '{$exercicio}'
        ";
$emendas = $db->carregar($sql);
$emendas = $emendas ? $emendas : [];

foreach($emendas as $emenda){
    $filtro = [
        'unocod'=>$emenda['unocod'],
        'emenumero'=>substr($emenda['emenumero'], 4)
    ];
    $filtro = [ 'unocod'=>42207, 'emenumero'=>14680001 ];

    $retorno = $obWS->consultarPropostaPorEmenda($filtro);

    ver($retorno, d);


//$sql = "select distinct aca.prgcod || aca.acacod as acao, unocodorcamento
//from emendas.emenda eme
//        inner join monitora.acao aca on aca.acaid = eme.acaid
//        inner join public.unidadeorcamentaria uno on uno.unoid = eme.unoid
//where eme.prsano = '$exercicio'";
//$acoes = $db->carregar($sql);
//
//foreach($acoes as $acao){
//
//    $_SESSION['unocodorcamento'] = $acao['unocodorcamento'];
//    $_SESSION['acao'] = $acao['acao'];
//
//    $filtro = ['unocod'=>$acao['unocodorcamento'], 'acao'=>$acao['acao']];
//    $filtro = ['unocod'=>42207, 'acao'=>'202720ZF'];
//    $filtro = ['unocod'=>20408, 'acao'=>'202720ZF'];

//    $retorno = $obWS->consultarPropostaPorEmenda($filtro);
//    $retorno = $obWS->consultarPropostaPorAcaoOrcamentaria($filtro);

    if(isset($retorno->propostas)){
        $mPropostaWS = new Siconv_Model_PropostaWs();
        
        foreach ($retorno->propostas as $PropostaWS){
            
            if($PropostaWS->ano < date('Y')){
                continue;
            }

            foreach ($PropostaWS as $indice => $dados){
                $indice = strtolower($indice);

                if(strpos($indice, 'data')!== false){

                } elseif(
                    'ws' == substr($indice, -2) ||
                    in_array($indice, ['parecerplanotrabalho', 'parecerproposta'])
                ){
                    $dados = json_encode($dados);
                }

                if(key_exists($indice, $mPropostaWS->arAtributos)){
                    $mPropostaWS->{$indice} = $dados;
                }
            }

//            $mPropostaWS->emeid = $emenda['emeid'];
            $mPropostaWS->salvar();
            $mPropostaWS->commit();
            $mPropostaWS->id = null;
        }
    }
//    ver('FIM', $mPropostaWS, d);
    sleep(5);
}

ver('FIM', $mPropostaWS, d);


$mPropostaWS = new Siconv_Model_PropostaWs();
$aPropostas = $mPropostaWS->recuperarTodos();
$aPropostas = $mPropostaWS->recuperarTodos('*', ['id = 155']);

foreach($aPropostas as $proposta){

    if(empty($proposta['situacaopropostaws']) && empty($proposta['parecerproposta'])){
        continue;
    }

    // SITUAÇÃO
    $situacao = json_decode($proposta['situacaopropostaws']);

    $sql = "select sitid from emendas.siconvsituacao where sitcodigo = '{$situacao->value}'";
    $sitid = $db->pegaUm($sql);

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

    $programa = json_decode($proposta['propostaprogramaws']);
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

    $origemRecursoPropProgramaWS = (array)$programa->origemRecursoPropProgramaWS;
    foreach($origemRecursoPropProgramaWS as $propProgramaWS){

        if(isset($propProgramaWS->cnpjProgramaEmendaWS->numeroEmendaParlamentar)){


            // BENEFICIÁRIO
            $procnpj = $propProgramaWS->cnpjProgramaEmendaWS->cnpj;
            $emenumero = $proposta['ano'] . $propProgramaWS->cnpjProgramaEmendaWS->numeroEmendaParlamentar;

            $sql = "select ben.benid
                    from emendas.emenda eme
                        inner join emendas.beneficiario ben on ben.emeid = eme.emeid
                        inner join emendas.proponente pro on pro.proid = ben.proid
                    where eme.emenumero = '{$emenumero}'
                    and pro.procnpj = '{$procnpj}'";
            $benid = $db->pegaUm($sql);

            if(!$benid){
               continue;
            }

            $mBeneficiario = new Emendas_Model_Beneficiario($benid);

            $mBeneficiario->sicid = $mSiconv->sicid;
            $mBeneficiario->salvar();
            
            $mSiconv->commit();
            $mSiconv->sicid = null;

            ver($emenda, $proposta, $procnpj, $propProgramaWS, $programa, d);

        }

//        ver($propProgramaWS->cnpjProgramaEmendaWS->numeroEmendaParlamentar, d);
    }
}
ver("Fim", d);



/**********************************
*   CRIAÇÃO DO DDL DAS TABELAS    *
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