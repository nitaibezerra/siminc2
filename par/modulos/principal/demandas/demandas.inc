<?php
$arrayPerfil = pegaArrayPerfil($_SESSION['usucpf']);

#Requisicao Limpar
if ($_POST['requisicao'] == 'limpar') {
    unset($_SESSION['par']['demanda']);

    echo "<script>window.location.href = window.location.href;</script>";
    exit();
}

#Requisicao Excluir
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

    echo "<script>alert('Demanda excluida.');window.location.href = window.location.href;</script>";
    exit();
}

#Requisicao Excel
if ($_POST['requisicao'] == 'excel') {
    listaDemandasXLS();
    exit();
}

include APPRAIZ . "includes/cabecalho.inc";

$db->cria_aba($abacod_tela, $url, $parametros);
monta_titulo('Lista de Demandas', '');

$habilitado = 'S';
$aba = ($_REQUEST['aba'] ? $_REQUEST['aba'] : 'A');
?>

<div id="divPopUp"></div>
<script type="text/javascript" src="../par/js/jquery-1.11.1.min.js"></script>
<script type="text/javascript" src="../includes/jQuery/jquery-migrate-1.2.1.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript">
        jQuery(function(){
            jQuery('body').on('click','.exibir', function(){
                var title = 'Demanda detalhada #' + jQuery(this).attr('id');
                var url = '/par/par.php?modulo=principal/demandaVisualizar&acao=A';
                jQuery("#divPopUp").load(url, {action: 'visualizar', id: $(this).attr('id')}, function(){
                    jQuery("#divPopUp").dialog({
                        modal: true,
                        width: 1000,
                        title: title,
                        position: ['center', 'top'],
                        buttons: {
                            Fechar: function() {
                                jQuery("#divPopUp").html('');
                                jQuery( this ).dialog( "close" );
                            }
                        }
                    });
                });
            });  
        });

    $(document).ready(function() {
        $('#button_limpar').click(function() {
            $('[name="requisicao"]').val('limpar');
            $('[name="form_pesquisa"]').submit();
        });

        $('#button_pesquisar').click(function() {
        	selectAllOptions(document.getElementById('esdid'));
            if ($('[name="datacadastroinicial"]').val() !== '' || $('[name="datacadastrofinal"]').val()) {
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
      
        $(document).on('click', '.alterar', '', function() {
            window.location.href = 'par.php?modulo=principal/demandas/cadDemandas&acao=A&dmdid=' + $(this).attr('id');
        });

        $(document).on('click', '.excluir', '', function() {
            if (confirm('Deseja realmente excluir esta demanda?')) {
                $('[name="requisicao"]').val('excluir');
                $('#dmdid_atual').val($(this).attr('id'));
                $('#form_pesquisa').submit();
            }
        });

        $('#geraXLS').click(function() {
        	selectAllOptions(document.getElementById('esdid'));
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
<?php
if ($aba != $_SESSION['par']['demanda']['aba']) {
    unset($_SESSION['par']['demanda']);
}
if ($_REQUEST['requisicao'] == 'pesquisar') {
    unset($_SESSION['par']['demanda']);
    $_SESSION['par']['demanda'] = $_REQUEST;
}

$_REQUEST = ($_SESSION['par']['demanda'] ? $_SESSION['par']['demanda'] : $_REQUEST);

extract($_REQUEST);
?>
<form method="post" name="form_pesquisa" id="form_pesquisa" action="" class="formulario" >
    <input type="hidden" name="requisicao" id="requisicao" value=""/>
    <input type="hidden" name="aba" id="aba" value="<?= $aba ?>"/>
    <input type="hidden" name="dmdid_atual" id="dmdid_atual" value="<?= $dmdid ?>"/>

    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr bgcolor="#cccccc">
            <td style="text-align: center;" colspan="4"><strong>Filtros</strong></td>
        </tr>
        <tr>
            <td class="SubTituloDireita" style="width: 15%"><label for="dmdnome">Código:</label></td>
            <td style="width: 35%"><?php echo campo_texto('dmdid', 'N', $habilitado, 'Id da Ação', 10, 50, '[#]', '', '', '', '', 'id="dmdid"', ''); ?></td>
            <td class="SubTituloDireita" valign="top" style="width: 15%"><label for="descricao_detalhada">Origem: </label></td>
            <td style="width: 35%">
                <?php $sql = "SELECT dmoid as codigo , dmonome as descricao FROM par.demanda_origem"; ?>
                <?php echo $db->monta_combo("dmoid", $sql, $habilitado, "Selecione", '', '', '', '400', 'N', 'dmoid', '', '', '', ''); ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"><label for="assunto">Prioridade:</label></td>
            <td><?php echo campo_texto('dmdprioridade', 'N', $habilitado, 'Prioridade', 10, 2, '##', '', '', '', '', 'id="dmdprioridade"', ''); ?></td>
            <td class="SubTituloDireita" valign="top"><label for="descricao_detalhada">Tipo: </label></td>
            <td>
                <?php $sql = "SELECT dmtid as codigo , dmtnome as descricao FROM par.demandatipo"; ?>
                <?php echo $db->monta_combo("dmtid", $sql, $habilitado, "Selecione", '', '', '', '400', 'N', 'dmtid', '', '', '', ''); ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" valign="middle"><label for="assunto">Urgente?:</label></td>
            <td>
                <input type="radio" name="dmdurgente" id="dmdurgentesim" value="TRUE" <?= (($dmdurgente == 'TRUE') ? 'checked="checked"' : ''); ?> /> Sim &nbsp;
                <input type="radio" name="dmdurgente" id="dmdurgentenao" value="FALSE" <?= (($dmdurgente == 'FALSE') ? 'checked="checked"' : ''); ?> /> Não &nbsp;
                <input type="radio" name="dmdurgente" id="dmdurgentetodos" <?= (empty($dmdurgente) ? 'checked="checked"' : ''); ?> value="" /> Todos 
            </td>
            <td class="SubTituloDireita" valign="top"><label for="descricao_detalhada">Estado: </label></td>
            <td>
                <?php
                
                if( $esdid[0] ){

					$sql = "SELECT distinct
                                esd.esdid as codigo,
                                esd.esddsc as descricao
                            FROM
                                workflow.estadodocumento esd
                            WHERE
                                esd.tpdid = 127
								AND esdid IN (".implode(',',$esdid).")
                                and esd.esdstatus = 'A'
                                and esd.esdid not in (" . PAR_WF_DEMANDA_ESTADO_CONCLUIDO . ")
                            ORDER BY
                                esd.esddsc";

                	$esdid = $db->carregar($sql);
                }
                
                $sql_combo = "SELECT distinct
                                esd.esdid as codigo,
                                esd.esddsc as descricao
                            FROM
                                workflow.estadodocumento esd
                            WHERE
                                esd.tpdid = 127
                                and esd.esdstatus = 'A'
                                and esd.esdid not in (" . PAR_WF_DEMANDA_ESTADO_CONCLUIDO . ")
                            ORDER BY
                                esd.esddsc";
                
                combo_popup( 'esdid', $sql_combo, 'Selecione o(s) Estado(s)', '400x400', 0, array(), '', 'S', true, true, 4, 400, null, null, $dados_conexao, null, null, true, false, '', true);
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" valign="middle"><label for="dmddescricao">Descrição detalhada: </label></td>
            <td>
                <?php echo campo_textarea('dmddescricao', 'N', $habilitado, 'Descrição Detalhada', 55, 2, 1000, $funcao = '', $acao = 0, $txtdica = '', $tab = false, 'Descrição detalhada', '', array('id' => 'dmddescricao')); ?>
            </td>
            <td class="SubTituloDireita"><label for="dmdnome">Assunto:</label></td>
            <td><?php echo campo_textarea('dmdnome', 'N', $habilitado, 'Descrição Ação', 55, 2, 100, $funcao = '', $acao = 0, $txtdica = '', $tab = false, 'Descrição detalhada', '', array('id' => 'dmdnome')); ?>
        </tr>
        <tr>
            <td class="SubTituloDireita" valign="middle">Data:</td>
            <td>
                <?php echo campo_data2('datacadastroinicial', 'N', 'S', 'Data Inicial', '', '', ''); ?>&nbsp;&nbsp;&nbsp;a&nbsp;&nbsp;&nbsp;
                <?php echo campo_data2('datacadastrofinal', 'N', 'S', 'Data Final', '', '', ''); ?>
            </td>
            <td class="SubTituloDireita"><label for="assunto">Responsável:</label></td>
            <td><?php echo campo_texto('dmdresponssavel', 'N', $habilitado, 'Responsável', 70, 70, '', '', '', '', '', 'id="dmdresponssavel"', ''); ?></td>
        </tr>
        <tr>
            <td class="SubTituloDireita" valign="middle">Solicitante:</td>
            <td <?php if (!in_array(PAR_PERFIL_SUPER_USUARIO, $arrayPerfil)) { ?> colspan="2" <?php } ?>>
                <?php
                $sql = "
                            SELECT DISTINCT
                            	usu.usunome as codigo,
                                usu.usunome as descricao
                            FROM 
                            	seguranca.perfilusuario pfl
                            	INNER JOIN seguranca.usuario usu ON usu.usucpf = pfl.usucpf
                            	INNER JOIN seguranca.usuario_sistema uss ON uss.usucpf = usu.usucpf
                            WHERE 
                            	pfl.pflcod IN (
                                    805,
                                    689,
                                    670,
                                    458,
                                    671,
                                    669,
                                    706,
                                    685,
                                    457,
                                    477,
                                    468,
                                    691,
                                    462,
                                    540,
                                    459,
                                    882,
                                    1194,
                                    1224,
                                    441
                                )   
                            	AND uss.sisid = 23
                            	AND susstatus = 'A'
                            ORDER BY
                                usu.usunome;
                        ";

                $db->monta_combo("dmdsolicitante", $sql, $habilitado, "Selecione", '', '', '', '520', 'S', 'dmdsolicitante', '', $demanda['dmdsolicitante'], '', 'required class="sel_chosen"');
                ?>
            </td>
            <?php
            if (in_array(PAR_PERFIL_SUPER_USUARIO, $arrayPerfil)) {
                ?>
                <td class="SubTituloDireita" valign="middle"><label for="assunto">Controle?:</label></td>
                <td>
                    <input type="radio" name="dmdcontrole" id="dmdcontrolesim" value="TRUE" <?= (($dmdcontrole == 'TRUE') ? 'checked="checked"' : ''); ?> /> Sim &nbsp;
                    <input type="radio" name="dmdcontrole" id="dmdcontrolenao" value="FALSE" <?= (($dmdcontrole == 'FALSE') ? 'checked="checked"' : ''); ?> /> Não &nbsp;
                    <input type="radio" name="dmdcontrole" id="dmdcontroletodos" <?= (empty($dmdcontrole) ? 'checked="checked"' : ''); ?> value="" /> Todos 
                </td>
                <?php
            }
            ?>
        </tr>
        <tr bgcolor="#dcdcdc">
            <td style="text-align:center" colspan="4">
                <input id="button_pesquisar" type="button" value="Pesquisar"/>
                <input id="geraXLS" type="button" value="Gerar Planilha XLS"/>
                <input id="button_limpar" type="button" value="Limpar Filtros"/>
            </td>
        </tr>
    </table>
</form>
<?
print carregaAbaDemanda("/par/par.php?modulo=principal/demandas/demandas&acao=A&aba=$aba");
?>
<table class="tabela" cellSpacing="1" cellPadding="3" align="center" >
    <tr>
        <td>
            <?php if ($aba == 'A') { ?>
                <table style="width: 100%" cellSpacing="1" cellPadding="3" align="center">
                    <tr>
                        <th>Demandas Pendentes</th>
                    </tr>
                    <tr>
                        <td valign="top" id="td_lista"><?php listaDemandas(); ?></td>
                    </tr>
                </table>
            <?php } else { ?>
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
    array_push($abas, array("id" => 1, "descricao" => "Concluídas", "link" => "/par/par.php?modulo=principal/demandas/demandas&acao=A&aba=C"));

    return montarAbasArray($abas, $stPaginaAtual);
}

function listaDemandas($concluida = false) {

    global $db;

    $arrPerfil = Array(PAR_PERFIL_DEMANDANTE_PAR);
    $arraySup = pegaArrayPerfil($_SESSION['usucpf']);

    $sql = "SELECT DISTINCT dmtid FROM par.demandatipo";
    $dmtids = $db->carregarColuna($sql);
    foreach ($dmtids as $dmtid) {
        tiraFuroOrdem($dmtid);
    }

    if (possuiPerfil($arrPerfil) && !$db->testa_superuser()) {
        $acoes = '
                (
                CASE
                    WHEN doc.esdid = ' . PAR_WF_DEMANDA_ESTADO_EM_CADASTRAMENTO . ' OR doc.esdid = ' . PAR_WF_DEMANDA_ESTADO_EM_DILIGENCIA . '
                    THEN
                        \'<center>
                            <a style="margin: 0 -2px 0 2px;" href="javascript: void()" id="\' || dmdid || \'" class="exibir">
                                <img src=\"/imagens/icone_lupa.png\" border=0 title=\"Exibir">
                            </a>
                            <a style="margin: 0 -2px 0 2px;" href="javascript: void()" id="\' || dmdid || \'" class="alterar">
                                <img src=\"/imagens/alterar.gif\" border=0 title=\"Alterar">
                            </a>
                            <a style="margin: 0 -2px 0 2px;" href="javascript: void()" id="\' || dmdid || \'" class="excluir">
                                <img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir">
                            </a>
                        </center>\'
                    ELSE
                        \'<center>
                            <a style="margin: 0 -2px 0 2px;" href="javascript: void()" id="\' || dmdid || \'" class="exibir">
                                <img src=\"/imagens/icone_lupa.png\" border=0 title=\"Exibir">
                            </a>
                        </center>\'
                END
                )';
    } elseif ($db->testa_superuser()) {
        $acoes = '\'<center>
                        <a style="margin: 0 -2px 0 2px;" href="javascript: void()" id="\' || dmdid || \'" class="exibir">
                            <img src=\"/imagens/icone_lupa.png\" border=0 title=\"Exibir">
                        </a>
                        <a style="margin: 0 -2px 0 2px;" href="javascript: void()" id="\' || dmdid || \'" class="alterar">
                            <img src=\"/imagens/alterar.gif\" border=0 title=\"Alterar">
                        </a>
                        <a style="margin: 0 -2px 0 2px;" href="javascript: void()" id="\' || dmdid || \'" class="excluir">
                            <img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir">
                        </a>
                    </center>\'';
    } else {
        $acoes = '\'<center>
                        <a style="margin: 0 -2px 0 2px;" href="javascript: void()" id="\' || dmdid || \'" class="exibir">
                            <img src=\"/imagens/icone_lupa.png\" border=0 title=\"Exibir">
                        </a>
                    </center>\'';
    }

    #Trata Filtros
    $where = getFilters();

    if ($concluida) {
        $_REQUEST['esdid'] = PAR_WF_DEMANDA_ESTADO_CONCLUIDO;

        $sqlConcluidas = "
                        SELECT
                            dmdid
                        FROM
                            par.demanda dmd
                        INNER JOIN workflow.documento doc ON dmd.docid = doc.docid
                        WHERE
                            doc.esdid IN (" . PAR_WF_DEMANDA_ESTADO_CONCLUIDO . ")";

        $sql = "UPDATE par.demanda SET
                    dmdprioridade = NULL
                WHERE
                    dmdid IN ( $sqlConcluidas )";
        $db->executar($sql);
        $db->commit();

        $cabecalho = array('Ação', 'Código', 'Origem', 'Solicitante', 'Nome', 'Data de <br>Criação', 'Estado', 'Tipo', 'Urgente');

        if (in_array(PAR_PERFIL_SUPER_USUARIO, $arraySup)) {
            $cabecalho = array('Ação', 'Código', 'Origem', 'Solicitante', 'Nome', 'Data de <br>Criação', 'Estado', 'Tipo', 'Urgente', 'Controle');
        }
        $prioridade = '';
    } else {
        $cabecalho = array('Ação', 'Código', 'Prioridade', 'N° de Priorizações', 'Prioridade por Origem', 'Origem', 'Solicitante', 'Nome', 'Data de <br>Criação', 'Estado', 'Tipo', 'Urgente');
        if (in_array(PAR_PERFIL_SUPER_USUARIO, $arraySup)) {
            $cabecalho = array('Ação', 'Código', 'Prioridade', 'N° de Priorizações', 'Prioridade por Origem', 'Origem', 'Solicitante', 'Nome', 'Data de <br>Criação', 'Estado', 'Tipo', 'Urgente', 'Controle');
        }
        $prioridade = "dmdprioridade, dmd.dmdqtdpriorizado, dmdprioridadeorigem,";
        $where[] = "esd.esdid NOT IN (" . PAR_WF_DEMANDA_ESTADO_CONCLUIDO . ")";
    }

    # Insere coluna Resonsável na lista
    $colunaResponsavel = null;
    if ($db->testa_superuser()) {
        $cabecalho = retornarCabecalhoResponsavel($cabecalho);
        $colunaResponsavel = 'coalesce(usu.usunome, dmd.dmdresponssavel) as dmdresponssavel,';
    }

    if (in_array(PAR_PERFIL_SUPER_USUARIO, $arraySup)) {
        $campoControle = ", CASE WHEN dmdcontrole IS TRUE THEN '<center><img src=/imagens/check_checklist.png width=15px border=0 title=Urgente ></center>' ELSE '' END as controle ";
    } else {
        $campoControle = "";
    }

    $sql = "SELECT
                {$acoes} as acao,
                dmdid,
                $prioridade
                dmonome,
                dmdsolicitante,
                {$colunaResponsavel}
                CASE WHEN dmdurgente IS TRUE THEN '<label style=color:red; >'||dmdnome||'</label>' ELSE dmdnome END as dmdnome,
                to_char(dmd.dmddatacadastro,'DD/MM/YYYY') as data_inclusao,
                esddsc,
                dmt.dmtnome,
                CASE WHEN dmdurgente IS TRUE THEN '<center><img src=/imagens/check_checklist.png width=15px border=0 title=Urgente ></center>' ELSE '' END as urgente
                {$campoControle}
            FROM
                par.demanda dmd
                LEFT  JOIN seguranca.usuario		usu ON usu.usucpf = dmd.dmdusucpfresponsavel
                LEFT  JOIN par.demanda_origem		dmo ON dmo.dmoid = dmd.dmoid AND dmostatus = 'A'
                INNER JOIN workflow.documento 		doc ON dmd.docid = doc.docid and doc.tpdid = 127
                INNER JOIN workflow.estadodocumento esd ON doc.esdid = esd.esdid
                INNER JOIN par.demandatipo 			dmt ON dmt.dmtid = dmd.dmtid
            WHERE
                " . implode(" AND ", $where) . "
            ORDER BY
                esddsc, dmdprioridade, dmd.dmddatacadastro desc";
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

# Relatorio Excel

function listaDemandasXLS() {

    #Retorna perfis do usuario logado.
    $arrPerfil = pegaArrayPerfil($_SESSION['usucpf']);

    ob_clean();
    header("Expires: Mon, 1 Apr 1974 05:00:00 GMT");
    header("Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT");
    header("Pragma: no-cache");
    header("Content-type: application/xls; name=relatoriogerencial_par_" . date("Ymdhis") . ".xls");
    header("Content-Disposition: attachment; filename=relatoriogerencial_par_" . date("Ymdhis") . ".xls");
    header("Content-Description: MID Gera excel");

    global $db;

    $where = getFilters();
    $cabecalho = array('Código', 'Prioridade', 'Origem', 'Tipo', 'Estado', 'Assunto', 'Descrição Detalhada', 'Data', 'Solicitante',  'Urgente');

    if (in_array(PAR_PERFIL_SUPER_USUARIO, $arrPerfil)) {
        array_push($cabecalho, 'N° de Priorizações', 'Controle', 'Responsável');
        $dmdcontrole = ",dmd.dmdqtdpriorizado, CASE WHEN dmdcontrole IS TRUE THEN 'Sim' ELSE 'Não' END as dmdcontrole, coalesce(usu.usunome, dmdresponssavel) as dmdresponssavel";
    } 

    $sql = "SELECT
                dmdid,
                dmdprioridade,
                dmonome,
                dmt.dmtnome,
                esddsc,
                dmdnome,
                dmddescricao,
                dmddatacadastro,
            	dmdsolicitante,
                CASE WHEN dmdurgente IS TRUE THEN 'Sim' ELSE 'Não' END as urgente
                {$dmdcontrole}
            FROM
                par.demanda dmd
                LEFT  JOIN seguranca.usuario 		usu ON usu.usucpf = dmd.dmdusucpfresponsavel
                LEFT  JOIN par.demanda_origem 		dmo ON dmo.dmoid = dmd.dmoid AND dmostatus = 'A'
                INNER JOIN workflow.documento 		doc ON dmd.docid = doc.docid
                INNER JOIN workflow.estadodocumento esd ON doc.esdid = esd.esdid
                INNER JOIN par.demandatipo 			dmt ON dmt.dmtid = dmd.dmtid
            WHERE
                " . implode(" AND ", $where) . "
            ORDER BY
                 esddsc, dmdprioridade, dmd.dmddatacadastro desc";
    $db->monta_lista_simples($sql, $cabecalho, 100000, 5, 'N', '100%', 'N', true, false, false, true);
    exit;
}

# Trata os Filtros de Demanda.
function getFilters() {
    $where = Array("dmdstatus = 'A'");
    
    if ($_REQUEST['dmdid'] != '') {
        $where[] = "dmd.dmdid = {$_REQUEST['dmdid']}";
    }

    if ($_REQUEST['dmdprioridade'] != '') {
        $where[] = "dmd.dmdprioridade = {$_REQUEST['dmdprioridade']}";
    }

    if ($_REQUEST['dmdurgente'] != '') {
        $where[] = "dmd.dmdurgente = {$_REQUEST['dmdurgente']}";
    }

    if ($_REQUEST['dmdcontrole'] != '') {
        $where[] = "dmd.dmdcontrole = {$_REQUEST['dmdcontrole']}";
    }

    if ($_REQUEST['dmdsolicitante'] != '') {
        $where[] = "dmd.dmdsolicitante ilike '%{$_REQUEST['dmdsolicitante']}%'";
    }

    if ($_REQUEST['dmdnome'] != '') {
        $dmdnomeTp = removeAcentos(str_replace("-", " ", (utf8_decode($_REQUEST['dmdnome']))));
        $where[] = "UPPER(public.removeacento(dmd.dmdnome)) ilike '%{$dmdnomeTp}%'";
    }

    if ($_REQUEST['dmoid'] != '') {
        $where[] = "dmd.dmoid = {$_REQUEST['dmoid']}";
    }
    
    // Trata Array de Estado
    if (is_array($_REQUEST['esdid'])) {
        foreach($_REQUEST['esdid'] as $chave => $valor){
            if ($valor == '') {
                unset($_REQUEST['esdid'][$chave]);
            }
        }
    }
    $_REQUEST['esdid'] = ($_REQUEST['aba'] == 'C' ? PAR_WF_DEMANDA_ESTADO_CONCLUIDO : $_REQUEST['esdid']);
    if (count($_REQUEST['esdid']) > 0) {
        if (is_array($_REQUEST['esdid'])) {
            $where[] = "esd.esdid IN (" . implode(", ", $_REQUEST['esdid']) . ")";
        } else {
            $where[] = "esd.esdid = {$_REQUEST['esdid']}";
        }
    }

    if ($_REQUEST['dmtid'] != '') {
        $where[] = "dmt.dmtid = {$_REQUEST['dmtid']}";
    }

    if ($_REQUEST['dmddescricao'] != '') {
        $where[] = "dmd.dmddescricao ilike '%{$_REQUEST['dmddescricao']}%'";
    }

    if ($_REQUEST['dmdresponssavel'] != '') {
        $where[] = "(dmd.dmdresponssavel ILIKE '%{$_REQUEST['dmdresponssavel']}%' OR usu.usunome ILIKE '%{$_REQUEST['dmdresponssavel']}%' )";
    }

    if (!empty($_REQUEST['datacadastroinicial']) && !empty($_REQUEST['datacadastrofinal'])) {
        $where[] = "dmd.dmddatacadastro between '" . formata_data_sql($_REQUEST['datacadastroinicial']) . "' and '" . formata_data_sql($_REQUEST['datacadastrofinal']) . "'";
    }

    return $where;
}
?>