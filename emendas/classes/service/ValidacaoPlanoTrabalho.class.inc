<?php

// Classe de validação das informações cadastradas no 
// Plano de Trabalho do sistema de Emendas.
class ValidacaoPlanoTrabalho {
	
	// variável de conexão com o banco
	private	$db;
	
	// chave primária do PTA. Recebida obrigatoriamente por parâmetro
	private $ptrid;
	
	// dados da view 'v_planotrabalho'
	private $entid;
	private $ptrexercicio;
	private $resid;
	private $ptrcontrapartida;
	private $ptrvalorconcedente;
	private $ptrvalorproponente;
	private $ptrvalortotal;
	private $ptivalorconcedente;
	private $ptivalorproponente;
	private $ptivalortotal;
	private $estadoAtual;
	
	// array com os dados do JOIN entre 'emenda.iniciativa' e 'emenda.v_ptiniciativa'
	private $iniciativas = array();
	
	// array para registro de todos as pendências encontradas
	public $retorno = array();
	
	// método construtor
	public function __construct( $ptrid ) {
		$this->ptrid = $ptrid;
		
		global $db;
		$this->db = ( ($db) ? $db : die("Ocorreu um erro com a conexão do Banco de Dados.") );
		
		$sql = "SELECT * FROM emenda.v_planotrabalho WHERE ptrid = ".$ptrid;
		$dados = $db->carregar($sql);
		
		if($dados) {
			$this->enbid				= $dados[0]["entid"];
			$this->ptrexercicio 		= $dados[0]["ptrexercicio"];
			$this->resid 				= $dados[0]["resid"];
			$this->ptrcontrapartida 	= $dados[0]["ptrcontrapartida"];
			$this->ptrvalorconcedente	= $dados[0]["ptrvalorconcedente"];
			$this->ptrvalorproponente 	= $dados[0]["ptrvalorproponente"];
			$this->ptrvalortotal 		= $dados[0]["ptrvalortotal"];
			$this->ptivalorconcedente 	= $dados[0]["ptivalorconcedente"];
			$this->ptivalorproponente 	= $dados[0]["ptivalorproponente"];
			$this->ptivalortotal 		= $dados[0]["ptivalortotal"];
			$this->ptrcontrapartida		= $dados[0]["ptrcontrapartida"];
		}

		$this->estadoAtual = pegarEstadoAtual( $ptrid );
		
		$sql = "SELECT
					*
				FROM
					emenda.v_ptiniciativa vpti
				INNER JOIN
					emenda.iniciativa ini ON ini.iniid = vpti.iniid
				WHERE
					vpti.ptrid = ".$ptrid;
		
		$dados = $db->carregar($sql);
		
		if($dados) {
		  for($i=0; $i<count($dados); $i++) {
			$this->iniciativas[$i]['ptiid']						= $dados[$i]['ptiid'];
			$this->iniciativas[$i]['ptrid']						= $dados[$i]['ptrid'];
			$this->iniciativas[$i]['ptidescricao'] 				= $dados[$i]['ptidescricao'];
			$this->iniciativas[$i]['iniid'] 					= $dados[$i]['iniid'];
			$this->iniciativas[$i]['tpeid'] 					= $dados[$i]['tpeid'];
			$this->iniciativas[$i]['ptivalorproponente']		= $dados[$i]['ptivalorproponente'];
			$this->iniciativas[$i]['ptivalorconcedente'] 		= $dados[$i]['ptivalorconcedente'];
			$this->iniciativas[$i]['ptivalortotal'] 			= $dados[$i]['ptivalortotal'];
			$this->iniciativas[$i]['ininome']					= $dados[$i]['ininome'];
			$this->iniciativas[$i]['temid'] 					= $dados[$i]['temid'];
			$this->iniciativas[$i]['resid'] 					= $dados[$i]['resid'];
			$this->iniciativas[$i]['gndcod'] 					= $dados[$i]['gndcod'];
			$this->iniciativas[$i]['inistatus'] 				= $dados[$i]['inistatus'];
			$this->iniciativas[$i]['iniescolasbeneficiasdas'] 	= $dados[$i]['iniescolasbeneficiasdas'];
		  }
		}
	}
	
