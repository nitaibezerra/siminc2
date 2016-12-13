<?php
/**
 * Relatório de responsáveis por ações.
 * $Id: acoes.inc 92264 2014-12-31 12:12:31Z lindalbertofilho $
 */
include_once APPRAIZ . 'includes/funcoesspo.php';
include '_funcoespi.php';
require_once(APPRAIZ . 'includes/library/simec/Listagem.php');

// -- Filtro de períodos
$sqlPeriodo = <<<DML
    SELECT 
        id_periodo_referencia AS codigo,
        titulo || ' : ' || to_char(inicio_validade,'DD/MM/YYYY') ||' a ' || to_char(fim_validade,'DD/MM/YYYY') as descricao
    FROM planacomorc.periodo_referencia p
    WHERE id_exercicio = '{$_SESSION['exercicio']}'
    ORDER BY id_periodo_referencia DESC
DML;
// -- Identificando / selecionando o periodo de consulta
if (!$_REQUEST['id_periodo_referencia']) {
    $_REQUEST['id_periodo_referencia'] = $db->pegaUm("SELECT id_periodo_referencia FROM planacomorc.periodo_referencia ORDER BY id_periodo_referencia DESC LIMIT 1");
}
//ver($_REQUEST['id_periodo_referencia'],!$_REQUEST['id_periodo_referencia'],d);
if (!isset($_REQUEST['responsavel'])) {
    $_REQUEST['responsavel'] = 'monitor';
}

$sqlResponsavel = '';
$colunasRelatorio = array(
    'UO',
    'Ações'
);
if ($_REQUEST['responsavel']) {
    if(!is_array($_REQUEST['responsavel'])){
        $sqlResponsavel = <<<PARCIAL_DML
                (SELECT COALESCE(' '|| REPLACE(TO_CHAR(m.moncpf::NUMERIC, '000:000:000-00'), ':', '.') || ' - ' || m.monnome, ' Não cadastrado') AS monitor
                   FROM planacomorc.monitorinterno m
                   WHERE m.orgcod = org.codigo
                   ORDER BY m.monid DESC
                   LIMIT 1) AS monitor
PARCIAL_DML;
        $colunasRelatorio[] = 'Monitor';
    }else{
        foreach ($_REQUEST['responsavel'] as $rps){
            if($rps == 'monitor'){
                if($sqlResponsavel){
                    $separador = ' , ';
                }
                $sqlResponsavel .= <<<PARCIAL_DML
                {$separador}(SELECT COALESCE(' '|| REPLACE(TO_CHAR(m.moncpf::NUMERIC, '000:000:000-00'), ':', '.') || ' - ' || m.monnome, ' Não cadastrado') AS monitor
                   FROM planacomorc.monitorinterno m
                   WHERE m.orgcod = org.codigo
                   ORDER BY m.monid DESC
                   LIMIT 1) AS monitor
PARCIAL_DML;
                 $colunasRelatorio[] = 'Monitor';
            }
            if($rps == 'coordenador'){
                if($sqlResponsavel){
                    $separador = ' , ';
                }
                $codigo = PFL_COORDENADORACAO;
                $sqlResponsavel .= <<<PARCIAL_DML
                {$separador}COALESCE((SELECT usucpf || ' - ' || usu.usunome
                            FROM planacomorc.usuarioresponsabilidade rpu
                              INNER JOIN seguranca.usuario usu USING(usucpf)
                            WHERE rpu.rpustatus = 'A'
                              AND rpu.id_acao_programatica = apr.id_acao_programatica
                              AND rpu.id_periodo_referencia = aac.id_periodo_referencia
                              AND rpu.pflcod = {$codigo}
                            ORDER BY rpu.rpuid DESC
                            LIMIT 1), '-') AS coordenador
PARCIAL_DML;
                $colunasRelatorio[] = 'Coordenador';
            }
            if($rps == 'validador'){
                 if($sqlResponsavel){
                    $separador = ' , ';
                }
                $sqlResponsavel .= <<<PARCIAL_DML
                {$separador}COALESCE((SELECT usucpf || ' - ' || usu.usunome
                            FROM planacomorc.usuarioresponsabilidade rpu
                              INNER JOIN seguranca.usuario usu USING(usucpf)
                            WHERE rpu.rpustatus = 'A'
                              AND rpu.id_acao_programatica = apr.id_acao_programatica
                              AND rpu.id_periodo_referencia = aac.id_periodo_referencia
                              AND rpu.pflcod = %d
                            ORDER BY rpu.rpuid DESC
                            LIMIT 1), '-') AS validador
PARCIAL_DML;
                $sqlResponsavel = sprintf($sqlResponsavel, PFL_VALIDADORACAO);
                $colunasRelatorio[] = 'Validador';
            }
            if($rps == 'val_subst'){
                if($sqlResponsavel){
                    $separador = ' , ';
                }
                $sqlResponsavel .= <<<PARCIAL_DML
                {$separador}COALESCE((SELECT usucpf || ' - ' || usu.usunome
                            FROM planacomorc.usuarioresponsabilidade rpu
                              INNER JOIN seguranca.usuario usu USING(usucpf)
                            WHERE rpu.rpustatus = 'A'
                              AND rpu.id_acao_programatica = apr.id_acao_programatica
                              AND rpu.id_periodo_referencia = aac.id_periodo_referencia
                              AND rpu.pflcod = %d
                            ORDER BY rpu.rpuid DESC
                            LIMIT 1), '-') AS val_subst
PARCIAL_DML;
                $sqlResponsavel = sprintf($sqlResponsavel, PFL_VALIDADOR_SUBSTITUTO);
                $colunasRelatorio[] = 'Validador Substituto';
            }
        }
    }
}
if (isset($_REQUEST['unicod']) && !empty($_REQUEST['unicod'])) {
    $sqlUnicod = <<<DML
    AND org.codigo = '{$_REQUEST['unicod']}'
DML;
}
if ($_REQUEST['requisicao'] == 'exportarXLS') {
    $list = new Simec_Listagem(Simec_Listagem::RELATORIO_XLS);
    $_REQUEST['_p'] = 'all';
} else {
    $list = new Simec_Listagem();
}

