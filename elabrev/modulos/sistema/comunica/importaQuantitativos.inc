<?php
ini_set("memory_limit", "2048M");
set_time_limit(30000);

function montaTabelaRetorno( $arrResponse = array(), $funcao = '' ){
	global $db;
	$html = '<table align="center" class="tabela" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">
				<tr>
					<th width="10%">Código</th>
                                        <th width="20%">Log</th>
					<th width="20%">Descrição</th>
					<th width="50%">Mensagem Retorno</th>
				</tr>';
	foreach ($arrResponse as $key => $retorno) {

            
            
            
		$sql = "SELECT prgcod || '.'
                                 || acacod || '.'
                                 || unicod || '.'
                                 || loccod AS codigo,
                               acadsc AS descricao,
                               unicod
			  FROM elabrev.ppaacao_orcamento 
			  WHERE prgano = '{$_SESSION['exercicio']}'
                            AND prgcod IS NOT NULL
                            AND acaid = {$retorno['acaid']}
                          GROUP BY prgcod, acacod, unicod, loccod, acadsc
                          ORDER BY prgcod || '.'
                                     || acacod || '.'
                                     || unicod || '.'
                                     || loccod || ' - ' || acadsc";
		$arrAcao = $db->pegaLinha( $sql );

		if(!$retorno['retorno']->return->sucesso){
			if( is_array($retorno['retorno']->return->mensagensErro) ){
				$strMensagem = implode('<br>', $retorno['retorno']->return->mensagensErro);
			} else {
				$strMensagem = $retorno['retorno']->return->mensagensErro;
			}
			$mensagem = '<b>Detalhe do erro:</b> <br><br>'.$strMensagem;
			$corTD = 'red';
                        if ($arrAcao['unicod'] == '26101'
                                || $arrAcao['unicod'] == '26290'
                                || $arrAcao['unicod'] == '26291'
                                || $arrAcao['unicod'] == '26298') {
                            $arrAcao['codigo'] = <<<HTML
<a href="{$_SESSION['sisdiretorio']}.php?modulo=principal/propostaorcamentaria/despesadetalhamentosub&acao=A&acaid={$retorno['acaid']}"
   target="_blank">
    {$arrAcao['codigo']}
</a>
HTML;
                        } else {
                            $arrAcao['codigo'] = <<<HTML
<a href="{$_SESSION['sisdiretorio']}.php?modulo=principal/propostaorcamentaria/despesadetalhamento&acao=A&acaid={$retorno['acaid']}"
   target="_blank">
    {$arrAcao['codigo']}
</a>
HTML;
                        }
                        // -- Log de informações dos dados enviados
                        $arrAcao['log'] = <<<LOG
<b>Valor Físico</b>: {$retorno['requisicao']->proposta->quantidadeFisico}<br />
<b>Quantidade Físico</b>: {$retorno['requisicao']->proposta->valorFisico}<br />
<b>Limite Adicional</b>: {$retorno['requisicao']->proposta->expansaoFisicaSolicitada}<br />
<b>Justificativa</b>: {$retorno['requisicao']->proposta->justificativa}<br />
<b>Justificativa Limite Adicional</b>: {$retorno['requisicao']->proposta->justificativaExpansaoSolicitada}<br />
----- FINANCEIROS -----<br />
LOG;
                        foreach ($retorno['requisicao']->proposta->financeiros as $key => $financeiro) {
                            $arrAcao['log'] .= <<<LOG
<b>ID PO</b>: {$financeiro->identificadorPlanoOrcamentario}<br />
<b>Valor</b>: {$financeiro->valor}<br />
<b>Limite Adicional</b>: {$financeiro->expansaoSolicitada}<br /><br />
LOG;
                        }
		} else {
			if( $funcao == 'cadastra' ){
				$mensagem = 'Proposta cadastrada com sucesso!';
				$sql = "UPDATE elabrev.ppaacao_orcamento SET acapropostaenviada = true, acadtenviosiop = now() WHERE acaid = {$retorno['acaid']}";
				$db->executar( $sql );
				$db->commit();
			} else if( $funcao == 'consulta' ){
				$mensagem = 'Consulta realizada com sucesso!';
			} else {
				$mensagem = 'Proposta excluida com sucesso!';
				$sql = "UPDATE elabrev.ppaacao_orcamento SET acapropostaenviada = false, acadtenviosiop = now() WHERE acaid = {$retorno['acaid']}";
				$db->executar( $sql );
				$db->commit();
			}
			$corTD = 'blue';
                        $arrAcao['log'] = '&nbsp;';
		}
		$key % 2 ? $cor = "#dedfde" : $cor = "";
		$html.= '<tr bgcolor="'.$cor.'" id="tr_'.$key.'" onmouseout="this.bgColor=\''.$cor.'\';" onmouseover="this.bgColor=\'#ffffcc\';">
					<td>'.$arrAcao['codigo'].'</td>
                                        <td style="overflow:auto">' . $arrAcao['log'] . '</td>
					<td>'.$arrAcao['descricao'].'</td>
					<td style="color: '.$corTD.';">'.$mensagem.'</td>
				</tr>';
	}
	$html.= '</table>';
	return $html;
}

