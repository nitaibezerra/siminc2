<?php
$where = array();

// Verificando responsabilidades
$unidadeExecutora	= new UnidadeExecutora();
$responsabilidades	= $unidadeExecutora->getResponsabilidadeUnidadesByUsuario();

if( !$db->testa_superuser() ){
	if(is_array( $responsabilidades ) && !empty( $responsabilidades )){
		$cond 		= " uexid IN ('".implode("','", $responsabilidades)."') ";
		$condLista	= " uex.uexid IN ('".implode("','", $responsabilidades)."') ";
	}else{
		$cond = $condLista = " FALSE ";
	}
}else{
	$cond = $condLista = " TRUE ";
}

$where[] = $condLista;

header( 'Content-Type: application/vnd.ms-excel' );
header( 'Expires: 0' );
header( 'Cache-Control: must-revalidate, post-check=0, pre-check=0' );
header( 'Content-Disposition: attachment; filename="iphan_planejamentos_' . date("YmdHi") . '.xls"' );

					$arCabecalho = array("ID Planejamento",
                                         "Unidade Executora",
										 "Tipo de Projeto",
                                         "Situação",
										 "Prioridade",
										 "Número do PI",
										 "Número da nota de crédito",
                                         "Título",
										 "Objetivos",
										 "Justificativa",
										 "Descrição das Atividades",
										 "Vinculação ao Planejamento Estratégico",
										 "Outras Iniciativas",
                                         "Tipo de Instrumento",
                                         "Segmento Cultural",
                                         "Meta PNC",
                                         "Data de Ínicio",
                                         "Data de Término",
                                         "Contribui para alguma Meta PPA?",
                                         "Emenda parlamentar",
                                         "Tipo de autor",
                                         "Partido",
                                         "Autor",
										 "Ações presentes nos APPCs",
										 "Localização da ação",
										 "País",
										 "Unidade Federativa",
										 "Município",
										 "Abrangência",
										 "Programa",
										 "Ação",
										 "Unidade Orçamentária",
										 "Localizador",
										 "Plano Orçamentário",
										 "Produto Secundário - Produto",
										 "Produto Secundário - Unidade Medida",
										 "Produto Secundário - Meta",
										 "Valor Previsto (anual) - Custeio",
										 "Valor Previsto (anual) - Capital",
										 "Valor Previsto (anual) - Total",
                                         "Naturezas",
                                         "Contratações",
										 "Cronograma Físico Janeiro",
										 "Cronograma Físico Fevereiro",
										 "Cronograma Físico Março",
										 "Cronograma Físico Abril",
										 "Cronograma Físico Maio",
										 "Cronograma Físico Junho",
										 "Cronograma Físico Julho",
										 "Cronograma Físico Agosto",
										 "Cronograma Físico Setembro",
										 "Cronograma Físico Outubro",
										 "Cronograma Físico Novembro",
										 "Cronograma Físico Dezembro",
										 "Cronograma Financeiro Capital Janeiro",
										 "Cronograma Financeiro Capital Fevereiro",
										 "Cronograma Financeiro Capital Março",
										 "Cronograma Financeiro Capital Abril",
										 "Cronograma Financeiro Capital Maio",
										 "Cronograma Financeiro Capital Junho",
										 "Cronograma Financeiro Capital Julho",
										 "Cronograma Financeiro Capital Agosto",
										 "Cronograma Financeiro Capital Setembro",
										 "Cronograma Financeiro Capital Outubro",
										 "Cronograma Financeiro Capital Novembro",
										 "Cronograma Financeiro Capital Dezembro",
										 "Cronograma Financeiro Custeio Janeiro",
										 "Cronograma Financeiro Custeio Fevereiro",
										 "Cronograma Financeiro Custeio Março",
										 "Cronograma Financeiro Custeio Abril",
										 "Cronograma Financeiro Custeio Maio",
										 "Cronograma Financeiro Custeio Junho",
										 "Cronograma Financeiro Custeio Julho",
										 "Cronograma Financeiro Custeio Agosto",
										 "Cronograma Financeiro Custeio Setembro",
										 "Cronograma Financeiro Custeio Outubro",
										 "Cronograma Financeiro Custeio Novembro",
										 "Cronograma Financeiro Custeio Dezembro",
										 "Cronograma Orçamentário Capital Janeiro",
										 "Cronograma Orçamentário Capital Fevereiro",
										 "Cronograma Orçamentário Capital Março",
										 "Cronograma Orçamentário Capital Abril",
										 "Cronograma Orçamentário Capital Maio",
										 "Cronograma Orçamentário Capital Junho",
										 "Cronograma Orçamentário Capital Julho",
										 "Cronograma Orçamentário Capital Agosto",
										 "Cronograma Orçamentário Capital Setembro",
										 "Cronograma Orçamentário Capital Outubro",
										 "Cronograma Orçamentário Capital Novembro",
										 "Cronograma Orçamentário Capital Dezembro",
										 "Cronograma Orçamentário Custeio Janeiro",
										 "Cronograma Orçamentário Custeio Fevereiro",
										 "Cronograma Orçamentário Custeio Março",
										 "Cronograma Orçamentário Custeio Abril",
										 "Cronograma Orçamentário Custeio Maio",
										 "Cronograma Orçamentário Custeio Junho",
										 "Cronograma Orçamentário Custeio Julho",
										 "Cronograma Orçamentário Custeio Agosto",
										 "Cronograma Orçamentário Custeio Setembro",
										 "Cronograma Orçamentário Custeio Outubro",
										 "Cronograma Orçamentário Custeio Novembro",
										 "Cronograma Orçamentário Custeio Dezembro"
					);


					$detalhePlanejamento = new DetalhePlanejamento();
					$rs = $detalhePlanejamento->listaByWhereXls($where, false);

                    $arConfig = array("style" 			=> "width:100%;",
									  "totalLinha" 		=> true,
									  "totalRegistro" 	=> true);

					//Definindo todas as colunas de texto como string
					$arParamCol[0] = array("type" => Lista::TYPESTRING);
					$arParamCol[1] = array("type" => Lista::TYPESTRING);
					$arParamCol[2] = array("type" => Lista::TYPESTRING);
					$arParamCol[3] = array("type" => Lista::TYPESTRING);
					$arParamCol[4] = array("type" => Lista::TYPESTRING);
					$arParamCol[5] = array("type" => Lista::TYPESTRING);
					$arParamCol[6] = array("type" => Lista::TYPESTRING);
					$arParamCol[7] = array("type" => Lista::TYPESTRING);
					$arParamCol[8] = array("type" => Lista::TYPESTRING);
					$arParamCol[9] = array("type" => Lista::TYPESTRING);

					$oLista = new Lista($arConfig);
					$oLista->setCabecalho( $arCabecalho );
					$oLista->setCorpo( $rs, $arParamCol );
					$oLista->show();
    die();