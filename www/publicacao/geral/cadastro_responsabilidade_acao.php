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

function gravarResponsabilidadeAcao($dados) {
    global $db, $esquema;

    $sql = "UPDATE {$esquema}.usuarioresponsabilidade SET rpustatus='I' WHERE usucpf='".$dados['usucpf']."' AND pflcod='".$dados['pflcod']."'";
    $db->executar($sql);

    if ($dados['usuacaresp']) {
        foreach($dados['usuacaresp'] as $acacod) {
            $sql = <<<DML
                INSERT INTO {$esquema}.usuarioresponsabilidade(pflcod, usucpf, rpustatus, rpudata_inc, acacod, prfid, unicod)
                VALUES ('{$dados['pflcod']}', '{$dados['usucpf']}', 'A', NOW(), '{$acacod}', '{$dados['prfid']}', '{$dados['unicod']}')
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
        <script>
            function marcarAcao(obj) {
                if(obj.checked) {
                    if (!jQuery('#usuacaresp option[value='+obj.value+']')[0]) {
                        jQuery("#usuacaresp").append('<option value='+obj.value+'>'+obj.parentNode.parentNode.cells[1].innerHTML+'</option>');
                    }
                } else {
                    jQuery('#usuacaresp option[value='+obj.value+']').remove();
                }
            }
            function removeOpcao(obj){
                if(event.keyCode == 44){
                    $('#chk_'+$(obj).val()).attr('checked',false);
                    $('#usuacaresp option[value='+$(obj).val()+']').remove();
                }
            }
            function desmarcaOpcao()
            {
                $.each($('#usuacaresp').val(),function(index,value){
                    $('#chk_'+value).attr('checked',false);
                    $('#usuacaresp option[value='+value+']').remove();
                });
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

                $('body').on('keypress',function(){
                    console.log(event.keyCode);
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

                $('#unicod').on('change',function(){
                    var url = 'cadastro_responsabilidade_acao.php?pflcod='+$('[name=pflcod]').val();
                    url += '&usucpf='+$('[name=usucpf]').val();
                    url += '&prfid='+$('[name=prfid]').val();
                    url += '&unicod='+$(this).val();
                    document.location.href = url;
                    
                });
            });
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
                        $sql = <<<DML
                            SELECT
                                uni.unicod AS codigo,
                                uni.unicod || ' - ' || uni.unidsc AS descricao
                            FROM public.unidade uni
                            WHERE uni.unistatus = 'A'
                                AND (uni.orgcod = '26000' OR uni.unicod IN('74902', '73107'))
                            ORDER BY uni.unicod
DML;
                            $unicod = $_GET['unicod'];
                            inputCombo('unicod',$sql,$unicod,'unicod',array('mantemSelecaoParaUm' => FALSE));
                        ?>
                        </section>
                    </section>
                </section>
            <?php
            // -- É feita uma verificação no SQL para saber se aquele acaid já foi escolhido previamente
            // -- com base nisso, é adicionado o atributo checked ao combo da unicod.acacod selecionado previamente.
            $unidadesObrigatorias = UNIDADES_OBRIGATORIAS;
            $whereUO = '';
            if($unicod){
                $whereUO = " AND aca.unicod = '$unicod'";
            }
            $sql = <<<DML
                SELECT
                    '<input type="checkbox" class="ckboxChild" name="acacod[]" id="chk_' || aca.acacod || '" value="' || aca.acacod || '" '
                    || 'onclick="marcarAcao(this)"' || case WHEN
                        (SELECT count(urp.rpuid)
                            FROM acomporc.usuarioresponsabilidade urp
                            WHERE urp.usucpf = '{$usucpf}'
                                AND urp.pflcod = '{$pflcod}'
                                AND urp.acaid = aca.acaid
                                AND urp.prfid = (SELECT prfid FROM acomporc.periodoreferencia WHERE prsano = '{$_SESSION['exercicio']}' AND prftipo = 'A' ORDER BY prfid DESC LIMIT 1)
                                AND urp.rpustatus = 'A'
                            ) > 0 THEN ' checked' ELSE '' END || '>' AS acaid,
                    unicod ||'.'|| acacod AS descricao
                FROM monitora.acao aca
                WHERE prgano = '{$_SESSION['exercicio']}'
                    $whereUO
                ORDER BY descricao
DML;
            $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
            $listagem->turnOnPesquisator();
            $listagem->setTitulo('Definição de responsabilidades - Ações');
            $listagem->setCabecalho(array("<input type=\"checkbox\" id=\"ckboxPai\">","UO / Ação"));
            $listagem->setQuery($sql);
            $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
            if($prfid){
                $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
            }else{
                echo <<<HTML
                <section class="alert alert-warning text-center">Selecione um período</section>
HTML;
            }
?>          </section>
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
                        <select class="form-control" multiple size="8" name="usuacaresp[]" id="usuacaresp" onkeypress="removeOpcao(this);">
                        <?
                        $sql = <<<DML
                            SELECT
                                urp.acacod AS codigo,
                                aca.unicod ||'.'|| aca.acacod AS descricao
                            FROM acomporc.usuarioresponsabilidade urp
                            INNER JOIN monitora.acao aca on urp.acaid = aca.acaid
                            WHERE urp.pflcod = '{$pflcod}'
                                AND urp.usucpf = '{$usucpf}'
                                AND urp.prfid = (SELECT prfid FROM acomporc.periodoreferencia WHERE prsano = '{$_SESSION['exercicio']}' AND prftipo = 'A' ORDER BY prfid DESC LIMIT 1)
                                AND urp.rpustatus = 'A'
DML;
                        if($prfid){
                            $usuarioresponsabilidade = $db->carregar($sql);

                            if($usuarioresponsabilidade[0]) {
                                foreach($usuarioresponsabilidade as $ur) {
                                    echo '<option value="'.$ur['codigo'].'">'.$ur['descricao'].'</option>';
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
        
    </body>
</html>
