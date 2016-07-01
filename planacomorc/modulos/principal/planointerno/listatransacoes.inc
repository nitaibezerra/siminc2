<?php
/**
 * Lista de transações de criação de PIs.
 * $Id: listatransacoes.inc 102774 2015-09-23 20:29:30Z werteralmeida $
 */

/**
 * Funções de apoio ao remanejamento de PI.
 * @see _funcoesremanejavalorpi.php
 */
include_once '_funcoesremanejavalorpi.php';

if ($_REQUEST['requisicao']) {
    switch ($_REQUEST['requisicao']) {
        case 'apagarTransacao':
            if ($_REQUEST['requisicao']($_REQUEST)) {
                $msg = 'Transação excluída com sucesso.';
            } else {
                $msg = 'Não foi possível excluir a transação.';
            }
            ?>
            <script type="text/javascript" language="javascript">
                alert('<?php echo $msg; ?>');
                location.href = 'planacomorc.php?modulo=principal/planointerno/listatransacoes&acao=A';
            </script>
            <?php
            break;
        default: $_REQUEST['requisicao']($_REQUEST);
    }
    exit;
}

//Chamada de programa
include APPRAIZ . "includes/cabecalho.inc";

//?>
<style type="text/css">
.listagem img{cursor:pointer;border:0}
</style>
<script src="./js/planacomorc.js"></script>
<script>
    function editRegistro(scpid) {
        location.href = '/planacomorc/planacomorc.php?modulo=principal/planointerno/cadastro&acao=A&scpid=' + scpid;
    }
        function reload()
    {
        window.location = 'planacomorc.php?modulo=principal/planointerno/listatransacoes&acao=A';
    }
    function deleteRegistro(scpid)
    {
        if (confirm('Deseja realmente excluir esta transação?')) {
            $('#scpid').val(scpid);
            $('#requisicao').val('apagarTransacao');
            $('#formulario').submit();
        }
    }
    jQuery(document).ready(function() {
        // -- Ação do botão buscar
        $('#buscar').click(function() {
            var uri = "planacomorc.php?modulo=principal/planointerno/listatransacoes&acao=A"
                    + '&buscalivre=' + $('#buscalivre').val();

            var tipoTransacao = $('input[name=tipoTransacao]:checked').val();
            //if ('todos' != wrfstatus) {
            uri += '&tipoTransacao=' + tipoTransacao;

            window.location = uri;
            //return false;
        });
    });
</script>
<?php
//monta_titulo("Plano Interno", 'Transações de Solicitação de Cadastro de PI');
/* Seleciona o tipo de transação a ser filtrado, default 'S' (Solicitações) */

if ($_REQUEST['tipoTransacao'] && $_REQUEST['tipoTransacao']!= 'undefined' ) {
    $tipoTransacao = $_REQUEST['tipoTransacao'];
}else{
    $tipoTransacao = 'S';
}
if ($_REQUEST['buscalivre']) {
    $whereBuscaLivre = "AND(";
    $whereBuscaLivre .= " sba.sbacod ilike '%" . $_REQUEST['buscalivre'] . "%'";
    $whereBuscaLivre .= " OR UPPER(public.removeacento(scp.scptitulo)) ilike '%" . removeAcentos(str_replace("-", "", $_REQUEST['buscalivre'])) . "%'";
    $whereBuscaLivre .= " OR UPPER(public.removeacento(usu.usunome)) ilike '%" . removeAcentos(str_replace("-", "", $_REQUEST['buscalivre'])) . "%'";
    $whereBuscaLivre .= ")";
}
if ($tipoTransacao == 'S') {
    $colunaTransacaoEfetivada = ",CASE WHEN scp.scpprocessado THEN 'SIM' ELSE 'NÃO' END as processado";
    $btnExcluir = <<<BTN
&nbsp;<a onclick="apagarTransacao(' || scp.scpid || ');"><img src="/imagens/excluir.gif" title="Apagar" /></a>
BTN;
}

