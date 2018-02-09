<?php

ini_set("memory_limit", "3024M");
set_time_limit(0);

define('BASE_PATH_SIMEC', realpath(dirname(__FILE__) . '/../../../'));

$_REQUEST['baselogin'] = "simec_espelho_producao"; //simec_desenvolvimento
// carrega as funções gerais
require_once BASE_PATH_SIMEC . "/global/config.inc";
require_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/human_gateway_client_api/HumanClientMain.php";

// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '00000000191';
$_SESSION['usucpf'] = '00000000191';
$_SESSION['sisid'] = '147';


$db = new cls_banco();


include_once APPRAIZ . 'www/obras2/_constantes.php';
include_once APPRAIZ . 'www/obras2/_funcoes.php';
include_once APPRAIZ . 'www/obras2/_componentes.php';
include_once APPRAIZ . "www/autoload.php";
include_once APPRAIZ . "includes/classes/Modelo.class.inc";
include_once APPRAIZ . "includes/classes/modelo/obras2/Obras.class.inc";
include_once APPRAIZ . "includes/classes/modelo/obras2/Email.class.inc";
require_once APPRAIZ . 'includes/classes/dateTime.inc';
require_once APPRAIZ . 'includes/classes/modelo/obras2/Contato.class.inc';
require_once APPRAIZ . 'includes/classes/modelo/obras2/DestinatarioEmail.class.inc';
require_once APPRAIZ . 'includes/classes/modelo/obras2/OrdemServicoMI.class.inc';
require_once APPRAIZ . 'includes/classes/modelo/obras2/ItensComposicaoObras.class.inc';
require_once APPRAIZ . 'includes/classes/modelo/obras2/Cronograma_PadraoMi.class.inc';
require_once APPRAIZ . 'includes/classes/modelo/obras2/Itens_Composicao_PadraoMi.class.inc';
require_once APPRAIZ . 'includes/classes/modelo/obras2/QtdItensComposicaoObraMi.class.inc';
require_once APPRAIZ . 'includes/classes/modelo/obras2/ObrasContrato.class.inc';
require_once APPRAIZ . 'includes/classes/modelo/obras2/Licitacao.class.inc';
require_once APPRAIZ . 'includes/classes/modelo/obras2/Contrato.class.inc';
require_once APPRAIZ . 'includes/classes/modelo/obras2/ObraLicitacao.class.inc';
require_once APPRAIZ . 'includes/workflow.php';




$sql = "

    SELECT
        os.osmid,
        os.tomid,
        os.obrid,
        h.usucpf,
        d.docid as docid_os,
        d.hstid as hstid_os,
        a.esdidorigem as esdidorigem_os,
        (SELECT h.hstid FROM workflow.historicodocumento h WHERE h.docid = d.docid ORDER BY h.htddata DESC LIMIT 1 OFFSET 1) as hstid_origem_os,
        d1.docid as docid_obr,
        d1.hstid as hstid_obr,
        ao.esdidorigem as esdidorigem_obr,
        (SELECT h.hstid FROM workflow.historicodocumento h WHERE h.docid = d1.docid ORDER BY h.htddata DESC LIMIT 1 OFFSET 1) as hstid_origem_obr
    FROM obras2.ordemservicomi  os
    JOIN workflow.documento d ON d.docid = os.docid
    JOIN workflow.estadodocumento e ON e.esdid = d.esdid
    JOIN workflow.historicodocumento h ON h.hstid = d.hstid
    JOIN workflow.acaoestadodoc a ON a.aedid = h.aedid
    JOIN obras2.obras o ON o.obrid = os.obrid AND o.obrstatus = 'A' AND o.obridpai IS NULL
    JOIN workflow.documento d1 ON d1.docid = o.docid
    JOIN workflow.estadodocumento eo ON eo.esdid = d1.esdid
    JOIN workflow.historicodocumento ho ON ho.hstid = d1.hstid
    JOIN workflow.acaoestadodoc ao ON ao.aedid = ho.aedid
    WHERE
        os.osmstatus = 'A' AND
        e.esdid = 905 AND
        h.aedid = 2082 AND
        h.usucpf NOT IN (SELECT usucpf FROM obras2.usuarioresponsabilidade urs WHERE  urs.rpustatus = 'A'  AND urs.pflcod IN (1008, 1036)) AND
        eo.esdid NOT IN (863, 693) AND

        CASE WHEN os.tomid = 1 THEN
            CASE WHEN ao.esdidorigem = 873 THEN  TRUE ELSE FALSE END
        ELSE TRUE END

    ORDER BY os.tomid, eo.esddsc;

