<?php
require (APPRAIZ . 'includes/library/simec/Listagem.php');
//Conforme solicitação do Juvenal, lote de segunda parcela está liberado sem data de prazo a pedido do usuário.
//$dataAtual = date('Y/m/d');
//$dataLimite = date('2015/09/30');

if ($_REQUEST['requisicao'] == 'listarGerarLote') {
    echo listarLote($_REQUEST['exercicioSel'], $_REQUEST['tipo']);
    die();
}
if ('listagemGrupoLote' == $_POST['requisicao']) {
    listagemGrupoLote($_REQUEST['dados'][0]);
    die();
}
if ($_REQUEST['requisicao']) {
    $_REQUEST['requisicao']($_REQUEST);
}

function listarLote($exercicio, $tipo) {
    global $db;
    if ($tipo) {
        $sql = "
                    SELECT  '2' AS acao,
                            est.estuf,
                            mun.mundescricao AS mundescricao,
                            mun.muncod,
                            cnpj.entnumcpfcnpj,
                            count(atecpf) as total_alunos,
                            count(atecpf) * (SELECT valvaloranual FROM eja.valoraluno WHERE valexercicio = '{$exercicio}') AS valor_custeio,
                            count(atecpf) * (SELECT valvaloranual FROM eja.valoraluno WHERE valexercicio = '{$exercicio}')/2 as valor_parcela,
                            --(select COUNT(*) from eja.alunoturmaeja ant INNER JOIN eja.novaturmaeja nte on ant.nteid = nte.nteid where ant.atestatus2p = 'C' and nte.nteenviofnde2p = 'V') as total_alunos,
                            --(select COUNT(*) from eja.alunoturmaeja ant INNER JOIN eja.novaturmaeja nte on ant.nteid = nte.nteid where ant.atestatus2p = 'C' and nte.nteenviofnde2p = 'V') * (SELECT valvaloranual FROM eja.valoraluno WHERE valexercicio = '{$exercicio}' and valstatus = 'A') AS valor_custeio,
                            -- (select COUNT(*) from eja.alunoturmaeja ant INNER JOIN eja.novaturmaeja nte on ant.nteid = nte.nteid where ant.atestatus2p = 'C' and nte.nteenviofnde2p = 'V') * (SELECT valvaloranual FROM eja.valoraluno WHERE valexercicio = '{$exercicio}' and valstatus = 'A')/2 as valor_parcela,
                            '2' as num_parcela,
                            (SELECT valexercicio FROM eja.valoraluno WHERE valexercicio = '{$exercicio}') AS ano_exercicio
                            --{$exercicio} AS ano_exercicio
                    FROM  eja.alunoturmaeja ate

                    JOIN eja.novaturmaeja nte ON nte.nteid = ate.nteid
                    JOIN entidade.entidade ent ON ent.entid = nte.pk_cod_entidade
                    JOIN entidade.endereco ende ON ende.entid = ent.entid
                    JOIN territorios.municipio mun ON mun.muncod = ende.muncod
                    JOIN territorios.estado est ON est.estuf = mun.estuf

                    LEFT JOIN(
			SELECT e.entnumcpfcnpj, ende.muncod
			FROM entidade.entidade e
			JOIN entidade.endereco ende ON ende.entid = e.entid
			JOIN entidade.funcaoentidade f on f.entid = e.entid AND f.funid = 1
                    ) AS cnpj on cnpj.muncod = ende.muncod

                    WHERE nte.ntestatus = 'A' AND ntefechaturma = 'F' AND nteenviofnde2p = 'V' AND atestatus2p <> 'N'
                    AND ent.entstatus = 'A' AND ( (atecpf <> '' OR atecpf IS NOT NULL) AND (atestatus2p IS NULL OR atestatus2p <> 'E') )
                    and ateexercicio = '{$exercicio}'
                    GROUP BY  est.estuf, mun.mundescricao, mun.muncod, cnpj.entnumcpfcnpj
                    ORDER BY est.estuf, mun.mundescricao
                ";
        $marcaTodos = "<input type=\"checkbox\" id=\"marcatodos\" name=\"marcatodos\" checked=\"checked\" onclick=\"marcarTodos(this);\" />";
        $cabecalho = array($marcaTodos, "Estado", "Municípo", "Código do IBGE", "CNPJ", "Total Alunos", "Valor Total", "Valor Parcela", "Nº Parcela", "Exercicio");

        $colunaTotalAluno = array("total_alunos", "valor_parcela");
        //$colunasValorTotal = array("valor_custeio","valor_parcela");

        $listagem = new Simec_Listagem();
        $listagem->setCabecalho($cabecalho);
        $listagem->setQuery($sql);
        $listagem->setFormOff();
        $listagem->addCallbackDeCampo($colunaTotalAluno, "mascaraNumero");
        $listagem->addCallbackDeCampo($colunasValorTotal, "mascaraMoeda");
        $listagem->addCallbackDeCampo(array('valor_custeio', 'valor_parcela'), 'mascaraMoeda');
        $listagem->addCallbackDeCampo('acao', 'serializa_linha');
        $listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, $colunaTotalAluno);
        $listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, $colunasValorTotal);
        // $retornoPi = $listagem->render();
        $html .= $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
    } else {
        $sqlVerif = "SELECT valvaloranual FROM eja.valoraluno WHERE valexercicio = '{$exercicio}' and valstatus = 'A'";
        $valorNulo = $db->pegaUm($sqlVerif);
        if ($valorNulo) {
            $sql = "
                    SELECT  '2' AS acao,
                            est.estuf,
                            mun.mundescricao AS mundescricao,
                            mun.muncod,
                            cnpj.entnumcpfcnpj,
                            count(atecpf) as total_alunos,
                            count(atecpf) * (SELECT valvaloranual FROM eja.valoraluno WHERE valexercicio = '{$exercicio}' and valstatus = 'A') AS valor_custeio,
                            count(atecpf) * (SELECT valvaloranual FROM eja.valoraluno WHERE valexercicio = '{$exercicio}' and valstatus = 'A')/2 as valor_parcela,
                            --(select COUNT(*) from eja.alunoturmaeja ant INNER JOIN eja.novaturmaeja nte on ant.nteid = nte.nteid where ant.atestatus2p = 'C' and nte.nteenviofnde2p = 'V') as total_alunos,
                            --(select COUNT(*) from eja.alunoturmaeja ant INNER JOIN eja.novaturmaeja nte on ant.nteid = nte.nteid where ant.atestatus2p = 'C' and nte.nteenviofnde2p = 'V') * (SELECT valvaloranual FROM eja.valoraluno WHERE valexercicio = '{$exercicio}' and valstatus = 'A') AS valor_custeio,
                            -- (select COUNT(*) from eja.alunoturmaeja ant INNER JOIN eja.novaturmaeja nte on ant.nteid = nte.nteid where ant.atestatus2p = 'C' and nte.nteenviofnde2p = 'V') * (SELECT valvaloranual FROM eja.valoraluno WHERE valexercicio = '{$exercicio}' and valstatus = 'A')/2 as valor_parcela,
                            '2' as num_parcela,
                            (SELECT valexercicio FROM eja.valoraluno WHERE valexercicio = '{$exercicio}') AS ano_exercicio
                            --{$exercicio} AS ano_exercicio
                    FROM  eja.alunoturmaeja ate

                    JOIN eja.novaturmaeja nte ON nte.nteid = ate.nteid
                    JOIN entidade.entidade ent ON ent.entid = nte.pk_cod_entidade
                    JOIN entidade.endereco ende ON ende.entid = ent.entid
                    JOIN territorios.municipio mun ON mun.muncod = ende.muncod
                    JOIN territorios.estado est ON est.estuf = mun.estuf

                    LEFT JOIN(
			SELECT e.entnumcpfcnpj, ende.muncod
			FROM entidade.entidade e
			JOIN entidade.endereco ende ON ende.entid = e.entid
			JOIN entidade.funcaoentidade f on f.entid = e.entid AND f.funid = 1
                    ) AS cnpj on cnpj.muncod = ende.muncod

                    WHERE nte.ntestatus = 'A' AND ntefechaturma = 'F' AND nteenviofnde2p = 'V' AND atestatus2p <> 'N'
                    AND ent.entstatus = 'A' AND ( (atecpf <> '' OR atecpf IS NOT NULL) AND (atestatus2p IS NULL OR atestatus2p <> 'E') )
                     and ateexercicio = '{$exercicio}'
                    GROUP BY  est.estuf, mun.mundescricao, mun.muncod, cnpj.entnumcpfcnpj
                    ORDER BY est.estuf, mun.mundescricao
                ";
            $marcaTodos = "<input type=\"checkbox\" id=\"marcatodos\" name=\"marcatodos\" checked=\"checked\" onclick=\"marcarTodos(this);\" />";
            $cabecalho = array($marcaTodos, "Estado", "Municípo", "Código do IBGE", "CNPJ", "Total Alunos", "Valor Total", "Valor Parcela", "Nº Parcela", "Exercicio");

            $colunaTotalAluno = array("total_alunos", "valor_parcela");
            //$colunasValorTotal = array("valor_custeio","valor_parcela");

            $listagem = new Simec_Listagem();
            $listagem->setCabecalho($cabecalho);
            $listagem->setQuery($sql);
            $listagem->setFormOff();
            $listagem->addCallbackDeCampo($colunaTotalAluno, "mascaraNumero");
            $listagem->addCallbackDeCampo($colunasValorTotal, "mascaraMoeda");
            $listagem->addCallbackDeCampo(array('valor_custeio', 'valor_parcela'), 'mascaraMoeda');
            $listagem->addCallbackDeCampo('acao', 'serializa_linha');
            $listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, $colunaTotalAluno);
            $listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, $colunasValorTotal);
            // $retornoPi = $listagem->render();
            $html .= $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
        } else {
            echo "<script>if(confirm('O ano selecionado não está ativo! Tem certeza que deseja continuar?')){listaGerarLote(" . $exercicio . ",'anoInativo') ;}else{ window.location = 'eja.php?modulo=principal/confirmarcpf/segunda_parcela&acao=A';} </script>";
        }
    }
    return $html;
}

