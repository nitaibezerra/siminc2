<?php

// Includes necessários
include_once APPPISO . 'classes/ParametrosPontuacaoTabelas.class.inc';
include_once APPPISO . 'classes/ParametrosPontuacao.class.inc';
include_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

// Objetos
$obParametrosPontuacao = new ParametrosPontuacao();
$obParametrosPontuacaoTabelas = new ParametrosPontuacaoTabelas();

// Cabeçalho
monta_titulo( 'Complementação do Piso Salarial', 'Folha de Pagamento' );
montaCabecalho();

$arPontuacao  = ParametrosPontuacao::$_pontos;
$rsTabela     = $obParametrosPontuacaoTabelas->recuperarTabelaPorId($_POST['co_tabela']);
$rsParametros = $obParametrosPontuacao->recuperarPontuacaoPorTabela($_POST['co_tabela']);

// Cria array de parâmetros
$rsParametros = $rsParametros ? $rsParametros : array();
foreach($rsParametros as $parametro){
    $indice = $parametro['nu_pontuacao'].$parametro['nu_coluna'];
    $arParametros[$indice] = $parametro['ds_parametro'];
}

$valor = 0;
$arPontos = array('false','false','false','false','false','false','false');
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">
<!--
$(document).ready(function(){
    $('.voltar').click(function(){
        $('#formVoltar').attr('action','pisosalarial.php?modulo=principal/folhapagamento/analise/resultadoGeral&acao=A');
        $('#formVoltar').submit();
    });
});
//-->
</script>
<style>
.grid{
    border: 1px solid #c0c0c0;
}
.grid td, th{
    border-left: 1px solid #c0c0c0;
    border-bottom: 1px solid #c0c0c0;
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
            <th width="2%"><span title="<?php echo $_REQUEST['nu_indice'];?>" alt="<?php echo $_REQUEST['nu_indice'];?>">Pts</span></th>
            <th>Percentual de aplicação dos recursos vinculados à MDE na remuneração do magistério - PARM</th>
            <th>Percentual de aplicação dos recursos do FUNDEB na remuneração do magistério - PAFR</th>
          </tr>
        </thead>
        <tbody>
            <?php foreach($arPontuacao as $pontos):?>
                <tr>
                    <td align="center" style="background:#CCCCCC;border-left: 0px solid black;"><?php echo $pontos ?></td>
                    <td width="14%" align="center" class="parametro" id="P<?php echo $pontos; ?>_C1">
                        <?php
                        $indice = $pontos.'1';
                        $arParams = explode(' ',$arParametros[$indice]);
                        foreach($arParams as $param){
                            if(strpos($param, '%') > 0){
                                $monta_formula = str_replace('%', '', $param)/100;
                            }else{
                                $monta_formula = $param;
                            }
                            $formula .= trim($monta_formula)." ";
                        }
                        $arIndice = explode('_',$_REQUEST['nu_indice']);
                        if($arPontos[1] == 'false'){
                            verificaCondicaoParametroPiso($formula, 'PARM', $arIndice[0], 1, $pontos, $valor, $arPontos);
                        }else{
                            echo '&nbsp;';
                        }
                        unset($formula, $arParams, $monta_formula);
                        ?>
                    </td>
                    <td width="14%" align="center" class="parametro" id="P<?php echo $pontos; ?>_C2">
                        <?php
                        $indice = $pontos.'2';
                        $arParams = explode(' ',$arParametros[$indice]);
                        foreach($arParams as $param){
                            if(strpos($param, '%') > 0){
                                $monta_formula = str_replace('%', '', $param)/100;
                            }else{
                                $monta_formula = $param;
                            }
                            $formula .= trim($monta_formula)." ";
                        }

                        if($arPontos[2] == 'false'){
                            verificaCondicaoParametroPiso($formula, 'PAFR', $arIndice[1], 2, $pontos, $valor, $arPontos);
                        }else{
                            echo '&nbsp;';
                        }
                        unset($formula, $arParams, $monta_formula);?>
                    </td>
                </tr>
            <?php endforeach; ?>
        </tbody>
    </table>
<?php elseif( in_array($_POST['co_tabela'], array(TB_ANALISE_PMDE, TB_ANALISE_IFLEI)) ): ?>
    <table class="tabela grid" align="center" bgcolor="#f5f5f5" border="0" cellpadding="5" cellspacing="1">
        <thead>
          <tr>
            <th width="2%"><span title="<?php echo $_REQUEST['nu_indice'];?>" alt="<?php echo $_REQUEST['nu_indice'];?>">Pts</span></th>
            <th><?php echo $rsTabela['ds_grid'] ?></th>
          </tr>
        </thead>
        <tbody>
            <?php foreach($arPontuacao as $pontos): ?>
                <tr>
                    <td align="center" style="background:#CCCCCC;border-left: 0px solid black;"><?php echo $pontos ?></td>
                    <td width="14%" align="center" class="parametro" id="P<?php echo $pontos; ?>_C1">
                        <?php
                        $indice = $pontos.'1';
                        $arParams = explode(' ',$arParametros[$indice]);
                        foreach($arParams as $param){
                            if(strpos($param, '%') > 0){
                                $monta_formula = str_replace('%', '', $param)/100;
                            }else{
                                $monta_formula = $param;
                            }
                            $formula .= trim($monta_formula)." ";
                        }
                        if($arPontos[1] == 'false'){
                            verificaCondicaoParametroPiso($formula, $rsTabela['sg_tabela'], $_REQUEST['nu_indice'], 1, $pontos, $valor, $arPontos);
                        }else{
                            echo '&nbsp;';
                        }
                        unset($formula, $monta_formula, $arParams);
                        ?>
                    </td>
                </tr>
             <?php endforeach; ?>
        </tbody>
    </table>
<?php else: ?>
    <table class="tabela grid" align="center" bgcolor="#f5f5f5" border="0" cellpadding="5" cellspacing="1">
        <thead>
          <tr>
            <th rowspan="4" style="border-left: 0px solid black;"><span title="<?php echo $_REQUEST['nu_indice'];?>" alt="<?php echo $_REQUEST['nu_indice'];?>">Pts</span></th>
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
                    <td width="14%" align="center" class="parametro" id="P<?php echo $pontos; ?>_C1">
                        <?php
                        $indice = $pontos.'1';
                        if($arPontos[1] == 'false'){
                            verificaCondicaoParametroPiso($arParametros[$indice], $rsTabela['sg_tabela'], $_REQUEST['nu_indice'], 1, $pontos, $valor, $arPontos);
                        }else{
                            echo '&nbsp;';
                        }
                        ?>
                    </td>
                    <td width="14%" align="center" class="parametro" id="P<?php echo $pontos; ?>_C2">
                        <?php
                        $indice = $pontos.'2';
                        if($arPontos[2] == 'false'){
                            verificaCondicaoParametroPiso($arParametros[$indice], $rsTabela['sg_tabela'], $_REQUEST['nu_indice'], 2, $pontos, $valor, $arPontos);
                        }else{
                            echo '&nbsp;';
                        }
                        ?>
                    </td>
                    <td width="14%" align="center" class="parametro" id="P<?php echo $pontos; ?>_C3">
                        <?php
                        $indice = $pontos.'3';
                        if($arPontos[3] == 'false'){
                            verificaCondicaoParametroPiso($arParametros[$indice], $rsTabela['sg_tabela'], $_REQUEST['nu_indice'], 3, $pontos, $valor, $arPontos);
                        }else{
                            echo '&nbsp;';
                        }
                        ?>
                    </td>
                    <td width="14%" align="center" class="parametro" id="P<?php echo $pontos; ?>_C4">
                        <?php
                        $indice = $pontos.'4';
                        if($arPontos[4] == 'false'){
                            verificaCondicaoParametroPiso($arParametros[$indice], $rsTabela['sg_tabela'], $_REQUEST['nu_indice'], 4, $pontos, $valor, $arPontos);
                        }else{
                            echo '&nbsp;';
                        }
                        ?>
                    </td>
                    <td width="14%" align="center" class="parametro" id="P<?php echo $pontos; ?>_C5">
                        <?php
                        $indice = $pontos.'5';
                        if($arPontos[5] == 'false'){
                            verificaCondicaoParametroPiso($arParametros[$indice], $rsTabela['sg_tabela'], $_REQUEST['nu_indice'], 5, $pontos, $valor, $arPontos);
                        }else{
                            echo '&nbsp;';
                        }
                        ?>
                    </td>
                    <td width="14%" align="center" class="parametro" id="P<?php echo $pontos; ?>_C6">
                        <?php
                        $indice = $pontos.'6';
                        if($arPontos[6] == 'false'){
                            verificaCondicaoParametroPiso($arParametros[$indice], $rsTabela['sg_tabela'], $_REQUEST['nu_indice'], 6, $pontos, $valor, $arPontos);
                        }else{
                            echo '&nbsp;';
                        }
                        ?>
                    </td>
                    <td width="14%" align="center" class="parametro" id="P<?php echo $pontos; ?>_C7">
                        <?php
                        $indice = $pontos.'7';
                        if($arPontos[7] == 'false'){
                            verificaCondicaoParametroPiso($arParametros[$indice], $rsTabela['sg_tabela'], $_REQUEST['nu_indice'], 7, $pontos, $valor, $arPontos);
                        }else{
                            echo '&nbsp;';
                        }
                        ?>
                    </td>
                </tr>
            <?php endforeach; ?>
        </tbody>
     </table>
 <?php endif; ?>
 <table class="tabela grid" align="center" bgcolor="#f5f5f5" border="0" cellpadding="5" cellspacing="1">
    <tr>
        <td class="SubtituloDireita">Total</td>
        <td width="60"><?php echo $valor ?></td>
    </tr>
 </table>
 <table class="tabela" align="center" bgcolor="#f5f5f5" border="0" cellpadding="5" cellspacing="1">
    <tr bgcolor="#cccccc">
       <td align="center">
            <!-- <input type="button" value="Salvar" /> -->
            <input type="button" value="Voltar" class="voltar" />
        </td>
    </tr>
 </table>
<form id="formVoltar" method="post" action="">
    <input type="hidden" name="muncod" value="<?php echo $_SESSION['piso']['muncod'] ?>" />

    <input type="hidden" name="parm" value="<?php echo $_POST['parm'] ?>">
    <input type="hidden" name="pafr" value="<?php echo $_POST['pafr'] ?>">
    <input type="hidden" name="pmde" value="<?php echo $_POST['pmde'] ?>">
    <input type="hidden" name="iflei" value="<?php echo $_POST['iflei'] ?>">
</form>