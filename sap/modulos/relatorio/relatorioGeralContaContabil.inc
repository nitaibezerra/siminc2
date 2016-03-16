<?php 
include APPRAIZ . 'includes/cabecalho.inc';
print "<br/>";
?>
<script src="../fabrica/js/autocomplete/jquery-1.7.2.js"></script>
<script>
$(document).ready(function(){
    
    $('.notscreen').remove('table');
    $('.rodape').remove('table');
    
});
</script>
<link rel="stylesheet" type="text/css" media="print" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' media="print" href='../includes/listagem.css'/>
<style type="text/css" media="print">
.noPrint{
        display:none;
}
</style>
<style type="text/css" media="print">
    table.tabela {border-collapse: collapse; width:95%;}
    table.tabela tr {border:1px solid #cccccc;}
    table.tabela th {background-color: #cccccc; display: table-body-group;}    
    thead {display: table-header-group;}
</style> 
<?php
header('Content-Type: text/html; charset=iso-8859-1'); 
/*TRATAR $_REQUEST - ABRE AQUI*/
$sqlContaContabil ="
        SELECT DISTINCT
            CONTACONTABIL.CCBID AS CONTA, 
            CONTACONTABIL.CCBDSC AS DESCRICAO, 
            CONTACONTABIL.CCBPERCRESIDUAL AS VALOR 
        FROM 
            SAP.CONTACONTABIL AS CONTACONTABIL
        ORDER 
            BY CONTACONTABIL.CCBID, CONTACONTABIL.CCBDSC	
        ";
$listaContaContabil = $db->carregar($sqlContaContabil);
?>
<style type="text/css">
</style> 
<table align="center" class="tabela" width="100%" border="0">
<thead>
<tr bgcolor="#ffffff" style="">
        <td valign="top" width="50" rowspan="2"><img src="../imagens/brasao.gif" width="45" height="45" border="0" colspan="1"></td>                     
        <td nowrap align="left" valign="middle" height="1" style="padding:5px 0 0 0;" colspan="2">                  
                SIMEC- Sistema Integrado do Ministério da Educação<br/>                               
                Ministério da Educação <br />
                Secretaria Executiva<br />
                Subsecretaria de Assuntos Administrativos<br />
                Coordenação Geral de Recursos Logísticos
        </td>
        <td align="right" valign="middle" height="1" style="padding:5px 0 0 0;" colspan="5">      
                <a class="noPrint" href="javascript:window.print();">
                    <img src="../imagens/print2.gif" border="0" width="20"></a><br/>                            
                Impresso por: <b><?php echo $_SESSION['usunome'] ?></b><br/>                       
                Hora da Impressão: <?php echo date( 'd/m/Y - H:i:s' ) ?><br />                                     
        </td>                                 
</tr>
<tr width="100%" align="center">
    <td colspan="8">
        <br />    
    </td>
</tr>
</thead>
<tbody>
<tr width="100%" align="center">
    <td colspan="8">
    <br />    
    <h2 align="center">
       RESUMO GERAL POR CONTA CONTÁBIL <br />
       <?php echo date( 'd/m/Y / H:i:s' ) ?>
     </h2><br />
    </td>
</tr>
    <?php if($listaContaContabil):?>
        <tr>
            <th colspan="2">CONTA</th>
            <th colspan="4">DESCRIÇÃO</th>
            <th colspan="2">VALOR</th>
        </tr>
        <?php 
            $valorTotal = 0;
            $resultado = $listaContaContabil;
            foreach ( $resultado as $key => $rowResultado ): 
        ?>
        <tr>
            <td colspan="2"><?php echo $rowResultado['conta']; ?></td>
            <td colspan="5"><?php echo $rowResultado['descricao']; ?></td>
            <td colspan="1"><?php echo number_format($rowResultado['valor'],2,',','.'); ?></td>
        </tr>
        <?php 
           $valorTotal = $valorTotal+$rowResultado['valor']; 
        ?>    
        <?php endforeach; ?>
        <tr style="background-color:#CCCCCC;">
        <td colspan="7">
            <b>QUANTIDADE  DE ITEM = </b><?php echo count($listaContaContabil); ?>
        </td>
        <td colspan="1">
            <b>TOTAL GERAL = <?php echo number_format($valorTotal,2,',','.'); ?></b>
        </td>
        </tr>
        <?php else: ?>
        <tr width="100%" align="center">
            <td colspan="8">
            <br />    
                <h2 align="center">Nenhum dado encontrado para o filtro de pesquisa informado.</h2>
            </td>
        </tr>
    <?php endif; ?>
</tbody>
</table>