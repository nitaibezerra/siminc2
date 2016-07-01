<?php
/**
 * Arquivo da aba principal do pedido de limites.
 * $Id: formPedido.inc 96489 2015-04-17 15:03:08Z lindalbertofilho $
 */

/**
 * Classe de listagem.
 * @see Simec_Listagem
 */
require_once(APPRAIZ . 'includes/library/simec/Listagem.php');

/**
 * Se o anexo estiver selecionado retorna a string 'active', ou '' se não estiver selecionado.
 * @param string $anexo Identificador do tipo de anexo.
 * @return string
 */
function anexoSelecionado($anexo)
{
    if ($anexo == $_REQUEST['anexo']) {
        return 'active';
    }
    return '';
}

/**
 * Retorna 'block' ou 'none' para mostrar ou esconder o detalhamento de um anexo.
 * @param string $anexo Identificador do tipo de anexo.
 * @return string
 */
function mostrarAnexo($anexo)
{
    if ($anexo == $_REQUEST['anexo']) {
        return 'block';
    }
    return 'none';
}

/**
 * Retorna 'block' se o anexo escolhido for I ou III. Usada para mostrar ou esconder
 * a div de referencia. Ou retorna como um booleano para verificações.
 * @param bool $asBool Indica se eh para retornar como um booleano ou como classe.
 * @return string
 */
function mostrarRefIeIII($asBool = false)
{
    if ($asBool) {
        return ($_REQUEST['anexo'] != 'I' && $_REQUEST['anexo'] != "III");
    }

    if ($_REQUEST['anexo'] != 'I' && $_REQUEST['anexo'] != "III") {
        return "none";
    } else {
        return "block";
    }
}

/**
 * Retorna 'active' caso a referencia selecionada seja igual a uma determinada
 * referencia, ou se ela for a referencia default.
 * @global int $pdlid
 * @param string $refSel Referencia selecionada
 * @param string $ref Referencia para verificação
 * @param bool $default Se este é o valor default
 * @return string
 */
function referenciaSelecionada($refSel, $ref, $default = false)
{
    global $pdlid;
    if (($refSel == $ref) || ($default && !$pdlid)) {
        return 'active';
    }
}

// -- Incluir a função limpar anexo no onchange do checkbox.
$limpaAnexo = '';
if ($pdlid) {
    $limpaAnexo = 'limpaAnexo(this.value);';
}
?>
<script type="text/javascript" language="javascript">
function teste(escolha)
{
    $(":input.editavel[readonly]").prop('readonly', false);
    $(":input.editavel").val('0,00');
    $(".totalatendido").val('0,00');
//    if (escolha == 'I1') {
//        $('#ampliacaoatendido_CA').prop('readonly', true);
//        $('#reducaoatendido_CA').prop('readonly', true);
//    }

//    if (escolha == 'I3') {
//        $('#ampliacaoatendido_CD').prop('readonly', true);
//        $('#reducaoatendido_CD').prop('readonly', true);
//    }
//    if (escolha == 'C1') {
//        $('#ampliacaoatendido_IA').prop('readonly', true);
//        $('#reducaoatendido_IA').prop('readonly', true);
//    }
//    if (escolha == 'C3') {
//        $('#ampliacaoatendido_ID').prop('readonly', true);
//        $('#reducaoatendido_ID').prop('readonly', true);
//    }
}

function myFunction(anexo)
{
    $("div[id^='div_']").css("display", "none");
    $('#div_' + anexo).css("display", "block");
}


function somaTotal(tipo, anexo, status)
{
    if (anexo == 'I') {
        if (status == 'atendido') {
            $('#' + tipo + '_I_atendido').val(somaEFormata([tipo + 'atendido_CA', tipo + 'atendido_IA']));
        }
        if (status == 'pedido') {
            $('#' + tipo + '_I_pedido').val(somaEFormata([tipo + '_CA', tipo + '_IA']));
        }
    }
    if (anexo == 'II') {
        if (status == 'atendido') {
            $('#' + tipo + '_II_atendido').val(somaEFormata([tipo + 'atendido_CB', tipo + 'atendido_IB']));
        }
        if (status == 'pedido') {
            $('#' + tipo + '_II_pedido').val(somaEFormata([tipo + '_CB', tipo + '_IB']));
        }
    }
    if (anexo == 'III') {
        if (status == 'atendido') {
            $('#' + tipo + '_III_atendido').val(somaEFormata([tipo + 'atendido_CD', tipo + 'atendido_ID']));
        }
        if (status == 'pedido') {
            $('#' + tipo + '_III_pedido').val(somaEFormata([tipo + '_CD', tipo + '_ID']));
        }
    }
    if (anexo == 'IV') {
        if (status == 'atendido') {
            $('#' + tipo + '_IV_atendido').val(somaEFormata([tipo + 'atendido_XA', tipo + 'atendido_XB']));
        }
        if (status == 'pedido') {
            $('#' + tipo + '_IV_pedido').val(somaEFormata([tipo + '_XA', tipo + '_XB']));
        }
    }
    if (anexo == 'V') {
        if (status == 'atendido') {
            $('#' + tipo + '_V_atendido').val(somaEFormata([tipo + 'atendido_PA', tipo + 'atendido_PD']));
        }
        if (status == 'pedido') {
            $('#' + tipo + '_V_pedido').val(somaEFormata([tipo + '_PA', tipo + '_PD']));
        }
    }
    if (anexo == 'VI') {
        if (status == 'atendido') {
            $('#' + tipo + '_VI_atendido').val(somaEFormata([tipo + 'atendido_OA', tipo + 'atendido_OD']));
        }
        if (status == 'pedido') {
            $('#' + tipo + '_VI_pedido').val(somaEFormata([tipo + '_OA', tipo + '_OD']));
        }
    }
    if (anexo == 'VII') {
        if (status == 'atendido') {
            $('#' + tipo + '_VII_atendido').val(somaEFormata([tipo + 'atendido_UD', tipo + 'atendido_UA', tipo + 'atendido_UB']));
        }
        if (status == 'pedido') {
            $('#' + tipo + '_VII_pedido').val(somaEFormata([tipo + '_UD', tipo + '_UA', tipo + '_UB']));
        }
    }
}