";


$obras = $db->carregar($sql);

$sql = array();
foreach ($obras as $obra){
    // Atualiza workflow OS
    $sql[] = "/* {$obra['obrid']} */ UPDATE workflow.documento SET hstid = {$obra['hstid_origem_os']}, esdid = {$obra['esdidorigem_os']} WHERE docid = {$obra['docid_os']};";
    $sql[] = "/* {$obra['obrid']} */ DELETE FROM workflow.historicodocumento WHERE hstid = {$obra['hstid_os']};";

    // Se os de execucao, voltar o workflow de determinadas obras
    if($obra['tomid'] == '1') {
        $sql[] = "/* {$obra['obrid']} */ UPDATE workflow.documento SET hstid = {$obra['hstid_origem_obr']}, esdid = {$obra['esdidorigem_obr']} WHERE docid = {$obra['docid_obr']};";
        $sql[] = "/* {$obra['obrid']} */ DELETE FROM workflow.historicodocumento WHERE hstid = {$obra['hstid_obr']};";
    }
}

echo implode('<br />', $sql);











exit;
/**
 * Corrige todos os contratos, ou cria um novo, preenchendo a data de inico, fim, e valor, vindos dos dados da os
 */

$sql = "SELECT o.obrid, h.osmid, h.osmdtinicio, h.osmdttermino, oc.ocrid, oc.crtid
            FROM obras2.obras o
              JOIN workflow.documento d ON d.docid = o.docid
              LEFT JOIN obras2.obrascontrato           oc ON oc.obrid   = o.obrid AND oc.ocrstatus = 'A'
              LEFT JOIN (
                     SELECT os.* FROM obras2.ordemservicomi os
                       JOIN workflow.documento d ON d.docid = os.docid
                       JOIN workflow.historicodocumento h ON d.docid = h.docid AND h.aedid = 2079
                     WHERE os.osmstatus = 'A' AND os.tomid = 1) h ON h.obrid = o.obrid
            WHERE o.obridpai IS NULL AND o.obrstatus = 'A' AND o.tpoid IN (104,105) AND d.esdid IN (690, 693)  AND oc.ocrdtinicioexecucao IS NULL; ";

$obras = $db->carregar($sql);

foreach ($obras as $obra) {

    $dadosContrato = array();
    $obrasContrato = new ObrasContrato();
    $objObra = new Obras($obra['obrid']);

    // Endereçp
    $end = $objObra->getEnderecoObra($objObra->obrid);

    // OS Execução
    $ordemservico = new OrdemServicoMI();
    $os = current($ordemservico->recuperarTodos("*", array("obrid = ".$objObra->obrid, "tomid = 1" ) ));

    // Cronograma
    $cronogramaPadrao = new Cronograma_PadraoMi();
    $cronograma = $cronogramaPadrao->pegaCronogramaPadrao($end['estuf'], $objObra->tpoid);

    // Empresa
    $sql = "SELECT
                               *
                            FROM obras2.empresami em
                            JOIN obras2.empresami_uf euf ON euf.emiid = em.emiid AND euf.eufstatus = 'A'
                            WHERE emistatus = 'A' AND euf.estuf = '{$end['estuf']}'";
    $empresa = $db->pegaLinha($sql);

    if (!empty($obra['ocrid'])) {
        $obrasContrato = new ObrasContrato($obra['ocrid']);
        $dadosContrato = $obrasContrato->getDados();
    } else {
        // Cria contrato
        $contrato = new Contrato();

        $licitacao = new Licitacao();
        $licitacao->popularDadosObjeto(array('orgid' => 3));
        $licitacao->salvar();

        $obralic = new ObraLicitacao();
        $obralic->popularDadosObjeto(array('obrid' => $obra['obrid'], 'licid' => $licitacao->licid));
        $obralic->salvar();

        $dados = array(
            'orgid' => 3,
            'crtvalorexecucao' => $cronograma['cpmvalor'],
            'crtprazovigencia' => $os['osmprazo'],
            'crtdttermino' => $os['osmdttermino'],
            'licid' => $licitacao->licid,
            'mlid' => 2
        );
        $contrato->popularDadosObjeto($dados);
        $contrato->salvar();
        $contrato->commit();
        $dadosContrato['crtid'] = $contrato->crtid;
    }


    $dadosContrato['ocrid'] = $obra['ocrid'];
    $dadosContrato['obrid'] = $obra['obrid'];
    $dadosContrato['ocrdtinicioexecucao'] = $os['osmdtinicio'];
    $dadosContrato['ocrdtterminoexecucao'] = $os['osmdttermino'];
    $dadosContrato['ocrprazoexecucao'] = $os['osmprazo'];
    $dadosContrato['ocrvalorexecucao'] = $cronograma['cpmvalor'];
    $dadosContrato['ocraditivado'] = 'f';

    $obrasContrato->popularDadosObjeto($dadosContrato);
    $obrasContrato->salvar();
    $obrasContrato->commit();
}

