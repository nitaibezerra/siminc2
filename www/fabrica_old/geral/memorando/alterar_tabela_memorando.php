<?php

include_once "config.inc";
include_once APPRAIZ . 'includes/classes_simec.inc';
include_once APPRAIZ . 'includes/classes/Modelo.class.inc';
include_once APPRAIZ . 'fabrica/classes/autoload.inc';
include_once APPRAIZ . 'www/fabrica/_constantes.php';

$ordemServicoRepositorio = new OrdemServico();
$idPrestadoraServico     = $_POST['empresaContratada'];
$formMemoTpDpsId		 = $_POST['formmemotpdpsid'];
$anomemorando            = $_POST['anomemorando'];
$memoId		 			 = $_POST['memoid'];

if ( $idPrestadoraServico == PrestadorServico::PRESTADORA_SERVICO_FABRICA ||
	 $idPrestadoraServico == PrestadorServico::PRESTADORA_SERVICO_POLITEC )
{
    //$listaDeOrdensDeServico = $ordemServicoRepositorio->recupereTodasOsCandidatasParaMemorandoDaFABRICA( $formMemoTpDpsId, $memoId );
    $listaDeOrdensDeServico = $ordemServicoRepositorio->recupereTodasOsCandidatasParaMemorandoDaFABRICAMenosQueEstaoEmMemorando( $memoId, $formMemoTpDpsId, $idPrestadoraServico, $anomemorando );
	
    $tabela                 = "
		<table class=\"listagem\" cellspacing=\"0\" cellpadding=\"2\" border=\"0\" align=\"center\" width=\"95%\">
			<thead>
				<tr>
					<th><!--<input type=\"checkbox\" name=\"checkall\" id=\"checkall\" value=\"\" />--></th>
    				<th>Data de Abertura da SS</th>
    				<th>Solicitacao de Serviço</th>
					<th>Ordem de Serviço</th>
					<th>Quantidade de Pontos de Função</th>
					<th>Porcentagens das disciplinas</th>
					<th>PF a pagar com % de esforço</th>
					<th>Glosa(PF)</th>
					<th>PF a pagar após cálculo da glosa</th>
					<th>Valor de Ponto de Função Unitário</th>
					<th width='8%'>Valor Total</th>
				</tr>
			</thead>";
    if ( $listaDeOrdensDeServico == null )
    {
        $tabela .= "
			<tbody>
				<tr>
					<td colspan=\"9\" class=\"alignCenter\">Não foram encontrados registros</td>
				</tr>
			</tbody>
		";
    } else
    {
        $tabela .= "
			<tbody>
		";
        $count = 1;
        $contaLinha = 0;
        foreach ( $listaDeOrdensDeServico as $os )
        {
            ++$contaLinha;
            if ( $count % 2 )
            {
                $rowColor = 'class="even"';
            } else
            {
                $rowColor = 'class="odd"';
            }
            
            $pfComEsforco           = 0;
            $pfComEsforcoGlosa      = 0;
            
            if ( $os->possuiGlosa() )
            {
                $glosa              = new Glosa();
                $glosa              = $glosa->recupereGlosaPeloId( $os->getIdGlosa() );
                //$valorGlosa         = $glosa->getValorEmPfComMascara();
                $valorGlosa         = $glosa->getValorEmPf();
                $valorTotalAReceber = $os->getValorAReceberGlosado();
            } else
            {
                $valorGlosa         = 0;
                $valorTotalAReceber = $os->getValorAReceberDaOs();
            }
            
            //die( $valorTotalAReceber);
            
            //PF a pagar com % de esforço
            $valorApagarComPorcentagemDeDisciplina = $os->getValorAReceberDaOs();
            
            $idOS                  = $os->getId();
            $idSS                  = $os->getIdSolicitacaoServico();

            $qtdePFDetalhada            = $os->getMenorValorPF();
            $qtdePFDetalhadaFormatada   = number_format( $os->getMenorValorPF(), 2, ",", "." );
            
            $porcentagemDisciplina = number_format( $os->getPorcentagemDisciplina(), 2, ",", "." );
            $valorUnitarioPF       = number_format( $os->getValorUnitarioDePf(), 2, ",", "." );

            $count++;
            
            $pfComEsforco       = ($os->getPorcentagemDisciplina() * $qtdePFDetalhada) / 100;
            $pfComEsforcoGlosa  = $pfComEsforco - $valorGlosa;
            $tabela .= "
				<tr $rowColor>
					<td><input type=\"checkbox\" class=\"selecionadas\" name=\"osSelecionadas[]\"
						tagpersonalida=\"". $os->temTermo( $os->getIdSolicitacaoServico() ) ."\"
						". fnChecar( $os->getIdMemorando() ) ." value=\"$idOS\"/></td>
					<td class=\"alignCenter\">".$os->getDataAbertura()."</td>
					<td class=\"alignCenter\">$idSS</td>
					<td class=\"alignCenter\">$idOS</td>
					<td id=\"{$idOS}memorandoQtdeValorPF\" class=\"alignRight\">$qtdePFDetalhadaFormatada</td>
					
					<td class=\"alignRight\">$porcentagemDisciplina%</td>
					<td class=\"alignRight\">". number_format($pfComEsforco, 2, ",", "." ) ."</td>
					<td id=\"{$idOS}memorandoQtdePFGlosa\" class=\"alignCenter\">". number_format( $valorGlosa, 2, ",", ".") ."</td>
					<td id=\"{$idOS}memorandoQtdePFGlosaApos\" class=\"alignRight\">". number_format($pfComEsforcoGlosa, 2, ",", "." ) ."</td>
					<!--<td id=\"{$idOS}memorandoTotalComPorcentagemDeDisciplina\" class=\"alignRight\">R$ ". number_format($valorApagarComPorcentagemDeDisciplina, 2, ",", "." ) ." </td> -->
					<td class=\"alignRight\">R$ " . number_format( $os->getValorUnitarioDePf(), 2, ",", "." ) ."</td>
					<td id=\"{$idOS}memorandoTotalAReceber\" class=\"alignRight\">R$ ". number_format($valorTotalAReceber, 2, ",", "." ) ."</td>
				</tr>
			";

            $subTotalQtdePontoFuncao 			= $subTotalQtdePontoFuncao + $qtdePFDetalhada;
            $subTotalQtdeGlosa       			= $subTotalQtdeGlosa + $valorGlosa;
            $subTotalAReceber        			= $subTotalAReceber + $valorTotalAReceber;
            $subTotalComPorcentagemDeDisciplina = $subTotalComPorcentagemDeDisciplina + $valorApagarComPorcentagemDeDisciplina;
            $subTotalpfComEsforcoGlosa          = $subTotalpfComEsforcoGlosa + $pfComEsforcoGlosa;
        }
        $tabela .= "
			</tbody>
		";

        $subTotalQtdePontoFuncao    = number_format( $subTotalQtdePontoFuncao, 2, ",", "." );
        $subTotalQtdeGlosa          = number_format( $subTotalQtdeGlosa, 2, ",", "." );
        $subTotalpfComEsforcoGlosa  = number_format( $subTotalpfComEsforcoGlosa, 2, ",", "." );

        $tabela .= "<tfoot>
                        <tr class=\"glosa-memorando\">
                            <td></td>
                            <td></td>
                            <td></td>
                            <td></td>
                            <td></td>
                            <td></td>
                            <td></td>
                            <td></td>
                            <td></td>
                            <td></td>
                            <td> R$ <span style=\"color: red\" id=\"valor_glosa_memorando\"></span> </td>
                        </tr>
						<tr>
							<td class=\"alignRight\" colspan=\"4\">Total:</td>
							<td id=\"memorandoSubTotalPF\" class=\"alignRight\">$subTotalQtdePontoFuncao</td>
							<td></td>
							<td></td>
							<td id=\"memorandoSubTotalGlosa\" class=\"alignCenter\">$subTotalQtdeGlosa</td>
							<td id=\"memorandoSubTotalGlosaApos\" class=\"alignRight\">$subTotalpfComEsforcoGlosa</td>
							<td id=\"memorandoSubTotalComPorcentagemDeDisciplina\" class=\"alignRight\">
							                            <!--
                                                            R$ 
                                                            <span id=\"valorTotalComPorcentagemDeDisciplina\">". number_format( $subTotalComPorcentagemDeDisciplina, 2, ",", "." ) ."</span>
                                                        </td>
                                                        -->
							<td id=\"memorandoSubtotalAReceber\" class=\"alignRight\">
                                                            R$ 
                                                            <span id=\"valor_total_memorando\">". number_format( $subTotalAReceber, 2, ",", "." )  ."</span>
                                                        </td>
						</tr>
                        <tr>
                            <td colspan='11'>Total de Registros: ".$contaLinha."</td>
                        </tr>
					</tfoot>";
    }


    $tabela .= "
		</table>
	";
    if ( $idPrestadoraServico == PrestadorServico::PRESTADORA_SERVICO_FABRICA ){
    	$textoMemorando = Memorando::TEXTO_PADRAO_MEMORANDO_SQUADRA;
    }
	if ( $idPrestadoraServico == PrestadorServico::PRESTADORA_SERVICO_POLITEC ){
    	$textoMemorando = Memorando::TEXTO_PADRAO_MEMORANDO_POLITEC;
    }
	 
	
    
} else
{
    //$listaDeOrdensDeServico = $ordemServicoRepositorio->recupereTodasOsCandidatasParaMemorandoDaAUDITORA( $formMemoTpDpsId, $memoId );
    $listaDeOrdensDeServico = $ordemServicoRepositorio->recupereTodasOsCandidatasParaMemorandoDaAUDITORAMenosQueEstaoEmMemorando( $memoId, $formMemoTpDpsId, $idPrestadoraServico );
    $tabela                 = "
		<table class=\"listagem\" cellspacing=\"0\" cellpadding=\"2\" border=\"0\" align=\"center\" width=\"95%\">
			<thead>
				<tr>
					<th><!--<input type=\"checkbox\" name=\"checkall\" id=\"checkall\" value=\"\" />--></th>
					<th>Solicitacao de Serviço</th>
					<th>Ordem de Serviço</th>
					<th>Quantidade de Pontos de Função</th>
					<th>Glosa (PF)</th>
					<th>PF a pagar após cálculo da glosa</th>
					<th>Valor de Ponto de Função Unitário</th>
					<th width='8%'>Valor Total</th>
				</tr>
			</thead>";
    if ( empty($listaDeOrdensDeServico)  )
    {
        $tabela .= "
			<tbody>
				<tr>
					<td colspan=\"8\" class=\"alignCenter\">Não foram encontrados registros</td>
				</tr>
			</tbody>
		";
    } else
    {
        $tabela .= "<tbody>";
        $count  = 1;
        $contaLinha = 0;
        
        foreach ( $listaDeOrdensDeServico as $os )
        {
            ++$contaLinha;
            if ( $count % 2 )
            {
                $rowColor = 'class="even"';
            } else
            {
                $rowColor = 'class="odd"';
            }
            
            $valorGlosa         = 0;
            $qtdPFGlosa         = 0;
            $glosa              = '';
            $pfEsforcoGlosa     = 0;
            $menorValorPF       = $os->getMenorValorPFEmpresaItem2( $os->getId());
            $valorUnitarioPF    = $os->getValorUnitarioDePf();

            //$valorUnitarioPF    = $os->getValorUnitarioDePf();
            $valorTotalAReceber = $valorUnitarioPF * $menorValorPF;
            
            if ( $os->possuiGlosa() )
            {
                $glosa              = new Glosa();
                $glosa              = $glosa->recupereGlosaPeloId( $os->getIdGlosa() );
                $qtdPFGlosa         = $glosa->getValorEmPf();
                $valorGlosa         = $qtdPFGlosa * $valorUnitarioPF;
            } 
            
            $valorTotalAReceber     -= $valorGlosa;
            $pfEsforcoGlosa         = $menorValorPF - $qtdPFGlosa;
            
            $idOS                  = $os->getId();
            $idSS                  = $os->getIdSolicitacaoServico();
            $qtdPF                 = number_format( $menorValorPF, 2, ",", "." );
            //$valorUnitarioPF       = number_format( $valorUnitarioPF, 2, ",", "." );
            
            $count++;
            $tabela .= "
				<tr $rowColor>
					<td><input type=\"checkbox\" class=\"selecionadas\" name=\"osSelecionadas[]\"
						tagpersonalida=\"". $os->temTermo( $os->getIdSolicitacaoServico() ) ."\"
						". fnChecar( $os->getIdMemorando() ) ."	value=\"$idOS\"/></td>
					<td class=\"alignCenter\">$idSS</td>
					<td class=\"alignCenter\">$idOS</td>
					<td id=\"{$idOS}memorandoQtdeValorPF\" class=\"alignRight\">". number_format( $qtdPF, 2, ",", "." ) ."</td>
					<td class=\"alignCenter\">". number_format( $qtdPFGlosa, 2, ",", "." ) ."</td>
					<td id=\"{$idOS}memorandoQtdePFGlosaApos\" class=\"alignRight\">". number_format( $pfEsforcoGlosa, 2, ",", "." ) ."</td>
					<td class=\"alignRight\">R$ ". number_format( $valorUnitarioPF, 2, ',', '.' ) ."</td>
					<td id=\"{$idOS}memorandoTotalAReceber\" class=\"alignRight\">R$ ". number_format( $valorTotalAReceber, 2, ",", "." ) ."</td>
				</tr>
			";

            $subTotalQtdePontoFuncao    = $subTotalQtdePontoFuncao + $qtdPF;
            $subTotalAReceber           += $valorTotalAReceber;
            $subTotalpfComEsforcoGlosa  = $subTotalpfComEsforcoGlosa + $pfComEsforcoGlosa;
        }
        $tabela .= "
			</tbody>
		";

        $subTotalQtdePontoFuncao = number_format( $subTotalQtdePontoFuncao, 2, ",", "." );
        $subTotalpfComEsforcoGlosa  = number_format( $subTotalpfComEsforcoGlosa, 2, ",", "." );

        $tabela .= "<tfoot>
                     <tr class=\"glosa-memorando\">
                            <td></td>
                            <td></td>
                            <td></td>
                            <td></td>
                            <td></td>
                            <td></td>
                            <td></td>
                            <td style=\"color: red\" class=\"alignRight\" > R$ <span id=\"valor_glosa_memorando\"></span> </td>
                        </tr>
        
						<tr>
							<td class=\"alignRight\" colspan=\"3\">Total:</td>
							<td id=\"memorandoSubTotalPF\" class=\"alignRight\">$subTotalQtdePontoFuncao</td>
							<td></td>
							<td></td>
							<td id=\"memorandoSubTotalGlosaApos\" class=\"alignRight\">$subTotalpfComEsforcoGlosa</td>
							<td id=\"memorandoSubtotalAReceber\" class=\"alignRight\">
							                            <!--
                                                            R$ 
                                                            <span id=\"valor_total_memorando\">". number_format( $subTotalAReceber, 2, ",", "." ) ."</span>
                                                        </td>
                                                        -->
						</tr>
                        <tr>
                            <td colspan='8'>Total de Registros: ".$contaLinha."</td>
                        </tr>
					</tfoot>";
    }


    $tabela .= "
		</table>
	";


    if ( $idPrestadoraServico == PrestadorServico::PRESTADORA_SERVICO_SAA ){
    	$textoMemorando = Memorando::TEXTO_PADRAO_MEMORANDO_SAA;
    }else{
    	$textoMemorando = Memorando::TEXTO_PADRAO_MEMORANDO_EFICACIA;
    }
    
}

$dados = array(
    "textoMemorando" => utf8_encode( $textoMemorando ),
    "tabela"         => utf8_encode( $tabela ),
    "totalmemo"      => $subTotalAReceber 
);

print simec_json_encode( $dados );

function fnChecar( $parametro )
{
	return ( !empty($parametro) ? "checked=\'checked\'" : "");
}