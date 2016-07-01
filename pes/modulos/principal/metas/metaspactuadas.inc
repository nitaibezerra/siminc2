<?php
include_once( APPRAIZ. "pes/classes/PesCelulaAcompanhamento.class.inc" );
include_once( APPRAIZ. "pes/classes/PesCelulaAcompNatDespesa.class.inc" );
include_once( APPRAIZ. "elabrev/www/_funcoes_esplanada_sustentavel.php" );

$entcodigo        = $_SESSION['pes']['codigo_entidade'];
$unicod           = $_SESSION['pes']['codigo_unidadeorcamentaria'];
$unidade_decricao = $_SESSION['pes']['nome_entidade'];

extract($_REQUEST);

if (isset($_POST['requisicao'])) {
    switch ($_POST['requisicao']) {
        case 'titleNaturezaDespesa':
            $_POST['requisicao']($_POST);
            break;
    }
    exit;
}
?>

<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<link href="css/estilo.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/funcoes.js"></script>


<?php
include APPRAIZ . 'includes/cabecalho.inc';

// Exibindo barra com os dados recursivos da entidade.
$controllerEntidade = new Controller_Usuario();
echo $controllerEntidade->barraEntidadeUsuarioAction();

$db->cria_aba( $abacod_tela, $url, '' );

$sql = "select unscodigo from elabrev.unidadesustentavel where unscodigo = '{$unicod}'";
$unscodigo = $db->pegaUm($sql);

