<?php
/**
 * Listagem de PTRESes e subações para remanejamento de subações.
 *
 * @see listaptres.inc
 * $Id: listaptres.inc 102359 2015-09-11 18:26:07Z maykelbraz $
 */

/**
 * Funções do remanejamento de subações.
 *
 * @see _funcoesremanejavalorsubacao.php
 */
include_once '_funcoesremanejavalorsubacao.php';

if ($_REQUEST ['requisicao']) {
    $_REQUEST ['requisicao'] ( $_REQUEST );
    exit ();
}
if($_POST['formulario'])
{
    // SUBAÇÃO
    $_SESSION['planacomorc']['listaptres']['tipobusca'] = $_POST['tipoBusca'];
    $_SESSION['planacomorc']['listaptres']['sbaapenassemPTRES'] = $_POST['sbaApenasSemPTRES'];
    $_SESSION['planacomorc']['listaptres']['buscalivresubacao'] = $_POST['buscalivresubacao'];

    // PTRES
    $_SESSION['planacomorc']['listaptres']['prgcod'] = $_POST['prgcod'];
    $_SESSION['planacomorc']['listaptres']['acacod'] = $_POST['acacod'];
    $_SESSION['planacomorc']['listaptres']['buscalivre'] = $_POST['buscalivre'];
}
/* Seleciona a pesquisa por PTRES ou por SUBAÇÃO */
$tipoPesquisa = 'ptres';
$buscaSemPtres = '';
$buscaSubacao = '';

$prgano = $_SESSION ['exercicio'];
$buscaPtres = '';
$prgcod = '';
$acacod = '';


//SUBALÇÃO
if ($_SESSION['planacomorc']['listaptres']['tipobusca']) {
    $tipoPesquisa = $_SESSION['planacomorc']['listaptres']['tipobusca'];
}

if ($_SESSION['planacomorc']['listaptres']['sbaapenassemPTRES']) {
    $buscaSemPtres = $_SESSION['planacomorc']['listaptres']['sbaapenassemPTRES'];
}

if ($_SESSION['planacomorc']['listaptres']['buscalivresubacao']) {
    $buscaSubacao = $_SESSION['planacomorc']['listaptres']['buscalivresubacao'];
}
//PTRES
if ($_SESSION['planacomorc']['listaptres']['prgcod']) {
    $prgcod = $_SESSION['planacomorc']['listaptres']['prgcod'];
}

if ($_SESSION['planacomorc']['listaptres']['acacod']) {
    $acacod = $_SESSION['planacomorc']['listaptres']['acacod'];
}

if ($_SESSION['planacomorc']['listaptres']['buscalivre']) {
    $buscaPtres = $_SESSION['planacomorc']['listaptres']['buscalivre'];
}
// Chamada de programa
include APPRAIZ . "includes/cabecalho.inc";

?>
<script>
    $(document).ready(function() {
    	pesquisaRapida('textFind');
        trocaTipoBusca('<?=$tipoPesquisa ?>');
    });

    function trocaTipoBusca(tipo) {
        $('.pesquisarPtres').hide();
        $('.pesquisarSubacao').hide();

        /* Pesquisar pelo PTRES */
        if (tipo == 'ptres') {
            $('.pesquisarPtres').show();
        }

        /* Pesquisar pela Subação */
        if (tipo == 'subacao') {
            $('.pesquisarSubacao').show();
        }
    }
    function detalheSubacao(sbaid){
    	var url ='planacomorc.php?modulo=principal/subacoes/detalhesubacao&acao=A&tp=r&sbaid=' + sbaid;
        $.post(url,function(html) {
            $('#modal-confirm .modal-body p').html(html);
            $('.modal-dialog').css('width','90%');
            $('#modal-confirm .modal-title').html('Dados da Subação');
            $('#modal-confirm .btn-primary').remove();
            $('#modal-confirm .btn-default').html('Fechar');
            $('.modal-dialog').show();
            $('#modal-confirm').modal();
        });
    }

    function pesquisaRapida(id){
        jQuery.expr[':'].contains = function(a, i, m) {
            return jQuery(a).text().toUpperCase().indexOf(m[3].toUpperCase()) >= 0;
        };

        $("#"+id).keyup(function()
        {
            $('table.table tbody tr td').removeClass('marcado');
            $('table.table tbody tr').removeClass('remover');
            stringPesquisa = $("#"+id).val();
            if (stringPesquisa) {
                $('table.table tbody tr td:contains(' + stringPesquisa + ')').addClass('marcado');
                $('table.table tbody tr:not(:contains(' + stringPesquisa + '))').addClass('remover');
            }
        });
   }
