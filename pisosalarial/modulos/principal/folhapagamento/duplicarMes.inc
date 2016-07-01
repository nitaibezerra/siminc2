<?php
include_once APPRAIZ . 'includes/cabecalho.inc';
include_once APPPISO . 'classes/FolhaPagamento.class.inc';
include_once APPPISO . 'classes/FolhaPagamentoProfissionais.class.inc';

$obFolhaPagamento = new FolhaPagamento();
$obFolhaProfissionais = new FolhaPagamentoProfissionais();

if(isset($_POST['mesdup'])){

    $arDados = array('muncod'           => $_SESSION['piso']['muncod'],
                     'flpanoreferencia' => $_REQUEST['anodup'],
                     'flpmesreferencia' => $_REQUEST['mesdup']);

    $obFolhaPagamento->gerarFolhaPagamento($arDados);

    $obFolhaProfissionais->duplicarMesFolhaPagamento($_REQUEST);
    header("Location: ?modulo=principal/folhapagamento/listaFolhaPagamento&acao=A");
}

// Monta abas
print '<br/>';
$arMnuid = array(ABA_FP_LISTAR_PROFIS, ABA_FP_FORM_PROFISSIONAL, ABA_FP_IMPORTAR_ARQUIVO);
$abacod_tela = '57365';
$db->cria_aba( $abacod_tela, $url, '', $arMnuid );

