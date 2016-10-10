<?php
/**
 * Cadastro de responsabilidades de usuário sobre UOs.
 * $Id: cadastro_responsabilidade_uo.php 89340 2014-10-29 18:40:36Z Kamylasakamoto $
 */

require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
require (APPRAIZ . 'www/altorc/_constantes.php');
require_once (APPRAIZ . 'includes/library/simec/Listagem.php');
include APPRAIZ . "includes/funcoesspo.php";
$db = new cls_banco();
$esquema = 'acomporc';

function gravarResponsabilidadeAcao($dados)
{
    global $db, $esquema;

    function expDivisor($item)
    {
        return array_combine(array('unicod', 'acacod'), explode('.', $item));
    }
    $resp = array_map('expDivisor', $dados['usuacaresp']?$dados['usuacaresp']:array());

    $sql = "UPDATE {$esquema}.usuarioresponsabilidade SET rpustatus='I' WHERE usucpf='".$dados['usucpf']."' AND pflcod='".$dados['pflcod']."'";
    $db->executar($sql);

    if ($resp) {
        foreach($resp as $_resp) {
            $sql = <<<DML
INSERT INTO {$esquema}.usuarioresponsabilidade(pflcod, usucpf, rpustatus, rpudata_inc, acacod, prfid, unicod)
  VALUES ('{$dados['pflcod']}', '{$dados['usucpf']}', 'A', NOW(), '{$_resp['acacod']}', '{$dados['prfid']}', '{$_resp['unicod']}')
DML;
            $db->executar($sql);
        }
    }
    $db->commit();
    echo "
        <script language=\"javascript\">
            alert(\"Operação realizada com sucesso!\");
            opener.location.reload();
            self.close();
        </script>";
}

function listarAcoesUo($dados)
{
    // -- É feita uma verificação no SQL para saber se aquele acaid já foi escolhido previamente
    // -- com base nisso, é adicionado o atributo checked ao combo da unicod.acacod selecionado previamente.
    $unidadesObrigatorias = UNIDADES_OBRIGATORIAS;
    $whereUO = '';
    if ($dados['unicod']) {
        $whereUO = " AND aca.unicod = '{$dados['unicod']}'";
    }
    $sql = <<<DML
SELECT aca.acacod,
       aca.unicod,
       (SELECT count(urp.rpuid)
          FROM acomporc.usuarioresponsabilidade urp
          WHERE urp.usucpf = '{$dados['usucpf']}'
            AND urp.pflcod = '{$dados['pflcod']}'
            AND urp.acaid = aca.acaid
            AND urp.prfid = (SELECT prfid
                               FROM acomporc.periodoreferencia
                               WHERE prsano = '{$_SESSION['exercicio']}'
                                 AND prftipo = 'A'
                               ORDER BY prfid DESC LIMIT 1)
            AND urp.rpustatus = 'A') AS marcado
  FROM monitora.acao aca
  WHERE prgano = '{$_SESSION['exercicio']}'
  {$whereUO}
  ORDER BY aca.unicod,
           aca.acacod
DML;

    $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
    $listagem->turnOnPesquisator();
    $listagem->setTitulo('Definição de responsabilidades - Ações');
    $listagem->setCabecalho(array("<input type=\"checkbox\" id=\"ckboxPai\">","UO / Ação"));
    $listagem->setQuery($sql)
        ->addCallbackDeCampo('acacod', 'acaoUoCheckbox')
        ->addCallbackDeCampo('unicod', 'formatarUoAcao')
        ->esconderColunas('marcado');

    $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
    $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);

    echo <<<JS
<script type="text/javascript">
$(function(){
    $('#lista-de-acoes input[type="checkbox"]').each(function(){
        if ($('#usuacaresp option[value="' + $(this).val() + '"]')[0]) {
            $(this).prop('checked', true);
        }
    });
});
</script>
JS;
}

function acaoUoCheckbox($acacod, $dados)
{
    $checked = $dados['marcado']?'checked':'';
    return <<<HTML
<input type="checkbox" class="ckboxChild" name="acacod[]" onclick="marcarAcao(this)"
       id="chk_{$dados['unicod']}_{$acacod}" value="{$dados['unicod']}.{$acacod}" {$checked} />
HTML;
}

function formatarUoAcao($unicod, $dados)
{
    return "{$unicod}.{$dados['acacod']}";
}

if($_REQUEST['requisicao']) {
    $_REQUEST['requisicao']($_REQUEST);
    exit;
}

