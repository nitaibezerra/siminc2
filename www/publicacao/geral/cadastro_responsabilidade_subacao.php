<?
// inicializa sistema
require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
require (APPRAIZ . 'www/acomporc/_constantes.php');
require_once (APPRAIZ . 'includes/library/simec/Listagem.php');
include APPRAIZ . "includes/funcoesspo.php";
$db = new cls_banco();

function gravarResponsabilidadeSubacao($dados) {
    global $db;
    $sql = <<<DML
        UPDATE acomporc.usuarioresponsabilidade SET rpustatus = 'I'
            WHERE usucpf = '{$dados['usucpf']}'
                AND pflcod = '{$dados['pflcod']}'
                AND prfid = {$dados['prfid']}
DML;
    $db->executar($sql);

    if ($dados['usuacaresp']) {
        foreach($dados['usuacaresp'] as $sbacod) {
            $sql = <<<DML
                INSERT INTO acomporc.usuarioresponsabilidade(pflcod, usucpf, rpustatus, rpudata_inc, sbacod, prfid)
                    VALUES ('{$dados['pflcod']}', '{$dados['usucpf']}', 'A', NOW(), '{$sbacod}', {$dados['prfid']});
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
        <META http-equiv="Pragma" content="no-cache">
        <title>Definição de responsabilidades - Subações</title>
        <script language="JavaScript" src="/includes/funcoes.js"></script>
        <script src="/library/jquery/jquery-1.10.2.js" type="text/javascript" charset="ISO-8895-1"></script>
        <script src="/library/jquery/jquery-ui-1.10.3/jquery-ui.min.js" type="text/javascript" charset="ISO-8895-1"></script>
        <script src="/library/bootstrap-3.0.0/js/bootstrap.min.js" type="text/javascript" charset="utf-8"></script>
        <link rel="stylesheet" href="/library/bootstrap-3.0.0/css/bootstrap.css">
        <link href="/library/chosen-1.0.0/chosen.css" rel="stylesheet"  media="screen">
        <script src="/library/chosen-1.0.0/chosen.jquery.js" type="text/javascript"></script>
        <link rel='stylesheet' type='text/css' href='/includes/loading.css'/>
    </head>
    <body LEFTMARGIN="0" TOPMARGIN="5" bottommargin="5" MARGINWIDTH="0" MARGINHEIGHT="0" BGCOLOR="#ffffff" onload="self.focus()">
        <script>
        $(document).ready(function(){
            $('#prfid').chosen();
            $('#prfid').change(function(){
                if($(this).val() != ''){
                    var url = 'cadastro_responsabilidade_subacao.php?pflcod='+$('[name=pflcod]').val();
                    url += '&usucpf='+$('[name=usucpf]').val();
                    url += '&prfid='+$(this).val();
                    document.location.href = url;
                }
            });

            $('#ckboxPai').on('click',function(){
                $('#loading').show();
                if($('#ckboxPai').prop('checked')){
                    if($('#textFind').val().trim() != ''){
                        $('table td[class=listagem-marcado]').prev().find('input:not(":checked")').each(function(){
                            $(this).click();
                        });
                    }else{
                        $('table td input:not(":checked")').each(function(){
                            $(this).click();
                        });
                    }
                }else{
                    if($('#textFind').val().trim() != ''){
                        $('table td[class=listagem-marcado]').prev().find('input:checked').each(function(){
                            $(this).click();
                        });
                    }else{
                        $('table td input:checked').each(function(){
                            $(this).click();
                        });
                    }
                }
                $('#loading').hide();
            });
        });

        function marcarSubacao(obj) {
            var periodo = $('#prfid');
            if(periodo.value == ''){
                obj.checked = false;
                alert('Escolha um Período antes!');
                periodo.focus();
                return false;
            }

            if(obj.checked) {
                if (!$('#usuacaresp option[value='+obj.value+']')[0]) {
                    $("#usuacaresp").append('<option value='+obj.value+'>'+obj.parentNode.parentNode.cells[1].innerHTML+'</option>');
                }
            } else {
                $('#usuacaresp option[value='+obj.value+']').remove();
            }
        }

        function enviarFormulario()
        {
            var periodo = document.getElementById('prfid');

            if(periodo.value == ''){
                alert('O campo Período é obrigatório!');
                periodo.focus();
                return false;
            }

            selectAllOptions(document.getElementById('usuacaresp'));
            document.formassocia.submit();
        }
        </script>
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
        <section style="overflow:auto;width:496px;height:350px;border:2px solid #ececec;background-color:white">
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
                            WHERE prftipo = 'S'
                            ORDER BY prsano, prfinicio, prffim DESC
DML;
                            $prfid = $_GET['prfid'];
                            inputCombo('prfid',$sql,$_GET['prfid'],'prfid',array('mantemSelecaoParaUm' => FALSE));
                        ?>
                        </section>
                    </section>
                </section>
            <?
            if($prfid){
                $sql = <<<DML
                    SELECT
                        '<input type=\"checkbox\" class="ckboxChild" name=\"sbacod\" id=\"chk_'||ss.sbacod||'\" value=\"'||ss.sbacod||'\" onclick=\"marcarSubacao(this);\"' ||
                            case when ss.sbacod = ur.sbacod then 'checked=\"checked\"' else '' end ||'>' as subacao,
                        ss.sbacod || ' - ' || ms.sbatitulo AS descricao
                    FROM acomporc.snapshotsubacao ss
                    INNER JOIN monitora.subacao ms USING(sbacod)
                    LEFT JOIN acomporc.usuarioresponsabilidade ur ON ur.sbacod = ss.sbacod
                        AND ss.prfid = ur.prfid
                        AND ur.usucpf = '{$usucpf}'
                        AND ur.rpustatus = 'A'
                    WHERE ss.prfid = {$prfid}
                    GROUP BY ss.sbacod, ur.sbacod, ms.sbatitulo
                    ORDER BY ss.sbacod
DML;

                $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
                $listagem->turnOnPesquisator();
                $listagem->setTitulo('Definição de responsabilidades - Subações');
                $listagem->setCabecalho(array("<input type=\"checkbox\" id=\"ckboxPai\">","Subação"));
                $listagem->setQuery($sql);
                $listagem->addCallbackDeCampo('descricao', 'alinhaParaEsquerda');
                $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
                $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
            }else{
                echo <<<HTML
                <section class="alert alert-warning text-center">Selecione um período</section>
HTML;
            }
?>
            </section>
        </section>
<?php
$usuarioresponsabilidade = array();
if($prfid){
    $sql = <<<DML
        SELECT
            ur.sbacod AS codigo,
            ur.sbacod || ' - ' || ms.sbatitulo AS descricao
        FROM acomporc.usuarioresponsabilidade ur
        INNER JOIN monitora.subacao ms USING(sbacod)
        WHERE ur.usucpf = '{$usucpf}'
            AND ur.pflcod = '{$pflcod}'
            AND ur.rpustatus = 'A'
            AND ur.prfid = {$prfid}
      ORDER BY ur.sbacod
DML;
    $usuarioresponsabilidade = $db->carregar($sql);
}
?>
        <section class="container">
            <form class="form-horizontal" name="formassocia" style="margin:0px;" method="POST">                
                <input type="hidden" name="usucpf" value="<?=$usucpf?>">
                <input type="hidden" name="pflcod" value="<?=$pflcod?>">
                <input type="hidden" name="prfid" value="<?=$prfid?>">
                <input type="hidden" name="requisicao" value="gravarResponsabilidadeSubacao">
                <section class="form-group">
                    <label class="control-label col-md-2" for="usuacaresp">Oções Marcadas</label>
                    <section class="col-md-10">
                        <select multiple size="8" name="usuacaresp[]" id="usuacaresp" class="form-control">
<?
                        if($usuarioresponsabilidade[0]) {
                            foreach($usuarioresponsabilidade as $ur) {
                                echo '<option value="'.$ur['codigo'].'">'.$ur['descricao'].'</option>';
                            }
                        }
?>
                        </select>
                    </section>
                </section>
                <section class="form-group">
                    <section class="col-md-12">
                        <input class="btn btn-success" type="Button" name="ok" value="Salvar" onclick="enviarFormulario();" id="ok">
                    </section>
                </section>
            </form>
        </section>
    </body>
</html>