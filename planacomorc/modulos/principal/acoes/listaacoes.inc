<?php
/**
 * Arquivo de listagem de localizadores
 * $Id: listaacoes.inc 92936 2015-01-21 13:40:14Z maykelbraz $
 */
include_once '_funcoesacoes.php';
include_once(APPRAIZ . 'includes/funcoesspo.php');
include_once (APPRAIZ . 'includes/library/simec/Listagem.php');

if (!isset($_REQUEST['percod'])) {
    if (isset($_SESSION['planacomorc']['percod'])) {
        $_REQUEST['percod'] = $_SESSION['planacomorc']['percod'];
    }
}
if (!isset($_REQUEST['acacod'])) {
    if (isset($_SESSION['planacomorc']['acacod'])) {
        $_REQUEST['acacod'] = $_SESSION['planacomorc']['acacod'];
    }
}

if ($_REQUEST['orgcod']) {
    $orgcod = $_REQUEST['orgcod'];
    $_SESSION['planacomorc']['orgcod'] = $orgcod;
} else {
    $orgcod = $_SESSION['planacomorc']['orgcod'];
}
?>
<style type="text/css">
.marcado{background-color:#C1FFC1!important}
.remover{display:none}
</style>
<script type="text/javascript">
$(document).ready(function(){
    $('select[name="id_acao_programatica"]').change(function() {
        window.location = "planacomorc.php?modulo=principal/acoes/gerenciarunidades&acao=A&aba=listaacoes&acacod=" + this.value +"<?=$_REQUEST['percod']?'&percod='.$_REQUEST['percod']:''?>";
    });
    $('#percod').change(function() {
        window.location = "planacomorc.php?modulo=principal/acoes/gerenciarunidades&acao=A&aba=listaacoes<?=$_REQUEST['acacod']?'&acacod='.$_REQUEST['acacod']:''?>&percod=" + this.value;
    });
});

function monitorarLocalizador(id_acao_programatica, loccod, percod) {
    window.location = 'planacomorc.php?modulo=principal/acoes/gerenciarunidades&acao=A&aba=monitoraracao'
            + '&id_acao_programatica=' + id_acao_programatica
            + '&loccod=' + loccod
            + '&percod=' + percod;
}

function consultarAcao(id,acacod,prgcod,loccod,unicod) {
    var url = "planacomorc.php?modulo=principal/acoes/gerenciarunidades&acao=A&aba=listaacoes&requisicao=consultarAcao&id_acao_programatica="+id+"&prgcod="+prgcod +"&acacod="+acacod+"&loccod="+loccod+"&unicod=" + unicod;
    $.post(url, function(html) {
        $('#modal-confirm .modal-body p').html(html);
        $('.modal-dialog').css('width', '90%');
        $('#modal-confirm .modal-title').html('Dados da Ação');
        $('#modal-confirm .btn-primary').remove();
        $('#modal-confirm .btn-default').html('Fechar');
        $('.modal-dialog').show();
        $('#modal-confirm').modal();
    });
}

function ajaxatualizar(params, iddestinatario) {
    $.ajax({
        type: "POST",
        url: window.location.href,
        data: params,
        async: false,
        success: function(html) {
            if (iddestinatario != '') {
                document.getElementById(iddestinatario).innerHTML = html;
            }
        }
    });
}

/**
 * Abre uma popup com uma busca ao log de envio daquela acaoprogramatica.
 * @param int idAcaoProgramatica ID da ação programática;
 */
function exibirLogEnvio(idAcaoProgramatica)
{
    $.post(window.location, {requisicao: 'exibirLogEnvio',id_acao_programatica: idAcaoProgramatica}, function(content){
        alert(content);
    });
}
</script>
<div id="modalAcao" style="display: none;" title="Detalhes da ação"></div>
<div id="modalLog" style="display: none;" title="Log de Envio ao SIOP"></div>
<?php
$perfis = pegaPerfilGeral();
if($_REQUEST['percod']){
   $periodo = $_REQUEST['percod'];
}else{
   $periodo = recuperaPeriodo('atual');
}
if (in_array(PFL_COORDENADORACAO, $perfis)) {
    $whpfl[] = "apr.id_acao_programatica IN(SELECT id_acao_programatica
                                                  FROM planacomorc.usuarioresponsabilidade
                                                  WHERE rpustatus='A'
                                                    AND usucpf='" . $_SESSION ['usucpf'] . "'
                                                    AND pflcod='" . PFL_COORDENADORACAO . "' AND id_periodo_referencia = {$periodo})";
}