	public function validacaoPlanoTrabalho() {
		$this->validaRecursosPTA();
		$this->validaIniciativas();
		$this->validaCronograma();
		$this->validaEscolasBeneficiadas();
		$this->validaDadosAdicionais();
		$this->validaAnexos();
		$this->validaAnexosGuia();
		$this->validaEnderecoEntidade();
		//$this->validaTipoImpositivo();
		
		return $this->retorno;
	}
	
	private function validaRecursosPTA() {
		$this->retorno['recursospta'] = array();
		
		$this->emendasExclusivasPTA();
		$bocontrapartida = validaContraPartidaPTA( $this->enbid );
		
		if( $_SESSION['emenda']['federal'] == 'N' && $bocontrapartida == 'true' ){
			$this->verificaContraPartida();
		}
	}
	
	private function emendasExclusivasPTA() {
		$sql = "SELECT
					ptr2.ptrid as num_pta
					,vede.emecod as emenda
					,case when vede.emerelator = 'S' then aut.autnome||' - Relator Geral' else aut.autnome end as autor
					,fup.fupfuncionalprogramatica
					,vede.gndcod as gnd
					,vede.mapcod as mod
					,vede.foncod as fonte
					,pde2.pedvalor as valor
				FROM
					emenda.planotrabalho ptr2
				INNER JOIN
					emenda.ptemendadetalheentidade pde2 ON pde2.ptrid = ptr2.ptrid
				INNER JOIN
					emenda.v_emendadetalheentidade vede ON vede.edeid = pde2.edeid
				INNER JOIN
					emenda.v_funcionalprogramatica fup ON fup.acaid = vede.acaid
				INNER JOIN
					emenda.autor aut ON aut.autid = vede.autid
				WHERE
					pde2.edeid in (
						SELECT
							pde.edeid 
						FROM 
							emenda.planotrabalho ptr
						INNER JOIN
							emenda.ptemendadetalheentidade pde ON pde.ptrid = ptr.ptrid
						WHERE
							ptr.ptrid = ".$this->ptrid.")
					AND ptr2.ptrid <> ".$this->ptrid."
					AND ptr2.ptridpai <> ".$this->ptrid."
					AND ptr2.ptrstatus = 'A'
					AND ptr2.ptrid NOT IN (SELECT tt.ptridpai FROM emenda.planotrabalho tt WHERE tt.ptridpai = ptr2.ptrid and tt.ptrstatus = 'A')";
		
		$valida = $this->db->carregar($sql);
		
		if($valida) {
			$texto = "";
			
			$texto .= 'As seguintes emendas já estão sendo utilizadas em outros Planos de Trabalho:<br />';
			$texto .= '<table width="50%" border="0" cellspacing="0" cellpadding="2" class="listagem">
			               <thead>
			               <tr>
			               		<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Número do PTA</strong></td>
			                    <td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Emenda</strong></td>
			                    <td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Autor</strong></td>
			                    <td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>GND</strong></td>
			                    <td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Mod</strong></td>
			                    <td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Fonte</strong></td>
			                    <td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Valor</strong></td>
			               </tr>
			               </thead>
			               <tbody>';
			
			for($i=0; $i<count($valida); $i++)
			{
				$cor = ($i % 2) ?  "#e0e0e0" : "#f4f4f4";
				
				$texto .= '<tr bgcolor="'.$cor.'">
		                   	 <td align="center">'.$valida[$i]['num_pta'].'</td>
		                   	 <td align="center">'.$valida[$i]['emenda'].'</td>
		                   	 <td align="center">'.$valida[$i]['autor'].'</td>
		                   	 <td align="center">'.$valida[$i]['gnd'].'</td>
		                   	 <td align="center">'.$valida[$i]['mod'].'</td>
		                   	 <td align="center">'.$valida[$i]['fonte'].'</td>
		                   	 <td align="center">R$ '.number_format($valida[$i]['valor'], 2, ",", ".").'</td>
		                   </tr>';
			}
			
			$texto .= '</tbody>
       				</table>';
			
			array_push($this->retorno['recursospta'], $texto);
		}
	}
	
