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
include_once APPRAIZ . "includes/classes/modelo/obras2/Empreendimento.class.inc";
include_once APPRAIZ . "includes/classes/modelo/obras2/DestinatarioEmail.class.inc";
include_once APPRAIZ . "includes/classes/modelo/obras2/AnexoEmail.class.inc";
include_once APPRAIZ . "includes/classes/modelo/obras2/Email.class.inc";
include_once APPRAIZ . "includes/classes/modelo/obras2/Restricao.class.inc";
include_once APPRAIZ . "includes/classes/modelo/entidade/Endereco.class.inc";
include_once APPRAIZ . "includes/classes/entidades.class.inc";
include_once APPRAIZ . "includes/classes/dateTime.inc";
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";


$sql = "
            SELECT
                o.*,
                ep.prfid,
                s.supdata,
                b.dcodatafim < NOW() conv_vencido,
                v.\"Fim Vigência Termo\" as fim_termo,
                DATE_PART('days', NOW() - eml.data) dias,
                (
                    SELECT
                        CASE
                        WHEN SUM(icovlritem) > 0 THEN
                            ROUND( (SUM( spivlrfinanceiroinfsupervisor ) /  SUM(icovlritem)) * 100, 2)
                        ELSE
                            0
                        END AS total
                    FROM
                        obras2.cronograma cro
                        JOIN obras2.itenscomposicaoobra i ON cro.croid = i.croid
                    LEFT JOIN
                        obras2.supervisaoitem sic ON sic.icoid = i.icoid
                        AND sic.supid = s.supid
                        AND sic.icoid IS NOT NULL
                        AND sic.ditid IS NULL
                        JOIN obras2.supervisao su ON su.supid = sic.supid
                    WHERE
                        i.icostatus = 'A' AND
                        i.relativoedificacao = 'D' AND
                        i.obrid = o.obrid AND
                        i.croid = s.croid AND
                        cro.crostatus IN ('A', 'H') AND su.croid = cro.croid
                ) as percentual,
                CASE WHEN r.obrid IS NOT NULL THEN 'S' ELSE 'N' END possui_restricao

                FROM obras2.obras o
                JOIN obras2.empreendimento ep ON ep.empid = o.empid
                JOIN workflow.documento d ON d.docid = o.docid
                JOIN workflow.estadodocumento e ON e.esdid = d.esdid
                JOIN (SELECT MAX(s.supid) as supid, s.obrid
                    FROM
                        obras2.supervisao s
                    WHERE s.emsid IS NULL AND s.smiid IS NULL AND s.supstatus = 'A' AND s.validadaPeloSupervisorUnidade = 'S'
                    GROUP BY s.obrid) as ult_sup ON ult_sup.obrid = o.obrid
                JOIN obras2.supervisao s ON s.supid = ult_sup.supid
                LEFT JOIN (SELECT
                            r.obrid
                        FROM obras2.restricao  r
                        JOIN workflow.documento d ON d.docid = r.docid
                        JOIN workflow.estadodocumento e ON e.esdid = d.esdid
                        WHERE r.rststatus = 'A' AND d.esdid != 1142 AND r.obrid IS NOT NULL
                        GROUP BY r.obrid) r ON r.obrid = o.obrid
                LEFT JOIN (SELECT MAX(emldata) as data, obrid FROM obras2.email e WHERE temid IN (25, 26) AND e.emlstatus NOT IN ('I') GROUP BY obrid) as eml ON eml.obrid = o.obrid
                LEFT JOIN painel.dadosconvenios b on b.dcoprocesso = Replace(Replace(Replace(o.obrnumprocessoconv,'.',''),'/',''),'-','')
                LEFT JOIN obras2.vm_termo_obras v ON v.\"ID Obra\" = o.obrid
                WHERE
                e.esdid = 690 AND
                o.obridpai IS NULL AND
                o.obrstatus = 'A' AND
                o.obrpercentultvistoria > 93 AND
                DATE_PART('days', NOW() - s.supdata) > 59 AND
                o.obrid = 28255

";

// Obras com mais de 59 dias acima de 93% de execução
$obras = $db->carregar($sql);
$arrObras1 = array();
$arrObras2 = array();
$arrObras3 = array();
$arrObras4 = array();