ver($obras, d);
exit;


/**
 * Corrige as obras que foram afetadas pela mudanca de regra no aceites
 */

$sql = "SELECT o.obrid, o.osmid, o.tomid, o.osmdtinicio, o.osmdttermino, TO_CHAR(h.htddata, 'YYYY-MM-DD') as htddata, o.osmprazo FROM obras2.ordemservicomi o
              JOIN (
                      SELECT os.osmid, MIN(h.hstid) as hstid, MIN(h.htddata) as htddata FROM obras2.ordemservicomi os
                      JOIN workflow.documento d ON d.docid = os.docid
                        JOIN workflow.historicodocumento h ON d.docid = h.docid AND h.aedid = 2082
                      WHERE os.osmstatus = 'A'
                      GROUP BY os.osmid ) h ON h.osmid = o.osmid
              ORDER BY o.osmid
            ;";

$oss = $db->carregar($sql);

foreach ($oss as $os){
    $ordemservico = new OrdemServicoMI();

    $sql = "UPDATE obras2.ordemservicomi SET
                            osmdtinicio = '{$os['htddata']}',
                            osmdttermino = '{$os['htddata']}'::date + '{$os['osmprazo']} days'::interval WHERE osmid = {$os['osmid']}";

    $ordemservico->executar($sql);
    $ordemservico->commit();

    if($os['tomid'] == 1) {
        $obra = new Obras($os['obrid']);
        $obra->exportarCronogramaPadraoParaObra();
    }
}
ver($oss, d);
exit;

/**
 * Corrige as obras que foram tramitadas OS Recusada por uma os de sondagem ou implantação
 */

$sql = "SELECT o.obrid, o.docid FROM obras2.obras o
  JOIN workflow.documento d ON d.docid = o.docid
  WHERE o.obridpai IS NULL AND o.obrstatus = 'A' AND o.tpoid IN (104,105) AND d.esdid = 874 AND o.obrid NOT IN (SELECT os.obrid FROM obras2.ordemservicomi os
    JOIN workflow.documento d ON d.docid = os.docid
  WHERE os.tomid = 1 AND d.esdid = 910) LIMIT 10";


$obras = $db->carregar($sql);

foreach ($obras as $obra) {
    $historico = $db->carregar("SELECT * FROM workflow.historicodocumento WHERE docid = {$obra['docid']} ORDER BY htddata DESC LIMIT 2");

        if($historico[0]['aedid'] == 1973){
            $sql = "UPDATE workflow.documento SET esdid = 873, hstid = {$historico[1]['hstid']} WHERE docid = {$obra['docid']}";
            $sql2 = "DELETE FROM workflow.historicodocumento WHERE hstid = {$historico[0]['hstid']}";
            $db->executar($sql);
            $db->executar($sql2);
        }

}
$db->commit();
ver($obras, d);
exit;

/**
 * Script para atualizar as obras que estao na situação 873 e ainda não possuem OS
 */