// -- Construção do filtro de usuário responsábilidade,
// -- baseado nas regras definidas pela SPO.
//$filtroUsuResponsabilidade = filtroUsuarioResponsabilidade(
//    TRANSACAO_CRIACAO_PI
//);
?>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<div class="row col-md-12">
    <ol class="breadcrumb">
        <li><a href="<?php echo MODULO; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
        <li class="active">Listar Transações de Solicitação de Cadastro de PI</li>
    </ol>
</div>
<div class="col-lg-12">
    <div class="well">
        <fieldset>
            <form id="formulario" name="formulario" method="post" action="" class="form-horizontal">
                   <input type="hidden" name="scpid" id="scpid" />
                <input type="hidden" name="requisicao" id="requisicao" />

               <div class="form-group">
                    <label for="inputUnidade" class="col-lg-2 control-label">Buscar:</label>
                    <div class="col-lg-10">
                        <input type="text" class="CampoEstilo normal form-control" name="buscalivre" id="buscalivre" />
                    </div>
                </div>
                <div class="form-group">
                    <label for="inputUnidade" class="col-lg-2 control-label">Situação:</label>
                    <div class="col-lg-10">
                        <div class="btn-group" data-toggle="buttons">
                                <label class="btn btn-default <?php if($_REQUEST['tipoTransacao'] == 'S' || !$_REQUEST['tipoTransacao'] ||$_REQUEST['tipoTransacao'] == 'undefined'){echo 'active';}?> ">
                                <input type="radio" name="tipoTransacao"
                                       id="tipoTransacaoS"
                                       value="S" /> Solicitação de Cadastro
                            </label>
                            <label class="btn btn-default <?php if($_REQUEST['tipoTransacao'] == 'E'){echo 'active';}?> ">
                                <input type="radio" name="tipoTransacao"
                                       id="tipoTransacaoE"
                                       value="E" /> Cadastro Efetivado
                            </label>
                        </div>
                    </div>
                </div>
                <div class="form-group">
                    <div class="col-lg-10 col-lg-offset-2">
                        <button class="btn btn-warning" type="reset" onclick="reload()">Limpar</button>
                        <button class="btn btn-primary" id="buscar" type="button" onclick="enviar()">Buscar</button>
                    </div>
                </div>
            </form>
        </fieldset>
    </div>
<?php
$strSqlGrid = <<<DML
SELECT scp.scpid AS acao,
       uni.unidsc,
       scptitulo as titulo,
       TO_CHAR(scp.scpdata,'dd/mm/yyyy') AS datatransacao,
       usu.usunome as usuario,
       sd.valorsolicitado{$colunaTransacaoEfetivada}
  FROM planacomorc.solicitacaocriacaopi scp
    LEFT JOIN seguranca.usuario usu ON usu.usucpf=scp.usucpf
    LEFT JOIN (SELECT scpid,
                      SUM(spdvalorsolicitado) AS valorsolicitado
                 FROM planacomorc.solicitacaopidotacao
                 GROUP BY scpid) AS sd ON sd.scpid = scp.scpid
    LEFT JOIN public.unidade uni ON uni.unicod = scp.unicod
    LEFT JOIN monitora.pi_subacao sba USING(sbaid)
  WHERE scp.tipotransacao = '{$tipoTransacao}' {$whereBuscaLivre} {$filtroUsuResponsabilidade}
    AND NOT(scp.scpprocessado)
    AND scp.scpstatus = 'A'
  ORDER BY scp.scpprocessado,
           scp.scpdata DESC
DML;

$listagem = new Simec_Listagem();
if ($tipoTransacao == 'S') {
    $listagem->setCabecalho(array(
        "Unidade",
        "Titulo",
        "Data da Operação",
        "Usuário",
        "Valor Total (R$)",
        "Processada"));
} else {
    $listagem->setCabecalho(array(
        "Unidade",
        "Titulo",
        "Data da Operação",
        "Usuário",
        "Valor Total (R$)"));
}
$listagem->setQuery($strSqlGrid);
$listagem->setAcoes(array(
    'edit' => 'editRegistro',
    'delete' => 'deleteRegistro'
));
$listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
?>
<?php if (false === $listagem->render()): ?>
    <div class="alert alert-info col-md-4 col-md-offset-4 text-center">
        Nenhum registro encontrado
    </div>
<?php endif; ?>
</div>