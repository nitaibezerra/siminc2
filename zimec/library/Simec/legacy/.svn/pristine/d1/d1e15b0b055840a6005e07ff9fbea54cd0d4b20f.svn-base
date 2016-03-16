<?php
set_time_limit(0);


//include "config.inc";
//include APPRAIZ."includes/classes_simec.inc";
//include APPRAIZ."includes/funcoes.inc";

include_once APPRAIZ . "includes/library/simec/Grafico.php";

$db = new cls_banco();

if($_REQUEST['alterar_grafico']){
    exibirGrafico();
    die;
}

function exibirGrafico(){
//    ver($_REQUEST);

    $dt_inicio = $_REQUEST['dt_inicio'] ? formata_data_sql($_REQUEST['dt_inicio']) : date('Y') . '-01-01';
    $dt_fim = $_REQUEST['dt_fim'] ? formata_data_sql($_REQUEST['dt_fim']) : date('Y-m-d');

    switch($_REQUEST['tipo_exibicao']){
        case('D'):
            $formatoBusca = 'YYYYMMDD';
            $formatoExibicao = 'DD/MM/YYYY';
            break;
        case('A'):
            $formatoBusca = 'YYYY';
            $formatoExibicao = 'YYYY';
            break;
        default:
            $formatoBusca = 'YYYYMM';
            $formatoExibicao = 'MM/YYYY';
            break;
    }


    $where[] = $_REQUEST['tpdid'] ? " e.tpdid = {$_REQUEST['tpdid']}" : " e.tpdid = 105";

    if(is_array($_REQUEST['esdid']) && count($_REQUEST['esdid'])){
        $where[] = 'e.esdid in (' . implode(', ', $_REQUEST['esdid']) . ')';
    }

    $sql = "
        select count(*) as valor, to_char(h.htddata, '{$formatoBusca}') as data, to_char(h.htddata, '{$formatoExibicao}') as categoria, e.esdid, e.esddsc as descricao
        from workflow.estadodocumento e
                inner join workflow.documento d on d.esdid = e.esdid
                inner join workflow.historicodocumento h on h.docid = d.docid
        where h.htddata BETWEEN '{$dt_inicio} 00:00:00' and '{$dt_fim} 23:59:59'
        and " . implode(' AND ', $where ) . "
        group by data, categoria, e.esdid, descricao
        order by data, descricao
        ";
//ver($sql);
    $grafico = new Grafico(Grafico::K_TIPO_LINHA);
    $grafico->setHeight('500px')->setFormatoTooltip(Grafico::K_TOOLTIP_DECIMAL_0)->gerarGrafico($sql);
}


?>
<form name="formulario-historico-workflow" id="formulario-historico-workflow">
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td>Fluxo:</td>
            <td>
                <div>
                    <?php

                        $sql = "select tpdid as codigo, tpddsc as descricao
                                from workflow.tipodocumento
                                where sisid = '{$_SESSION['sisid']}'
                                order by descricao
                                ";
                        echo $db->monta_combo("tpdid", $sql, 'S', "Selecione...", 'pesquisarSistema', '', '', '500', 'N', 'tpdid', '', $_REQUEST['tpdid'], '', 'class="sel_chosen"');
                    ?>
                </div>
            </td>
        </tr>
        <tr>
            <td>Situação:</td>
            <td>
                <div>
                    <?php

                        $sql = "select esdid as codigo, esddsc as descricao
                                from workflow.estadodocumento
                                where tpdid = 105
                                order by descricao
                                ";
                        echo $db->monta_combo("esdid[]", $sql, 'S', "Selecione...", 'pesquisarSistema', '', '', '500', 'N', 'esdid', '', $_REQUEST['tpdid'], '', 'class="sel_chosen" multiple="multiple"');
                    ?>
                </div>
            </td>
        </tr>
        <tr>
            <td>Período:</td>
            <td>
                <div>
                    <?php echo campo_texto('dt_inicio', 'N', $save, '', 10, 10, '##/##/####', '', '', '', '', 'id="dt_inicio"', ''); ?>
                    a
                    <?php echo campo_texto('dt_fim', 'N', $save, '', 10, 10, '##/##/####', '', '', '', '', 'id="dt_inicio"', ''); ?>

                </div>
            </td>
        </tr>
        <tr>
            <td>Exibição:</td>
            <td>
                <div>
                    <input type="radio" name="tipo_exibicao" value="D" />Diário
                    <input type="radio" name="tipo_exibicao" value="M" style="margin-left: 10px;" />Mensal
                    <input type="radio" name="tipo_exibicao" value="A" style="margin-left: 10px;" />Anual
                </div>
            </td>
        </tr>
        <tr>
            <td colspan="2">
                <div>
                    <input type="button" value="Atualizar" id="botao-atualizar" />
                </div>
            </td>
        </tr>
    </table>
</form>


<script type="text/javascript">
    jq('.sel_chosen').chosen({allow_single_deselect:true});

    jq('#botao-atualizar').click(function(){
        jq('#grafico-historico-workflow').load('/obras2/ajax.php?exibirGraficoHistoricoWorkflow=1&' + jq('#formulario-historico-workflow').serialize());
        return false;

    });
</script>

<div id="grafico-historico-workflow">
    <?php exibirGrafico(); ?>
</div>