	private function verificaContraPartida() {
				
		$valorCalculoContrapartida = 100 - $this->ptrcontrapartida;
		
		$valorContrapartidaEsperada = ($this->ptrvalorconcedente / $valorCalculoContrapartida);
		//$valorContrapartidaEsperada = ($valorContrapartidaEsperada + 0.01);
		
		$valorContrapartidaEsperada = substr($valorContrapartidaEsperada, 0, (strpos($valorContrapartidaEsperada, ".")+3));
		
		if((float)$this->ptrvalorproponente < (float)$valorContrapartidaEsperada )
			array_push($this->retorno['recursospta'], "O valor mínimo exigido para contrapartida do proponente é de R$ ".number_format($valorContrapartidaEsperada, 2, ",", ".").". O valor informado (R$ ".number_format($this->ptrvalorproponente, 2, ",", ".").") não atende esse limite.");
	}
	
	private function validaDadosAdicionais() {
		$this->retorno['dadosadicionais'] = array();
		
		$sql = "SELECT
					ptrjustificativa,
					ptrtipodirigente,
					entiddirigente,
					bcoid,
					ptragenciabancaria
				FROM
					emenda.planotrabalho
				WHERE
					ptrid = ".$this->ptrid;
		
		$valida = $this->db->carregar($sql);
		
		if($valida) {
			extract($valida[0]);
			
			if(!isset($ptrjustificativa))
				array_push($this->retorno['dadosadicionais'], "É necessário informar a 'Justificativa do Projeto'.");
				
			/*if(!isset($ptrtipodirigente) || !isset($entiddirigente)) 
			   array_push($this->retorno['dadosadicionais'], "É necessário informar o 'Dirigente/Representante Legal'.");*/
			   
			if( (!isset($bcoid) || !isset($ptragenciabancaria)) && $_SESSION['emenda']['federal'] == 'N' )
			 	array_push($this->retorno['dadosadicionais'], "É necessário informar um 'Banco' e uma 'Agência' para abertura de conta.");
		}
	}
	
	private function validaAnexos() {
		$this->retorno['anexo'] = array();
		
		$sql = "SELECT 
					count(anx.anxid)
				FROM 
					emenda.anexo anx
					inner join public.arquivo arq on anx.arqid = arq.arqid
				WHERE 
					ptrid = ".$this->ptrid."
				    and anx.anxtipo = 'P' 
				    and anx.anxtermoref is not null";
		
		$valida = $this->db->pegaUm($sql);
		
		if($valida == 0) {
			array_push($this->retorno['anexo'], "Deve ser cadastrado pelo menos um anexo de \"Termo de Referência\" ou \"Projeto Básico\".");
		}
	}
	
	private function validaAnexosGuia() {
		$this->retorno['anexoguia'] = array();
		
		$sql = "SELECT 
					ggd.gdoid,
				    anx.arqid
				FROM
					emenda.guia_planotrabalho gp
				    inner join emenda.guia g on g.guiid = gp.guiid
				    inner join emenda.guia_guiainiciativa ggi on ggi.guiid = g.guiid
				    inner join emenda.guia_guiadocumento ggd on ggd.ginid = ggi.ginid
				    left join emenda.anexo anx on anx.gdoid = ggd.gdoid and anx.gdoid is not null
				WHERE
					gp.ptrid =".$this->ptrid;
		
		$arrAnexos = $this->db->carregar($sql);
		
		if($arrAnexos) {
			$boValida = true;
			foreach ($arrAnexos as $anexos) {
				if( empty($anexos['arqid']) ){
					$boValida = false;
				}
			}
			
			if( !$boValida ) array_push($this->retorno['anexoguia'], "Deve ser cadastrado pelo menos um anexo da Guia.");
		}
	}
	