include APPRAIZ . "includes/cabecalho.inc";
?>
<style type="text/css">
    table thead > tr > th, .table tbody > tr > th, .table tfoot > tr > th, .table thead > tr > td, .table tbody > tr > td, .table tfoot > tr > td {
        padding: 0px !important;
    }
</style>
<script type="text/javascript">

    $(document).ready(function() {
        $('#bntSegundaParcela').click(function() {
            var confirma = confirm("Deseja realmente criar o lote para segunda parcela?");
            if (confirma) {
                $('#formulario').serialize();

                var erro;

                if (!erro) {
                    $('#requisicao').val('salvarDadosSegundaParcela');
                    $('#formulario').submit();
                }
            }
        });
    });


    function listaGerarLote(exercicio, tipo) {
        if (tipo) {
            $.post("eja.php?modulo=principal/confirmarcpf/segunda_parcela&acao=A&requisicao=listarGerarLote&tipo=anoInativo&exercicioSel=" + exercicio, function(html) {
//            alert(html);
                $('#list_gerar_lote').html(html);
            });
            document.getElementById("list_gerar_lote").style.display = "block";
        } else {
            $.post("eja.php?modulo=principal/confirmarcpf/segunda_parcela&acao=A&requisicao=listarGerarLote&exercicioSel=" + exercicio, function(html) {
//            alert(html);
                $('#list_gerar_lote').html(html);
            });
            document.getElementById("list_gerar_lote").style.display = "block";

        }
    }

    function excluirSegundaParcela(id) {
        var confirma = confirm("Deseja realmente \"EXCLUIR\" lote para segunda parcela?");
        if (confirma) {
            $('#lotnumero').val(id);
            $('#requisicao').val('excluirSegundaParcela');
            $('#formulario').submit();
        }
    }

    function imprimirPortaria(id) {
        var url = 'eja.php?modulo=principal/gerarlote/impressao_portaria&acao=A&lotnumero=' + id;
        window.open(url, 'imprimir_portaria', 'width=700,height=720,scrollbars=yes,scrolling=no,resizebled=no');
    }

    function listagemGrupoLote(lotid) {
        if (lotid) {
            $.ajax({
                type: "POST",
                url: window.location.href,
                data: "requisicao=listagemGrupoLote&lotid=" + lotid,
                async: false,
                success: function(resp) {
                    return html(resp);
                }
            });
        }
    }

    function desabilita_linha(obj) {
        var input = obj.id.replace('linha_', 'turma_');
        var check = obj.checked;

        if (check === false) {
            $('#' + input).prop('disabled', true);
        } else {
            $('#' + input).prop('disabled', false);
        }
    }

    function marcarTodos(obj) {
        var input;
        if (obj.checked == true) {
            $("input[type='checkbox']").each(function(i, o) {
                input = o.id.replace('linha_', 'turma_');
                if (input != 'marcatodos') {
                    $('#' + input).prop('disabled', false);
                    $(this).prop("checked", true);
                }
            });
        } else {
            $("input[type='checkbox']").each(function(i, o) {
                input = o.id.replace('linha_', 'turma_');
                if (input != 'marcatodos') {
                    $('#' + input).prop('disabled', true);
                    $(this).prop("checked", false);
                }
            });
        }
    }

