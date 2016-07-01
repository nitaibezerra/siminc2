<?php

// captura dados do projeto da requisição
//$atiid = $_REQUEST['atiid'] ? (integer) $_REQUEST['atiid'] : PROJETO;

// captura o id da estrutura
$obrid  = ($_REQUEST['obrid'] ? $_REQUEST['obrid'] : $_SESSION['obras2']['obrid']);

// captura o id da etapa
$icoid  = ($_REQUEST['icoid'] ? $_REQUEST['icoid'] : $_SESSION['obras2']['icoid']);

// captura o id do detalhamento
$ditid  = ($_REQUEST['ditid'] ? $_REQUEST['ditid'] : $_SESSION['obras2']['ditid']);
$_SESSION['obras2']['ditid'] = $ditid;

if ( empty($icoid) || empty($obrid) || empty($ditid) ){
    die("<script>
            alert('Faltam parametros para acessar esta tela!');
            history.go(-1);
         </script>");
}

$detalheitem = new DetalheItem( $ditid );

if ( $detalheitem->ditstatus != 'A' ){
    die("<script>
            alert('Detalhamento inexistente!');
            history.go(-1);
         </script>");
}

$arIdAllChildren = $detalheitem->getIdAllChildren($ditid);

if ( $_POST['evt'] == 'salvar' ){
    $prev = new PrevisaoDetalheItem();
    $prev->desabilitar($ditid);

    foreach ($_POST['pdivalor'] as $ano => $dado){
        foreach ($dado as $mes => $pdivalor){
            $pdimetafisica = $_POST['pdimetafisica'][$ano][$mes];
            $mes 		   = str_pad($mes, 2, "0", STR_PAD_LEFT);
            $arDados = array('ditid'         => $ditid,
                             'pdivalor'      => ($pdivalor ? desformata_valor( $pdivalor ) : '0.00'),
                             'pdimetafisica' => ($pdimetafisica ? desformata_valor( $pdimetafisica ) : '0.00'),
                             'pdimes'        => "{$ano}-{$mes}-01");
            $prev->popularDadosObjeto( $arDados )->salvar();
            $prev->clearDados();
        }
    }
    $db->commit();

//    $vlrObra = new ValorMensalObra();
//    $vlrObra->calculaCronograma($atiid);

    die("<script>
            alert('Operação realizada com sucesso!');
            window.location = '?modulo=principal/previsao_valor_detalhe&acao=A';
         </script>");
}

// Inclusão de arquivos padrão do sistema
include APPRAIZ . 'includes/cabecalho.inc';
// Cria as abas do módulo
echo '<br>';
$db->cria_aba(ID_ABA_PREVISAO_DETALHAMENTO_OBRA,$url,$parametros);

echo cabecalhoObra($obrid);
echo '<br>';

// Cria o título da tela
monta_titulo( $titulo_modulo, '<b>Após preencher os valores, clique em salvar.</b>' );

//echo cabecalhoEstrutura($atiid,$obrid,$icoid,$ditid);
echo cabecalhoCronograma( $obrid, $icoid, $ditid );

$unidadeMedida = new UnidadeMedidaCronograma( $detalheitem->umcid );

$arResult = array();
$dataIni = date('n-Y', strtotime($detalheitem->ditdatainicio));
list($mesIni, $anoIni) = explode("-", $dataIni);


$dataFim = date('n-Y', strtotime($detalheitem->ditdatatermino));
list($mesFim, $anoFim) = explode("-", $dataFim);

$prev = new PrevisaoDetalheItem();
while( ($mesIni <= $mesFim && $anoIni == $anoFim) || ($anoIni < $anoFim) ){
    $valor = $prev->getValor(($arIdAllChildren ? $arIdAllChildren : $ditid), $anoIni, $mesIni);
    $valor = number_format($valor,2,',','.');
    $mesExtenso = mes_extenso( $mesIni );
    $habil = ( count($arIdAllChildren) > 0 ? 'N' : 'S' );

    $arResult[$anoIni][$mesIni] = "<tr>
                                    <td>{$mesExtenso} de {$anoIni}</td>
                                    <td>" . (campo_texto( 'pdivalor[' . $anoIni . '][' . $mesIni . ']', 'N', $habil, '', 10, 14, '', '', 'right', '', 0, 'onkeypress="calculoDetalhamento.setDefaultValue( this )";', 'calculoDetalhamento.manager( this );', $valor)) . "</td>";

    if ( $unidadeMedida->umcdsc && empty($arIdAllChildren) ){
        $pdimetafisica = $prev->getMetaFisica($ditid, $anoIni, $mesIni);
        $pdimetafisica = number_format($pdimetafisica, 2, ',', '.');
        $arResult[$anoIni][$mesIni] .= " <td>" . (campo_texto( 'pdimetafisica[' . $anoIni . '][' . $mesIni . ']', 'N', $habil, '', 10, 14, '', '', 'right', '', 0, 'onkeypress="calculoMetaFisica.setDefaultValue( this )";', 'calculoMetaFisica.manager( this );', $pdimetafisica)) . "</td>
                                        <td>" . $unidadeMedida->umcdsc . "</td>";
    }

    $arResult[$anoIni][$mesIni] .= "</tr>";

    if ($mesIni < 12){
        $mesIni++;
    }else{
        $anoIni++;
        $mesIni = 1;
    }
}

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<form method="post" id="formPrevisao">
<input type="hidden" name="evt" id="evt" value="salvar">
<input type="hidden" name="ditid" id="ditid" value="<?php echo $detalheitem->ditid; ?>">
<input type="hidden" name="ditvalor" id="ditvalor" value="<?php echo number_format($detalheitem->ditvalor,2,',','.'); ?>">
<input type="hidden" name="ditmetafisica" id="ditmetafisica" value="<?php echo number_format($detalheitem->ditmetafisica, 2, ',', '.') ?>">
<table class="tabela" bgcolor="#F5F5F5" cellSpacing="1" cellPadding="3" align="center">
<?php
if ( count($arIdAllChildren) > 0 ){
?>
    <tr>
        <td align="center" style="color:red;">
            Os valores exibidos tratam-se da soma dos sub itens deste item.
            <br>
            Para alterar os valores deste item edite os seus sub itens.
        </td>
    </tr>
<?php
}
?>
<?php
if ( $unidadeMedida->umcdsc && empty($arIdAllChildren) ){
?>
    <tr>
        <td align="center" style="color:red;">
            A meta física do detalhamento é de: <?php echo number_format($detalheitem->ditmetafisica, 2, ',', '.') . ' ' .  $unidadeMedida->umcdsc?>
            <br>
            A soma dos campos "Meta Física Prevista" não deve ultrapassá-la.
        </td>
    </tr>
<?php
}
?>
    <tr>
        <td>
<?php
        foreach ($arResult as $grupoAno){
?>
            <center>
            <table class="prevTable">
                <tr>
                    <th>Mês</th>
                    <th>&nbsp;Valor Previsto (R$)&nbsp;</th>
<?php
            if ( $unidadeMedida->umcdsc && empty($arIdAllChildren) ){
?>
                    <th>&nbsp;Meta Física Prevista&nbsp;</th>
                    <th>&nbsp;Unidade de Medida&nbsp;</th>
<?php
            }
?>
                </tr>
                <?php echo implode('', $grupoAno); ?>
            </table>
            </center>
<?php
        }
?>
        </td>
    </tr>

    <tr bgcolor="#C0C0C0">
        <td>
            <div style="float: left;">
    <?php
if ( count($arIdAllChildren) == 0 ){
?>
                <input type="submit" id="salvar" value="Salvar" style="cursor: pointer">
<?php
}
?>
                <input type="button" value="Voltar" style="cursor: pointer" onclick="location.href='?modulo=principal/detalhe_da_etapa&acao=A'">
            </div>
        </td>
    </tr>
    
</table>
</form>

<script>
function calculoMetaFisica(){
    var obCalc       = new Calculo();
    var percentTotal = new Number(0);
    var itemEdicao        = '';
    var vlrItemAnterior   = '';

    this.manager = function (obj){
        obj = obj || '';

        if ( obj != '' ){
            formatarNumeroMonetario( obj );
        }

        calculaTotal();

        if ( obj != '' ){
            formatarNumeroMonetario( obj );
        }
    }

    this.setDefaultValue = function ( obj ){
        if ( obj != itemEdicao ){
            itemEdicao      = obj;
            vlrItemAnterior = obj.value;
        }
    }

    function calculaTotal(){
        var vlrTotal = new String('0');

        $('[name*=pdimetafisica]').each( function (i, objItem){
            var vlrItem = $(objItem).val();

            vlrTotal = obCalc.operacao(vlrTotal, vlrItem, '+');
            vlrTotal = mascaraglobal("[###.]###,##", vlrTotal);

        });

        if ( obCalc.comparar(vlrTotal, $('#ditmetafisica').val(), '>') ){
            alert('A soma dos campos "Meta Física Prevista" excede o valor da "Meta Física" do item de detalhamento!');
            itemEdicao.value = vlrItemAnterior;
        }

    }

    function formatarNumeroMonetario( obj ){
        var valor = new String( obCalc.retiraZeroEsquerda(obj.value) );

        if ( valor.length == 2 ){
            valor = '0' + valor;
        }else if ( valor.length == 1 ){
            valor = '00' + valor;
        }else if ( valor.length == 0 ){
            valor = '000';
        }

        obj.value = mascaraglobal("[###.]###,##", valor);
        return obj.value;
    }

}

calculoMetaFisica = new calculoMetaFisica();

function calculoDetalhamento(){
    var obCalc       = new Calculo();
    var percentTotal = new Number(0);
    var itemEdicao        = '';
    var vlrItemAnterior   = '';

    this.manager = function (obj){
        obj = obj || '';

        if ( obj != '' ){
            formatarNumeroMonetario( obj );
        }

        calculaTotal();

        if ( obj != '' ){
            formatarNumeroMonetario( obj );
        }
//        calculaRestante();
    }

    this.setDefaultValue = function ( obj ){
        if ( obj != itemEdicao ){
            itemEdicao      = obj;
            vlrItemAnterior = obj.value;
        }
    }

    function calculaTotal(){
        var vlrTotal = new String('0');

        $('[name*=pdivalor]').each( function (i, objItem){
        	var vlrItem = $(objItem).val();

            vlrTotal = obCalc.operacao(vlrTotal, vlrItem, '+');
            vlrTotal = mascaraglobal("[###.]###,##", vlrTotal);

        });

        if ( obCalc.comparar(vlrTotal, $('#ditvalor').val(), '>') ){
            alert('A soma dos valores excede o valor do item de detalhamento!');
            itemEdicao.value = vlrItemAnterior;
        }

    }

    function formatarNumeroMonetario( obj ){
        var valor = new String( obCalc.retiraZeroEsquerda(obj.value) );

        if ( valor.length == 2 ){
            valor = '0' + valor;
        }else if ( valor.length == 1 ){
            valor = '00' + valor;
        }else if ( valor.length == 0 ){
            valor = '000';
        }

        obj.value = mascaraglobal("[###.]###,##", valor);
        return obj.value;
    }

}

calculoDetalhamento = new calculoDetalhamento();

$('.prevTable tr:even').css('background', '#CCCCCC');
$('.prevTable tr:odd').css('background', '#FFFFFF');
</script>

<style>
.prevTable{
<?php
    if ( count($arResult) > 1 ):
?>
    float : left;
    margin-left : 33px;
    margin-bottom : 15px;
<?php
    endif;
?>
}

.prevTable th{
    border-top : 1px solid #000000;
}
.prevTable td{
    padding : 3px;
    text-align : center;
}

</style>