foreach ($obras as $obra) {
    if($obra['possui_restricao'] == 'N'){
        // Disparar o email 1 se não existirem restrições ou incoformidades ou todas estiverem superadas. 30 dias após esse disparo verificar novamente e disparar o email 1, desde que a condição de não existirem as restrições/inconformidades ou estarem superadas estiver obedecida e não conclusão da obra.
        $arrObras1[] = $obra['obrid'];
    } else {
        //Se ocorrer de existir restrição ou inconformidade não superada (ag providencia, analise fnde, ag correção), disparar o email 2. e 30 dias após fazer o mesmo se mantiver a conclusão de não superação e não conclusão da obra.
        $arrObras2[] = $obra['obrid'];
    }

    if($obra['prfid'] == '41' && ( $obras['conv_vencido'] == 't' || verificaDataTermoVencido($obra['fim_termo']))){
        $arrObras3[] = $obra['obrid'];
    }

    if ($obra['dias'] > 0) {
        if($obra['dias'] % 7 == 0)
            $arrObras4[] = $obra['obrid'];
    }
}
//ver($arrObras1,$arrObras2,$arrObras3,$arrObras4,d);
foreach ($arrObras1 as $key => $obrid) {

    $conteudo = geraConteudo1($obrid);
    if($conteudo['prazo']){
        if(enviaEmail($obrid, $conteudo['conteudo'], 25))
            criaRestricao($obrid, $conteudo['descricao'], $conteudo['providencia'], $conteudo['prazo']);
    }
}

foreach ($arrObras2 as $key => $obrid) {

    $conteudo = geraConteudo2($obrid);
    if($conteudo['prazo']) {
        if(enviaEmail($obrid, $conteudo['conteudo'], 26))
            criaRestricao($obrid, $conteudo['descricao'], $conteudo['providencia'], $conteudo['prazo']);
    }
}

foreach ($arrObras3 as $key => $obrid) {

    $conteudo = geraConteudo3($obrid);
    if($conteudo['prazo']) {
        if(enviaEmail($obrid, $conteudo['conteudo'], 27))
            criaRestricao($obrid, $conteudo['descricao'], $conteudo['providencia'], $conteudo['prazo']);
    }
}






foreach ($arrObras4 as $key => $obrid) {

    $conteudo = geraConteudo4($obrid);
    enviaEmail($obrid, $conteudo['conteudo'], 28, false);
}

function criaRestricao($obrid, $descricao, $providencia, $prazo)
{
    // Criar inconformidade
    $obra = new Obras($obrid);

    // CPF 21269017500
    $dados = array(
        'rstid' => null,
        'tprid' => 16,
        'fsrid' => 1,
        'empid' => $obra->empid,
        'obrid' => $obra->obrid,
        'usucpf' => '21269017500',
        'rstdsc' => $descricao,
        'rstdscprovidencia' => $providencia,
        'rstitem' => 'I',
        'rstdtprevisaoregularizacao' => "NOW() + interval '{$prazo['n']}' day",
        'rstdtinclusao' => 'NOW()',
        'rststatus' => 'A',
    );


    $sql = "insert into obras2.restricao ( tprid, fsrid, empid, usucpf, rstdsc, rstdtprevisaoregularizacao, rstdscprovidencia, rstdtinclusao, rststatus, obrid, rstitem )
              values ( {$dados['tprid']}, {$dados['fsrid']}, {$dados['empid']},  {$dados['usucpf']}, '{$dados['rstdsc']}', {$dados['rstdtprevisaoregularizacao']}, '{$dados['rstdscprovidencia']}', {$dados['rstdtinclusao']}, '{$dados['rststatus']}', {$dados['obrid']}, '{$dados['rstitem']}' )
					 returning rstid";
    $restricao = new Restricao();
    $rstid = $restricao->pegaUm($sql);
    $restricao->commit();

    $restricao->atualizaDocidNullRetricao($rstid, 1);
    $restricao->commit();
}

