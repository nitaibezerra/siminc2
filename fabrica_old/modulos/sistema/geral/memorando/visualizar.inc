<?php
header('content-type: text/html; charset=iso-8859-1;');

include_once APPRAIZ . 'includes/cabecalho.inc';
print "<br/>";

$memorandoRepositorio   = new MemorandoRepositorio();
$memorando              = $memorandoRepositorio->recuperePorId( $_GET['memo'] );
$ordemServico           = new OrdemServico ();

$dadosGlosaMemorando   = '';

if( $memorando->possuiGlosa()  ){
    $dadosGlosaMemorando = $memorandoRepositorio->getDadosGlosaMemorando( $memorando->getGlosaMemorando() );
}

// Popula Tabela de Relatório
function geraListaMemorandoOs( $listaOs = array() ){
    
    global $db;
    
    // Valida se existe lista de OS 
    if( count($listaOs) > 0 ){
        
        foreach ($listaOs as $chave => $valor ){
            
            $sql          = "select odsid from fabrica.memorandorelatorio
                             where odsid = " . $valor['odsid'] . ' and memoid = ' . $valor['memoid'];
            $resultado    = $db->pegaLinha( $sql );
            
            if( $resultado === false){
                
                $xporcentagemdisciplina  = empty($valor['xporcentagemdisciplina']) ? 'null' : $valor['xporcentagemdisciplina'];
                $xpagarpfporcentagem     = empty($valor['xpagarpfporcentagem'])    ? 'null' : $valor['xpagarpfporcentagem'];
                $xvlpfunitario           = empty($valor['vlpfunitario'])           ? 'null' : $valor['vlpfunitario'];
                $xvltotal                = empty($valor['vltotal'])                ? 'null' : $valor['vltotal'];
                
                $insMemoRel = "INSERT INTO fabrica.memorandorelatorio(odsid, scsid
                                , memoid, ctrid, qtdpf, glosapf, pagaraposglosa
                                , xporcentagemdisciplina, xpagarpfporcentagem, vlpfunitario, vltotal
                                , mosdtcadastro, mosstatus)";
                $insMemoRel.= " VALUES(";
                $insMemoRel.= $valor['odsid'];
                $insMemoRel.= "," . $valor['scsid'];
                $insMemoRel.= "," . $valor['memoid'];
                $insMemoRel.= "," . $valor['ctrid'];
                $insMemoRel.= "," . $valor['qtdpf'];
                $insMemoRel.= "," . $valor['glosapf'];
                $insMemoRel.= "," . $valor['pagaraposglosa'];
                $insMemoRel.= "," . $xporcentagemdisciplina;
                $insMemoRel.= "," . $xpagarpfporcentagem;
                $insMemoRel.= "," . $xvlpfunitario;
                $insMemoRel.= "," . $xvltotal;
                $insMemoRel.= ", current_timestamp";
                $insMemoRel.= ", 'A'";
                $insMemoRel.= ")  RETURNING mosid;";
                
                if( !$db->pegaUm($insMemoRel) ){
                    throw new Exception( "Não foi possível gerar relatório de OS." );
                }

                $db->commit();
            }
        }
    }    
}
?>
<br/>

<script type="text/javascript" src="./js/jquery-1.7.min.js"></script>
<script type="text/javascript" src="./js/jquery-ui-1.8.16.custom.min.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/jquery.validate.js"></script>
<script type="text/javascript" src="./js/fabrica.js"></script>

<link href="./css/fabrica.css" type="text/css" rel="stylesheet" media="all"/>

<div class="notprint">
    <?php if($memorando->getStatusMemorando()==StatusMemorando::MEMORANDO_IMPRESSO){?>
        <input class="notprint" type="button" value="Imprimir" onclick="window.print();"/>
    <?php } ?>
    <input class="notprint" type="button" value="Voltar" onclick="parent.location='?modulo=sistema/geral/memorando/listar&acao=A'" />
