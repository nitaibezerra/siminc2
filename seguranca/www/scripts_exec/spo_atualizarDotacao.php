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
require_once APPRAIZ . "www/planacomorc/_constantes.php";

# Verificando IP de origem da requisição é autorizado para executar os SCRIPTS.
controlarExecucaoScript();

/**
 * Classe de conexão com o SIOP, serviço WSQuantitativo.
 * @see Spo_Ws_Sof_Quantitativo
 */
require_once(APPRAIZ . 'spo/ws/sof/Quantitativo.php');

// -- Abrindo conexão com o banco de dados
$db = new cls_banco();

$exercicio = date('Y');
$sql = "select  exe.programa, exe.acao, exe.planoorcamentario, exe.unidadeorcamentaria, exe.anoexercicio, exe.anoreferencia, exe.categoriaeconomica,
                exe.programa || '.' || exe.acao || '.' || exe.planoorcamentario || '.' || exe.unidadeorcamentaria funcional,
                sum(exe.dotatual::numeric) dotatual, sum(exe.dotinicialsiafi::numeric) dotinicialsiafi,
                sum(exe.dotacaoantecipada::numeric) dotacaoantecipada, sum(exe.dotacaoinicial::numeric) dotacaoinicial
        from wssof.ws_execucaoorcamentariadto exe
        where exe.anoexercicio = '$exercicio'
        and exe.anoreferencia = '$exercicio'
        group by exe.acao, exe.planoorcamentario, exe.unidadeorcamentaria,  exe.programa, exe.anoexercicio, exe.anoreferencia, exe.categoriaeconomica
        order by  funcional";

$dados = $db->carregar($sql);
$dados = $dados ? $dados : [];

$dadosSiop = [];
foreach($dados as $dado){
    $dadosSiop[$dado['funcional']][$dado['categoriaeconomica']] = $dado['dotatual'];
}

//$sql = "select  psu.psuid, ptr.ptrid, ptr.unicod, psu.suoid, psu.ptrdotacaocapital, psu.ptrdotacaocusteio, funcional funcionalptres,
$sql = "
    SELECT
        psu.psuid,
        ptr.ptrid,
        ptr.unicod,
        psu.suoid,
        coalesce(psu.ptrdotacaocapital, 0) ptrdotacaocapital,
        coalesce(psu.ptrdotacaocusteio, 0) ptrdotacaocusteio,
        funcional funcionalptres,
        ptr.prgcod || '.' || ptr.acacod || '.' || ptr.plocod || '.' || ptr.unicod funcional,
        suo.suonome,
        suo.unosigla,
        suo.suosigla
    FROM spo.ptressubunidade psu
        JOIN monitora.vw_ptres ptr ON ptr.ptrid = psu.ptrid
        JOIN public.vw_subunidadeorcamentaria suo ON suo.suoid = psu.suoid
    WHERE
        ptr.ptrano = '$exercicio'
        AND suo.unofundo = 'f'
        AND ptr.irpcod != '6'
    ORDER BY
        ptr.unicod,
        funcional
";
//ver($sql, d);
$dados = $db->carregar($sql);
$dados = $dados ? $dados : [];

$dadosSiminc = [];
foreach($dados as $dado){
    $dadosSiminc[$dado['funcional']][] = $dado;
}

$sqls = [];
$htmlTabela1 = "
<h3>Foram realizadas alterações na Dotação das seguintes unidades:</h3>
<table border='1' width='100%' style='font-size: 10px;'>
    <thead>
    <tr>
        <th>Funcional</th>
        <th>Unidade</th>
        <th>Tipo</th>
        <th>Vl. Antigo</th>
        <th>Vl. Atual</th>
    </tr>   
    </thead>
    <tbody>       
";


$htmlTabela2 = "
<h3>As seguintes funcionais compartilhadas estão com valores divergentes:</h3>
<style>
       /* classe mid para alterar a formatação das colunas para o meio/centro */
    .mid{
        vertical-align: middle;
}
</style>
<table border='1' width='100%' style='font-size: 10px;'>
    <thead>
    <tr>
        <th>Funcional</th>
        <th>Unidade</th>
        <th>Tipo</th>
        <th>Soma</th>
        <th>Dotação Atual</th>
    </tr>   
    </thead>
    <tbody>";

