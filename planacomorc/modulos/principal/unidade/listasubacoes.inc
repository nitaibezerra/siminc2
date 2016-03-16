<?php
include_once '_funcoespi.php';
require_once (APPRAIZ . 'includes/library/simec/Listagem.php');
$perfis = pegaPerfilGeral();
if (in_array(PFL_GESTAO_ORCAMENTARIA_IFS, $perfis)) {
    $sqlUO = <<<DML
EXISTS (SELECT 1
         FROM planacomorc.usuarioresponsabilidade rpu
         WHERE rpu.usucpf = '%s'
           AND rpu.pflcod = %d
           AND rpu.rpustatus = 'A'
           AND rpu.unicod  = uni.unicod)
DML;
    $where[] = $whereUO = sprintf($sqlUO, $_SESSION['usucpf'], PFL_GESTAO_ORCAMENTARIA_IFS);
    $whereUO = " AND {$whereUO}";
}

include_once '_funcoessubacoes.php';
if ($_REQUEST['requisicao']) {
    $_REQUEST['requisicao']($_REQUEST);
    exit;
}

//Chamada de programa
include APPRAIZ . "includes/cabecalho.inc";
?>
<script>
    $(document).ready(function() {
        pesquisaRapida('textFind');
    });

    /* Editar a Subação */
    function alterarSubacao(sbaid) {
        location.href = 'planacomorc.php?modulo=principal/unidade/cadastro_subacoes&acao=A&sbaid=' + sbaid;
    }

    function reload() {
        location.href = 'planacomorc.php?modulo=principal/unidade/listasubacoes&acao=A';
    }

    /* Excluir a Subação */
    function removerSubacao(sbaid, sbacod) {
        if (confirm('Deseja apagar a Subação ?')) {
            location.href = 'planacomorc.php?modulo=principal/unidade/cadastro_subacoes&acao=A&apagar=true&sbaidExcluir=' + sbaid;
        }
    }
    function pesquisaRapida(id) {
        jQuery.expr[':'].contains = function(a, i, m) {
            return jQuery(a).text().toUpperCase().indexOf(m[3].toUpperCase()) >= 0;
        };

        $("#" + id).keyup(function()
        {
            $('table.table tbody tr td').removeClass('marcado');
            $('table.table tbody tr').removeClass('remover');
            stringPesquisa = $("#" + id).val();
            if (stringPesquisa) {
                $('table.table tbody tr td:contains(' + stringPesquisa + ')').addClass('marcado');
                $('table.table tbody tr:not(:contains(' + stringPesquisa + '))').addClass('remover');
            }
        });
    }

    function detalheSubacao(sbaid) {
        var url = 'planacomorc.php?modulo=principal/subacoes/detalhesubacao&acao=A&sbaid=' + sbaid;
        $.post(url, function(html) {
            $('#modal-confirm .modal-body p').html(html);
            $('.modal-dialog').css('width', '90%');
            $('#modal-confirm .modal-title').html('Dados da Subação');
            $('#modal-confirm .btn-primary').remove();
            $('#modal-confirm .btn-default').html('Fechar');
            $('.modal-dialog').show();
            $('#modal-confirm').modal();
        });
    }
</script> 
<div id="modalMonitorInterno" style="display:none;"></div>
<div class="col-md-12">
    <ol class="breadcrumb">
        <li><a href="/planacomorc/planacomorc.php?modulo=inicio&acao=C"><?= $_SESSION['sisdsc']; ?></a></li>
        <li class="active">Instituições Federais / Lista de Subações <?= $_SESSION['exercicio']; ?></li>
    </ol>	
    
    <br />
    <div class="well">
        <fieldset>
            <form id="formulario" name="formulario" method="POST"
                  class="form-horizontal">
                <input type='hidden' name='pesquisa' id='pesquisa' value='1' />
                <div class="form-group">
                    <label for="inputUnidade" class="col-lg-2 control-label">Unidade Orçamentária (UO):</label>
                    <div class="col-lg-10">
                        <?php
                        $obrigatorias = UNIDADES_OBRIGATORIAS;
                        if (in_array(PFL_CGO_EQUIPE_ORCAMENTARIA, $perfis) ||
                                in_array(PFL_SUPER_USUARIO, $perfis)) {
                            $whereUO = "";
                        }
                        $sql = <<<DML
                            SELECT
                                uni.unicod AS codigo,
                                uni.unicod || ' - ' || unidsc AS descricao
                            FROM public.unidade uni
                            WHERE (uni.orgcod = '26000' OR  uni.unicod IN('74902', '73107'))
                                AND uni.unistatus = 'A'
                                {$whereUO}
                                AND uni.unicod NOT IN($obrigatorias)
                            ORDER BY uni.unicod
DML;
                        if ($_REQUEST['unicod']) {
                            $unicod = $_REQUEST['unicod'];
                        }
                        $db->monta_combo('unicod', $sql, 'S', 'Selecione', null, null, null, null, 'N', 'unicod', null, isset($unidade) ? $unidade : '', null, 'class="form-control chosen-select" style="width=100%;""', null, null);
                        ?>
                    </div>
                </div>
                <div class="form-group">
                    <div class="col-lg-10 col-lg-offset-2">
                        <button class="btn btn-warning" type="reset" onclick="reload()">Limpar</button>
                        <button class="btn btn-primary" id="buscar" type="submit">Buscar</button>
                    </div>
                </div>
            </form>
        </fieldset>
    </div>   