</script>

<?php
/* Unidades Obrigatórias (AD, CAPES, INEP, FNDE, FIES, SUP.MEC, EBSERH */
$obrigatorias = UNIDADES_OBRIGATORIAS;

if ($tipoPesquisa == 'ptres') {
    $atiid = $_POST [atiid] ? $_POST [atiid] : $_GET [atiid];
    $sbaid = $_REQUEST ['sbaid'];
    $prgano = $_SESSION ['exercicio'];

    $where .= $_REQUEST ['sbaid'] ? "AND dt.sbaid = '" . $_REQUEST ['sbaid'] . "'" : '';
    $where .= $prgcod ? "AND UPPER(dtl.prgcod) LIKE('%" . strtoupper ( $_POST ['prgcod'] ) . "%')" : '';
    $where .= $acacod ? "AND UPPER(dtl.acacod) LIKE('%" . strtoupper ( $_POST ['acacod'] ) . "%')" : '';

    $buscaLivre = removeAcentos ( str_replace ( "-", " ", $buscaPtres ) );
    $where .= $buscaLivre ? "AND (public.removeacento(trim(aca.prgcod||'.'||aca.acacod||'.'||aca.loccod||' - '||aca.acadsc)) ilike('%" . $buscaLivre . "%') OR public.removeacento(dtl.ptres) ilike '%" . $buscaLivre . "%')" : '';

    $sql_lista = <<<DML
        SELECT
            dtl.ptrid AS remanejar,
            dtl.ptres,
            TRIM(aca.prgcod || '.' || aca.acacod || '.' || aca.unicod || '.' || aca.loccod || ' - ' ||
            aca.acatitulo) AS descricao,
            uni.unicod || ' - ' || uni.unidsc as unidsc,
            SUM(dtl.ptrdotacao)           AS dotacaoinicial,
            COALESCE(SUM(dt.valor), 0.00) AS det_subacao,
            COALESCE(SUM(dtl.ptrdotacao) - COALESCE(SUM(dt.valor), 0.00), 0.00) AS nao_det_subacao,
            COALESCE((pemp.total), 0.00)                                     AS empenhado,
            (SUM(dtl.ptrdotacao) - COALESCE((pemp.total), 0.00)) AS nao_empenhado
        FROM monitora.acao aca
        INNER JOIN monitora.ptres dtl ON aca.acaid = dtl.acaid
        INNER JOIN public.unidade uni ON uni.unicod = dtl.unicod
        LEFT JOIN (SELECT ptrid, SUM(sadvalor) AS valor FROM monitora.pi_subacaodotacao GROUP BY ptrid) dt ON dtl.ptrid = dt.ptrid
        LEFT JOIN (SELECT ptrid, SUM(dtl.valorpi) AS valorpi, dtl.prgano FROM monitora.v_pi_detalhepiptres dtl GROUP BY dtl.ptrid, dtl.prgano) dt2 ON dtl.ptrid = dt2.ptrid AND dt2.prgano = aca.prgano
        LEFT JOIN siafi.ptrempenho pemp ON (pemp.ptres = dtl.ptres AND pemp.exercicio = aca.prgano)
        WHERE aca.prgano = '{$_SESSION['exercicio']}'
            AND dtl.ptrano = '{$_SESSION['exercicio']}'
            AND dtl.ptrstatus = 'A'
            AND aca.acasnrap = FALSE
            AND aca.unicod IN({$obrigatorias})
DML;

    $sql_lista .= $where ? $where : '';
    $sql_lista .= " GROUP BY dtl.ptrid,dtl.ptres,descricao,uni.unidsc, pemp.total, remanejar,uni.unicod ORDER BY 1";
    /*
     * global $db; $r = $db->carregar($sql_lista); ver($r,d);
     */
    $cabecalho = array (
        "PTRES",
        "Ação",
        "Unidade Orçamentária",
        "Dotação (R$)",
        'Subação (R$)' => array (
            'Detalhado',
            'Não Detalhado'
        ),
        'Empenho (R$)' => array (
            'Empenhado',
            'Não Empenhado'
        )
    );
} else {
    $params ['where'] = " ";
    if($buscaSemPtres != 'on'){
        $params ['where'] .= " AND (ptr.unicod IN('26101','26291', '26290', '26298', '26443', '74902', '73107') OR ptr.unicod IS NULL) ";
        $join = 'LEFT JOIN monitora.ptres ptr ON ptr.ptrid = sdt.ptrid';
    }
    $params ['where'] .= $_REQUEST ['buscalivresubacao'] ? "AND  sba.sbacod ilike '%{$_REQUEST['buscalivresubacao']}%'" : '';



    #$sql_lista = retornaConsultaSubacao_bootstrap($params);
    $sql_lista = <<<DML
        SELECT
            sbaid,
            codigo,
            sbatitulo,
            dotacao,
            SUM(empenhado) AS empenhado,
            SUM(dotacao) - SUM(empenhado) AS saldo_nao_empenhado
        FROM
            (SELECT
                sba.sbacod as codigo,
                sba.sbacod as codigo2,
                COALESCE(uni.unicod||' - '||uni.unidsc, ' - ') as unidsc,
                sba.sbatitulo,
                sba.sbaid,
                (   SELECT COALESCE(SUM(sadvalor),0.00)
                    FROM monitora.pi_subacaodotacao sd
                    JOIN monitora.ptres pt on pt.ptrid = sd.ptrid
                    JOIN monitora.acao aca on aca.acaid = pt.acaid
                    WHERE aca.prgano = '{$_SESSION['exercicio']}'
                        AND sd.sbaid = sba.sbaid
                        AND aca.unicod IN('26101','26291', '26290', '26298', '26443', '74902', '73107')
                ) AS dotacao,
                COALESCE(dtpi.valorpi, 0.00) AS detalhado_pi,
                COALESCE(semp.total, 0.00) AS empenhado
            FROM monitora.pi_subacao sba
            LEFT JOIN monitora.pi_subacaodotacao sdt ON sdt.sbaid = sba.sbaid
            {$join}
            LEFT JOIN (
                SELECT sbaid, SUM(pip.pipvalor) as valorpi FROM monitora.pi_planointerno pli
                INNER JOIN monitora.pi_planointernoptres pip ON pli.pliid = pip.pliid
                WHERE pli.pliano = '{$_SESSION['exercicio']}'
                    AND pli.plistatus = 'A'
                    AND pli.plisituacao IN ('A','C','S','T')
                GROUP BY sbaid) dtpi ON dtpi.sbaid = sba.sbaid
            LEFT JOIN siafi.sbaempenho semp on semp.sbacod = sba.sbacod AND semp.exercicio = sba.sbaano
            LEFT JOIN monitora.pi_subacaounidade psu ON    psu.sbaid = sba.sbaid
            LEFT JOIN public.unidade uni on psu.unicod = uni.unicod
            WHERE sba.sbastatus = 'A'
                AND sba.sbaano = '{$_SESSION['exercicio']}'
                {$params ['where']}
            GROUP BY 2, 3, sba.sbaid, sba.sbacod, semp.total, sba.sbatitulo, dtpi.valorpi, uni.unicod, uni.unidsc
            ORDER BY sba.sbacod ) AS foo
        GROUP BY
            codigo,
            sbatitulo,
            unidsc,
            dotacao,
            sbaid
        ORDER BY 2
DML;

    $cabecalho = array (
        "Código",
        "Subação",
        "Orçamento Atual (R$)",
        "Empenhado (R$)",
        "Não Empenhado (R$)"
    );
}
#ver($sql_lista);
?>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<section class="col-md-12">
    <ol class="breadcrumb">
        <li><a href="/planacomorc/planacomorc.php?modulo=inicio&acao=C"><?=$_SESSION['sisdsc']; ?></a></li>
        <li class="">Subações <?=$_SESSION['exercicio'];?></li>
        <li class="active">Solicitação - Remanejamento de Subação (por PTRES)</li>
    </ol>
