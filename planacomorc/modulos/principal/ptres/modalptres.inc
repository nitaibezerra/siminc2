<?php
/**
 * Modal PTRES
 */
#ver($_REQUEST);
// verificando se é undidade ou unidade gestora
$params['where'] .= $_POST['prgcod'] ? "AND UPPER(dtl.prgcod) LIKE('%" . strtoupper($_POST['prgcod']) . "%')" : '';
$params['where'] .= $_POST['acacod'] ? "AND UPPER(aca.acacod) LIKE('%" . strtoupper($_POST['acacod']) . "%')" : '';

$params ['SELECT'] = <<<SELECT
    SELECT
        dtl.ptrid,
        dtl.ptres,
        trim(aca.prgcod || '.' || aca.acacod || '.' || aca.unicod || '.' || aca.loccod || ' - ' || aca.acatitulo) AS descricao,
        uni.unidsc,
        COALESCE(SUM(dtl.ptrdotacao), 0.00) AS dotacaoinicial,
        COALESCE(SUM(dt.valor), 0.00) AS det_subacao,
        -- dotacaoinicial - det_subacao
        (COALESCE(SUM(dtl.ptrdotacao), 0.00) - COALESCE(SUM(dt.valor), 0.00)) AS nao_det_subacao,
        --COALESCE(SUM(dt2.valorpi), 0.00) AS det_pi,
        -- det_subacao - det_pi
        --(COALESCE(SUM(dt.valor), 0.00) - COALESCE(SUM(dt2.valorpi), 0.00)) AS nao_det_pi,
        COALESCE((pemp.total), 0.00) AS empenhado,
        COALESCE(SUM(dtl.ptrdotacao), 0.00) - COALESCE(pemp.total, 0.00) AS nao_empenhado
SELECT;



//$sql_lista = retornaConsultaPTRES($params);
$sql_lista = <<<DML
    {$params ['SELECT']}
    FROM monitora.acao aca
    INNER JOIN monitora.ptres dtl ON aca.acaid = dtl.acaid
    INNER JOIN public.unidade uni ON uni.unicod = dtl.unicod
    LEFT JOIN (SELECT ptrid,
                      SUM(sadvalor) AS valor
                 FROM monitora.pi_subacaodotacao
                 GROUP BY ptrid) dt ON dtl.ptrid = dt.ptrid
    /*LEFT JOIN (SELECT ptrid,
                      SUM(dtl.valorpi) AS valorpi
                 FROM monitora.v_pi_detalhepiptres dtl
                 WHERE prgano = '{$_SESSION['exercicio']}'
                 GROUP BY dtl.ptrid) dt2 ON dtl.ptrid = dt2.ptrid*/
    LEFT JOIN siafi.ptrempenho pemp
      ON (pemp.ptres = dtl.ptres AND pemp.exercicio = aca.prgano)
    /*LEFT JOIN (SELECT pliid,
                      ptrid,
                      SUM(pipvalor) AS valor
                 FROM monitora.pi_planointernoptres
                 GROUP BY pliid, ptrid) pli ON pli.ptrid = dt.ptrid*/
    WHERE aca.prgano='{$_SESSION['exercicio']}'
        AND dtl.ptrano='{$_SESSION['exercicio']}'
        {$params['where']}
        AND ptrstatus = 'A'
        AND aca.acasnrap = FALSE
        AND aca.unicod IN('26101','26291', '26290', '26298', '26443', '74902', '73107')

    GROUP BY dtl.ptrid,dtl.ptres,descricao,uni.unidsc, pemp.total,  uni.unicod ORDER BY 1
DML;