$sqlRelatorio = <<<DML
    SELECT DISTINCT
        org.codigo || ' - ' || uni.unidsc AS uo,
        pro.codigo || '. <strong>' || aca.codigo || '</strong>' || ' - ' || dae.titulo AS acao
        %s
    FROM planacomorc.acao aca
    LEFT JOIN planacomorc.acao_programatica apr ON apr.id_acao = aca.id_acao
    LEFT JOIN planacomorc.dados_acao_exercicio dae ON (dae.id_acao = aca.id_acao AND dae.id_exercicio = apr.id_exercicio)
    LEFT JOIN planacomorc.localizador_programatica lpr USING(id_acao_programatica)
    LEFT JOIN planacomorc.programa pro ON pro.id_programa = aca.id_programa
    LEFT JOIN planacomorc.orgao org ON org.id_orgao = apr.id_orgao
    LEFT JOIN planacomorc.acompanhamento_acao aac ON (aac.id_localizador = lpr.id_localizador AND aac.id_acao_programatica = lpr.id_acao_programatica)
    LEFT JOIN public.unidade uni on org.codigo = uni.unicod
    WHERE aac.id_periodo_referencia = {$_REQUEST['id_periodo_referencia']}
    {$sqlUnicod}
    ORDER BY 1, 2
DML;

$query = sprintf($sqlRelatorio, !empty($sqlResponsavel) ? ',' . trim($sqlResponsavel) : '');
//ver($query,d);
$list->addCallbackDeCampo(array('acao', 'uo','monitor','coordenador','validador','val_subst'), 'alinhaParaEsquerda');
$list->setCabecalho($colunasRelatorio)->setQuery($query);
if ($_REQUEST['requisicao'] == 'exportarXLS') {
    $list->render();
    echo "
        <script>
            window.location = window.location.href;
        </script>
    ";
    die();
}
/**
 * Cabeçalho base do SIMEC.
 */
include APPRAIZ . 'includes/cabecalho.inc';
?>
<script type="text/javascript">
    $(function() {
        pesquisaRapida('textFind');

        $(".btn-consulta-responsaveis").on("click", function() {
            $('#requisicao').attr('value', '');
            if (!$("#id_periodo_referencia").val()) {
                alert("Voce precisa selecionar um período!");
                $("#id_periodo_referencia").focus();
                return false;
            }

            $("#filtroacao").submit();
        });
        
         $('#exportar').click(function() {
            $('#requisicao').attr('value', 'exportarXLS');
            $('#filtroacao').submit();
        });
    });

    function pesquisaRapida(id) {
        jQuery.expr[':'].contains = function(a, i, m) {
            return jQuery(a).text().toUpperCase().indexOf(m[3].toUpperCase()) >= 0;
        };

        $("#" + id).on("keyup", function() {
            $('table.table tbody tr td').removeClass('marcado');
            $('table.table tbody tr').removeClass('remover');
            stringPesquisa = $("#" + id).val();
            if (stringPesquisa) {
                $('table.table tbody tr td:contains(' + stringPesquisa + ')').addClass('marcado');
                $('table.table tbody tr:not(:contains(' + stringPesquisa + '))').addClass('remover');
            }
        });
    }