	private function validaEscolasBeneficiadas() {
		$this->retorno['escolasbeneficiadas'] = array();
		
		$exigeCadastroEscolaBeneficiada = false;
		
		if(!empty($this->iniciativas)) {
			for($i=0; $i<count($this->iniciativas); $i++) {
				if($this->iniciativas[$i]['iniescolasbeneficiasdas'] == 't') {
					$exigeCadastroEscolaBeneficiada = true;
				}
			}
		}
		
		if($exigeCadastroEscolaBeneficiada) {
			$sql = "SELECT
						count(*)
					FROM
						emenda.ptescolasbeneficiadas
					WHERE
						ptrid = ".$this->ptrid;
			
			$valida = $this->db->pegaUm($sql);
			
			if($valida == 0) array_push($this->retorno['escolasbeneficiadas'], "Deve ser cadastrada pelo menos uma Escola Beneficiada.");
		}
	}
	
	private function validaCronograma() {
		$this->retorno['cronograma'] = array();
		
		if($this->validaParcelasConcedente()) {
			$this->validaSomaParcelasIniciativaConcedente();
		}
		if( $_SESSION['emenda']['federal'] == 'N' ){
			if($this->validaParcelasProponente()) {
				$this->validaSomaParcelasIniciativaProponente();
			}
		}
	}
	
	private function validaParcelasConcedente() {
		$sql = "SELECT
					count(*) as parcelas_concedente
				FROM
					emenda.ptparceladesembolso
				WHERE
					ptrid = ".$this->ptrid." and
					prdtipo = 'C' and
					prdminuta = 'P'";
		$valida = $this->db->pegaUm($sql);
		
		if($valida == 0) { 
			array_push($this->retorno['cronograma'], "É necessário informar as parcelas de desembolso do concedente.");
			return false;
		} else {
			return true;
		}
	}
	
	private function validaParcelasProponente() {
		$sql = "SELECT
					count(*) as parcelas_proponente
				FROM
					emenda.ptparceladesembolso
				WHERE
					ptrid = ".$this->ptrid." and
					prdtipo = 'P' and
					prdminuta = 'P'";
		$valida = $this->db->pegaUm($sql);
		
		if($valida == 0) {
			array_push($this->retorno['cronograma'], "É necessário informar as parcelas de desembolso do proponente.");
			return false;
		} else {
			return true;
		}
	}
	
	private function validaSomaParcelasIniciativaConcedente() {
		if(!empty($this->iniciativas)) {
			
			$existePendencia = true;
			
			for($i=0; $i<count($this->iniciativas); $i++) {
				$sql = "SELECT
							sum(pri.privalor) as privalor
						FROM
							emenda.ptparcelainiciativa pri
						INNER JOIN
							emenda.ptparceladesembolso prd ON prd.prdid = pri.prdid
														  AND prd.prdtipo = 'C'
														  AND prdminuta = 'P'
						WHERE
							pri.ptiid = ".$this->iniciativas[$i]["ptiid"];

				$concedente = $this->db->pegaUm($sql);
				
				if($concedente != $this->iniciativas[$i]['ptivalorconcedente']) {
					if($existePendencia) {
						$existePendencia = false;
						array_push($this->retorno['cronograma'], "Pendências encontradas nas parcelas do concedente:");
					}
					
					array_push($this->retorno['cronograma'], "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;O total das parcelas da iniciativa '".$this->iniciativas[$i]["ininome"]."' deve ser igual ao valor total da iniciativa.");
				}
			}
		}
	}
	
	private function validaSomaParcelasIniciativaProponente() {
		if(!empty($this->iniciativas)) {
			
			$existePendencia = true;
			
			for($i=0; $i<count($this->iniciativas); $i++) {
				$sql = "SELECT
							sum(pri.privalor) as privalor
						FROM
							emenda.ptparcelainiciativa pri
						INNER JOIN
							emenda.ptparceladesembolso prd ON prd.prdid = pri.prdid
														  AND prd.prdtipo = 'P'
														  AND prdminuta = 'P'
						WHERE
							pri.ptiid = ".$this->iniciativas[$i]["ptiid"];
				
				$proponente = $this->db->pegaUm($sql);
				
				if($proponente != $this->iniciativas[$i]['ptivalorproponente']) {
					if($existePendencia) {
						$existePendencia = false;
						array_push($this->retorno['cronograma'], "Pendências encontradas nas parcelas do proponente:");
					}
					
					array_push($this->retorno['cronograma'], "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;O total das parcelas da iniciativa '".$this->iniciativas[$i]["ininome"]."' deve ser igual ao valor total da iniciativa.");
				}
			}
		}
	}
	
