<?php
/**
 * Arquivo de CSS do relatório impresso em PDF.
 * @see css-relatorio-pdf.php
 */
?>
<style type="text/css">
.quadro-tcu p{font-family:times;font-size:13px;margin-bottom:2px}
.quadro-tcu table{width:100%;font-family:times;font-size:13px}
.quadro-tcu table,.quadro-tcu table th,.quadro-tcu table td{border:1px solid black;border-collapse:collapse;padding:5px}
.quadro-tcu table th{text-align:center}
.quadro-tcu table thead{background-color:#bbbbbb}
.quadro-tcu thead tr.level2{background-color:#cccccc}
.quadro-tcu thead tr.leveln{background-color:#dddddd}
.quadro-tcu td.titulo{background-color:#eeeeee;width:25%;font-weight:bold}
hr.quadro-tcu{color:white;border:0 solid white}
@media print{hr.quadro-tcu{page-break-after:always}}
</style>