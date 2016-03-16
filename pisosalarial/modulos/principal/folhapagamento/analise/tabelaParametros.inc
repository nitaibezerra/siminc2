<?php
include_once APPPISO . 'classes/ParametrosPontuacaoTabelas.class.inc';
include_once APPPISO . 'classes/ParametrosPontuacao.class.inc';
include_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$obParametrosPontuacao = new ParametrosPontuacao();
$obParametrosPontuacaoTabelas = new ParametrosPontuacaoTabelas();

monta_titulo( 'Complementação do Piso Salarial', 'Tipo de Pontuação' );
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script>

var arOperadores = new Array();
arOperadores = ['<', '>', '>=', '<=', 'ou'];

$(document).ready(function(){

    $('select[name=co_tabela]').change(function(){
        $('#formTipoPontuacao').submit();
    });

    //seleciona os elementos a com atributo name="modal"
    $('.parametro').click(function(e) {
        //cancela o comportamento padrão do link
        e.preventDefault();

        //armazena o atributo href do link
        var janela = '#dialog';
        var celula = $(this).attr('id');
        var co_tabela = $('input[name=co_tabela]').val();

        //armazena a largura e a altura da tela
        var maskHeight = $(document).height();
        var maskWidth = $(window).width();

        //Define largura e altura do div#mask iguais ás dimensões da tela
        $('#mask').css({'width':maskWidth,'height':maskHeight});

        //efeito de transição
        $('#mask').fadeIn(1000);
        $('#mask').fadeTo("slow",0.8);

        //armazena a largura e a altura da janela
        var winH = $(window).height();
        var winW = $(window).width();
        //centraliza na tela a janela popup
        $(janela).css('top',  winH/2-$(janela).height()/2);
        $(janela).css('left', winW/2-$(janela).width()/2);
        //efeito de transição
        $(janela).fadeIn(2000);

        $.ajax({
            url: 'ajax.php',
            type: 'post',
            data: 'requisicao=inserePontuacaoParametro&celula='+celula+'&co_tabela='+co_tabela,
            success: function(e){
                $('#modal_content').html(e);
            }
        });
    });

    //se o botãoo fechar for clicado
    $('.window .close').live('click',function (e) {
        //cancela o comportamento padrão do link
        e.preventDefault();
        $('#mask, .window').hide();
    });

    //se div#mask for clicado
    $('#mask').click(function () {
        $(this).hide();
        $('.window').hide();
    });

    $('.voltar').click(function(){
        document.location.href = 'pisosalarial.php?modulo=principal/folhapagamento/analise/tabelaParametros&acao=A';
    });

    $('.CampoEstilo').live('click',function(){

        name = this.name;
        value = this.value;

        if(name == 'variavel[]'){
            if(value == 'numerico'){
                $('#insere_valor').toggle();
                $('select[name=variavel[]]').attr('disabled', true);
                $('input[name=numerico]').focus();
                return false;
            }
        }

        // insere na div
        formula = $('#formula').html()+' '+value;
        $('#formula').html(trim(formula));

        // insere no campo hidden
        parametro = $('input[name=ds_parametro]').val()+' '+value;
        $('input[name=ds_parametro]').val(trim(parametro));

        if(name == 'variavel[]'){
            $('select[name=variavel[]]').attr('disabled', true);
            $('select[name=operador[]]').attr('disabled', false);
        }
        if(name == 'operador[]'){
            $('select[name=variavel[]]').attr('disabled', false);
            $('select[name=operador[]]').attr('disabled', true);
        }
    });

    $('.inserir').live('click', function(){

        value = $('input[name=numerico]').val();
        co_tabela = $('input[name=co_tabela]').val();

        if(value == ''){
            alert('Informe um valor!');
            $('input[name=numerico]').focus();
            return false;
        }
//        alert(co_tabela);
        var tabPorcentagens = new Array();
        tabPorcentagens = [4,5,6,'4','5','6'];
        if($.inArray(co_tabela, tabPorcentagens) > 0){
            value = value+'%';
        }

        // insere na div
        formula = $('#formula').html()+' '+value;
        $('#formula').html(trim(formula));

        // insere no campo hidden
        parametro = $('input[name=ds_parametro]').val()+' '+value;
        $('input[name=ds_parametro]').val(trim(parametro));

        $('#insere_valor').toggle();
        $('select[name=variavel[]]').attr('disabled', true);
        $('select[name=operador[]]').attr('disabled', false);
        $('input[name=numerico]').val('');

    });

    $('.salvaParametro').live('click', function(){

        celula = this.id;
        tabela = $('input[name=co_tabela]').val();
        coluna = $('input[name=nu_coluna]').val();
        pontuacao = $('input[name=nu_pontuacao]').val();
        formula = $('input[name=ds_parametro]').val();

        if(formula == ''){
            alert('Crie a fórmula primeiro!');
            return false;
        }
        arFormula = formula.split(' ');
        nuUltimo = arFormula.length-1;



        if(nuUltimo < 2){
            alert('Insira um operador.');
            return false;
        }

        if($.inArray(arFormula[nuUltimo], arOperadores) >= 0){
            alert('Informe uma variável!');
            return false;
        }

        if($('#insere_valor').css('display') != 'none'){
            if($('input[name=numerico]').val() == ''){
                alert('Informe um valor.');
                $('input[name=numerico]').focus();
                return false;
            }
        }

        $.ajax({
            url: 'ajax.php',
            type: 'post',
            data: 'requisicao=gravaParametro&co_tabela='+tabela+'&nu_coluna='+coluna+'&nu_pontuacao='+pontuacao+'&ds_parametro='+formula,
            success: function(e){
                if(e == 'true'){
                    $('#'+celula).html(formula);
                    $('#mask, .window').hide();
                    alert('Operação realizada com sucesso.');
                }
                if(e == 'existe'){
                    alert('Fórmula já cadastrada para esta coluna!');
                    $('input[name=ds_parametro]').val('');
                    $('#formula').html('&nbsp;');
                    $('select[name=variavel[]]').attr('disabled', false);
                    $('select[name=operador[]]').attr('disabled', true);
                    return false;
                }
                if(e == 'false'){
                    alert('Operação não realizada!');
                    return false;
                }
            }
        });
    });

    $('.parametro').attr('title', 'Clique aqui para inserir os parâmetros.');

    $('.limpar').live('click', function(){
        if(confirm('Deseja limpar os campos?')){
            $('#formula').html(' ');
            $('input[name=ds_parametro]').val();
            $('select[name=variavel[]]').attr('disabled', false);
            $('select[name=operador[]]').attr('disabled', true);
        }
    });

    $('.voltar_um').live('click',function(){

        formula = $('input[name=ds_parametro]').val();

        arFormula = trim(formula).split(' ');
        total = arFormula.length-1;

        var nova_formula = '';
        for(x=0;x<total;x++){
            nova_formula += arFormula[x]+' ';
        }

        nova_formula = trim(nova_formula);

        if($.inArray(arFormula[total], arOperadores) >= 0){
            $('select[name=variavel[]]').attr('disabled', true);
            $('select[name=operador[]]').attr('disabled', false);
        }else{
            $('select[name=variavel[]]').attr('disabled', false);
            $('select[name=operador[]]').attr('disabled', true);
        }

        $('#formula').html(nova_formula);
        $('input[name=ds_parametro]').val(nova_formula);

    });
});