require_once APPRAIZ . "www/planacomorc/_funcoes.php";
require_once APPRAIZ . "www/planacomorc/_constantes.php";
require_once APPRAIZ . "www/planacomorc/_funcoespi.php";
?>
<html>
    <head>
        <script src="/library/chosen-1.0.0/chosen.jquery.js" type="text/javascript"></script>
        <link href="../library/chosen-1.0.0/chosen.css" rel="stylesheet"  media="screen" >
        <script type="text/javascript" src="../includes/funcoes.js"></script>
        <script>

            function parametrosFiltro(){
                var params = {};
                $('#modal-ptres #formulario select').each(function() {
                    var inputName = $(this).attr('name'),
                    inputValue = $(this).val();
                    params[inputName] = inputValue;
                });
                return params;
            }

            function atualizaListagemPTRES(params){
                $('#modal-ptres .modal-body').html('');
                $.post('planacomorc.php?modulo=principal/ptres/modalptres&acao=A&obrigatorio=n', params, function(response) {
                    $('#modal-ptres .modal-body').html(response);
                    delegatePaginacao();
                    $('#modal-ptres .chosen-select').chosen();
                    //$('#modal-ptres .chosen-container').css('width', '100%');
                });
            }

            function delegatePaginacao(){
                $('.container-listing').on('click', 'li[class="pgd-item"]:not(".disabled")', function() {
                    // -- definindo a nova página
                    var novaPagina = $(this).attr('data-pagina');
                    var params = parametrosFiltro();
                    params['listagem[p]'] = novaPagina;
                    atualizaListagemPTRES(params);
                });
            }

            $(document).ready(function(){
                $('#modal-ptres .btn-info').click(function(e) {
                    e.stopPropagation();
                    var params = parametrosFiltro();
                    atualizaListagemPTRES(params);
                    return false;
                });
                /*$('.switch-mini').on('switch-change', function(e, d){
                    d.value;
                    var id = $('input[type=checkbox]', e.target).attr('id');
                    var onclick = $('#' + id).attr('onclick');
                    onclick = onclick.replace('this', '$(\'#\' + id)[0]');
                    eval(onclick);
                });
                apresentaLinhasSelecionadas($('#orcamento'),3);*/
            });
        </script>
    </head>
    <body>
        <section class="well">
            <fieldset>
                <form id="formulario" name="formulario" method="post" action="" class="form-horizontal">
                    <div class="form-group">
                        <label for="inputUnidade" class="col-lg-2 control-label">Programa:</label>
                        <div class="col-lg-10">
                            <?php
                            $sql = <<<DML
                                SELECT
                                    p.prgcod as codigo,
                                    (p.prgcod || ' - ' || p.prgdsc) as descricao
                                FROM monitora.programa p
                                WHERE p.prgano = '{$_SESSION['exercicio']}'
                                ORDER BY prgcod
DML;
                            $db->monta_combo('prgcod', $sql, 'S', 'Selecione', 'carregaAcao', null, null, 400, 'N', 'prgcod', null, (isset($_POST['prgcod']) ? $_POST['prgcod'] : null), null, 'class="form-control chosen-select" style="width=100%;""', null, null);
                            ?>
                        </div>
                    </div>
                    <div class="form-group">
                        <label for="inputUnidade" class="col-lg-2 control-label">Ação:</label>
                        <div class="col-lg-10 comboAcao" >
                            <?php
                            $sql = <<<DML
                                SELECT
                                    ac.acacod as codigo,
                                    (ac.acacod || ' - ' || ac.acadsc) as descricao
                                FROM monitora.acao ac
                                WHERE ac.unicod IS NOT NULL
                                    AND ac.prgano = '{$_SESSION['exercicio']}'
                                    AND ac.acastatus = 'A'
                                    AND ac.acasnrap = false
                                GROUP BY ac.acacod, ac.acadsc
                                ORDER BY ac.acacod
