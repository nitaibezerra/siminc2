<?php
if ($_POST['requisicao'] == 'limpar') {
    unset($_SESSION['par']['demanda']);

    echo "<script>
            window.location.href = window.location.href;
          </script>";
    exit();
}

if ($_POST['requisicao'] == 'excluir') {
    global $db;

    if ($_REQUEST['dmdid_atual']) {
        $sql = "UPDATE par.demanda SET dmdstatus = 'I' WHERE dmdid = {$_REQUEST['dmdid_atual']}";
        $db->executar($sql);
        $db->commit();

        $sql = "SELECT
                    dmdprioridade,
                    dmtid
                FROM
                    par.demanda
                WHERE
                    dmdid = {$_REQUEST['dmdid_atual']}";

        $dados = $db->pegaLinha($sql);
        tiraFuroOrdem($dados['dmtid']);
    }

    echo "<script>
            alert('Demanda excluida.');
            window.location.href = window.location.href;
          </script>";
    exit();
}

if ($_POST['requisicao'] == 'excel') {
    listaDemandasXLS();
    exit();
}

?>
<div id="temporizador1" style="background: none repeat 0% 0% rgb(255, 255, 255); opacity: 0.65;
     text-align: center; position: absolute; top: 0px; left: 0px; width: 100%;
     height: 300%; z-index: 1000; display:none;">
    <span id="spanCarregando" style="position: relative; top: 225px;">
        <img src="/imagens/carregando.gif">
        <center>Carregando...</center>
    </span>
</div>
<?php
include APPRAIZ . "includes/cabecalho.inc";

$db->cria_aba($abacod_tela, $url, $parametros);

monta_titulo('Minhas Pendências', '');

$habilitado = 'S';

$aba = ($_REQUEST['aba'] ? $_REQUEST['aba'] : 'A');
?>
<link href="../includes/JQuery/jquery-1.9.1/css/jquery-ui-1.10.3.custom.css" rel="stylesheet">
<script src="../includes/JQuery/jquery-1.9.1/jquery-1.9.1.js"></script>
<script src="../includes/JQuery/jquery-1.9.1/jquery-ui-1.10.3.custom.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.9.1/funcoes.js"></script>

<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script>
    $(document).ready(function () {
        $('#button_limpar').click(function () {
            /*$('input[type!="button"][name!="no_dmddescricao"]').val('');
             $('textarea').val('');
             $('select').val('');*/
            $('[name="requisicao"]').val('limpar');
            $('[name="form_pesquisa"]').submit();
        });

        $('#button_pesquisar').click(function () {
            if ($('[name="datacadastroinicial"]').val() != '' || $('[name="datacadastrofinal"]').val()) {
                if (!validaData(document.getElementById('datacadastroinicial'))) {
                    alert('Data início está no formato incorreto.');
                    document.getElementById('datacadastroinicial').focus();
                    return false;
                } else if (!validaData(document.getElementById('datacadastrofinal'))) {
                    alert('Data fim está no formato incorreto.');
                    document.getElementById('datacadastrofinal').focus();
                    return false;
                } else if (!validaDataMaior(document.getElementById('datacadastroinicial'), document.getElementById('datacadastrofinal'))) {
                    alert("A data inicial não pode ser maior que data final.");
                    document.getElementById('datacadastroinicial').focus();
                    return false;
                }
            }

            $('[name="requisicao"]').val('pesquisar');
            $('[name="form_pesquisa"]').submit();
        });

        $(document).on('click', '.exibir', '', function () {
            var title = 'Demanda detalhada';
            var data = {action: 'visualizar', id: $(this).attr('id')};
            var url = 'par.php?modulo=principal/demandaVisualizar&acao=A';

            dialogAjax(title, data, url, null, false, true, true, true, false, 900, 400);
        });

        $(document).on('click', '.alterar', '', function () {
            window.location.href = 'par.php?modulo=principal/demandas/cadDemandas&acao=A&dmdid=' + $(this).attr('id');
        });

        $(document).on('click', '.excluir', '', function () {
            if (confirm('Deseja realmente excluir esta demanda?')) {
                $('[name="requisicao"]').val('excluir');
                $('#dmdid_atual').val($(this).attr('id'));
                $('#form_pesquisa').submit();
            }
        });

        $('#geraXLS').click(function () {
            $('[name="requisicao"]').val('excel');
            $('[name="form_pesquisa"]').submit();
        });

        jq('.sel_chosen').chosen({allow_single_deselect: true});
    });
</script>
<style>
    .listagem{
        width: 100%;
    }
</style>