</script>
<style>

    /* O z-index do div#mask deve ser menor que do div#boxes e do div.window */
    #mask {
        position:absolute;
        z-index:9000;
        background-color:#000;
        display:none;
        top:0px;
        left:0px;
    }

    #boxes .window {
        position:absolute;
        width:440px;
        height:200px;
        display:none;
        z-index:9999;
        padding:10px;
    }

    /* Personalize a janela modal aqui. Você pode adicionar uma imagem de fundo. */
    #boxes #dialog {
        width:375px;
        height:320px;
    }
    #dialog{
        background:white;
        overflow:auto;
    }

    /* posiciona o link para fechar a janela */
    .close {
        display:block;
        text-align:right;
    }

</style>
<?php if(empty($_POST['co_tabela'])): ?>
    <form id="formTipoPontuacao" method="post" action="">
        <table class="tabela" align="center" bgcolor="#f5f5f5" border="0" cellpadding="5" cellspacing="1">
            <tr>
                <td class="SubtituloCentro" colspan="2">Tipo de Pontuação</td>
            </tr>
            <tr>
                <td class="SubtituloDireita">Selecione uma planilha</td>
                <td><?
                    $sql = "SELECT co_tabela as codigo, ds_tabela as descricao FROM pisosalarial.parametrospontuacaotabelas WHERE st_tabela = 'A' ORDER BY nu_ordem";
                    $db->monta_combo("co_tabela", $sql, 'S', 'Selecione...', '', '', '', '265', 'S', '', '', '', 'Tabela' );
                ?></td>
            </tr>
        </table>
    </form>
