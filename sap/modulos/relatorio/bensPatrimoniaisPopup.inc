<script language="JavaScript" src="../includes/funcoes.js"></script>            
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<style type="text/css" media="print">
.noPrint{
        display:none;
}
</style>
<style type="text/css">
    table.tabela {border-collapse: collapse; width:95%;}
    table.tabela th {border:1px solid #000;}
    .bordaCelula {border:1px solid #000;}
    .bordaLinha {border:1px solid #000;}
    thead {display: table-header-group;}
</style> 
<?php
header('Content-Type: text/html; charset=iso-8859-1'); 
/*TRATAR $_REQUEST - ABRE AQUI*/
$_REQUEST['nu_matricula_siape'] = strtoupper($_REQUEST['nu_matricula_siape']);
$_REQUEST['descricao_do_bem'] = trim($_REQUEST['descricao_do_bem']);
if(!empty($_REQUEST['tipo_relatorio'])){
    if($_REQUEST['tipo_relatorio'] == "orderRgp"){
        $classficarRelatorio = " ORDER BY rgp.rgpnum;";
    }else if($_REQUEST['tipo_relatorio'] == "orderTermo"){
         $classficarRelatorio = " ORDER BY rgp.rgpnum;";
    }else if($_REQUEST['tipo_relatorio'] == "orderDescricaodoBem"){
         $classficarRelatorio = " ORDER BY material.matdsc;";
    }else if($_REQUEST['tipo_relatorio'] == "orderLocalizacao"){
         $classficarRelatorio = " ORDER BY unidade.uorno, unidade.uorco_interno_uorg, endandar.enaid, endandarsala.enaid;";
    }
}
$ocultarNtermo = false;
$ocultarResponsavel = false;
$ocultarSituacaoDoBem = false;
    if(!empty($_REQUEST['ocultarNtermo']) and $_REQUEST['ocultarNtermo'] == "ocultarNtermo"){
        $ocultarNtermo = true;
    }
    if(!empty($_REQUEST['ocultarResponsavel']) and  $_REQUEST['ocultarResponsavel'] == "ocultarResponsavel"){
        $ocultarResponsavel = true;
    }
    if(!empty($_REQUEST['ocultarSituacaoDoBem']) and  $_REQUEST['ocultarSituacaoDoBem'] == "ocultarSituacaoDoBem"){
        $ocultarSituacaoDoBem = true;
    }
    /*Bens com entrada Até:*/
   if (!empty($_REQUEST['auddata_ini'])){
      $auddata_ini = formata_data_sql($_REQUEST['auddata_ini']);
   }else{
       $auddata_ini = formata_data_sql(date("d/m/Y")); 
   } 
    /*TRATAR $_REQUEST - FECHA AQUI*/
    $sqlBensPatrimoniaisFull ="SELECT DISTINCT
                                classe.clsdescclasse as classe,
                                situacaobem.sbmdsc situacao_do_bem,     
                                contacontabil.ccbid as conta_contabil_ccbid,
                                x.codcontacontabil as conta_contabil,
                                contacontabil.ccbdsc as conta_contabil_descricao,
                                situacaobem.sbmcod as sbmcod,
                                bensmaterial.bmtvlrunitario AS valor_total_historico,
                                x.vdepreciacaoacumulada AS valor_total_depreciado
                           FROM   sap.rgp rgp
                                    inner join sap.fn_relatorio_depreciacao_mensal('{$auddata_ini}') x
                                    on rgp.rgpnum= x.rgpnum
                                    INNER JOIN sap.estadomotivos estadomotivos
                                        ON estadomotivos.esmid = rgp.esmid
                                    INNER JOIN sap.estadoconservacao estadoconservacao
                                        ON estadomotivos.ecoid = estadoconservacao.ecoid      
                                    INNER JOIN sap.bensmaterial bensmaterial
                                        ON bensmaterial.bmtid = rgp.bmtid
                                    INNER JOIN sap.bens
                                        ON bensmaterial.benid = bens.benid        	       
                                     LEFT JOIN siorg.uorgendereco uorgend 
                                        ON bens.uendid=uorgend.uendid
                                    LEFT JOIN siorg.unidadeorganizacional unidade 
                                        ON uorgend.uorco_uorg_lotacao_servidor=unidade.uorco_uorg_lotacao_servidor
                                    LEFT JOIN siorg.enderecoandarsala endandarsala 
                                        ON uorgend.easid=endandarsala.easid
                                    LEFT JOIN siorg.enderecoandar endandar 
                                        ON endandarsala.enaid=endandar.enaid
                                    INNER JOIN siorg.endereco endereco 
                                        ON endereco.endid=endandar.endid    
                                    INNER JOIN sap.bensmovimentacaorgp movimentargp 
                                            ON rgp.rgpid=movimentargp.rgpid
                                    INNER JOIN sap.movimentacaobens movimenta 
                                            ON movimentargp.mvbid=movimenta.mvbid
                                    INNER JOIN siape.vwservidorativo responsavel
                                            ON responsavel.nu_matricula_siape=movimenta.nu_matricula_siape
                                    INNER JOIN sap.material	material
                                        ON material.matid = bensmaterial.matid       
                                    INNER JOIN sap.catalogo catalogo
                                        ON catalogo.catid = material.catid
                                    INNER JOIN sap.classe classe
                                        ON classe.clscodclasse = catalogo.clscodclasse             
                                    INNER JOIN sap.situacaobem as situacaobem
                                        ON situacaobem.sbmid = rgp.sbmid       
                                    INNER JOIN sap.itemcontacontabil itemcontacontabil
                                        ON itemcontacontabil.icbid = catalogo.icbid
                                        AND itemcontacontabil.ccbid = catalogo.ccbid
                                    INNER JOIN sap.contacontabil  contacontabil
                                        ON contacontabil.ccbid = itemcontacontabil.ccbid WHERE 1=1 ";

    if(!empty($_REQUEST['contacontabil'])) { $sqlBensPatrimoniaisFull .= " and contacontabil.ccbid = '{$_REQUEST['contacontabil']}' "; }
    if(!empty($_REQUEST['item_conta_contabil'])) { $sqlBensPatrimoniaisFull .= " and itemcontacontabil.icbid = '{$_REQUEST['item_conta_contabil']}' "; }
    if(!empty($_REQUEST['nu_matricula_siape'])) { $sqlBensPatrimoniaisFull .= " and responsavel.nu_matricula_siape = '{$_REQUEST['nu_matricula_siape']}' "; }
    if(!empty($_REQUEST['no_servidor'])) { $sqlBensPatrimoniaisFull .= " and responsavel.nu_matricula_siape = '{$_REQUEST['nu_matricula_siape']}' "; }
    if(!empty($_REQUEST['unidade'])) { $sqlBensPatrimoniaisFull .= " and unidade.uorco_interno_uorg = '{$_REQUEST['unidade']}' "; }
    if(!empty($_REQUEST['endereco'])) { $sqlBensPatrimoniaisFull .= " and endereco.endid = '{$_REQUEST['endereco']}' "; }
    if(!empty($_REQUEST['endereco_andar'])) { $sqlBensPatrimoniaisFull .= " and endandar.enaid = '{$_REQUEST['endereco_andar']}' "; }
    if(!empty($_REQUEST['endereco_sala'])) { $sqlBensPatrimoniaisFull .= " and endandarsala.enaid = '{$_REQUEST['endereco_sala']}' "; }
    if(!empty($_REQUEST['descricao_do_bem_hidden'])) { $sqlBensPatrimoniaisFull .= " and material.matid = '{$_REQUEST['descricao_do_bem_hidden']}' "; }
    if(!empty($_REQUEST['estado_de_conservacao'])) { $sqlBensPatrimoniaisFull .= " and estadoconservacao.ecoid = '{$_REQUEST['estado_de_conservacao']}' "; }
    if(!empty($_REQUEST['situacao_do_bem'])) { $sqlBensPatrimoniaisFull .= " and situacaobem.sbmcod = '{$_REQUEST['situacao_do_bem']}' "; }
    /*ORDER BY*/
    if(!empty($_REQUEST['tipo_relatorio'])) { $sqlBensPatrimoniaisFull .= $classficarRelatorio; }

  /*  $sqlBensPatrimoniaisFull .=" GROUP BY classe.clsdescclasse,
                                situacaobem.sbmdsc,
                                contacontabil.ccbid,
                                x.codcontacontabil,
                                contacontabil.ccbdsc,
                                situacaobem.sbmcod  ";
   */
  //ver($sqlBensPatrimoniaisFull);exit;
$listaBensPatrimoniaisFull= $db->carregar($sqlBensPatrimoniaisFull);

?>
<style type="text/css">
</style> 
<table align="center" class="tabela" width="100%" border="0">
<thead>
<tr bgcolor="#ffffff" style="">
        <td valign="top" width="50" rowspan="2" style=""><img src="../imagens/brasao.gif" width="45" height="45" border="0" colspan="1"></td>                     
        <td nowrap align="left" valign="middle" height="1" style="padding:5px 0 0 0;" colspan="2">                  
                <?php echo NOME_SISTEMA; ?><br/>                               
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
<tr  width="95%" align="center">
    <td colspan="8">
    <br />    
    <h2 align="center">RELATÓRIO DE BENS PATRIMONIAIS</h2>
    </td>
</tr>
    <?php if($listaBensPatrimoniaisFull):?>
    <?php $resultado = $listaBensPatrimoniaisFull;
        $quantidadeGeral = 0;
        $total_geral_historico = 0;
        $total_geral_depreciado = 0;
    foreach ( $resultado as $key => $rowResultado ): 
    ?>

<?php 
   // ver($resultado);
?>
    <tr>
        <td colspan="8" style="padding:5px;">
            &nbsp;
        </td>     
    </tr>
    <tr>
        <td colspan="8" style="background-color:#EAEAEA;" class="bordaCelula">
            <table border="0" width="100%" cellpadding="5px" cellspacing="5px">
                <tr>
                    <td>
                        <b>Classe: </b><?php echo $rowResultado['classe']; ?> 
                    </td>
                    <td>
                        <?php if (!$ocultarSituacaoDoBem) :?>
                        <b>Situação do Bem:</b> <?php echo $rowResultado['situacao_do_bem']; ?> 
                        <?php endif;?>
                    </td>
                </tr>
                <tr>
                    <td>
                        <b>Conta Contábil: </b> <?php echo $rowResultado['conta_contabil']; ?>    
                    </td>
                    <td>
                        <?php echo $rowResultado['conta_contabil_descricao']; ?>    
                    </td>
                </tr>
            </table>
        </td>
    </tr>
<?php


    $sqlBensPatrimoniais ="SELECT DISTINCT
                                classe.clsdescclasse as classe,
                                x.codcontacontabil as conta_contabil,
                                contacontabil.ccbid as conta_contabil_ccbid,
                                situacaobem.sbmcod as sbmcod,
                                situacaobem.sbmdsc as situacao_do_bem,
                                rgp.rgpnum AS rgp,
                                itemcontacontabil.icbdsc AS descricao,
                                bens.bennumproc as numero_do_termo_responsavel,
                                (coalesce((unidade.uorno),'') || ', ' || coalesce(cast(endandar.enanumero as varchar),'') || ', ' || coalesce(cast(endandarsala.easnumero as varchar),''	)) 
                                as localizacao, 
                                estadoconservacao.ecodsc AS estado_conservacao,
                                responsavel.no_servidor	AS responsavel,
                                bensmaterial.bmtvlrunitario AS valor_historio,
                                x.vdepreciacaoacumulada AS valor_depreciado
                           FROM   sap.rgp rgp
                                    inner join sap.fn_relatorio_depreciacao_mensal('{$auddata_ini}') x
                                    on rgp.rgpnum= x.rgpnum
                                    INNER JOIN sap.estadomotivos estadomotivos
                                        ON estadomotivos.esmid = rgp.esmid
                                    INNER JOIN sap.estadoconservacao estadoconservacao
                                        ON estadomotivos.ecoid = estadoconservacao.ecoid      
                                    INNER JOIN sap.bensmaterial bensmaterial
                                        ON bensmaterial.bmtid = rgp.bmtid
                                    INNER JOIN sap.bens
                                        ON bensmaterial.benid = bens.benid        	       
                                    LEFT JOIN siorg.uorgendereco uorgend 
                                        ON bens.uendid=uorgend.uendid
                                    LEFT JOIN siorg.unidadeorganizacional unidade 
                                        ON uorgend.uorco_uorg_lotacao_servidor=unidade.uorco_uorg_lotacao_servidor
                                    LEFT JOIN siorg.enderecoandarsala endandarsala 
                                        ON uorgend.easid=endandarsala.easid
                                    LEFT JOIN siorg.enderecoandar endandar 
                                        ON endandarsala.enaid=endandar.enaid
                                    INNER JOIN siorg.endereco endereco 
                                        ON endereco.endid=endandar.endid    
                                    INNER JOIN sap.bensmovimentacaorgp movimentargp 
                                            ON rgp.rgpid=movimentargp.rgpid
                                    INNER JOIN sap.movimentacaobens movimenta 
                                            ON movimentargp.mvbid=movimenta.mvbid
                                    INNER JOIN siape.vwservidorativo responsavel
                                            ON responsavel.nu_matricula_siape=movimenta.nu_matricula_siape
                                    INNER JOIN sap.material	material
                                        ON material.matid = bensmaterial.matid       
                                    INNER JOIN sap.catalogo catalogo
                                        ON catalogo.catid = material.catid
                                    INNER JOIN sap.classe classe
                                        ON classe.clscodclasse = catalogo.clscodclasse             
                                    INNER JOIN sap.situacaobem as situacaobem
                                        ON situacaobem.sbmid = rgp.sbmid       
                                    INNER JOIN sap.itemcontacontabil itemcontacontabil
                                        ON itemcontacontabil.icbid = catalogo.icbid
                                        AND itemcontacontabil.ccbid = catalogo.ccbid
                                    INNER JOIN sap.contacontabil  contacontabil
                                        ON contacontabil.ccbid = itemcontacontabil.ccbid WHERE 1=1 ";
     $sqlBensPatrimoniais .="AND contacontabil.ccbid = '{$rowResultado['conta_contabil_ccbid']}'
                            AND situacaobem.sbmcod = '{$rowResultado['sbmcod']}'";
                                
    if(!empty($_REQUEST['nu_matricula_siape'])) { $sqlBensPatrimoniais .= " and responsavel.nu_matricula_siape = '{$_REQUEST['nu_matricula_siape']}' "; }
    if(!empty($_REQUEST['no_servidor'])) { $sqlBensPatrimoniais .= " and responsavel.nu_matricula_siape = '{$_REQUEST['nu_matricula_siape']}' "; }
    if(!empty($_REQUEST['unidade'])) { $sqlBensPatrimoniais .= " and unidade.uorco_interno_uorg = '{$_REQUEST['unidade']}' "; }
    if(!empty($_REQUEST['endereco'])) { $sqlBensPatrimoniaisFull .= " and endereco.endid = '{$_REQUEST['endereco']}' "; }
    if(!empty($_REQUEST['endereco_andar'])) { $sqlBensPatrimoniais .= " and endandar.enaid = '{$_REQUEST['endereco_andar']}' "; }
    if(!empty($_REQUEST['endereco_sala'])) { $sqlBensPatrimoniais .= " and endandarsala.enaid = '{$_REQUEST['endereco_sala']}' "; }
    if(!empty($_REQUEST['descricao_do_bem_hidden'])) { $sqlBensPatrimoniais .= " and material.matid = '{$_REQUEST['descricao_do_bem_hidden']}' "; }
    if(!empty($_REQUEST['estado_de_conservacao'])) { $sqlBensPatrimoniais .= " and estadoconservacao.ecoid = '{$_REQUEST['estado_de_conservacao']}' "; }
    if(!empty($_REQUEST['situacao_do_bem'])) { $sqlBensPatrimoniais .= " and situacaobem.sbmcod = '{$_REQUEST['situacao_do_bem']}' "; }
    /*ORDER BY*/
    if(!empty($_REQUEST['tipo_relatorio'])) { $sqlBensPatrimoniais .= $classficarRelatorio; }
    //ver($sqlBensPatrimoniais);
$listaBensPatrimoniais= $db->carregar($sqlBensPatrimoniais);
?>
            <tr>
                <th>RGP</th>
                <th>Descrição do Bem</th>
                <th colspan="<?php if (!$ocultarNtermo and !$ocultarResponsavel) {;?>0<?php } elseif (!$ocultarNtermo or !$ocultarResponsavel){;?>2<?php }else{ ?>3<?php };?>">Localização do Bem <br /> (Unidade, Andar, Sala)
                </th>
                <th>Estado de Conservação</th>
                <?php if (!$ocultarNtermo):?>
                <th>Nº do Termo</th>
                <?php endif;?>
                <?php if (!$ocultarResponsavel):?>
                <th>Responsável</th>
                <?php endif;?>    
                <th>Valor Histórico </th>
                <th>Valor Depreciado </th>
            </tr>
            <?php $resultado = $listaBensPatrimoniais;
            $var_valor_total_historico = 0;
            $var_valor_total_depreciado = 0;
                foreach ( $resultado as $key => $rowResultadolista ): 
            ?>
                <tr>
                    <td class="bordaCelula"><?php echo $rowResultadolista['rgp']; ?></td>
                    <td class="bordaCelula" colspan="<?php if (!$ocultarNtermo and !$ocultarResponsavel) {;?>0<?php } elseif (!$ocultarNtermo or !$ocultarResponsavel){;?>2<?php }else{ ?>3<?php };?>"><?php echo $rowResultadolista['descricao']; ?></td>
                    <td class="bordaCelula"><?php echo $rowResultadolista['localizacao']; ?></td>
                    <td class="bordaCelula"><?php echo $rowResultadolista['estado_conservacao']; ?></td>
                    <?php if (!$ocultarNtermo) :?>
                    <td class="bordaCelula"><?php echo $rowResultadolista['numero_do_termo_responsavel']; ?></td>
                    <?php endif;?>
                    <?php if (!$ocultarResponsavel) :?>
                    <td class="bordaCelula"><?php echo $rowResultadolista['responsavel']; ?></td>
                    <?php endif;?>
                    <td class="bordaCelula"><?php echo number_format($rowResultadolista['valor_historio'],2,',','.'); ?></td>
                    <td class="bordaCelula"><?php echo number_format($rowResultadolista['valor_depreciado'],2,',','.'); ?></td>
                </tr>
            <?php 
            
            $var_valor_total_historico  += $rowResultadolista['valor_historio'];
            $var_valor_total_depreciado += $rowResultadolista['valor_depreciado'];
            
            endforeach; ?>
            <tr style="background-color:#CCCCCC;">
                <td colspan="6">
                    <b>QUANTIDADE  DE ITEM = </b> <?php echo count($listaBensPatrimoniais);?>
                </td>
                <td align="right">
                    <b>TOTAL HISTORICO = </b>
                     <?php echo number_format($var_valor_total_historico,2,',','.'); ?>
                    <?php
                        $quantidadeGeral += count($listaBensPatrimoniais);
                        $total_geral_historico      += $var_valor_total_historico;
                        $total_geral_depreciado     += $var_valor_total_depreciado;
                    ?>
                </td>
                 <td align="right">
                    <b>TOTAL DEPRECIADO = </b>
                    <?php echo number_format($var_valor_total_depreciado,2,',','.'); ?>
                </td>
            </tr>
            <tr style="background-color:#CCCCCC;">
                <td colspan="6"></td>
               <td></td>
               <td></td>
            </tr>
<?php endforeach; ?>
<tr style="background-color:#CCCCCC;">
    <td colspan="6">
        <b>QUANTIDADE GERAL = </b>
        <?php echo $quantidadeGeral; ?>
    </td>
    <td align="right">
        <b>TOTAL GERAL HISTORICO = </b>
        <?php echo number_format($total_geral_historico,2,',','.'); ?>
    </td>
    <td align="right">
        <b>TOTAL GERAL DEPRECIADO = </b>
        <?php echo number_format($total_geral_depreciado,2,',','.'); ?>
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