</section>
<section id="modalDadosSubacao" title="Dados da Subação"></section>
<section class="col-md-12">
    <section class="well">
        <form class="form-horizontal" role="form" action="" method="post" name="formulario">
            <input type="hidden" name="formulario" value="filtro" />
            <section class="form-group">
                <label class="control-label col-md-2">Pesquisar por:</label>
                <section class="col-md-10">
                    <div class="btn-group" data-toggle="buttons">
                        <label onclick="trocaTipoBusca('ptres');" class="btn btn-default  <?php if ($tipoPesquisa == 'ptres') echo 'active'; ?>">
                            <input type="radio" name="tipoBusca" value="ptres" <?php if ($tipoPesquisa == 'ptres') echo 'checked'; ?> /> PTRES
                        </label>
                        <label onclick="trocaTipoBusca('subacao');" class="btn btn-default <?php if ($tipoPesquisa == 'subacao') echo 'active'; ?>">
                            <input type="radio" name="tipoBusca" value="subacao" <?php if ($tipoPesquisa == 'subacao') echo 'checked'; ?>/> Subação
                        </label>
                    </div>
                </section>
            </section>
            <section class="form-group pesquisarPtres">
                <label class="control-label col-md-2" for="prgcod">Programa:</label>
                <section class="col-md-10">
                <?php
                    $sql = "
                        SELECT
                            p.prgcod as codigo,
                            (p.prgcod || ' - ' || p.prgdsc) as descricao
                        FROM monitora.programa p
                        WHERE p.prgano = '{$prgano}'
                        ORDER BY prgcod
                    ";
                    inputCombo('prgcod', $sql, $prgcod, 'prgcod');
                ?>
                </section>
            </section>
            <section class="form-group pesquisarPtres">
                <label class="control-label col-md-2" for="acacod">Ação:</label>
                <section class="col-md-10">
                <?php
                    $sql = "
                        SELECT
                            ac.acacod as codigo,
                            (ac.acacod || ' - ' || ac.acadsc) as descricao
                        FROM monitora.acao ac
                        WHERE ac.prgano = '{$prgano}' AND ac.acastatus = 'A' AND ac.acasnrap = false
                        GROUP BY ac.acacod, ac.acadsc
                        ORDER BY ac.acacod
                    ";
                    inputCombo ( 'acacod', $sql, $acacod, 'acacod' );
                ?>
                </section>
            </section>
            <section class="form-group pesquisarPtres">
                <label class="control-label col-md-2" for="buscalivre">Buscar:</label>
                <section class="col-md-10">
                    <?php inputTexto('buscalivre', $buscaPtres, 'buscalivre', 150,false); ?>
                </section>
            </section>
            <section class="form-group pesquisarSubacao">
                <label class="control-label col-md-2" for="buscalivresubacao">Buscar:</label>
                <section class="col-md-10">
                    <?php inputTexto('buscalivresubacao', $buscaSubacao, 'buscalivresubacao', 150,false);?>
                </section>
            </section>
            <section class="form-group pesquisarSubacao">
                <section class="col-md-10 col-md-offset-2">
                    <div class="btn-group" data-toggle="buttons">
                        <label class="btn btn-default <?php if($buscaSemPtres == 'on') echo "active" ; ?>">
                            <input type="checkbox" id="sbaApenasSemPTRES" name="sbaApenasSemPTRES"
                            <?php if($_REQUEST['sbaApenasSemPTRES'] && $_REQUEST['sbaApenasSemPTRES']=='on') echo "checked" ;?> />
                            Pesquisar apenas Subações sem PTRES vinculado
                        </label>
                    </div>
                </section>
            </section>
            <section class="form-group">
                <section class="col-md-10 col-md-offset-2">
                    <input type="submit" class="btn btn-primary" name="botao" value="Pesquisar" />
                </section>
            </section>
        </form>
    </section>
    <section class="">
