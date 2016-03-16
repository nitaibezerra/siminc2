<?php
	
class HistoricoMonitoramentoDadosBancarios extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "cte.historicomonitoramentodadosbancarios";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "hmdid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'hmdid' => null, 
									  	'hmdmes' => null, 
									  	'hmdano' => null, 
									  	'hmdsaldo1diames' => null, 
									  	'hmdsaldoultimodiames' => null, 
									  	'hmdvalorcontrapartida' => null, 
									  	'hmdrendimento' => null, 
									  	'prsid' => null, 
									  );
									  
	public function listaDadosBancarios($prsid, $boImpressao = false){
		global $db;
		
		$nrQtdPorPagina = $boImpressao ? 200 : 20;
		
		$sqlData			= " SELECT 
									hmdano as maiorano, hmdmes as maiormes
								FROM cte.historicomonitoramentodadosbancarios
								WHERE prsid = '".$prsid."' 
								AND hmdmes in (
									SELECT 
										MAX(hmdmes) as maiormes
									FROM cte.historicomonitoramentodadosbancarios
									WHERE prsid = '".$prsid."' AND hmdano = (SELECT MAX(hmdano) as maiorano 
													FROM cte.historicomonitoramentodadosbancarios
													WHERE prsid = '".$prsid."')
								) 
								AND hmdano in (SELECT MAX(hmdano) as maiorano 
										FROM cte.historicomonitoramentodadosbancarios
										WHERE prsid = '".$prsid."')
							  ";
		$ultimaData			= $db->pegaLinha($sqlData);
		$ultimoano			= $ultimaData['maiorano'];
		$ultimomes			= $ultimaData['maiormes'];
		$select 			= '';
		
		if( !$boImpressao ){
			if($ultimomes && $ultimaData){
				$select = 'CASE WHEN hmdmes = '.$ultimomes.' AND hmdano = '.$ultimoano.'
									THEN \'<center><img id="removeMes" name="removeMes" style="display:block" onclick="return removerMes(\' || hmdid || \')" src="/imagens/excluir.gif" title="Excluir" alt="Excluir \' || hmdmes || \'" /></center>\' 
									ELSE \'<center><img id="removeMes" name="removeMes" style="display:none" onclick="return removerMes(\' || hmdid || \')" src="/imagens/excluir.gif" title="Excluir" alt="Excluir \' || hmdmes || \'" /></center>\'
									END as deletar,';
			}else{
				$select = " '' as deletar, ";
			}
		}
		
		$listaDados = 'SELECT 	'.$select.'
								hmdmes, 
								hmdano, 
								hmdsaldo1diames, 
								hmdsaldoultimodiames, 
								hmdvalorcontrapartida, 
								hmdrendimento 
						FROM cte.historicomonitoramentodadosbancarios 
						WHERE prsid = '.$prsid.' 
						ORDER BY hmdano, hmdmes ASC';
		
		if( $boImpressao ){
			$cabecalho = array("Mês","Ano", "Saldo 1° dia do Mês", "Saldo último dia mês","Valor contra partida", "Rendimento");
		}
		else{
			$cabecalho = array("Ação","Mês","Ano", "Saldo 1° dia do Mês", "Saldo último dia mês","Valor contra partida", "Rendimento");
		}
		
		return $this->monta_lista($listaDados,$cabecalho,$nrQtdPorPagina, 10, 'N', '100%', '');
	}
	
	/**
	 * function carregaMeses($prsid, $ano);
	 * @desc   : Faz a busca no banco e retorna a lista de Meses passivel de monitoramento do Convênio. 
	 * @author : Thiago Tasca Barbosa
	 * @param  : 
	 * @return : 
	 * @since 17/03/2009
	 */
	public function carregaMeses($prsid, $ano, $boImprimir = true){
		global $db;
		$temMonitoramento = 0;

		$sql = "	SELECT db.hmdmes, db.hmdano, hc.hmcid, to_char(hc.hmcdatamonitoramento, 'MM') AS mes, db.prsid , hc.hmcstatus, db.hmdid
					FROM cte.historicomonitoramentodadosbancarios db
					LEFT JOIN cte.historicomonitoramentoconvenio hc ON hc.prsid = db.prsid 
									and to_char(hmcdatamonitoramento, 'MM')::integer = hmdmes
									and to_char(hmcdatamonitoramento, 'YYYY')::integer = hmdano
					WHERE db.prsid = ".$prsid." and db.hmdano >= ".$ano." ORDER BY hmdano, hmdmes";
		//dbg($sql,1);
		$meses = $db->carregar($sql);
		if(is_array($meses) ){
			foreach($meses as $dados){
				if($dados['hmcid']){
					$temMonitoramento = 1;
					break;
				}
			}
		}
		
		$sql = "SELECT db.hmdano, count(hmdano) AS ano
				FROM cte.historicomonitoramentodadosbancarios db
				LEFT JOIN cte.historicomonitoramentoconvenio hc ON hc.prsid = db.prsid 
					and to_char(hmcdatamonitoramento, 'MM')::integer = hmdmes
					and to_char(hmcdatamonitoramento, 'YYYY')::integer = hmdano
				WHERE db.prsid = ".$prsid."  and db.hmdano >= ".$ano."
				GROUP BY db.hmdano
				ORDER BY count(hmdano) desc";
		$totalMesesPorAno = $db->carregar($sql);
		
		$conteudo = $this->getMeses($meses, $prsid, $temMonitoramento, $totalMesesPorAno, $ano, $boImprimir);
		return $conteudo; 
		
	}
	
	/**
	 * function getMeses;
	 * @desc   : Monta a lista de itens de composição. 
	 * @author : Thiago Tasca Barbosa
	 * @param  : 
	 * @return : 
	 * @since 17/03/2009
	 */
	public function getMeses($meses, $prsid, $temMonitoramento, $totalMesesPorAno = NULL, $anoImpressao = null, $boImprimir = true){
		$resultBoleano = true;
		$porcentagemTotal = 0;
		$stTitulo = $boImprimir ? 'Selecione um mês do convênio para iniciar o Monitoramento.' : 'Monitoramento'; 
		$perfilConv = Array(CTE_PERFIL_SUPER_USUARIO, CTE_PERFIL_ADMINISTRADOR, CTE_PERFIL_ADMINISTRADOR_FINANCEIRO);
		$InformacoesConvenio = cte_possuiPerfil( $perfilConv ) ? '<a style="cursor:pointer;float:left;" onclick="abreTelaCadastroInformacoesConvenio('.$prsid.')">Informações sobre o convênio.</a>' : ' ';
		$conteudo = '
		<table align="center" border="0" class="tabela tabelaMonitoramento" cellpadding="3" cellspacing="1">
				<tr>
					<th class="texto" > 
						'.$InformacoesConvenio.'
						' .  $stTitulo . '
					</th>
				</tr>
		</table>
		<table align="center" border="0" class="tabela tabelaMonitoramento" cellpadding="3" cellspacing="1">';
		if(is_array($meses)){
			
			$arMeses = array();
			foreach( $meses as $arDados ){
				$arMeses[$arDados["hmdano"]][] = $arDados; 
			}
			
			$nrMaior = $totalMesesPorAno[0]["ano"];
			
			foreach( $totalMesesPorAno as $arDados ){
				$arColspan[$arDados["hmdano"]] = $arDados["ano"]; 
			}
			
			global $db;
			$data = new Data();
			
			foreach( $arMeses as $ano => $arDados ){
				
				$colspan = ( $arColspan[$ano] != $nrMaior ) ? $nrMaior - $arColspan[$ano] : 1; 
				
				$conteudo .= '<tr>';
				foreach( $arDados as $dados ){
					if( $dados['mes'] == $dados['hmdmes'] && $dados['hmcstatus'] == "I"){
						$btn = ' onclick="carregaMonitoramento('.$prsid.','.$dados['hmcid'].','.$dados['hmdid'].');"';
					}else if( $dados['hmcstatus'] == "F"){
						$btn = ' onclick="carregaMonitoramento('.$prsid.','.$dados['hmcid'].','.$dados['hmdid'].');"';
					}else if($dados['hmcstatus'] == NULL && $this->validaSePodeInserirMonitoramento($dados['hmdid'], $resultBoleano) == true){
						$btn = ' onclick="validaSePodeInserirMonitoramento('.$dados['hmdid'].','.$prsid.');"';	
					}else if($dados['hmcstatus'] == NULL && $this->validaSePodeInserirMonitoramento($dados['hmdid'], $resultBoleano) == false ){
						$btn = ' onclick="validaSePodeInserirMonitoramento('.$dados['hmdid'].','.$prsid.');"';
					}
					
					$mes = $data->mesTextual($dados['hmdmes']);
					$mes = substr($mes,0,3);
					$conteudo .='	<td class="SubTituloEsquerda" ><a style="cursor:pointer;" '.$btn.'>'.$mes.'/'.$dados["hmdano"].'</a></td>';
					
				}
				$conteudo .= $colspan != 1 ? '<td class="SubTituloEsquerda" colspan="'.$colspan.'">&nbsp;</td>' : "";
				$conteudo .= '</tr><tr>';

				foreach( $arDados as $dados ){
					 
					$imagemStatus =  $this->porcentagensMesesMonitoramento($_SESSION['inuid'],$dados['hmdano'], $dados['prsid'], $dados['hmdmes'], $porcentagemTotal);
					
					if($dados['mes'] == $dados['hmdmes'] && $dados['hmcstatus'] == "I"){
						$btn = ' onclick="carregaMonitoramento('.$prsid.','.$dados['hmcid'].','.$dados['hmdid'].');"';
						$imagem = '<div style=" position: relative; "> <img width="13" height="13"  src="../../imagens/lapis.png"/> </div>';
						
					}else if( $dados['hmcstatus'] == "F"){
						$btn = ' onclick="carregaMonitoramento('.$prsid.','.$dados['hmcid'].','.$dados['hmdid'].');"';
						$imagem = '<div style=" position: relative; "> <img width="13" height="13"  src="../../imagens/check_p.gif"/> </div>';
						
					}else if($dados['hmcstatus'] == NULL && $this->validaSePodeInserirMonitoramento($dados['hmdid'], $resultBoleano ) == true){
						$imagem = '<div style=" position: relative; "> <img width="18" height="13"  src="../../imagens/cadeadoAberto.png"/> </div>';
						$btn = ' onclick="validaSePodeInserirMonitoramento('.$dados['hmdid'].','.$prsid.');"';
						
					}else if($dados['hmcstatus'] == NULL && $this->validaSePodeInserirMonitoramento($dados['hmdid'], $resultBoleano) == false ){
						$btn = ' onclick="validaSePodeInserirMonitoramento('.$dados['hmdid'].','.$prsid.');"';
						$imagem = '<div style=" position: relative; "><img width="13" height="13"  src="../../imagens/cadiado.png"/> </div>';
						
					}
					$conteudo .='<td style="background-color: #F5F5F5;"><a style="cursor:pointer;" onmouseout="SuperTitleOff( this );" onmousemove="SuperTitleAjax(\'../../cte/monitoraFinanceiro/infoMonitora.php?hmcid='.$dados['hmcid'].'&ano='.$dados['hmdano'].'\')", this)  '.$btn.'>'.$imagemStatus.$imagem.'</a></td>';
					
				}
				$conteudo .= $colspan != 1 ? '<td style="background-color: #F5F5F5;" colspan="'.$colspan.'">&nbsp;</td>' : "";
				$conteudo .= '</tr>';
			}
		}else{
			$conteudo.= '<td class="SubTituloEsquerda"  style="padding-top: 10px; padding-bottom: 10px; background-color:#F5F5F5;"> Não existe meses cadastrados para ser monitorado. Entre em contato com o Gestor do Sistema. </td>';
		}
		
		$conteudo .='</table>';	
//						<tr>
//							<td colspan="12" class="textoEsquerda" ><a style="cursor:pointer;float:left;" onclick="abreTelaCadastroInformacoesConvenio('.$prsid.', '.$ano.')">Informações sobre o convênio.</a></td>
//						</tr>
					 
		
		if( $boImprimir ){
			$conteudo .= '			
				<div style="text-align: center; margin-top: 20px;">
					<a style="cursor: pointer" onclick="return false;">
						<img src="/imagens/print.png" alt="Versão para impressão" onclick="janelaImpressao('.$prsid.', '.$anoImpressao.')"> 
					</a>
				</div>';		
		}
//		else{
//			$conteudo .= '
//				<div style="text-align: center; margin-top: 20px;">
//					<input type="button" value="imprimir" onclick="javascript:window.print()" />
//				</div>';
//		}
		return $conteudo;
	}
	
	/**
	 * function podeIniciarConvenio($arryDataConvenio);
	 * @desc   : Verifica se o convenio pode iniciar.
	 * 			 Regra: O monitoramento so pode começar no apartir do dia 15 do proximo mês. 
	 * @author : Thiago Tasca Barbosa
	 * @param  : array $arryDataConvenio 
	 * @return : boleano $retorno (retorna true se ok ou false)
	 * @since 20/04/2009
	 */
	function podeIniciarConvenio($mes, $ano){
		$data = new Data();
		if($mes != 12){
			$mes += 01;
		}else{
			$mes = '01';
			$ano += 1; 
		}
		$dataMinima = $ano.'-'.$mes.'-15';
		$retorno = $data->diferencaEntreDatas(date('Y-m-d'),$dataMinima, 'maiorDataBolean','','YYYY-MM-DD');
		return $retorno;
	}
	
	public function validaSePodeInserirMonitoramento($hmdid, $resultBoleano = false){
		// INSTANCIA OBJETOS VARIAVEIS
		$obdata 			= new Data();
		$boResultSucesso 	= false;
		
		// VERIFICA SE TEM MESES E DADOS BANCARIOS CADASTRADOS 
		$sql = "SELECT hmdmes, hmdano, prsid 
				FROM cte.historicomonitoramentodadosbancarios 
				WHERE hmdid = ".$hmdid;
		$dadosMeses = $this->pegaLinha($sql);
		
		if($dadosMeses){ // se existe meses cadastrados para ser monitorado
			
			//RECUPERA TODOS OS MONITORAMENTOS EXISTENTES
			$monitoramento 	= new HistoricoMonitoramentoConvenio();
			$monitoramentos = $monitoramento->recuperaTodosMonitoramentos($dadosMeses['prsid']);
			if($monitoramentos){ // se já existe algum monitoramento valida regras
				
				// REGRA 1 - O monitoramento do último mês monitorado foi finalizado e o mês e o proximo?
				$sql = "SELECT hmcid, prsid, hmcdatamonitoramento, hmcstatus, hmdid, 
						date_part('year', hmcdatamonitoramento) as ano, 
						date_part('months', hmcdatamonitoramento) as mes 
						FROM cte.historicomonitoramentoconvenio 
						WHERE prsid = ".$dadosMeses['prsid']."
						ORDER BY hmcdatamonitoramento DESC LIMIT 1"; // recupera o mes do ultimo monitoramento feito.
				$dadosUltimoMonit = $monitoramento->pegaLinha($sql);
				
				$nrUltimoMesMonitorado = $dadosUltimoMonit['mes'];
				$nrUltimoAnoMonitorado = $dadosUltimoMonit['ano'];
				$stStatusUltimoMesMonitorado = $dadosUltimoMonit['hmcstatus'];
				if( 
					( 
						( $dadosMeses['hmdmes'] - $nrUltimoMesMonitorado ==  1  && $dadosMeses['hmdano'] - $nrUltimoAnoMonitorado == 0 ) ||
						( $dadosMeses['hmdmes'] - $nrUltimoMesMonitorado == -11 && $dadosMeses['hmdano'] - $nrUltimoAnoMonitorado == 1 ) 
					) && $stStatusUltimoMesMonitorado == 'F' 
				){
					if($resultBoleano){
						$boResultSucesso = true;
					}
				}else if($stStatusUltimoMesMonitorado == 'I'){
					if($resultBoleano){
						return false;
					}else{
						return "Já existe monitoramento aberto. \n Finalize-o primeiro para poder iniciar o próximo.";
					}
				}else{
					if($resultBoleano){
						return false;
					}else{
						return "Este mês não pode ser monitorado. \n Monitore os meses anteriores para que possa monitorar este mês.";
					}
				}
				
				// REGRA 2 - O monitoramento não pode ser iniciado antes do dia 15 do próximo mês do convênio.
				$passouDataMonit = $this->podeIniciarConvenio($dadosMeses['hmdmes'], $dadosMeses['hmdano']);
				if($passouDataMonit == false){
					if($resultBoleano){
						return false;
					}
					return "O monitoramento não pode ser iniciado antes do dia 15 do próximo mês cadastrado.";
				}else{
					$boResultSucesso = true;
				}
				
				//SE TODAS AS REGRAS RETORNAR SUCESSO, PODE INICIAR MONITORAMENTO.
				if($boResultSucesso){
					return true;
				}else{
					return false;
				}
			}else{ // se não existem monitoramentos habilita o monitoramento do 1 mês cadastrado.
				//REGRA 1 - HABILITA MONITORAMENTO DO 1 MÊS.
				$sql = "SELECT hmdid, hmdmes, hmdano
						FROM cte.historicomonitoramentodadosbancarios 
						WHERE prsid = ".$dadosMeses['prsid']."
						ORDER BY hmdano, hmdmes ASC limit 1";
				
				$PrimeiroMes = $this->pegaLinha($sql);
				if($PrimeiroMes['hmdid'] == $hmdid){ // se for 1 mês deixa aberto para monitorar
						$boResultSucesso = true;
				}else {
					if($resultBoleano){
						return false; 
					}
					return "O monitoramento do mês de ".$obdata->mesTextual($dadosMeses['hmdmes'])." de ".$dadosMeses['hmdano']." não pode ser iniciado.\n Monitore primeiro o mês de ".$obdata->mesTextual($PrimeiroMes['hmdmes'])." de ".$PrimeiroMes['hmdano'].".";
				}
				
				// REGRA 2 - O monitoramento não pode ser iniciado antes do dia 15 do próximo mês do convênio.
				$passouDataMonit = $this->podeIniciarConvenio($dadosMeses['hmdmes'], $dadosMeses['hmdano']);
				if($passouDataMonit == false){
					if($resultBoleano){
						return false;
					}
					return "O monitoramento não pode ser iniciado antes do dia 15 do próximo mês do convênio.";
				}else{
					$boResultSucesso = true;
				}
				
				//SE TODAS AS REGRAS RETORNAR SUCESSO, PODE INICIAR MONITORAMENTO.
				if($boResultSucesso){
					return true;
				}else{
					return false;
				}
			}
			
		}else{ // se não existe meses cadastrados para ser monitorado
			if($resultBoleano){
				return false; 
			}
		}
	}

	public function recuperaTodosMeses($prsid){
		$sql = "SELECT hmdid, hmdmes, hmdano
						FROM cte.historicomonitoramentodadosbancarios 
						WHERE prsid = ".$prsid."
						ORDER BY hmdid ASC";
		$meses = $this->carregar($sql);
		return $meses;
	}
	
	public function recuperaMes($hmdid){
		$sql = "SELECT hmdid, hmdmes, hmdano, prsid
						FROM cte.historicomonitoramentodadosbancarios 
						WHERE hmdid = ".$hmdid;
		$mes = $this->pegaLinha($sql);
		return $mes;
	}

	
	
	public function porcentagensMesesMonitoramento($inuid,$ano, $prsid, $mesAtual, &$porcentagemTotal){
		global $db;
		$data = new Data();
		if(!$prsid){
			$prsid = 0;
		}
		
		$sql = "SELECT p.prsnumconvsape , hc.hmcid
				FROM cte.historicomonitoramentodadosbancarios d
				INNER JOIN cte.projetosape p ON p.prsid = d.prsid
				LEFT JOIN cte.historicomonitoramentoconvenio hc ON hc.prsid = d.prsid and d.hmdid = hc.hmdid
				WHERE p.prsid = ".$prsid." and hmdmes = '".$mesAtual."'  and d.hmdano = ".$ano;
		
		$dadosGerais = $this->pegaLinha($sql);
		$hmcid = $dadosGerais["hmcid"] == NULL ? 0 : $dadosGerais["hmcid"];
		
		$sql = "SELECT 	SUM(total) AS total, 
						SUM(analisado) AS analisado
				FROM(	SELECT  p.prsvalorconvenio as total,
							0 AS analisado,
							p.prsnumconvsape
					FROM cte.projetosape p
					INNER JOIN cte.historicomonitoramentoconvenio hc ON hc.prsid = p.prsid
					WHERE  hc.hmcid = ".$hmcid."
					UNION ALL
					SELECT  0 AS total,
							sum(hmsvalortotalempenhado) AS analisado,
							p.prsnumconvsape
					FROM cte.historicomonitoramentoconvenio hc
					INNER JOIN cte.historicomonitoramentoconvsubac hs ON hc.hmcid = hs.hmcid
					INNER JOIN cte.historicoconvitemcomposicao hci ON hci.hmsid = hs.hmsid 
					INNER JOIN cte.projetosape p ON  hc.prsid = p.prsid
					WHERE hmsvalortotalempenhado IS NOT NULL 
					AND hc.hmcid <= ".$hmcid."
					AND p.inuid = ".$inuid."
					GROUP BY p.prsnumconvsape
				) AS valores GROUP BY prsnumconvsape";
		
		$analise = $db->pegaLinha($sql);
		
		if(is_array($analise)){
			
			if( $analise['total'] > 0){
				$porcentagem = ($analise['analisado'] * 100) /  $analise['total'];		
				$porcentagem = arredondar_dois_decimal($porcentagem); 
			}else{
				$porcentagem = 0;
				$porcentagem = arredondar_dois_decimal($porcentagem);
			}
			$porcentagemTotal = $porcentagem;
			
			if($porcentagemTotal  == 49 || $porcentagemTotal  == 50  || $porcentagemTotal  == 51 ){
				$cor 	= "#FFFFFF";
				$cor1 	= "black";
			}else if($porcentagemTotal  < 48){
				$cor 	= "#cococo";
				$cor1 	= "black";
			}else if ($porcentagemTotal  > 51 ){
				$cor 	= "#FFFFFF";
				$cor1 	= "#FFFFFF";
			}
						
			if($porcentagemTotal > 100){
				$corBarra = 100;
			}else{
				$corBarra = $porcentagemTotal;
			}
				
			$barra 	= '<center>';
			$barra .= '	<div style="float: left;">
						<div class="barra1">
							<div style="text-align:center; position: absolute; width: 50px; height: 10px; padding: 0 0 0 0;  margin-bottom: 0px; " >
								<div style=" color:'.$cor.'; font-size: 10px; max-height: 10px;  ">'.$porcentagemTotal.'<span style="color:'.$cor1.'" >%</span></div>
							</div>
						<img class="imgBarra" style="width: '.$corBarra.'%;" src="../../imagens/cor1.gif"/>
						</div>
						</div>
						</center>';
		}else{
			$barra  = '<center>';
			$barra .= '	<div style="float: left;">
						<div class="barra1" style=" text-align:center;">
						<span style=" color:cococo;font-size: 10px; "> 0 %</span>
						</div>
						</div>
						</center>';	
		}
		
		return $barra;
	}
	
	public function barraListaHistoricoMonitoramento($cosano, $prsnumconvsape, $hmcid){
		global $db;
		if($hmcid == NULL){
			$hmcid = 0;
		}
		$sql = "SELECT 	SUM(total) AS total, 
						SUM(analisado) AS analisado,					
						prsnumconvsape
				FROM(
					SELECT  --sum(cs.cosvlruni * cs.cosqtd) AS total,
							prsvalorconvenio as total,
							0 AS analisado,
							p.prsnumconvsape
					FROM cte.projetosape p
					INNER JOIN cte.projetosapesubacao ps ON p.prsid = ps.prsid
					INNER JOIN cte.subacaoindicador si ON si.sbaid = ps.sbaid
					INNER JOIN cte.composicaosubacao cs ON cs.sbaid = si.sbaid
					INNER JOIN cte.historicomonitoramentoconvenio hc ON hc.prsid = p.prsid
					WHERE p.prsnumconvsape = ".$prsnumconvsape."  AND cs.cosano = ".$cosano." AND hc.hmcid = ".$hmcid."
					GROUP BY prsnumconvsape, total
					UNION ALL
					SELECT  0 AS total,
							sum(hmsvalortotalempenhado) AS analisado,						
							p.prsnumconvsape
					FROM cte.historicomonitoramentoconvenio hc
					INNER JOIN cte.historicomonitoramentoconvsubac hs ON hc.hmcid = hs.hmcid
					INNER JOIN cte.historicoconvitemcomposicao hci ON hci.hmsid = hs.hmsid 
					INNER JOIN cte.projetosape p ON  hc.prsid = p.prsid
					WHERE hmsvalortotalempenhado IS NOT NULL AND hc.hmcid = ".$hmcid."
					GROUP BY p.prsnumconvsape
				) AS valores GROUP BY prsnumconvsape";
		
		return $db->pegaLinha($sql);
	}
	
	public function carregaPorcentagemConvenio($prsid, $ano){
		
		global $db;

		$sql = "SELECT 	
					CASE WHEN SUM(total) > 0
						THEN (SUM(analisado) * 100) / SUM(total)
						ELSE 0
					END as porcentagem1
					--SUM(total) AS total, 
					--SUM(analisado) AS analisado,					
					--prsnumconvsape
				FROM(
					SELECT  
						--sum(cs.cosvlruni * cs.cosqtd) AS total,
						prsvalorconvenio as total,
						0 AS analisado,
						p.prsnumconvsape		
					FROM cte.projetosape p
						INNER JOIN cte.projetosapesubacao ps ON p.prsid = ps.prsid
						INNER JOIN cte.subacaoindicador si ON si.sbaid = ps.sbaid
						INNER JOIN cte.composicaosubacao cs ON cs.sbaid = si.sbaid
						INNER JOIN cte.historicomonitoramentoconvenio hc ON hc.prsid = p.prsid
					WHERE p.prsid = ".$prsid."  AND cs.cosano = ".$ano."
					GROUP BY prsnumconvsape, total
					
					UNION ALL
					
					SELECT  
						0 AS total,
						sum(hmsvalortotalempenhado) AS analisado,						
						p.prsnumconvsape
					FROM cte.historicomonitoramentoconvenio hc
						INNER JOIN cte.historicomonitoramentoconvsubac hs ON hc.hmcid = hs.hmcid
						INNER JOIN cte.historicoconvitemcomposicao hci ON hci.hmsid = hs.hmsid 
						INNER JOIN cte.projetosape p ON  hc.prsid = p.prsid
					WHERE hmsvalortotalempenhado IS NOT NULL and p.prsid = ".$prsid."
					GROUP BY p.prsnumconvsape
					
				) AS valores GROUP BY prsnumconvsape";
		$valor = round($db->pegaUm($sql), 2);
				
		if($valor  == 49 || $valor  == 50  || $valor  == 51 ){
			$cor 	= "#FFFFFF";
			$cor1 	= "black";
		}else if($valor  < 48){
			$cor 	= "#cococo";
			$cor1 	= "black";
		}else if ($valor  > 51 ){
			$cor 	= "#FFFFFF";
			$cor1 	= "#FFFFFF";
		}
					
		if($valor > 100){
			$corBarra = 100;
		}else{
			$corBarra = $valor;
		}
		
		$barra 	= '<center>';
			$barra .= '	<div style="float: left;">
						<div class="barra1">
							<div style="text-align:center; position: absolute; width: 50px; height: 10px; padding: 0 0 0 0;  margin-bottom: 0px; " >
								<div style=" color:'.$cor.'; font-size: 10px; max-height: 10px;  ">'.$valor.'<span style="color:'.$cor1.'" >%</span></div>
							</div>
						<img class="imgBarra" style="width: '.$corBarra.'%;" src="../../imagens/cor1.gif"/>
						</div>
						</div>
						</center>';
		
		return $barra;
		
	}
	
}