function escolhaFunction(campo, anexo)
{
    validaCusteioInvestimento();
    if (campo == 'C3') {
        $('#ampliacao_CD').prop('readonly', false);
        $('#reducao_CD').prop('readonly', false);
        $('#ampliacao_ID').prop('readonly', true);
        $('#reducao_ID').prop('readonly', true);
    }
    if (campo == 'I3') {
        $('#ampliacao_CD').prop('readonly', true);
        $('#reducao_CD').prop('readonly', true);
        $('#ampliacao_ID').prop('readonly', false);
        $('#reducao_ID').prop('readonly', false);
    }
    if (campo == 'R3') {
        $('#ampliacao_CD').prop('readonly', false);
        $('#reducao_CD').prop('readonly', false);
        $('#ampliacao_ID').prop('readonly', false);
        $('#reducao_ID').prop('readonly', false);
    }
    if (campo == 'C1') {
        $('#ampliacao_CA').prop('readonly', false);
        $('#reducao_CA').prop('readonly', false);
        $('#ampliacao_IA').prop('readonly', true);
        $('#reducao_IA').prop('readonly', true);
    }
    if (campo == 'I1') {
        $('#ampliacao_CA').prop('readonly', true);
        $('#reducao_CA').prop('readonly', true);
        $('#ampliacao_IA').prop('readonly', false);
        $('#reducao_IA').prop('readonly', false);
    }
    if (campo == 'R1') {
        $('#ampliacao_CA').prop('readonly', false);
        $('#reducao_CA').prop('readonly', false);
        $('#ampliacao_IA').prop('readonly', false);
        $('#reducao_IA').prop('readonly', false);
    }
}
/**
 * Função para não deixar preencher amplicação em Custeio e Investimento ao mesmo Tempo
 * para Anexos I e III
 */
function validaCusteioInvestimento()
{
    $('#ampliacao_IA').change(function() {
        $('#ampliacao_CA').val('0,00');
    });
    $('#ampliacao_CA').change(function() {
        $('#ampliacao_IA').val('0,00');
    });
    $('#reducao_IA').change(function() {
        $('#reducao_CA').val('0,00');
    });
    $('#reducao_CA').change(function() {
        $('#reducao_IA').val('0,00');
    });
    return true;
}

function somaEFormata(listaIDs)
{
    var total = 0;
    for (x in listaIDs) {
        if ('string' !== typeof listaIDs[x]) {
            continue;
        }
        var id = '#' + listaIDs[x];
        var valor = parseFloat(
            $(id).val()
                ? str_replace(['.', ','], ['', '.'], $(id).val())
                : 0
            );
        total = total + valor;
    }
    return number_format(total, 2, ',', '.');
}

function referencia(anexo)
{
    if (anexo == 'I' || anexo == 'III') {
        $('#referenciaE').css("display", "none");
        $('#referencia').css("display", "block");
    } else {
        $('#referenciaE').css("display", "block");
        $('#referencia').css("display", "none");
    }
}

function limpaAnexo(anexo)
{
    if (confirm('Deseja zerar os demais valores?')) {
        $(".zerar").val(number_format(0, 2, ',', '.'));
        $(".total").val(number_format(0, 2, ',', '.'));
    } else {
        window.location = 'progorc.php?modulo=principal/limite/formulario&acao=A&execucao=lista'
                        + '&pdlid= <?php echo $_REQUEST ['pdlid'] ?>'
                        + '&anexo=<?php echo $_REQUEST['anexo'] ?>';
    }
}