if (in_array(PFL_VALIDADORACAO, $perfis)) {
    $whpfl[] = "apr.id_acao_programatica IN(SELECT id_acao_programatica
                                                  FROM planacomorc.usuarioresponsabilidade
                                                  WHERE rpustatus='A'
                                                    AND usucpf='" . $_SESSION ['usucpf'] . "'
                                                    AND pflcod='" . PFL_VALIDADORACAO . "' AND id_periodo_referencia = {$periodo})";
}

if (in_array(PFL_VALIDADOR_SUBSTITUTO, $perfis)) {
    $whpfl[] = "apr.id_acao_programatica IN(SELECT id_acao_programatica
                                                  FROM planacomorc.usuarioresponsabilidade
                                                  WHERE rpustatus='A'
                                                    AND usucpf='" . $_SESSION['usucpf'] . "'
                                                    AND pflcod='" . PFL_VALIDADOR_SUBSTITUTO . "' AND id_periodo_referencia = {$periodo})";
}

if (in_array(PFL_CONSULTA, $perfis)) {
    $SQLParcial = <<<DML_PARTIAL
      EXISTS (SELECT 1
                FROM planacomorc.usuarioresponsabilidade rpu
                WHERE rpu.id_acao_programatica = apr.id_acao_programatica
                  AND rpu.id_periodo_referencia = {$periodo}
                  AND rpu.usucpf = '{$_SESSION['usucpf']}'
                  AND rpu.pflcod = %d
                  AND rpu.rpustatus = 'A')
DML_PARTIAL;
    $whpfl[] = sprintf($SQLParcial, PFL_CONSULTA);
}

if ($_REQUEST['id_acao_programatica']) {
    $whpfl[] = "apr.id_acao_programatica = '" . $_REQUEST['id_acao_programatica'] . "'";
}

