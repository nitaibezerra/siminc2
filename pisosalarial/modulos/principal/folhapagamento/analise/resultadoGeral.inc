<?php

// Includes necessários
include_once APPRAIZ . 'includes/cabecalho.inc';
include_once APPPISO . 'classes/ParametrosPontuacaoTabelas.class.inc';
include_once APPPISO . 'classes/FolhaPagamentoProfissionais.class.inc';
print '<br/>';

// Objetos
$obPontuacaoTabelas = new ParametrosPontuacaoTabelas();
$obFolhaProfissionais = new FolhaPagamentoProfissionais();

// Cabeçalho
monta_titulo( 'Complementação do Piso Salarial', 'Folha de Pagamento' );
montaCabecalho();

// Resultados
$rsJornadaProfessor     = $obFolhaProfissionais->recuperarJornadaProfissionais(PISO_CARGO_PROFESSOR);
$rsJornadaDocencia      = $obFolhaProfissionais->recuperarJornadaProfissionais(PISO_CARGO_APOIO_DOCENCIA);
$rsJornadaEscolar       = $obFolhaProfissionais->recuperarJornadaProfissionais(PISO_CARGO_APOIO_ESCOLAR);
$rsAnosReferencia       = $obFolhaProfissionais->recuperarAnosReferencia();
$rsTabelas              = $obPontuacaoTabelas->recuperarTodos();

$arAnos = array('2009'=>'2009',
                '2010'=>'2010',
                '2011'=>'2010');

foreach($arAnos as $k => $v){

    if(isset($_SESSION['rsJornadaAlunos'.$k])){
        ${'rsJornadaAlunos'.$k} = $_SESSION['rsJornadaAlunos'.$k];
    }else{
        ${'rsJornadaAlunos'.$k} = $obFolhaProfissionais->recuperarJornadaAluno($v);
        $_SESSION['rsJornadaAlunos'.$k] = ${'rsJornadaAlunos'.$k};
    }

    if(isset($_SESSION['rsAP'.$k])){
        ${'rsAP'.$k} = $_SESSION['rsAP'.$k];
    }else{
        ${'rsAP'.$k}   = $rsJornadaDocencia > 0 ? round(${'rsJornadaAlunos'.$k}/$rsJornadaProfessor) : 0;
        $_SESSION['rsAP'.$k]   = ${'rsAP'.$k};
    }

    if(isset($_SESSION['rsAPAD'.$k])){
        ${'rsAPAD'.$k} = $_SESSION['rsAPAD'.$k];
    }else{
        ${'rsAPAD'.$k} = $rsJornadaDocencia > 0 ? round(${'rsJornadaAlunos'.$k}/$rsJornadaDocencia) : 0;
        $_SESSION['rsAPAD'.$k] = ${'rsAPAD'.$k};
    }

    if(isset($_SESSION['rsAPAE'.$k])){
        ${'rsAPAE'.$k} = $_SESSION['rsAPAE'.$k];
    }else{
        ${'rsAPAE'.$k} = $rsJornadaEscolar  > 0 ? round(${'rsJornadaAlunos'.$k}/$rsJornadaEscolar) : 0;
        $_SESSION['rsAPAE'.$k] = ${'rsAPAE'.$k};
    }
}

$rsPARM  = str_replace(',', '.', str_replace('%', '', $_POST['parm']))/100;
$rsPAFR  = str_replace(',', '.', str_replace('%', '', $_POST['pafr']))/100;
$rsPMDE  = str_replace(',', '.', str_replace('%', '', $_POST['pmde']))/100;
$rsIFLEI = str_replace(',', '.', str_replace('%', '', $_POST['iflei']))/100;

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">
<!--
$(document).ready(function(){
    $('.voltar').click(function(){
        $('#formVoltar').attr('action','pisosalarial.php?modulo=principal/folhapagamento/analise/analisarMunicipio&acao=A');
        $('#formVoltar').submit();
    });
    $('.resultado_tabela').click(function(){
        arParams = this.id.split('_');
        $('input[name=co_tabela]').val(arParams[0]);
        if(arParams[0] == '4' || arParams[0] == 4){
            $('input[name=nu_indice]').val(arParams[1]+'_'+arParams[2]);
        }else{
            $('input[name=nu_indice]').val(arParams[1]);
        }
        $('#formTabela').attr('action','pisosalarial.php?modulo=principal/folhapagamento/analise/resultadoTabela&acao=A');
        $('#formTabela').submit();
    });
});
//-->
</script>
<table class="tabela" align="center" bgcolor="#f5f5f5" border="0" cellpadding="5" cellspacing="1">
    <tr>
        <td class="SubtituloCentro">Resultado da Análise do Município</td>
    </tr>