function valorTotal()
{
    var podeenviar = false;
    $('.total').each(function() {

        if ($(this).val() != '0,00') {
            podeenviar = true;
        }
    });

    if (podeenviar == false) {
        alert('Favor inserir um valor para o pedido');
    }
    return podeenviar;
}

//função para apagar arquivos anexados ao formulario
function deletarArquivo(arqid, pdlid, anexo)
{
    if (confirm("Deseja realmente excluir este arquivo permanentemente?")) {
        $.ajax({
            type: "POST",
            url: window.location,
            data: "requisicaoAjax=apagarArquivos&arqid=" + arqid,
            success: function(msg) {
                if (!msg) {
                    alert('Registro excluído com sucesso!');
                    window.location = 'progorc.php?modulo=principal/limite/formulario&acao=A&execucao=lista'
                                    + '&pdlid=' + pdlid + '&anexo=' + anexo;
                } else {
                    alert("Não foi possível excluir o pedido!");
                }
            }
        });
    }
}

jQuery(document).ready(function() {
    // -- Formatando o botão de input de arquivo
    $('input[type=file]').bootstrapFileInput();

    validaCusteioInvestimento();

    if ($("#escolha").val() == 'I1') {
        $('#ampliacao_CA').prop('readonly', true);
        $('#reducao_CA').prop('readonly', true);
    }
    if ($("#escolha").val() == 'I3') {
        $('#ampliacao_CD').prop('readonly', true);
        $('#reducao_CD').prop('readonly', true);
    }
    if ($("#escolha").val() == 'C1') {
        $('#ampliacao_IA').prop('readonly', true);
        $('#reducao_IA').prop('readonly', true);
    }
    if ($("#escolha").val() == 'C3') {
        $('#ampliacao_ID').prop('readonly', true);
        $('#reducao_ID').prop('readonly', true);
    }

    // -- Anexos
    var anexos = [
        // -- Anexo I
        'ampliacao_CA', 'ampliacao_IA', 'reducao_CA', 'reducao_IA', 'ampliacaoatendido_CA',
        'ampliacaoatendido_IA', 'reducaoatendido_CA', 'reducaoatendido_IA',
        // -- Anexo II
        'reducao_IB', 'ampliacao_IB', 'ampliacao_CB', 'reducao_CB', 'ampliacaoatendido_IB',
        'ampliacaoatendido_CB', 'reducaoatendido_IB', 'reducaoatendido_CB',
        // -- Anexo III
        'ampliacao_CD', 'ampliacao_ID', 'reducao_CD', 'reducao_ID', 'ampliacaoatendido_CD',
        'ampliacaoatendido_ID', 'reducaoatendido_CD', 'reducaoatendido_ID',
        // -- Anexo VI
        'ampliacao_XA', 'ampliacao_XD', 'reducao_XA', 'reducao_XD', 'ampliacaoatendido_XA',
        'ampliacaoatendido_XD', 'reducaoatendido_XA', 'reducaoatendido_XD',
        // -- Anexo V
        'ampliacao_PA', 'ampliacao_PD', 'reducao_PA', 'reducao_PD', 'ampliacaoatendido_PA',
        'ampliacaoatendido_PD', 'reducaoatendido_PA', 'reducaoatendido_PD',
        // -- Anexo VI
        'ampliacao_OD', 'ampliacao_OA', 'reducao_OD', 'reducao_OA', 'ampliacaoatendido_OD',
        'ampliacaoatendido_OA', 'reducaoatendido_OD', 'reducaoatendido_OA',
        // -- Anexo VII
        'ampliacao_UB', 'ampliacao_UD', 'ampliacao_UA', 'reducao_UB', 'reducao_UD', 'reducao_UA',
        'ampliacaoatendido_UB', 'ampliacaoatendido_UD', 'ampliacaoatendido_UA', 'reducaoatendido_UB',
        'reducaoatendido_UD', 'reducaoatendido_UA'
    ];
    for (x in anexos) {
        if ('string' !== typeof anexos[x]) {
            continue;
        }
        var id = '#' + anexos[x];
        $(id).val(mascaraglobal('[.###],##', $(id).val()));
    }

    var somatorios = {
        'ampliacao_I_pedido': ['ampliacao_CA', 'ampliacao_IA'],
        'reducao_I_pedido': ['reducao_CA', 'reducao_IA'],
        'ampliacao_I_atendido': ['ampliacaoatendido_CA', 'ampliacaoatendido_IA'],
        'reducao_I_atendido': ['reducaoatendido_CA', 'reducaoatendido_IA'],

        'ampliacao_II_pedido': ['ampliacao_CB', 'ampliacao_IB'],
        'reducao_II_pedido': ['reducao_CB', 'reducao_IB'],
        'ampliacao_II_atendido': ['ampliacaoatendido_CB', 'ampliacaoatendido_IB'],
        'reducao_II_atendido': ['reducaoatendido_CB', 'reducaoatendido_IB'],

        'ampliacao_III_pedido': ['ampliacao_CD', 'ampliacao_ID'],
        'reducao_III_pedido': ['reducao_CD', 'reducao_ID'],
        'ampliacao_III_atendido': ['ampliacaoatendido_CD', 'ampliacaoatendido_ID'],
        'reducao_III_atendido': ['reducaoatendido_CD', 'reducaoatendido_ID'],

        'ampliacao_IV_pedido': ['ampliacao_XA', 'ampliacao_XB'],
        'reducao_IV_pedido': ['reducao_XA', 'reducao_XB'],
        'ampliacao_IV_atendido': ['almpliacaoatendido_XA', 'ampliacaoatendido_XB'],
        'reducao_IV_atendido': ['reducaoatendido_XA', 'reducaoatendido_XB'],

        'ampliacao_V_pedido': ['ampliacao_PA', 'ampliacao_PD'],
        'reducao_V_pedido': ['reducao_PA', 'reducao_PD'],
        'ampliacao_V_atendido': ['ampliacaoatendido_PA', 'ampliacaoatendido_PD'],
        'reducao_V_atendido': ['reducaoatendido_PA', 'reducaoatendido_PD'],

        'ampliacao_VI_pedido': ['ampliacao_OA', 'ampliacao_OD'],
        'reducao_VI_pedido': ['reducao_OA', 'reducao_OD'],
        'ampliacao_VI_atendido': ['ampliacaoatendido_OA', 'ampliacaoatendido_OD'],
        'reducao_VI_atendido': ['reducaoatendido_OA', 'reducaoatendido_OD'],

        'ampliacao_VII_pedido': ['ampliacao_UD', 'ampliacao_UA', 'ampliacao_UB'],
        'reducao_VII_pedido': ['reducao_UD', 'reducao_UA', 'reducao_UB'],
        'ampliacao_VII_atendido': ['ampliacaoatendido_UD', 'ampliacaoatendido_UA', 'ampliacaoatendido_UB'],
        'reducao_VII_atendido': ['reducaoatendido_UD', 'reducaoatendido_UA', 'reducaoatendido_UB']
    };

    for (x in somatorios) {
        $('#' + x).val(somaEFormata(somatorios[x]));
    }

    $('#alterar').click(function() {
        if ($('#unicod').val() == '') {
            alert('É obrigatório o preenchimento da Unidade Orçamentária.');
            return false;
        }
    });
});
</script>
<form id="formPrincipal" onsubmit="return valorTotal();" name="formPrincipal" method="POST"
      class="form-horizontal" enctype="multipart/form-data">