$boAlteracao = $boAlteracaoCompartilhada = $boAlteracaoProvisionado = false;

foreach($dadosSiminc as $funcional => $dado){

    $dadosSiop[$funcional][3] = !empty($dadosSiop[$funcional][3]) ? $dadosSiop[$funcional][3] : 0;
    $dadosSiop[$funcional][4] = !empty($dadosSiop[$funcional][4]) ? $dadosSiop[$funcional][4] : 0;

    if(isset($dadosSiop[$funcional]) && count($dado) == 1){

        // Alteração do valor de Custeio
        if(isset($dadosSiop[$funcional][3]) && $dado[0]['ptrdotacaocusteio'] != $dadosSiop[$funcional][3]){
            $htmlTabela1 .= "
                <tr>
                    <td>{$dado[0]['funcionalptres']}</td>
                    <td>{$dado[0]['unosigla']} - {$dado[0]['suosigla']}</td>
                    <td>3 - CUSTEIO</td>
                    <td style='text-align: right; color: red;'>" . number_format($dado[0]['ptrdotacaocusteio'], 0, ',', '.') . "</td>
                    <td style='text-align: right; color: green;'>" . number_format($dadosSiop[$funcional][3], 0, ',', '.') . "</td>
                </tr>
            ";
            $sqls[] = "update spo.ptressubunidade psu set ptrdotacaocusteio = {$dadosSiop[$funcional][3]} where psuid = {$dado[0]['psuid']}";
            $boAlteracao = true;
        }

        // Alteração do valor de Capital
        if(isset($dadosSiop[$funcional][4]) && $dado[0]['ptrdotacaocapital'] != $dadosSiop[$funcional][4]){

            $htmlTabela1 .= "
                <tr>
                    <td>{$dado[0]['funcionalptres']}</td>
                    <td>{$dado[0]['unosigla']} - {$dado[0]['suosigla']}</td>
                    <td>4 - CAPITAL</td>
                    <td style='text-align: right; color: red;'>" . number_format($dado[0]['ptrdotacaocapital'], 0, ',', '.') . "</td>
                    <td style='text-align: right; color: green;'>" . number_format($dadosSiop[$funcional][4], 0, ',', '.') . "</td>
                </tr>
            ";

            $sqls[] = "update spo.ptressubunidade psu set ptrdotacaocapital = {$dadosSiop[$funcional][4]} where psuid = {$dado[0]['psuid']}";
            $boAlteracao = true;
        }
    } elseif(count($dado) > 1){
        $total[3] = $total[4] = 0;
        foreach($dado as $subunidade){
            $total[3] += $subunidade['ptrdotacaocusteio'];
            $total[4] += $subunidade['ptrdotacaocapital'];

            $unidades[3][] .= "{$subunidade['unosigla']} - {$subunidade['suosigla']} (R$ " . number_format($subunidade['ptrdotacaocusteio'], 0, ',', '.') . ")";
            $unidades[4][] .= "{$subunidade['unosigla']} - {$subunidade['suosigla']} (R$ " . number_format($subunidade['ptrdotacaocapital'], 0, ',', '.') . ")";
        }

        if(isset($dadosSiop[$funcional][3]) && $total[3] != $dadosSiop[$funcional][3]){
            $htmlTabela2 .= "
                    <tr>
                        <td class='mid'>{$dado[0]['funcionalptres']}</td>
                        <td>" . implode('<hr />', $unidades[3]) . "</td>
                        <td class='mid'>3 - CUSTEIO</td>
                        <td class='mid' style='text-align: right; color: red;'>" . number_format($total[3], 0, ',', '.') . "</td>
                        <td class='mid' style='text-align: right; color: green;'>" . number_format($dadosSiop[$funcional][3], 0, ',', '.') . "</td>
                    </tr>
                ";
            $boAlteracaoCompartilhada = true;
        }

        if(isset($dadosSiop[$funcional][4]) && $total[4] != $dadosSiop[$funcional][4]){
            $htmlTabela2 .= "
                    <tr>
                        <td>{$dado[0]['funcionalptres']}</td>
                        <td>" . implode('<hr />', $unidades[4]) . "</td>
                        <td>4 - CAPITAL</td>
                        <td style='text-align: right; color: red;'>" . number_format($total[4], 0, ',', '.') . "</td>
                        <td style='text-align: right; color: green;'>" . number_format($dadosSiop[$funcional][4], 0, ',', '.') . "</td>
                    </tr>
                ";
            $boAlteracaoCompartilhada = true;
        }
    }
}

