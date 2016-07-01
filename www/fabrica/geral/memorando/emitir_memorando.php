<?php
header('content-type: text/html; charset=iso-8859-1;');

include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/classes/Modelo.class.inc";
include_once APPRAIZ . "includes/workflow.php";

include_once APPRAIZ . 'fabrica/classes/StatusMemorando.class.inc';
include_once APPRAIZ . 'fabrica/classes/autoload.inc';
include_once APPRAIZ . 'www/fabrica/_constantes.php';
include_once APPRAIZ . 'www/fabrica/_funcoes.php';
include_once APPRAIZ . 'www/fabrica/_componentes.php';

$db = new cls_banco();

$status  = false;
$retorno = '';

$idEmpresaContratada = (int) $_POST['empresaContratada'];
$idTipoGlosa         = (int) $_POST['array'];
$numeroMemorando     = $_POST['numeroMemorando'];
$cpfServidor         = $_POST['cpfServidorPublico'];
$justificativaGlosa  = $_POST['justificativaGlosaMemorando'];
$textoMemorando      = utf8_decode( $_POST['textoMemorando']);
$descricaoAjuste     = utf8_decode( $_POST['memodscajuste'] );
$dataMemorando       = formata_data_sql( $_POST['dataMemorando'] );
$ordensServico       = $_POST['osSelecionadas'];

$memovlrajuste       = ( $_POST['memovlrajuste'] ? str_replace('.' , '', $_POST['memovlrajuste']) : null );
$memovlrajuste       = (float) ( $memovlrajuste ? str_replace(',' , '.', $memovlrajuste) : null );

$idTipoGlosa         = $idTipoGlosa == 0 ? 'NULL' : $idTipoGlosa;