<?php if ($pdlid): ?>
    <input type="hidden" name="escolha" id="escolha" value="<?php echo $pdlescolha; ?>" />
<?php endif; ?>
    <input type="hidden" name="acao" id="acao" value="salvar" />
    <input type="hidden" name="docid" id="docid" value="<?php echo $docid; ?>" />
    <input type="hidden" name="capid" id="capid" value="<?php echo $capid; ?>" />
    <input type="hidden" name="unicod" value="<?php echo $unicod; ?>" id="unidade" />
    <input type="hidden" name="foncod" value="<?php echo $foncod; ?>" id="fonte" />
    <input type="hidden" name="nrccod" value="<?php echo $nrccod; ?>" id="natureza" />
    <input type="hidden" name="valorSOFT" value="<?php echo $totalInformado; ?>" id="valorSOFT" />
    <input type="hidden" name="usucpf" value="<?php echo $usucpf; ?>" id="usucpf" />
    <input type="hidden" name="pdlid" id="pdlid" value="<?php echo $_REQUEST['pdlid'] ?>">
    <input type="hidden" name="pddid" id="pddid" value="<?php echo $pddid ?>" />

    <div class="form-group">
        <label for="inputUnidade" class="col-lg-2 control-label">Unidade Orçamentária (UO):</label>
        <div class="col-lg-10">
        <?php
        if ($unicod) {
            $sql = <<<DML
SELECT uni.unicod AS codigo,
       uni.unicod || ' - ' || unidsc AS descricao
  FROM public.unidade uni
  WHERE uni.unicod = '{$unicod}'
DML;
        } else {
            if (in_array(PFL_UO_EQUIPE_TECNICA, $perfis)) {
                $sqlUO = <<<DML
EXISTS (SELECT 1
         FROM progorc.usuarioresponsabilidade rpu
         WHERE rpu.usucpf = '%s'
           AND rpu.pflcod = %d
           AND rpu.rpustatus = 'A'
           AND rpu.unicod  = uni.unicod)
DML;
                $where[] = $whereUO = sprintf($sqlUO, $_SESSION['usucpf'], PFL_UO_EQUIPE_TECNICA);
                $whereUO = " AND {$whereUO}";
            }
            $sql = <<<DML
SELECT uni.unicod AS codigo,
       uni.unicod || ' - ' || unidsc AS descricao
  FROM public.unidade uni
  WHERE (uni.orgcod = '26000' OR uni.unicod IN('74902', '73107'))
    AND uni.unistatus = 'A'
    {$whereUO}
  ORDER BY uni.unicod
DML;
        }

        $unidadeorcamentaria = $db->pegaLinha($sql);
        $unicodDes = $unidadeorcamentaria ['descricao'];
        $unicodObras = $unidadeorcamentaria ['codigo'];

        $sqlObras = <<<DML