function geraConteudo1($obrid)
{
    $obra = new Obras($obrid);
    $empreendimento = new Empreendimento($obra->empid);
    $vistoria = pegaDadosVistoria($obrid);
    $endereco = new Endereco(($obra->endid ? $obra->endid : $empreendimento->endid));

    $prazoTotal = array(
        1 => array('n' => 5, 'e' => 'cinco'),
        2 => array('n' => 20, 'e' => 'vinte'),
        3 => array('n' => 30, 'e' => 'trinta'),
    );

    if ($vistoria['percentual'] >= 97) $prazo = $prazoTotal[1];
    if ($vistoria['percentual'] >= 94 && $vistoria['percentual'] <= 96.9) $prazo = $prazoTotal[2];
    if ($vistoria['percentual'] >= 90 && $vistoria['percentual'] <= 93.9) $prazo = $prazoTotal[3];

    $conteudo = '<p>
                    1. Em decorrência do acompanhamento realizado por meio do Sistema Integrado de
                    Monitoramento, Execução e Controle do Ministério da Educação (SIMEC), no Módulo OBRAS 2.0, a obra
                    em epigrafe encontra-se com o percentual de ' . $vistoria["percentual"] . '% há ' . $vistoria["dias"] . ' dias. Assim, de modo a viabilizar os demais
                    repasses do MEC/ FNDE, como o E.I Manutenção, que pode chegar ao montante de R$ ' . pegaAlunos($endereco->estuf) . ' aluno/mês
                    (Resolução/CD/ FNDE/MEC nº 15 e 16/2013), é condição que a obra já esteja concluída.
                </p>

                <p>
                    2. Solicitamos que a inserção de vistoria de conclusão seja realizada no prazo máximo de ' . $prazo['n'] . '
                    (' . $prazo['e'] . ') dias, conforme manual de orientação de preenchimento do SIMEC OBRAS 2.0, disponibilizado
                    no sitio do FNDE, (link: http://www.fnde.gov.br/programas/proinfancia/proinfancia-manuais/item/4967-monitoramento),
                    e quando concluída, seja informadas a esta Autarquia, por meio da superação da respectiva
                    inconformidade, na aba Restrição e Inconformidade. O não atendimento da providência poderá acarretar
                    sanções administrativas, conforme previsto no instrumento pactuado.
                </p>';

    $descricao = 'A obra em epigrafe encontra-se com o percentual de ' . $vistoria["percentual"] . '% há ' . $vistoria["dias"] . ' dias.';
    $providencia = 'Solicitamos que a inserção de vistoria de conclusão seja realizada no prazo máximo de ' . $prazo['n'] . ' (' . $prazo['e'] . ') dias, conforme manual de orientação de preenchimento do SIMEC OBRAS 2.0, disponibilizado no sitio do FNDE, (link: http://www.fnde.gov.br/programas/proinfancia/proinfancia-manuais/item/4967-monitoramento)';

    return array(
        'conteudo' => $conteudo,
        'descricao' => $descricao,
        'providencia' => $providencia,
        'prazo' => $prazo,
    );
}

function geraConteudo2($obrid)
{
    $obra = new Obras($obrid);
    $empreendimento = new Empreendimento($obra->empid);
    $vistoria = pegaDadosVistoria($obrid);
    $endereco = new Endereco(($obra->endid ? $obra->endid : $empreendimento->endid));

    $prazoTotal = array(
        1 => array('n' => 5, 'e' => 'cinco'),
        2 => array('n' => 20, 'e' => 'vinte'),
        3 => array('n' => 30, 'e' => 'trinta'),
    );
    if ($vistoria['percentual'] >= 97) $prazo = $prazoTotal[1];
    if ($vistoria['percentual'] >= 94 && $vistoria['percentual'] <= 96.9) $prazo = $prazoTotal[2];
    if ($vistoria['percentual'] >= 90 && $vistoria['percentual'] <= 93.9) $prazo = $prazoTotal[3];

    $conteudo = '<p>
                    1. Em decorrência do acompanhamento realizado por meio do Sistema Integrado de
                    Monitoramento, Execução e Controle do Ministério da Educação (SIMEC), no Módulo OBRAS 2.0, verificamos que a obra
                    supracitada encontra-se sem a vistoria de conclusão, embora encontre-se com o percentual de ' . $vistoria["percentual"] . '% há ' . $vistoria["dias"] . ' dias.
                </p>

                <p>
                    2. Ressaltamos que essa situação pode estar inviabilizando repasses do FNDE ao município, como o E I
                    Manutenção, que podem chegar ao montante de R$ ' . pegaAlunos($endereco->estuf) . ' aluno/mês (Resoluções FNDE nr. 15, 16 e 17/2013).
                </p>

                <p>
                    3. Quanto às restrições e inconformidades, que comprometem a boa execução e a plena realização do
                    objeto pactuado, identificadas em visita de supervisão realizada in loco por empresa contratada pelo FNDE,
                    e registradas no Simec- Módulo Obras 2.0 - na aba "Restrições e Inconformidades" -, devem ser sanadas
                    seguindo as orientações constantes na mesma aba. As resoluções dessas pendências podem ocorrer mesmo
                    após a obra ter sido dada como concluída no sistema.
                </p>

                <p>
                    4. Quando houver a superação das restrições e/ou inconformidades, o fato deve ser informado ao
                    FNDE através da tramitação pela barra de trabalho, localizada no canto direito da tela (orientação encontra-se na página de acesso inicial do SIMEC-Módulo Obras 2.0, do fiscal ou do gestor da obra), para que não
                    impactem quando da prestação de contas do instrumento pactuado.
                </p>

                <p>
                    5. Solicitamos que a inserção de vistoria de conclusão seja realizado no prazo máximo de ' . $prazo['n'] . ' (' . $prazo['e'] . ') dias, e quando concluída, seja informadas a esta Autarquia, por meio de e-mail, endereçado para
                    fabio.cardoso@fnde.gov.br e monitoramento.obras@fnde.gov.br . O não atendimento da providência poderá
                    acarretar sanções administrativas, conforme previsto no instrumento pactuado.
                </p>

                ';

    $descricao = 'Verificamos que a obra supracitada encontra-se sem a vistoria de conclusão, embora encontre-se com o percentual de ' . $vistoria["percentual"] . '% há ' . $vistoria["dias"] . ' dias';
    $providencia = 'Solicitamos que a inserção de vistoria de conclusão seja realizada no prazo máximo de ' . $prazo['n'] . ' (' . $prazo['e'] . ') dias, conforme manual de orientação de preenchimento do SIMEC OBRAS 2.0, disponibilizado no sitio do FNDE, (link: http://www.fnde.gov.br/programas/proinfancia/proinfancia-manuais/item/4967-monitoramento)';

    return array(
        'conteudo' => $conteudo,
        'descricao' => $descricao,
        'providencia' => $providencia,
        'prazo' => $prazo,
    );
}

function geraConteudo3($obrid)
{

    $vistoria = pegaDadosVistoria($obrid);
    $prazoTotal = array(
        1 => array('n' => 5, 'e' => 'cinco'),
        2 => array('n' => 20, 'e' => 'vinte'),
        3 => array('n' => 30, 'e' => 'trinta'),
    );
    if ($vistoria['percentual'] >= 97) $prazo = $prazoTotal[1];
    if ($vistoria['percentual'] >= 94 && $vistoria['percentual'] <= 96.9) $prazo = $prazoTotal[2];
    if ($vistoria['percentual'] >= 90 && $vistoria['percentual'] <= 93.9) $prazo = $prazoTotal[3];

    $conteudo = '<p>
                    1. Em decorrência do acompanhamento realizado através do Sistema Integrado de Monitoramento,
                    Execução e Controle do Ministério da Educação (SIMEC), módulo Obras2.0, verificamos que a obra
                    supracitada encontra-se sem a vistoria de conclusão, embora o instrumento pactuado já esteja vencido, e em
                    fase de preparação para a prestação de contas.
                </p>

                <p>
                    2. Com o intuito de regularizar a situação, orientamos que seja inserida a vistoria de conclusão de obra,
                    conforme manual de orientações de preenchimento do SIMEC OBRAS 2.0, disponibilizado através do link:
                    http://www.fnde.gov.br/programas/proinfancia/proinfancia-manuais/item/4967-monitoramento.
                </p>
                <p>
                    3. Solicitamos que a inserção de vistoria de conclusão seja realizado no prazo máximo de ' . $prazo['n'] . ' (' . $prazo['e'] . ') dias,
                    e quando concluída, seja informada a esta Autarquia, por meio da superação da respectiva inconformidade, na
                    aba Restrição e Inconformidade.
                </p>
                <p>
                    4. O não atendimento da providência poderá acarretar sanções administrativas, conforme previsto no
                    instrumento pactuado, o que pode incluir a glosa total do objeto pactuado (por falta de seu cumprimento) e a
                    instauração de Tomada de Contas Especial (TCE).
                </p>
                ';

    $descricao = 'A obra supracitada encontra-se sem a vistoria de conclusão, embora o instrumento pactuado já esteja vencido.';
    $providencia = 'Inserir a vistoria de conclusão de obra, conforme manual de orientações de preenchimento do SIMEC OBRAS 2.0, disponibilizado através do link: http://www.fnde.gov.br/programas/proinfancia/proinfancia-manuais/item/4967-monitoramento';

    return array(
        'conteudo' => $conteudo,
        'descricao' => $descricao,
        'providencia' => $providencia,
        'prazo' => $prazo,
    );
}


function geraConteudo4($obrid)
{
    $obra = new Obras($obrid);

    $conteudo = '<p>
                    1. Em razão de estar próxima a conclusão da obra '.$obra->obrnome.', apresentamos as orientações que se seguem.
                </p>

                <p>
                    2. Quando da conclusão da obra e seu recebimento provisório, cabe ao responsável por seu acompanhamento e fiscalização, a verificação do cumprimento do objeto contratado em conformidade com o projeto e de acordo com as normas da Associação Brasileira de Normas Técnicas - ABNT, que refletem os requisitos mínimos de qualidade, utilidade, resistência e segurança, conforme determina a Lei nº 4.150, de 1962, o inciso X do art. 6º e o art. 66 da Lei nº 8.666, de 1993.
                </p>

                <p>
                    3. Caso seja verificada a existência de vícios, defeitos ou incorreções em razão da execução ou da qualidade dos materiais utilizados, cabe à empresa contratada adotar as providências necessárias à superação dessas irregularidades, caso contrário, a Administração poderá rejeitar a obra no todo ou em parte, se executada em desacordo com o contrato, conforme previsão legal constante nos artigos 69, 70 e 76 da Lei nº 8.666, de 1993.
                </p>

                <p>
                    4. Esclarecemos que, mesmo após o recebimento provisório ou definitivo da obra, a empresa contratada continua sendo responsável civilmente pela solidez e segurança do empreendimento pelo prazo de cinco anos, devendo apresentar a correção dos vícios que surgirem nesse período, nos termos do §2º do art. 73 da Lei nº 8.666/93 c/c art. 618 da Lei nº 10.406, de 2012. Há que se observar, ainda, a orientação do Tribunal de Contas da União constante no sumário do Relatório de Auditoria (TC 018.842/2013-5 - Acórdão nº 1816/2014- Plenário) sobre a necessidade de acompanhamento periódico de obra concluída, nos seguintes termos:
                    <p style="padding-left:200px">
                        É recomendável a realização de acompanhamento periódico da obra concluída, mormente nos cinco anos posteriores ao seu término, com a finalidade de identificar falhas que devam ser corrigidas pelo executor sem ônus para a Administração Pública, bem como de garantir o seu adequado funcionamento durante a vida útil de projeto, sendo a boa prática a elaboração de um manual de utilização, inspeção e manutenção para empreendimento em questão.
                    </p>
                </p>

                <p>
                    5. Quando da conclusão da obra, a unidade gestora deverá garantir condições de funcionamento e habitabilidade da edificação com as ligações definitivas de energia elétrica e água potável. Caso haja atraso das concessionárias de serviços públicos em fornecer esses serviços, orientamos o gestor público a acionar as Agências Reguladoras responsáveis.
                </p>

                <p>
                    6. Isto posto, colocamo-nos à disposição em caso de dúvidas através do email atendimento.monitora@fnde.gov.br.
                </p>
                ';


    return array(
        'conteudo' => $conteudo
    );
}


function enviaEmail($obrid, $conteudo, $temid, $verifica_email = true)
{
    global $db;

    $obra = new Obras($obrid);
    $email = new Email();

    if($verifica_email) {
        if ($email->verificaEmailEnviado($temid, $obrid, 30)) {
            return false;
        }
    }

    $esfera = $db->pegaUm("select empesfera from obras2.empreendimento where empid = " . $obra->empid);

    // Por enquanto e-mail enviado somente para prefeitura
    if ($esfera != 'M')
        return;

    $entPrefeito = $email->pegaEntidadePar($obrid, 2);
    $entPrefeitura = $email->pegaEntidadePar($obrid, 7);

    $enderecoPrefeitura = current($entPrefeitura->enderecos);

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
                                            <td style="text-align: center; font-size:12px">
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
                                            <td style="line-height: 20px;">
                                                A Sua Excelência a Senhor(a)
                                                <br />
                                                ' . $entPrefeito->getEntnome() . '
                                                <br />
                                                Prefeito(a) do Município de ' . $enderecoPrefeitura['mundescricao'] . ' - ' . $enderecoPrefeitura['estuf'] . '
                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="padding:10px 0 20px 0;">
                                              Assunto: <b>Inconformidades na obra (' . $obrid . ') ' . $obra->obrnome . '</b>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="line-height: 20px; text-align:justify">
                                                <p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Senhor Prefeito(a),</p>

                                                ' . $conteudo . '

                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="padding: 1px 0 0 0;">
                                                    Atenciosamente,
                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="text-align: center; padding: 1px 0 0 0;">
                                                    <img align="center" style="height:80px;margin-top:5px;margin-bottom:5px;" src="data:image/png;base64,' . base64_encode(file_get_contents(APPRAIZ . 'www/imagens/obras/assinatura-fabio.png')) . '" />
                                                    <br />
                                                    <b>Fábio Lúcio de Almeida Cardoso<b>
                                                    <br />
                                                    Coordenador Geral de Implementação e Monitoramento de Projetos Educacionais
                                                    <br />
                                                    CGIMP/DIRPE/FNDE/MEC
                                            </td>
                                        </tr>
                                    </tbody>
                                    <tfoot>

                                    </tfoot>
                                </table>
                            </body>
                        </html>
                                    ',
        'emlassunto' => 'Inconformidades na obra (' . $obrid . ') ' . $obra->obrnome,
        'temid' => $temid,
        'emlregistroatividade' => true,
        'obrid' => $obrid
    );

    $dadosRemetentes = array($entPrefeito->getEntEmail(), $entPrefeitura->getEntEmail());
//    $dadosRemetentes = array($_SESSION['email_sistema']);

    $email->popularDadosObjeto($dados);
    $email->salvar($dadosRemetentes);
    $email->enviar();

    return true;
}

