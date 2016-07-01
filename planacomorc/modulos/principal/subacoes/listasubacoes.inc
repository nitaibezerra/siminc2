<?php
die("ok");
/**
 * Listagem de subações.
 *
 * @package SiMEC
 * @subpackage planejamento-acompanhamento-orcamentario
 * @version $Id: listasubacoes.inc 97960 2015-05-28 18:43:57Z mariluciaqueiroz $
 */

/**
 *
 */
include_once '_funcoessubacoes.php';

if ($_REQUEST['requisicao']) {
    $_REQUEST['requisicao']($_REQUEST);
    exit;
}

//Chamada de programa
include APPRAIZ . "includes/cabecalho.inc";
require APPRAIZ . 'includes/library/simec/Listagem.php';
?>

<script src="./js/planacomorc.js"></script>
<script language="javascript" type="text/javascript" src="../includes/webservice/cpf.js" /></script>
<script>
/* Manuais */
$(document).ready(function()
{
	$('#btnManualPlanejamentoAcompanhamento').click(function() {
	    window.open('../planacomorc/Manual_SIMEC_Planejamento_e_Acompanhamento_Orcamentario_2013.pdf');
	});
	$('#btnAcompanhamentoAcoes').click(function() {
	    window.open('/planacomorc/planacomorc.php?modulo=principal/acoes/listaunidades&acao=A');
	});
});
</script>
<section class="col-md-12">
    <ol class="breadcrumb">
        <li><a href="/planacomorc/planacomorc.php?modulo=inicio&acao=C"><?=$_SESSION['sisdsc']; ?></a></li>
        <li class="">Subações <?=$_SESSION['exercicio'];?></li>
        <li class="active">Acompanhamento</li>
    </ol>
</section>
<?php
// -- Consultando dados do período de referência e validando se foi encontrado algum para o exercicio selecionado
$sqlPeriodos = "
    SELECT
            id_periodo_referencia AS codigo,
    titulo || ' : ' || TO_CHAR(inicio_validade, 'DD/MM/YYYY') ||' a ' || TO_CHAR(fim_validade, 'DD/MM/YYYY') AS descricao
    FROM planacomorc.periodo_referencia p
    WHERE id_exercicio = '{$_SESSION['exercicio']}'
    ORDER BY id_periodo_referencia DESC
";
    if (!($periodos = $db->carregar($sqlPeriodos))){
    ?>
        <script language="JavaScript" type="text/javascript">
            alert('Não existem períodos de referência cadastrados para o exercício selecionado(<?= $_SESSION['exercicio'] ?>).');
            window.history.back();
        </script>
    <?php
        die();
    }
    ?>
<style>
.marcado {background-color: #C1FFC1 !important;}
.remover {display: none;}
</style>
<script>
    $(document).ready(function()
    {
    	pesquisaRapida('textFind');
    });

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

    function gerenciarSubacao(id_subacao) {
        window.location = 'planacomorc.php?modulo=principal/subacoes/listasubacoes&acao=A&requisicao=carregarSubacao&id_subacao=' + id_subacao
                        + '&id_periodo_referencia=' + $('#id_periodo_referencia').val();
        ;
    }

    function selecionarPeriodo(id_periodo_referencia){
        window.location = 'planacomorc.php?modulo=principal/subacoes/listasubacoes&acao=A&id_periodo_referencia='+id_periodo_referencia;
    }
    function gerenciarMonitorInterno(orgcod) {
        ajaxatualizar('requisicao=gerenciarMonitorInterno&orgcod=' + orgcod, 'modalMonitorInterno');

        jQuery("#modalMonitorInterno").dialog({
            draggable: true,
            resizable: true,
            width: 800,
            height: 600,
            modal: true,
            close: function() {
            }
        });
    }

    function validarMonitor() {
        var p = 'requisicao=gravarMonitorInterno';

        if (jQuery('#moncpf').val() == '') {
            alert('Preencha um cpf');
            return false;
        }

        jQuery('#moncpf').val(mascaraglobal('###.###.###-##', jQuery('#moncpf').val()))
        if (!validar_cpf(jQuery('#moncpf').val())) {
            alert('CPF inválido');
            return false;
        }
        p += '&moncpf=' + jQuery('#moncpf').val();

        if (jQuery('#monnome').val() == '') {
            alert('Preencha um nome');
            return false;
        }
        p += '&monnome=' + jQuery('#monnome').val();

        if (jQuery('#monfoneddd').val() == '') {
            alert('Preencha o DDD');
            return false;
        }

        if (jQuery('#monfone').val() == '') {
            alert('Preencha o Telefone');
            return false;
        }
        p += '&monfone=' + jQuery('#monfoneddd').val() + jQuery('#monfone').val();

        if (jQuery('#monemail').val() == '') {
            alert('Preencha o Email');
            return false;
        }

        if (!validaEmail(jQuery('#monemail').val())) {
            alert('Email inválido');
            return false;
        }
        p += '&monemail=' + jQuery('#monemail').val();
        p += '&orgcod=' + jQuery('#orgcod').val();

        ajaxatualizar(p, '');
        ajaxatualizar('requisicao=listaUnidades', 'dv_listaunidades');
        jQuery('#modalMonitorInterno').dialog('close');
    }

    function carregaUsuario() {
        var usucpf = jQuery('#moncpf').val();
        usucpf = usucpf.replace('-', '');
        usucpf = usucpf.replace('.', '');
        usucpf = usucpf.replace('.', '');

        var comp = new dCPF();
        comp.buscarDados(usucpf);
        var arrDados = new Object();
        if (!comp.dados.no_pessoa_rf) {
            alert('CPF Inválido');
            return false;
        }
        jQuery('#monnome').val(comp.dados.no_pessoa_rf);
    }
</script>
<div id="modalMonitorInterno" style="display:none;"></div>
<section class="col-md-12">
    <div id="dv_listaunidades">
        <section class="well">
            <section class="form-horizontal">
                <section class="form-group">
                    <label class="col-md-3 control-label" for="id_periodo_referencia">Selecione o período de referência:</label>
                    <section class="col-md-9">
                    <?php
                    /* Selecionar o periodo atual */
                    if(!isset($_REQUEST['id_periodo_referencia'])){
                        $_REQUEST['id_periodo_referencia'] = recuperaUltimoPeriodoReferencia();
                    }
                    inputCombo('id_periodo_referencia', $periodos, $_REQUEST['id_periodo_referencia'], 'id_periodo_referencia',array('acao'=>'selecionarPeriodo'));
                    ?>
                    </section>
            </section>
            </section>
        </section>
        <label for="textFind" style="margin-bottom: 3px">Pesquisa rápida: </label>
    <input class="normal form-control" type="text" id="textFind"></input>
    <br></p>
        <?php
        if(!isset($_REQUEST['id_periodo_referencia'])){$_REQUEST['id_periodo_referencia'] = 2;}
        listaSubacoes_bootstrap(array('id_periodo_referencia'=>$_REQUEST['id_periodo_referencia']));
        ?>
    </div>
</section>