$sql = "
    SELECT
        pli.plicod,
        pli.pliid,
        pli.suosigla,
        plititulo,
        pli.unosigla,
        pli.funcional,
        pli.previsto::numeric,
        pli.autorizado::numeric,
        pli.empenhado::numeric,
        pli.liquidado::numeric,
        pli.pago::numeric pago
    FROM monitora.vw_planointerno pli
        JOIN monitora.vw_ptres ptr ON pli.ptrid = ptr.ptrid
    WHERE
        pli.pliano = '$exercicio'
        AND pli.autorizado > pli.previsto
        AND ptr.irpcod <> '6'
    ORDER BY
        pli.unosigla,
        pli.suosigla,
        pli.funcional
";

$dados = $db->carregar($sql);
$dadosProvisionado = $dados ? $dados : [];

$htmlTabela3 = "
<h3>Lista de PIs com valor Provisionado maior do que o valor Previsto:</h3>
<style>
       /* classe mid para alterar a formatação das colunas para o meio/centro */
    .mid{
        vertical-align: middle;
}
</style>
<table border='1' width='100%' style='font-size: 10px;'>
    <thead>
    <tr>
        <th>Funcional</th>
        <th style='width: 75px'>Unidade</th>
        <th>PI</th>
        <th>Título</th>
        <th style='text-align: right; color: green;'>Previsto</th>
        <th style='text-align: right; color: red;'>Provisionado</th>
        <th style='text-align: right;'>Empenhado</th>
        <th style='text-align: right;'>Liquidado</th>
        <th style='text-align: right;'>Pago</th>
    </tr>   
    </thead>
    <tbody>";

foreach($dadosProvisionado as $dado){
    $htmlTabela3 .= "
        <tr>
            <td class='mid'>{$dado['funcional']}</td>
            <td class='mid'>{$dado['unosigla']} - {$dado['suosigla']}</td>
            <td class='mid'>{$dado['plicod']}</td>
            <td class='mid'>{$dado['plititulo']}</td>
            <td class='mid'>". number_format($dado['previsto'], 0, ',', '.') . "</td>
            <td class='mid'>". number_format($dado['autorizado'], 0, ',', '.') . "</td>
            <td class='mid'>". number_format($dado['empenhado'], 0, ',', '.') . "</td>
            <td class='mid'>". number_format($dado['liquidado'], 0, ',', '.') . "</td>
            <td class='mid'>". number_format($dado['pago'], 0, ',', '.') . "</td>
        </tr>
    ";
    $boAlteracaoProvisionado = true;
}

$sql = "
    SELECT
	funcionais.ptrid,
	funcionais.ptres,
	funcionais.funcional,
	funcionais.subunidade,
	COALESCE(sec_geral.empenhado, 0.00) - COALESCE(funcionais.empenhado, 0.00) AS empenhado,
	COALESCE(sec_geral.liquidado, 0.00) - COALESCE(funcionais.liquidado, 0.00) AS liquidado,
	COALESCE(sec_geral.pago, 0.00) - COALESCE(funcionais.pago, 0.00) AS pago