	private function validaIniciativas() {
		$this->retorno['iniciativas'] = array();
		
		if($this->validaExistenciaIniciativas() && $this->validaEspecificacaoAtiva()) {
			
			if( $_SESSION['emenda']['federal'] == 'N' ){
				$this->validaBeneficiariosObrigatorios();
				//$this->validaSomaValorProponente();
			}
			$this->validaCamposObrigatoriosEspecificacoes();
			$this->validaEspecificacoesCadastroKit();
			$this->validaSomaValoresEspecificacaoKit();
			$this->validaSomaRecursosValorConcedente();
			$this->validaSomaRecursosEmendaDetalheEntidade();
			
		}
	}
	
	private function validaExistenciaIniciativas() {
		if(empty($this->iniciativas)) {
			array_push($this->retorno['iniciativas'], "É necessário cadastrar as iniciativas e as especificações do Plano de Trabalho.");
			return false;
		} else {
			return true;
		}
	}
	
	private function validaEspecificacaoAtiva() {
		$sql = "SELECT
					ini.ininome
				FROM
					emenda.ptiniciativa pti
				INNER JOIN emenda.iniciativa ini
					on pti.iniid = ini.iniid
				LEFT JOIN	
					emenda.ptiniciativaespecificacao pte ON pte.ptiid = pti.ptiid
														AND pte.ptestatus = 'A'
				WHERE
					pte.pteid is null
					AND pti.ptrid = ".$this->ptrid;
		
		$valida = $this->db->carregar($sql);
		
		$retorno = true;
		
		if($valida) {
			$retorno = false;
			for($i=0; $i<count($valida); $i++) {
				array_push($this->retorno['iniciativas'], "É necessário informar pelo menos uma especificação da iniciativa '".$valida[$i]['ininome']."'. Para informá-la, vá até a aba 'Iniciativas', selecione a iniciativa '".$valida[$i]['ininome']."' e clique na aba 'Especificações'.");
			}
		}
		
		return $retorno;
	}
	
	private function validaBeneficiariosObrigatorios() {
		$sql = "SELECT 
					ben.bennome,
					ptb.ptbquantidaderural,
					ptb.ptbquantidadeurbana,
					ini.ininome
				FROM 
					emenda.ptiniciativa pti
				INNER JOIN
					emenda.iniciativa ini ON ini.iniid = pti.iniid
				INNER JOIN 
					emenda.iniciativabeneficiario icb ON pti.iniid = icb.iniid
													 AND icb.icbobrigatorio = true
													 and icb.icbstatus = 'A'
				INNER JOIN 
					emenda.beneficiario ben ON ben.benid = icb.benid
				LEFT JOIN 
					emenda.ptiniciativabeneficiario ptb ON ptb.icbid = icb.icbid
					and ptb.ptiid = pti.ptiid
				WHERE
					pti.ptrid = ".$this->ptrid;
		
		$valida = $this->db->carregar($sql);
		
		if($valida) {
			for($i=0; $i<count($valida); $i++) {
				$vInformado = false;
				
				if( ($valida[$i]['ptbquantidaderural'] != NULL && $valida[$i]['ptbquantidaderural'] > 0) ||
				    ($valida[$i]['ptbquantidadeurbana'] != NULL && $valida[$i]['ptbquantidadeurbana'] > 0) ){
					$vInformado = true;
				}
					
				if(!$vInformado) array_push($this->retorno['iniciativas'], "É necessário informar a quantidade de '".$valida[$i]['bennome']."' beneficiados(as) da iniciativa '".$valida[$i]['ininome']."'. Para informá-la, vá até a aba 'Iniciativas', selecione a iniciativa '".$valida[$i]['ininome']."' e clique na aba 'Beneficiários'.");
			}
		}
	}
	