<?php else: ?>
    <?php
    $arPontuacao  = ParametrosPontuacao::$_pontos;
    $rsTabela     = $obParametrosPontuacaoTabelas->recuperarTabelaPorId($_POST['co_tabela']);
    $rsParametros = $obParametrosPontuacao->recuperarPontuacaoPorTabela($_POST['co_tabela']);

    // Cria array de parâmetros
    $rsParametros = $rsParametros ? $rsParametros : array();
    foreach($rsParametros as $parametro){
        $indice = $parametro['nu_pontuacao'].$parametro['nu_coluna'];
        $arParametros[$indice] = $parametro['ds_parametro'];
    }
    $arParametros = $arParametros ? $arParametros : array();
    ?>
    <style>
    .grid{
        border: 1px solid #c0c0c0;
    }
    .grid td, th{
        border-left: 1px solid #c0c0c0;
        border-bottom: 1px solid #c0c0c0;
    }
    .parametro{
        cursor:pointer;
    }
    </style>
    <input type="hidden" id="co_tabela" name="co_tabela" value="<?php echo $_POST['co_tabela'] ?>" />
    <table class="tabela" align="center" bgcolor="#f5f5f5" border="0" cellpadding="5" cellspacing="1">
        <tr>
            <td class="SubtituloCentro"><?php echo $rsTabela['ds_tabela']; ?></td>
        </tr>
    </table>
    <?php if($_POST['co_tabela'] == TB_ANALISE_PARM_PAFR):?>
        <table class="tabela grid" align="center" bgcolor="#f5f5f5" border="0" cellpadding="5" cellspacing="1">
            <thead>
              <tr>
                <th width="2%">Pts</th>
                <th>Percentual de aplicação dos recursos vinculados à MDE na remuneração do magistério - PARM</th>
                <th>Percentual de aplicação dos recursos do FUNDEB na remuneração do magistério - PAFR</th>
              </tr>
            </thead>
            <tbody>
                <?php foreach($arPontuacao as $pontos):?>
                    <tr>
                        <td align="center" style="background:#CCCCCC;border-left: 0px solid black;"><?php echo $pontos ?></td>
                        <td align="center" class="parametro" id="P<?php echo $pontos; ?>_C1_PARM"><?php $indice = $pontos.'1'; echo !empty($arParametros[$indice]) ? $arParametros[$indice] : '&nbsp;'; ?></td>
                        <td align="center" class="parametro" id="P<?php echo $pontos; ?>_C2_PAFR"><?php $indice = $pontos.'2'; echo !empty($arParametros[$indice]) ? $arParametros[$indice] : '&nbsp;'; ?></td>
                    </tr>
                <?php endforeach; ?>
            </tbody>
        </table>
    <?php elseif( in_array($_POST['co_tabela'], array(TB_ANALISE_PMDE, TB_ANALISE_IFLEI)) ): ?>
        <table class="tabela grid" align="center" bgcolor="#f5f5f5" border="0" cellpadding="5" cellspacing="1">
            <thead>
              <tr>
                <th width="2%">Pts</th>
                <th><?php echo $rsTabela['ds_grid'] ?></th>
              </tr>
            </thead>
            <tbody>
                <?php foreach($arPontuacao as $pontos): ?>
                    <tr>
                        <td align="center" style="background:#CCCCCC;border-left: 0px solid black;"><?php echo $pontos ?></td>
                        <td align="center" class="parametro" id="P<?php echo $pontos; ?>_C1"><?php $indice = $pontos.'1'; echo !empty($arParametros[$indice]) ? $arParametros[$indice] : '&nbsp;'; ?></td>
                    </tr>
                 <?php endforeach; ?>
            </tbody>
        </table>
    <?php else: ?>
        <table class="tabela grid" align="center" bgcolor="#f5f5f5" border="0" cellpadding="5" cellspacing="1">
            <thead>
              <tr>
                <th rowspan="4" style="border-left: 0px solid black;">Pts</th>
                <th colspan="7"><?php echo $rsTabela['ds_grid'] ?></th>
              </tr>
              <tr>
                <th colspan="2" rowspan="2">Educação infantil <br/>(urbana e rural)</th>
                <th colspan="4">Ensino Fundamental</th>
                <th rowspan="3">Ensino médio <br/>(urbana e rural)</th>
              </tr>
              <tr>
                <th colspan="2">Urbano</th>
                <th colspan="2">Rural</th>
              </tr>
              <tr>
                <th>Creche</th>
                <th>Pré-Escola</th>
                <th>Séries Iniciais</th>
                <th>Séries Finais</th>
                <th>Séries Iniciais</th>
                <th>Séries Finais</th>
              </tr>
            </thead>
            <tbody>
                <?php foreach($arPontuacao as $pontos):?>
                    <tr>
                        <td align="center" style="background:#CCCCCC;border-left: 0px solid black;"><?php echo $pontos ?></td>
                        <td width="14%" align="center" class="parametro" id="P<?php echo $pontos; ?>_C1"><?php $indice = $pontos.'1'; echo !empty($arParametros[$indice]) ? $arParametros[$indice] : '&nbsp;'; ?></td>
                        <td width="14%" align="center" class="parametro" id="P<?php echo $pontos; ?>_C2"><?php $indice = $pontos.'2'; echo !empty($arParametros[$indice]) ? $arParametros[$indice] : '&nbsp;'; ?></td>
                        <td width="14%" align="center" class="parametro" id="P<?php echo $pontos; ?>_C3"><?php $indice = $pontos.'3'; echo !empty($arParametros[$indice]) ? $arParametros[$indice] : '&nbsp;'; ?></td>
                        <td width="14%" align="center" class="parametro" id="P<?php echo $pontos; ?>_C4"><?php $indice = $pontos.'4'; echo !empty($arParametros[$indice]) ? $arParametros[$indice] : '&nbsp;'; ?></td>
                        <td width="14%" align="center" class="parametro" id="P<?php echo $pontos; ?>_C5"><?php $indice = $pontos.'5'; echo !empty($arParametros[$indice]) ? $arParametros[$indice] : '&nbsp;'; ?></td>
                        <td width="14%" align="center" class="parametro" id="P<?php echo $pontos; ?>_C6"><?php $indice = $pontos.'6'; echo !empty($arParametros[$indice]) ? $arParametros[$indice] : '&nbsp;'; ?></td>
                        <td width="14%" align="center" class="parametro" id="P<?php echo $pontos; ?>_C7"><?php $indice = $pontos.'7'; echo !empty($arParametros[$indice]) ? $arParametros[$indice] : '&nbsp;'; ?></td>
                    </tr>
                <?php endforeach; ?>
            </tbody>
         </table>
     <?php endif; ?>
     <table class="tabela" align="center" bgcolor="#f5f5f5" border="0" cellpadding="5" cellspacing="1">
        <tr bgcolor="#cccccc">
           <td align="center">
                <!-- <input type="button" value="Salvar" /> -->
                <input type="button" value="Voltar" class="voltar" />
            </td>
        </tr>
     </table>
<?php endif; ?>
<div id="boxes">
    <div id="dialog" class="window">
        <div id="modal_content" style="border: 1px solid #c0c0c0">
        </div>
    </div>

    <div id="mask"></div>
</div>