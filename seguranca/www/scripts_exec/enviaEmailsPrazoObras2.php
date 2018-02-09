<?php
ini_set( 'display_errors', 1 );
ini_set("memory_limit", "9024M");
ini_set("default_socket_timeout", "70000000");

define('BASE_PATH_SIMEC', realpath(dirname(__FILE__) . '/../../../'));

$_REQUEST['baselogin'] = "simec_espelho_producao";//simec_desenvolvimento
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
include_once APPRAIZ . "includes/classes/modelo/obras2/DestinatarioEmail.class.inc";
include_once APPRAIZ . "includes/classes/modelo/obras2/AnexoEmail.class.inc";
include_once APPRAIZ . "includes/classes/modelo/obras2/Email.class.inc";
include_once APPRAIZ . "includes/classes/modelo/obras2/ContatosObra.class.inc";
include_once APPRAIZ . "includes/classes/modelo/obras2/Restricao.class.inc";
include_once APPRAIZ . "includes/classes/dateTime.inc";

/*
  INSERT INTO obras2.tipoemail (temid, temnome, temdescricao)
    VALUES
        (29, 'Prazo da Obra', 'Prazo de Execução da Obra Planejamento Pelo Proponente'),
        (30, 'Prazo da Obra', 'Prazo de Execução da Obra Licitação'),
        (31, 'Prazo da Obra', 'Prazo de Execução da Outras Situações');

 */
$sql = "
            SELECT
                o.*,
                -- pag.*,
                -- rep.*,
                e.*,
                DATE_PART('days', NOW() - CASE WHEN pag.pagdatapagamento IS NOT NULL THEN pag.pagdatapagamento ELSE rep.drcdatapagamento END) days,
                CASE WHEN DATE_PART('days', NOW() - CASE WHEN pag.pagdatapagamento IS NOT NULL THEN pag.pagdatapagamento ELSE rep.drcdatapagamento END) >= 60 AND d.esdid = 689 THEN
                    1
                WHEN DATE_PART('days', NOW() - CASE WHEN pag.pagdatapagamento IS NOT NULL THEN pag.pagdatapagamento ELSE rep.drcdatapagamento END) >= 180 AND d.esdid = 763 THEN
                    2
                WHEN DATE_PART('days', NOW() - CASE WHEN pag.pagdatapagamento IS NOT NULL THEN pag.pagdatapagamento ELSE rep.drcdatapagamento END) >= 210 AND d.esdid NOT IN (763, 689) THEN
                    3
                END as regra
            FROM obras2.obras o
            LEFT JOIN
                (SELECT
                    ppo.preid,
                    MIN(pagdatapagamento) as pagdatapagamento
                FROM par.pagamentoobrapar ppo
                INNER JOIN par.pagamento pag ON pag.pagstatus = 'A' AND pag.pagsituacaopagamento ILIKE '%EFETIVADO%'
                GROUP BY ppo.preid

                UNION

                SELECT
                    ppo.preid,
                    MIN(pagdatapagamento) as pagdatapagamento
                FROM par.pagamentoobrapar ppo
                INNER JOIN par.pagamento pag ON pag.pagid = ppo.pagid AND pag.pagstatus = 'A' AND pag.pagsituacaopagamento ILIKE '%EFETIVADO%'
                GROUP BY ppo.preid
                ) as pag ON pag.preid = o.preid

            LEFT JOIN (
                SELECT
                    drcprocesso,
                    MIN(drcdatapagamento::date) as drcdatapagamento
                FROM painel.dadosrepassesconvenios drc
                GROUP BY drcprocesso
            ) as rep ON  rep.drcprocesso = Replace(Replace(Replace(Replace(o.obrnumprocessoconv,'.',''),';',''),'/',''),'-','')
            JOIN workflow.documento d ON d.docid = o.docid
            JOIN workflow.estadodocumento e ON e.esdid = d.esdid AND (e.esdid NOT IN (690, 693, 766, 764, 769, 691, 1084, 1230))
            WHERE
                o.obridpai IS NULL
                AND o.obrstatus = 'A'
                AND (pag.pagdatapagamento IS NOT NULL OR rep.drcdatapagamento IS NOT NULL)
                AND (
                    (DATE_PART('days', NOW() - CASE WHEN pag.pagdatapagamento IS NOT NULL THEN pag.pagdatapagamento ELSE rep.drcdatapagamento END) >= 60 AND d.esdid = 689) OR
                    (DATE_PART('days', NOW() - CASE WHEN pag.pagdatapagamento IS NOT NULL THEN pag.pagdatapagamento ELSE rep.drcdatapagamento END) >= 180 AND d.esdid = 763) OR
                    (DATE_PART('days', NOW() - CASE WHEN pag.pagdatapagamento IS NOT NULL THEN pag.pagdatapagamento ELSE rep.drcdatapagamento END) >= 210 AND d.esdid NOT IN (763, 689))
                )
                AND
                CASE WHEN DATE_PART('days', NOW() - CASE WHEN pag.pagdatapagamento IS NOT NULL THEN pag.pagdatapagamento ELSE rep.drcdatapagamento END) >= 60 AND d.esdid = 689 THEN
                            (select count(*) from obras2.email eml
                    where
                        eml.emlid IN (select emlid from obras2.email eml
                                where eml.obrid = o.obrid and eml.temid = 29 and (eml.emlstatus = 'S' or eml.emlstatus = 'A')
                                order by eml.emlid desc
                                limit 1)
                        AND ((emldata::date + interval '15 day') < now()) ) > 0
                        WHEN DATE_PART('days', NOW() - CASE WHEN pag.pagdatapagamento IS NOT NULL THEN pag.pagdatapagamento ELSE rep.drcdatapagamento END) >= 180 AND d.esdid = 763 THEN
                            (select count(*) from obras2.email eml
                    where
                        eml.emlid IN (select emlid from obras2.email eml
                                where eml.obrid = o.obrid and eml.temid = 30 and (eml.emlstatus = 'S' or eml.emlstatus = 'A')
                                order by eml.emlid desc
                                limit 1)
                        AND ((emldata::date + interval '15 day') < now()) ) > 0
                        WHEN DATE_PART('days', NOW() - CASE WHEN pag.pagdatapagamento IS NOT NULL THEN pag.pagdatapagamento ELSE rep.drcdatapagamento END) >= 210 AND d.esdid NOT IN (763, 689) THEN
                            (select count(*) from obras2.email eml
                    where
                        eml.emlid IN (select emlid from obras2.email eml
                                where eml.obrid = o.obrid and eml.temid = 31 and (eml.emlstatus = 'S' or eml.emlstatus = 'A')
                                order by eml.emlid desc
                                limit 1)
                        AND ((emldata::date + interval '15 day') < now()) ) > 0
                END