try {
    
    $sqlDadosFiscal = "SELECT DISTINCT u.usucpf
                            FROM fabrica.fiscalcontrato f
                            INNER JOIN seguranca.usuario u 
                                ON u.usucpf = f.usucpf
                            where u.usucpf = '{$cpfServidor}'";
                            
    $dadosFiscal    = $db->pegaUm($sqlDadosFiscal);
    
    if( $dadosFiscal == false )
    {
        throw new Exception( "O CPF: {$cpfServidor} informado não é de um fiscal válido" );
    }
    
    $sqlAtualizaMemrando = "UPDATE fabrica.memorando SET 
                            memonumero                   = {$numeroMemorando}
                            , memocpfservidorresponsavel = '{$cpfServidor}'
                            , memoidprestadorservico     = {$idEmpresaContratada}
                            , memotexto                  = '{$textoMemorando}'
                            , memodata                   = '{$dataMemorando}' 
                            , memostatus                 = '". StatusMemorando::MEMORANDO_IMPRESSO  ."'
                            , tpglmemoid                 = {$idTipoGlosa}
                            , memojustificativaglosa     = '{$justificativaGlosa}'
                            , memodscajuste              = '{$descricaoAjuste}'
                            , memovlrajuste              = {$memovlrajuste}
                            WHERE memoid = {$_POST['memo']} RETURNING memoid;";
                            
                            
     $idMemorando = $db->pegaUm( $sqlAtualizaMemrando );
    
    if( !$idMemorando) 
    {
        throw new Exception('Não foi possível emitir o memorando');
    }
    
    foreach ( $ordensServico as $odsId ) {
        
        $odsId = (int) $odsId;
        
        $sqlDadosOS = "SELECT os.odsid, os.tosid
                                , CASE os.tosid
                                    WHEN ". TIPO_OS_GERAL ." THEN docos.docid
                                    ELSE docpf.docid
                                END as docid,
                                os.scsid	
                            FROM fabrica.memorando memo
                            INNER JOIN fabrica.ordemservico os
                                ON memo.memoid = os.memoid
                            LEFT JOIN workflow.documento docos
                                ON os.docid = docos.docid
                            LEFT JOIN workflow.documento docpf
                                ON os.docidpf = docpf.docid 
                            WHERE os.odsid = {$odsId}";
                            
        $dadosOS = $db->pegaLinha( $sqlDadosOS );
        
        $arrSS[] = $dadosOS['scsid'];
        
        if (  $dadosOS['tosid'] == TIPO_OS_GERAL ) 
        {
            $wfEstadoAguardandoPagamento = WF_ESTADO_OS_AGUARDANDO_PAGAMENTO;
            $wfEstadoFinalizada          = WF_ESTADO_OS_FINALIZADA;
        }else{
            $wfEstadoAguardandoPagamento = WF_ESTADO_CPF_AGUARDANDO_PAGAMENTO;
            $wfEstadoFinalizada          = WF_ESTADO_CPF_FINALIZADA;
        }

        // Pega Estado Atual
        $rsEstadoAtual = wf_pegarEstadoAtual( $dadosOS['docid'] );
        
        // Deixa tramitar somente OS's no estado AGUARDANDO_PAGAMENTO
        if( $rsEstadoAtual['esdid'] == WF_ESTADO_OS_AGUARDANDO_PAGAMENTO 
                || $rsEstadoAtual['esdid'] == WF_ESTADO_CPF_AGUARDANDO_PAGAMENTO ){

            $dados    = wf_pegarAcao( $wfEstadoAguardandoPagamento, $wfEstadoFinalizada );
            $alterado = wf_alterarEstado( $dadosOS['docid'], $dados['aedid'], 'Memorando finalizado', array( 'odsid' => $odsId ) );

            if ( !$alterado ) 
            {
                throw new Exception( "Não foi possível tramitar a OS nº {$odsId} para o estado Finalizada" );
            }

            $sqlAtualizaOS = "UPDATE fabrica.ordemservico SET memoid = {$idMemorando} WHERE odsid = {$odsId} RETURNING odsid";

            if( !$db->pegaUm( $sqlAtualizaOS) )
                throw new Exception( "Não foi possível atualizar a OS nº {$odsId}" );
        }else{
            //TODO remover esta exception antes de enviar para Produção
            //throw new Exception( "OS nº {$odsId} esta no estado ".$rsEstadoAtual['esddsc'] );
        }
    }
    

    // Início geração HTML do Memorando
    $memorandoRepositorio   = new MemorandoRepositorio();
    $memorando              = $memorandoRepositorio->recuperePorId( $idMemorando );
    $ordemServico           = new OrdemServico ();
    $htmlGerado             = '';
    
    $dadosGlosaMemorando   = '';
    
    if( $memorando->possuiGlosa()  ){
        $dadosGlosaMemorando = $memorandoRepositorio->getDadosGlosaMemorando( $memorando->getGlosaMemorando() );
    }
    
    $htmlGerado = '<div id="conteudoMemorando" > ';
    $htmlGerado.= '<p>Memo n&ordm;. ' . $memorando->getNumeroMemorando() . '/' . $memorando->getDataMemorando()->format("Y") . '/CGD/DTI/SE/MEC</p>';
    $htmlGerado.= '	<p class="dataMemorando"> '. $memorando->getDataMemorandoFormatada() . '.</p> ';
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
    
    if( $memorando->getPrestadorServicoMemorando() == PrestadorServico::PRESTADORA_SERVICO_FABRICA ||
    	$memorando->getPrestadorServicoMemorando() == PrestadorServico::PRESTADORA_SERVICO_POLITEC ||
        $memorando->getPrestadorServicoMemorando() == PrestadorServico::PRESTADORA_SERVICO_MBA ) {
            
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

        if ($listaOsSelecionadas!=null) {
            $count = 1; 
            $contaLinha = 0;
            
            foreach ($listaOsSelecionadas as $os){
                //Calculando a glosa; 
                ++$contaLinha;
                $pfComEsforco       = 0;
                $pfComEsforcoGLosa  = 0;
                
                if($os->possuiGlosa()){
                    $glosa = new Glosa();
                    $glosa = $glosa->recupereGlosaPeloId($os->getIdGlosa());
                    $valorGlosa = $glosa->getValorEmPf();
                    $valorTotalAReceber = $os->getValorAReceberGlosado();
                } else {
                    $valorGlosa = 0;
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
            }
        } else {

            $htmlGerado.= '<tr>';
            $htmlGerado.= ' <td colspan="10">Não foram encontrados registros</td>';
		    $htmlGerado.= '</tr>';
        }
        
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
        $listaOsSelecionadas = $ordemServico->recupereOSQueEstaoVinculadasAoMemorandoAUDITORA( $idMemorando );
        
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
 
        if ($listaOsSelecionadas!=null) {
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
 
                }
            } else { 
            
                $htmlGerado.= '<tr>';
                $htmlGerado.= '<td colspan="5">N&atilde;o foram encontrados registros</td>';
                $htmlGerado.= '</tr>';
                
            }
            
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
    $sqlInsert.= " VALUES ('". addslashes( $htmlGerado ). "', ". $idMemorando .", current_timestamp, 'A') RETURNING mmgid;";
    
    //die( $sqlInsert );
    
    if( !$db->pegaUm($sqlInsert) ){
        throw new Exception( "Não foi possível gerar memorando." );
    }

    $status     = true;
    $retorno    = $idMemorando;
    
    $db->commit();
    
} catch ( Exception $e ) {
    $db->rollback();
    $status = false;
    $retorno  = $e->getMessage();
}

echo simec_json_encode( array(
    'status'  => $status,
    'retorno' => utf8_encode( $retorno )
        )
);