</script>

<!-- breadCrumb da page -->
<div class="col-lg-12">
    <ol class="breadcrumb">
        <li><a href="/planacomorc/planacomorc.php?modulo=inicio&acao=C"><?= $_SESSION['sisdsc']; ?></a></li>
        <li class="">Ações <?= $_SESSION['exercicio']; ?></li>
        <li class="active">Relatório de Responsáveis por Ações</li>
    </ol>
</div>

<!-- formulário com grid de opções para envio ao ws -->
<div class="col-lg-12">
    <div class="well">
        <form class="form-horizontal" role="form" name="filtroacao" id="filtroacao" action="" method="post">
            <input type="hidden" name="requisicao" id="requisicao">
            <div class="form-group">
                <label class="control-label col-md-2" for="id_periodo_referencia">Período</label>
                <div class="col-md-10">
                    <?php echo inputCombo('id_periodo_referencia', $sqlPeriodo, isset($_POST['id_periodo_referencia']) ? $_POST['id_periodo_referencia'] : '', 'id_periodo_referencia'); ?>
                </div>
            </div>
            <div class="form-group">
                <label class="control-label col-md-2" for="unicod">Unidade Orçamentária</label>
                <div class="col-md-10">
                    <?php
                    $sqlUO = "
                        SELECT
                            uni.unicod AS codigo,
                            uni.unicod || ' - ' || unidsc AS descricao
                            FROM
                            public.unidade uni
                        WHERE
                            (
                                uni.orgcod = '". CODIGO_ORGAO_SISTEMA. "'
                                OR  uni.unicod IN('74902', '73107')
                            )
                        AND uni.unistatus = 'A'
                        ORDER BY uni.unicod
";
                    ?>
                    <?php echo inputCombo('unicod', $sqlUO, isset($_POST['unicod']) ? $_POST['unicod'] : '', 'unicod'); ?>
                </div>
            </div>
            <div class="form-group">
                <label class="control-label col-md-2" for="unicod">Tipo de Responsável</label>
                    <?php
                    $responsavel = $_REQUEST['responsavel'];
                    $responsaveis = array(
                        array('codigo' => 'monitor', 'descricao' => 'Monitor'),
                        array('codigo' => 'coordenador', 'descricao' => 'Coordenador'),
                        array('codigo' => 'validador', 'descricao' => 'Validador'),
                        array('codigo' => 'val_subst', 'descricao' => 'Validador Substituto')
                    );
                    ?>
                <div class="col-lg-10">
                    <select name="responsavel[]" id="responsavel" multiple class="form-control chosen-select-no-single" required="required" data-placeholder="Selecione">
                        <option value=""></option>
                        <?php
                        if ($responsavel && is_array($responsavel)) {
                            foreach ($responsaveis as $resp){ ?>
                        <option <?php if (in_array($resp['codigo'],$responsavel)) echo 'selected="selected"' ?>  value="<?php echo $resp['codigo'] ?>"><?php echo $resp['descricao'] ?></option>
                                    <?php
                            }
                        }else {
                            foreach ($responsaveis as $resp){
                            ?>
                                <option  value="<?php echo $resp['codigo'] ?>"><?php echo $resp['descricao'] ?></option>
                            <?php
                            }
                        }
                        ?>
                    </select>
                </div>
            </div>
            <div class="form-group">                
                <div class="col-md-10 col-md-offset-2">
                    <button type="button" class="btn btn-primary btn-consulta-responsaveis">Buscar</button>
                    <button type="button" class="btn btn-danger" id="exportar">Exportar XLS</button>
                </div>
            </div>            
        </form>
    </div>
</div>

<div class="col-lg-12">
<?php
$list->turnOnPesquisator();
$list->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
//echo $db->monta_lista($t, $colunasRelatorio, 3000, 5, 'N', 'center');
?>
</div>