DML;
                            $db->monta_combo('acacod', $sql, 'S', 'Selecione', '', null, null, 400, 'N', 'acacod', null, (isset($_POST['acacod']) ? $_POST['acacod'] : null), null, 'class="form-control chosen-select" style="width=100%;""', null, null);
                            ?>
                        </div>
                    </div>

                    <div class="form-group">
                        <div class="col-lg-10 col-lg-offset-2">
                            <input type="submit" class="btn btn-info" value="Pesquisar" name="botao" />
                        </div>
                    </div>
                </form>
            </fieldset>
        </section>
        <section class="form-group">
            <div id="list-render">
            <?php
            if ($_REQUEST ['tipo'] == 'pi') {
                $cabecalho = array ("PTRES","Ação",	"Unidade Orçamentária",	"Dotação Atual(R$)",
                    'Subação (R$)' => array ('Detalhado','Não Detalhado'),
                    'Empenho (R$)' => array ('Empenhado','Não Empenhado'));
            } else {
                $cabecalho = array ("PTRES","Ação","Unidade Orçamentária","Dotação Atual(R$)",
                    'Subação (R$)' => array ('Detalhado','Não Detalhado'),
                    'Empenho (R$)' => array ('Empenhado','Não Empenhado'));
            }
            $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_PAGINADO);
            $listagem->addAcao('view', 'visualizarRegistro');
            $listagem->setIdLinha();
            $listagem->addAcao('select',array('func' => 'resultado', 'extra-params' => array('ptres')));
            $listagem->setCabecalho($cabecalho);
            $listagem->turnOnPesquisator();
            $listagem->addCallbackDeCampo(array('dotacaoinicial','det_subacao','nao_det_subacao','empenhado','nao_empenhado'), 'mascaraMoeda');
            #ver($sql_lista);
            $listagem->setQuery($sql_lista);
            $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
            $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
            ?>
            </div>
            <script>
                jQuery(document).ready(function() {
                    apresentaLinhasSelecionadas($('#orcamento'), 3);
                });

                function carregaAcao(prgcod) {
                    url = "planacomorc.php?modulo=principal/unidade/listarProgramaUG&acao=A";
                    $.ajax({
                        url:url,
                        type: 'POST',
                        async: false,
                        data: {prgcod: prgcod, comboacao:true},
                        success: function(html) {
                            $('.comboAcao').html(html);
                            $(".chosen-select").chosen();
                            $('.comboAcao').find('select').removeAttr('style');
                            $('.comboAcao').find('select').addClass('chosen-select form-control').chosen();
                        },
                        error: function (jqXHR, textStatus, errorThrown) {
                            console.log(jqXHR);
                            console.log(textStatus);
                            console.log(errorThrown);
                        }
                    });
                }
                /* CARREGANDO OS DADOS DE PTRES */
                var tabelaorigem = document.getElementById('orcamento');
                for (i = 2; i < tabelaorigem.rows.length - 2; i++) {
                    if (document.getElementById("chk_" + tabelaorigem.rows[i].cells[0].innerHTML)) {
                        document.getElementById("chk_" + tabelaorigem.rows[i].cells[0].innerHTML).checked = true;
                    }
                }
                /* FIM CARREGANDO OS DADOS DE PTRES */

                function resultado(id, selecionado, adicionais) {
                    var ptres = $('tr#' + id + ' td:nth-child(3)').text();
                    if (!ptres) {
                        alert('Não existe PTRES. Entre em contato com o administrador do sistema.');
                        return false;
                    }

                    if (selecionado == false) {
                        var tabelaorigem = document.getElementById('orcamento');
                        var colunaPtres = document.getElementById('ptres_' + id);
                        if (colunaPtres != null) {
                            tabelaorigem.deleteRow(document.getElementById('ptres_' + id).rowIndex);
                        }

                        // -- Criando a nova linha na tabela da página de origem
                        var linha = tabelaorigem.insertRow(3);
                        linha.id = "ptres_" + id;
                        linha.style.height = '30px';

                        // -- O número da primeira linha da tabela onde serão inseridos os novos dados
                        var linha = 3;

                        // setando o ptres
                        var celPTRES = tabelaorigem.rows[linha].insertCell(0);
                        celPTRES.style.textAlign = "center";
                        celPTRES.innerHTML = ptres;

                        var celAcao = tabelaorigem.rows[linha].insertCell(1);
                        var acao = $('tr#' + id + ' td:nth-child(4)').text();
                        celAcao.innerHTML = acao;

                        var celDotacao = tabelaorigem.rows[linha].insertCell(2);
                        celDotacao.style.textAlign = "right";
                        var dotacao = $('tr#' + id + ' td:nth-child(6)').text();
                        celDotacao.innerHTML = dotacao;

                        var celSubacaoDet = tabelaorigem.rows[linha].insertCell(3);
                        celSubacaoDet.style.textAlign = "right";
                        var subacaoDet = $('tr#' + id + ' td:nth-child(7)').text();
                        celSubacaoDet.innerHTML = subacaoDet;

                        var celSubacaoNaoDet = tabelaorigem.rows[linha].insertCell(4);
                        celSubacaoNaoDet.style.textAlign = "right";
                        var subacaoNaoDet = $('tr#' + id + ' td:nth-child(8)').text();
                        celSubacaoNaoDet.innerHTML = subacaoNaoDet;

                        var celEmpenhoEmp = tabelaorigem.rows[linha].insertCell(5);
                        celEmpenhoEmp.style.textAlign = "right";
                        var empenhoEmp = $('tr#' + id + ' td:nth-child(9)').text();
                        celEmpenhoEmp.innerHTML = empenhoEmp + "<input type='hidden' name='plivalor[" + ptres + "][" + id + "]' size='28'"
                                + "data-ptres='" + ptres + "' value ='0.00' />";
                        ;

                        var celEmpenhoNaoEmp = tabelaorigem.rows[linha].insertCell(6);
                        celEmpenhoNaoEmp.style.textAlign = "right";
                        var empenhoNaoEmp = $('tr#' + id + ' td:nth-child(10)').text();
                        celEmpenhoNaoEmp.innerHTML = empenhoNaoEmp + "<input type='hidden' name='acaid[" + ptres + "]' value='" + id + "'>";

                        var celValorPrevisto = tabelaorigem.rows[linha].insertCell(7);
                        celValorPrevisto.style.textAlign = "right";
                        celValorPrevisto.innerHTML =	'<input type="text" name="plivalor[' + ptres + '][' + id + ']" size="28" '
                        + 'onKeyUp="this.value=mascaraglobal(\'###.###.###.###,##\',this.value);calculovalorPI()" '
                        + 'onBlur="return verificaDisponivel(this,\'' + id + '\',\'0\')" '
                        + 'data-ptres="' + ptres + '" '
                        + 'class="form-control somar" onmouseover="MouseOver(this)" onfocus="MouseClick(this);this.select()" '
                        + 'onmouseout="MouseOut(this)" onblur="MouseBlur(this)" style="text-align:right;width:20ex" />';

                    } else {
                        var tabelaorigem = document.getElementById('orcamento');
                        tabelaorigem.deleteRow(document.getElementById('ptres_' + id).rowIndex);
                        //calculovalorPI();
                        //jogando ids desmarcados para verificar se existem no banco, se sim deletar vinculo.
                        //ptresApagar(id);
                    }
                }

                function apresentaLinhasSelecionadas(tabelaOrigem,apartirDe){
                    var val;
                    for (i = apartirDe; i < tabelaOrigem.find('tr').length; i++) {
                        val = tabelaOrigem.find('tr')[i].id.split("_");
                        if (document.getElementById(val[1])) {
                            console.log(val[1]);
                            campo = $('tr#' + val[1] + ' td:nth-child(2)').find('span');
                            campo.attr('class','glyphicon glyphicon-remove');
                            campo.attr('title','Remover item');
                            campo.css('color','gray');
                        }
                    }
                }

                function visualizarRegistro(ptrid) {
                    url = 'planacomorc.php?modulo=principal/ptres/detalheptresmodal&acao=A&detalhe=ok&ptrid=' + ptrid;
                    $.post(url, function(html) {
                        $('#modal-confirm .modal-body').html(html);
                        $('#modal-confirm .modal-title').html('Detalhamento PTRES');
                        $('#modal-confirm .btn-primary').remove();
                        $('#modal-confirm .btn-default').html('Fechar');
                        $('.modal-dialog').show();
                        $('#modal-confirm').modal();
                    });
                }
            </script>
        </section>
    </body>
</html>
<?php
unset($_REQUEST['prgcod']);
unset($_REQUEST['acacod']);
exit();