monta_titulo('Duplicar Mês', "Folha de Pagamento");
montaCabecalho();
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">
$(document).ready(function(){

    $('.monetario').change(function(){
        arId = this.id.split('_');

        fppsalbase = parseFloat(replaceAll(replaceAll($('#fppsalbase_'+arId[1]).val(), '.', ''), ',', '.'));
        fppadigrat = parseFloat(replaceAll(replaceAll($('#fppadigrat_'+arId[1]).val(), '.', ''), ',', '.'));

        if(fppsalbase > 0 && fppadigrat > 0){
            fppcalculo = fppsalbase+fppadigrat;
            $('#fppcalculo_'+arId[1]).val(mascaraglobal('###.###.###.###,##', fppcalculo.toFixed(2)));
        }
    });

    $('.excluir').click(function(){
        if(confirm('Deseja excluir este registro?')){
            $(this).parent().parent().parent().remove();
        }
    });

    $('.salvar').click(function(){

        if($('select[name=anodup]').val() == ''){
            alert('Selecione um ano para duplicar!');
            $('select[name=anodup]').focus();
            return false;
        }
        if($('select[name=mesdup]').val() == ''){
            alert('Selecione um mês para duplicar!');
            $('select[name=mesdup]').focus();
            return false;
        }

        $.each($('input[name=fppid[]]'), function(key, obj){
            $('#formulario').append('<input type="hidden" name="fppid[]" value="'+obj.value+'" />');
        });
        $.each($('input[name=fppsalbase[]]'), function(key, obj){
            $('#formulario').append('<input type="hidden" name="fppsalbase['+obj.id.replace('fppsalbase_', '')+']" value="'+obj.value+'" />');
        });
        $.each($('input[name=fppadigrat[]]'), function(key, obj){
            $('#formulario').append('<input type="hidden" name="fppadigrat['+obj.id.replace('fppadigrat_', '')+']" value="'+obj.value+'" />');
        });
        $.each($('input[name=fppcargahoraria[]]'), function(key, obj){
            $('#formulario').append('<input type="hidden" name="fppcargahoraria['+obj.id.replace('fppcargahoraria_', '')+']" value="'+obj.value+'" />');
        });
        $.each($('select[name=carid[]]'), function(key, obj){
            $('#formulario').append('<input type="hidden" name="carid['+obj.id.replace('carid_', '')+']" value="'+obj.value+'" />');
        });
        $.each($('select[name=sitid[]]'), function(key, obj){
            $('#formulario').append('<input type="hidden" name="sitid['+obj.id.replace('sitid_', '')+']" value="'+obj.value+'" />');
        });

        campos = $('#formulario').serialize();
        $.ajax({
            url: 'ajax.php',
            type: 'post',
            data: 'requisicao=verificaAntesDuplicar&'+campos,
            success: function(e){
                if(e > 0){
                    if(!confirm('Está operação irá sobrescrever CPF já existentes no mês selecionado. Deseja continuar?')){
                        return false;
                    }
                }
                $('#formulario').submit();
            }
        });
    });

    $('.voltar').click(function(){
        document.location.href = 'pisosalarial.php?modulo=principal/folhapagamento/listaFolhaPagamento&acao=A';
    });

    $('select[name=anodup]').change(function(){
        $.ajax({
            url: 'ajax.php',
            dataType: 'json',
            type: 'post',
            data: 'requisicao=recuperarMesesFolhaPagamento&ano='+this.value+'&anoref='+$('input[name=anoref]').val()+'&mesref='+$('input[name=mesref]').val(),
            success: function(e){

                $i = true;
                var firstOption = 'Selecione...';
                $('select[name=mesdup]').empty();
                $('<option>').html( firstOption ).val('').appendTo($('select[name=mesdup]'));

                $.each (e, function(key , value) {
                    var option = $("<option>").val(key).html(value).appendTo($('select[name=mesdup]'));
                });
//                $('select[name=mesdup]').val(valor);
            }
        });
    });

    fppid = $('input[name=fppid[]]');

    carid = $('select[name=carid[]]');
    $('input[name=carid_temp[]]').each(function(key, obj){
        carid[key].value = obj.value;
        carid[key].id = 'carid_'+fppid[key].value;
    });
    sitid = $('select[name=sitid[]]');
    $('input[name=sitid_temp[]]').each(function(key, obj){
        sitid[key].value = obj.value;
        sitid[key].id = 'sitid_'+fppid[key].value;
    });

});
</script>
<style>
<!--
.acao_folha_pagamento{
    cursor:pointer;
}
.tabela{
    border-bottom: 0px solid white;
}
.tabelaProfissionais{
    border-top: 0px solid white;
}
-->
</style>
<form id="formulario" method="post" action="">
    <input type="hidden" name="mesref" value="<?php echo $_REQUEST['mesref'] ?>" />
    <input type="hidden" name="anoref" value="<?php echo $_REQUEST['anoref'] ?>" />
    <input type="hidden" name="muncod" value="<?php echo $_SESSION['piso']['muncod'] ?>" />
    <input type="hidden" name="fppsalbase" value="" />
    <input type="hidden" name="fppadigrat" value="" />
    <table class="tabela tabelaProfissionais" align="center" bgcolor="#f5f5f5" border="0" cellpadding="5" cellspacing="1">
        <!--
         <tr>
            <td colspan="2" class="subtitulocentro">Lista de Profissionais</td>
        </tr>
        -->
        <tr>
            <td class="SubtituloDireita" style="width: 19%">Mês / Ano da Duplicação:</td>
            <td>
                <?php
                $sql = "SELECT DISTINCT farano as codigo, farano as descricao FROM pisosalarial.folhaanoreferencia WHERE farstatus = 'A' ORDER BY farano";
                $db->monta_combo("anodup", $sql, 'S', 'Selecione...', '', '', '', '', 'S', '', '', '', 'Ano duplicar' );
                ?>
                &nbsp;/&nbsp;
                <?php
                $db->monta_combo("mesdup", array(), 'S', 'Selecione...', '', '', '', '', 'S', '', '', '', 'Ano duplicar' );
                ?>
            </td>
        </tr>
        <tr bgcolor="#D0D0D0">
            <td></td>
            <td style="text-align: left">
            <input class="salvar" type="button" value="Salvar" />
            <input class="voltar" type="button" value="Cancelar" />
           </td>
        </tr>
    </table>
</form>
<?php
$obFolhaProfissionais->listaDadosProfissionaisDuplicar($_REQUEST);
?>