	private function validaCamposObrigatoriosEspecificacoes() {
		$sql = "SELECT
					pte.ptequantidade,
					pte.ptevalorunitario,
					pte.ptedatainicio,
					pte.ptedatafim,
					esp.espnome,
					ini.ininome
				FROM
					emenda.ptiniciativa pti
					INNER JOIN emenda.iniciativa ini ON ini.iniid = pti.iniid
					INNER JOIN emenda.ptiniciativaespecificacao pte ON pte.ptiid = pti.ptiid AND pte.ptestatus = 'A'
					INNER JOIN emenda.iniciativaespecificacao ice ON ice.iceid = pte.iceid
					INNER JOIN emenda.especificacao esp ON esp.espid = ice.espid
					inner join emenda.especificacao_programacaoexercicio epe on epe.espid = esp.espid and epe.prsano = '".$_SESSION['exercicio']."'
				WHERE
					pti.ptrid = ".$this->ptrid."
				ORDER BY 
					pti.ptiid";
		
		$valida = $this->db->carregar($sql);
		
		if($valida) {
			$texto = "";
			$arrCampos = array();
			
			for($i=0; $i<count($valida); $i++) {
				if(!$valida[$i]['ptequantidade'] || $valida[$i]['ptequantidade'] == 0 || !$valida[$i]['ptevalorunitario'] || 
				   $valida[$i]['ptevalorunitario'] == 0 || !$valida[$i]['ptedatainicio'] || !$valida[$i]['ptedatafim'])
				{
					$texto .= "Na iniciativa '".$valida[$i]['ininome']."', especificação '".$valida[$i]['espnome']."', é necessário informar os seguintes campos: ";
					
					if(!$valida[$i]['ptequantidade'] || $valida[$i]['ptequantidade'] == 0)
						array_push($arrCampos, "'Quantidade'");
					
					if(!$valida[$i]['ptevalorunitario'] || $valida[$i]['ptevalorunitario'] == 0)
						array_push($arrCampos, "'Valor Unitário'");
						
					if(!$valida[$i]['ptedatainicio'])
						array_push($arrCampos, "'Data de Início'");
						
					if(!$valida[$i]['ptedatafim'])
						array_push($arrCampos, "'Data Fim'");
						
					$texto .= implode(',', $arrCampos) . ".";
					array_push($this->retorno['iniciativas'], $texto);
				}
			}
		}
	}
	
	private function validaEspecificacoesCadastroKit() {
		$sql = "SELECT
					esp.espnome,
					ini.ininome
				FROM
					emenda.ptiniciativa pti
					INNER JOIN emenda.iniciativa ini ON ini.iniid = pti.iniid
					INNER JOIN emenda.ptiniciativaespecificacao pte ON pte.ptiid = pti.ptiid AND pte.ptestatus = 'A'
					INNER JOIN emenda.iniciativaespecificacao ice ON ice.iceid = pte.iceid
					INNER JOIN emenda.especificacao esp ON esp.espid = ice.espid AND esp.espkit = true
					inner join emenda.especificacao_programacaoexercicio epe on epe.espid = esp.espid and epe.prsano = '".$_SESSION['exercicio']."'
					LEFT JOIN emenda.ptitemkit ptk ON ptk.pteid = pte.pteid
				WHERE
					ptk.ptkid is null
					AND pti.ptrid = ".$this->ptrid."
				ORDER BY pti.ptiid";

		$valida = $this->db->carregar($sql);
		
		if($valida) {
			for($i=0; $i<count($valida); $i++) {
				array_push($this->retorno['iniciativas'], "Na iniciativa '".$valida[$i]["ininome"]."', especificação '".$valida[$i]["espnome"]."', é necessário informar o itens que a compõem.");
			}
		}
	}
	