$usucpf = $_REQUEST['usucpf'];
$pflcod = $_REQUEST['pflcod'];
$prfid = $_GET['prfid'];
?>
<html>
    <head>
        <meta http-equiv="Pragma" content="no-cache">
        <title>Definição de responsabilidades - Ações</title>
        <script language="JavaScript" src="/includes/funcoes.js"></script>
        <script src="/library/jquery/jquery-1.10.2.js" type="text/javascript" charset="ISO-8895-1"></script>
        <script src="/library/jquery/jquery-ui-1.10.3/jquery-ui.min.js" type="text/javascript" charset="ISO-8895-1"></script>
        <script src="/library/bootstrap-3.0.0/js/bootstrap.min.js" type="text/javascript" charset="utf-8"></script>
        <link rel="stylesheet" href="/library/bootstrap-3.0.0/css/bootstrap.css">
        <link href="/library/chosen-1.0.0/chosen.css" rel="stylesheet"  media="screen">
        <script src="/library/chosen-1.0.0/chosen.jquery.js" type="text/javascript"></script>
        <link rel='stylesheet' type='text/css' href='/includes/loading.css'/>
    </head>
    <body leftmargin="0" topmargin="5" bottommargin="5" marginwidth="0" marginheight="0" bgcolor="#ffffff" onload="self.focus()">

        <!-- begin loader -->
		<div class="loading-dialog notprint" id="loading">
	        <div id="overlay" class="loading-dialog-content">
	            <div class="ui-dialog-content">
	            	<img src="/library/simec/img/loading.gif">
					<span>
						O sistema esta processando as informações. <br/>
						Por favor aguarde um momento...
					</span>
	            </div>
	        </div>
	    </div>
        <section style="overflow:auto;width:496px;height:350px;border:2px solid #ececec;background-color:white;">
            <section class="container">
                <section class="form-horizontal well">
                    <section class="form-group">
                        <label class="control-label col-md-2">Período:</label>
                        <section class="col-md-10">
                        <?php
                        $sql = <<<DML
                            SELECT
                                prfid AS codigo,
                                prftitulo || ' - ' || TO_CHAR(prfinicio, 'DD/MM/YYYY') || ' a ' || TO_CHAR(prffim, 'DD/MM/YYYY') AS descricao
                            FROM acomporc.periodoreferencia
                            WHERE prftipo = 'A'
                            ORDER BY prsano, prfinicio, prffim DESC
DML;
                            $prfid = $_GET['prfid'];
                            inputCombo('prfid',$sql,$_GET['prfid'],'prfid',array('mantemSelecaoParaUm' => FALSE));
                        ?>
                        </section>
                    </section>

                    <section class="form-group">
                        <label class="control-label col-md-2" for="unicod">Unidade Orçamentária:</label>
                        <section class="col-md-10">
                        <?php
                        $sql = "
                            SELECT
                                uni.unicod AS codigo,
                                uni.unicod || ' - ' || uni.unidsc AS descricao
                            FROM public.unidade uni
                            WHERE uni.unistatus = 'A'
                                AND (uni.orgcod = '". CODIGO_ORGAO_SISTEMA. "' OR uni.unicod IN('74902', '73107'))
                            ORDER BY uni.unicod
";
                            $unicod = $_GET['unicod'];
                            inputCombo('unicod',$sql,$unicod,'unicod',array('mantemSelecaoParaUm' => FALSE));
                        ?>
                        </section>
                    </section>
                </section>
            <?php

            echo '<div id="lista-de-acoes">';
            if ($prfid){
                listarAcoesUo($_REQUEST);
            }else{
                echo <<<HTML
                <section class="alert alert-warning text-center">Selecione um período</section>
HTML;
            echo '</div>';
            }
            ?>
            </section>
        </section>
        <section class="container">
            <form class="form-horizontal" name="formassocia" style="margin:0px;" method="POST">
                <input type="hidden" name="usucpf" value="<?=$usucpf?>">
                <input type="hidden" name="pflcod" value="<?=$pflcod?>">
                <input type="hidden" name="prfid" value="<?=$prfid?>">
                <input type="hidden" name="requisicao" value="gravarResponsabilidadeAcao">
                <section class="form-group">
                    <label class="control-label col-md-2" for="usuacaresp">Opções Marcadas</label>
                    <section class="col-md-10">
                        <select class="form-control" multiple size="8" name="usuacaresp[]" id="usuacaresp" onkeypress="removeOpcao(event);">
                        <?
                        $sql = <<<DML
