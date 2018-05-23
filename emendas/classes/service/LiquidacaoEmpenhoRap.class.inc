<?php
/**
 * Modelo de Liquidação de Empenho Inscritos em RAP
 * @author Wesley Romualdo
 */
class LiquidacaoEmpenhoRap extends Modelo{
	
	/**
	 * Nome da tabela especificada
	 * @name $stNomeTabela
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "";
	
	private $arCodUfSIAF = array(
								array("estuf" => "AC", "codsiaf" => "01"),
								array("estuf" => "AL", "codsiaf" => "03"),
								array("estuf" => "AM", "codsiaf" => "05"),
								array("estuf" => "AP", "codsiaf" => "04"),
								array("estuf" => "BA", "codsiaf" => "07"),
								array("estuf" => "CE", "codsiaf" => "09"),
								array("estuf" => "DF", "codsiaf" => "11"),
								array("estuf" => "ES", "codsiaf" => "13"),
								array("estuf" => "GO", "codsiaf" => "15"),
								array("estuf" => "MA", "codsiaf" => "17"),
								array("estuf" => "MG", "codsiaf" => "23"),
								array("estuf" => "MS", "codsiaf" => "21"),
								array("estuf" => "MT", "codsiaf" => "19"),
								array("estuf" => "PA", "codsiaf" => "25"),
								array("estuf" => "PB", "codsiaf" => "27"),
								array("estuf" => "PE", "codsiaf" => "31"),
								array("estuf" => "PI", "codsiaf" => "33"),
								array("estuf" => "PR", "codsiaf" => "29"),
								array("estuf" => "RJ", "codsiaf" => "35"),
								array("estuf" => "RN", "codsiaf" => "37"),
								array("estuf" => "RO", "codsiaf" => "41"),
								array("estuf" => "RR", "codsiaf" => "42"),
								array("estuf" => "RS", "codsiaf" => "39"),
								array("estuf" => "SC", "codsiaf" => "43"),
								array("estuf" => "SE", "codsiaf" => "47"),
								array("estuf" => "SP", "codsiaf" => "45"),
								array("estuf" => "TO", "codsiaf" => "48")										
							);
							
	private $arEstadual   = array("610280", "520229"); 
	private $arMunicipal  = array("610283", "520229"); 
	private $arParticular = array("610007", "520229"); 
	private $arFederal 	  = array("610007", "520229"); 

	/**
	 * Chave primaria.
	 * @name $arChavePrimaria
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array();

	/**
	 * Atributos da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributos	= array();

	/**
     * Gera o filtro de pesquisa para listas as entidades com as suas contrapartidas mínimas
     * @name geraFiltroContrapartida
     * @author Wesley Romualdo
     * @access public
     * @param array $dados - post do formulário
     * @return array
     */
	public function enviaLiquidaEmpenhoRAP( $dados = array() ){
		$chkConvenio = $dados['chkLiquidar'];
		$usuario = $dados['usuario'];
		$senha = $dados['senha'];
		
		if(count($chkConvenio) < 1){
			echo '<script>
				alert("Escolha pelo menos um Convênio!");
				window.location = "emenda.php?modulo=principal/liquidacaoEmpenhoRap&acao=A";
			  </script>';
			die;
		} else {		
			$sql = "SELECT DISTINCT
					    exf.exfnumsolempenho,
					    exf.exfnumempenhooriginal,
					    orb.orbnumparcela,
					    ptr.ptrnumconvenio,
					    ptm.pmcnumconveniosiafi,
					    (sum(exf.exfvalor) - COALESCE(orb.orbvalorparcela, 0)) as vllancamento,
						exf.exfnaturezadespesa,
					    (CASE WHEN vede.mapcod = '90' THEN 'federal'
					          WHEN vede.mapcod = '30' THEN 'estadual'
					          WHEN vede.mapcod = '40' THEN 'municipal'
					          WHEN (vede.mapcod = '50' or vede.mapcod = '60') THEN 'particular'
					     ELSE '' END) as esferaadm,
					     enb.estuf  
					FROM
					    emenda.planotrabalho ptr
					    inner join emenda.ptemendadetalheentidade pte on pte.ptrid = ptr.ptrid
					    inner join emenda.execucaofinanceira exf on exf.ptrid = ptr.ptrid and pte.pedid = exf.pedid
					    inner join emenda.ptminutaconvenio ptm on ptm.ptrid = ptr.ptrid and ptm.pmcstatus = 'A'
					    left join emenda.ordembancaria orb on orb.exfid = exf.exfid
					    inner join emenda.v_emendadetalheentidade vede on vede.edeid = pte.edeid and vede.edestatus = 'A'
					    inner join emenda.entidadebeneficiada enb on enb.enbid = vede.entid
					WHERE
						ptr.ptrid in (".implode(",",array_keys($chkConvenio)).") 
    					--and orb.orbnumordembancaria is not null
    					and enb.enbstatus = 'A'
                        and ptr.ptrstatus = 'A'
                        and exf.exfstatus = 'A'
                        and ptr.ptrexercicio = '{$_SESSION['exercicio']}'
					GROUP BY
                    	exf.exfnumsolempenho,
					    exf.exfnumempenhooriginal,
					    orb.orbnumparcela,
					    ptr.ptrnumconvenio,
					    ptm.pmcnumconveniosiafi,
                        enb.estuf,
                        exf.exfnaturezadespesa,
                        orb.orbvalorparcela,
                        vede.mapcod";
			
			$arrDado = $this->carregar($sql);
			$arrDado = !$arrDado ? array() : $arrDado;
			//$usuario 		 = $post['usuario'];
			//$senha   		 = $post['senha'];
			$xmlRetorno = "";
					
			if($arrDado){
				foreach($arrDado as $dados){
					$estuf = $dados['estuf'];
					foreach ( $this->arCodUfSIAF as $v) {						
						if( $dados['estuf'] == $v['estuf'] ){							
							$codsiaf = $v['codsiaf'];
							break;
						}
					}
					
					if( $dados['esferaadm'] == 'federal' ){
						echo "<script>
								alert('Erro-Serviço liquidar empenho rap, não existe movimento para a esfera federal.');
								window.location.href = 'emenda.php?modulo=principal/liquidacaoEmpenhoRap&acao=A';
							 </script>";
						die;
						$arEsfera = $this->arFederal;
						$co_tipo_mov_pagamento = '';
					} else if( $dados['esferaadm'] == 'estadual' ){
						$arEsfera = $this->arEstadual;
						$co_tipo_mov_pagamento = '21';
					} else if( $dados['esferaadm'] == 'municipal' ){
						$arEsfera = $this->arMunicipal;
						$co_tipo_mov_pagamento = '25';
					} else if( $dados['esferaadm'] == 'particular' ){
						$arEsfera = $this->arParticular;
						$co_tipo_mov_pagamento = '26';
					}
					
					
					
					$nu_empenho = $dados['exfnumempenhooriginal']; //$dados['exfnumsolempenho'];
					$nu_parcela = $dados['orbnumparcela'];
					$nu_convenio_original = $dados['ptrnumconvenio'];
					$nu_convenio_siafi = $dados['pmcnumconveniosiafi'];
					$vl_lancamento = $dados['vllancamento'];
					$co_programa_fnde = '03';
					$co_observacao = '37';
					$nu_proposta_siconv = '';
					$nu_sistema = '2';
					
					$co_inscricao1 = $dados['exfnumempenhooriginal'];
					$co_inscricao2 = $dados['ptrnumconvenio'].$dados['orbnumparcela'];
					$co_classificacao1 = '3'.$dados['exfnaturezadespesa'].$codsiaf;
					$co_classificacao2 = '212110302';
					$vl_detalhe_nl = $vl_lancamento;
					
					//$arParcela = $this->carregaParcelaRAP( $dados['exfid'] );
					
					foreach ($arEsfera as $co_evento_contabil) {
						$item.= "<item>
								  <co_evento_contabil>$co_evento_contabil</co_evento_contabil> 
								  <co_inscricao1>$co_inscricao1</co_inscricao1> 
								  <co_inscricao2>$co_inscricao2</co_inscricao2> 
								  <co_classificacao1>$co_classificacao1</co_classificacao1> 
								  <co_classificacao2>$co_classificacao2</co_classificacao2> 
								  <vl_detalhe_nl>$vl_detalhe_nl</vl_detalhe_nl> 
						  		</item>";							
					}
									
			$dataAtual = date("c");
		
	
$arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
<header>
<app>string</app>
<version>string</version>
<created>{$dataAtual}</created>
</header>
	<body>
	<auth>
        <usuario>$usuario</usuario>
    	<senha>$senha</senha>
    </auth>
	<params>	    
	  <nu_empenho>$nu_empenho</nu_empenho> 
	  <nu_parcela>$nu_parcela</nu_parcela> 
	  <nu_convenio_original>$nu_convenio_original</nu_convenio_original> 
	  <nu_convenio_siafi>$nu_convenio_siafi</nu_convenio_siafi> 
	  <vl_lancamento>$vl_lancamento</vl_lancamento> 
	  <co_programa_fnde>$co_programa_fnde</co_programa_fnde> 
	  <co_observacao>$co_observacao</co_observacao>
	  <co_tipo_mov_pagamento>$co_tipo_mov_pagamento</co_tipo_mov_pagamento>
	  <nu_proposta_siconv>$nu_proposta_siconv</nu_proposta_siconv> 
	  <nu_sistema>$nu_sistema</nu_sistema> 
	  	<detalhamento>
  			$item			  		
	  	</detalhamento>
	</params>
	</body>
</request>
XML;

						if ( $_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
							//$urlWS = "http://172.20.200.116/webservices/sigef/aymar/public/index.php/financeiro/nl/solicitarNlComum";
							$urlWS = "http://172.20.65.98/webservices/sigef/aymar/public/index.php/financeiro/nl/solicitarNlComum";
						} else {
							$urlWS = "http://www.fnde.gov.br/webservices/sigef/index.php/financeiro/nl/solicitarNlComum";
						}
						
						try {
							$xsd = APPRAIZ.'emenda/modulos/sistema/ws-sigef/xsd/xml.fnde.ws-sigef_integracao.financeiro.nl.solicitarnlcomum.request.xsd';
							//$arMessage = validaXML( $arqXml, $xsd );
		
						    if( empty( $arMessage ) ){
						    	$xml = Fnde_Webservice_Client::CreateRequest()
									->setURL($urlWS)
									->setParams( array('xml' => $arqXml, 'login' => $usuario, 'senha' => $senha, 'method' => 'solicitarNlComum') )
									->execute()
								;
						    } else {
						    	echo 'Erro-WS: validação xml - Solicitar liquidar empenho rap: <br>';
						    	foreach ($arMessage as $msg) {
						    		print_r( $msg['text'] );
						    	}
						    	die;
						    }
									
								$xmlRetorno = $xml;
	
								$xml = simplexml_load_string( stripslashes($xml));
								
						        if ( (int) $xml->status->result ){
						        	$nu_seq_mov_pag = $xml->body->nu_seq_mov_pag;
						        	/**
									 * @var LogErroWS
									 * Bloco que grava o erro em nossa base
									 */
									$obLogErroWS = new LogErroWS();
									$erro = "STATUS RESULT = 1<br />".$arqXml;
									$obLogErroWS->montaTextoErro($erro);
									$erro = "XML de Envio: <br />".$arqXml;
									$obLogErroWS->montaTextoErro($erro);
									$erro = "XML de Retorno: <br />".$xmlRetorno;
									$obLogErroWS->montaTextoErro($erro);
									$obLogErroWS->ptrid 		  = $post['ptrid'];
									$obLogErroWS->exfid 		  = $exfid;
									$obLogErroWS->logtipo 		  = 'ER';
									$obLogErroWS->usucpf 		  = $_SESSION['usucpf'];
									$obLogErroWS->logdatainclusao = date('Y-m-d H:i:s');
									$obLogErroWS->logerro 		  = str_replace( "'", '"', $obLogErroWS->logerro );
									$obLogErroWS->salvar();
									$obLogErroWS->commit();
									unset($obLogErroWS);
									
									# Grava dados do pagamento na tabela ordem bancaria
									$sql = "INSERT INTO emenda.ordembancaria(exfid, orbnumparcela, orbmesparcela, orbanoparcela, orbvalorparcela, orbnumsolicitacao, spgcodigo, usucpf, orbdatainclusao) 
											VALUES ( $exfid, '$parcela', '$mesparcela', $anoparcela, $vparcela, '$nu_seq_mov_pag', '0', '{$_SESSION['usucpf']}', 'now()') RETURNING orbid";
					
									$orbid = $this->pegaUm( $sql );
									
									# Grava o Historico
									$obHistOrdemBanc = new Ordembanchistorico();
									$obHistOrdemBanc->orbid = $orbid;
									$obHistOrdemBanc->spgcodigo = '0';
									$obHistOrdemBanc->orbvalorpagamento = $valor;
									$obHistOrdemBanc->usucpf = $_SESSION['usucpf'];
									$obHistOrdemBanc->obhdataalteracao = date('Y-m-d H:i:s');
									$obHistOrdemBanc->salvar();
									
									$boSucesso = $this->commit();
								} else {
									$erroMSG = str_replace(array(chr(13),chr(10)), ' ',utf8_decode($xml->status->error->message->text));
							    	$erroMSG = str_replace( "'", '"', $erroMSG );
							    							
									echo "<script>
											alert('Erro-WS liquidar empenho rap no SIGEF: $erroMSG');
										 </script>";
									
								 	throw new Exception(utf8_decode($xml->status->error->message->text));
								}
				    		$count++;
						} catch (Exception $e){
							/**
							 * @var LogErroWS
							 * Bloco que grava o erro em nossa base
							 */
							$obLogErroWS = new LogErroWS();
							$erro = "XML de Envio: <br />".$arqXml;
							$obLogErroWS->montaTextoErro($erro);
							$erro = "XML de Retorno: <br />".$xmlRetorno;
							$obLogErroWS->montaTextoErro($erro);
							$erro = "Objeto Exception: <br />".$e;
							$obLogErroWS->montaTextoErro($erro);
							
							$obLogErroWS->ptrid 		  = $post['ptrid'];
							$obLogErroWS->exfid 		  = $exfid;
							$obLogErroWS->logtipo 		  = 'ER';
							$obLogErroWS->usucpf 		  = $_SESSION['usucpf'];
							$obLogErroWS->logdatainclusao = date('Y-m-d H:i:s');
							$obLogErroWS->logerro 		  = str_replace( "'", '"', $obLogErroWS->logerro );
							$obLogErroWS->salvar();						
							$obLogErroWS->commit();
							unset($obLogErroWS);
	
							# Erro 404 página not found
							if($e->getCode() == 404){
								echo "<script>
										alert('Erro-Serviço liquidar empenho rap encontra-se temporariamente indisponível. Favor tente mais tarde.');
										window.location.href = 'emenda.php?modulo=principal/liquidacaoEmpenhoRap&acao=A';
									 </script>";
								die;
							}
							$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
							$erroMSG = str_replace( "'", '"', $erroMSG );
							
							echo "<script>
									alert('Erro-WS liquidar empenho rap: $erroMSG');
									window.location.href = 'emenda.php?modulo=principal/liquidacaoEmpenhoRap&acao=A';
								 </script>";
							die;
						}
				} // Fim do foreach
				if($boSucesso){
					echo "<script>
							alert('liquidar empenho rap solicitado com sucesso.');
							window.location.href = 'emenda.php?modulo=principal/liquidacaoEmpenhoRap&acao=A';
						 </script>";
					die;
				} else {
					echo "<script>
							alert('Solicitação de liquidar empenho rap não efetuado com sucesso.');
							window.location.href = 'emenda.php?modulo=principal/liquidacaoEmpenhoRap&acao=A';
						 </script>";
					die;			
				}
			} else {// Fim do if Array de Dados
				echo "<script>
						alert('Erro-Não há dados para liquidar empenho rap.');
						window.location.href = 'emenda.php?modulo=principal/liquidacaoEmpenhoRap&acao=A';
					 </script>";
				die;	
			}
		}
	}
	
	public function carregaParcelaRAP( $exfid ){
		$sql = "";
	}
}
?>