<table class="tabela" cellSpacing="1" cellPadding="3" align="center" >
    <tr>
        <td>
            <?php

            if($aba == 'A'){ ?>
            <table style="width: 100%" cellSpacing="1" cellPadding="3" align="center">
                <tr>
                    <th>Demandas Pendentes</th>
                </tr>
                <tr>
                    <td valign="top" id="td_lista"><?php listaDemandas(); ?></td>
                </tr>
            </table>
            <?php }else{ ?>
            <table style="width: 100%"cellSpacing="1" cellPadding="3" align="center">
                <tr>
                    <th>Demandas Concluídas</th>
                </tr>
                <tr>
                    <td valign="top"><?php listaDemandas(true); ?></td>
                </tr>
            </table>
            <?php } ?>
        </td>
    </tr>
</table>

<?php

function carregaAbaDemanda($stPaginaAtual = null) {
    $abas = array();
    array_push($abas, array("id" => 0, "descricao" => "Pendentes", "link" => "/par/par.php?modulo=principal/demandas/demandas&acao=A&aba=A"));
    array_push($abas, array("id" => 1, "descricao" => "Concluidas", "link" => "/par/par.php?modulo=principal/demandas/demandas&acao=A&aba=C"));

    return montarAbasArray($abas, $stPaginaAtual);
}

function listaDemandas($concluida = false) {
    global $db;

    $arrPerfil = Array(PAR_PERFIL_DEMANDANTE_PAR);

    $sql = "SELECT DISTINCT dmtid FROM par.demandatipo";
    $dmtids = $db->carregarColuna($sql);
    foreach ($dmtids as $dmtid) {
        tiraFuroOrdem($dmtid);
    }

    if (possuiPerfil($arrPerfil) && !$db->testa_superuser()) {
        $acoes = '(
                CASE
                    WHEN (doc.esdid = ' . PAR_WF_DEMANDA_ESTADO_EM_CADASTRAMENTO . ' OR doc.esdid = ' . PAR_WF_DEMANDA_ESTADO_EM_DILIGENCIA . ' or doc.esdid = ' . PAR_WF_DEMANDA_ESTADO_AGUARDANDO_VALIDACAO . ')
                    THEN
                        \'<center>
                            <a style="margin: 0 -5px 0 5px;" href="#" id="\' || dmdid || \'" class="exibir">
                                <img src=\"/imagens/icone_lupa.png\" border=0 title=\"Exibir">
                            </a>
                            <a style="margin: 0 -5px 0 5px;" href="#" id="\' || dmdid || \'" class="alterar">
                                <img src=\"/imagens/alterar.gif\" border=0 title=\"Alterar">
                            </a>
                        </center>\'
                    ELSE
                        \'<center>
                            <a style="margin: 0 -5px 0 5px;" href="#" id="\' || dmdid || \'" class="exibir">
                                <img src=\"/imagens/icone_lupa.png\" border=0 title=\"Exibir">
                            </a>
                        </center>\'
                END
                )';
    } elseif ($db->testa_superuser()) {
        $acoes = '\'<center>
                        <a style="margin: 0 -5px 0 5px;" href="#" id="\' || dmdid || \'" class="exibir">
                            <img src=\"/imagens/icone_lupa.png\" border=0 title=\"Exibir">
                        </a>
                        <a style="margin: 0 -5px 0 5px;" href="#" id="\' || dmdid || \'" class="alterar">
                            <img src=\"/imagens/alterar.gif\" border=0 title=\"Alterar">
                        </a>
                        <a style="margin: 0 -5px 0 5px;" href="#" id="\' || dmdid || \'" class="excluir">
                            <img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir">
                        </a>
                    </center>\'';
    } else {
        $acoes = '\'<center>
                        <a style="margin: 0 -5px 0 5px;" href="#" id="\' || dmdid || \'" class="exibir">
                            <img src=\"/imagens/icone_lupa.png\" border=0 title=\"Exibir">
                        </a>
                        <a style="margin: 0 -5px 0 5px;" href="#" id="\' || dmdid || \'" class="alterar">
                            <img src=\"/imagens/alterar.gif\" border=0 title=\"Alterar">
                        </a>
                    </center>\'';
    }

    $where = Array("dmdstatus = 'A'");

    $cabecalho = array('Ação', 'Código', 'Prioridade', 'N° de Priorizações', 'Origem', 'Solicitante', 'Nome', 'Data de <br>Criação', 'Estado', 'Tipo', 'Urgente');
    $prioridade = "dmdprioridade, dmd.dmdqtdpriorizado,";

    if ($_SESSION['usucpf']) {
        $where[] = "dmd.dmdcpfsolicitante = '{$_SESSION['usucpf']}'";
    }

    $where[] = "esd.esdid IN (" . PAR_WF_DEMANDA_ESTADO_EM_DILIGENCIA . ", " . PAR_WF_DEMANDA_ESTADO_AGUARDANDO_VALIDACAO . ")";

    # Insere coluna Resonsável na lista
    $colunaResponsavel = null;
    if ($db->testa_superuser()) {
        $cabecalho = retornarCabecalhoResponsavel($cabecalho);
        $colunaResponsavel = 'coalesce(usu.usunome, dmd.dmdresponssavel) as dmdresponssavel,';
    }

    $sql = "
        SELECT
            {$acoes} AS acao,
            dmdid,
            $prioridade
            dmonome,
            dmdsolicitante,
            {$colunaResponsavel}
            CASE 
                WHEN dmdurgente IS TRUE 
                    THEN '<label style=color:red; >' || dmdnome || '</label>' 
                ELSE dmdnome 
            END AS dmdnome,
            TO_CHAR(dmd.dmddatacadastro,'DD/MM/YYYY') as data_inclusao,
            esddsc,
            dmt.dmtnome,
            CASE 
                WHEN dmdurgente IS TRUE 
                    THEN '<center><img src=/imagens/check_checklist.png width=15px border=0 title=Urgente></center>' 
                ELSE '' 
            END AS urgente
        FROM
            par.demanda dmd
            LEFT  JOIN seguranca.usuario 		usu ON usu.usucpf = dmd.dmdusucpfresponsavel
            LEFT  JOIN par.demanda_origem 		dmo ON dmo.dmoid = dmd.dmoid AND dmostatus = 'A'
            INNER JOIN workflow.documento 		doc ON dmd.docid = doc.docid AND doc.tpdid = 127
            INNER JOIN workflow.estadodocumento esd ON doc.esdid = esd.esdid
            INNER JOIN par.demandatipo 			dmt ON dmt.dmtid = dmd.dmtid
        WHERE
            " . implode(" AND ", $where) . "
        ORDER BY
            dmdprioridade, 
            esddsc, 
            dmd.dmddatacadastro DESC";
          
    $db->monta_lista($sql, $cabecalho, 50, 10, '', 'center', 'N', '', '', '', '', '');
}