<?php
if($tipoPesquisa == 'ptres') {
    $list = new Simec_Listagem ();
    $list->setCabecalho($cabecalho);
    $list->addAcao('send2','resultado');
    $list->addCallbackDeCampo(array('dotacaoinicial','det_subacao',	'nao_det_subacao','empenhado','nao_empenhado'),'mascaraMoeda');
    $list->setQuery($sql_lista);
    $list->addCallbackDeCampo(array('descricao', 'unidsc' ), 'alinhaParaEsquerda');
    $list->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA,array('dotacaoinicial','det_subacao','nao_det_subacao','empenhado','nao_empenhado'));
    $list->turnOnPesquisator();
    $list->render(SIMEC_LISTAGEM::SEM_REGISTROS_MENSAGEM);
}else{
    $list = new Simec_Listagem ();
    $list->setCabecalho($cabecalho);
    $list->addAcao('view','detalheSubacao');
    $list->addCallbackDeCampo(array('dotacao','empenhado','saldo_nao_empenhado'),'mascaraMoeda');
    $list->addCallbackDeCampo(array('sbatitulo' ), 'alinhaParaEsquerda');
    $list->setQuery($sql_lista);
    $list->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, array ('dotacao','empenhado','saldo_nao_empenhado'));
    $list->turnOnPesquisator();
    $list->render(SIMEC_LISTAGEM::SEM_REGISTROS_MENSAGEM);
}
?>
    </section>
</section>
<script type="text/javascript">
    function resultado(ptrid) {
        if (!ptrid) {
            alert('Não existe PTRES. Entre em contato com o administrador do sistema.');
            return false;
        } else {
            location.href = '/planacomorc/planacomorc.php?modulo=principal/remanejavalorsubacao/remaneja&acao=A&ptrid=' + ptrid;
        }
    }
</script>