<?php
if($_REQUEST['unicod']){
    $whereUnicod = "AND psu.unicod = '{$_REQUEST['unicod']}'";
}
$sql = <<<DML
    SELECT sbaid as acoes,
        codigo,
        sbatitulo,
        unidsc,
        dotacao,
        SUM(CAST(detalhado_pi AS NUMERIC)) AS detalhado_pi,
        SUM(empenhado) AS empenhado,
        SUM(dotacao) - SUM(CAST(detalhado_pi AS NUMERIC)) AS saldo_nao_detalhado,
        SUM(dotacao) - SUM(empenhado) AS saldo_nao_empenhado
    FROM (
        SELECT  sba.sbacod  as codigo,
            sba.sbacod,
            COALESCE(uni.unicod||' - '||uni.unidsc, ' - ') as unidsc,
            sba.sbatitulo,
            sba.sbaid,
            (SELECT COALESCE(SUM(sadvalor),0.00) 
                FROM monitora.pi_subacaodotacao sd 
                JOIN monitora.ptres pt on pt.ptrid=sd.ptrid
                JOIN monitora.acao aca on aca.acaid = pt.acaid
                WHERE aca.prgano = '{$_SESSION['exercicio']}' AND sd.sbaid = sba.sbaid 
                    AND aca.unicod NOT IN('26101','26291', '26290', '26298', '26443', '74902', '73107')
            ) AS dotacao,
            COALESCE(dtpi.valorpi, 0.00) AS detalhado_pi,
            COALESCE(semp.total, 0.00) AS empenhado 
        FROM monitora.pi_subacao sba
        LEFT JOIN monitora.pi_subacaodotacao sdt ON sdt.sbaid = sba.sbaid
        LEFT JOIN monitora.ptres ptr ON ptr.ptrid = sdt.ptrid
        LEFT JOIN (
            SELECT
                sbaid,
                SUM(pip.pipvalor) as valorpi
            FROM monitora.pi_planointerno pli
            INNER JOIN monitora.pi_planointernoptres pip ON pli.pliid = pip.pliid
            WHERE pli.pliano = '{$_SESSION['exercicio']}'
                AND pli.plistatus = 'A'
            GROUP BY sbaid
            ) dtpi ON dtpi.sbaid = sba.sbaid
        LEFT JOIN monitora.pi_subacaounidade psu ON    psu.sbaid = sba.sbaid
        LEFT JOIN (
	    SELECT ex.unicod, substr(ex.plicod,2,4) as sbacod, ex.exercicio, sum(ex.vlrempenhado) AS total
            FROM spo.siopexecucao ex
	    WHERE ex.unicod NOT IN('26101','26291', '26290', '26298', '26443', '74902', '73107')
                AND ex.exercicio = '{$_SESSION['exercicio']}'
	    GROUP BY ex.unicod, sbacod, ex.exercicio
        ) semp ON semp.unicod = psu.unicod AND semp.sbacod = sba.sbacod AND semp.exercicio = sba.sbaano
            
        LEFT JOIN public.unidade uni on psu.unicod = uni.unicod                
           
        WHERE sba.sbastatus = 'A' 
            AND sba.sbaano = '{$_SESSION['exercicio']}'
            AND psu.unicod is not null
            AND (ptr.unicod NOT IN('26101','26291', '26290', '26298', '26443', '74902', '73107')  OR ptr.unicod IS NULL)
            AND sba.sbaid NOT IN ( SELECT sbaid FROM monitora.pi_subacaounidade WHERE unicod IN('26101','26291','26290','26298','26443','74902','73107')
                OR ungcod IN ( SELECT ungcod FROM public.unidadegestora WHERE unicod IN('26101','26291','26290','26298','26443','74902','73107')))
            {$whereUnicod}
            {$whereUO}
        GROUP BY 2, 3, sba.sbaid, sba.sbacod, semp.total, sba.sbatitulo, dtpi.valorpi, uni.unicod, uni.unidsc
        ORDER BY sba.sbacod 
    ) AS foo
    GROUP BY sbacod,
        codigo,
        sbatitulo,
        unidsc,
        dotacao,
        acoes,
        sbaid
    ORDER BY 2
DML;
$listagem = new Simec_Listagem();
$listagem->setCabecalho(array(
    "Código",
    "Subação",
    "Unidade Orçamentária",
    "Orçamento Atual (R$)",
    "Detalhado em PI (R$)",
    "Empenhado (R$)",
    "Não Detalhado em PI (R$)",
    "Não Empenhado (R$)"));
$listagem->setQuery($sql);
$listagem->addCallbackDeCampo(array(
    'dotacao', 'dotacaoinicial', 'det_subacao', 'det_pi', 'empenhado', 'saldo', 'detalhado_pi', 'saldo_nao_detalhado', 'saldo_nao_empenhado'), 'mascaraMoeda');
$listagem->setAcoes(array(
    'edit' => 'alterarSubacao',
    'delete' => 'removerSubacao'
));
$listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
$listagem->turnOnPesquisator();
$listagem->addCallbackDeCampo(array('sbatitulo', 'unidsc'), 'alinhaParaEsquerda');
#ver($sql);
$listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
?>
</div>