	private function validaSomaValoresEspecificacaoKit() {
		$sql = "SELECT
					sum(vptk.ptkvalortotal) as soma_kit,
					pte.pteid,
					pte.ptequantidade,
					pte.ptevalorunitario,
					esp.espnome,
					ini.ininome
				FROM
					emenda.ptiniciativa pti
					INNER JOIN emenda.iniciativa ini ON ini.iniid = pti.iniid
					INNER JOIN emenda.ptiniciativaespecificacao pte ON pte.ptiid = pti.ptiid AND pte.ptestatus = 'A'
					INNER JOIN emenda.iniciativaespecificacao ice ON ice.iceid = pte.iceid
					INNER JOIN emenda.especificacao esp ON esp.espid = ice.espid AND esp.espkit = true
					inner join emenda.especificacao_programacaoexercicio epe on epe.espid = esp.espid and epe.prsano = '".$_SESSION['exercicio']."'
					INNER JOIN emenda.v_ptitemkit vptk ON vptk.pteid = pte.pteid
				WHERE
					pti.ptrid = ".$this->ptrid."
				GROUP BY
					pte.pteid, pte.ptequantidade, pte.ptevalorunitario, esp.espnome,ini.ininome";
		
		$valida = $this->db->carregar($sql);
				
		if($valida) {
			for($i=0; $i<count($valida); $i++) {
				if($valida[$i]["ptequantidade"] != 1)
					array_push($this->retorno['iniciativas'], "Na iniciativa '".$valida[$i]["ininome"]."', especificação '".$valida[$i]["espnome"]."', o campo 'quantidade' deve ser igual a 1");
					
				if($valida[$i]["soma_kit"] != $valida[$i]["ptevalorunitario"])
					array_push($this->retorno['iniciativas'], "Na iniciativa '".$valida[$i]["ininome"]."', especificação '".$valida[$i]["espnome"]."', o campo 'valor unitário' deve ser igual ao valor total dos itens que compõem a especificação.");
			}
		}
	}
	
	private function validaSomaValorProponente() {
		$sql = "SELECT
					sum(pte.ptevalorproponente) as soma_proponente
				FROM
					emenda.planotrabalho ptr
				INNER JOIN
					emenda.ptiniciativa pti ON pti.ptrid = ptr.ptrid
				INNER JOIN
					emenda.ptiniciativaespecificacao pte ON pte.ptiid = pti.ptiid AND pte.ptestatus = 'A'
				WHERE
					ptr.ptrid = ".$this->ptrid;
		
		$somaProponente = $this->db->pegaUm($sql);
		
		if($somaProponente != $this->ptrvalorproponente)
			array_push($this->retorno['iniciativas'], "O valor total do proponente informado nas especificações (R$ ".number_format($somaProponente,2,",",".").") deve ser igual ao valor do proponente definido para o Plano de Trabalho (R$ ".number_format($this->ptrvalorproponente,2,",",".").")");
	}

	private function validaSomaRecursosValorConcedente() {
		$sql = "SELECT
					sum(COALESCE(per.pervalor,0)) as pervalorespecificacao,
					pte.pteid,
					ptevalorconcedente,
					espnome,
					ini.ininome
				FROM
					emenda.v_ptiniciativaespecificacao pte
					INNER JOIN emenda.iniciativaespecificacao ice ON ice.iceid = pte.iceid
					INNER JOIN emenda.especificacao esp ON esp.espid = ice.espid
					inner join emenda.especificacao_programacaoexercicio epe on epe.espid = esp.espid and epe.prsano = '".$_SESSION['exercicio']."'
					INNER JOIN emenda.ptiniciativa pti ON pti.ptiid = pte.ptiid
					INNER JOIN emenda.iniciativa ini ON ini.iniid = pti.iniid
					INNER JOIN emenda.planotrabalho ptr ON pti.ptrid = ptr.ptrid
	 				LEFT JOIN emenda.ptiniesprecurso per ON pte.pteid = per.pteid
				WHERE
					pte.ptestatus = 'A' AND
					ptr.ptrid = ".$this->ptrid."
				GROUP BY
					pte.pteid, pte.ptevalorconcedente, espnome, ini.ininome";

		$valida = $this->db->carregar($sql);
		
		if($valida) {
			for($i=0; $i<count($valida); $i++) {
				if($valida[$i]["pervalorespecificacao"] != $valida[$i]["ptevalorconcedente"]) {
					array_push($this->retorno['iniciativas'], "Na iniciativa '".$valida[$i]["ininome"]."', especificação '".$valida[$i]["espnome"]."', o valor debitado das emendas deve ser igual ao valor do concedente da especificação");
				}
			}
		}
	}
		
