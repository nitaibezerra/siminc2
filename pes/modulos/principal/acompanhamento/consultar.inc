<?php

//Chamada de programa
include APPRAIZ . "includes/cabecalho.inc";

// Exibindo barra com os dados recursivos da entidade.
$controllerUsuario = new Controller_Usuario();
echo $controllerUsuario->barraEntidadeUsuarioAction();
echo '<br />';

monta_titulo($titulo_modulo, '');


if($controllerUsuario->user()->superuser &&  isset($_REQUEST['codigo_entidade']) && !empty($_REQUEST['codigo_entidade'])){
    $entCodigo = $_REQUEST['codigo_entidade'];
    $ano = $_REQUEST['ano'];
    
    $model = new Model_Entidade();
    $entidade = $model->getByValues(array('entcodigo' => $entCodigo));
    monta_titulo('Entidade: ' . $entidade['entnome'], '&nbsp;');
    
} else  {
    $ano = null;
    $entCodigo = $_SESSION['pes']['codigo_entidade'];
}


$model = new Model_Validacao();
$result = $model->carregarDespesasPorEntidade($entCodigo , $ano);

$despesasTr = array();
$despesasTh = array();

if ($result) {

    $arrStatusMes = array();
    foreach ($result as $arrValue) {
        $despesasTr[$arrValue['tidcodigo']][$arrValue['lcocodigo']] = $arrValue;
        $despesasTh[$arrValue['lcocodigo']]['nome'] = $arrValue['lconome'];
        $despesasTh[$arrValue['lcocodigo']]['codigo'] = $arrValue['lcocodigo'];
        $arrStatusMes[$arrValue['lcocodigo']][$arrValue['tidcodigo']] = $arrValue['status'];
    }

    foreach ($arrStatusMes as $codigoMes => $arrStatus) {
        $classThAnterior = '';
        foreach ($arrStatus as $status) {

            if ($classThAnterior) {
                if ($status != $classThAnterior) {
                    $despesasTh[$codigoMes]['status'] = 'NI';
                }
            } else {
                $despesasTh[$codigoMes]['status'] = $status;
            }

            $classThAnterior = $status;
        }
    }
}

function getClassByStatus($status) {
    if ($status == 'REP') {
        $class = 'red';
    } else if ($status == 'APR') {
        $class = 'green';
    } else {
        $class = 'white';
    }

    return $class;
}

//ver($result);
?>
<style>
    .tabela_filha{
        border-collapse: collapse;
        border: 0px solid red; padding: 0px; margin: 0px;
        width: 100%;
    }
    .td_acao {
        /*border: 2px solid blue;*/
        border-right: 2px solid #e0e0e0;
        border-bottom: 2px solid #e0e0e0;
        padding: 0px;
        margin: 0px;
    }

    .td_nome {
        /*border: 2px solid blue;*/
        border-right: 0px solid #e0e0e0;
        border-bottom: 2px solid #e0e0e0;
        padding: 0px;
        margin: 0px;
    }

    /*.tabela_filha tbody {border: 1px solid blue; padding: 0px; margin: 0px;}*/
    .tabela_filha tr {
        border: 0px solid red;
        padding: 0px;
        margin: 0px;
        width: 100%;
        height: 20px;
        background-color: #fafafa;
    }

    .linha_listagem{
        background-color: #fafafa;
    }

    .linha_listagem:hover{
        background-color: #ffffcc;
    }

    a {
        color: #000;
    }

    a:hover{
        color: #000;
        text-decoration:none;
    }

    thead tr{
        background-color: #e0e0e0;
        text-align: center;
    }

    tbody .list:hover {
        background-color: #e0e0e0 !important;
    }

    td .red:hover{
        background-color: #FF6A6A;
    }

    td .green:hover{
        background-color: #7CCD7C;
    }

    td .red{
        background-color: #FFC1C1;
    }

    td .green{
        background-color:#90EE90;
    }

    td .white{
        background-color:#FFFFFF;
    }

    td .white:hover{
        background-color:#EEE9E9;
    }
</style>
<table align="center" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
    <?php if ($result): ?>
        <thead>
            <tr>
                <td style=" width: 300px;">Nome da Despesa</td>
                <?php foreach ($despesasTh as $despesaTh): $class = getClassByStatus($despesaTh['status']); ?>
                    <td style="background-color: #e0e0e0;" class="mes_<?php echo $despesaTh['codigo'] . ' ' . $class ?>" onclick="javascript:checkTr(this);"><?php echo $despesaTh['nome'] ?></td>
                <?php endforeach ?>
                <td style="background-color: #e0e0e0;" class="total_despesa">Total</td>
            </tr>
        </thead>
        <tbody>
            <?php