</table>
<table class="tabela" align="center" bgcolor="#f5f5f5" border="0" cellpadding="5" cellspacing="1">
    <tr>
        <td class="SubtituloEsquerda" width="19%">
            &nbsp;
        </td>
        <?php foreach($rsAnosReferencia as $anos): ?>
            <td class="SubtituloEsquerda">
                <?php echo $anos['farano'] ?>
            </td>
        <?php endforeach; ?>
    </tr>
    <?php foreach($rsTabelas as $tabela): ?>
        <?php if($tabela['sg_tabela'] == 'PARM_PAFR'): ?>
            <tr>
                <td class="SubtituloDireita" width="19%">
                    Percentual em face dos parâmetros: PARM
                </td>
                <?php foreach($rsAnosReferencia as $anos): ?>
                    <td>
                        <a href="javascript:void(0)" class="resultado_tabela" id="<?php echo $tabela['co_tabela']."_".$rsPARM."_".$rsPAFR ?>"><?php echo $rsPARM*100; ?>%</a>
                    </td>
                <?php endforeach; ?>
            </tr>
            <tr>
                <td class="SubtituloDireita" width="19%">
                    Percentual em face dos parâmetros: PAFR
                </td>
                <?php foreach($rsAnosReferencia as $anos): ?>
                    <td>
                        <a href="javascript:void(0)" class="resultado_tabela" id="<?php echo $tabela['co_tabela']."_".$rsPARM."_".$rsPAFR ?>"><?php echo $rsPAFR*100; ?>%</a>
                    </td>
                <?php endforeach; ?>
            </tr>
        <?php else: ?>
            <tr>
                <td class="SubtituloDireita" width="19%">
                    <?php echo $tabela['ds_resultado'] ?>
                </td>
                <?php foreach($rsAnosReferencia as $anos): ?>
                    <td>

                        <a href="javascript:void(0)" class="resultado_tabela" id="<?php echo $tabela['co_tabela']."_".${'rs'.$tabela['sg_tabela'].$anos['farano']} ?>">
                            <?php
                                if(in_array($tabela['sg_tabela'], array('IFLEI','PMDE'))){
                                    echo ((${'rs'.$tabela['sg_tabela'].$anos['farano']})*100)."%";
                                }else{
                                    echo ${'rs'.$tabela['sg_tabela'].$anos['farano']};
                                } ?></a>

                    </td>
                <?php endforeach; ?>
            </tr>
        <?php endif; ?>
    <?php endforeach; ?>
</table>
<table class="tabela" align="center" bgcolor="#f5f5f5" border="0" cellpadding="5" cellspacing="1">
    <tr>
        <td class="SubtituloDireita" width="19%">
            &nbsp;
        </td>
        <td class="SubtituloEsquerda">
            <input type="button" value="Voltar" class="voltar">
        </td>
    </tr>
</table>
<form id="formTabela" method="post" action="">
    <input type="hidden" name="co_tabela" value="" />
    <input type="hidden" name="nu_indice" value="" />

    <input type="hidden" name="parm" value="<?php echo $_POST['parm'] ?>">
    <input type="hidden" name="pafr" value="<?php echo $_POST['pafr'] ?>">
    <input type="hidden" name="pmde" value="<?php echo $_POST['pmde'] ?>">
    <input type="hidden" name="iflei" value="<?php echo $_POST['iflei'] ?>">
</form>
<form id="formVoltar" method="post" action="">
    <input type="hidden" name="muncod" value="<?php echo $_SESSION['piso']['muncod'] ?>" />

    <input type="hidden" name="parm" value="<?php echo $_POST['parm'] ?>">
    <input type="hidden" name="pafr" value="<?php echo $_POST['pafr'] ?>">
    <input type="hidden" name="pmde" value="<?php echo $_POST['pmde'] ?>">
    <input type="hidden" name="iflei" value="<?php echo $_POST['iflei'] ?>">
</form>