FROM(
    SELECT
        agrupado.ptrid,
        agrupado.ptres,
        agrupado.funcional,
        CASE WHEN (
            SELECT
                COUNT(1)
            FROM spo.ptressubunidade
            WHERE
                ptressubunidade.ptrid = agrupado.ptrid
        ) > 1 THEN
                'Várias'
        ELSE
           (SELECT
                suo.unosigla || ' - ' || suo.suosigla AS subunidade
            FROM spo.ptressubunidade
                JOIN public.vw_subunidadeorcamentaria suo ON ptressubunidade.suoid = suo.suoid
            WHERE
                ptressubunidade.ptrid = agrupado.ptrid
                LIMIT 1
                )
        END
         AS subunidade,
        SUM(agrupado.empenhado) AS empenhado,
        SUM(agrupado.liquidado) AS liquidado,
        SUM(agrupado.pago) AS pago
    FROM (
        SELECT
            ptr.ptrid,
            ptr.ptres,
            ptr.funcional,
            (
                SELECT
                    COUNT(1)
                FROM spo.ptressubunidade
                WHERE
                    ptressubunidade.ptrid = ptr.ptrid
            ) AS compartilhada,
            COALESCE(sec.empenhado, 0.00) AS empenhado,
            COALESCE(sec.liquidado, 0.00) AS liquidado,
            COALESCE(sec.pago, 0.00) as pago
        FROM public.vw_subunidadeorcamentaria suo
            JOIN spo.ptressubunidade psu on psu.suoid = suo.suoid
            JOIN monitora.vw_ptres ptr on ptr.ptrid = psu.ptrid AND ptr.ptrano = suo.prsano
            LEFT JOIN monitora.pi_planointernoptres ppt on ppt.ptrid = ptr.ptrid
            LEFT JOIN monitora.pi_planointerno pli on(pli.pliid = ppt.pliid AND pli.ungcod = suo.suocod AND pli.unicod = suo.unocod AND plistatus = 'A')
            LEFT JOIN planacomorc.pi_complemento pic on pic.pliid = pli.pliid
            LEFT JOIN planacomorc.unidadegestora_limite lmu on lmu.ungcod = suo.suocod AND lmu.lmustatus = 'A' AND lmu.prsano = suo.prsano
            LEFT JOIN(
                SELECT
                    siopexecucao.unicod,
                    pi_planointerno.ungcod,
                    siopexecucao.ptres,
                    SUM(COALESCE(siopexecucao.vlrautorizado, 0.00))::NUMERIC AS provisionado,
                    SUM(COALESCE(siopexecucao.vlrempenhado, 0.00))::NUMERIC AS empenhado,
                    SUM(COALESCE(siopexecucao.vlrliquidado, 0.00))::NUMERIC AS liquidado,
                    SUM(COALESCE(siopexecucao.vlrpago, 0.00))::NUMERIC AS pago
                FROM spo.siopexecucao
                    LEFT JOIN monitora.pi_planointerno ON(
                        siopexecucao.plicod = pi_planointerno.plicod
                        AND siopexecucao.exercicio = pi_planointerno.pliano
                        AND pi_planointerno.plistatus = 'A')
                WHERE
                    pi_planointerno.ungcod IS NOT NULL
                    AND siopexecucao.exercicio = '$exercicio'
                GROUP BY
                    siopexecucao.ptres,
                    siopexecucao.unicod,
                    pi_planointerno.ungcod
            ) sec ON(ptr.ptres = sec.ptres AND suo.unocod = sec.unicod AND suo.suocod = sec.ungcod)
        WHERE
            suo.prsano = '$exercicio'
            AND suo.unofundo = FALSE
            AND suo.suostatus = 'A'
            AND ptr.irpcod <> '6'
        GROUP BY
            ptr.ptrid,
            ptr.ptres,
            ptr.funcional,
            sec.empenhado,
            sec.liquidado,
            sec.pago
    ) agrupado
    GROUP BY
        agrupado.ptrid,
        agrupado.ptres,
        agrupado.funcional
) AS funcionais
    LEFT JOIN(
	SELECT
	    siopexecucao.ptres,
	    SUM(COALESCE(siopexecucao.vlrautorizado, 0.00))::NUMERIC AS provisionado,
	    SUM(COALESCE(siopexecucao.vlrempenhado, 0.00))::NUMERIC AS empenhado,
	    SUM(COALESCE(siopexecucao.vlrliquidado, 0.00))::NUMERIC AS liquidado,
	    SUM(COALESCE(siopexecucao.vlrpago, 0.00))::NUMERIC AS pago
	FROM spo.siopexecucao
	WHERE
	    siopexecucao.exercicio = '$exercicio'
	GROUP BY
	    siopexecucao.ptres
    ) sec_geral ON(funcionais.ptres = sec_geral.ptres)
