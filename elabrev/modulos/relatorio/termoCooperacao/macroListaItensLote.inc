<?php

if($_REQUEST['req'] == 'excluirTermoLote'){
    if($_REQUEST['loiid']){

        $lotid = $db->pegaUm("select lotid from elabrev.lotemacroitens where loiid = {$_REQUEST['loiid']}");

        $sql = "delete from elabrev.lotemacroitens where loiid = {$_REQUEST['loiid']}";
        $db->executar($sql);

        /*
         * select
                                loi.tcpid
                            from elabrev.lotemacroitens loi
                             join monitora.termocooperacao tcp on tcp.tcpid = loi.tcpid
                            join workflow.documento doc on doc.docid = tcp.docid and doc.esdid = ".EM_DESCENTRALIZACAO."
                            where loi.lotid = {$lotid}
         */
        $rs = $db->carregar("select tcpid from elabrev.lotemacroitens where lotid = {$lotid}");

        if($rs){
            foreach($rs as $dado){
                $arTcpid[] = $dado['tcpid'];
            }
        }

        if($arTcpid){
            $sql = "update elabrev.lotemacro set lotdsc = 'Termos: ".implode(',', $arTcpid)."' where lotid = {$lotid}";
            $db->executar($sql);
        }

        if($db->commit()){
            echo "1";
            die;
        }
    }
    echo "0";
    die;
}

require_once APPRAIZ . 'includes/cabecalho.inc';
echo "<br>";
monta_titulo( "Lista de Termos de Cooperação", 'Marque os Termos para Gerar a Macro' );

// Recupera termos para geração das macros
//$rs = recuperarLoteTermos();
$rs = recuperarLoteTermosTeste();

// Recupera quantidade maxima de previsão orçamentária do termo
//$qtdMaxPrevisao = contaQtdMaxPrevisaoOrcamentaria();
$qtdMaxPrevisao = contaQtdMaxPrevisaoOrcamentariaTeste();

$arMesNum = array('/01/','/02/','/03/','/04/','/05/','/06/','/07/','/08/','/09/','/10/','/11/','/12/');
$arMesDsc = array('/jan/','/fev/','/mar/','/abr/','/mai/','/jun/','/jul/','/ago/','/set/','/out/','/nov/','/dez/');
?>

