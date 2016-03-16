<?php
/**
 * Listagem de unidades
 *
 * @package SiMEC
 * @subpackage planejamento-acompanhamento-orcamentario
 * @version $Id: listaunidades.inc 94320 2015-02-23 21:09:34Z OrionMesquita $
 */

/**
 * Funções de ações.
 * @see _funcoesacoes.php
 */
include_once '_funcoesacoes.php';
include_once(APPRAIZ . 'includes/funcoesspo.php');

if($_REQUEST['requisicao'] == 'recuperaCPF'){
    echo recuperaCPF($_REQUEST['cpf']);
    die();
}
if ($_REQUEST['requisicao']) {
    $_REQUEST['requisicao']($_REQUEST);
    exit;
}

function recuperaCPF($cpf) {
    global $db;
    $sql = <<<DML
SELECT usu.usunome AS monnome,
       monfone,
       monemail
  FROM seguranca.usuario usu
    LEFT JOIN planacomorc.monitorinterno mon ON(usucpf = moncpf)
  WHERE usu.usucpf = '{$cpf}'
  ORDER BY monid DESC
  LIMIT 1
DML;
    $dadosCPF = $db->pegaLinha($sql);

    return utf8_encode(simec_json_encode($dadosCPF));
}
/* Gato para parar de dar erro de Período, está vindo padrão o periodo 4 e está errado. */
if ($_SESSION['exercicio'] == '2013' && !isset($_REQUEST['percod'])) {
    $_REQUEST['percod'] = 2;
} elseif ('2014' == $_SESSION['exercicio'] && !isset($_REQUEST['percod'])) {
    $_REQUEST['percod'] = 5;
}

//Chamada de programa
include APPRAIZ . "includes/cabecalho.inc";
?>
<script type="text/javascript" language="javascript" src="/includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/webservice/cpf.js" /></script>
<script type="text/javascript">
$(document).ready(function(){
    $.expr[':'].contains = function(a, i, m) {
        return $(a).text().toUpperCase().indexOf(m[3].toUpperCase()) >= 0;
    };
    $("#textFind").keyup(function() {
        $('table.table tbody tr td').removeClass('marcado');
        $('table.table tbody tr').removeClass('remover');
        stringPesquisa = $("#textFind").val();
        if (stringPesquisa) {
            $('table.table tbody tr td:contains(' + stringPesquisa + ')').addClass('marcado');
            $('table.table tbody tr:not(:contains(' + stringPesquisa + '))').addClass('remover');
        }
    });
    $('#percod').change(function() {
        window.location = "planacomorc.php?modulo=principal/acoes/listaunidades&acao=A&percod=" + this.value;
    });
});

function gerenciarUnidadeAcao(orgcod, percod) {
    window.location = 'planacomorc.php?modulo=principal/acoes/listaunidades&acao=A&requisicao=carregarUnidade'
            + '&orgcod=' + orgcod
            + '&percod=' + $('#percod').val()
            ;
}

function gerenciarMonitorInterno(orgcod) {
	var url = 'planacomorc.php?modulo=principal/acoes/listaunidades&acao=A&requisicao=gerenciarMonitorInterno&orgcod=' + orgcod;
	$.post(url, function(html) {
        $('#modal-confirm .modal-body p').html(html);
        //$('.modal-dialog').css('width', '90%');
        $('#modal-confirm .modal-title').html('Monitor Interno');
        $('#modal-confirm .btn-primary').attr('onclick','validarMonitor();');
        $('#modal-confirm .btn-primary').html('Salvar');
        $('#modal-confirm .btn-default').html('Fechar');
        $('.modal-dialog').show();
        $('#modal-confirm').modal();
    });
}

