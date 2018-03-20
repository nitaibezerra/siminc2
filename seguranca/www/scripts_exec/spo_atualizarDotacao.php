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
$sql = "select  psu.psuid, ptr.ptrid, ptr.unicod, psu.suoid, coalesce(psu.ptrdotacaocapital, 0) ptrdotacaocapital, coalesce(psu.ptrdotacaocusteio, 0) ptrdotacaocusteio, funcional funcionalptres,
                ptr.prgcod || '.' || ptr.acacod || '.' || ptr.plocod || '.' || ptr.unicod funcional,
                suo.suonome, suo.unosigla, suo.suosigla
        from spo.ptressubunidade psu
                inner join monitora.vw_ptres ptr on ptr.ptrid = psu.ptrid
                inner join public.vw_subunidadeorcamentaria suo on suo.suoid = psu.suoid
        where ptr.ptrano = '$exercicio'
        and suo.unofundo = 'f'
        order by ptr.unicod, funcional";

$dados = $db->carregar($sql);
$dados = $dados ? $dados : [];

$dadosSiminc = [];
foreach($dados as $dado){
    $dadosSiminc[$dado['funcional']][] = $dado;
}

$sqls = [];
$htmlTabela1 = "
<h3>Foram realizadas alterações na Dotação das seguintes unidades:</h3>
<table border='1' width='100%' style='font-size: 12px;'>
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
<table border='1' width='100%' style='font-size: 12px;'>
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
                        <td>{$dado[0]['funcionalptres']}</td>
                        <td>" . implode('<hr />', $unidades[3]) . "</td>
                        <td>3 - CUSTEIO</td>
                        <td style='text-align: right; color: red;'>" . number_format($total[3], 0, ',', '.') . "</td>
                        <td style='text-align: right; color: green;'>" . number_format($dadosSiop[$funcional][3], 0, ',', '.') . "</td>
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

$sql = "select  pli.plicod, pli.pliid, pli.suosigla, plititulo, pli.unosigla, pli.funcional,        
                pli.previsto::numeric, pli.autorizado::numeric, pli.empenhado::numeric, pli.liquidado::numeric, pli.pago::numeric pago
        from monitora.vw_planointerno pli
        where pli.pliano = '$exercicio'
        and pli.autorizado > pli.previsto
        order by pli.unosigla, pli.suosigla, pli.funcional";

$dados = $db->carregar($sql);
$dadosProvisionado = $dados ? $dados : [];

$htmlTabela3 = "
<h3>Lista de PIs com valor Provisionado maior do que o valor Previsto:</h3>
<table border='1' width='100%' style='font-size: 12px;'>
    <thead>
    <tr>
        <th>Funcional</th>
        <th>Unidade</th>
        <th>PI</th>
        <th>Título</th>
        <th>Previsto</th>
        <th>Provisionado</th>
        <th>Empenhado</th>
        <th>Liquidado</th>
        <th>Pago</th>
    </tr>   
    </thead>
    <tbody>";

foreach($dadosProvisionado as $dado){
    $htmlTabela3 .= "
        <tr>
            <td>{$dado['funcional']}</td>
            <td>{$dado['unosigla']} - {$dado['suosigla']}</td>
            <td>{$dado['plicod']}</td>
            <td>{$dado['plititulo']}</td>
            <td style='text-align: right; color: green;'>" . number_format($dado['previsto'], 0, ',', '.') . "</td>
            <td style='text-align: right; color: red;'>" . number_format($dado['autorizado'], 0, ',', '.') . "</td>
            <td style='text-align: right;'>" . number_format($dado['empenhado'], 0, ',', '.') . "</td>
            <td style='text-align: right;'>" . number_format($dado['liquidado'], 0, ',', '.') . "</td>
            <td style='text-align: right;'>" . number_format($dado['pago'], 0, ',', '.') . "</td>
        </tr>
    ";
    $boAlteracaoProvisionado = true;
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

//    $destinatario = ["douglas.fontes@cultura.gov.br"];

    $remetente = '';
    $assunto = '[SIMINC 2] Alterações de Dotação';
    $conteudo = $textoEmailV3;


    simec_email($remetente, $destinatario, $assunto, $conteudo);
}