function montaTabelaRetornoAcompanhamento( $arrResponse = array(), $funcao = '' ){
	global $db;
	
	//if( $funcao != 'consultarAcompanhamento' ){
		$html = '<table align="center" class="tabela" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">
					<tr>
						<th width="10%">Programática</th>
						<th width="10%">Detalhes</th>
						<th width="30%">Descrição</th>
						<th width="50%">Mensagem Retorno</th>
					</tr>';
	/*} else {
		$html = '<table align="center" class="tabela" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">';
	}*/
	
	$sql = "select COALESCE(max(l.loglote), 0) from elabrev.logerrows l 
				where l.logtipo = 'cadastrarAcompanhamentoOrcamentario'";
	$lote = $db->pegaUm($sql);				
	$lote = $lote + 1;
	
	if( $_SESSION['elabrev']['logidarr'][0] ){
		$sql = "UPDATE elabrev.logerrows SET
	  				loglote = $lote
	  			WHERE
	  				logid in (".implode(', ', $_SESSION['elabrev']['logidarr']).")";
	  	$db->executar( $sql );
	  	$db->commit();
	}
//	ver($arrResponse, d);
	foreach ($arrResponse as $key => $retorno) {

            $sql = <<<DML
SELECT a.codigo AS codigo,
       a.codigo || ' - ' || dae.titulo AS descricao,
       org.codigo AS unicod
  FROM planacomorc.acao a
    INNER JOIN planacomorc.acao_programatica apr ON apr.id_acao = a.id_acao
    INNER JOIN planacomorc.orgao org USING(id_orgao)
    INNER JOIN planacomorc.dados_acao_exercicio dae ON dae.id_acao = a.id_acao
      AND dae.id_exercicio = apr.id_exercicio
    LEFT JOIN planacomorc.programa prg USING(id_programa)
    LEFT JOIN planacomorc.esfera esf USING(id_esfera)
    LEFT JOIN planacomorc.subfuncao sfu USING(id_subfuncao)
  WHERE a.codigo = '{$retorno['acacod']}'
    AND org.codigo = '{$retorno['unicod']}'
DML;
/*		$sql = "SELECT 
						   	a.acacod as codigo, 
						   	a.acacod || ' - ' || a.acatitulo as descricao,
						   	a.unicod
						FROM 
							monitora.programa p 
							inner join monitora.acao a ON a.prgid = p.prgid
		                    inner join monitora.avaliacaoparecer avp on avp.acaid = a.acaid
		                    inner join workflow.documento doc on doc.docid = avp.docid
		                    inner join workflow.estadodocumento esd on esd.esdid = doc.esdid 
						WHERE 
		                	p.prgano = '{$_SESSION['exercicio']}'
		                    and acasnrap = false 
		                    and a.acacod = '{$retorno['acacod']}'
		                    and a.unicod = '{$retorno['unicod']}'
		                	and trim(prodsc) != '' 
		                    and p.prgcod not in ('1073','1449')
		                    and esd.esdid in (620, 648)
						GROUP BY a.acacod, a.acatitulo, a.unicod  
						ORDER BY a.acacod, a.acatitulo";*/
		$arrAcao = $db->pegaLinha( $sql );
		
		$mensagem = '';	
		if(!$retorno['retorno']->return->sucesso){
			
			if( is_array($retorno['retorno']->return->mensagensErro) ){
				$strMensagem = implode('<br>', $retorno['retorno']->return->mensagensErro);
			} else {
				$strMensagem = $retorno['retorno']->return->mensagensErro;
			}
			$mensagem = '<b>Detalhe do erro:</b> <br><br>'.$strMensagem;
			$corTD = 'red';
		} else {
			
			if ($funcao == 'cadastrarAcompanhamento') {
                            $strMensagem = $mensagem = '';
                            if (is_array($retorno['retorno']->return->alertas)){
                                    $strMensagem .= implode('<br>', $retorno['retorno']->return->alertas);
                                    $mensagem .= '<b>Acompanhamento cadastrado com sucesso com ALERTA:</b> <br><br>'.$strMensagem;
                            } elseif (is_object($retorno['retorno']->return->alertas) || is_object($retorno['retorno']->return->pendencias)) {
                                if (is_array($retorno['retorno']->return->alertas->alerta)) {
                                    $strMensagem .= implode('<br />', $retorno['retorno']->return->alertas->alerta);
                                } elseif ($retorno['retorno']->return->alertas->alerta) {
                                    $strMensagem .= $retorno['retorno']->return->alertas->alerta;
                                }
                                if ($strMensagem) {
                                    $mensagem .= '<b>Acompanhamento cadastrado com sucesso com ALERTA:</b> <br><br>'.$strMensagem . '<br /><br/>';
                                    $strMensagem = '';
                                }
                                if (is_array($retorno['retorno']->return->pendencias->pendencia)) {
                                    $strMensagem .= implode('<br />', $retorno['retorno']->return->pendencias->pendencia);
                                } elseif($retorno['retorno']->return->pendencias->pendencia) {
                                    $strMensagem .= $retorno['retorno']->return->pendencias->pendencia;
                                }
                                if ($strMensagem) {
                                    $mensagem .= '<b>Acompanhamento cadastrado com sucesso com PENDÊNCIAS:</b> <br><br>'.$strMensagem;
                                }
                            } else {
                                    $strMensagem .= $retorno['retorno']->return->mensagensErro;
                                    if( $strMensagem ){
                                            $mensagem .= '<b>Acompanhamento cadastrado com sucesso com ALERTA:</b> <br><br>'.$strMensagem;
                                    } else {
                                            $mensagem .= 'Acompanhamento cadastrado com sucesso!<br>';
                                    }
                            }
                            include_once APPRAIZ . "includes/workflow.php";

                            /*$sql = "SELECT DISTINCT avp.docid
                                      FROM monitora.acao a
                                        INNER JOIN monitora.avaliacaoparecer avp ON avp.acaid = a.acaid
                                        INNER JOIN workflow.documento doc on doc.docid = avp.docid
                                        INNER JOIN workflow.estadodocumento esd on esd.esdid = doc.esdid
                                      WHERE a.acacod = '{$retorno['acacod']}' 
                                        AND a.prgano = '{$_SESSION['exercicio']}' 
                                        AND a.acasnrap = false
                                        AND esd.esdid = 620
                                        AND (a.prodsc <> '' OR a.prodsc IS NOT NULL)";*/
                            $sql = <<<DML
SELECT acc.docid
  FROM planacomorc.acompanhamento_acao acc
    INNER JOIN planacomorc.acao_programatica apr USING(id_acao_programatica)
    INNER JOIN planacomorc.localizador_programatica lpr USING(id_acao_programatica, id_localizador)
    INNER JOIN planacomorc.quantitativo_sof qsf USING(id_acao_programatica, loccod)
    INNER JOIN workflow.documento wfd USING(docid)
  WHERE id_acao_programatica = {$retorno['acaid']}
    AND esdid = 753
    AND qsf.quantidade_fisico != 0
DML;
//  ver($sql, d);
                            foreach ($db->carregar($sql) as $programatica) {
                                $aedid = 1767;
                                $dados = array();
                                $result = wf_alterarEstado($programatica['docid'], $aedid, $cmddsc = 'Tramitação feita em lote - SIOP', $dados);
                            }
//                            $arrDocid = $db->carregarColuna( $sql );
//                            $arrDocid = $arrDocid ? $arrDocid : array();

//                            foreach ($arrDocid as $docid) {
//                                    $aedid = 1629; #Enviar para SIOP
//                                    $dados = array();
//
//                                    $result = wf_alterarEstado( $docid, $aedid, $cmddsc = 'Tramitação feita em lote - SIOP', $dados);
//                            }
			} elseif( $funcao == 'consultarAcompanhamento' ){
				$mensagem = 'Consulta Realizada com sucesso!';
				$arrRetornoConsulta = $retorno['retorno']->return->acompanhamentosAcoes->acompanhamentoAcao;
				//ver($arrRetornoConsulta,d);
				$periodoOrdem 		= $arrRetornoConsulta->periodoOrdem;
				$exercicio			= $arrRetornoConsulta->exercicio;
				$codigoMomento		= $arrRetornoConsulta->codigoMomento;
				$esfcod 			= $arrRetornoConsulta->esfera;
			    $unicod 			= $arrRetornoConsulta->unidadeOrcamentaria;
			    $funcod 			= $arrRetornoConsulta->funcao;
			    $sfucod 			= $arrRetornoConsulta->subFuncao;
			    $prgcod 			= $arrRetornoConsulta->programa;
			    $acacod				= $arrRetornoConsulta->acao;
			    $acatipoinclusao 	= $arrRetornoConsulta->codigoTipoInclusaoAcao;
			    $snPendencia 		= $arrRetornoConsulta->snPendencia;
			    $dataHoraAlteracao 	= $arrRetornoConsulta->dataHoraAlteracao;
			    ver($acacod);
			    if( $acacod ) $acacod = $db->pegaUm("select DISTINCT a.acacod || ' - ' || a.acatitulo as descricao from monitora.acao a where a.acacod = '$acacod'");
			    if( $unicod ) $unicod = $db->pegaUm("select DISTINCT u.unicod || ' - ' || u.unidsc as descricao from public.unidade u where u.unicod = '$unicod'");
			    if( $prgcod ) $prgcod = $db->pegaUm("select DISTINCT p.prgcod || ' - ' || p.prgdsc as descricao from monitora.programa p where p.prgcod = '$prgcod'");
			    //if( $prgcod ) $db->pegaUm("select DISTINCT l.loccod || ' - ' || l.locdsc as descricao from public.localizador l where l.loccod = '20RK'");
			    ver($acacod,d);
			    
			    $arrLocalizadores 	= $arrRetornoConsulta->acompanhamentosLocalizadores->acompanhamentoLocalizador;
			    
			    $key % 2 ? $cor = "#dedfde" : $cor = "";
				$html.= '<tr>
							<td valign="top">
								<table align="center" class="tabela" style="width: 100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">
									<tr>
										<td class="subtitulodireita" style="width: 15%"><b>periodoOrdem</b></td><td style="width: 35%">'.$periodoOrdem.'</td>
										<td class="subtitulodireita" style="width: 15%"><b>subFuncao</b></td><td style="width: 35%">'.$sfucod.'</td>
									</tr>
									<tr>
										<td class="subtitulodireita"><b>exercicio</b></td><td>'.$exercicio.'</td>
										<td class="subtitulodireita"><b>programa</b></td><td>'.$prgcod.'</td>
									</tr>
									<tr>
										<td class="subtitulodireita"><b>codigoMomento</b></td><td>'.$codigoMomento.'</td>
										<td class="subtitulodireita"><b>acao</b></td><td>'.$acacod.'</td>
									</tr>
									<tr>
										<td class="subtitulodireita"><b>esfera</b></td><td>'.$esfcod.'</td>
										<td class="subtitulodireita"><b>codigoTipoInclusaoAcao</b></td><td>'.$acatipoinclusao.'</td>
									</tr>
									<tr>
										<td class="subtitulodireita"><b>unidadeOrcamentaria</b></td><td>'.$unicod.'</td>
										<td class="subtitulodireita"><b>snPendencia</b></td><td>'.$snPendencia.'</td>
									</tr>
									<tr>
										<td class="subtitulodireita"><b>funcao</b></td><td>'.$funcod.'</td>
										<td class="subtitulodireita"><b>dataHoraAlteracao</b></td><td>'.$dataHoraAlteracao.'</td>
									</tr>
								</table>
							</td></tr>
							<tr><td colspan="2">
								<table align="center" class="listagem" width="100%" bgcolor="#f5f5f5" cellspacing="0" cellpadding="2" align="center">
									<tr>
										<th>localizador</th>
										<th>TipoLocalizador</th>
										<th>meta</th>
										<th>reprogramado</th>
										<th>realizadoLOA</th>
										<th>dataApuracaoLOA</th>
										<th>dotacaoAtual</th>
										<th>limite</th>
										<th>empenhado</th>
										<th>liquidado</th>
										<th>realizadoRAP</th>
										<th>dataApuracaoRAP</th>
										<th>rapInscritoLiquido</th>
										<th>rapLiquidadoAPagar</th>
										<th>rapPago</th>
										<th>justificativa</th>
									</tr>';
				
				$count = 0;
				if( is_array($arrLocalizadores) ){
				foreach ($arrLocalizadores as $chave => $localizadores) {
						$chave % 2 ? $cor = "#dedfde" : $cor = "";
						
						$localizador 					= $localizadores->localizador;
						$codigoTipoInclusaoLocalizador 	= $localizadores->codigoTipoInclusaoLocalizador;
						$meta 							= $localizadores->meta;
						$reprogramado 					= $localizadores->reprogramado;
						$realizadoLOA 					= $localizadores->realizadoLOA;
						$dataApuracaoLOA 				= $localizadores->dataApuracaoLOA;
						$dotacaoAtual 					= $localizadores->dotacaoAtual;
						$limite 						= $localizadores->limite;
						$empenhado 						= $localizadores->empenhado;
						$liquidado 						= $localizadores->liquidado;
						$realizadoRAP 					= $localizadores->realizadoRAP;
						$dataApuracaoRAP 				= $localizadores->dataApuracaoRAP;
						$rapInscritoLiquido 			= $localizadores->rapInscritoLiquido;
						$rapLiquidadoAPagar 			= $localizadores->rapLiquidadoAPagar;
						$rapPago 						= $localizadores->rapPago;
						$justificativa 					= substr($localizadores->justificativa, 0, 500);
						
						$html.= "<tr bgcolor=\"".$cor."\" onmouseout=\"this.bgColor='".$cor."';\" onmouseover=\"this.bgColor='#ffffcc';\">
									<td>$localizador</td>
									<td>$codigoTipoInclusaoLocalizador</td>
									<td>$meta</td>
									<td>$reprogramado</td>
									<td>$realizadoLOA</td>
									<td>$dataApuracaoLOA</td>
									<td>$dotacaoAtual</td>
									<td>$limite</td>
									<td>$empenhado</td>
									<td>$liquidado</td>
									<td>$realizadoRAP</td>
									<td>$dataApuracaoRAP</td>
									<td>$rapInscritoLiquido</td>
									<td>$rapLiquidadoAPagar</td>
									<td>$rapPago</td>
									<td>$justificativa</td>
								</tr>";
						$count++;
					}
					$html.= '</table></td></tr>';				
				} else {
					$localizador 					= $arrLocalizadores->localizador;
					$codigoTipoInclusaoLocalizador 	= $arrLocalizadores->codigoTipoInclusaoLocalizador;
					$meta 							= $arrLocalizadores->meta;
					$reprogramado 					= $arrLocalizadores->reprogramado;
					$realizadoLOA 					= $arrLocalizadores->realizadoLOA;
					$dataApuracaoLOA 				= $arrLocalizadores->dataApuracaoLOA;
					$dotacaoAtual 					= $arrLocalizadores->dotacaoAtual;
					$limite 						= $arrLocalizadores->limite;
					$empenhado 						= $arrLocalizadores->empenhado;
					$liquidado 						= $arrLocalizadores->liquidado;
					$realizadoRAP 					= $arrLocalizadores->realizadoRAP;
					$dataApuracaoRAP 				= $arrLocalizadores->dataApuracaoRAP;
					$rapInscritoLiquido 			= $arrLocalizadores->rapInscritoLiquido;
					$rapLiquidadoAPagar 			= $arrLocalizadores->rapLiquidadoAPagar;
					$rapPago 						= $arrLocalizadores->rapPago;
					$justificativa 					= substr($arrLocalizadores->justificativa, 0, 500);
					
					$html.= "<tr>
									<td>$localizador</td>
									<td>$codigoTipoInclusaoLocalizador</td>
									<td>$meta</td>
									<td>$reprogramado</td>
									<td>$realizadoLOA</td>
									<td>$dataApuracaoLOA</td>
									<td>$dotacaoAtual</td>
									<td>$limite</td>
									<td>$empenhado</td>
									<td>$liquidado</td>
									<td>$realizadoRAP</td>
									<td>$dataApuracaoRAP</td>
									<td>$rapInscritoLiquido</td>
									<td>$rapLiquidadoAPagar</td>
									<td>$rapPago</td>
									<td>$justificativa</td>
								</tr>
							</table></td></tr>";
					$count++;
				}
				$html.= '<tr>
								<td colspan="2"><table class="listagem" cellspacing="0" cellpadding="2" border="0" align="center" width="100%">
										<tbody>
										<tr bgcolor="#ffffff">
											<td><b>Total de Registros: '.$count.'</b></td>
										</tr>
										</tbody>
									  </table></td>
							</tr>';
			}
			$corTD = 'blue';
			
		}
		//if( $funcao != 'consultarAcompanhamento' ){
			$key % 2 ? $cor = "#dedfde" : $cor = "";
			$html.= '<tr bgcolor="'.$cor.'" id="tr_'.$key.'" onmouseout="this.bgColor=\''.$cor.'\';" onmouseover="this.bgColor=\'#ffffcc\';">
                                    <td>'.$arrAcao['unicod'] . '.' . $retorno['prgcod'] . '.' . $arrAcao['codigo'] . '</td>
                                    <td>
                                      Função: ' . $retorno['funcao'] . '<br />
                                      Subfunção: ' . $retorno['subfuncao'] . '<br />
                                      Esfera: ' . $retorno['esfera'] . '<br />
                                      Tipo inclusão: '.$retorno['tipoinclusao'].'<br />
                                    </td>
                                    <td>'.$arrAcao['descricao'].'</td>
                                    <td style="color: '.$corTD.';">'.$mensagem.'</td>
                                 </tr>';
		//}
	}
	$html.= '<tr>
                    <td colspan="4"><table class="listagem" cellspacing="0" cellpadding="2" border="0" align="center" width="100%">
                        <tbody>
                            <tr bgcolor="#ffffff">
                                <td><b>Total de Registros: '.sizeof($arrResponse).'</b></td>
                            </tr>
                        </tbody>
                    </table></td>
                <tr></table>';
	return $html;
}