";
//$sql = "SELECT * FROM obras2.vm_tmp_pagamentoobras LIMIT 40";
$obras = $db->carregar($sql);
$email = new Email();

$obrasRegra1 = array();
$obrasRegra2 = array();
$obrasRegra3 = array();

foreach ($obras as $obra) {
    if ($obra['regra'] == 1)
        $obrasRegra1[] = $obra;
    else if ($obra['regra'] == 2)
        $obrasRegra2[] = $obra;
    else if ($obra['regra'] == 3)
        $obrasRegra3[] = $obra;
}

foreach ($obrasRegra1 as $obra){

    $conteudo = "
        A Obra ({$obra['obrid']}) {$obra['obrnome']} tinha 60 dias após o primeiro repasse para sair da situação Planejamento pelo proponente.
    ";

    enviaEmail($obra['obrid'], 29, $conteudo);
}

foreach ($obrasRegra2 as $obra){

    $conteudo = "
        A Obra ({$obra['obrid']}) {$obra['obrnome']} tinha 180 dias após o primeiro repasse para sair da situação licitação e entrar em execução.
    ";

    enviaEmail($obra['obrid'], 30, $conteudo);
}

foreach ($obrasRegra3 as $obra){

    $conteudo = "
        A Obra ({$obra['obrid']}) {$obra['obrnome']} tinha 210 dias após o primeiro repasse para entrar em execução.
    ";

    enviaEmail($obra['obrid'], 31, $conteudo);
}


ver($obrasRegra1, $obrasRegra2, $obrasRegra3, 'EXECUTADO', d);

