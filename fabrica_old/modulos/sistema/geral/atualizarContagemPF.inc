<?php

    // monta cabeçalho
    include APPRAIZ . 'includes/cabecalho.inc';
    print "<br/>";
    monta_titulo( "Atualizar Contagem de PF" , "" );

    include APPRAIZ."fabrica/classes/SolicitacaoServico.class.inc";
    $solicitacaoServico = new SolicitacaoServico();
    
    # Verifica se foi submetido
    if ( count( $valores = $_POST[ "valores" ] ) > 0 )
    {
        foreach( $valores as $tosid => $arrContagem )
        {
            foreach( $arrContagem as $scsidOdsidpai => $arrValor )
            {
                if ( $tosid != 1 )
                {
                    $odsid = "";
                    list( $scsid , $odsidpai ) = explode( "-" , $scsidOdsidpai );
                }
                else
                {
                    $odsid      = $scsidOdsidpai;
                    $scsid      = "";
                    $odsidpai   = "";
                }
                
                extract( $arrValor );
                if ( empty( $odssubtotalpf ) || empty( $odsqtdpfestimada ) && ( empty( $odssubtotalpfdetalhada ) || empty( $odsqtdpfdetalhada ) && $tosid == 1 ) ) $dadosVazios = true;

                $odssubtotalpf          = empty( $odssubtotalpf )           ? "NULL" : $odssubtotalpf;
                $odsqtdpfestimada       = empty( $odsqtdpfestimada )        ? "NULL" : $odsqtdpfestimada;
                $odssubtotalpfdetalhada = empty( $odssubtotalpfdetalhada )  ? "NULL" : $odssubtotalpfdetalhada;
                $odsqtdpfdetalhada      = empty( $odsqtdpfdetalhada )       ? "NULL" : $odsqtdpfdetalhada;
            
                if ( $tosid == 1 )
                {
                    $set        = " odssubtotalpf = " . $odssubtotalpf . " , odsqtdpfestimada = " . $odsqtdpfestimada . " , odssubtotalpfdetalhada = " . $odssubtotalpfdetalhada . " , odsqtdpfdetalhada = " . $odsqtdpfdetalhada;
                    $strWhere   = " odsid = " . $odsid;
                }
                else if ( $tosid == 2 )
                {
                    $set        = " odssubtotalpf = " . $odssubtotalpf . " , odsqtdpfestimada = " . $odsqtdpfestimada;
                    $strWhere   = " scsid = " . $scsid . " AND odsidpai = " . $odsidpai;
                }
                else
                {
                    $set        = " odssubtotalpf = " . $odssubtotalpf . " , odsqtdpfdetalhada = " . $odsqtdpfdetalhada;
                    $strWhere   = " scsid = " . $scsid . " AND odsidpai = " . $odsidpai;
                }
                
                if ( ( $odsid != "") || ( ( $scsid != "" ) && ( $odsidpai != "" ) ) )
                {
                    $sqlAtualizacao .= "UPDATE
                                            fabrica.ordemservico
                                        SET " . $set . "
                                        WHERE " . $strWhere . "
                                        AND
                                            tosid = " . $tosid . ";";
                }
                $odssubtotalpf          = "";
                $odsqtdpfestimada       = "";
                $odssubtotalpfdetalhada = "";
                $odsqtdpfdetalhada      = "";
            }
        }
        if ( $dadosVazios )
        {
            echo "<script>
                    if( !confirm( 'Existem campos não preenchidos. Deseja realmente atualizar ?' ) )
                    {
                        location.href='?modulo=sistema/geral/atualizarContagemPF&acao=A';
                    }
                </script>";
        }

        if ( $solicitacaoServico->executar( $sqlAtualizacao ) )
        {
            $solicitacaoServico->commit();
            echo "<script type='text/javascript'>
                    alert( 'Atualização concluída' );
                    location.href='?modulo=sistema/geral/atualizarContagemPF&acao=A';
                </script>";
        }
        else
        {
            $solicitacaoServico->rollback();
            echo "<script type='text/javascript'>alert('Não foi possível concluir a atualização.')</script>";
        }
    }
    
    # Busca os dados das OS no banco
    $strQuery = "SELECT
                    *
                FROM
                (
                    SELECT DISTINCT ON ( t1.odsid )
                        t1.odsid ,
                        t1.tosid ,
                        t1.scsid ,
                        t1.odssubtotalpf ,
                        t1.odsqtdpfestimada ,
                        t1.odssubtotalpfdetalhada ,
                        t1.odsqtdpfdetalhada ,
                        t2.odsidpai AS pai_t2,
                        t2.odssubtotalpf AS odssubtotalpf_t2 ,
                        t2.odsqtdpfestimada AS odsqtdpfestimada_t2 ,
                        t3.odsidpai AS pai_t3 ,
                        t3.odssubtotalpf AS odssubtotalpf_t3,
                        t3.odsqtdpfdetalhada AS odsqtdpfdetalhada_t3
                    FROM
                        fabrica.ordemservico t1
                    LEFT JOIN fabrica.ordemservico t2 ON
                        t1.odsid = t2.odsidpai and t2.tosid = 2
                    LEFT JOIN fabrica.ordemservico t3 ON
                        t1.odsid = t3.odsidpai and t3.tosid = 3
                    WHERE
                        t1.tosid = 1
                ) AS TMP
                ORDER BY
                    scsid , odsid";
    
    # Executa a query
    $arrOrdemServico = $dados = $solicitacaoServico->carregar( $strQuery );