if($_POST['requisicao'] == 'enviasolicitacao') {
	
	//include(APPRAIZ."monitora/classes/ImportacaoSiop.class.inc");
	include_once APPRAIZ."elabrev/classes/WSQuantitativo.class.inc";
	include_once APPRAIZ.'elabrev/classes/SiopQuantitativo.class.inc';

	$arrParam = array('usuario' => $_POST['wsusuario'],
							'senha' => $_POST['wssenha'],
							'post' 	=> $_POST
						);
	
	$obResponse = new SiopQuantitativo($arrParam);
	$documento = $_POST['documento'];
	
	if( $documento == 'cadastrarProposta' ){
		if( $_POST['acaid_cadastra'][0] ){
			$arrResponse = $obResponse->cadastrarProposta();
//			ver($arrResponse, d);

			if( $arrResponse != 'erro' ){
				$html = montaTabelaRetorno( $arrResponse, 'cadastra' );
			} else {
				$html = 'erro';
			}
		}
		
	} else if( $documento == 'consultarProposta' ){
		if( $_POST['acaid_consulta'][0] ){
			$arrResponse = $obResponse->consultarProposta();
			//ver($arrResponse,d);
			
			if( $arrResponse != 'erro' ){
				$html = montaTabelaRetorno( $arrResponse, 'consulta' );
			} else {
				$html = 'erro';
			}
		}
		
	} else if( $documento == 'excluirProposta' ){
		if( $_POST['acaid_exclui'][0] ){
			$arrResponse = $obResponse->excluirProposta();
			//ver($arrResponse);
			
			if( $arrResponse != 'erro' ){
				$html = montaTabelaRetorno( $arrResponse, 'exclui' );
			} else {
				$html = 'erro';
			}
		}
		
	} else if( $documento == 'cadastrarAcompanhamento' ){
		if( $_POST['acaid_cadastraAcompanhamento'][0] ){
			$_SESSION['elabrev']['logidarr'] = array();
			$arrResponse = $obResponse->cadastrarAcompanhamento();
//                        ver($arrResponse, d);
			if( $arrResponse != 'erro' ){
				$html = montaTabelaRetornoAcompanhamento($arrResponse, 'cadastrarAcompanhamento');
			} else {
				$html = 'erro';
			}
		}
		
	} else if( $documento == 'consultarAcompanhamento' ){
		if( $_POST['acaid_concultaAcompanhamento'][0] ){
			$arrResponse = $obResponse->consultarAcompanhamento();
			//ver($arrResponse);
			
			if( $arrResponse != 'erro' ){
				$html = montaTabelaRetornoAcompanhamento( $arrResponse, 'consultarAcompanhamento' );
			} else {
				$html = 'erro';
			}
		}
		
	} else if($documento == 'obterExecucaoOrcamentariaSam') {
		$response = $obResponse->obterExecucaoOrcamentariaSam();
		ver($response,d);
				
	} else if($documento == 'obterProgramacaoCompletaQuantitativo') {
		$response = $obResponse->obterProgramacaoCompletaQuantitativo();
		//ver($response,d); 
		
		if( $response->return->sucesso ){
			if( is_array($response->return->proposta) ){
				$db->executar( "DELETE FROM elabrev.ws_financeirodto where opcid in (select opcid from elabrev.ws_obterprogramacaocompletaquantitativo where exercicio = '{$_SESSION['exercicio']}')" );
				$db->executar( "DELETE FROM elabrev.ws_obterprogramacaocompletaquantitativo where exercicio = '{$_SESSION['exercicio']}'" );
				$totalRegistro = 0;
				foreach ($response->return->proposta as $valor) {
					
					$sql = "INSERT INTO elabrev.ws_obterprogramacaocompletaquantitativo(codigoacao, codigoesfera, codigofuncao, codigolocalizador,
  								codigomomento, codigoorgao, codigoprograma, codigosubfuncao, codigotipodetalhamento, codigotipoinclusaoacao,
  								codigotipoinclusaolocalizador, exercicio, quantidadefisico, snatual, valorfisico) 
							VALUES ('{$valor->codigoAcao}', '{$valor->codigoEsfera}', '{$valor->codigoFuncao}', '{$valor->codigoLocalizador}',
  								'{$valor->codigoMomento}', '{$valor->codigoOrgao}', '{$valor->codigoPrograma}', '{$valor->codigoSubFuncao}', 
  								'{$valor->codigoTipoDetalhamento}', '{$valor->codigoTipoInclusaoAcao}', '{$valor->codigoTipoInclusaoLocalizador}', 
  								'{$valor->exercicio}', '{$valor->quantidadeFisico}', '{$valor->snAtual}', '{$valor->valorFisico}') returning opcid";
  					//ver($sql,d);
					$opcid = $db->pegaUm( $sql );
					if( $opcid ){
						if( is_array($valor->financeiros) ){
							foreach ($valor->financeiros as $v) {
								$sql = "INSERT INTO elabrev.ws_financeirodto(opcid, fonte, idoc, iduso, identificadorplanoorcamentario,
	  										naturezadespesa, resultadoprimarioatual, resultadoprimariolei, valor) 
										VALUES ($opcid, '{$v->fonte}', '{$v->idOC}', '{$v->idUso}', '{$v->identificadorPlanoOrcamentario}',
	  										'{$v->naturezaDespesa}', '{$v->resultadoPrimarioAtual}', '{$v->resultadoPrimarioLei}', '{$v->valor}')";
	  							
								$db->executar( $sql );
							}
						} else {
							$sql = "INSERT INTO elabrev.ws_financeirodto(opcid, fonte, idoc, iduso, identificadorplanoorcamentario,
  										naturezadespesa, resultadoprimarioatual, resultadoprimariolei, valor) 
									VALUES ($opcid, '{$valor->financeiros->fonte}', '{$valor->financeiros->idOC}', '{$valor->financeiros->idUso}', '{$valor->financeiros->identificadorPlanoOrcamentario}',
  										'{$valor->financeiros->naturezaDespesa}', '{$valor->financeiros->resultadoPrimarioAtual}', '{$valor->financeiros->resultadoPrimarioLei}', '{$valor->financeiros->valor}')";
  							
							$db->executar( $sql );
						}
					}
					$db->commit();
					$totalRegistro++;
				}
			}
			$html = 'Dados carregados com sucesso. Total de Registros: '.$totalRegistro;
		}
		//ver($response,d);
		
	} else if($documento == 'obterTabelasApoioQuantitativo') {		
				
		$response = $obResponse->obterTabelasApoioQuantitativo();
		//ver($response,d);
		$boFonte = false;
		$boUso = false;
		$boDoc = false;
		$boNat = false;
		$boResult = false;
		$dataini = date('H:i:s');
		if( $response->return->sucesso ){
			if( is_array($response->return->fonte) ){
				$totalFonte = 0;
				$boFonte = true;
				$db->executar( "DELETE FROM elabrev.ws_fonte" );
				foreach ($response->return->fonte as $v) {
					$sql = "INSERT INTO elabrev.ws_fonte(codigofonte, descricao, exercicio, snativo) 
							VALUES ('{$v->codigoFonte}', '{$v->descricao}', '{$v->exercicio}', '{$v->snAtivo}')";
					$db->executar( $sql );
					$totalFonte++;
				}
			}
			if( is_array($response->return->idUso) ){				
				$db->executar( "DELETE FROM elabrev.ws_iduso" );
				$totalUso = 0;
				$boUso = true;
				foreach ($response->return->idUso as $v) {
					$sql = "INSERT INTO elabrev.ws_iduso(codigoiduso, descricao, exercicio, snativo) 
							VALUES ('{$v->codigoIdUso}', '".str_ireplace("'", "", $v->descricao)."', '{$v->exercicio}', '{$v->snAtivo}')";
					$db->executar( $sql );
					$totalUso++;	
				}
			}
			if( is_array($response->return->idoc) ){
				$db->executar( "DELETE FROM elabrev.ws_idoc" );
				$totalIdoc = 0;
				$boDoc = true;
				foreach ($response->return->idoc as $v) {
					$sql = "INSERT INTO elabrev.ws_idoc(codigoidoc, descricao, exercicio, snativo) 
							VALUES ('{$v->codigoIdOc}', '".str_ireplace("'", "", $v->descricao)."', '{$v->exercicio}', '{$v->snAtivo}')";
					$db->executar( $sql );
					$totalIdoc++;	
				}
			}
			if( is_array($response->return->natureza) ){
				$db->executar( "DELETE FROM elabrev.ws_naturezadespesa" );
				$totalNat = 0;
				$boNat = true;
				foreach ($response->return->natureza as $v) {
					$sql = "INSERT INTO elabrev.ws_naturezadespesa(codigonatureza, descricao, descricaoabreviada, exercicio, snativo) 
							VALUES ('{$v->codigoNatureza}', '{$v->descricao}', '{$v->descricaoAbreviada}', '{$v->exercicio}', '{$v->snAtivo}')";
					$db->executar( $sql );
					$totalNat++;	
				}				
			}
			if( is_array($response->return->resultadoPrimario) ){
				$db->executar( "DELETE FROM elabrev.ws_resultadoprimario" );
				$totalResult = 0;
				$boResult = true;
				foreach ($response->return->resultadoPrimario as $v) {
					$sql = "INSERT INTO elabrev.ws_resultadoprimario(codigoresultadoprimario, descricao, exercicio) 
							VALUES ('{$v->codigoResultadoPrimario}', '{$v->descricao}', '{$v->exercicio}')";
					$db->executar( $sql );
					$totalResult++;	
				}				
			}
			$db->commit();
			$dataFim = date('H:i:s');
			$html = '<table align="center" width="95%" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">
						<tr>
							<th colspan="2">Total de Registro Carregados</th>
						</tr>';
			if( $boFonte ){
				$html .= '<tr>
							<td width="30%" class="SubtituloDireita">Fontes:</td>
							<td>'.$totalFonte.'</td>
						</tr>';
			}
			if($boUso){
				$html .= '<tr>
							<td width="30%" class="SubtituloDireita">IdOcs:</td>
							<td>'.$boUso.'</td>
						</tr>';
			}
			if($boDoc){
				$html .= '<tr>
							<td width="30%" class="SubtituloDireita">IdUsos:</td>
							<td>'.$totalIdoc.'</td>
						</tr>';
			}
			if($boNat){
				$html .= '<tr>
							<td width="30%" class="SubtituloDireita">Natureza Despeza:</td>
							<td>'.$totalNat.'</td>
						</tr>';
			}
			if($boResult){
				$html .= '<tr>
							<td width="30%" class="SubtituloDireita">Resultado Primario:</td>
							<td>'.$totalResult.'</td>
						</tr>';
			}
			$html .= '</table>';
			
			echo $html;
		}
		//ver($response,d);
	} else {
		$operacoes = $wsQuantitativo->__getFunctions();
		$tipos = $wsQuantitativo->__getTypes();
	} 
	//exit();
}

// cabecalho padrão do SIMEC
include APPRAIZ . "includes/cabecalho.inc";
monta_titulo('Comunicação de Dados - SIOP', 'Quantitativos');

$wsusuario = WEB_SERVICE_SIOP_USUARIO;
$wssenha = WEB_SERVICE_SIOP_SENHA;

$largura 	= "280px";
$altura 	= "150px";
$id 		= "div_auth";
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script language="JavaScript" src="../includes/funcoes.js"></script>

<style>	
	.popup_alerta
	{
		width:<?php echo $largura ?>;
		height:<?php echo $altura ?>;
		position:absolute;
		z-index:0;
		top:50%;
		left:50%;
		margin-top:-<?php echo $altura/2 ?>;
		margin-left:-<?php echo $largura/2 ?>;
		border:solid 2px black;
		background-color:#FFFFFF;
		display:none;
		overflow: auto;
	}
</style>
<style>
	label { cursor: pointer; }
</style>
<form method="post" name="formulario" id="formulario">	
	
<table align="center" width="95%" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">
	<tr>
		<td class="subtitulodireita">Funções:</td>
		<td style="padding: 0 20px 20px 20px;">
			<table align="center" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">
				<tr>
					<td>
						<label>
							<input type="radio" name="documento" id="documento_cadastra" value="cadastrarProposta" onclick="mostraCombo('cadastra')"/>
							Cadastrar Proposta:
							<div id="cadastra" style="display: none;">
							<?php
                                $whereAdd = '';
                                if ($_REQUEST['tpdid']) {
                                    $whereAdd = " AND tda.tpdid IN({$_REQUEST['tpdid']})";
                                }
				$sql = "SELECT ppo.acaid as codigo, 
					       ppo.prgcod || '.' || ppo.acacod || '.' || ppo.unicod || '.' || ppo.loccod || ' - ' || ppo.acadsc AS descricao
					  FROM elabrev.ppaacao_orcamento ppo
                        	            INNER JOIN elabrev.despesaacao des ON des.acaid = ppo.acaid
                                            INNER JOIN elabrev.tipodetalhamentoacao tda ON tda.acaid = ppo.acaid
                                          WHERE ppo.prgano = '{$_SESSION['exercicio']}' 
                                            AND ppo.prgcod IS NOT NULL {$whereAdd}
                                            AND ppo.acapropostaenviada = FALSE
                                          GROUP BY ppo.acaid, ppo.prgcod, ppo.acacod, ppo.unicod, ppo.loccod, ppo.acadsc
					  ORDER BY ppo.prgcod, ppo.acacod, ppo.unicod, ppo.loccod, ppo.acadsc";
//                                            ver($sql, d);
				mostrarComboPopup('<b>Proposta</b>', 'acaid_cadastra',  $sql, $stSqlCarregados, 'Selecione a(s) Proposta(s)' );
							?></div>
						</label>
					</td>
				</tr>
				<tr>
					<td>
						<label>
				<input type="radio" name="documento" id="documento_consulta" value="consultarProposta" onclick="mostraCombo('consulta')"/>
				Consultar Proposta:
					<div id="consulta" style="display: none;">
				<?php 
				$sql = "select 
							acaid as codigo, 
							prgcod||'.'||acacod||'.'||unicod||'.'||loccod|| ' - ' || acadsc as descricao
						from elabrev.ppaacao_orcamento 
						where 
							prgano = '{$_SESSION['exercicio']}' 
							and prgcod is not null 
							and acapropostaenviada = true
						group by 
							acaid, 
							prgcod,
							acacod,
							unicod,
							loccod,
							acadsc
						order by 
							prgcod,
							acacod,
							unicod,
							loccod,
							acadsc";

				mostrarComboPopup('<b>Proposta</b>', 'acaid_consulta',  $sql, $stSqlCarregados, 'Selecione a(s) Proposta(s)' );
				?></div>
			</label>
			</td></tr>
			<tr><td>
			<label>
				<input type="radio" name="documento" id="documento_exclui" value="excluirProposta" onclick="mostraCombo('exclui')"/>
				Excluir Proposta:
					<div id="excluir" style="display: none;">
				<?php 
				$sql = "select 
							acaid as codigo, 
							prgcod||'.'||acacod||'.'||unicod||'.'||loccod|| ' - ' || acadsc as descricao
						from elabrev.ppaacao_orcamento 
						where 
							prgano = '{$_SESSION['exercicio']}' 
							and prgcod is not null 
							and acapropostaenviada = true
						group by 
							acaid, 
							prgcod,
							acacod,
							unicod,
							loccod,
							acadsc
						order by 
							prgcod,
							acacod,
							unicod,
							loccod,
							acadsc";

				mostrarComboPopup('<b>Proposta</b>', 'acaid_exclui',  $sql, $stSqlCarregados, 'Selecione a(s) Proposta(s)' );
				?></div>
			</label>
			</td></tr>
<!--			<tr><td>
			<label>
				<input type="radio" name="documento" id="documento_cadastraAcompanhamento" value="cadastrarAcompanhamento" onclick="mostraCombo('cadastraAcompanhamento')"/>
				Cadastrar Acompanhamento Orcamentario:
					<div id="cadastrarAcompanhamento" style="display: none;">
				<?php 
///*				$sql = "SELECT
//						   	a.acacod as codigo,
//						   	count(a.loccod) ||' - '|| a.acacod || ' - ' || a.acatitulo  as descricao
//						FROM
//							monitora.programa p
//							inner join monitora.acao a ON a.prgid = p.prgid
//		                    inner join monitora.avaliacaoparecer avp on avp.acaid = a.acaid
//		                    inner join monitora.referencia ref on ref.refcod = avp.refcod
//		                    inner join workflow.documento doc on doc.docid = avp.docid
//		                    inner join workflow.estadodocumento esd on esd.esdid = doc.esdid
//						WHERE
//		                	p.prgano = '{$_SESSION['exercicio']}'
//		                    and acasnrap = false
//		                    and ref.refcod = 263
//		                	and trim(prodsc) != ''
//		                    and p.prgcod not in ('1073','1449')
//		                    and esd.esdid in (620, 648)
//						GROUP BY a.acacod, a.acatitulo
//						ORDER BY count(a.loccod)";*/
//                                $esdidFinalizado = 753; // -- planacomorc finalizada
//				$sql = <<<DML
//SELECT DISTINCT apr.id_acao_programatica AS codigo,
//                org.codigo || '.' || prg.codigo  || '.' || a.codigo || ' - ' ||dae.titulo AS descricao
//  FROM planacomorc.acao a
//    INNER JOIN planacomorc.acao_programatica apr ON apr.id_acao = a.id_acao
//    INNER JOIN planacomorc.orgao org USING(id_orgao)
//    INNER JOIN planacomorc.localizador_programatica lpr USING(id_acao_programatica)
//    INNER JOIN planacomorc.quantitativo_sof qsf USING(id_acao_programatica, loccod)
//    INNER JOIN planacomorc.dados_acao_exercicio dae ON dae.id_acao = a.id_acao
//      AND dae.id_exercicio = apr.id_exercicio
//    INNER JOIN planacomorc.acompanhamento_acao acc USING(id_acao_programatica)
//    INNER JOIN workflow.documento wfd USING(docid)
//    LEFT JOIN planacomorc.programa prg USING(id_programa)
//    LEFT JOIN planacomorc.produto prd USING(id_produto)
//    LEFT JOIN planacomorc.unidade_medida u ON u.id_unidade_medida = dae.id_unidade_medida
//  WHERE apr.id_exercicio = {$_SESSION['exercicio']}
//    AND qsf.quantidade_fisico != 0
//    AND esdid = {$esdidFinalizado}
//DML;
//				mostrarComboPopup('<b>Proposta</b>', 'acaid_cadastraAcompanhamento',  $sql, $stSqlCarregados, 'Selecione a(s) Proposta(s)' );
				?></div>
			</label>
			</td></tr>-->
			<tr><td>
			<label>
				<input type="radio" name="documento" id="documento_concultaAcompanhamento" value="consultarAcompanhamento" onclick="mostraCombo('concultaAcompanhamento')"/>
				Consultar Acompanhamento Orcamentario:
					<div id="consultarAcompanhamento" style="display: none;">
				<?php 
                        	
                $sql = "SELECT 
						   	a.acacod as codigo, 
						   	a.acacod || ' - ' || a.acatitulo as descricao
						FROM 
							monitora.programa p 
							inner join monitora.acao a ON a.prgid = p.prgid
		                    inner join monitora.avaliacaoparecer avp on avp.acaid = a.acaid
		                    inner join monitora.referencia ref on ref.refcod = avp.refcod
		                    inner join workflow.documento doc on doc.docid = avp.docid
		                    inner join workflow.estadodocumento esd on esd.esdid = doc.esdid 
						WHERE 
		                	p.prgano = '{$_SESSION['exercicio']}'
		                    and acasnrap = false
		                    and ref.refcod = 263  
		                	and trim(prodsc) != '' 
		                    and p.prgcod not in ('1073','1449')
		                    and esd.esdid in (648)
						GROUP BY a.acacod, a.acatitulo 
						ORDER BY a.acacod, a.acatitulo";

				mostrarComboPopup('<b>Proposta</b>', 'acaid_concultaAcompanhamento',  $sql, $stSqlCarregados, 'Selecione a(s) Proposta(s)' );
				?></div>
			</label>
			</td></tr>
			<!--  <tr><td>
			<label>
				<input type="radio" name="documento" id="documento_obterExecucaoOrcamentariaSam" value="obterExecucaoOrcamentariaSam" onclick="mostraCombo('obterExecucaoOrcamentariaSam')"/>
				Obter Execucao Orcamentaria Sam:
					<div id="obterExecucaoOrcamentariaSam" style="display: none;"></div>
			</label>
			</td></tr>-->
			<tr><td>
			<label>
				<input type="radio" name="documento" id="documento_obterProgramacaoCompletaQuantitativo" value="obterProgramacaoCompletaQuantitativo" onclick="mostraCombo('obterProgramacaoCompletaQuantitativo')"/>
				Obter Programacao Completa Quantitativo:
					<div id="obterProgramacaoCompletaQuantitativo" style="display: none;"></div>
			</label>
			</td></tr>
			<tr><td>
			<label>
				<input type="radio" name="documento" id="documento_obterTabelasApoioQuantitativo" value="obterTabelasApoioQuantitativo" onclick="mostraCombo('obterTabelasApoioQuantitativo')"/>
				Obter Tabelas Apoio Quantitativo:
					<div id="obterTabelasApoioQuantitativo" style="display: none;"></div>
			</label>
			</td></tr>
			</table>
			&nbsp;<br/>
			<!--  <label>
				<input id="todos" type="checkbox" name="" value="acao" onclick="selecionarTodos( this );"/>
				<b>Todos</b>
			</label>-->
		</td>
	</tr>
	<tr>
		<td class="subtitulodireita"><b>Código do Momento:</b></td>
		<td><? echo campo_texto("codigomomento","S","S","codigomomento","22","","[#]","","","","","id='codigomomento'", '', '2000'); ?></td>
	</tr>
	<tr>
		<td colspan="2" align="center" style="background-color:#c0c0c0;"><input type="button" class="botao" value="Enviar Solicitação" onclick="solicitarExecucao();"></td>
	</tr>
</table>
<br>
<div id="erro"><?=$html; ?></div>
<div id="mostraerro" style="display:none">
<table align="center" width="95%" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">
	<tr>
		<th>Problema(s) Ocorrido(s) na Comunicação</th>
	</tr>
	<tr>
		<td><div id="debug"></div></td>
	</tr>
</table>
</div>
<div id="mostra_tabela_apoio" style="display: none"></div>
<script type="text/javascript">

document.getElementById('tr_acaid_cadastra').style.display = 'none';
document.getElementById('tr_acaid_consulta').style.display = 'none';
document.getElementById('tr_acaid_exclui').style.display = 'none';
//document.getElementById('tr_acaid_cadastraAcompanhamento').style.display = 'none';
document.getElementById('tr_acaid_concultaAcompanhamento').style.display = 'none';

var form = document.getElementById('formulario');

function onOffCampo( campo ){
	var div_on = document.getElementById( campo + '_campo_on' );
	var div_off = document.getElementById( campo + '_campo_off' );
	var input = document.getElementById( campo + '_campo_flag' );
	
	if ( div_on.style.display == 'none' ){
		div_on.style.display = 'block';
		div_off.style.display = 'none';
		input.value = '1';
	}else{
		div_on.style.display = 'none';
		div_off.style.display = 'block';
		input.value = '0';
	}
}

function mostraCombo(tipo){
	
	for( i=0; i<form.length; i++ ){
		if( form.elements[i].type == 'select-multiple' ){
			if( document.getElementById('documento_'+tipo).checked == true && tipo != 'obterProgramacaoCompletaQuantitativo' && tipo != 'obterTabelasApoioQuantitativo' ){
				document.getElementById('tr_acaid_'+tipo).style.display = '';
				document.getElementById( 'acaid_'+tipo+'_campo_on' ).style.display = '';
				document.getElementById( 'acaid_'+tipo+'_campo_off' ).style.display = 'none';
				document.getElementById( 'acaid_'+tipo+'_campo_flag' ).value = '1';
			}
			
			var arObjeto = form.elements[i].id.split('_');
			if( tipo != arObjeto[1] ){
				//alert(arObjeto[1]);
				//document.getElementById('debug1').innerHTML = arObjeto[1]; 
				document.getElementById('tr_acaid_'+arObjeto[1]).style.display = 'none';
				document.getElementById( 'acaid_'+arObjeto[1]+'_campo_on' ).style.display = 'none';
				document.getElementById( 'acaid_'+arObjeto[1]+'_campo_off' ).style.display = 'none';
				document.getElementById( 'acaid_'+arObjeto[1]+'_campo_flag' ).value = '0';	
			}
		}
	}
	$('#obterTabelasApoioQuantitativo').hide();
	$('#obterProgramacaoCompletaQuantitativo').hide();
	if( tipo == 'obterProgramacaoCompletaQuantitativo'){
		$('#obterProgramacaoCompletaQuantitativo').show(); 
	}
	if( tipo == 'obterTabelasApoioQuantitativo' ){
		$('#obterTabelasApoioQuantitativo').show();
	}
}

function solicitarExecucao(){
	var docCadastra = document.getElementById('documento_cadastra').checked;
	var docConsulta = document.getElementById('documento_consulta').checked;
	var docExclui = document.getElementById('documento_exclui').checked;
	var doccadastraAcompanhamento = document.getElementById('documento_cadastraAcompanhamento').checked;
	var docconcultaAcompanhamento = document.getElementById('documento_concultaAcompanhamento').checked;
	var docObterProg = document.getElementById('documento_obterProgramacaoCompletaQuantitativo').checked;
	var docObterTabela = document.getElementById('documento_obterTabelasApoioQuantitativo').checked;

	
	if( docCadastra == false && docConsulta == false && docExclui == false && doccadastraAcompanhamento == false && docconcultaAcompanhamento == false && docObterProg == false && docObterTabela == false ){
		alert('Selecione uma função');
		return false;
	} else {
		if( docObterProg == false && docObterTabela == false ){
			if( docCadastra == true ){
				var acaid_cadastra = document.getElementById('acaid_cadastra').options;
				if( acaid_cadastra[0].value == '' ){
					alert('Selecione pelo menos uma proposta!');
					return false;	
				}
			}
			if( docConsulta == true ){
				var acaid_cadastra = document.getElementById('acaid_consulta').options;
				if( acaid_cadastra[0].value == '' ){
					alert('Selecione pelo menos uma proposta!');
					return false;	
				}
			}
			if( docExclui == true ){
				var acaid_cadastra = document.getElementById('acaid_exclui').options;
				if( acaid_cadastra[0].value == '' ){
					alert('Selecione pelo menos uma proposta!');
					return false;	
				}
			}
			if( doccadastraAcompanhamento == true ){
				var acaid_cadastra = document.getElementById('acaid_cadastraAcompanhamento').options;
				if( acaid_cadastra[0].value == '' ){
					alert('Selecione pelo menos uma proposta!');
					return false;	
				}
			}
			if( docconcultaAcompanhamento == true ){
				var acaid_cadastra = document.getElementById('acaid_concultaAcompanhamento').options;
				if( acaid_cadastra[0].value == '' ){
					alert('Selecione pelo menos uma proposta!');
					return false;	
				}
			}
		}
		document.getElementById('div_auth').style.display = 'block';
	}
}
function enviaSolicitacao(){
				
	var form = document.getElementById('formulario');
	
	var usuario = document.getElementById('wsusuario').value;
	var senha = document.getElementById('wssenha').value;
	var codigomomento = document.getElementById('codigomomento').value;
	
	if(!usuario) {
		alert('Favor informar o usuário!');
		return false;
	}
	
	if(!senha) {
		alert('Favor informar a senha!');
		return false;
	}
	
	if(!codigomomento) {
		alert('Favor informar o Código do Momento!');
		return false;
	}

	//divCarregando();
	
	selectAllOptions( document.getElementById('acaid_cadastra') );
	selectAllOptions( document.getElementById('acaid_consulta') );
	selectAllOptions( document.getElementById('acaid_exclui') );
	selectAllOptions( document.getElementById('acaid_cadastraAcompanhamento') );
	selectAllOptions( document.getElementById('acaid_concultaAcompanhamento') );
	$('#requisicao').val('enviasolicitacao');
	$('#formulario').submit();
	
	//var dados = $('#formulario').serialize();

	/*$.ajax({
   		type: "POST",
   		url: "elabrev.php?modulo=sistema/comunica/importaQuantitativos&acao=A",
   		data: "wsusuario=" + usuario + "&wssenha=" + senha + "&requisicao=enviasolicitacao&"+dados,
   		async: false,
   		success: function(msg){
	   		
   			if( msg == 'erro' ){
   				alert('Nenhuma proposta encontrada para a(s) ação(ões) selecionada(s)!');
   			} else {
	   			for( i=0; i<form.length; i++ ){
					if( form.elements[i].type == 'radio' ){
						if(form.elements[i].checked == true){
							if( form.elements[i].value == 'cadastrarProposta' || form.elements[i].value == 'consultarProposta' || form.elements[i].value == 'excluirProposta' || 
									form.elements[i].value == 'cadastrarAcompanhamento' || form.elements[i].value == 'consultarAcompanhamento'){
								//limpaCombo();
								document.getElementById('debug').innerHTML = msg;
	   							document.getElementById('mostraerro').style.display='';
	   							document.getElementById('mostra_tabela_apoio').style.display = 'none';
	   							
							} else if( form.elements[i].value == 'obterProgramacaoCompletaQuantitativo' ){
								alert('Operação realizada com sucesso!\nForam carregados '+msg+' registro');
								document.getElementById('mostra_tabela_apoio').style.display = 'none';
								document.getElementById('mostraerro').style.display='none';
								//document.getElementById('erro').innerHTML = msg;
								
							} else if( form.elements[i].value == 'obterTabelasApoioQuantitativo' ){
								document.getElementById('mostra_tabela_apoio').style.display = '';
								document.getElementById('mostra_tabela_apoio').innerHTML = msg;
								document.getElementById('mostraerro').style.display='none';
							}
						}
					}
				}
			}
   			document.getElementById('<?php echo $id ?>').style.display='none';
   		}
	});*/

	//divCarregado();
	
}

function limpaCombo(){
	for( a=0; a<form.length; a++ ){
		if( form.elements[a].type == 'select-multiple' ){
			var arObjeto = form.elements[a].id.split('_');
			
			var codigo = document.getElementById( 'acaid_'+arObjeto[1] ).value;
			var campo_select = document.getElementById( 'acaid_'+arObjeto[1] );
			
			var removeu = false;
			for( var i = 0; i <= campo_select.length-1; i++ ){
				if ( codigo == campo_select.options[i].value ){
					campo_select.options[i] = null;
					removeu = true;
				}
			}
			if ( campo_select.options.length == 0 ){
				campo_select.options[0] = new Option( 'Duplo clique para selecionar da lista', '', false, false );
			}
		}
	}
}
</script>
<input type="hidden" value="" name="requisicao" id="requisicao">
<div id="<?php echo $id ?>" class="popup_alerta <?php echo $classeCSS ?>" >
	<div style="width:100%;text-align:right">
		<img src="../imagens/fechar.jpeg" title="Fechar" style="margin-top:5px;margin-right:5px;cursor:pointer" onclick="document.getElementById('<?php echo $id ?>').style.display='none'" />
	</div>
	<div style="padding:5px;text-align:justify;">
		<table class="tabela" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td width="30%" class="SubtituloDireita">Usuário:</td>
			<td><?php
				$wsusuario = $usuario; 
				echo campo_texto("wsusuario","S","S","Usuário","22","","","","","","","id='wsusuario'", '', WEB_SERVICE_SIOP_USUARIO) ?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Senha:</td>
			<td>
				<input type="password" class="obrigatorio normal" title="Senha" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" value="<?php echo WEB_SERVICE_SIOP_USUARIO; ?>" size="23" id="wssenha" name="wssenha">
				<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
			</td>
		</tr>
		<tr>
			<td align="center" bgcolor="#D5D5D5" colspan="2">
				<input type="button" name="btn_enviar" onclick="enviaSolicitacao()" value="ok" />
				<input type="button" name="btn_cancelar" onclick="document.getElementById('<?php echo $id ?>').style.display='none'" value="cancelar" />
			</td>
		</tr>
		</table>
	</div>
</div>
<div id="debug1"></div>