<?php if($rs): ?>

    <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
    <script>
        $(function(){

            largura = (98*$(window).width())/100;
            $('#divConteudo').css('width', largura);

            $('.gerarLote').click(function(){
                if ($("[name^='proid[]']:checked").length<1) {
                    alert('Selecione pelo menos um termo.');
                    return false;
                }
                $('#aba').val('macroGeraExcel');
                $('#formulario').submit();
                $('#conteudoPagina').html('<center><p><input type="button" value="Voltar" id="voltar" class="voltar" /></p></center>');
            });

            /*$('#selectAll').click(function () {
                $("[name^='tcpid[]']").each(function(i,v){
                    if($(v).attr('checked')){
                        $(v).attr('checked', false);
                    }else{
                        $(v).attr('checked', true);
                    }
                });
            });*/
            $('#selectAll').click(function () {
                $("[name^='proid[]']").each(function(i,v){
                    if($(v).attr('checked')){
                        $(v).attr('checked', false);
                    }else{
                        $(v).attr('checked', true);
                    }
                });
            });

            $('.voltar').live('click', function(){
                document.location = 'elabrev.php?modulo=relatorio/termoCooperacao/macroTC&acao=A';
            });

            $('.excluirTermoLote').click(function(){

                if(confirm('Deseja excluir o termo deste lote?')){
                    $.ajax({
                        url		: 'elabrev.php?modulo=relatorio/termoCooperacao/macroTC&acao=A&aba=macroListaItensLote',
                        type	: 'post',
                        data	: 'req=excluirTermoLote&loiid='+this.id,
                        success	: function(e){
                            if(e == '1'){
                                window.location.reload();
                            }else{
                                alert('Erro ao excluir termo do lote.');
                            }
                        }
                    });
                }
            });

            $(".verCO").click(function(){
                var tcpid = $(this).attr("data-target-id")
                    , $element;
                if (tcpid) {
                    $element = $("#tr_tcpid_"+tcpid);
                    ($element.css("display") == 'none') ? $element.attr("style", "display:") : $element.attr("style", "display:none;");
                }
            });
        });

        $(window).resize(function(){
            largura = (93*$(window).width())/100;
            $('#divConteudo').css('width', largura);
        });
    </script>
    <center>
        <div id="conteudoPagina">
            <form id="formulario" name="formulario" method="post" action="elabrev.php?modulo=relatorio/termoCooperacao/macroTC&acao=A">
                <input type="hidden" name="aba" id="aba" value="" />
                <div style="overflow:auto;" id="divConteudo">
                    <table border="0" bgcolor="#DCDCDC" align="center" cellspacing="0" cellpadding="3" style="border-top: none; border-bottom: none;" class="tabela">
                        <thead>
                        <tr>
                            <?php if(empty($_REQUEST['lotid'])): ?>
                                <th><input type="checkbox" name="selectAll" id="selectAll" class="selectAll"/></th>
                            <?php else: ?>
                                <th>Ação</th>
                            <?php endif; ?>
                            <th>UG PROPONENTE</th>
                            <th>GESTÃO PROPONENTE</th>
                            <th>RESPONSÁVEL 1 - PROPONENTE</th>
                            <th>UG CONCEDENTE</th>
                            <th>GESTAO CONCEDENTE</th>
                            <th>RESPONSÁVEL 2 - CONCEDENTE</th>
                            <th>NÚMERO ORIGINAL</th>
                            <th>TIPO DE CADASTRO</th>
                            <th>NÚMERO DO PROCESSO</th>
                            <th>UG REPASSADORA</th>
                            <th>GESTÃO REPASSADORA</th>
                            <th>UG RECEBEDORA DO RECURSO</th>
                            <th>GESTÃO RECEBEDORA</th>
                            <th>TÍTULO</th>
                            <th>INÍCIO DA VIGÊNCIA</th>
                            <th>FIM DA VIGÊNCIA</th>
                            <th>VALOR FIRMADO</th>
                            <th>VALOR TOTAL</th>
                            <th>DESCRIÇÃO</th>
                            <th>JUSTIFICATIVA</th>
                            <th>EXERCÍCIO 1</th>
                            <th>EXERCÍCIO 2</th>
                            <th>EXERCÍCIO 3</th>
                            <th>EXERCÍCIO 4</th>
                            <th>EXERCÍCIO 5</th>
                            <th>EXERCÍCIO 6</th>
                            <th>PRAZO 1</th>
                            <th>PRAZO 2</th>
                            <th>PRAZO 3</th>
                            <th>PRAZO 4</th>
                            <th>PRAZO 5</th>
                            <th>% REAL</th>
                            <?php if($qtdMaxPrevisao>0): ?>
                                <?php for($x=1;$x<=$qtdMaxPrevisao;$x++): ?>
                                    <th>EVENTO <?php echo $x; ?></th>
                                    <th>ESFERA <?php echo $x; ?></th>
                                    <th>PTRES <?php echo $x; ?></th>
                                    <th>FONTE <?php echo $x; ?></th>
                                    <th>PI <?php echo $x; ?></th>
                                    <th>ND <?php echo $x; ?></th>
                                    <th>VALOR ND <?php echo $x; ?></th>
                                <?php endfor; ?>
                            <?php endif; ?>
                            <th>N° de Registro</th>
                        </tr>
                        </thead>
                        <tbody>
                        <?php foreach($rs as $k => $dado): ?>
                            <?php if (fmod($k,2) == 0 && $_GET['req'] != 'gerarExcel') $marcado = '' ; else $marcado='#F7F7F7'; ?>
                            <tr bgcolor="<?php echo $marcado; ?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?php echo $marcado; ?>';">
                                <?php if(empty($_REQUEST['lotid'])): ?>
                                    <td>
                                        <img style="cursor:pointer;" src="../imagens/seta_filho.gif" class="verCO" data-target-id="<?php echo $dado['tcpid'] ?>" />
                                        <input type="hidden" name="tcpid[]" value="<?php echo $dado['tcpid']; ?>"/>
                                    </td>
                                <?php else: ?>
                                    <td>
                                        <!-- <img style="cursor:pointer;" src="../imagens/excluir.gif" class="excluirTermoLote" id="<?= $dado['tcpid'] ?>-<?= $_GET['lotid']?>" /> -->
                                        <img style="cursor:pointer;" src="../imagens/seta_filho.gif" class="verCO" data-target-id="<?php echo $dado['tcpid'] ?>" />
                                    </td>
                                <?php endif; ?>
                                <td><?php echo $dado['ungcodproponente']; ?></td>
                                <td><?php echo $dado['gescodproponente']; ?></td>
                                <td><?php echo $dado['cpfreplegalproponente']; ?></td>
                                <td><?php echo $dado['ungcodconcedente']; ?></td>
                                <td><?php echo $dado['gescodconcedente']; ?></td>
                                <td><?php echo $dado['cpfreplegalconcedente']; ?></td>
                                <td><?php echo $dado['tcpid']; ?></td>
                                <td>6</td>
                                <td><?php echo $dado['tcpid']; ?></td>
                                <td>152734</td>
                                <td>00001</td>
                                <td><?php echo $dado['ungcodproponente']; ?></td>
                                <td><?php echo $dado['gescodproponente']; ?></td>
                                <td alt="<?php echo $dado['tcptitulo']; ?>" title="<?php echo $dado['tcptitulo']; ?>"><?php echo substr($dado['tcptitulo'], 0, 15); ?>...</td>
                                <td><?php echo $dado['data_vigencia'] ? str_replace($arMesNum, $arMesDsc, formata_data($dado['data_vigencia'])) : ''; ?></td>
                                <td><?php echo $dado['data_vigencia'] ? str_replace($arMesNum, $arMesDsc, makeDateSoma(formata_data($dado['data_vigencia']), 0, $dado['crdmesexecucao'])) : ''; ?></td>
                                <td><?php echo $dado['valor_total'] ? str_replace('.',',',$dado['valor_total']) : ''; ?></td>
                                <td><?php echo $dado['valor_total'] ? number_format($dado['valor_total'], 0, ".", "") : ''; ?></td>
                                <td alt="<?php echo $dado['tcpobjetivoobjeto']; ?>" title="<?php echo $dado['tcpobjetivoobjeto']; ?>"><?php echo substr($dado['tcpobjetivoobjeto'], 0, 15); ?>...</td>
                                <td alt="<?php echo $dado['tcpjustificativa']; ?>" title="<?php echo $dado['tcpjustificativa']; ?>"><?php echo substr($dado['tcpjustificativa'], 0, 15); ?>...</td>
                                <td></td>
                                <td></td>
                                <td></td>
                                <td></td>
                                <td></td>
                                <td></td>
                                <td>360</td>
                                <td></td>
                                <td></td>
                                <td></td>
                                <td></td>
                                <td>10000</td>

                                <?php

                                if($_REQUEST['lotid']){
                                    $where[] = "pro.proid in (select loi.proid from elabrev.lotemacroitens loi where loi.proid = pro.proid and loi.loistatus = 'A')";
                                }

                                $sql = "SELECT *, substr(ndpcod, 1, 6) as natureza FROM monitora.previsaoorcamentaria pro
										LEFT JOIN monitora.ptres ptr ON ptr.ptrid = pro.ptrid
										LEFT JOIN monitora.pi_planointerno pi on pi.pliid = pro.pliid
										LEFT JOIN public.naturezadespesa ndp ON ndp.ndpid = pro.ndpid
										WHERE pro.tcpid = {$dado['tcpid']}
										".(is_array($where) ? ' AND '.implode(' AND ',$where) : '')."
										ORDER BY pro.proid";
                                $rsPrevisoes = $db->carregar($sql);
                                ?>

                                <?php if($qtdMaxPrevisao>0): ?>
                                    <?php for($x=0;$x<$qtdMaxPrevisao;$x++): ?>

                                        <?php if($rsPrevisoes[$x]['proid']>0): ?>
                                            <td></td>
                                            <td></td>
                                            <td><?php echo $rsPrevisoes[$x]['ptres']; ?></td>
                                            <td></td>
                                            <td><?php echo $rsPrevisoes[$x]['plicod']; ?></td>
                                            <td><?php echo $rsPrevisoes[$x]['natureza']; ?></td>
                                            <td><?php echo str_replace('.','',$rsPrevisoes[$x]['provalor']); ?></td>
                                        <?php else: ?>
                                            <td>&nbsp;</td>
                                            <td>&nbsp;</td>
                                            <td>&nbsp;</td>
                                            <td>&nbsp;</td>
                                            <td>&nbsp;</td>
                                            <td>&nbsp;</td>
                                            <td>&nbsp;</td>
                                        <?php endif; ?>

                                    <?php endfor; ?>
                                <?php endif; ?>
                                <td><?php echo $k+1; ?></td>
                            </tr>

                            <?php if ('gerarExcel' != $_GET['req']): ?>
                                <tr class="tr_sub" id="tr_tcpid_<?= $dado['tcpid']; ?>" style="display:none">
                                    <td colspan="47">
                                        <?php celulaOrcamentariaTable($dado['tcpid'], $_GET['lotid']); ?>
                                    </td>
                                </tr>
                            <?php endif; ?>

                        <?php endforeach; ?>
                        </tbody>
                    </table>
                </div>
            </form>
            <p>
                <?php if(empty($_REQUEST['lotid'])): ?>
                    <input type="button" value="Gerar Lote" id="lote" class="gerarLote" />
                <?php endif; ?>
                <input type="button" value="Voltar" id="voltar" class="voltar" /></p>
        </div>
    </center>
<?php else: ?>
    <center><b>Sem registros.</b>
        <p><input type="button" value="Voltar" id="voltar" class="voltar" /></p></center>
<?php endif; ?>