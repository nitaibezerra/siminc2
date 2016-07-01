<?php
class Empenho {
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "execucaofinanceira.empenho";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "empid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'empid' => null, 
									  	'proid' => null, 
									  	'empnumero' => null,
    									'empano' => null,
    									'emppi' => null,
    									'empptres' => null,
    									'empnumeroempenhosigef' => null,
    									'empfonterecurso' => null,
    									'empnaturezadespesa' => null,
    									'empcentrogestaosolic' => null,
    									'empnumeroconvenio' => null,
    									'empanoconvenio' => null,
    									'empgestaoeminente' => null,
    									'empunidadegestoraeminente' => null,
    									'empprogramafnde' => null,
    									'empsituacao' => null,
    									'usucpf' => null,
    									'empprotocolo' => null
									  );
	
	private $obProcesso; 
									  
	function __construct( $obProcesso ) {
		$this->obProcesso = $obProcesso;
	}
	
	public function carregarListaEmpenhosRealizados(){
		global $db;
		
		switch ($this->obProcesso->tipid) {
			case SUBACOES_GENERICO_PAR:
				$titulo = "Dados para pagamentos";
				$obSubacao = new SubacaoGenericoPar( $this->obProcesso );
				$sql = $obSubacao->sqlListaEmpenhosRealizados();
			break;
			case OBRAS_PAR:
				$titulo = "Dados para pagamentos";
				$obObrasPAR = new ObrasPar( $this->obProcesso );
				$sql = $obObrasPAR->sqlListaEmpenhosRealizados();
			break;
			case OBRAS_PAC:
				$titulo = "Dados para pagamentos";
				$obObrasPAC = new ObrasPac( $this->obProcesso );
				$sql = $obObrasPAC->sqlListaEmpenhosRealizados();
			break;
		}
		$cabecalho = array("&nbsp;", "Nº do Empenho", "Ano Empenho", "Valor Empenhado em Relação ao Projeto", "(%)Porcentagem Empenhada em Relação ao Projeto", "Valor a Pago", 'Ações' );
		
		echo '<table class="tabela" id="listaEmpenho" align="center" style="width: 100%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
				<thead>
					<tr>
						<td valing="top" align="center"><b>Legenda:</b>
									<img src="../imagens/exclusao.gif" border="0"> Solicitar Empenho Parcial &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src="../imagens/valida3.gif" border="0"> Cancelar Empenho &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src="../imagens/money.gif" border="0"> Solicitar Pagamento</td>
					</tr>
					<tr>
						<th>'.$titulo.'</th>
					</tr>
				</thead>
				<tbody>
					<tr>
						<td><div style="overflow-x: auto; overflow-y: auto; width:100%; height:250px;">';
		
							$db->monta_lista_simples($sql, $cabecalho, 500, 5, 'S', '99.5%', 'S');
		echo '			</div></td>
				</tr>
			</tbody>
		</table>
		';	
	}
	
	public function solicitarEmpenhos($post, Processo $obProcesso){
		// Mudar o TCPID
		$retorno = $this->executarEmpenho($post, $obProcesso);
		if($obProcesso->tipid == SUBACOES_GENERICO_PAR){
		}else if($obProcesso->tipoExecucao == 2){
		
		}
		/*$cabecalho = array("&nbsp;", "Nº do Empenho", "Ano", "Valor Empenhado", "% Empenhado", "Valor a Pago", 'Ações' );
				
		echo '<table class="tabela" id="listaEmpenho" align="center" style="width: 100%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
				<thead>
					<tr>
						<th>'.$titulo.'</th>
					</tr>
				</thead>
				<tbody>
					<tr>
						<td><div style="overflow-x: auto; overflow-y: auto; width:100%; height:250px;">';
		echo $retorno;
							//$this->monta_lista_simples($sql, $cabecalho, 500, 5, 'N', '99.5%', 'S');
		echo '			</div></td>
				</tr>
			</tbody>
		</table>
		';	*/
	}
	
	public function listaDadosPassiveisDeEmpenho(){
		global $db;
		
		switch ($this->obProcesso->tipid) {
			case SUBACOES_GENERICO_PAR:
				$obSubGenPAR = new SubacaoGenericoPar($this->obProcesso);
				$titulo = "Lista de Subações a serem Empenhadas";
				$cabecalho = array("Selecione", "Nº do Empenho", "Subação", "Ano", "Valor da Subação", "PI", "PTRES", "% Empenhado", "Valor Empenhado", "% a Empenhar", "Valor a Empenhar" );
				$sql = $obSubGenPAR->sqlListaAEmpenhar();
			break;
			case OBRAS_PAR:
				$obObrasPar = new ObrasPar($this->obProcesso);
				$titulo = "Lista de Obras PAR a serem Empenhadas";
				$cabecalho = array("Selecione", "Nº do Empenho", "Subação", "Ano", "Valor da Subação", "PI", "PTRES", "% Empenhado", "Valor Empenhado", "% a Empenhar", "Valor a Empenhar" );
				$sql = $obObrasPar->sqlListaAEmpenhar();
			break;
			case OBRAS_PAC:
				$obObrasPac = new ObrasPac($this->obProcesso);
				$titulo = "Lista de Obras PAR a serem Empenhadas";
				$cabecalho = array("Selecione", "Nº do Empenho", "Subação", "Ano", "Valor da Subação", "PI", "PTRES", "% Empenhado", "Valor Empenhado", "% a Empenhar", "Valor a Empenhar" );
				$sql = $obObrasPac->sqlListaAEmpenhar();
			break;
			case EMENDAS_PARLAMENTARES:
				$obEmenda = new EmendaParlamentar($this->obProcesso);
				$titulo = "Lista de Emendas a serem Empenhadas";
				$cabecalho = array("Selecione", "Nº do PTA", "Valor do PTA", "Ano da Emenda", "Nº da Emenda", "Valor da Emenda", "% Empenhado", "Valor Empenhado", "% a Empenhar", "Valor a Empenhar" );
				$sql = $obEmenda->sqlListaAEmpenhar( );
			break;
		}
		
		$arrDados = $db->carregar($sql);
		$arrDados = $arrDados ? $arrDados : array();
		
		$html .='<table class="tabela" id="dadosASeremEmpenhados" align="center" style="width: 100%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
				<thead>
					<tr>
						<th>'.$titulo.'</th>
					</tr>
				</thead>
				<tbody>
					<tr>
						<td valign=top>
							<table width="99.5%" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">
								<thead>
								<tr>';
							if( $arrDados ){
								foreach ($cabecalho as $label) {
									$html.= '<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">'.$label.'</td>';
								}
								$html.= '</tr></thead><tbody>';
								
								$totalEmpenho 	= 0;
								$totalTotal 	= 0;
								$totalPercEmp	= 0;
								foreach ($arrDados as $key => $v) {
									$key % 2 ? $cor = "#dedfde" : $cor = "";
									$totalEmpenho	+= (float)$v['valorempenhado'];
									$totalTotal		+= (float)$v['valortotal'];
									$totalPercEmp	+= (float)$v['percentualempenhado'];
									$cont = 0;
									$html.= '<tr bgcolor="'.$cor.'" id="tr_'.$key.'" onmouseout="this.bgColor=\''.$cor.'\';" onmouseover="this.bgColor=\'#ffffcc\';">';								
									$html.= '<td valign="top" title="'.$cabecalho[$cont++].'"><input type=checkbox name=chk[] onclick=marcarChk(this); id=chk_'.$v['codigo'].' value='.$v['codigo'].'></td>
											<td valign="top" title="'.$cabecalho[$cont++].'">'.$v['numeroempenho'].'</td>
											<td valign="top" title="'.$cabecalho[$cont++].'">'.$v['descricao'].'</td>
											<td valign="top" title="'.$cabecalho[$cont++].'">'.$v['ano'].'</td>
											<td align="right" valign="top" style="color:#999999;" title="'.$cabecalho[$cont++].'">'.number_format($v['valortotal'], 2, ',', '.').'<br></td>
											<td valign="top" title="'.$cabecalho[$cont++].'"><input type=hidden id=pi_'.$v['codigo'].' name=pi_'.$v['codigo'].' value='.$v['planointerno'].'>'.$v['planointerno'].'</td>
											<td valign="top" title="'.$cabecalho[$cont++].'"><input type=hidden id=ptres_'.$v['codigo'].' name=ptres_'.$v['codigo'].' value='.$v['ptres'].'>'.$v['ptres'].'</td>
											<td align="right" valign="top" style="color:#999999;" title="'.$cabecalho[$cont++].'">'.$v['percentualempenhado'].'</td>
											<td align="right" valign="top" style="color:#999999;" title="'.$cabecalho[$cont++].'">'.number_format($v['valorempenhado'], 2, ',', '.').'</td>
											<td valign="top" title="'.$cabecalho[$cont++].'">
												<input type="text" id="id_'.$v['codigo'].'" value="'.$v['porcentagem'].'" name="name_'.$v['codigo'].'" size="6" onkeyup="calculaEmpenho(this);" onblur="verificaPreenchimentoPorcentagem(this)" class="normal" onfocus="this.select();">
						    					<input type="hidden" id="vlr_'.$v['codigo'].'" name="vlr_'.$v['codigo'].'" value="'.$v['porcentagem'].'">
						    					<input type="hidden" id="ano_'.$v['codigo'].'" name="ano_'.$v['codigo'].'" value="'.$v['ano'].'">
						    					<input type=hidden id=vlrTotal_'.$v['codigo'].' name=vlrTotal_'.$v['codigo'].' value='.$v['valoraempenhartotal'].'>
						    					</td>
						    				<td valign="top" title="'.$cabecalho[$cont++].'">
						    					<input type="text" style="text-align:right;" onkeyup="calculaEmpenho(this);" class="normal" id="id_vlr_'.$v['codigo'].'" value="'.number_format(trim($v['valoraempenhar']), 2, ',', '.').'" name="name_vlr_'.$v['codigo'].'&lt;/td"></td>
						    			</tr>';
								}
								
								$html.= '</tbody>
						    			<tfoot>
						    			<tr>
						    				<td align="right">Totais:   </td>
						    				<td align="right" colspan="3"></td>
						    				<td align="right">'.number_format($totalTotal, 2, ',', '.').'</td>
						    				<td align="right" colspan="3">'.$totalPercEmp.'</td>
						    				<td align="right">'.number_format($totalEmpenho, 2, ',', '.').'</td>
						    				<td align="right"></td>
						    				<td align="right"></td>
						    			</tr>
						    			</tfoot>
						    		</table>';
							} else {
								$html .='<table width="99.5%" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">
										<tbody>
											<tr>
												<td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td>
											</tr>
										</tbody>
										</table>';
							}
							//$obProcesso->monta_lista_simples($sql,$cabecalho,50000,5,'S','99.5%','S');
		$html.= '			</td>
				</tr>
			</tbody>
		</table>
		';
		echo $html;
		return true;
	}

	public function listaDados100ProcentoEmpenhadas($obProcesso){
		
		if($obProcesso->tipid == SUBACOES_GENERICO_PAR){
			$titulo = "Lista de Subações 100% empenhadas do tipo Termo e 99% do tipo Convênio";
			$sql = $this->sqlListaDeSubacoes100Empenhado($obProcesso);
		}else if($obProcesso->tipoExecucao == 2){
		
		}
			
		echo '<table class="tabela" id="dadosASeremEmpenhados" align="center" style="width: 100%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
				<thead>
					<tr>
						<td class="subtituloCentro">'.$titulo.'</td>
					</tr>
				</thead>
				<tbody>
					<tr>
						<td>';
				$obProcesso->monta_lista_simples($sql, $cabecalho, 500, 5, 'S', '100%', 'S');
		echo '			</td>
				</tr>
			</tbody>
		</table>';	
		return true;
	}
	
	public function mostraInformacoesEmpenho($empid){
		global $db;
		
		switch ($this->obProcesso->tipid) {
			case SUBACOES_GENERICO_PAR:
				$titulo = "Detalhe do Empenho";
				$obSubacao = new SubacaoGenericoPar( $this->obProcesso );
				$sql = $obSubacao->sqlInformacoesEmpenho($empid);
			break;
		}
		
		$cabecalho = array("Ações", "Subação", "Valor da Subação", "% Empenhado", "Valor Empenhado", "Ano", "Situação" );
			
		echo '<table style="width: 100%" align="left" cellspacing="1" cellpadding="3">
			<tr>
				<td align="right" width="03%" valign="top"><img style="cursor:pointer" src="/imagens/seta_filho.gif" border="0"></td>
				<td>
					<table class="tabela" style="width: 100%" id="cabecalhoempenho" align="left" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
						<thead>
							<tr>
								<td class="subtituloCentro" colspan="10">'.$titulo.'</td>
							</tr>
						</thead>
						<tbody>
							<tr>
								<td>'.$db->monta_lista_simples_html($sql, $cabecalho, 500, 5, 'N', '100%', 'S').'</td>
							</tr>
						</tbody>
					</table>
				</td>
			</tr>
		</table>';	
		//return true;
	}
	
	public function executarEmpenho($dados, Processo $obProcesso){

		$tipoExecucao     = $obProcesso->tprid;
      	$boPregao         = $obProcesso->tipvaiparasigarp;
      	
      	if($tipoExecucao == CONVENIO){
      		$entidadeHabilitada = $obProcesso->consultaHabilitaEntidade($obProcesso->procnpj);
			
            if((int)$entidadeHabilitada['codigo'] != 3){
            	$obProcesso->mensagem('Consulta Habilita Entidade', $entidadeHabilitada['descricao']);
            	return false;
            }
      	}
      	if( $boPregao == true  ){
      		$ItensdePregaoAtualizado = $this->atualizaValoresSubacao($dados, $obProcesso);
      		if($ItensdePregaoAtualizado == false){
      			return false;
      		}
      	}
      	$obContaCorrente = new ContaCorrente();      	
      	$contaCorrenteAtiva = $obContaCorrente->atualizaDadosContaCorrente( $dados, $obProcesso );
      	if( $contaCorrenteAtiva == false ){
      		$solicitaContaCorrente = $obContaCorrente->solicitarContaCorrente($dados, $obProcesso);
      		if($solicitaContaCorrente == false){
      			return false;
      		}
      	}
      	$res_sp = $obProcesso->vincularProcesso($dados);
      	if($tipoExecucao == CONVÊNIO){
      		$dadosConvenioFNDE = $obProcesso->solicitarConvenio($dados);
      	}
      	$res_se = $this->solicitacaoEmpenho($dados, $obProcesso);
      	
      	if( $boPregao && $ItensdePregaoAtualizado ){
      		$this->enviarItensSigarp($dados, $obProcesso);
      	}
      	
      	if( $obProcesso->tipvaiparamonitoramentodeobras == true ){
      		$this->insereMonitoramentoObras($obProcesso);
      	}
	}
	
	public function solicitacaoEmpenho($dados){
		try {
			$data_created 	= date("c");
			$usuario 		= $dados['wsusuario'];
			$senha   		= $dados['wssenha'];			
		    
		    /*$emendas = $db->carregar("SELECT DISTINCT sbaemenda FROM par.subacao s
		    							inner join par.subacaodetalhe sd on sd.sbaid = s.sbaid
		    						  WHERE sbdid IN (".implode(', ', $dados['chk']).") ");
		    
		    $totalsubEmenda = count($emendas);
	
		    if($totalsubEmenda > 1){
			    if(is_array($emendas) && $emendas[0]){
			    	foreach($emendas as $emenda ){
			    		if($emenda['sbaemenda'] == NULL or $emenda['sbaemenda'] == '' ){
			    			echo $obProcesso->mensagem( 'REGRA DE NEGÓCIO PARA EMPENHO',  'Uma das Subações é de Emenda e outra não. É necessário empenhar separadamente.');			    			 
			    			 return false; 
			    		}
			    	}
			    }
		    }*/
		   
			$sql = "SELECT  
						(((((((pt.funcod::text) || pt.sfucod::text)) || pt.prgcod::text)) || pt.acacod::text)) || pt.loccod::text AS fupfuncionalprogramatica
					FROM monitora.pi_planointerno pi
					inner join monitora.pi_planointernoptres plpt on pi.pliid = plpt.pliid
					inner join monitora.ptres pt on pt.ptrid = plpt.ptrid
					WHERE
						pt.ptres = '".$dados['ptres']."'";
			
			$funcional = $db->pegaUm( $sql );
			
			if($this->obProcesso->proesfera == 'E') {
	        	// CNPJ do Estado
				$nu_cnpj_favorecido 		= $this->obProcesso->procnpj;
				$co_natureza_despesa_solic 	= '44304200';
	
	        }else{
	        	// CNPJ da prefeitura
				$nu_cnpj_favorecido 		= $this->obProcesso->procnpj;
				$co_natureza_despesa_solic 	= '44404200';
	        }
	        
	        $co_natureza_despesa_solic  	= $dados['naturezadespesasolicitacao'];
	        $co_centro_gestao_solic			= $dados['gestaosolicitacao'];
	        $nu_processo					= $this->obProcesso->pronumeroprocesso;
			$co_fonte_recurso_solic			= $dados['fonte'];
			$co_plano_interno_solic			= $dados['planointerno'];
			$co_ptres_solic 				= $dados['ptres'];
			$frpfuncionalprogramatica 		= $funcional;
			$nu_sistema 					= $this->obProcesso->tipnumerosistemasigef;
			$co_programa_fnde				= "CM";
			$an_convenio					= $this->obProcesso->proano;
			$nu_convenio					= $this->obProcesso->pronumeroconveniosigef;
			$co_tipo_empenho				= $this->obProcesso->tiptipoempenhosigef;
			$nu_empenho_original			= null;
			$an_exercicio_original			= null;
			$vl_empenho						= str_replace(array(".",","),array("","."),$dados['name_total']);
			$co_especie_empenho				= "01";
			$co_esfera_orcamentaria_solic	= "1";
			$co_observacao					= "2";
			$co_descricao_empenho			= "0010";
			$co_gestao_emitente				= "15253";
	        // condição tipoobra=5(Quadra) entao programa=CN senao programa=BW
			$co_unidade_gestora_emitente	= "153173";
			$co_unidade_orcamentaria_solic	= "26298";
			$nu_proposta_siconv				= null;
	
	    	$arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
	<header>
		<app>string</app>
		<version>string</version>
		<created>$data_created</created>
	</header>
	<body>
		<auth>
			<usuario>$usuario</usuario>
			<senha>$senha</senha>
		</auth>
		<params>
		<nu_cnpj_favorecido>$nu_cnpj_favorecido</nu_cnpj_favorecido>
		<nu_empenho_original>$nu_empenho_original</nu_empenho_original>
		<an_exercicio_original>$an_exercicio_original</an_exercicio_original>
		<vl_empenho>$vl_empenho</vl_empenho>
		<nu_processo>$nu_processo</nu_processo>
		<co_especie_empenho>$co_especie_empenho</co_especie_empenho>
		<co_plano_interno_solic>$co_plano_interno_solic</co_plano_interno_solic>
		<co_esfera_orcamentaria_solic>$co_esfera_orcamentaria_solic</co_esfera_orcamentaria_solic>
		<co_ptres_solic>$co_ptres_solic</co_ptres_solic>
		<co_fonte_recurso_solic>$co_fonte_recurso_solic</co_fonte_recurso_solic>
		<co_natureza_despesa_solic>$co_natureza_despesa_solic</co_natureza_despesa_solic>
		<co_centro_gestao_solic>$co_centro_gestao_solic</co_centro_gestao_solic>
		<an_convenio>$an_convenio</an_convenio>
		<nu_convenio>$nu_convenio</nu_convenio>
		<co_observacao>$co_observacao</co_observacao>
		<co_tipo_empenho>$co_tipo_empenho</co_tipo_empenho>
		<co_descricao_empenho>$co_descricao_empenho</co_descricao_empenho>
		<co_gestao_emitente>$co_gestao_emitente</co_gestao_emitente>
		<co_programa_fnde>$co_programa_fnde</co_programa_fnde>
		<co_unidade_gestora_emitente>$co_unidade_gestora_emitente</co_unidade_gestora_emitente>
		<co_unidade_orcamentaria_solic>$co_unidade_orcamentaria_solic</co_unidade_orcamentaria_solic>
		<nu_proposta_siconv>$nu_proposta_siconv</nu_proposta_siconv>
		<nu_sistema>$nu_sistema</nu_sistema>
		</params>
	</body>
</request>
XML;

		if( $dados['tipo'] == 'visualiza' ){
			echo '<pre>';		
			echo simec_htmlentities($arqXml);
			exit;
	    }
    	
		if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
			$urlWS = 'http://172.20.200.116/webservices/sigef/integracao/public/index.php/orcamento/ne';
		} else {
			$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/orcamento/ne';
		}

		$xml = Fnde_Webservice_Client::CreateRequest()
				->setURL($urlWS)
				->setParams( array('xml' => $arqXml, 'method' => 'solicitar') )
				->execute();
		
		$xmlRetorno = $xml;
	    $xml = simplexml_load_string( stripslashes($xml));
	
			$result = (integer) $xml->status->result;
			if(!$result) {
				
				$erros = $xml->status->error->message;
				if(count($erros)>0) {
					foreach($erros as $err) {
						$mensagem .= ' '.iconv("UTF-8", "ISO-8859-1", $err->text).' ';
					}
				}
				echo $db->mensagem( 'ERRO AO TENTAR SOLICITAR O EMPENHO',  $mensagem);
				$arrHistorico = array('tipo' => 'solicitarEmpenho - Erro',
									  'proid' => $this->obProcesso->proid,
									  'envio' => $arqXml,
									  'retorno' => $xmlRetorno
									);
				$this->obProcesso->insereHistoricoWS($arrHistorico);
				
			    return false;
			} else {
				$arrHistorico = array('tipo' => 'solicitarEmpenho - Sucesso',
									  'proid' => $this->obProcesso->proid,
									  'envio' => $arqXml,
									  'retorno' => $xmlRetorno
									);
				$this->obProcesso->insereHistoricoWS($arrHistorico);
				
				$sql = "INSERT INTO execucaofinanceira.empenho(proid, empnumero, empnotadeempenho, empano, emppi, empptres, empnumeroempenhosigef, empfonterecurso,
  							empnaturezadespesa, empcentrogestaosolic, empnumeroconvenio, empanoconvenio, empgestaoeminente, empunidadegestoraeminente, empprogramafnde,
  							empsituacao, usucpf, empprotocolo) 
						VALUES ({$this->obProcesso->proid},
								".(($nu_empenho_original)?"'".$nu_empenho_original."'":"NULL").",
								null, --nota
								".(($an_exercicio_original)?"'".$an_exercicio_original."'":"NULL").",
								".(($co_plano_interno_solic)?"'".$co_plano_interno_solic."'":"NULL").",
								".(($co_ptres_solic)?"'".$co_ptres_solic."'":"NULL").",
								null, --empnumeroempenhosigef
								".(($co_fonte_recurso_solic)?"'".$co_fonte_recurso_solic."'":"NULL").",
								".(($co_natureza_despesa_solic)?"'".$co_natureza_despesa_solic."'":"NULL").",
								".(($co_centro_gestao_solic)?"'".$co_centro_gestao_solic."'":"NULL").",
								".(($nu_convenio)?"'".$nu_convenio."'":"NULL").",
								".(($an_convenio)?"'".$an_convenio."'":"NULL").",
								".(($co_gestao_emitente)?"'".$co_gestao_emitente."'":"NULL").",
								".(($co_unidade_gestora_emitente)?"'".$co_unidade_gestora_emitente."'":"NULL").",
								".(($co_programa_fnde)?"'".$co_programa_fnde."'":"NULL").",
								'8 - SOLICITAÇÃO APROVADA',
								'".$_SESSION['usucpf']."',
								'".$xml->body->nu_seq_ne."',
								 ) returning empid";
							
				$empid = $db->pegaUm($sql);
	
				$sql = "INSERT INTO execucaofinanceira.historicoempenho(usucpf, empid, hepdata, hepsituacao)
	    				VALUES ('".$_SESSION['usucpf']."', '".$empid."', NOW(), '8 - SOLICITAÇÃO APROVADA')";	
				$db->executar($sql);
	
				if($dados['chk']) {
					foreach($dados['chk'] as $valor){
						$valorPorSubacao = str_replace(array(".",","),array("","."),number_format($dados['vlrTotal_'.$valor], 2, ',', '.'));
						switch ($this->obProcesso->tipid) {
							case SUBACOES_GENERICO_PAR || SUBACOES_PAR_EMENDAS:
								$sbdid = ($valor ? $valor : 'null');
								$preid = 'null';
								$ptrid = 'null';
							break;
							case OBRAS_PAR || OBRAS_PAC:
								$sbdid = 'null';
								$preid = ($valor ? $valor : 'null');
								$ptrid = 'null';
							break;
							case EMENDAS_PARLAMENTARES:
								$sbdid = 'null';
								$preid = 'null';
								$ptrid = ($valor ? $valor : 'null');
							break;
						}
						
						$sql = "INSERT INTO execucaofinanceira.empenhocomposicao(empid, tipid, sbdid, preid, ptrid, emcpercentualemp, emcvaloremp, emcdata, usucpf) 
								VALUES ($empid, {$this->obProcesso->tipid}, $sbdid, $preid, $ptrid, '{$dados['name_'.$valor]}', '{$valorPorSubacao}', now(), '{$_SESSION['usucpf']}')";
						$db->executar($sql);
					}
				}
				echo $this->mensagem( 'EMPENHO EFETIVADO COM SUCESSO ',  '');				
				$db->commit();
				return true;
			}
	
		} catch (Exception $e){
	
			# Erro 404 página not found
			if($e->getCode() == 404){
				
				$mensagem = 'Erro-Serviço encontra-se temporariamente indisponível.Favor tente mais tarde.';
				echo $this->mensagem( 'ERRO AO EXECUTAR A SOLICITAÇÃO DE EMPENHO',  $mensagem);
			}
			$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
			$erroMSG = str_replace( "'", '"', $erroMSG );
	
			$arrHistorico = array('tipo' => 'solicitarEmpenho - Erro',
								  'proid' => $this->obProcesso->proid,
								  'envio' => $arqXml,
								  'retorno' => $xmlRetorno
								);
			$this->obProcesso->insereHistoricoWS($arrHistorico);
			echo $this->mensagem( 'ERRO AO EXECUTAR A SOLICITAÇÃO DE EMPENHO',  $erroMSG);	
		}
	}
	
	public function atualizaValoresSubacao( $dados, $obProcesso ){
	
			// Verifico os itens da subação
			$sql = "SELECT 
						s.sbaid,
						sd.sbdano,
						sic.icoid,
						pic.idsigarp,
						CASE WHEN sbacronograma = 1 --global
						THEN
							CASE WHEN sic.icovalidatecnico IS NULL -- antigos
							THEN coalesce(sic.icoquantidade,0)
							ELSE -- novos
								CASE WHEN sic.icovalidatecnico = 'S' THEN -- validado (caso não o item não é contado)
									sum(coalesce(sic.icoquantidadetecnico,0))
								END
							END
						ELSE -- escolas
							CASE WHEN (s.frmid = 2) OR ( s.frmid = 4 AND s.ptsid = 42 ) OR ( s.frmid = 12 AND s.ptsid = 46 )
							THEN -- escolas sem itens
								CASE WHEN se.sesvalidatecnico IS NULL -- antigos
									THEN 
										sum(coalesce(se.sesquantidade,0))
									ELSE -- novos
										sum(coalesce(se.sesquantidadetecnico,0))
									END
								ELSE -- escolas com itens
									CASE WHEN sic.icovalidatecnico IS NULL -- antigos
									THEN
										sum(coalesce(ssi.seiqtd,0))
									ELSE -- novos
										CASE WHEN sic.icovalidatecnico = 'S' THEN -- validado (caso não o item não é contado)
											sum(coalesce(ssi.seiqtdtecnico,0))
										END
									END
								END
						END as quantidade,
						sic.icovalor as valor
					FROM
						par.subacao s
					INNER JOIN par.subacaodetalhe 		     sd  ON sd.sbaid = s.sbaid
				--	INNER JOIN par.processopar 	             pr  ON pr.prpid = sd.prpid and pr.prpstatus = 'A'
					LEFT  JOIN par.subacaoitenscomposicao 	 sic ON sic.sbaid = s.sbaid  AND sic.icoano = sd.sbdano
					LEFT  JOIN par.subacaoescolas 		     se  ON se.sbaid = sic.sbaid AND se.sesano = sic.icoano
					LEFT  JOIN par.subescolas_subitenscomposicao ssi ON ssi.sesid = se.sesid AND ssi.icoid = sic.icoid
					INNER JOIN par.propostaitemcomposicao        pic ON pic.picid = sic.picid
					WHERE
						s.sbastatus = 'A' AND sd.sbdid IN ( ".implode(", ", $dados['chk'])." ) AND 
						
						 CASE WHEN sbacronograma = 1 
	                     THEN sic.icovalidatecnico <> 'N' 
	                     ELSE 
	                     	CASE WHEN (s.frmid = 2) OR ( s.frmid = 4 AND s.ptsid = 42 ) OR ( s.frmid = 12 AND s.ptsid = 46 ) THEN
	                        	se.sesvalidatecnico <> 'N'
	                        ELSE
	                        	sic.icovalidatecnico <> 'N' 
	                        END
	                     END
	                group by
						s.sbaid, sd.sbdano, sic.icoid, pic.idsigarp, s.sbacronograma, sic.icovalidatecnico, sic.icoquantidade, s.frmid, s.ptsid, se.sesvalidatecnico, sic.icovalor";
				//		pr.prpid = ".$_SESSION['par_var']['prpid'];
	
			$itens = $obProcesso->carregar( $sql );
			
			$it = array();
			$erro = array();
			
			if( is_array($itens) ){
				foreach( $itens as $item ){
					if( !$item['idsigarp'] == '' ){
						$valorSigarp = $this->diponibilizaPregoesItens($item['idsigarp'],'valor', $obProcesso);
						if( $valorSigarp ){
							$valor = (string) $valorSigarp;
							$valorSimec = number_format($item['valor'],2,',','.');
							$valor = str_ireplace('.', '', $valor);
							$valor = str_ireplace(',', '.', $valor);
							$valorSimec = str_ireplace('.', '', $valorSimec);
							$valorSimec = str_ireplace(',', '.', $valorSimec);
							$it[$item['icoid']]['icoid'] = $item['icoid'];
							$it[$item['icoid']]['idsigarp'] = $item['idsigarp'];
							$it[$item['icoid']]['valor'] = $item['valor'];
							$it[$item['icoid']]['valorSigarp'] = $valor;
							if( $valorSimec != $valor ){
								$erro[] = $item['icoid']; 
							}
						}
					}
				}
			} else {
				$obProcesso->mensagem('Não existem itens aprovados na subação.', 'Necessário analisar a subação!');
				return false;
			}
	
			if( is_array($erro) && $erro[0] ){ // O valor é diferente então atualizo os valores dos itens.
				foreach( $erro as $err ){
					$sqlAtualizaItens .= " UPDATE par.subacaoitenscomposicao SET icovalor = ".$it[$err]['valorSigarp']." WHERE icoid = ".$it[$err]['icoid']."; ";
				}
				$obProcesso->executar( $sqlAtualizaItens );
				$obProcesso->commit();
				
				$obProcesso->mensagem('VALORES DOS ITENS DE COMPOSIÇÃO DA SUBAÇÃO FORAM ATUALIZADOS DE ACORDO COM O PREGÃO.', 'Solicite novamente o empenho!');				
				return false;
			}
			return true;
	
	}
	
	public function diponibilizaPregoesItens($item, $tipo, $obProcesso){
				
		try {
			
			$data_created = date("c");
	
			if( $_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
				$usuario = "MECVICTOR";
				$senha   = "94123776";
			} else {
				$usuario = "MECVICTOR";
				$senha   = "94123776";
			}
			$sql = "SELECT
						CASE WHEN estuf IS NOT NULL 
							THEN estuf 
							ELSE mun_estuf 
						END as estuf 
					FROM
						par.instrumentounidade 
					WHERE 
						inuid = ".$obProcesso->inuid;
			$estuf = $obProcesso->pegaUm( $sql );
	
	$arqXml = <<<XML
<?xml version="1.0" encoding="ISO-8859-1"?>
<request>
    <header>
        <app>SIGARP</app>
        <version>1.0</version>
        <created>$data_created</created>
    </header>
    <body>
        <auth>
            <usuario>$usuario</usuario>
            <senha>$senha</senha>
        </auth>
        <params>
            <nu_seq_item>{$item}</nu_seq_item>
            <uf>{$estuf}</uf>
        </params>
    </body>
</request>
XML;
		
			if( $_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
				$urlWS = 'http://172.20.65.98/webservices/wssigarp/ws/pregao';
				//$urlWS = 'http://www.fnde.gov.br/webservices/wssigarp/ws/pregao';
			} else {
				$urlWS = 'http://www.fnde.gov.br/webservices/wssigarp/ws/pregao';
			}
			$xml = Fnde_Webservice_Client::CreateRequest()
					->setURL($urlWS)
					->setParams( array('xml' => $arqXml, 'method' => 'listar') )
					->execute();
	
			$xmlRetorno = $xml;
			$xml = simplexml_load_string( stripslashes($xml));
			$result = (integer) $xml->status->result;
	
			if(!$result) {
				$arrHistorico = array('tipo' => 'diponibilizaPregoesItens - Erro',
									  'proid' => $obProcesso->proid,
									  'envio' => $arqXml,
									  'retorno' => $xmlRetorno
									);
				$obProcesso->insereHistoricoWS($arrHistorico);
				
				$erros = $xml->status->error->message->text;
				$obProcesso->mensagem('ERRO AO CONSULTAR ITENS DO PREGÃO (SIGARP)', iconv("UTF-8", "ISO-8859-1", $erros));
			} else { //sucesso
				
				$arrHistorico = array('tipo' => 'diponibilizaPregoesItens - Sucesso',
									  'proid' => $obProcesso->proid,
									  'envio' => $arqXml,
									  'retorno' => $xmlRetorno
									);
				$obProcesso->insereHistoricoWS($arrHistorico);
	
				if( $tipo == 'valor' ){
					return $xml->body->VL_ITEM ? $xml->body->VL_ITEM : null;
				} else {
					return (integer) $xml->body->NU_SEQ_PREGAO ? $xml->body->NU_SEQ_PREGAO : false;
				}
			}
			
		} catch (Exception $e){
			
			# Erro 404 página not found
			if($e->getCode() == 404){
				$obProcesso->mensagem('ERRO AO CONSULTAR ITENS DO PREGÃO (SIGARP)', 'Temporariamente indisponível.Favor tente mais tarde.');
				
			}
			
			$arrHistorico = array('tipo' => 'diponibilizaPregoesItens - Erro',
								  'proid' => $obProcesso->proid,
								  'envio' => $arqXml,
								  'retorno' => $xmlRetorno
								);
			$obProcesso->insereHistoricoWS($arrHistorico);
			
			$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
			$erroMSG = str_replace( "'", '"', $erroMSG );
			$obProcesso->mensagem('ERRO AO CONSULTAR ITENS DO PREGÃO (SIGARP)', 'Erro-WS SIGARP: '.$erroMSG.'<br>
								<pre>
								'.print_r($e).'
								</pre>');
		}
	}

	public function enviarItensSigarp($dados, $obProcesso){
		try {
			// Verifico os itens da subação
			$sql = "SELECT 
						s.sbaid,
						sd.sbdano,
						sd.sbdid,
						sic.icoid,
						pic.idsigarp,
						CASE WHEN sbacronograma = 1 --global
						THEN
							CASE WHEN sic.icovalidatecnico IS NULL -- antigos
							THEN coalesce(sic.icoquantidade,0)
							ELSE -- novos
								CASE WHEN sic.icovalidatecnico = 'S' THEN -- validado (caso não o item não é contado)
									sum(coalesce(sic.icoquantidadetecnico,0))
								END
							END
						ELSE -- escolas
							CASE WHEN (s.frmid = 2) OR ( s.frmid = 4 AND s.ptsid = 42 ) OR ( s.frmid = 12 AND s.ptsid = 46 )
							THEN -- escolas sem itens
								CASE WHEN se.sesvalidatecnico IS NULL -- antigos
									THEN 
										sum(coalesce(se.sesquantidade,0))
									ELSE -- novos
										sum(coalesce(se.sesquantidadetecnico,0))
									END
								ELSE -- escolas com itens
									CASE WHEN sic.icovalidatecnico IS NULL -- antigos
									THEN
										sum(coalesce(ssi.seiqtd,0))
									ELSE -- novos
										CASE WHEN sic.icovalidatecnico = 'S' THEN -- validado (caso não o item não é contado)
											sum(coalesce(ssi.seiqtdtecnico,0))
										END
									END
								END
						END as quantidade,
						sic.icovalor as valor
					FROM
						par.subacao s
					INNER JOIN par.subacaodetalhe 		     sd  ON sd.sbaid = s.sbaid
				--	INNER JOIN par.processopar 	             pr  ON pr.prpid = sd.prpid and pr.prpstatus = 'A'
					LEFT  JOIN par.subacaoitenscomposicao 	 sic ON sic.sbaid = s.sbaid  AND sic.icoano = sd.sbdano
					LEFT  JOIN par.subacaoescolas 		     se  ON se.sbaid = sic.sbaid AND se.sesano = sic.icoano
					LEFT  JOIN par.subescolas_subitenscomposicao ssi ON ssi.sesid = se.sesid AND ssi.icoid = sic.icoid
					INNER JOIN par.propostaitemcomposicao        pic ON pic.picid = sic.picid
					WHERE
						s.sbastatus = 'A' AND sd.sbdid IN ( ".implode(", ", $dados['chk'])." ) AND 
						
						 CASE WHEN sbacronograma = 1 
	                     THEN sic.icovalidatecnico <> 'N' 
	                     ELSE 
	                     	CASE WHEN (s.frmid = 2) OR ( s.frmid = 4 AND s.ptsid = 42 ) OR ( s.frmid = 12 AND s.ptsid = 46 ) THEN
	                        	se.sesvalidatecnico <> 'N'
	                        ELSE
	                        	sic.icovalidatecnico <> 'N' 
	                        END
	                     END
	                group by
						s.sbaid, sd.sbdano, sd.sbdid, sic.icoid, pic.idsigarp, s.sbacronograma, sic.icovalidatecnico, sic.icoquantidade, s.frmid, s.ptsid, se.sesvalidatecnico, sic.icovalor";
				//		pr.prpid = ".$_SESSION['par_var']['prpid'];
	
			$itens = $obProcesso->carregar( $sql );
			
			$cnpj = $obProcesso->procnpj;
			
			$data_created = date("c");
			if( $_SESSION['baselogin'] == "simec_desenvolvimento" ){
				$usuario = "GABRIELAS";
				$senha   = "76201626";
			} else {
				$usuario = "MECVICTOR";
				$senha   = "94123776";
			}
			
			$itPerdido = array();
			$it = 0;
			
	$arqXmlSt = 
'<?xml version="1.0" encoding="ISO-8859-1"?>
<request>
    <header>
        <app>SIGARP</app>
        <version>1.0</version>
        <created>'.$data_created.'</created>
    </header>
    <body>
        <auth>
            <usuario>'.$usuario.'</usuario>
            <senha>'.$senha.'</senha>
        </auth>
        <params>
        	<cnpj>'.$cnpj.'</cnpj>
        	<nu_seq_tipo_pagamento>1</nu_seq_tipo_pagamento>';
        	foreach( $itens as $item ){
        		if($item['idsigarp']){
	        		$pregao = $this->diponibilizaPregoesItens($item['idsigarp'],'pregao', $obProcesso);
	        		if( $item['quantidade'] <> 0 ){
		        		if( $pregao ){
		        			$it++;
							$arqXmlSt .= '
								<item>
									<nu_seq_pregao>'.$pregao.'</nu_seq_pregao> 
									<nu_seq_item>'.$item['idsigarp'].'</nu_seq_item> 
									<qt_item>'.$item['quantidade'].'</qt_item>  
									<vl_pagamento>'.$this->diponibilizaPregoesItens($item['idsigarp'],'valor', $obProcesso).'</vl_pagamento>
								</item>';
		        		} else {
		        			$itPerdido[] = array('sbdid' => $item['sbdid'],
												'icoid' => $item['icoid']);
		        		}
	        		}
        		}
        		$pregao = false;
			}
		$arqXmlSt .= '</params>
    </body>
</request>';
		
$arqXml = <<<XML
{$arqXmlSt}
XML;

			if( $it == 0 ){ //Não tem nenhum item para enviar!
				foreach( $itens as $item ){ // Salvo todos os itens que estão sendo empenhados!
					if($item['sbdid'] && $item['icoid'] ){
						$sqlInsertItem .= " INSERT INTO execucaofinanceira.empenhopregaoitem (
									proid,
									sbdid,
									icoid,
									episituacao )
								VALUES
									(".$obProcesso->proid.",
									".$item['sbdid'].",
									".$item['icoid'].",
									'itensNaoEnviado'); ";
					}
				}
				$obProcesso->executar($sqlInsertItem);
				$obProcesso->commit();
				return true;
			}
		
			if( $_SESSION['baselogin'] == "simec_desenvolvimento" ){
				$urlWS = 'http://172.20.65.98/webservices/wssigarp/ws/item';
			} else {
				$urlWS = 'http://www.fnde.gov.br/webservices/wssigarp/ws/item';
			}
			$xml = Fnde_Webservice_Client::CreateRequest()
					->setURL($urlWS)
					->setParams( array('xml' => $arqXml, 'method' => 'solicitar') )
					->execute();
	
			$xmlRetorno = $xml;								
			$xml = simplexml_load_string( stripslashes($xml));	
			$result = (integer) $xml->status->result;
	
			if(!$result) { //ERRO
				$arrHistorico = array('tipo' => 'enviarItensSigarp - Erro',
									  'proid' => $obProcesso->proid,
									  'envio' => $arqXml,
									  'retorno' => $xmlRetorno
									);
				$obProcesso->insereHistoricoWS($arrHistorico);		
				
				$erros = $xml->status->error->message->text;
				
				$obProcesso->mensagem('ERRO AO TENTAR ADERIR AO PREGÃO', $erros);
				
				if( is_array($itens) && $itens[0] ){
					foreach( $itens as $item ){
					if($item['sbdid'] && $item['icoid'] ){
						$sqlInsert .= " INSERT INTO execucaofinanceira.empenhopregaoitem (
									proid,
									sbdid,
									icoid,
									episituacao )
								VALUES
									(".$obProcesso->proid.",
									".$item['sbdid'].",
									".$item['icoid'].",
									'perdido'); ";
						}
					}
					$obProcesso->executar($sqlInsert);
					$obProcesso->commit();
				}
			} else {
				// Deu certo
				$arrHistorico = array('tipo' => 'enviarItensSigarp - Sucesso',
									  'proid' => $obProcesso->proid,
									  'envio' => $arqXml,
									  'retorno' => $xmlRetorno
									);
				$obProcesso->insereHistoricoWS($arrHistorico);
				
				if( is_array($itPerdido) && $itPerdido[0] ){
					foreach( $itPerdido as $it ){
						if( $it['sbdid'] && $it['icoid'] ){
							$sqlInsert .= " INSERT INTO execucaofinanceira.empenhopregaoitem (
										proid,
										sbdid,
										icoid,
										episituacao )
									VALUES
										(".$obProcesso->proid.",
										".$it['sbdid'].",
										".$it['icoid'].",
										'enviado'); ";
						}
					}
					$obProcesso->executar($sqlInsert);
					$obProcesso->commit();
				}
				
				return true;
			}
			
		} catch (Exception $e){
	
			# Erro 404 página not found
			if($e->getCode() == 404){
				$obProcesso->mensagem('ERRO AO TENTAR ADERIR AO PREGÃO', 'Erro-Serviço do SIGARP encontra-se temporariamente indisponível. Favor tente mais tarde.');
				
			}
			$arrHistorico = array('tipo' => 'enviarItensSigarp - Erro',
								  'proid' => $obProcesso->proid,
								  'envio' => $arqXml,
								  'retorno' => $xmlRetorno
								);
			$obProcesso->insereHistoricoWS($arrHistorico);
			$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
			$erroMSG = str_replace( "'", '"', $erroMSG );
		
			$obProcesso->mensagem('ERRO AO TENTAR ADERIR AO PREGÃO', 'Erro-WS Enviar dados para o SIGARP: '.$erroMSG);
			
		}
	}

	public function insereMonitoramentoObras($obProcesso){
		
		$arrPreid = $obProcesso->carregarColuna("SELECT distinct preid FROM execucaofinanceira.processocomposicao WHERE proid = {$obProcesso->proid} and preid is not null");
		$arrPreid = $arrPreid ? $arrPreid : array();
		
		$obObrasPar = new ObrasPar();
		
		foreach ($arrPreid as $preid) {			
			/*** Recupera dados da Pre Obra ***/
			switch ($obProcesso->tipid) {
				case OBRAS_PAR:
					$sql = $obObrasPar->sqlRecuperaDadosPreObras($preid);
				break;
				case OBRAS_PAC:
					$sql = $obObrasPar->sqlRecuperaDadosPreObras($preid);
				break;
			}
			
			$dadosPreObra = array();
			if( $sql ) $dadosPreObra = $obProcesso->carregar($sql);
						
			/*** Insere novo endereço da obra ***/
			$sql = "INSERT INTO entidade.endereco (endcep, endlog, endcom, endbai, muncod, estuf, endnum, medlatitude, medlongitude, endstatus)					
					VALUES( '".$dadosPreObra[0]['precep']."', 
						  '".$dadosPreObra[0]['prelogradouro']."', 
						  '".$dadosPreObra[0]['precomplemento']."', 
						  '".$dadosPreObra[0]['prebairro']."', 
						  '".$dadosPreObra[0]['muncod']."', 
						  '".$dadosPreObra[0]['estuf']."', 
						  '".$dadosPreObra[0]['prenumero']."',  
						  '".$dadosPreObra[0]['prelatitude']."', 
						  '".$dadosPreObra[0]['prelongitude']."', 
						  'A' ) RETURNING endid";
			
			$endid = $obProcesso->pegaUm($sql);
						
			/*** Insere a nova obra ***/
			$sql = "INSERT INTO 
					obras.obrainfraestrutura(obrdesc, entidunidade, orgid, tooid, preid, endid, obrtipoesfera, tpoid, prfid, moeid, tobraid, cloid, obrvalorprevisto) 
					VALUES('".addslashes($dadosPreObra[0]['nome_obra'])."',
							".$dadosPreObra[0]['unidade_implantadora'].",
							3,
							'".$dadosPreObra[0]['fonte']."',
							".$preid.",
							".$endid.",
							'".$dadosPreObra[0]['preesfera']."',
							'".$dadosPreObra[0]['ptoid']."',
							'".$dadosPreObra[0]['programa']."',
							'".$dadosPreObra[0]['modalidadedeensino']."',
							'".$dadosPreObra[0]['tipodeobra']."',
							'".$dadosPreObra[0]['classificacaoobra']."',
							'".$dadosPreObra[0]['valorobra']."') 
					RETURNING obrid";
			$obrid = $obProcesso->pegaUm($sql);
						
			/*** Recupera as fotos do terreno no Pré Obra ***/
			$sql = "SELECT DISTINCT arq.arqid
					FROM public.arquivo arq
						INNER JOIN obras.preobrafotos pof ON arq.arqid = pof.arqid
						INNER JOIN obras.preobra pre ON pre.preid = pof.preid
					WHERE							
						pre.preid = ".$preid."
						AND (substring(arqtipo,1,5) = 'image')";
			
			$fotosTerreno = $obProcesso->carregar($sql);
			
			if( $fotosTerreno )
			{
				/*** Insere as fotos para galeria de fotos da obra ***/
				foreach($fotosTerreno as $foto)
				{
					$sql = "INSERT INTO obras.arquivosobra(obrid, tpaid, arqid, usucpf, aqodtinclusao, aqostatus)
							VALUES(".$obrid.", 21, ".$foto['arqid'].", '".$_SESSION['usucpf']."', '".date("Y-m-d H:i:s")."', 'A')";
					$obProcesso->executar($sql);
				}
			}
						
			/*** Recupera os documentos anexos no Pré Obra ***/
			$sql = "SELECT DISTINCT arq.arqid
					FROM obras.preobraanexo p
						INNER JOIN public.arquivo arq ON arq.arqid = p.arqid
					WHERE p.preid = ".$preid;
			$anexos = $obProcesso->carregar($sql);
			
			if( $anexos )
			{
				/*** Insere os documentos nos arquivos da obra ***/
				foreach($anexos as $anexo)
				{
					$sql = "INSERT INTO obras.arquivosobra(obrid, tpaid, arqid, usucpf, aqodtinclusao, aqostatus)
							VALUES(".$obrid.", 21, ".$anexo['arqid'].", '".$_SESSION['usucpf']."', '".date("Y-m-d H:i:s")."', 'A')";
					$obProcesso->executar($sql);
				}
			}
			
			/*** Inclue o ID da nova obra na tabela do pre obra ***/
			$sql = "UPDATE obras.preobra SET obrid = ".$obrid." WHERE preid = ".$preid;
			$obProcesso->executar($sql);
		}		
	}
}
?>