?>
<style type="text/css">
    .listaAtualizacao td
    {
        text-align: center;
    }
    thead td
    {
        background-color: #e9e9e9;
        font-weight: bold;
    }
    .line1 td
    {
        background-color:#fff;
    }
    .line2 td
    {
        background-color:#efefef;
    }
</style>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script language='javascript' type='text/javascript' src='../includes/funcoes.js'></script>
<script type="text/javascript">
    Event.observe(window, 'load', function() {
        $$(".listaAtualizacao input").each(function(i){
            i.setAttribute('onmouseover', '');
            i.setAttribute('onmouseout', '');
        });
    });
</script>

<div style="width:100%;text-align: center;">
    <form id="formulario" name="formulario" action="" method="post" enctype="multipart/form-data" >
        <table class="tabela listaAtualizacao" cellspacing="1" cellpadding="3" align="center" border="0">
            <thead>
                <tr>
                    <td rowspan="3" align="center" width="4%"> SS </td>
                    <td rowspan="3" align="center" width="4%"> OS </td>
                    <td colspan="4" align="center"> Empresa Item 1 </td>
                    <td colspan="4" align="center"> Empresa Item 2 </td>
                </tr>
                <tr>
                    <td colspan="2"> Estimado </td>
                    <td colspan="2"> Detalhado </td>
                    <td colspan="2"> Estimado </td>
                    <td colspan="2"> Detalhado </td>
                </tr>
                <tr>
                    <td width="12%"> Subtotal de PF </td>
                    <td width="12%"> Qtd. PF </td>
                    <td width="12%"> Subtotal de PF Detalhado </td>
                    <td width="12%"> PF a Pagar </td>
                    <td width="12%"> Subtotal de PF </td>
                    <td width="12%"> Qtd. PF </td>
                    <td width="12%"> Subtotal de PF Detalhado </td>
                    <td width="12%"> PF a Pagar </td>
                </tr>
            </thead>
        </table>
        <div style="height: 300px; overflow-y: scroll; width: 95%;margin: auto auto auto auto">
            <table class="tabela listaAtualizacao" cellspacing="0" cellpadding="3" align="center" border="0" style="width:100%;">
                <tbody>
                    <?php
                    $i = 0; 
                    foreach( $arrOrdemServico as $ordemServico )
                    {?>
                        <tr class="line<?php echo $i++ % 2 == 0 ? "1" : "2" ; ?>">
                            <td width="4%">
                                <?php echo $ordemServico[ "scsid" ]; ?>
                            </td>
                            <td width="4%">
                                <?php echo $ordemServico[ "odsid" ]; ?>
                            </td>
                            <td width="12%">
                                <?php echo campo_texto( "valores[1][" . $ordemServico[ "odsid" ] . "][odssubtotalpf]"             , "N" , "S"     , "" , 10 , 10 , "[#].##" , "" , "" , "" , "" , 'id=""' , "" , $ordemServico[ "odssubtotalpf" ] );?>
                            </td>
                            <td width="12%">
                                <?php echo campo_texto( "valores[1][" . $ordemServico[ "odsid" ] . "][odsqtdpfestimada]"          , "N" , "S"     , "" , 10 , 10 , "[#].##" , "" , "" , "" , "" , 'id=""' , "" , $ordemServico[ "odsqtdpfestimada" ] );?>
                            </td>
                            <td width="12%">
                                <?php echo campo_texto( "valores[1][" . $ordemServico[ "odsid" ] . "][odssubtotalpfdetalhada]"    , 'N' , "S"     , "" , 10 , 10 , "[#].##" , "" , "" , "" , "" , 'id=""' , "" , $ordemServico[ "odssubtotalpfdetalhada" ] );?>
                            </td>
                            <td width="12%">
                                <?php echo campo_texto( "valores[1][" . $ordemServico[ "odsid" ] . "][odsqtdpfdetalhada]"         , "N" , "S"     , "" , 10 , 10 , "[#].##" , "" , "" , "" , "" , 'id=""' , "" , $ordemServico[ "odsqtdpfdetalhada"] );?>
                            </td>
                            <?php $habil = ( ! is_null( $ordemServico[ "pai_t2" ] ) ) ? "S" : "N";?>
                            <td width="12%">
                                <?php echo campo_texto( "valores[2][" . $ordemServico[ "scsid" ] . "-" . $ordemServico[ "pai_t2" ] . "][odssubtotalpf]"             , "N" , $habil  , "" , 10 , 10 , "[#].##" , "" , "" , "" , "" , 'id=""' , "" , $ordemServico[ "odssubtotalpf_t2" ] );?>
                            </td>
                            <td width="12%">
                                <?php echo campo_texto( "valores[2][" . $ordemServico[ "scsid" ] . "-" . $ordemServico[ "pai_t2" ] . "][odsqtdpfestimada]"          , "N" , $habil  , "" , 10 , 10 , "[#].##" , "" , "" , "" , "" , 'id=""' , "" , $ordemServico[ "odsqtdpfestimada_t2" ] );?>
                            </td>
                            <?php $habil = ( ! is_null( $ordemServico[ "pai_t3" ] ) ) ? "S" : "N";?>
                            <td width="12%">
                            <?php if ($ordemServico[ "odsqtdpfdetalhada_t3" ] == ""){
                            	 $ordemServico[ "odssubtotalpf_t3" ] = "";
                            	 }?>    	
                            	<?php echo campo_texto( "valores[3][" . $ordemServico[ "scsid" ] . "-" . $ordemServico[ "pai_t3" ] . "][odssubtotalpf]"             , "N" , $habil  , "" , 10 , 10 , "[#].##" , "" , "" , "" , "" , 'id=""' , "" , $ordemServico[ "odssubtotalpf_t3" ] );?>
                            </td>
                            <td width="12%">
                                <?php echo campo_texto( "valores[3][" . $ordemServico[ "scsid" ] . "-" . $ordemServico[ "pai_t3" ] . "][odsqtdpfdetalhada]"         , "N" , $habil  , "" , 10 , 10 , "[#].##" , "" , "" , "" , "" , 'id=""' , "" , $ordemServico[ "odsqtdpfdetalhada_t3" ] );?>
                            </td>
                        </tr>
                    <?php
                    } ?>
                </tbody>
            </table>
        </div>
        <table class="tabela listaAtualizacao" cellspacing="1" cellpadding="3" align="center" border="0">
            <tbody>
                <tr>
                    <td colspan="9">
                        <input type="submit" name="atualizar" id="atualizar" value="Atualizar" />
                        <input type="reset" name="reset" id="reset" value="Desfazer" />
                    </td>
                </tr>
            </tbody>
        </table>
    </form>
</div>