function pegaAlunos($uf)
{
    $alunos = array(
        'AC' => '3.622,85',
        'AL' => '2.971,24',
        'AM' => '2.971,24',
        'AP' => '4.362,13',
        'BA' => '2.971,24',
        'CE' => '2.971,24',
        'DF' => '3.230,31',
        'ES' => '3.548,72',
        'GO' => '3.533,72',
        'MA' => '2.971,24',
        'MG' => '3.131,44',
        'MS' => '3.483,45',
        'MT' => '3.030,59',
        'PA' => '2.971,24',
        'PB' => '2.971,24',
        'PE' => '2.971,24',
        'PI' => '2.971,24',
        'PR' => '3.088,41',
        'RJ' => '3.395,17',
        'RN' => '2.971,24',
        'RO' => '3.265,40',
        'RR' => '5.105,31',
        'RS' => '3.863,42',
        'SC' => '3.527,49',
        'SE' => '3.571,18',
        'SP' => '3.944,06',
        'TO' => '3.839,87'
    );

    return $alunos[$uf];
}

function pegaDadosVistoria($obrid)
{
    global $db;
    $sql = "SELECT s.supdata, date_part('day',now() - s.supdata) as dias, ( SELECT
                                  CASE
                                      WHEN sum(i.icovlritem) > 0::numeric THEN round(sum(sic.spivlrfinanceiroinfsupervisor) / sum(i.icovlritem) * 100::numeric, 2)
                                      ELSE 0::numeric
                                  END AS total
                                   FROM obras2.itenscomposicaoobra i
                                   JOIN obras2.cronograma cro ON cro.croid = i.croid AND cro.crostatus = 'A'
                                    LEFT JOIN obras2.supervisaoitem sic ON sic.icoid = i.icoid AND sic.supid = s.supid AND sic.icoid IS NOT NULL AND sic.ditid IS NULL
                                   WHERE i.icostatus = 'A'::bpchar AND i.relativoedificacao = 'D'::bpchar AND cro.obrid = $obrid AND i.obrid = cro.obrid) AS percentual
            FROM obras2.supervisao s
            WHERE s.obrid = $obrid AND s.emsid IS NULL AND s.smiid IS NULL AND s.supstatus = 'A'::bpchar AND s.validadapelosupervisorunidade = 'S'::bpchar
            ORDER BY s.supdata DESC LIMIT 1";

    return $db->pegaLinha($sql);
}


function verificaDataTermoVencido($data){
    require_once APPRAIZ . "includes/classes/dateTime.inc";

    $data = trim($data);

    $m = '';
    $d = '';
    $a = '';

    if(strlen($data) == 7){
        $data = explode('/', $data);
        $d = '01';
        $m = $data[0];
        $a = $data[1];
    } else if(strlen($data) == 10){
        $data = explode('/', $data);
        $d = $data[0];
        $m = $data[1];
        $a = $data[2];
    } else {
        return false;
    }
    $data = new Data();
    return $data->diferencaEntreDatas($data->dataAtual(), "$d/$m/$a 00:00:00", 'maiorDataBolean', 'string', '');
}