	private function validaSomaRecursosEmendaDetalheEntidade() {
		$sql = "SELECT
					sum(COALESCE(per.pervalor,0)) as pervaloremenda,
					ped.pedid,
					--ped.pedvalor,
					(select sum(pedvalor) from emenda.ptemendadetalheentidade where ptrid = ".$this->ptrid." ) as pedvalor,
					ef.fupdsc
				FROM
					emenda.ptemendadetalheentidade ped
				INNER JOIN
					emenda.emendadetalheentidade ede ON ede.edeid = ped.edeid
				INNER JOIN
					emenda.emendadetalhe emd ON emd.emdid = ede.emdid
				INNER JOIN
					emenda.emenda eme ON eme.emeid = emd.emeid
				INNER JOIN
					emenda.v_funcionalprogramatica ef ON ef.acaid = eme.acaid 
				LEFT JOIN
					emenda.ptiniesprecurso per ON ped.pedid = per.pedid
				WHERE
					ped.ptrid = ".$this->ptrid."
					--AND ped.ptrid NOT IN (SELECT tt.ptridpai FROM emenda.planotrabalho tt WHERE tt.ptridpai = ped.ptrid and tt.ptrstatus = 'A')
					and per.pervalor is not null
					and per.pervalor <> 0
				GROUP BY
					ped.pedid,
					ped.pedvalor,ef.fupdsc";
		
		$valida = $this->db->carregar($sql);
		
		if($valida) {
			for($i=0; $i<count($valida); $i++) {
				if($valida[$i]["pervaloremenda"] > $valida[$i]["pedvalor"]) {
					array_push($this->retorno['iniciativas'], "O valor utilizado da emenda '".$valida[$i]["fupdsc"]."' no Plano de Trabalho não pode ser superior ao valor da emenda.");
				}
			}
		}
	}
	
	private function validaEnderecoEntidade(){
		$this->retorno['entidade'] = array();
		
		$sql = "SELECT
				    count(ptr.ptrid) as total
				FROM
					emenda.entidadebeneficiada enb
				    INNER JOIN emenda.planotrabalho ptr
				    	ON ptr.enbid = enb.enbid  
				WHERE
					enb.muncod IS NOT NULL AND 
					enb.estuf IS NOT NULL AND 
					ptr.ptrid = ".$this->ptrid;	
		
		$valida = $this->db->pegaUm($sql);
			
		if($valida == 0) array_push($this->retorno['entidade'], "Deve ser cadastrado o endereço da entidade.");
	}
		
	private function validaTipoImpositivo(){
		$this->retorno['impositivo'] = array();
		
		if( $_SESSION['exercicio'] > 2014 )	$filtroImp = ' and e.edeid = v.edeid';
		
		$sql = "select 
					v.emecod,
				    count(e.ediid) as impositivo 
				from 
					emenda.ptemendadetalheentidade p
				    inner join emenda.v_emendadetalheentidade v on v.edeid = p.edeid
				    left join emenda.emendadetalheimpositivo e on e.emdid = v.emdid and e.edistatus = 'A' $filtroImp
				where
					p.ptrid = {$this->ptrid}
				    and v.edestatus = 'A'
				group by
					v.emecod";	
		
		$valida = $this->db->pegaLinha($sql);
			
		if($valida['impositivo'] == 0) array_push($this->retorno['impositivo'], "Deve ser informado se exsite algum tipo de impedimento para a realização do plano de trabalho.");
	}	
}

?>