if ($whpfl) {
    $wh = " AND (" . implode(" OR ", $whpfl) . ")";
}
?>
<div class="col-md-12">
    <div class="well">
        <div class="form-horizontal">
            <div class="form-group">
                <div class="col-md-2 text-right">
                    <label class="control-label">Unidade Orçamentária (UO)</label>
                </div>
                <div class="col-md-10">
                    <p class="form-control-static">
                    <?
                    $sql = "select descricao from planacomorc.orgao  where codigo = '{$_SESSION['planacomorc']['orgcod']}'";
                    $descricao = $db->pegaUm($sql);
                    echo $_SESSION['planacomorc']['orgcod'] . ' - '.$descricao;
                    ?>
                    </p>
                </div>
            </div>
            <div class="form-group">
                <input type=hidden name="orgcod" value="<?php echo $_SESSION['planacomorc']['orgcod']; ?>" />
                <div class="col-md-2 text-right">
                    <label class="control-label" for="id_acao_programatica">
                        Ação
                    </label>
                </div>
                <div class="col-md-10">
                    <?php
                    $sql_f_acoes = "
                        SELECT
                            DISTINCT
                            apr.id_acao_programatica as codigo,
                            aca.codigo || ' - ' || dae.titulo as descricao,
                            aca.codigo AS codigo_,
                            dae.titulo as titulo_
                        FROM planacomorc.acao aca
                        JOIN planacomorc.acao_programatica apr ON apr.id_acao = aca.id_acao
                        JOIN planacomorc.dados_acao_exercicio dae ON dae.id_acao = aca.id_acao
                            AND dae.id_exercicio=apr.id_exercicio
                        JOIN planacomorc.orgao org ON org.id_orgao = apr.id_orgao
                        JOIN planacomorc.localizador_programatica lpr USING(id_acao_programatica)
                        LEFT JOIN planacomorc.quantitativo_sof qts USING(id_acao_programatica, loccod)
                        WHERE org.codigo='" . $_SESSION['planacomorc'] ['orgcod'] . "'
                            AND apr.id_exercicio='" . $_SESSION['exercicio'] . "'
                            AND qts.quantidade_fisico != 0{$wh}
                        ORDER BY aca.codigo, dae.titulo";
                    $valor = $_REQUEST['acacod'];
                    $opcoesAdicionais = array();
                    #ver($sql_f_acoes);
                    $v = $db->carregar($sql_f_acoes);
                    if(!$v)
                        $v = array();

                    inputCombo('id_acao_programatica', $v, $valor, 'acacod', $opcoesAdicionais);
                    ?>
                </div>
            </div>
            <div class="form-group">
                <div class="col-md-2 text-right">
                    <label class="control-label" for="percod">
                        Período
                    </label>
                </div>
                <div class="col-md-10">
                    <?php
                    $sql_f_periodo = "
 						SELECT
 							id_periodo_referencia AS codigo,
			               	titulo || ' : ' || to_char(inicio_validade,'DD/MM/YYYY') ||' a ' || to_char(fim_validade,'DD/MM/YYYY') as descricao
			        	FROM planacomorc.periodo_referencia p
			        	WHERE id_exercicio = '" . $_SESSION['exercicio'] . "'
			        	ORDER BY id_periodo_referencia desc
    				";
                    $valor = $_REQUEST['percod'];
                    $opcoesAdicionais = array();
                    inputCombo('percod', $sql_f_periodo, $valor, 'percod', $opcoesAdicionais);
                    ?>
                </div>
            </div>
        </div>
    </div>
    <?php
    if($_REQUEST['acacod'])
        $where = "AND apr.id_acao_programatica = '{$_REQUEST['acacod']}'";
    $sql = "
    	SELECT DISTINCT
            apr.id_acao_programatica,
            aca.codigo as acacod,
            org.codigo as unicod,
            lpr.loccod,
            aac.id_periodo_referencia as percod,
            pro.codigo AS programa,
            aca.codigo || ' - '  || dae.titulo AS acatitulo,
            lpr.loccod || ' - ' || lpr.locdescricao as localizador,
            TO_CHAR(aac.instante_alteracao,'dd/mm/YYYY HH24:MI') AS instante_alteracao,
            esddsc,
            ultimoretornosiop AS retorno_siop
        FROM planacomorc.acao aca
        JOIN planacomorc.acao_programatica apr ON apr.id_acao = aca.id_acao
        JOIN planacomorc.dados_acao_exercicio dae ON dae.id_acao = aca.id_acao
            AND dae.id_exercicio=apr.id_exercicio
        INNER JOIN planacomorc.localizador_programatica lpr USING (id_acao_programatica)
        LEFT JOIN planacomorc.quantitativo_sof qsf ON (qsf.loccod = lpr.loccod AND qsf.id_acao_programatica = apr.id_acao_programatica)
        JOIN planacomorc.programa pro ON pro.id_programa = aca.id_programa
        JOIN planacomorc.orgao org ON org.id_orgao = apr.id_orgao
        LEFT JOIN planacomorc.acompanhamento_acao aac ON (aac.id_localizador = lpr.id_localizador
            AND aac.id_acao_programatica = lpr.id_acao_programatica )
        LEFT JOIN workflow.documento doc ON(doc.docid = aac.docid)
        LEFT JOIN workflow.estadodocumento esd USING(esdid)
        WHERE org.codigo='{$_SESSION['planacomorc']['orgcod']}'
            AND qsf.quantidade_fisico != 0
            AND dae.id_produto IS NOT NULL
            AND apr.id_exercicio='{$_SESSION['exercicio']}'
            AND aac.id_periodo_referencia = {$_REQUEST['percod']}{$wh}
            {$where}
        ORDER BY 3";
    $cabecalho = array("Programa", "Ação", "Localizador", "Última atualização", 'Estado Documento', "Status no SIOP");

    $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
    $listagem->setCabecalho($cabecalho);
    $listagem->setQuery($sql);
    $listagem->esconderColuna(array('loccod', 'percod','acacod','unicod'));
    $listagem->addCallbackDeCampo('retorno_siop', 'statusNoSIOP')
        ->addCallbackDeCampo('esddsc', 'descEstadoDocumento');
    $listagem->addCallbackDeCampo(array('acatitulo', 'localizador' ), 'alinhaParaEsquerda');
    $listagem->turnOnPesquisator();
    $listagem->setAcoes(
        array(
            'edit' => array('func' => 'monitorarLocalizador', 'extra-params' => array('loccod', 'percod')),
            'view' => array('func' => 'consultarAcao', 'extra-params' => array('acacod','programa','loccod','unicod'))));
    $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
    $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);

    function statusNoSIOP($status, $dados, $id) {
        $html = <<<HTML
<span class="glyphicon glyphicon-%s" style="color:%s" %s></span>
HTML;
        switch ($status) {
            case 'E':
                return sprintf($html, 'thumbs-down', '#D9534F;cursor:pointer', ' onclick="exibirLogEnvio(' . $id . ')"');
            case 'S':
                return sprintf($html, 'thumbs-up', '#5CB85C', ' onclick="exibirLogEnvio(' . $id . ')"');
            default:
                return sprintf($html, 'minus', '#F0AD4E', '');
        }
    }

    function descEstadoDocumento($esddsc)
    {
        if ($esddsc) {
            return $esddsc;
        }

        return '<center>-</center>';
    }
    ?>
</div>