SELECT DISTINCT COUNT(0) AS numero
  FROM obras.obrainfraestrutura oie
    INNER JOIN entidade.entidade e ON oie.entidunidade = e.entid
  WHERE DATE_PART('days', NOW() - obrdtvistoria) > 60
    AND oie.obrpercexec < 100
    AND e.entunicod IS NOT NULL
    AND e.entunicod = '{$unicodObras}'
DML;

        $unidadeobras = $db->pegaLinha($sqlObras);
        $numeroobras = $unidadeobras ['numero'];

        if ($unicod) {
            echo '<p class="form-control-static">'.$unicodDes."</p>";
        } else {
            $unicod = $_REQUEST['unicod'];
            $db->monta_combo('unicod', $sql, 'S', 'Selecione uma Unidade', null, null, null, null, 'S', 'unicod', null, '', null, 'required="required" class="form-control chosen-select" style="width=100%;"" ', null, (isset($unicod) ? $unicod : null));
        }
        ?>
        </div>

    </div>
            <?php
        if($uggestao){
?>
	<div class="form-group">
            <label for="" class="col-lg-2 control-label">UG/Gestão:</label>
            <div class="col-lg-10">
                <p class="form-control-static"><?=$uggestao ?></p>
            </div>
	</div>
<?php
			}
    ?>
    <div class="form-group">
        <div class="col-md-2"></div>
        <div class="col-md-10">
    <?php
    $sql = "
        SELECT
            cc.ccrcod,
            dtc.dtcdotacaoatual as dotacaoatual,
            (SELECT sum(pdd.pddvaloramplicacao) FROM progorc.pedidolimite pdl INNER JOIN progorc.pedidodetalhe pdd ON(pdl.pdlid = pdd.pdlid AND pdd.ccrid = dtc.ccrid) WHERE pdl.unicod = dtc.unicod AND pdl.pdlreferencia = dtc.referencia AND EXTRACT(YEAR FROM pdldatapedido)::varchar = dtc.exercicio) AS total_ampliacao,
            (SELECT sum(pdd.pddvalorreducao) FROM progorc.pedidolimite pdl INNER JOIN progorc.pedidodetalhe pdd ON(pdl.pdlid = pdd.pdlid AND pdd.ccrid = dtc.ccrid) WHERE pdl.unicod = dtc.unicod AND pdl.pdlreferencia = dtc.referencia AND EXTRACT(YEAR FROM pdldatapedido)::varchar = dtc.exercicio) AS total_reducao,
            (SELECT sum(pdd.pddvaloramplicacao) FROM progorc.pedidolimite pdl INNER JOIN progorc.pedidodetalhe pdd ON(pdl.pdlid = pdd.pdlid AND pdd.ccrid = dtc.ccrid) INNER JOIN workflow.documento doc ON (pdl.docid = doc.docid) INNER JOIN workflow.estadodocumento esd ON (doc.esdid = esd.esdid) WHERE pdl.unicod = dtc.unicod AND pdl.pdlreferencia = dtc.referencia AND EXTRACT(YEAR FROM pdldatapedido)::varchar = dtc.exercicio AND esd.esdid = 1070) AS total_atendido_ampliacao,
            (SELECT sum(pdd.pddvalorreducao) FROM progorc.pedidolimite pdl INNER JOIN progorc.pedidodetalhe pdd ON(pdl.pdlid = pdd.pdlid AND pdd.ccrid = dtc.ccrid) INNER JOIN workflow.documento doc ON (pdl.docid = doc.docid) INNER JOIN workflow.estadodocumento esd ON (doc.esdid = esd.esdid) WHERE pdl.unicod = dtc.unicod AND pdl.pdlreferencia = dtc.referencia AND EXTRACT(YEAR FROM pdldatapedido)::varchar = dtc.exercicio AND esd.esdid = 1070) AS total_atendido_reducao
        FROM progorc.dotacaoconta dtc
        INNER JOIN progorc.contacorrente cc ON (dtc.ccrid = cc.ccrid)
        WHERE dtc.unicod = '{$unicod}'
            AND dtc.referencia = '{$pdlreferencia}'
            AND dtc.exercicio = '{$_SESSION['exercicio']}'
        GROUP BY dtc.ccrid, cc.ccrcod, dtc.dtcdotacaoatual, dtc.unicod, dtc.referencia, dtc.exercicio;
    ";
    $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO, Simec_Listagem::RETORNO_BUFFERIZADO);
    $listagem->setQuery($sql)->setCabecalho(array('Conta Corrente', 'Dotação Atual', 'Total' => array('Ampliação','Redução'), 'Total Atendido' => array('Ampliação','Redução')))->setFormOff()->addCallbackDeCampo(array('dotacaoatual', 'total_ampliacao', 'total_reducao', 'total_atendido_ampliacao', 'total_atendido_reducao'),'mascaraMoeda');
    echo montaItemAccordion(
        '<span class="glyphicon glyphicon-info-sign"></span> Quadro de Informações de Dotações', 'resumocat', $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM), array('accordionID' => 'accordion2', 'retorno' => true)
    );
    ?>
        </div>
    </div>
    <?php
    if ($esdid == STDOC_ANALISE_SPO || $esdid == STDOC_ACERTOS_SPO || $esdid == STDOC_ANALISE_SECRETARIA
            || $esdid == STDOC_ACERTOS_SECRETARIA || $esdid == STDOC_ATENDIDO || $esdid == STDOC_RECUSADO): ?>
    <div class="form-group">
        <label for="inputName" class="col-lg-2 control-label">Anexo:</label>
        <div class="col-md-10">
            <div class="btn-group" data-toggle="buttons">
                <input type="hidden" name="anexo" id="anexo" value="<?php echo $_REQUEST['anexo'] ?>"> <p class="form-control-static">Anexo <?php echo $_REQUEST['anexo'] ?></p>
            </div>
        </div>
    </div>
    <?php else: ?>
    <div class="form-group">
        <label for="inputName" class="col-lg-2 control-label">Anexo:</label>
        <div class="col-md-10">
            <div class="btn-group" data-toggle="buttons">
                <label class="btn btn-default <?php echo anexoSelecionado('I'); ?>">
                    <input type="radio" name="anexo" id="anexo" value="I" readonly="readonly"
                           onchange="myFunction(this.value);referencia(this.value);<?php echo $limpaAnexo; ?>" />Anexo I
                </label>
                <label
                    class="btn btn-default <?php echo anexoSelecionado('II'); ?>">
                    <input type="radio" name="anexo" id="anexo_I" value="II"
                           onchange="myFunction(this.value);referencia(this.value);<?php echo $limpaAnexo; ?>" />Anexo II
                </label>
                <label class="btn btn-default <?php echo anexoSelecionado('III'); ?>">
                    <input type="radio" name="anexo" id="anexo_II" value="III"
                           onchange="myFunction(this.value);referencia(this.value);<?php echo $limpaAnexo; ?>" />Anexo III
                </label>
                <label class="btn btn-default <?php echo anexoSelecionado('IV'); ?>">
                    <input type="radio" name="anexo" id="anexo_III" value="IV"
                           onchange="myFunction(this.value);referencia(this.value);<?php echo $limpaAnexo; ?>" />Anexo IV
                </label>
                <label class="btn btn-default <?php echo anexoSelecionado('V'); ?>">
                    <input type="radio" name="anexo" id="anexo_V" value="V"
                           onchange="myFunction(this.value);referencia(this.value);<?php echo $limpaAnexo; ?>" />Anexo V
                </label>
                <label class="btn btn-default <?php echo anexoSelecionado('VI'); ?>">
                    <input type="radio" name="anexo" id="anexo_VI" value="VI"
                           onchange="myFunction(this.value);referencia(this.value);<?php echo $limpaAnexo; ?>" />Anexo VI
                </label>
                <label class="btn btn-default  <?php echo anexoSelecionado('VII'); ?>">
                    <input type="radio" name="anexo" id="anexo_VII" value="VII"
                           onchange="myFunction(this.value);referencia(this.value);<?php echo $limpaAnexo; ?>" />Anexo VII
                </label>
            </div>
        </div>
    </div>
