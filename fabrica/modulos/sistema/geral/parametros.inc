<?php
// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ."fabrica/classes/ParametrosSistema.class.inc";
print '<br/>';

monta_titulo("Parâmetros do Sistema", null);

$parametroSistema = new ParametrosSistema();
$parametroSistema->recuperaPeloNome(ParametrosSistema::PERIODO_DETALHAMENTO_AVALIACAO);


if($_SERVER['REQUEST_METHOD'] == "POST"){
	$parametroSistema->setValor($_POST['psvalor']);
	$parametroSistema->atualize($parametroSistema);
}

?>

<script type="text/javascript" src="/includes/prototype.js"></script>
<script language='javascript' type='text/javascript' src='../includes/funcoes.js'></script>

<form id="formulario" name="formulario" action="" method="post" enctype="multipart/form-data">
    <table class="tabela listaAtualizacao" cellspacing="1" cellpadding="3" align="center" border="0">
        <tr>
            <td class="SubTituloDireita campoPequeno" style="width: 45%;"><?php echo $parametroSistema->getDescricao()?></td>
            <td>
            	<input class="normal" type="hidden" name="idParametro" value="<?php echo $parametroSistema->getId();?>" />
                <? echo campo_texto('psvalor', 'N',  'S', $parametroSistema->getDescricao(), '', '', '##',
                    '', '', '', 0, 'id="psvalor"', '', $parametroSistema->getValor(), '', ''); ?>
            </td>
        </tr>
		<tr>
            <td colspan="2" style="text-align: center;">
                <input type="submit" value="Salvar" />
            </td>
        </tr>
    </table>

</form>