</div>
<?php 
if( !empty($_GET['memo']) ){
    
    //Print Nota Fiscal
    $sql = "select 
                b.nfsdsc as notafiscal
            from fabrica.memorando a
            inner join fabrica.notafiscal b 
        	    on a.nfsid = b.nfsid
            where a.memoid = ".$_GET['memo'];
    
    $resNotaFiscal = $db->pegaUm( $sql );
    
    if( $resNotaFiscal ){
        echo '<div class="conteudoNotaFiscalMemorando"><p>Nota fiscal nº. ';
        echo     $resNotaFiscal;
        echo '</p></div>';
    }
    
    $sql          = "select * from fabrica.memorandogerado where mmgstatus='A' and memoid = ".$_GET['memo']; 
    $resultado    = $db->pegaLinha( $sql );
    $idMemorando  = $_GET['memo'];
   
    if( $resultado === false){
        
        try {
            
            // Início geração HTML do Memorando
            $memorandoRepositorio   = new MemorandoRepositorio();
            $memorando              = $memorandoRepositorio->recuperePorId( $idMemorando );
            $ordemServico           = new OrdemServico ();
            $htmlGerado             = '';
            $dadosOs                = array();
            
            $dadosGlosaMemorando   = '';
            
            if( $memorando->possuiGlosa()  ){
                $dadosGlosaMemorando = $memorandoRepositorio->getDadosGlosaMemorando( $memorando->getGlosaMemorando() );
            }
            
            $htmlGerado = '<div id="conteudoMemorando" > ';
            $htmlGerado.= '<p>Memo n&ordm;. ' . $memorando->getNumeroMemorando() . '/' . $memorando->getDataMemorando()->format("Y") . '/CGD/DTI/SE/MEC</p>';
            $htmlGerado.= '	<p class="dataMemorando"> '. $memorando->getDataMemorandoFormatada() . '.</p>';
            $htmlGerado.= '<div class="textoMemorando">';
            $htmlGerado.= $memorando->getTextoMemorando();
            $htmlGerado.= '</div>';
            $htmlGerado.= '  	<div class="servidorResponsavelMemorando">';
            $htmlGerado.= '		<p>' . $memorando->getFiscal()->getNome() . '</p>';
            $htmlGerado.= '     <p>Fiscal T&eacute;cnico</p>';
            $htmlGerado.= ' <p>Coordena&ccedil;&atilde;o Geral de Desenvolvimento</p>';
            $htmlGerado.= ' <p>Diretoria de Tecnologia da Informa&ccedil;&atilde;o</p>';
            $htmlGerado.= ' </div>';
            $htmlGerado.= ' </div>';
            $htmlGerado.= ' <div style="page-break-before: always;">';
            $htmlGerado.= '';
            $htmlGerado.= '<h1 class="tituloAnexo QuebraDePagina">Anexo</h1>';
            
            if( $memorando->getPrestadorServicoMemorando() == PrestadorServico::PRESTADORA_SERVICO_FABRICA || $memorando->getPrestadorServicoMemorando() == PrestadorServico::PRESTADORA_SERVICO_POLITEC ) {
            
                $listaOsSelecionadas = $ordemServico->recupereOSQueEstaoVinculadasAoMemorandoFabrica( $idMemorando );
            
                $htmlGerado.= '<table class="listagem" cellspacing="0" cellpadding="2" border="0" align="center" width="95%">';
                $htmlGerado.= '';
                $htmlGerado.= '<thead>';
                $htmlGerado.= '<tr>';
                $htmlGerado.= '<th>Solicita&ccedil;&atilde;o de Serviço</th>';
                $htmlGerado.= '<th>Ordem de Serviço</th>';
                $htmlGerado.= '<th>Quantidade de Pontos de Função</th>';
                $htmlGerado.= '<th>Porcentagens das disciplinas</th>';
                $htmlGerado.= '<th>PF a pagar com % de esforço</th>';
                $htmlGerado.= '<th>Glosa(PF)</th>';
                $htmlGerado.= '<th>PF a pagar após cálculo da glosa</th>';
                $htmlGerado.= '<th>Valor de Ponto de Função Unitário</th>';
                $htmlGerado.= '<th colspan=\'2\' width=\'8%\'>Valor Total</th>';
            
                $htmlGerado.= '</tr>';
                $htmlGerado.= '</thead>';
                $htmlGerado.= '<tbody>';
            
                if ( $listaOsSelecionadas != null ) {
                    $count = 1;
                    $contaLinha = 0;
            
                    foreach ( $listaOsSelecionadas as $os ){
                        //Calculando a glosa;
                        ++$contaLinha;
                        $pfComEsforco       = 0;
                        $pfComEsforcoGLosa  = 0;
            
                        if( $os->possuiGlosa() ){
                            $glosa 				= new Glosa();
                            $glosa 				= $glosa->recupereGlosaPeloId($os->getIdGlosa());
                            $valorGlosa 		= $glosa->getValorEmPf();
                            $valorTotalAReceber = $os->getValorAReceberGlosado();
                        } else {
                            $valorGlosa 		= 0;
                            $valorTotalAReceber = $os->getValorAReceberDaOs();
                        }
            
                        $subTotalPorcentagemEsforco = $valorTotalAReceber;
                        $subTotalQtdePontoFuncao    = $subTotalQtdePontoFuncao + $os->getMenorValorPF();
                        $subTotalQtdeGlosa          = $subTotalQtdeGlosa + $valorGlosa;
                        $subTotalAReceber           = $subTotalAReceber + $valorTotalAReceber;
                        $menorValorPF               = $os->getMenorValorPF();
                        $porcentagemDisciplina      = $os->getPorcentagemDisciplina();
            
                        $pfComEsforco = ($menorValorPF  * $porcentagemDisciplina) / 100;
                        $pfComEsforcoGLosa  = $pfComEsforco - $valorGlosa;

                        $subTotalpfComEsforcoGLosa = $subTotalpfComEsforcoGLosa + $pfComEsforcoGLosa;
            
                        $htmlGerado.= '<tr ';
                        $htmlGerado.= $count % 2 ? 'class="even"' : 'class="odd"';
                        $htmlGerado.= '>';
                        $htmlGerado.= '<td class="alignCenter">';
                        $htmlGerado.= $os->getIdSolicitacaoServico();
                        $htmlGerado.= '</td>';
                        $htmlGerado.= '<td class="alignCenter">';
                        $htmlGerado.= $os->getId();
                        $htmlGerado.= '</td>';
                        $htmlGerado.= '<td class="alignRight">';
                        $htmlGerado.= number_format($menorValorPF,2,",",".");
                        $htmlGerado.= '</td>';
                        $htmlGerado.= '<td class="alignRight">';
                        $htmlGerado.= number_format($porcentagemDisciplina,2,",",".") . "%";
                        $htmlGerado.= '</td>';
                        $htmlGerado.= '<td class="alignRight">';
                        $htmlGerado.= number_format($pfComEsforco,2,",",".");
                        $htmlGerado.= '</td>';
                        $htmlGerado.= '<td class="alignCenter">';
                        $htmlGerado.= $valorGlosa;
                        $htmlGerado.= '</td>';
                        $htmlGerado.= '<td class="alignCenter">';
                        $htmlGerado.= number_format($pfComEsforcoGLosa,2,",",".");
                        $htmlGerado.= '</td>';
                        $htmlGerado.= '<td class="alignRight">';
                        $htmlGerado.= "R$ " . number_format($os->getValorUnitarioDePf(),2,",",".");
                        $htmlGerado.= '</td>';
                        $htmlGerado.= '<td class="alignRight" colspan="2">';
                        $htmlGerado.= "R$ " . number_format($valorTotalAReceber,2,",",".");
                        $htmlGerado.= '</td>';
            
                        $count = $count + 1;
                        	
                        $htmlGerado.= '</tr>';
                        
                        //echo('<pre>');var_dump( $os );exit;

                        $dadosOs[] = array('odsid'                 => $os->getId()
                                         , 'scsid'                 => $os->getIdSolicitacaoServico()
                                         , 'memoid'                => $idMemorando
                                         , 'ctrid'                 => $os->getIdContrato()
                                         , 'qtdpf'                 => $menorValorPF
                                         , 'xporcentagemdisciplina'=> $porcentagemDisciplina
                                         , 'xpagarpfporcentagem'   => $pfComEsforco
                                         , 'glosapf'               => $valorGlosa
                                         , 'pagaraposglosa'        => $pfComEsforcoGLosa
                                         , 'vlpfunitario'          => $os->getValorUnitarioDePf()
                                         , 'vltotal'               => $valorTotalAReceber ) ;
                    }
                    
                } else {
            
                    $htmlGerado.= '<tr>';
                    $htmlGerado.= ' <td colspan="10">Não foram encontrados registros</td>';
                    $htmlGerado.= '</tr>';
                }
                
                geraListaMemorandoOs( $dadosOs );
            
                $htmlGerado.= '</tbody>';
                $htmlGerado.= '<tfoot>';
            
                $descricaoAjusteMemorando   = $memorando->getDescricaoAjuste();
                $valorAjuste                = $memorando->getValorAjuste();
            
                if( !empty( $descricaoAjusteMemorando ) ){
                    $subTotalAReceber -= $valorAjuste;
            
                    $htmlGerado.= '<tr>';
                    $htmlGerado.= '<td></td>';
                    $htmlGerado.= '<td class="alignCenter" colspan="7" >';
                    $htmlGerado.= nl2br($descricaoAjusteMemorando);
                    $htmlGerado.= '</td>';
                    $htmlGerado.= '<td class="alignRight" style="color: red;">';
                    $htmlGerado.= 'R$ '. number_format( $valorAjuste , 2, ",", ".");
                    $htmlGerado.= '</td>';
                    $htmlGerado.= '</tr>';
            
                }
            
                if( !empty($dadosGlosaMemorando) )
                {
                    $valorGlosa = $dadosGlosaMemorando['tpglmemopercvalor'] * ( $subTotalAReceber / 100 );
                    $subTotalAReceber -= $valorGlosa;
            
                    $htmlGerado.= '<tr>';
                    $htmlGerado.= '<td class="alignCenter" colspan="2">';
                    $htmlGerado.= 'Grau '. $dadosGlosaMemorando['tpglmemograuvalor'];
                    $htmlGerado.= '</td>';
                    $htmlGerado.= ' <td class="alignCenter" colspan="5" >';
                    $htmlGerado.= nl2br( $memorando->getJustificativaGlosaMemorando() );
                    $htmlGerado.= '</td>';
                    $htmlGerado.= '<td class="alignRight">';
                    $htmlGerado.= number_format($dadosGlosaMemorando['tpglmemopercvalor'], 1, ",", ".");
                    $htmlGerado.= '%';
                    $htmlGerado.= '</td>';
                    $htmlGerado.= '<td class="alignRight" style="color: red;">';
                    $htmlGerado.= "R$ " . number_format($valorGlosa, 2, ",", ".");
                    $htmlGerado.= '</td>';
                    $htmlGerado.= '</tr>';
                }
            
                $htmlGerado.= '<tr>';
                $htmlGerado.= ' <td class="alignRight" colspan="2">Total:</td>';
                $htmlGerado.= ' <td class="alignRight">';
                $htmlGerado.= number_format($subTotalQtdePontoFuncao,2,",",".");
                $htmlGerado.= '</td>';
                $htmlGerado.= '<td></td>';
                $htmlGerado.= '<td></td>';
                $htmlGerado.= '<td class="alignCenter">';
                $htmlGerado.= number_format($subTotalQtdeGlosa, 2, ",", ".");
                $htmlGerado.= '</td>';
                $htmlGerado.= '<td class="alignCenter">';
                $htmlGerado.= number_format($subTotalpfComEsforcoGLosa, 2, ",", ".");
                $htmlGerado.= '</td>';
                $htmlGerado.= '<td></td>';
                $htmlGerado.= '<td class="alignRight"  colspan="2">';
                $htmlGerado.= "R$ " . number_format($subTotalAReceber, 2, ",", ".");
                $htmlGerado.= '</td></tr><tr>';
                $htmlGerado.= "<td colspan='10'>Total de Registros: ";
                $htmlGerado.= $contaLinha;
                $htmlGerado.= '</td></tr></tfoot></table>';
            
            } else {
                $listaOsSelecionadas = $ordemServico->recupereOSQueEstaoVinculadasAoMemorandoAUDITORA( $_GET['memo'] );
            
                $htmlGerado.= '<table class="listagem" cellspacing="0" cellpadding="2" border="0" align="center" width="95%">';
                $htmlGerado.= '<thead><tr>';
                $htmlGerado.= '<th>Solicita&ccedil;&atilde;o de Serviço</th>';
                $htmlGerado.= '<th>Ordem de Serviço</th>';
                $htmlGerado.= '<th>Quantidade de Pontos de Função</th>';
                $htmlGerado.= '<th>Glosa (PF)</th>';
                $htmlGerado.= '<th>PF a pagar após cálculo da glosa</th>';
                $htmlGerado.= '<th>Valor de Ponto de Função Unitário</th>';
                $htmlGerado.= '<th width="8%">Valor Total</th>';
                $htmlGerado.=  '</tr> </thead> <tbody>';
            
                if ( $listaOsSelecionadas!=null ) {

                    $count = 1;
                    $subTotalQtdePontoFuncao    = 0;
                    $subTotalAReceber           = 0;
                    $totalPFGlosa               = 0;
                    $contaLinha                 = 0;
            
                    foreach ( $listaOsSelecionadas as $os ) {
                        ++$contaLinha;
                        $qtdPFGlosa                 = 0;
                        $valorGlosa                 = 0;
                        $pfComGlosa                 = 0;
            
                        $menorValorPF               = $os->getMenorValorPFEmpresaItem2( $os->getId());
                        $valorUnitarioPF            = $os->getValorUnitarioDePf();
                        $valorTotalAReceber         = $valorUnitarioPF * $menorValorPF;
                        $subTotalQtdePontoFuncao    = $subTotalQtdePontoFuncao + $menorValorPF;

                        if ( $os->possuiGlosa() ) {
                            $glosa      = new Glosa();
                            $glosa      = $glosa->recupereGlosaPeloId( $os->getIdGlosa() );
                            $qtdPFGlosa = $glosa->getValorEmPf();
                            $valorGlosa = $qtdPFGlosa * $valorUnitarioPF;
                        }
            
                        $valorTotalAReceber         -= $valorGlosa;
                        $subTotalAReceber           = $subTotalAReceber + $valorTotalAReceber;
                        $pfComGlosa                 = $menorValorPF - $qtdPFGlosa;
                        $totalPFGlosa               += $qtdPFGlosa;

                        $subTotalpfComGlosa = $subTotalpfComGlosa + $pfComGlosa;
                        
                        $htmlGerado.= '<tr ';
                        $htmlGerado.= $count % 2 ? 'class="even"' : 'class="odd"' ;
                        $htmlGerado.= '>';
                        $htmlGerado.= ' <td class="alignCenter">';
                        $htmlGerado.= $os->getIdSolicitacaoServico();
                        $htmlGerado.= '</td>';
                        $htmlGerado.= '<td class="alignCenter">';
                        $htmlGerado.= $os->getId();
                        $htmlGerado.= '</td>';
                        $htmlGerado.= '<td class="alignRight">';
                        $htmlGerado.= number_format($menorValorPF,2,",",".");
                        $htmlGerado.= '</td>';
                        $htmlGerado.= '<td class="alignCenter">';
                        $htmlGerado.= number_format( $qtdPFGlosa, 2, ',', '.' );
                        $htmlGerado.= '</td>';
            
                        $htmlGerado.= '<td class="alignCenter">';
                        $htmlGerado.= number_format($pfComGlosa, 2, ',', '.' );
                        $htmlGerado.= '</td>';
                        $htmlGerado.= '<td class="alignCenter">';
                        $htmlGerado.= "R$ " . number_format( $valorUnitarioPF, 2, ',', '.' );
                        $htmlGerado.= '</td>';
                        $htmlGerado.= '<td class="alignRight">';
                        $htmlGerado.= "R$ " . number_format( $valorTotalAReceber, 2, ',', '.' );
                        $htmlGerado.='</td>';
            
                        $count = $count + 1;
            
                        $htmlGerado.= '</tr>';
            
                        //echo('<pre>');var_dump( $os );exit;

                        $dadosOs[] = array('odsid'                 => $os->getId()
                                         , 'scsid'                 => $os->getIdSolicitacaoServico()
                                         , 'memoid'                => $idMemorando
                                         , 'ctrid'                 => $os->getIdContrato()
                                         , 'qtdpf'                 => $menorValorPF
                                         , 'xporcentagemdisciplina'=> null
                                         , 'xpagarpfporcentagem'   => null
                                         , 'glosapf'               => $qtdPFGlosa
                                         , 'pagaraposglosa'        => $pfComGlosa
                                         , 'vlpfunitario'          => $valorUnitarioPF
                                         , 'vltotal'               => $valorTotalAReceber ) ;
                    }
                    
                } else {
            
                    $htmlGerado.= '<tr>';
                    $htmlGerado.= '<td colspan="5">N&atilde;o foram encontrados registros</td>';
                    $htmlGerado.= '</tr>';
            
                }

                geraListaMemorandoOs( $dadosOs );

                $htmlGerado.= '</tbody>';
                $htmlGerado.= ' <tfoot>';
            
                $descricaoAjusteMemorando = $memorando->getDescricaoAjuste();
                $valorAjuste              = $memorando->getValorAjuste();
            
                if( !empty( $descricaoAjusteMemorando ) ){
                    $subTotalAReceber -= $valorAjuste;
            
                    $htmlGerado.= '<tr>';
                    $htmlGerado.= '<td></td>';
                    $htmlGerado.= '<td class="alignCenter" colspan="5" >';
                    $htmlGerado.= nl2br($descricaoAjusteMemorando);
                    $htmlGerado.= '</td>';
                    $htmlGerado.= '<td class="alignRight" style="color: red;">';
                    $htmlGerado.= 'R$ ' . number_format( $valorAjuste , 2, ",", ".");
                    $htmlGerado.= '</td></tr>';
            
                }
            
                if( !empty($dadosGlosaMemorando) ) {
                    $valorGlosa         = $dadosGlosaMemorando['tpglmemopercvalor'] * ( $subTotalAReceber / 100 );
                    $subTotalAReceber   -= $valorGlosa;
            
                    $htmlGerado.= '<tr>';
                    $htmlGerado.= '<td class="alignRight" colspan="3">';
                    $htmlGerado.= 'Grau '. $dadosGlosaMemorando['tpglmemograuvalor'];
                    $htmlGerado.= '</td>';
                    $htmlGerado.= '<td class="alignCenter" colspan="2" >';
                    $htmlGerado.= $memorando->getJustificativaGlosaMemorando();
                    $htmlGerado.= '</td>';
                    $htmlGerado.= '<td class="alignCenter">';
                    $htmlGerado.= number_format($dadosGlosaMemorando['tpglmemopercvalor'], 1, ",", ".");
                    $htmlGerado.= '%';
                    $htmlGerado.= '</td>';
                    $htmlGerado.= '<td class="alignRight" style="color: red;">';
                    $htmlGerado.= "R$ " . number_format($valorGlosa, 2, ",", ".");
                    $htmlGerado.= '</td>';
                    $htmlGerado.= '</tr>';
            
                }
            
                $htmlGerado.= '<tr>';
                $htmlGerado.= '<td class="alignRight" colspan="2">Total:</td>';
                $htmlGerado.= '<td class="alignRight">';
                $htmlGerado.= number_format( $subTotalQtdePontoFuncao, 2, ',', '.' );
                $htmlGerado.= '</td>';
                $htmlGerado.= '<td class="alignCenter">';
                $htmlGerado.= number_format( $totalPFGlosa, 2, ',', '.' ) ;
                $htmlGerado.= '</td>';
                $htmlGerado.= '<td class="alignCenter">';
                $htmlGerado.= number_format( $subTotalpfComGlosa, 2, ',', '.' ) ;
                $htmlGerado.= '</td>';
                $htmlGerado.= '<td></td>';
                $htmlGerado.= '<td class="alignRight">';
                $htmlGerado.= "R$ " . number_format( $subTotalAReceber, 2, ',', '.' ) ;
                $htmlGerado.= '</td>';
                $htmlGerado.= '</tr><tr>';
                $htmlGerado.= '<td colspan="7">Total de Registros: ';
                $htmlGerado.= $contaLinha;
                $htmlGerado.= '</td>';
                $htmlGerado.= '</tr>';
                $htmlGerado.= '</tfoot>';
                $htmlGerado.= '</table>';
            }
            
            $htmlGerado.= '</div>';
            $htmlGerado.= '<script type="text/javascript">';
            $htmlGerado.= "$('#conteudoMemorando ol li').wrapInner('<span>');";
            $htmlGerado.= '</script>';
            // Fim geração HTML
            
            $sqlInsert = "INSERT INTO fabrica.memorandogerado(mmghtml, memoid, mmgdtcadastro, mmgstatus)";
            $sqlInsert.= " VALUES ('". addslashes( $htmlGerado ) . "', ". $idMemorando .", current_timestamp, 'A') RETURNING mmgid;";
            
            //die( $sqlInsert );
            
            if( !$db->pegaUm($sqlInsert) ){
                throw new Exception( "Não foi possível gerar memorando." );
            }
            
            $status     = true;
            $retorno    = $idMemorando;
            
            $db->commit();
            
        } catch ( Exception $e ) {
            $db->rollback();
            $status  = false;
            $retorno = $e->getMessage();
        }
        
        echo $htmlGerado;

        //echo '<script type="text/javascript">';
        //echo "alert('Memorando migrado com sucesso.');";
        //echo '</script>';

    }else{

        echo( $resultado['mmghtml'] );   
    }
}
?>