$uo = $db->pegaLinha("SELECT codentidadesispes, to_char(dataenviosispes, 'DD/MM/YYYY') as dataenviosispes
                      FROM elabrev.unidadeordenadora codentidadesispes
                      WHERE unicod = '{$unicod}'");

?>
<div class="div_info">  </div>
<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="" style="border-top: none; border-bottom: none; width: 80%; margin: 10px auto;">
    <tr class="TituloTela">
        <td width="100%" align="center"><label class="TituloTela" style="color:#000000;"><?php echo $titulo_modulo; ?></label></td>
    </tr>
    <tr>
        <td bgcolor="#e9e9e9" align="center" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" >
            <label style="color:#000000; font-weight: normal;"><?php echo $unidade_decricao; ?></label>
        </td>
    </tr>
</table>

<table style="width: 80%;" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
    <tr>
        <th >UO: <?php echo $unicod . ' - ' . $unidade_decricao; ?> </th>
        <th > Projeto Esplanada Sustentável</th>
    </tr>
    <?php if($uo['codentidadesispes'] && $uo['dataenviosispes']): ?>
    <tr>
        <th style=" font-size: 12px; text-align: left; width: 35%; ">Código da entidade no SisPES: <?php echo $uo['codentidadesispes'] ?> </th>
        <th style=" font-size: 14px;">Data do último envio ao SisPES: <?php echo $uo['dataenviosispes'] ?></th>
    </tr>
    <?php endif ?>
    <tr>
        <th colspan="2" style="font-size: 12px; text-align: left;">Meta de despesa 2013 <!--Pactuada por aluno equivalente--></th>
    </tr>
</table>
<script language="javascript" type="text/javascript"> // lsametareducaoaluno  hab_campo 
    jQuery(document).ready(function(){
        jQuery("[name^=lcsvalormeta],[name^=lsametareducaoaluno],[name^=lsametavalorreducaoaluno],[name=hab_campo]").each(function(){
            jQuery(this).attr('disabled', true).addClass('disabled').removeClass('normal');
        })
    });
</script>
<?php if ( $unscodigo ) { ?>

    <table style="width: 80%;" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
        <tr>
            <th rowspan="2" width="45%">Despesa</th>
            <th rowspan="2" width="25%">Despesas Liquidadas 2012 </th>
            <th colspan="2" width="30%">Meta de Economia Proposta</th>
        </tr>
        <tr>
            <th width="12%">%</th>
            <th width="18%">Valor</th>
        </tr>

        <?php listaItensDespesas( $unicod ); ?>
    </table>
    <?php $dados = valorPercentualTitulo($unicod); ?>
    <table style="width: 80%;" class="tabela" bgcolor="#f5f5f5" cellSpacing="0" cellPadding=0 align="center">
        <tr>
            <th style="text-align: left;" colspan="2">Observações da UO:</th>
        </tr>
        <tr>
            <td>
            <?php echo campo_textarea('lcsobservacao', 'N', 'N', 'Observação ', 165, 2, 2000, '', '', '', '', 'id="lcsobservacao', $dados['lcsobservacao'], '' ); ?>
            </td>
        </tr>
    </table>

<table style="width: 80%;" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
    <tr>
        <th colspan="2" style="text-align: left;">Notas:</th>
    </tr>
    <tr>
        <td width="50%" style="border:thin; border-style:solid; border-color:#BEBEBE;">
            - Despesas Liquidadas na LOA 2012 (Contas Contábeis = +292130301-292130203+292130203 +292130201+292130202), Fonte: SIAFI Gerencial.<br>
<?php echo notaRodaPe( $unicod ); ?>
        </td>
        <td width="50%" align="justify" style="border:thin; border-style:solid; border-color:#BEBEBE;">
            - Para fins de padronização, os montantes referentes às operações de cálculos efetuados acima estão expressos em duas casas decimas, com aproximação a partir de valores absolutos.
        </td>
    </tr>
    <tr>
        <td align="justify" style="border:thin; border-style:solid; border-color:#BEBEBE;"><?php echo textoUltimaCargaSIAFI(); ?></td>
        <td>&nbsp;</td>
    </tr>
</table>

<?php } else { ?>
    <table style="width: 80%;" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
        <tr>
            <th rowspan="3" width="22%">Despesa</th>
            <th rowspan="3" width="12%">Despesas Liquidadas 2012 <br> (A)</th>
            <th rowspan="3" width="8%">Aluno Equivalente <br> (B)</th>
            <th rowspan="3" width="12%">Despesa por Aluno Equivalente <br>(C) = (A / B)</th>
            <th colspan="3" width="36%">Meta de Economia Proposta</th>
            <th rowspan="3" width="21%">Despesa 2013 pactuada por aluno equivalente <br> (G) = (C - E)</th>
        </tr>
        <tr>
            <th width="12%">% Redução por aluno equivalente <br> (D) = (E / C)%</th>
            <th width="12%">Valor por aluno equivalente <br> (E) = (C x D)</th>
            <th width="12%" rowspan="2">Valor Total <br> (F) = (B x E)</th>
        </tr>
        <tr>
            <th><input id="habil_perce" name="hab_campo" type="radio" value="P" checked="checked"></th>
            <th><input id="habil_valor" name="hab_campo" type="radio" value="V"></th>
        </tr>
        <?php listaItensDespesasaAlunos($unicod); ?>
    </table>
    <table style="width: 80%;" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
        <th style="text-align: left;" colspan="2">Observações da UO:</th>
        <tr>
            <td>
<?php echo campo_textarea(lsaobservacao, 'N', 'N', 'Observação ', 165, 2, 2000, '', '', '', '', 'id="lsaobservacao', $dados['lsaobservacao'], ''); ?>
            </td>
        </tr>
    </table>
<table style="width: 80%;" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
    <tr>
        <th colspan="2" style="text-align: left;">Notas:</th>
    </tr>
    <tr>
        <td width="50%" style="border:thin; border-style:solid; border-color:#BEBEBE;">
            - Despesas Liquidadas na LOA 2012 (Contas Contábeis = +292130301-292130203+292130203 +292130201+292130202), Fonte: SIAFI Gerencial.<br>
<?php echo notaRodaPe($unicod); ?>
        </td>
        <td width="50%" align="justify" style="border:thin; border-style:solid; border-color:#BEBEBE;">
            - Para fins de padronização, os montantes refereUO: ntes às operações de cálculos efetuados acima estão expressos em duas casas decimas, com aproximação a partir de valores absolutos.
        </td>
    </tr>
    <tr>
        <td align="justify" style="border:thin; border-style:solid; border-color:#BEBEBE;"><?php echo textoUltimaCargaSIAFI(); ?></td>
        <td>&nbsp;</td>
    </tr>
</table>
<?php } ?>