</script>
<!-- LISTAGEM DOS LOTE GERADOS - GRID 1 -->
<div class="page-header">
    <h4 id="forms"> Lotes Gerados - Segunda Parcela</h4>
</div>
<div class="row col-lg-12">
    <div id="list_lotes_gerados" style="border-bottom:1px solid black; height:260px; overflow: auto;">
<?PHP
$perfis = pegaPerfilGeral();

$sql = "
                SELECT  '' AS acao, 
                        lotnumero AS acao,
                        'Lote número - '|| lotnumero ||' - Criado em: ' || to_char(lotdtgerado, 'DD/MM/YYYY') AS lote
                FROM eja.lote 
                where lotnumparcela = '2'
                GROUP BY lotnumero, lotdtgerado
                ORDER BY lotnumero
            ";
$cabecalho = array("Lote");

$listagem = new Simec_Listagem();
$listagem->setCabecalho($cabecalho);
$listagem->setQuery($sql);
$listagem->addAcao('plus', 'listagemGrupoLote');
$listagem->addAcao('print', 'imprimirPortaria');

#APENAS SUPER USUARIO E GESTOR MEC PODE VISUALIZAR A AÇÃO EXCLUIR LOTE.
if (in_array(PERFIL_EJA_SUPER_USUARIO, $perfis) || in_array(PERFIL_EJA_GESTOR_MEC, $perfis)) {
    $listagem->addAcao('delete', 'excluirSegundaParcela');
}

$listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
$listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
?>
    </div>
</div>

<!-- LISTAGEM DOS MUNICÍPIOS QUE TEM LOTE QUE PODEM SER GERADOS - GRID 2 -->
<div class="page-header">
    <h4 id="forms"> Gerar Lotes - Segunda Parcela </h4>
</div>
<div class="row col-lg-12">
    <form name="formulario" id="formulario" method="POST">
        <input type="hidden" id="requisicao" name="requisicao" value="">
        <input type="hidden" id="lotnumero" name="lotnumero" value="">
<?php //if (strtotime($dataAtual) <= strtotime($dataLimite)) { ?>
            <label for="inputUnidade" class="col-lg-2 control-label">Exercício:</label>
            <!--            </div>
                        <div class="col-lg-10">-->
            <select name="exercicioLote" id="exercicioLote" class="form-control chosen-select" onchange="listaGerarLote(this.value, null)">
                <option value="">Selecione...</option>
                <option value="2014">2014</option>
                <option value="2015">2015</option>
            </select> 
            <div id="list_gerar_lote" style="border-bottom:1px solid black; height:280px; overflow: auto;">
            </div>

            <div class="text-center"><br>
                <button class="btn btn-success" id="bntSegundaParcela" type="button" <?php echo $semRegistro; ?>>
                    <i class="glyphicon glyphicon-upload"></i> Criar Lote 2ª Parcela
                </button>
            </div>
        </form>

<?php
//} else {
//    echo "*A partir de hoje (1/10/2015) os lotes de segunda parcela não podem ser gerados.";
//}
?>
</div>