SELECT DISTINCT aca.unicod,
                aca.acacod
  FROM acomporc.usuarioresponsabilidade urp
    INNER JOIN monitora.acao aca on urp.acacod = aca.acacod AND urp.unicod = aca.unicod
    INNER JOIN public.unidade uni on aca.unicod = uni.unicod
  WHERE urp.usucpf = '{$usucpf}'
    AND urp.pflcod = '{$pflcod}'
    AND urp.prfid = (SELECT prfid
                       FROM acomporc.periodoreferencia
                       WHERE prsano = '{$_SESSION['exercicio']}'
                         AND prftipo = 'A'
                       ORDER BY prfid DESC
                       LIMIT 1)
    AND urp.rpustatus = 'A'
    AND urp.acacod IS NOT NULL
    AND urp.unicod IS NOT NULL
  ORDER BY aca.unicod,
           aca.acacod
DML;
                        if ($prfid) {
                            $usuarioresponsabilidade = $db->carregar($sql);
                            if ($usuarioresponsabilidade[0]) {
                                foreach($usuarioresponsabilidade as $ur) {
                                    echo <<<HTML
<option value="{$ur['unicod']}.{$ur['acacod']}">{$ur['unicod']}.{$ur['acacod']}</option>
HTML;
                                }
                            }
                        }
                        ?>
                        </select>
                    </section>
                </section>
                <section class="form-group">
                    <section class="col-md-12">
                        <button type="Button" class="btn btn-warning"
                        onclick="desmarcaOpcao()">Desmarcar Opções</button>
                        <input type="Button" name="ok" value="Salvar" class="btn btn-success"
                        onclick="selectAllOptions(document.getElementById('usuacaresp'));document.formassocia.submit();"
                        id="ok">
                    </section>

                </section>
            </form>
        </section>
        <script type="text/javascript">
        function marcarAcao(obj)
        {
            if (obj.checked) {
                if (!jQuery('#usuacaresp option[value="'+obj.value+']"')[0]) {
                    jQuery("#usuacaresp").append('<option value="'+obj.value+'">'+obj.parentNode.parentNode.cells[1].innerHTML+'</option>');
                }
            } else {
                jQuery('#usuacaresp option[value="'+obj.value+'"]').remove();
            }
        }
        function removeOpcao(event)
        {
            if (46 === event.keyCode) {
                var id = $(event.target).val()[0];
                id = id.replace('\.', '_');

                $('#chk_' + id).prop('checked', false);
                $('#usuacaresp option[value="' + $(event.target).val() + '"]').remove();
            }
        }
        function desmarcaOpcao()
        {
            $('#lista-de-acoes input[type="checkbox"]').attr('checked', false);
            $('#usuacaresp option').remove();
        }
        $(document).ready(function(){
            $('#unicod').chosen();
            $('#prfid').chosen();
            $('#prfid').change(function(){
                if($(this).val() != ''){
                    var url = 'cadastro_responsabilidade_acao.php?pflcod='+$('[name=pflcod]').val();
                    url += '&usucpf='+$('[name=usucpf]').val();
                    url += '&prfid='+$(this).val();
                    url += '&unicod='+$('[name=unicod]').val();
                    document.location.href = url;
                }
            });
            $('#lista-de-acoes').on('#ckboxPai', 'click', function(){
                $('#loading').show();
                if ($('#ckboxPai').prop('checked')) {
                    if ($('#textFind').val().trim() != '') {
                        $('table td[class=listagem-marcado]').prev().find('input:not(":checked")').each(function(){
                            $(this).click();
                        });
                    } else {
                        $('table td input:not(":checked")').each(function(){
                            $(this).click();
                        });
                    }
                } else {
                    if ($('#textFind').val().trim() != '') {
                        $('table td[class=listagem-marcado]').prev().find('input:checked').each(function(){
                            $(this).click();
                        });
                    } else {
                        $('table td input:checked').each(function(){
                            $(this).click();
                        });
                    }
                }
                $('#loading').hide();
            });

            $('#unicod').on('change',function(){
                var url = 'cadastro_responsabilidade_acao.php?pflcod='+$('[name=pflcod]').val();
                url += '&usucpf='+$('[name=usucpf]').val();
                url += '&prfid='+$('[name=prfid]').val();
                url += '&unicod='+$(this).val();

                $.get(url, {requisicao:'listarAcoesUo'}, function(response){
                    $('#lista-de-acoes').empty().html(response);
                }, 'html');
            });
        });
        </script>
    </body>
</html>
