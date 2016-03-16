<?php
    
    if($_REQUEST['req']) {	
		$_REQUEST['req']($_REQUEST);
		exit;
    }
    
    if ($_POST['requisicao'] == 'salvarDados') {

        if ($_REQUEST['cmeid'] && $_REQUEST['cmemeta']) {
            $sql = 'delete from projovemurbano.metasdoprograma where cmeid = ' . $_REQUEST['cmeid'] . ' and tpmid in (7, 13);';

            if ($_REQUEST['cmeid'] && $_REQUEST['metaDestinada'] == 'J') {
                $sql .= "
                        Insert into projovemurbano.metasdoprograma (tpmid, ppuid, suaid, cmeid, mtpvalor) values (7, {$_SESSION['projovemurbano']['ppuid']}, null, {$_REQUEST['cmeid']}, " . $_REQUEST['cmemeta'] . ");
                        Insert into projovemurbano.metasdoprograma (tpmid, ppuid, suaid, cmeid, mtpvalor) values (13, {$_SESSION['projovemurbano']['ppuid']}, null, {$_REQUEST['cmeid']}, 0);
                ";
            } elseif ($_REQUEST['cmeid'] && $_REQUEST['metaDestinada'] == 'P') {
                $sql .= "
                        Insert into projovemurbano.metasdoprograma (tpmid, ppuid, suaid, cmeid, mtpvalor) values (7, {$_SESSION['projovemurbano']['ppuid']}, null, {$_REQUEST['cmeid']}, 0);
                        Insert into projovemurbano.metasdoprograma (tpmid, ppuid, suaid, cmeid, mtpvalor) values (13, {$_SESSION['projovemurbano']['ppuid']}, null, {$_REQUEST['cmeid']}, " . $_REQUEST['cmemeta'] . ");
                ";
            }

            if ( $db->executar($sql) ){
                $db->commit();
                $db->sucesso('principal/termoAdesao', '');
            }else{
                $db->insucesso('Ocorreu um erro, procure o administrador do sistema.','', 'principal/termoAdesao');
            }
        }
    }

    include_once APPRAIZ . 'includes/cabecalho.inc';
    echo '<br>';

    echo montarAbasArray(montaMenuProJovemUrbano(), $_SERVER['REQUEST_URI']);

    monta_titulo( 'Projovem Urbano', montaTituloEstMun() );

    if( $_SESSION['projovemurbano']['ppuid'] != '3'){
	    $habil = 'N';
    }else{
	    $habil = 'S';
    }
    $size = 20;

    if ($_SESSION['projovemurbano']['muncod']) {
        $cmecodibge = $_SESSION['projovemurbano']['muncod'];
    } else {
        $cmecodibge = $db->pegaUm("select estcod from territorios.estado where estuf = '{$_SESSION['projovemurbano']['estuf']}'");
    }

    $sql = "select * from projovemurbano.cargameta where cmecodibge = '{$cmecodibge}' and ppuid = {$_SESSION['projovemurbano']['ppuid']}";
    $rsCargaMeta = $db->pegaLinha($sql);
    
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script>
    
    $(document).ready(function(){
        $.ajax({
            type	: "POST",
            url		: window.location,
            data	: "req=verificaMetaDestinada&cmeid=<?=$rsCargaMeta['cmeid'];?>&metaDestinada=atendida",
            async	: false,
            success: function(resp){
                if(resp === '7'){
                    $('#metaDestinada_J').attr('checked',true);
                }else{
                    $('#metaDestinada_P').attr('checked',true);
                }
            }
        });
        <?php if( $habil == 'N' ){?>
        $('input').attr('disabled',true);
        <?php }?>
    });
    
    $(function() {
        $('#btnSalvar').click(function(){
            var valida = validarForm();
            if(valida){
                erro = 0;
                $.each($("[name^='mtpvalor']"), function(i, v) {
                    if ($(v).val() == '') {
                        erro++;
                        $(v).focus();
                        return false;
                    }
                });

                $('[name=requisicao]').val('salvarDados');
                $('#formulario').submit();
            }else{
                alert('O formulario deve ser preenchido corretamente, selecione uma programa.');
            }
        });

    });
    
    function validarForm(){
        var metaDestinada = $('input[name=metaDestinada]:checked').val();
            
        if(metaDestinada === 'P' || metaDestinada === 'J'){
            return true;
        }else{
            return false;
        }
    }

    function recuperaTotalMetas(){
        total = 0;
        $.each($("[name^='mtpvalor']"), function(i, v) {
            if (parseInt($(v).val()) > 0)
                total = parseInt(total) + parseInt($(v).val());
        });
        return total;
    } 
</script>
<form id="formulario" name="formulario" method="post" action="">
    <input type="hidden" name="requisicao" value="" />
    <input type="hidden" name="cmeid" value="<?php echo $rsCargaMeta['cmeid']; ?>" />
    
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
        <tr>
            <td class="subtituloDireita" width="250">Meta Total:</td>
            <td>
                <!-- mtpvalor[4] -->
                <b><?=formata_numero($rsCargaMeta['cmemeta']); ?></b>&nbsp;Estudantes
                <input type="hidden" name="cmemeta" value="<?php echo $rsCargaMeta['cmemeta']; ?>" />
            </td>
        </tr>
        <td class="SubTituloDireita">Meta destinada:</td>
        <td>
            <input type="radio" id="metaDestinada_J" name="metaDestinada" value="J"> Juventude Viva 
            <input type="radio" id="metaDestinada_P" name="metaDestinada" value="P"> Público Geral
        </td>
        <?php if( $habil == 'S' ){?>
        <tr>
            <td class="subtituloDireita"></td>
            <td><input type="button" value="Salvar" id="btnSalvar" /></td>
        </tr>
        <?php }?>
    </table>
</form>