/**
 * Insere a coluna Responsavel no cabeçalho/nome de colunas da lista depois do item Solicitante
 * 
 * @param array $cabecalho
 * @return array $cabecalho
 */
function retornarCabecalhoResponsavel($cabecalho) {
    $resultado = $cabecalho;
    $posicao = array_search('Solicitante', $cabecalho);

    if (!empty($posicao)) {
        $cabecaoParte1 = array_slice($cabecalho, 0, $posicao + 1, true);
        $cabecaoParte1[] = 'Responsável';
        $cabecalhoParte2 = array_slice($cabecalho, $posicao + 1, count($cabecalho) - 1);
        $resultado = array_merge($cabecaoParte1, $cabecalhoParte2);
    }

    return $resultado;
}

function listaDemandasXLS() {

    ob_clean();
    header("Expires: Mon, 1 Apr 1974 05:00:00 GMT");
    header("Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT");
    header("Pragma: no-cache");
    header("Content-type: application/xls; name=relatoriogerencial_par_" . date("Ymdhis") . ".xls");
    header("Content-Disposition: attachment; filename=relatoriogerencial_par_" . date("Ymdhis") . ".xls");
    header("Content-Description: MID Gera excel");
    $cabecalho = 'Lista de Demandas';

    global $db;

    $sql = "SELECT count( * ) FROM seguranca.perfilusuario WHERE usucpf = '{$_SESSION['usucpf']}' AND pflcod = " . PAR_PERFIL_ADMINISTRADOR;
    $perfilAdministrador = $db->pegaUm($sql);

    $isSuperUser = ($_SESSION['superuser'] == 1) ? true : false;

    $where = Array("dmdstatus = 'A'");

    if ($_SESSION['usucpf']) {
        $where[] = "dmd.dmdcpfsolicitante = '{$_SESSION['usucpf']}'";
    }

    $where[] = "esd.esdid IN (" . PAR_WF_DEMANDA_ESTADO_EM_DILIGENCIA . ", " . PAR_WF_DEMANDA_ESTADO_AGUARDANDO_VALIDACAO . ")";


    $cabecalho = array('Código', 'Origem', 'Solicitante', 'Nome', 'Estado', 'Tipo', 'Urgente', 'N° de Priorizações');
    $sql = "
        SELECT
            dmdid,
            dmonome,
            dmdsolicitante,
            dmdnome,
            esddsc,
            dmt.dmtnome,
            CASE WHEN dmdurgente IS TRUE 
                THEN 'Urgente' 
                ELSE '' 
            END AS urgente,
            dmd.dmdqtdpriorizado
        FROM
            par.demanda dmd
        LEFT JOIN par.demanda_origem dmo ON dmo.dmoid = dmd.dmoid AND dmostatus = 'A'
        INNER JOIN workflow.documento doc ON dmd.docid = doc.docid
        INNER JOIN workflow.estadodocumento esd ON doc.esdid = esd.esdid
        INNER JOIN par.demandatipo dmt ON dmt.dmtid = dmd.dmtid
        WHERE
                " . implode(" AND ", $where) . "
        ORDER BY
            dmdprioridade, 
            esddsc;";

    $db->monta_lista_tabulado($sql, $cabecalho, 100000, 5, 'N', '100%', $par2, 'N', true);
    exit;
}
?>