function validarMonitor() {
    var p = 'requisicao=gravarMonitorInterno';
    if ($('#moncpf').val() == '') {
        alert('Preencha um cpf');
        return false;
    }
    $('#moncpf').val(mascaraglobal('###.###.###-##', $('#moncpf').val()))
    if (!validar_cpf($('#moncpf').val())) {
        alert('CPF inválido');
        return false;
    }
    p += '&moncpf=' + $('#moncpf').val();
    if ($('#monnome').val() == '') {
        alert('Preencha um nome');
        return false;
    }
    p += '&monnome=' + $('#monnome').val();
    if ($('#monfoneddd').val() == '') {
        alert('Preencha o DDD');
        return false;
    }
    if ($('#monfone').val() == '') {
        alert('Preencha o Telefone');
        return false;
    }
    p += '&monfone=' + $('#monfoneddd').val() + $('#monfone').val();
    if ($('#monemail').val() == '') {
        alert('Preencha o Email');
        return false;
    }
    if (!validaEmail($('#monemail').val())) {
        alert('Email inválido');
        return false;
    }
    p += '&monemail=' + $('#monemail').val();
    p += '&orgcod=' + $('#orgcod').val();
    ajaxatualizar(p, '');
    ajaxatualizar('', '');
    alert('Dados do monitor interno atualizados com sucesso');
     window.location.href = window.location.href;
}

function carregaUsuario() {
    var usucpf = $('#moncpf').val();
    if (usucpf.length == 14){
	    usucpf = usucpf.replace('-', '').replace('.', '').replace('.', '');
        var url = 'planacomorc.php?modulo=principal/acoes/listaunidades&acao=A&requisicao=recuperaCPF&cpf=' + usucpf;
        $.ajax(url, {async:false, dataType:'json', success:function(data){
            if (data) {                                
                if(data.monfone != null)
                    var telefone = data.monfone.trim();
                
                $('#monnome').val(data.monnome);
                if(telefone != null){
                    $('#monfoneddd').val(telefone.substr(0, 2));
                    $('#monfone').val(telefone.substr(2));
                }else{
                    $('#monfoneddd').val('');
                    $('#monfone').val('');
                }
                    
                $('#monemail').val(data.monemail);

                return;
            }

            $('#monnome').val('');
            $('#monfoneddd').val('');
            $('#monfone').val('');
            $('#monemail').val('');
        }});
    }
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
</script>
<style type="text/css">
.marcado{background-color:#C1FFC1!important}
.remover{display:none}
</style>

<div id="modalMonitorInterno" title="Monitor Interno" style="display:none;"></div>
<div class="col-md-12">
	<ol class="breadcrumb">
    	<li><a href="/planacomorc/planacomorc.php?modulo=inicio&acao=C"><?= $_SESSION['sisdsc']; ?></a></li>
    	<li class="">Ações <?= $_SESSION['exercicio']; ?></li>
    	<li class="active">Acompanhamento de Ações</li>
	</ol>
	<div class="well">
            <div class="form-horizontal ">
                <div class="form-group">
                    <div class="col-md-2 text-right">
                        <label class="control-label" for="percod">Período</label>
                    </div>
                    <div class="col-md-10">
                    <?php
                    /* Seleção de Período Padrão */
                    if (!isset($_REQUEST['percod']) || $_REQUEST['percod'] == '') {
                        $periodoSelecionado = recuperaUltimoPeriodoReferencia();
                    } else {
                        $periodoSelecionado = $_REQUEST['percod'];
                    }
                    $percod = $periodoSelecionado;
                    $_REQUEST['percod'] = $periodoSelecionado;
                    /* FIM Seleção de Período Padrão */
                    $sql = <<<DML
                        SELECT 
                            id_periodo_referencia AS codigo,
                            titulo || ' : ' || TO_CHAR(inicio_validade,'DD/MM/YYYY') ||' a ' || TO_CHAR(fim_validade, 'DD/MM/YYYY') AS descricao
                        FROM planacomorc.periodo_referencia p
                        WHERE id_exercicio = '{$_SESSION['exercicio']}'
                        ORDER BY id_periodo_referencia desc
DML;
                        $valor = $_REQUEST['percod'];
                        $opcoesAdicionais = array();
                        inputCombo('percod', $sql, $valor, 'percod', $opcoesAdicionais);
                        ?>
                    </div>
	    	</div>
	    </div>
	</div>
    <style type="text/css">
        .marcado{background-color:#C1FFC1!important}
        .remover{display:none}
    </style>
    <div>
        <label for="textFind" style="margin-bottom:3px">Pesquisa rápida: </label>
        <input class="normal form-control" type="text" id="textFind" style="margin-bottom:5px" />
        <br />
        <?php listaUnidadesAcompanhamento($_REQUEST); ?>
    </div>
</div>