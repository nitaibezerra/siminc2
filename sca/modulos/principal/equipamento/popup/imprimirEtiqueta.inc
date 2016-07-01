<?php
include APPRAIZ."sca/classes/Equipamento.class.inc";

$oEquipamento = new Equipamento();

if($_REQUEST['eqmid'] && !empty($_REQUEST['eqmid'])){
    $Equipamento = $oEquipamento->carregaEquipamentoPorId($_REQUEST['eqmid']);
    extract($Equipamento);
?>
<html>
<head></head>
<body>
    <applet name="PrinterApplet" code="PrinterApplet.class" archive="componentes/codebar.jar">
        <param name="MarcaModelo" value="<?=strtoupper($tpedsc) . ' - ' . strtoupper($mcedsc)?>" />
        <param name="NumeroEtiqueta" value="<?=$eqmnumetiqueta?>" />
        <param name="LarguraEtiqueta" value="<?=LARGURA_ETIQUETA_SCA?>" />
        <param name="AlturaEtiqueta" value="<?=ALTURA_ETIQUETA_SCA?>" />
    </applet>
</body>
</html>
<?php
}
?>