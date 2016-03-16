<?php
if ( $_REQUEST['pesquisa'] == 'xls' ) {
    include APPRAIZ . "obras2/modulos/relatorio/popupRelatorioUsuariosObras.inc";
}

extract( $_POST );

switch ($_REQUEST['ajax']){
    case 'municipio':
        header('content-type: text/html; charset=ISO-8859-1');
        $municipio = new Municipio();
        echo $db->monta_combo("muncod", $municipio->listaCombo( array('estuf' => $_POST['estuf']) ), 'S', 'Selecione...', '', '', '', 200, 'N', 'muncod');
        exit;
    case 'ajax_carrega_campo_orgao':
        carrega_campo_orgao();
        die;
    case 'ajax_carrega_campo_unidade':
        carrega_campo_unidade();
        die;
    case 'ajax_carrega_campo_unidade_gestora':
        carrega_campo_unidade_gestora();
        die;
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";
monta_titulo('Relatório de Usuários', '&nbsp;');

?>


<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js">
</script>
<script type="text/javascript">
    
function ajax_carrega_campo_unidade_gestora ( unicod ){
    
    var td = $('#td_unidade');
    if (entid != '') {
        var dados_ajax = {};
        dados_ajax = {ajax: 'ajax_carrega_campo_unidade_gestora', unicod: unicod};
        var url = location.href;
        $.ajax({
            url: url,
            type: 'post',
            data: dados_ajax,
            dataType: "html",
            async: false,
            beforeSend: function() {
                divCarregando();
                td.find('select option:first').attr('selected', true);
            },
            error: function() {
                divCarregado();
            },
            success: function(data) {
                td.html(data);
                divCarregado();
            }
        });
    } else {
        td.find('select option:first').attr('selected', true);
        td.find('select').attr('selected', true).attr('disabled', true);
    }
}

function ajax_carrega_campo_unidade(entid) {

    var td = $('#td_unidade');
    if (entid != '') {
        var dados_ajax = {};
        dados_ajax = {ajax: 'ajax_carrega_campo_unidade', entid: entid};
        var url = location.href;
        $.ajax({
            url: url,
            type: 'post',
            data: dados_ajax,
            dataType: "html",
            async: false,
            beforeSend: function() {
                divCarregando();
                td.find('select option:first').attr('selected', true);
            },
            error: function() {
                divCarregado();
            },
            success: function(data) {
                td.html(data);
                divCarregado();
            }
        });
    } else {
        td.find('select option:first').attr('selected', true);
        td.find('select').attr('selected', true)
                .attr('disabled', true);
    }
}

function ajax_carrega_campo_orgao(tpocod) {

    var td = $('#td_orgao');
    if (tpocod != '') {

        var dados_ajax = {};

        if (($('muncod').value == '') && ($('estuf').value == '')) {
            dados_ajax = {ajax: 'ajax_carrega_campo_orgao', tpocod: tpocod};

        } else {
            dados_ajax = {ajax: 'ajax_carrega_campo_orgao', tpocod: tpocod, muncod: $('#muncod').value, estuf: $('#estuf').value};
        }

        var url = location.href;
        $.ajax({
            url: url,
            type: 'post',
            data: dados_ajax,
            dataType: "html",
            async: false,
            beforeSend: function() {
                divCarregando();
                td.find('select option:first').attr('selected', true);
            },
            error: function() {
                divCarregado();
            },
            success: function(data) {
                td.html(data);
                divCarregado();
            }
        });
    } else {
        td.find('select option:first').attr('selected', true);
        td.find('select').attr('selected', true)
                .attr('disabled', true);
    }

    if ($('#unicod')) {
        $('#unicod').value = '';
        ajax_carrega_campo_unidade_gestora($('#unicod').value);
    }

    if ($('#entid')) {
        $('#entid').value = '';
        ajax_carrega_campo_unidade($('#entid').value);
    }

}

function carregarMunicipio(estuf) {
    var td = $('#td_municipio');
    if (estuf != '') {
        var url = location.href;
        $.ajax({
            url: url,
            type: 'post',
            data: {ajax: 'municipio',
                estuf: estuf},
            dataType: "html",
            async: false,
            beforeSend: function() {
                divCarregando();
                td.find('select option:first').attr('selected', true);
            },
            error: function() {
                divCarregado();
            },
            success: function(data) {
                td.html(data);
                divCarregado();
            }
        });
    } else {
        td.find('select option:first').attr('selected', true);
        td.find('select').attr('selected', true)
                .attr('disabled', true);
    }
}

$(document).ready(function() {

    $('.pesquisar').click(function() {
        $('#formListaObra').submit();
    });

    $('.exportarxls').click(function() {
        $('#xls').val('1');
        $('#formListaObra').submit();
    });

});

function gerarRelatorio(tipo) {
    formulario = document.getElementById('formListaObra');

    if (tipo != 'xls') {
        formulario.target = 'RelatorioOsObras';
        var janela = window.open('obras2.php?modulo=relatorio/popupRelatorioUsuariosObras&acao=A', 'RelatorioOsObras', 'status=no,menubar=no,toolbar=no,scrollbars=1,resizable=no,fullscreen=yes');
        janela.focus();

        // Tipo de relatorio
    } else {
        formulario.target = null;
    }
    document.getElementById('pesquisa').value = tipo;
    formulario.submit();
}
$(function() {
    $('.pos_sim').change(function() {
        if ($(this).val() == 'nao') {
            $('.f_supervisao').hide();
        } else {
            $('.f_supervisao').show();
        }
    });
});
</script>
<form method="post" name="formListaObra" id="formListaObra" action="obras2.php?modulo=relatorio/popupRelatorioUsuariosObras&acao=A">
    <input type="hidden" name="pesquisa" id="pesquisa" value="" />
    <table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
        <tr>
            <td class="SubTituloDireita" width="15%">ID da Obra:</td>
            <td>
                <?=campo_texto('obrid','N','S','',70,100,'','', '', '', '', 'id="obrid"', '', $_REQUEST['obrid']);?>
            </td>
        </tr>

        <tr>
            <td class="SubTituloDireita">UF:</td>
            <td>
                <?php
                $uf = new Estado();
                $db->monta_combo("estuf", $uf->listaCombo(), 'S', 'Selecione...', 'carregarMunicipio', '', '', 200, 'N', 'estuf');
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Município:</td>
            <td id="td_municipio">
                <?php
                    if ($estuf) {
                        $municipio = new Municipio();
                        $dado = $municipio->listaCombo(array('estuf' => $estuf));
                        $habMun = 'S';
                    } else {
                        $dado = array();
                        $habMun = 'N';
                    }
                    $habMun = ($disable == 'N' ? $disable : $habMun);
                    echo $db->monta_combo("muncod", $dado, $habMun, 'Selecione...', '', '', '', 200, 'N', 'muncod');
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Situação:</td>
            <td id="td_situacao">
                <?php
                $situacoes = array(
                    array('codigo' => 'A', 'descricao' => 'Ativo'),
                    array('codigo' => 'B', 'descricao' => 'Bloqueado'),
                    array('codigo' => 'P', 'descricao' => 'Pendente')
                );
                echo $db->monta_combo("suscod", $situacoes, 'S', 'Selecione...', '', '', '');
                ?>
            </td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita">Perfil:</td>
            <td>
                <?php
                    $sql = "SELECT pflcod AS codigo, pfldsc AS descricao
                            FROM seguranca.perfil
                            WHERE pflstatus = 'A' and sisid = ".$_SESSION['sisid']."
                            ORDER BY pfldsc";
                    $db->monta_combo("pflcod", $sql, 'S', 'Todos', '', '', '', '', '', 'pflcod' );
                ?>
            </td>
        </tr>
        
        <tr>
            <td align='right' class="subtitulodireita">Tipo do Órgão:</td>
            <td>
                <?php
                    $sql = "SELECT tpocod as codigo, tpodsc as descricao
                            FROM public.tipoorgao
                            WHERE tpostatus='A'";
                    $db->monta_combo("tpocod",$sql,'S','Todos','ajax_carrega_campo_orgao','','','170','S','tpocod');
                ?>
            </td>
	</tr>
        <tr>
            <td align='right' class="subtitulodireita">Órgão:</td>
            <td id="td_orgao">
                    <span id="spanOrgao">
                            <?php
                                if ($tpocod) {
                                    if ( ($tpocod == 2 || $tpocod == 3)){ $entid = 999999; }
                                    carrega_campo_orgao();
                                } else {
                                    $dado = array();
                                    $habOrg = 'N';
                                    echo $db->monta_combo("entid",$dado,$habOrg,"&nbsp;",'ajax_carrega_campo_unidade','','','350','S', 'entid');
                                }
                            ?>
                    </span>
            </td>
	</tr>
        <tr bgcolor="#F2F2F2">
            <td align='right' class="subtitulodireita">Unidade Orçamentária:</td>
            <td id="td_unidade">
                    <span id="unidade">
                            <?php
                                 if ($entid) {
                                    carrega_campo_unidade();
                                } else {
                                    $dado = array();
                                    $habOrg = 'N';
                                    echo $db->monta_combo("unicod",$dado,$habOrg,"&nbsp;",'ajax_carrega_campo_unidade_gestora','','','350','S', 'unicod');
                                }   
                            ?>
                    </span>
            </td>
	</tr>
        <tr bgcolor="#F2F2F2">
            <td align='right' class="subtitulodireita">Unidade Gestora:</td>
            <td>
                    <span id="unidade_gestora">
                            <?php
                                if ($unicod) {
                                    carrega_campo_unidade_gestora();
                                } else {
                                    $dado = array();
                                    $habOrg = 'N';
                                    echo $db->monta_combo("ungcod",$dado,$habOrg,"&nbsp;",'','','','350','S', 'ungcod');
                                }   
                            ?>
                    </span>
            </td>
	</tr>
        
        <tr>
            <td style="background-color:#DCDCDC" width="15%" colspan="2" align="center">
                <input type="button" name="Gerar Relatório" value="Gerar Relatório" onclick="javascript:gerarRelatorio('visual');" /> 
                <input type="button" name="Gerar Relatório" value="Visualizar XLS" onclick="javascript:gerarRelatorio('xls');" />
            </td>
        </tr>
    </table>
</form>

<?php

function carrega_campo_orgao(){


    global $muncod, $regcod, $tpocod, $db;

    // Caso o tipo seja municipal, verifica o municipio escolhido
    $inner = ( $tpocod == 3 || $tpocod == 2 ) ? 'INNER JOIN entidade.endereco eed ON eed.entid = ee.entid ' : '';

    $uniao = ( $tpocod == 3 || $tpocod == 2 ) ? " UNION ALL ( SELECT 999999 AS codigo, 'OUTROS' AS descricao )" : '';

    $clausula = '';
    
    if ($tpocod == 2) {
        $estuf = ($_REQUEST['estuf']) ? $_REQUEST['estuf'] : $estuf;
        $clausula = "AND eed.estuf = '" . $estuf . "'";
    } elseif ($tpocod == 3) {
        $clausula = "AND eed.muncod = '" . $muncod . "'";
    }

    $sql = "(SELECT
                    ee.entid AS codigo,
                    CASE WHEN ee.entorgcod is not null THEN ee.entorgcod ||' - '|| ee.entnome
                    ELSE ee.entnome END AS descricao
             FROM
                    entidade.entidade ee
             INNER JOIN entidade.funcaoentidade ef ON ef.entid = ee.entid
             INNER JOIN public.tipoorgaofuncao tpf ON ef.funid = tpf.funid
                    " . $inner . "
             WHERE
                ee.entstatus = 'A' and
                    tpf.tpocod = '{$tpocod}'
                    " . $clausula . " AND
                    ( ee.entorgcod is null or ee.entorgcod <> '73000' )

             ORDER BY
                    ee.entnome)" . $uniao;

    $db->monta_combo("entid", $sql, 'S', "&nbsp;", 'ajax_carrega_campo_unidade', '', '', '350', 'S', 'entid');
}

