<script language="JavaScript" src="../includes/funcoes.js"></script>

<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>

<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<style type="text/css" media="print">
    
.noPrint{
        display:none;
}
</style>
<style type="text/css">
    table.bordasimples {border-collapse: collapse; width:95%;}
    table.bordasimples tr {border:1px solid #cccccc;}
    thead {display: table-header-group;}
</style> 
<?php
header('Content-Type: text/html; charset=iso-8859-1'); 
/*TRATAR $_REQUEST - ABRE AQUI*/
//var_dump($_REQUEST);exit;

$arrFiltrolabel = array();

if (!empty($_REQUEST["nu_matricula_siape"])) {$arrFiltrolabel[] = " Matricula SIAPE"; }
if (!empty($_REQUEST["no_servidor"])) {  $arrFiltrolabel[] = " Nome"; }
if (!empty($_REQUEST["unidade"])) { $arrFiltrolabel[] = " Unidade"; }
if (!empty($_REQUEST["endereco"])) { $arrFiltrolabel[] = " Endereço"; }
if (!empty($_REQUEST["endereco_andar"])) { $arrFiltrolabel[] = " Andar"; }
if (!empty($_REQUEST["endereco_sala"])) { $arrFiltrolabel[] = " Sala"; }
if (!empty($_REQUEST["numero_rgp"])) { $arrFiltrolabel[] = " Nº RGP"; }
$retornoFiltrolabel = implode(",", $arrFiltrolabel);

/*TRATAR $_REQUEST - FECHA AQUI*/

$sqlBensInventariados ="
         SELECT 
		rgp.rgpnum AS rgp,
		--contacontabil.ccbdsc AS descricao,
                estadoconservacao.ecodsc AS descricao,
		(coalesce((unidade.uorno),'') || ', ' || coalesce(cast(endereco.endlog as varchar),'') || ', ' || coalesce(cast(endandar.enanumero as varchar),'') || ', ' || coalesce(cast(endandarsala.easnumero as varchar),''	)) 
		as localizacao,
		bens.bennumproc as numero_de_serie,
		estadoconservacao.ecodsc AS estado_conservacao,
		--usuario.usunome as usuario,
                estadoconservacao.ecodsc AS usuario,
		bensmaterial.bmtvlrunitario AS valor_do_bem
                            FROM   
                                   --rgp
				   sap.rgp rgp
				    -- estado_conservacao	
                                    INNER JOIN sap.estadomotivos estadomotivos
                                        ON estadomotivos.esmid = rgp.esmid
                                    INNER JOIN sap.estadoconservacao estadoconservacao
                                        ON estadomotivos.ecoid = estadoconservacao.ecoid      
 		                     --valor_do_bem
                                    INNER JOIN sap.bensmaterial bensmaterial
                                        ON bensmaterial.bmtid = rgp.bmtid
                                    INNER JOIN sap.bens
                                        ON bensmaterial.benid = bens.benid
                                    -- usuario    
                                    /*INNER JOIN seguranca.usuario usuario
                                        ON usuario.usucpf = bens.usucpf*/
                                    -- localizacao
                                    LEFT JOIN siorg.uorgendereco uorgend 
                                        ON bens.uendid=uorgend.uendid
                                    LEFT JOIN siorg.enderecoandarsala endandarsala 
                                        ON uorgend.easid=endandarsala.easid
                                    LEFT JOIN siorg.enderecoandar endandar 
                                        ON endandarsala.enaid=endandar.enaid        
                                    LEFT JOIN siorg.endereco endereco 
                                        ON endereco.endid=endandar.endid
                                    LEFT JOIN siorg.unidadeorganizacional unidade 
                                        ON uorgend.uorco_uorg_lotacao_servidor=unidade.uorco_uorg_lotacao_servidor    
                                    /*contacontabil.ccbdsc AS descricao
                                    LEFT JOIN sap.bensmovimentacaorgp movimentargp 
                                            ON rgp.rgpid=movimentargp.rgpid
                                    LEFT JOIN sap.movimentacaobens movimenta 
                                            ON movimentargp.mvbid=movimenta.mvbid
                                    INNER JOIN siape.vwservidorativo responsavel
                                            ON responsavel.nu_matricula_siape=movimenta.nu_matricula_siape
                                    INNER JOIN sap.material	material
                                        ON material.matid = bensmaterial.matid       
                                    INNER JOIN sap.catalogo catalogo
                                        ON catalogo.catid = material.catid          
                                    INNER JOIN sap.itemcontacontabil itemcontacontabil
                                        ON itemcontacontabil.icbid = catalogo.icbid
                                        AND itemcontacontabil.ccbid = catalogo.ccbid
                                    INNER JOIN sap.contacontabil  contacontabil
                                        ON contacontabil.ccbid = itemcontacontabil.ccbid */
                                    WHERE 1=1 ";



    if(!empty($_REQUEST['nu_matricula_siape'])) { $sqlBensInventariados .= " AND responsavel.nu_matricula_siape = '{$_REQUEST['nu_matricula_siape']}' "; }
    if(!empty($_REQUEST['no_servidor'])) { $sqlBensInventariados .= " AND responsavel.nu_matricula_siape = '{$_REQUEST['nu_matricula_siape']}' "; }
    if(!empty($_REQUEST['unidade'])) { $sqlBensInventariados .= " AND unidade.uorco_interno_uorg = '{$_REQUEST['unidade']}' "; }
    if(!empty($_REQUEST['endereco'])) { $sqlBensInventariados .= " AND endereco.endid = '{$_REQUEST['endereco']}' "; }
    if(!empty($_REQUEST['endereco_andar'])) { $sqlBensInventariados .= " AND endandar.enaid = '{$_REQUEST['endereco_andar']}' "; }
    if(!empty($_REQUEST['endereco_sala'])) { $sqlBensInventariados .= " AND endandarsala.enaid = '{$_REQUEST['endereco_sala']}' "; }
    if(!empty($_REQUEST['numero_rgp'])) { $sqlBensInventariados .= " AND rgp.rgpnum = '{$_REQUEST['numero_rgp']}' "; }
    /*ORDER BY*/
//ver($sqlBensInventariados);exit;
$listaBensInventariados= $db->carregar($sqlBensInventariados);
//echo "<pre>";
//ver($listaBensInventariados);
//$somaSaldoAnterior=0;
//$somaOrcamentario=0;
//$somaExtraOrcamentario=0;
//$somaSaida=0;
//$somaSaldoAtual=0;
?>
<style type="text/css">
</style> 
<table align="center" class="tabela" width="100%" border="0" class="notscreen1 debug">
<thead>
<tr bgcolor="#ffffff" style="">
        <td valign="top" width="50" rowspan="2" style=""><img src="../imagens/brasao.gif" width="45" height="45" border="0" colspan="1"></td>                     
        <td nowrap align="left" valign="middle" height="1" style="padding:5px 0 0 0;" colspan="2">                  
                SIMEC- Sistema Integrado do Ministério da Educação<br/>                               
                Ministério da Educação <br />
                Secretaria Executiva<br />
                Subsecretaria de Assuntos Administrativos<br />
                Coordenação Geral de Recursos Logísticos
        </td>
        <td align="right" valign="middle" height="1" style="padding:5px 0 0 0;" colspan="5">      
                <a class="noPrint" href="javascript:window.print();"><img src="../imagens/print2.gif" border="0" width="20"></a><br/>                            
                Impresso por: <b><?php echo $_SESSION['usunome'] ?></b><br/>                       
                Hora da Impressão: <?php echo date( 'd/m/Y - H:i:s' ) ?><br />                                     
        </td>                                 
</tr>
</thead>
<tbody>
<tr width="100%" align="center">
    <td colspan="8">
    <br />    
    <h2 align="center">
        LEVANTAMENTO DE BENS INVENTARIADOS
     </h2><br />
     <h2 align="center">
        FILTRO ESCOLHIDO
        <?php if(!empty($retornoFiltrolabel)){
            echo ("( $retornoFiltrolabel )");
        }?>
     </h2>
    </td>
</tr>
    <?php if($listaBensInventariados):?>
        <tr>
        <th>RGP</th>
        <th>DESCRIÇÃO DO BEM</th>
        <th>LOCALIZAÇÃO DO BEM <br /> (Unidade, Endereço, andar, sala)</th>
        <th>NÚMERO DE SERIE</th>
        <th>ESTADO DE CONSERVAÇÃO</th>
        <th>USUÁRIO</th>
        <th>VALOR DO BEM</th>
        </tr>
        <?php 
            $valorTotal = 0;
            $resultado = $listaBensInventariados;
            foreach ( $resultado as $key => $rowResultado ): 
        ?>
            <tr>
            <td><?php echo $rowResultado['rgp']; ?></td>
            <td><?php echo $rowResultado['descricao']; ?></td>
            <td><?php echo $rowResultado['localizacao']; ?></td>
            <td><?php echo $rowResultado['numero_de_serie']; ?></td>
            <td><?php echo $rowResultado['estado_conservacao']; ?></td>
            <td><?php echo $rowResultado['usuario']; ?></td>
            <td><?php echo number_format($rowResultado['valor_do_bem'],2,',','.'); ?></td>
            </tr>
        <?php 
           $valorTotal = $valorTotal+$rowResultado['valor_do_bem']; 
        ?>    
        <?php endforeach; ?>
        <tr style="background-color:#CCCCCC;">
        <td colspan="4">
            <b>QUANTIDADE  DE ITEM = </b><?php echo count($listaBensInventariados); ?>
        </td>
        <td colspan="4">
            <b>TOTAL PARCIAL = <?php echo number_format($valorTotal,2,',','.'); ?></b>
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