function enviaEmail($obrid, $temid, $conteudo){
    $obra = new Obras($obrid);

    $destinatarios = getResponsaveis($obrid);

    if(empty($destinatarios))
        return false;

    $data = new Data();
    $data = $data->formataData($data->dataAtual(), 'Brasília, DD de mesTextual de YYYY.');
    $dados = array(
        'usucpf' => $_SESSION['usucpf'],
        'emlconteudo' => '
                        <html>
                            <head>
                                <title></title>
                            </head>
                            <body>
                                <table style="width: 100%;">
                                    <thead>
                                        <tr>
                                            <td style="text-align: center;">
                                                <p><img  src="data:image/png;base64,' . base64_encode(file_get_contents(APPRAIZ . '/www/' . 'imagens/brasao.gif')) . '" width="70"/><br/>
                                                <b>MINISTÉRIO DA EDUCAÇÃO</b><br/>
                                                FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO - FNDE<br/>
                                                DIRETORIA DE GESTÃO, ARTICULAÇÃO E PROJETOS EDUCACIONAIS - DIGAP<br/>
                                                COORDENAÇÃO GERAL DE IMPLEMENTAÇÃO E MONITORAMENTO DE PROJETOS EDUCACIONAIS - CGIMP<br/>
                                                SBS Q.2 Bloco F Edifício FNDE - 70.070-929 - Brasília, DF - E-mail: monitoramento.obras@fnde.gov.br<br/>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td>
                                                <p style="float:left; text-align: left; padding: 40px 0 0 0;">Comunicado Nº __RGAID__ - CGIMP/DIGAP/FNDE</p>
                                                <p style="float-right; text-align: right; padding: 40px 0 0 0;">' . $data . '</p>
                                            </td>
                                        </tr>
                                    </thead>
                                    <tbody>
                                        <tr>
                                            <td style="padding:20px 0 20px 0;">
                                              Assunto: <b>Prazo para a execução da obra (' . $obrid . ') ' . $obra->obrnome . '</b>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="line-height: 15px; text-align:justify">
                                                <p>Senhor,</p>

                                                <p>' . $conteudo . '</p>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td>&nbsp;</td>
                                            <td>&nbsp;</td>
                                        </tr>
                                        <tr>
                                            <td style="line-height: 15px; text-align:center; bgcolor: #ccc;" colspan="2">
                                                <b> ESTE E-MAIL FOI ENVIADO AUTOMATICAMENTE PELO SISTEMA, FAVOR NÃO RESPONDER. </b>
                                            </td>
                                        </tr>
                                    </tbody>
                                    <tfoot>
                                        <tr>
                                            <td>&nbsp;</td>
                                            <td>&nbsp;</td>
                                        </tr>
                                    </tfoot>
                                </table>
                            </body>
                        </html>
                                    ',
        'emlassunto' => 'Prazo para a execução da obra ' . $obra->obrnome,
        'temid' => $temid,
        'emlregistroatividade' => true,
        'obrid' => $obrid
    );

    $email = new Email();
    $email->popularDadosObjeto($dados);
    $email->salvar($destinatarios);
    $email->enviar();
}

function getResponsaveis($obrid){
    $contato = new ContatosObra();
    $contatos = $contato->getContatos($obrid);
    $contatos = (empty($contatos) ? array() : $contatos);
    foreach ($contatos as $c) {
        if ($c['usuemail']) {
            $dadosRemetentes[] = $c['usuemail'];
        }
    }
    return $dadosRemetentes;
}

//function criaRestricao($obrid, $descricao, $providencia, $prazo)
//{
//    // Criar inconformidade
//    $obra = new Obras($obrid);
//
//    // CPF 21269017500
//    $dados = array(
//        'rstid' => null,
//        'tprid' => 17,
//        'fsrid' => 1,
//        'empid' => $obra->empid,
//        'obrid' => $obra->obrid,
//        'usucpf' => "'00000000191'",
//        'rstdsc' => $descricao,
//        'rstdscprovidencia' => $providencia,
//        'rstitem' => 'I',
//        'rstdtprevisaoregularizacao' => "NOW() + interval '{$prazo}' day",
//        'rstdtinclusao' => 'NOW()',
//        'rststatus' => 'A',
//    );
//
//
//    $sql = "insert into obras2.restricao ( tprid, fsrid, empid, usucpf, rstdsc, rstdtprevisaoregularizacao, rstdscprovidencia, rstdtinclusao, rststatus, obrid, rstitem )
//              values ( {$dados['tprid']}, {$dados['fsrid']}, {$dados['empid']},  {$dados['usucpf']}, '{$dados['rstdsc']}', {$dados['rstdtprevisaoregularizacao']}, '{$dados['rstdscprovidencia']}', {$dados['rstdtinclusao']}, '{$dados['rststatus']}', {$dados['obrid']}, '{$dados['rstitem']}' )
//					 returning rstid";
//
//    $restricao = new Restricao();
//    $rstid = $restricao->pegaUm($sql);
//    $restricao->commit();
//
//    $restricao->atualizaDocidNullRetricao($rstid, 1);
//    $restricao->commit();
//}