<link rel="stylesheet" type="text/css" href="/library/jquery/jquery-ui-1.10.3/themes/custom-theme/jquery-ui-1.10.3.custom.min.css"/>

<?php if($_POST['carregarSituacaoPainel'] && $_POST['tpdid']):

    $sql = "select esdid as codigo, esddsc as descricao
        from workflow.estadodocumento
        where tpdid = {$_POST['tpdid']}
        order by descricao
        ";

    echo $db->monta_combo("esdid[]", $sql, 'S', "", '', '', '', '500', 'N', 'esdid', '', $_REQUEST['tpdid'], '', 'class="sel_chosen chosen" multiple="multiple"');
    exit;
    ?>

<?php elseif($_POST['gerarGraficoPainel']): ?>

    <?php include_once APPRAIZ . "includes/library/simec/Grafico.php"; ?>

<!--    <div class="notprint" style="padding: 15px; ">-->
        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
            <tr>
                <td>
                    <?php

                    $dt_inicio = $_REQUEST['dt_inicio'] ? formata_data_sql($_REQUEST['dt_inicio']) : date('Y') . '-01-01';
                    $dt_fim = $_REQUEST['dt_fim'] ? formata_data_sql($_REQUEST['dt_fim']) : date('Y-m-d');
                    $tpdid = $_POST['tpdid'];
                    $esdid = $_POST['esdid'];


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

                    if($esdid && is_array($esdid)){
                        $esdid =  'and aed.esdiddestino in (' . implode(', ', array_filter( $esdid ) ) . ')';
                    } else {
                        $esdid = '';
                    }

                    if( $tpdid == 44){
                        // Se for fluxo PAR
                        $innerPar = 'inner join par.instrumentounidade iu on iu.docid = d.docid';
                    } else {
                        $innerPar = '';
                    }

                    $sql = "select count(*) as valor, to_char(h.htddata, '{$formatoBusca}') as data, to_char(h.htddata, '{$formatoExibicao}') as categoria, aed.esdiddestino, e.esddsc as descricao
                            from
                                workflow.historicodocumento h
                                inner join  workflow.documento d  on h.docid = d.docid
                                inner join workflow.acaoestadodoc aed on aed.aedid = h.aedid
                                inner join workflow.estadodocumento e on aed.esdiddestino = e.esdid
                                {$innerPar}
                            where h.htddata BETWEEN '{$dt_inicio} 00:00:00' and '{$dt_fim} 23:59:59'
                            and e.tpdid = {$tpdid}
                            and e.esdstatus = 'A'
                            and aed.aedstatus = 'A'
                            $esdid
                            group by data, categoria, aed.esdiddestino, descricao
                            order by data, descricao";

                    $sql2 = "select count (d.docid) as valor, esd.esddsc  descricao
                                FROM workflow.documento d
                                inner join workflow.estadodocumento esd on esd.esdid = d.esdid
                              {$innerPar}
                                where esd.tpdid = {$tpdid}
                                group by esd.esddsc,esd.tpdid";

//                                        ver($sql , $sql2);
//                                        exit;

//                    $dados = $db->carregar($sql);
//                    $dadosAgrupados = agruparDadosGrafico($dados);

//                    ver($dadosAgrupados, d);
                    $grafico = new Grafico();
                    //                    $grafico->setTipo(Grafico::K_TIPO_LINHA)->setHeight('300px')->gerarGrafico($sql);
                    //ver($grafico, d);
                    ?>

                                <?php $grafico->setTipo(Grafico::K_TIPO_LINHA)->setWidth('50%')->setHeight('300px')->setTitulo('Histórico')->gerarGrafico($sql); ?>


                    <?php
                    //                    $grafico->setTipo(Grafico::K_TIPO_COLUNA)->setHeight('500px')->gerarGrafico($sql);
                    //                    $grafico->setTipo(Grafico::K_TIPO_BARRA)->setHeight('500px')->gerarGrafico($sql);

                    //                    $grafico = new Grafico();
                    //                    $grafico->setTipo(Grafico::K_TIPO_COLUNACOLUNA)->gerarGrafico($sql);
                    //                    $grafico->setTipo(Grafico::K_TIPO_LINHA)->gerarGrafico($sql);
                    //                    $grafico->setHeight('500px')->setTipo(Grafico::K_TIPO_BARRA)->gerarGrafico($sql);

                    ?>
                </td>
                <td><?php $grafico->setTipo(Grafico::K_TIPO_PIZZA)->setWidth('40%')->setHeight('300px')->setTitulo('Situação atual')->gerarGrafico($sql2); ?></td>
            </tr>
        </table>
    </div>
    <?php exit;
    endif; ?>