<?php endif; ?>
    <div class="form-group" id="div_I" style="display:<?php echo mostrarAnexo('I'); ?>">
        <label for="inputJustificativa" class="col-lg-2 control-label"></label>
        <div class="col-lg-10">
        <?php recuperaConta('I', $esdid, $pdlescolha); ?>
        </div>
    </div>
    <div class="form-group" id="div_II" style="display:<?php echo mostrarAnexo('II'); ?>">
        <label for="inputJustificativa" class="col-lg-2 control-label"></label>
        <div class="col-lg-10">
        <?php recuperaConta('II', $esdid, $pdlescolha); ?>
        </div>
    </div>
    <div class="form-group" id="div_III" style="display:<?php echo mostrarAnexo('III'); ?>">
        <label for="inputJustificativa" class="col-lg-2 control-label"></label>
        <div class="col-lg-10">
        <?php recuperaConta('III', $esdid, $pdlescolha); ?>
        </div>
    </div>
    <div class="form-group" id="div_IV" style="display:<?php echo mostrarAnexo('IV'); ?>">
        <label for="inputJustificativa" class="col-lg-2 control-label"></label>
        <div class="col-lg-10">
        <?php recuperaConta('IV', $esdid, $pdlescolha); ?>
        </div>
    </div>
    <div class="form-group" id="div_V" style="display:<?php echo mostrarAnexo('V'); ?>">
        <label for="inputJustificativa" class="col-lg-2 control-label"></label>
        <div class="col-lg-10">
        <?php recuperaConta('V', $esdid, $pdlescolha); ?>
        </div>
    </div>
    <div class="form-group" id="div_VI" style="display:<?php echo mostrarAnexo('VI'); ?>">
        <label for="inputJustificativa" class="col-lg-2 control-label"></label>
        <div class="col-lg-10">
        <?php recuperaConta('VI', $esdid, $pdlescolha); ?>
        </div>
    </div>
    <div class="form-group" id="div_VII" style="display:<?php echo mostrarAnexo('VII'); ?>">
        <label for="inputJustificativa" class="col-lg-2 control-label"></label>
        <div class="col-lg-10">
        <?php recuperaConta('VII', $esdid, $pdlescolha); ?>
        </div>
    </div>