function carrega_campo_unidade(){
	global $entid, $db;
        $uo_total = 0;
        if ( !empty($entid) ){
                $uo_total = $db->pegaUm( "SELECT
                                                count(u.unicod)
                                          FROM
                                                unidade u
                                          INNER JOIN
                                                entidade.entidade e ON
                                                u.orgcod = e.entorgcod
                                          WHERE
                                                unistatus='A' AND
                                                unitpocod='U' AND
                                                e.entid ='{$entid}'" );

        }

        if ( $uo_total > 0 ){

                $sql = "SELECT DISTINCT
                                        u.unicod AS codigo,
                                        u.unicod||' - '||unidsc AS descricao
                                FROM
                                        unidade u
                                INNER JOIN
                                        entidade.entidade e ON
                                        orgcod = entorgcod
                                WHERE
                                        unistatus='A' AND
                                        u.unitpocod='U' AND
                                        entid = '{$entid}'
                                ORDER BY
                                        u.unicod";
                echo $db->monta_combo("unicod",$sql,'S',"&nbsp;",'ajax_carrega_campo_unidade_gestora', '', '','','S','unicod');

        } else {
                echo '<font style="color:#909090;">Este órgão não possui uma unidade.</font>';
        }
                        
}

function carrega_campo_unidade_gestora(){
	global $unicod, $db;
        $sql = "SELECT
                        ungcod AS codigo,
                        ungcod||' - '||ungdsc as descricao
                FROM
                        unidadegestora
                WHERE
                        ungstatus = 'A' AND
                        unitpocod = 'U' AND
                        unicod = '".$unicod."'
                ORDER BY
                        ungdsc";
        if($db->carregar($sql)){
                $db->monta_combo("ungcod",$sql,'S',"&nbsp;",'','','', '', 'S', 'ungcod');
        }else{
                echo '<font style="color: #909090;">Esta unidade não possui uma Unidade Gestora.</font>';
        }
}


?>