<?php
function montarPainelWorkflow()
{
    global $db;
    ?>
<div id="panel_content">
    <div  style="padding: 15px;">
        <form name="formulario-historico-workflow" id="formulario-historico-workflow">
            <table style="opacity:1.65;
                    -moz-opacity: 1.65;
                    filter: alpha(opacity=165);
                    z-index: 1900
                    "
                   class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
                <tr>
                    <td style="text-align: right; font-weight: bold;" >Fluxo:</td>
                    <td>
                        <div>
                            <?php
                            $sql = "select tpdid as codigo, tpddsc as descricao
                                    from workflow.tipodocumento
                                    where sisid = '{$_SESSION['sisid']}'
                                    --and painel = 'A'
                                    order by descricao
                                    ";
                            echo $db->monta_combo("tpdid", $sql, 'S', "Selecione...", '', '', '', '500', 'N', 'painel_programa', '', $_REQUEST['tpdid'], '', 'class="situacaoPainel"');
                            ?>
                        </div>
                    </td>
                </tr>
                <tr>
                    <td style="text-align: right; font-weight: bold;">Situação:</td>
                    <td id="situacaoPainel">
                        <?php echo $db->monta_combo("esdid[]", array(), 'S', "Selecione...", 'situacaoPainel', '', '', '500', 'N', 'esdid', '', $_REQUEST['tpdid'], '', 'class="sel_chosen"'); ?>
                    </td>
                </tr>
                <tr>
                    <td style="text-align: right; font-weight: bold;">Período:</td>
                    <td>
                        <div>
                            <?php echo campo_texto('dt_inicio', 'N', 'S', '', 10, 10, '##/##/####', '', '', '', '', 'id="dt_inicio_painel" class="data" placeholder="dd/mm/aaaa"', ''); ?>
                            a
                            <?php echo campo_texto('dt_fim', 'N', 'S', '', 10, 10, '##/##/####', '', '', '', '', 'id="dt_fim_painel" class="data" placeholder="dd/mm/aaaa"', ''); ?>
                        </div>
                    </td>
                </tr>
                <tr>
                    <td style="text-align: right;  font-weight: bold;">Exibição:</td>
                    <td>
                        <div>
                            <input type="radio" name="tipo_exibicao" value="D" />Diário
                            <input type="radio" name="tipo_exibicao" value="M" style="margin-left: 10px;" checked="checked" />Mensal
                            <input type="radio" name="tipo_exibicao" value="A" style="margin-left: 10px;" />Anual
                        </div>
                    </td>
                </tr>
                <tr>
                    <td colspan="2" style="text-align: center;">
                        <div>
                            <input type="button" value="Atualizar" id="botao_atualizar" />
                        </div>
                    </td>
                </tr>
            </table>
        </form>
    </div>
    <div id="container_painel_grafico">
    </div>
    <script language="JavaScript">
            //    jQuery('.data')datepicker();
                    $1_11( ".data" ).datepicker(
                        {
                            beforeShow: function(input, inst) {
            //            inst.dpDiv.css({"z-index":2000});
                                setTimeout(function(){
                                    $1_11('.ui-datepicker').css('zIndex', 999999);
                                }, 0);
                            }

                            , changeMonth: true
                            , changeYear: true
                        }
                    ).datepicker();


                    $1_11('#painel_programa').change(function(){
                        url = window.location.href;
            //
                        dataForm = $1_11('#formulario-historico-workflow').serialize();
                        data = dataForm + '&carregarSituacaoPainel=true';
            //            console.info(data);
                        $1_11.post(
                            url
                            , data
                            , function(html){
            //                    $1_11( '#situacaoPainel').empty();
                                $1_11( '#situacaoPainel' ).hide().html(html).fadeIn('slow');
            //                    $1_11( '#situacaoPainel' ).fadeIn();
            //                    setTimeout(function(){
                                    $1_11( '#situacaoPainel' ).find('.sel_chosen').attr('multiple' , 'multiple');
                                    $1_11( '#situacaoPainel' ).find('.sel_chosen').attr('data-placeholder' , 'Selecione...');
                                    $1_11( '#situacaoPainel' ).find('option').attr('selected', 'selected');
                                    $1_11( '#situacaoPainel' ).find('.sel_chosen').chosen();
                                    console.info($1_11( '#situacaoPainel' ));
                                    console.info($1_11().jquery);
            //                    } , 300);
                            }
                        );
                    });

                    jQuery('#botao_atualizar').click(function(){

                        isValid = true;
                        if(jQuery('#painel_programa').val() == ''){
                            alert('Por favor, preencha o campo programa!');
                            jQuery('#painel_programa').focus();
                            isValid = false;
                            return false;
                        }
                        if(jQuery('#dt_inicio').val() == ''){
                            alert('Por favor, preencha a data de início do campo período!');
                            jQuery('#dt_inicio').focus();
                            isValid = false;
                            return false;
                        }
                        if(jQuery('#dt_fim').val() == ''){
                            alert('Por favor, preencha a data final do campo período!');
                            jQuery('#dt_fim').focus();
                            isValid = false;
                            return false;
                        }
            //        if(jQuery('input[name=tipo_exibicao]').val() == ''){
            //            alert('Por favor, preencha a data final do campo período!');
            //            jQuery('input[name=tipo_exibicao]').focus();
            //            isValid = false;
            //            return false;
            //        }

                        if(isValid){
                            url = window.location.href;

                            dataForm = jQuery('#formulario-historico-workflow').serialize();
                            data = dataForm + '&gerarGraficoPainel=true';
            //            console.info(data);
                            jQuery.post(
                                url
                                , data
                                , function(html){
                                    jQuery('#container_painel_grafico').hide().html(html).fadeIn('slow');
                                }
                            );
                        }

                    });
        </script>
<?php
}
?>