<?php
if ($esdid == STDOC_ANALISE_SPO || $esdid == STDOC_ACERTOS_SPO || $esdid == STDOC_ATENDIDO) {
    if (in_array(PFL_CGO_EQUIPE_ORCAMENTARIA, $perfis) || 1 == $_SESSION['superuser']): ?>
    <div class="form-group">
        <label for="inputName" class="col-lg-2 control-label"></label>
        <div class="col-md-10">
            <div class="btn-group" data-toggle="buttons">
                <label class="btn btn-default" onclick="teste('<?php echo $pdlescolha; ?>');">
                    <input type="checkbox" name="alterar" id="alterar" value="D" onclick="teste('<?php echo $pdlescolha; ?>');" />Alterar
                </label>
            </div>
        </div>
    </div>
<?php
    endif;
}
?>
<?php if (mostrarRefIeIII(true)): ?>
    <div class="form-group" id ="referenciaE" style="display:block; clear:both;">
        <label for="inputName" class="col-lg-2 control-label">Referência:</label>
        <div class="col-md-10">
            <div class="btn-group" data-toggle="buttons">
                <label class="btn btn-default <?php echo referenciaSelecionada($pdlreferencia, 'D', true); ?>">
                    <input type="radio" name="pdlreferencia" id="pdlreferencia_D" value="D" />Demais Dotações
                </label>
            </div>
        </div>
    </div>