$sqlObras = "SELECT o.*, u.usunome,  to_char(h.htddata,'DD/MM/YYYY') htddata FROM obras2.obras o
  LEFT JOIN workflow.documento        d ON d.docid  = o.docid
  LEFT JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid
  LEFT JOIN workflow.historicodocumento h ON h.hstid = d.hstid
  LEFT JOIN seguranca.usuario u ON u.usucpf = h.usucpf
  WHERE
    o.obridpai IS NULL AND
    o.obrstatus = 'A' AND
    o.tpoid IN (104,105) AND
    d.esdid = 873 AND
    o.obrid NOT IN (SELECT sm.obrid FROM obras2.ordemservicomi sm
      LEFT JOIN workflow.documento        d ON d.docid  = sm.docid
      LEFT JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid
    WHERE tomid = 1 AND sm.osmstatus = 'A' AND d.esdid = 904)
";

$obras = $db->carregar($sqlObras);

foreach ($obras as $obra) {
    $r = wf_alterarEstado($obra['docid'], 2663, 'A obra foi tramitada no SIMEC para "Aguardando aceite da O.S pelo fornecedor" sem o cadastramento da Ordem de Serviços de Execução. Por este motivo ela retornou para "Aguardando emissão da O.S. pelo fornecedor".', array('obrid' => $obra['obrid']));

    $email = new Email();
    $data = new Data();
    $data = $data->formataData($data->dataAtual(), 'Brasília, DD de mesTextual de YYYY.');
    $emailTemplate = '

    <html>
        <head>
            <title></title>
        </head>
        <body>
            <table style="width: 100%;">
                <thead>
                    <tr>
                        <td style="text-align: center;">
                            <p><img  src="data:image/png;base64,'. base64_encode( file_get_contents( APPRAIZ. '/www/' . 'imagens/brasao.gif' ) ) . '" width="70"/><br/>
                            <b>MINISTÉRIO DA EDUCAÇÃO</b><br/>
                            FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO - FNDE<br/>
                            DIRETORIA DE GESTÃO, ARTICULAÇÃO E PROJETOS EDUCACIONAIS - DIGAP<br/>
                            COORDENAÇÃO GERAL DE IMPLEMENTAÇÃO E MONITORAMENTO DE PROJETOS EDUCACIONAIS - CGIMP<br/>
                            SBS Q.2 Bloco F Edifício FNDE - 70.070-929 - Brasília, DF - Telefone: (61) 2022.4696/4694 - E-mail: monitoramento.obras@fnde.gov.br<br/>
                        </td>
                    </tr>
                    <tr>
                        <td style="text-align: right; padding: 40px 0 0 0;">
                            '.$data.'
                        </td>
                    </tr>
                </thead>
                <tbody>

                    <tr>
                        <td style="line-height: 15px; text-align:justify">
                            <p>A obra ' .'('.$obra['obrid'] .') ' . $obra['obrnome'] . ' foi tramitada no SIMEC para "Aguardando aceite da O.S pelo fornecedor" sem o cadastramento da Ordem de Serviços de Execução, pelo usuário '.$obra['usunome'].', em '.$obra['htddata'].'.</p>
                            <p>Por este motivo ela retornou para "Aguardando emissao da O.S. pelo fornecedor".</p>
                            <p>Para enviá-la novamente para a situação "Aguardando aceite da O.S. pelo forncedor" é preciso inserir a O.S. de execução através do menu Principal->Metodologias Inovadoras->Ordens de Serviço e tramitar a O.S. inserida para aceite pelo fornecedor.</p>
                            <p>Em caso de dúvidas consultar o manual "Emissão e aceite de O.S." no link:</p>
                            <p>http://www.fnde.gov.br/programas/proinfancia/proinfancia-manuais</p>
                        </td>
                    </tr>

                </tbody>
                <tfoot>

                </tfoot>
            </table>
        </body>
    </html>
    ';

    $dados = array(
        'usucpf' => $_SESSION['usucpf'],
        'emlconteudo' => $emailTemplate,
        'emlassunto' => 'Retorno da obra ' . $obra['obrnome'] . ' para Aguardando Emissão de O.S.',
        'temid' => 10,
        'emlregistroatividade' => true,
        'obrid' => $obra['obrid']
    );


    $contato = new Contato();
    $dadosRemetentes = array();
    $sql = $contato->ReponsavelObraEGestorUnidade($obra['empid']);
    $resp = $db->carregar($sql);
    if(empty($resp))
        continue;

    foreach($resp as $usuario){
        $dadosRemetentes[] = $usuario['usuemail'];
    }

    $email->popularDadosObjeto($dados);
    $email->salvar($dadosRemetentes);

    $db->commit();

    echo $obra['obrid'] . '<br />';
}