//                        ver($despesasTr);
            $totalColuna = array();
            foreach ($despesasTr as $despesaTr):
                $nColuna = 0;
                ?>
                <tr class="list" >
                    <?php
                    $totalLinha = 0;
                    foreach ($despesaTr as $keyDespesaColuna => $valueDespesaColuna):

                        $totalLinha += $valueDespesaColuna['total'];
                        $totalColuna[$keyDespesaColuna] += $valueDespesaColuna['total'];

                        $class = getClassByStatus($valueDespesaColuna['status']);
                        ?>
                        <?php if ($nColuna < 1): $nColuna++ ?>
                            <td style="text-align: left; background-color: #e0e0e0;"><?php echo $valueDespesaColuna['tidnome'] ?></td>
                        <?php endif ?>
                        <td style="text-align: right;" id="<?php echo "{$nColuna}_{$valueDespesaColuna['lcocodigo']}_{$valueDespesaColuna['tidcodigo']}_{$valueDespesaColuna['valcodigo']}" ?>" onclick="javascript:checkTd(this);" class="mes_<?php echo $valueDespesaColuna['lcocodigo'] . ' ' . $class ?> "><?php echo ($valueDespesaColuna['total']) ? number_format($valueDespesaColuna['total'], 2, ',', '.') : '-' ?></td>
                    <?php endforeach ?>
                    <td style="text-align: right; font-weight: bold;"><?php echo ($totalLinha) ? number_format($totalLinha, 2, ',', '.') : '-' ?></td>
                </tr>
            <?php endforeach ?>
            <tr>
                <td style=" width: 300px; font-weight: bold;">Total</td>
                <?php foreach ($despesaTr as $keyDespesaColuna => $valueDespesaColuna): ?>
                    <td style="text-align: right; font-weight: bold;"><?php echo ($totalColuna[$keyDespesaColuna]) ? number_format($totalColuna[$keyDespesaColuna], 2, ',', '.') : '-' ?></td>
                <?php endforeach ?>
                <td style="text-align: right; background-color: #e0e0e0; font-weight: bold;"><?php echo number_format(array_sum($totalColuna), 2, ',', '.'); ?></td>
            </tr>
    <!--                        <tr>
                <td style="background-color: #e0e0e0; width: 300px;"></td>
            <?php foreach ($despesasTh as $despesaTh): $class = getClassByStatus($despesaTh['status']); ?>
                        <td style="text-align:center; background-color: #e0e0e0;" class="mes_<?php echo $despesaTh['codigo'] . ' ' . $class ?>">
                            <input type="button" value="Validar" onclick="javascript:confirmar('<?php echo $despesaTh['codigo'] ?>' , '<?php echo $despesaTh['status'] ?>')"/>
                        </td>
            <?php endforeach ?>
                <td style="background-color: #e0e0e0;" class="total_despesa"></td>
            </tr>-->
        </tbody>
        <table align="center" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" >
                        <tr>
                            <td style="border-width: 3px;" style="width: 32%;">
                                <fieldset>
                                    <legend>Legenda:</legend>
                                    <table>
                                        <tr>
                                            <td class="white" style="cursor: default">BRANCO OU INCOLOR: Não validado pelo Líder da UO</td>
                                        </tr>
                                        <tr>
                                            <td class="green" style="cursor: default">VERDE: Validado pelo Líder de UO</td>
                                        </tr>
                                        <tr>
                                            <td class="red" style="cursor: default">VERMELHO: Recusado pelo Líder da UO</td>
                                        </tr>
                                    </table>
                                </fieldset>
                            </td>
                            <td style="width: 78%;" valign="top"> 
                                <p>
                                    <b>Observação:</b>
                                    <br />
                                    A tabela acima apresenta um painel gerencial para que o Líder da UO (Pró Reitor/Reitor) possa validar/ratificar ou recusar as informações inseridas pela sua equipe de cadastradores.
                                    <br />
                                    Para validar os valores é muito simples, um clique sobre o valor e pronto! Sua despesa naquele mês já passou pelo crivo da autoridade competente (Líder de UO). Tal clique "pintará" a célula de verde.
                                    <br />
                                    É importante também atentar-se para a legenda, posicionada logo abaixo da tabela, com o indicativo das cores, pois a quantidade de cliques altera o status da despesa, entre <b class="green" style="cursor: default">validado (verde)</b>, <b class="red" style="cursor: default">recusado (vermelho)</b> e <b class="white">não validado (incolor ou branco)</b>, nessa ordem.
                                    <br />
                                    Caso queira validar todo o mês, é só clicar sobre aquele mês e pronto, você terá validado todas as despesas daquele mês com um único "clique", lembre-se que a quantidade de cliques altera o status.
                                    <br />
                                    A validação deverá ser feita mesmo para àquelas despesas que não possuem valor (zerado) ou de despesa que não tenha sido pactuada, pois nesse caso o Líder de UO se posiciona de acordo com o lançamento (ou ausência desse).
                                    <br />
                                    Para aqueles casos em que o Líder discorde dos valores informados pela equipe de cadastradores, esse poderá enviar um "parecer" para a equipe solicitando a retificação dos dados, bastando para isso, clicar no botão "Enviar para correção" (posicionado ao centro da parte inferior à tabela).
                                    <br />
                                    Assim que o mês for validado completamente <b class="green" style="cursor: default">(totalmente verde)</b> a SPO/MEC ficará ciente da validação e o papel do Líder de UO terá sido cumprido, naquele mês!
                                </p>
                            </td>
                        </tr>
                    </table>
    <?php else: ?>
        <tr>
            <td style="background-color: #ffffcc; text-align: center; color: red;" >Não possui despesas!</td>
        </tr>
    <?php endif ?>
        <tr>
            <td>
                <table style="width: 80%;" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
                    <tr>
                        <th colspan="2" style="text-align: left;">Notas:</th>
                    </tr>
                    <tr>
                        <td align="justify" style="border:thin; border-style:solid; border-color:#BEBEBE;width:50%">
                            <?php echo textoUltimaCargaSIAFI(); ?>
                        </td>
                        <td>&nbsp;</td>
                    </tr>
                </table>
            </td>
        </tr>
</table>