<?php endif; ?>
    <div class="form-group" id="referencia" style="display:<?php echo mostrarRefIeIII(); ?>">
        <label for="inputName" class="col-lg-2 control-label">Referência:</label>
        <div class="col-md-10">
            <div class="btn-group" data-toggle="buttons">
                <label class="btn btn-default <?php echo referenciaSelecionada($pdlreferencia, 'E'); ?>">
                    <input type="radio" name="pdlreferencia" id="pdlreferencia_E" value="E" />Emendas
                </label>
                <label class="btn btn-default <?php echo referenciaSelecionada($pdlreferencia, 'S'); ?>">
                    <input type="radio" name="pdlreferencia" id="pdlreferencia_S" value="S" />Superávit
                </label>
                <label class="btn btn-default <?php echo referenciaSelecionada($pdlreferencia, 'D', true); ?>">
                    <input type="radio" name="pdlreferencia" id="pdlreferencia_D"
                           value="D" />Demais Dotações
                </label>
            </div>
        </div>
    </div>
    <div class="form-group">
        <label for="inputJustificativa" class="col-lg-2 control-label">Justificativa:</label>
        <div class="col-lg-10">
            <?php
            $complemento = '';
            if ($captipo == 'C') {
                $complemento = "readonly placeholder='De acordo'";
            }
            ?>
            <textarea class="form-control" rows="3" id="pdljustificativa" name="pdljustificativa"
                      maxlength="500" <?php echo $complemento; ?> required><?php echo $pdljustificativa; ?></textarea>
        </div>
    </div>
    <div class="form-group">
        <label for="file" class="col-lg-2 control-label">Anexar Arquivos:</label>
        <div class="col-lg-10">
            <input type="file" name="file" id="file" class="btn btn-primary start" title="Selecionar arquivo" />
        </div>
    </div>
    <div class="form-group">
        <label for="file" class="col-lg-2 control-label">Arquivos Anexados:</label>
        <div class="col-lg-10">
            <?php
            // -- Query de listagem de arquivos anexos
            $sql = "SELECT DISTINCT
                        arq.arqid as codarq,
                        ang.pdlid,
                        ane.anxcod,
                        '<a href=\"progorc.php?modulo=principal/limite/formulario&acao=A&download=S&arqid='|| arq.arqid ||'\">' || arq.arqnome || '.' || arq.arqextensao || '</a>' as arquivo,
                        arq.arqtamanho,
                        to_char(arq.arqdata, 'DD/MM/YYYY') || ' ' || arq.arqhora as arqdata,
                        usu.usunome
                    FROM
                        progorc.anexogeral ang
                    INNER JOIN
                        progorc.pedidodetalhe pde ON pde.pdlid = ang.pdlid
                    INNER JOIN
                        progorc.contacorrente ccr ON ccr.ccrid = pde.ccrid
                    INNER JOIN
                        progorc.anexo ane ON ane.anxcod = ccr.anxcod
                    INNER JOIN
                        public.arquivo arq on ang.arqid = arq.arqid
                    INNER JOIN
                        seguranca.usuario usu on usu.usucpf = arq.usucpf
                    WHERE
                        ang.angtip is null
                    AND ang.pdlid = %d";

            $sql = sprintf($sql, $pdlid);
            $cabecalho = array("Descrição", "Tamanho(bytes)", "Data inclusão", "Responsável");
            echo '<br>';

            $listagem = new Simec_Listagem ();
            $listagem->setCabecalho($cabecalho);
            $listagem->esconderColunas(array('pdlid','anxcod'));
            $listagem->setQuery($sql);
            /* Apenas o Super Usuário ou a CGO podem apagar a Solicitação */
            if (in_array(PFL_SUPER_USUARIO, $perfis) || in_array(PFL_UO_EQUIPE_TECNICA, $perfis)) {
                $listagem->addAcao('delete',array('func' => 'deletarArquivo','extra-params'=>array('pdlid','anxcod')
                ));
            }
            $listagem->esconderColuna('troca');


            if (false === $listagem->render()) { ?>
                <div class="alert alert-info col-md-4 col-md-offset-4 text-center">Nenhum registro encontrado</div>
            <?php } ?>
        </div>
    </div>
    <div class="form-group">
        <label for="usunome" class="col-lg-2 control-label">Dados do Usuário:</label>
        <div class="col-lg-10" style="margin-top: 7px"><?php echo $nome; ?></div>
    </div>
    <div class="form-group">
        <label for="usuemail" class="col-lg-2 control-label">Email:</label>
        <div class="col-lg-3">
            <input name="usuemail" id="usuemail" class="form-control"
                   type="text" value="<?php echo $usuemail ?>"
                   <?php
                   if ($captipo == 'C') {
                       echo "readonly";
                   }
                   ?>
                   required="required" size="6" />
        </div>
        <label for="usufoneddd" class="col-lg-1 control-label">DDD:</label>
        <div class="col-lg-1">
            <input name="usufoneddd" id="usufoneddd" class="form-control"
                   type="text" value="<?php echo $ddd ?>" maxlength="2"
                   <?php
                   if ($captipo == 'C') {
                       echo "readonly";
                   }
                   ?>
                   required="required" size="6" />
        </div>
        <label for="usufonenum" class="col-lg-1 control-label">Telefone:</label>
        <div class="col-lg-3">
            <input name="usufonenum" id="usufonenum" class="form-control"
                   type="text" value="<?php echo $telefone ?>" maxlength="10"
                   <?php
                   if ($captipo == 'C') {
                       echo "readonly";
                   }
                   ?>
                   required="required" size="6" />
        </div>
    </div>
    <div id="botoesHabilitados">
    <?php if ($habilitado && ($esdid != STDOC_RECUSADO)) { ?>
        <div class="form-group" align="center">
            <div class="col-lg-10">
                <button class="btn btn-danger" type="button" id="voltar"
                        onclick="voltarLista()">Voltar</button>
                        <?php
                        $gravar = '';
                        //ver($gravar,$perfis,$esdid,d);
                        //if ((in_array(PFL_UO_EQUIPE_TECNICA, $perfis)&& ($esdid == null || $esdid == STDOC_ACERTOS_SPO || $esdid == STDOC_ACERTOS_SECRETARIA))&& (in_array(PFL_SUPER_USUARIO, $perfis))) {
                        if (in_array(PFL_SUPER_USUARIO, $perfis)) {
                            $gravar = '<button class="btn btn-info" id="atualizar">Gravar</button>';
                        }
                        if (in_array(PFL_CGO_EQUIPE_ORCAMENTARIA, $perfis)) {
                            // if ($esdid == STDOC_ANALISE_SPO) {
                            $gravar = '<button class="btn btn-info" id="atualizar">Gravar</button>';
                            // }
                        }

                        if (in_array(PFL_UO_EQUIPE_TECNICA, $perfis)) {
                            if ($esdid == null || $esdid == '' || $esdid == STDOC_NAO_ENVIADO || $esdid == STDOC_ACERTOS_SPO || $esdid == STDOC_ACERTOS_SECRETARIA) {
                                $gravar = '<button class="btn btn-info" id="atualizar">Gravar</button>';
                            }
                        } echo $gravar;
                        ?>
            </div>
        </div>
    <?php } else { ?>
        <div class="form-group" align="center">
            <button class="btn btn-danger" type="button" id="voltar" onclick="voltarLista()">Voltar</button>
            <br><br><center>Formulário desabilitado para edição.</center>
        </div>
    <?php } ?>
    </div>
</form>