WHERE
	sec_geral.empenhado - funcionais.empenhado > 0
	OR sec_geral.liquidado - funcionais.liquidado > 0
	OR sec_geral.pago - funcionais.pago > 0
ORDER BY
    funcionais.funcional
";
//ver($sql, d);
$listaResultado = $db->carregar($sql);
$listaDivergenciaPlanejada = $listaResultado? $listaResultado: [];

$htmlTabela4 = "
    <h3>Valor sem PI por Funcional:</h3>
    <style>
        /* classe mid para alterar a formatação das colunas para o meio/centro */
        .mid {
            vertical-align: middle;
        }
    </style>
    <table border='1' width='100%' style='font-size: 10px;'>
        <thead>
        <tr>
            <th>Funcional</th>
            <th style='width: 75px'>Unidade</th>
            <th style='text-align: right;'>Empenhado</th>
            <th style='text-align: right;'>Liquidado</th>
            <th style='text-align: right;'>Pago</th>
        </tr>
        </thead>
        <tbody>";

foreach($listaDivergenciaPlanejada as $dado){
    $htmlTabela4 .= "
        <tr>
            <td class='mid'>{$dado['funcional']}</td>
            <td class='mid'>{$dado['subunidade']}</td>
            <td class='mid'>". number_format($dado['empenhado'], 0, ',', '.') . "</td>
            <td class='mid'>". number_format($dado['liquidado'], 0, ',', '.') . "</td>
            <td class='mid'>". number_format($dado['pago'], 0, ',', '.') . "</td>
        </tr>
    ";
    $boDivergenciaPlanejada = true;
}

$htmlTabela1 .= "
        </tbody>
    </table>
    ";

$htmlTabela2 .= "
        </tbody>
    </table>
    ";

$htmlTabela3 .= "
        </tbody>
    </table>
    ";

$htmlTabela4 .= "
        </tbody>
    </table>
    ";

$corpoEmailV3 = '';
if($boAlteracao){
    $corpoEmailV3 .= $htmlTabela1;
}

if($boAlteracaoCompartilhada){
    $corpoEmailV3 .= $htmlTabela2;
}

if($boAlteracaoProvisionado){
    $corpoEmailV3 .= $htmlTabela3;
}

if($boDivergenciaPlanejada){
    $corpoEmailV3 .= $htmlTabela4;
}

if($corpoEmailV3){


    $corpoEmailV3 = "<h2 style='color: red; text-align: center'>Relatório de Alterações e Divergências</h2>" . $corpoEmailV3;

    if(count($sqls)){
        $sqls = implode('; ', $sqls);

        $db->executar($sqls);
        $db->commit();
    }

    include_once APPRAIZ. "includes/email-template.php";

    // Recuperando email dos super-usuários e administradores
    $sql = "select distinct usu.usucpf, usu.usunome, usu.usuemail
            from seguranca.perfilusuario pu
                    inner join seguranca.usuario usu on usu.usucpf = pu.usucpf and usu.suscod = 'A'
                    inner join seguranca.usuario_sistema us on us.usucpf = usu.usucpf and us.suscod = 'A' and us.sisid = 157
            where pu.pflcod in (" . PFL_ADMINISTRADOR . ", " . PFL_SUPERUSUARIO . ")
            order by usuemail";
    $destinatario = $db->carregar($sql);

//    $destinatario = ["teste@teste.com"];

    $remetente = '';
    $assunto = '[SIMINC 2] Alterações de Dotação';
    $conteudo = $textoEmailV3;
ver($conteudo, d);
    simec_email($remetente, $destinatario, $assunto, $conteudo);
}
